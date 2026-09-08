/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "gameComm/itemselect.h"
#include <utilitieslib/stdtypes.h>
#include "gridcoll/gridcoll.h"
#include "group/group.h"
#include "gridcoll/ctri.h"
#include "graphics/camera.h"
#include "cmdparse/cmdgame.h"
#include "win/win_init.h"
#include "entity/entclient.h"
#include "player/player.h"
#include "graphics/gfxwindow.h"
#include "graphics/gfxwindow.h"
#include "assert.h"
#include "group/groupdynrecv.h"
#include "graphics/font.h"
#include "float.h"
#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "player/pmotion.h"
#include "gameComm/trayCommon.h"
#include "entity/character_base.h"
#include "entity/character_target.h"
#include "entity/motion.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entDebugPrivate.h"
#include "entity/entPlayer.h"
#include "UI/uiPet.h"
#include "bases/baseedit.h"
#include "entity/teamup.h"
#include "UI/uiGame.h"

// extra pixels for clicking on objects
#define RETICLE_SELECT_ALLOWANCE 6


int canSelectAnyEntity()
{
    return game_state.select_any_entity || game_state.place_entity || debug_state.debugCamera.editMode;
}

static int entCursorFind(Vec3 startWorld, Vec3 endWorld, Vec2 mousePos, DefTracker *tracker)
{
    F32        d,closest=16e16;
    int        i,idx;
    Vec3    ent_pos_cam;
    F32        l1len;
     Entity    *player = playerPtr();
    float z, endLen;
    Vec3 vec;
    GroupDef *blocker = tracker?tracker->def:NULL;
    
    subVec3(endWorld, cam_info.cammat[3], vec);
    endLen = lengthVec3(vec); 

    if( game_state.selectionDebug )  
        xyprintf(25, 18, "EndLen %f!", endLen );

    idx = 0;

    PERFINFO_AUTO_START("entCursorFind",1);

    for(i=1;i<entities_max;i++)
    {
        Entity* e;
        Mat4 capMat;
        EntityCapsule cap;

        if(!(entity_state[i] & ENTITY_IN_USE ))
            continue;

        e = entities[i];

        if(player == e && (control_state.first || !canSelectAnyEntity()))
            continue;
    
        if( !isEntitySelectable( e ) )
            continue;

        if( e->svr_idx <= 0 ) //I thought redundant, but maybe not 
            continue;

        //UGLY
        //An associated world geometry piece (the black poly with "door door" property) is selectable
        //Doors (entities spawned by door generators) that don't have collisionType Door(GEO_COLLISION) are bugged because
        //many have capsule collisions in the enttype file.  If they do, ignore them.  In the future fix this by adding 
        //the right kind of collision to all doors.   
        //Also Collision Type door is made to imply selection type door, which is a little strange
        //TO DO Get rid of this and put selection thing in the enttype 
        //mw 3.806 removed this.  The above comment was wrong, I think.  If a door is put down, it should be given the right
        //collision capsule, or set to collision type none.  I don't actually think there is anywhere that screws up like this
        //except the cathedral pf oina door entity that just needed to be set to noColl
        //if ( ENTTYPE(e) == ENTTYPE_DOOR && e->seq->type->collisionType != SEQ_ENTCOLL_DOOR ) 
        //    continue;  
 
        cap = e->motion->capsule; 
        positionCapsule( ENTMAT(e), &cap, capMat );
        //capMat is oriented with the collision capsule, and capmat[3] is the near end of the collision capsule line)

        //if( e != playerPtr() && e->seq->gfx_root->flags & 2 )
        //{
        //    xyprintf( 20, 30, "Hid!" ); 
        //}

        mulVecMat4(capMat[3], cam_info.viewmat, ent_pos_cam);  
        z = -ent_pos_cam[2]; //distance from camera to ent

        if (z < 0) //Ent is behind near clip plane
            continue;

        //minus cap.radius to be more forgiving of big guys. allowing ease of selection, but it's theoretically possible to select through a thin wall.
        //(- cap.radius isn't precise, since the capsule could be tilted, or you above it but this was always a rough estimate) 
        if (z > endLen + cap.radius )  
        {
            //Ent is beyond clickable distance or World geometry that is not part of the ent is blocking selection of the ent
            if( !tracker || tracker->ent_id != e->svr_idx ) 
                continue;
        }
 
        if( e->seq->type->selection == SEQ_SELECTION_LIBRARYPIECE || e->seq->type->collisionType == SEQ_ENTCOLL_DOOR )
        {
            //Selection Library Piece only works if your collisionType is also Library Piece.
            //  (HealthFx/Library Piece gets inserted into e->coll_tracker)
            //Selection Door only works if your collisionType is also Door
            //  (Door's Animation's GEO_COLLISION becomes e->coll_tracker)

            //Both LibraryPiece's HealthFx/Geometry and Door's Animation's GEO_COLLISION become 
            if( !tracker || tracker->ent_id == -1 || tracker->ent_id != e->svr_idx ) //-1 since tracker *could* have a bad ent
                continue;
            
            //TO DO maybe : if you have a library piece object that is not collidable, selection library piece won't work
            //either flag this as an error to the artist making the piece, or else do a line/object collision just for this 
            //thing right here.
        }
        else if ( e->seq->type->selection == SEQ_SELECTION_CAPSULE || !mousePos ) //!mousepos == don't use bone selection on minimap calculator
        {
            Vec3 l1dir, l1isect, l2isect;

            subVec3(endWorld,startWorld,l1dir);
            l1len = normalVec3(l1dir);

            d = coh_LineLineDist(    startWorld,    l1dir,                l1len,        l1isect,
                                    capMat[3],    ENTMAT(e)[cap.dir],    cap.length,    l2isect );

            if ( d > SQR( cap.radius ) )
                continue;
        }
        else //e->seq->type->selection == SEQ_SELECTION_BONES
        {
            Vec2 ul, lr;

            if( !seqGetScreenAreaOfBones( e->seq, ul, lr, NULL, NULL ) )
                continue; //something's wrong with this guy, he has no gfx or he's off screen -- in any case not click selectable

            if (mousePos[0] < (ul[0] - RETICLE_SELECT_ALLOWANCE) || mousePos[0] > (lr[0] + RETICLE_SELECT_ALLOWANCE))
                continue;

            if (mousePos[1] < (lr[1] - RETICLE_SELECT_ALLOWANCE) || mousePos[1] > (ul[1] + RETICLE_SELECT_ALLOWANCE))
                continue;
        }

        if (z < closest)   
        {
            if( game_state.selectionDebug )
                xyprintf(25, 27+e->owner, "%s %f!", e->name, z );
            closest = z;
            idx = e->svr_idx;
            assert(idx > 0);
        }
    }

    PERFINFO_AUTO_STOP();

    return idx;
}


int cursorFind3D(Vec3 start, Vec3 end, ItemSelect *item, Mat4 matWorld, bool *hitWorld)
{
    DefTracker    *tracker;
    Entity *player = playerPtr();

    item->def = 0;
    tracker = groupFindOnLine(start,end,matWorld,hitWorld,0);

    item->ent_id = entCursorFind(start,end, 0, tracker);

    if (tracker)
    {
        copyMat4(tracker->mat, item->mat);
        item->tracker = tracker;
    }
    else
    {
        copyMat4(matWorld, item->mat);
        item->tracker = 0;
    }

    if (item->def || item->ent_id >= 0)
        return 1;
    return 0;
}

int cursorFind(int x,int y,F32 dist,ItemSelect *item_sel,Vec3 end,Mat4 matWorld,bool *hitWorld, int defMaxDistance, Vec3 probe)
{
    Vec3    start,dv,startCam;
    DefTracker    *tracker;
    Vec2 mouseCursor;
    Entity *player = playerPtr();

    item_sel->def = 0;

    //Cast ray, (tracker == first found up the tree with properties or ent_id, if no trackers have properties or ent_ids, they aren't used)
    //Notice that end gets truncated to collision location
    gfxCursor3d(x,y,dist,start,end);
    tracker = groupFindOnLine(start,end,matWorld,hitWorld,0);
    
    // Save the probe vector - we actually save it's reverse (from end to start)
    if (probe)
        subVec3(start, end, probe);

    //Find the nearest entity on this line
    mulVecMat4(start,cam_info.viewmat,startCam);
    gfxWindowScreenPos(startCam, mouseCursor);
    item_sel->ent_id = entCursorFind(start,end,mouseCursor,tracker);

    if (tracker)
    {
        copyMat4(tracker->mat, item_sel->mat);
        item_sel->tracker = tracker;
    }
    else
    {
        copyMat4(matWorld, item_sel->mat);
        item_sel->tracker = 0;
    }

    if (tracker && playerPtr())
    {
        subVec3(item_sel->mat[3],ENTPOS(playerPtr()),dv);
        if (lengthVec3(dv) < defMaxDistance)
        {
            item_sel->def = tracker->def;
        }
    }

    if( game_state.selectionDebug )
    {
        xyprintf( 20, 20, "hitWorld %d", *hitWorld );
        xyprintf( 20, 21, "item_sel->tracker 0x%p", item_sel->tracker );
        if( item_sel->def )
            xyprintf( 20, 22, "item_sel->def      %s", item_sel->def->name );
        xyprintf( 20, 23, "item_sel->ent_id       %d", item_sel->ent_id );
        if( item_sel->ent_id > 0 )
            xyprintf( 20, 24, "Entity: %s", entFromId( item_sel->ent_id )->name );
    }

    if (item_sel->def || item_sel->ent_id )
        return 1;
    return 0;
}

bool isEntityPerceptible(Entity *e)
{
    Character *pcharTarget, *pcharPlayer;
    float fEffPercept, fDistSQR;
    Vec3 perceptionPos;
    assert(e);
    
    if(!playerPtr())
    {
        return false;
    }
    
    pcharTarget = e->pchar;
    pcharPlayer = playerPtr()->pchar;

    if (current_menu() == MENU_GAME &&
        !entity_TargetIsInVisionPhase(playerPtr(), e))
    {
        return false;
    }    

    if ( game_state.viewCutScene )
        copyVec3( game_state.cutSceneCameraMat[3], perceptionPos );
    else
        copyVec3( ENTPOS(playerPtr()), perceptionPos );

    // If it's an npc (contact, etc) or a same-team character, it's perceptible
    if (!pcharTarget || character_TargetIsFriend(pcharPlayer,pcharTarget))
    {
        return 1;
    }

    fEffPercept = pcharPlayer->attrCur.fPerceptionRadius;
    fDistSQR = distance3Squared(perceptionPos, ENTPOS(e));

    // If the target is not stealthed, or it's not a player and it's not stealthed or has negative stealth ...
    if (pcharTarget->attrCur.fStealthRadiusPlayer == 0.0f || (e->pl == NULL && pcharTarget->attrCur.fStealthRadiusPlayer <= 0.0f ))
    {
        // Assume the player's percept radius.. however if the monster wants to be seen
        //  from farther away and we haven't been debuffed, go with that instead
        if (e->seq && 
            e->seq->type->fade[1] > fEffPercept && 
            (pcharPlayer->pclass && pcharPlayer->pclass->ppattrBase[0]->fPerceptionRadius <= fEffPercept))
        {
            fEffPercept = e->seq->type->fade[1];
        }

        return fDistSQR < SQR(fEffPercept);
    }
    return (sqrt(fDistSQR) + pcharTarget->attrCur.fStealthRadiusPlayer) < fEffPercept;
}

bool isEntityPlacatingMe(Entity *e)
{
    if (g_pperPlacators)
    {
        EntityRef er = erGetRef(e);
        int i;
        for (i = eaSize(&g_pperPlacators)-1; i >= 0; i--)
        {
            if (er == *g_pperPlacators[i])
                return true;
        }
    }
    return false;
}

int isEntitySelectable(Entity *e)
{
    if(canSelectAnyEntity()) 
    {
        // Every entity is selectable.
        return 1;
    }
        
    if (e->seq)
    {
        // If we're doing some sort of fading in or out, trigger alpha inheritance on effects
        e->seq->forceInheritAlpha = (e->fadedirection != ENT_NOT_FADING);
    }

    // Don't allow this to be selected (or its fading mucked with) if it is flagged as 
    //  undergoing permanent fade out (and has thus been deleted from this client)
    if (!e || e->waitingForFadeOut)
        return 0;

    // We check perception to fiddle with fading, even if you're not selectable (trip mines, etc)
    if (isEntityPerceptible(e))
    {
        if (e->seq) 
            e->seq->notPerceptible = false;
        if (e->fadedirection == ENT_FADING_OUT) 
            e->fadedirection = ENT_FADING_IN;
    }
    else 
    {
        if (e->seq) 
            e->seq->notPerceptible = true;
        if (e->fadedirection != ENT_FADING_OUT) 
            e->fadedirection = ENT_FADING_OUT;
    }

    // order matters
    if (e->seq && (e->seq->type->notselectable || !seqIsMoveSelectable(e->seq)))
        return 0;
    else if (e->notSelectable)
        return 0;
    else if (isEntityPlacatingMe(e))
        return 0;
    else if (team_IsMember(playerPtr(), e->db_id) || league_IsMember(playerPtr(), e->db_id))
        return 1;
    else
        return e->fadedirection != ENT_FADING_OUT;
}

GroupDef *g_pObjGroupDef = NULL;
int excludeObjectTest(void* trackerData, int backside)
{
    DefTracker * tracker = (DefTracker *)trackerData;
    if (g_pObjGroupDef == NULL) return 1;
    while(tracker != NULL)
    {
        if (tracker->def == g_pObjGroupDef) return 0;
        tracker = tracker->parent;
    }
    return 1;
}

static int selectEntityVisible(Entity *e)
{
    Vec3        view_pos,pos;
    CollInfo    coll;
    int            i,j;
    U32            collflags = COLL_DISTFROMSTART|COLL_HITANY;

    if (game_state.select_any_entity)
        return 1;
    copyVec3(ENTPOS(e),pos);
    for(i=0;i<3;i++)
    {
        mulVecMat4(pos,cam_info.viewmat,view_pos);
        if (gfxSphereVisible(view_pos,1.5f))
            break;
        pos[1] += 3;
    }
    if (i >= 3)
        return 0;

    if(e->seq && e->seq->type->collisionType == SEQ_ENTCOLL_LIBRARYPIECE)
    {
        // If you are supposed to be using the worldGroup.
        if (e->seq->worldGroupPtr == NULL)
        {
            seqUpdateWorldGroupPtr(e->seq,e); // Try to get it set up
        }

        // We have to ignore this object during the collision test
        if (e->seq->worldGroupPtr)
        {
            g_pObjGroupDef = e->seq->worldGroupPtr;
            coll.node_callback = excludeObjectTest;
            collflags |= COLL_NODECALLBACK;
        }
    }
    else
    {
        g_pObjGroupDef = NULL;
    }

    for(j=0;j<3;j++)
    {
        copyVec3(ENTPOS(e),pos);
        moveVinX(pos,cam_info.cammat,(j - 1) * 1.f);

        for(i=0;i<3;i++)
        {
            if (!collGrid(0,cam_info.cammat[3],pos,&coll,0,collflags))
            {
                return 1;
            }

            pos[1] += 3;
        }
    }
    return 0;
}

int selectNextEntity(int last_svr_idx, bool bBackwards, int FriendOrFoe, 
                     int DeadOrAlive, int BasePassiveOrNot, int MyPetOrNot, int TeammateOrNot, char * name )
{ // for the 'Or' variables, -1 is first only, 0 is both, 1 is second only
    int            idx, best_idx = -1, fallbackIdx = -1;
    Entity        *e=NULL;
    F32            referenceDist, bestDist, fallbackDist;
    Entity        *player = playerPtr();

    // Find the previously selected entity if it exists
    if (last_svr_idx>0)
    {
        e = entFromId(last_svr_idx);
        if (!e)
        {
            last_svr_idx = -1;
        }
    }

    if (last_svr_idx<=0)
    {
        // Our previously selected entity is no longer valid
        referenceDist = 0.0;
        last_svr_idx = -1;
    }
    else
    {
        referenceDist = distance3Squared(ENTPOS(playerPtr()), ENTPOS(e));
    }
    best_idx = -1;

    for(idx=1;idx<entities_max;idx++)
    {
        F32 dist;
        if (!(entity_state[idx] & ENTITY_IN_USE ))
            continue;
        e = entities[idx];
        if (e->svr_idx==last_svr_idx)
            continue;

        if (!canSelectAnyEntity())
        {
            if(e == playerPtr())
                continue;

            if(!character_IsConfused(playerPtr()->pchar) && FriendOrFoe)
            {
                int target_type;
                if(FriendOrFoe < 0)
                {
                    if( DeadOrAlive > 0)
                        target_type = kTargetType_Friend;
                    else if( DeadOrAlive < 0 )
                        target_type = kTargetType_DeadFriend;
                    else
                        target_type = kTargetType_DeadOrAliveFriend;

                    if(!character_TargetMatchesType(player->pchar, e->pchar, target_type, false))
                        continue;
                }
                else
                {    
                    if( DeadOrAlive > 0)
                        target_type = kTargetType_Foe;
                    else if( DeadOrAlive < 0 )
                        target_type = kTargetType_DeadFoe;
                    else
                        target_type = kTargetType_DeadOrAliveFoe;

                    if(!character_TargetMatchesType(player->pchar, e->pchar, target_type, false))
                        continue;
                }
            }
        }

        if (!isEntitySelectable(e))
            continue;
        if (!selectEntityVisible(e))
            continue;

        if (BasePassiveOrNot>0 && baseedit_IsPassiveDetailEnt(e)) //If we're skipping non-threatening things
            continue;
        if (BasePassiveOrNot<0 && baseedit_IsPassiveDetailEnt(e))
            continue;

        if( MyPetOrNot<0 && !entIsPet(e) )
            continue;
        if( MyPetOrNot>0 && entIsPet(e) )
            continue;

        if( TeammateOrNot<0 && !team_IsMember(player, e->db_id) )
            continue;
        if( TeammateOrNot>0 && team_IsMember(player, e->db_id) )
            continue;

        // don't select this entity if a name's been given and it doesn't match or it's an enemy player
        if( name && name[0] && (!strstri(e->name,name) || (ENTTYPE(e) == ENTTYPE_PLAYER && character_TargetMatchesType(player->pchar, e->pchar, kTargetType_Foe, false))))
            continue;

        dist = distance3Squared(ENTPOS(player), ENTPOS(e));
        if (fallbackIdx==-1
            || !bBackwards && (dist < fallbackDist || dist == fallbackDist && e->svr_idx < fallbackIdx)
            || bBackwards && (dist > fallbackDist || dist == fallbackDist && e->svr_idx > fallbackIdx))
        {
            fallbackDist = dist;
            fallbackIdx = e->svr_idx;
        }

        if (!bBackwards && (dist < referenceDist || (dist == referenceDist && e->svr_idx < last_svr_idx))
            || bBackwards && (dist > referenceDist || (dist == referenceDist && e->svr_idx > last_svr_idx)))
        {
            continue;
        }

        if (best_idx!=-1  // We have a previous best
            && ( !bBackwards && (dist > bestDist || dist == bestDist && e->svr_idx > best_idx)// AND This one is father than the previous best
                || bBackwards && (dist < bestDist || dist == bestDist && e->svr_idx < best_idx)))
        {
            continue;
        }

        best_idx = e->svr_idx;
        bestDist = dist;
    }

    if (best_idx==-1)
    {
        if (fallbackIdx!=-1)
            return fallbackIdx;

        return last_svr_idx;
    }

    return best_idx;
}

void selectNearestTarget( int type, int dead )
{
    int            i,idx = -1;
    Entity        *e;
    float        smallestDist = FLT_MAX;
    Entity        *player = playerPtr();

    for( i=1; i<=entities_max; i++)
    {
        float dist;

        if (!(entity_state[i] & ENTITY_IN_USE ) )
            continue;

        e = entities[i];

        if ( ENTTYPE_BY_ID(i) != type || e == player )
            continue;
        if (!selectEntityVisible(e))
            continue;
        if (!isEntitySelectable(e))
            continue;

        if( dead )
        {
            if( !entMode( e, ENT_DEAD ) )
                continue;
        }
        else if ( entMode( e, ENT_DEAD ) )
            continue;

        dist = distance3Squared( ENTPOS(player), ENTPOS(e) );
        if ( dist < smallestDist )
        {
            idx = i;
            smallestDist = dist;
        }
    }

    if ( idx < 0 )
        current_target = 0;
    else
        current_target = entities[idx];

    gSelectedDBID = 0;
    gSelectedHandle[0] = 0;
}

EntityRef g_erTaunter;
EntityRef **g_pperPlacators;

void selectNearestTargetForPower(int iset, int ipow)
{
    Entity *pSrc = playerPtr();
    Power *ppow = pSrc->pchar->ppPowerSets[iset]->ppPowers[ipow];

    int            i,idx = -1;
    Entity        *e;
    float        smallestDist = FLT_MAX;

    for( i=1; i<=entities_max; i++)
    {
        float dist;

        if (!(entity_state[i] & ENTITY_IN_USE ) )
            continue;


        e = entities[i];

        if (ppow->ppowBase->eTargetType == kTargetType_Foe
            && g_erTaunter
            && g_erTaunter != erGetRef(e))
            continue;
        if (!character_PowerAffectsTarget(pSrc->pchar, e->pchar, ppow) || e == playerPtr())
            continue;
        if (!selectEntityVisible(e))
            continue;
        if (!isEntitySelectable(e))
            continue;

        dist = distance3Squared( ENTPOS(playerPtr()), ENTPOS(e) );
        if ( dist < smallestDist )
        {
            idx = i;
            smallestDist = dist;
        }
    }

    if ( idx < 0 )
        current_target = 0;
    else
        current_target = entities[idx];

    gSelectedDBID = 0;
    gSelectedHandle[0] = 0;
}

/* End of File */
