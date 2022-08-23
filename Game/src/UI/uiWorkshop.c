/***************************************************************************
 *     Copyright (c) 2005-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "UI/uiCursor.h"
#include "UI/uiDialog.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "UI/uiTray.h"
#include "UI/uiCursor.h"
#include "UI/uiToolTip.h"
#include "UI/uiGame.h"
#include "UI/uiNet.h"
#include "UI/uiContextMenu.h"
#include "UI/uiWindows_init.h"
#include "UI/uiScrollBar.h"
#include "UI/uiInput.h"
#include "UI/uiOptions.h"
#include "UI/uiGift.h"

#include "sound.h"

#include "character_base.h"
#include "character_workshop.h"
#include "character_eval.h"
#include "wdwbase.h"
#include "win_init.h"
#include "powers.h"
#include "player.h"
#include "entity.h"
#include "entplayer.h"
#include "entVarUpdate.h"
#include "clientcomm.h"
#include "cmdcommon.h"
#include "entity_power_client.h"
#include "input.h"
#include "cmdcommon.h"
#include "cmdgame.h"
#include "sprite_base.h"
#include "sprite_text.h"
#include "sprite_font.h"
#include "textureatlas.h"
#include "mathutil.h"
#include "character_inventory.h"
#include "character_workshop.h"
#include "DetailRecipe.h"

#include "strings_opt.h"
#include "earray.h"
#include "smf_main.h"
#include "groupThumbnail.h"
#include "bases.h"
#include "basedata.h"
#include "uiTabControl.h"
#include "uiComboBox.h"
#include "MessageStoreUtil.h"

static bool s_bReparse = 0;

static int s_Hide;
static int s_Empowerment; //Hack for now 

ContextMenu * s_WorkshopRecipeContext = 0;
ContextMenu * s_WorkshopRecipeNoGiftContext = 0;

typedef enum {
	HIDE_MISSING_SALVAGE = 1,
	HIDE_WRONG_TABLE = 2,
	HIDE_DETAIL = 4,
	HIDE_COMPONENT = 8,
};

void workshopReparse(void) {
	s_bReparse = 1;
}

static TextAttribs s_taDefaults =
{
	/* piBold        */  (int *)0,
	/* piItalic      */  (int *)0,
	/* piColor       */  (int *)0xffffffff,
	/* piColor2      */  (int *)0,
	/* piColorHover  */  (int *)0xffffffff,
	/* piColorSelect */  (int *)0xff0000ff,
	/* piColorSelectBG*/ (int *)0x333333ff,
	/* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
	/* piFace        */  (int *)&smfSmall,
	/* piFont        */  (int *)0,
	/* piAnchor      */  (int *)0,
	/* piLink        */  (int *)0x80e080ff,
	/* piLinkBG      */  (int *)0,
	/* piLinkHover   */  (int *)0x66ff66ff,
	/* piLinkHoverBG */  (int *)0,
	/* piLinkSelect  */  (int *)0xff3333ff,
	/* piLinkSelectBG*/  (int *)0x666666ff,
	/* piOutline     */  (int *)1,
	/* piShadow      */  (int *)0,
};

typedef struct RecipeDisplayState {
	int index;
	SMFBlock recipeFormat;
	int expanded;
	int have_needed;
	int usable_here;
	int special;
	int inventory_display;				// is this a personal recipe and we are in a personal recipe display state
	int count;							// count of how many of this recipe the player has
	DetailRecipe *recipe;
	StuffBuff text;
	SalvageInventoryItem **components;
	AtlasTex *icon;
	AtlasTex *icon2;
} RecipeDisplayState;
static int lastExpandedItemUID = -1;

// Caches the formatting for each contact. Parsing and formatting are rather
// slow, so it's good to cache it.
static RecipeDisplayState **recipeDisplayStates = NULL;
static DetailRecipe **detailRecipes = NULL;
// List of workshops needed by items we can make
static Detail **neededWorkshops = NULL;

// Update the recipeDisplayState, -1 on error, 0 if same, 1 if updated
static int updateRecipeDisplayState(RecipeDisplayState *dis, DetailRecipe *dr, int expanded)
{
	Entity *e = playerPtr();
	RoomDetail *pdetail = baseGetDetail(&g_base, e->pl->detailInteracting, NULL);
	char *pWorkshopType = e->pl->workshopInteracting;
	int size=EArrayGetSize(&dr->ppWorkshops);
	int sizeSalReq = EArrayGetSize(&dr->ppSalvagesRequired);
	int i,j;
	static Detail **workshops;

	if (size <= 0)
		return 0;

	if (dis->recipe == dr && expanded == dis->expanded && !s_bReparse) 
		return 0;

	if (!verify(dis && dr)) 
		return -1;

	if (dis->text.size > 0) freeStuffBuff(&dis->text);
	initStuffBuff(&dis->text, 1024);
	if (dis->components) {
		EArrayDestroy(&dis->components);
	}
	EArrayCreate(&dis->components);

	dis->have_needed = character_DetailRecipeCreatable(e->pchar, dr, &dis->components, false, false);
	dis->expanded = expanded;
	dis->recipe = dr;
	if (pdetail || pWorkshopType)
		dis->special = dr->ppchRequires && *dr->ppchRequires ? 1 : 0;
	else 
		dis->special = 0;
	dis->usable_here = false;
	dis->inventory_display = (dr->Type == kRecipeType_Drop) && !pdetail && (!pWorkshopType || pWorkshopType[0] == 0);

	if (dr->Type == kRecipeType_Drop)
	{
		dis->count = character_RecipeCount(e->pchar, dr);
	} else {
		dis->count = 0;
	}

	// See if this recipe can be used at this worktable
	if(size==0 && !pWorkshopType[0])
	{
		dis->usable_here = true;
	}
	else
	{
		for(i=0; i<size; i++)
		{
			if (dr->ppWorkshops[i] != NULL) 
			{
				if(pdetail && dr->ppWorkshops[i] == pdetail->info)
				{
					dis->usable_here = true;
					break;
				}
				if(pWorkshopType && stricmp(pWorkshopType, dr->ppWorkshops[i]->pchName) == 0)
				{
					dis->usable_here = true;
					break;
				}
				else if (dis->have_needed)
				{ // Add to the static array of workshops
					if (EArrayFindIndex(&neededWorkshops,dr->ppWorkshops[i]) == -1)
						EArrayPush(&neededWorkshops,dr->ppWorkshops[i]);
				}
			}
		}
		if (i == size && i != 0)
		{
			workshops = dr->ppWorkshops;
		}
	}

	if (pdetail)
	{ // if we don't have a detail, don't check it for stuff
		// Check to see if all salvage can be used at this table
		for(i = 0; i < sizeSalReq; i++)
		{
			unsigned int curcount = 0;
			SalvageRequired *s = dr->ppSalvagesRequired[i];
			if (!s || !s->pSalvage)
			{
				dis->usable_here = false;
				continue;
			}
			size = EArrayGetSize(&s->pSalvage->ppWorkshops);
			for(j=0; j<size; j++)
			{
				if(s->pSalvage->ppWorkshops[j] ==pdetail->info)
				{
					break;
				}
				else if (dis->have_needed)
				{
					if (EArrayFindIndex(&neededWorkshops,s->pSalvage->ppWorkshops[j]) == -1)
						EArrayPush(&neededWorkshops, s->pSalvage->ppWorkshops[j]);
				}
			}
			if (j == size && size != 0)
			{ //we didn't find the workshop, let them make it anyway for now.
				//dis->usable_here = false;
				//workshops = s->pSalvage->ppWorkshops;
			}
		}
	}

	if ((!dis->have_needed || !dis->usable_here) && !dis->inventory_display)
		addStringToStuffBuff(&dis->text,"<color gray>");

	if(dis->usable_here || dis->inventory_display)
	{
		addStringToStuffBuff(&dis->text,"%s",textStd(dr->ui.pchDisplayHelp));
		if (expanded)
		{
			int cost = 0;

			if (e->pchar && dr->CreationCost != NULL)
				cost = chareval_Eval(e->pchar, dr->CreationCost);

			// check for influence cost
			if (cost > 0)
			{
				addStringToStuffBuff(&dis->text,"<br><table><tr border=0><td width=60%%><b>%s</b></td>",textStd("Influence"));
				if (cost > e->pchar->iInfluencePoints) {
					addStringToStuffBuff(&dis->text,"<color red><td width=40%% align=right><b>%d</b></td></color>",cost);
				} else {
					addStringToStuffBuff(&dis->text,"<td width=40%% align=right><b>%d</b></td>",cost);
				}
				addStringToStuffBuff(&dis->text,"</tr></table>");
			}

			// list required salvage
			addStringToStuffBuff(&dis->text,"<br><table><tr border=0><td><b>%s</b></td>",textStd("UiInventoryTypeSalvage"));
			addStringToStuffBuff(&dis->text,"<td width=20%% align=right><b>%s</b></td>",textStd("NeedString"));
			addStringToStuffBuff(&dis->text,"<td width=20%% align=right><b>%s</b></td></tr>",textStd("HaveString"));

			for(i = 0; i < sizeSalReq; i++)
			{
				unsigned int curcount = 0;
				SalvageRequired *s = dr->ppSalvagesRequired[i];
				if (!s || !s->pSalvage) {
					continue;
				}
				if (dis->components[i]) curcount = dis->components[i]->amount;
				if (curcount < s->amount) {
					addStringToStuffBuff(&dis->text,"<color red><tr border=0><td>%s:</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</color></td></tr>",textStd(s->pSalvage->ui.pchDisplayName),s->amount,curcount);
				}
				else addStringToStuffBuff(&dis->text,"<tr border=0><td>%s:</td><td width=20%% align=right>%d</td><td width=20%% align=right>%d</td></tr>",textStd(s->pSalvage->ui.pchDisplayName),s->amount,curcount);
			}
			addStringToStuffBuff(&dis->text,"</table>");
		}
	}
	else
	{
		if(dis->have_needed)
			addStringToStuffBuff(&dis->text,"%s",textStd("CantBuildHaveSalvage"));
		else
			addStringToStuffBuff(&dis->text,"%s",textStd("CantBuild"));
		addStringToStuffBuff(&dis->text,textStd("ToBuildUse"));
		size = EArrayGetSize(&workshops);
		for(j=0;j<size;j++)
		{
			if (j != 0)
				addStringToStuffBuff(&dis->text,", ");
			addStringToStuffBuff(&dis->text,"%s",textStd(workshops[j]->pchDisplayName));
		}
		dis->expanded = false;
	}
  
	if ((!dis->have_needed || !dis->usable_here) && !dis->inventory_display)
		addStringToStuffBuff(&dis->text,"</color>"); 

	return 1;
}

static void updateRecipeIcon(RecipeDisplayState *dis, DetailRecipe *dr, int expanded)
{
	if(dr->pDetailIcon)
	{
		if (dis->expanded && dis->have_needed)
		{
			dis->icon = groupThumbnailGet(detailGetThumbnailName(dr->pDetailIcon), 1, 0, 0);
		}
		else if (!dis->icon || s_bReparse)
		{			
			dis->icon = groupThumbnailGet(detailGetThumbnailName(dr->pDetailIcon), 0, 0, 0);
		}
	}
	if(dr->ui.pchIcon)
	{
		dis->icon2 = atlasLoadTexture(dr->ui.pchIcon);
	}	
}




// Create a fake recipe to display the needed workshops
void setupFakeRecipe(RecipeDisplayState *dis)
{
	int j,size;
	dis->have_needed = 1;
	dis->usable_here = 1;
	dis->icon2 = atlasLoadTexture("Icon_clue_generic.tga");
	if (dis->text.size > 0) freeStuffBuff(&dis->text);
	initStuffBuff(&dis->text, 1024);
	addStringToStuffBuff(&dis->text,"%s",textStd("WrongTableText"));
	size = EArrayGetSize(&neededWorkshops);
	for(j=0;j<size;j++)
	{
		if (j != 0)
			addStringToStuffBuff(&dis->text,", ");
		addStringToStuffBuff(&dis->text,"%s",textStd(neededWorkshops[j]->pchDisplayName));
	}
}

static void collapseAllRecipes()
{
	int i;
	for(i = EArrayGetSize(&recipeDisplayStates)-1; i >= 0; i--)
	{
		RecipeDisplayState *rd = recipeDisplayStates[i];
		updateRecipeDisplayState(rd,rd->recipe,0);
	}
}

static void setExpandRecipe(int uid, int value)
{
	int i;
	for(i = EArrayGetSize(&recipeDisplayStates)-1; i >= 0; i--)
	{
		if(i == uid)
		{
			RecipeDisplayState *rd = recipeDisplayStates[i];
			assert(EArrayGetSize(&recipeDisplayStates) > i);
			updateRecipeDisplayState(rd,rd->recipe,value);

		}
	}
}



/////////////////////////////////////////////////////////////////////////////////
RecipeDisplayState *deleteme_rd;
static DialogCheckbox deleteDCB[] = { { "HideDeleteRecipePrompt", 0, kUO_HideDeleteRecipePrompt } };

void recipe_delete()
{
	Entity  * e = playerPtr();
	if(!deleteme_rd)
		return;

	sendRecipeDelete( deleteme_rd->recipe->pchName, 1 );
}

static void cmrecipe_Delete( void * data )
{
	Entity  * e = playerPtr();
	deleteme_rd = (RecipeDisplayState *) data;

	if (optionGet(kUO_HideDeleteRecipePrompt))
		recipe_delete();
	else
		dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallyDeleteRecipe", NULL, recipe_delete, NULL, NULL,0,sendOptions,deleteDCB,1,0,0 );
	sndPlay( "Reject5", SOUND_GAME );
}



/////////////////////////////////////////////////////////////////////////////////
void recipe_sell()
{
	Entity  * e = playerPtr();
	if(!deleteme_rd)
		return;

	sendRecipeSell( deleteme_rd->recipe->pchName, 1 );
}

static void cmrecipe_Sell( void * data )
{
	Entity  * e = playerPtr();
	deleteme_rd = (RecipeDisplayState *) data;

	dialog( DIALOG_YES_NO, -1, -1, -1, -1, "ReallySellRecipe", NULL, recipe_sell, NULL, NULL,0,sendOptions,NULL,1,0,0 );
	sndPlay( "Reject5", SOUND_GAME );
}

static int cmrecipe_canSell( void * data )
{
	RecipeDisplayState *pRecDetail = (RecipeDisplayState *) data;
	if (pRecDetail->recipe->SellToVendor > 0)
		return CM_AVAILABLE;
	else 
		return CM_HIDE;
} 

/////////////////////////////////////////////////////////////////////////////////


static void initWorkshopContext()
{
	s_WorkshopRecipeContext = contextMenu_Create(NULL);
	s_WorkshopRecipeNoGiftContext = contextMenu_Create(NULL);

	// the commands
//	contextMenu_addVariableTitle( s_SalvageContext, cmsalvage_Name, 0);
//	contextMenu_addVariableText( s_SalvageContext, cmsalvage_Desc, 0);
//	contextMenu_addCode( s_SalvageContext, alwaysAvailable, 0, cmsalvage_Info, 0, "CMInfoString", 0  );
	contextMenu_addCode( s_WorkshopRecipeContext, alwaysAvailable, 0, cmrecipe_Delete, 0, "CMRemoveRecipe", 0  );
	contextMenu_addCode( s_WorkshopRecipeContext, cmrecipe_canSell, 0, cmrecipe_Sell, 0, "CMSellRecipe", 0  );
//	contextMenu_addVariableTextCode( s_SalvageContext, cmsalvage_canMove, 0, cmsalvage_MoveOne, 0, cmsalvage_moveOneText, 0, 0  );
//	contextMenu_addVariableTextCode( s_SalvageContext, cmsalvage_canMove, 0, cmsalvage_MoveStack, 0, cmsalvage_moveStackText, 0, 0  );

	/*
	contextMenu_addVariableTitle( s_SalvageContextNoGift, cmsalvage_Name, 0);
	contextMenu_addVariableText( s_SalvageContextNoGift, cmsalvage_Desc, 0);
	contextMenu_addCode( s_SalvageContextNoGift, alwaysAvailable, 0, cmsalvage_Info, 0, "CMInfoString", 0  );
	contextMenu_addCode( s_SalvageContextNoGift, alwaysAvailable, 0, cmsalvage_Delete, 0, "CMRemoveSalvage", 0  );
	contextMenu_addVariableTextCode( s_SalvageContextNoGift, cmsalvage_canMove, 0, cmsalvage_MoveOne, 0, cmsalvage_moveOneText, 0, 0  );
	contextMenu_addVariableTextCode( s_SalvageContextNoGift, cmsalvage_canMove, 0, cmsalvage_MoveStack, 0, cmsalvage_moveStackText, 0, 0  );
	*/

	gift_addToContextMenu(s_WorkshopRecipeContext);
}


void workshopContextMenu(RecipeDisplayState *rd, int wdw)
{
	int mx, my;
	static TrayObj to;

	if(!s_WorkshopRecipeContext)
		initWorkshopContext();

 	rightClickCoords(&mx,&my);
	contextMenu_setAlign( s_WorkshopRecipeContext, mx, my, CM_LEFT, CM_TOP, rd);
}

#define BORDER_SPACE    15
#define RECIPE_SPACE   5

static void displayRecipe(RecipeDisplayState *rd, int xorig, float *y, int z, int wdorig, float sc, int color, int visible)
{
	CBox box;
	int ht = 0;
	int x = xorig;
	int yorig = *y;
	int wd = wdorig;
	int foreColor = CLR_NORMAL_FOREGROUND;
	int backColor = CLR_NORMAL_BACKGROUND;
	Costume *pCostume = NULL;
	int index = rd->index;
	float iconSize = 32.0;
	int usable = (rd->recipe && rd->have_needed && (rd->usable_here || rd->inventory_display)) ;

	static float timer = 0;

	*y += PIX3*sc;
	x  += (PIX3+4)*sc;
	wd -= (PIX3*2+4)*sc;

	if (!verify(rd)) return; 

	font( &game_14 );

	// Picture

	if (rd->expanded)
		iconSize = 64.0;

	if (rd->recipe && rd->recipe->level > 0) { 
		font_color (0xffffffff, 0xffffffff);
		cprnt( x+iconSize*sc/2, *y+8*sc+iconSize*sc, z+2, sc*iconSize/60.0f, sc*iconSize/60.0f, "%d", rd->recipe->level); 
	}

	if (rd->icon && visible)
	{
		if (usable) 
			display_sprite(rd->icon, x, *y+4*sc, z, iconSize*sc/rd->icon->width, iconSize*sc/rd->icon->height, CLR_WHITE );
		else 
			display_sprite(rd->icon, x, *y+4*sc, z, iconSize*sc/rd->icon->width, iconSize*sc/rd->icon->height, 0xFFFFFF44 );
	}
	if (rd->icon2 && visible)
	{
		if (usable) 
			display_sprite(rd->icon2, x, *y+4*sc, z+1, iconSize*sc/rd->icon2->width, iconSize*sc/rd->icon2->height, CLR_WHITE );
		else 
			display_sprite(rd->icon2, x, *y+4*sc, z+1, iconSize*sc/rd->icon2->width, iconSize*sc/rd->icon2->height, 0xFFFFFF44 );
	}
	if (rd->icon || rd->icon2)
	{
		x  += (RECIPE_SPACE + iconSize)*sc;
		wd -= (RECIPE_SPACE + iconSize)*sc;
	}
 
	if (usable) 
		font_color(0x00deffff, 0x00deffff);
	else 
		font_color (0x0deff88, 0x00deff88);

	if (visible)
	{
		if (rd->recipe)
		{
			prnt( x+4*sc, *y + 18*sc, z, sc, sc, rd->recipe->ui.pchDisplayName);
			if (rd->count > 0)
				prnt( x+wd-(10+RECIPE_SPACE)*sc, *y + 18*sc, z, sc, sc, "%d", rd->count); 

		}
		else
		{ // This is the fake recipe
			prnt( x+4*sc, *y + 18*sc, z, sc, sc, "WrongTableTitle");
		}
	}

	s_taDefaults.piScale = (int*)((int)(0.85f*sc*SMF_FONT_SCALE));
	if (visible)
	{
		ht = smf_ParseAndDisplay(&rd->recipeFormat,
			rd->text.buff,
			x+5*sc, *y+20*sc, z,
			wd-(10+RECIPE_SPACE)*sc, 0,
			false, false, &s_taDefaults, NULL,
			"uiWorkshop.c::displayRecipe", true);
	}
	else
	{ //don't display it, but still find the height needed
		ht = smf_ParseAndFormat(&rd->recipeFormat,
			rd->text.buff,
			x+5*sc, *y+20*sc, z,
			wd-(10+RECIPE_SPACE)*sc, 0,
			false, false, &s_taDefaults);
	}

	ht = max(ht+(25 +RECIPE_SPACE)*sc,(iconSize+PIX3*2+4+4)*sc);

	BuildCBox( &box, xorig, *y, wdorig, ht);

	// set the context menu
	if( rd->recipe && rd->recipe->Type == kRecipeType_Drop && mouseClickHit( &box, MS_RIGHT ) )
	{
		workshopContextMenu(rd, wd);
	}

	*y += ht;
	if(!(rd->recipe && (rd->usable_here || rd->inventory_display))) 
	{
		// Possibly do something for non-makeable and fake recipes
	}
	else if(!rd->expanded)
	{
		if (visible)
		{		
			if(D_MOUSEHIT == drawStdButton(xorig+(wdorig-27*sc), *y-5*sc, (float)z, 25*sc, 20*sc, window_GetColor(WDW_WORKSHOP), ">>", sc, 0))
			{
				setExpandRecipe(index,1);
			}
		}
	}
	else
	{
		*y += 16 *sc;
		ht += 16 *sc;
		if (visible)
		{
			if(D_MOUSEHIT == drawStdButton(xorig+(wdorig-27*sc), *y-5*sc, (float)z, 25*sc, 20*sc, window_GetColor(WDW_WORKSHOP), "<<", sc, 0))
			{
				setExpandRecipe(index,0);
			}
			if (!rd->inventory_display) 
			{
				if(D_MOUSEHIT == drawStdButton(xorig+(PIX3+3 + iconSize / 2)*sc, *y-15*sc, (float)z, 51*sc, 20*sc, window_GetColor(WDW_WORKSHOP), "CreateString", sc, !rd->have_needed)) {
					Entity *e = playerPtr();
					character_WorkshopSendDetailRecipeBuild(e->pchar, rd->recipe->pchName, DETAILRECIPE_USE_RECIPE_LEVEL, false );
				}
			}
		}
	}

	if (!rd->recipe || rd->recipe->Type == kRecipeType_Drop) 
	{
		foreColor = CLR_NORMAL_FOREGROUND;
		backColor = CLR_NORMAL_BACKGROUND;
	} else {
		switch (rd->recipe->Rarity) {
			case kRecipeRarity_Common:
				backColor = foreColor = CLR_NORMAL_FOREGROUND;
				break;
			case kRecipeRarity_Uncommon:
				backColor = foreColor = CLR_GOLD_FOREGROUND;
				break;
			case kRecipeRarity_Rare:
				backColor = foreColor = CLR_ORANGE_FOREGROUND;
				break;
			case kRecipeRarity_VeryRare:
				backColor = foreColor = CLR_PURPLE_FOREGROUND;
				break;
		}
	}


	if( usable && visible) 
	{
		drawFlatFrame(PIX2, R10, xorig, yorig, z-0.5, wdorig, ht, sc, foreColor, backColor);
	}

	if (visible)
		drawFlatFrame(PIX2, R10, xorig, yorig, z-0.5, wdorig, ht, sc, foreColor, backColor);

	*y += RECIPE_SPACE*sc; // Space between contacts
}

static int compareRecipe(const DetailRecipe** a, const DetailRecipe** b)
{  
	static char temp[128];
	int result;

	if ((*a)->Type == kRecipeType_Drop && !(*b)->Type == kRecipeType_Drop)
		return -1;
	else if ((*b)->Type == kRecipeType_Drop && !(*a)->Type == kRecipeType_Drop)
		return 1;

	if (!(*a)->pchDisplayTabName)
		return -1;
	if (!(*b)->pchDisplayTabName)
		return 1;
	strcpy(temp,textStd((*a)->pchDisplayTabName));
	result = stricmp( temp,textStd((*b)->pchDisplayTabName) );
	if (result)
		return result;
	strcpy(temp,textStd((*a)->ui.pchDisplayName));
	result = stricmp( temp,textStd((*b)->ui.pchDisplayName) );
	if (result)
		return result;
	return (*a)->id - (*b)->id;
}

static uiTabControl *workshopCategories = NULL;

#define CBOXHEIGHT 21

int workshopWindow()
{
	float x, y, z, wd, ht, sc;
	int color, bcolor;
	int i,size;
	float oldy;
	Entity *e = playerPtr();
	static int init = 1;
	char *curTab;
	static ScrollBar sb = { WDW_WORKSHOP, 0 };
	static RecipeDisplayState fakeState;
	static ComboBox hideBox = {0};
 
	// disable once invention comes online
	if (INVENTION_ENABLED && window_getMode( WDW_WORKSHOP ) != WINDOW_DOCKED)
	{
		window_setMode( WDW_WORKSHOP, WINDOW_DOCKED );
		return 0; 
	}

	if(!window_getDims(WDW_WORKSHOP, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
	{
		init = 1;
		workshopReparse();
		return 0;
	}

/*
	if( !e->pl->detailInteracting )
	{
		window_setMode( WDW_WORKSHOP, WINDOW_DOCKED );
		return 0;
	}
*/

//	if (init) 
	if (window_getMode( WDW_WORKSHOP ) == WINDOW_SHRINKING)
	{
		cmdParse("emote none");
	} 
	else if (init || (s_bReparse && window_getMode(WDW_WORKSHOP) == WINDOW_DISPLAYING ))
	{
		char *last = NULL;
		RoomDetail *pdetail = baseGetDetail(&g_base, e->pl->detailInteracting, NULL);

		if (!workshopCategories)
		{
			workshopCategories = uiTabControlCreate(TabType_Undraggable, 0, 0, 0, 0, 0);
		}
		if (detailRecipes)
		{
			EArrayDestroy(&detailRecipes);
		}
		if (neededWorkshops)
		{
			EArrayDestroy(&neededWorkshops);
		}
		EArrayCreate(&neededWorkshops);

		for( i = 0; i < EArrayGetSize(&recipeDisplayStates); ++i )
		{
			recipeDisplayStates[i]->recipe = NULL; //make sure we update it
		}
	
		if (pdetail && strstri(pdetail->info->pchName,"empower"))
			s_Empowerment = 1;
		else
			s_Empowerment = 0;

		detailRecipes = entity_GetValidDetailRecipes(e, pdetail);
		EArrayQSort(detailRecipes, compareRecipe);
		uiTabControlRemoveAll(workshopCategories);	
		if (init)
		{	
			s_Hide = 0;
			comboboxClear(&hideBox);
			combocheckbox_init(&hideBox,0,0,0,200,200,CBOXHEIGHT,400,"HideTitle",WDW_WORKSHOP,1,0);
			combocheckbox_add(&hideBox,0,NULL,"HideMissingSalvage",HIDE_MISSING_SALVAGE);
			if (!s_Empowerment)
			{		
				combocheckbox_add(&hideBox,0,NULL,"HideWrongTable",HIDE_WRONG_TABLE);
				combocheckbox_add(&hideBox,0,NULL,"HideDetailRecipes",HIDE_DETAIL);
				combocheckbox_add(&hideBox,0,NULL,"HideComponentRecipes",HIDE_COMPONENT);
			}
			else
			{
				s_Hide = HIDE_WRONG_TABLE;
			}
			if (pdetail || (e->pl && e->pl->workshopInteracting))
				cmdParse("emote invent");

		}

		init = 0;
	}

	oldy = y;
	drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );
	curTab = drawTabControl(workshopCategories, x+R10*sc, y, z, wd - 2*R10*sc, TAB_HEIGHT, sc, color, color );
	set_scissor( 1 );

	scissor_dims( x + PIX3*sc, y + (PIX3+TAB_HEIGHT)*sc, wd - PIX3*2*sc, ht - (PIX3*3 + TAB_HEIGHT + CBOXHEIGHT)*sc );

	y += (BORDER_SPACE*sc + TAB_HEIGHT*sc - sb.offset);

	i = EArrayGetSize(&detailRecipes) - EArrayGetSize(&recipeDisplayStates);
	while(i>0)
	{ // Make room for more displaystates
		EArrayPush(&recipeDisplayStates, calloc(sizeof(RecipeDisplayState), 1));
		i--;
	}

	if (!curTab && EArrayGetSize(&neededWorkshops) > 0 && fakeState.text.buff && !s_Empowerment && e->pl->detailInteracting)
	{  //lets display the fake recipe
		displayRecipe(&fakeState, x + BORDER_SPACE, &y, z+1, wd - BORDER_SPACE*2, sc, color,1);
	}
	size = EArrayGetSize(&detailRecipes);
	for( i = 0; i < size; ++i )
	{
		int visible = true; //figure out if we're on screen
		recipeDisplayStates[i]->index = i;
		if (y+ht/4 < oldy || y > oldy+ht)
			visible = false;
		if (visible && curTab && strcmp(detailRecipes[i]->pchDisplayTabName,curTab) != 0)
			visible = false;
		if (window_getMode(WDW_WORKSHOP) != WINDOW_DISPLAYING )
			visible = false;
		updateRecipeDisplayState(recipeDisplayStates[i],detailRecipes[i],recipeDisplayStates[i]->expanded);
		if (window_getMode(WDW_WORKSHOP) == WINDOW_DISPLAYING &&
			(!curTab || strcmp(detailRecipes[i]->pchDisplayTabName,curTab) == 0) &&
			(!(s_Hide & HIDE_MISSING_SALVAGE) || recipeDisplayStates[i]->have_needed) &&
			(!(s_Hide & HIDE_DETAIL) || !detailRecipes[i]->pDetail) &&
			(!(s_Hide & HIDE_COMPONENT) || detailRecipes[i]->pDetail) &&
			((!(s_Hide & HIDE_WRONG_TABLE) && (recipeDisplayStates[i]->special || recipeDisplayStates[i]->recipe->Type == kRecipeType_Drop)) || recipeDisplayStates[i]->usable_here))
		{
			if (visible)
				updateRecipeIcon(recipeDisplayStates[i],detailRecipes[i],recipeDisplayStates[i]->expanded);
			displayRecipe(recipeDisplayStates[i], x + BORDER_SPACE, &y, z+1, wd - BORDER_SPACE*2, sc, color, visible);
		}
	}
	set_scissor( 0 );
	hideBox.x = wd/sc - 200;
	hideBox.y = ht/sc - CBOXHEIGHT - PIX3;
	hideBox.z = 0;
	combobox_display(&hideBox);
	if (hideBox.changed)
	{
		int newhide = 0;
		newhide |= combobox_idSelected(&hideBox,HIDE_MISSING_SALVAGE) * HIDE_MISSING_SALVAGE;
		if (!s_Empowerment)
		{		
			newhide |= combobox_idSelected(&hideBox,HIDE_WRONG_TABLE) * HIDE_WRONG_TABLE;
			newhide |= combobox_idSelected(&hideBox,HIDE_DETAIL) * HIDE_DETAIL;
			newhide |= combobox_idSelected(&hideBox,HIDE_COMPONENT) * HIDE_COMPONENT;
		}
		else
		{
			newhide |= HIDE_WRONG_TABLE;
		}
		s_Hide = newhide;		
		workshopReparse();
	}

	if (s_bReparse && window_getMode(WDW_WORKSHOP) == WINDOW_DISPLAYING )
	{ // We need to redo the tabs because visible status changed
		char tabName[50];
		char *last = NULL;
		char *lastTab = uiTabControlGetTabName(workshopCategories, uiTabControlGetSelectedIdx(workshopCategories));
		int i;
		if (lastTab)
		{		
			strncpyt(tabName,lastTab,49);
		}
		uiTabControlRemoveAll(workshopCategories);
		uiTabControlAdd(workshopCategories,"AllString",NULL);
		for (i = 0; i < EArrayGetSize(&detailRecipes);i++)
		{
			if ((!last || strcmp(detailRecipes[i]->pchDisplayTabName,last) != 0) &&
				(!(s_Hide & HIDE_MISSING_SALVAGE) || recipeDisplayStates[i]->have_needed) &&
				(!(s_Hide & HIDE_DETAIL) || !detailRecipes[i]->pDetail) &&
				(!(s_Hide & HIDE_COMPONENT) || detailRecipes[i]->pDetail) &&
				((!(s_Hide & HIDE_WRONG_TABLE) && (recipeDisplayStates[i]->special || recipeDisplayStates[i]->recipe->Type == kRecipeType_Drop)) || recipeDisplayStates[i]->usable_here))
			{
				last = detailRecipes[i]->pchDisplayTabName;
				uiTabControlAdd(workshopCategories,detailRecipes[i]->pchDisplayTabName,detailRecipes[i]->pchDisplayTabName);
			}
		}
		if (lastTab)
			uiTabControlSelectByName(workshopCategories,tabName);
		else
			uiTabControlSelectByIdx(workshopCategories,  1); // if there is no second, this function will automatically select the 'ALL' catagory

		setupFakeRecipe(&fakeState);
		s_bReparse = 0;
	}
	
 	
	doScrollBar( &sb, ht - (PIX3*2+R10*2+CBOXHEIGHT)*sc, y - oldy + sb.offset, wd, (PIX3+R10)*sc, z , 0, 0 );

	return 0;
}

/* End of File */
