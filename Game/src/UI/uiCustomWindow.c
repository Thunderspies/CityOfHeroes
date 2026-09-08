
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include "gameComm/wdwbase.h"
#include "win/win_init.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/FolderCache.h>
#include "cmdparse/cmdgame.h"

#include "UI/uiUtil.h"
#include "UI/uiUtilGame.h"
#include "UI/uiWindows.h"
#include "uiWindows.h"
#include "UI/uiWindows_init.h"
#include "UI/uiClipper.h"
#include "UI/uiBox.h"
#include "UI/uiContextMenu.h"
#include "UI/uiDialog.h"
#include "UI/uiInput.h"
#include "UI/uiCursor.h"
#include "UI/uiTray.h"
#include "gameComm/trayCommon.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/ttFontUtil.h"
#include "graphics/textureatlas.h"
#include "UI/uiScrollBar.h"

typedef struct CustomWindowItem
{
    char * pchName;
    char * pchCommand;
} CustomWindowItem;

typedef struct CustomWindow
{
    char * pchName;
    char * pchFileName;
    F32 x;
    F32 y;
    F32 wd;
    F32 ht;
    CustomWindowItem **ppItem;
    int id;
    int opened;
    ScrollBar sb;
} CustomWindow;

typedef struct CustomWindowList
{
    CustomWindow **ppWindow;
} CustomWindowList;

TokenizerParseInfo ParseCustomWindowButton[] =
{
    { "Name", TOK_STRUCTPARAM |TOK_STRING(CustomWindowItem, pchName, 0) },
    { "Command", TOK_STRUCTPARAM |TOK_STRING(CustomWindowItem, pchCommand, 0) },
    { "\n", TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCustomWindowText[] =
{
    { "Name", TOK_STRUCTPARAM |TOK_STRING(CustomWindowItem, pchName, 0) },
    { "\n", TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCustomWindow[] =
{
    { "Name", TOK_STRUCTPARAM |TOK_STRING(CustomWindow,pchName, 0)     },
    { "X", TOK_STRUCTPARAM |TOK_F32(CustomWindow, x, 0)     },
    { "Y", TOK_STRUCTPARAM |TOK_F32(CustomWindow, y, 0)     },
    { "WD", TOK_STRUCTPARAM |TOK_F32(CustomWindow, wd, 0)     },
    { "HT", TOK_STRUCTPARAM |TOK_F32(CustomWindow, ht, 0)     },
    { "Button", TOK_STRUCT(CustomWindow, ppItem, ParseCustomWindowButton) },
    { "TextDivider", TOK_REDUNDANTNAME|TOK_STRUCT(CustomWindow, ppItem, ParseCustomWindowText) },
    { "Open", TOK_INT(CustomWindow, opened, 1) },
    { "Filename", TOK_CURRENTFILE(CustomWindow, pchFileName) },
    { "End", TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCustomWindowList[] =
{
    { "Window", TOK_STRUCT(CustomWindowList, ppWindow, ParseCustomWindow) },
    { "", 0, 0 }
};

CustomWindowList s_CustomWindows = {0};

CustomWindow * customWindowGet( int id )
{
    int i;
    for( i = eaSize(&s_CustomWindows.ppWindow)-1; i>=0; i-- )
    {
        if( s_CustomWindows.ppWindow[i]->id == id )
            return s_CustomWindows.ppWindow[i];
    }

    return 0;
}

int customWindowIsOpen( int id )
{
    CustomWindow * pCW = customWindowGet(id);
    if(pCW)
        return pCW->opened;

    return -1;
}

void customWindowToggle( char * pchName )
{
    int i;
    for( i = eaSize(&s_CustomWindows.ppWindow)-1; i>=0; i-- )
    {
        if( stricmp( pchName, s_CustomWindows.ppWindow[i]->pchName) == 0 )
            s_CustomWindows.ppWindow[i]->opened = !s_CustomWindows.ppWindow[i]->opened;
    }
}

static void cleanUpCustomWindows()
{
    int i;
    for( i = MAX_WINDOW_COUNT; i < MAX_WINDOW_COUNT+MAX_CUSTOM_WINDOW_COUNT; i++ )
    {
        Wdw *wdw = wdwGetWindow(i);
        wdw->id = 0;
        wdw->code = 0;
    }
    custom_window_count = 0;
    StructClear(ParseCustomWindowList,  &s_CustomWindows);
}

void saveCustomWindows(void)
{
    int i,j;
    CustomWindowList **fileList=0; 

      for( i = 0; i < eaSize(&s_CustomWindows.ppWindow); i++ )
    {
        int w, h, found = 0;
        int id = s_CustomWindows.ppWindow[i]->id;
        int x, y;
        
        windowClientSize(&w, &h);

        window_getUpperLeftPos(id, &x, &y);

        s_CustomWindows.ppWindow[i]->x = (F32)x / (F32)w;
        s_CustomWindows.ppWindow[i]->y = (F32)y / (F32)h;
        s_CustomWindows.ppWindow[i]->wd = winDefs[id].loc.wd;
        s_CustomWindows.ppWindow[i]->ht = winDefs[id].loc.ht;
        
        for( j = 0; j < eaSize(&fileList); j++ )
        {
            if( stricmp( fileList[j]->ppWindow[0]->pchFileName, s_CustomWindows.ppWindow[j]->pchFileName) == 0 )
            {
                eaPush( &fileList[j]->ppWindow, s_CustomWindows.ppWindow[i] );
                found = 1;
                continue;
            }
        }

        if(!found)
        {
            CustomWindowList *pCW = calloc(1,sizeof(ParseCustomWindowList));
            eaPush( &fileList, pCW );
            eaPush( &pCW->ppWindow, s_CustomWindows.ppWindow[i] );
        }
    }

    for( i = eaSize(&fileList)-1; i>=0; i-- )
    {
        CustomWindowList *pCW = fileList[i];
        mkdir("C:/game/data/customwindows");
        ParserWriteTextFile(pCW->ppWindow[0]->pchFileName, ParseCustomWindowList, pCW, 0, 0);
        eaDestroy(&pCW->ppWindow);
        eaRemove(&fileList, i);
        SAFE_FREE(pCW);
    }
    eaDestroy(&fileList);
}

int customWindow(void);

static void createWdwFromDefs()
{
    int i;
    for( i = 0; i < eaSize(&s_CustomWindows.ppWindow) && i < MAX_CUSTOM_WINDOW_COUNT; i++ )
    {
        CustomWindow *pCW = s_CustomWindows.ppWindow[i];
        Wdw *wdw =  wdwCreate(MAX_WINDOW_COUNT+i,1);
        int w, h;
        char buf[2000];
        windowClientSize( &w, &h );

        wdw->code = customWindow;

        window_setUpperLeftPos(pCW->id, pCW->x * w, pCW->y * h);
        wdw->loc.wd = MAX(30, pCW->wd);
        wdw->loc.ht = MAX(30, pCW->ht);
        wdw->loc.draggable_frame = RESIZABLE;

        wdw->max_wd = 10000; 
        wdw->min_wd = 30; 
        wdw->max_ht = 10000; 
        wdw->min_ht = 30; 

        wdw->loc.start_shrunk = DEFAULT_OPEN;
        wdw->radius = R10 + PIX3;

        wdw->loc.color = 0xffffffff;
        wdw->loc.back_color = 0x00000088;
        wdw->loc.sc = 1.f;
        wdw->loc.mode = WINDOW_DISPLAYING;
        pCW->sb.wdw = wdw->id = pCW->id = MAX_WINDOW_COUNT+i;

        memcpy( &wdw->loc_minimized, &wdw->loc, sizeof(WdwBase) );

        sprintf(buf, "customwindowtoggle %s", pCW->pchName );
         window_initChild( wdw, 0, 0, pCW->pchName, buf );
        wdw->num_children = 1;
        custom_window_count++;
    }
}



static void reloadCustomWindowsCallback(const char *relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);

    cleanUpCustomWindows();
    // Don't free data, we leave it around (leak like crazy)
    memset(&s_CustomWindows, 0, sizeof(s_CustomWindows));
    if (!ParserLoadFiles("customwindows", ".window", 0, 0, ParseCustomWindowList,  &s_CustomWindows, NULL, NULL, NULL))
    {
        cleanUpCustomWindows();
        return;
    }
    createWdwFromDefs();
}

static ContextMenu * s_CustomButtonContext = 0;

static void customButtoncallback_setName(CustomWindowItem * pDialogCWB)
{
    if( pDialogCWB )
    {
        StructFreeString(pDialogCWB->pchName);
        pDialogCWB->pchName = StructAllocString(dialogGetTextEntry());
        saveCustomWindows();
    }
}

static void customButtoncallback_setCommand(CustomWindowItem * pDialogCWB)
{
    if( pDialogCWB )
    {
        StructFreeString(pDialogCWB->pchCommand);
        pDialogCWB->pchCommand = StructAllocString(dialogGetTextEntry());
        saveCustomWindows();
    }
}

static void customButtoncallback_setNameAdapter(void * arg0)
{
    customButtoncallback_setName((CustomWindowItem *)arg0);
}

void customButtonCM_editName( CustomWindowItem * pCWB )
{
    dialogSetTextEntry(pCWB->pchName);
    dialog(DIALOG_OK_CANCEL_TEXT_ENTRY, -1, -1, -1, -1,    "Edit Name", NULL, customButtoncallback_setNameAdapter, NULL, NULL, DLGFLAG_GAME_ONLY|DLGFLAG_NO_TRANSLATE, NULL, NULL, 0, 0, 256, pCWB );
    
}

static void customButtoncallback_setCommandAdapter(void * arg0)
{
    customButtoncallback_setCommand((CustomWindowItem *)arg0);
}

void customButtonCM_editCommand( CustomWindowItem * pCWB )
{
    dialogSetTextEntry(pCWB->pchCommand);
    dialog(DIALOG_OK_CANCEL_TEXT_ENTRY, -1, -1, -1, -1,    "Edit Command", NULL, customButtoncallback_setCommandAdapter, NULL, NULL, DLGFLAG_GAME_ONLY|DLGFLAG_NO_TRANSLATE, NULL, NULL, 0, 0, 256, pCWB );
}

void customButtonCM_Remove( CustomWindowItem * pCWB )
{
    // no back pointers yet, ha
    int i, j;
    for( i = eaSize(&s_CustomWindows.ppWindow)-1; i>=0; i-- )
    {
        for( j = eaSize(&s_CustomWindows.ppWindow[i]->ppItem); j>=0; j--)
        {
            if( pCWB == s_CustomWindows.ppWindow[i]->ppItem[j] )
            {    
                CustomWindowItem *pDel = eaRemove(&s_CustomWindows.ppWindow[i]->ppItem, j);
                StructFree(pDel);
                saveCustomWindows();
            }
        }
    }
}

static void customButtonCM_editNameAdapter(void* arg0)
{
    customButtonCM_editName((CustomWindowItem *)arg0);
}

static void customButtonCM_editCommandAdapter(void* arg0)
{
    customButtonCM_editCommand((CustomWindowItem *)arg0);
}

static void customButtonCM_RemoveAdapter(void* arg0)
{
    customButtonCM_Remove((CustomWindowItem *)arg0);
}

void loadCustomWindows()
{
    cleanUpCustomWindows();

    if (!ParserLoadFiles("customwindows", ".window", 0, 0, ParseCustomWindowList,  &s_CustomWindows, NULL, NULL, NULL))
    {
        cleanUpCustomWindows();
        return;
    }

    if(!s_CustomButtonContext)
    {
        s_CustomButtonContext = contextMenu_Create(0);
        contextMenu_addCode(s_CustomButtonContext, alwaysAvailable, 0, customButtonCM_editNameAdapter, 0, "Edit &Name", 0 );
        contextMenu_addCode(s_CustomButtonContext, alwaysAvailable, 0, customButtonCM_editCommandAdapter, 0, "Edit &Command", 0 );
        contextMenu_addCode(s_CustomButtonContext, alwaysAvailable, 0, customButtonCM_RemoveAdapter, 0, "Delete", 0 );
    }

    createWdwFromDefs();
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "*.window", reloadCustomWindowsCallback);
}

void insertCustomWindowButton(CustomWindow *pCW, char *pchName, char *pchCommand, void * current_element )
{
    CustomWindowItem *pCB = StructAllocRaw(sizeof(CustomWindowItem));
    pCB->pchName = pchName?StructAllocString(pchName):StructAllocString("Edit Me");
    pCB->pchCommand = pchCommand?StructAllocString(pchCommand):StructAllocString("nop");

    if( current_element )
    {    
        int idx = eaFind(&pCW->ppItem, current_element);
        eaInsert(&pCW->ppItem, pCB, idx);
        saveCustomWindows();
        return;
    }
    
    eaPush(&pCW->ppItem, pCB);
    saveCustomWindows();
}


void addCustomWindowButton(CustomWindow *pCW, char *pchName, char *pchCommand)
{
    insertCustomWindowButton( pCW, pchName, pchCommand, 0 );
}



void createCustomWindow( char * pchName )
{
    CustomWindow *pCW = StructAllocRaw(sizeof(CustomWindow));
    pCW->x = .5;
    pCW->y = .5;
    pCW->wd = 200;
    pCW->ht = 100;
    pCW->pchName = StructAllocString(pchName);
    pCW->pchFileName = StructAllocString("customwindows/custom.window");
    eaPush(&s_CustomWindows.ppWindow, pCW);
    createWdwFromDefs();
}

void addCustomWindowButtonToAll(char *pchName, char *pchCommand)
{
    int i;

    if(!custom_window_count)
        createCustomWindow( "Custom Window" );

    for( i = 0; i < custom_window_count; i++ )
    {
        CustomWindow *pCW = customWindowGet(MAX_WINDOW_COUNT+i);
        addCustomWindowButton(pCW, pchName, pchCommand);
    }
}

static void customWindowDrag(CustomWindowItem *pCWI)
{
    TrayObj to;
    buildMacroTrayObj( &to, pCWI->pchCommand, pCWI->pchName, 0, 0);
    trayobj_startDragging( &to, atlasLoadTexture("Macro_Button_01"), 0);
}

int customWindow()
{
    F32 x, y, z, wd, ht, sc, cx, cy, twd, tht = 0;
    int i, color, bcolor;
    UIBox box;
    CBox cbox;
    CustomWindow *pCW;

    pCW = customWindowGet(gCurrentWindefIndex);

    if( !pCW || !window_getDims( gCurrentWindefIndex, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor ) )
        return 0;

    if( !pCW->opened )
        return 0;

    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );

    uiBoxDefine(&box, x+PIX3*sc, y+PIX3*sc, wd - PIX3*sc*2, ht - PIX3*sc*2);
    clipperPush(&box);

     cx = x + wd/2;
     cy = y - pCW->sb.offset + 30*sc/2;
     twd = wd-3*R10*sc;

      for( i = 0; i < eaSize(&pCW->ppItem); i++ )
    {
        CustomWindowItem *pCB = pCW->ppItem[i];
        
        if( pCB->pchCommand )
        {
             if( drawStdButton( cx, cy, z, twd, 20*sc, color, pCB->pchName, 1.f, 0 ) == D_MOUSEHIT )
                cmdParsef( pCB->pchCommand );
        }
        else
        {
             font(&game_12);
            font_color(CLR_WHITE, CLR_WHITE);
            cprntEx( cx, cy, z, sc, sc, CENTER_X|CENTER_Y, pCB->pchName );
        }

          BuildCBox(&cbox, x + R10*sc, cy - 12*sc, wd - 3*R10*sc, 24*sc );
        if( mouseClickHit(&cbox, MS_RIGHT ) )
            contextMenu_displayEx(s_CustomButtonContext, pCB);
        if( mouseLeftDrag(&cbox) )
        {
            customWindowDrag(pCB);
            eaRemove(&pCW->ppItem, i);
        }

        tht += 24*sc;
        cy += 24*sc;

        // accept macro drags
        if( cursor.dragging && mouseCollision(&cbox) && !isDown(MS_LEFT) )
        {
            TrayObj * to = &cursor.drag_obj;
            if( to->type == kTrayItemType_Macro )
            {
                insertCustomWindowButton( pCW, to->shortName, to->command, pCB );
                trayobj_stopDragging();
            }
        }
    }

    // new button
       if( drawStdButton( cx, cy, z, twd, 20*sc, color, "New Command", 1.f, 0 ) == D_MOUSEHIT )
        addCustomWindowButton( pCW, 0, 0);

    tht += 24*sc;

    // accept macro drags
      BuildCBox(&cbox, x, y, wd, ht);
    
     if( cursor.dragging && mouseCollision(&cbox) && !isDown(MS_LEFT) )
    {
        TrayObj * to = &cursor.drag_obj;
        if( to->type == kTrayItemType_Macro )
        {
            addCustomWindowButton( pCW, to->shortName, to->command);
            trayobj_stopDragging();
        }
    }

    clipperPop(&box);
      doScrollBar( &pCW->sb, ht - 2*R10*sc, tht, x + wd, R10, z, 0, &box ); 
    
    return 0;
}

