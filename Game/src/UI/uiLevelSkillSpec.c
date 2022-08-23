#include "stdtypes.h"

#include "uigame.h"
#include "uiUtil.h"
#include "uiUtilMenu.h"
#include "uiUtilGame.h"
#include "uiCombineSpec.h"
#include "uiPowers.h"
#include "uiInput.h"
#include "uiNet.h"
#include "uiFx.h"
#include "uiEditText.h"
#include "uiScrollBar.h"
#include "uiPowerInventory.h"
#include "uiLevelSpec.h"

#include "sprite_base.h"
#include "sprite_text.h"
#include "sprite_font.h"
#include "cmdgame.h"

#include "player.h"
#include "entVarUpdate.h"
#include "character_base.h"
#include "character_level.h"
#include "powers.h"
#include "origins.h"
#include "classes.h"
#include "earray.h"
#include "win_init.h"
#include "timing.h"
#include "clientcomm.h"
#include "contactCommon.h"
#include "ttFontUtil.h"
#include "sound.h"
#include "textureatlas.h"
#include "entity.h"

extern int gNumEnhancesLeft;

//-------------------------------------------------------------
#ifndef TEST_CLIENT

#define ENHANCE_MOVE_SPEED 10
#define ENHANCE_GROW_SPEED .1f
#define FRAME_WIDTH  320
#define FRAME_SPACE  (DEFAULT_SCRN_WD - 3*FRAME_WIDTH)/4
#define SL_FRAME_TOP 92
#define FRAME_HEIGHT 623
#define FRAME_HT	 623

#define POW_SPACER   50

#define SKILL_OFFSET 16
#define SKILL_FRAME_WIDTH (DEFAULT_SCRN_WD-5*SKILL_OFFSET)/4
#define SKILL_FRAME_SPACE (DEFAULT_SCRN_WD - 4*SKILL_FRAME_WIDTH)/5
#define FRAME_X(x)  (SKILL_OFFSET*(x+1) + x*SKILL_FRAME_WIDTH)

static void drawPowerAndEnhancements( Character *pchar, Power * pow, int x, int y, int z, float scale, int available, int *offset, int selectable)
{
	int i, hit, tx = x;
	EnhancementAnim *ea;
	AtlasTex *icon = atlasLoadTexture(pow->ppowBase->pchIconName);

 	if( stricmp( icon->name, "white" ) == 0 )
		icon = atlasLoadTexture( "Power_Missing.tga" );

	ea = enhancementAnim( pow );
	if( ea )
	{
		//enahncement_NewSlot( ea );
		for( i = 0; i < ea->count; i++ )
		{
			CBox box;
   			drawMenuEnhancement( NULL, x + (30 + SPEC_WD*.95f*(i+1))*scale, y + 20*scale, z+100, scale*.45f*ea->scale[i], kBoostState_Selected, 0, true, MENU_LEVEL_SKILL_SPEC, true);
			BuildCBox( &box, x + (30 + SPEC_WD*.95f*(i+1))*scale - SPEC_WD/2, y + 20*scale - SPEC_WD/2, SPEC_WD, SPEC_WD );
			if( mouseUpHit( &box, MS_LEFT) )
			{
				ea->shrink[i] = !ea->shrink[i];
				collisions_off_for_rest_of_frame = 1;
			}
		}
  		tx += ea->xoffset;
	}

   	hit = drawCheckBar( x+30, y, z, scale, SKILL_FRAME_WIDTH-45*scale, pow->ppowBase->pchDisplayName, 0, selectable, 1, 1, icon );

	if( hit == D_MOUSEHIT )
	{
		if( !gNumEnhancesLeft )
		{
			fadingText( DEFAULT_SCRN_WD/2, SL_FRAME_TOP+FRAME_HEIGHT-5, z+250, 1.5f, CLR_WHITE, CLR_WHITE, 30, "NoSlots" );
		}
		else if( (ea
			&& (ea->count+pow->iNumBoostsBought >= MAX_BOOSTS_BOUGHT
			|| ea->count+EArrayGetSize(&pow->ppBoosts) >= pow->ppowBase->iMaxBoosts))
			|| pow->iNumBoostsBought >= MAX_BOOSTS_BOUGHT
			|| EArrayGetSize(&pow->ppBoosts) >= pow->ppowBase->iMaxBoosts
			|| EArrayIntGetSize(&pow->ppowBase->pBoostsAllowed)<=0)
		{
			fadingText( x+POWER_OFFSET + POWER_WIDTH/2, y+15, z+250, 1.5f, CLR_RED, CLR_RED, 30, "FullString" );
		}
		else
		{
			if( enhancementAnim_add( pow ) )
			{
				ea = enhancementAnim( pow );
				ea->id[ea->count-1] = electric_init( x + (30 + SPEC_WD*.95f*ea->count)*scale, y + 20*scale + *offset, z+190, .8, 0, -1, 1, offset );
				sndPlay( "powerselect2", SOUND_GAME );
			}
		}
	}

	for( i = 0; i < EArrayGetSize(&pow->ppBoosts); i++ )
	{
		// do some availability checks here
		int clickable = TRUE;

		if( !available )
			clickable = FALSE;

		drawMenuPowerEnhancement( pchar, pow->psetParent->idx, pow->idx, i, tx + (30 + (i+1)*SPEC_WD*.95f)*scale,
       			y + 20*scale, z+40, scale*.45f, CLR_WHITE, false);
	}
}

//-----------------------------------------------------------------------------------------------------
// Main Loop //////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------


void levelSkillSpecMenu()
{
	Entity *e = playerPtr();
	static int init = FALSE;
	int i, color;
	bool bFakedLevel = false;
	static ScrollBar sb[4] = { 0 };

  	float primY =  110 - sb[0].offset, secY = 110 - sb[1].offset, 
 		  threeY = 110 - sb[2].offset, poolY = 110 - sb[3].offset;

	int one = 0, two = 0, three = 0;

	// HACK: Pretend that we're already a level higher to get the right options
 	if( character_SystemCalcExperienceLevel(e->pchar, kPowerSystem_Skills) > e->pchar->iWisdomLevel)
	{
		e->pchar->iWisdomLevel++;
		bFakedLevel = true;
	}

	drawBackground( NULL ); //atlasLoadTexture("_Enhancement_customizepower_MOCKUP.tga") );
	toggle_3d_game_modes( SHOW_NONE );
	drawMenuTitle( 10, 30, 20, 860, "AddEnhancementSlot", 1.f, !init );

	if( !init )
	{
		init = TRUE;
		gNumEnhancesLeft = character_SystemCountBuyBoostForLevel( e->pchar, kPowerSystem_Skills, e->pchar->iWisdomLevel);
	}

	// the frames
   	drawMenuFrame( R12, FRAME_X(0), 70, 10, SKILL_FRAME_WIDTH, FRAME_HT+2*R12, CLR_WHITE, 0x00000088, 0 ); // left
 	drawMenuFrame( R12, FRAME_X(1), 70, 10, SKILL_FRAME_WIDTH, FRAME_HT+2*R12, CLR_WHITE, 0x00000088, 0 ); // middle
  	drawMenuFrame( R12, FRAME_X(2), 70, 10, SKILL_FRAME_WIDTH, FRAME_HT+2*R12, CLR_WHITE, 0x00000088, 0 ); // right
	drawMenuFrame( R12, FRAME_X(3), 70, 10, SKILL_FRAME_WIDTH, FRAME_HT+2*R12, CLR_WHITE, 0x00000088, 0 ); // left

	font( &game_12 );
	if( !gNumEnhancesLeft )
	{
		font_color( CLR_GREEN, CLR_GREEN );
		color = CLR_GREEN;
	}
	else
	{
		font_color( CLR_RED, CLR_RED );
		color = CLR_RED;
	}

 	drawFlatFrame( PIX3, R10, DEFAULT_SCRN_WD/2 - 100, SL_FRAME_TOP+FRAME_HEIGHT-20, 50, 200, 30, 1.f, color, CLR_BLACK ); // middle
	cprntEx( DEFAULT_SCRN_WD/2, SL_FRAME_TOP+FRAME_HEIGHT-5, 55, 1.f, 1.f, (CENTER_X|CENTER_Y), "EnhancmentsAllocated", gNumEnhancesLeft );
 	scissor_dims( 0, 74, 2000, FRAME_HEIGHT - PIX4*2 - 1 );

	// The zeroth slot in power sets is for temporary powers, which don't have specs
	for( i = 1; i < EArrayGetSize(&e->pchar->ppPowerSets); i++ )
	{
		int j, top;

   		if( e->pchar->ppPowerSets[i]->psetBase->bShowInManage )
		{
			if( !one && e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Skill] )
			{
				top = primY;
 				font_color( CLR_YELLOW, CLR_ORANGE );
				cprnt( FRAME_X(0) + SKILL_FRAME_WIDTH/2, top-23, 20, 1.f, 1.f, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

				set_scissor(TRUE);
				for( j = 0; j < EArrayGetSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
				{
					Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
					if( pow->ppowBase->bShowInManage )
					{

  						drawPowerAndEnhancements( e->pchar, pow, FRAME_X(0), primY, 20, 1.f, FALSE, &sb[0].offset, TRUE );
   						primY += SKILL_SPEC_HT;
					}
				}
				primY += SKILL_SPEC_HT;
				one = true;
			}
			else if( !two &&  e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Skill] )
			{
				top = secY;
				font_color( CLR_YELLOW, CLR_ORANGE );
				cprnt( FRAME_X(1) + SKILL_FRAME_WIDTH/2, top-23, 20, 1.f, 1.f, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

				set_scissor(TRUE);
				for( j = 0; j < EArrayGetSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
				{
					Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
					if( pow->ppowBase->bShowInManage )
					{
						drawPowerAndEnhancements( e->pchar, pow, FRAME_X(1), secY, 20, 1.f, FALSE, &sb[1].offset, TRUE );
						secY += SKILL_SPEC_HT;
					}
				}

				secY += SKILL_SPEC_HT;
				two = true;
			}
			else if( !three &&  e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Skill] )
			{
				top = threeY;
				font_color( CLR_YELLOW, CLR_ORANGE );
				cprnt( FRAME_X(2) + SKILL_FRAME_WIDTH/2, top-23, 20, 1.f, 1.f, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

				set_scissor(TRUE);
				for( j = 0; j < EArrayGetSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
				{
					Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
					if( pow->ppowBase->bShowInManage )
					{
						drawPowerAndEnhancements( e->pchar, pow, FRAME_X(2), threeY, 20, 1.f, FALSE, &sb[1].offset, TRUE );
						threeY += SKILL_SPEC_HT;
					}
				}
				threeY += SKILL_SPEC_HT;
				three = true;
			}
			else if( e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Skill] )
			{
				top = poolY;
 				font_color( CLR_YELLOW, CLR_ORANGE );
				cprnt( FRAME_X(3) + SKILL_FRAME_WIDTH/2, top-23, 20, 1.f, 1.f, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName );

				set_scissor(TRUE);
				for( j = 0; j < EArrayGetSize(&e->pchar->ppPowerSets[i]->ppPowers); j++ )
				{
					Power *pow = e->pchar->ppPowerSets[i]->ppPowers[j];
					if( pow->ppowBase->bShowInManage )
					{
						drawPowerAndEnhancements( e->pchar, pow, FRAME_X(3), poolY, 20, 1.f, FALSE, &sb[2].offset, TRUE);
 						poolY += SKILL_SPEC_HT;
					}
				}
				poolY += SKILL_SPEC_HT;
			}
		}
		set_scissor(FALSE);
	}
	//wrappedText( 40, 650, 10, 940, "EnhancementExplanation", INFO_CHAT_TEXT );

  	doScrollBar( &sb[0], FRAME_HEIGHT-R12*2, primY + sb[0].offset, FRAME_X(0) + SKILL_FRAME_WIDTH, SL_FRAME_TOP + R12, 20 );
	doScrollBar( &sb[1], FRAME_HEIGHT-R12*2, secY  + sb[1].offset, FRAME_X(1) + SKILL_FRAME_WIDTH, SL_FRAME_TOP + R12, 20 );
	doScrollBar( &sb[2], FRAME_HEIGHT-R12*2, threeY + sb[2].offset, FRAME_X(2) + SKILL_FRAME_WIDTH, SL_FRAME_TOP + R12, 20 );
	doScrollBar( &sb[3], FRAME_HEIGHT-R12*2, poolY + sb[3].offset, FRAME_X(3) + SKILL_FRAME_WIDTH, SL_FRAME_TOP + R12, 20 );

	enhancementAnim_update(kPowerSystem_Skills);

	if ( D_MOUSEHIT == drawStdButton( FRAME_SPACE + 50, 740, 10, 100, 40, CLR_DARK_RED, "CancelString", 2.f, 0) )
	{
		enhancementAnim_clear();
		fadingText_clearAll();
		electric_clearAll();
		init = 0;

		// Continue the dialog with the NPC
		START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
		pktSendBitsPack(pak, 1, CONTACTLINK_BYE);
		END_INPUT_PACKET

		start_menu( MENU_GAME );
	}

	if ( D_MOUSEHIT == drawStdButton( DEFAULT_SCRN_WD - FRAME_SPACE - 50, 740, 10, 100, 40, CLR_GREEN, "FinishString", 2.f, (gNumEnhancesLeft>0)) )
	{
		spec_sendAddSlot( 0, kPowerSystem_Skills );

		enhancementAnim_clear();
		fadingText_clearAll();
		electric_clearAll();
		init = 0;

		// Continue the dialog with the NPC
		START_INPUT_PACKET(pak, CLIENTINP_CONTACT_DIALOG_RESPONSE);
		pktSendBitsPack(pak, 1, CONTACTLINK_HELLO);
		END_INPUT_PACKET

		level_increaseLevel( kPowerSystem_Skills ); // Make the level increase permanent. Do this after everything else.

		start_menu( MENU_GAME );
	}

	// HACK: stop pretending
	if(bFakedLevel)
	{
		e->pchar->iWisdomLevel--;
	}
}



#endif // TEST_CLIENT
