/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/

#include "win/input.h"
#include "player/player.h"

#include "cmdparse/cmdgame.h"
#include "win/win_init.h"
#include "comm_game.h"
#include "clientcomm/clientcomm.h"

#include "entity/character_base.h"
#include "arena/arenagame.h"

#include "UI/uiPet.h"
#include "UI/uiNet.h"
#include "UI/uiGame.h"
#include "UI/uiDock.h"
#include "UI/uiChat.h"
#include "UI/uiUtil.h"
#include "UI/uiArena.h"
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "UI/Hybrid/uiRegister.h"
#include "UI/uiUtilGame.h"
#include "UI/uiEditText.h"
#include "UI/uiContactDialog.h"
#include "uiBaseStorage.h"
#include "UI/uiCustomWindow.h"
#include "UI/uiCompass.h"
#include "UI/Hybrid/uiHybridMenu.h"

#include "winuser.h"
#include "UI/uiWindows.h"
#include "UI/uiWindows_init.h"
#include "UI/uiDialog.h"

#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/font.h"
#include "win/input.h"
#include "UI/uiStatus.h"
#include "entity/character_base.h"
#include "UI/uiEmail.h"
#include "uiChatOptions.h"
#include "entity/chatSettings.h"
#include "UI/uiOptions.h"
#include "graphics/textureatlas.h"
#include "entity/entity.h"
#include <utilitieslib/utils/mathutil.h>
#include "bases/baseedit.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/timing.h>
#include "edit/edit_cmd.h"
#include "entity/entPlayer.h"
#include "UI/uiTraySingle.h"

#include <utilitieslib/language/MessageStoreUtil.h>
#include "graphics/truetype/ttFontDraw.h"
#include "uiLevelingpact.h"
#include "UI/Hybrid/uiWebStoreFrame.h"
#include "player/inventory_client.h"
#include "graphics/gfxLoadScreens.h"


#define WINDOW_SHRINK_TIME  ( 5 )
#define WINDOW_GROW_TIME    ( 5 )
#define JELLY_FADE_SPEED    ( .05 )
#define JELLY_MIN_OPACITY   ( .3f )
#define JELLY_MOUSE_OPACITY ( .5 )
#define JELLY_MOVE_OPACITY  ( 1.0 )
#define CHILD_BUTTON_SPACE  ( 7 )
#define CHILD_OPACITY       ( .5 )
#define MIN_OPACITY         ( 77 )

#define MIN_SCALE            ( .65f )
#define MIN_RES_MAX_SCALE    (  1.f )
#define MAX_RES_MAX_SCALE    ( 2.4f )

#define BASE_WINDOW_Z   ( 10 )
#define Z_SEPERATION    ( 100 )
#define ZCALC(X)        BASE_WINDOW_Z + winDefs[X].window_depth * Z_SEPERATION

int gWindowDragging = 0;

// debug only, to temporarily avoid having to redefine all window handlers (ie chatWindow()) to pass in window ID
int gCurrentWindefIndex = 0;

int custom_window_count = 0;
//--------------------------------------------------------------------------------------------
// Child Windows /////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------
//
//

static void window_alignChildren( int i )
{
    int j, w, h;
      Wdw * parent = &winDefs[i];

       windowClientSizeThisFrame(&w, &h);

    for (j = 0; j < parent->num_children; j++)
    {
        Wdw *child = parent->children[j].wdw;
        ChildWindow *cWin = &parent->children[j];
        int parentLeftX, parentTopY;
        float childLeftX, childTopY;

        if (!child || !child->loc.locked || child->loc.mode == WINDOW_DOCKED)
            continue;

        window_getUpperLeftPos(i, &parentLeftX, &parentTopY);

        // make sure that child's appearance matches parent
        child->loc.sc            = parent->loc.sc;
         child->loc.back_color    = parent->loc.back_color;
        child->loc.color        = parent->loc.color;
        child->opacity            = parent->opacity;
        child->min_opacity        = parent->min_opacity;

        // center children that are bigger than parent, align others
        if (child->loc.wd > parent->loc.wd)
        {
            childLeftX = parentLeftX + (parent->loc.wd - child->loc.wd)*parent->loc.sc/2;

             if (childLeftX < 0)
                childLeftX = 0;
            else if (childLeftX + child->loc.wd*child->loc.sc > w)
                childLeftX = w - child->loc.wd*child->loc.sc;                

            if (ABS(childLeftX-parentLeftX) < 44*child->loc.sc)
                childLeftX = parentLeftX;
            else if (ABS(childLeftX - (parentLeftX + (parent->loc.wd - child->loc.wd)*parent->loc.sc)) < 44*child->loc.sc)
                childLeftX = parentLeftX + (parent->loc.wd - child->loc.wd)*parent->loc.sc;
        }
        else
        {
             if( parent->left )
                childLeftX = parentLeftX;
            else
                childLeftX = parentLeftX + (parent->loc.wd - child->loc.wd)*parent->loc.sc;
        }

        if( (!parent->below && !parent->flip) || (parent->below && parent->flip) )
        {
            childTopY = parentTopY - (JELLY_HT + child->loc.ht)*parent->loc.sc;

            if( child->loc.draggable_frame && childTopY < 0 )
                child->loc.ht = MAX( childTopY + child->loc.ht, child->min_ht );
        }
        else
        {
            childTopY = parentTopY + (parent->loc.ht + JELLY_HT)*parent->loc.sc;

            if( child->loc.draggable_frame && childTopY + child->loc.ht > h)
                child->loc.ht = MAX( child->loc.ht - (h-childTopY+child->loc.ht), child->min_ht );
        }
        
        window_setUpperLeftPos(child->id, childLeftX, childTopY);
    }
}

static int window_getMainPosition( Wdw * w, float *xp, float *yp );

int childButton_getWidth(float scale, ChildWindow *cwdw)
{
    if (cwdw->cached.scale != scale)
    {
        if( scale < .82f )
            cwdw->cached.width = str_wd( &game_12_dynamic, scale, scale, cwdw->name );
        else
            cwdw->cached.width = str_wd( &game_12, scale, scale, cwdw->name );
        cwdw->cached.scale = scale;
    }
    return cwdw->cached.width;
}

//
//
void childButton_getCenter( Wdw *wdw, int child_id,  float *x, float *y )
{
    int i, color, bcolor, w, h;
    float sx, tx, ty, tz, wd, ht, scale;
     Entity *e = playerPtr();

     windowClientSize(&w, &h);
 
       if (!window_getDims( wdw->id, &sx, &ty, &tz, &wd, &ht, &scale, &color, &bcolor ))
    {
        window_getMainPosition( wdw, x, y );
        return;
    }

    if(!customWindowIsOpen(wdw->id))
    {
        *x = sx + wd/2;
        *y = ty + ht/2;
        return;
    }

     tx = sx+(min(13, wdw->radius) + CHILD_BUTTON_SPACE)*scale;

     for( i = 0; i < child_id; i++ )
        tx += childButton_getWidth(scale, &wdw->children[i])  + CHILD_BUTTON_SPACE*scale;

    tx += childButton_getWidth(scale, &wdw->children[child_id])/2;

      if( wdw->parent ) // parent AND child! oy vey!
        tx += 20*wdw->loc.sc;

    if( wdw->id == WDW_STAT_BARS )
    {
        ty += 12*wdw->loc.sc;
        tx += 10*wdw->loc.sc;
    }
    else if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) ) 
        ty -= JELLY_HT*scale/2;
    else
        ty += ht + JELLY_HT*scale/2;

     *x = tx; 
    *y = ty;
}

//--------------------------------------------------------------------------------------------
// change window attributes //////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------

int gWindowRGBA[4];

void window_SetColor( int wdw, int color )
{
    winDefs[wdw].loc.color = color;
}

int window_GetColor( int wdw )
{
    if (winDefs[wdw].forceColor || windowIsSaved(wdw))
        return winDefs[wdw].loc.color;
    else
        return winDefs[WDW_STAT_BARS].loc.color;
}

float window_GetOpacity( int wdw )
{
    if( winDefs[wdw].loc.maximized )
        return 1.0;
    return MAX(winDefs[wdw].opacity, winDefs[wdw].min_opacity);
}

void window_SetTitle(int wdw, const char *title)
{
    if (title && title[0])
    {
        strncpy(winDefs[wdw].title, title, 49);
        winDefs[wdw].useTitle = 1;
    }
    else
    {
        winDefs[wdw].useTitle = 0;
    }
}

void window_EnableCloseButton(int wdw, int enableClose)
{
    winDefs[wdw].noCloseButton = !enableClose;
}

void window_SetFlippage(int wdw, int flip)
{
    winDefs[wdw].forceNoFlip = !flip;
}

void windows_ChangeColor(void)
{
    int i, clr;
    int clrBack;

    clr = (gWindowRGBA[0] << 24) + (gWindowRGBA[1] << 16) + (gWindowRGBA[2] << 8) + (255);

    // TODO: Allow a color for the background?
    clrBack = (0 << 24) + (0 << 16) + (0 << 8) + gWindowRGBA[3];

    for( i = 0; i < MAX_WINDOW_COUNT+custom_window_count; i++ )
    {
        winDefs[i].loc.color = clr;
        winDefs[i].loc.back_color = clrBack;
        window_UpdateServer( i );
    }
}

typedef struct WindowMap
{
    char *pchWindowName;
    WindowName wdw;
    int openable;
    int closable;
    int scalable;
} WindowMap;

// This is the list of windows the user is able to open and close via slash
// commands or keybinds.

// This should probably be integrated into the big table in uiWindow_init so people can't forget to add it here
static WindowMap s_aUserWindows[] =
{
    { "status",         WDW_STAT_BARS, 1, 0, 1        },
    { "health",         WDW_STAT_BARS, 1, 0, 1        },
    { "target",         WDW_TARGET, 1, 1, 1            },
    { "tray",           WDW_TRAY, 1, 1, 1              },
    { "power",          WDW_TRAY, 1, 1, 1              },
    { "powers",         WDW_TRAY, 1, 1, 1              },
    { "chat",           WDW_CHAT_BOX, 1, 1, 1          },
    { "chat0",            WDW_CHAT_BOX, 1, 1, 1          },
    { "chat1",            WDW_CHAT_CHILD_1, 1, 1       },
    { "chat2",            WDW_CHAT_CHILD_2, 1, 1       },
    { "chat3",            WDW_CHAT_CHILD_3, 1, 1       },
    { "chat4",            WDW_CHAT_CHILD_4, 1, 1       },
    { "powerlist",      WDW_POWERLIST, 1, 1, 1         },
    { "group",          WDW_GROUP, 1, 1, 1             },
    { "team",           WDW_GROUP, 1, 1, 1             },
    { "compass",        WDW_COMPASS, 1, 1, 1           },
    { "nav",            WDW_COMPASS, 1, 1, 1           },
    { "map",            WDW_MAP, 1, 1, 1               },
    { "friend",         WDW_FRIENDS, 1, 1, 1           },
    { "friends",        WDW_FRIENDS, 1, 1, 1           },
    { "inspiration",    WDW_INSPIRATION, 1, 1, 1       },
    { "inspirations",   WDW_INSPIRATION, 1, 1, 1       },
    { "insp",           WDW_INSPIRATION, 1, 1, 1       },
    { "insps",          WDW_INSPIRATION, 1, 1, 1       },
    { "supergroup",     WDW_SUPERGROUP, 1, 1, 1        },
    { "super",          WDW_SUPERGROUP, 1, 1, 1        },
    { "sg",             WDW_SUPERGROUP, 1, 1, 1        },
    { "email",          WDW_EMAIL, 1, 1, 1             },
    { "compose",        WDW_EMAIL_COMPOSE, 1, 1, 1     },
    { "contact",        WDW_CONTACT, 1, 1, 1           },
    { "contacts",       WDW_CONTACT, 1, 1, 1           },
    { "mission",        WDW_MISSION, 1, 1, 1           },
    { "clue",           WDW_CLUE, 1, 1, 1              },
    { "clues",          WDW_CLUE, 1, 1, 1              },
    { "info",           WDW_INFO, 0, 1, 1              },
    { "help",           WDW_HELP, 1, 1, 0              },
    { "actions",        WDW_TARGET_OPTIONS, 1, 1, 1    },
    { "enhancements",   WDW_ENHANCEMENT, 1, 1, 1       },
    { "costume",        WDW_COSTUME_SELECT, 1, 1, 1    },
    { "search",            WDW_LFG, 1, 1, 0              },
    { "badge",            WDW_BADGES, 1, 1, 1           },
    { "salvage",        WDW_SALVAGE, 1, 1, 1           },
    { "recipes",        WDW_RECIPEINVENTORY, 1, 1, 1},
    { "recipe",            WDW_RECIPEINVENTORY, 1, 1, 1},
    { "options",        WDW_OPTIONS, 1, 1, 1         },
    { "pet",            WDW_PET, 1, 1, 1             },
    { "chansearch",        WDW_CHANNEL_SEARCH, 1, 1, 1 },
    { "combatnumbers",    WDW_COMBATNUMBERS, 1, 1, 1     },
    { "contactdialog",    WDW_CONTACT_DIALOG, 0, 1, 1    },
    { "quit",            WDW_QUIT, 1, 1, 1            },
    { "missionsummary",    WDW_MISSION_SUMMARY, 0, 1, 0},
    { "store",            WDW_STORE, 0, 1, 0            },
    { "petition",        WDW_PETITION, 1, 1, 0        },
    { "defeat",            WDW_DEATH,    0, 0, 1            },
    { "costumeselect",    WDW_COSTUME_SELECT, 1, 1, 1    },
    { "combatmonitor",    WDW_COMBATMONITOR, 0, 0, 1    },
    { "auction",        WDW_AUCTION, 0, 1, 1    },
    { "tray1",            WDW_TRAY_1, 1, 1, 1            },
    { "tray2",            WDW_TRAY_2, 1, 1, 1            },
    { "tray3",            WDW_TRAY_3, 1, 1, 1            },
    { "tray4",            WDW_TRAY_4, 1, 1, 1            },
    { "tray5",            WDW_TRAY_5, 1, 1, 1            },
    { "tray6",            WDW_TRAY_6, 1, 1, 1            },
    { "tray7",            WDW_TRAY_7, 1, 1, 1            },
    { "tray8",            WDW_TRAY_8, 1, 1, 1            },
    { "playernote",        WDW_PLAYERNOTE, 1, 1, 1        },
    { "mm",                WDW_MISSIONMAKER, 1, 1, 1   }, // TODO: Remove before release
    { "missionreview",    WDW_MISSIONREVIEW, 1, 1, 1  }, 
    { "badgemonitor",    WDW_BADGEMONITOR, 0, 0, 1    },
    { "cvg",            WDW_CUSTOMVILLAINGROUP, 0, 1, 1 },
    { "incarnate",        WDW_INCARNATE, 1, 1, 0    },
    { "pophelp",        WDW_POP_HELP, 1, 0, 0    },
    { "pophelptext",    WDW_POP_HELP_TEXT, 1, 1, 1    },
    { "scriptui",        WDW_SCRIPT_UI, 1, 1, 1        },
//    { "karma",            WDW_KARMA_UI, 1, 1, 1        },
    { "lfg",            WDW_TURNSTILE, 1, 1, 1,        },
    { "lfgdialog",        WDW_TURNSTILE_DIALOG, 0, 0, 1 },
    { "razertray",        WDW_TRAY_RAZER, 0, 0, 0        },
    { "league",            WDW_LEAGUE,    1, 1, 1            },
    { "contactfinder",    WDW_CONTACT_FINDER, 1, 1, 0    },
    { "paragonrewards",    WDW_LOYALTY_TREE, 1, 1, 0    },
    { "loyaltytreeaccess",WDW_LOYALTY_TREE_ACCESS, 1, 0, 0    },
    { "lwcui",            WDW_LWC_UI, 0, 0 ,0 },
    { "salvageopen",    WDW_SALVAGE_OPEN, 0, 0 ,0 },
    { "convertenhancement", WDW_CONVERT_ENHANCEMENT, 0, 1, 1 },
    { "newfeatures",    WDW_NEW_FEATURES, 1, 1, 0},
    { NULL,  0 },
};

// This list of windows is used by the PERFINFO_AUTO_START/STOP timers.
static WindowMap s_aNonUserWindows[] =
{
    { "dock",                WDW_DOCK, },
    { "chatOptions",        WDW_CHAT_OPTIONS, },
    { "contactDialog",        WDW_CONTACT_DIALOG, },
    { "trade",                WDW_TRADE, },
    { "quit",                WDW_QUIT, },
    { "missionSummary",        WDW_MISSION_SUMMARY, },
    { "browser",            WDW_BROWSER, },
    { "store",                WDW_STORE, },
    { "dialog",                WDW_DIALOG, },
    { "betaComment",        WDW_BETA_COMMENT, },
    { "petition",            WDW_PETITION, },
    { "titleSelect",        WDW_TITLE_SELECT, },
    { "death",                WDW_DEATH, },
    { "mapSelect",            WDW_MAP_SELECT, },
    { "rewardChoice",        WDW_REWARD_CHOICE, },
    { "deprecated",            WDW_DEPRECATED_1, },
    { "arenaCreate",           WDW_ARENA_CREATE, },
    { "arenaOptions",        WDW_ARENA_OPTIONS, },
    { "arenaList",            WDW_ARENA_LIST, },
    { "arenaGladiator",        WDW_ARENA_GLADIATOR_PICKER, },
    { "arenaResult",        WDW_ARENA_RESULT, },
    { "arenaJoin",            WDW_ARENA_JOIN, },
    { "renderStats",        WDW_RENDER_STATS, },
    { "inventory",            WDW_INVENTORY, },
    { "conceptinv",            WDW_CONCEPTINV, },
    { "recipeinv",            WDW_RECIPEINV, },
    { "invent",                WDW_INVENT, },
    { "baseprops",            WDW_BASE_PROPS,     },
    { "baseinv",            WDW_BASE_INVENTORY, },
    { "baseroom",            WDW_BASE_ROOM,      },
    { "base_storage",        WDW_BASE_STORAGE,  },
    { "storedsalvage",        WDW_STOREDSALVAGE,  },
    
    { NULL,  0 },
};

Wdw *windows_Find(char *pch, bool bAllowNumbers, int reason)
{
    WindowMap *pwin = s_aUserWindows;
    int i;

    while(pwin->pchWindowName!=NULL && pwin->pchWindowName[0]!='\0')
    {
        if( !reason ||
            (reason == WDW_FIND_OPEN && pwin->openable) ||
            (reason == WDW_FIND_CLOSE && pwin->closable) ||
            (reason == WDW_FIND_SCALE && pwin->scalable) )
        {
            if(stricmp(pch, pwin->pchWindowName)==0)
            {
                return wdwGetWindow(pwin->wdw);
            }
        }
        pwin++;
    }

    if(bAllowNumbers)
    {
        // Haven't found it yet, was it a number?
        i = atoi(pch);
        if(i>0)
        {
            return wdwGetWindow(i);
        }
    }

    return NULL;
}

const char* windows_GetNameByNumber(int num)
{
    WindowMap *pwin;

    for(pwin = s_aUserWindows; pwin->pchWindowName && pwin->pchWindowName[0]; pwin++)
    {
        if(pwin->wdw == num)
        {
            return pwin->pchWindowName;
        }
    }
    
    for(pwin = s_aNonUserWindows; pwin->pchWindowName && pwin->pchWindowName[0]; pwin++)
    {
        if(pwin->wdw == num)
        {
            return pwin->pchWindowName;
        }
    }

    return NULL;
}

void windows_Show(char *pch)
{
    Wdw *pwdw = windows_Find(pch, false, WDW_FIND_OPEN);
    if(pwdw)
    {
        window_setMode(pwdw->id, WINDOW_GROWING);
    }
}

void windows_ShowDbg(char *pch)
{
    Wdw *pwdw = windows_Find(pch, true, WDW_FIND_ANY);
    if(pwdw)
    {
        window_setMode(pwdw->id, WINDOW_GROWING);
    }
}

void windows_Hide(char *pch)
{
    Wdw *pwdw = windows_Find(pch, true, WDW_FIND_CLOSE);
    if(pwdw)
    {
        window_setMode(pwdw->id, WINDOW_SHRINKING);
    }
}

void windows_Toggle(char *pch)
{
    Wdw *pwdw = windows_Find(pch, false, WDW_FIND_ANY);
    if(pwdw)
    {
        if(windowUp(pwdw->id))
            pwdw =  windows_Find(pch, false, WDW_FIND_CLOSE);
        else
            pwdw =  windows_Find(pch, false, WDW_FIND_OPEN);

        if(pwdw)
            window_openClose(pwdw->id);
    }
}

void window_Names(void)
{
    WindowMap *pwin=s_aUserWindows;
    while(pwin->pchWindowName!=NULL && pwin->pchWindowName[0]!='\0')
    {
        if( pwin->openable && pwin->closable )
            //addSystemChatMsg( pwin->pchWindowName, INFO_USER_ERROR, 0 );
            addSystemChatMsg( textStd("WindowOpenCloseScaleable", pwin->pchWindowName), INFO_SVR_COM, 0 );    
        else if( pwin->closable )
            //addSystemChatMsg( pwin->pchWindowName, INFO_USER_ERROR, 0 );
            addSystemChatMsg( textStd("WindowCloseScaleable", pwin->pchWindowName), INFO_SVR_COM, 0 );    
        else
            //addSystemChatMsg( pwin->pchWindowName, INFO_USER_ERROR, 0 );
            addSystemChatMsg( textStd("WindowScaleable", pwin->pchWindowName), INFO_SVR_COM, 0 );    
        pwin++;
    }
        

}

//
//
int windowUp( int wdw )
{
    return (winDefs[wdw].loc.mode == WINDOW_DISPLAYING || winDefs[wdw].loc.mode == WINDOW_GROWING);
}

//
//
int window_getMode( int wdw )
{
    return winDefs[wdw].loc.mode;
}

void window_openClose( int w )
{
    Wdw * wdw = &winDefs[w];

    if( wdw->loc.mode == WINDOW_DOCKED || wdw->loc.mode == WINDOW_SHRINKING )
        window_setMode( w, WINDOW_GROWING );
    else
        window_setMode( w, WINDOW_SHRINKING );

}

//
//
void window_setMode( int w, int mode )
{
    int i;

    Wdw * wdw = &winDefs[w];

    if( wdw->loc.mode != mode)
    {
        if( wdw->loc.mode == WINDOW_DOCKED )
        {
            if( mode == WINDOW_SHRINKING )
                return;
            else
                wdw->timer = 0.f;
        }
        else if ( wdw->loc.mode == WINDOW_DISPLAYING )
        {
            if( mode == WINDOW_GROWING )
            {
                return;
            }
            else
                wdw->timer = 0.f;
        }
        else // either shrinking or growing
        {
            wdw->timer = WINDOW_GROW_TIME - wdw->timer;
        }

        if( wdw->parent && mode == WINDOW_GROWING )
        {
            for( i = 0; i < wdw->parent->num_children; i++ )
            {
                Wdw * child = wdw->parent->children[i].wdw;
                if(child && child->loc.locked )
                {
                    if( wdw->loc.locked && wdw != wdw->parent->children[i].wdw &&
                        windowUp(wdw->parent->children[i].wdw->id))
                    {
                        window_setMode( wdw->parent->children[i].wdw->id, WINDOW_SHRINKING );
                    }
                }
            }

            if( wdw->loc.locked && !windowUp( wdw->parent->id ) )
                wdw->parent->loc.mode = WINDOW_GROWING;
        }

        for( i = 0; i < wdw->num_children; i++ )
        {
            if( wdw->children[i].wdw && wdw->children[i].wdw->loc.locked )
            {
                if( (mode == WINDOW_GROWING && wdw->open_child && wdw->child_id == i) || mode == WINDOW_SHRINKING )
                    window_setMode( wdw->children[i].wdw->id, mode );
            }
        }

        wdw->loc.mode = mode;
        if( mode == WINDOW_DISPLAYING )
            window_bringToFront(w);
        if(!shell_menu())
            window_UpdateServer( w );
    }
}

#define SNAP_WD 10
//
//
static void window_putInBounds( int i )
{
    int winWd, winHt;
    static int recurse_prevention = 0;
    Wdw * wdw = &winDefs[i];
    WdwBase *loc = &wdw->loc;
    int winXp, winYp;

    window_getUpperLeftPos(i, &winXp, &winYp);

    // set bounding box
     if( !wdw->loc.maximized )
        BuildCBox( &wdw->cbox, winXp, winYp, loc->wd*loc->sc, loc->ht*loc->sc );

    // make sure window is in client window bounds
    if (isMenu(MENU_LOGIN) && !loadingScreenVisible()) //HACK so the settings menu in the login screen keeps in bounds properly
    {
        winWd = DEFAULT_SCRN_WD;
        winHt = DEFAULT_SCRN_HT;
    }
    else
    {
         windowClientSizeThisFrame( &winWd, &winHt );
    }

    if(!customWindowIsOpen(i))
    {
        F32 x ,y,wd;
        ChildWindow *cWin =  &wdw->children[0];

        if(!recurse_prevention)
        {
            wd = str_wd( &game_12, wdw->loc.sc, wdw->loc.sc, cWin->name );
            recurse_prevention = 1;
            childButton_getCenter( wdw, 0, &x, &y );
            recurse_prevention = 0;

             BuildCBox( &wdw->cbox, x - ( wd + CHILD_BUTTON_SPACE*wdw->loc.sc)/2, y - JELLY_HT*wdw->loc.sc/2, wd + CHILD_BUTTON_SPACE*wdw->loc.sc, JELLY_HT*wdw->loc.sc );
            //memcpy( &wdw->loc_custom, &wdw->loc, sizeof(WdwBase) ); 
            wdw->loc_minimized.wd = wd;
            wdw->loc_minimized.ht = JELLY_HT*wdw->loc.sc;
            loc = &wdw->loc_minimized;
        }
        else
            return;
    }

       if ( winWd > 0 && winHt > 0 && loc->mode == WINDOW_DISPLAYING )
    {
        if( i >=WDW_TRAY_1 && i <= WDW_TRAY_8 && (loc->wd == 50 || loc->ht == 50) ) 
        {
            // special bound hack to allow trays out of bounds for bendy trays
             if(loc->wd == 50 ) // vertical tray
            {
                if( wdw->cbox.lx < SNAP_WD )
                    window_setUpperLeftPos(i, 0, -1);
                if( wdw->cbox.hx > winWd-SNAP_WD )
                    window_setUpperLeftPos(i, winWd - loc->wd*loc->sc, -1);
            }
            else // horizontal
            {
                if( winXp + loc->wd < 50*loc->sc  )
                    window_setUpperLeftPos(i, 50*loc->sc - loc->wd, -1);
                if( winXp > winWd-50*loc->sc )
                    window_setUpperLeftPos(i, winWd-50*loc->sc, -1);

                if( wdw->cbox.ly < SNAP_WD )
                    window_setUpperLeftPos(i, -1, 0);
                if( wdw->cbox.hy > winHt-SNAP_WD )
                    window_setUpperLeftPos(i, -1, winHt - loc->ht*loc->sc);
            }
        }
        else
        {
             if ( i == WDW_MAP )
            {
                if( wdw->cbox.lx < SNAP_WD )
                    window_setUpperLeftPos(i, 0, -1);
                if( wdw->cbox.hx + 5*loc->sc > winWd-SNAP_WD )
                    window_setUpperLeftPos(i, winWd - winDefs[i].loc.wd*loc->sc - 5*loc->sc, -1);
            }
            else
            {
                if( wdw->cbox.lx < SNAP_WD )
                    window_setUpperLeftPos(i, 0, -1);
                else if( i != WDW_DOCK && wdw->cbox.hx > winWd-SNAP_WD )
                    window_setUpperLeftPos(i, winWd - loc->wd*loc->sc, -1);
            }

            if (wdw->forceNoFlip && winYp <= JELLY_HT*loc->sc)
                window_setUpperLeftPos(i, -1, JELLY_HT*loc->sc);
            else if( wdw->cbox.hy > winHt-SNAP_WD )
                window_setUpperLeftPos(i, -1, winHt - loc->ht*loc->sc);
            else if( wdw->cbox.ly < SNAP_WD )
                window_setUpperLeftPos(i, -1, 0);


            if( i == WDW_BASE_INVENTORY )
            {
                if( wdw->cbox.hy > winHt-SNAP_WD )
                     window_setUpperLeftPos(i, -1, winHt - loc->ht*loc->sc-5*loc->sc);
            }

            // collapse if the window is larger than the screen
            if( loc->draggable_frame )
            {
                // This makes sure the frame is visible on both sides, so user can resize window if needed
                 if( loc->draggable_frame != VERTONLY && wdw->cbox.hx - wdw->cbox.lx > winWd )
                {
                    loc->wd = MAX( wdw->min_wd, winWd/loc->sc ); 
                    window_UpdateServer(wdw->id);
                }

                if( loc->draggable_frame != HORZONLY && wdw->cbox.hy - wdw->cbox.ly > winHt )
                {
                    window_setUpperLeftPos(i, -1, 0);
                    loc->ht = MAX( wdw->min_ht, winHt/loc->sc );
                    window_UpdateServer(wdw->id);
                }
            }
            else if( wdw->cbox.hy - wdw->cbox.ly > winHt )
            {
                // if window is not resizeble, force visibility of drag bar
                window_setUpperLeftPos(i, -1, JELLY_HT*loc->sc);
                window_UpdateServer(wdw->id);
            }
        }
    }

     window_alignChildren( i );
}

static int window_getMainPosition( Wdw * w, float *xp, float *yp )
{
     if( w->id == WDW_CHAT_BOX )
    {
        childButton_getCenter( wdwGetWindow(WDW_STAT_BARS), 0, xp, yp );
        return 1;
    }
    else if( w->id == WDW_TRAY )
    {
        childButton_getCenter( wdwGetWindow(WDW_STAT_BARS), 1, xp, yp );
        return 1;
    }
    else if( w->id == WDW_TARGET )
    {
        childButton_getCenter( wdwGetWindow(WDW_STAT_BARS), 2, xp, yp );
        return 1;
    }
    else if( w->id == WDW_COMPASS )
    {
        childButton_getCenter( wdwGetWindow(WDW_STAT_BARS), 3, xp, yp );
        return 1;
    }

    *xp = *yp = 0;

    return 0; 
}

//
//
static void window_getDockPosn( int w, float *xp, float *yp )
{
    Wdw * wdw = &winDefs[w];

       if( wdw->parent )
    {
        int i;
        for( i = 0; i < wdw->parent->num_children; i++ )
        {
            if( wdw->parent->children[i].wdw == wdw )
                childButton_getCenter( wdw->parent, i, xp, yp );
        }
    }
    else if( !window_getMainPosition( wdw, xp, yp ))
    {
        // fall back to center of screen
        int wd, ht;
        //float w,h;
        if( getShowLogCenter( w, xp, yp ) )
            return;        

        windowClientSizeThisFrame( &wd, &ht );
         *xp = (wd/2);// - wdw->loc.wd)/2;
        *yp = (ht/2);// - wdw->loc.ht)/2;
    }
}

float window_getMaxScale(void)
{
    int w, h;
    
    windowClientSizeThisFrame( &w, &h );

    return MIN_RES_MAX_SCALE + (MAX_RES_MAX_SCALE-MIN_RES_MAX_SCALE)*(float)(w-800)/(2048-800);
}

void window_setScale(int wdw, float sc )
{
    if( wdw != -1 )  //bad window id.
    {
        
        float max_sc = window_getMaxScale();
 
        if( sc < MIN_SCALE )
            sc = MIN_SCALE;
        if( sc > max_sc )
            sc = max_sc;

        // if we are shrinking windows, stick
        // don't do this for bendy trays, because being off the edge is okay
        if (sc < winDefs[wdw].loc.sc && (wdw < WDW_TRAY_1 || wdw > WDW_TRAY_8))
        {
            int w,h;
            Wdw * win = &winDefs[wdw];
            int xp, yp;

            windowClientSizeThisFrame(&w,&h);
            window_getUpperLeftPos(wdw, &xp, &yp);

            // if windows are within 5 pixels of edge, push them entirely past the edge
            // this will ensure that 'window_putInBounds' will push them back to edge no matter
            // what we change the scale to
            if( xp + win->loc.wd > w - 5)
                window_setUpperLeftPos(wdw, w, -1);
            if( yp + win->loc.ht > h - 5)
                window_setUpperLeftPos(wdw, -1, h);
        }

        winDefs[wdw].loc.sc = sc;

        window_putInBounds( wdw );
        window_UpdateServer( wdw );
    }
}

void window_SetScaleAll( float sc )
{
    int i;
    for(i = 0; i < MAX_WINDOW_COUNT+custom_window_count; i++)
        window_setScale(i, sc);
}


void window_putInScale( int wdw )
{
    int w, h;
    float max_sc;
    float sc = winDefs[wdw].loc.sc;

    windowClientSizeThisFrame( &w, &h );

    // first make sure gloabal scale is ok
     max_sc = 10;//MIN_RES_MAX_SCALE + (MAX_RES_MAX_SCALE-MIN_RES_MAX_SCALE)*(float)(w-800)/(2048-800);

    if( !sc )
        winDefs[wdw].loc.sc = 1.f;
      if( sc < MIN_SCALE )
        winDefs[wdw].loc.sc = MIN_SCALE;
    if( sc > max_sc && w > 800 )
        winDefs[wdw].loc.sc = max_sc;
}

void window_set_edit_mode( int wdw, int editor)
{
    Wdw        *win = wdwGetWindow( wdw );
    win->editor=editor;
}

// This exists to support the old way of doing things.  Don't use this in new code!
// See typedef struct WdwBase in wdwbase.h!
void window_getUpperLeftPos(int wdw, int *xp, int *yp)
{
    WindowHorizAnchor horiz;
    WindowVertAnchor vert;

    window_getAnchors(wdw, &horiz, &vert);

    if (xp)
    {
        if (!customWindowIsOpen(wdw))
            (*xp) = winDefs[wdw].loc_minimized.xp;
        else
            (*xp) = winDefs[wdw].loc.xp;

        switch (horiz)
        {
        case kWdwAnchor_Left:
            (*xp) -= 0;
        xcase kWdwAnchor_Center:
            (*xp) -= floor(winDefs[wdw].loc.wd * winDefs[wdw].loc.sc / 2 + .5f);
        xcase kWdwAnchor_Right:
            (*xp) -= floor(winDefs[wdw].loc.wd * winDefs[wdw].loc.sc + .5f);
        }
    }

    if (yp)
    {
        if (!customWindowIsOpen(wdw))
            (*yp) = winDefs[wdw].loc_minimized.yp;
        else
            (*yp) = winDefs[wdw].loc.yp;

        switch (vert)
        {
        case kWdwAnchor_Top:
            (*yp) -= 0;
        xcase kWdwAnchor_Middle:
            (*yp) -= floor(winDefs[wdw].loc.ht * winDefs[wdw].loc.sc / 2 + .5f);
        xcase kWdwAnchor_Bottom:
            (*yp) -= floor(winDefs[wdw].loc.ht * winDefs[wdw].loc.sc + .5f);
        }
    }
}

// This exists to support the old way of doing things.  Don't use this in new code!
// See typedef struct WdwBase in wdwbase.h!
void window_setUpperLeftPos(int wdw, float xp, float yp)
{
    WindowHorizAnchor horiz;
    WindowVertAnchor vert;
    float offset = 0.0f;

    window_getAnchors(wdw, &horiz, &vert);

    if (xp > -0.05f)
    {
        switch (horiz)
        {
        case kWdwAnchor_Left:
            offset = 0.0f;
        xcase kWdwAnchor_Center:
            offset = winDefs[wdw].loc.wd * winDefs[wdw].loc.sc / 2;
        xcase kWdwAnchor_Right:
            offset = winDefs[wdw].loc.wd * winDefs[wdw].loc.sc;
        }

        if (!customWindowIsOpen(wdw))
            winDefs[wdw].loc_minimized.xp = floor(xp + offset + .5f);
        else
            winDefs[wdw].loc.xp = floor(xp + offset + .5f);
    }

    if (yp > -0.05f)
    {
        switch (vert)
        {
        case kWdwAnchor_Top:
            offset = 0.0f;
        xcase kWdwAnchor_Middle:
            offset = winDefs[wdw].loc.ht * winDefs[wdw].loc.sc / 2;
        xcase kWdwAnchor_Bottom:
            offset = winDefs[wdw].loc.ht * winDefs[wdw].loc.sc;
        }

        if (!customWindowIsOpen(wdw))
            winDefs[wdw].loc_minimized.yp = floor(yp + offset + .5f);
        else
            winDefs[wdw].loc.yp = floor(yp + offset + .5f);
    }
}

//
//
int window_getDimsEx( int wdw, float *xp, float *yp, float *zp, float* wd, float *ht, float *scale, int * color, int * back_color, bool useAnchor)
{
    int        winXp, winYp;
    float   vec;
    Wdw     *win = wdwGetWindow( wdw );
    WdwBase *loc = &win->loc;

    // NaN test
     if( xp && *xp != *xp )
        *xp = 0;
    if( xp && *yp != *yp )
        *yp = 0;

    PERFINFO_AUTO_START("window_getDims", 1);

    window_putInScale( wdw );

    switch( loc->mode )
    {
        case WINDOW_SHRINKING:
            vec = win->timer / ( float )WINDOW_SHRINK_TIME;
        break;

        case WINDOW_GROWING:
            vec = 1.f - ( win->timer / ( float )WINDOW_GROW_TIME );
        break;

        case WINDOW_DOCKED:
            vec = 1.f;
        break;

        case WINDOW_UNINITIALIZED:
        case WINDOW_DISPLAYING:
        default:
            vec = 0.f;
        break;
    }

    window_putInBounds( wdw );

    if(!customWindowIsOpen(wdw))
        loc = &win->loc_minimized;

    if (useAnchor)
    {
        winXp = loc->xp;
        winYp = loc->yp;
    }
    else
    {
        window_getUpperLeftPos(wdw, &winXp, &winYp);
    }

    if( wdw != WDW_DOCK && xp && yp )
        window_getDockPosn( wdw, xp, yp );

    if(xp)
        *xp = winXp + ( vec * ( *xp - winXp ) );
    if(yp)
        *yp = winYp + ( vec * ( *yp - winYp ) );

     if(zp)
    {
        if( window_getMode( wdw ) == WINDOW_SHRINKING )
            *zp = BASE_WINDOW_Z;
        else
        {
            if( loc->locked && win->parent )
                *zp = ZCALC( win->parent->id );
            else
                *zp = ZCALC(wdw);
        }
    }
   
    if (winDefs[wdw].forceColor || windowIsSaved(wdw))
    {     
        if(ht)
            *ht = loc->sc*loc->ht*(1-vec);
        if(wd)
            *wd = loc->sc*loc->wd*(1-vec);
        if(scale)
            *scale = loc->sc*(1-vec);
        if(color)
            *color = (loc->color&0xffffff00)|(int)((loc->color&0xff)*(1-vec));
        if(back_color)
            *back_color = (loc->back_color&0xffffff00)|(int)((loc->back_color&0xff)*(1-vec));
    }
    else 
    {
        // If this window isn't saved to the database, use the color and scale of one that is

        Wdw     *goodwin = wdwGetWindow( WDW_STAT_BARS );
        if(ht)
            *ht = goodwin->loc.sc*loc->ht*(1-vec);
        if(wd)
            *wd = goodwin->loc.sc*loc->wd*(1-vec);
        if(scale)
        {
            *scale = goodwin->loc.sc*(1-vec);
            if (*scale > 0)
                loc->sc = *scale;    // save it for future calculations
        }
        if(color)
            *color = (goodwin->loc.color&0xffffff00)|(int)((goodwin->loc.color&0xff)*(1-vec));
        if(back_color)
            *back_color = (goodwin->loc.back_color&0xffffff00)|(int)((goodwin->loc.back_color&0xff)*(1-vec));
    }

    if( loc->maximized )
     {
        int w, h;
        windowClientSize(&w,&h);

        if(xp)
            *xp = 0;
        if(yp)
            *yp = JELLY_HT*(loc->sc);
        if(wd)
            *wd = MAX(*wd, w);
        if(ht)
            *ht = MAX(*ht, h - JELLY_HT*(loc->sc));
        if(back_color)
            *back_color = 0x000000ff;
    }

    PERFINFO_AUTO_STOP();
    
    return loc->mode != WINDOW_DOCKED &&
           loc->mode != WINDOW_UNINITIALIZED &&
           (1 - vec) != 0;
}

int window_getDims( int wdw, float *xp, float *yp, float *zp, float* wd, float *ht, float *scale, int * color, int * back_color )
{
    return window_getDimsEx(wdw, xp, yp, zp, wd, ht, scale, color, back_color, false);
}

//
//
void window_setDims( int wdw, float x, float y, float width, float height )
{
      if( wdw != -1 /*&& winDefs[wdw].loc.mode == WINDOW_DISPLAYING*/ )  //bad window id.
    {
        if( width >= 0 )
            winDefs[wdw].loc.wd = floor(width/winDefs[wdw].loc.sc+.5f);
        if( height >= 0 )
            winDefs[wdw].loc.ht = floor(height/winDefs[wdw].loc.sc+.5f);

        window_setUpperLeftPos(wdw, x, y);

        window_putInBounds( wdw );
    }
}

//
//
void window_UpdateServer(int wdw)
{
    if(wdw != -1 && !game_state.edit_base && !isMenu(MENU_LOGIN)) //prevent the settings window hack from sending window data
    {
        sendWindowToServer(&winDefs[wdw].loc, wdw, windowNeedsSaving(wdw));
    }
}

void window_UpdateServerAllWindows()
{
    int winIndex;
    for (winIndex = 0; winIndex < (MAX_WINDOW_COUNT + MAX_CUSTOM_WINDOW_COUNT); ++winIndex)
    {
        if (windowNeedsSaving(winDefs[winIndex].id))
        {
            window_UpdateServer(winDefs[winIndex].id);
        }
    }
}

//
//
void window_permSetDims( int wdw )
{
    window_UpdateServer(wdw);
}


//--------------------------------------------------------------------------------------------
// Display Window Helpers ////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------
//
//
enum
{
    DRAG_NONE,
    DRAG_LR,
    DRAG_LL,
    DRAG_UL,
    DRAG_UR,
    DRAG_L,
    DRAG_R,
    DRAG_UP,
    DRAG_DOWN,
};

#define VOID_COL_WD 7

static int window_xDragDiff, window_yDragDiff;
//
//
static void window_checkFrameCollision( int w, int mx, int my )
{
    CBox box, inside;

    Wdw * wdw = &winDefs[w];

    F32 sc, x, y, wd, ht;
    int rad;
    int hit = FALSE;

    int upper_lock = wdw->loc.locked;
    int lower_lock = wdw->loc.locked;

     window_getDims( w, &x, &y, 0, &wd, &ht, &sc, 0, 0 );

    rad = (R10 + PIX3)*sc;
    if( wdw->parent )
    {
        if( wdw->parent->below )
            upper_lock = 0;
        else
            lower_lock = 0;
    }

    if( wdw->loc.draggable_frame != VERTONLY )
    {
        //right
        //----------------------------------------------------------
        BuildCBox( &box, x + wd - PIX3*sc, y + rad, PIX3*sc, ht - 2*rad );

        if( mouseCollision(&box ) )
        {
            hit = TRUE;
            setCursor( "cursor_win_horizresize.tga", NULL, FALSE, 15, 10 );

            if( mouseDown(MS_LEFT) )
            {
                window_xDragDiff    = mx - (x+wd);
                wdw->drag_mode      = DRAG_R;
                collisions_off_for_rest_of_frame = TRUE;
            }
        }
    }

    if( (!wdw->loc.locked || upper_lock) && wdw->loc.draggable_frame != HORZONLY )
    {
        //top
        //----------------------------------------------------------
        BuildCBox( &box, x + rad, y, wd - 2*rad, PIX3*sc );

        if( mouseCollision(&box ) )
        {
            hit = TRUE;
            setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );

            if( mouseDown(MS_LEFT) )
            {
                window_yDragDiff = my - y;
                wdw->drag_mode = DRAG_UP;
                collisions_off_for_rest_of_frame = TRUE;
            }
        }

        if( wdw->loc.draggable_frame != VERTONLY && wdw->loc.draggable_frame != TRAY_FIXED_SIZES )
        {
            // upper right
            //----------------------------------------------------------
            BuildCBox( &box, x + wd - rad, y, rad, rad );
            BuildCBox( &inside, x + wd - rad, y + rad - VOID_COL_WD, VOID_COL_WD, VOID_COL_WD );

            if( mouseCollision(&box ) && !mouseCollision(&inside) )
            {
                hit = TRUE;
                setCursor( "cursor_win_diagresize_backward.tga", NULL, FALSE, 12, 12 );

                if( mouseDown( MS_LEFT) )
                {
                    window_xDragDiff = mx - (x+wd);
                    window_yDragDiff = my - y;
                    wdw->drag_mode = DRAG_UR;
                    collisions_off_for_rest_of_frame = TRUE;
                }
            }
        }
    }

    if((!wdw->loc.locked || lower_lock) && wdw->loc.draggable_frame != HORZONLY )
    {
        //bottom
        //----------------------------------------------------------
        BuildCBox( &box, x + rad, y + ht - PIX3*sc, wd - 2*rad, PIX3*sc );

        if( mouseCollision(&box ) )
        {
            hit = TRUE;
            setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );

            if( mouseDown(MS_LEFT) )
            {
                window_yDragDiff = my - (y+ht);
                wdw->drag_mode = DRAG_DOWN;
                collisions_off_for_rest_of_frame = TRUE;
            }
        }

        if( wdw->loc.draggable_frame != VERTONLY && wdw->loc.draggable_frame != TRAY_FIXED_SIZES)
        {
            // lower right
            //----------------------------------------------------------
            BuildCBox( &box, x + wd - rad, y + ht - rad, rad, rad );
            BuildCBox( &inside, x + wd - rad, y + ht - rad, VOID_COL_WD, VOID_COL_WD );

            if( mouseCollision(&box ) && !mouseCollision(&inside) )
            {
                hit = TRUE;
                setCursor( "cursor_win_diagresize_foreward.tga", NULL, FALSE, 12, 12 );

                if( mouseDown( MS_LEFT) )
                {
                    wdw->drag_mode = DRAG_LR;
                    window_xDragDiff = mx - (x+wd);
                    window_yDragDiff = my - (y+ht);
                    collisions_off_for_rest_of_frame = TRUE;
                }
            }
        }
    }

    if( !wdw->loc.locked && wdw->loc.draggable_frame != VERTONLY)
    {
        if (wdw->loc.draggable_frame != HORZONLY && wdw->loc.draggable_frame != TRAY_FIXED_SIZES) 
        {        
            // upper left
            //----------------------------------------------------------
            BuildCBox( &box, x, y, rad, rad );
            BuildCBox( &inside, x + rad - VOID_COL_WD, y + rad - VOID_COL_WD, VOID_COL_WD, VOID_COL_WD );

            if( mouseCollision(&box ) && !mouseCollision(&inside) )
            {
                hit = TRUE;
                setCursor( "cursor_win_diagresize_foreward.tga", NULL, FALSE, 12, 12 );

                if( mouseDown( MS_LEFT) )
                {
                    window_xDragDiff = mx - x;
                    window_yDragDiff = my - y;
                    wdw->drag_mode = DRAG_UL;
                    collisions_off_for_rest_of_frame = TRUE;
                }
            }

            // lower left
            //----------------------------------------------------------
            BuildCBox( &box, x, y + ht - rad, rad, rad );
            BuildCBox( &inside, x + rad - VOID_COL_WD, y + ht - rad, VOID_COL_WD, VOID_COL_WD );

            if( mouseCollision(&box ) && !mouseCollision(&inside) )
            {
                hit = TRUE;
                setCursor( "cursor_win_diagresize_backward.tga", NULL, FALSE, 12, 12 );
    
                if( mouseDown( MS_LEFT) )
                {
                    window_xDragDiff    = mx - x;
                    window_yDragDiff    = my - (y+ht);
                    wdw->drag_mode = DRAG_LL;
                    collisions_off_for_rest_of_frame = TRUE;
                }
            }
        }

        // left
        //----------------------------------------------------------
        BuildCBox( &box, x, y + rad, PIX3*sc, ht - 2*rad );
        if( mouseCollision(&box ) )
        {
            hit = TRUE;
            setCursor( "cursor_win_horizresize.tga", NULL, FALSE, 15, 10 );

            if( mouseDown(MS_LEFT) )
            {
                window_xDragDiff    = mx - x;
                wdw->drag_mode = DRAG_L;
                collisions_off_for_rest_of_frame = TRUE;
            }
        }
    }
}

//
//
static void window_Resize( int w )
{
    int i;
    int anotherX, anotherY;
    F32 sc = winDefs[w].loc.sc;
     F32 twd = winDefs[w].loc.wd*sc;
    F32 tht = winDefs[w].loc.ht*sc;
    F32 tx, ty;
    F32 wd = winDefs[w].loc.wd*sc;
    F32 ht = winDefs[w].loc.ht*sc;
    F32 xp, yp;

    int clientWd, clientHt, mx, my;

     if( winDefs[w].being_dragged || winDefs[w].drag_mode == DRAG_NONE )
        return;

    window_getUpperLeftPos(w, &anotherX, &anotherY);
    xp = tx = anotherX;
    yp = ty = anotherY;

    if (!gWindowDragging)
    {
        gWindowDragging = TRUE;
        // Don't reset to true if it's 2...
    }

    inpMousePos( &mx, &my );
    if (isLetterBoxMenu())
    {
        reversePointToScreenScalingFactori(&mx, &my);
        clientWd = DEFAULT_SCRN_WD;
        clientHt = DEFAULT_SCRN_HT;
    }
    else
    {
        windowClientSizeThisFrame( &clientWd, &clientHt );
    }

    if( mx <= SNAP_WD)
    {
        mx = 0;
        window_xDragDiff = 0;
    }
    if( mx >= clientWd-SNAP_WD )
    {
        mx = clientWd;
        window_xDragDiff = 0;
    }
    if( my <= SNAP_WD )
    {
        my = 0;
        window_yDragDiff = 0;
    }
    if( my >= clientHt-SNAP_WD )
    {
        my = clientHt;
        window_yDragDiff = 0;
    }

    switch( winDefs[w].drag_mode )
    {
        case DRAG_R:
            {
                setCursor( "cursor_win_horizresize.tga", NULL, FALSE, 15, 10 );
                twd = (mx - window_xDragDiff)-xp;
            }break;
        case DRAG_DOWN:
            {
                setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );
                tht = (my - window_yDragDiff)-yp;
            }break;
        case DRAG_LR:
            {
                setCursor( "cursor_win_diagresize_foreward.tga", NULL, FALSE, 12, 12 );
                twd = (mx - window_xDragDiff)-xp;
                tht = (my - window_yDragDiff)-yp;
            }break;

        case DRAG_LL:
            {
                setCursor( "cursor_win_diagresize_backward.tga", NULL, FALSE, 12, 12 );
                tht = (my - window_yDragDiff)-yp;
                twd = -(mx - window_xDragDiff)+xp+wd;
                tx  = (mx - window_xDragDiff);
                ty = yp;
            }break;

        case DRAG_L:
            {
                setCursor( "cursor_win_horizresize.tga", NULL, FALSE, 15, 10 );
                tx  = (mx - window_xDragDiff);
                twd = -(mx - window_xDragDiff)+xp+wd;
            }break;
        case DRAG_UP:
            {
                setCursor( "cursor_win_vertresize.tga", NULL, FALSE, 10, 15 );
                tht = -(my - window_yDragDiff)+yp+ht;
                ty  = (my - window_yDragDiff);
            }break;
        case DRAG_UL:
            {
                setCursor( "cursor_win_diagresize_foreward.tga", NULL, FALSE, 12, 12 );
                tx  = (mx - window_xDragDiff);
                twd = -(mx - window_xDragDiff)+xp+wd;
                tht = -(my - window_yDragDiff)+yp+ht;
                ty  = (my - window_yDragDiff);
            }break;

        case DRAG_UR:
            {
                setCursor( "cursor_win_diagresize_backward.tga", NULL, FALSE, 12, 12 );
                twd = (mx - window_xDragDiff)-xp;
                tht = -(my - window_yDragDiff)+yp+ht;
                ty  = (my - window_yDragDiff);
                tx  = xp;
            }break;

        case DRAG_NONE:
        default:
            {
                return;
            }

    }

    // check movement against bounds now
    if( twd < winDefs[w].min_wd*sc )
    {
        twd = winDefs[w].min_wd*sc;

        if( winDefs[w].drag_mode == DRAG_UL || winDefs[w].drag_mode == DRAG_LL || winDefs[w].drag_mode == DRAG_L )
            tx = xp + wd - twd;
    }
    if( twd > winDefs[w].max_wd*sc )
    {
        twd = winDefs[w].max_wd*sc;

        if( winDefs[w].drag_mode == DRAG_UL || winDefs[w].drag_mode == DRAG_LL || winDefs[w].drag_mode == DRAG_L )
            tx = xp + wd - twd;
    }
    if ( tht < winDefs[w].min_ht*sc )
    {
        tht = winDefs[w].min_ht*sc;
        if( winDefs[w].drag_mode == DRAG_UL || winDefs[w].drag_mode == DRAG_UR || winDefs[w].drag_mode == DRAG_UP )
            ty = yp + ht - tht;
    }
    if( tht > winDefs[w].max_ht*sc )
    {
        tht = winDefs[w].max_ht*sc;

        if( winDefs[w].drag_mode == DRAG_UL || winDefs[w].drag_mode == DRAG_UR || winDefs[w].drag_mode == DRAG_UP )
            ty = yp + ht - tht;
    }

    // snap to parent window width
    if( winDefs[w].loc.locked && winDefs[w].parent && 
         ((twd > (winDefs[w].parent->loc.wd - 44)*sc && twd < winDefs[w].parent->loc.wd*sc) || 
          (twd < (winDefs[w].parent->loc.wd + 44)*sc && twd > winDefs[w].parent->loc.wd*sc)) ) // forty is for curvy jelly at end
    {
        twd = winDefs[w].parent->loc.wd*sc;
    }

     for( i = 0; i < winDefs[w].num_children; i++ )
    {
         Wdw * child = winDefs[w].children[i].wdw;
        if( child && child->loc.locked && window_getMode(child->id) == WINDOW_DISPLAYING )
        {
            if( (twd > (child->loc.wd-44)*sc && twd < child->loc.wd*sc) ||
                (twd < (child->loc.wd+44)*sc && twd > child->loc.wd*sc) )
                twd = child->loc.wd*sc;
        }
    }

    if( winDefs[w].loc.draggable_frame == TRAY_FIXED_SIZES )
    {
        float fixed_size[] = {50,90,130,170,250,450}; //6

        if( winDefs[w].drag_mode == DRAG_L || winDefs[w].drag_mode == DRAG_R )
        {
            for( i = 0; i < 6; i++ )
            {
                if( twd <= fixed_size[i] || i == 5)
                {
                    twd = fixed_size[i]*winDefs[w].loc.sc;
                    tht = fixed_size[5-i]*winDefs[w].loc.sc;
                    break;
                }
            }
        }
        else if( winDefs[w].drag_mode == DRAG_UP || winDefs[w].drag_mode == DRAG_DOWN )
        {
            for( i = 0; i < 6; i++ )
            {
                if( tht <= fixed_size[i] || i == 5)
                {
                    tht = fixed_size[i]*winDefs[w].loc.sc;
                    twd = fixed_size[5-i]*winDefs[w].loc.sc;
                    break;
                }
            }
        }

        if( winDefs[w].drag_mode == DRAG_L )
            tx = xp + wd - twd; 
        if( winDefs[w].drag_mode == DRAG_UP )
            ty = yp + ht - tht;
    }

       window_setDims( w, tx, ty, twd, tht );

    if( !isDown(MS_LEFT) )
    {
        window_permSetDims( w );
        winDefs[w].drag_mode = DRAG_NONE;
        saveCustomWindows();
    }
}



//
//
int window_childButton( Wdw *wdw, float xOffset, float z, int child_id, CBox *out_box )
{
    int i, hit = FALSE, clr;
    float x, y, wd, sc;
    CBox box;
    ChildWindow *cWin =  &wdw->children[child_id];

    if(cWin->command && ((strstr(cWin->command,"toggle pet") && !optionGet(kUO_ShowPets))))
        return 0;
    if( cWin->idx == WDW_EMAIL && optionGet(kUO_DisableEmail) )
        return 0;

    // Disable the Convert Enhancement button unless you have some Converters
    if(cWin->idx == WDW_CONVERT_ENHANCEMENT)
    {
        Entity *e = playerPtr();
        if(e && e->pchar && character_SalvageCount(e->pchar, salvage_GetItem( "S_EnhancementConverter" )) <= 0)
            return 0;
    }

    sc = wdw->loc.sc;
     wd = str_wd( &game_12, sc, sc, cWin->name );

    childButton_getCenter( wdw, child_id, &x, &y );

    x += xOffset;

     BuildCBox( &box, x - ( wd + CHILD_BUTTON_SPACE*sc)/2, y - JELLY_HT*sc/2, wd + CHILD_BUTTON_SPACE*sc, JELLY_HT*sc );
    if( out_box )
        BuildCBox( out_box, x - ( wd + CHILD_BUTTON_SPACE*sc)/2, y - JELLY_HT*sc/2, wd + CHILD_BUTTON_SPACE*sc, JELLY_HT*sc );

    if( mouseCollision(&box) )
    {
        hit = D_MOUSEOVER;
        cWin->opacity += TIMESTEP*JELLY_FADE_SPEED*2;
        if( cWin->opacity > 1.f )
            cWin->opacity = 1.f;

         if( mouseClickHit( &box, MS_LEFT) )
        {
            hit = D_MOUSEDOWN;

            if( cWin->command )
                cmdParse( cWin->command );
            else if( cWin->wdw )
            {
                if( window_getMode( cWin->wdw->id ) == WINDOW_GROWING ||  window_getMode( cWin->wdw->id ) == WINDOW_DISPLAYING )
                    window_setMode( cWin->wdw->id, WINDOW_SHRINKING );
                else
                {
                    if( cWin->wdw->loc.locked )
                    {
                        for( i = 0; i < wdw->num_children; i++ )
                        {
                            Wdw * child = wdw->children[i].wdw;
                            if( child && child->loc.locked )
                                window_setMode( child->id, WINDOW_SHRINKING );
                        }
                    }

                    window_setMode( cWin->wdw->id, WINDOW_GROWING );
                }
            }
        }
        // if chat tabs are dragged onto child window #'s, the child windows will auto-open
        else if(cWin->wdw && cWin->wdw->id >= WDW_CHAT_CHILD_1 && cWin->wdw->id <=WDW_CHAT_CHILD_4  && cursor.dragging 
            && cursor.drag_obj.type == kTrayItemType_Tab
            && window_getMode( cWin->wdw->id ) != WINDOW_DISPLAYING)
        {
            if( cWin->wdw->loc.locked )
            {
                for( i = 0; i < wdw->num_children; i++ )
                {
                    Wdw * child = wdw->children[i].wdw;
                    if( child && child->loc.locked )
                        window_setMode( child->id, WINDOW_SHRINKING );
                }
            }
            window_setMode( cWin->wdw->id, WINDOW_GROWING );
        }
    }
    else
    {
        cWin->opacity -= TIMESTEP*JELLY_FADE_SPEED;
        if( cWin->opacity < CHILD_OPACITY )
            cWin->opacity = CHILD_OPACITY;
    }

     clr = (0xffffff00 | (int)(255*cWin->opacity) );

    if( !cWin->command && cWin->wdw && windowUp( cWin->wdw->id ) )
        clr = CLR_GREEN;
    else if (!cWin->command && cWin->wdw)
    {
        if( (playerPtr() && cWin->wdw->id == WDW_GROUP && playerPtr()->pl->lfg&LFG_NOGROUP) ||
            (playerPtr() && cWin->wdw->id == WDW_LEAGUE && playerPtr()->pl->lfg&LFG_NOGROUP) )
        {
            clr = CLR_RED;
        }
        else if (cWin->wdw->id == WDW_EMAIL)
        {
            static int s_timerTracker;
            s_timerTracker++;
            if (hasEmail(MVM_CERTIFICATION) || hasEmail(MVM_VOUCHER))
            {
                if (optionGet(kUO_BlinkCertifications))
                {
                    float pulsePercent = (sinf(((int)((s_timerTracker+i)*0.5f)%90) *2*PI/90)+1)/2.f;
                    interp_rgba(pulsePercent, CLR_RED, clr, &clr);
                }
                else
                {
                    clr = CLR_RED;
                }
            }
            else if (hasEmail(MVM_MAIL))
            {
                clr = CLR_RED;
            }
        }
    }

     if( cWin->wdw && cWin->wdw->id == WDW_ENHANCEMENT ) 
    {
           if( character_BoostInventoryFull(playerPtr()->pchar) )
            clr = CLR_RED;
        //if( window_getMode(WDW_TRADE) != WINDOW_DOCKED )
        //    clr = 0x88000000 |(int)(255*cWin->opacity);
         if(mouseDoubleClickHit(&box, MS_LEFT))
        {
            cmdParse("manage");
        }
    }

    if( cWin->wdw && cWin->wdw->id == WDW_INSPIRATION ) 
    {
        if( character_InspirationInventoryFull(playerPtr()->pchar) )
            clr = CLR_RED;
    }

    if( cWin->command && strstr(cWin->command,"toggle salvage") ) 
    {
        if( character_InventoryFull(playerPtr()->pchar, kInventoryType_Salvage) )
            clr = CLR_RED;
    }

    if( cWin->command && strstr(cWin->command,"toggle recipe") ) 
    {
        if( character_InventoryFull(playerPtr()->pchar, kInventoryType_Recipe) )
            clr = CLR_RED;
    }

    if (wdw->id == WDW_EMAIL && windowUp( wdw->id ))
    {
        int currentView = email_getView();
        if( cWin->command && stricmp(cWin->command,"mailview certification")==0 ) 
        {
            if (currentView == MVM_CERTIFICATION)
            {
                clr = CLR_GREEN;
            }
            else
            {
                static int s_timerTracker;
                s_timerTracker++;
                if (hasEmail(MVM_CERTIFICATION))
                {
                    if (optionGet(kUO_BlinkCertifications))
                    {
                        float pulsePercent = (sinf(((int)((s_timerTracker+i)*0.5f)%90) *2*PI/90)+1)/2.f;
                        interp_rgba(pulsePercent, CLR_RED, clr, &clr);
                    }
                    else
                    {
                        clr = CLR_RED;
                    }
                }
                
            }
        }

        if( cWin->command && stricmp(cWin->command,"mailview voucher")==0 ) 
        {
            if (currentView == MVM_VOUCHER)
            {
                clr = CLR_GREEN;
            }
            else
            {
                static int s_timerTracker;
                s_timerTracker++;
                if (hasEmail(MVM_VOUCHER))
                {
                    if (optionGet(kUO_BlinkCertifications))
                    {
                        float pulsePercent = (sinf(((int)((s_timerTracker+i)*0.5f)%90) *2*PI/90)+1)/2.f;
                        interp_rgba(pulsePercent, CLR_RED, clr, &clr);
                    }
                    else
                    {
                        clr = CLR_RED;
                    }
                }
            }
        }
        
        if (cWin->command && stricmp(cWin->command, "mailview mail") == 0)
        {
            if (currentView == MVM_MAIL)
            {
                clr = CLR_GREEN;
            }
        }
    }
    
      font( &game_12 ); 
    font_color( clr, clr );
    if( strcmp(cWin->name, "+")==0)
        y += 2*sc;

    if( sc < .82f )
        font( &game_12_dynamic ); 

    cprntEx( x, y, z+1, sc, sc, (CENTER_X | CENTER_Y), cWin->name );
    return hit;
}

int window_maximizeButton( Wdw *wdw, float tx, float ty, float z, float sc )
{
    CBox box1, box2, cbox;
    int hit = FALSE, clr = (0xffffff00 | (int)(255*window_GetOpacity(wdw->id)) );
    F32 wd = 15, ht = 10;

     z+=3;

    if( wdw->loc.maximized )
    {
         BuildCBox( &box1, tx - wd/2*sc, ty - ht/2*sc, (wd-5)*sc, (ht-3)*sc );
        BuildCBox( &box2, tx - wd/2*sc + 5*sc, ty - ht/2*sc + 3*sc, (wd-5)*sc, (ht-3)*sc );
    }
    else
    {
          BuildCBox( &box1, tx - wd/2*sc, ty - ht/2*sc, wd*sc, 3*sc );
        BuildCBox( &box2, tx - wd/2*sc, ty - ht/2*sc, wd*sc, ht*sc );
    }

    drawBox( &box1, z, clr, 0 );
    drawBox( &box2, z, clr, 0 );

     BuildCBox( &cbox, tx - wd/2*sc - 3*sc, ty - ht/2*sc - 3*sc, (wd+6)*sc, (ht+6)*sc );

    if( mouseCollision( &cbox ) )
    {
        hit = D_MOUSEOVER;
        clr = CLR_WHITE;

        drawBox( &box1, z, CLR_WHITE, 0xffffff33 );
        drawBox( &box2, z, CLR_WHITE, 0xffffff33 );

        if( isDown( MS_LEFT ) )
            hit = D_MOUSEDOWN;

        if( mouseClickHit( &cbox, MS_LEFT) )
        {
            wdw->loc.maximized = !wdw->loc.maximized;
            window_UpdateServer(wdw->id);
        }
    }
    return hit;
}

//
//
int window_closeButton( Wdw *wdw, float x, float ty, float z, float sc )
{
    CBox box;
    float tx;
    int hit = FALSE, clr = (0xffffff00 | (int)(255*window_GetOpacity(wdw->id)) );

    static AtlasTex * border;
    static AtlasTex * click;
    static AtlasTex * button;
    static int texBindInit=0;

    if (wdw->noCloseButton)
        return 0;

    if (!texBindInit)
    {
        texBindInit = 1;
        border = atlasLoadTexture( "Jelly_close_bevel.tga" );
        click  = atlasLoadTexture( "Jelly_close_click.tga" );
        button = atlasLoadTexture( "Jelly_close_rest.tga" );
    }

    if(wdw->num_children == 0 || wdw->parent || wdw->id == WDW_PET ) // Yes this is an ugly hack
    {
        tx = x - border->width*sc;

          BuildCBox( &box, tx - border->width*sc/2 - 5*sc, ty - border->height*sc/2 - 5*sc, (border->width + 10)*sc, (border->height + 10)*sc );
        if( mouseCollision( &box ) )
        {
            hit = D_MOUSEOVER;
            clr = CLR_WHITE;

            if( isDown( MS_LEFT ) )
            {
                hit = D_MOUSEDOWN;
                display_sprite( click, tx - click->width*sc/2, ty - click->height*sc/2, z+3, sc, sc, CLR_WHITE );
            }

            if( mouseClickHit( &box, MS_LEFT) )
            {
                if(wdw->id == WDW_CONTACT_DIALOG)
                {
                    cd_exit();
                    cd_clear();
                }
                else if(wdw->id == WDW_TRADE )
                    sendTradeCancel( TRADE_FAIL_I_LEFT );
                else if(wdw->id == WDW_CHAT_OPTIONS)
                    chatTabClose(1);
                else if(wdw->id == WDW_OPTIONS)
                    sendOptions(0);

                window_setMode( wdw->id, WINDOW_SHRINKING );
            }
        }

        if( window_getMode( wdw->id) == WINDOW_SHRINKING )
            display_sprite( click, tx - click->width*sc/2, ty - click->height*sc/2, z+3, sc, sc, CLR_WHITE );

        display_sprite( button, tx - button->width*sc/2, ty - button->height*sc/2, z+2, sc, sc, clr );
        display_sprite( border, tx - border->width*sc/2, ty - border->height*sc/2, z+1, sc, sc, window_GetColor(wdw->id) & (0xffffff00 | (int)(255*window_GetOpacity(wdw->id)) ) );
    }

    return hit;
}

//
//
int window_lockButton( Wdw *wdw, float x, float ty, float z, float sc )
{
    CBox box;
    float tx;
    int hit = FALSE, clr = window_GetColor(wdw->id) & (0xffffff00 | (int)(255*window_GetOpacity(wdw->id)) );
    static AtlasTex * pin;
    static AtlasTex * unpin;
    static AtlasTex * border;
    static int texBindInit=0;
    if (!texBindInit) {
        texBindInit = 1;
        pin    = atlasLoadTexture( "Jelly_pin.tga" );
        unpin  = atlasLoadTexture( "Jelly_pin_ring_open.tga" );
        border = atlasLoadTexture( "Jelly_pin_ring.tga" );
    }

    tx = x + border->width*sc;

    BuildCBox( &box, tx - border->width*sc/2, ty - border->height*sc/2, border->width*sc, border->height*sc );

    if( mouseCollision( &box ) )
    {
        hit = D_MOUSEOVER;
        clr = window_GetColor(wdw->id);

        if( mouseClickHit( &box, MS_LEFT) )
        {
             hit = D_MOUSEDOWN;

            if( wdw->loc.locked )
            {
                wdw->loc.locked = 0;
                window_UpdateServer(wdw->id);
            }
            else
            {
                wdw->loc.locked = 2;
                window_UpdateServer(wdw->id);
            }

            if( wdw->loc.locked && (wdw->parent->open_child || wdw->parent->loc.mode != WINDOW_DISPLAYING) )
            {
                window_setMode( wdw->parent->children[wdw->parent->child_id].wdw->id, WINDOW_SHRINKING );
            }
        }
    }

     if( wdw->loc.locked )
    {
        display_sprite( pin, tx - pin->width*sc/2, ty - pin->height*sc/2, z+2, sc, sc, clr|0xff );
        display_sprite( border, tx - border->width*sc/2, ty - border->height*sc/2, z+1, sc, sc, window_GetColor(wdw->id) & (0xffffff00 | (int)(255*window_GetOpacity(wdw->id)) ) );
    }
    else
        display_sprite( unpin, tx - unpin->width*sc/2, ty - unpin->height*sc/2, z+1, sc, sc, clr|0xff );

    return hit;
}

static int window_storeButton( Wdw *wdw, float x, float ty, float z, float sc, const char * storeLink )
{
    
    CBox box;
    float tx;
    int hit = FALSE;
    int alpha = (int)(255*window_GetOpacity(wdw->id));
    int clr = window_GetColor(wdw->id) & (0xffffff00 | alpha );
    int txtClr = 0xffffff00 | alpha;
    static AtlasTex * storeLinkTex;
    static AtlasTex * storeSlowLinkTex;
    static AtlasTex * storeDownLinkTex;
    static int texBindInit=0;
    AccountServerStatus status = inventoryClient_GetAcctAuthoritativeState();
    char *toolTipText;

    if (!texBindInit) 
    {
        texBindInit = 1;
        storeLinkTex = atlasLoadTexture("icon_ig_storeaccess_small");
        storeSlowLinkTex = atlasLoadTexture("icon_ig_serverslow_small");
        storeDownLinkTex = atlasLoadTexture("icon_ig_serverdown_small");
    }

    sc *= 0.8f;  //scale down asset for now

    tx = x - storeLinkTex->width*sc;

    BuildCBox( &box, tx, ty - storeLinkTex->height*sc/2, storeLinkTex->width*sc, storeLinkTex->height*sc );

    if( mouseCollision( &box ) )
    {
        hit = D_MOUSEOVER;
        clr = window_GetColor(wdw->id);
        txtClr = CLR_WHITE;

        if( mouseClickHitNoCol( MS_LEFT) && status == ACCOUNT_SERVER_UP)
        {
            hit = D_MOUSEDOWN;
            if( strlen(storeLink) == 8 )
            {
                webStoreOpenProduct(storeLink);
            }
            else
            {
                webStoreOpenCategory(storeLink);
            }
        }

        switch(status)
        {
            case ACCOUNT_SERVER_UP:
                toolTipText = "paragonStoreButton";
                break;
            case ACCOUNT_SERVER_SLOW:
                toolTipText = "paragonstoreSlowButton";
                break;
            case ACCOUNT_SERVER_DOWN:
                toolTipText = "paragonstoreDownButton";
                break;
        }
        setHybridTooltipWindow(&box, toolTipText, wdw->id);
    }

    switch(status)
    {
    case ACCOUNT_SERVER_UP:
        display_sprite( storeLinkTex, tx, ty - storeLinkTex->height*sc/2, z+1.f, sc, sc, clr );
        break;
    case ACCOUNT_SERVER_SLOW:
        display_sprite( storeSlowLinkTex, tx, ty - storeLinkTex->height*sc/2, z+1.f, sc, sc, clr );
        break;
    case ACCOUNT_SERVER_DOWN:
        display_sprite( storeDownLinkTex, tx, ty - storeLinkTex->height*sc/2, z+1.f, sc, sc, clr );
        break;
    }

    font(&game_12);
    font_color(txtClr, txtClr);
    cprnt(tx+(x-tx)*sc*0.7f, ty+storeLinkTex->height*sc/6.f, z+2.f, sc, sc, "ShopString");

    return hit;
}

int jelly_collision( Wdw * wdw, int reverse )
{
    CBox box;
    int winXp, winYp;

    if( !windowUp( wdw->id ) )
        return FALSE;

    window_getUpperLeftPos(wdw->id, &winXp, &winYp);

    if( ((!wdw->below && !wdw->flip) || (wdw->below && wdw->flip)) )
    {
        if( reverse )
            BuildCBox( &box, winXp, winYp + wdw->loc.ht*wdw->loc.sc, wdw->loc.wd*wdw->loc.sc, JELLY_HT*wdw->loc.sc );
        else
            BuildCBox( &box, winXp, winYp - JELLY_HT*wdw->loc.sc, wdw->loc.wd*wdw->loc.sc, JELLY_HT*wdw->loc.sc);
    }
    else
    {
        if( reverse )
            BuildCBox( &box, winXp, winYp - JELLY_HT*wdw->loc.sc, wdw->loc.wd*wdw->loc.sc, JELLY_HT*wdw->loc.sc);
        else
            BuildCBox( &box, winXp, winYp + wdw->loc.ht*wdw->loc.sc, wdw->loc.wd*wdw->loc.sc, JELLY_HT*wdw->loc.sc );
    }

    return mouseCollision( &box );
}

//recursive
static Wdw * getOldestAttachedAncestor(Wdw * wdw)
{
    if( wdw->loc.locked && wdw->parent )
        return getOldestAttachedAncestor(wdw->parent);
    else
        return wdw;
}

//recursive
static int getJellyFamilyCollision(Wdw * wdw)
{
    int i;
    // first check this window
    if( jelly_collision( wdw, 0 ) )
    {
        return true;
    }
    else if ( wdw->parent && wdw->loc.locked && jelly_collision( wdw->parent, 0 ) ) // now check parent if applicable
        return 2;
    else
    {
        // now check all children
        for( i = 0; i < wdw->num_children; i++ )
        {
            if( wdw->children[i].wdw && wdw->children[i].wdw->loc.locked)
            {
                int ret = getJellyFamilyCollision(wdw->children[i].wdw);
                if (ret)
                {
                    return ret;
                }
                else if( wdw->children[i].wdw->loc.wd > wdw->loc.wd && jelly_collision( wdw->children[i].wdw, 1 ))
                {
                    return 3;
                }
            }
        }
    }

    return false;
}

int jelly_assemblyCollision( Wdw * wdw )
{
    //get the oldest ancestor
    Wdw * ancestor = getOldestAttachedAncestor(wdw);

    //check all children
    return getJellyFamilyCollision(ancestor);
}

int window_drawJelly( Wdw * wdw )
{
    int w, h, hit = 0, i, color, bcolor, clr = window_GetColor(wdw->id) & (0xffffff00 | (int)(255*window_GetOpacity(wdw->id)) );
    float x, y, wd, ht, scale, tx, ty, z, ychild, htchild=0;
    CBox box;

    AtlasTex *left, *right;
    static AtlasTex * meat;
    static AtlasTex * Jelly_lg_end_L;
    static AtlasTex * Jelly_lg_end_R;
    static AtlasTex * Jelly_sm_end_L;
    static AtlasTex * Jelly_sm_end_R;
    static int texBindInit=0;
    Wdw * ancestor = getOldestAttachedAncestor(wdw);
    if (!texBindInit) {
        texBindInit = 1;
        meat = atlasLoadTexture( "Jelly_meat.tga" );
        Jelly_lg_end_L = atlasLoadTexture( "Jelly_lg_end_L.tga" );
        Jelly_lg_end_R = atlasLoadTexture( "Jelly_lg_end_R.tga" );
        Jelly_sm_end_L = atlasLoadTexture( "Jelly_sm_end_L.tga" );
        Jelly_sm_end_R = atlasLoadTexture( "Jelly_sm_end_R.tga" );
    }

    windowClientSizeThisFrame( &w, &h );
    window_getDims( wdw->id, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor );
    ychild = y;

     z -= 50;

    wdw->open_child = 0;
    for( i = 0; i < wdw->num_children; i++ )
    {
        if( !wdw->children[i].command && wdw->children[i].wdw && window_getMode( wdw->children[i].wdw->id) == WINDOW_DISPLAYING && wdw->children[i].wdw->loc.locked )
        {
            int childYp;

            window_getUpperLeftPos(wdw->children[i].wdw->id, NULL, &childYp);

            wdw->open_child = TRUE;
            wdw->child_id = i;

            htchild = wdw->children[i].wdw->loc.ht*scale;
            if(wdw->below)
                ychild = childYp + wdw->children[i].wdw->loc.ht*scale;
            else
                ychild = childYp - JELLY_HT*scale;
        }
    }

    // if the window is a locked child, do the same as the parent, otherwise check for flippation
    if( wdw->loc.locked )
    {
        wdw->below = wdw->parent->below;
    }
    else if (wdw->forceNoFlip)
    {
        wdw->flip = FALSE;
    }
    else
    {
        // check to see if the window has just crossed the boundary
         if( y < h/8 && y+ht < h-JELLY_HT*scale  )
        {
            if( !wdw->below )
            {
                wdw->below = TRUE;
                wdw->flip = TRUE;
                for( i = 0; i < wdw->num_children; i++ )
                {
                    if( wdw->children[i].wdw && wdw->children[i].wdw->loc.locked )
                        window_setMode( wdw->children[i].idx, WINDOW_SHRINKING );
                }
            }
        }
        else if(!wdw->open_child || y+wdw->loc.ht*scale > 7*h/8)
        {
            if( wdw->below )
            {
                wdw->below = FALSE;
                wdw->flip = TRUE;
                for( i = 0; i < wdw->num_children; i++ )
                {
                    if( wdw->children[i].wdw && wdw->children[i].wdw->loc.locked )
                        window_setMode( wdw->children[i].idx, WINDOW_SHRINKING );
                }
            }
        }

        // handle the flip by ramping it down in opacity then ramping it up in the new location
        if( wdw->flip )
        {
            wdw->opacity -= TIMESTEP*JELLY_FADE_SPEED*3;
            if( wdw->opacity <= 0 )
            {
                wdw->opacity = 0;
                wdw->flip = FALSE;
            }
        }

        // now check left right snap
          if( x + wd/2 < w/2 )
            wdw->left = TRUE;
        else
            wdw->left = FALSE;
    }

    for( i = 0; i < wdw->num_children; i++ )
    {
        int tmp = window_childButton( wdw, 0, z, i, 0 );
        if( tmp > hit )
            hit = tmp;
    }

    if (wdw->useTitle)
    {
        int color, bcolor;
        float tx, ty, tz, wd, ht, scale;

        if (window_getDims( wdw->id, &tx, &ty, &tz, &wd, &ht, &scale, &color, &bcolor ))
        {
            tx += (min(13, wdw->radius) + CHILD_BUTTON_SPACE)*scale;

            if( wdw->parent ) // parent AND child! oy vey!
                tx += 20*wdw->loc.sc;

            if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
                ty -= JELLY_HT*scale/2;
            else
                ty += ht + JELLY_HT*scale/2;

            font( &game_12 );
            font_color( 0xffffffff, 0xffffffff );
            cprntEx( tx, ty, z+1, scale, scale, CENTER_Y, wdw->title );
        }
    }

    //check for grab
      if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
    {
        int tmp;

        // DGNOTE
        // Some day when I get motivated I'll refactor this to have windows that don't want a close button to note that
        // in the wdw structure.  That'll change this huge collection of compares with seemingly random values to something
        // that each window takes responsibility for on its own behalf.
        if( wdw->id != WDW_DIALOG && wdw->id != WDW_DEATH && wdw->id != WDW_SCRIPT_UI && wdw->id != WDW_KARMA_UI &&
            wdw->id != WDW_REWARD_CHOICE && wdw->id != WDW_CHAT_OPTIONS && wdw->id != WDW_ARENA_CREATE &&
            wdw->id != WDW_ARENA_JOIN && wdw->id != WDW_ARENA_OPTIONS && wdw->id != WDW_BASE_PROPS &&
            wdw->id != WDW_BASE_INVENTORY && wdw->id != WDW_BASE_ROOM && wdw->id != WDW_TURNSTILE_DIALOG &&
            !( wdw->id == WDW_PET && amOnPetArenaMap() ) )
        {
            tmp = window_closeButton( wdw, x + wd - wdw->radius*scale, y - JELLY_HT*scale/2, z, scale );
            hit = MAX( hit, tmp );
        }

         if( wdw->maximizeable )
        {
            tmp = window_maximizeButton( wdw, x + wd - wdw->radius*scale - 35*scale, y - JELLY_HT*scale/2, z, scale );
            hit = MAX( hit, tmp );
        }

        if( wdw->parent )
        {
            tmp = window_lockButton( wdw, x + wdw->radius*scale, y - JELLY_HT*scale/2, z, scale );
            hit = MAX( hit, tmp );
        }

        BuildCBox( &box, x, y - JELLY_HT*scale, wd, JELLY_HT*scale );
    }
    else
    {
        int tmp;

        if( wdw->id != WDW_DIALOG && wdw->id != WDW_DEATH && wdw->id != WDW_SCRIPT_UI && wdw->id != WDW_KARMA_UI &&
            wdw->id != WDW_REWARD_CHOICE && wdw->id != WDW_CHAT_OPTIONS && wdw->id != WDW_ARENA_CREATE &&
            wdw->id != WDW_ARENA_JOIN && wdw->id != WDW_ARENA_OPTIONS && wdw->id != WDW_BASE_PROPS &&
            wdw->id != WDW_BASE_INVENTORY && wdw->id != WDW_BASE_ROOM && wdw->id != WDW_TURNSTILE_DIALOG &&
            !( wdw->id == WDW_PET && amOnPetArenaMap() ) )
        {
            tmp = window_closeButton( wdw, x + wd - wdw->radius*scale, y + ht + JELLY_HT*scale/2, z, scale );
            hit = MAX( hit, tmp );
        }

         if( wdw->maximizeable )
        {
            tmp = window_maximizeButton( wdw, x + wd - wdw->radius*scale - 35*scale, y + ht + JELLY_HT*scale/2, z, scale );
            hit = MAX( hit, tmp );
        }

        if( wdw->parent )
        {
            tmp = window_lockButton( wdw, x + wdw->radius, y + ht + JELLY_HT*scale/2, z, scale );
            hit = MAX( hit, tmp );
        }

        BuildCBox( &box, x, y + ht, wd, JELLY_HT*scale );
    }

       if( ( mouseLeftDrag( &box ) || // collision with our jelly
           (!hit && wdw->parent && jelly_assemblyCollision( wdw ) == 3 && mouseDown( MS_LEFT)) ) // collision with extended jelly
         && !wdw->being_dragged && !wdw->drag_mode &&
         !dialogMoveLocked(wdw->id) )
    {
        int winXp, winYp;

        // This one call works for both use cases, because the second is when ancestor == wdw
        window_getUpperLeftPos(ancestor->id, &winXp, &winYp);
        
        if (ancestor != wdw)
        {
            if (!ancestor->being_dragged)
            {
                ancestor->being_dragged = TRUE;
                ancestor->relX = gMouseInpCur.x - winXp;
                ancestor->relY = gMouseInpCur.y - winYp;
                ShowCursor( 0 );
            }
        }
        else
        {
            wdw->being_dragged = TRUE;
            wdw->relX = gMouseInpCur.x - winXp;
            wdw->relY = gMouseInpCur.y - winYp;
            ShowCursor( 0 );
        }
    }

       if( mouseCollision( &box ) && isDown(MS_LEFT) )
         hit = D_MOUSEDOWN;

    if( wdw->being_dragged || (ancestor != wdw && ancestor->being_dragged) )
    {
        hit = D_MOUSEDOWN;

        if( !wdw->flip )
        {
            wdw->opacity += TIMESTEP*JELLY_FADE_SPEED;
            if( wdw->opacity > 1.0 )
                wdw->opacity = 1.0;
        }

        // button was released
        if( !isDown(MS_LEFT) && wdw->being_dragged && !wdw->drag_mode)
        {
            wdw->being_dragged = FALSE;
            window_UpdateServer( wdw->id );
            saveCustomWindows();
            ShowCursor( 1 );
        }
    }
    else if( jelly_assemblyCollision( wdw ) )
    {
         if( !wdw->flip && wdw->opacity < JELLY_MOUSE_OPACITY  )
        {
            wdw->opacity += TIMESTEP*JELLY_FADE_SPEED;
            if( wdw->opacity > JELLY_MOUSE_OPACITY )
                wdw->opacity = JELLY_MOUSE_OPACITY;
        }
        else if( !wdw->flip && wdw->opacity > JELLY_MOUSE_OPACITY  )
        {
            wdw->opacity -= TIMESTEP*JELLY_FADE_SPEED;
            if( wdw->opacity < JELLY_MOUSE_OPACITY )
                wdw->opacity = JELLY_MOUSE_OPACITY;
        }

        if( hit < D_MOUSEOVER )
            hit = D_MOUSEOVER;
    }
    else
    {
        if( !wdw->flip && !wdw->being_dragged )
        {
            if( wdw->opacity > JELLY_MIN_OPACITY )
            {
                wdw->opacity -= TIMESTEP*JELLY_FADE_SPEED;
                if( wdw->opacity < JELLY_MIN_OPACITY )
                    wdw->opacity = JELLY_MIN_OPACITY;
            }
            else if( wdw->opacity < JELLY_MIN_OPACITY )
            {
                wdw->opacity += TIMESTEP*JELLY_FADE_SPEED;
                if( wdw->opacity > JELLY_MIN_OPACITY )
                    wdw->opacity = JELLY_MIN_OPACITY;
            }
        }
    }

    // choose sprites
    if( wdw->radius > R10+PIX3  )
    {
        left  = Jelly_lg_end_L;
        right = Jelly_lg_end_R;
    }
    else
    {
        left  = Jelly_sm_end_L;
        right = Jelly_sm_end_R;
    }

    clr = (window_GetColor(wdw->id) & 0xffffff00) | (int)(255*window_GetOpacity(wdw->id));
    tx = x;

     if( (!wdw->below && !wdw->flip) || (wdw->below && wdw->flip) )
    {
        ty = y - scale*JELLY_HT/2;

        if( !wdw->num_children || !wdw->open_child )
        {
            display_sprite( left,  tx, ty - left->height*scale/2, z, scale, scale, clr  );
            display_sprite( right, tx + wd - scale*right->width, ty - scale*right->height/2, z, scale, scale, clr  );
            display_sprite( meat,  tx + scale*left->width, ty - scale*meat->height/2, z, (wd - scale*(left->width + right->width))/meat->width, scale, clr  );
        }
        else // pain
        {
            Wdw * child = wdw->children[wdw->child_id].wdw;
            AtlasTex *leftEdge, *leftInset, *rightInset;
            int winXp;
            int childXp;

            window_getUpperLeftPos(wdw->id, &winXp, NULL);
            window_getUpperLeftPos(child->id, &childXp, NULL);

            // load up sprites
            if( wdw->radius > R10+PIX3 && child->loc.wd > wdw->loc.wd )
            {
                leftEdge   = atlasLoadTexture( "Jelly_lg_edge_L.tga" );
                leftInset  = atlasLoadTexture( "Jelly_lg_inset_R.tga" );
                rightInset = atlasLoadTexture( "Jelly_lg_inset_L.tga" );

                if( child->radius > R10+PIX3 )
                {
                    left  = Jelly_sm_end_L;
                    right = Jelly_sm_end_R;
                }
                else
                {
                    left  = Jelly_sm_end_L;
                    right = Jelly_sm_end_R;
                }
            }
            else
            {
                if( wdw->radius > R10+PIX3 )
                    leftEdge = atlasLoadTexture( "Jelly_lg_edge_top_L.tga" );
                else
                    leftEdge   = atlasLoadTexture( "Jelly_sm_edge_L.tga" );

                leftInset  = atlasLoadTexture( "Jelly_sm_inset_L.tga" );
                rightInset = atlasLoadTexture( "Jelly_sm_inset_L.tga" );
            }

            // window smaller than parent (chat/tray)
             if( child->loc.wd < wdw->loc.wd )
            {
                if( wdw->left )
                {
                    display_sprite( leftEdge, tx, ty - scale*leftEdge->height/2, z, scale, scale, clr );

                    // center meat between insets and end
                    display_sprite( meat, tx + scale*leftEdge->width, ty - scale*meat->height/2, z,
                                    scale*(child->loc.wd - child->radius - leftEdge->width)/meat->width, scale, clr );

                    // right inset
                    display_sprite( rightInset, childXp + scale*(child->loc.wd - child->radius), y - scale*rightInset->height, z, scale, scale, clr );

                    // right end
                    display_sprite( right, tx + wd - scale*right->width, ty - right->height*scale/2, z, scale, scale, clr  );

                    // right meat between inset and right end
                     tx = childXp + (child->loc.wd - child->radius + rightInset->width)*scale;
                    display_sprite( meat, tx, ty - meat->height*scale/2, z, (x + wd - tx - scale*right->width)/meat->width, scale, clr );
                }
                else
                {
                    AtlasTex *rightEdge;

                    if( wdw->radius > R10+PIX3 )
                        rightEdge = atlasLoadTexture( "Jelly_lg_edge_top_R.tga" );
                    else
                        rightEdge = atlasLoadTexture( "Jelly_sm_edge_R.tga" );

                    //left curve
                      display_sprite( left, tx, ty - scale*left->height/2, z, scale, scale, clr );

                    // center meat between insets and end
                    display_sprite( meat, tx + scale*left->width, ty - scale*meat->height/2, z,
                           scale*(wdw->loc.wd - child->loc.wd - child->radius - right->width)/meat->width, scale, clr );

                    // right inset
                       display_sprite( leftInset, winXp + scale*(wdw->loc.wd - child->loc.wd - child->radius), y - scale*leftInset->height, z, scale, scale, clr );

                    // right end
                     display_sprite( rightEdge, tx + wd - scale*rightEdge->width, ty - rightEdge->height*scale/2, z, scale, scale, clr  );

                     // right meat between inset and right end
                      tx = winXp + scale*(wdw->loc.wd - child->loc.wd - child->radius + leftInset->width);
                      display_sprite( meat, tx, ty - meat->height*scale/2, z, (scale*(wdw->loc.wd - rightEdge->width)-(tx - winXp))/meat->width, scale, clr );
                }
            }
             else if( child->loc.wd-5 > wdw->loc.wd ) // child larger (compass)
            {
                int offset = 0;
                AtlasTex * tmp;
                 tmp   = left;
                 left  = right;
                right = tmp;

                tx = childXp;

                // left end
                 if( childXp == winXp )
                {
                     AtlasTex *rightEdge   = atlasLoadTexture( "Jelly_lg_edge_R.tga" );
                      display_rotated_sprite( rightEdge, tx, ty - rightEdge->height*scale/2, z, scale, scale, clr, PI, 0 );
                    
                    // center meat between insets and end
                     display_rotated_sprite( meat, x + scale*rightEdge->width, ty - meat->height*scale/2, z, (wd - wdw->radius*scale - scale*rightEdge->width)/meat->width, scale, clr, PI, 0 );

                    // right inset
                    display_rotated_sprite( rightInset, x + wd - scale*wdw->radius, ty - scale*JELLY_HT/2, z, scale, scale, clr, PI, 0 );

                    // right end
                    display_rotated_sprite( right, tx + scale*(child->loc.wd - right->width), ty - right->height*scale/2, z, scale, scale, clr, PI, 0 );

                    // right meat between inset and right end
                    tx = x + wd + (rightInset->width - wdw->radius)*scale - offset;
                    display_rotated_sprite( meat, tx, ty - meat->height*scale/2, z,
                        (childXp + child->loc.wd*scale - tx - right->width*scale)/meat->width, scale, clr, PI, 0 );

                }
                else if( ABS( (childXp+child->loc.wd*child->loc.sc)-(winXp+wdw->loc.wd*wdw->loc.sc)) < 1.f )
                {
                    display_rotated_sprite( left, tx, ty - left->height*scale/2, z, scale, scale, clr, PI, 0 );

                    // left meat between inset and end
                    display_rotated_sprite( meat, tx - offset + scale*left->width, ty - meat->height*scale/2, z,
                        (x + offset + (wdw->radius - leftInset->width)*scale - (childXp + scale*left->width))/meat->width, scale, clr, PI, 0 );

                    // left inset
                     display_rotated_sprite( leftInset, x + scale*(wdw->radius - leftInset->width), ty - scale*JELLY_HT/2, z, scale, scale, clr, PI, 0 );

                     // center meat between insets and end
                     display_rotated_sprite( meat, x + scale*wdw->radius, ty - meat->height*scale/2, z, (wd - wdw->radius*scale - leftEdge->width*scale)/meat->width, scale, clr, PI, 0 );

                    // right end
                    display_rotated_sprite( leftEdge, tx + scale*(child->loc.wd - leftEdge->width), ty - leftEdge->height*scale/2, z, scale, scale, clr, PI, 0 );
                }
                else        
                {
                    display_rotated_sprite( left, tx, ty - left->height*scale/2, z, scale, scale, clr, PI, 0 );

                    // left meat between inset and end
                    display_rotated_sprite( meat, tx - offset + scale*left->width, ty - meat->height*scale/2, z,
                         (x + offset + (wdw->radius - leftInset->width)*scale - (childXp + scale*left->width))/meat->width, scale, clr, PI, 0 );

                    // left inset
                    display_rotated_sprite( leftInset, x + scale*(wdw->radius - leftInset->width), ty - scale*JELLY_HT/2, z, scale, scale, clr, PI, 0 );

                    // center meat between insets and end
                    display_rotated_sprite( meat, x + scale*wdw->radius, ty - meat->height*scale/2, z, (wd - 2*wdw->radius*scale)/meat->width, scale, clr, PI, 0 );

                    // right inset
                    display_rotated_sprite( rightInset, x + wd - scale*wdw->radius, ty - scale*JELLY_HT/2, z, scale, scale, clr, PI, 0 );

                    // right end
                    display_rotated_sprite( right, tx + scale*(child->loc.wd - right->width), ty - right->height*scale/2, z, scale, scale, clr, PI, 0 );

                    // right meat between inset and right end
                    tx = x + wd + (rightInset->width - wdw->radius)*scale - offset;
                    display_rotated_sprite( meat, tx, ty - meat->height*scale/2, z,
                        (childXp + child->loc.wd*scale - tx - right->width*scale)/meat->width, scale, clr, PI, 0 );

                }
            }
            else // same size
            {
                AtlasTex * rightEdge;

                if( wdw->radius > R10+PIX3 )
                {
                     rightEdge   = atlasLoadTexture( "Jelly_lg_edge_top_R.tga" );
                    leftEdge    = atlasLoadTexture( "Jelly_lg_edge_top_L.tga" );
                }
                else
                {
                      rightEdge   = atlasLoadTexture( "Jelly_sm_edge_R.tga" );
                    leftEdge    = atlasLoadTexture( "Jelly_sm_edge_L.tga" );
                 }

                display_sprite( leftEdge, tx, ty - scale*leftEdge->height/2, z, scale, scale, clr );

                display_sprite( rightEdge, tx + wd - scale*rightEdge->width, ty - scale*rightEdge->height/2, z, scale, scale, clr );

                // center meat between insets and end
                display_sprite( meat, tx + scale*leftEdge->width, ty - scale*meat->height/2, z,
                                (wd - scale*(rightEdge->width + leftEdge->width))/meat->width, scale, clr );
            }
        }
    }
    else // on bottom
    {
        // sprites get reversed and flipped
          int offset = 0;//!( wdw->radius > R10+PIX3 );

        AtlasTex * tmp;
        tmp   = left;
         left  = right;
        right = tmp;

        ty = y + ht + JELLY_HT*scale/2;

         if( !wdw->num_children || !wdw->open_child )
        {
            display_rotated_sprite( left,  tx, ty - left->height*scale/2, z, scale, scale, clr, PI, 0 );
            display_rotated_sprite( right, tx + wd - scale*right->width, ty - scale*right->height/2, z, scale, scale, clr, PI, 0 );
             display_rotated_sprite( meat,  tx + scale*left->width, ty - scale*meat->height/2, z, (wdw->loc.wd*scale - scale*(left->width + right->width))/meat->width, scale, clr, PI, 0 );
        }
        else // more pain
        {
             Wdw * child = wdw->children[wdw->child_id].wdw;
            AtlasTex *leftEdge, *leftInset, *rightInset;
            int winXp;
            int childXp;

            window_getUpperLeftPos(wdw->id, &winXp, NULL);
            window_getUpperLeftPos(child->id, &childXp, NULL);

            // load up sprites
            if( wdw->radius > R10+PIX3 && child->loc.wd > wdw->loc.wd )
            {
                leftEdge   = atlasLoadTexture( "Jelly_lg_edge_R.tga" );
                leftInset  = atlasLoadTexture( "Jelly_lg_inset_L.tga" );
                rightInset = atlasLoadTexture( "Jelly_lg_inset_R.tga" );

                if( child->radius > R10+PIX3 )
                {
                    left  = atlasLoadTexture( "Jelly_sm_end_L.tga" );
                    right = atlasLoadTexture( "Jelly_sm_end_R.tga" );
                }
                else
                {
                    left  = atlasLoadTexture( "Jelly_sm_end_R.tga" );
                    right = atlasLoadTexture( "Jelly_sm_end_L.tga" );
                }
            }
            else
            {
                if( wdw->radius > R10+PIX3 )
                    leftEdge = atlasLoadTexture( "Jelly_lg_edge_top_R.tga" );
                else
                    leftEdge   = atlasLoadTexture( "Jelly_sm_edge_R.tga" );

                leftInset  = atlasLoadTexture( "Jelly_sm_inset_L.tga" );
                rightInset = atlasLoadTexture( "Jelly_sm_inset_R.tga" );
            }

            // window smaller than parent (chat/tray)
            if( child->loc.wd < wdw->loc.wd )
            {
                if( wdw->left )
                {
                    display_rotated_sprite( leftEdge, tx, ty - scale*leftEdge->height/2, z, scale, scale, clr, PI, 0 );

                    // center meat between insets and end
                    display_rotated_sprite( meat, tx + scale*leftEdge->width - offset, ty - scale*meat->height/2, z,
                         scale*(child->loc.wd - child->radius - leftEdge->width + offset)/meat->width, scale, clr, PI, 0 );

                    // right inset
                    display_rotated_sprite( rightInset, childXp + scale*(child->loc.wd - child->radius), y + ht, z, scale, scale, clr, PI, 0 );

                    // right end
                    display_rotated_sprite( right, tx + wd - scale*right->width, ty - right->height*scale/2, z, scale, scale, clr, PI, 0 );

                    // right meat between inset and right end
                    tx = childXp + (child->loc.wd - child->radius + rightInset->width)*scale;
                    display_rotated_sprite( meat, tx, ty - meat->height*scale/2, z, (x + wd - tx - right->width*scale)/meat->width, scale, clr, PI, 0 );
                }
                else
                {
                    AtlasTex *rightEdge;

                      if( wdw->radius > R10+PIX3 )
                    {
                        rightEdge = atlasLoadTexture( "Jelly_lg_edge_top_L.tga" );

                        // center meat between insets and end
                        display_rotated_sprite( meat, tx + scale*left->width, ty - scale*meat->height/2, z,
                            scale*(wdw->loc.wd - child->loc.wd - child->radius - right->width)/meat->width, scale, clr, PI, 0 );
                    }
                    else
                    {
                        rightEdge = atlasLoadTexture( "Jelly_sm_edge_L.tga" );

                        // center meat between insets and end
                        display_rotated_sprite( meat, tx + scale*left->width, ty - scale*meat->height/2, z,
                              scale*(wdw->loc.wd - child->loc.wd - child->radius - right->width)/meat->width, scale, clr, PI, 0 );
                    }

                    //left curve
                     display_rotated_sprite( left, tx, ty - scale*left->height/2, z, scale, scale, clr, PI, 0 );

                    // right inset
                    display_rotated_sprite( leftInset, winXp + scale*(wdw->loc.wd - child->loc.wd - child->radius), y + ht, z, scale, scale, clr, PI, 0 );

                    // right end
                    display_rotated_sprite( rightEdge, tx + wd - scale*rightEdge->width, ty - rightEdge->height*scale/2, z, scale, scale, clr, PI, 0 );

                    // right meat between inset and right end
                    tx = winXp + scale*(wdw->loc.wd - child->loc.wd - child->radius + leftInset->width);
                     display_rotated_sprite( meat, tx, ty - meat->height*scale/2, z, (scale*(wdw->loc.wd - rightEdge->width)-(tx - winXp))/meat->width, scale, clr, PI, 0 );
                }
            }
            else if( child->loc.wd > wdw->loc.wd ) // child larger (compass)
            {
                AtlasTex * tmp;
                tmp   = left;
                left  = right;
                right = tmp;

                tx = childXp;


                // left end
                if( childXp == winXp )
                {
                      AtlasTex *rightEdge   = atlasLoadTexture( "Jelly_lg_edge_L.tga" );
                    // left end
                      display_sprite( rightEdge, tx, ty - rightEdge->height*scale/2, z, scale, scale, clr );

                    // center meat between insets and end
                    display_sprite( meat, x + scale*rightEdge->width, ty - meat->height*scale/2, z, (wd - (wdw->radius+rightEdge->width)*scale)/meat->width, scale, clr );

                     // right inset
                    display_sprite( rightInset, x + wd - scale*wdw->radius, ty + scale*(JELLY_HT/2-rightInset->height), z, scale, scale, clr );

                    // right end
                    display_sprite( right, tx + scale*(child->loc.wd - right->width), ty - scale*right->height/2, z, scale, scale, clr );

                    // right meat between inset and right end
                    tx = x + wd + (rightInset->width - wdw->radius)*scale;
                    display_sprite( meat, tx, ty - meat->height*scale/2, z,
                        (childXp + child->loc.wd*scale - tx - right->width*scale)/meat->width, scale, clr );

                }
                else if( ABS( (childXp+child->loc.wd*child->loc.sc)-(winXp+wdw->loc.wd*wdw->loc.sc)) < 1.f )
                {
                    // left end
                    display_sprite( left, tx, ty - left->height*scale/2, z, scale, scale, clr );

                    // left meat between inset and end
                    display_sprite( meat, tx + scale*left->width, ty - meat->height*scale/2, z,
                        (x + (wdw->radius - leftInset->width)*scale - (childXp + scale*left->width))/meat->width, scale, clr );

                    // left inset
                    display_sprite( leftInset, x + scale*(wdw->radius - leftInset->width), ty + scale*(JELLY_HT/2-leftInset->height), z, scale, scale, clr );

                    // center meat between insets and end
                       display_sprite( meat, x + scale*wdw->radius, ty - meat->height*scale/2, z, (wd - wdw->radius*scale-leftEdge->width*scale)/meat->width, scale, clr );

                    // right end
                    display_sprite( leftEdge, tx + scale*(child->loc.wd - leftEdge->width), ty - scale*leftEdge->height/2, z, scale, scale, clr );

                }
                else        
                {
                    // left end
                      display_sprite( left, tx, ty - left->height*scale/2, z, scale, scale, clr );

                    // left meat between inset and end
                    display_sprite( meat, tx + scale*left->width, ty - meat->height*scale/2, z,
                        (x + (wdw->radius - leftInset->width)*scale - (childXp + scale*left->width))/meat->width, scale, clr );

                    // left inset
                    display_sprite( leftInset, x + scale*(wdw->radius - leftInset->width), ty + scale*(JELLY_HT/2-leftInset->height), z, scale, scale, clr );

                    // center meat between insets and end
                     display_sprite( meat, x + scale*wdw->radius, ty - meat->height*scale/2, z, (wd - 2*wdw->radius*scale)/meat->width, scale, clr );

                    // right inset
                    display_sprite( rightInset, x + wd - scale*wdw->radius, ty + scale*(JELLY_HT/2-rightInset->height), z, scale, scale, clr );

                    // right end
                    display_sprite( right, tx + scale*(child->loc.wd - right->width), ty - scale*right->height/2, z, scale, scale, clr );

                    // right meat between inset and right end
                    tx = x + wd + (rightInset->width - wdw->radius)*scale;
                    display_sprite( meat, tx, ty - meat->height*scale/2, z,
                         (childXp + child->loc.wd*scale - tx - right->width*scale)/meat->width, scale, clr );
                }
            }
            else // same size
            {
                AtlasTex * rightEdge;

                 if( wdw->radius > R10+PIX3 )
                {
                    rightEdge   = atlasLoadTexture( "Jelly_lg_edge_top_L.tga" );
                    leftEdge    = atlasLoadTexture( "Jelly_lg_edge_top_R.tga" );
                }
                 else
                {
                    rightEdge   = atlasLoadTexture( "Jelly_sm_edge_L.tga" );
                    leftEdge    = atlasLoadTexture( "Jelly_sm_edge_R.tga" );
                }

                display_rotated_sprite( rightEdge, tx + wd - scale*rightEdge->width, ty - scale*rightEdge->height/2, z, scale, scale, clr, PI, 0 );
 
                display_rotated_sprite( leftEdge, tx, ty - scale*leftEdge->height/2, z, scale, scale, clr, PI, 0 );

                // center meat between insets and end
                display_rotated_sprite( meat, tx + scale*leftEdge->width, ty - scale*meat->height/2, z,
                    (wd - scale*(rightEdge->width + leftEdge->width))/meat->width, scale, clr, PI, 0 );
            }
        }
    }

    return hit;

}


static int window_drawStatusJelly()
{
    Wdw * wdw = wdwGetWindow( WDW_STAT_BARS );
    float x, y, z, wd, ht, scale;
    int i, hit = 0, color, bcolor;
    CBox box;
     AtlasTex * jelly = atlasLoadTexture( "Jelly_healthbar.tga" );
    AtlasTex * jelly2 = atlasLoadTexture("Jelly_healthbar_HP_END_Only.tga");
    Entity *e = playerPtr();
    int showRage;
    int leftX, upperY;

    if( !window_getDims( WDW_STAT_BARS, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    window_getUpperLeftPos(WDW_STAT_BARS, &leftX, &upperY);

    if (!e || !e->pchar || !e->pchar->pclass)
        showRage = 0;
    else if (strncmp(e->pchar->pclass->pchName,"Class_Blaster",13) == 0)
        showRage = 0;
    else if (strncmp(e->pchar->pclass->pchName,"Class_Brute",11) == 0)
        showRage = 2;
    else if (strncmp(e->pchar->pclass->pchName,"Class_Dominator",15) == 0)
        showRage = 3;
    else 
        showRage = 0;

    BuildCBox( &box, x, y, jelly->width*scale, jelly->height*scale );

    for( i = 0; i < wdw->num_children; i++ )
     {
        int tmp = window_childButton( wdw, 30, z, i, 0 );
        if( tmp > hit )
            hit = tmp;
    }

     if( mouseLeftDrag( &box ) && !wdw->being_dragged && !wdw->drag_mode )
    {
        wdw->being_dragged = TRUE;
        wdw->relX = gMouseInpCur.x - leftX;
        wdw->relY = gMouseInpCur.y - upperY;
        ShowCursor( 0 );
    }

    if( mouseCollision( &box ) && isDown(MS_LEFT) )
        hit = D_MOUSEDOWN;


    if( wdw->being_dragged )
    {
        hit = D_MOUSEDOWN;

        wdw->opacity += TIMESTEP*JELLY_FADE_SPEED;
        if( wdw->opacity > 1.0 )
            wdw->opacity = 1.0;

        // button was released
        if( !isDown(MS_LEFT) && wdw->being_dragged )
        {
            wdw->being_dragged = FALSE;
            window_UpdateServer( wdw->id );
            ShowCursor( 1 );
        }
    }
    else if( mouseCollision(&box) )
    {
        if( wdw->opacity < JELLY_MOUSE_OPACITY  )
        {
            wdw->opacity += TIMESTEP*JELLY_FADE_SPEED;
            if( wdw->opacity > JELLY_MOUSE_OPACITY )
                wdw->opacity = JELLY_MOUSE_OPACITY;
        }
        else if( wdw->opacity > JELLY_MOUSE_OPACITY  )
        {
            wdw->opacity -= TIMESTEP*JELLY_FADE_SPEED;
            if( wdw->opacity < JELLY_MOUSE_OPACITY )
                wdw->opacity = JELLY_MOUSE_OPACITY;
        }

        if( D_MOUSEOVER > hit )
            hit = D_MOUSEOVER;
    }
    else
    {
        if( !wdw->being_dragged )
        {
            if( wdw->opacity > JELLY_MIN_OPACITY )
            {
                wdw->opacity -= TIMESTEP*JELLY_FADE_SPEED;
                if( wdw->opacity < JELLY_MIN_OPACITY )
                    wdw->opacity = JELLY_MIN_OPACITY;
            }
            else if( wdw->opacity < JELLY_MIN_OPACITY )
            {
                wdw->opacity += TIMESTEP*JELLY_FADE_SPEED;
                if( wdw->opacity > JELLY_MIN_OPACITY )
                    wdw->opacity = JELLY_MIN_OPACITY;
            }
        }
    }

    if (showRage)
    {
        display_sprite( jelly, x, y+1, z, scale, scale, (color & 0xffffff00) | (int)(window_GetOpacity(wdw->id)*255)  );
    }
    else
    {
        display_sprite( jelly2, x, y+1, z, scale, scale, (color & 0xffffff00) | (int)(window_GetOpacity(wdw->id)*255)  );
    }


    return hit;
}

static int window_NoFrameHit( Wdw * wdw )
{
    float x, y, z, wd, ht, scale;
    int i, color, bcolor, hit = 0;
    int bend_units, w, h;
    int leftX, upperY;

    CBox box, box2 = {0};

    windowClientSizeThisFrame( &w, &h );

    if( !window_getDims( wdw->id, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    window_getUpperLeftPos(wdw->id, &leftX, &upperY);

    if( wdw->loc.draggable_frame == TRAY_FIXED_SIZES )
        BuildCBox( &box, x+PIX3*scale, y+PIX3*scale, wd-PIX3*2*scale, ht-PIX3*2*scale );
    else
        BuildCBox( &box, x, y, wd, ht );

     for( i = 0; i < wdw->num_children; i++ )
    {
        int tmp = window_childButton( wdw, 0, z, i, &box );
        if( tmp > hit )
            hit = tmp;
         drawBox(&box, z-1, 0x000000ff, 0x888888ff);
        memcpy( &wdw->cbox, &box, sizeof(CBox));
    }

    
    if( wdw->id >= WDW_TRAY_1 && wdw->id <= WDW_TRAY_8 )
        traySingle_getDims(wdw->id , 0, 0,0, 0,0, &box2 );

    if( (mouseLeftDrag( &box ) || mouseLeftDrag( &box2 ))  && !wdw->being_dragged && !wdw->drag_mode )
    {
        wdw->being_dragged = TRUE;
        wdw->relX = gMouseInpCur.x - leftX;
        wdw->relY = gMouseInpCur.y - upperY;
        ShowCursor( 0 );
    }

    if( (mouseCollision( &box ) || mouseCollision( &box2 )) && isDown(MS_LEFT) )
        hit = D_MOUSEDOWN;

    if( wdw->being_dragged )
    {
        hit = D_MOUSEDOWN;

        // button was released
        if( !isDown(MS_LEFT) && wdw->being_dragged )
        {
            // special clamp for tray windows which were allowed out of bounds
            if( wdw->id >= WDW_TRAY_1 && wdw->id <= WDW_TRAY_8 )
            {
                wdw->being_dragged = FALSE;
            
                x = leftX;
                y = upperY;

                if( x < 0 ) 
                {
                    bend_units = MIN(10,(int)ceilf(ABS(x)/(40.f*scale)));
                    window_setUpperLeftPos(wdw->id, -bend_units*40*scale, -1);
                }
                else if ( x > w-wd )
                {
                    bend_units = MIN(10,(int)ceilf(ABS(x-(w-wd))/(40.f*scale)));
                    window_setUpperLeftPos(wdw->id, w - wdw->loc.wd*scale + bend_units*40*scale, -1);
                }
                else if( y < 0 )
                {
                    bend_units = MIN(10,(int)ceilf(ABS(y)/(40.f*scale)));
                    window_setUpperLeftPos(wdw->id, -1, -bend_units*40*scale);
                }
                else if ( y > h-ht )
                {
                    bend_units = MIN(10,(int)ceilf(ABS(y-(h-ht))/(40.f*scale)));
                    window_setUpperLeftPos(wdw->id, -1, h - wdw->loc.ht*scale + bend_units*40*scale);
                }
            }

            wdw->being_dragged = FALSE;
            window_UpdateServer( wdw->id );
            ShowCursor( 1 );
        }
    }
    else if( mouseCollision(&box)||mouseCollision(&box2) )
    {
        if( D_MOUSEOVER > hit )
            hit = D_MOUSEOVER;
    }

    return hit;
}

//
//
static float window_calcScale( int wdw )
{
    float   scale;

    switch( winDefs[wdw].loc.mode )
    {
    case WINDOW_SHRINKING:
        scale = 1.f - ( winDefs[wdw].timer / ( float )WINDOW_SHRINK_TIME );
        break;
    case WINDOW_GROWING:
        scale = winDefs[wdw].timer / ( float )WINDOW_GROW_TIME;
        break;
    case WINDOW_DOCKED:
        scale = 0.f;
        break;
    case WINDOW_DISPLAYING:
    case WINDOW_UNINITIALIZED:
    default:
        scale = 1.f;
        break;
    }

    return scale*winDefs[wdw].loc.sc;
}

//recursive
static int getChildMouseCollision(Wdw * wdw)
{
    int i;
    for( i = 0; i < wdw->num_children; i++ )
    {
        if( wdw->children[i].wdw &&
            wdw->children[i].wdw->loc.mode == WINDOW_DISPLAYING && 
            wdw->children[i].wdw->loc.locked)
        {
            if (mouseCollision( &wdw->children[i].wdw->cbox ) )
            {
                return true;
            }
            else
            {
                return getChildMouseCollision(wdw->children[i].wdw);  //recursive call to other children
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------------------
// The once mighty Display Window ////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------
//
//
int displayWindow( int i )
{
    float x, y, z, wd, ht, opacity;
    int color, bcolor, big_hit, jellyHit = 0;

     Wdw *wdw = &winDefs[i];

    float scale = window_calcScale( i );

    // Snap Child if it is locked
     if( wdw->parent && wdw->loc.locked )
    {
        int parentYp;

        window_getUpperLeftPos(wdw->parent->id, NULL, &parentYp);

        if( (wdw->parent->below && wdw->parent->flip) || (!wdw->parent->below && !wdw->parent->flip) )
            window_setUpperLeftPos(wdw->id, -1, parentYp - (wdw->loc.ht + JELLY_HT)*scale); //above
        else
            window_setUpperLeftPos(wdw->id, -1, parentYp + (wdw->parent->loc.ht + JELLY_HT)*scale); //below

        // snap to parent window width
        if( wdw->loc.locked && wdw->parent && 
             ((wdw->loc.wd > (wdw->parent->loc.wd - 44*wdw->loc.sc ) && wdw->loc.wd < wdw->parent->loc.wd) || 
            (wdw->loc.wd < (wdw->parent->loc.wd + 44*wdw->loc.sc ) && wdw->loc.wd > wdw->parent->loc.wd)) ) // forty is for curvy jelly at end
        {
            wdw->loc.wd = wdw->parent->loc.wd;
        }
    }

    // get the current window position
    window_getDims( i, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor );
    BuildCBox( &wdw->cbox, x, y, wd, ht );

    // Allow the options menu to be dragged in the front end
    if( window_getMode( wdw->id ) == WINDOW_DISPLAYING && (wdw->id == WDW_OPTIONS || (wdw->id != WDW_STAT_BARS && !shell_menu())) )
    {
        if( wdw->noFrame || wdw->id >= MAX_WINDOW_COUNT && customWindowIsOpen( wdw->id )<=0 )
            jellyHit = window_NoFrameHit( wdw );
        else
            jellyHit = window_drawJelly( wdw );
    }

     if(  wdw->id == WDW_STAT_BARS && !baseedit_Mode() )
        jellyHit = window_drawStatusJelly();

     if( wdw->loc.draggable_frame && !wdw->drag_mode && !jellyHit )
    {
        int mx = gMouseInpCur.x;
        int my = gMouseInpCur.y;
        if (isLetterBoxMenu())
        {
            reversePointToScreenScalingFactori(&mx, &my);
        }
         window_checkFrameCollision(i, mx, my);
    }

    if( wdw->id > 0 && !wdw->loc.locked )
    {
        Wdw *fadewdw;
         int fadeup;
        int temp = collisions_off_for_rest_of_frame;    // hack to allow fading out even if some UI element has turned off collisions.
         collisions_off_for_rest_of_frame = 0;            //        This is was initially used because holding mouse over a Tab would still dim the enclosing chat window, but seems to be a good idea overall. 
                                                        //        On the negative side, this will cause overlapped windows to fade-in when the mouse is over them.
        fadeup = (chat_mode()  && ( wdw->id == WDW_CHAT_BOX     || (wdw->id >= WDW_CHAT_CHILD_1 && wdw->id <= WDW_CHAT_CHILD_4)))  || mouseCollision( &wdw->cbox ) || jellyHit; 

        // check open children
        fadeup = getChildMouseCollision(wdw);

        collisions_off_for_rest_of_frame = temp;

        fadewdw = getOldestAttachedAncestor(wdw);

        if( window_Isfading(fadewdw) )
        {
            opacity = (color & 0xff);
            color &= 0xffffff00;

             if( fadeup )
            {
                opacity += TIMESTEP*40;
                if( opacity > 255 )
                    opacity = 255;

                fadewdw->fadeTimer = 30;
            }
            else
            {
                if( fadewdw->fadeTimer <= 0 )
                {
                    opacity -= TIMESTEP*5;
                    if( opacity < MIN_OPACITY )
                        opacity = MIN_OPACITY;
                }
                else
                    fadewdw->fadeTimer -= TIMESTEP;
            }

               color |= (int)opacity;
               winDefs[fadewdw->id].loc.back_color = (int)(((float)opacity/255.f)*winDefs[0].loc.back_color);

            window_SetColor( fadewdw->id, color );
        }
    }

    // super hack
       if( wdw->id == WDW_MAP )
        BuildCBox( &wdw->cbox, x, y, wd + 5*wdw->loc.sc, ht );

    big_hit = mouseDownHit( &wdw->cbox, MS_LEFT );

     if( big_hit || jellyHit >= D_MOUSEDOWN)
    {
        window_bringToFront(i);
    }

    if( mouseCollision( &wdw->cbox ) || jellyHit >= D_MOUSEOVER )
        return TRUE;
    else
        return FALSE;
}

//-----------------------------------------------------------------------------------------------
// SERVE WINDOW HELPER FUNCTIONS ////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
//
//
void window_bringToFront( int i )
{
    int j;

    if( winDefs[i].loc.locked && winDefs[i].parent )
        i = winDefs[i].parent->id;

     for( j = 0; j<MAX_WINDOW_COUNT+custom_window_count; j++ )            //All windows that were higher than this one will slide back one slot
        if( winDefs[j].window_depth > winDefs[i].window_depth )    //thus retaining their current order with respect to each other.
            winDefs[j].window_depth--;

    winDefs[i].window_depth = (MAX_WINDOW_COUNT+custom_window_count)-1;    //put this window in top of all.
}

int window_isFront( int i )
{
    return (winDefs[i].window_depth == ((MAX_WINDOW_COUNT + custom_window_count)-1));
}
//
//
static void moveDraggedWindows()
{
    int i;

    for(i=0;i<(MAX_WINDOW_COUNT+custom_window_count);i++ )
    {
        if( winDefs[i].being_dragged )
        {
            extern int mouse_dx, mouse_dy;
            int wd, ht;
            
            if( winDefs[i].loc.mode == WINDOW_DOCKED )
            {
                winDefs[i].being_dragged = false;
                ShowCursor( 1 );
                continue;
            }

            windowClientSizeThisFrame( &wd, &ht );

            if( gWindowDragging == 2 || gMouseInpCur.x < 0 || gMouseInpCur.y < 0 || gMouseInpCur.x > wd-2 || gMouseInpCur.y > ht-2 )
            {
                WdwBase *loc = &winDefs[i].loc;
                gWindowDragging = 2;
                loc->xp += mouse_dx;
                loc->yp += mouse_dy;
            }
            else
            {
                gWindowDragging = TRUE;
                window_setUpperLeftPos(i, gMouseInpCur.x - winDefs[i].relX, gMouseInpCur.y - winDefs[i].relY);
            }

            window_alignChildren( i );
            return;
        }
    }

    gWindowDragging = FALSE;
}

static void insert( int ele, int len, int *ele_list, int *idx_list )
{
    int i, j;

    for(i=0;i<len;i++)
    {
        if(ele_list[i] < ele )
        {
            for( j=len-1; j>=i; j--)    //found smaller, make room for new element.
            {
                ele_list[j+1] = ele_list[j];
                idx_list[j+1] = idx_list[j];
            }
            ele_list[i] = ele;
            idx_list[i] = len;
            return;
        }
    }

    ele_list[len] = ele;
    idx_list[len] = len;
}


bool window_collision(void)
{
    int i, colls;
    colls = collisions_off_for_rest_of_frame;
    collisions_off_for_rest_of_frame = false;

    for(i=0; i<MAX_WINDOW_COUNT+custom_window_count; i++ )
    {
        Wdw * win = &winDefs[i];
        float x, y, wd, ht, sc;

        if (win->editor && !editMode()) 
            continue;
        if (!win->editor && editMode()) 
            continue;

        if( window_getDims( i, &x, &y, NULL, &wd, &ht, &sc, NULL, NULL) )
        {
            CBox box;

            if( win->below )
                BuildCBox( &box, x, y, wd, ht+JELLY_HT*sc );
            else
                BuildCBox( &box, x, y - JELLY_HT*sc, wd, ht+JELLY_HT*sc );
            
            if( mouseCollision(&box))
            {
                collisions_off_for_rest_of_frame = colls;
                return true;
            }
        }
    }

    collisions_off_for_rest_of_frame = colls;
    return false;
}

static void startWindowTimer(int i)
{
    static struct {
        char* name;
        #if USE_NEW_TIMING_CODE
            PerfInfoStaticData* piStatic;
        #else
            PerformanceInfo* piStatic;
        #endif
    }* windowTimers;
    
    const char* name = windows_GetNameByNumber(i);
    if(!windowTimers)
        windowTimers = calloc(MAX_WINDOW_COUNT+custom_window_count, sizeof(*windowTimers));
    if(!name)
    {
        if(!windowTimers[i].name)
        {
            char buffer[30];
            sprintf(buffer, "unknown:%d", i);
            windowTimers[i].name = strdup(buffer);
        }
        name = windowTimers[i].name;
    }
    PERFINFO_AUTO_START_STATIC(name, &windowTimers[i].piStatic, 1);
}

static void stopWindowTimer(int i)
{
}

int idx_list[MAX_WINDOW_COUNT+MAX_CUSTOM_WINDOW_COUNT];

int window_IsBlockingCollison( int wdw )
{
    // see if any window in front of this one will block this mouse coord
    int i = 0;
     while( idx_list[i] != wdw && i < MAX_WINDOW_COUNT+MAX_CUSTOM_WINDOW_COUNT)
    {
        int wdx = idx_list[i];
        Wdw * win = &winDefs[wdx];
        float x, y, wd, ht, sc;

        if( window_getDims( wdx, &x, &y, NULL, &wd, &ht, &sc, NULL, NULL) )
        {
            CBox box;
            if( win->below )
                BuildCBox( &box, x, y, wd, ht+JELLY_HT*sc );
            else
                BuildCBox( &box, x, y - JELLY_HT*sc, wd, ht+JELLY_HT*sc );

            if( mouseCollision(&box))
                return 1;
        }

        i++;
    }

    return 0;
}

//recursive
static void displayWindowChildren(Wdw * parent, int * collision, int debugLine)
{
    int k;

    for( k = 0; k < parent->num_children; k++ )
    {
        int displayChildChildren = 0;
        if( parent->children[k].wdw )
        {
            if(parent->children[k].wdw->loc.locked > 2)
                parent->children[k].wdw->loc.locked = 1;

            if( parent->children[k].wdw->loc.locked == 1 && window_getMode( parent->children[k].wdw->id ) != WINDOW_DOCKED && 
                window_getMode( parent->children[k].wdw->id ) != WINDOW_UNINITIALIZED  )
            {
                // hack for multiple chat windows... (MJP-- this will be fixed...)
                gCurrentWindefIndex = parent->children[k].wdw->id;

                PERFINFO_RUN(
                    PERFINFO_AUTO_START("displayChild1", 1);
                startWindowTimer(gCurrentWindefIndex);
                )

                parent->children[k].wdw->code();
                *collision |= displayWindow( parent->children[k].wdw->id );

                if( *collision && game_state.showMouseCoords )
                {
                    xyprintf( 10, debugLine, "Mouse on Child Window: %i", parent->children[k].wdw->id  );
                }
                PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();

                displayChildChildren = 1;
            }
            else if ( parent->children[k].wdw->loc.locked == 2 )
            {
                int childID = parent->children[k].wdw->id;

                PERFINFO_RUN(
                    PERFINFO_AUTO_START("displayChild2", 1);
                startWindowTimer(childID);
                )

                gCurrentWindefIndex = parent->children[k].wdw->id;
                parent->children[k].wdw->code();
                parent->children[k].wdw->loc.locked = 1;
                window_UpdateServer(parent->id);

                PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();

                displayChildChildren = 1;
            }

            if (displayChildChildren)
            {
                displayWindowChildren( parent->children[k].wdw, collision, ++debugLine );  //recursive call to other children
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------
// SERVE WINDOW (Main Window Loop) //////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------
//
//


void serveWindows()
{
    int j, i, zlist[MAX_WINDOW_COUNT+MAX_CUSTOM_WINDOW_COUNT];
    const int stackDepthAtEntry = timing_state.autoStackDepth;
    int winFirst, winLast;
    bool anyDisplaying = false;
    bool forceModal = false;

     if (!initWindows())
    {
        return;
    }

    gWaitingToEnterGame = 0;

    moveDraggedWindows();

    if (isMenu(MENU_GAME))
    {
        winFirst = 0;
        winLast = MAX_WINDOW_COUNT+custom_window_count-1;

        //Build z sorted list.
        for(i=winFirst; i<=winLast; i++ )
            insert( winDefs[i].window_depth, i, (int*)&zlist, (int*)&idx_list );

        if (!playerPtr())
            return;
    }
    else if (isMenu(MENU_LOGIN))
    {
        // For the login screen, only allow the options menu to work
        winFirst = 0;
        winLast = 0;
        idx_list[0] = WDW_OPTIONS;
        forceModal = true;
    }
    else
    {
        winFirst = 0;
        winLast = -1;
    }

    for(j=winFirst; j<=winLast; j++ )
    {
        i = idx_list[j];
        window_Resize( i );
    }

    for(j=winFirst; j<=winLast; j++ )
    {
        int sc = 0;
        i = idx_list[j];

         if( i == WDW_DIALOG )
            continue;

        if (winDefs[i].editor && !editMode()) 
            continue;
        if (!winDefs[i].editor && editMode()) 
            continue;

        // hack for multiple chat windows... (MJP, this will be fixed...)
        gCurrentWindefIndex = i;

        if(stackDepthAtEntry != timing_state.autoStackDepth)
        {
            const int idx = idx_list[j - 1];
            Errorf(    "Window #%d, <%s>, introduced an autoTimer start/stop mismatch (depth now=%d, @ entry=%d)", idx, winDefs[idx].title, timing_state.autoStackDepth, stackDepthAtEntry);
        }


        PERFINFO_RUN(startWindowTimer(i);)

        if( winDefs[i].parent == NULL )
        {
            winDefs[i].loc.locked = 0;
        }

         if( !winDefs[i].loc.locked )
        {
             if( !winDefs[i].code )
             {
                 PERFINFO_AUTO_STOP();
                continue;
            }
            if (winDefs[i].code())
            {
                 PERFINFO_AUTO_STOP();
                return;
            }
        }

        switch( winDefs[i].loc.mode )
        {
            case WINDOW_SHRINKING:
            {
                PERFINFO_AUTO_START("shrinking", 1);
                displayWindow( i );
                if( winDefs[i].timer < WINDOW_SHRINK_TIME )
                {
                    winDefs[i].timer += TIMESTEP;

                    if( winDefs[i].timer >= WINDOW_SHRINK_TIME )
                        winDefs[i].timer = WINDOW_SHRINK_TIME;
                }
                else
                    window_setMode( i, WINDOW_DOCKED );
                PERFINFO_AUTO_STOP();
            }break;

            case WINDOW_GROWING:
            {
                PERFINFO_AUTO_START("growing", 1);
                displayWindow( i );
                if( winDefs[ i ].timer < WINDOW_GROW_TIME )
                {
                    winDefs[i].timer += TIMESTEP;

                    if( winDefs[i].timer >= WINDOW_GROW_TIME )
                        winDefs[i].timer = WINDOW_GROW_TIME;
                }
                else
                {
                    window_setMode( i, WINDOW_DISPLAYING );
                    window_bringToFront( i );
                }
                PERFINFO_AUTO_STOP();
            }break;

            case WINDOW_DISPLAYING:
            {
                 if( !winDefs[i].loc.locked )
                {
                    int collision;

                    PERFINFO_AUTO_START("displaying", 1);

                    collision = displayWindow( i );

                    if( collision  && game_state.showMouseCoords)
                            xyprintf( 10, 12, "Mouse on Window: %i", i );

                    displayWindowChildren( &winDefs[i], &collision, 13 );

                    if( collision )
                    {
                        collisions_off_for_rest_of_frame = TRUE;
                    }

                    anyDisplaying = true;

                    PERFINFO_AUTO_STOP();
                }
            }break;

            case WINDOW_DOCKED: 
                if( i >= MAX_WINDOW_COUNT )
                    displayWindow(i); // This is sweet hack, custom windows don't really close all the way
                break;
            case WINDOW_UNINITIALIZED:
            default:
                break;
        }
        
        PERFINFO_AUTO_STOP(); // Matches the PERFINFO_AUTO_START_STATIC above that calls windows_GetNameByNumber.
    }

    // In the login menu, make the options menu acts as modal dialogs
    if (forceModal && anyDisplaying)
    {
        collisions_off_for_rest_of_frame = TRUE;
    }

    return;
}

void window_closeAlways(void)
{
    int i;

    for( i = 0; i < MAX_WINDOW_COUNT+custom_window_count; i++ )
    {
        if( winDefs[i].loc.start_shrunk == ALWAYS_CLOSED )
            window_setMode( i, WINDOW_DOCKED );
    }
}



