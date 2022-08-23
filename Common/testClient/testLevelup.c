#include "testLevelup.h"
#include "player/player.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/origins.h"
#include <utilitieslib/utils/utils.h>
#include "entity/powers.h"
#include <utilitieslib/components/earray.h>
#include "gameData/randomCharCreate.h"
#include "UI/uiNet.h"
#include "clientcomm/clientcomm.h"
#include "entity/entclient.h"
#include <utilitieslib/network/netio.h>
#include "entity/entVarUpdate.h"
#include "gameComm/dooranimcommon.h"
#include "entity/entity.h"
#include "entity/character_eval.h"
#ifdef TEST_CLIENT
#include "testUtil.h"
#define TC_BROADCAST(fmt, s) \
    sprintf(buf, fmt, s); \
    commAddInput(buf);
#else
#pragma warning (disable:4002)

#define TC_BROADCAST(fmt, s)

typedef enum {
    TCLOG_FATAL,
    TCLOG_SUSPICIOUS,
    TCLOG_DEBUG,
    TCLOG_STATS,
} TestClientLogLevel;

void testClientLog(TestClientLogLevel level, const char *fmt, ...) {
    char str[20000] = {0};
    char *name="No playerPtr";
    va_list ap;

    va_start(ap, fmt);
    vsprintf(str, fmt, ap);
    va_end(ap);

    if (playerPtr()) {
        name = playerPtr()->name;
    }
    if (!strEndsWith(str, "\n"))
        strcat(str, "\n");

    if (level == TCLOG_FATAL || level == TCLOG_SUSPICIOUS)
        consoleSetFGColor(COLOR_RED | COLOR_BRIGHT);
    else
        consoleSetFGColor(COLOR_GREEN | COLOR_BRIGHT);
    printf("%s: %s", name, str);
    consoleSetFGColor(COLOR_RED | COLOR_GREEN | COLOR_BLUE);

}
#endif

typedef struct Option {
    int type; // 0=power, 1=powerpoolset, 2=cancel, 3=boostslot
    PowerSet *pset;
    const BasePower *psetBase;
    int iset;
    int ipow;
} Option;

// used to hold lists of available power/slot options to select from
static Option **s_options_data=NULL;
static Option ***s_options = &s_options_data;

// used to hold the enhancement slots we've selected
static Option **s_picked_slot_array_data=NULL;
static Option ***s_picked_slot_array = &s_picked_slot_array_data;

static int s_local_level_adjustment=0;

void checkLevelUp(void) {
    testLevelup_automatedLevelup(0);    /* 0 - sequential power/slot selection, 1 - randomize*/
}

int contact_idx=-1;

bool findSignatureNPC(void) {
    int            i;
    Entity        *e;

    for( i=1; i<=entities_max; i++)
    {
        if (!(entity_state[i] & ENTITY_IN_USE ) )
            continue;
        e = entities[i];
        if (stricmp(e->name, "Ms Liberty")==0) {
            contact_idx = e->svr_idx;
            return true;
        }
    }
    return false;
}

bool gotoSignatureNPC() {
    Entity        *e;
    Entity        *player = playerPtr();
    static int    sent_setpos=0;
    if (contact_idx<=0)
        return false;

    e = ent_xlat[contact_idx];
    if (!e || e->owner<=0)
        return false;

    if (!player)
        return false;

    if (distance3(ENTPOS(player), ENTPOS(e)) < INTERACT_DISTANCE) {
        return true;
    }

    if (sent_setpos<=0) {
        char buf[512];
        printf("testLevelup.c: Sending position...\n");
        sprintf(buf, "setpos %1.4f %1.4f %1.4f", ENTPOSX(e), ENTPOSY(e)+10, ENTPOSZ(e));
        commAddInput(buf);
        sent_setpos=10;
    } else {
        printf("testLevelup.c: Waiting for position to change...\n");
        sent_setpos--;
    }
    return false;
}

// Touch a signature NPC to train a level up
// @todo Does this even work if you don't have Access Level
// (e.g., only looks for Ms. Liberty) and if you do have
// Access Level is it even necessary?
// Seems like something from original feature development testing
bool testLevelUp_touchSignatureNPC()
{
    Entity *e = playerPtr();
    devassert(e && e->pchar);

    // if we don't have Access Level need to interact with a trainer NPC
    if (e->access_level<ACCESS_DEBUG)
    {
        if (!findSignatureNPC()) // Check to see if we've got a signature NPC near bye
        {
            //printf("testLevelup.c: No signature NPCs within range\n");
            return false;
        }
        if (!gotoSignatureNPC())
        {
            printf("testLevelup.c: Not near enough to signature NPC to perform level up\n");
            return false;
        }
    }

    // Interact with NPC
    START_INPUT_PACKET(pak, CLIENTINP_TOUCH_NPC );
    pktSendBitsPack(pak, PKT_BITS_TO_REP_SVR_IDX, contact_idx);
    END_INPUT_PACKET
    return true;
}

// do the actual level increase so that we can buy powers/slots
// to complete training up to the next security level
bool testLevelUp_doLevelIncrease()
{
    Entity *e = playerPtr();
    devassert(e && e->pchar);

    // remove the local level adjustment we added in order to determine
    // available powers/slots at new level
    e->pchar->iLevel -= s_local_level_adjustment;
    s_local_level_adjustment = 0;

    // @todo
    // for an automated level increase where we aren't trying to give the user
    // the option of making selections or canceling we could probably move
    // the level_increaseLevel() to the beginning and avoid hacking with the
    // e->pchar->iLevel
    level_increaseLevel();

    return true;
}

// return number of slots bought
int testLevelup_BuyChosenBoostSlots()
{
    Entity *e = playerPtr();
    int num_slots_to_add;
    int num_slots_bought = 0;

    devassert(e && e->pchar);

    // proceed to purchase the slots
    num_slots_to_add = eaSize(s_picked_slot_array);
    devassert( num_slots_to_add == character_CountBuyBoostForLevel( e->pchar, e->pchar->iLevel ) );

//    printf("Buying %d new specialization slots\n", num_slots_to_add );

    // buy the slots
    START_INPUT_PACKET(pak, CLIENTINP_LEVEL_ASSIGN_BOOST_SLOT );
    {
        int k;

        pktSendBitsPack( pak, 1, num_slots_to_add );
        pktSendBitsPack( pak, kPowerSystem_Numbits, 0 );

        for( k = 0; k < eaSize(s_picked_slot_array); k++ )
        {
            Option* picked = s_picked_slot_array_data[k];
            Power*    ppow = e->pchar->ppPowerSets[picked->iset]->ppPowers[picked->ipow];
            devassert(ppow);

            if (character_BuyBoostSlot(e->pchar,ppow))
            {
                pktSendBitsPack( pak, 1, picked->iset );
                pktSendBitsPack( pak, 1, picked->ipow );
                printf("added boost slot on set='%s' [%d] pow='%s' [%d]\n", e->pchar->ppPowerSets[picked->iset]->psetBase->pchFullName, picked->iset, ppow->ppowBase->pchName, picked->ipow);
                num_slots_bought++;
            }
            else
            {
                devassert(false);
                // @todo can this happen? Do these need to be added at the end of the valid ones?
                pktSendBitsPack( pak, 1, 0 );
                pktSendBitsPack( pak, 1, 0 );
                printf("ERROR: Could not add boost slot: set='%s' [%d] pow='%s' [%d]\n", e->pchar->ppPowerSets[picked->iset]->psetBase->pchFullName, picked->iset, ppow->ppowBase->pchName, picked->ipow);
            }
        }
    }
    END_INPUT_PACKET
    devassert(character_CountBuyBoostForLevel(e->pchar, e->pchar->iLevel) == 0);    // if we didn't buy them all then we are going to have trouble later on
    return num_slots_bought;
}

// walk the power pools and build up the Options array with powers
// that we can potentially add a slot to. Note that since this can
// be called multiple times during a single level up we also need to
// check the contents of the 's_picked_slot_array' list to exclude powers where
// we have already used up the available slots
// (i.e., this is all before we actually submit the slot buy request)
void testLevelup_buildAvailableSlotList( void )
{
    Entity *e = playerPtr();
    int i;

    devassert(e && e->pchar);

    eaClearEx(s_options, NULL);

    for( i = 0; i < eaSize(&e->pchar->ppPowerSets); i++ )
    {
        PowerSet *pset = e->pchar->ppPowerSets[i];

        if(pset->psetBase->eSystem == kPowerSystem_Powers)
        {
            const BasePowerSet *psetBase = pset->psetBase;
            int iSizePows = eaSize( &e->pchar->ppPowerSets[i]->ppPowers );
            int j;

            // loop though powers
            for( j = 0; j < iSizePows; j++ )
            {
                Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
                int num_bought = 0;
                int num_picked = 0;
                int k;

                if(!pow)
                    continue;

                // skip the inherent category, "AccoladeMode" has max boosts of 6
                // but it doesn't seem that we should assign slots to it
                // note that this also skips Inherent Fitness
                if ( strstr( pow->ppowBase->pchFullName, "Inherent") )
                    continue;

                // doesn't skip the Incarnate category
                // but they shouldn't be able to take boost slots anyways

                // skip the temporary powers category
                if (strstr(pow->ppowBase->pchFullName, "Temporary_Powers"))
                    continue;

                num_bought = pow->iNumBoostsBought;

                // walk the list of slots we've already picked and adjust the slot
                // count accordingly so we don't try to overflow a given power
                for( k = 0; k < eaSize(s_picked_slot_array); k++ )
                {
                    Option* picked = s_picked_slot_array_data[k];
                    if (picked->iset == i && picked->ipow == j)
                    {
                        num_picked++;
                    }
                }

                // now if we still haven't exceeded the max allowable slot count for this
                // power add it as an available choice
                if (num_bought + num_picked < MAX_BOOSTS_BOUGHT && eaSize(&pow->ppBoosts) + num_picked < pow->ppowBase->iMaxBoosts)
                {
                    Option* opt = calloc(sizeof(Option),1);
                    opt->type = 3;
                    opt->iset = i;
                    opt->ipow = j;
                    eaPush(s_options, opt);
                }
            }
        }
    }
}

// this level up awards boost slots, so select them
// returns the number of slots bought
int testLevelup_addBoostSlots( int randomize )
{
    Entity *e = playerPtr();
    int num_slots_to_add;
    int i;

    devassert(e && e->pchar && character_CanBuyBoost(e->pchar));

    // create/clear out array of selected slots to add
    if (s_picked_slot_array_data==NULL) {
        eaCreate(s_picked_slot_array);
    } else {
        eaClearEx(s_picked_slot_array, NULL);
    }

    num_slots_to_add = character_CountBuyBoostForLevel( e->pchar, e->pchar->iLevel );
//    printf("You can purchase %d new Specialization Slots at this level\n", num_slots_to_add);

    // build the options array with all the valid choices for this slot buy choice
    for (i = 0; i < num_slots_to_add; ++i)
    {
        int iChoice;
        Option* picked;

        testLevelup_buildAvailableSlotList();
        devassert( eaSize(s_options) );    // we should always have something to pick from
        
        // now select one of the available slots either sequentially or randomly and
        // add it to our pick list
        iChoice = randomize ? randInt2(eaSize(s_options)) : 0;    // pick first available choice unless we randomize

        picked = calloc(sizeof(Option),1);
        *picked = *s_options_data[iChoice];    // copy the chosen power specification
        eaPush(s_picked_slot_array, picked);
    }

    return testLevelup_BuyChosenBoostSlots();
}

// walk the power pools and build up the Options array with powers
// that we can choose from
// (i.e., this is all before we actually submit the power buy request)
void testLevelup_buildAvailablePowerList( void )
{
    Entity *e = playerPtr();
    int category;

    devassert(e && e->pchar && character_CanBuyPower(e->pchar));

    eaClearEx(s_options, NULL);

    for (category = 0; category < kCategory_Count; category++)
    {
        int i;
        // find the power set corresponding to this category
        int iSizeSets = eaSize(&e->pchar->ppPowerSets);

        for( i = 0; i < iSizeSets; i++)
        {
            int j;
            PowerSet *pset = e->pchar->ppPowerSets[i];

            if( pset->psetBase->pcatParent == e->pchar->pclass->pcat[category] )
            {
                const BasePowerSet *psetBase = pset->psetBase;
                int iSizePows = eaSize( &psetBase->ppPowers );

                if (psetBase->ppchSetBuyRequires && !chareval_Eval(e->pchar, psetBase->ppchSetBuyRequires, psetBase->pchSourceFile))
                    continue;

                // loop though powers and check availability
                for( j = 0; j < iSizePows; j++ )
                {
                    if ( !character_OwnsPower( e->pchar, psetBase->ppPowers[j] ) )
                    {
                        // number of levels needed until the power is available, <= 0 means that it's available now.
                        if (powerset_BasePowerAvailableByIdx(pset, j, e->pchar->iLevel) <= 0
                            && (!psetBase->ppPowers[j]->ppchBuyRequires 
                                || chareval_Eval(e->pchar, psetBase->ppPowers[j]->ppchBuyRequires, psetBase->pchSourceFile)))
                        {
                            Option* opt = calloc(sizeof(Option), 1);
                            opt->pset = pset;
                            opt->psetBase = psetBase->ppPowers[j];
                            opt->type = 0;
                            eaPush(s_options, opt);
                        }
                    }
                } // end for every power
            } // end if power set is in category
        } // end for every power set
    } // end for categories

    // if we can buy a power pool set add that as an option as well
    // @todo an existing legacy function is used to randomize the selection of the pool and the power chosen
    // @todo for purposes of limiting variables to debug levelup only choose a new power pool if there are no other
    // possible choices
    if( eaSize(s_options) == 0 && character_CanBuyPoolPowerSet(e->pchar) || character_CanBuyEpicPowerSet(e->pchar) )
    {
        Option* opt = calloc(sizeof(Option), 1);
        opt->type = 1;
        eaPush(s_options, opt);
    }
}

// this level up awards a power so select it and buy it
// returns the power bought for logging purposes
static const BasePower* testLevelup_addPower( int randomize )
{
    Entity *e = playerPtr();
    const BasePower* pbase = NULL;

    devassert(e && e->pchar && character_CanBuyPower(e->pchar));

    testLevelup_buildAvailablePowerList();
    // now select one of the available slots either sequentially or randomly and
    // add it to our pick list
    devassert( eaSize(s_options) );    // we should always have something to pick from

    if (eaSize(s_options))
    {
        int iChoice = randomize ? randInt2(eaSize(s_options)) : 0;    // pick first available choice unless we randomize
        Option* opt = s_options_data[iChoice];
        Power *ppow = NULL;

        switch (opt->type)
        {
            case 0:
                // Buy power
                {
                    pbase = opt->psetBase;
                    ppow = character_BuyPower(e->pchar, opt->pset, pbase, 0);
                }
                break;
            case 1:
                // Buy power pool set
                printf("Buying new power pool set\n");
                {
                    ppow = testChooseAPowerSet(e, false, kCategory_Pool);
                    pbase = ppow->ppowBase;
                }
                break;
            default:
                devassertmsg(false,  "ERROR: " __FUNCTION__ ": invalid pick type.");
                break;
        }

        level_buyNewPower(ppow, 0);
    }

    return pbase;
}

// This function levels character up one level and also trains them up to that level.
// This is either done by randomly selecting from available powers and enhancement slots
// or letting the developer choose using the console.
// As far as I can tell on a given level transition you will either be able to select a new
// power (or power set and power) or add enhancement slots, but not both at the same time
void testLevelup_automatedLevelup(int randomize)
{
    Entity *e = playerPtr();
#ifdef TEST_CLIENT
    char buf[512] = {0};        // used by TC_BROADCAST macro        
#endif
    
    if (!e || !e->pchar) return;

    // if we are already at out experience level we can't train up
    if (e->pchar->iLevel==character_CalcExperienceLevel(e->pchar))
        return;

    if (s_options_data==NULL)    // lazy creation of s_options array
        eaCreate(s_options);

    // our experience level is higher than our current level
    // increase our current level. This increments e->pchar->iLevel and
    // grants any auto issue powers/slots for the level
    level_increaseLevel();

    if (character_CanBuyBoost(e->pchar))
    {
        //**
        // Enhancement Slot level up

        // choose the new slots we are allowed to add (either sequentially or randomly)
        int num_slots_bought = testLevelup_addBoostSlots(randomize);
        TC_BROADCAST("b I've just gone up a level and purchased new specialization slots\n", "");
        testClientLog(TCLOG_DEBUG, "Level %d, Exp level %d, I bought %d slots\n", e->pchar->iLevel+1, character_CalcExperienceLevel(e->pchar)+1, num_slots_bought);
    }
    else if ( character_CanBuyPower(e->pchar) )
    {
        //**
        // Power and power pool level up
        const BasePower* pbase = testLevelup_addPower(randomize);
        if (pbase)
        {
            TC_BROADCAST("b I've just gone up a level and purchased %s\n", pbase->pchName );
            testClientLog(TCLOG_DEBUG, "Level %d, Exp level %d, I bought a new power %s\t[%s]!\n", e->pchar->iLevel + 1, character_CalcExperienceLevel(e->pchar)+1, pbase->pchName, pbase->pchFullName );
        }
        else
            testClientLog(TCLOG_SUSPICIOUS, "Level %d, Exp level %d, unable to buy new power\n", e->pchar->iLevel + 1, character_CalcExperienceLevel(e->pchar)+1);    
    }
}

void testLevelup(int levelNumber)
{
    Entity *e = playerPtr();
    
    if (!e || !e->pchar) 
    {
        return;
    }

    if (levelNumber==0) 
    {
        levelNumber = character_CalcExperienceLevel(e->pchar)+1;
    } else {
        // Make it 0-based
        levelNumber = levelNumber-1;
    }
    if (levelNumber > e->pchar->iLevel)
    {
        int i;

        if(levelNumber < MAX_PLAYER_SECURITY_LEVEL)
        {
            e->pchar->iExperiencePoints=g_ExperienceTables.aTables.piRequired[levelNumber];

            for (i = e->pchar->iLevel; i < levelNumber; i++) 
            {
                testLevelup_automatedLevelup(0);    /* 0 - sequential power/slot selection, 1 - randomize*/
            }
        }
    }    
}

// @todo 
// Check version history to see how old levelup function could be used to display (and allow user selection) of
// powers if you want to try to resurrect that and use it for a display function...but it is buggy for selection

void testLevelup_displayPowers(void)
            {
    testClientLog(TCLOG_DEBUG, "UNIMPLEMENTED: testLevelup_displayPowers()\n");

}
