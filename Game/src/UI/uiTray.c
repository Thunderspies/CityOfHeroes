/***************************************************************************/

#include "UI/uiTray.h"
#include "varutils.h"
#include "player/player.h"
#include <utilitieslib/components/HashFunctions.h>
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "UI/uiCursor.h"
#include "UI/uiChat.h"
#include "UI/uiWindows.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "player/playerState.h"
#include "UI/uiGame.h"
#include "UI/uiUtilGame.h"
#include "UI/uiContextMenu.h"
#include "win/input.h"
#include "UI/uiPet.h"
#include "win/win_init.h"
#include "UI/uiUtilMenu.h"
#include "UI/uidialog.h"
#include "entity/entclient.h"
#include "assert.h"
#include "UI/uiInput.h"
#include "cmdparse/cmdgame.h"
#include "language/langClientUtil.h"
#include "entity/PowerInfo.h"
#include "entity/entity_power_client.h"           // for entity_ActivatePowerSend
#include "entity/character_base.h"
#include "entity/powers.h"
#include "gameComm/wdwbase.h"
#include "UI/uiConsole.h"
#include "gameComm/itemselect.h"             // for clect target
#include "graphics/camera.h"                 // for crazy define
#include "player/pmotion.h"                // for entMode
#include <utilitieslib/components/Earray.h>
#include "UI/uiUtil.h"
#include "UI/uiToolTip.h"
#include "UI/uiInspiration.h"
#include "UI/uiInfo.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/timing.h>
#include "UI/uiOptions.h"
#include "UI/uiKeybind.h"
#include "UI/uiEditText.h"
#include "UI/uiWindows_init.h"
#include "entity/character_target.h"
#include "entity/character_eval.h"
#include "UI/uiTeam.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/strings_opt.h>
#include "entity/EntPlayer.h"
#include "UI/uiReticle.h"
#include "clientcomm/clientcommreceive.h"
#include "entity/entity.h"
#include "gameComm/petCommon.h"
#include "entity/character_inventory.h"
#include "UI/uiPowerInventory.h"
#include "UI/uiTrade.h"
#include "uiGift.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/character_eval.h"
#include "entity/entworldcoll.h"
#include "UI/uiOptions.h"
#include "fxutil.h"
#include "graphics/ttFontUtil.h"

U32 g_timeLastPower = 0;
U32 g_timeLastAttack = 0;

static float    tray_timer;
static int      print_state;

// -AB: changed name from gShowAltTray to reflect new functionality :2005 Jan 27 09:37 AM
// -AB: refactored to be static local :2005 Feb 09 11:20 AM

static int             s_TrayStickyMask = 0;
static int             s_TrayMomentaryMask = 0; // a mask of AltTrayMomentaryMask type

TrayObj         new_empty_tray_obj = {0};
TrayObj         *s_ptsActivated = 0;
static TrayObj  s_toTemp = {0};
int                gMacroOnstack = 0;
F32                gfTargetConfusion = 0.5;

#define DONT_UPDATE_SERVER ( FALSE )
#define PLAYER_HEIGHT 6.0f

static void trayobj_select(Entity* e, TrayObj* ts, Vec3 loc);

//------------------------------------------------------------
// set the cur tray. currently used only by uiNet.c
// mode: the current mask shifted right by 1 (since primary is always active)
// see uiNet.c(461):receiveTrayMode
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
// -AB: net traffic optimization :2005 Feb 09 11:26 AM
//----------------------------------------------------------
void tray_setMode(int mode)
{
    assert( kCurTrayMask_Primary == 1 );
    mode = (mode << 1) | kCurTrayMask_Primary;
    assert(INRANGE(mode, 0, kCurTrayMask_ALL + 1));
    s_TrayStickyMask = mode;
}

//------------------------------------------------------------
// set the global s_TrayMomentaryMask to the passed
// value.
// -AB: created :2005 Jan 31 02:02 PM
//----------------------------------------------------------
void tray_momentary_alt_toggle( CurTrayMask mask, int toggle )
{
    if (toggle)
        s_TrayMomentaryMask |= mask;
    else
        s_TrayMomentaryMask &= ~mask;
}


//------------------------------------------------------------
// helper for incrementing the tray mode
// -AB: created :2005 Feb 08 05:20 PM
//----------------------------------------------------------
static int s_next_tray_mode( int tray_mask )
{    
    // --------------------
    // find the first un-set bit
    
    int i;
    for( i = 0; i <= kCurTrayType_Alt2; ++i )
    {
        int mask = 1<<i;
        
        if( !(mask & tray_mask) )
        {
            tray_mask |= mask;
            break;
        }
    }
    
    // --------------------
    // if not found, reset sticky to just show primary
    
    if( i > kCurTrayType_Alt2 )
    {
        tray_mask = kCurTrayMask_Primary;
    }

    // --------------------
    // finally, return

    return tray_mask;
}



//------------------------------------------------------------
// toggle the next tray mode. set the first non-set bit
// if all bits are set, clear it.
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
// -AB: with any alt tray capable of being sticky, just set the first non-set bit :2005 Feb 08 05:10 PM
// -AB: no need to send primary tray bit, this is always set :2005 Feb 09 11:24 AM
//----------------------------------------------------------
void tray_toggleMode()
{
    s_TrayStickyMask = s_next_tray_mode( s_TrayStickyMask );
    sendTrayMode( s_TrayStickyMask>>1 );
}

//-------------------------------------------------------------------------------------------------------
// TrayObj functions ////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------

/**********************************************************************func*
* trayobj_ActivatePower
*
*/
static void trayobj_ActivatePower(TrayObj *pto, Entity *e, Entity* ptarget)
{
    if (!verify(pto && e && e->pchar))
        return;
    g_timeLastAttack = g_timeLastPower = timerSecondsSince2000();
    inspirationUnqueue();

    START_INPUT_PACKET(pak, CLIENTINP_ACTIVATE_POWER);
    entity_ActivatePowerSend(e, pak, e->pchar->iCurBuild, pto->iset, pto->ipow, ptarget);
    END_INPUT_PACKET

    // Forcibly shut off the targetting cursor;
    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;
}

/**********************************************************************func*
* trayobj_ActivatePowerAtLocation
*
*/
static void trayobj_ActivatePowerAtLocation(TrayObj *pto, Entity *e, Entity *ptarget, Vec3 loc)
{
    if (!verify(pto && e && e->pchar))
        return;
    g_timeLastAttack = g_timeLastPower = timerSecondsSince2000();

    START_INPUT_PACKET(pak, CLIENTINP_ACTIVATE_POWER_AT_LOCATION);
    entity_ActivatePowerAtLocationSend(e, pak, e->pchar->iCurBuild, pto->iset, pto->ipow, ptarget, loc);
    END_INPUT_PACKET

    // Forcibly shut off the targetting cursor;
    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;
}

/**********************************************************************func*
* trayobj_SetDefaultPower
*
*/
void trayobj_SetDefaultPower(TrayObj *pto, Entity *e)
{
    if(pto)
    {
        if(e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow]->ppowBase->fInterruptTime!=0.0f)
        {
            addSystemChatMsg(textStd("PowerCannotAutoAttackWithInterruptible"), INFO_COMBAT_ERROR, 0);
        }
        else if (e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow]->ppowBase->eTargetType==kTargetType_Location
                || e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow]->ppowBase->eTargetTypeSecondary==kTargetType_Location
                || e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow]->ppowBase->eTargetType==kTargetType_Teleport
                || e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow]->ppowBase->eTargetTypeSecondary==kTargetType_Teleport)
        {
            addSystemChatMsg(textStd("PowerCannotAutoAttackWithLocation"), INFO_COMBAT_ERROR, 0);
        }
        else if(e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow]->ppowBase->eType!=kPowerType_Click)
        {
            addSystemChatMsg(textStd("PowerCannotAutoAttackWithNonClick"), INFO_COMBAT_ERROR, 0);
        }
        else
        {
            e->pchar->ppowDefault = e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow];

            START_INPUT_PACKET(pak, CLIENTINP_SET_DEFAULT_POWER);
            entity_SetDefaultPowerSend(e, pak, e->pchar->iCurBuild, pto->iset, pto->ipow);
            END_INPUT_PACKET
        }
    }
    else
    {
        e->pchar->ppowDefault = NULL;
        EMPTY_INPUT_PACKET(CLIENTINP_CLEAR_DEFAULT_POWER);
    }
}

/**********************************************************************func*
* trayobj_EnterStance
*
*/
static void trayobj_EnterStance(TrayObj *pto, Entity *e, bool bSend)
{
    Power *ppow;

    if (!verify(pto && e))
        return;

    ppow = e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow];

    if (!character_ModesAllowPower(e->pchar, ppow->ppowBase)
        || (!e->access_level && ppow->ppowBase->ppchActivateRequires && !chareval_Eval(e->pchar, ppow->ppowBase->ppchActivateRequires, ppow->ppowBase->pchSourceFile)))
    {
        return;
    }

    if(e->pchar->ppowStance != ppow)
    {
        if(bSend)
        {
            START_INPUT_PACKET(pak, CLIENTINP_STANCE);
            entity_EnterStanceSend(e, pak, e->pchar->iCurBuild, pto->iset, pto->ipow);
            END_INPUT_PACKET
        }
    }

    g_timeLastPower = timerSecondsSince2000();

    // Forcibly shut off the targetting cursor;
    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;
}

/**********************************************************************func*
* trayobj_ExitStance
*
*/
void trayobj_ExitStance(Entity *e)
{
    if(e->pchar->ppowStance!=NULL)
    {
        START_INPUT_PACKET(pak, CLIENTINP_STANCE);
        entity_ExitStanceSend(e, pak);
        END_INPUT_PACKET
    }

    e->pchar->ppowStance = NULL;
}

/**********************************************************************func*
* entity_CancelQueuedPower
*
*/
void entity_CancelQueuedPower(Entity *e)
{
    EMPTY_INPUT_PACKET(CLIENTINP_CANCEL_QUEUED_POWER);
}

/**********************************************************************func*
* entity_CancelWindupPower
*
*/
void entity_CancelWindupPower(Entity *e)
{
    EMPTY_INPUT_PACKET(CLIENTINP_CANCEL_WINDUP_POWER);
}

/**********************************************************************func*
 * tray_CancelQueuedPower
 *
 */
void tray_CancelQueuedPower(void)
{
    Entity *e = playerPtr();

    entity_CancelQueuedPower(e);
    inspirationUnqueue();

    // Forcibly shut off the targetting cursor;
    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;
}

/**********************************************************************func*
* trayobj_AssistCurrentTarget
*
*/
static void trayobj_AssistCurrentTarget(Entity *e)
{
    if(current_target)
    {
        char buf[256];

        sprintf(buf, "assist \"%s\"", current_target->name);
        cmdParse(buf);
    }
}


//
//
int trayobj_IsDefault(TrayObj *pto)
{
    Entity *e = playerPtr();
    if (!pto)
        return 0;
    return (e->pchar->ppowDefault
        && e->pchar->ppowDefault->idx==pto->ipow
        && e->pchar->ppowDefault->psetParent->idx==pto->iset);
}


//
//
static const char *trayobj_Name( TrayObj *obj )
{
    Entity  *e;

    e = playerPtr();
    
    if( obj && !shell_menu() && obj->type == kTrayItemType_Power)
    {
        const Power *power = character_GetPowerFromArrayIndices(e->pchar, obj->iset, obj->ipow);
        return power->ppowBase->pchDisplayName;
    }

    return "Nothing";
}

//
//
void trayobj_copy( TrayObj *dest, TrayObj *src, int update_server )
{
    int bUpdate = 0;

    if (!verify(dest && src))
        return;

    bUpdate = update_server && 
        (src->type == kTrayItemType_Power || src->type == kTrayItemType_Inspiration || src->type == kTrayItemType_MacroHideName || src->type == kTrayItemType_Macro ||
        dest->type == kTrayItemType_Power || dest->type == kTrayItemType_Inspiration || dest->type == kTrayItemType_MacroHideName || dest->type == kTrayItemType_Macro);

    memset( dest, 0, sizeof(TrayObj) );
    dest->ipow = src->ipow;
    dest->iset = src->iset;
    dest->ispec = src->ispec;
    dest->icat = src->icat;
    dest->itray = src->itray;
    dest->islot = src->islot;
    dest->type = src->type;
    dest->icon = src->icon;
    dest->tab = src->tab;
    dest->invIdx = src->invIdx;
    dest->nameSalvage = src->nameSalvage;
    dest->idDetail = src->idDetail;

    if( src->command )
        strcpy( dest->command, src->command );

    if( src->shortName )
        strcpy( dest->shortName, src->shortName );

    if( src->iconName )
        strcpy( dest->iconName, src->iconName );

    if( bUpdate )
    {
        START_INPUT_PACKET(pak, CLIENTINP_RECEIVE_TRAYS);
        sendTray( pak, playerPtr() );
        END_INPUT_PACKET
    }

    // make sure to add assigns here as you add members
    //STATIC_ASSERT(sizeof(*src) == 636); 
}

//
//
void trayobj_startDraggingEx(TrayObj *to, AtlasTex *icon, AtlasTex *overlay, float icon_scale)
{
    if (!to)
        return;
    if( optionGet(kUO_DisableDrag) && to->type == kTrayItemType_Power )
        return;

    // We're reusing the cursor structure and the icon references are
    // potentially going to change so it's just not safe to keep the
    // reticle active.
    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;

    trayobj_copy( &cursor.drag_obj, to, 1 );
    cursor.dragging             = TRUE;
    cursor.dragged_icon         = icon;
    cursor.dragged_icon_scale    = icon_scale;
    cursor.overlay              = overlay;
    cursor.overlay_scale        = icon_scale;

    // When you drag off of the server tray, copy instead of cut.
    if (to->icat != kTrayCategory_ServerControlled)
        to->type = kTrayItemType_None;

    if( cursor.drag_obj.type == kTrayItemType_Power )
        InfoBox( INFO_USER_ERROR, "DragPower", trayobj_Name( &cursor.drag_obj ) );

}

void trayobj_startDragging(TrayObj *to, AtlasTex *icon, AtlasTex *overlay)
{
    trayobj_startDraggingEx(to, icon, overlay, 1.0f);
}

//
//
void trayobj_stopDragging()
{
    cursor.dragging      = FALSE;
    memset( &cursor.drag_obj, 0, sizeof(TrayObj) );
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

//homeless create macro function
//
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
void macro_create(char *shortName, char *command, char *icon, int slot)
{
    TrayObj to = {0};
    Entity * e = playerPtr();
    assert(e && e->pchar && e->pl);

	buildMacroTrayObj(&to, command, shortName, icon, icon != NULL);

    if (slot >= 0)
        trayobj_copy(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, slot / TRAY_WD, slot % TRAY_WD, e->pchar->iCurBuild, true), &to, TRUE);
    else
    {
        TrayObj *iteratedTrayObj;
        resetTrayIterator(e->pl->tray, e->pchar->iCurBuild, kTrayCategory_PlayerControlled);
        while (getNextTrayObjFromIterator(&iteratedTrayObj))
        {
            if (!iteratedTrayObj || iteratedTrayObj->type == kTrayItemType_None)
            {
                if (!iteratedTrayObj)
                    createCurrentTrayObjViaIterator(&iteratedTrayObj);

                trayobj_copy(iteratedTrayObj, &to, TRUE);
                return;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------
// TraySlot functions ///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------

//
//
TrayObj *trayslot_Get( Entity *e, TrayCategory tray_cat, int tray_num, int slot_num )
{
    if( e && e->pchar && e->pl && tray_num >= 0 && slot_num >= 0 )
    {
        TrayObj *pto = getTrayObj(e->pl->tray, tray_cat, tray_num, slot_num, e->pchar->iCurBuild, false);
        if (!pto)
            return NULL;
        if(pto->type==kTrayItemType_Power)
        {
            if( pto->iset >= eaSize(&e->pchar->ppPowerSets))
                pto->iset = eaSize(&e->pchar->ppPowerSets)-1;

            if( pto->ipow >= eaSize(&e->pchar->ppPowerSets[pto->iset]->ppPowers))
                pto->ipow = eaSize(&e->pchar->ppPowerSets[pto->iset]->ppPowers)-1;

            if(e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow] == NULL)
            {
                // This power has been removed.
                clearTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, tray_num, slot_num, e->pchar->iCurBuild);
                return NULL;                
            }
        }
        return pto;
    }
    else
        return NULL;
}

//
//
static int inTray( TrayObj * power, int tray_num )
{
    int i;
    TrayObj * tmp;
    Entity *e = playerPtr();

    for( i = 0; i < TRAY_SLOTS; i++ )
    {
        tmp = trayslot_Get( e, kTrayCategory_PlayerControlled, tray_num, i );
        if (tmp && power && tmp->type == power->type && power->type == kTrayItemType_Power)
        {
            if( tmp->iset==power->iset && tmp->ipow==power->ipow )
                return true;
        }
    }

    return false;
}

//
//
static int hotKeyAttack()
{
    int i;

    // Don't process "hotkeyAttack" if the user is in the console or when the user is chatting
    if( !okToInput(0) )
        return 0;

    for( i = INP_1; i <= INP_0; i++ )
        if( inpEdge( i ) )
            return ( i - INP_1 ) + 1;


    return 0;
}

//
//
TrayObj *trayslot_find( Entity *e, const char *pch )
{
    int i, j;
    TrayObj *pto;

    // First try all the display names (since these are the ones most people will see)
    resetTrayIterator(e->pl->tray, e->pchar->iCurBuild, -1);
    while (getNextTrayObjFromIterator(&pto))
    {
        if (pto && pto->type == kTrayItemType_Power)
        {            
            Power *power = e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow];

            if(stricmp(textStd(power->ppowBase->pchDisplayName), pch)==0)
            {
                getCurrentTrayIteratorIndices(&pto->icat, &pto->itray, &pto->islot, NULL);
                return pto;
            }
        }
    }

    // That didn't work. Try the internal names.
    resetTrayIterator(e->pl->tray, e->pchar->iCurBuild, -1);
    while (getNextTrayObjFromIterator(&pto))
    {
        if (pto && pto->type == kTrayItemType_Power)
        {        
            Power *power = e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow];

            if( stricmp(power->ppowBase->pchName, pch)==0)
            {
                getCurrentTrayIteratorIndices(&pto->icat, &pto->itray, &pto->islot, NULL);
                return pto;
            }
        }
    }

    // OK, none of that worked. See if the power exists in the inventory
    // and make a temporary TrayObj for it.

    for(i=eaSize(&e->pchar->ppPowerSets)-1; i>=0; i--)
    {
        for(j=eaSize(&e->pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
        {
            Power *ppow = e->pchar->ppPowerSets[i]->ppPowers[j];
            if(ppow
                && (stricmp(ppow->ppowBase->pchName, pch)==0
                    || stricmp(textStd(ppow->ppowBase->pchDisplayName), pch)==0))
            {
                s_toTemp.ipow = j;
                s_toTemp.iset = i;
                s_toTemp.type = kTrayItemType_Power;
                s_ptsActivated = NULL;

                return &s_toTemp;
            }
        }
    }

    return NULL;
}

//
//
void trayslot_findAndSelect( Entity *e, const char *pch, int toggleonoff  )
{

    if(pch && pch[0]!='\0')
    {
        TrayObj *pto = trayslot_find(e, pch);

        if(pto)
        {
            PowerRef ppowRef;
            ppowRef.buildNum = e->pchar->iCurBuild;
            ppowRef.power = pto->ipow;
            ppowRef.powerSet = pto->iset;

            if (toggleonoff > 0 && powerInfo_PowerIsActive(e->powerInfo, ppowRef))
                return;

            if (toggleonoff < 0 && !powerInfo_PowerIsActive(e->powerInfo, ppowRef))
                return;

			trayobj_select(e, pto, NULL);
		}
	}
}

// Backsteps and returns length squared
static float rayCastBackstepHelper( const Vec3 src, Vec3 dest, float backsteplen )
{
	float fDistSqr;
	Vec3 dir;

	// Backstep slightly from hit location
	subVec3(dest, src, dir);				// Direction vector
	fDistSqr = lengthVec3Squared(dir);		// Get length
	normalVec3(dir);						// Normalize direction
	scaleVec3(dir, backsteplen, dir);		// Scale to backstep length
	subVec3(dest, dir, dest);				// Move back from hit along direction

	return fDistSqr;
}

static void rayCastForLocTarget( const Vec3 csrc, Vec3 loc, int hitNotSelect )
{
	Vec3 src, dest;
	bool bHit;

	// Cast a ray along the direction vector to make it stop at any geometry hit
	copyVec3(csrc, src);
	copyVec3(loc, dest);

	src[1] += .02;									// Fudge Y axis slightly so we're
													// not coplanar with the ground
	groupFindOnLine(src, dest, 0, &bHit, hitNotSelect);		// Will adjust dest if hit
	if (bHit)
	{
		float footDistSqr = rayCastBackstepHelper(src, dest, 0.1);

		// Try again with the head to skip over low ledges
		src[1] = csrc[1] + PLAYER_HEIGHT;
		loc[1] += PLAYER_HEIGHT;					// Use loc directly to not change dest
		groupFindOnLine(src, loc, 0, &bHit, hitNotSelect);
		if (!bHit || (rayCastBackstepHelper(src, loc, 0.1) > footDistSqr) ) {
			// We did better on this one, so cast a ray downward up to the
			// amount we fudged and return that
			copyVec3(loc, src);
			loc[1] -= PLAYER_HEIGHT;
			groupFindOnLine(src, loc, 0, &bHit, hitNotSelect);
			return;
		}

	}

	// else loc is good to use as-is
}

// Cast a ray straight down
static void floorPosition( Vec3 loc )
{
	Vec3 dest;
	bool bHit;

	copyVec3(loc, dest);
	dest[1] -= 20000;
	groupFindOnLine(loc, dest, 0, &bHit, 0);

	if (bHit)		// only change loc if we actually hit something
		copyVec3(dest, loc);
}

// Helper for followTerrain; overwrites dest
static float tryFollowAngle( const Vec3 csrc, const Vec3 dir, float angle, float dist, Vec3 dest )
{
	Vec3 src;
	Vec3 temp;						// Target of first ray, source of vertical drop
	bool bHit;

	// Use approximate eye position as source
	copyVec3(csrc, src);
	src[1] += PLAYER_HEIGHT * 5.0f / 6.0f;

	scaleVec3(dir, dist, temp);		// Scale out player facing
	addVec3(src, temp, temp);		// Translate into place
	temp[1] += tan(angle) * dist;	// Trignometry ftw

	// Cast the ray
	groupFindOnLine(src, temp, 0, &bHit, 0);
	if (bHit)									// go back slightly so we don't
		rayCastBackstepHelper(src, temp, 1.0);	// end up in a corner

	// Now cast a ray straight down
	copyVec3(temp, dest);
	dest[1] -= 20000;
	groupFindOnLine(temp, dest, 0, &bHit, 0);

	// Return the distance from original source
	subVec3(dest, csrc, temp);
	return lengthVec3(temp);
}

// Attempt to follow terrain to desired distance
static bool followTerrain( const Vec3 csrc, const Vec3 dir, float dist, Vec3 loc )
{
	Vec3 dest;
	float best = 0;
	float angle;

	// Try various angles. Kind of slow, but this is clientside
	// and we can afford it.
	for (angle = 0; angle >= -PI/4.0f; angle += PI/96.0f * (angle < 0 ? -1 : 1), angle *= -1)
	{
		int tries = 0;
		float trydist = dist;
		float absdist;

		while (tries <= 3 && trydist >= 1) {
			float lasttrydist = trydist;
			absdist = tryFollowAngle(csrc, dir, angle, trydist, dest);
			printf("tryFollowAngle(angle = %f, dist = %f); absdist = %f\n", DEG(angle), trydist, absdist);

			if (absdist <= dist)		// Probably hit something
				break;

			// absdist > dist, so let's try to reduce it
			trydist *= dist / absdist;					// triangles are proportial, yay
			trydist -= MIN(trydist * 0.05, 0.25);		// fudge slightly to not go over
			trydist = MIN(trydist, lasttrydist - 1);	// decrease by at least 1 to not stick on the cusp
			tries++;
		}

		if (absdist <= dist && absdist > best) {
			copyVec3(dest, loc);
			best = absdist;

			if (dist - absdist < 1.0f)
			{
				printf("EXACT(ish) absdist = %f; requested dist = %f\n", best, dist);
				return true;				// close enough
			}
		}
	}

	printf("best absdist = %f; requested dist = %f\n", best, dist);

	return (best > 0);
}

void trayslot_findAndSelectLocation( Entity *e, const char *locname, const char *pch )
{
	Mat4 lmat;
	Vec3 loc, origcam;
	TrayObj *pto;
	Power *power;
	bool bIsTeleport;
	char *lc = 0;

	// Be extra paranoid since this is only called from a slash command
	// and we can spare the cycles.

	if (!e || !pch || pch[0] == '\0' || !locname || locname[0] == '\0')
		return;

	pto = trayslot_find(e, pch);
	if (!pto)
		return;

	if (pto->type != kTrayItemType_Power)
		return;				// Shouldn't be possible but why risk it

	power = e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow];
	bIsTeleport = (power->ppowBase->eTargetType == kTargetType_Teleport
		|| power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport);

	if (!stricmp(locname, "me") || !stricmp(locname, "self"))
	{
		copyVec3(ENTPOS(e), loc);
		if(power->ppowBase->bTargetNearGround)
			floorPosition(loc);
	}
	else if (!stricmp(locname, "target"))
	{
		if (!current_target)
			return;			// TODO: Error message?
		copyVec3(ENTPOS(current_target), loc);
		if(power->ppowBase->bTargetNearGround)
			floorPosition(loc);
// Uncomment this if target mode turns out to be exploitable
//		if (bIsTeleport)
//			rayCastForLocTarget(ENTPOS(e), loc);		// Teleport must have LOS
	} else {
		// Fun fun, we're using directional mode
		Vec3 dir = { 0, 0, 0 };
		float dist = 0;
		bool bFollowTerrain = false;
		char *ds;

		lc = strdup(locname);

		ds = strchr(lc, ':');
		if (!ds || ds == lc)
			goto out;

		*(ds++) = '\0';		// Split the string, ds is the distance string
		if (!*ds)			// Make sure they didn't end the string with a colon
			goto out;

		if (!stricmp(lc, "up"))
			dir[1] = 1;
		else if (!stricmp(lc, "down"))
			dir[1] = -1;
		else if (!stricmp(lc, "camera")) {
			Vec3 camdir = { 0, 0, -1 };
			mulVecMat3(camdir, cam_info.cammat, dir);
		} else {
			// "SMRT" directions
			// First figure out unit vector for direction
			Vec3 facingmod = { 0, 0, 0 };
			if (!stricmp(lc, "forward"))
				facingmod[2] = 1;
			else if (!stricmp(lc, "back"))
				facingmod[2] = -1;
			else if (!stricmp(lc, "left"))
				facingmod[0] = -1;
			else if (!stricmp(lc, "right"))
				facingmod[0] = 1;
			else {
				float angle = atof(lc);
				facingmod[0] = sin(RAD(angle));
				facingmod[2] = cos(RAD(angle));
			}
			mulVecMat3(facingmod, ENTMAT(e), dir);		// Apply player's transform matrix

			// If player is near the ground, or this is a power that must be used on the ground,
			// engage terrain following mode
			if(power->ppowBase->bTargetNearGround || entHeight(e, 2.0f) <= 1.0f)
				bFollowTerrain = true;
		}

		if (!stricmp(ds, "max"))
			dist = power->fRangeLast;
		else
			dist = atof(ds);

		if (bFollowTerrain)
		{
			if (!followTerrain(ENTPOS(e), dir, dist, loc))
				goto out;							// TODO: Error message?
		}
		else
		{
			// Straight line
			scaleVec3(dir, dist, dir);				// Scale modified unit vector
			addVec3(ENTPOS(e), dir, loc);			// Add to player position

			// Make sure we don't hit geometry
			rayCastForLocTarget(ENTPOS(e), loc, bIsTeleport ? 1 : 0);
		}
	}

	copyMat4(unitmat, lmat);						// Calc*Target wants a matrix
	copyVec3(loc, lmat[3]);

	if(bIsTeleport)
	{
		F32 probe[6];

		// Create a probe vector from location back to the player for teleport check to
		// be able to backstep if necessary
		subVec3(ENTPOS(e), loc, probe);
		copyVec3(ENTPOS(e), &probe[3]);
		CalcTeleportTarget(lmat, loc, probe);

		// --- Slight hack ---
		// Temporarily change the camera position to one that we know will pass
		// ExtraTeleportChecks on the server. We've already done raycasting so this
		// shouldn't allow teleporting into geometry.
		copyVec3(cam_info.cammat[3], origcam);
		copyVec3(loc, cam_info.cammat[3]);
	} else
	{
		CalcLocationTarget(lmat, loc);
	}

	trayobj_select(e, pto, loc);

	if (bIsTeleport)
	{	// Restore original camera position
		copyVec3(origcam, cam_info.cammat[3]);
	}

out:
	if (lc)
		free(lc);
	return;
}

//
//
void trayslot_findAndSetDefaultPower( Entity *e, const char *pch )
{
    if(pch && pch[0]!='\0')
    {
        TrayObj *pto = trayslot_find(e, pch);

        if(pto)
        {
            // Toggle the auto power
            if(e->pchar->ppowDefault == e->pchar->ppPowerSets[pto->iset]->ppPowers[pto->ipow])
            {
                trayobj_SetDefaultPower(NULL, e);
            }
            else
            {
                trayobj_SetDefaultPower(pto, e);
            }
        }
    }
    else
    {
        trayobj_SetDefaultPower(NULL, e);
    }
}


static bool AliveEnough(Entity *e, Power *pow)
{
    bool bDead = entMode(e, ENT_DEAD);

    if(bDead)
        return g_abTargetAllowedDead[pow->ppowBase->eTargetType];
    else
        return g_abTargetAllowedAlive[pow->ppowBase->eTargetType];
}

//
//
static void trayobj_select(Entity *e, TrayObj *ts, Vec3 loc)
{
    if (!verify(ts))
        return;

    if(isMenu(MENU_GAME) )
    {
        if(ts->type == kTrayItemType_Power)
        {
            // Make sure to keep this section in sync with trayslot_drawIcon [see COH-24115]
            Power *power = e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow];

            if (!power)
                return;

            if( !character_AtLevelCanUsePower( e->pchar, power) )
            {
                return;
            }

            if( powerIsMarkedForTrade(power) )
            {
                addSystemChatMsg(textStd("CannotActivateTradePower", power->ppowBase->pchDisplayName), INFO_USER_ERROR, 0);
                return;
            }

            // Do NOT check modes or activation requires here!  Let the server enforce these.
            // Do this in case this is a second power in a macro where the first one
            // will change the state such that the second one becomes able to be activated.

            // If I'm confused
            //  and I have someone targeted
            //  and this power targets
            //   a friendly player or 
            //   a foe and I'm not taunted,
            // THEN see if we want to randomize my target
            if ( character_IsConfused(e->pchar)
                && current_target
                && (power->ppowBase->eTargetType == kTargetType_Player
                    || (power->ppowBase->eTargetType == kTargetType_Foe 
                        && !g_erTaunter)))
            {
                F32 frand = (F32)rand()/(F32)(RAND_MAX+1);
                if (frand<gfTargetConfusion)
                {
                    Entity * previous_target = current_target;
                    selectTarget(1,0,1,0,0,0,0,0);
                    if (current_target != previous_target)
                    {
                        handleFloatingInfo(e->svr_idx, "FloatConfused", kFloaterStyle_Info, 0.0, 0);
                        addSystemChatMsg(textStd("YouAreConfused"), INFO_COMBAT_ERROR, 0);
                    }
                }
            }
            
            // If this is a foe-targeted power and I'm taunted and I'm not targeting
            //  my taunter, cancel my current target (which will get set appropriately
            //  later in the function)
            if ( power->ppowBase->eTargetType == kTargetType_Foe 
                && g_erTaunter
                && current_target != erGetEnt(g_erTaunter))
            {
                current_target = NULL;
                handleFloatingInfo(e->svr_idx, "FloatTaunted", kFloaterStyle_Info, 0.0, 0);
                addSystemChatMsg(textStd("YouAreTaunted"), INFO_COMBAT_ERROR, 0);
            }
            
            if ( power->ppowBase->eType == kPowerType_Click)
            {
                if( power->ppowBase->eTargetType == kTargetType_Caster )
                {
                    if( power->ppowBase->eTargetTypeSecondary != kTargetType_Location
                        && power->ppowBase->eTargetTypeSecondary != kTargetType_Teleport )
                    {
                        trayobj_ActivatePower(ts, e, e );
                        return;
                    }
                    					else if (loc)
					{
						trayobj_ActivatePowerAtLocation(ts, e, e, loc);
					}
                    else
                    {
                        if(s_ptsActivated && s_ptsActivated->ipow==ts->ipow && s_ptsActivated->iset==ts->iset)
                        {
                            cursor.target_world = kWorldTarget_None;
                            s_ptsActivated = NULL;
                        }
                        else
                        {
                            cursor.target_world = kWorldTarget_Normal;
                            s_ptsActivated = ts;
                        }
                    }
                }
                // am I in this stance already?
                else if( e->pchar->ppowStance == e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow] )
                {
                    if(power->ppowBase->eTargetType == kTargetType_Location
                        || power->ppowBase->eTargetType == kTargetType_Teleport
                        || power->ppowBase->eTargetTypeSecondary == kTargetType_Location
                        || power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport)
                    {
                        if(!current_target
                            && (power->ppowBase->eTargetTypeSecondary == kTargetType_Location
                                || power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport))
                        {
                            selectNearestTargetForPower(ts->iset, ts->ipow);
                            if(!current_target)
                            {
                                addSystemChatMsg(textStd("PowerCannotFindTarget"), INFO_COMBAT_ERROR, 0);
                            }
                        }

                        if(power->ppowBase->eTargetType == kTargetType_Location
                            || power->ppowBase->eTargetType == kTargetType_Teleport
                            || current_target)
                        {
							if (loc)
							{
								trayobj_ActivatePowerAtLocation(ts, e, current_target ? current_target : e, loc);
							}
							else if(s_ptsActivated && s_ptsActivated->ipow==ts->ipow && s_ptsActivated->iset==ts->iset)
                            {
                                cursor.target_world = kWorldTarget_None;
                                s_ptsActivated = NULL;
                            }
                            else
                            {
                                cursor.target_world = kWorldTarget_Normal;
                                s_ptsActivated = ts;
                            }
                        }
                    }
                    else if(e->pchar->erAssist!=0
                        && (!current_target
                            || current_target==erGetEnt(e->pchar->erAssist))) // Does the player Already have someone they are assisting?
                    {
                        trayobj_ActivatePower(ts, e, NULL);
                    }
                    else if(current_target && AliveEnough(current_target, power)) // Does the player Already have a Target?
                    {
                        trayobj_ActivatePower(ts, e, current_target );
                    }
                    else
                    {
                        selectNearestTargetForPower(ts->iset, ts->ipow);
                        if ( current_target ) // acquire target
                        {
                            // attack
                            if(power->ppowBase->eTargetType == kTargetType_Location
                                || power->ppowBase->eTargetType == kTargetType_Teleport
                                || power->ppowBase->eTargetTypeSecondary == kTargetType_Location
                                || power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport)
                            {
                                if(s_ptsActivated && s_ptsActivated->ipow==ts->ipow && s_ptsActivated->iset==ts->iset)
                                {
                                    cursor.target_world = kWorldTarget_None;
                                    s_ptsActivated = NULL;
                                }
                                else if (loc)
								{
									trayobj_ActivatePowerAtLocation(ts, e, current_target, loc);
								}
                                else
                                {
                                    cursor.target_world = kWorldTarget_Normal;
                                    s_ptsActivated = ts;
                                }
                            }
                            else
                            {
                                trayobj_ActivatePower(ts, e, current_target );
                            }
                        }
                    }
                }
                else
                {
                    if(power->ppowBase->eTargetType == kTargetType_Location
                        || power->ppowBase->eTargetType == kTargetType_Teleport
                        || power->ppowBase->eTargetTypeSecondary == kTargetType_Location
                        || power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport)
                    {
                        if(!current_target
                            && (power->ppowBase->eTargetTypeSecondary == kTargetType_Location
                                || power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport))
                        {
                            selectNearestTargetForPower(ts->iset, ts->ipow);
                            if(!current_target)
                            {
                                addSystemChatMsg(textStd("PowerCannotFindTarget"), INFO_COMBAT_ERROR, 0);
                            }
                        }

                        if(power->ppowBase->eTargetType == kTargetType_Location
                            || power->ppowBase->eTargetType == kTargetType_Teleport
                            || current_target)
                        {
                            							if(loc)
							{
								trayobj_ActivatePowerAtLocation(ts, e, current_target ? current_target : e, loc);
							}
							else if(s_ptsActivated && s_ptsActivated->ipow==ts->ipow && s_ptsActivated->iset==ts->iset)
                            {
                                cursor.target_world = kWorldTarget_None;
                                s_ptsActivated = NULL;
                            }
                            else
                            {
                                cursor.target_world = kWorldTarget_Normal;
                                s_ptsActivated = ts;
                            }
                        }
                    }
                    else if(e->pchar->erAssist!=0
                        && (!current_target
                            || current_target==erGetEnt(e->pchar->erAssist))) // Does the player Already have someone they are assisting?
                    {
                        trayobj_EnterStance(ts, e, false);
                        trayobj_ActivatePower(ts, e, NULL);
                    }
                    else if(current_target)
                    {
                        trayobj_EnterStance(ts, e, false);

                        if(AliveEnough(current_target, power))
                            trayobj_ActivatePower(ts, e, current_target);
                        else
                            selectNearestTargetForPower(ts->iset, ts->ipow);
                    }
                    else
                    {
                        trayobj_EnterStance(ts, e, true);
                        selectNearestTargetForPower(ts->iset, ts->ipow);
                    }
                }
            }
            else if ( power->ppowBase->eType == kPowerType_Toggle )
            {
                if( power->ppowBase->eTargetType == kTargetType_Caster ) // if the power targets caster auto-fire it without changing current target
                {
                    trayobj_ActivatePower(ts, e, e );
                }
                else if(power->ppowBase->eTargetType == kTargetType_Location
                    || power->ppowBase->eTargetType == kTargetType_Teleport)
                {
                    if(trayslot_IsActive(e, ts))
                    {
                        trayobj_ActivatePower(ts, e, NULL);
                    }
                    else if (loc)
					{
						trayobj_ActivatePowerAtLocation(ts, e, e, loc);
					}
                    else
                    {
                        cursor.target_world = kWorldTarget_Normal;
                        s_ptsActivated = ts;
                    }
                }
                else if(e->pchar->erAssist!=0
                    && (!current_target
                        || current_target==erGetEnt(e->pchar->erAssist))) // Does the player Already have someone they are assisting?
                {
                    trayobj_ActivatePower(ts, e, NULL);
                }
                else if(current_target && AliveEnough(current_target, power))
                {
                    trayobj_ActivatePower( ts, e, current_target );
                }
                else
                {
                    selectNearestTargetForPower(ts->iset, ts->ipow);
                    if(current_target)
                    {
                        trayobj_ActivatePower( ts, e, current_target );
                    }
                    else
                        trayobj_ActivatePower(ts, e, NULL);
                }

            }
        }
        else if ( IS_TRAY_MACRO(ts->type) )
        {
            gMacroOnstack = 1;
            cmdParse( ts->command );
            gMacroOnstack = 0;
        }
    }
}

//
//
void trayslot_select(Entity *e, TrayCategory trayCategory, int trayNumber, int slotNumber)
{
    TrayObj  *ts;
    int type = ENTTYPE_NOT_SPECIFIED;

    ts = trayslot_Get(e, trayCategory, trayNumber, slotNumber);
    if (ts)
		trayobj_select(e, ts, NULL);
}


//------------------------------------------------------------
// helper: check the passed slot type
// -AB: created :2005 Jan 26 03:25 PM
//----------------------------------------------------------
bool trayslot_istype( Entity *e, int slot_num, int tray_num, TrayItemType type)
{
    int checktype;
    assert(e->pchar);
    checktype = typeTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, tray_num, slot_num, e->pchar->iCurBuild);
    return checktype == type;
}

//
//
int trayslot_IsActive(Entity *e, TrayObj * to)
{    
    if( to && to->type == kTrayItemType_Power )
    {
        PowerRef ppowRef;
        ppowRef.buildNum = e->pchar->iCurBuild;
        ppowRef.power = to->ipow;
        ppowRef.powerSet = to->iset;

        return powerInfo_PowerIsActive(e->powerInfo, ppowRef);
    }

    return 0;
}

//
//
void trayslot_Timer(Entity *e, TrayObj * to, float * sc)
{
    if( to && to->type == kTrayItemType_Power )
    {
        PowerRef ppowRef;
        PowerRechargeTimer * prt;

        ppowRef.buildNum = e->pchar->iCurBuild;
        ppowRef.power = to->ipow;
        ppowRef.powerSet = to->iset;

        prt = powerInfo_PowerGetRechargeTimer(e->powerInfo, ppowRef);

        if( prt )
        {
            *sc = 1.0-(prt->rechargeCountdown /
                e->pchar->ppPowerSets[to->iset]->ppPowers[to->ipow]->ppowBase->fRechargeTime);
        }

        // rechargeCountdown can be greater than the RechargeTime if the player
        // is debuffed.
        // Make sure the scale doesn't get too small (or negative).
        if(*sc < 0.1f)
        {
            *sc = 0.1f;
        }
    }
}

void trayslot_RemainingTime(Entity *e, TrayObj * to, char * remainingTime)
{
    if( to && to->type == kTrayItemType_Power )
    {
        PowerRef ppowRef;
        PowerRechargeTimer * prt;

        ppowRef.buildNum = e->pchar->iCurBuild;
        ppowRef.power = to->ipow;
        ppowRef.powerSet = to->iset;

        prt = powerInfo_PowerGetRechargeTimer(e->powerInfo, ppowRef);

        if (prt)
        {
            if (prt->rechargeCountdown >= 10.f)
            {
                if (prt->rechargeCountdown > 60.f)
                {
                    sprintf(remainingTime, "%.0f:%02.0f", floor(prt->rechargeCountdown / 60.f), floor(fmod(prt->rechargeCountdown, 60.f)));
                }
                else
                {
                    sprintf(remainingTime, "%.0f", floor(prt->rechargeCountdown));
                }
            }
            else
            {
                sprintf(remainingTime, "%.1f", prt->rechargeCountdown);
            }
        }
    }
}

//
//
void traySlot_fireFlash( TrayObj * ts, float x, float y, float z )
{
    AtlasTex *glow = atlasLoadTexture("costume_button_link_glow.tga");
    AtlasTex *star = atlasLoadTexture("sharpstar.tga");

    int clr = 0xffff8800;
    int clr2 = 0xffffff00;
    float angle;
    float sc;

    if (!ts)
        return;

    ts->flash_scale += TIMESTEP *.15f;
    if( ts->flash_scale > 2 )
    {
        ts->flash_scale = 0;
        ts->state = 0;
    }

    sc = ts->flash_scale;
    clr |= (int)(((2-sc)/2)*255);
    clr2 |= (int)( 128 + ((2-sc)/2)*127);
    angle = ((2-sc)/2)*(PI);

    display_rotated_sprite( star, x - sc*star->width/2, y - sc*star->height/2, z+10, sc, sc, clr, angle, 1 );
    display_rotated_sprite( star, x - .9f*sc*star->width/2, y - .9f*sc*star->height/2, z+11, .9f*sc, .9f*sc, clr2, -angle, 1 );

}

//-------------------------------------------------------------------------------------------------------
// Tray ContextMenu /////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------

static ContextMenu *gPowerContext = 0;
static ContextMenu *gGenericTrayInfo = 0;

void trayInfo_GenericContext( CBox *box, TrayObj * to )
{
    int rx, ry;
    int wd, ht;
    CMAlign alignVert;
    CMAlign alignHoriz;

    if(!gGenericTrayInfo)
    {
        gGenericTrayInfo = contextMenu_Create(NULL);
        contextMenu_addVariableTitle( gGenericTrayInfo, traycm_PowerText, 0);
        contextMenu_addVariableText( gGenericTrayInfo, traycm_PowerInfo, 0);
        contextMenu_addCode( gGenericTrayInfo, alwaysAvailable, 0, traycm_Info, 0, "CMInfoString",    0  );
    }

    windowClientSizeThisFrame( &wd, &ht );

    rx = (box->lx+box->hx)/2;
    alignHoriz = CM_CENTER;

    ry = box->ly<ht/2 ? box->hy : box->ly;
    alignVert = box->ly<ht/2 ? CM_TOP : CM_BOTTOM;

    contextMenu_setAlign( gGenericTrayInfo, rx, ry, alignHoriz, alignVert, to );
}

const char *traycm_PowerText( void *data )
{
    Entity *e = playerPtr();
    TrayObj *ts = (TrayObj*)data;

     if (!ts)
        return "UNKNOWN";

    if( ts->type == kTrayItemType_Power )
        return e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppowBase->pchDisplayName;

    if( ts->type == kTrayItemType_Inspiration )
    {
        if( e->pchar->aInspirations[ts->iset][ts->ipow] )
            return e->pchar->aInspirations[ts->iset][ts->ipow]->pchDisplayName;
        else
            return "EmptySlot";
    }

    if( ts->type == kTrayItemType_SpecializationInventory )
    {
        if( e->pchar->aBoosts[ts->ispec] )
            return e->pchar->aBoosts[ts->ispec]->ppowBase->pchDisplayName;
        else
            return "EmptySlot";
    }

    if( ts->type == kTrayItemType_SpecializationPower )
    {
        if( e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppBoosts[ts->ispec] )
            return e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppBoosts[ts->ispec]->ppowBase->pchDisplayName;
        else
            return "EmptySlot";
    }

    if( ts->type == kTrayItemType_StoredEnhancement )
    {
        return ts->enhancement->ppowBase->pchDisplayName;
    }

    if( ts->type == kTrayItemType_StoredInspiration )
    {
        return ts->ppow->pchDisplayName;
    }

    if( IS_TRAY_MACRO(ts->type) )
        return ts->shortName;
    if( ts->type  == kTrayItemType_Salvage)
    {
        const SalvageItem *sal = e->pchar->salvageInv[ts->invIdx]->salvage;
        return sal->ui.pchDisplayName;
    }
    return "Unknown";
}

const char *traycm_PowerInfo( void *data )
{
    Entity *e = playerPtr();
    TrayObj *ts = (TrayObj*)data;

     if (!ts)
        return "UNKNOWN";

    if( ts->type == kTrayItemType_Power )
        return e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppowBase->pchDisplayShortHelp;

    if( ts->type == kTrayItemType_Inspiration )
    {
        if( e->pchar->aInspirations[ts->iset][ts->ipow] )
            return e->pchar->aInspirations[ts->iset][ts->ipow]->pchDisplayShortHelp;
        else
            return "EmptySlot";
    }

    if( ts->type == kTrayItemType_SpecializationInventory )
    {
        if( e->pchar->aBoosts[ts->ispec] )
            return e->pchar->aBoosts[ts->ispec]->ppowBase->pchDisplayShortHelp;
        else
            return "EmptySlot";
    }

    if( ts->type == kTrayItemType_SpecializationPower )
    {
        if( e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppBoosts[ts->ispec] )
            return e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppBoosts[ts->ispec]->ppowBase->pchDisplayShortHelp;
        else
            return "EmptySlot";
    }
    if( ts->type == kTrayItemType_Macro)
        return ts->command;

    if( ts->type == kTrayItemType_StoredEnhancement )
    {
        return ts->enhancement->ppowBase->pchDisplayShortHelp;
    }

    if( ts->type == kTrayItemType_StoredInspiration )
    {
        return ts->ppow->pchDisplayShortHelp;
    }

    if( ts->type == kTrayItemType_MacroHideName)
    {
        return masterMindHackGetHelpFromCommandName(ts->command);
    }
    if( ts->type  == kTrayItemType_Salvage)
    {
        const SalvageItem *sal = e->pchar->salvageInv[ts->invIdx]->salvage;
        return sal->ui.pchDisplayShortHelp;
    }


    return "Unknown";
}

static int traycm_translate( void * data )
{
    TrayObj *ts = (TrayObj*)data;

    if( !ts || ! ts->type )
        return true;

    if( IS_TRAY_MACRO(ts->type) )
        return false;
    else
        return true;
}

int traycm_showExtra(void *data)
{
    TrayObj *ts = (TrayObj*)data;
    Entity *e = playerPtr();
    Power *pow;

    if( !ts || ! ts->type )
        return false;

    if (ts->type != kTrayItemType_Power)
        return false;

    pow = e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow];

    if ( !pow || !pow->ppowBase->bHasUseLimit)
        return false;
    
    return true;
}

const char *traycm_PowerExtra(void *data)
{
    TrayObj *ts = (TrayObj*)data;
    Entity *e = playerPtr();
    Power *pow;    
    static char info[128];
    static char time[128];

    if( !ts || ! ts->type )
        return "UNKNOWN";

    if (ts->type != kTrayItemType_Power)
        return "UNKNOWN";

    pow = e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow];

    if ( !pow || !pow->ppowBase->bHasUseLimit)
        return "UNKNOWN";
    
    if (pow->ppowBase->iNumCharges)
    {        
        sprintf(info,"%s",textStd("PowerUsesLeft",pow->iNumCharges));
    }
    else if (pow->ppowBase->fUsageTime)
    {
        timerMakeOffsetStringFromSeconds(time,pow->fUsageTime);        
        sprintf(info,"%s",textStd("PowerTimeLeft",time));
    }
    return info;
}



//------------------------------------------------------------
// 
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
//----------------------------------------------------------
void traycm_Remove( void * data )
{
    TrayObj *ts = (TrayObj*)data;
    TrayObj empty = {0};
    Entity * e = playerPtr();
    TrayObj *to;
    int i, j;

    assert(e->pchar);
    i = ts->itray;
    j = ts->islot;

    to = getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, i, j, e->pchar->iCurBuild, false);

    if (!to)
    {
        // do nothing
    }
    else if ( ts->type == kTrayItemType_Power && to->iset == ts->iset && to->ipow == ts->ipow )
    {
        if( s_ptsActivated || cursor.target_world )
        {
            cursor.target_world = kWorldTarget_None;
            s_ptsActivated = NULL;
        }
        trayobj_copy( to, &empty, 1 );
    }
    else if ( IS_TRAY_MACRO(ts->type) && IS_TRAY_MACRO(to->type) && !(stricmp(ts->command, to->command)) && !(stricmp(ts->shortName, to->shortName)) )
    {
        trayobj_copy( to, &empty, 1 );
    }
}

static void tray_AddCurrentTrayObj( TrayObj *to );

void traycm_Add( void *data )
{
    TrayObj * ts = (TrayObj*)data;
        
    if(!power_getVisibleTrayObj( ts, NULL, NULL))
        tray_AddCurrentTrayObj( ts );
}

int traycm_RemoveAvailable(void *data)
{
    TrayObj *ts = (TrayObj *)data;

    if (!ts || ts->icat == kTrayCategory_ServerControlled)
        return CM_HIDE;

    return CM_AVAILABLE;
}

int traycm_InfoAvailable( void * data )
{
    TrayObj *ts = (TrayObj*)data;
    if (!ts)
        return CM_HIDE;

    if( IS_TRAY_MACRO(ts->type) )
        return CM_HIDE;

    return CM_AVAILABLE;
}

void traycm_Info( void * data )
{
    Entity *e = playerPtr();
    TrayObj *ts = (TrayObj*)data;

    if (!ts)
        return;

    switch( ts->type )
    {
        case kTrayItemType_Power:
        {
            info_SpecificPower(ts->iset, ts->ipow);
        }break;

        case kTrayItemType_Inspiration:
        {
            info_Power(e->pchar->aInspirations[ts->iset][ts->ipow], 0);
        }break;

        case kTrayItemType_SpecializationInventory:
        {
            if( e->pchar->aBoosts[ts->ispec] )
                info_CombineSpec(e->pchar->aBoosts[ts->ispec]->ppowBase, 1, e->pchar->aBoosts[ts->ispec]->iLevel, e->pchar->aBoosts[ts->ispec]->iNumCombines );
        }break;

        case kTrayItemType_SpecializationPower:
        {
            Boost * pboost =  e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppBoosts[ts->ispec];
            info_CombineSpec(pboost->ppowBase, 0, pboost->iLevel, pboost->iNumCombines );
        }break;
        case kTrayItemType_Salvage:
        {
            const SalvageItem *sal = e->pchar->salvageInv[ts->invIdx]->salvage;
            info_Salvage(sal);
        }break;
    }
}

int traycm_DeletableTempPower( void * data )
{
    TrayObj *ts = (TrayObj*)data;

    if (ts && ts->type == kTrayItemType_Power )
    {
        Power * ppow = playerPtr()->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow];
        if(ppow && stricmp( ppow->psetParent->psetBase->pchName, "Temporary_Powers" ) == 0)
        {
            if(ppow->ppowBase->bDeletable)
                return CM_AVAILABLE;
        }
    }
    return CM_HIDE;
}

static int iDeletePow=0;

static void deleteTempPower(void * data)
{
    sendPowerDelete(iDeletePow);
}

void traycm_DeletePower( void * data )
{
    TrayObj *ts = (TrayObj*)data;
    Power * ppow = playerPtr()->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow];

    if( traycm_DeletableTempPower(data) != CM_AVAILABLE )
        return;
    
    iDeletePow = ts->ipow;
    dialogStd( DIALOG_YES_NO, textStd("ReallyDeletePower", ppow->ppowBase->pchDisplayName), 0, 0, deleteTempPower, 0, 0 );
}

int traycm_IsMacro( void * data )
{
    TrayObj *ts = (TrayObj*)data;

    if (!ts)
        return CM_HIDE;

    if( IS_TRAY_MACRO(ts->type) )
        return CM_AVAILABLE;

    return CM_HIDE;
}

void updateMacro(TrayObj *macroUpdateObj)
{
    if( macroUpdateObj )
    {
        strcpy( macroUpdateObj->command, dialogGetTextEntry() );
        dialogClearTextEntry();

        START_INPUT_PACKET(pak, CLIENTINP_RECEIVE_TRAYS);
        sendTray( pak, playerPtr() );
        END_INPUT_PACKET
    }
}

void traycm_EditMacro( void * data )
{
    Entity * e = playerPtr();
    TrayObj *ts = (TrayObj*)data;
    TrayObj *update = 0;
    int i, j;
    TrayObj *to;

    if (!ts)
        return;

    assert(e && e->pchar && e->pl);
     i = ts->itray;
    j = ts->islot;

    to = getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, i, j, e->pchar->iCurBuild, false);
    
    if (to && IS_TRAY_MACRO(ts->type) && IS_TRAY_MACRO(to->type) && stricmp(ts->command, to->command)==0 && stricmp(ts->shortName, to->shortName)==0 )
        update = to;

     if( update && IS_TRAY_MACRO(update->type) )
    {
        dialogRemove(NULL,updateMacro,NULL);
        dialogSetTextEntry( update->command );
        dialog(DIALOG_OK_CANCEL_TEXT_ENTRY, -1, -1, -1, -1,
            textStd("MacroEdit", update->shortName ), NULL, updateMacro, NULL, NULL,
            DLGFLAG_GAME_ONLY|DLGFLAG_NO_TRANSLATE, NULL, NULL, 0, 0, 255, update );
    }
}
//-------------------------------------------------------------------------------------------------------
// Tray functions ///////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------
//
//
void tray_init(void)
{
    s_ptsActivated = 0;
    cursor.target_world = kWorldTarget_None;
}


//
//
void tray_dragPower( int i, int j, AtlasTex * icon )
{
    TrayObj  tmp;

    buildPowerTrayObj(&tmp, i, j, 0);

    trayobj_startDragging( &tmp, icon, NULL );
    cursor.drag_obj.islot = -1;         // tag the slot this power is comming from
    cursor.drag_obj.itray = -1;
    cursor.drag_obj.icat = -1;
}


//
//
//------------------------------------------------------------
// 
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
//----------------------------------------------------------
static void tray_AddCurrentTrayObj( TrayObj *to )
{
    int i;
    int cur_tray_i;
    Entity *e = playerPtr();

    if (!to)
        return;

    assert(e && e->pchar && e->pl);
    cur_tray_i = get_cur_tray(e);

    for( i = 0; i < TRAY_SLOTS; i++ )
    {
        if( trayslot_istype( e, i, e->pl->tray->current_trays[cur_tray_i], kTrayItemType_None ) )
        {
            trayobj_copy(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, e->pl->tray->current_trays[cur_tray_i], i, e->pchar->iCurBuild, true), to, TRUE);
            return;
        }
    }
}

void tray_AddObj( TrayObj *to )
{
    int i;
    int cur_tray_i;
    Entity *e = playerPtr();

    // COH-16680: Also look for entities which have not been populated yet.
    // Ideally, the server side should be fixed to not send SERVER_SEND_TRAY_ADD
    // before the entity has been sent to the client
    if (!e || !e->pchar || !e->pl || e->auth_name[0] == 0)
        return;

    for( cur_tray_i = 0; cur_tray_i < kCurTrayType_Count; ++cur_tray_i )
    {
        // try current tray first
        for( i = 0; i < TRAY_SLOTS; i++ )
        {            
            if( trayslot_istype( e, i, e->pl->tray->current_trays[cur_tray_i], kTrayItemType_None ) )
            {
                trayobj_copy(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, e->pl->tray->current_trays[cur_tray_i], i, e->pchar->iCurBuild, true), to, TRUE);
                return;
            }
        }
    }
}

void tray_AddObjTryAt(TrayObj *to, int trayIndex)
{
    int i;
    int cur_tray_i;
    Entity *e = playerPtr();

    if (!e || !e->pchar || !e->pl)
        return;

    for( cur_tray_i = 0; cur_tray_i < kCurTrayType_Count; ++cur_tray_i )
    {
        if( trayslot_istype( e, trayIndex, e->pl->tray->current_trays[cur_tray_i], kTrayItemType_None ) )
        {
            trayobj_copy(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, e->pl->tray->current_trays[cur_tray_i], trayIndex, e->pchar->iCurBuild, true), to, TRUE );
            return;
        }

        for( i = 0; i < TRAY_SLOTS; i++ )
        {            
            if( trayslot_istype( e, i, e->pl->tray->current_trays[cur_tray_i], kTrayItemType_None ) )
            {
                trayobj_copy(getTrayObj(e->pl->tray, kTrayCategory_PlayerControlled, e->pl->tray->current_trays[cur_tray_i], i, e->pchar->iCurBuild, true), to, TRUE );
                return;
            }
        }
    }
}

//
//
static AtlasTex *tray_GetIcon(TrayObj *pto, Entity *e)
{
    static AtlasTex *defaultTex=NULL;
    AtlasTex *ptex;
    if (defaultTex==NULL) {
        defaultTex = atlasLoadTexture("default_tray.tga");
    }
    ptex = defaultTex;

    if (!pto)
        return ptex;

    if(pto->type == kTrayItemType_Power)
    {
        const Power *power = character_GetPowerFromArrayIndices(e->pchar, pto->iset, pto->ipow);
        ptex = basepower_GetIcon(power->ppowBase);
    }
    else if(pto->type == kTrayItemType_Inspiration)
    {
        if(pto->iset>=0
            && pto->ipow >= 0
            && pto->iset < e->pchar->iNumInspirationCols
            && pto->ipow < e->pchar->iNumInspirationRows)
        {
            const BasePower *power = e->pchar->aInspirations[pto->iset][pto->ipow];
            if(power!=NULL)
            {
                ptex = basepower_GetIcon(power);
            }
        }
    }
    else if ( IS_TRAY_MACRO(pto->type) )
    {
        if( pto->iconName && pto->iconName[0]  )
            ptex = atlasLoadTexture( pto->iconName );
        else
            ptex = atlasLoadTexture( "Macro_Button_01.tga" );
    }

    return ptex;
}


static void tray_verifyValidPowers(void)
{
    Entity *e = playerPtr();
    TrayObj *trayj;

    if (!e || !e->pchar)
        return;

    resetTrayIterator(e->pl->tray, e->pchar->iCurBuild, kTrayCategory_PlayerControlled);
    while (getNextTrayObjFromIterator(&trayj))
    {
        if(!trayj)
            continue;

        if( trayj->type == kTrayItemType_Inspiration )
            destroyCurrentTrayObjViaIterator();

        if( trayj->type == kTrayItemType_Power)
        {
            int iset = trayj->iset;
            int ipow = trayj->ipow;

            if(iset<0
                || iset>=eaSize(&e->pchar->ppPowerSets)
                || ipow<0
                || ipow>=eaSize(&e->pchar->ppPowerSets[iset]->ppPowers))
            {
                destroyCurrentTrayObjViaIterator();
            }
            else
            {
                const Power *ppow = character_GetPowerFromArrayIndices(e->pchar, iset, ipow);
                if(!ppow || !ppow->ppowBase || ppow->ppowBase->eType == kPowerType_Auto)
                {
                    destroyCurrentTrayObjViaIterator();
                }
            }
        }
    }
}

enum
{
    TRAY_HORIZONTAL = 0,
    TRAY_VERTICAL,
};

#define SPACER              2
#define ICON_SCALE_SPEED    .1f


static ToolTip traySlotToolTip;
static ToolTipParent trayTipParent = {0};

typedef struct ActivePowerTimer
{
    const BasePower * power;
    float        angle;
    int            rotatedThisTick;
} ActivePowerTimer;

static ActivePowerTimer **activePowerTimers = {0};

void power_clearActive()
{
    int i;

    if( !activePowerTimers )
        return;

    for( i = 0; i < eaSize(&activePowerTimers); i++ )
    {
        activePowerTimers[i]->rotatedThisTick = 0;
    }
}

#define SPIN_TOGGLE_SCALE    .05f
#define SPIN_CLICK_SCALE    .01f

static float activePowerTimer_getAngle( const BasePower * pow )
{
    int i;
    ActivePowerTimer * apt;

    if( !activePowerTimers )
        eaCreate( &activePowerTimers );

    for( i = 0; i < eaSize(&activePowerTimers); i++ )
    {
        if( pow == activePowerTimers[i]->power )
        {
            if( activePowerTimers[i]->rotatedThisTick )
                return activePowerTimers[i]->angle;
            else
            {
                float fAngleDelta;

                activePowerTimers[i]->rotatedThisTick = TRUE;
                
                //@todo SHAREDMEM shouldn't have to init this here and violate const
                if( !pow->fActivatePeriod )
                    (cpp_const_cast(BasePower*)(pow))->fActivatePeriod = 1.f/30;

                if( pow->eType == kPowerType_Toggle )
                    fAngleDelta = TIMESTEP*( pow->fEnduranceCost / pow->fActivatePeriod )*SPIN_TOGGLE_SCALE;
                else
                    fAngleDelta = TIMESTEP*( pow->fEnduranceCost )*SPIN_CLICK_SCALE;

                if(fAngleDelta < 0.075f)
                    fAngleDelta = 0.075f;
                activePowerTimers[i]->angle -= TIMESTEP*fAngleDelta;

                return activePowerTimers[i]->angle;
            }
        }
    }

    apt = calloc( 1, sizeof(ActivePowerTimer) );
    apt->power = pow;
    eaPush( &activePowerTimers, apt );

    return 0;
}


void trayslot_drawIcon( TrayObj * ts, float xp, float yp, float zp, float scale, int inventory )
{
    Entity *e = playerPtr();

    AtlasTex * spr;

    // this icon will be gotten from differnet sources later
    AtlasTex *ic = tray_GetIcon(ts, e);
    int     clr = CLR_WHITE;
    int     type;
    int     usable = 1;
    int     disabled = 0;
    float   sc = 1.f;
    bool    isCooldownActive;
    char    remainingTime[20];
    CBox    box;

    static AtlasTex * power_ring;
    static AtlasTex * spin;
    static AtlasTex * autoexec;
    static AtlasTex * target;
    static AtlasTex * queued;
    static AtlasTex * ap;
    static AtlasTex * highlight;
    static texBindInit=0;
    if (!texBindInit) {
        texBindInit = 1;
        power_ring            = atlasLoadTexture( "tray_ring_power.tga"             );
        spin                  = atlasLoadTexture( "tray_ring_endurancemeter.tga"    );
        autoexec              = atlasLoadTexture( "tray_ring_iconovr_autoexecute.tga" );
        target                = atlasLoadTexture( "tray_ring_iconovr_3Dcursor.tga"    );
        queued                = atlasLoadTexture( "tray_ring_iconovr_cued.tga"        );
        ap                    = atlasLoadTexture( "GenericPower_AutoOverlay.tga" );
        highlight             = atlasLoadTexture( "tray_ring_iconovr_highlight.tga" );
    }

    if (!ts)
        return;

    type = ts->type;
    if( stricmp( ic->name, "white" ) == 0)
        ic = atlasLoadTexture( "Power_Missing.tga" );

    trayslot_Timer( e, ts, &sc );
    isCooldownActive = sc < 1.0f;

    sc = sc/2 + .5f;

    if( sc < ts->scale )
    {
        ts->scale -= TIMESTEP*ICON_SCALE_SPEED;
        if( ts->scale < sc )
        {
            ts->scale = sc;
            if ( sc == 1.0 )
                ts->state = 1;
        }
    }
    else if ( sc > ts->scale )
    {
        ts->scale += TIMESTEP*ICON_SCALE_SPEED;
        if( ts->scale > sc )
        {
            ts->scale = sc;
            if( sc == 1.0 )
                ts->state = 1;
        }
    }

    if( type == kTrayItemType_Power )
    {
        // Make sure to keep this section in sync with trayobj_select [see COH-24115]
        Power *pPower = e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow];
        if( !pPower || !character_ModesAllowPower(e->pchar, pPower->ppowBase) || powerIsMarkedForTrade(pPower) ||
            (!e->access_level && pPower->ppowBase->ppchActivateRequires && !chareval_Eval(e->pchar, pPower->ppowBase->ppchActivateRequires, pPower->ppowBase->pchSourceFile)) )
            usable = 0;

        if( !character_AtLevelCanUsePower( e->pchar, pPower ) ) 
            usable = 0;

        disabled = !pPower->bEnabled;
    }

    if (sc < 1.0f || (!usable && !inventory) || disabled)
    {
        // if we're using cooldown timer display, fade the icon further to make the numbers easier to see, and reduce the icon scaling so its not hidden.
        if (isCooldownActive && optionGet(kUO_ShowTimer))
        {
            clr = 0xffffff33;
            sc = 1.0f;
        }
        else
        {
            clr = 0xffffff66;
        }
    }

    BuildCBox( &box, xp - ic->width*ts->scale*sc/2, yp - ic->width*ts->scale*sc/2, ic->width*sc*scale, ic->width*sc*scale );
    if( mouseCollision(&box) && !inventory && usable )
    {
        sc *= .9f;
        if( isDown(MS_LEFT) )
            sc *= .9f;
    }

    // draw the normal icon
    display_sprite( ic, xp - ic->width*ts->scale*sc*scale/2, yp - ic->width*ts->scale*sc*scale/2, zp + 2, ts->scale*scale*sc, ts->scale*scale*sc, clr );

    if( isCooldownActive && optionGet(kUO_ShowTimer) )
    {
        trayslot_RemainingTime( e, ts, remainingTime );

        // Display the remaining time on screen
        font( &game_9 );
        font_color( CLR_WHITE, CLR_WHITE );
        cprntEx(xp, yp, zp, scale, scale, (CENTER_X | CENTER_Y), remainingTime);
    }

    if( ts->autoPower )
    {
        int        autoColor = 0xffffff22;

        BuildCBox( &box, xp - ap->width*ts->scale*sc*scale/2, yp - ap->width*ts->scale*sc*scale/2, ap->width*ts->scale*scale*sc, ap->width*ts->scale*scale*sc );
        if( mouseCollision(&box) )
            autoColor = CLR_WHITE;
        display_sprite( ap, xp - ap->width*ts->scale*sc*scale/2, yp - ap->width*ts->scale*sc*scale/2, zp + 3, ts->scale*scale*sc, ts->scale*scale*sc, autoColor );
    }

    if( trayslot_IsActive( e, ts ) && type == kTrayItemType_Power )
    {
        const Power *power = character_GetPowerFromArrayIndices(e->pchar, ts->iset, ts->ipow);
        float angle = activePowerTimer_getAngle(power->ppowBase);

        display_rotated_sprite( spin, xp - spin->width*scale/2, yp - spin->width*scale/2, zp+5, scale, scale, clr, angle, 1 );
    }
    else
        ts->angle = 0;

    // lastly, pick the frame style and draw it
    if ( ts->type == kTrayItemType_Power )
    {
        int draw = 1;
        Power *ppow = e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow];

        // do the highlight ring first, so other rings can show on top of it
        if(ppow->ppowBase->ppchHighlightEval && chareval_Eval(e->pchar, ppow->ppowBase->ppchHighlightEval, ppow->ppowBase->pchSourceFile))
        {
            const U8 *rgba = ppow->ppowBase->rgbaHighlightRing;
            int hclr = rgba[0];
            hclr <<= 8;
            hclr |= rgba[1];
            hclr <<= 8;
            hclr |= rgba[2];
            hclr <<= 8;
            hclr |= rgba[3];
            display_sprite(highlight, xp - highlight->width*scale*ts->scale/2, yp - highlight->width*ts->scale*scale/2, zp, scale*ts->scale, scale*ts->scale, hclr );
        }

        spr = power_ring;

        if(ppow == e->pchar->ppowQueued)
            spr = queued;
        else if(trayobj_IsDefault(ts))
        {
            int found = FALSE;
            TrayObj * tmp;

            resetTrayIterator(e->pl->tray, -1, -1);
            while (getNextTrayObjFromIterator(&tmp))
            {
                if (tmp && ts && tmp->type == ts->type && ts->type == kTrayItemType_Power
                    && tmp->iset==ts->iset && tmp->ipow==ts->ipow)
                {
                    found = TRUE;
                    break;
                }
            }

            if( found )
                spr = autoexec;
            else
                trayobj_SetDefaultPower(NULL, e);
        }
        else if(cursor.target_world != kWorldTarget_None
            && s_ptsActivated
            && ts->ipow==s_ptsActivated->ipow
            && ts->iset==s_ptsActivated->iset)
        {
            spr = target;
        }
        else
            draw = 0;


        if( draw )
            display_sprite( spr, xp - spr->width*scale*ts->scale/2, yp - spr->width*ts->scale*scale/2, zp, scale*ts->scale, scale*ts->scale, clr );
    }
     else if( ts->type == kTrayItemType_Macro )
    {
        char tmp[6];

        set_scissor(TRUE);
        scissor_dims(xp - ic->width*scale*ts->scale/2, yp - ic->width*scale*ts->scale/2, ic->width, ic->height );

        font( &game_12 );
        font_color( CLR_WHITE, CLR_WHITE );
        Strncpyt( tmp, ts->shortName );
        cprnt( xp, yp + 4, zp+5, 1.f, 1.f, "%s", tmp );
        set_scissor(FALSE);
    }
}

//
//
//------------------------------------------------------------
// display a tray
// i: the slot to display
// currentTray: the tray index to display (1-9)
// curTrayType: the alt-level of the tray (kCurTrayType_Primary, etc.)
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
//----------------------------------------------------------
void trayslot_Display( Entity *e, int slotNumber, int raw_tray_value, int curTrayType, float xp, float yp, float z,
                            float scale, int color, int window )
{
    int     j = slotNumber, k = slotNumber, num;
    int        build;
    int     type;
    float   zp;
    char    temp[128];
    static TrayObj *contextTs = 0;
    TrayCategory currentCategory = (raw_tray_value == -1 ? kTrayCategory_ServerControlled : kTrayCategory_PlayerControlled);
    int currentTray = (raw_tray_value == -1 ? 0 : raw_tray_value);
    TrayObj *ts = trayslot_Get(e, currentCategory, currentTray, slotNumber);
    ToolTipParent *tipParent = (window == WDW_TRAY ? &trayTipParent : NULL);

    CBox box;

    AtlasTex * spr;
    static AtlasTex * power_ring;
    static AtlasTex * inspiration_square;
    static AtlasTex * power_back;
    static AtlasTex * left_arrow;
    static AtlasTex * right_arrow;
    static texBindInit=0;

    assert(e->pchar);
    build = e->pchar->iCurBuild;

    if (!texBindInit) {
        texBindInit = 1;
        power_ring            = atlasLoadTexture( "tray_ring_power.tga"             );
        inspiration_square    = atlasLoadTexture( "tray_ring_inspiration.tga"       );
        power_back            = atlasLoadTexture( "EnhncTray_RingHole.tga"          );
        left_arrow            = atlasLoadTexture( "tray_arrow_L.tga"                );
        right_arrow           = atlasLoadTexture( "tray_arrow_R.tga"                );
    }

    type = ts?ts->type:kTrayItemType_None;

    k = 0;
    zp = z;

    // display the little numbers
    num = slotNumber + 1;
    if ( num >= 10 )
        num = 0;

    // make a collision box for the mouse
      BuildCBox( &box, xp-scale*BOX_SIDE/2, yp - scale*BOX_SIDE/2, scale*BOX_SIDE, scale*BOX_SIDE );
    //drawBox(&box, 5000, CLR_RED, 0 );
    STR_COMBINE_BEGIN(temp);
    STR_COMBINE_CAT("tray_ring_number_");
    STR_COMBINE_CAT_D(num);
    STR_COMBINE_CAT(".tga");
    STR_COMBINE_END();
    spr = atlasLoadTexture( temp );
    display_sprite( spr, xp - scale*BOX_SIDE/2, yp + (BOX_SIDE/2 - spr->height)*scale, zp + 3, scale, scale, CLR_WHITE );

    if( type == kTrayItemType_None ) // empty tray slot
    {
        int back_color = 0x00000088;
        float sc = 1.f;
        float holesc = .5f;

        if ( cursor.dragging )
        {
            if( cursor.drag_obj.type == kTrayItemType_Power && control_state.canlook )
            {
                TrayObj *last = getTrayObj(e->pl->tray, cursor.drag_obj.icat, cursor.drag_obj.itray, cursor.drag_obj.islot, build, true);
                trayobj_copy( last, &cursor.drag_obj, TRUE );
                trayobj_stopDragging();
            }

            if (mouseCollision(&box))
            {
                sc *= .9;

                if (!isDown(MS_LEFT))
                {
                    if (currentCategory == kTrayCategory_ServerControlled)
                    {
                        InfoBox(INFO_SVR_COM, "CantControlServerTray");
                    }
                    else if (inTray(&cursor.drag_obj, currentTray))
                    {
                        InfoBox(INFO_SVR_COM, "ItemInTray");
                    }
                    else
                    {
                        ts = getTrayObj(e->pl->tray, currentCategory, currentTray, slotNumber, build, true);
                         if( cursor.drag_obj.type == kTrayItemType_Power || IS_TRAY_MACRO(cursor.drag_obj.type) )
                        {                        
                            trayobj_copy( ts, &cursor.drag_obj, TRUE );     
                        }
                        trayobj_stopDragging();
                    }
                }
            }
        }

        display_sprite( power_ring, xp - sc*power_ring->width*scale/2, yp - sc*power_ring->height*scale/2, zp + 2, scale*sc, scale*sc, color );
        display_sprite( power_back, xp - holesc*scale*sc*power_back->width/2,
                        yp - holesc*scale*sc*power_back->height/2, zp+1, holesc*scale*sc, holesc*scale*sc, 0xffffff66 );

        if (mouseCollision(&box))
            setToolTip(&traySlotToolTip, &box, "EmptyTraySlot", tipParent, MENU_GAME, window);
    }
    else // tray slot with power in it
    {
        if( ts->state == 1 )
            traySlot_fireFlash( ts, xp, yp, zp );

        if ( cursor.dragging )
        {
            if( cursor.drag_obj.type == kTrayItemType_Power && control_state.canlook )
            {
                TrayObj *last = getTrayObj(e->pl->tray, cursor.drag_obj.icat, cursor.drag_obj.itray, cursor.drag_obj.islot, build, true);

                if( last )
                    trayobj_copy( last, &cursor.drag_obj, TRUE );
                trayobj_stopDragging();
            }

            if ( mouseCollision(&box) && !isDown(MS_LEFT) )
            {
                if (currentCategory == kTrayCategory_ServerControlled)
                {
                    InfoBox(INFO_SVR_COM, "CantControlServerTray");
                }
                else if (inTray(&cursor.drag_obj, currentTray))
                {
                    InfoBox(INFO_SVR_COM, "ItemInTray");
                }
                else
                {
                    if( cursor.drag_obj.type == kTrayItemType_Power || IS_TRAY_MACRO(cursor.drag_obj.type) )
                    {
                        // if slot is occupied copy it to the slot the new one is coming from
                        if ((ts->type == kTrayItemType_Power || IS_TRAY_MACRO(ts->type))
                            && cursor.drag_obj.ispec >= 0
                            && cursor.drag_obj.icat >= 0
                            && cursor.drag_obj.itray >= 0
                            && cursor.drag_obj.islot >= 0
                            && cursor.drag_obj.icat != kTrayCategory_ServerControlled)
                        {
                            TrayObj *last = getTrayObj(e->pl->tray, cursor.drag_obj.icat, cursor.drag_obj.itray, cursor.drag_obj.islot, build, true);
                            trayobj_copy( last, ts, TRUE );
                        }
                        
                        trayobj_copy( ts, &cursor.drag_obj, TRUE );
                    }

                    trayobj_stopDragging();
                }
            }
        }
        else if (mouseLeftDrag(&box))
        {
            AtlasTex *ic = tray_GetIcon(ts, e);
            trayobj_startDragging( ts, ic, NULL );  // or start dragging if and inventory is up
            cursor.drag_obj.islot = slotNumber;     // tag the slot this power is coming from
            cursor.drag_obj.itray = currentTray;
            cursor.drag_obj.icat = currentCategory;
        }
        else if( mouseCollision( &box ) )
        {
            // first draw the icon
            if ( mouseClickHit( &box, MS_LEFT) ) // is there was a mouse press
            {
                // @hack -AB: crazy hack for control clicking a sticky tray :2005 Mar 21 11:33 AM
                // if this is an alt tray that is showing temporarily do not process this as a 
                // control click, instead process as a regular click.
                U32 alttray_mask = curtraytype_to_altmask( curTrayType );
                bool isMomentaryTray = alttray_mask & s_TrayMomentaryMask && !(s_TrayStickyMask & alttray_mask);
                
                if( !isMomentaryTray && inpLevel(INP_CONTROL))
                {
                     TrayObj *ts = trayslot_Get(e, currentCategory, currentTray, slotNumber);

                    if(isMenu(MENU_GAME) )
                    {
                        if(ts->type == kTrayItemType_Power)
                        {
                            // Set this as the default power.
                            if(trayobj_IsDefault(ts))
                            {
                                trayobj_SetDefaultPower(NULL, e);
                            }
                            else
                            {
                                trayobj_SetDefaultPower(ts, e);
                            }
                        }
                    }
                }
                else
                {
                    trayslot_select(e, currentCategory, currentTray, slotNumber); // activate power
                }
            }
        }

        if( mouseClickHit( &box, MS_RIGHT ) )
        {
            int x, y;
            int wd, ht;
            CMAlign alignVert;
            CMAlign alignHoriz;

            windowClientSizeThisFrame( &wd, &ht );

            x = (box.lx+box.hx)/2;
            alignHoriz = CM_CENTER;

            y = box.ly<ht/2 ? box.hy : box.ly;
            alignVert = box.ly<ht/2 ? CM_TOP : CM_BOTTOM;

            if( ts->type == kTrayItemType_Power )
            {
                contextTs = ts;
                contextTs->icat = currentCategory;
                contextTs->itray = currentTray;
                contextTs->islot = slotNumber;
                contextMenu_setAlign( gPowerContext, x, y, alignHoriz, alignVert, contextTs );
            }
            else if ( IS_TRAY_MACRO(ts->type) )
            {
                // macro right-click goes here
                contextTs = ts;
                contextTs->icat = currentCategory;
                contextTs->itray = currentTray;
                contextTs->islot = slotNumber;
                contextMenu_setAlign( gPowerContext, x, y, alignHoriz, alignVert, contextTs );
            }
            collisions_off_for_rest_of_frame = 1;
        }

        trayslot_drawIcon( ts, xp, yp, zp, scale, 0 );

        if (mouseCollision(&box))
        {
             if( ts->type == kTrayItemType_Power )
            {
                setToolTip( &traySlotToolTip, &box, e->pchar->ppPowerSets[ts->iset]->ppPowers[ts->ipow]->ppowBase->pchDisplayName,
                    tipParent, MENU_GAME, window );
            }
            if( IS_TRAY_MACRO(ts->type) )
                setToolTipEx( &traySlotToolTip, &box, ts->shortName, tipParent, MENU_GAME, window, 0, TT_NOTRANSLATE );
        }
    }
}

CurTrayType tray_razer_adjust( CurTrayType tray )
{
    CurTrayType retval = tray;

    // The Razer tray takes over the role of the primary tray if it's open and
    // the normal trays 'shift down' one so that the bottom tray is alt-num,
    // the middle tray is ctrl-num and the top tray cannot be activated by
    // keypresses.
    if( game_state.razerMouseTray )
    {
        if( tray == kCurTrayType_Primary )
            retval = kCurTrayType_Razer;
        else if( tray == kCurTrayType_Alt )
            retval = kCurTrayType_Primary;
        else if( tray == kCurTrayType_Alt2 )
            retval = kCurTrayType_Alt;
        else
            retval = -1;
    }

    return retval;
}

//------------------------------------------------------------
// move the passed tray forward or backwards for the
//  current user, and update the server.
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
//----------------------------------------------------------
void tray_change( int alt, int forward )
{
    Entity *e = playerPtr();

    if( !INRANGE( alt, kCurTrayType_Primary, kCurTrayType_Count ) )
        return;

    e->pl->tray->current_trays[alt] = tray_changeIndex(kTrayCategory_PlayerControlled, e->pl->tray->current_trays[alt], forward);

    START_INPUT_PACKET(pak, CLIENTINP_RECEIVE_TRAYS);
    sendTray( pak, e );
    END_INPUT_PACKET
}

//------------------------------------------------------------
// set the primary curtray to the specified tray
// traynum: the actual traynum to set it to (user tray - 1)
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
// -AB: refactored into tray_goto_tray :2005 Feb 03 11:30 AM
//----------------------------------------------------------
void tray_goto( int traynum )
{
    tray_goto_tray( game_state.razerMouseTray ? kCurTrayType_Razer : kCurTrayType_Primary, traynum );
}

//------------------------------------------------------------
// set the tray num in the spec
// curtray, traynum: the actual slot (user input - 1)
// -AB: created :2005 Feb 02 05:31 PM
//----------------------------------------------------------
void tray_goto_tray( int curtray, int traynum )
{
    if( INRANGE( traynum, 0, 9 ) && INRANGE( curtray, 0, kCurTrayType_Count ) )
    {
        Entity *e = playerPtr();
        assert(e);
        
        e->pl->tray->current_trays[ curtray ] = traynum;
        START_INPUT_PACKET(pak, CLIENTINP_RECEIVE_TRAYS);
        sendTray( pak, e );
        END_INPUT_PACKET
    }
}

//------------------------------------------------------------
// render and handle interaction with the arrows for
// changing the index of the rendered tray
// y: the y of the tray for which the arrows are being drawn (not the window itself)
// -AB: refactored for new cur tray functionality :2005 Jan 26 03:41 PM
//----------------------------------------------------------
void tray_drawArrows( Entity *e, int cur_tray, float x, float y, float z, float wd, float ht, float scale, int color )
{
    float xp = x, yp;
    char temp[8];

    static AtlasTex * power_ring;
    static AtlasTex * left_arrow;
    static AtlasTex * right_arrow;
    static int texBindInit=0;
    if (!texBindInit) 
    {
        texBindInit = 1;
        power_ring            = atlasLoadTexture( "tray_ring_power.tga" );
        left_arrow            = atlasLoadTexture( "teamup_arrow_contract.tga" );
        right_arrow           = atlasLoadTexture( "teamup_arrow_expand.tga" );
    }

    assert(INRANGE(cur_tray,0,kCurTrayType_Count));


//     if( cur_tray )
//         yp = y + (TRAY_YOFF + power_ring->width/2)*scale;
//     else
//         yp = y + (TRAY_YOFF + power_ring->width/2)*scale + (ht-DEFAULT_TRAY_HT*scale);

    // -AB: attempting to scale by what tray this is, this may send it off the bottom tho :2005 Jan 26 04:26 PM
    yp = y + (TRAY_YOFF + power_ring->width/2)*scale;

    // left arrow
    if(  D_MOUSEHIT == drawGenericButton( left_arrow, xp + (ARROW_OFFSET-3)*scale, yp - (left_arrow->height)*scale/2, z, scale, (color & 0xffffff00) | 0x88, color, 1 ) )
    {
        tray_change( cur_tray, 0 );
    }

    // display number
    itoa( e->pl->tray->current_trays[ cur_tray ] + 1, temp, 10 );
    font( &title_18 );
    font_color( 0xffffff88, 0xffffff88 );
    cprnt( xp + (left_arrow->width)*scale + (ARROW_OFFSET*2-3)*scale, yp + 8*scale, z, scale, scale, temp );

    // right arrow
    if(  D_MOUSEHIT == drawGenericButton( right_arrow, xp + (ARROW_OFFSET*4-2)*scale, yp - (right_arrow->height*scale)/2,
        z, scale, (color & 0xffffff00) | 0x88, color, 1 ) )
    {
        tray_change( cur_tray, 1 );
    }
}


/**********************************************************************func*
 * tray_HandleWorldCursor
 *
 */
void tray_HandleWorldCursor(Entity *e, Mat4 matCursor, Vec3 probe)
{
    Vec3 camera;
    Vec3 rpy;
    CalcTeleportTargetResult teleportResult;

    if(cursor.target_world)
    {
        if(s_ptsActivated!=NULL)
        {
            Power *power = 0;

            if( s_ptsActivated->type != kTrayItemType_PetCommand )
                power = e->pchar->ppPowerSets[s_ptsActivated->iset]->ppPowers[s_ptsActivated->ipow];

            if( (!power && s_ptsActivated->type == kTrayItemType_PetCommand)
                || power->ppowBase->eTargetType == kTargetType_Location
                || power->ppowBase->eTargetType == kTargetType_Teleport
                || power->ppowBase->eTargetTypeSecondary == kTargetType_Location
                || power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport
                )
            {
                //cursor.click_move_allow = 0; //Replaced this with switch statement in handleMouseOverAndOrClick()

                // Fix up the location target if we're teleporting
                if(cursor.target_world != kWorldTarget_Blocked
                    && cursor.target_world != kWorldTarget_OutOfRange
                    && power && (power->ppowBase->eTargetType==kTargetType_Teleport
                        || power->ppowBase->eTargetTypeSecondary == kTargetType_Teleport))
                {
                    // Determine if we should hand the probe vector to CalcTeleportTarget, this is used inside that routine 
                    // to determine if it should do the stepback checks to avoid "red reticle of death" syndrome.
                    // We get a valid probe vector for any teleport power (teleport self, recall friend and teleport foe),
                    // because it's needed for the ExtraTeleportChecks.
                    int useProbe = power != NULL && power->ppowBase->eTargetType == kTargetType_Teleport &&
                        power->ppowBase->eTargetTypeSecondary == kTargetType_None;

                    copyVec3(cam_info.cammat[3], camera);
                    teleportResult = CalcTeleportTarget(matCursor, current_location_target, useProbe ? probe : NULL);
                    if (teleportResult != TELEPORT_TARGET_OK)
                    {
                        // Enter here if CalcTeleportTarget used the step back code to alter the target location.
                        // If that happens we have to adjust where the cursor is placed.
                        fxFaceTheCamera(current_location_target, matCursor);
                        copyVec3(current_location_target, matCursor[3]);

                        // If the cursor is at the player's head, move the targeting reticle up to match.
                        if (teleportResult == TELEPORT_TARGET_HEAD)
                        {
                            matCursor[3][1] += PLAYER_HEIGHT;
                        }

                        // For reasons unknown, the matrix is twisted PI by 2 about the x axis.  Untwist here.
                        rpy[0]=-PI/2.0f;
                        rpy[1]=0.0f;
                        rpy[2]=0.0f;

                        rotateMat3(rpy, matCursor);
                    }
                    if(!IsTeleportTargetValid(current_location_target) || (probe && !ExtraTeleportChecks(current_location_target, &probe[3], camera)))
                    {
                        cursor.target_world = kWorldTarget_Blocked;
                    }
                }
                else
                {
                    CalcLocationTarget(matCursor, current_location_target);
                }

                // If the cursor is in the air somewhere, make sure that they
                // are allowed to select stuff up there.
                if(cursor.target_world == kWorldTarget_OffGround)
                {
                    if(power && power->ppowBase->bTargetNearGround)
                        cursor.target_world = kWorldTarget_Blocked;
                }

                // If the cursor hasn't been nixed for any other reason,
                // make sure it's in range.
                if(cursor.target_world != kWorldTarget_Blocked
                    && cursor.target_world != kWorldTarget_OutOfRange)
                {
                    float f = distance3(current_location_target, ENTPOS(e));

                    if(f<0)
                        f=-f;

                    if(f<=cursor.target_distance)
                        cursor.target_world = kWorldTarget_InRange;
                    else
                        cursor.target_world = kWorldTarget_Normal;
                }

                // Finally, we check to see if they've clicked. If all is well,
                // then activate the power.
                if(mouseDown(MS_LEFT)
                    && cursor.target_world != kWorldTarget_Blocked
                    && cursor.target_world != kWorldTarget_OutOfRange)
                {
                    if( s_ptsActivated->type == kTrayItemType_PetCommand )
                    {
                        pet_selectCommand( s_ptsActivated->iset, s_ptsActivated->ipow, s_ptsActivated->command, s_ptsActivated->islot, current_location_target );
                    }
                    else
                    {
                        Entity *target = (power->ppowBase->eTargetType==kTargetType_Caster||!current_target) ? e : current_target;
                        trayobj_ActivatePowerAtLocation(s_ptsActivated, e, target, current_location_target);
                    }
                }
                else if(!inpEdge(INP_ESCAPE))
                {
                    return;
                }
            }
        }
    }

    cursor.target_world = kWorldTarget_None;
    s_ptsActivated = NULL;

}

void tray_SetWorldCursorRange(void)
{
    if(cursor.target_world && s_ptsActivated)
    {
        if( s_ptsActivated->type == kTrayItemType_PetCommand )
        {
            cursor.target_distance = 100.f;
        }
        else
        {
            Entity *e = playerPtr();
            PowerSet *pset = e ? e->pchar->ppPowerSets[s_ptsActivated->iset] : NULL;
            Power *power = pset ? pset->ppPowers[s_ptsActivated->ipow] : NULL;
            if (power)
                cursor.target_distance = power->fRangeLast;
            else
                s_ptsActivated = NULL;
        }
    }
}

//-------------------------------------------------------------------------------------------------------
// Master Tray //////////////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------
#define TRAY_SPEED      10

//------------------------------------------------------------
// convert the passed mask to a CurTrayType
// mask: must be a power of 2
// @returns CurTrayType: the found type, or kCurTrayType_Primary if not found
// -AB: created :2005 Jan 31 11:57 AM
//----------------------------------------------------------
static CurTrayType type_from_mask( int mask )
{
    int i;
    CurTrayType res = kCurTrayType_Primary;
    assert(POW_OF_2(mask));
    
    // go until a bit is set
    for( i = 0; i < kCurTrayType_Count; ++i )
    { 
        if( !((1<<i) & mask) )
        {
            res = (CurTrayType)i;
            break;
        }
    }

    // --------------------
    // return the result
    
    return res;
}


//------------------------------------------------------------
// helper for determining the final alttray height
// -AB: created :2005 Jan 26 12:52 PM
//----------------------------------------------------------
static float calc_alttray_height( int num_trays_showing, float scale, bool flip )
{
    return (num_trays_showing*DEFAULT_TRAY_HT /* + (flip ? -PIX3 : PIX3) */)*scale;
}


//------------------------------------------------------------
// helper for seeing if the passed tray is showing
// -AB: created :2005 Feb 08 05:27 PM
//----------------------------------------------------------
static bool tray_is_showing( CurTrayType tray_type )
{
    int mask = curtraytype_to_altmask( tray_type );
    return mask & s_TrayStickyMask || mask & s_TrayMomentaryMask;
}



//------------------------------------------------------------
// helper: figure out the desired current tray level.
// the behavior is this: if the tray is already open at a level to or
// past the alt level, then do nothing, otherwise, alt it
// -AB: created :2005 Jan 28 04:49 PM
// -AB: refactoring for any-order sticky trays :2005 Feb 08 05:03 PM
//----------------------------------------------------------
static int get_num_trays_showing()
{
    int i;

    // start the number at the current level + 1 (primary is zero)
    int res = 0;

    // for each mask above the current level, if a bit is set
    // count it as showing. Note that we only deal with the three normal
    // trays here - the Razer tray is not part of the tray stack.
    for( i = 0; i <= kCurTrayType_Alt2; ++i )
    { 
        if( tray_is_showing( i ) )
        {
            res++;
        }
    }
    return res;
}


void tray_Clear(void)
{
    Entity *e = playerPtr();
    TrayObj *obj;

    resetTrayIterator(e->pl->tray, e->pchar->iCurBuild, kTrayCategory_PlayerControlled);
    while (getNextTrayObjFromIterator(&obj))
    {
        if (obj && !IS_TRAY_MACRO(obj->type))
        {        
            destroyCurrentTrayObjViaIterator();
        }
    }

    START_INPUT_PACKET(pak, CLIENTINP_RECEIVE_TRAYS);
    sendTray( pak, playerPtr() );
    END_INPUT_PACKET
}

typedef struct ServerTrayItem
{
    int priority;
    int iset;
    int ipow;
} ServerTrayItem;

ServerTrayItem **s_serverTrayPriorities = NULL;
bool s_displayServerTray = false;

static void ServerTrayItemDestroy(void *data)
{
    SAFE_FREE(data);
}

void clearServerTrayPriorities(void)
{
    eaDestroyEx(&s_serverTrayPriorities, ServerTrayItemDestroy);
}

static int cmp_ServerTrayItem(const ServerTrayItem** a, const ServerTrayItem** b)
{
    return (*b)->priority - (*a)->priority;
}

static int cmp_ServerTrayItemByPower(const ServerTrayItem** a, const ServerTrayItem** b)
{
    if ((*b)->iset != (*a)->iset)
        return (*b)->iset - (*a)->iset;
    else
        return (*b)->ipow - (*a)->ipow;
}

// Inserts at the TrayObj array level.
// See serverTray_AddPower to add to the ServerTrayItem array.
static void serverTray_InsertServerTrayItem(Entity *e, const Power *ppow, int newIndex)
{
    int index;
    int iset, ipow;

    clearTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, TRAY_WD - 1, 0);

    for (index = TRAY_WD - 1; index > newIndex; index--)
    {
        TrayObj *source = getTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, index - 1, 0, false);
        TrayObj *dest = getTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, index, 0, !!source);

        if (source && dest)
        {
            trayobj_copy(dest, source, false);
            dest->islot = index;
        }
        else if (dest)
        {
            clearTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, index, 0);
        }
    }

    power_GetIndices(ppow, &iset, &ipow);
    buildPowerTrayObj(getTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, newIndex, 0, true), iset, ipow, false);
}

static void serverTray_RemoveServerTrayItem(Entity *e, int oldIndex)
{
    int index;

    clearTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, oldIndex, 0);

    for (index = oldIndex; index < TRAY_WD; index++)
    {
        TrayObj *source = getTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, index + 1, 0, false);
        TrayObj *dest = getTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, index, 0, !!source);

        if (source && dest)
        {
            trayobj_copy(dest, source, false);
            dest->islot = index;
        }
        else if (dest)
        {
            clearTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, index, 0);
        }
    }

    // Always leaves the last tray slot open, call serverTray_InsertServerTrayItem to fill it back up.
}

void serverTray_checkActivationRequirements(Entity *e)
{
    int trayIndex = 0, priorityIndex = 0, endIndex = TRAY_WD - 1;

    s_displayServerTray = false;
    while (trayIndex < TRAY_WD && priorityIndex < eaSize(&s_serverTrayPriorities))
    {
        const Power *ppow = character_GetPowerFromArrayIndices(e->pchar, s_serverTrayPriorities[priorityIndex]->iset, s_serverTrayPriorities[priorityIndex]->ipow);
        bool shouldDisplay = ppow && character_ModesAllowPower(e->pchar, ppow->ppowBase)
                            && (!ppow->ppowBase->ppchActivateRequires || chareval_Eval(e->pchar, ppow->ppowBase->ppchActivateRequires, ppow->ppowBase->pchSourceFile));
        TrayObj *curTray = getTrayObj(e->pl->tray, kTrayCategory_ServerControlled, 0, trayIndex, 0, false);
        bool wasDisplayed;
        
        wasDisplayed = (curTray && curTray->iset == s_serverTrayPriorities[priorityIndex]->iset && curTray->ipow == s_serverTrayPriorities[priorityIndex]->ipow);
        
        if (shouldDisplay && !wasDisplayed)
        {
            serverTray_InsertServerTrayItem(e, ppow, trayIndex);
        }
        else if (!shouldDisplay && wasDisplayed)
        {
            serverTray_RemoveServerTrayItem(e, trayIndex);
        }

        if (shouldDisplay)
        {
            s_displayServerTray = true;
            trayIndex++;
        }

        priorityIndex++;
    }

    while (trayIndex <= endIndex)
    {
        serverTray_RemoveServerTrayItem(e, endIndex);
        endIndex--;
    }
}

// Adds to the ServerTrayItem array.
// The called functions handle the TrayObj array.
void serverTray_AddPower(Entity *e, Power *ppow, int priority)
{
    int newIndex, iset, ipow;
    ServerTrayItem *newItem = malloc(sizeof(ServerTrayItem));
    power_GetIndices(ppow, &iset, &ipow);
    newItem->priority = ppow->ppowBase->iServerTrayPriority;
    newItem->iset = iset;
    newItem->ipow = ipow;

    newIndex = eaBFind(s_serverTrayPriorities, cmp_ServerTrayItem, newItem);
    eaInsert(&s_serverTrayPriorities, newItem, newIndex);
}

// Alters the ServerTrayItem array.
// The called functions handle the TrayObj array.
void serverTray_AlterPriority(Entity *e, Power *ppow, int priority)
{
    int oldIndex, newIndex, iset, ipow;
    ServerTrayItem *newItem = malloc(sizeof(ServerTrayItem));
    power_GetIndices(ppow, &iset, &ipow);
    newItem->priority = ppow->ppowBase->iServerTrayPriority;
    newItem->iset = iset;
    newItem->ipow = ipow;

    for (oldIndex = 0; oldIndex < eaSize(&s_serverTrayPriorities); oldIndex++)
    {
        if (s_serverTrayPriorities[oldIndex]->iset == iset
            && s_serverTrayPriorities[oldIndex]->ipow == ipow)
        {
            break;
        }
    }

    if (oldIndex != eaSize(&s_serverTrayPriorities))
    {
        eaRemoveAndDestroy(&s_serverTrayPriorities, oldIndex, ServerTrayItemDestroy);
    }

    newIndex = eaBFind(s_serverTrayPriorities, cmp_ServerTrayItem, newItem);
    eaInsert(&s_serverTrayPriorities, newItem, newIndex);
}

// Removes from the ServerTrayItem array.
// The called functions handle the TrayObj array.
void serverTray_RemovePower(Entity *e, Power *ppow)
{
    int oldIndex, iset, ipow;
    power_GetIndices(ppow, &iset, &ipow);

    for (oldIndex = 0; oldIndex < eaSize(&s_serverTrayPriorities); oldIndex++)
    {
        if (s_serverTrayPriorities[oldIndex]->iset == iset
            && s_serverTrayPriorities[oldIndex]->ipow == ipow)
        {
            break;
        }
    }

    if (oldIndex != eaSize(&s_serverTrayPriorities))
    {
        eaRemoveAndDestroy(&s_serverTrayPriorities, oldIndex, ServerTrayItemDestroy);
    }
}

void serverTray_Rebuild()
{
    Entity *e = playerPtr();
    int setIndex, setCount;
    PowerSet *pset;
    int powIndex, powCount;
    Power *ppow;

    if (!e || !e->pchar)
        return;

    clearServerTrayPriorities();

    setCount = eaSize(&e->pchar->ppPowerSets);
    for (setIndex = 0; setIndex < setCount; setIndex++)
    {
        pset = e->pchar->ppPowerSets[setIndex];
        if (!pset)
            continue;

        powCount = eaSize(&pset->ppPowers);
        for (powIndex = 0; powIndex < powCount; powIndex++)
        {
            ppow = pset->ppPowers[powIndex];

            if (!ppow || !ppow->ppowBase)
                continue;

            if (ppow->ppowBase->iServerTrayPriority)
            {
                serverTray_AddPower(e, ppow, ppow->ppowBase->iServerTrayPriority);
            }
        }
    }
}


//
//
// -AB: refactored trays to support more than one alt tray :2005 Feb 01 02:25 PM
// -AB: added test for zero height to slam the height to the desired height. :2005 Feb 01 02:25 PM
int trayWindow()
{
    static int init = FALSE, lastAltShow;
    static float altHt = 0;

    Entity  *e = playerPtr();
    float x, y, z, ht, wd, scale;
    int i, color, bcolor;
    int tray_lvl = 0;
    bool serverTrayOverride = character_IsInPowerMode(e->pchar, kPowerMode_ServerTrayOverride);
    
    AtlasTex *arrow;
    Wdw * wdw;

    static AtlasTex * left;
    static AtlasTex * right;
    static AtlasTex * meat;
    static AtlasTex * arrow_up;
    static AtlasTex * arrow_down;
    static int texBindInit=0;
    if (!texBindInit) 
    {
        texBindInit = 1;
        left  = atlasLoadTexture( "Jelly_tray_edge_L.tga" );
        right = atlasLoadTexture( "Jelly_tray_edge_R.tga" );
        meat = atlasLoadTexture( "Jelly_tray_meat.tga" );
        arrow_up = atlasLoadTexture( "Jelly_arrow_up.tga" );
        arrow_down = atlasLoadTexture( "Jelly_arrow_down.tga" );
    }

    PERFINFO_AUTO_START("tray_verifyValidPowers", 1);
        tray_verifyValidPowers();
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("serverTray_checkActivationRequirements", 1);
        serverTray_checkActivationRequirements(e);
    PERFINFO_AUTO_STOP();

    tray_timer += TIMESTEP; 

    if ( !init )
    {
        memset( &traySlotToolTip, 0, sizeof(ToolTip) );
        addToolTip( &traySlotToolTip );

        gPowerContext = contextMenu_Create(NULL);
        contextMenu_addVariableTitleTrans( gPowerContext, traycm_PowerText, 0, traycm_translate, 0);
        contextMenu_addVariableTextTrans( gPowerContext, traycm_PowerInfo, 0, traycm_translate, 0);
        contextMenu_addVariableTextVisible( gPowerContext, traycm_showExtra, 0, traycm_PowerExtra, 0 );
        contextMenu_addCode( gPowerContext, traycm_RemoveAvailable, 0, traycm_Remove, 0, "CMRemoveFromTray", 0  );
        contextMenu_addCode( gPowerContext, traycm_InfoAvailable, 0, traycm_Info, 0, "CMInfoString", 0  );
        contextMenu_addCode( gPowerContext, traycm_IsMacro, 0, traycm_EditMacro, 0, "CMEditString", 0  );
        gift_addToContextMenu(gPowerContext);
        init = TRUE;
    }

    power_clearActive();

    // x and y are unreliable here
    // they represent the upper-left corner, which changes in the code that follows
    // x and y will be reliable after the next window_getDims() call.
    // if you need these variables, use window_getDimsEx(..., true),
    //   and make sure you write the code with x and y as the anchor corner, not the upper-left.
    if( !window_getDims( WDW_TRAY, NULL, NULL, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    wdw = wdwGetWindow( WDW_TRAY ); 

    // animate the tray opening or closing
    {
        float goal_height;

        if (!serverTrayOverride)
            tray_lvl += get_num_trays_showing();
        if (serverTrayOverride || s_displayServerTray)
            tray_lvl++;

        goal_height = calc_alttray_height( tray_lvl, scale, wdw->below);

        // special case for tray hack: the tray has no default
        // height. so the first time it loads up it was given an
        // arbitrary height of 55. the result of this was that,
        // because the tray is anchored at the bottom (i.e. y + ht),
        // it would shrink and move the top of the tray.
        //
        // the fix is to set the height to zero in
        // uiWindows_init.c:window_getDefaultSize
        // and if the ht is zero, slam the height, but leave y
        // untouched.
        //
        // ARM NOTE:  Now that the y value stored in the database 
        //   is the bottom of this window, we could eliminate this hack
        //   by storing the tray height in the database and altering
        //   the code that minmax's the height of the window.
        //   One problem:  how to translate the previous state where there
        //   was no stored height data?
        if( ht == 0.f )
        {
            ht = goal_height;
        }
        else if( ht != goal_height )
        {
            float delta = (int)((TIMESTEP*TRAY_SPEED)*scale);
            if( goal_height > ht )
            {
                // window is growing
                 ht += delta;
                
                if( ht > goal_height )
                {
                    // goal reached
                    ht = goal_height;
                }
            }
            else
            {
                // window is shrinking

                // check for this to stick it to the top of the screen.
                 ht -= delta;

                if( goal_height > ht )
                {
                    // goal reached
                    ht = goal_height;
                }
            }
        }

        // set the new dimensions of the window (if applicable)
        if( window_getMode( WDW_TRAY ) == WINDOW_DISPLAYING )
            window_setDims( WDW_TRAY, -1, -1, -1, ht );

        // since we aren't updating y anymore, we don't need to update the server with the info
        // re-enable this if you ever transmit the ht of this window to the server
//         if( updateServer )
//         {
//             window_UpdateServer( WDW_TRAY );
//         }
    }
    
    window_getDims( WDW_TRAY, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor );

    // show the down arrow if all trays or showing, otherwise the up arrow
    // -AB: accounting for the window flip :2005 Jan 31 11:01 AM
    // -AB: down if the next one is primary (only) :2005 Feb 08 05:22 PM
    {
        bool down = s_next_tray_mode( s_TrayStickyMask ) == kCurTrayMask_Primary;

        if( down && !wdw->below || !down && wdw->below )
            arrow = arrow_down;
        else
            arrow = arrow_up;
        
    }
    
    // draw the up/down arrow, handle clicks    
    if(window_getMode(WDW_TRAY) == WINDOW_DISPLAYING ) 
    {
        if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
        {
            if( D_MOUSEHIT == drawGenericButton( arrow, x + wd - 50*scale, y - (JELLY_HT + arrow->height)*scale/2, z, scale, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1 ) )
                cmdParse( "alttraysticky" );
        }
        else
        {
            if( D_MOUSEHIT == drawGenericButton( arrow, x + wd - 50*scale, y + ht + (JELLY_HT - arrow->height)*scale/2, z, scale, (color & 0xffffff00) | (0x88), (color & 0xffffff00) | (0xff), 1 ) )
                cmdParse( "alttraysticky");
        }
    }
    BuildCBox( &trayTipParent.box, x, y, wd, ht );

    // the way this loop works as follows:
    // for each tray:
    //  - if it is should be rendered, render it
    {
        // this is the ui level being rendered (i.e. lvl 1 could be alt or alt2)
        // we explicitly don't go past alt2 because the Razer tray isn't part
        // of the tray stack
        int trays_drawn = 0;
        float curr_ht = 0.0f;
        float goal_ht = 0.0f;
        float tray_y;
        // make sure that the tray should be rendered. it should be rendered
        // until the window is too small to fit it.
        int tray_to_use = 0; // either CurTrayType or -1 for server-controlled tray
        do
        {
            curr_ht = calc_alttray_height(trays_drawn, scale, wdw->below);
            goal_ht = calc_alttray_height(trays_drawn + 1, scale, wdw->below);
            tray_y = y + (goal_ht <= ht ? ht - goal_ht : 0);

            drawFrame( PIX3, R22, x, tray_y, z, wd, DEFAULT_TRAY_HT*scale, scale, color, bcolor );
            
            if (trays_drawn == 1 && !tray_is_showing(trays_drawn) && tray_is_showing(trays_drawn + 1))
            {
                // This if check is a super ugly hack to get this done quickly for I22 launch.
                //   I think the whole way the tray tracks what trays are being displayed
                //   should be completely rewritten from scratch.  Track the state of each of
                //   the four trays (main, alt, alt2, server) as a WindowDisplayMode...
                tray_to_use = trays_drawn + 1;
            }
            else if (((serverTrayOverride || s_displayServerTray || !tray_is_showing(trays_drawn)) && ht <= goal_ht) 
                || trays_drawn > kCurTrayType_Alt2)
            {
                tray_to_use = -1;
            }
            else
            {
                tray_to_use = trays_drawn;
            }
            
            // draw the tray slots
            for( i = 0; i < TRAY_SLOTS; i++ )
            {
                F32 xp = x + (35 + i * BOX_SIDE + BOX_SIDE/2)*scale;
                F32 yp = tray_y + (BOX_SIDE/2 + TRAY_YOFF)*scale;
                int current_tray = tray_to_use != -1 ? e->pl->tray->current_trays[tray_to_use] : tray_to_use;
                trayslot_Display( e, i, current_tray, tray_to_use, xp, yp, z, scale, color, WDW_TRAY );
            }

            // do the tray switching arrows
            if (tray_to_use != -1)
                tray_drawArrows( e, tray_to_use, x, tray_y, z, wd, ht, scale, color );
            
            if (goal_ht <= ht - DEFAULT_TRAY_HT*scale)
            {
                Wdw * wdw = wdwGetWindow( WDW_TRAY );
                float left_y = tray_y - left->height/2;
                float right_y = tray_y - right->height/2;
                float meat_y = tray_y - meat->height/2;
                
                display_sprite( left, x + 5, left_y, z-5, 1.f, 1.f, (color & 0xffffff00) | (int)(255*wdw->opacity) );
                display_sprite( right, x + wd - right->width - 5, right_y, z-5, 1.f, 1.f, (color & 0xffffff00) | (int)(255*wdw->opacity) );
                display_sprite( meat, x + left->width + 5, meat_y, z-5, (wd - left->width - right->width - 10)/meat->width, 1.f, (color & 0xffffff00) | (int)(255*wdw->opacity) );
            }                

            // increment the drawn trays count
            trays_drawn++;
        }
        while (ht > goal_ht);
    }
    
    // Let the player cancel a queued power.
    if(inpEdge(INP_ESCAPE))
    {
        tray_CancelQueuedPower();
        entity_CancelWindupPower(e);
        trayobj_ExitStance(e);
    }

    if(!cursor.target_world)
    {
        // If we haven't fought for a while, exit the stance.
        if(e->pchar->ppowStance!=NULL && timerSecondsSince2000() > (g_timeLastPower+25))
        {
            trayobj_ExitStance(e);
        }
    }

    PERFINFO_AUTO_START("tray_verifyNoDoubles", 1);
        tray_verifyNoDoubles(e->pl->tray, e->pchar->iCurBuild);
    PERFINFO_AUTO_STOP();

    return 0;
}


//------------------------------------------------------------
// determine the current tray. This is derived from
// a combination of global variables:
// s_TrayStickyMask and s_TrayMomentaryMask
// this is starting to lose meaning with alt and alt2
// trays.
// -AB: created :2005 Jan 26 03:37 PM
// -AB: refactored for new sticky tray order. this function is becoming meaningless :2005 Feb 08 05:30 PM
//----------------------------------------------------------
CurTrayType get_cur_tray( Entity *e )
{
    int i;
    assert(e);

    for( i = kCurTrayType_Count - 1; i >= 0;  --i )
    {
        CurTrayMask mask = curtraytype_to_altmask( i );
        if( s_TrayMomentaryMask & mask || s_TrayStickyMask & mask )
        {
            return e->pl->tray->current_trays[i];
        }
    }

    return -1;
}

void tray_setSticky( CurTrayType tray, int toggle )
{
    int mask = curtraytype_to_altmask( tray ); 
    if (toggle)
        s_TrayStickyMask |= mask;
    else
        s_TrayStickyMask &= ~mask;    
}



//------------------------------------------------------------
//  toggle the given tray's sticky state.
//----------------------------------------------------------
void  tray_toggleSticky( CurTrayType tray )
{
    int mask = curtraytype_to_altmask( tray );
    if( s_TrayStickyMask & mask )
    {
        s_TrayStickyMask &= ~mask;
    }
    else
    {
        s_TrayStickyMask |= mask;
    }
}

/* End of File */

