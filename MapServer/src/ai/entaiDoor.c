
#include <utilitieslib/stdtypes.h>
#include "ailib/aiBehaviorInterface.h"
#include "entaiprivate.h"
#include "entaiPriority.h"
#include "door.h"
#include "dbcomm/dbdoor.h"
#include "gameComm/dooranimcommon.h"
#include "storyarc/storyarcinterface.h"
#include "entity/entserver.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include "group/group.h"

#define DOOR_OPENDISTANCE    SERVER_INTERACT_DISTANCE + 1 // hold a walkthrough door open if entities are in this distance

void aiDoorInit(Entity* e)
{
    AIVars* ai = ENTAI(e);
    DoorEntry* door;

    // figure out whether I'm a walkthrough door based on the type of door link nearby
    door = dbFindLocalMapObj(ENTPOS(e),0,MAX_DOOR_BEACON_DIST);
    if (door &&
        (door->type == DOORTYPE_FREEOPEN ||
         (door->type == DOORTYPE_KEY && !door->special_info)))
    {
        ai->door.walkThrough = 1;
    }
}

void aiDoorSetOpen(Entity* e, int open){
    AIVars* ai = ENTAI(e);

    if (open){
        SETB(e->seq->state, STATE_RECLAIM); // just flash this bit for one frame
    }

    //if( ai->door.open && !TSTB(e->seq->sticky_state,STATE_OPEN) )
    //    printf("!!!!!");

    if(ai->door.open != open){
        ai->door.open = open;

        if(open){
            SETB(e->seq->state,            STATE_OPEN);
            SETB(e->seq->sticky_state,    STATE_OPEN);
            CLRB(e->seq->state,            STATE_SHUT);
            CLRB(e->seq->sticky_state,    STATE_SHUT);
        }else{
            CLRB(e->seq->state,            STATE_OPEN);
            CLRB(e->seq->sticky_state,    STATE_OPEN);
            SETB(e->seq->state,            STATE_SHUT);
            SETB(e->seq->sticky_state,    STATE_SHUT);
        }
    }
    
    if(open){
        ai->door.timeOpened = ABS_TIME;
    }
}

void aiDoorPriorityDoActionFunc(Entity* e,
                                const AIPriorityAction* action,
                                const AIPriorityDoActionParams* params)
{
    AIVars* ai = ENTAI(e);

    if(params->switchedPriority){
        seqClearState(e->seq->sticky_state);
    }

    if(action){
        int animBitCount = action->animBitCount;

        if(animBitCount){
            int i;
            int* animBits = action->animBits;
            
            for(i = 0; i < animBitCount; i++){
                if(animBits[i] >= 0){
                    SETB(e->seq->state, animBits[i]);
                    SETB(e->seq->sticky_state, animBits[i]);
                }
            }
        }
    }

    if(ai->door.open)
    {
        if (ai->door.walkThrough)
        {
            if (OnInstancedMap())
            {
                // we don't close walkthrough doors on mission maps right now
            }
            else 
            {
                // keep doors open if ents in proximity
                F32 closeDist = max(DOOR_OPENDISTANCE, (e->coll_tracker && e->coll_tracker->def ? e->coll_tracker->def->radius : 0));
                if(    ABS_TIME_SINCE(ai->door.timeOpened) > SEC_TO_ABS(5) &&
                    !entWithinDistance(ENTMAT(e), closeDist, NULL, ENTTYPE_PLAYER, 0) &&
                    !entWithinDistance(ENTMAT(e), closeDist, NULL, ENTTYPE_CRITTER, 0))
                {
                    aiDoorSetOpen(e, 0);
                }
            }
        }
        else // transfer door
        {
            if(ABS_TIME_SINCE(ai->door.timeOpened) > SEC_TO_ABS(5)){
                aiDoorSetOpen(e, 0);
            }
        }
    }
}

void aiDoor(Entity* e, AIVars* ai){
    aiBehaviorProcess(e, ai->base, &ai->base->behaviors);
}

