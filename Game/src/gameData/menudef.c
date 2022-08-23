/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/textparser.h>

#include "cmdparse/cmdgame.h"

#include <utilitieslib/language/AppLocale.h>
#include "UI/uiContextMenu.h"
#include "UI/uiInput.h"

#include "menudef.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "auth/authUserData.h"
#include "player/badges.h"
#include "entity/RewardToken.h"
#include "player/player.h"
#include "entity/costume_client.h"
#include "account/AccountData.h"
#include "player/inventory_client.h"

typedef enum MenuItemType
{
    kMenuItemType_Option,
    kMenuItemType_Divider,
    kMenuItemType_Title,
    kMenuItemType_Menu,

    kMenuItemType_Count,
} MenuItemType;

typedef struct MenuItem MenuItem;
typedef struct MenuItem
{
    MenuItemType eType;

    char *pchDisplayName;
    char *pchCommand;

    char **ppchBadges;
    char **ppchCostumeKeys;
    char **ppchAuthbits;
    char **ppchRewardTokens;
    char **ppchStoreProducts;

    MenuItem **ppItems;

    ContextMenu *pContext;
} MenuItem;


MenuItem g_MenuDefs;

TokenizerParseInfo ParseOptionMenuItem[] =
{
    { "Type",         TOK_INT(MenuItem, eType, kMenuItemType_Option) },
    { "DisplayName",  TOK_STRUCTPARAM|TOK_STRING(MenuItem, pchDisplayName, 0) },
    { "Command",      TOK_STRUCTPARAM|TOK_STRING(MenuItem, pchCommand, 0) },
    { "\n",           TOK_END,                      0 },
    { 0 }
};

TokenizerParseInfo ParseLockedOptionMenuItem[] =
{
    { "{",            TOK_START,                      0 },
    { "Type",         TOK_INT(MenuItem, eType, kMenuItemType_Option) },
    { "DisplayName",  TOK_STRUCTPARAM|TOK_STRING(MenuItem, pchDisplayName, 0) },
    { "Command",      TOK_STRUCTPARAM|TOK_STRING(MenuItem, pchCommand, 0) },
    { "Badge",        TOK_STRINGARRAY(MenuItem, ppchBadges) },
    { "CostumeKey",   TOK_STRINGARRAY(MenuItem, ppchCostumeKeys) },
    { "Authbit",      TOK_STRINGARRAY(MenuItem, ppchAuthbits) },
    { "RewardToken",  TOK_STRINGARRAY(MenuItem, ppchRewardTokens) },
    { "StoreProduct", TOK_STRINGARRAY(MenuItem, ppchStoreProducts) },
    { "}",            TOK_END,                      0 },
    { 0 }
};

TokenizerParseInfo ParseDividerMenuItem[] =
{
    { "Type",         TOK_INT(MenuItem, eType, kMenuItemType_Divider) },
    { "\n",           TOK_END,                      0 },
    { 0 }
};

TokenizerParseInfo ParseTitleMenuItem[] =
{
    { "Type",         TOK_INT(MenuItem, eType, kMenuItemType_Title) },
    { "DisplayName",  TOK_STRUCTPARAM|TOK_STRING(MenuItem, pchDisplayName, 0) },
    { "\n",           TOK_END,                      0 },
    { 0 }
};

TokenizerParseInfo ParseMenuMenuItem[] =
{
    { "{",           TOK_START,                      0 },
    { "Type",        TOK_INT(MenuItem, eType, kMenuItemType_Menu) },
    { "DisplayName", TOK_STRUCTPARAM|TOK_STRING(MenuItem, pchDisplayName, 0)            },
    { "Command",     TOK_STRING(MenuItem, pchCommand, 0)                },
    { "Title",       TOK_REDUNDANTNAME|TOK_STRUCT(MenuItem, ppItems, ParseTitleMenuItem)   },
    { "Divider",     TOK_REDUNDANTNAME|TOK_STRUCT(MenuItem, ppItems, ParseDividerMenuItem) },
    { "Option",      TOK_REDUNDANTNAME|TOK_STRUCT(MenuItem, ppItems, ParseOptionMenuItem)  },
    { "LockedOption",TOK_STRUCT(MenuItem, ppItems, ParseLockedOptionMenuItem)    },
    { "Menu",        TOK_STRUCT(MenuItem, ppItems, ParseMenuMenuItem)    },
    { "}",           TOK_END,                        0 },
    { 0 }
};

/**********************************************************************func*
 * load_MenuDefs
 *
 */
void load_MenuDefs(void)
{
    char pchPath[MAX_PATH];

    strcpy(pchPath, "/texts/");
    strcat(pchPath, locGetName(locGetIDInRegistry()));
    strcat(pchPath, "/menus/");

    ParserLoadFiles(pchPath, ".mnu", 0, 0, ParseMenuMenuItem, &g_MenuDefs, NULL, NULL, NULL);
}

/**********************************************************************func*
 * menu_GetDef
 *
 */
static MenuItem *menu_GetDef(char *pchName, MenuItem *pMenuBase)
{
    int i;
    for(i=eaSize(&pMenuBase->ppItems)-1; i>=0; i--)
    {
        if(pMenuBase->ppItems[i]->eType==kMenuItemType_Menu
            && stricmp(pMenuBase->ppItems[i]->pchDisplayName, pchName)==0)
        {
            return pMenuBase->ppItems[i];
        }
    }

    return NULL;
}

/**********************************************************************func*
 * DoMenuCommand
 *
 */
static void DoMenuCommand(void *pv)
{
    char *pchCmd = (char *)pv;
    cmdParse(pchCmd);
}

static int checkMenuLocks(void *data)
{
    int locked = 1;
    int hidden = 1;
    MenuItem *pItem = data;
    int count;

    // Entries with nothing to check are always unlocked.
    if(pItem && (pItem->ppchAuthbits || pItem->ppchBadges || pItem->ppchCostumeKeys || pItem->ppchStoreProducts || pItem->ppchRewardTokens))
    {
        for(count=0; locked && count<eaSize(&pItem->ppchAuthbits); count++)
        {
            if(authUserGetFieldByName((U32*)(playerPtr()->pl->auth_user_data), pItem->ppchAuthbits[count]))
                locked = 0;
        }

        for(count=0; locked && count<eaSize(&pItem->ppchBadges); count++)
        {
            if(entity_OwnsBadge(playerPtr(), pItem->ppchBadges[count]))
                locked = 0;
        }

        for(count=0; locked && count<eaSize(&pItem->ppchCostumeKeys); count++)
        {
            if(costumereward_find(pItem->ppchCostumeKeys[count], 0))
                locked = 0;
        }

        for(count=0; locked && count<eaSize(&pItem->ppchRewardTokens); count++)
        {
            if(getRewardToken(playerPtr(), pItem->ppchRewardTokens[count]))
                locked = 0;
        }

        for(count=0; locked && count<eaSize(&pItem->ppchStoreProducts); count++)
        {
            if (!AccountHasStoreProductOrIsPublished(inventoryClient_GetAcctInventorySet(), skuIdFromString(pItem->ppchStoreProducts[count])))
                return CM_HIDE;

            if (AccountHasStoreProduct(ent_GetProductInventory( playerPtr()), skuIdFromString(pItem->ppchStoreProducts[count])))
                locked = 0;
        }
    }
    else
    {
        locked = 0;
    }

    return locked ? CM_VISIBLE : CM_AVAILABLE;
}

/**********************************************************************func*
 * menu_CreateContextMenu
 *
 */
static ContextMenu *menu_CreateContextMenu(MenuItem *pMenu)
{
    ContextMenu *pContext = NULL;

    if(pMenu!=NULL)
    {
        if(pMenu->pContext!=NULL)
        {
            pContext=pMenu->pContext;
        }
        else
        {
            int i;
            int iSize = eaSize(&pMenu->ppItems);
            pContext = contextMenu_Create(NULL);

            for(i=0; i<iSize; i++)
            {
                switch(pMenu->ppItems[i]->eType)
                {
                    case kMenuItemType_Option:
                        contextMenu_addCode(pContext,
                            checkMenuLocks, pMenu->ppItems[i],
                            DoMenuCommand, pMenu->ppItems[i]->pchCommand,
                            pMenu->ppItems[i]->pchDisplayName,
                            NULL);
                        break;

                    case kMenuItemType_Menu:
                        contextMenu_addCode(pContext,
                            alwaysAvailable, NULL,
                            NULL, NULL,
                            pMenu->ppItems[i]->pchDisplayName,
                            menu_CreateContextMenu(pMenu->ppItems[i]));
                        break;

                    case kMenuItemType_Divider:
                        contextMenu_addDivider(pContext);
                        break;

                    case kMenuItemType_Title:
                        contextMenu_addTitle(pContext, pMenu->ppItems[i]->pchDisplayName);
                        break;
                }
            }
        }
    }

    return pContext;
}

/**********************************************************************func*
 * menu_CreateContextMenuByName
 *
 */
static ContextMenu *menu_CreateContextMenuByName(char *pchName)
{
    MenuItem *pMenu = menu_GetDef(pchName, &g_MenuDefs);

    return menu_CreateContextMenu(pMenu);
}

/**********************************************************************func*
 * menu_Show
 *
 */
void menu_Show(char *pchName, int x, int y)
{
    ContextMenu *pMenu;

    if(x<0) x=gMouseInpCur.x;
    if(y<0) y=gMouseInpCur.y;

    pMenu = menu_CreateContextMenuByName(pchName);
    if(pMenu)
    {
        contextMenu_set(pMenu, x, y, NULL);
    }
}

/* End of File */
