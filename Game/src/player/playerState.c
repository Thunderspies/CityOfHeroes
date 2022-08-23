// DEBUG!! need to have a system that implies priorities for what states can overwrite other states.

#include "entity/entity.h"
#include "player/playerState.h"
#include "player/player.h"
#include <utilitieslib/network/netio.h>
#include "entity/entVarUpdate.h"
#include "UI/uiGame.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "render/tex.h"
#include "graphics/textureatlas.h"
#include "graphics/gfxwindow.h"
#include "entity/entclient.h"
#include "gridcoll/gridcoll.h"
#include <utilitieslib/components/HashFunctions.h>
#include "win/win_init.h"
#include "varutils.h"
#include "clientcomm/clientcomm.h"
#include "UI/uiChat.h"
#include "UI/uiCursor.h"
#include "UI/uiDialog.h"
#include "UI/uiStatus.h"
#include "UI/uiInput.h"
#include "player/pmotion.h"
#include "entity/costume_client.h"
#include "UI/uiWindows.h"
#include "entity/character_base.h"
#include "cmdparse/cmdcommon.h"
#include "entity/teamCommon.h"
#include "entity/character_target.h"

static ClientGameState  state;
static int s_baseGurney;

#define TIME_TO_WAIT_BEFORE_DISPLAYING_DEATH_DIALOG 75

char *states[] =
{
    "SIMPLE",
    "CREATE_TEAM_CONTAINER",
    "CREATE_TEAM_CONTAINER_WAIT_MAPSVRR_RESPONSE",
    "DEAD",
    "RESURRECT",
    "AWAITING_GURNEY_XFER"
};

//
//
void setState( ClientGameState  st, int baseGurney)
{
    if( state >= 0 && state < TOTAL_GAME_STATES && st >=0 && st < TOTAL_GAME_STATES )
        printf("Switching from state %s to state %s.\n", states[ state ], states[ st ] );

    state               = st;
    s_baseGurney = baseGurney;
}

Entity  *teleport_cursor;
static int teleport_range;

// NOTE to MW, This teleport thing is wack, There is no good reason to create another entity just to hold a Vec3
//
static void teleportCursorWork()
{
    Vec3    start, end;
    CollInfo coll;
    Entity *player = playerPtr();

    //collisions_off_for_rest_of_frame = TRUE;

    // x,y are 2d screen positions
    // len is how far to move endpoint
    // start and end are outputs
    // This creates the line from the focal point through the mouse cursor (no collision detection)
    gfxCursor3d( (F32)gMouseInpCur.x, (F32)gMouseInpCur.y, (F32)teleport_range, start, end );

    // if the line collided with anything, get the collision point, otherwise use the end of the line
    if (collGrid(0,start,end,&coll,0,COLL_DISTFROMSTART|COLL_HITANY))
        entUpdatePosInstantaneous( teleport_cursor, coll.mat[3] );
    else
        entUpdatePosInstantaneous( teleport_cursor, end );
}

#define MAX_UNCLAIMED_FLOATERS (128)

typedef struct UnclaimedFloater
{
    int svr_idx;
    int age;
    Floater floater;
} UnclaimedFloater;

UnclaimedFloater s_aUnclaimedFloaters[MAX_UNCLAIMED_FLOATERS];
static int s_iCntUnclaimedFloaters = 0;

/**********************************************************************func*
 * addFloatingInfo
 *
 */
void addFloatingInfo(int svr_idx, char *pch, U32 colorFG, U32 colorBG,
    U32 colorBorder, float fScale, float fTime, float fDelay, EFloaterStyle style, float *loc)
{
    int i;
    Entity *e = entFromId(svr_idx);

    if(e!=NULL && entity_TargetIsInVisionPhase(e, playerPtr()))
    {
        if(e->iNumFloaters<FLOATER_MAX_COUNT)
        {
            // Move delayed ones back if necessary
            for(i=e->iNumFloaters-1;
                i>=0 && e->aFloaters[i].fTimer<fDelay;
                i--)
            {
                e->aFloaters[i+1] = e->aFloaters[i];
            }

            strncpy(e->aFloaters[i+1].ach, pch, FLOATER_MAX_STRING_LEN-1);
            e->aFloaters[i+1].eStyle      = style;
            e->aFloaters[i+1].colorFG     = colorFG;
            e->aFloaters[i+1].colorBG     = colorBG;
            e->aFloaters[i+1].colorBorder = colorBorder;
            e->aFloaters[i+1].fScale      = fScale;
            e->aFloaters[i+1].fSpeed      = 1.0f;
            e->aFloaters[i+1].fMaxTimer   = fTime;
            e->aFloaters[i+1].fTimer      = -fDelay;
            e->aFloaters[i+1].iXLast      = -1;
            e->aFloaters[i+1].iYLast      = -1;
            e->aFloaters[i+1].bUseLoc     = (loc!=NULL);
            e->aFloaters[i+1].bScreenLoc  = (style==kFloaterStyle_Caption);
            if(loc) 
                copyVec3(loc,e->aFloaters[i+1].vecLocation);

            e->iNumFloaters++;
        }
    }
    else if(s_iCntUnclaimedFloaters<MAX_UNCLAIMED_FLOATERS)
    {
        Floater *pfloater = &s_aUnclaimedFloaters[s_iCntUnclaimedFloaters].floater;

        s_aUnclaimedFloaters[s_iCntUnclaimedFloaters].svr_idx = svr_idx;
        s_aUnclaimedFloaters[s_iCntUnclaimedFloaters].age = 0;

        strncpy(pfloater->ach, pch, FLOATER_MAX_STRING_LEN-1);
        pfloater->eStyle      = style;
        pfloater->colorFG     = colorFG;
        pfloater->colorBG     = colorBG;
        pfloater->colorBorder = colorBorder;
        pfloater->fScale      = fScale;
        pfloater->fSpeed      = 1.0f;
        pfloater->fMaxTimer   = fTime;
        pfloater->fTimer      = -fDelay;
        pfloater->iXLast      = -1;
        pfloater->iYLast      = -1;
        pfloater->bUseLoc     = (loc!=NULL);
        pfloater->bScreenLoc  = (style==kFloaterStyle_Caption);
        if(loc) 
            copyVec3(loc,pfloater->vecLocation);

        s_iCntUnclaimedFloaters++;
    }
}

/**********************************************************************func*
 * ClearUnclaimedFloaters
 *
 */
void ClearUnclaimedFloaters(void)
{
    int i;
    for(i=0; i<s_iCntUnclaimedFloaters; i++)
    {
        s_aUnclaimedFloaters[i].age++;
        if(s_aUnclaimedFloaters[i].age>4)
        {
            CopyStructsFromOffset(s_aUnclaimedFloaters + i, 1, ARRAY_SIZE(s_aUnclaimedFloaters)-i-1);
            i--; // Hack since we removed one and compacted
            s_iCntUnclaimedFloaters--;
        }
    }
}

/**********************************************************************func*
 * DistributeUnclaimedFloaters
 *
 */
void DistributeUnclaimedFloaters(Entity *e)
{
    int i;
    for(i=0; i<s_iCntUnclaimedFloaters; i++)
    {
        if(e->svr_idx == s_aUnclaimedFloaters[i].svr_idx)
        {
            s_aUnclaimedFloaters[i].svr_idx = 0;

            if(e->iNumFloaters<FLOATER_MAX_COUNT)
            {
                int j;
                int iExtra = 0;

                // Move delayed ones back if necessary
                for(j=e->iNumFloaters-1;
                    j>=0 && e->aFloaters[j].fTimer<-s_aUnclaimedFloaters[i].floater.fTimer;
                    j--)
                {
                    e->aFloaters[j+1] = e->aFloaters[j];
                }

                e->aFloaters[j+1] = s_aUnclaimedFloaters[i].floater;

                // Peel off the entity's name
                if(strnicmp(e->name, e->aFloaters[j+1].ach, strlen(e->name))==0)
                {
                    iExtra += strlen(e->name)+1;
                }
                while(e->aFloaters[j+1].ach[iExtra]==' ')
                {
                    iExtra++;
                }
                if(e->aFloaters[j+1].ach[iExtra]==':')
                {
                    iExtra++;
                    while(e->aFloaters[j+1].ach[iExtra]==' ')
                    {
                        iExtra++;
                    }
                }
                strcpy(e->aFloaters[j+1].ach, &e->aFloaters[j+1].ach[iExtra]);

                CopyStructsFromOffset(s_aUnclaimedFloaters + i, 1, ARRAY_SIZE(s_aUnclaimedFloaters)-i-1);
                i--; // Hack since we removed one and compacted

                e->iNumFloaters++;
                s_iCntUnclaimedFloaters--;
            }
        }
    }
}

/**********************************************************************func*
 * addFloatingDamage
 *
 */
void addFloatingDamage(Entity *victim, Entity *damager, int dmg, char *pch, float *loc, bool wasAbsorb)
{
    Entity *player = playerPtr();
    char pchTmp[FLOATER_MAX_STRING_LEN];
    U32 clr;
    float fScale = 1.8f;  

    if (!entity_TargetIsInVisionPhase(victim, player))
    {
        return;
    }

    if(dmg==0)
    {
        if(pch==NULL || *pch=='\0')
            return;
        fScale = 2.2f;
    }
    else
    {
        sprintf(pchTmp, "%+d", dmg);
        pch = pchTmp;
    }

    if(dmg > 0)
    {
        if (wasAbsorb)
            clr = 0xffffffff;
        else
            clr = 0x00ff0000; //green
    }
    else if( player && victim->db_id == player->db_id )
    {
        clr = 0xff000000; // red
    }
    else if( (damager && player && damager->db_id == player->db_id ) || // if I am the damager
             (damager && damager->erOwner && erGetEnt(damager->erOwner) == player) ||  // or if I own this entity
             (damager && damager->erCreator && erGetEnt(damager->erCreator) == player) )
    {
        clr = 0xff800000; // orange
    }
    else
    {
        clr = 0xa0a0a000; // light grey
    }

    addFloatingInfo(victim->svr_idx, pch, clr, clr, clr, fScale, MAX_FLOAT_DMG_TIME, 0.0f, kFloaterStyle_Damage, loc);
}

void respawnYes(bool bBase)
{
    if( entMode( playerPtr(), ENT_DEAD ) )
    {
        printf("client said yes\n");
        START_INPUT_PACKET( pak, CLIENTINP_REQUEST_GURNEY_XFER );
        pktSendBits( pak, 1, bBase );
        END_INPUT_PACKET
    }
}

//mw It's important to know that state is *not* a bit field, thus an
//entity can only be in one of these states at a time
void runState()
{
//  int wd, ht, top;
    Entity  *e = playerPtr();
    static F32 time_since_death = 0;

    // TODO: This should be unneeded now. -poz
    //if( isMenu( "Game" ) )
    //{
    //  // Detect dead player.
    //
    //  assert(e->pchar);
    //  if( e->pchar->attrCur.fHitPoints <= 0.0f &&
    //      state != STATE_DEAD &&
    //      state != STATE_AWAITING_GURNEY_XFER
    //      )
    //  {
    //      setState( STATE_DEAD, -1 );
    //
    //  }
    //}

    //Special timer code to make the DEAD dialog box only pop up after you've fallen down.
    if( state == STATE_DEAD)
        time_since_death += TIMESTEP;
    else
        time_since_death = 0;
    //End special timer

    switch( state )
    {
        case STATE_DEAD:
            // TODO: This is a hack. It seems like the HP get unsynched
            //       sometimes. This is a quick fix for now. --poz
            //if(e->pchar->attrCur.fHitPoints > 0.0f)
            //    e->pchar->attrCur.fHitPoints = 0.0f;

            if( time_since_death > TIME_TO_WAIT_BEFORE_DISPLAYING_DEATH_DIALOG)
            {
                if (!e) break;
                setState( STATE_AWAITING_GURNEY_XFER, s_baseGurney );
                if (!server_visible_state.disablegurneys)         // show respawn dialog unless suppressed
                     window_setMode( WDW_DEATH, WINDOW_GROWING );
            }

            // This has been replaced with the server specifically pushing
            // the client into STATE_SIMPLE after gurney transfer or
            // resurrection.
            //
            // TODO: Took this out since it's naughty. We shouldn't have any
            //       client-side resurrection. -poz
            //if( e && actualHp(e) > 0 )
            //  setState( STATE_SIMPLE, -1 );
        break;

        case STATE_AWAITING_GURNEY_XFER:
            // This has been replaced with the server specifically pushing
            // the client into STATE_SIMPLE after gurney transfer or
            // resurrection.
            //
            // TODO: Took this out since it's naughty. We shouldn't have any client-side
            //       resurrection. -poz
            //if( actualHp(e) > 0 )
            //  setState( STATE_SIMPLE, -1 );
        break;

        case STATE_RESURRECT:
            // Get rid of the hospital dialog box.
            setState(STATE_SIMPLE, s_baseGurney);
        break;

        case STATE_SIMPLE:
        case STATE_CREATE_TEAM_CONTAINER:
        case STATE_CREATE_TEAM_CONTAINER_WAIT_MAPSVRR_RESPONSE:
        break;
    }
}

bool isBaseGurney() {
    return s_baseGurney;
}

#if 0
bool isTeamMixedFactions() {
    bool retval = false;

    if (playerPtr() && playerPtr()->teamup->mixedFactions)
    {
        retval = true;
    }

    return retval;
}
#endif
