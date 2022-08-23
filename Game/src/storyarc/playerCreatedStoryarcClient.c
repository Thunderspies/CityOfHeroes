

#include "storyarc/playerCreatedStoryarc.h"
#include "storyarc/playerCreatedStoryarcClient.h"
#include "player/player.h"

#include <utilitieslib/components/earray.h>
#include "entity/costume.h"
#include "gameComm/Npc.h"
#include "entity/entclient.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "entity/costume_client.h"
#include "graphics/seqgraphics.h"

#include <utilitieslib/language/MessageStoreUtil.h>

ClientSideMissionMakerContactCostume gMissionMakerContact;

void setClientSideMissionMakerContact( int npc_idx, Costume * pCostume, char * pchName, int useCachedContact, CustomNPCCostume *pNPCCostume )
{
    static Costume *originalNPCCostume = NULL;
    static Costume *costumeCpy = NULL;
    Costume * pCostumeNew = 0;
    int idx;
    Entity *e;
    static char * defaultName = 0;

    static F32 revert_timer = 0;

    if( !defaultName )
        defaultName = strdup(textStd("ArchitectContactNameDefault"));

     if(pCostume)
        pCostumeNew = pCostume;
    else if( npc_idx >= 0 )
    {
        if (originalNPCCostume)
        {
            costume_destroy(originalNPCCostume);
        }
        originalNPCCostume = costume_clone(npcDefsGetCostume(npc_idx, 0));
        pCostumeNew = originalNPCCostume;
        if (pNPCCostume)
        {
            costumeApplyNPCColorsToCostume(pCostumeNew,pNPCCostume );
        }
    }

     revert_timer -= TIMESTEP;

     for(idx=1;idx<entities_max;idx++)
    {
        if (!(entity_state[idx] & ENTITY_IN_USE ))
            continue;
        e = entities[idx];
        if(e->contactArchitect)
        {
             if(pCostumeNew)
            {
                e->costume = costume_as_const(pCostumeNew);
                revert_timer = 300;
            }
            else if ((revert_timer < 0 ) || (npc_idx < 0))    //    If reverting or npc_idx is -1, use the entities default npcIndex
            {
                revert_timer = -1;
                e->costume = npcDefsGetCostume(e->npcIndex,0);
            }

            if( pchName && *pchName )
                strcpy( e->name, pchName );
            else
                strcpy( e->name, defaultName );

            costume_Apply(e);
            
        }
    }
    if(pCostumeNew)
    {
        if( !useCachedContact )
        {
            if (costumeCpy)
            {
                costume_destroy(costumeCpy);
            }
            costumeCpy = costume_clone(costume_as_const(pCostumeNew));
            seqRemoveHeadshotCached(costume_as_const(costumeCpy), 0 );
        }
    }

    if(pCostume)
    {
        gMissionMakerContact.npc_idx = npc_idx;
        gMissionMakerContact.pCostume = costumeCpy;
    }
    else
    {
        gMissionMakerContact.npc_idx = 0;
        gMissionMakerContact.pCostume = 0;
    }
}


