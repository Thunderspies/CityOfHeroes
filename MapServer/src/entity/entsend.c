#define _USE_MATH_DEFINES
#include <math.h>
#include "entity/character_workshop.h"
#include "entity/Invention.h"
#include <string.h>
#include "entserver.h"
#include "entity/entPlayer.h"
#include "entity/entityRef.h"
#include "entity/motion.h"
#include "cmdparse/cmdcommon.h"
#include "cmdparse/cmdcontrols.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/net_packetutil.h>
#include <utilitieslib/network/net_packet.h>
#include "comm_game.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/network/netio.h>
#include "cmdparse/cmdserver.h"
#include "entity/entVarUpdate.h"
#include "entsend.h"
#include "combat/combat.h"
#include <utilitieslib/assert/assert.h>
#include "svr/svr_base.h"
#include "player/clientEntityLink.h"
#include "entity/entStrings.h"
#include "ai/entai.h"
#include "group/groupdynsend.h"
#include "entity/PowerInfo.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include "character_net_server.h"
#include "entity/origins.h"
#include "entity/attrib_net.h"
#include "cmdparse/cmdcommon.h"
#include "container/team.h"
#include "container/league.h"
#include "friends.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/StringCache.h>    // for stringToReference
#include "storyarc/storyarcinterface.h"
#include "svr/svr_base.h"
#include "svr/svr_player.h"
#include "gameComm/VillainDef.h"
#include <utilitieslib/components/earray.h>
#include "entity/costume.h"
#include "language/langServerUtil.h"
#include "dbcomm/dbcomm.h"
#include "storyarc/SouvenirClue.h"
#include "gameComm/sendToClient.h"
#include "gridcoll/megaGrid.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "entity/costume.h"
#include "player/badges.h"
#include "player/badges_server.h"
#include "gameComm/Npc.h"
#include "entity/netfx.h"
#include "gameSys/arenamapserver.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "gameSys/arenamap.h"
#include "gameSys/cutScene.h"
#include <utilitieslib/utils/mathutil.h>
#include "utils/position.h"
#include "Reward.h"
#include "player/inventory_server.h"
#include "script/ZoneEvents/ScriptUI.h"
#include "entity/pvp.h"
#include "dbcomm/dbnamecache.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/ragdoll.h"
#include "entity/character_net.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "SgrpServer.h"
#include "storyarc/storySend.h"
#include "storyarc/contactdef.h"
#include "storyarc/storyarcprivate.h"
#include "bases/basedata.h"
#include "bases/DetailRecipe.h"
#include <utilitieslib/utils/Quat.h>
#include "storyarc/taskforce.h"
#include "seq/seqstate.h"
#include "entity/attrib_description.h"
#include "dbcomm/staticMapInfo.h"
#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "entity/power_customization.h"
#include "character_animfx.h"
#include "storyarc/pnpcCommon.h"
#include "auth/authUserData.h"
#include "gameSys/Turnstile.h"
#include "entity/character_target.h"

U32        need_to_send_cache[(MAX_ENTITIES_PRIVATE*2+31)/32];    // 2-bitfield to track which server ents need to be sent this tick
// bit 0 = has it been cacled, bit 1 = the value

#define MAX_MSG_TARGETS ( 32 )
#define MAX_CLIENT_STR 256
#define MAX_CONSOLE_STR 10000
#define MAX_ENTITIES_SENT_TO_A_CLIENT 100

//max time to delay animations and fx to compensate for the movement delay.
#define MAX_PACKETDELAY_COMPENSATION 15

InterpState interp_state;

static void setNetFxParams(Entity *e);

MP_DEFINE(EntNetLink);
void *createEntNetLink()
{
    MP_CREATE(EntNetLink,16);
    return MP_ALLOC(EntNetLink);
}

void destroyEntNetLink(EntNetLink *data)
{
    MP_FREE(EntNetLink, data);
}

/* ########## Handle Fx Create, Destroy, and Update Commands ####################### */

//NETFX
//To add an fx to an entShannon and others fill out the NetFx struct, and call this
U32 entAddFx(Entity *e,NetFx *netfx)
{
    static int net_id = 0; //net_id should never be zero, negative numbers are client-only

    NetFx * newNetFx;

    netfx->timeCalled = ((F32)ABS_TIME/100.0);

    //If you already have a maintained version of this fx on this entity,
    //you can't have another, you can only extend it's serverTimeout
    //SERVERTIMEOUT has been unused long enough to be possibly unreliable
    if( netfx->command == CREATE_MAINTAINED_FX )
    {
        NetFx * nfxInList;
        int i;

        for( i = 0 ; i < e->netfx_count ; i++ )
        {
            nfxInList = &e->netfx_list[i];

            if( netfx->handle == nfxInList->handle  )
            {
                nfxInList->serverTimeout    = netfx->serverTimeout;
                nfxInList->timeCalled        = netfx->timeCalled;
                return nfxInList->net_id;
            }
        }
    }

    assert(netfx->command);

    //Otherwise Add this fx to the netfx list to be shipped off on the next network update

    netfx->commandStatus = NEW_FX_COMMAND;
    ++net_id;
    if (net_id <= 0)
        net_id = 1;
    netfx->net_id = net_id;

    newNetFx = dynArrayAddStruct(&e->netfx_list,&e->netfx_count,&e->netfx_max);

    *newNetFx = *netfx; //copy structure into list

    return net_id;
}

// just utility to get a default fx
int entAddFxByName(Entity * e, const char * name, U32 command)
{
    NetFx netfx = {0};

    netfx.command         = command;
    netfx.handle          = stringToReference(name);

    netfx.radius          = 10;
    netfx.power           = 10;

    return entAddFx(e, &netfx);
}

/*go through e->netfx_list (maintained fx and newly requested fx)
delete the given fx by string reference or netid (slightly odd two-in-one function)
replace it with a command to destroy this fx.
*/
#define DESTROY_FX_BY_NETID 0
#define DESTROY_FX_BY_NAME  1
static int entDelFx(Entity *e, U32 id, int id_type )
{
    NetFx   *netfx;
    int     i, success = 0;
    int     net_id;
    
    if(id == 0 || e == 0)
    {
        Errorf("Called DelFx with net id or name hash of 0\n");
        return 0;
    }

    //Look through maintained fx on this ent
    for( i=0; i<e->netfx_count; i++ )
    {
        netfx = &e->netfx_list[i];
        if ( (id_type == DESTROY_FX_BY_NETID && netfx->net_id == id) ||
             (id_type == DESTROY_FX_BY_NAME  && netfx->handle == id) )
        {
            if (netfx->command == CREATE_MAINTAINED_FX)
            {
                if( netfx->commandStatus != OLD_FX_COMMAND &&
                    e->netfx_effective == -1)
                {
                    //If it hasn't been sent out yet, just remove it from the list.
                    
                    CopyStructsFromOffset( e->netfx_list + i, 1, e->netfx_count - i - 1);
                    e->netfx_count--;
                    i--;
                }
                else
                {
                    // Otherwise turn it into a kill self command.
                    
                    net_id = netfx->net_id;
                    ZeroStruct(netfx);
                    netfx->net_id = net_id;

                    netfx->command = DESTROY_FX;
                    
                    // If the command is queued to send on this update and netfx_effective was
                    // already calculated, then queue it to send on the next update and decrement
                    // netfx_effective.
                    
                    if(    netfx->commandStatus == NEW_FX_COMMAND_TO_SEND &&
                        e->netfx_effective >= 0)
                    {
                        // If it's greater than or equal to zero then it better be greater than zero.
                        // Or else it somehow was set to NEW_FX_COMMAND_TO_SEND without being 
                        // considered in netfx_effective, and that's a bug.
                        
                        assert(e->netfx_effective > 0);
                        
                        e->netfx_effective--;
                    }
                    
                    netfx->commandStatus = NEW_FX_COMMAND;
                }

                success = 1;
                break;
            }
            else
            {
                Errorf("Fx used as both ContinuingFX and One-shot FX (%s)\n", stringFromReference(id));
            }
        }
    }

    //You didn't find this fx to destroy.  This happens a huge amount.  A quirk in Shannon's code, I don't
    //know why.  Maybe figure it out someday.
    //if(!success)
        //printf("(Failure)" );

    return success;
}

int entDelFxByNetId( Entity *e, U32 id )
{
    return entDelFx(e, id, DESTROY_FX_BY_NETID );
}

int entDelFxByName(Entity * e, const char * name)
{
    int id;
    id = stringToReference( name );

    //printf( "entDelFxByName %s\n", name );
    if(!id && !server_state.nofx)
    {
        Errorf("FX %s doesn't exist in table.\n", name);
        return 0;
    }

    return entDelFx(e, id, DESTROY_FX_BY_NAME );
}

// Manage the sending of Net Fx

static void updateCustomFX(Entity* e)
{
    int i;
    PowerListItem* ppowListItem;
    PowerListIter pIter;
    AttribMod* mod;
    AttribModListIter aIter;
    EntityRef eRef = erGetRef(e);

    if (!e || !e->pchar)
        return;

    for (mod = modlist_GetFirstMod(&aIter, &e->pchar->listMod);
        mod != NULL;
        mod = modlist_GetNextMod(&aIter))
    {
        PowerCustomization* cust;
        const BasePower* power = mod->ptemplate->ppowBase;

        if(mod->parentPowerInstance)
            power = mod->parentPowerInstance->ppowOriginal;

        if (mod->erSource != eRef)
            continue;

        cust = powerCust_FindPowerCustomization(powerCustList_current(e), power);

        if (cust && !isNullOrNone(cust->token))
        {
            int j;

            mod->customFXToken = cust->token;

            for (j = 0; j < eaSize(&power->customfx); ++j)
            {
                if (stricmp(cust->token, power->customfx[j]->pchToken) == 0)
                    mod->fx = &power->customfx[j]->fx;
            }

            mod->uiTint = cust->customTint;
        }
        else
        {
            mod->customFXToken = NULL;
            mod->fx = &power->fx;
            mod->uiTint = power->fx.defaultTint;
        }
    }

    // Iterate backward. entDelFx may compact the array.
    for(i=e->netfx_count - 1; i >= 0; --i)
        entDelFx(e, e->netfx_list[i].net_id, DESTROY_FX_BY_NETID );

    for (ppowListItem = powlist_GetFirst(&e->pchar->listTogglePowers, &pIter);
        ppowListItem != NULL;
        ppowListItem = powlist_GetNext(&pIter))
    {
        character_EnterStance(e->pchar, ppowListItem->ppow, false);
    }
}

/*One Shot Fx and Destroy Fx commands don't need to be remembered; remove them from the
array and collapse the array after they are sent. A CREATE_MAINTAINED_FX command needs to be
kept around for folks just arriving on the scene, and is marked commandStatus = OLD_FX_COMMAND so we know
it's been sent to everyone who was on hand at its creation*/
static void entUpdateNetFxStatusAfterNetSend(Entity *e)
{
    int     i;
    NetFx * netfx;
    F32 timeSinceLastUpdate;
    F32 time;
    F32 timeElapsed;

    //Figure out time since last call so we know how much to decrement the timers
    //(this function is only called after a net Send
    time = ((F32)ABS_TIME/100.0);
    timeSinceLastUpdate = time - e->lastTimeEntSent;

    //Check and manage each outstanding netfx
    //assert( e->netfx_count <= MAX_NETFX );


    for(i=0;i<e->netfx_count;i++)
    {
        netfx = &e->netfx_list[i];

        switch(netfx->commandStatus){
            xcase NEW_FX_COMMAND:
            {
                // Force this to get updated on the next send.
                
                netfx->commandStatus = NEW_FX_COMMAND_TO_SEND;
            }

            xcase NEW_FX_COMMAND_TO_SEND:
            {
                //MAINTAINED FX, need their clientTimers and serverTimeout's decreased
                if ( netfx->command == CREATE_MAINTAINED_FX )
                {
                    netfx->commandStatus = OLD_FX_COMMAND; //usually redundant

                    //The last time this was updated was either when it was created, or the last time this function was called
                    timeElapsed = MIN(timeSinceLastUpdate, time - netfx->timeCalled);

                    //If this fx has a client timer, decrement it on the server so folks entering the
                    //scene late and getting this fx get the right timer.
                    netfx->clientTimer = MAX(netfx->clientTimer - timeElapsed, 0);

                    //If this fx was given a timeout, decrement that.
                    //Currently no one does this, and hasn't in so long, I'm not %100 it still works right
                    if( netfx->serverTimeout )
                    {
                        //never call entCreate and entDel on the same frame
                        netfx->serverTimeout -= timeElapsed;
                        if( netfx->serverTimeout <= 0.0 )
                        {
                            entDelFx(e, netfx->net_id, DESTROY_FX );
                        }
                    }
                }
                else 
                {
                    //ONESHOT FX and DESTROY FX commands get removed from the queue,
                    //(every one who will ever get this fx has gotten it)
                    
                    CopyStructsFromOffset( e->netfx_list + i, 1, e->netfx_count - i - 1 );
                    e->netfx_count--;
                    i--;
                }
            }
        }
    }

    //Debug
    if( server_state.netfxdebug )
    {
        for(i=0;i<e->netfx_count;i++)
        {
            netfx = &e->netfx_list[i];
            if(netfx->commandStatus == OLD_FX_COMMAND)
            {
                assert(netfx->command == CREATE_MAINTAINED_FX);
            }
            else
            {
                assert(netfx->commandStatus == NEW_FX_COMMAND_TO_SEND);
            }            
        }
    }
    //if( server_state.netfxdebug && e->owner == entSendGetInterpStoreEntityId() )
    //    printf("Net Send Finished, cleaning up. Holding %d maintained fx\n", e->netfx_count);
    //End debug
}

static int s_ignoreDistancePrioritization(Entity *e)
{
    return (e && e->villainDef && villainAE_isAVorHigher(e->villainDef->rank));
}
char commandStrings[8][100];

void debugShowNetfx( Entity * e, NetFx * netfx )
{
    char name[2000];
    const char * shortName = 0;
    strcpy( commandStrings[CREATE_ONESHOT_FX],        "ONESHOT   " );
    strcpy( commandStrings[CREATE_MAINTAINED_FX],    "MAINTAINED" );
    strcpy( commandStrings[DESTROY_FX],                "DESTROY   " );

    //Crop Fx name for convenience
    shortName = stringFromReference( netfx->handle );
    if( shortName)
    {
        strcpy( name, shortName );
        if(name[0])
        {
            shortName = strrchr( name, '/' );
            shortName++;
        }
    }

    //Print Info about this netfx
    printf("       %s %s (ID %d) (Triggers %3.1f/%.2d) \n",
        commandStrings[netfx->command], shortName, netfx->net_id, netfx->clientTimer, netfx->clientTriggerFx );
}

/*Send down netfx commands.  Those CREATE_MAINTAINED_FX commands that are just hanging
around in the queue (commandStatus == OLD_FX_COMMAND) are only sent if force_send (sender and receiver
just met, usually)  */
static int entSendNetFx( Packet *pak, Entity *sender, int force_send, Entity * receiving_ent /*debug only*/ )
{
    int     i, fxSentCount = 0;
    NetFx   *netfx;
    F32        packetDelay;
    int        actualSentCount = 0;

    //## How many netfx are are being sent? (Leave out old fx unless this is a full update )
    fxSentCount = sender->netfx_count;
    if (!force_send)
    {
        //Fpr perf, the first guy to get sender this tick sets netfx_effective,
        //(netfx_effective get reset to -1 at the end of the tick)
        if (sender->netfx_effective==-1)
            setNetFxParams(sender);
        fxSentCount = sender->netfx_effective;
    }
    else
    {
        for(i = 0; i < sender->netfx_count; i++)
        {
            if(sender->netfx_list[i].commandStatus == NEW_FX_COMMAND)
            {
                // Don't EVER send NEW_FX_COMMANDs.
                
                fxSentCount--;
            }
        }
    }
    pktSendBitsPack(pak,1,fxSentCount);

    //Debug
    if( fxSentCount && server_state.netfxdebug && receiving_ent->owner == entSendGetInterpStoreEntityId() )
        printf("    #### sending %d fx from %s\n", fxSentCount, sender->seq->type->name);
    //End debug

    //## Send them
    for(i=0;i<sender->netfx_count;i++)
    {
        netfx = &sender->netfx_list[i];

        assert( netfx->command && netfx->net_id );

        if(    netfx->commandStatus == NEW_FX_COMMAND ||
            !force_send && netfx->commandStatus != NEW_FX_COMMAND_TO_SEND)
        {
            // We never want to send fx that haven't been counted in netfx_effective,
            // and if this isn't a full send then we only want to send new fx.
            continue;
        }

        actualSentCount++;
        
        //Debug
        if( server_state.netfxdebug && receiving_ent->owner == entSendGetInterpStoreEntityId() )
            debugShowNetfx( sender, netfx );
        //End debug

        if( !netfx->clientTriggerFx ) //don't do this for Fx Triggered stuff.
            packetDelay = MINMAX( netfx->timeCalled - sender->lastTimeEntSent, 0, MAX_PACKETDELAY_COMPENSATION );
        else
            packetDelay = 0;

        assert(!(netfx->command&~0x3));
        assert(netfx->command);
        pktSendBits(pak,2,netfx->command);
        pktSendBits(pak,32, netfx->net_id);
        pktSendBits(pak,1, netfx->pitchToTarget);//move this out of fx someday...
        pktSendIfSetBits(pak,4, netfx->powerTypeflags);
        pktSendIfSetBitsPack(pak,12,netfx->handle);
        pktSendBits(pak,1, netfx->hasColorTint);
        if (netfx->hasColorTint) {
            pktSendBits(pak, 32, netfx->colorTint.primary.integer);
            pktSendBits(pak, 32, netfx->colorTint.secondary.integer);
        }
        pktSendIfSetBitsPack(pak,4,(int)(netfx->clientTimer + packetDelay) );
        pktSendIfSetBits(pak,32, netfx->clientTriggerFx);
        pktSendIfSetF32( pak, netfx->duration );
        pktSendIfSetF32( pak, netfx->radius );
        pktSendIfSetBits( pak, 4, netfx->power ); //sends low 4 bits, so I'm OK
        pktSendIfSetBits( pak, 32, netfx->debris );

        //Send the Origin
        pktSendIfSetBits( pak, 2, netfx->originType ); // JE: This should really be changed to a pktSendBits(pak, 1), since there are only 2 values!
        if( netfx->originType == NETFX_POSITION )
        {
            pktSendF32( pak, netfx->originPos[0] );
            pktSendF32( pak, netfx->originPos[1] );
            pktSendF32( pak, netfx->originPos[2] );
        }
        else if( netfx->originType == NETFX_ENTITY )
        {
            pktSendIfSetBitsPack(pak,8,netfx->originEnt);
            pktSendBitsPack(pak,2,netfx->bone_id);
        }
        else assertmsg( 0, "Bad Nfx" );

        //Send the Target
        pktSendIfSetBits( pak, 2, netfx->targetType );// JE: This should really be changed to a pktSendBits(pak, 1), since there are only 2 values!
        if( netfx->targetType == NETFX_POSITION )
        {
            pktSendF32( pak, netfx->targetPos[0] );
            pktSendF32( pak, netfx->targetPos[1] );
            pktSendF32( pak, netfx->targetPos[2] );
            pktSendIfSetBitsPack(pak,12,netfx->targetEnt);
        }
        else if( netfx->targetType == NETFX_ENTITY )
        {
            pktSendIfSetBitsPack(pak,12,netfx->targetEnt);
        }
        else assertmsg( 0, "Bad Nfx" );
    }
    
    assert(fxSentCount == actualSentCount);

    return fxSentCount ? 1 : 0;
}
/*############ End Handle Net Fx Sent from Game code #############33*/


/*Ask sequencer to find a move based on the state given.  Then and tell the client to do that
move when the trigger condition is met.  Synchs up attacks and reactions.  Currently the only
trigger conditions are time and when hit by the fx given
*/
int entSetTriggeredMove(Entity *e, U32 * added_state, int ticksToDelay, int triggerFxNetId )
{
    TriggeredMove * tm;

    if(!e || e->triggered_move_count >= MAX_TRIGGEREDMOVES || !added_state || ticksToDelay < 0 )
        return 0;

    e->set_triggered_moves = 1; //dear sequencer: there's something here for you

    tm                    = &(e->triggered_moves[e->triggered_move_count++]);
    tm->ticksToDelay    = ticksToDelay;
    tm->triggerFxNetId  = triggerFxNetId;
    tm->timeCreated        = ((F32)ABS_TIME/100.0);
    memcpy(tm->state, added_state, sizeof(tm->state) );

    //The rest of this is done in entUpdate/entCheckForTriggeredMove so the seq->state is sure to be up to date
    return 1;
}

static float offset_table[131];
static int offset_table_init = 0;

static void initOffsetTable()
{
    void entGetNetInterpOffsets(float* array, int size);

    entGetNetInterpOffsets(offset_table, ARRAY_SIZE(offset_table));

    offset_table_init = 1;
}

float decodeOffset(char char_offset, int max_bits, int cur_bits)
{
    int abs_offset;

    if(!char_offset)
        return 0;

    if(!offset_table_init)
    {
        initOffsetTable();
    }

    abs_offset = char_offset & 0x7f;

    return offset_table[abs_offset + 1] * (char_offset < 0 ? -1 : 1) * (1 << (max_bits - cur_bits));
}

static void encodeOffsets(Vec3 offset_vec, char* char_offsets, Vec3 guess_offsets, int max_bits)
{
    int i;
    int max_val = ((1 << (max_bits - 1)) - 1);

    F32 multiplier = 1.0 / (1 << (8 - max_bits));

    for(i = 0; i < 3; i++)
    {
        float d = fabs(offset_vec[i]) * multiplier;
        int l = 2;
        int h = l + max_val + 1;
        int fib_index;

        if(d >= offset_table[2] * 0.5)
        {
            while(l + 1 < h)
            {
                int index = (h + l) / 2;
                float value = offset_table[index];

                if(value > d)
                {
                    h = index;
                }
                else if(value < d)
                {
                    l = index;
                }
                else
                {
                    h = l = index;
                    break;
                }
            }

            if(h != l)
            {
                if(fabs(d - offset_table[l]) < fabs(d - offset_table[h]))
                    fib_index = l;
                else
                    fib_index = h;
            }
            else
            {
                fib_index = h;
            }

            char_offsets[i] = fib_index - 1;
            guess_offsets[i] = offset_table[fib_index] / multiplier;

            if(offset_vec[i] < 0)
            {
                char_offsets[i] |= 0x80;
                guess_offsets[i] *= -1;
            }
        }
        else
        {
            //printf("yep, it's zero (%d bits, %f)!\n", max_bits, d);
            char_offsets[i] = 0;
            guess_offsets[i] = 0;
        }
    }
}


void entSendSetInterpStoreEntityId(int id)
{
    interp_state.store_entity_id = id;
}

int entSendGetInterpStoreEntityId()
{
    return interp_state.store_entity_id;
}

// This array translates from the binary-tree-position index to the actual position-array index.
// VM - extended array to avoid a crash in entSetGenericInterpPositions.  Maybe.

static const int binpos_to_pos[9] = { 3, 1, 5, 0, 2, 4, 6, 7, 8 };

// This array says which position-array indexes are the parent positions of the binary-tree-positions.
// Indexes 0-6 are the middle positions because the math is nicer, so you get this beautifulness:
//
//      7 = source position.
//      8 = target position.

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

static const int binpos_bits[7] = { 8, 7, 7, 6, 6, 6, 6 };
static const int binpos_level[7] = { 1, 2, 2, 3, 3, 3, 3 };
static const int binpos_parent[7] = { 0, 0, 0, 1, 1, 2, 2 };

static void setInterpData(Entity* e, Vec3 old_net_pos, Vec3 new_net_pos)
{
    Vec3* real_pos_hist = e->pos_history.pos;
    Vec3 pos_hist[ENT_POS_HISTORY_SIZE];
    int base = e->pos_history.net_begin + ENT_POS_HISTORY_SIZE;
    int steps = e->pos_history.end + 1 - e->pos_history.net_begin;
    int i;
    Vec3 positions[7];
    Vec3 client_guess_positions[9];
    float total_time;
    int cur_time_step;
    InterpPosition* ip = e->interp_positions;

    // And now the code of love.

    if(steps < 0)
        steps += ENT_POS_HISTORY_SIZE;

    assert(steps >= 0);

    if(steps <= 2)
    {
        // Only two steps since last time, so ignore it.

        ip[0].y_used = 0;
        ip[0].xz_used = 0;
        return;
    }

    if(steps > 16)
        steps = 16;

    cur_time_step = steps - 2;
    total_time = global_state.time_since[cur_time_step];

    // Interp the real_pos_hist between the offsets at the first and last positions (to smooth the curve).
    {
        int j;
        int first_pos = base % ENT_POS_HISTORY_SIZE;
        int last_pos = (base + steps - 1) % ENT_POS_HISTORY_SIZE;
        Vec3 old_diff;
        Vec3 new_diff;
        Vec3 total_diff;

        subVec3(old_net_pos, real_pos_hist[first_pos], old_diff);
        subVec3(new_net_pos, real_pos_hist[last_pos], new_diff);
        subVec3(new_diff, old_diff, total_diff);

        for(i = first_pos, j = 0;; i = (i + 1) % ENT_POS_HISTORY_SIZE, j++)
        {
            Vec3 diff;
            float scale = (float)j / (steps - 1);

            scaleVec3(total_diff, scale, diff);
            addVec3(diff, old_diff, diff);

            addVec3(real_pos_hist[i], diff, pos_hist[i]);

            //copyVec3(real_pos_hist[i], pos_hist[i]);

            if(i == last_pos)
                break;

            //addVec3(real_pos_hist[i], diff, real_pos_hist[i]);
        }

        //if(0)
        //{
        //    printf("real positions:\t\t");
        //    for(i = first_pos; i != last_pos; i = (i + 1) % ENT_POS_HISTORY_SIZE)
        //    {
        //        j = (i + 1) % ENT_POS_HISTORY_SIZE;
        //        printf("%1.2f(%1.2f), ", distance3(real_pos_hist[i], real_pos_hist[j]), j != last_pos ? global_state.time_since[steps - 3 - ((i + ENT_POS_HISTORY_SIZE) - first_pos) % ENT_POS_HISTORY_SIZE] : 0);
        //    }
        //    printf("\n");
        //}
    }

    // Calculate 7 positions that are evenly spaced in time between the first and last positions.

    for(i = 0; i < 7; i++)
    {
        F32        cur_time = (i + 1) * total_time / 8.0;
        Vec3    pos1, pos2;
        F32        time_diff;
        F32        factor;

        while(cur_time_step >= 0 && total_time - global_state.time_since[cur_time_step] < cur_time)
            cur_time_step--;

        if(cur_time_step >= 0)
        {
            // Somewhere before previous frame.

            time_diff = global_state.time_since[cur_time_step + 1] - global_state.time_since[cur_time_step];
            factor = (cur_time - (total_time - global_state.time_since[cur_time_step + 1])) / time_diff;

            //assert(factor >= 0.0 && factor <= 1.0);

            scaleVec3(pos_hist[(base + steps - (cur_time_step + 2)) % ENT_POS_HISTORY_SIZE], factor, pos1);
            factor = 1.0 - factor;
            scaleVec3(pos_hist[(base + steps - (cur_time_step + 3)) % ENT_POS_HISTORY_SIZE], factor, pos2);

            addVec3(pos1, pos2, positions[i]);
        }
        else
        {
            // Time is since last frame.

            time_diff = global_state.time_since[0];
            factor = (cur_time - (total_time - time_diff)) / time_diff;

            //assert(factor >= 0.0 && factor <= 1.0);

            scaleVec3(pos_hist[(base + steps - 1) % ENT_POS_HISTORY_SIZE], factor, pos1);
            factor = 1.0 - factor;
            scaleVec3(pos_hist[(base + steps - 2) % ENT_POS_HISTORY_SIZE], factor, pos2);

            addVec3(pos1, pos2, positions[i]);
        }
    }

    //if(0)
    //{
    //    printf("interp positions:\t");
    //    for(i = 0; i < 6; i++)
    //    {
    //        printf("%1.2f, ", distance3(positions[i], positions[i+1]));
    //    }
    //    printf("\n");
    //}

    if(interp_state.store_entity_id && interp_state.store_entity_id == e->owner)
    {
        CopyArray(interp_state.interp_positions, positions);
        copyVec3(old_net_pos, interp_state.last_pos);
    }

    //if(e->owner > 1 && e->type == ENTTYPE_PLAYER)
    //{
    //  printf("diff: %1.2f\t%1.2f\t%1.2f\n", diff_vec[0], diff_vec[1], diff_vec[2]);
    //  printf("%d net_pos: %1.2f\t%1.2f\t%1.2f\n", e->owner, e->net_pos[0], e->net_pos[1], e->net_pos[2]);
    //}

    // Positions 7 and 8 are the left and right respectively, it gets uglier and uglier.

    copyVec3(old_net_pos, client_guess_positions[7]);
    copyVec3(new_net_pos, client_guess_positions[8]);

    // Set these because then I don't have to check if i==0 when looking for parent state.

    ip[0].y_used = 1;
    ip[0].xz_used = 1;

    #define PRINT_OFFSETS 0

    if(!offset_table_init)
    {
        initOffsetTable();
    }

    for(i = 0; i < 7; i++)
    {
        Vec3 guess_pos;
        Vec3 offset_vec;
        Vec3 guess_offsets;

        if(!ip[binpos_parent[i]].y_used && !ip[binpos_parent[i]].xz_used){
            ip[i].y_used = 0;
            ip[i].xz_used = 0;
            continue;
        }

        // Calculate what the client will think the position is, the middle of the parent positions.

        copyVec3(client_guess_positions[binpos_to_parentbinpos[i][0]], guess_pos);
        addVec3(client_guess_positions[binpos_to_parentbinpos[i][1]], guess_pos, guess_pos);
        scaleVec3(guess_pos, 0.5, guess_pos);

        // Get the offset from what it actually is.

        subVec3(positions[binpos_to_pos[i]], guess_pos, offset_vec);

        // Get the offsets.

        encodeOffsets(offset_vec, ip[i].offsets, guess_offsets, binpos_bits[i]);

        copyVec3(guess_pos, client_guess_positions[i]);

        // Set the y_used state.

        if(ip[binpos_parent[i]].y_used && ip[i].offsets[1])
        {
            ip[i].y_used = 1;

            client_guess_positions[i][1] += guess_offsets[1];

            #if PRINT_OFFSETS
                if(i < 3)
                {
                    printf(    "%d: adding to Y: %f\t(%d, %d)\n",
                            i,
                            guess_offsets[1],
                            binpos_bits[i],
                            ip[i].offsets[1]);
                }
            #endif
        }
        else
        {
            ip[i].y_used = 0;
        }

        // Set the xz_used state.

        if(ip[binpos_parent[i]].xz_used && (ip[i].offsets[0] || ip[i].offsets[2]))
        {
            ip[i].xz_used = 1;

            client_guess_positions[i][0] += guess_offsets[0];
            client_guess_positions[i][2] += guess_offsets[2];

            #if PRINT_OFFSETS
                if(i < 3)
                {
                    printf(    "%d: adding to X: %f\t(%d, %d)\n",
                            i,
                            guess_offsets[0],
                            binpos_bits[i],
                            ip[i].offsets[0]);

                    printf(    "%d: adding to Z: %f\t(%d, %d)\n",
                            i,
                            guess_offsets[2],
                            binpos_bits[i],
                            ip[i].offsets[2]);
                }
            #endif
        }
        else
        {
            ip[i].xz_used = 0;
        }
    }

    if(interp_state.store_entity_id && interp_state.store_entity_id == e->owner)
    {
        // This memcpy is intentionally copying only 7 array elements, not 9.
        
        memcpy(interp_state.guess_pos, client_guess_positions, sizeof(interp_state.guess_pos));

        for(i = 0; i < 7; i++){
            if(!ip[i].y_used && !ip[i].xz_used)
                zeroVec3(interp_state.guess_pos[i]);
        }
    }
}

static void setNetPosition(Entity *e, int e_idx)
{
    int     update_pos,j,k;
    F32        val,maxval;
    int     ipos[3],send_sub,shift,ent_create=0;
    int        t;
    Vec3    new_net_pos;
    int        low_bits_mask;
    int        low_bits_max;
    int        low_bits_half_max;
    int        scale_pos[3];

    send_sub = 1;
    update_pos  = 0;

    shift = 0;
    if(!ent_create)
    {
        // Find the largest required shift.

        for(j=0;j<3;j++)
        {
            val = fabs(ENTPOS(e)[j] - e->net_pos[j]);
            maxval = (256.f/POS_SCALE) / 8.f;
            for(k=0;k<8;k++)
            {
                if (val < maxval)
                {
                    if (k > shift)
                        shift = k;
                    break;
                }
                maxval *= 2;
            }
        }

        if (shift > 6)
            send_sub = 0;
    }

    if(shift)
    {
        low_bits_max = 1 << shift;
        low_bits_half_max = low_bits_max >> 1;
        low_bits_mask = low_bits_max - 1;
    }

    #define GET_SCALE_POS(x) (0.5 + x * POS_SCALE + (1 << (POS_BITS-1)))

    for(j=0;j<3;j++)
    {
        t = scale_pos[j] = GET_SCALE_POS(ENTPOS(e)[j]);

        if(shift)
        {
            int low_bits = e->net_ipos[j] & low_bits_mask;

            t -= low_bits;

            t = ((F32)t + low_bits_half_max) / low_bits_max;
            t <<= shift;
            t |= low_bits;
        }

        if ((e->net_ipos[j] ^ t) & (~255 << shift))
        {
            #if 0
                if(ENTTYPE(e) == ENTTYPE_PLAYER)
                {
                    int diff = (e->net_ipos[j] >> (shift + 8)) - (t >> (shift + 8));

                    printf(    diff > 0 ?
                                "no %d sub @ shift %d (%1.2f, %1.2f), diff: +%-3d (0x%6.6x, 0x%6.6x)\n" :
                                "no %d sub @ shift %d (%1.2f, %1.2f), diff: %-4d (0x%6.6x, 0x%6.6x)\n",
                            j,
                            shift,
                            maxval,
                            e->mat[3][1],
                            diff,
                            e->net_ipos[j],
                            t);
                }
            #endif

            send_sub = 0;
        }

        ipos[j] = t;
        //assert(t >= 0);
        if (ipos[j] != e->net_ipos[j])
            update_pos = 1;
    }
    if (update_pos)
        update_pos = shift + 1;

    if (!send_sub)
    {
        copyVec3(scale_pos, ipos);

        update_pos = 7;
    }

    for(j = 0; j < 3; j++)
    {
        float c;
        int new_ipos = ipos[j];

        // Decode ipos the same way the client will to get the F32 component.

        t = new_ipos;
        t -= (1 << (POS_BITS-1));
        c = t / POS_SCALE;

        e->net_ipos[j] = new_ipos;
        new_net_pos[j] = c;
    }

    copyVec3(e->net_pos, e->old_net_pos);
    copyVec3(new_net_pos, e->net_pos);

    if(ENTINFO_BY_ID(e_idx).send_hires_motion && !ENTINFO_BY_ID(e_idx).send_on_odd_send)
    {
        setInterpData(e, e->old_net_pos, new_net_pos);
    }

    e->pos_update = update_pos;
    e->pos_shift = shift;
}

// now using quats
/*
static U32 zeroEuler;
static F32 zeroEulerUnpacked;

static void setZeroEuler()
{
    static int init;
    if(!init){
        init = 1;
        zeroEuler = packEuler(0,9);
        zeroEulerUnpacked = unpackEuler(zeroEuler,9);
    }
}
*/

static void setNetOrientation(Entity *e)
{
    int     j,update_qrot = 0;
    U32     iqrot[3];
    Quat    qrot;
    Quat    new_net_qrot;


    mat3ToQuat(ENTMAT(e), qrot    );

    // We need to be sure that the W is positive, because when we extract it in quatCalcWFromXYZ(), 
    // we use a sqrt to calc it, and lose the sign information
    quatNormalize(qrot);
    quatForceWPositive(qrot);


    for(j=0;j<3;j++)
    {
        iqrot[j] = packQuatElem(qrot[j],9);
        // Check to see if the value has changed
        if (iqrot[j] != e->net_iqrot[j])
        {
            // It has changed, so send it and set the update_qrot flag
            new_net_qrot[j] = unpackQuatElem(iqrot[j], 9);
            update_qrot |= (1<<j);
        }
        else
        {
            // Hasn't changed, so just copy the old value and don't set the flag
            new_net_qrot[j] = e->net_qrot[j];
        }
    }

    // If our quat has changed at all, we need to recalculate the W element and normalize it
    if (update_qrot)
    {
        // Extracts W from the other elements (quaternions are unit length)
        // note: assumes W was positive going into packing.
        quatCalcWFromXYZ(new_net_qrot);
        quatNormalize(new_net_qrot);
    }
    else
    {
        quatW(new_net_qrot) = quatW(e->net_qrot);
    }

    copyQuat(e->net_qrot, e->old_net_qrot);
    copyQuat(new_net_qrot, e->net_qrot);
    copyVec3(iqrot,e->net_iqrot);
    e->qrot_update = update_qrot;

/*
    if(!*(int*)&e->mat[1][0] && !*(int*)&e->mat[1][2])
    {
        pyr[0] = 0;
        pyr[1] = getVec3Yaw(e->mat[2]);
        pyr[2] = 0;

        ipyr[0] = zeroEuler;
        ipyr[1] = packEuler(pyr[1],9);
        ipyr[2] = zeroEuler;

        new_net_pyr[0] = zeroEulerUnpacked;
        new_net_pyr[1] = unpackEuler(ipyr[1],9);
        new_net_pyr[2] = zeroEulerUnpacked;

        if (e->net_ipyr[0] != zeroEuler)
            update_pyr |= 1;
        if (e->net_ipyr[1] != ipyr[1])
            update_pyr |= 2;
        if (e->net_ipyr[2] != zeroEuler)
            update_pyr |= 4;
    }
    else
    {
        getMat3YPR(e->mat,pyr);
        for(j=0;j<3;j++)
        {
            ipyr[j] = packEuler(pyr[j],9);
            if (ipyr[j] != e->net_ipyr[j])
            {
                new_net_pyr[j] = unpackEuler(ipyr[j], 9);
                update_pyr |= (1 << j);
            }
            else
            {
                new_net_pyr[j] = e->net_pyr[j];
            }
        }
    }

    copyVec3(e->net_pyr, e->old_net_pyr);
    copyVec3(new_net_pyr, e->net_pyr);
    copyVec3(ipyr,e->net_ipyr);
    e->pyr_update = update_pyr;
    */
}

#ifdef RAGDOLL
static void setNetRagdoll(Entity *e)
{
    int iNumBones;
    
    assert( e->ragdoll );
    iNumBones = e->ragdoll->numBones;
    if (!e->ragdollCompressed)
        e->ragdollCompressed = malloc(sizeof(int) * iNumBones * 3);

    compressRagdoll(e->ragdoll, e->ragdollCompressed);

}
#endif



void entSendSetNetPositionAndOrientation(Entity* e)
{
    // MS:    This function is specifically for doors to force their net vars to be set for synchronizing
    //        the dynamic collision object correctly.  You must call entInitNetVars(e) afterwards.
    //        Seriously, I swear if you call this without resetting the net vars, there will be trouble.

    setNetPosition(e, e->owner);
    setNetOrientation(e);
}

static int INLINEDBG sendThisFrame(int owner) // Takes an index into the entities array
{
    return !ENTINFO_BY_ID(owner).send_on_odd_send || server_state.net_send.is_odd_send;
}

//Figure out how much extra to delay the fx on the client to synch with the animation

//SEQQ
/*This sends the last non-obvious move (move that all clients couldn't have predicted.
*/
static void setNetMove(Entity *e)
{
    int     movenum;
    F32        packetDelay;

    movenum = e->seq->animation.move_to_send->raw.idx;
    e->move_update |= ( movenum != e->move_idx );
    e->move_idx = movenum;
    if( !e->move_instantly )
        packetDelay = MINMAX( e->timeMoveChanged - e->lastTimeEntSent, 0, MAX_PACKETDELAY_COMPENSATION );
    else
        packetDelay = 0;

    e->net_move_change_time = (int)(packetDelay + 0.5);

    //if( e->move_update )
    //    assert( e->timeMoveChanged );
    e->timeMoveChanged = 0; //cleared for assert

    //if( e->move_update && 0 == stricmp( e->seq->type->name, "male" ))
    //    printf( "Move packetDelay : %f\n", ( packetDelay ) );

    //AI_LOG(    AI_LOG_SEQ,
    //        (e,
    //        "MoveLastFrame: ^3%s ^0(^3%s^0)\n",
    //        e->seq->animation.move_lastframe ? e->seq->animation.move_lastframe->name : "NONE",
    //        seqGetStateString(e->seq->state_lastframe)));

    assert(e->move_idx >= 0);
}

static void setNetFxParams(Entity *e)
{
    int i;
    // Determine number of new FX
    e->netfx_effective = e->netfx_count;
    for(i=0;i<e->netfx_count;i++)
    {
        switch( e->netfx_list[i].commandStatus )
        {
            xcase NEW_FX_COMMAND:
                // Queue this to get sent in this update.
                
                e->netfx_list[i].commandStatus = NEW_FX_COMMAND_TO_SEND;
            
            xcase NEW_FX_COMMAND_TO_SEND:
                // Was queued to get sent in entUpdateNetFxStatusAfterNetSend.

            xcase OLD_FX_COMMAND:
                e->netfx_effective--;
            
            xdefault:
                assertmsg(0, "NetFx in an invalid state.");
        }
    }
}

static void setNetParams(Entity *e, int e_idx)
{
    setNetPosition(e, e_idx);
    setNetOrientation(e);
    setNetMove(e);
    setNetFxParams(e);
#ifdef RAGDOLL
    if (e->ragdoll)
        setNetRagdoll(e);
#endif

    SET_ENTINFO_BY_ID(e_idx).net_prepared = 1;
}

static struct {
    int                last_idx_sent;
    ClientLink*        client;
    Packet*            destpak;
    EntNetLink*        sent_buf;
    int                full_update;
    Entity*            player_ent;
    int                reliable;
    int                interp_bits;
    int                interp_level;
    int                is_generic_pos_update;
    U32                old_abs_time;
    U32                new_abs_time;
    U32                abs_time_diff;
} send_ent;



static void s_addBuffPower(PowerListItem *ppowListItem, Entity *e, int diffOnly)
{
    UniqueBuff buff;
    if(ppowListItem->ppow->ppowBase->bShowBuffIcon)
    {
        int i;
        buff.uiID = ppowListItem->uiID;
        buff.er   = erGetRef(e);
        buff.ppow = ppowListItem->ppow->ppowBase;
        buff.time_remaining = 0;
        buff.mod_cnt = 0;
        buff.old_mod_cnt = 0;

        for(i=0; i<e->iCntBuffs; i++) // look for matching buffs
        {
            if( buff.er == e->aBuffs[i].er && buff.ppow == e->aBuffs[i].ppow && buff.uiID == e->aBuffs[i].uiID )
            {
                if (diffOnly)
                {
                    e->aBuffs[i].addedOrChangedThisTick = 1;
                }
                break;
            }
        }

        if(i>=e->iCntBuffs && e->iCntBuffs<MAX_BUFFS)
        {
            i = e->iCntBuffs;
            if (diffOnly)
            {
                buff.addedOrChangedThisTick = 1;
                eaiSortedPushUnique(&e->diffBuffs, i);
            }
            e->aBuffs[e->iCntBuffs++] = buff;
        }
    }
}

static void BuildBuffList(Entity *e)
{
    AttribMod *pmod = NULL;
    AttribModListIter amiter = {0};
    PowerListItem *ppowListItem;
    PowerListIter iter;
    Character *p = e->pchar;
    int i;
    int diffOnly;

    if (!p)
        return;

    if (ENTTYPE(e) != ENTTYPE_PLAYER)
    {
        Entity *owner = erGetEnt(e->erCreator);
        if (!owner || ENTTYPE(owner) != ENTTYPE_PLAYER)
            return;
    }

    eaiDestroy(&e->diffBuffs);

    if (!e->team_buff_update && !e->team_buff_full_update)
        return;

    diffOnly = !e->team_buff_full_update;

    if (diffOnly)
    {
        //    decompress the array
        for (i = e->iCntBuffs-1; i >= 0; --i)
        {
            if (e->aBuffs[i].addedOrChangedThisTick == -1)
            {
                if (i < (e->iCntBuffs-1))
                {
                    memcpy(&e->aBuffs[i], &e->aBuffs[i+1], sizeof(UniqueBuff)*(e->iCntBuffs-(i+1)));
                }
                e->iCntBuffs--;
            }
        }
        if (e->iCntBuffs < MAX_BUFFS)
            memset(&e->aBuffs[e->iCntBuffs], 0, sizeof(UniqueBuff) * (MAX_BUFFS-e->iCntBuffs));
        for (i = 0; i < e->iCntBuffs; ++i)
        {
            int j;
            e->aBuffs[i].addedOrChangedThisTick = 0;
            e->aBuffs[i].old_mod_cnt = e->aBuffs[i].mod_cnt;
            e->aBuffs[i].time_remaining = 0;
            for (j = 0; j < e->aBuffs[i].mod_cnt; ++j)
            {
                e->aBuffs[i].old_mod[j].fMag = e->aBuffs[i].mod[j].fMag;
                e->aBuffs[i].old_mod[j].iHash = e->aBuffs[i].mod[j].iHash;
                e->aBuffs[i].mod[j].fMag = e->aBuffs[i].mod[j].iHash= 0;
            }
            e->aBuffs[i].mod_cnt = 0;
        }
    }
    else
    {
        e->iCntBuffs = 0;
    }

    // Add auto powers
    for (ppowListItem = powlist_GetFirst(&p->listAutoPowers, &iter);
        ppowListItem != NULL && (diffOnly || e->iCntBuffs < MAX_BUFFS);
        ppowListItem = powlist_GetNext(&iter))
    {
        s_addBuffPower(ppowListItem, e, diffOnly);
    }

    // Add toggle powers that show up even in they cause no
    // mods on the player themselves.
    for (ppowListItem = powlist_GetFirst(&p->listTogglePowers, &iter);
        ppowListItem != NULL && (diffOnly || e->iCntBuffs < MAX_BUFFS);
        ppowListItem = powlist_GetNext(&iter))
    {
        s_addBuffPower(ppowListItem, e, diffOnly);
    }


     pmod = modlist_GetFirstMod(&amiter, &p->listMod);
     while(pmod!=NULL)
     {
         // Don't send things about to expire. By the time the user sees
         // it, it'll probably be gone. Toggle and Auto powers are likely
         // to renew the attribmod, though, so do show them, even when they
         // have short durations.
         // Don't put auto powers or toggle powers from the player in yet.
         if(pmod->ptemplate->ppowBase->bShowBuffIcon    && 
             ((pmod->fDuration - pmod->ptemplate->fDelay)>.25f
             || pmod->ptemplate->ppowBase->eType==kPowerType_Toggle
             || pmod->ptemplate->ppowBase->eType==kPowerType_Auto) )
         {
             UniqueBuff buff;
             F32 *pf = ATTRIB_GET_PTR(&g_attrModBase,pmod->ptemplate->offAttrib);
             buff.uiID = pmod->uiInvokeID;
             buff.er   = pmod->erSource;
             buff.ppow = pmod->ptemplate->ppowBase;
             if( pmod->ptemplate->offAttrib == offsetof(CharacterAttributes, fRechargeTime) )
                 buff.mod[0].fMag = pmod->fMagnitude;
             else
                 buff.mod[0].fMag = (pmod->fMagnitude + *pf);
 
             buff.mod_cnt = 1;
            buff.old_mod_cnt = 0;
             buff.mod[0].iHash = modGetBuffHash(pmod->ptemplate); 
             buff.time_remaining = pmod->fDuration;
 
             for(i=0; i<e->iCntBuffs; i++) // look for matching buffs, use largest duration
             {
                 if(  e->aBuffs[i].mod_cnt < MAX_CONTRIB && buff.uiID ==  e->aBuffs[i].uiID && buff.er ==  e->aBuffs[i].er && buff.ppow == e->aBuffs[i].ppow )
                 {
                     int j, collate = 0;
 
                     e->aBuffs[i].time_remaining = MAX( buff.time_remaining, e->aBuffs[i].time_remaining );
                     for(j = 0; j <  e->aBuffs[i].mod_cnt; j++ )
                     {
                         if( e->aBuffs[i].mod[j].iHash == modGetBuffHash(pmod->ptemplate) && !pmod->ptemplate->fDelay ) // if there are delayed effects, too bad I guess
                         {
                             e->aBuffs[i].mod[j].fMag += buff.mod[0].fMag;
                             collate = 1;
                         }
                     }
 
                     if(!collate)
                     {
                         e->aBuffs[i].mod[e->aBuffs[i].mod_cnt].fMag = buff.mod[0].fMag;
                         e->aBuffs[i].mod[e->aBuffs[i].mod_cnt].iHash = buff.mod[0].iHash;
                         e->aBuffs[i].mod_cnt++;
                     }
                     break;
                 }
             }
 
             if(i>=e->iCntBuffs && e->iCntBuffs<MAX_BUFFS)
             {
                 // Need to add it to the list
                 i = e->iCntBuffs;
                 e->aBuffs[e->iCntBuffs++] = buff;
             }
         }
         pmod = modlist_GetNextMod(&amiter);
     }

    for (i = e->iCntBuffs-1; i>=0; --i)
    {
        int max_original_duration = 0;
        int j;
        for( j=eaSize( &e->aBuffs[i].ppow->ppTemplates )-1; j>=0; j-- )
            max_original_duration = MAX( max_original_duration, e->aBuffs[i].ppow->ppTemplates[j]->fDuration + e->aBuffs[i].ppow->ppTemplates[j]->fDelay );

        if (max_original_duration >= 30)
        {
            if (diffOnly)
            {
                if (e->aBuffs[i].time_remaining && e->aBuffs[i].time_remaining < 10)
                {
                    e->aBuffs[i].addedOrChangedThisTick = 1;
                    eaiSortedPushUnique(&e->diffBuffs, i);
                }
            }
        }
        else
        {
            e->aBuffs[i].time_remaining = 0;
        }
    }

    if (diffOnly)
    {
        for (i = e->iCntBuffs-1; i>=0; --i)
        {
            int j;
            for (j = 0; j < e->aBuffs[i].mod_cnt; ++j)
            {
                int k;
                e->aBuffs[i].addedOrChangedThisTick = 1;
                for (k = 0; k < e->aBuffs[i].old_mod_cnt; ++k)
                {
                    if (e->aBuffs[i].mod[j].iHash == e->aBuffs[i].old_mod[k].iHash)
                    {
                        if (e->aBuffs[i].mod[j].fMag != e->aBuffs[i].old_mod[k].fMag)
                        {
                            eaiSortedPushUnique(&e->diffBuffs, i);
                            j = e->aBuffs[i].mod_cnt;        //    break out entirely
                        }
                        break;
                    }
                }
                if (k >= e->aBuffs[i].old_mod_cnt)
                {
                    eaiSortedPushUnique(&e->diffBuffs, i);
                    break;
                }
            }
            if (!e->aBuffs[i].addedOrChangedThisTick)
            {
                e->aBuffs[i].addedOrChangedThisTick = -1;
                eaiSortedPushUnique(&e->diffBuffs, i);
            }
        }
    }
}

void entPrepareUpdate()
{
    static  U32 count;
     int            i;
     Entity *e;

    // now using quats
    //setZeroEuler();

    count++;

    send_ent.old_abs_time = send_ent.new_abs_time;
    send_ent.new_abs_time = ABS_TIME;
    send_ent.abs_time_diff = send_ent.new_abs_time - send_ent.old_abs_time;

    memset(need_to_send_cache, 0, sizeof(need_to_send_cache));
 
     for(i=1;i<entities_max;i++)
     {
         if ((entity_state[i] & ENTITY_IN_USE ) || ((entity_state[i] & ENTITY_SLEEPING) && (ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER)))
         {
             if (!sendThisFrame(i))
                 continue;
 
             e = entities[i];
             BuildBuffList(e);
         }
     }
}

void entFinishUpdate()
{
    int            i;
    Entity*        e;
    F32            lastTimeEntSent = ((F32)ABS_TIME/100.0);
    int            count = player_ents[PLAYER_ENTS_ACTIVE].count;
    Entity**    players = player_ents[PLAYER_ENTS_ACTIVE].ents;

    aiResetEntSend();
    ent_delete_count = 0;
    cmdOldSetSends(server_visible_cmds,0);
    groupDynResetSends();
    for(i=1;i<entities_max;i++)
    {
        // JS:  Yet another special hack.
        //      Normally, entities that are not in use do not have their updates sent and, therefore,
        //      do not need to run the update cleanup code here. BUT...
        //      When the server initially starts sending a game client their entity,
        //      the entity is not in ENTITY_IN_USE mode.  However, since the entity updates
        //      has been sent out, the server must execute this update cleanup code for those
        //      entities.
        if ((entity_state[i] & ENTITY_IN_USE ) || ((entity_state[i] & ENTITY_SLEEPING) && (ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER)))
        {
            e = entities[i];

            playerCreatedStoryArc_Update(e);

            if (e->send_packet) {
                pktFree(e->send_packet);
                e->send_packet = NULL;
            }

            if (!sendThisFrame(i))
                continue;

            if(e->pos_history.generic_end > 0)
            {
                int diff;
                e->pos_history.net_begin = e->pos_history.generic_end - 1;
                diff = (e->pos_history.end - e->pos_history.net_begin + ARRAY_SIZE(e->pos_history.pos)) % ARRAY_SIZE(e->pos_history.pos);
                e->pos_history.count -= diff;
                e->pos_history.end = e->pos_history.net_begin;
                e->pos_history.generic_end = 0;
            }
            else if(e->pos_update)
            {
                copyVec3(e->net_pos, e->pos_history.pos[0]);
                copyVec3(e->net_qrot, e->pos_history.qrot[0]);
                e->pos_history.time[0] = ABS_TIME;
                e->pos_history.count = 1;
                e->pos_history.net_begin = 0;
                e->pos_history.end = 0;
                e->pos_history.generic_end = 0;
            }
            else
            {
                int diff = (e->pos_history.end - e->pos_history.net_begin + ARRAY_SIZE(e->pos_history.pos)) % ARRAY_SIZE(e->pos_history.pos);
                e->pos_history.count -= diff;
                e->pos_history.end = e->pos_history.net_begin;
            }

            if (e->updated_powerCust)
                updateCustomFX(e);

            if(e->netfx_count)
            {
                entUpdateNetFxStatusAfterNetSend(e);
            }
            e->netfx_effective = -1;

            seqClearRandomNumberUpdateFlag();

            //JE: removed this printf check while doing performance tuning
            //if(e->resend_hp && !(e->lastticksent == global_state.global_frame_count) )
            //    printf("Clearing resend_hp without ever sending them to anyone!\n");

            e->send_xlucency              = FALSE;
            e->send_costume               = FALSE;
            e->updated_powerCust        = FALSE;
            e->send_all_costume            = FALSE;
            e->send_all_powerCust        = FALSE;
            e->resend_hp                  = FALSE;
            e->team_buff_update            = FALSE;
            e->team_buff_full_update    = FALSE;
            e->tf_params_update            = FALSE;
            e->target_update              = FALSE;
            e->clear_target                = FALSE;
            e->cached_send_packet         = FALSE;
            e->level_update                  = FALSE;
            e->general_update            = FALSE;
            e->status_effects_update    = FALSE;
            e->pvp_update                = FALSE;
            e->petinfo_update            = FALSE;
#if CONFUSED_COSTUMES
            e->send_others_costumes        = FALSE;
#endif

            if(e->pl)
            {
                e->pl->send_afk_string = FALSE;
            }

            //no longer any need to hold your breath on which move to use. so might as well
            //make these two accurate. //SEQQ
            e->move_idx = e->seq->animation.move->raw.idx;
            assert(e->move_idx >= 0);
            e->seq->animation.move_to_send = e->seq->animation.move;
            assert(e->seq->animation.move_to_send);
            e->move_update = 0;
            e->move_instantly = 0;

            e->state.last_sent_mode = e->state.mode;
            e->lastTimeEntSent = lastTimeEntSent;

            if (e->triggered_move_count) {
                ZeroArray(e->triggered_moves);
                e->triggered_move_count = 0;
            }
            e->set_triggered_moves = 0;  //This should never be true.  What is going on??

            // JE: Hack because logout_update may be set *after* entSendUpdate but before entFinishUpdate
            if (e->logout_update)
                e->logout_update--;

            e->supergroup_update = 0;
            e->team_update = 0;
            e->collision_update = 0;
            e->draw_update = 0;
            e->send_on_odd_send_update = 0;
            e->levelingpact_update = 0;
            e->helper_status_update = 0;
            e->name_update = 0;

            //JE: put this last, since it thrashes the cache the most
            if(e->pchar && e->sentStats)
            {
                PERFINFO_AUTO_START("sdCopyStruct", 1);
                    e->sentStats = 0;
                    if (ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER) {
                        sdCopyStruct(SendFullCharacter, e->pchar, e->pcharLast);
                    } else {
                        sdCopyStruct(SendBasicCharacter, e->pchar, e->pcharLast);
                    }
                PERFINFO_AUTO_STOP();
            }

            //if( 0 == stricmp( entities[i]->seq->type->name, "male") )
            //    printf("#######Update End ##### \n");
        }
    }

    for(i = 0; i < count; i++){
        Entity* e = players[i];

        if(!e)
            continue;

        // Send the packet for this entity (includes the entUpdate packet in the beginning, and all of the SERVER_GAME_CMDS
        if (e->entityPacket)
        {
            ClientLink  *client = clientFromEnt(e);
            if (client)
            {
                //JE: chat messages tend to throw this assert:
                // assert(client->ready); // Should not be sending packets to a client who isn't ready!
                pktSendBitsPack(e->entityPacket, 1, 0 ); // mark end of packet

                PERFINFO_RUN(
                    if(e->entityPacket->compress)
                    {
                        PERFINFO_AUTO_START("pktSendCompressed", 1);
                    }
                    else
                    {
                        PERFINFO_AUTO_START("pktSend", 1);
                    }
                );

                    pktSend(&e->entityPacket,client->link);

                    client->sentResendWaypointUpdate = 0;

                PERFINFO_AUTO_STOP();

                e->entityPacket = NULL;
            }
        }
    }
    groupDynFinishUpdate();
}

/*

    FYI: This info is all out of date.
    Update Packet:
    SERVER_SCENE / SERVER_UPDATE
    F32 frame_time
    0/1 GetNames
    EntUpdates

    [full/delta] [idx] [select] [xyz] [py] [r] [move]
        1        1->16   3       15     6/11    3/6/12
    all = 1 + 8 + 3 + 15 + 11 + 6 = 44
    avg = 5 + 5 + 15 + 10 = 35

    bst = 1 + 1 + 3 + 15 + 6 = 26
    bst4= 1 + 1 + 3 + 12 + 5 = 22


    full update
    [full/delta] [idx] [gfxidx] [seqidx] [player] [xyz] [py] [move]
        1        1->16     6        6       1       96   18     3/6
        avg = 1 + 4 + 6 + 6 + 1 + 96 + 18 + 9 = 141

    missed frame
    [xyz] [py]
      96   18
        96 + 18 = 114

*/
// step <= 1/8 speed
// numsteps = 16 * speed
//speed 0..1    1/32
//speed 1..4    1/8
//speed 4..16   1/2

static int entSendDeletes(Packet *pak,EntNetLink *sent_buf,U32 *sent_ents_bits)
{
    int     i,j,reliable=0,count=0,idx,word_count;
    int     deletes[MAX_DELETES*2]; // This array is svr, id, svr, id, etc

    // Only need to send deletes for entities this client knows about
    for(i=0;i<ent_delete_count;i++)
    {
        if (TSTB(sent_buf->sent_ids,ent_deletes[i]) && count < MAX_DELETES*2-1)
        {
            deletes[count++] = ent_deletes[i];
            assert(ent_delete_ids[i]!=-1);
            deletes[count++] = ent_delete_ids[i];
            CLRB(sent_buf->sent_ids,ent_deletes[i]);
        }
    }

    // tell client to delete entities that went out of his visible range
    word_count = (entities_max + 31)/32;
    for(j=0;j<word_count;j++)
    {
        U32        del_bits,tracked,sent;

        tracked = sent_buf->sent_ids[j];
        sent    = sent_ents_bits[j];
        del_bits = tracked & ~sent;
        if (!del_bits)
            continue;
        for(i=0;i<32;i++)
        {
            if ((del_bits & (1 << i)) && count < MAX_DELETES*2-1)
            {
                idx = i + (j << 5);
                  deletes[count++] = idx;
                deletes[count++] = entities[idx]->id;
                CLRB(sent_buf->sent_ids,idx);
            }
        }
    }

    pktSendBitsPack(pak,1,count>>1);
    for(i=0;i<count;i++) {
        pktSendBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX,deletes[i]);
    }
    if (count)
        return 1;
    return 0;
}

// This function has also doubled as a lazy init of setNetParams
static int determineNeedToSend(int i) {
    Entity *e;
    if (entity_state[i] & ENTITY_SLEEPING) {
        setNetParams(entities[i], i);
        return 0;
    }
    if (!(entity_state[i] & ENTITY_IN_USE )) {
        return 0;
    }
    e = entities[i];
    if (sendThisFrame(i) || !ENTINFO_BY_ID(i).net_prepared) {
        setNetParams(e, i);
    }
    if ( e->draw_update ||
        (e->state.mode != e->state.last_sent_mode) ||
        e->resend_hp ||
        e->netfx_effective ||
        e->updated_powerCust||
        e->send_costume||
        e->supergroup_update||
        e->send_xlucency||
        e->general_update ||
        e->pos_update ||
        e->qrot_update ||
        e->move_update ||
        e->triggered_move_count ||
        e->tf_params_update ||
        e->target_update ||
        e->team_update ||
        e->collision_update ||
        e->logout_update==1 ||
        e->level_update ||
        (e->pl && e->pl->send_afk_string) ||
        e->status_effects_update ||
        e->pvp_update ||
        e->petinfo_update ||
        e->levelingpact_update ||
        e->helper_status_update ||
        e->name_update ||
#ifdef RAGDOLL
        // ragdoll always send, for now
        e->ragdoll ||
        e->lastRagdollFrame || // need to send when the ragdoll has been destroyed
#endif
        // current or max attributes changed since last update?
        e->pchar && sdHasDiff(SendBasicCharacter, e->pcharLast,  e->pchar)
        )
    {
        return 1;
    }

    //printf("not sending ent %d, type %d: %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
    //    e->owner,
    //    ENTTYPE(e),
    //    e->draw_update,
    //    (e->state.mode != e->state.last_sent_mode),
    //    e->resend_hp,
    //    e->netfx_effective,
    //    e->send_costume,
    //    e->pos_update,
    //    e->qrot_update,
    //    e->move_update,
    //    e->logout_update,
    //    sdHasDiff(&basicAttribDef, &e->lastCurrentAttribs,  &e->pchar->attrCur),
    //    sdHasDiff(&basicAttribDef, &e->lastMaxAttribs,      &e->pchar->attrMax));

    return 0;
}

void cacheDebugCounting(Packet *pak, Entity *e, int is_generic_send) {
    if (is_generic_send) {
        PERFINFO_AUTO_START("is_generic_send = true", 1);
        if (e->cached_send_packet) {
            PERFINFO_AUTO_START("cached_send_packet = true", 1);
            PERFINFO_AUTO_STOP();
        } else {
            PERFINFO_AUTO_START("cached_send_packet = FALSE", 1);
            PERFINFO_AUTO_STOP();
        }
        PERFINFO_AUTO_STOP();
    } else {
        PERFINFO_AUTO_START("is_generic_send = FALSE", 1);
        PERFINFO_AUTO_STOP();
    }
}

// Receive code for this shows up twice in entrecv.c (general receive and entCreate)
static void INLINEDBG entSendPetInfo(Packet *pak, Entity *e) {
    Entity *entOwner;
    if((entOwner=erGetEnt(e->erOwner))!=NULL)
    {
        // This is a pet, send the creator
        pktSendBits(pak,1,1);
        pktSendBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX,entOwner->owner);

        entOwner=erGetEnt(e->erCreator);
        pktSendBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX,entOwner?entOwner->owner:0);
        if( e->pchar && e->pchar->ppowCreatedMe )
        {
            pktSendBits(pak,1,1);
            basepower_Send(pak, e->pchar->ppowCreatedMe);
        }
        else
        {
            pktSendBits(pak,1,0);
        }


        pktSendBits( pak, 1, e->commandablePet );
        pktSendBits( pak, 1, isPetDisplayed(e) );
        pktSendBits( pak, 1, e->petDismissable );

        //If I'm a pet with a special pet power, send down its name so I can display it.
        pktSendIfSetString(pak, e->specialPowerDisplayName );
        pktSendBitsPack( pak, 1, e->specialPowerState );
        pktSendIfSetString(pak, e->petName );
    }
    else
    {
        pktSendBits(pak,1,0);
    }
}

static void INLINEDBG entSendCreate(Packet *pak, Entity *e, Entity *player_ent, int inPhase) {
    pktSendBits(pak,1,0);   // 1 means end of ent list
    pktSendBitsPack(pak,PKT_BITS_TO_REP_SVR_IDX,e->id);
    pktSendBitsPack(pak,2,ENTTYPE(e));
    if (ENTTYPE(e) == ENTTYPE_PLAYER)
    {
        pktSendBits(pak,1,e == player_ent);
        if (e == player_ent)
            pktSendBitsPack(pak,1,player_ent->access_level);
        pktSendBitsPack(pak,PKT_BITS_TO_REP_DB_ID,e->db_id);
    }
    else
    {
        entSendPetInfo(pak,e);
    }

    if(ENTTYPE(e) == ENTTYPE_PLAYER || ENTTYPE(e) == ENTTYPE_CRITTER)
    {
        pktSendBitsPack(pak,1, e->pl && e->pl->isBeingCritter);

        // send class, origin, title, and level
        if( ENTTYPE(e) == ENTTYPE_PLAYER && !e->pl->isBeingCritter )
        {
            pktSendBitsPack( pak, 1, classes_GetIndexFromPtr( &g_CharacterClasses, e->pchar->pclass) );
            pktSendBitsPack( pak, 1, origins_GetIndexFromPtr( &g_CharacterOrigins, e->pchar->porigin) );
        }
        else
        {
            pktSendBitsPack( pak, 1, classes_GetIndexFromPtr( &g_VillainClasses, e->pchar->pclass) );
            pktSendBitsPack( pak, 1, origins_GetIndexFromPtr( &g_VillainOrigins, e->pchar->porigin) );
        }

        if( e->pl )
        {
            pktSendBits(pak,1,1);
            pktSendBits(pak, 1, e->pl->playerType); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
            pktSendBits(pak, 2, e->pl->playerSubType); STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= (1<<2))
            pktSendBits(pak, 3, e->pl->praetorianProgress); STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= (1<<3))
            pktSendBitsPack(pak,5,e->pl->titleThe);
            pktSendIfSetString(pak, e->pl->titleTheText);
            pktSendIfSetString(pak, e->pl->titleCommon);
            pktSendIfSetString(pak, e->pl->titleOrigin);
            pktSendBitsAuto(pak, e->pl->titleColors[0]);
            pktSendBitsAuto(pak, e->pl->titleColors[1]);
            pktSendBits(pak, 32, e->pl->titleBadge);
            pktSendIfSetString(pak, e->pl->titleSpecial);
            pktSendIfSetString(pak, e->pl->chat_handle );
        }
        else
            pktSendBits(pak,1,0);

        if (e->pchar)
        {
            pktSendBits(pak,1,1);
            pktSendBits(pak,1,e->pchar->playerTypeByLocation); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
        }
        else
            pktSendBits(pak,1,0);
    }

    if( e->name )
    {
        pktSendBits(pak,1,1);
        pktSendString( pak, e->name );
    }
    else
        pktSendBits(pak,1,0);

    //mm if this guy has just been created, and has the fade flag, the client should
    //fade him in.  If he's been around a while, and just coming into view, don't fade him in in any event
    // ARM always fade in if flagged, even if they have been around for a while
    // this will play nicely with phasing, and you aren't going to notice whether they fade or not
    // if they aren't newly visible due to phasing, because they're so far away.

    {
        pktSendBits( pak, 1, e->fade ); 
        pktSendBits( pak, 1, inPhase );
    }

    //for synchronizing client gfx
    pktSendBits(pak,32, e->randSeed);

    if(ENTTYPE(e)==ENTTYPE_CRITTER && e->villainDef)
    {
        pktSendBits(pak, 1, 1);
         pktSendBitsPack(pak, 2, villainRankConningAdjustment(e->villainRank));
        pktSendString( pak, localizedPrintf(player_ent, villainGroupNameFromEnt(e)) );    
        pktSendBits(pak, 1, server_state.clickToSource);
        if (server_state.clickToSource)
        {            
            pktSendString(pak, e->villainDef->fileName);
            pktSendString(pak, e->villainDef->name);
        }
        pktSendIfSetBitsPack(pak,5,e->idDetail);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    //MW if there's an override in the villaindef file for the class name, add it here. (see pchDisplayClassNameOverride on client)
    if(ENTTYPE(e)==ENTTYPE_CRITTER && e->villainDef && e->villainDef->displayClassName && !e->dontOverrideName)
    {
        pktSendBits(pak, 1, 1);
        pktSendString( pak, localizedPrintf(player_ent,e->villainDef->displayClassName) );
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    //favoriteWeaponAnimList
    //MW if there's a favoriteWeaponAnimList in the villaindef file add it here.
    if(ENTTYPE(e)==ENTTYPE_CRITTER && e->favoriteWeaponAnimList )
    {
        pktSendBits(pak, 1, 1);
        pktSendString( pak, localizedPrintf(player_ent,e->favoriteWeaponAnimList ) );
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    if (e->persistentNPCInfo && e->persistentNPCInfo->pnpcdef && e->persistentNPCInfo->pnpcdef->name)
    {
        pktSendBits(pak, 1, 1);
        pktSendString(pak, e->persistentNPCInfo->pnpcdef->name);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    if (e->persistentNPCInfo)
    {
        pktSendBitsAuto(pak, e->persistentNPCInfo->contact);
    }
    else
    {
        pktSendBitsAuto(pak, 0);
    }
}

static void entSetGenericInterpPositions(    Packet* pak,
                                            Entity* e,
                                            InterpPosition ip[7],
                                            int y_pos_used[7],
                                            int xz_pos_used[7],
                                            Vec3 old_pos,
                                            Vec3 new_pos,
                                            Quat old_qrot,
                                            Quat new_qrot)
{
    int i;
    Vec3 unpacked_pos[9];
    int used[7];
    int used_count = 0;
    int max_y = 0;
    int max_xz = 0;

    // record the actual length of the incoming arrays to prevent reading past the end. (mapserver.exe crash on entity move)
    if (y_pos_used)
        max_y = sizeof(y_pos_used);
    if (xz_pos_used)
        max_xz = sizeof(xz_pos_used);                                        

    if(y_pos_used && xz_pos_used)
    {
        // Copy the source and target position so they can be looked up as parent positions.

        copyVec3(old_pos, unpacked_pos[7]);
        copyVec3(new_pos, unpacked_pos[8]);

        for(i = 0; i < 7; i++)
        {
            if(y_pos_used[i] || xz_pos_used[i])
            {
                Vec3 temp_pos;
                int cur_bits = send_ent.interp_bits + 1 - binpos_level[i];

                // Calculate the midpoint.

                copyVec3(unpacked_pos[binpos_to_pos[binpos_to_parentbinpos[i][0]]], temp_pos);
                addVec3(unpacked_pos[binpos_to_pos[binpos_to_parentbinpos[i][1]]], temp_pos, temp_pos);
                scaleVec3(temp_pos, 0.5, temp_pos);

                if(i<max_y && y_pos_used[i])
                {
                    temp_pos[1] += decodeOffset(ip[i].offsets[vecYidx], send_ent.interp_bits, cur_bits);
                }

                if(i<max_xz && xz_pos_used[i])
                {
                    temp_pos[0] += decodeOffset(ip[i].offsets[vecXidx], send_ent.interp_bits, cur_bits);
                    temp_pos[2] += decodeOffset(ip[i].offsets[vecZidx], send_ent.interp_bits, cur_bits);
                }

                copyVec3(temp_pos, unpacked_pos[binpos_to_pos[i]]);
                used[binpos_to_pos[i]] = 1;
                used_count++;
            }
            else
            {
                used[binpos_to_pos[i]] = 0;
            }
        }
    }

    {
        int        cur_pos =            e->pos_history.net_begin;
        Vec3*    real_pos_hist =        e->pos_history.pos;
        Quat*    real_qrot_hist =        e->pos_history.qrot;
        U32*    real_time_hist =    e->pos_history.time;

        if(e->pos_history.net_begin != e->pos_history.end)
        {
            cur_pos = (cur_pos + 1) % ARRAY_SIZE(e->pos_history.pos);

            //#define SEND_VEC3(x) pktSendF32(pak, (x)[0]);pktSendF32(pak, (x)[1]);pktSendF32(pak, (x)[2]);

            #define PRINT_POSITIONS 0

            #if PRINT_POSITIONS
                if(ENTTYPE(e) != ENTTYPE_PLAYER)
                {
                    printf("%d : %d -----------------------------------------------------------\n", e->owner, ABS_TIME);

                    printf("%2d: %f\t%f\t%f\t\t%f\t%f\t%f\n", cur_pos, vecParamsXYZ(old_pos), quatParamsXYZW(old_qrot));
                }
            #endif

            if(used_count)
            {
                // Interp the Quat until I come up with something better..

                /*
                pyr_diff[0] = subAngle(new_pyr[0], old_pyr[0]);
                pyr_diff[1] = subAngle(new_pyr[1], old_pyr[1]);
                pyr_diff[2] = subAngle(new_pyr[2], old_pyr[2]);
                */

                for(i = 0; i < 7; i++)
                {
                    if(used[i])
                    {
                        float scale = (float)(i + 1) / 8.0f;
                        //int k;

                        real_time_hist[cur_pos] = send_ent.old_abs_time + (send_ent.abs_time_diff * (i+1)) / 8;
                        copyVec3(unpacked_pos[i], real_pos_hist[cur_pos]);
                        quatInterp(scale, old_qrot, new_qrot, real_qrot_hist[cur_pos]);
    
                        /*
                        for(k = 0; k < 3; k++)
                            real_pyr_hist[cur_pos][k] = addAngle(old_pyr[k], pyr_diff[k] * scale);
                            */

                        #if PRINT_POSITIONS
                            if(ENTTYPE(e) != ENTTYPE_PLAYER)
                            {
                                printf("%2d: %f\t%f\t%f\t\t%f\t%f\t%f\n", cur_pos, vecParamsXYZ(real_pos_hist[cur_pos]), quatParamsXYZW(real_qrot_hist[cur_pos]));
                            }
                        #endif

                        //SEND_VEC3(real_pos_hist[cur_pos]);
                        //SEND_VEC3(real_pyr_hist[cur_pos]);

                        cur_pos = (cur_pos + 1) % ARRAY_SIZE(e->pos_history.pos);
                    }
                }
            }

            #if PRINT_POSITIONS
                if(ENTTYPE(e) != ENTTYPE_PLAYER)
                {
                    printf("%2d: %f\t%f\t%f\t\t%f\t%f\t%f\n", cur_pos, vecParamsXYZ(new_pos), quatParamsXYZW(new_qrot));
                }
            #endif

            real_time_hist[cur_pos] = send_ent.new_abs_time;
            copyVec3(new_pos, real_pos_hist[cur_pos]);
            copyQuat(new_qrot, real_qrot_hist[cur_pos]);

            //SEND_VEC3(new_pos);
            //SEND_VEC3(new_pyr);

            e->pos_history.generic_end = cur_pos + 1;
        }
    }
}

static void INLINEDBG entSendPosUpdate(    Packet *pak,
                                        Entity *e,
                                        int pos_update,
                                        int ent_create,
                                        int is_generic_send)
{
    int i, j;
    pktSendBits(pak,3,pos_update);

    if (pos_update)
    {
        for(j=0;j<3;j++)
        {
            if (pos_update != 7)
            {
                pktSendBits(pak,8,(e->net_ipos[j] >> e->pos_shift) & 255);
                //if(e->type == ENTTYPE_CRITTER)
                //{
                //  printf("sent pos_update %d: %d, %d\n", j, pos_update, (e->net_ipos[j] >> e->pos_shift) & 255);
                //}
            }
            else
            {
                pktSendBits(pak,POS_BITS,e->net_ipos[j]);
                //if(e->type == ENTTYPE_CRITTER)
                //{
                //  printf("sent pos_update %d: %d, %d\n", j, pos_update, e->net_ipos[j] & 0xffffff);
                //}
            }
        }
    }

    if(!ent_create)
    {
        InterpPosition* ip = e->interp_positions;
        int set_generic = 0;

        if(pos_update)
        {
            int y_used;
            int xz_used;
            int extra_info = 0;
            int move_instantly = e->move_instantly;

            if(move_instantly)
            {
                extra_info = 1;
            }
            else if(send_ent.interp_level && !e->firstRagdollFrame    )
            {
                int send_bits = send_ent.interp_bits - binpos_level[0];

                y_used =    ip[0].y_used &&
                            (ip[0].offsets[1] & ((1 << send_bits) - 1)) == (ip[0].offsets[1] & 0x7f);

                xz_used =    ip[0].xz_used &&
                            (ip[0].offsets[0] & ((1 << send_bits) - 1)) == (ip[0].offsets[0] & 0x7f) &&
                            (ip[0].offsets[2] & ((1 << send_bits) - 1)) == (ip[0].offsets[2] & 0x7f);

                extra_info = y_used || xz_used;
            }

            pktSendBits(pak, 1, extra_info);

            if(extra_info)
            {
                pktSendBits(pak, 1, move_instantly);

                if(send_ent.interp_level && !move_instantly && !e->firstRagdollFrame)
                {
                    int y_pos_used[7] = {1, 1, 1, 1, 1, 1, 1};
                    int xz_pos_used[7] = {1, 1, 1, 1, 1, 1, 1};

                    if(y_used && xz_used)
                    {
                        // Send one bit if both are set, which is hopefully the most common case.

                        pktSendBits(pak, 1, 1);
                    }
                    else
                    {
                        // Send two bits if only one is set.

                        pktSendBits(pak, 1, 0);
                        pktSendBits(pak, 1, y_used ? 1 : 0);
                    }

                    #define SIGN_BIT (0x80)
                    #define SEND_NODE_BIT(value) if(i){pktSendBits(pak, 1, value);}
                    #define SEND_OFFSET(idx)                                        \
                        pktSendBits(pak, 1, ip[i].offsets[idx] & SIGN_BIT ? 1 : 0);    \
                        pktSendBits(pak, send_bits, ip[i].offsets[idx]);

                    // Send the Y tree.

                    for(i = 0; i < 7; i++)
                    {
                        if(y_pos_used[binpos_parent[i]] && binpos_level[i] <= send_ent.interp_level)
                        {
                            if(!i && y_used || i && ip[i].y_used)
                            {
                                int send_bits = send_ent.interp_bits - binpos_level[i];
                                int hi_mask = BIT_RANGE(send_bits, 6);

                                if(i && (ip[i].offsets[vecYidx] & hi_mask))
                                {
                                    SEND_NODE_BIT(0);

                                    y_pos_used[i] = 0;
                                }
                                else
                                {
                                    SEND_NODE_BIT(1);

                                    SEND_OFFSET(vecYidx);
                                }
                            }
                            else
                            {
                                SEND_NODE_BIT(0);

                                y_pos_used[i] = 0;
                            }
                        }
                        else
                        {
                            y_pos_used[i] = 0;
                        }
                    }

                    // Send the XZ tree.

                    for(i = 0; i < 7; i++)
                    {
                        if(xz_pos_used[binpos_parent[i]] && binpos_level[i] <= send_ent.interp_level)
                        {
                            if(!i && xz_used || i && ip[i].xz_used)
                            {
                                int send_bits = send_ent.interp_bits - binpos_level[i];
                                int hi_mask = BIT_RANGE(send_bits, 6);

                                if(i && ((ip[i].offsets[vecXidx] | ip[i].offsets[vecZidx]) & hi_mask))
                                {
                                    SEND_NODE_BIT(0);

                                    xz_pos_used[i] = 0;
                                }
                                else
                                {
                                    SEND_NODE_BIT(1);

                                    SEND_OFFSET(vecXidx);
                                    SEND_OFFSET(vecZidx);
                                }
                            }
                            else
                            {
                                SEND_NODE_BIT(0);

                                xz_pos_used[i] = 0;
                            }
                        }
                        else
                        {
                            xz_pos_used[i] = 0;
                        }
                    }

                    // Unpack back into the position buffer, for collision prediction.

                    if(is_generic_send && !e->pos_history.generic_end)
                    {
                        entSetGenericInterpPositions(pak, e, ip, y_pos_used, xz_pos_used, e->old_net_pos, e->net_pos, e->old_net_qrot, e->net_qrot);
                        set_generic = 1;
                    }
                }
            }
        }

        if(!set_generic && is_generic_send && !e->pos_history.generic_end)
        {
            entSetGenericInterpPositions(pak, e, ip, NULL, NULL, e->old_net_pos, e->net_pos, e->old_net_qrot, e->net_qrot);
        }
    }
    e->firstRagdollFrame = 0;
}

static void INLINEDBG entSendQrotUpdate(Packet *pak, Entity *e, int qrot_update) {
    int j;

    //JE: 1 bit flag, since most of the time this isn't changing, we were sending 3 bits to say no
    pktSendIfSetBits(pak,3,qrot_update);

    if (!qrot_update)
        return;

    for(j=0;j<3;j++)
    {
        if (qrot_update & (1 << j))
            pktSendBits(pak,9,e->net_iqrot[j]);
    }
}

static void INLINEDBG entSendRagdollUpdate(Packet *pak, Entity *e, int ragdoll_update )
{
#ifdef RAGDOLL
    int j;
    U8 uiNumBones;

    // Slightly more efficient than pktSendIfSetBits
    if (!ragdoll_update)
    {
        pktSendBits(pak, 1, 0);
        return;
    }

    pktSendBits(pak, 1, 1);


    uiNumBones = e->ragdoll->numBones;
    pktSendBits(pak,5,uiNumBones);




    if ( e->fakeRagdoll)
        e->fakeRagdollFramesSent++;

    for (j=0; j<uiNumBones*3;++j)
    {
        pktSendBits(pak,NUM_BITS_PER_RAGDOLL_QUAT_ELEM,e->ragdollCompressed[j]);
    }
#else
    pktSendBits(pak,1,0);
#endif
}
static INLINEDBG void entSendSeqMoveUpdate(Packet *pak, Entity *e, int move_update)
{
    int i;
    U16    move_bits = 0; STATIC_INFUNC_ASSERT(NEXTMOVEBIT_COUNT <= 16);
    
    pktSendBits(pak,1,move_update);
    if (!move_update)
        return;
    pktSendIfSetBitsPack(pak,8,e->move_idx);
    pktSendIfSetBitsPack(pak,4,e->net_move_change_time);
    if(SAFE_MEMBER2(e->pchar,ppowStance,ppowBase))
    {
        const PowerFX* fx = power_GetFX(e->pchar->ppowStance);
        assert(fx);

        for(i = eaiSize(&fx->piModeBits)-1; i >= 0; --i)
        {
            switch(fx->piModeBits[i])
            {            
                xcase STATE_RIGHTHAND:
                    move_bits |= NEXTMOVEBIT_RIGHTHAND;
                xcase STATE_LEFTHAND:
                    move_bits |= NEXTMOVEBIT_LEFTHAND;
                xcase STATE_TWOHAND:
                    move_bits |= NEXTMOVEBIT_TWOHAND;
                xcase STATE_DUALHAND:
                    move_bits |= NEXTMOVEBIT_DUALHAND;
                xcase STATE_EPICRIGHTHAND:
                    move_bits |= NEXTMOVEBIT_EPICRIGHTHAND;
                xcase STATE_SHIELD:
                    move_bits |= NEXTMOVEBIT_SHIELD;
                xcase STATE_HOVER:
                    move_bits |= NEXTMOVEBIT_HOVER;
            }
        }
    }

    if( e && e->pchar )
    {
        for( i = 0; i < eaiSize(&e->pchar->piStickyAnimBits); i++ )
        {
            if(e->pchar->piStickyAnimBits[i] == STATE_AMMO)
                move_bits |= NEXTMOVEBIT_AMMO;
            if(e->pchar->piStickyAnimBits[i] == STATE_INCENDIARYROUNDS)
                move_bits |= NEXTMOVEBIT_INCENDIARYROUNDS;
            if(e->pchar->piStickyAnimBits[i] == STATE_CRYOROUNDS)
                move_bits |= NEXTMOVEBIT_CRYOROUNDS;
            if(e->pchar->piStickyAnimBits[i] == STATE_CHEMICALROUNDS)
                move_bits |= NEXTMOVEBIT_CHEMICALROUNDS;
        }
    }

    if(TSTB(e->seq->sticky_state,STATE_SHIELD))
        move_bits |= NEXTMOVEBIT_SHIELD;
    if(TSTB(e->seq->sticky_state,STATE_HOVER))
        move_bits |= NEXTMOVEBIT_HOVER;

    pktSendIfSetBits(pak,NEXTMOVEBIT_COUNT,move_bits);
}

static void INLINEDBG entSendAiAnimList(Packet *pak, Entity *e, int send_appearance)
{
    if(send_appearance || e->aiAnimListUpdated )
    {
        pktSendBits(pak,1,1);
        pktSendIfSetString(pak, e->aiAnimList);
    }
    else
    {
        pktSendBits(pak,1,0);
    }
}

static void INLINEDBG entSendSeqTriggeredMoves(Packet *pak, Entity *e)
{
    int j;
    TriggeredMove * tm;
    F32 packetDelay; //Time between when the last packet was sent out this move was created

    if(e->set_triggered_moves) //something went wrong, skip triggered move stuff
    {
        pktSendBitsPack(pak,1,0);
    }
    else
    {
        pktSendBitsPack(pak,1,e->triggered_move_count);
        for( j = 0 ; j < e->triggered_move_count ; j++ )
        {
            tm = &e->triggered_moves[j];
            if( !tm->triggerFxNetId ) //don't do this for Fx triggered
                packetDelay = MINMAX( tm->timeCreated - e->lastTimeEntSent, 0, MAX_PACKETDELAY_COMPENSATION );
            else
                packetDelay = 0;

            //if( 0 == stricmp( e->seq->type->name, "male" ))
            //    printf( "TriggeredMove packetDelay : %f\n", ( packetDelay ) );
            // FIX ME WOOMER: Fix this because fixing it would be good.
            pktSendBitsPack(pak,10,tm->idx);
            pktSendBitsPack(pak,6,(int)(tm->ticksToDelay + packetDelay));
            pktSendIfSetBitsPack(pak,16,tm->triggerFxNetId);
        }
    }
}

static void INLINEDBG entSendLogoutUpdate(Packet *pak, Entity *e, int logout_update) {
    if (!(ENTTYPE(e) == ENTTYPE_PLAYER))
        logout_update=0;
    pktSendBits(pak,1,logout_update==1);
    if (logout_update==1)
    {
        pktSendBits(pak,1,e->logout_bad_connection);
        pktSendIfSetBitsPack(pak,5,(int)(e->logout_timer / 30.f));
    }
}

static void INLINEDBG entSendPowerCust(Packet *pak, Entity *e, Entity *player_ent)
{
    int send_type;

    // For players entities and computer controlled entities that look like other heroes...
      if( (ENTTYPE(e) == ENTTYPE_PLAYER || ENTTYPE(e) == ENTTYPE_HERO) && e->pl && (e->updated_powerCust || e->send_all_powerCust)) // It's a player
        // we may have to loosen up on this check if we start using customizable weapons costume parts elsewhere
    {
        // Send the entire costume.
        send_type = AT_ENTIRE_COSTUME;
    }
    // For some special entities...
    else if((ENTTYPE(e) == ENTTYPE_CRITTER) && e->ownedPowerCustomizationList && (e->updated_powerCust || e->send_all_powerCust))
    {
        send_type = AT_OWNED_COSTUME;
    }
    else
    {
        send_type = AT_NO_APPEARANCE;
    }

    pktSendIfSetBitsPack(pak, 2, send_type);

    switch(send_type) {
        case AT_ENTIRE_COSTUME:
            {
                if (e == player_ent)
                {
                    //    sending yourself
                    pktSendBits( pak, 1, 1 );// sending the player themself
                    pktSendBits( pak, 8, e->pl->current_powerCust );
                    pktSendBits( pak, 8, e->pl->num_costume_slots );
                    if (e->send_all_powerCust)
                    {
                        int i;
                        pktSendBits( pak, 1, 1 );    //    full update
                        pktSendBits( pak, 4, costume_get_num_slots(e) );    //    how many?
                        for( i = 0; i < costume_get_num_slots(e) && i < MAX_COSTUMES; i++ )
                        {
                            powerCustList_send( pak, e->pl->powerCustomizationLists[i]);
                        }
                    }
                    else
                    {
                        pktSendBits( pak, 1, 0 );    // not sending more than one
                        powerCustList_send( pak, e->pl->powerCustomizationLists[e->pl->current_powerCust]);
                    }
                }
                else
                {
                    //    sending other
                    pktSendBits( pak, 1, 0 );    // sending the player themself
                }
            }break;
        case AT_OWNED_COSTUME:
            powerCustList_send( pak, e->ownedPowerCustomizationList);
            break;
    }
}

static void INLINEDBG entSendCostume(Packet *pak, Entity *e, Entity *player_ent, int send_appearance) {
    int send_type;
#if CONFUSED_COSTUMES
    int viewer_confused = player_ent->pchar->attrCur.fConfused > 0.0f || player_ent->unstoppable;
#endif
    int npcIndex;
    int costumeIndex;

    // Do we want to specifiy the appearance of this entity?
     if (!send_appearance) {
        send_type = AT_NO_APPEARANCE;
    }
    // If the viewer is confused show the confused NPC costume.
#if CONFUSED_COSTUMES
    else if(viewer_confused)
    {
        static int s_npcIndex;
        send_type = AT_NPC_COSTUME;

        if(!s_npcIndex)
        {
            NPCDef *npcDef = npcFindByName("MaleNPC_Villain_01", &s_npcIndex);
        }

        npcIndex = s_npcIndex;
        costumeIndex = 0;
    }
#endif
    // For players entities and computer controlled entities that look like other heroes...
    else if( (ENTTYPE(e) == ENTTYPE_PLAYER || ENTTYPE(e) == ENTTYPE_HERO) && e->pl ) // It's a player
    // we may have to loosen up on this check if we start using customizable weapons costume parts elsewhere
    {
        // Send the entire costume.
        send_type = AT_ENTIRE_COSTUME;
        npcIndex = e->npcIndex;
        costumeIndex = e->costumeIndex;
    }
    // For some special entities...
    else if((ENTTYPE(e) == ENTTYPE_CRITTER) && e->ownedCostume)
    {
        send_type = AT_OWNED_COSTUME;
    }
    else if(ENTTYPE(e) == ENTTYPE_MISSION_ITEM)
    {
        // Send the seq name to define the geometry and animation only.
        // The entity gets default geometry and texture.
        send_type = AT_SEQ_NAME;
    }
    else
    {
        send_type = AT_NPC_COSTUME;
        npcIndex = e->npcIndex;
        costumeIndex = e->costumeIndex;
    }

    pktSendIfSetBitsPack(pak, 2, send_type);

    switch(send_type) {
        case AT_ENTIRE_COSTUME:
                if( e==player_ent )
                {
                    pktSendBits( pak, 1, 1 );// sending the player themself

                    ent_SendCostumeRewards(pak, e);
                    pktSendBits( pak, 1, e->pl->pendingCostumeForceTime > timerSecondsSince2000() ? 1 : 0);
                    pktSendBits( pak, 8, e->pl->current_costume );
                    pktSendBits( pak, 8, e->pl->num_costume_slots );

                    if( e->send_all_costume )
                    {
                        int i;
                        pktSendBits( pak, 1, 1 ); //send full update
                        pktSendBits( pak, 4, e->pl->num_costumes_stored); // tell them how many we're sending
                        for( i = 0; i < e->pl->num_costumes_stored && i < MAX_COSTUMES; i++ )
                            costume_send( pak, e->pl->costume[i], 1 );
                    }
                    else
                    {
                        pktSendBits( pak, 1, 0 ); //just send current
                          costume_send( pak, e->pl->costume[e->pl->current_costume], 1 );
                    }

                    if( e->pl && e->supergroup_id )
                    {
                        pktSendBits( pak, 1, 1 );
                        costume_send( pak, costume_makeSupergroup( e ), 1 );
                        pktSendBits( pak, 1, e->pl->supergroup_mode );
                        pktSendBits( pak, 1,  e->pl->hide_supergroup_emblem);
                    }
                    else
                        pktSendBits( pak, 1, 0 );
                }
                else
                {
                      pktSendBits( pak, 1, 0 );  // sending another entity
                    pktSendBits( pak, 1, e->pl && e->pl->pendingCostumeForceTime > timerSecondsSince2000() ? 1 : 0);
                    pktSendBits( pak, 1, 0 );  // not sending more than one

                    if( e->pl && e->supergroup_id && e->pl->supergroup_mode )
                        costume_send( pak, costume_makeSupergroup( e ), 0 );
                    else
                        costume_send( pak, costume_current(e), 0 );

                    pktSendBits( pak, 1, 0 );    // not sending supergroup costume
                }
                if (e->pl && e->pl->costumeFxSpecial[0]) {
                    pktSendBits(pak, 1, 1);
                    pktSendString( pak, e->pl->costumeFxSpecial);
                } else {
                    pktSendBits(pak, 1, 0);
                }
                if(e->pl && e->pl->npc_costume) {
                    pktSendBits(pak, 1, 1);
                } else {
                    pktSendBits(pak, 1, 0);
                    break;
                }

            // Inentional fall-through
        case AT_NPC_COSTUME:
            pktSendBitsPack(pak, 12, npcIndex);
            pktSendBitsPack(pak, 1, costumeIndex);
            pktSendIfSetString( pak, e->costumeWorldGroup );
            if (e->customNPC_costume)
            {
                pktSendBitsPack(pak, 1, 1);
                NPCcostume_send(pak, e->customNPC_costume);
            }
            else
            {
                pktSendBitsPack(pak, 1, 0);
            }
            break;
        case AT_SEQ_NAME:
            pktSendString( pak, e->seq->type->name );
            pktSendIfSetString( pak, e->costumeWorldGroup );
            break;
        case AT_OWNED_COSTUME:
            costume_send(pak, e->ownedCostume, 0);
            break;
    }
}

static void INLINEDBG entSendTargetUpdate(Packet *pak, Entity *e, int target_update)
{
    if(target_update)
    {
        Entity *eAssist = e->pchar?erGetEnt(e->pchar->erAssist):NULL;
        Entity *eTarget = (e->pchar && !OnArenaMap())?erGetEnt(e->pchar->erTargetInstantaneous):NULL;
        Entity *eTaunter = (e->pchar && e->pchar->pmodTaunt)?erGetEnt(e->pchar->pmodTaunt->erSource):NULL;
        int iNumPlacates = e->pchar?eaSize(&e->pchar->ppmodPlacates):0;
        int iPlacate;

        if (e->clear_target) 
        {
            eTarget = NULL;
        }

        pktSendBits(pak, 1, 1);
        if(eTarget==NULL)
        {
            pktSendBits(pak, 1, 0);
        }
        else
        {
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, eTarget->owner);
        }

        if(eAssist==NULL)
        {
            pktSendBits(pak, 1, 0);
        }
        else
        {
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, eAssist->owner);
        }

        if(eTaunter==NULL)
        {
            pktSendBits(pak, 1, 0);
        }
        else
        {
            pktSendBits(pak, 1, 1);
            pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, eTaunter->owner);
        }

        if(e->clear_target)
        {
            pktSendBits(pak,1,1);
        }
        else
        {
            pktSendBits(pak,1,0);
        }

        pktSendBitsPack(pak, 2, iNumPlacates);
        for (iPlacate = iNumPlacates-1; iPlacate >= 0; iPlacate--)
        {
            Entity *pPlacater = erGetEnt(e->pchar->ppmodPlacates[iPlacate]->erSource);
            if( e->pchar->ppmodPlacates[iPlacate]->fMagnitude > 0 )
                pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, pPlacater?pPlacater->owner:0);
            else
                pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, 0);
        }
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

static void INLINEDBG entSendCharacterStats(Packet *pak, Entity *e, Entity *player_ent, int full_update) {
    int haspchar = (NULL != e->pchar);
    int sent_usual=0;
    // Usual case =
    //  has pchar
    //  no sidekick
    //  no stat update
    //  -> send a 0
    if (haspchar) {
        if (!full_update) {
            // We might be in the usual case, see if anything changed
            if (e == player_ent || !sdHasDiff(SendBasicCharacter, e->pcharLast, e->pchar)) {
                // General case!  Yay!
                pktSendBits(pak, 1, 0);
                sent_usual = 1;
            }
        }
    }

    if (sent_usual)
        return;

    pktSendBits(pak, 1, 1); // Not the "usual" case
    // Do we want to send stats with this character?
    pktSendBits(pak, 1, haspchar);
    if(haspchar)
    {
        if (e == player_ent) {
            // Fake a diff where nothing changed (1 bit)
            PERFINFO_AUTO_START("SendEmptyDiff", 1);
            sdPackEmptyDiff(SendBasicCharacter, pak, e->pcharLast, e->pchar, full_update, false, true);
            PERFINFO_AUTO_STOP();
        } else {
            PERFINFO_AUTO_START("SendBasicCharacter", 1);
            sdPackDiff(SendBasicCharacter, pak, e->pcharLast, e->pchar, full_update, false, true, 0, 0 );
            PERFINFO_AUTO_STOP();
            e->sentStats = 1;
        }
    }
}

static void INLINEDBG entSendModes(Packet *pak, Entity *e, int send_modes)
{
    // Has state changed???
    pktSendBits( pak, 1, send_modes?1:0 );
    if( send_modes )
    {
        pktSendIfSetBitsPack( pak, TOTAL_ENT_MODES, e->state.mode );
        //*reliable = TRUE;
    }
}

static void INLINEDBG entSendAfk(Packet *pak, Entity *e, int send_afk_string)
{
    int afk = e->pl && e->pl->afk;
    pktSendBits(pak, 1, afk);
    if (afk) {
        if(send_afk_string)
        {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, e->pl->afk_string);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
    }
}

static void INLINEDBG entSendLevelingpactInfo(Packet *pak, Entity *e, int update)
{
    int i;
    //send whether or not there's an update
    pktSendBits(pak, 1, (update)?1:0);

    if(!update)
        return;    //leave if there's nothing to update.

    //if we're just updating that the leveling pact has stopped existing, send that
    if(!e->levelingpact)
    {
        pktSendBitsAuto(pak, (int)0);    //id.
        return;
    }
    
    //send the id of the leveling pact.
    pktSendBitsAuto(pak, e->levelingpact_id);

    // send down how many members of the pact we will send
    //this should always be the number of members
    pktSendBitsAuto( pak, e->levelingpact->members.count); 

    for( i = 0; i < e->levelingpact->members.count; i++ )
    {
        // send their db_id
        pktSendBitsAuto( pak, e->levelingpact->members.ids[i]);

        //We're now always sending the names for the leveling pact because it because there's not an easy way for the
        //mapserver to track when the client needs an update and when they don't.  If you want to stop sending them,
        //send a 0 here instead of a 1.
            //pktSendBits( pak, 1, 0 );
        pktSendBits( pak, 1, 1 );
        pktSendString( pak, e->levelingpact->members.names[i] );
    }
}

static void INLINEDBG entSendOtherSupergroupInfo(Packet *pak, Entity *e, int update, int sendingSelf)
{
    if(update)
    {
        pktSendBits(pak, 1, 1);
        if(e->supergroup_id && e->supergroup)
        {
            pktSendBitsPack(pak, 2, e->supergroup_id);
            pktSendString(pak, e->supergroup->name);
            pktSendString(pak, e->supergroup->emblem);
            pktSendBits(pak, 32, e->supergroup->colors[0]);
            pktSendBits(pak, 32, e->supergroup->colors[1]);
            pktSendBitsPack(pak, 2, e->supergroup->spacesForItemsOfPower);

            // ----------
            // Send the supergroup task

            // send if active, and target is owner
            if (!db_state.is_endgame_raid && e->supergroup->activetask && sendingSelf)
            {
                pktSendBits(pak, 1, 1);// has a task
                // Uses the contact definition from the task context
                TaskStatusSend(e, e->supergroup->activetask, ContactDefinition(e->supergroup->activetask->sahandle.context), pak);
            }
            else
            {
                pktSendBits(pak, 1, 0);// does not have a task
            }
            
            // ----------
            // send the tokens
            
            if(sendingSelf)
            {
                int i;
                int n = eaSize(&e->supergroup->rewardTokens);
                pktSendBitsAuto(pak, n);
                
                for( i = 0; i < n; ++i ) 
                {
                    RewardToken *rewardtoken = e->supergroup->rewardTokens[i];
                    const char *rw = rewardtoken ? rewardtoken->reward : "";
                    int val = rewardtoken ? rewardtoken->val : 0;
                    int time = rewardtoken ? rewardtoken->timer : 0;
                    
                    pktSendString(pak,rw);
                    pktSendBitsPack(pak, 1, val);
                    pktSendBitsPack(pak, 1, time);
                }

                // Send the supergroup's recipes
                n = eaSize(&e->supergroup->invRecipes);
                pktSendBitsAuto(pak, n);
                for(i=0; i<n; i++)
                {
                    RecipeInvItem *pRecipe = e->supergroup->invRecipes[i];

                    // 1: 1 = Unlimited
                    //       str: Name
                    //    0 = Count
                    //       2: Count!=0
                    //           str: Name

                    if(pRecipe && pRecipe->recipe)
                    {
                        if(pRecipe->count < 0)
                            pktSendBitsPack(pak, 1, 1);
                        else
                        {
                            pktSendBitsPack(pak, 1, 0);
                            pktSendBitsPack(pak, 3, pRecipe->count);
                        }
                        pktSendString(pak, pRecipe->recipe->pchName);
                    }
                }

                // send special details
                n = eaSize(&e->supergroup->specialDetails);
                pktSendBitsAuto(pak, n);
                for(i=0; i<n; i++)
                {
                    SpecialDetail *pSpecial = e->supergroup->specialDetails[i];
                    if (pSpecial->pDetail)
                        pktSendString(pak, pSpecial->pDetail->pchName);
                    else 
                        pktSendString(pak, "");
                    pktSendBitsAuto(pak, pSpecial->creation_time);
                    pktSendBitsAuto(pak, pSpecial->expiration_time);
                    pktSendBitsAuto(pak, pSpecial->iFlags);
                }
            }
            else
            {
                pktSendBitsAuto(pak, 0);
                pktSendBitsAuto(pak, 0);
                pktSendBitsAuto(pak, 0);
            }
        }
        else
        {
            pktSendBitsPack(pak, 2, 0);
        }
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}



static void INLINEDBG entSendTaskforceParameters(TaskForce* tf, Packet* pak)
{
    U32 count;

    if (tf)
    {
        pktSendBitsAuto(pak, tf->parametersSet);
        tf->architect_useCachedCostume = 0;
        for (count = 0; count < MAX_TASKFORCE_PARAMETERS; count++)
        {
            if (tf->parametersSet & (1 << count))
            {
                // We store absolute time values but clients aren't
                // guaranteed to be in the same timezone as us or just
                // generally on the same timebase, so we have to convert
                // to a relative time and let them add back their own
                // timebase.
                if (count == TFPARAM_TIME_LIMIT - 1 ||
                    count == TFPARAM_START_TIME - 1)
                {
                    pktSendBitsAuto(pak, tf->parameters[count] -
                        timerSecondsSince2000());
                }
                else
                {
                    pktSendBitsAuto(pak, tf->parameters[count]);
                }
            }
        }
    }
}

static void INLINEDBG entSendTaskforceArchitectInfo(Entity *e, Packet* pak)
{
    if(TaskForceIsOnArchitect(e))
    {
        PlayerCreatedStoryArc *pArc = playerCreatedStoryArc_GetPlayerCreatedStoryArc(e->taskforce_id);
        pktSendBits(pak, 1, 1 );
        pktSendBitsAuto(pak, e->taskforce->architect_flags );
        pktSendBitsAuto(pak, e->taskforce->architect_id );
        pktSendBitsAuto(pak, e->taskforce->architect_contact_override );
        pktSendString(pak, e->taskforce->architect_contact_name );
        if ( e->taskforce->architect_contact_costume )
        {
            pktSendBits(pak,1,1);
            if( !e->taskforce->architect_useCachedCostume )
            {
                pktSendBits(pak, 1, 1 );
                costume_send( pak, e->taskforce->architect_contact_costume, 0 );
                e->taskforce->architect_useCachedCostume = 1;
            }
            else
                pktSendBits(pak, 1, 0 ); 
        }
        else
        {
            pktSendBits(pak,1,0);
        }
        if ( e->taskforce->architect_contact_npc_costume )
        {
            pktSendBits(pak,1,1);
            if ( !e->taskforce->architect_useCachedCostume )
            {
                pktSendBits(pak, 1, 1);
                NPCcostume_send(pak, e->taskforce->architect_contact_npc_costume);
                e->taskforce->architect_useCachedCostume = 1;
            }
            else
                pktSendBits(pak, 1, 0);
        }
        else
        {
            pktSendBits(pak,1,0);
        }
        if( pArc )
        {
            pktSendBits(pak, 1, 1 );
            pktSendString(pak, pArc->pchName);
            pktSendString(pak, pArc->pchAuthor);
            pktSendString(pak, pArc->pchDescription);
            pktSendBitsAuto(pak, pArc->rating );
            pktSendBitsAuto(pak, pArc->european);
            pktSendBitsAuto(pak, e->invincible);
            pktSendBitsAuto(pak, e->untargetable);
        }
        else
            pktSendBits(pak, 1, 0 ); // shouldn't happen
    }
    else
        pktSendBits(pak, 1, 0 );
}

static void appendTeammatePacket(Packet *pak, Entity *recvEnt, Entity *e)
{
    if (e == recvEnt ||
        (e->erOwner && erGetEnt(e->erOwner) == recvEnt) ||
        (recvEnt->teamup_id && recvEnt->teamup_id == e->teamup_id) ||
        (recvEnt->league_id && recvEnt->league_id == e->league_id))

    {
        pktSendBits(pak, 1, 1);

        if (e->pchar && (e->team_buff_full_update || eaiSize(&e->diffBuffs)))
        {
            pktSendBits(pak, 1, 1);
            // Send buff information to teammates and yourself
            // TODO: Only send these if they changed?  Is this a big deal? YES!
            character_SendBuffs(pak, recvEnt, e->pchar);
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}


// MS: This function is too long to __forceinline apparently, so I'll just make it faster.
static void sendEntity(int idx, Entity* e, int inPhase)
{
    int         ent_create=0;
    int            pos_update;
    int            qrot_update;
    int            move_update;
    int            logout_update;
    int            send_appearance;
    int            updated_powerCust;
    int            send_status_effects;
    int            team_update;
    int            levelingpact_update;
    int            draw_update;
    int            pvp_update;
    int            petinfo_update;
    int            collision_update;
    int            send_on_odd_send_update;
    int            send_afk_string;
    int            rare_bits;
    int            medium_rare_bits;
    int            very_rare_bits;
    int         send_modes;
    int            send_tf_params;
    int            send_architectinfo;
    int            interp_level=DEFAULT_CLIENT_INTERP_DATA_LEVEL, interp_bits=5 + DEFAULT_CLIENT_INTERP_DATA_PRECISION;
    int            is_generic_send=1; // Set this to false if any parameter changes because of the *receiving* entity
                                   // If this is true, we can cache the packet and re-use it later.
    int            need_to_send=1;
    Packet*        pak = NULL;
    int            bitcount=0; // for debugging
    int            supergroup_update = 0;
    int            send_pchar_things;
    int            sendingSelf = (e == send_ent.player_ent);

    if (!TSTB(need_to_send_cache, idx*2)) 
    {
        // Hasn't gone through determineNeedToSend (setNetParams)
        SETB(need_to_send_cache, idx*2);
        // calculate it!
        if (determineNeedToSend(idx))
            SETB(need_to_send_cache, idx*2+1);
    }

    assert(ENTINFO(e).net_prepared);

    //PERFINFO_AUTO_START("checks", 1) 
        pos_update = e->pos_update;
        qrot_update = e->qrot_update;
        move_update = e->move_update;
        logout_update = e->logout_update;
        send_afk_string = e->pl && e->pl->send_afk_string;
        supergroup_update = e->supergroup_update;
        team_update = e->team_update;
        levelingpact_update = (e->levelingpact_update && sendingSelf);
        draw_update = e->draw_update;
        collision_update = e->collision_update;
        pvp_update = e->pvp_update;
        petinfo_update = e->petinfo_update;
        send_on_odd_send_update = e->send_on_odd_send_update;
        send_status_effects = (sendingSelf && e->status_effects_update);

        // Clients only care about their own taskforce parameters, not
        // teammates (who will all have the same ones) or nearby people.
        if (sendingSelf && e->taskforce)
        {
            send_tf_params = e->tf_params_update;
            send_architectinfo = 1;
        } 
        else 
        {
            send_tf_params = 0;
            send_architectinfo = 0;
        }

        if (send_ent.full_update || !TSTB(send_ent.sent_buf->sent_ids,idx))
        {
            ent_create = 1;
            e->send_all_costume = 1;
            e->send_all_powerCust = 1;            //    we don't set e->updated_powerCust because it means that the power customization has to reset 
                                                //    but at this point, it doesn't need a reset, and will actually clear the fx
            SETB(send_ent.sent_buf->sent_ids,idx);
            pos_update = 7;
            qrot_update = 7;
            move_update = 1;
            logout_update = 1;
            need_to_send = 1;
            is_generic_send = 0;
            send_afk_string = 1;
            supergroup_update = 1;
            team_update = 1;
            levelingpact_update = 1;
            draw_update = 1;
            collision_update = 1;
            pvp_update = 1;
            petinfo_update = 1;
            send_on_odd_send_update = 1;

            if (sendingSelf && e->taskforce)
            {
                send_tf_params = 1;
                send_architectinfo = 1;
            }
        }

    //PERFINFO_AUTO_STOP();

    if (!need_to_send)
        return;

    // It has been determined we need to send the entity, send it!

    //printf("sending ent %d, type %d\n", e->owner, ENTTYPE(e));

     send_modes = e->state.mode != e->state.last_sent_mode || ent_create;
    send_appearance = e->send_costume;
    updated_powerCust = e->updated_powerCust || e->send_all_powerCust;
#if CONFUSED_COSTUMES
    send_appearance |= send_ent.player_ent->send_others_costumes;
#endif
    send_appearance |= ent_create | send_ent.full_update;

    if(!send_ent.is_generic_pos_update)
    {
        is_generic_send = 0;
    }

    if(    sendingSelf ||
        (e->erOwner && erGetEnt(e->erOwner) == send_ent.player_ent))
    {
        // When sending an entity to oneself, lots of things are different
        is_generic_send = 0;
    }

    if(    send_ent.destpak->compress ||
        (    e->send_packet &&
            e->send_packet->hasDebugInfo != send_ent.destpak->hasDebugInfo) ||
        send_ent.destpak->hasDebugInfo != pktGetDebugInfo())
    {
        is_generic_send = 0;
    }

    // Debug stuff to figure out how often caching of entity packets speeds things up - turns out to be quite a bit!
    // cacheDebugCounting(pak, e, is_generic_send);

    pktSendBitsPack(send_ent.destpak, 1, idx - send_ent.last_idx_sent - 1); // Calls:  1358714 Zeroes:   650589 Avg:  2.61 Anz:  5.00 numbits:  1
    send_ent.last_idx_sent = idx;

    //is_generic_send = 0; // disables use of pktAppend, generates all packets "naturally" (useful for debugging)

    if (is_generic_send) {
        PERFINFO_AUTO_START("genericSend", 1);
        if (e->cached_send_packet) {
            // simply append and return!
            PERFINFO_AUTO_START("pktAppend", 1);
                pktAppend(send_ent.destpak, e->send_packet);
            PERFINFO_AUTO_STOP();

            appendTeammatePacket(send_ent.destpak, send_ent.player_ent, e);

            PERFINFO_AUTO_STOP(); // For "genericSend".
            //assert(!e->cached_send_packet || e->send_packet);
            return;
        } else {
            // Needs to be generated!
            assert(!e->send_packet);
            pak = e->send_packet = pktCreate();
//            pktSetCompression(pak, DEFAULT_COMPRESS_ENT_PACKET);
        }
    } else {
        pak = send_ent.destpak;
        PERFINFO_AUTO_START("notGenericSend", 1);
    }


    if(e->resend_hp)
        e->lastticksent = global_state.global_frame_count;

    pktSendBits(pak, 1, send_ent.full_update || ent_create);
    if(send_ent.full_update | ent_create)
    {
        PERFINFO_AUTO_START("entSendCreate", 1);
            entSendCreate(pak, e, send_ent.player_ent, inPhase);
        PERFINFO_AUTO_STOP();
    }
/*
// Macros for counting the bitrate of each stream
// Note you also need to set is_generic_send=0 above in order for these to be accurate
#define COUNT_BITS \
    PERFINFO_AUTO_START("BitCount", pak->stream.bitLength - bitcount); \
    bitcount = pak->stream.bitLength;    \
    PERFINFO_AUTO_STOP();

#define COUNT(var) \
    PERFINFO_AUTO_START(#var,1);        \
    if (var) {                            \
    PERFINFO_AUTO_START("YES",1);    \
    COUNT_BITS;                            \
    } else {                            \
    PERFINFO_AUTO_START("NO",1);    \
    COUNT_BITS;                            \
    }                                    \
    PERFINFO_AUTO_STOP();                \
    PERFINFO_AUTO_STOP();
/* */
#define COUNT_BITS
#define COUNT(x)
/* */
    bitcount = pak->stream.bitLength;

    // Place rarely used bits here!

    very_rare_bits =    updated_powerCust ||                        
                        logout_update ||
                        supergroup_update ||
                        send_afk_string ||
                        team_update ||
                        e->tf_params_update ||
                        levelingpact_update ||
                        draw_update ||
                        collision_update ||
                        send_on_odd_send_update ||
                        pvp_update ||
                        petinfo_update ||
                        e->helper_status_update ||
                        send_architectinfo ||
                        e->name_update;

    medium_rare_bits =    send_modes ||
                        e->triggered_move_count ||
                        send_appearance ||
                        e->send_xlucency;

    rare_bits =    move_update ||
                medium_rare_bits ||
                very_rare_bits;

    pktSendBits(pak, 1, rare_bits);
    COUNT(rare_bits);

    if(rare_bits)
    {
        pktSendBits(pak, 1, medium_rare_bits);
        pktSendBits(pak, 1, very_rare_bits);
    }

    if(medium_rare_bits)
    {
        entSendModes(pak, e, send_modes);
        COUNT(send_modes); // 99% = NO
    }

    entSendPosUpdate(pak, e, pos_update, ent_create, send_ent.is_generic_pos_update);
    COUNT(pos_update); // 93% = YES

    entSendQrotUpdate(pak, e, qrot_update);
    COUNT(qrot_update); // 80% == NO


    if(rare_bits)
    {
        entSendSeqMoveUpdate(pak, e, move_update);
        COUNT(move_update); // 96% = NO
    }

    if(medium_rare_bits)
    {
        entSendSeqTriggeredMoves(pak, e );
        COUNT(e->triggered_move_count); // 99% = NO

        entSendAiAnimList( pak, e, send_appearance );
    }

    send_pchar_things = e->netfx_count || e->pchar;

    pktSendBits(pak, 1, send_pchar_things);

    pktSendBits(pak, 1, inPhase);
    if(inPhase && send_pchar_things)
    {
        entSendNetFx(pak, e, ent_create || send_ent.full_update, send_ent.player_ent /*debug only*/);
        COUNT(e->netfx_count);
    }

    if (medium_rare_bits)
    {
        entSendCostume(pak, e, send_ent.player_ent, send_appearance);

        {
            int xlucency_eff = CLAMP((e->xlucency*255), 0, 255);
            pktSendIfSetBits(pak, 8, 255-xlucency_eff);
        }

        COUNT(send_appearance); // 95% = NO (only on creates and rare circumstances)
    }

    if (very_rare_bits) {
        entSendPowerCust(pak, e, send_ent.player_ent);

        if(e->pl)
        {
            pktSendBits(pak,1,1);
            pktSendString(pak, e->name);
             pktSendBitsPack(pak,5,e->pl->titleThe);
            pktSendIfSetString(pak, e->pl->titleTheText);
            pktSendIfSetString(pak, e->pl->titleCommon);
            pktSendIfSetString(pak, e->pl->titleOrigin);
            pktSendBitsAuto(pak, e->pl->titleColors[0]);
            pktSendBitsAuto(pak, e->pl->titleColors[1]);
            pktSendBits(pak, 32, e->pl->titleBadge);
            pktSendIfSetString(pak, e->pl->titleSpecial);
            pktSendBits(pak, 1, e->pl->glowieUnlocked);
            pktSendBits(pak, 2, e->gender);
            pktSendBits(pak, 2, e->name_gender);
            pktSendBits(pak, 1, e->pl->attachArticle);
            pktSendBits(pak, 32, e->pl->respecTokens);
            if (db_state.is_endgame_raid)
            {
                pktSendBits(pak, 32, 0);
                pktSendBits(pak, 32, 0);
            }
            else
            {
                pktSendBits(pak, 32,e->pl->lastTurnstileEventID);
                pktSendBits(pak, 32,e->pl->lastTurnstileMissionID);
            }
            pktSendBitsAuto(pak, e->pl->helperStatus);
        }
        else
        {
            pktSendBits(pak,1,0);
        }
    }

#ifdef RAGDOLL
    // Note that this has to happen after the costume is sent, since the client
    // needs the costume info to make the ragdoll
    entSendRagdollUpdate(pak, e, (e->ragdoll) ? 1 : 0);
    COUNT(e->ragdoll ? 1 : 0); // all or none for now
#else
    entSendRagdollUpdate(pak, e, 0);
#endif


    if(send_pchar_things)
    {
        PERFINFO_AUTO_START("entSendCharacterStats", 1);
            entSendCharacterStats(pak, e, send_ent.player_ent, send_ent.full_update||ent_create||e->level_update);

        PERFINFO_AUTO_STOP();
        COUNT((NULL != e->pchar)); // 100% = YES (will change when pchars are removed from NPCs)

        
    }

    COUNT(e == player_ent && "friendslist");

    pktSendBitsAuto(pak, e->bitfieldVisionPhases);
    pktSendBitsAuto(pak, e->exclusiveVisionPhase);

    pktSendBitsAuto(pak, e->shouldIConYellow);

    if(send_pchar_things)
    {
        if (e->pchar)
        {
            pktSendBitsAuto(pak, e->pchar->iCombatModShiftToSendToClient);
        }
        else
        {
            pktSendBitsAuto(pak, 0);
        }

        // Send target updates
        entSendTargetUpdate(pak, e, e->target_update || send_ent.full_update || ent_create);
        COUNT(e->target_update); // 10% = YES

        pktSendBits(pak, 1, send_status_effects);
        if (send_status_effects)
        {
            character_SendStatusEffects(pak, e->pchar);
        }    
        if ( sendingSelf)
            character_SendInventorySizes(e->pchar, pak);
    }


    if(very_rare_bits)
    {
        if ( sendingSelf)
        {
            pktSendBits(pak, 1, send_architectinfo);
            if(send_architectinfo)
                entSendTaskforceArchitectInfo(e,pak);

            pktSendBitsPack(pak, 1, TaskForceIsOnFlashback(e));
            pktSendBits(pak, 1, send_tf_params);
            if(send_tf_params)
                entSendTaskforceParameters(e->taskforce, pak);
        }

        pktSendBits(pak, 1, ENTINFO(e).send_on_odd_send);

        pktSendBitsPack(pak, 2, e->pchar ? e->pchar->iAllyID : 0);
        pktSendBitsPack(pak, 4, e->pchar ? e->pchar->iGangID : 0);
        pktSendBitsPack(pak, 1, e->pchar ? e->pchar->bIgnoreCombatMods : 0);
        if (e->pchar && e->pchar->bIgnoreCombatMods)
        {
            // DGNOTE 8/2/2010 - Yes, it is my intent to offset this by 8.  Without this, -1 would take 32 bits to send.
            // Doing the offset thing allows anything down to and including -8 to go as a positive number, taking far fewer bits.
            // Look for a DGNOTE with the same date stamp in entrecv.c for where this is handled when receiving.
            pktSendBitsAuto(pak, e->pchar->ignoreCombatModLevelShift + 8);
        }

        pktSendBits(pak, 1, SAFE_MEMBER(e->pl, playerType)); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))
        pktSendBits(pak, 2, SAFE_MEMBER(e->pl, playerSubType)); STATIC_INFUNC_ASSERT(kPlayerSubType_Count <= (1<<2))
        pktSendBits(pak, 3, SAFE_MEMBER(e->pl, praetorianProgress)); STATIC_INFUNC_ASSERT(kPraetorianProgress_Count <= (1<<3))
        pktSendBits(pak, 1, SAFE_MEMBER(e->pchar, playerTypeByLocation)); STATIC_INFUNC_ASSERT(kPlayerType_Count <= (1<<1))

        // PvP-related stuff, shouldn't change very often
        pktSendBits(pak, 1, (e->pl && e->pl->pvpSwitch));
        pktSendBits(pak, 1, (e->pchar && e->pchar->bPvPActive));
        pktSendBitsPack(pak, 5, (int)(e->pvpZoneTimer));
        pktSendBits(pak, 1, (e->pl && e->pl->bIsGanker));

        pktSendBits(pak, 1, e->motion->input.no_ent_collision);

        pktSendBits(pak, 1, e->notSelectable);

        // Send the bit for whether the entity should draw on the client.
        pktSendBits(pak, 1, e->noDrawOnClient);
        COUNT(e->noDrawOnClient); // 00% = NO

        // Send the bit for whether the entity should appear on the minimap
        pktSendBits(pak, 1, e->showOnMap);
        COUNT(e->showOnMap);

        // bit to let client know if its a contact
        pktSendBits(pak, 1, e->contactOrPnpc);
        pktSendBits(pak, 1, e->contactArchitect);
        COUNT(e->contactOrPnpc); // 00% = NO

        // always show conning info
        pktSendBits(pak, 1, e->alwaysCon);
        COUNT(e->alwaysCon); // 00% = NO

        // see around corners
        pktSendBits(pak, 1, e->seeThroughWalls);
        COUNT(e->seeThroughWalls); // 00% = NO

        if( e->name_update && e->name )
        {
            pktSendBits(pak,1,1);
            pktSendString( pak, e->name );
        }
        else
            pktSendBits(pak,1,0);

        // send all pet-related info
        entSendPetInfo(pak, e);

        entSendAfk(pak, e, send_afk_string);
        COUNT(send_afk_string); // 100% = NO

        entSendOtherSupergroupInfo(pak, e, supergroup_update, sendingSelf);
        COUNT(supergroup_update); // 100% = NO

        entSendLevelingpactInfo(pak, e, levelingpact_update);
        
        COUNT(levelingpact_update);

        // Send logout information.
        entSendLogoutUpdate(pak, e, logout_update);
        COUNT(logout_update); // 95% = NO (only on creates and rare circumstances)
    }

    PERFINFO_AUTO_STOP(); // For "genericSend" or "notGenericSend".

    if (is_generic_send) {
        //PERFINFO_AUTO_START("finalPktAppend", 1);
        assert(!e->cached_send_packet);
        pktAppend(send_ent.destpak, e->send_packet);
        e->cached_send_packet = 1;
        //PERFINFO_AUTO_STOP();
    }

    appendTeammatePacket(send_ent.destpak, send_ent.player_ent, e);
    return;
}

static void sendSkyFade(Packet *pak)
{
    pktSendBits(pak, 1, server_state.skyFadeInteresting);
    if (server_state.skyFadeInteresting) {
        pktSendF32(pak, server_state.skyFadeWeight);
        pktSendBitsPack(pak, 2, server_state.skyFade1);
        pktSendBitsPack(pak, 2, server_state.skyFade2);
    }
}

static int __cdecl compareEntityDistance(const int* ent1, const int* ent2)
{
    int d1 = ent1[1];
    int d2 = ent2[1];

    if(d1 < d2)
        return -1;
    else if(d1 > d2)
        return 1;
    else
        return 0;
}

void entSendUpdate(NetLink *link,Entity *player_ent,int re_predict,int full_update,int tick_number)
{
    static U32*    sent_ents_bits; // sent_ent_bits is the bitfield describing what entities are in range of the player, and should probably be sent
    static U32    max_sent_ents;
    static U32*    teammate_bits; // bitfield describing what entities are on the same team as the receivnig player
    static U32    max_teammate_ents;
    static U32*    leaguemate_bits;
    static U32    max_leaguemate_ents;
    static int    player_has_teammates=1; // Whether this player has teammates, also used to check if the *last* player had any teammates (and then we need to clear)
    static int    player_has_leaguemates = 1;

    int         j,i,k,count;
    Packet*        pak;
    Entity*        e;
    Vec3        plr_pos;
    int            vis_ents[MAX_ENTITIES_PRIVATE][2];
    int            sent_ent_count = 0;
    ClientLink* client = (ClientLink*)link->userData;

    int            ent_word_count;
    int            entDebugInfo = client->entDebugInfo;
    int            debugMenuFlags = client->debugMenuFlags;
    int            selectedEntId = client->controls.selected_ent_server_index;
    Packet*        player_pak;
    int            outside_clamp_count;
    F32            max_radius_SQR;
    int            inPhase;

    ent_word_count = ((entities_max + sizeof(sent_ents_bits[0]) * 8 - 1)/(sizeof(sent_ents_bits[0])*8));
    dynArrayFitStructs(&sent_ents_bits,&max_sent_ents,ent_word_count);
    dynArrayFitStructs(&teammate_bits,&max_teammate_ents,ent_word_count);
    dynArrayFitStructs(&leaguemate_bits,&max_leaguemate_ents,ent_word_count);
    ZeroStructs(sent_ents_bits,ent_word_count);
    if (player_has_teammates) 
    {
        ZeroStructs(teammate_bits,ent_word_count);
        player_has_teammates=0;
    }
    if (player_has_leaguemates) 
    {
        ZeroStructs(leaguemate_bits,ent_word_count);
        player_has_leaguemates=0;
    }

    if (!player_ent)
        return;

    if( server_state.viewCutScene )
        copyVec3(server_state.cutSceneCameraMat[3],plr_pos);
    else
        copyVec3(ENTPOS(player_ent),plr_pos);

    PERFINFO_AUTO_START("mgGetNodesInRange", 1);

        count = mgGetNodesInRangeWithSize(0, plr_pos, (void**)vis_ents, sizeof(vis_ents[0]));

    PERFINFO_AUTO_STOP();

    if(!client->ent_send.is_sorted)
    {
        int realCount = 0;

        PERFINFO_AUTO_START("sort entities", 1);

            client->ent_send.is_sorted = 1;

            for(i = 0; i < count; i++)
            {
                int distSQR;
                int idx = vis_ents[i][0];

                e = entities[idx];

                distSQR = distance3Squared(ENTPOS_BY_ID(idx), plr_pos);

                // Check if the entity is in send range.

                if(distSQR < SQR(ENTFADE_BY_ID(idx))
                    && entity_TargetIsInVisionPhase(player_ent, e)
                        || (player_ent->teamup_id && player_ent->teamup_id == e->teamup_id)
                        || (player_ent->league_id && player_ent->league_id == e->league_id))
                {
                    vis_ents[realCount][0] = idx;
                    if (s_ignoreDistancePrioritization(e))
                        vis_ents[realCount][1] = 0;                        //    set to 0 so that it becomes first on the list of entities to send
                    else
                        vis_ents[realCount][1] = distSQR;
                    realCount++;
                }
            }

            PERFINFO_AUTO_START("qsort", 1);
                qsort(vis_ents, realCount, sizeof(vis_ents[0]), compareEntityDistance);
            PERFINFO_AUTO_STOP();

            //count = min(realCount, 100);

            if(realCount >= MAX_ENTITIES_SENT_TO_A_CLIENT )
            {
                client->ent_send.max_radius_SQR = vis_ents[MAX_ENTITIES_SENT_TO_A_CLIENT-1][1];
            }
            else
            {
                client->ent_send.max_radius_SQR = SQR(100000.0);
            }

        PERFINFO_AUTO_STOP();
    }

    max_radius_SQR = client->ent_send.max_radius_SQR;

    PERFINFO_AUTO_START("entSendUpdateTop", 1);

        for(i=0;i<count;i++)
        {
            SETB(sent_ents_bits,vis_ents[i][0]);
        }

        if(debugMenuFlags == -1)
        {
            int entCount[ENTTYPE_COUNT];
            int entCountSent[ENTTYPE_COUNT];

            ZeroArray(entCount);
            ZeroArray(entCountSent);

            client->debugMenuFlags = 0;
            debugMenuFlags = 0;

            printf("%d ents ------------------ \n", count);

            for(i=0;i<count;i++)
            {
                int idx = (int)(intptr_t)vis_ents[i];
                Entity* e = validEntFromId(idx);

                if(e)
                {
                    int sendMe = distance3Squared(ENTPOS_BY_ID(idx), ENTPOS(player_ent)) <= SQR(ENTFADE_BY_ID(idx));
                    F32 dmin = min(fabs(ENTPOS_BY_ID(idx)[vecXidx] - ENTPOSX(player_ent)),
                        fabs(ENTPOS_BY_ID(idx)[vecYidx] - ENTPOSY(player_ent)));
                    F32 dmax = max(fabs(ENTPOS_BY_ID(idx)[vecXidx] - ENTPOSX(player_ent)),
                        fabs(ENTPOS_BY_ID(idx)[vecYidx] - ENTPOSY(player_ent)));

                    dmin = min(dmin, fabs(ENTPOS_BY_ID(idx)[vecZidx] - ENTPOSZ(player_ent)));
                    dmax = max(dmax, fabs(ENTPOS_BY_ID(idx)[vecZidx] - ENTPOSZ(player_ent)));

                    entCount[ENTTYPE_BY_ID(idx)]++;

                    if(sendMe)
                        entCountSent[ENTTYPE_BY_ID(idx)]++;

                    printf(    "%s\t%1.f\t%1.f\t%1.f\t%s\n",
                            sendMe ? "IN" : "",
                            dmin,
                            dmax,
                            e->seq->type->fade[1],
                            e->seq->type->name);
                }
            }

            printf(    "\n"
                    "players: \t%d\t%d\n"
                    "critters:\t%d\t%d\n"
                    "cars:    \t%d\t%d\n"
                    "doors:   \t%d\t%d\n"
                    "npcs:    \t%d\t%d\n",
                    entCount[ENTTYPE_PLAYER],    entCountSent[ENTTYPE_PLAYER],
                    entCount[ENTTYPE_CRITTER],    entCountSent[ENTTYPE_CRITTER],
                    entCount[ENTTYPE_CAR],        entCountSent[ENTTYPE_CAR],
                    entCount[ENTTYPE_DOOR],        entCountSent[ENTTYPE_DOOR],
                    entCount[ENTTYPE_NPC],        entCountSent[ENTTYPE_NPC]);
        }

        // JS:    Both contact status and contact visit location appends a seperate
        //        sub packet into the packet attached to the entity.
        //
        //        These *must* be sent before compression is determined for main packet.
        PERFINFO_AUTO_START("send storyarc status", 1);
            ContactStatusSendChanged(player_ent);
            // this means that the accessible lists aren't always up to date on the client!
            //ContactStatusAccessibleSendAll(player_ent);
            ContactVisitLocationSendChanged(player_ent);
            ClueSendChanged(player_ent);
            KeyClueSendChanged(player_ent);
            scSendChanged(player_ent);
            if (eaSize(&player_ent->scriptUIData))
            {
                ScriptUISendUpdate(player_ent);
            }
            if ((player_ent->access_level > 1) && server_state.net_send.is_odd_send)
            {
                CharacterKarmaStatsSend(player_ent, g_currentKarmaTime);
            }
        PERFINFO_AUTO_STOP();

        // send incremental kiosk updates as needed
        PERFINFO_AUTO_START("send kiosk updates", 1);
            ArenaKioskSendIncremental(player_ent);
        PERFINFO_AUTO_STOP();

        // if there's a pending reward choice, resend in case choices have changed
        PERFINFO_AUTO_START("send pending reward", 1);
            sendPendingReward(player_ent);
        PERFINFO_AUTO_STOP();

        // Send future pushes.
        
        send_ent.sent_buf = client->entNetLink;
        pak = pktCreate();
        player_pak = player_ent->entityPacket;
        if (player_pak)
        {
            // If the packet already has compression set or not set, we have to use it in
            // order to append later!!!
            pak->hasDebugInfo = player_pak->hasDebugInfo;

            pktSetCompression(pak, player_pak->compress);
        }
        else
        {
            pktSetCompression(pak, DEFAULT_COMPRESS_ENT_PACKET(player_ent));
        }

        send_ent.reliable = 0;

        // Update type / scene file
        if(full_update)
        {
            // Make sure map/scene changes get through.
            send_ent.reliable = 1;
            pktSendBitsPack(pak,1,SERVER_ALLENTS);
            
            client->lastSentFullUpdateID++;
            
            pktSendBitsPack(pak, 1, client->lastSentFullUpdateID);
        }
        else
        {
            pktSendBitsPack(pak,1,SERVER_UPDATE);
        }

        if(client->lastSentFullUpdateID != client->ackedFullUpdateID){
            pktSetOrdered(pak, 1);
        }
        
        // Send a bit to indicate whether this packet includes odd-send entities.

        if (server_state.net_send.is_odd_send || full_update)
        {
            pktSendBits(pak,1,1); // does include odd-send entities
        }
        else
        {
            pktSendBits(pak,1,0); // doesn't include odd-send entities
        }

        send_ent.reliable |= cmdOldServerSend(pak,full_update);
        pktSendBits(pak,32,ABS_TIME); // Frame time
        pktSendBits(pak,32,dbSecondsSince2000());    // absolute server time
        if (full_update)
            send_ent.sent_buf->namecount_sent = 0;

        {
			if(!(player_ent->access_level || (client->entDebugInfo == ENTDEBUGINFO_RUNNERDEBUG)))
            {
                pktSendBits(pak, 1, 1);
            }
            else
            {
                int hasDebugInfo = entDebugInfo || debugMenuFlags & DEBUGMENUFLAGS_QUICKMENU;
                int isDefaultInterp =    client->interpDataLevel == DEFAULT_CLIENT_INTERP_DATA_LEVEL &&
                                        client->interpDataPrecision == DEFAULT_CLIENT_INTERP_DATA_PRECISION;

                if(!hasDebugInfo && isDefaultInterp)
                {
                    pktSendBits(pak, 1, 1);
                }
                else
                {
                    pktSendBits(pak, 1, 0);

                    // Send a bit indicating if there is entDebugInfo in the packet.

                    pktSendBits(pak, 1, hasDebugInfo ? 1 : 0);

                    // Send a bit indicating if there is interp data in the packet.

                    pktSendBits(pak, 1, client->interpDataLevel ? 1 : 0);

                    if(client->interpDataLevel)
                    {
                        client->interpDataLevel = max(0, min(3, client->interpDataLevel));
                        client->interpDataPrecision = max(0, min(3, client->interpDataPrecision));

                        pktSendBits(pak, 2, client->interpDataLevel);
                        pktSendBits(pak, 2, client->interpDataPrecision);
                    }
                }
            }
        }
    PERFINFO_AUTO_STOP();

    // Add all pets into the bit array as wanting to get sent
    if( player_ent->teamup )
    {
        int i;
        Entity *e;
        int memberCount = player_ent->teamup->members.count;
        int* ids = player_ent->teamup->members.ids;

        PERFINFO_AUTO_START("flagTeamates", 1);
            for( i = 0; i < memberCount; i++ )
            {
                e = entFromDbId(ids[i]);
                if( e )
                {
                    SETB(sent_ents_bits,e->owner);
                    SETB(teammate_bits,e->owner);
                    player_has_teammates = 1;
                }
            }
        PERFINFO_AUTO_STOP();
    }

    if( player_ent->pl && player_ent->petList )
    {
        int i;
        Entity *e;
        int count = player_ent->petList_size;

        PERFINFO_AUTO_START("flagPets", 1);
        for( i = 0; i < count; i++ )
        {
             e = erGetEnt(player_ent->petList[i]);
            if( e )
            {
                SETB(sent_ents_bits,e->owner);
                SETB(teammate_bits,e->owner);
                player_has_teammates = 1;
            }
        }
        PERFINFO_AUTO_STOP();
    }

    // Add all leaguemates into the bit array as wanting to get sent
    if( player_ent->league )
    {
        int i;
        Entity *e;
        int memberCount = player_ent->league->members.count;
        int* ids = player_ent->league->members.ids;

        PERFINFO_AUTO_START("flagLeaguemates", 1);
        for( i = 0; i < memberCount; i++ )
        {
            e = entFromDbId(ids[i]);
            if( e )
            {
                SETB(sent_ents_bits,e->owner);
                SETB(leaguemate_bits,e->owner);
                player_has_leaguemates = 1;
            }
        }
        PERFINFO_AUTO_STOP();
    }

    // Send all the entities that are visible and not hidden (unless I'm in entity-debugging mode).

    PERFINFO_AUTO_START("for all visible", 1);

    send_ent.client = client;

    send_ent.interp_level = DEFAULT_CLIENT_INTERP_DATA_LEVEL;
    send_ent.interp_bits = 5 + DEFAULT_CLIENT_INTERP_DATA_PRECISION;

    if(    send_ent.interp_level != send_ent.client->interpDataLevel ||
        send_ent.interp_bits != 5+ send_ent.client->interpDataPrecision)
    {
        send_ent.interp_level = send_ent.client->interpDataLevel;
        send_ent.interp_bits = 5 + send_ent.client->interpDataPrecision;
        send_ent.is_generic_pos_update = 0;
    }
    else
    {
        send_ent.is_generic_pos_update = 1;
    }

    send_ent.destpak = pak;
    send_ent.last_idx_sent = -1;
    send_ent.player_ent = player_ent;
    send_ent.full_update = full_update;

    outside_clamp_count = 0;

    for(k=0;k<ent_word_count;k++)
    {
        U32        *base;

        if (!sent_ents_bits[k])
            continue;
        base = &sent_ents_bits[k];
        for(j=0;j<32;j++)
        {
            if (!TSTB(base,j))
                continue;

            i = k*32 + j;

            if (!sendThisFrame(i) && !full_update)
                continue;

            //PERFINFO_AUTO_START("cached bits", 1);
            if (!full_update && TSTB(send_ent.sent_buf->sent_ids, i)) {
                // We can rely on the cached info
                if (!TSTB(need_to_send_cache, i*2)) {
                    // Has not yet been calculated
                    SETB(need_to_send_cache, i*2);
                    // calculate it!
                    if (determineNeedToSend(i)) {
                        SETB(need_to_send_cache, i*2+1);
                    } // else it's already cleared in entPrepareUpdate
                }
                if (!TSTB(need_to_send_cache, i*2+1)) {
                    // We do not need to send this (nothing changed, and the client already got a create)
                    //PERFINFO_AUTO_STOP();

                    //printf("not sending because of cache: ent %d, type %d\n", e->owner, ENTTYPE(e));

                    // Check to see if the entity is either the player or on the same team as the player,
                    //  if so then we need to check additional flag(s)
                    if(    player_has_teammates && TSTB(teammate_bits,i) ||
                        player_has_leaguemates && TSTB(leaguemate_bits,i) ||
                        player_ent->owner == i)
                    {
                        // It's a teammate, don't skipt out on sending them if these flag(s) are set
                        if (!(entities[i]->team_buff_full_update || eaiSize(&entities[i]->diffBuffs))) {
                            continue;
                        } else {
                            // A team-specific flag changed
                        }
                    } else {
                        continue;
                    }
                }
            }
            //PERFINFO_AUTO_STOP();
            e = entities[i];

            //printf("entity %d : type %d\n", j, ENTTYPE(e));

            //PERFINFO_AUTO_START("Dereference", 1);
            assert(e->owner);
            //PERFINFO_AUTO_STOP();

            inPhase = entity_TargetIsInVisionPhase(player_ent, e);

            //PERFINFO_AUTO_START("VecStuff", 1);

            // Don't care about distance or phase if they're a teammate or a pet or a league mate
            if ((!player_ent->teamup_id || player_ent->teamup_id != e->teamup_id)
                && (!player_ent->league_id || player_ent->league_id != e->league_id)
                && (!e->erOwner || player_ent != erGetEnt(e->erOwner)))
            {
                F32 distSQR = distance3Squared(plr_pos,ENTPOS_BY_ID(i));

                if( distSQR > SQR(ENTFADE_BY_ID(i)) || ENTHIDE_BY_ID(i) )
                {
                    CLRB(base,j);
                    continue;
                }
                else if(ENTTYPE_BY_ID(i) != ENTTYPE_CAR &&
                    ENTTYPE_BY_ID(i) != ENTTYPE_DOOR &&
                    !s_ignoreDistancePrioritization(e) &&
                    distSQR > max_radius_SQR)
                {
                    // Inside my vis radius, but outside the current max.

                    vis_ents[outside_clamp_count][0] = i;
                    vis_ents[outside_clamp_count][1] = distSQR;

                    outside_clamp_count++;

                    CLRB(base,j);
                    continue;
                }

                if (!inPhase)
                {
                    CLRB(base,j);
                    continue;
                }
            }
            //PERFINFO_AUTO_STOP();

            //if(!ENTPAUSED_BY_ID(i))
            //{
            //    AI_POINTLOG(AI_LOG_THINK, (e, posPoint(e), 0xff0000ff, "\nSent: %1.2f", server_state.net_send.acc));
            //}

#define MAKE_CASE(x) case ENTTYPE_##x: PERFINFO_AUTO_START("send:" #x, 1);break

            PERFINFO_RUN(
                if(e->send_packet && e != player_ent)
                {
                    PERFINFO_AUTO_START("prepped", 1);
                }
                else
                {
                    PERFINFO_AUTO_START("not prepped", 1);
                }

                if(e == player_ent)
                {
                    PERFINFO_AUTO_START("send:ME", 1);
                }
                else
                {
                    switch(ENTTYPE_BY_ID(i))
                    {
                        MAKE_CASE(PLAYER);
                        MAKE_CASE(CRITTER);
                        MAKE_CASE(CAR);
                        MAKE_CASE(NPC);
                        MAKE_CASE(DOOR);
                    default:
                        PERFINFO_AUTO_START("send:Other", 1);
                        break;
                    }
                }
                );
                {
                    sendEntity(i, e, inPhase);

                    if(    ENTTYPE_BY_ID(i) != ENTTYPE_CAR &&
                        ENTTYPE_BY_ID(i) != ENTTYPE_DOOR)
                    {
                        sent_ent_count++;
                    }
                }
                PERFINFO_RUN(
                    PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();
                );
        }
    }
    PERFINFO_AUTO_STOP();

    if(debugMenuFlags & DEBUGMENUFLAGS_PLAYERLIST)
    {
        // Send all the entities to people who want all that info.
        int i;
        for(i = 1; i < player_ents[PLAYER_ENTS_ACTIVE].count; i++)
        {
            Entity* e = player_ents[PLAYER_ENTS_ACTIVE].ents[i];
            int idx = e->owner;

            if(    entity_state[e->owner] & ENTITY_IN_USE &&
                !TSTB(sent_ents_bits, idx))
            {
                if(character_IsInitialized(e->pchar))
                {
                    SETB(sent_ents_bits,e->owner);

                    PERFINFO_AUTO_START("sendEntity:players", 1);
                    sendEntity(e->owner, e, entity_TargetIsInVisionPhase(player_ent, e));

                    sent_ent_count++;
                    PERFINFO_AUTO_STOP();
                }
                else
                {
                    //JE: I think I tracked down this bug, if this happens again, well, remove the
                    //    assert and we've got more tracking to do!
                    assert(!"trying to send uninitialized entity!");
                    Errorf("Character (name:%s, dbid: %d) isn't initialized but we tried to send it!", e->name?e->name:"(null)", e->db_id);
                }
            }
        }
    }

    // Adjust radius.

    if(server_state.net_send.is_odd_send)
    {
/* Overloaded variable - Condition removed since 256 is now the EnhanceAndInspire flag - VJD
        if(client->debugMenuFlags & 256)
        {
            printf("sent ents: %d @\t%1.fft\n", sent_ent_count, sqrt(max_radius_SQR));
        }
*/
        if(max_radius_SQR < SQR(MIN_PLAYER_CRITTER_RADIUS) && sent_ent_count < MAX_ENTITIES_SENT_TO_A_CLIENT)
        {
            client->ent_send.is_sorted = 0;
            //max_radius_SQR = sqrt(max_radius_SQR);
            //max_radius_SQR += 50;
            //max_radius_SQR = SQR(max_radius_SQR);
        }
    }

    // Send my selected entity if I have debug info turned on.

    if(    entDebugInfo &&
        selectedEntId > 0 &&
        selectedEntId < entities_max &&
        entities[selectedEntId] &&
        entities[selectedEntId]->owner > 0)
    {
        e = entities[selectedEntId];
        if(!TSTB(sent_ents_bits,e->owner))
        {
            SETB(sent_ents_bits,e->owner);
            PERFINFO_AUTO_START("sendEntity:selected", 1);
                sent_ent_count++;

                sendEntity(e->owner, e, entity_TargetIsInVisionPhase(player_ent, e));
            PERFINFO_AUTO_STOP();
        }
    }


    if(!TSTB(sent_ents_bits,player_ent->owner))
    {
        // Always send the player entity, otherwise RESUME will never work.
        e = player_ent;
        SETB(sent_ents_bits,e->owner);
        PERFINFO_AUTO_START("sendEntity:self", 1);
            sent_ent_count++;

            sendEntity(e->owner, e, 1); // always in phase with self
        PERFINFO_AUTO_STOP();
    }

    if(sent_ent_count >= MAX_ENTITIES_SENT_TO_A_CLIENT)
    {
        client->ent_send.is_sorted = 0;
    }

    SET_ENTINFO(player_ent).sent_self = 1;

    // send end of ent list message
    pktSendBitsPack(pak,1,0);
    pktSendBits(pak,1,1); // full update bit
    pktSendBits(pak,1,1); // end of list bit

    if(    entDebugInfo ||
        debugMenuFlags & DEBUGMENUFLAGS_QUICKMENU)
    {
        Entity* selected_ent = validEntFromId(client->controls.selected_ent_server_index);

        // Send entity debug info.

        if(selected_ent){
            pktSendBitsPack(pak, 10, selected_ent->owner);
            aiGetEntDebugInfo(selected_ent, client, pak);
        }

        // Send myself.

        pktSendBitsPack(pak, 10, player_ent->owner);
        aiGetEntDebugInfo(player_ent, client, pak);

        // Send end of list.

        pktSendBitsPack(pak, 10, 0);

        // Send global debug lines and texts.

        aiSendGlobalEntDebugInfo(client, pak);
    }

    PERFINFO_AUTO_START("entSendUpdateBottom", 1);

        // Send the control state.

        send_ent.reliable |= svrPlayerSendControlState(pak,player_ent,link,full_update | re_predict);

        send_ent.reliable |= entSendDeletes(pak,send_ent.sent_buf,sent_ents_bits);

        if(full_update)
        {
            sendCharacterToClient(pak, player_ent);
        } else {
            // Send all stats in full to the client.
            sdPackDiff(SendCharacterToSelf, pak, player_ent->pcharLast, player_ent->pchar, false, false, true, 0, 0);
            player_ent->sentStats = 1;
        }

        // Send power info updates if the entity we're updating is the player
        // entity the update is being sent to.
        entity_SendPowerInfoUpdate(player_ent, pak);
        entity_SendPowerModeUpdate(player_ent, pak);

        // Send any badge updates which might be needed
        entity_SendBadgeUpdate(player_ent, pak, full_update);

        if(full_update)
        {
            badgeMonitor_SendInfo(player_ent, pak);
        }

        // send the changed inventory
        entity_SendInvUpdate( player_ent, pak );

        // send any invention updates
        entity_SendInventionUpdate( player_ent, pak );

        // send workshop updates
//        character_SendWorkshopUpdate( player_ent->pchar, pak );
        pktSendString(pak, player_ent->pl->workshopInteracting);
        
        // send the teammate list
        
        if (player_ent->teamlist_uptodate && !full_update)
        {
            pktSendBits(pak, 1, 1);
        }
        else
        {
            pktSendBits(pak, 1, 0);
            team_sendList( pak, player_ent, full_update );
            player_ent->teamlist_uptodate = 1;
        }
        sgroup_sendList( pak, player_ent, full_update);

        if (player_ent->leaguelist_uptodate && !full_update)
        {
            pktSendBits(pak, 1, 1);
        }
        else
        {
            pktSendBits(pak, 1, 0);
            league_sendList( pak, player_ent, full_update );
            player_ent->leaguelist_uptodate = 1;
        }

#define SEND_COMBAT_ATTRIBS_EVERY_THIS_MANY_CALLS 8
        if ((player_ent->owner % SEND_COMBAT_ATTRIBS_EVERY_THIS_MANY_CALLS) == (tick_number % SEND_COMBAT_ATTRIBS_EVERY_THIS_MANY_CALLS))
        {
            pktSendBits(pak, 1, 1);
            sendAttribDescription( player_ent, pak, 1 ); 
        }
        else
        {
            pktSendBits(pak, 1, 0);
        }

        send_ent.reliable |= groupDynSend(pak, full_update);

        sendSkyFade(pak);

        // Send the PYR of the camera if I'm a slave.
        if((clientIsSlave(client) || clientGetSlave(client)) && !clientGetControlledEntity(client)){
            ClientLink* controlClient;

            if(clientIsSlave(client)){
                controlClient = clientGetMaster(client);
            }else{
                controlClient = clientGetSlave(client)->client;
            }

            if(controlClient){
                float* pyr = controlClient->controls.pyr.cur;

                pktSendBits(pak, 1, 1);

                pktSendF32(pak, pyr[0]);
                pktSendF32(pak, pyr[1]);
                pktSendF32(pak, pyr[2]);
            }else{
                pktSendBits(pak, 1, 0);
            }
        }
        else{
            pktSendBits(pak, 1, 0);
        }

        //printf("frame %i\n", global_state.global_frame_count);
        pak->reliable = 1; //reliable;

    PERFINFO_AUTO_STOP();

    if (player_ent->entityPacket) {
        PERFINFO_AUTO_START("pktAppend", 1);
            //
            // Append player_ent->entityPacket onto this one.
            //
            // Formerly, we used pktAppend(), but this caused problems with data
            // which had been aligned to a byte boundary within player_ent->entityPacket.
            // By blindly tacking it to the end of pak at pak's current bit offset
            // we'd end up with a copy of player_ent->entityPacket's data in pak which was
            // not aligned as it had been.
            //
            // Instead of using pktAppend(), we add a meta-command to the stream
            // which warns the receiver that byte-aligned data follows. Then we
            // use pktSendBitsArray() to add player_ent->entityPacket to pak at a byte boundary.
            // So any data within pak which had been byte aligned will also be byte
            // aligned in the copy.
            //
            pktSendBitsPack(pak,1,SERVER_BEGIN_BYTE_ALIGNED_DATA);
            pktSendBitsArray(pak,bsGetBitLength(&player_ent->entityPacket->stream),player_ent->entityPacket->stream.data);
            if (pktGetOrdered(player_ent->entityPacket))
            {
                pktSetOrdered(pak, 1);
            }
            //
            // pktSendBitsArray() will push the cursor to the next byte boundary.
            // We don't want that to happen here, as it will cause the consumer of this
            // data to grope ahead into toward within those lost bits and become confused.
            // So pull the cursor back to the end of the bit stream.
            //
            pak->stream.cursor.byte = ( pak->stream.bitLength >> 3 );
            pak->stream.cursor.bit = ( pak->stream.bitLength - ( pak->stream.cursor.byte << 3 ));
            pktFree(player_ent->entityPacket);
        PERFINFO_AUTO_STOP();
    }

    player_ent->entityPacket = pak;
}

typedef struct InfoTab
{
    char *name;
    void (*code)(void *, char **);
    bool send;
} InfoTab;

typedef struct InfoBoostData
{
    char *displayName;
    int count;
}InfoBoostData;

static int boostListComparator(const InfoBoostData **a, const InfoBoostData **b)
{
    if (a && b && *a && *b)
    {
        int retVal = (*a)->count - (*b)->count;
        return retVal ? retVal : -stricmp((*a)->displayName,(*b)->displayName);
    }
    else
    {
        return 0;    //    who knows what this is at this point...
    }
}
void tab_Desc( void * entity, char ** buffer )
{
    Entity *e = (Entity*)entity;
    estrPrintCharString(buffer, localizedPrintf(e,"InfoDescription", e->strings->playerDescription));
}

void tab_Power(void *entity, char **buffer)
{
    Entity *e = (Entity *)entity;
    int i, j, iSizePows, iSizeSets;
    bool addSet = false;
    PowerSet *powerSet = 0;
    Power *power = 0;
    char *tempBuffer = 0, *sharedBuffer = 0, *specificBuffer = 0, **bufPtr = 0;

    if (!e || !e->pchar)
        return;

    estrCreate(&tempBuffer);
    estrCreate(&sharedBuffer);
    estrCreate(&specificBuffer);
    estrClear(buffer);

    iSizeSets = eaSize(&e->pchar->ppPowerSets);

    for (i = 0; i < iSizeSets; i++)
    {
        powerSet = e->pchar->ppPowerSets[i];

        if (!powerSet || !powerSet->psetBase || !powerSet->psetBase->bShowInInfo)
        {
            continue;
        }

        iSizePows = eaSize(&powerSet->ppPowers);
        
        if (iSizePows == 0)
        {
            continue;
        }

        estrClear(&tempBuffer);
        addSet = false;

        if (stricmp(powerSet->psetBase->pchName, "Set_Bonus") == 0)
        {
            InfoBoostData **boostList = NULL;
            for (j = 0; j < iSizePows; j++)
            {
                int k;
                char *displayName = NULL;
                power = powerSet->ppPowers[j];

                if (!power || !power->ppowBase || !power->ppowBase->bShowInInfo)
                {
                    continue;
                }
                displayName = localizedPrintf(NULL, power->ppowBase->pchDisplayName);
                for (k = 0; k < eaSize(&boostList); ++k)
                {
                    if (stricmp(boostList[k]->displayName, displayName) == 0)
                    {
                        boostList[k]->count++;
                        break;
                    }
                }
                if (k >= eaSize(&boostList))
                {
                    InfoBoostData *newData = calloc(1,sizeof(InfoBoostData));
                    newData->displayName = strdup(displayName);
                    newData->count = 1;
                    eaPush(&boostList, newData);
                }

            }
            eaQSort(boostList, boostListComparator);
            
            for (j = eaSize(&boostList)-1; j >= 0; --j)
            {
                if (addSet)
                {
                    estrConcatStaticCharArray(&tempBuffer, ", ");
                }
                estrConcatCharString(&tempBuffer, boostList[j]->displayName);
                if (boostList[j]->count > 1)
                {
                    estrConcatf(&tempBuffer, " x %i", boostList[j]->count);
                }
                addSet = true;
                free(boostList[j]->displayName);
                free(boostList[j]);
            }
            eaDestroy(&boostList);
        }
        else
        {
            for (j = 0; j < iSizePows; j++)
            {
                power = powerSet->ppPowers[j];

                if (!power || !power->ppowBase || !power->ppowBase->bShowInInfo)
                {
                    continue;
                }

                if (addSet)
                {
                    estrConcatStaticCharArray(&tempBuffer, ", ");
                }

                estrConcatCharString(&tempBuffer, localizedPrintf(e, power->ppowBase->pchDisplayName));

                addSet = true;
            }
        }
        if (!addSet)
            continue;

        if (i < g_numSharedPowersets)
        {
            bufPtr = &sharedBuffer;
        }
        else
        {
            bufPtr = &specificBuffer;
        }
        
        estrConcatStaticCharArray(bufPtr, "<color paragon>");
        estrConcatCharString(bufPtr, localizedPrintf(e, powerSet->psetBase->pchDisplayName));
        estrConcatStaticCharArray(bufPtr, ":</color>");
        estrConcatEString(bufPtr, tempBuffer);
        estrConcatStaticCharArray(bufPtr, "<br>");
    }

    estrDestroy(&tempBuffer);

    estrConcatEString(buffer, specificBuffer);
    estrConcatEString(buffer, sharedBuffer);
}

void tab_Arena( void * entity, char ** buffer )
{
    Entity ** ents = (Entity**)entity;

    //we don't know the player's arena information right now, so we will send a 
    //tab that says 'requesting data', and send a packet to the arenaserver 
    //asking for the information, when it gets it it will tell us, and we will relay
    //the information to the client
    ArenaRequestUpdatePlayer(ents[1]->db_id,ents[0]->db_id,1);

    estrClear(buffer);
}

void tab_PvP(void * entity, char ** buffer )
{
    Entity **ents = (Entity**)entity;
    char reptemp[32];

    estrClear(buffer);

    if (!ents[0]->pl)
    {
        return;
    }
    
    // PvP Active state
    estrConcatStaticCharArray(buffer, "<color paragon>" );
    estrConcatCharString(buffer, localizedPrintf(ents[0],"InfoPvPActive"));
    estrConcatStaticCharArray(buffer, ":</color> " );
    estrConcatCharString(buffer, localizedPrintf(ents[0],ents[0]->pchar->bPvPActive ? "InfoPvPEnabled" : "InfoPvPDisabled"));
    estrConcatStaticCharArray(buffer, "<br><br>");

    // PvP Switch state
    /*** JW: Removed until someone decides they want to irritate people again
    estrConcatCharString(buffer, "<color paragon>" );
    estrConcatCharString(buffer, localizedPrintf("InfoPvPSwitch"));
    estrConcatCharString(buffer, ":</color> " );
    estrConcatCharString(buffer, localizedPrintf(ents[0]->pl->pvpSwitch ? "InfoPvPEnabled" : "InfoPvPDisabled"));
    estrConcatCharString(buffer, "<br>");
    ***/

    // Reputation
    estrConcatStaticCharArray(buffer, "<color paragon>" );
    estrConcatCharString(buffer, localizedPrintf(ents[0],"InfoReputation"));
    estrConcatStaticCharArray(buffer, ":</color> " );
    sprintf(reptemp, "%3.1f<br>",playerGetReputation(ents[0]));
    estrConcatCharString(buffer, reptemp);

    // If requesting info about myself, send defeat list
    if (ents[0] == ents[1])
    {
        int i = 0;
        DefeatRecord *defeats = ents[0]->pl->pvpDefeatList;
        unsigned long curTime = dbSecondsSince2000();
        char timeout[16];

        pvp_CleanDefeatList(ents[0]);

        estrConcatStaticCharArray(buffer, "<br><color paragon>" );
        estrConcatCharString(buffer, localizedPrintf(ents[0],"InfoDefeatList"));
        estrConcatStaticCharArray(buffer, ":</color><br>" );

        for (i = 0; i < MAX_DEFEAT_RECORDS; i++)
        {
            if (defeats[i].defeat_time)
            {
                int dt = defeats[i].defeat_time + DEFEAT_RECORD_TIME -curTime;
                sprintf(timeout, "%01d:%02d ",dt/60,dt%60);

                estrConcatStaticCharArray(buffer, "  <color paragon>");
                estrConcatCharString(buffer, dbPlayerNameFromId(defeats[i].victor_id));
                estrConcatStaticCharArray(buffer, " :</color>  ");
                estrConcatCharString(buffer, timeout);
                estrConcatCharString(buffer, localizedPrintf(ents[0],"MinuteString"));
                estrConcatStaticCharArray(buffer, "</color><br>");
            }
            else
            {
                break; // pvp_CleanDefeatList ordered them for us
            }
        }
    }
}

void sendEntityInfo( Entity *requester, int svr_id, int tab )
{
    Entity *e = entFromId( svr_id );

    char *tmp;
    int i;
    static InfoTab infoTab[INFO_TAB_MAX] = {    {"DescriptionString",    tab_Desc,        0    },
                                                {"PowerString",            tab_Power,        0    },
                                                {"BadgesString",        0,                0    },
                                                {"AlignmentTabString",    0,                0    },
                                                {"PvPTabString",        tab_PvP,        0    },
                                                {"ArenaTabString",        tab_Arena,        0    }};

    if( !e )
        return;

    START_PACKET( pak, requester, SERVER_SEND_PLAYER_INFO );

      if(ENTTYPE(e)==ENTTYPE_PLAYER)
    {
        int iNumTabs = 0;
        int bBadgeFound = 0;
        RewardToken *tempToken;
        int tempValue;

        pktSendBits(pak,1,1); // valid entity
        pktSendBits(pak,1,1); // player

        //send badge info for players only
        for( i = 0; i < BADGE_ENT_BITFIELD_SIZE; i++ )
        {
            pktSendBits( pak, 32, e->pl->aiBadgesOwned[i] );
            bBadgeFound |= e->pl->aiBadgesOwned[i];
        }

        if( e->pl->titleBadge )
        {
            pktSendBits(pak,1,1);
            pktSendBits(pak, 32, e->pl->titleBadge );
        }
        else
            pktSendBits(pak,1,0);

        // send alignment info
        pktSendBitsAuto(pak, (tempToken = getRewardToken(e, "roguepoints_hero")) ? tempToken->val : 0);
        pktSendBitsAuto(pak, (tempToken = getRewardToken(e, "roguepoints_vigilante")) ? tempToken->val : 0);
        pktSendBitsAuto(pak, (tempToken = getRewardToken(e, "roguepoints_villain")) ? tempToken->val : 0);
        pktSendBitsAuto(pak, (tempToken = getRewardToken(e, "roguepoints_rogue")) ? tempToken->val : 0);

        pktSendBitsAuto(pak, e->last_time);

        tempToken = getRewardToken(e, "TimePlayedAsCurrentAlignment");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "AlignmentChangeCount");
        tempValue = tempToken ? tempToken->timer : 0;
        pktSendBitsAuto(pak, tempValue);

        // same tempToken
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        pktSendBitsAuto(pak, badge_StatGet(e, "ParagonAlignmentMissionsCompleted"));
        pktSendBitsAuto(pak, badge_StatGet(e, "HeroAlignmentMissionsCompleted"));
        pktSendBitsAuto(pak, badge_StatGet(e, "VigilanteAlignmentMissionsCompleted"));
        pktSendBitsAuto(pak, badge_StatGet(e, "RogueAlignmentMissionsCompleted"));
        pktSendBitsAuto(pak, badge_StatGet(e, "VillainAlignmentMissionsCompleted"));
        pktSendBitsAuto(pak, badge_StatGet(e, "TyrantAlignmentMissionsCompleted"));

        tempToken = getRewardToken(e, "TotalAlignmentTipsDismissed");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "ParagonSwitchMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "HeroSwitchMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "VigilanteSwitchMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "RogueSwitchMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "VillainSwitchMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "TyrantSwitchMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "ParagonReinforceMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "HeroReinforceMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "VigilanteReinforceMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "RogueReinforceMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "VillainReinforceMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "TyrantReinforceMoralityMissionsCompleted");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        tempToken = getRewardToken(e, "TotalMoralityTipsDismissed");
        tempValue = tempToken ? tempToken->val : 0;
        pktSendBitsAuto(pak, tempValue);

        iNumTabs++;
        infoTab[INFO_TAB_POWER].send = true;

        // tabs
        if( e->strings->playerDescription && e->strings->playerDescription[0] != '\0')
        {
            iNumTabs++;
            infoTab[INFO_TAB_DESC].send = true;
        }
        else
            infoTab[INFO_TAB_DESC].send = false;

        if( e->pl->aiBadgesOwned )
        {
            iNumTabs++;
            infoTab[INFO_TAB_BADGE].send = true;
        }
        else
            infoTab[INFO_TAB_BADGE].send = false;

        //if we want to send the arena tab (find out under what circumstances we do this)
        iNumTabs++;
        infoTab[INFO_TAB_ARENA].send = true;

        if (1) { //e->pl->pvpSwitch) { //only send PVP tab if we're doing PVP // pvpSwitch is DEFUNKED
            iNumTabs++;
            infoTab[INFO_TAB_PVP].send = true;
        }
        else 
            infoTab[INFO_TAB_PVP].send = false;

        if (e->pchar->iLevel >= 19 && !ENT_IS_IN_PRAETORIA(e) && (e->pl->displayAlignmentStatsToOthers || e->db_id == requester->db_id))
        {
            iNumTabs++;
            infoTab[INFO_TAB_ALIGNMENT].send = true;
        }
        else
        {
            infoTab[INFO_TAB_ALIGNMENT].send = false;
        }

        pktSendBits( pak, 16, iNumTabs );

        estrCreate(&tmp);

        for( i = 0; i < INFO_TAB_MAX; i++ )
        {
            if( infoTab[i].send )
            {
                if( tab == i ) // indicate this tab is selected
                    pktSendBits(pak,1,1);
                else
                    pktSendBits(pak,1,0);

                pktSendString(pak, localizedPrintf(requester,infoTab[i].name) );

                if( infoTab[i].code )
                {
                    if (i==INFO_TAB_ARENA
                        || i==INFO_TAB_PVP) {
                        void * ents[2];                    //arenatab and pvptab need to know both
                        ents[0]=e;                        //the target player, and the
                        ents[1]=requester;                //requesting player
                        infoTab[i].code( ents, &tmp);
                    } else
                        infoTab[i].code( e, &tmp );
                    pktSendString(pak, tmp );
                }
                else if (i == INFO_TAB_BADGE)
                    pktSendString(pak, "badge");
                else if (i == INFO_TAB_ALIGNMENT)
                    pktSendString(pak, "alignment");
                else
                    pktSendString(pak, "ERROR");
            }
        }

        estrDestroy(&tmp);
    }
    else if(ENTTYPE(e)==ENTTYPE_CRITTER)
    {
        const U32 *eaSalvageId = NULL;
        int salvage_drops = 0;
        rewardGetAllPossibleSalvageId(&eaSalvageId,e);
        salvage_drops = eaiSize(&eaSalvageId);

        pktSendBits(pak,1,1);    // valid entity
        pktSendBits(pak,1,0);    // critter (not player)

        pktSendBitsPack(pak,5,salvage_drops); // probably not more than 30 for a single critter
        for(i = 0; i < salvage_drops; ++i)
            pktSendBitsPack(pak,9,eaSalvageId[i]); // last I checked, there's 288 salvage
        eaiDestroy(&eaSalvageId);

        pktSendBits(pak,16,salvage_drops ? 2 : 1);    // # of tabs

        pktSendBits(pak,1,tab==0);    // Description tab selected?
        pktSendString(pak,localizedPrintf(requester,infoTab[INFO_TAB_DESC].name));    // tab name
        if( e->description && e->description[0] )
        {
            pktSendString(pak, localizedPrintf(requester, e->description));
        }
        else if ( e->villainDef && e->villainDef->description && e->villainDef->description[0] )
        {
            pktSendString(pak, localizedPrintf(requester, e->villainDef->description));
        }
        else
        {
            pktSendString(pak, localizedPrintf(requester,e->pchar->pclass->pchDisplayHelp));
        }
        if(salvage_drops)
        {
            pktSendBits(pak,1,tab==1);        // Salvage tab selected?
            pktSendString(pak,localizedPrintf(requester,"SalvageDropsString"));    // tab name
            pktSendString(pak,"salvage_drops");    // client-generated content based on salvage list above
        }
    }
    else
        pktSendBits(pak, 1, 0); //invalid entity

    END_PACKET
}
