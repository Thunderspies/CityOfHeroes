/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include "string.h"
#include "time.h"

#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>
#include "entity/motion.h"

// For the networking
#include <utilitieslib/network/netio.h>
#include "entity/entVarUpdate.h"
#include "comm_game.h"

#include "entity/entGameActions.h"
#include "NpcServer.h"
#include "NpcNames.h"
#include "gameComm/Npc.h"        // For NPCDef structure
#include "entity/costume.h"    // For Costume structure
#include "generator/entgen.h"     // For entCreateEx().  JS: Why the heck is that function in entgen?

// For character stat assignment
#include "entity/character_base.h"   // For character structure definition
#include "entity/character_combat.h" // For character_ActivateAllAutoPowers
#include "entity/classes.h"          // For character class initialization
#include "entity/origins.h"          // For character origin initialization
#include "entity/powers.h"           // For character power assignment
#include "seq/seq.h"
#include "entity/entity.h"

/**********************************************************************func*
 * npcAddPower
 *
 */
int npcAddPower(Character* character, const char* categoryName, const char* powerSetName, const char* powerName, int dontSetStance, int autopower_only )
{
    int i;
    const BasePowerSet*   basePowerSet;
    PowerSet*       powerSet;

    // Get the base power set
    if((basePowerSet = powerdict_GetBasePowerSetByName(&g_PowerDictionary, categoryName, powerSetName)) == NULL)
    {
        return 0;
    }

    // Make the power set
    if((powerSet=character_BuyPowerSet(character, basePowerSet))==NULL)
    {
        return 0;
    }

    {
        const BasePower *basePower;

        if(powerName && powerName[0] == '*')
        {
            // Buy all powers in the said power set.
            for(i = eaSize(&basePowerSet->ppPowers) - 1; i >= 0; i--)
            {
                Power* pow;

                if( autopower_only && basePowerSet->ppPowers[i]->eType != kPowerType_Auto )
                    continue;

                pow = character_BuyPower(character, powerSet, basePowerSet->ppPowers[i], 0);
                if(pow!=NULL)
                {
                    pow->bDontSetStance = dontSetStance;
                }
            }
        }
        else
        {
            Power *pow;
            // Buy a single power in the said power set.
            if((basePower = powerdict_GetBasePowerByName(&g_PowerDictionary, categoryName, powerSetName, powerName))==NULL)
            {
                return 0;
            }

            if( autopower_only && basePower->eType != kPowerType_Auto )
                return 1;

            pow = character_BuyPower(character, powerSet, basePower, 0);
            if(pow!=NULL)
            {
                pow->bDontSetStance = dontSetStance;
            }
        }
    }

    return 1;
}

/**********************************************************************func*
 * npcCreate
 *
 * Given the name of an NPC as defined in one of the NPC definitions,
 * instanciate the NPC as an Enity.
 *
 */
Entity* npcCreate(const char *name, EntType ent_type)
{
    Entity* e;              // The instanciated NPC.
    const NPCDef* npcDef = NULL;  // What kind of NPC are we trying to create?
    int         npcIndex;
    const cCostume* costume = NULL;
    int         costumeIndex;

    npcDef = npcFindByName(name, &npcIndex);

    if(!npcDef)
    {
        return NULL;
    }


    // Assign the NPC a random costume.
    costumeIndex = randInt(eaSize(&npcDef->costumes));

    if(npcDef->costumes)
    {
        costume = npcDef->costumes[costumeIndex];

        // Create the NPC with the correct body.
        e = entCreateEx(costume->appearance.entTypeFile, ent_type);
    }
    else
    {
        e = entCreateEx(name, ent_type);
    }

    e->npcIndex = npcIndex;
    entSetImmutableCostumeUnowned( e, costume ); // e->costume = costume;
    e->costumeIndex = costumeIndex;
    e->ready = 1; // Only ready entities are processed.

    if (ent_type == ENTTYPE_DOOR) {
        strcpy(e->name, "Dr");
    } else {
        if( e->seq && e->seq->type->hasRandomName )
            npcAssignName(e);
    }
    if (ent_type == ENTTYPE_CRITTER) {
        //    npcInitClassOrigin(e, e->npcIndex); //Craig says this is unused 7.26.04
        //npcInitPowers(e, npcDef->powers);  //Craig says this is unused 7.26.04

        character_ActivateAllAutoPowers(e->pchar);
    } else {
        // Default NPC run speed for when it's not specified!
        setSpeed(e, SURFPARAM_GROUND, 0.7 / BASE_PLAYER_FORWARDS_SPEED);
        setSurfaceModifiers(e, SURFPARAM_GROUND, 1.0, 1.0, 1.0, 1.0);
        setSurfaceModifiers(e, SURFPARAM_AIR, 0.1, 1.0, 1.0, 1.0);
    }

    return e;
}

/**********************************************************************func*
 * npcInitPowers
 *
 */
int npcInitPowers(Entity* e, const PowerNameRef** powers, int autopower_only)
{
    int i;
    int iSize;
    int retcode = 1;

    if(!e->pchar)
        return 0;

    // Assign all NPC powers.
    iSize = eaSize(&powers);
    for(i = 0; i < iSize; i++)
    {
        const PowerNameRef* powerNameRef = eaGetConst(&powers, i);
        if (!npcAddPower(e->pchar, powerNameRef->powerCategory, powerNameRef->powerSet, powerNameRef->power, powerNameRef->dontSetStance, autopower_only))
            retcode = 0;
    }

    return retcode;
}

/* End of File */
