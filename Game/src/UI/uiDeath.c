

#include "win/win_init.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "player/playerState.h"
#include "player/player.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "formatter/smf_main.h"
#include "entity/character_base.h"
#include "arena/arenagame.h"
#include "entity/entity.h"
#include "bases/bases.h"
#include "entity/Supergroup.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "cmdparse/cmdcommon.h"

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xff4848ff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xff4848ff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.1f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

static TextAttribs gCountdownAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.5f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&game_12,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

int deathWindow(void)
{
    float x, y, z, wd, ht, sc;
    int color, back_color;
    static SMFBlock *smText, *smTimer;
    Entity *e;

       if( !window_getDims( WDW_DEATH, &x, &y, &z, &wd, &ht, &sc, &color, &back_color ) )
    {
        if( playerPtr() && !playerPtr()->pchar->attrCur.fHitPoints && !server_visible_state.disablegurneys)
            window_setMode( WDW_DEATH, WINDOW_GROWING );
        return 0;
    }

    if (!smText)
    {
        smText = smfBlock_Create();
        smTimer = smfBlock_Create();
        smf_SetFlags(smTimer, SMFEditMode_ReadOnly, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
    }

    if( playerPtr() && playerPtr()->pchar->attrCur.fHitPoints || server_visible_state.disablegurneys )
        window_setMode( WDW_DEATH, WINDOW_SHRINKING );

     drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, back_color );
    set_scissor(1);
    scissor_dims( x+PIX3*sc, y+PIX3*sc, wd - PIX3*2*sc, ht-PIX3*2*sc );

    gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*sc));
    if(amOnArenaMap())
    {
        smf_SetFlags(smText, SMFEditMode_ReadOnly, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Center, 0, 0, 0);
        if(getArenaStockLives())
        {
            smf_SetRawText(smText, textStd("WaitForResurrectRespawn"), false);
        }
        else
        {
            smf_SetRawText(smText, textStd("WaitForResurrectCam"), false);
        }
        smf_SetRawText(smTimer, getArenaRespawnTimerString(), false);
        smf_Display(smText, x+PIX3*2*sc, y+PIX3*2*sc, z+1, wd-PIX3*4*sc, 0, 0, 0, &gTextAttr, NULL); 
        smf_Display(smTimer, x+PIX3*2*sc, y+PIX3*2*sc+50, z+1, wd-PIX3*4*sc, 0, 0, 0, &gCountdownAttr, NULL);
        smf_SetFlags(smText, SMFEditMode_ReadOnly, 0, 0, 0, 0, 0, 0, 0, SMAlignment_Left, 0, 0, 0);
    }
    else
    {
        if (g_isBaseRaid)
        {
            smf_ParseAndDisplay(smText, textStd("BaseTransport"), x+PIX3*2*sc, y+PIX3*2*sc, z+1, wd-PIX3*4*sc, 0, 0, 0, &gTextAttr, NULL, 0, true); 

            if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - (PIX3+13)*sc, z+1, 140*sc, 20*sc, CLR_RED, "GoToBase", sc, 0))
            {
                respawnYes(1);
            }
            set_scissor(0);
            return 0;
        }
        // As well as the existance of a base gurney, we must now check if the player is dark, and disable the base rez option if so.
        if (isBaseGurney() && (e = playerPtr()) != NULL && !sgroup_isPlayerDark(e))
        {
#if 0
            bool mixed = isTeamMixedFactions();
#endif

            smf_ParseAndDisplay(smText, textStd("BaseHospitalTransport"), x+PIX3*2*sc, y+PIX3*2*sc, z+1, wd-PIX3*4*sc, 0, 0, 0, &gTextAttr, NULL, 0, true); 
            if(D_MOUSEHIT == drawStdButton(x + wd/4*1, y + ht - (PIX3+13)*sc, z+1, 100*sc, 20*sc, CLR_RED, "GoToHospital", sc, 0))
            {
                respawnYes(0);
            }
#if 0
            if(D_MOUSEHIT == drawStdButton(x + wd/4*3, y + ht - (PIX3+13)*sc, z+1, 100*sc, 20*sc, CLR_RED, "GoToBase", sc, mixed ? (BUTTON_LOCKED | BUTTON_DIMMED) : 0))
#else
            if(D_MOUSEHIT == drawStdButton(x + wd/4*3, y + ht - (PIX3+13)*sc, z+1, 100*sc, 20*sc, CLR_RED, "GoToBase", sc, 0))
#endif
            {
                respawnYes(1);
            }
            set_scissor(0);
            return 0;
        }
        smf_ParseAndDisplay(smText, textStd("HospitalTransport"), x+PIX3*2*sc, y+PIX3*2*sc, z+1, wd-PIX3*4*sc, 0, 0, 0, &gTextAttr, NULL, 0, true); 

        if(D_MOUSEHIT == drawStdButton(x + wd/2, y + ht - (PIX3+13)*sc, z+1, 140*sc, 20*sc, CLR_RED, "GoToHospital", sc, 0))
        {
            respawnYes(0);
        }
    }

    set_scissor(0);
    return 0;
}