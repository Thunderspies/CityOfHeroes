#include "UI/Hybrid/uiMainStoreAccess.h"
#include "UI/Hybrid/uiHybridMenu.h"
#include "UI/uiWindows.h"
#include "UI/uiUtil.h"
#include "wdwbase.h"
#include "UI/uiWebStoreFrame.h"
#include "UI/uiUtilGame.h"
#include "inventory_client.h"
#include "sprite_text.h"
#include "sprite_font.h"
#include "win_init.h"
#include "UI/uiOptions.h"
#include "dbclient.h"

#define CENTER_X 25.f
#define CENTER_Y 24.5f

static int AccountIsReallyVIP()
{
	// AccountIsVIP only checks the AccountServer. If the AccountServer is down, we can get the info from the DbServer.
	// In theory, we'd only need the info from the DbServer, except we might force the VIP debug flag on, in which case we'd still want to use it.
	return db_info.vip || AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags());
}

int mainStoreAccessWindow()
{
	F32 x, y, z, wd, ht, scale;
	int color, bcolor;
	int winWd, winHt;
	static F32 textAlpha = 0.f;

	if (AccountIsReallyVIP() && optionGet(kUO_HideStoreAccessButton)) // only VIPs may hide the in-game store button
	{
		return 0;
	}

	//always on top
	window_bringToFront(WDW_MAIN_STORE_ACCESS);
	
	// Do everything common windows are supposed to do.
	if ( !window_getDims( WDW_MAIN_STORE_ACCESS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ))
		return 0;

	//draw store icon
	drawHybridStoreWindowButton( NULL, "icon_ig_storeaccess_color_full", x+CENTER_X*scale, y+CENTER_Y*scale, z, scale, CLR_WHITE, 0, HB_SHRINK_OVER|HB_DO_NOT_EDIT_ELEMENT, "paragonStoreButton", WDW_MAIN_STORE_ACCESS);

	drawFrame( PIX3, R22, x, y, z-2.f, wd, ht, scale, color, bcolor );

	//print floating store text
	windowClientSize(&winWd, &winHt);
	if (winHt-ht-20.f*scale > y )
	{
		textAlpha = stateScaleSpeed(textAlpha, 0.3f, 1);
	}
	else
	{
		textAlpha = stateScaleSpeed(textAlpha, 0.3f, 0);
	}
	font(&hybridbold_12);
	font_color(CLR_WHITE&(0xffffff00|(int)(0xff*textAlpha)), CLR_WHITE&(0xffffff00|(int)(0xff*textAlpha)));
	cprnt(x+wd/2.f, y+ht+12.f*scale, z, scale, scale, "ShopString");
	font_color(CLR_WHITE&(0xffffff00|(int)(0xff*(1.f-textAlpha))), CLR_WHITE&(0xffffff00|(int)(0xff*(1.f-textAlpha))));
	cprnt(x+wd/2.f, y-3.f*scale, z, scale, scale, "ShopString");

	return 0;
}
