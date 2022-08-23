
#include "gameComm/wdwbase.h"

#include "UI/uiGame.h"
#include "UI/uiUtil.h"
#include "UI/uiChat.h"
#include "UI/uiInput.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiContextMenu.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"

#include "win/input.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/mathutil.h>
#include "win/win_init.h"
#include "UI/uiKeybind.h"
#include "player/player.h"
#include "language/langClientUtil.h"
#include "graphics/ttFontUtil.h"
#include <utilitieslib/utils/StringUtil.h>
#include <utilitieslib/language/MessageStoreUtil.h>
//----------------------------------------------------------------------------------------

// these enums are only used internally, but declared in header for structure definition
typedef enum ContextElementType
{
    CM_CODE,        // this is the most verstile type, but take lots of parameters, only type that can have sub-menus
    CM_TITLE,       // this will simply display un-clickable text
    CM_TEXT,        // this will display un-clickable text that can vary
    CM_DIVIDER,     // This will add a space (to group like elements)
    CM_CHECKMARK,   // special check mark button, cause shannon wanted it
} ContextElementType;

typedef struct ContextMenuElement
{
    void    *visData;       // function pointers and data pointers
    int     (*visState)(void*);

    void    *codeData;
    void    (*code)(void*);

    void    *textData;
    const char    *(*textCode)(void*);

    void    *transData;
    int        (*translate)(void*);

    int        no_translate;

    ContextElementType type;    // type of Element

    AtlasTex *icon;                // Icon to display

    char *txt;                  // display text
    ContextMenu * cm;           // pointer to sub menu

} ContextMenuElement;

MP_DEFINE(ContextMenuElement);

typedef struct ContextMenu
{

    int                 (*stopDisplay)(); // catch all function to close menu
    int                 visible;          // used for subMenus
    int                 dontClose;
    int                    useCustomColors;
    int                    customColors[2];
    int                    disableScreenScaling;

    float               x, y, wd, ht;     // dimensions

    void                *data;            // place to give data to context menu when called

    KeyBindProfile      *kbp;             // keybind profile (contextMenus hijack all keyboard input until they are gone)
    ContextMenuElement  **elements;       // elements in context menu

    CBox                box;              // handy

} ContextMenu;

MP_DEFINE(ContextMenu);

static ContextMenu * gCurrentContextMenu = 0;  // the current menu we are displaying

#define ELEMENT_HT      20      // Height per element
#define ELEMENT_SPACE   4       // space between elements
#define WD_SPACER       20      // space padding on horizontal edges
#define CONTEXT_Z       (MAX_WINDOW_COUNT+MAX_CUSTOM_WINDOW_COUNT)*100 + 1000    // z value, 1000 should be infront of most things

MP_DEFINE(KeyBindProfile);

static int context_z = CONTEXT_Z;

static int displayContextMenu( ContextMenu *cm );
//----------------------------------------------------------------------------------------

// default function pointer for things that are always true
int alwaysAvailable( void *foo )
{
    return CM_AVAILABLE;
}

int neverAvailable( void *foo )
{
    return 0;
}

int alwaysVisible( void *foo )
{
    return CM_VISIBLE;
}

// allocates context menu, returns pointer
//
ContextMenu *contextMenu_Create( int (*destroyConditions)() )
{
    ContextMenu * cm;
    MP_CREATE(ContextMenu, 64);
    cm = MP_ALLOC(ContextMenu);
    cm->stopDisplay = destroyConditions;

    return cm;
}

// adds an element to contextMenu
//
static void contextMenu_addElement( ContextMenu * cm, ContextMenuElement * cme )
{
    if( !cm || !cme )
        return;

    if( !cm->elements )
        eaCreate( &cm->elements );

    eaPush( &cm->elements, cme );
}

static void contextMenu_setElement(ContextMenuElement * cme, ContextElementType type, int (*visState)(void*), void* visData, void (*code)(void*),
                                   void *codeData, const char *(*textCode)(void*), void* textData, const char * txt, ContextMenu * sub, int (*translate)(void*), void* transData, AtlasTex * icon )
{
    if( visState )
        cme->visState = visState;
    else
        cme->visState = alwaysAvailable; // use default

    if( visData )
        cme->visData = visData;
    cme->code = code;

    if( codeData )
        cme->codeData = codeData;

    cme->textCode = textCode;

    if( textData )
        cme->textData = textData;

    if( translate )
        cme->translate = translate;

    if( transData )
        cme->transData = transData;

    if( icon )
        cme->icon = icon;

    // translate and copy text
    if( txt )
    {
        cme->txt = strdup(textStd(txt));
    }

    cme->cm = sub;
    cme->type = type;
}

// generic create element function used internally
//
static ContextMenuElement * contextMenu_createElement( ContextMenu * cm, ContextElementType type, int (*visState)(void*), void* visData, void (*code)(void*),
                                                       void *codeData, const char *(*textCode)(void*), void* textData, const char * txt, ContextMenu * sub, int (*translate)(void*), void* transData, AtlasTex *icon  )
{
    ContextMenuElement * cme;
    MP_CREATE(ContextMenuElement, 512);
    cme = MP_ALLOC(ContextMenuElement);
    contextMenu_setElement( cme, type, visState, visData, code, codeData, textCode, textData, txt, sub, translate, transData, icon );
    return cme;
}

//---------------------------------------------------------------------------------------------------------
// these fabulous functions are for external use //////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------
void contextMenu_addDividerVisible( ContextMenu *cm, int(*visible)(void*) )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_DIVIDER, visible, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) );
}

void contextMenu_addDivider( ContextMenu *cm )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_DIVIDER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) );
}

void contextMenu_addTitle( ContextMenu *cm, const char * txt )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TITLE, 0, 0, 0, 0, 0, 0, txt, 0, 0, 0, 0) );
}

void contextMenu_addVariableText( ContextMenu *cm, const char *(*textCode)(void*), void *textData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TEXT, 0, 0, 0, 0, textCode, textData, 0, 0, 0, 0, 0 ) );
}

void contextMenu_addVariableTextVisible( ContextMenu *cm, int(*visible)(void*), void *visibleData, const char *(*textCode)(void*), void *textData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TEXT, visible, visibleData, 0, 0, textCode, textData, 0, 0, 0, 0, 0 ) );
}

void contextMenu_addVariableTextTrans( ContextMenu *cm, const char *(*textCode)(void*), void *textData, int(*translate)(void*), void *translateData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TEXT, 0, 0, 0, 0, textCode, textData, 0, 0, translate, translateData, 0 ) );
}

void contextMenu_addVariableTitle( ContextMenu *cm, const char *(*textCode)(void*), void *textData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TITLE, 0, 0, 0, 0, textCode, textData, 0, 0, 0, 0, 0 ) );
}

void contextMenu_addVariableTitleVisible( ContextMenu *cm, int(*visible)(void*), void *visibleData, const char *(*textCode)(void*), void *textData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TITLE, visible, visibleData, 0, 0, textCode, textData, 0, 0, 0, 0, 0 ) );
}

void contextMenu_addVariableTitleVisibleNoTrans( ContextMenu *cm, int(*visible)(void*), void *visibleData, const char *(*textCode)(void*), void *textData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TITLE, visible, visibleData, 0, 0, textCode, textData, 0, 0, neverAvailable, 0, 0 ) );
}

void contextMenu_addVariableTitleTrans( ContextMenu *cm, const char *(*textCode)(void*), void *textData, int(*translate)(void*), void *translateData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_TITLE, 0, 0, 0, 0, textCode, textData, 0, 0, translate, translateData, 0 ) );
}

void contextMenu_addCheckBox( ContextMenu *cm, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, const char *txt )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_CHECKMARK, visible, visData, code, codeData, 0, 0, txt, 0, 0, 0, 0) );
}

void contextMenu_addIconCheckBox( ContextMenu *cm, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, const char *txt, AtlasTex * icon )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_CHECKMARK, visible, visData, code, codeData, 0, 0, txt, 0, 0, 0, icon) );
}

void contextMenu_addVariableTextCheckBox( ContextMenu *cm, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, const char *(*textCode)(void*), void *textData )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_CHECKMARK, visible, visData, code, codeData, textCode, textData, 0, 0, 0, 0, 0) );
}

void contextMenu_addCode( ContextMenu *cm, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, const char *txt, ContextMenu *sub  )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_CODE, visible, visData, code, codeData, 0, 0, txt, sub, 0, 0, 0 ) );
}

void contextMenu_addIconCode( ContextMenu *cm, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, const char *txt, ContextMenu *sub,AtlasTex * icon)
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_CODE, visible, visData, code, codeData, 0, 0, txt, sub, 0, 0, icon ) );
}

void contextMenu_addVariableTextCode( ContextMenu *cm, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, const char *(*textCode)(void*), void *textData, ContextMenu *sub  )
{
    contextMenu_addElement( cm, contextMenu_createElement( cm, CM_CODE, visible, visData, code, codeData, textCode, textData, 0, sub, 0, 0, 0) );
}


void contextMenu_setCode( ContextMenu* cm, int(*visible)(void*), void *visData, void (*code)(void*), void *codeData, const char *txt, ContextMenu *sub  )
{
    ContextMenuElement* cme;
    int i;

    for(i = eaSize(&cm->elements)-1; i >= 0; i--)
    {
        cme = cm->elements[i];
        if(cme->code == code)
        {
            contextMenu_setElement(cme, cme->type, visible, visData, code, codeData, NULL, NULL, txt, sub, 0, 0, 0);
            return;
        }
    }

    contextMenu_addCode(cm, visible, visData, code, codeData, txt, sub);
}
//------------------------------------------------------------------------------------------------------------
// Functions to make menu go away ////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------
//
//
void contextMenu_closeSubs( ContextMenu * cm )
{
    int i;

    if( !cm )
        return;

    for( i = eaSize(&cm->elements)-1; i >= 0; i-- )
    {
        if( cm->elements[i]->cm )
        {
            if( cm->elements[i]->cm->visible )
                unbindKeyProfile( cm->elements[i]->cm->kbp );
            cm->elements[i]->cm->visible = 0;
            contextMenu_closeSubs( cm->elements[i]->cm );
        }
    }
}

//
//
void contextMenu_closeAll()
{
    if( gCurrentContextMenu && !gCurrentContextMenu->dontClose )
    {
        if( gCurrentContextMenu )
            unbindKeyProfile(gCurrentContextMenu->kbp);    // release hold on keybindings

        contextMenu_closeSubs( gCurrentContextMenu );  // shut down sub menus
        gCurrentContextMenu = 0;                       // set current to null
    }
}


//------------------------------------------------------------------------------------------------------------
// Internal initializations //////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------------------------------------
//
//
static void findBindKey( char *txt, char *key ) // finds the special keybind key
{
    int     i;

    for( i = 0; i < (int)strlen(txt); i++ )
    {
        if ( txt[i] == '&' )
        {
            strncpy( key, &txt[i+1], 1 );
        }
    }
}

static void contextMenu_findHt( ContextMenu * cm )
{
    int i;
    int iSize = eaSize( &cm->elements );
    float count = 0;
    float wd = 0;

     for( i = 0; i < iSize; i++ )
    {
        if( cm->elements[i]->visData )
        {
            if( cm->elements[i]->visState( cm->elements[i]->visData ) >= CM_VISIBLE )
            {
                if ( cm->elements[i]->type == CM_DIVIDER )
                    count += .5f;
                else
                    count += 1.f;
            }
        }
        else
        {
            if( cm->elements[i]->visState( cm->data ) >= CM_VISIBLE )
            {
                if ( cm->elements[i]->type == CM_DIVIDER )
                    count += .5f;
                else
                    count += 1.f;
            }
        }
    }

    cm->ht = ELEMENT_HT * count + ELEMENT_SPACE;
    BuildCBox( &cm->box, cm->x, cm->y, cm->wd, cm->ht );
}


// Initializes variables for a given context menu
//
static void contextMenu_init( ContextMenu *cm, float x, float y, CMAlign alignHoriz, CMAlign alignVert)
{
    int i,j, wdwd, wdht;
    float wd = 0;

    // set up our keybind profile
    if ( !cm->kbp )
    {
        MP_CREATE(KeyBindProfile, 8);
        cm->kbp = MP_ALLOC(KeyBindProfile);
    } 
    else
    {
        for( i = 0; i < BIND_MAX; i++ )
        {
            for( j = 0; j < MOD_TOTAL; j++ ) {
                if (cm->kbp->binds[i].command[j])
                {
                    free(cm->kbp->binds[i].command[j]);
                }
            }
        }
    }

    chat_exit();

    bindKeyProfile(cm->kbp);
    memset(cm->kbp, 0, sizeof(KeyBindProfile));
    cm->kbp->parser = cmdGameParse;
    cm->kbp->allOtherKeyCallback = contextMenu_closeAll;

    //for every element
    for( i = eaSize( &cm->elements )-1; i >= 0; i-- )
    {
        bool visible = 0;

        // set default callbacks (just in case)
        if( !cm->elements[i]->visState )
            cm->elements[i]->visState = alwaysAvailable;

         if( cm->elements[i]->textCode )
        {
            const char *buffer;

            // translate?
            if( cm->elements[i]->translate )
            {
                if( cm->elements[i]->transData )
                    cm->elements[i]->no_translate = !(cm->elements[i]->translate( cm->elements[i]->transData ));
                else
                    cm->elements[i]->no_translate = !(cm->elements[i]->translate( cm->data ));
            }
            else
                cm->elements[i]->no_translate = 0;

            // this made messy by optional translation
            if( cm->elements[i]->textData )
            {
                if( cm->elements[i]->textData )
                {
                    if( cm->elements[i]->no_translate )
                        buffer = cm->elements[i]->textCode(cm->elements[i]->textData);
                    else
                        buffer = textStd( cm->elements[i]->textCode(cm->elements[i]->textData) );
                }
                else
                {
                    if( cm->elements[i]->no_translate )
                        buffer = cm->elements[i]->textCode(cm->data);
                    else
                        buffer = textStd( cm->elements[i]->textCode(cm->data) );
                }
            }
            else
            {
                if( cm->elements[i]->no_translate )
                    buffer = cm->elements[i]->textCode(cm->data);
                else
                    buffer = textStd( cm->elements[i]->textCode(cm->data) );
            }

            if(cm->elements[i]->txt)
            {
                free(cm->elements[i]->txt);
                cm->elements[i]->txt = 0;
            }
            if( buffer )
            {
                cm->elements[i]->txt = malloc( sizeof(char)*(strlen(buffer)+1) );
                strcpy( cm->elements[i]->txt, buffer );
            }
        }

         if( cm->elements[i]->visData )
        {
            if( cm->elements[i]->visState( cm->elements[i]->visData ) >= CM_VISIBLE )
                visible = true;
        }
        else
        {
            if( cm->elements[i]->visState( cm->data ) >= CM_VISIBLE )
                visible = true;
        }

        // find width and hotkey
        if( cm->elements[i]->txt && visible )
        {
            char key[2] = {0};
            int tmp_wd;

            // update width tracker
            if( cm->elements[i]->no_translate )
            {
                if( cm->elements[i]->type == CM_TITLE )
                    tmp_wd = str_wd_notranslate( &game_14, 1.f, 1.f, cm->elements[i]->txt  );
                else
                    tmp_wd = str_wd_notranslate( &game_12, 1.f, 1.f, cm->elements[i]->txt  );
            }
            else
            {
                if( cm->elements[i]->type == CM_TITLE )
                    tmp_wd = str_wd( &game_14, 1.f, 1.f, cm->elements[i]->txt  );
                else
                    tmp_wd = str_wd( &game_12, 1.f, 1.f, cm->elements[i]->txt  );
            }

            if( cm->elements[i]->icon )
                tmp_wd += ELEMENT_HT+ELEMENT_SPACE;

            wd = MAX(tmp_wd,wd);

            // find a bind key
            findBindKey( cm->elements[i]->txt, key );

            if( key[0] != '\0' && (cm->elements[i]->visData?(cm->elements[i]->visState( cm->elements[i]->visData ) >= CM_AVAILABLE):(cm->elements[i]->visState( cm->data ) >= CM_AVAILABLE)) )
            {
                char buf[256];

                sprintf( buf, "ContextMenu %i", i ); // console command to execute context menu item
                bindKey( key, buf, 0 );
            }
        }
    }

    // set dimensions
    contextMenu_findHt( cm );

    cm->wd = wd + WD_SPACER*2;
    cm->x = MAX(x,0);
    cm->y = MAX(y,0);

    // By default it's CM_TOP
    if(alignVert == CM_BOTTOM)
    {
        cm->y = cm->y-cm->ht;
    }
    else if(alignVert == CM_CENTER)
    {
        cm->y = cm->y-cm->ht/2;
    }

    // By default it's CM_LEFT
    if(alignHoriz == CM_RIGHT)
    {
        cm->x = cm->x-cm->wd;
    }
    else if(alignHoriz == CM_CENTER)
    {
        cm->x = cm->x-cm->wd/2;
    }

    cm->x = MAX(cm->x, 0);
    cm->y = MAX(cm->y, 0);

    //bounds check
    windowClientSizeThisFrame( &wdwd, &wdht );
    if( cm->y + cm->ht > wdht )
        cm->y = wdht - cm->ht;

    if( cm->x + cm->wd > wdwd )
        cm->x = wdwd - cm->wd;

    BuildCBox( &cm->box, cm->x, cm->y, cm->wd, cm->ht + ELEMENT_SPACE );
    contextMenu_closeSubs( cm );
}

// sets the current context menu
//
void contextMenu_setAlign( ContextMenu * cm, float x, float y, CMAlign alignHoriz, CMAlign alignVert, void *data )
{
    if(!cm)
        return;

    cm->data = data;

    if( gCurrentContextMenu != cm ) // figure out where it goes and how big it is
    {
        if( gCurrentContextMenu )
            unbindKeyProfile( gCurrentContextMenu->kbp );

        gCurrentContextMenu = cm;
        contextMenu_init( gCurrentContextMenu, x, y, alignHoriz, alignVert );
        gCurrentContextMenu->dontClose = 2;
    }
}

void contextMenu_display(ContextMenu * cm)
{
    int x, y;
    inpMousePos( &x, &y );
    contextMenu_set(cm, x, y, NULL);
}

void contextMenu_displayEx(ContextMenu * cm, void * data )
{
    int x, y;
    inpMousePos( &x, &y );
    contextMenu_set(cm, x, y, data);
}

// sets the current context menu
//
void contextMenu_set( ContextMenu * cm, float x, float y, void *data )
{
    contextMenu_setAlign(cm, x, y, CM_LEFT, CM_TOP, data);
}

// for recusrive calls into sub-menus
static void contextMenu_activateSubElement( ContextMenu *cm, int i )
{
    int j;

    if (!cm)
        return;
    if (!EAINRANGE(i, cm->elements))
        return;

    if( cm->elements[i] )
    {
        for( j = eaSize( &cm->elements)-1; j >= 0; j-- )
        {
            if( cm->elements[j]->cm && cm->elements[j]->cm->visible )
            {
                contextMenu_activateSubElement( cm->elements[j]->cm, i );
                return;
            }
        }
    }

    if( cm->elements[i]->code )
    {
        if( cm->elements[i]->codeData )
            cm->elements[i]->code(cm->elements[i]->codeData);
        else
            cm->elements[i]->code(gCurrentContextMenu->data);

        contextMenu_closeAll();
    }

    if( cm->elements[i]->cm )
    {
        contextMenu_init( cm->elements[i]->cm , cm->x + cm->wd, cm->y + i*(ELEMENT_HT+ELEMENT_SPACE), CM_LEFT, CM_TOP );
        cm->elements[i]->cm->visible = 1;
        collisions_off_for_rest_of_frame = 1;
        gCurrentContextMenu->dontClose = 2;
    }

}

// special doohickey for cmdParse to execute a contextMenu element
//
void contextMenu_activateElement( int i )
{
    int j;

    if( gCurrentContextMenu && EAINRANGE(i, gCurrentContextMenu->elements))
    {
        for( j = eaSize( &gCurrentContextMenu->elements)-1; j >= 0; j-- )
        {
            if( gCurrentContextMenu->elements[j]->cm && gCurrentContextMenu->elements[j]->cm->visible )
            {
                contextMenu_activateSubElement( gCurrentContextMenu->elements[j]->cm, i );
                return;
            }
        }

        if( gCurrentContextMenu->elements[i]->cm )
        {
            contextMenu_init( gCurrentContextMenu->elements[i]->cm , gCurrentContextMenu->x + gCurrentContextMenu->wd, gCurrentContextMenu->y + i*(ELEMENT_HT+ELEMENT_SPACE), CM_LEFT, CM_TOP );
            gCurrentContextMenu->elements[i]->cm->visible = 1;
            collisions_off_for_rest_of_frame = 1;
            gCurrentContextMenu->dontClose = 2;
        }
        else if( gCurrentContextMenu->elements[i]->code )
        {
            if( gCurrentContextMenu->elements[i]->codeData )
                gCurrentContextMenu->elements[i]->code(gCurrentContextMenu->elements[i]->codeData);
            else
                gCurrentContextMenu->elements[i]->code(gCurrentContextMenu->data);

            contextMenu_closeAll();
        }
    }
}

//---------------------------------------------------------------------------------------------

// strips out ampersand and displays hotkey differently
static void displayContextText( float x, float y, char * txt, int available )
{
    int i;
    float xScale, yScale;
    int flag = 0;

    calc_scrn_scaling(&xScale, &yScale);
    font_color(  0xffffffff, 0x00ffffff );

    for( i = 0; i < (int)strlen(txt); i++ )
    {
        if( txt[i] == '&' )
        {
            char *firstPart, *hotkey, *secondPart;
            int wd1 = 0, wd2 = 0;

            estrCreate(&firstPart);
            estrCreate(&hotkey);
            estrCreate(&secondPart);

            if( i > 0 )
                estrConcatFixedWidth(&firstPart, txt, i);

            font( &game_12 );

            estrConcatChar(&hotkey, txt[i+1]);
            estrConcatCharString(&secondPart, &txt[i+2]);

              wd1 = str_layoutWidth( &game_12, 1.f, 1.f, firstPart )/xScale;

            font_bold( 1 );
             wd2 = str_layoutWidth( &game_12, 1.f, 1.f, hotkey )/xScale;
            font_bold( 0 );

            if(!available)
            {
                font_color( CLR_OFFLINE_ITEM, CLR_OFFLINE_ITEM);
                wd2 = str_layoutWidth( &game_12, 1.f, 1.f, hotkey )/xScale;
                prnt( x, y, context_z+2, 1.f, 1.f, firstPart );
                prnt( x+wd1+wd2, y, context_z+2, 1.f, 1.f, secondPart );
                prnt( x+wd1, y, context_z+2, 1.f, 1.f, hotkey );
            }
            else
            {
                prnt( x, y, context_z+2, 1.f, 1.f, firstPart );
                prnt( x+wd1+wd2, y, context_z+2, 1.f, 1.f, secondPart );

                font_bold( 1 );
                font_color( CLR_WHITE, CLR_WHITE );
                prnt( x+wd1, y, context_z+2, 1.f, 1.f, hotkey );
                font_bold( 0 );
            }

            estrDestroy(&firstPart);
            estrDestroy(&hotkey);
            estrDestroy(&secondPart);

            return;
        }
    }

    // no hotkey so just print text like normal
    if(!available)
        font_color( CLR_OFFLINE_ITEM, CLR_OFFLINE_ITEM);
    cprntEx( x, y, context_z+2, 1.f, 1.f, NO_MSPRINT, txt );

}


//
//
static int displayContextElement( ContextMenu *cm, int i, float y )
{
    ContextMenuElement * element = cm->elements[i];
    CBox box;
     int retval = 0, color = 0xaaaaaa44;

    float x = cm->x + ELEMENT_SPACE;
    float icon_space = 0;

    CMVisType state;

    if( element->visData )
        state = element->visState(element->visData);
    else
        state = element->visState(cm->data);

    if( state == CM_HIDE || element->type == CM_DIVIDER )
        return 0;

    BuildCBox( &box, x, y, cm->wd - ELEMENT_SPACE*2, ELEMENT_HT - ELEMENT_SPACE );
    font( &game_12 );

    // execute submenu if its open
    if( element->cm && element->cm->visible )
    {
         element->cm->data = cm->data;
        context_z += 10;
        retval = displayContextMenu( element->cm );
        context_z -= 10;
    }

    if( state >= CM_AVAILABLE )
    {
        //key element highlighted if it has an open submenu
        if( element->cm && element->cm->visible )
            drawFlatFrame( PIX3, R6, box.lx, box.ly, context_z, box.hx-box.lx, box.hy-box.ly, 1.f, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );

        if( mouseCollision(&box) )
        {
            // close other submenus mouse is over a different one
            if( !element->cm )
                contextMenu_closeSubs( cm );
            else if ( element->cm && !element->cm->visible )
            {
                contextMenu_closeSubs( cm );
                contextMenu_init( element->cm, cm->x + cm->wd, y, CM_LEFT, CM_TOP );
                element->cm->visible = 1;
                collisions_off_for_rest_of_frame = 1;
            }

            // highlight
            if(element->type != CM_TITLE && element->type != CM_TEXT)
            {
                if( isDown( MS_LEFT ) || isDown( MS_RIGHT ) )
                    drawFlatFrame( PIX3, R6, box.lx, box.ly, context_z+1, box.hx-box.lx, box.hy-box.ly, 1.f, CLR_SELECTION_FOREGROUND, CLR_SELECTION_BACKGROUND );
                else
                    drawFlatFrame( PIX3, R6, box.lx, box.ly, context_z+1, box.hx-box.lx, box.hy-box.ly, 1.f, CLR_MOUSEOVER_FOREGROUND, CLR_MOUSEOVER_BACKGROUND );
            }

            // or execute code and close if clicked
            if( mouseClickHit( &box, MS_LEFT) && ( element->code ) )
            {
                if( element->codeData )
                    element->code(element->codeData);
                else
                    element->code(cm->data);

                contextMenu_closeAll();
                collisions_off_for_rest_of_frame = 1;
            }
        }
    }

    // if its a title change font color and don't draw box
    if( element->type == CM_TITLE || element->type == CM_TEXT )
    {
        if( element->type == CM_TITLE )
            font( &game_14 );
        font_color( CLR_WHITE, CLR_WHITE );
    }

    // special sprite for check bars
    if( element->type == CM_CHECKMARK  )
    {
        AtlasTex * mark;

        if( state == CM_CHECKED || state == CM_CHECKED_UNAVAILABLE )
        {
            mark = atlasLoadTexture( "Context_checkbox_filled.tga" );
            display_sprite( mark, x, y, context_z + 3, 1.f, 1.f, (winDefs[0].loc.color & 0xffffff00)|0xff  );

            mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
            display_sprite( mark, x, y, context_z + 2, 1.f, 1.f, (winDefs[0].loc.color & 0xffffff00)|0xff );
        }
        else
        {
            mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
            display_sprite( mark, x, y, context_z + 2, 1.f, 1.f, winDefs[0].loc.color & 0xffffffff );
        }
    }

    if( element->icon )
    {
        float icon_sc = ((float)ELEMENT_HT-ELEMENT_SPACE)/(MAX(element->icon->width,element->icon->height));
         icon_space = ELEMENT_HT+ELEMENT_SPACE;
        display_sprite( element->icon, cm->x + WD_SPACER + (icon_space - element->icon->width*icon_sc)/2, 
                        y + (ELEMENT_HT-ELEMENT_SPACE-element->icon->height*icon_sc)/2, context_z+2, 
                        icon_sc, icon_sc, CLR_WHITE );
    }

    // title just gets printed, other wise display text crazylike
    if( element->type == CM_TITLE || element->type == CM_TEXT )
    {
        if( element->no_translate )
            cprntEx( cm->x + cm->wd/2, y + ELEMENT_HT - 2*ELEMENT_SPACE, context_z+2, 1.f, 1.f, CENTER_X|NO_MSPRINT, "%s", element->txt );
        else
            cprnt( cm->x + cm->wd/2, y + ELEMENT_HT - 2*ELEMENT_SPACE, context_z+2, 1.f, 1.f, "%s", element->txt );
    }
    else
    {
        if( !element->no_translate )
        {
            if( !element->txt )
            {
                contextMenu_closeAll();
                return 0;
            }

            if( stricmp(element->txt, textStd(element->txt)) != 0 )
            {
                char * tmp = strdup(textStd(element->txt));
                free(element->txt);
                element->txt = tmp;
            }
        }
        displayContextText( cm->x + WD_SPACER + icon_space, y + ELEMENT_HT - 2*ELEMENT_SPACE, element->txt, (state >= CM_AVAILABLE) );
    }
    // special sprite for check bars
    if( element->cm )
    {
        AtlasTex * mark = atlasLoadTexture( "Chat_separator_arrow_up.tga" );
        display_rotated_sprite( mark, cm->x + cm->wd - mark->width - 2, y + ELEMENT_HT/2 - mark->height/2 - 1, context_z + 2, 1.0f, 1.0f, CLR_WHITE, PI/2, 0 );
    }

    return retval;
}

//
//
static int displayContextMenu( ContextMenu *cm )
{
    int i, ret = 0, count = 0, hit = 0, color, bcolor;
    int iSize;
    float y;

    if( !cm ) // nevermind
        return 0;

    if( cm->dontClose > 0 )
        cm->dontClose--;

    // check catch-all function
    if( cm->stopDisplay && !cm->stopDisplay() )
        contextMenu_closeAll();

    // have to check ht every frame in case a visible callback changes
    contextMenu_findHt( cm );

    if (cm->ht == ELEMENT_SPACE) // this means no elements are visible
        return 0;

    y = cm->y + ELEMENT_SPACE;
    if (cm->useCustomColors)
    {
        color = cm->customColors[0];
        bcolor = cm->customColors[1];
    }
    else
    {
        color = (winDefs[0].loc.color & 0xffffff00)|0x66;
        bcolor = winDefs[0].loc.back_color;
    }
    drawFrame( PIX2, R10, cm->box.lx, cm->box.ly, context_z, (cm->box.hx-cm->box.lx), (cm->box.hy-cm->box.ly), 1.f, color, bcolor ); // big box around elements

    iSize = eaSize(&cm->elements); 
    for( i = 0; i < iSize; i++ )
    {
        int visible = 0;

        if( cm->elements[i]->visData )
            visible = cm->elements[i]->visState(cm->elements[i]->visData);
        else
            visible = cm->elements[i]->visState(cm->data);

        if( visible )
        {
            if( cm->elements[i]->type == CM_DIVIDER ) // divders just add a space
            {
                AtlasTex * left = atlasLoadTexture( "Headerbar_HorizDivider_L.tga" );
                AtlasTex * right = atlasLoadTexture( "Headerbar_HorizDivider_R.tga" );
                display_sprite( left,  cm->x + cm->wd/2 - left->width, y + ELEMENT_HT/4, context_z, 1.f, 1.f, CLR_WHITE );
                display_sprite( right, cm->x + cm->wd/2,               y + ELEMENT_HT/4, context_z, 1.f, 1.f, CLR_WHITE );
                y += ELEMENT_HT/2;
            }
            else
            {
                if( displayContextElement( cm, i, y ) )
                    hit = 1;
                y += ELEMENT_HT;
            }
        }
    }

    ret = ( hit || mouseCollision( &cm->box ));

    if( mouseCollision(&cm->box) )
        collisions_off_for_rest_of_frame = 1;

    return  ret; // returns tru if mouse is over any context menu or submenus so we can close
                                                  // it if the user clicks somewhere else
}

bool contextMenu_IsActive(void)
{
    if( playerPtr() && gCurrentContextMenu)
        return true;
    else
        return false;
}

// function for main loop which tries to display current context menu
//
void displayCurrentContextMenu()
{
    if( playerPtr() && gCurrentContextMenu)
    {
        int hit;
        int previousDisabled;

        previousDisabled = is_scrn_scaling_disabled();
        if (gCurrentContextMenu->disableScreenScaling)
            set_scrn_scaling_disabled(1);
        hit = displayContextMenu(gCurrentContextMenu); 
        set_scrn_scaling_disabled(previousDisabled);

        if( !hit && ( mouseDown(MS_LEFT) || mouseDown( MS_RIGHT )))
        {
            //PETUI MW changed this, becuse it was annoying for pet right click, need to see if it should be permanent
            //collisions_off_for_rest_of_frame = 1;
            contextMenu_closeAll();
        }
        else if ( hit )
            collisions_off_for_rest_of_frame = 1;
    }
}

static int noTranslateFunc(void *param)
{
    return 0;
}

void contextMenu_SetNoTranslate(ContextMenu *cm, int no_translate)
{
    int i, iSize;
    iSize = eaSize(&cm->elements);
    for( i = 0; i < iSize; i++ )
    {
        cm->elements[i]->translate = no_translate?noTranslateFunc:0;
    }
}

void contextMenu_SetCustomColors(ContextMenu *cm, int color, int bcolor)
{
    cm->customColors[0] = color;
    cm->customColors[1] = bcolor;
    cm->useCustomColors = 1;
}

void contextMenu_UnsetCustomColors(ContextMenu *cm)
{
    cm->useCustomColors = 0;
}

void contextMenu_SetDisableScreenScaling(ContextMenu *cm)
{
    cm->disableScreenScaling = 1;
}
