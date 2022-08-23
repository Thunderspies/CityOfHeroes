//
// dialogs
//
//-----------------------------------------------------------------------------------------------------


#include "win/win_init.h"
#include "graphics/gfx.h"
#include <utilitieslib/components/earray.h>
#include "entity/entVarUpdate.h"
#include "language/langClientUtil.h"
#include <utilitieslib/utils/mathutil.h>

#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiInput.h"
#include "UI/uiGame.h"
#include "UI/uiDialog.h"
#include "UI/uiUtil.h"
#include "uiSMFView.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtilGame.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "cmdparse/cmdcommon.h"
#include "filter/validate_name.h"
#include <utilitieslib/components/EString.h>
#include "cmdparse/cmdgame.h"
#include "UI/uiNet.h"
#include "UI/uiChat.h"
#include "UI/uiOptions.h"
#include "UI/uiMissionMaker.h"

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "player/player.h"

#include "graphics/ttFontUtil.h"
#include "formatter/smf_main.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "sound/sound.h"


static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)1,
    /* piShadow      */  (int *)0,
};

static TextAttribs s_ArchitectAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)1,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
    /* piColorSelect */  (int *)0,
    /* piColorSelectBG*/ (int *)0x333333ff,
    /* piScale       */  (int *)(int)(1.0f*SMF_FONT_SCALE),
    /* piFace        */  (int *)&smfSmall,
    /* piFont        */  (int *)0,
    /* piAnchor      */  (int *)0,
    /* piLink        */  (int *)0x80e080ff,
    /* piLinkBG      */  (int *)0,
    /* piLinkHover   */  (int *)0x66ff66ff,
    /* piLinkHoverBG */  (int *)0,
    /* piLinkSelect  */  (int *)0,
    /* piLinkSelectBG*/  (int *)0x666666ff,
    /* piOutline     */  (int *)0,
    /* piShadow      */  (int *)0,
};

#define BUTTON_HEIGHT  (20)
#define BOTTOM_SPACER  (6)
#define DEFAULT_WD     (300)
#define MIN_WD         (300)
#define CHECKBOX_HT    (16)

static SMFBlock *dialogEdit = 0;
static char      dialogEntryText[32];
static char *dialogEditLastSetText;

typedef struct dialog
{
    int type;
    int flags;
    int id;
    int dcbCount;
    int maxChars;

    float x;
    float y;
    float wd;
    float ht;
    float sc;
    float timer;
    float footer;

    char *txt;

    char *response1;
    char *response2;
    char *response3;

    void ( *code1 )(void *data);
    void ( *code2 )(void *data);
    void ( *code3 )(void *data);
    void ( *checkboxCallback )(void *data);
    void *data;

    DialogCheckbox *dcb;

}Dialog;

MP_DEFINE(Dialog);

static Dialog **dialogQueue;

static int gDialogUniqueCount = 0;
static int choose_name_immediate;

bool nameEditDialogCleanup(SMFBlock *pSMFBlock, bool allowHandle);

// New Dialogs
//-------------------------------------------------------------------------------------------------------------

static void dialogImplementation( int type, float x, float y, float wd, float ht, const char * txt,
            const char * option1, void (*code1)(void*data), const char * option2, void (*code2)(void*data), const char *option3, void (*code3)(void *data),
            int flags, void (*checkboxCallback)(void*data), DialogCheckbox *dcb, int dcbCount, int timer, int maxChars, void * data )
{
     Dialog *dlog;
    int i;

    if( !dialogQueue )
        eaCreate( &dialogQueue );

    for( i = eaSize(&dialogQueue)-1; i >= 0; i-- )
    {
        if( stricmp( dialogQueue[i]->txt, txt ) == 0 )
            return; // already in queue
    }

    MP_CREATE(Dialog, 32);
    dlog = MP_ALLOC(Dialog);

    if( txt )
    {
        dlog->txt = malloc( (strlen(txt) + 1)*sizeof( char ) );
        strcpy( dlog->txt, txt );
    }
    if( option1 )
    {
        dlog->response1 = malloc( (strlen(option1) + 1)*sizeof( char ) );
        strcpy( dlog->response1, option1 );
    }
    if( option2 )
    {
        dlog->response2 = malloc( (strlen(option2) + 1)*sizeof( char ) );
        strcpy( dlog->response2, option2 );
    }
    if( option3 )
    {
        dlog->response3 = malloc( (strlen(option3) + 1)*sizeof( char ) );
        strcpy( dlog->response3, option3 );
    }

    dlog->type = type;
    dlog->x = x;
    dlog->y = y;
    dlog->wd = wd;
    dlog->ht = ht;
    dlog->code1 = code1;
    dlog->data = data;
    dlog->code2 = code2;
    dlog->code3 = code3;
    dlog->flags = flags;
    dlog->dcb = dcb;
    dlog->dcbCount = dcbCount;
    dlog->maxChars = maxChars;
    dlog->timer = timer;
    dlog->id = gDialogUniqueCount;
    gDialogUniqueCount++;
    dlog->checkboxCallback = checkboxCallback;

    eaPush( &dialogQueue, dlog );
}

void dialog( int type, float x, float y, float wd, float ht, const char * txt, const char * option1, void (*code1)(void *data), char * option2, void (*code2)(void *data),
            int flags, void (*checkboxCallback)(void *data), DialogCheckbox *dcb, int dcbCount, int timer, int maxChars, void *data )
{
    dialogImplementation(type, x, y, wd, ht, txt, option1, code1, option2, code2, 0, 0, flags, checkboxCallback, dcb, dcbCount, timer, maxChars, data);
}

//
//
void dialogStd( int type, const char * txt, const char * response1, const char * response2, void(*code1)(void*data), void(*code2)(void*data), int flags )
{
    // -1s will have the dialog auto-size
    dialogImplementation(type, -1, -1, -1, -1, txt, response1, code1, response2, code2, 0, 0, flags, 0, 0, 0, 0, (flags & DLGFLAG_NAMES_ONLY) ? 30 : 0, 0 );
}

void dialogStd3( int type, const char * txt, const char * response1, const char * response2, const char *response3, void(*code1)(void*data), void(*code2)(void*data), void(*code3)(void*data), int flags )
{
    // -1s will have the dialog auto-size
    dialogImplementation(type, -1, -1, -1, -1, txt, response1, code1, response2, code2, response3, code3, flags, 0, 0, 0, 0, (flags & DLGFLAG_NAMES_ONLY) ? 30 : 0, 0 );
}

void dialogStdCB( int type, const char * txt, const char * response1, const char * response2, void(*code1)(void*data), void(*code2)(void*data), int flags, void *data )
{
    // -1s will have the dialog auto-size
    dialogImplementation(type, -1, -1, -1, -1, txt, response1, code1, response2, code2, 0, 0, flags, 0, 0, 0, 0, (flags & DLGFLAG_NAMES_ONLY) ? 30 : 0, data );
}

void dialogStdWidth( int type, const char * txt, const char * response1, const char * response2, void(*code1)(void*data), void(*code2)(void*data), int flags, float width )
{
    dialogImplementation(type, -1, -1, width, -1,    txt, response1, code1, response2, code2, 0, 0, flags, 0, 0, 0, 0, (flags & DLGFLAG_NAMES_ONLY) ? 30 : 0, 0 );
}

//
void dialogNameEdit(const char * txt, const char * response1, const char * response2, void(*code1)(void*data), void(*code2)(void*data), int maxChars, int flags )
{
    dialogImplementation(DIALOG_NAME_TEXT_ENTRY, -1, -1, -1, -1,    txt, response1, code1, response2, code2, 0, 0, (DLGFLAG_GAME_ONLY | flags), 0, 0, 0, 0, maxChars, 0 );
}

//
//
void dialogTimed( int type, const char * txt, const char * response1, const char * response2, void(*code1)(void*data), void(*code2)(void*data), int flags, int time )
{
    if( type == DIALOG_NAME_TEXT_ENTRY || type == DIALOG_NAME_ACCEPT )
        flags &= DLGFLAG_NAMES_ONLY;

    dialogImplementation(type, -1, -1, -1, -1, txt, response1, code1, response2, code2, 0, 0, flags, 0, 0, 0, time, 0, 0 );
}

void dialogArchitect( int type, int count )
{
    // -1s will have the dialog auto-size
    dialogImplementation(DIALOG_ARCHITECT_OPTIONS, -1, -1, -1, (count+1)*55, "Architect", 0, missionMakerDialogDummyFunction, 0, 0, 0, 0, DLGFLAG_ARCHITECT, 0, 0, 0, 0, 0, (void*)type ); 
}

void setMoralWatermark(const char *lwatermark, const char *rwatermark);

void dialogMoralChoice( const char *ltext, const char * rtext, void(*code1)(void*data), void(*code2)(void*data), const char *lwatermark, const char *rwatermark )
{
    setMoralWatermark(lwatermark, rwatermark);
    // -1s will have the dialog auto-size
    dialogImplementation(DIALOG_MORAL_CHOICE, -1, -1, -1, -1, "MoralChoice", ltext, code1, rtext, code2, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
}

void dialogChooseNameImmediate(int first_time)
{
    Entity * e = playerPtr();

    if( e )
    {
        if( !dialogInQueue( NULL, updatePlayerName, NULL ) )
        {
            dialogSetTextEntry( e->name );
            dialog(DIALOG_NAME_ACCEPT, -1, -1, -1, -1,
                first_time?textStd("PickNewName"):textStd("PickNewNameAgain"), NULL, updatePlayerName, NULL, NULL,
                DLGFLAG_GAME_ONLY|DLGFLAG_NAG|DLGFLAG_NAMES_ONLY, NULL, NULL, 0, 5, 30, 0 );
        }

        choose_name_immediate = true;
    }
    else
    {
        choose_name_immediate = false;
    }
}

int dialogMoveLocked(int wdw)
{
    if( wdw == WDW_DIALOG && choose_name_immediate )
        return true;
    
    return false;
}

static void freeDialog( Dialog * dlog )
{
    if( !dlog )
        return;

    SAFE_FREE(dlog->txt)
    SAFE_FREE(dlog->response1)
    SAFE_FREE(dlog->response2)
    MP_FREE(Dialog, dlog)
}

void dialogClearQueue( int game )
{
    int i, count = eaSize(&dialogQueue);

     for( i = count-1; i >= 0; i-- )
    {
        if( game == (dialogQueue[i]->flags & DLGFLAG_GAME_ONLY))
        {
            // Depending on the type of dialog, cancel as appropriate.
            switch(dialogQueue[i]->type)
            {
                case DIALOG_OK:
                    if( dialogQueue[i]->code1 )
                        dialogQueue[i]->code1(dialogQueue[i]->data);
                    break;
                case DIALOG_OK_CANCEL_TEXT_ENTRY:
                case DIALOG_NAME_TEXT_ENTRY:
                case DIALOG_ACCEPT_CANCEL:
                case DIALOG_YES_NO:
                    if( dialogQueue[i]->code2 )
                        dialogQueue[i]->code2(dialogQueue[i]->data);
                    break;
                case DIALOG_TWO_RESPONSE: // No sane way to cancel this
                default:
                    // Do nothing
                    break;
            }

            freeDialog( eaRemove( &dialogQueue, i ));
        }
    }
}

//
//
void dialogRemove(const char *pch, void (*code1)(void *data), void (*code2)(void *data))
{
    int i;

    for(i=eaSize(&dialogQueue)-1; i>=0; i--)
    {
        bool bMatches;

        bMatches = (pch==NULL || stricmp(dialogQueue[i]->txt, pch)==0);
        bMatches = bMatches && (code1==NULL || dialogQueue[i]->code1==code1);
        bMatches = bMatches && (code2==NULL || dialogQueue[i]->code2==code2);

        if(bMatches)
        {
            freeDialog(eaRemove(&dialogQueue, i));
            break;
        }
    }
}

//
//
bool dialogInQueue(const char *pch, void (*code1)(void *data), void (*code2)(void *data) )
{
    int i;

    for(i=eaSize(&dialogQueue)-1; i>=0; i--)
    {
        bool bMatches;

        bMatches = (pch==NULL || stricmp(dialogQueue[i]->txt, pch)==0);
        bMatches &= (code1==NULL || dialogQueue[i]->code1==code1);
        bMatches &= (code2==NULL || dialogQueue[i]->code2==code2);

        if(bMatches)
            return true;
    }

    return false;
}

//
//
void dialogYesNo( int yesno )
{
    int i, count = eaSize(&dialogQueue);

    for( i = 0; i < count; i++ )
    {
        if( !(dialogQueue[i]->flags & DLGFLAG_GAME_ONLY) || !shell_menu() )
        {
            Dialog *dlog = dialogQueue[i];

            switch( dlog->type)
            {
                case DIALOG_OK:
                case DIALOG_SINGLE_RESPONSE:
                {
                    if( dlog->code1 )
                        dlog->code1(dlog->data);

                    freeDialog(eaRemove( &dialogQueue, i ));
                }break;

                case DIALOG_YES_NO:
                case DIALOG_ACCEPT_CANCEL:
                {
                    if( yesno && dlog->code1 )
                        dlog->code1(dlog->data);
                    else if( !yesno && dlog->code2 )
                        dlog->code2(dlog->data);

                    freeDialog(eaRemove( &dialogQueue, i ));

                }break;

                default:
                    break;
            }
            return;
        }
    }
}

//
//
void dialogAnswer( const char * text )
{
    int i, count = eaSize(&dialogQueue);

    for( i = 0; i < count; i++ )
    {
        if( !(dialogQueue[i]->flags & DLGFLAG_GAME_ONLY) || !shell_menu() )
        {
            Dialog *dlog = dialogQueue[i];

            if( (dlog->type == DIALOG_OK && stricmp( textStd("OkString"), text ) == 0) ||
                (dlog->type == DIALOG_YES_NO && stricmp( textStd("YesString"), text ) == 0) ||
                 (dlog->type == DIALOG_ACCEPT_CANCEL && stricmp( textStd("AcceptString"), text ) == 0) ||
                (dlog->response1 && stricmp( dlog->response1, text ) == 0 ) )
            {
                if( dlog->code1 )
                    dlog->code1(dlog->data);
                freeDialog(eaRemove( &dialogQueue, i ));
            }
            else if( (dlog->type == DIALOG_YES_NO && stricmp( textStd("NoString"), text ) == 0) ||
                     (dlog->type == DIALOG_ACCEPT_CANCEL && stricmp( textStd("CancelString"), text ) == 0) ||
                     (dlog->response2 && stricmp( dlog->response2, text ) == 0 ) )
            {
                if( dlog->code2 )
                    dlog->code2(dlog->data);
                freeDialog(eaRemove( &dialogQueue, i ));
            }
            return;
        }
    }
}


static void calc_dialog_text_size( int *retwd, int *retht, SMFView *view, float wsc, int available_wd)
{
    int ht;
    int wd = DEFAULT_WD*wsc;

    smfview_SetSize(view, wd, 0);
    gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*wsc));
    smfview_SetAttribs(view, &gTextAttr);
    ht = smfview_GetHeight(view);
    if(ht > 3*wd/2)
    {
        float fArea = wd*ht;
        wd = fsqrt(fArea);
        if(wd<MIN_WD*wsc)
            wd = MIN_WD*wsc;
        if( wd > available_wd*.75f)
            wd = available_wd*.75f;
        smfview_SetSize(view, wd, 0);
        ht = smfview_GetHeight(view);
    }

    ht += 15*wsc; // extra space to deal with quirks of text wrapping
    *retht = ht;
    *retwd = wd+(PIX3*2+R10)*wsc;
}

//
// This should probably be wrapped into the dialog.
//
static AtlasTex *g_lMoralWatermark;
static AtlasTex *g_rMoralWatermark;

void setMoralWatermark(const char *lwatermark, const char *rwatermark)
{
    if (lwatermark && lwatermark[0])
        g_lMoralWatermark = atlasLoadTexture( lwatermark );
    else
        g_lMoralWatermark = atlasLoadTexture( "Resistance_Watermark_256px" ); //left == Resistance

    if (rwatermark && rwatermark[0])
        g_rMoralWatermark = atlasLoadTexture( rwatermark );
    else
        g_rMoralWatermark = atlasLoadTexture( "Loyalist_Watermark_256px" );    //right == Loyalist
}

// 
// MoralChoice is now so different from a normal Dialog, it's handled separately
//
static void displayMoralChoice(void)
{
    Dialog * dlog = 0;
    static int last_dialog = -1;
    CBox box;
    static SMFView *s_lview;
    static SMFView *s_rview;
    static SMFView *s_pview;
    float xsc = 1.f, ysc = 1.f;
    float wx, wy, wz, winwd, winht, wsc;
    float lwx, rwx;
    F32    l_watermark_scale;
    F32    r_watermark_scale;
    U32 lAlpha, rAlpha;

    int    color, back_color;
    //int response;
    int dlog_idx;
    bool buttonenabled = true;
    static char * estr;
    static int mid_spacing;
    int temp_ht;
    int mainwin_wd, mainwin_ht;
    static int old_mainwin_wd, old_mainwin_ht;
    int num_dialogs;

    num_dialogs = eaSize( &dialogQueue );

    // choose current dialog
    for( dlog_idx = 0; dlog_idx < num_dialogs; dlog_idx++ )
    {
        if( !(dialogQueue[dlog_idx]->flags & DLGFLAG_GAME_ONLY) || !shell_menu() )
        {
            dlog = dialogQueue[dlog_idx];
            break;
        }
    }

    if( !dlog )
        return;

    window_bringToFront(WDW_DIALOG);

    window_getDims( WDW_DIALOG, &wx, &wy, &wz, &winwd, &winht, &wsc, &color, &back_color );

    if (dlog->timer > 0 && (dlog->flags & DLGFLAG_NAG))
        buttonenabled = false;

    window_getDims( WDW_DIALOG, &wx, &wy, &wz, &winwd, &winht, &wsc, &color, &back_color );
    if( shell_menu() )
        wsc = 1.f;

    if(s_lview==NULL)
    {
        s_lview = smfview_Create(0);
        smfview_SetScrollBar(s_lview, false);
        smfview_SetLocation(s_lview, PIX3*wsc, (PIX3+1)*wsc, 0);
        s_pview = s_lview;
    }
    if(s_rview==NULL)
    {
        s_rview = smfview_Create(0);
        smfview_SetScrollBar(s_rview, false);
        smfview_SetLocation(s_rview, PIX3*wsc, (PIX3+1)*wsc, 0);
    }

    smfview_SetText(s_lview, dlog->response1);
    smfview_SetText(s_rview, dlog->response2);


    if( shell_menu() )
    {
        mainwin_wd = DEFAULT_SCRN_WD;
        mainwin_ht = DEFAULT_SCRN_HT;
    }
    else
    {
        windowClientSizeThisFrame(&mainwin_wd, &mainwin_ht);
    }

    
    if( (shell_menu() || window_getMode(WDW_DIALOG)==WINDOW_DISPLAYING) &&
        (last_dialog != dlog->id || winwd <= 0 ) || 
        mainwin_wd != old_mainwin_wd || mainwin_ht != old_mainwin_ht)
    {
        //do this once, when dialog changes.
        smfview_Reparse(s_lview);    
        smfview_Reparse(s_rview);    
        smfview_ResetScrollBarOffset(s_pview);
        //if( dlog->wd <= 0 || winwd <= 0 )
        {
            int ht;
            int wd = DEFAULT_WD*wsc;

            if (dlog->wd > 0)
                wd = dlog->wd*wsc;

            old_mainwin_ht = mainwin_ht;
            old_mainwin_wd = mainwin_wd;


            //----------------------
            //calc left size
            //calc right size
            // take the max of the two.
            {
                int lwd, lht, rwd, rht;
                calc_dialog_text_size( &lwd, &lht, s_lview, wsc, mainwin_wd/2);
                calc_dialog_text_size( &rwd, &rht, s_rview, wsc, mainwin_wd/2);
            
                dlog->wd = MAX(lwd, rwd);
                ht = MAX(lht, rht);
            }
            
            //---------------------------------

            dlog->footer = (PIX3*2+R10+BOTTOM_SPACER)*wsc;

            dlog->ht = ht + dlog->footer;
            //Set maximum height slightly shorter than normal dialog max height,
            // because the moral choice dialog is usually over a cut scene, which is in letterbox.
            if(dlog->ht > mainwin_ht*.60f)    
                dlog->ht = mainwin_ht*.60f;

            mid_spacing = (60 * wsc);
            if (mainwin_wd < dlog->wd*2+mid_spacing)
                mid_spacing = mainwin_wd - dlog->wd*2;

            dlog->y = mainwin_ht/2-dlog->ht/2;
            dlog->x = mainwin_wd/2-dlog->wd- mid_spacing/2;

        }
        window_setDims( WDW_DIALOG, dlog->x, dlog->y, dlog->wd, dlog->ht );
        window_setMode( WDW_DIALOG, WINDOW_DOCKED );
        window_setMode( WDW_DIALOG, WINDOW_GROWING );
        last_dialog = dlog->id;
    }


    window_getDims( WDW_DIALOG, &wx, &wy, &wz, &winwd, &winht, &wsc, &color, &back_color );

    lwx = wx;
    rwx = wx+mid_spacing+winwd;

    drawFrame( PIX3, R10, lwx, wy, wz, winwd, winht, wsc, color, back_color );
    drawFrame( PIX3, R10, rwx, wy, wz, winwd, winht, wsc, color, back_color );

    if( winDefs[WDW_DIALOG].timer < 5 )
    {
        winDefs[WDW_DIALOG].timer += TIMESTEP;

        if( winDefs[WDW_DIALOG].timer >= 5 )
            winDefs[WDW_DIALOG].timer = 5;
    }
    else
        window_setMode( WDW_DIALOG, WINDOW_DISPLAYING );

    if( window_getMode( WDW_DIALOG ) != WINDOW_DISPLAYING )
    {
        dialogClearTextEntry();
        return;
    }
    else if (dialogEditLastSetText && dialogEdit)
    {
        estrDestroy(&dialogEditLastSetText);
        dialogEditLastSetText = 0;
    }

    //Hack to make the right side window also have a jelly top
    window_setDims( WDW_DIALOG, rwx, wy, winwd, winht );
    displayWindow( WDW_DIALOG );
    // set it back
    window_setDims( WDW_DIALOG, wx, wy, winwd, winht );
    displayWindow( WDW_DIALOG );

    //left side
    temp_ht = smfview_GetHeight(s_lview);
    smfview_SetSize(s_lview, winwd-(PIX3+R10)*2*wsc, MIN(winht-dlog->footer, temp_ht));

    if( winht < smfview_GetHeight(s_lview) )
        smfview_SetScrollBar(s_lview, 1);
    else
        smfview_SetScrollBar(s_lview, 0);

    smfview_SetLocation(s_lview, 0, 0, 0);
    smfview_SetBaseLocation(s_lview, lwx+(PIX3+R10)*wsc, wy+(PIX3*2)*wsc, wz, winwd-(R10+PIX3)*wsc, winht);
    smfview_Draw(s_lview);

    //right side
    temp_ht = smfview_GetHeight(s_rview);
    smfview_SetSize(s_rview, winwd-(PIX3+R10)*2*wsc, MIN(winht-dlog->footer, temp_ht));

    if( winht < smfview_GetHeight(s_rview) )
        smfview_SetScrollBar(s_rview, 1);
    else
        smfview_SetScrollBar(s_rview, 0);

    smfview_SetLocation(s_rview, 0, 0, 0);
    smfview_SetBaseLocation(s_rview, rwx+(PIX3+R10)*wsc, wy+(PIX3*2)*wsc, wz, winwd-(R10+PIX3)*wsc, winht);
    smfview_Draw(s_rview);

    //
    
    BuildCBox( &box, lwx, wy, winwd, winht );
    if( mouseCollision( &box ) )
    {
        lAlpha = 0xffffff88;
    } else {
        lAlpha = 0xffffff44;
    }
    if( mouseClickHit( &box, MS_LEFT ) )
    {
        if( dlog->code1 )
            dlog->code1(dlog->data);

        mouseClear();
        freeDialog(eaRemove( &dialogQueue, dlog_idx ));
        collisions_off_for_rest_of_frame = TRUE;
    }

    BuildCBox( &box, rwx, wy, winwd, winht );
    if( mouseCollision( &box ) )
    {
        rAlpha = 0xffffff88;
    } else {
        rAlpha = 0xffffff44;
    }
    if( mouseClickHit( &box, MS_LEFT ) )
    {
        if( dlog->code2 )
            dlog->code2(dlog->data);

        mouseClear();
        freeDialog(eaRemove( &dialogQueue, dlog_idx ));
        collisions_off_for_rest_of_frame = TRUE;
    }

    l_watermark_scale = MIN( winwd/g_lMoralWatermark->width, winht/g_lMoralWatermark->height );
    display_sprite(g_lMoralWatermark, lwx + (winwd - g_lMoralWatermark->width*l_watermark_scale)/2, 
        wy + (winht - g_lMoralWatermark->height*l_watermark_scale)/2, wz, l_watermark_scale, l_watermark_scale, lAlpha );  

    r_watermark_scale = MIN( winwd/g_rMoralWatermark->width, winht/g_rMoralWatermark->height );
    display_sprite(g_rMoralWatermark, rwx + (winwd - g_rMoralWatermark->width*r_watermark_scale)/2, 
        wy + (winht - g_rMoralWatermark->height*r_watermark_scale)/2, wz, r_watermark_scale, r_watermark_scale, rAlpha );  

}

//
//
void displayDialogQueue()
{
    Dialog * dlog = 0;
    static int last_dialog = -1;
    CBox box;
    static SMFView *s_pview;
     float xsc = 1.f, ysc = 1.f;
    float wx, wy, ty, wz, winwd, winht, wsc;
    int    dlog_idx, color, back_color, response;
    bool buttonenabled = true;
    static char * estr;

    if( choose_name_immediate )
        dialogChooseNameImmediate(0);

     if ( eaSize( &dialogQueue ) == 0 )
    {
        window_setMode( WDW_DIALOG, WINDOW_DOCKED );
        return;
    }
    else
    {
         int count = eaSize( &dialogQueue );

        // increment timers
        for( dlog_idx = 0; dlog_idx < count; dlog_idx++ )
        {
            if( dialogQueue[dlog_idx]->timer > 0)
                dialogQueue[dlog_idx]->timer -= TIMESTEP/30;

            // choose NO if timer runs out
            if( dialogQueue[dlog_idx]->timer < 0 && !(dialogQueue[dlog_idx]->flags & DLGFLAG_NAG))
            {
                if( dialogQueue[dlog_idx]->code2 )
                    dialogQueue[dlog_idx]->code2(dialogQueue[dlog_idx]->data);
                freeDialog( eaRemove( &dialogQueue, dlog_idx ));
            }
        }

        count = eaSize( &dialogQueue );

        // choose current dialog
        for( dlog_idx = 0; dlog_idx < count; dlog_idx++ )
        {
            if( !(dialogQueue[dlog_idx]->flags & DLGFLAG_GAME_ONLY) || !shell_menu() )
            {
                dlog = dialogQueue[dlog_idx];
                break;
            }
        }
    }

    if( !dlog )
        return;

     window_bringToFront(WDW_DIALOG);

    if (dlog->type == DIALOG_MORAL_CHOICE)
    {
        displayMoralChoice();
        return;
    }

    if (dlog->timer > 0 && (dlog->flags & DLGFLAG_NAG))
        buttonenabled = false;

     window_getDims( WDW_DIALOG, &wx, &wy, &wz, &winwd, &winht, &wsc, &color, &back_color );
    if( shell_menu() )
        wsc = 1.f;
    
    if(s_pview==NULL)
    {
        s_pview = smfview_Create(0);
        smfview_SetScrollBar(s_pview, false);
        smfview_SetLocation(s_pview, PIX3*wsc, (PIX3+1)*wsc, 0);
    }

     if (dlog->flags & DLGFLAG_NO_TRANSLATE)
        smfview_SetText(s_pview, dlog->txt);
    else if (dlog->flags & DLGFLAG_ARCHITECT)
    {
        if(!estr)
            estrCreate(&estr);
        estrClear(&estr);
        estrConcatf(&estr, "<font outline=0 color=architect>%s</color>", textStd(dlog->txt) );
        smfview_SetText(s_pview, estr);
    }
    else
          smfview_SetText(s_pview, textStd(dlog->txt));

       if( dlog->type == DIALOG_ARCHITECT_OPTIONS )
    {
        int win_wd, win_ht;
        smfview_SetText(s_pview, "");
        if( last_dialog != dlog->id )
        {
            windowClientSizeThisFrame(&win_wd, &win_ht);
            dlog->x = win_wd/2-dlog->wd/2;
            dlog->y = win_ht/2-dlog->ht/2;
            window_setDims( WDW_DIALOG, dlog->x, dlog->y, DEFAULT_WD, dlog->ht );
            last_dialog = dlog->id;
            smfview_ResetScrollBarOffset(s_pview);
        }

    }
    else if( (shell_menu() || window_getMode(WDW_DIALOG)==WINDOW_DISPLAYING) &&
        (last_dialog != dlog->id || winwd <= 0 ) )
    {
           smfview_Reparse(s_pview);    
        smfview_ResetScrollBarOffset(s_pview);
        //if( dlog->wd <= 0 || winwd <= 0 )
        {
            int ht;
            int wd = DEFAULT_WD*wsc;
            int win_wd, win_ht;

            if (dlog->wd > 0)
                wd = dlog->wd*wsc;

            if( shell_menu() )
            {
                win_wd = DEFAULT_SCRN_WD;
                win_ht = DEFAULT_SCRN_HT;
            }
            else
            {
                windowClientSizeThisFrame(&win_wd, &win_ht);
            }

            smfview_SetSize(s_pview, wd, 0);
            gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*wsc));
            smfview_SetAttribs(s_pview, &gTextAttr);
            ht = smfview_GetHeight(s_pview);
            if(ht > 3*wd/2)
            {
                float fArea = wd*ht;
                wd = fsqrt(fArea);
                if(wd<MIN_WD*wsc)
                    wd = MIN_WD*wsc;
                if( wd > win_wd*.75f)
                    wd = win_wd*.75f;
                smfview_SetSize(s_pview, wd, 0);
                ht = smfview_GetHeight(s_pview);
            }

             ht += 15*wsc; // extra space to deal with quirks of text wrapping

            dlog->wd = wd+(PIX3*2+R10)*wsc;

            if( dlog->type == DIALOG_SMF )
                dlog->footer = (PIX3*2+R10+BOTTOM_SPACER)*wsc;
            else
                dlog->footer = (PIX3*2+R10 + BUTTON_HEIGHT+BOTTOM_SPACER)*wsc;

             if( dlog->type == DIALOG_OK_CANCEL_TEXT_ENTRY)
                dlog->footer += BOTTOM_SPACER*wsc;

            if( dlog->type == DIALOG_TWO_RESPONSE_CANCEL)
                dlog->footer += 2*BOTTOM_SPACER*wsc;
            

            if( dlog->type == DIALOG_TWO_RESPONSE_CANCEL || dlog->type == DIALOG_TWO_RESPONSE )
            {
                int button_wd;
                if( dlog->flags & DLGFLAG_ARCHITECT )
                    button_wd = max(str_wd(&title_12, wsc, wsc, dlog->response1 ), str_wd(&title_12, wsc, wsc, dlog->response2 ));
                else
                    button_wd = max(str_wd(&game_12, wsc, wsc, dlog->response1 ), str_wd(&game_12, wsc, wsc, dlog->response2 ));

                dlog->wd = MAX( dlog->wd, button_wd * 2 + R10*3*wsc );
            }

            if( dlog->dcbCount )    
                dlog->footer += (CHECKBOX_HT*dlog->dcbCount + PIX3*2)*wsc;

            if( dlog->timer && !(dlog->flags & DLGFLAG_NAG) )
                dlog->footer += 20*wsc;

            dlog->ht = ht + dlog->footer;
            if(dlog->ht > win_ht*.75f)
                dlog->ht = win_ht*.75f;
                

            dlog->x = win_wd/2-dlog->wd/2;
            dlog->y = win_ht/2-dlog->ht/2;

        }
        window_setDims( WDW_DIALOG, dlog->x, dlog->y, dlog->wd, dlog->ht );
        window_setMode( WDW_DIALOG, WINDOW_DOCKED );
        window_setMode( WDW_DIALOG, WINDOW_GROWING );
        last_dialog = dlog->id;
    }
    

    window_getDims( WDW_DIALOG, &wx, &wy, &wz, &winwd, &winht, &wsc, &color, &back_color );

    if( shell_menu() )
    {
        color = SELECT_FROM_UISKIN( CLR_BTN_HERO, CLR_BTN_VILLAIN, CLR_BTN_ROGUE );
         back_color = 0x000000dd;
        wsc = 1.f;
    }

      if( dlog->flags & DLGFLAG_ARCHITECT )
        drawWaterMarkFrame( wx, wy, wz, wsc,  winwd, winht, 0 );
    else 
        drawFrame( PIX3, R10, wx, wy, wz, winwd, winht, wsc, color, back_color );

    displayWindow( WDW_DIALOG );

    if( winDefs[WDW_DIALOG].timer < 5 )
    {
        winDefs[WDW_DIALOG].timer += TIMESTEP;

        if( winDefs[WDW_DIALOG].timer >= 5 )
            winDefs[WDW_DIALOG].timer = 5;
    }
    else
        window_setMode( WDW_DIALOG, WINDOW_DISPLAYING );

    if( window_getMode( WDW_DIALOG ) != WINDOW_DISPLAYING )
    {
        dialogClearTextEntry();
        return;
    }
    else if (dialogEditLastSetText && dialogEdit)
    {
        estrDestroy(&dialogEditLastSetText);
        dialogEditLastSetText = 0;
    }

      gTextAttr.piScale = (int*)((int)(SMF_FONT_SCALE*wsc));
      smfview_SetSize(s_pview, winwd-(PIX3+R10)*2*wsc, MIN(winht-dlog->footer, smfview_GetHeight(s_pview))); // SIDE EFFECTS FTL

    if( winht-dlog->footer < smfview_GetHeight(s_pview) )
        smfview_SetScrollBar(s_pview, 1);
    else
        smfview_SetScrollBar(s_pview, 0);

       smfview_SetLocation(s_pview, 0, 0, 0);
       smfview_SetBaseLocation(s_pview, wx+(PIX3+R10)*wsc, wy+(PIX3*2)*wsc, wz, winwd-(R10+PIX3)*wsc, winht);
    smfview_Draw(s_pview);

    if( dlog->dcb )
    {
        int i;
         float dy = wy + winht - PIX3*2*wsc;
        AtlasTex * mark;
 
         for( i = 0; i < dlog->dcbCount; i++ )
        {
            CBox box;

             if( (dlog->dcb[i].variable && *dlog->dcb[i].variable) ||
                (dlog->dcb[i].optionID && optionGet(dlog->dcb[i].optionID)) )
            {
                 mark = atlasLoadTexture( "Context_checkbox_filled.tga" );
                 display_sprite( mark, wx + (R10+PIX3)*wsc, dy-(i+1)*CHECKBOX_HT*wsc, wz+3, wsc, wsc, (winDefs[0].loc.color & 0xffffff00)|0xff  );

                mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
                display_sprite( mark, wx + (R10+PIX3)*wsc, dy-(i+1)*CHECKBOX_HT*wsc, wz+2, wsc, wsc, (winDefs[0].loc.color & 0xffffff00)|0xff );
            }
            else
            {
                mark = atlasLoadTexture( "Context_checkbox_empty.tga" );
                display_sprite( mark, wx + (R10+PIX3)*wsc, dy-(i+1)*CHECKBOX_HT*wsc, wz+3, wsc, wsc, winDefs[0].loc.color & 0xffffffff );
            }

            font(&game_9);
            font_color( 0xffffff88, 0xffffff88 );
               cprntEx(wx + (R10+2*PIX3+mark->width)*wsc, dy-((i)*CHECKBOX_HT+CHECKBOX_HT/2)*wsc, wz+3, wsc, wsc, CENTER_Y, dlog->dcb[i].txt );

              BuildCBox( &box, wx+PIX3*wsc, dy-(i+1)*CHECKBOX_HT*wsc, winwd - 2*PIX3*wsc, CHECKBOX_HT*wsc );
            if( mouseDownHit( &box, MS_LEFT ) )
            {
                if( dlog->dcb[i].variable )
                    *dlog->dcb[i].variable = !(*dlog->dcb[i].variable);
                if( dlog->dcb[i].optionID )
                {
                    //0 used because it's not necessary to trigger sendOptions in optionToggle every time user toggles option.
                    //Pressing OK in dialog box will call sendOptions.
                    optionToggle(dlog->dcb[i].optionID, 0 );
                }
                    if( dlog->checkboxCallback )
                    dlog->checkboxCallback(dlog->data);
            }
            
        }

        ty = wy + winht - (2*PIX3+BUTTON_HEIGHT/2+BOTTOM_SPACER/2+dlog->dcbCount*CHECKBOX_HT)*wsc;
     }
    else if ( dlog->timer > 0 && (!(dlog->flags & DLGFLAG_NAG)))
    {
        // the 55.55 is just to get a constant string width so the centered text doesn't bounce around while the timer falls
        float strwd = str_wd( font_grp, wsc, wsc, textStd("TimeRemaining", 55.55))/2;
        font(&game_12);
        font_color( 0xffffff88, 0xffffff88 );
        cprntEx(wx + winwd/2 - strwd, wy + winht - (PIX3+10)*wsc, wz+3, wsc, wsc, (CENTER_Y), textStd("TimeRemaining", dlog->timer ));
        ty = wy + winht - (PIX3+BUTTON_HEIGHT/2+BOTTOM_SPACER/2+20)*wsc;
    }
    else
        ty = wy + winht - (PIX3+BUTTON_HEIGHT/2+BOTTOM_SPACER/2)*wsc;

    
    switch( dlog->type )
    {
        case DIALOG_OK:
        {
            if( dlog->flags & DLGFLAG_ARCHITECT )
                response = drawMMButton("OkString", 0, 0, wx + winwd/2, ty, wz, 60*wsc, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
            else
            {
                if(!buttonenabled) 
                {
                    char count[8];
                    sprintf(count,"%d",(int)dlog->timer + 1);
                    response = (D_MOUSEHIT ==drawStdButton(wx + winwd/2, ty, wz, 40*wsc, BUTTON_HEIGHT*wsc, color, count, wsc, 1));

                }
                else
                    response = (D_MOUSEHIT == drawStdButton(wx + winwd/2, ty, wz, 40*wsc, BUTTON_HEIGHT*wsc, color, "OkString", wsc, !buttonenabled));
            }

            if(response)
            {
                if( dlog->code1 )
                    dlog->code1(dlog->data);

                mouseClear();
                freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                collisions_off_for_rest_of_frame = TRUE;
            }
        }break;

        case DIALOG_SINGLE_RESPONSE:
        {
            int button_wd = str_wd(&game_12, wsc, wsc, dlog->response1 )+10*wsc;
            if(!buttonenabled) 
            {
                char count[8];
                sprintf(count,"%d",(int)dlog->timer + 1);
                response = drawStdButton(wx + winwd/2, ty, wz, button_wd*wsc, BUTTON_HEIGHT*wsc, CLR_RED, count, wsc, 1);

            }            
            else response = drawStdButton(wx + winwd/2, ty, wz, button_wd*wsc, BUTTON_HEIGHT*wsc, CLR_RED, dlog->response1, wsc, !buttonenabled);
            if(D_MOUSEHIT == response)
            {
                if( dlog->code1 )
                    dlog->code1(dlog->data);

                mouseClear();
                freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                collisions_off_for_rest_of_frame = TRUE;
            }
        }break;

        case DIALOG_OK_CANCEL_TEXT_ENTRY: /* fall through */
        case DIALOG_NAME_TEXT_ENTRY:
        case DIALOG_NAME_ACCEPT:
            {
                char* text = NULL;
                bool locked = 1;

                if( !dialogEdit )
                {
                    dialogEdit = smfBlock_Create();
                    smf_SetFlags(dialogEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 
                        SMFInputMode_AnyTextNoTagsOrCodes, dlog->maxChars ? dlog->maxChars : MAX_PLAYER_NAME_LEN(playerPtr()),
                        SMFScrollMode_InternalScrolling, SMFOutputMode_StripAllTags, dlog->flags & DLGFLAG_SECRET_INPUT ? SMFDisplayMode_AsterisksOnly : SMFDisplayMode_AllCharacters,
                        SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
                    smf_SetCharacterInsertSounds(dialogEdit, "type2", "mouseover");
                    if (dialogEditLastSetText)
                    {
                        smf_SetRawText(dialogEdit, dialogEditLastSetText, false);
                        smf_SetCursor(dialogEdit, estrLength(&dialogEditLastSetText));
                    }
                    else
                    {
                        smf_SetRawText(dialogEdit, dialogEntryText, false);
                        smf_SetCursor(dialogEdit, strlen(dialogEntryText));
                    }
                }
            
                if(dlog->type == DIALOG_NAME_TEXT_ENTRY || dlog->type == DIALOG_NAME_ACCEPT || (dlog->flags & DLGFLAG_NAMES_ONLY))
                {
                    int maxLen = dlog->maxChars;
                    SMFInputMode input = SMFInputMode_AnyTextNoTagsOrCodes;

                    if (dlog->type == DIALOG_NAME_TEXT_ENTRY || dlog->type == DIALOG_NAME_ACCEPT)
                    {
                        if (dlog->flags & DLGFLAG_ALLOW_HANDLE)
                            input = SMFInputMode_PlayerNamesAndHandles;
                        else
                            input = SMFInputMode_PlayerNames;
                    }

                    if (nameEditDialogCleanup(dialogEdit, (dlog->flags & DLGFLAG_ALLOW_HANDLE)))
                    {
                        // if first letter is a '@', don't count this in the max
                        maxLen++;
                    }

                    smf_SetFlags(dialogEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 
                        input, maxLen,
                        SMFScrollMode_InternalScrolling, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters,
                        SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
                }

                 if( dlog->flags & DLGFLAG_ARCHITECT )
                    drawTextEntryFrame( wx + R10*wsc, ty - (3*(BUTTON_HEIGHT/2)+BOTTOM_SPACER/2+2)*wsc, wz+1, winwd - 2*(R10)*wsc, 22*wsc, wsc, smfBlock_HasFocus(dialogEdit) );
                else
                    drawFrame( PIX2, R4,  wx + (PIX3+R10)*wsc, ty  - (3*(BUTTON_HEIGHT/2)+BOTTOM_SPACER/2)*wsc, wz+1, winwd - 2*(R10+PIX3)*wsc, 20*wsc, wsc, 0, 0xffffff44 );

                smf_SetScissorsBox(dialogEdit, wx + R10+PIX3*2*wsc, ty - (3*(BUTTON_HEIGHT/2)+BOTTOM_SPACER/2)*wsc, winwd - 2*(R10+PIX3)*wsc - 6*wsc, 20*wsc);
                smf_Display(dialogEdit, wx + R10+PIX3*2*wsc, ty - (3*(BUTTON_HEIGHT/2)+BOTTOM_SPACER/2)*wsc, wz+2,
                    winwd - 2*(R10+PIX3)*wsc - 6*wsc, 20*wsc, 0, 0, (dlog->flags&DLGFLAG_ARCHITECT)?&s_ArchitectAttr:&gTextAttr_WhiteHybridBold12, 0);

                // OK button only available if at least one character has been entered
                if (dialogEdit->outputString)
                {
                    locked = 0;
                }

                if(dlog->type == DIALOG_NAME_ACCEPT)
                {
                    int hit = 0;

                    if( dlog->flags&DLGFLAG_ARCHITECT)
                        hit |= drawMMButton("AcceptString",0, 0, wx + winwd/2, ty, wz, 70*wsc, .75f*wsc, MMBUTTON_SMALL|(locked|!buttonenabled), 0  );
                    else
                        hit |= (D_MOUSEHIT == drawStdButton(wx + winwd/2, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, "AcceptString", wsc, locked|!buttonenabled));
                    if( hit )
                    {
                        if( dlog->code1 )
                            dlog->code1(dlog->data);

                        mouseClear();
                        dialogClearTextEntry();
                        freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                        collisions_off_for_rest_of_frame = TRUE;
                        choose_name_immediate = false;
                    }
                }
                else
                {
                    int hit = 0, hit2 = 0;

                     if( dlog->flags&DLGFLAG_ARCHITECT)
                        hit |= drawMMButton("OkString",0, 0, wx + winwd/3, ty, wz, 100*wsc, .75f*wsc, MMBUTTON_SMALL, 0  );
                    else
                        hit |= (D_MOUSEHIT == drawStdButton(wx + winwd/3, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, "OkString", wsc, locked));

                    if( dlog->flags&DLGFLAG_ARCHITECT)
                        hit2 |= drawMMButton("CancelString",0, 0, wx + 2*winwd/3, ty, wz, 100*wsc, .75f*wsc, MMBUTTON_SMALL, 0  );
                    else
                        hit2 |= (D_MOUSEHIT == drawStdButton(wx + 2*winwd/3, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, "CancelString", wsc, 0));

                    if( hit )
                    {
                        if( dlog->code1 )
                            dlog->code1(dlog->data);

                        mouseClear();
                        dialogClearTextEntry();
                        freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                        collisions_off_for_rest_of_frame = TRUE;
                    }
                    else if( hit2) 
                    {
                        if( dlog->code2 )
                            dlog->code2(dlog->data);

                        mouseClear();
                        dialogClearTextEntry();
                        freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                        collisions_off_for_rest_of_frame = TRUE;
                    }
                }
            }break;

        case DIALOG_YES_NO:
        {
            int response1 = 0, response2 = 0;

            if( dlog->flags & DLGFLAG_ARCHITECT )
                response1 = drawMMButton("YesString", 0, 0, wx + winwd/3, ty, wz, 100*wsc, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
            else
                response1 = (D_MOUSEHIT == drawStdButton(wx + winwd/3, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, "YesString", wsc, !buttonenabled));

            if( dlog->flags & DLGFLAG_ARCHITECT )
                response2 = drawMMButton("NoString", 0, 0, wx + 2*winwd/3, ty, wz, 100*wsc, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
            else
                response2 = (D_MOUSEHIT == drawStdButton(wx + 2*winwd/3, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, "NoString", wsc, !buttonenabled));

            if(response1)
            {
                if( dlog->code1 )
                    dlog->code1(dlog->data);

                sndPlay("N_Select", SOUND_GAME);
                mouseClear();
                freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                collisions_off_for_rest_of_frame = TRUE;
            }
            else if(response2)
            {
                if( dlog->code2 )
                    dlog->code2(dlog->data);

                sndPlay("N_Deselect", SOUND_GAME);
                mouseClear();
                freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                collisions_off_for_rest_of_frame = TRUE;
            }
        }break;

        case DIALOG_ACCEPT_CANCEL:
            {
                int response1 = 0, response2 = 0;

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    response1 = drawMMButton((dlog->response1 ? dlog->response1 : "AcceptString"), 0, 0, wx + winwd/3, ty, wz, 100*wsc, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
                else
                    response1 = (D_MOUSEHIT == drawStdButton(wx + winwd/3, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, (dlog->response1 ? dlog->response1 : "AcceptString"), wsc, !buttonenabled));

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    response2 = drawMMButton((dlog->response2 ? dlog->response2 : "CancelString"), 0, 0, wx + 2*winwd/3, ty, wz, 100*wsc, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
                else
                    response2 = (D_MOUSEHIT == drawStdButton(wx + 2*winwd/3, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, (dlog->response2 ? dlog->response2 : "CancelString"), wsc, !buttonenabled));


                if(response1)
                {
                    if( dlog->code1 )
                        dlog->code1(dlog->data);

                    sndPlay("N_Select", SOUND_GAME);                    
                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }
                else if(response2)
                {
                    if( dlog->code2 )
                        dlog->code2(dlog->data);

                    sndPlay("N_Deselect", SOUND_GAME);                    
                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }
            }break;

        case DIALOG_TWO_RESPONSE:
        {
            int button_wd = max(str_wd(&game_12, wsc, wsc, dlog->response1 ),
                                str_wd(&game_12, wsc, wsc, dlog->response2 ));
            int response1 = 0, response2 = 0;

            if( dlog->flags & DLGFLAG_ARCHITECT )
                button_wd = max(str_wd(&title_12, wsc, wsc, dlog->response1 ), str_wd(&title_12, wsc, wsc, dlog->response2 ));
            else
                button_wd = max(str_wd(&game_12, wsc, wsc, dlog->response1 ), str_wd(&game_12, wsc, wsc, dlog->response2 ));

            if( dlog->flags & DLGFLAG_ARCHITECT )
                response1 = drawMMButton(dlog->response1, 0, 0, wx + winwd/4, ty, wz, button_wd, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
            else
                response1 = (D_MOUSEHIT == drawStdButton(wx + winwd/4, ty, wz, button_wd, BUTTON_HEIGHT*wsc, color, dlog->response1, wsc, !buttonenabled));

            if( dlog->flags & DLGFLAG_ARCHITECT )
                response2 = drawMMButton(dlog->response2, 0, 0, wx + 3*winwd/4, ty, wz, button_wd, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
            else
                response2 = (D_MOUSEHIT == drawStdButton(wx + 3*winwd/4, ty, wz, button_wd, BUTTON_HEIGHT*wsc, color, dlog->response2, wsc, !buttonenabled));

            if(response1)
            {
                if( dlog->code1 )
                    dlog->code1(dlog->data);

                mouseClear();
                freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                collisions_off_for_rest_of_frame = TRUE;
            }
            else if(response2)
            {
                if( dlog->code2 )
                    dlog->code2(dlog->data);

                mouseClear();
                freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                collisions_off_for_rest_of_frame = TRUE;
            }
        } break;

        case DIALOG_TWO_RESPONSE_CANCEL:
            {
                int button_wd, response1 = 0, response2 = 0, cancel = 0;

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    button_wd = max(str_wd(&title_12, wsc, wsc, dlog->response1 ), str_wd(&title_12, wsc, wsc, dlog->response2 ));
                else
                    button_wd = max(str_wd(&game_12, wsc, wsc, dlog->response1 ), str_wd(&game_12, wsc, wsc, dlog->response2 ));

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    response1 = drawMMButton(dlog->response1, 0, 0, wx + winwd/4, ty-30*wsc, wz, button_wd, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
                else
                    response1 = (D_MOUSEHIT == drawStdButton(wx + winwd/4, ty-30*wsc, wz, button_wd, BUTTON_HEIGHT*wsc, color, dlog->response1, wsc, !buttonenabled));

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    response2 = drawMMButton(dlog->response2, 0, 0, wx + 3*winwd/4, ty-30*wsc, wz, button_wd, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
                else
                    response2 = (D_MOUSEHIT == drawStdButton(wx + 3*winwd/4, ty-30*wsc, wz, button_wd, BUTTON_HEIGHT*wsc, color, dlog->response2, wsc, !buttonenabled));

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    cancel = drawMMButton("CancelString", 0, 0, wx + winwd/2, ty, wz, 100*wsc, .75*wsc, MMBUTTON_SMALL|MMBUTTON_ERROR, 0  );
                else
                    cancel = (D_MOUSEHIT == drawStdButton(wx + winwd/2, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, "CancelString", wsc, 0));

                if(response1)
                {
                    if( dlog->code1 )
                        dlog->code1(dlog->data);

                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }
                else if(response2)
                {
                    if( dlog->code2 )
                        dlog->code2(dlog->data);

                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }
                else if( cancel )            
                {
                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }

            } break;

        case DIALOG_THREE_RESPONSE:
            {
                int button_wd, response1 = 0, response2 = 0, response3 = 0;

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    button_wd = max(str_wd(&title_12, wsc, wsc, dlog->response1 ), str_wd(&title_12, wsc, wsc, dlog->response2 ));
                else
                    button_wd = max(str_wd(&game_12, wsc, wsc, dlog->response1 ), str_wd(&game_12, wsc, wsc, dlog->response2 ));

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    response1 = drawMMButton(dlog->response1, 0, 0, wx + winwd/4, ty, wz, button_wd, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
                else
                    response1 = (D_MOUSEHIT == drawStdButton(wx + winwd/4, ty, wz, button_wd, BUTTON_HEIGHT*wsc, color, dlog->response1, wsc, !buttonenabled));

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    response2 = drawMMButton(dlog->response2, 0, 0, wx + winwd/2, ty, wz, button_wd, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0 );
                else
                    response2 = (D_MOUSEHIT == drawStdButton(wx + winwd/2, ty, wz, button_wd, BUTTON_HEIGHT*wsc, color, dlog->response2, wsc, !buttonenabled));

                if( dlog->flags & DLGFLAG_ARCHITECT )
                    response3 = drawMMButton(dlog->response3, 0, 0, wx + 3*winwd/4, ty, wz, 100*wsc, .75*wsc, MMBUTTON_SMALL|!buttonenabled, 0  );
                else
                    response3 = (D_MOUSEHIT == drawStdButton(wx + 3*winwd/4, ty, wz, button_wd, BUTTON_HEIGHT*wsc, color, dlog->response3, wsc, !buttonenabled));

                if(response1)
                {
                    if( dlog->code1 )
                        dlog->code1(dlog->data);

                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }
                else if(response2)
                {
                    if( dlog->code2 )
                        dlog->code2(dlog->data);

                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }
                else if(response3)            
                {
                    if( dlog->code3 )
                        dlog->code3(dlog->data);

                    mouseClear();
                    freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                    collisions_off_for_rest_of_frame = TRUE;
                }

            } break;

        case DIALOG_SMF:
            if(D_MOUSEHIT == drawStdButton(wx + winwd/2, ty, wz, 50*wsc, BUTTON_HEIGHT*wsc, color, "CancelString", wsc, !buttonenabled))
            {
                if( dlog->code1 )
                    dlog->code1(dlog->data);

                mouseClear();
                freeDialog(eaRemove( &dialogQueue, dlog_idx ));
                collisions_off_for_rest_of_frame = TRUE;
             }break;

        case DIALOG_ARCHITECT_OPTIONS:
               if( missionMaker_drawDialogOptions( wx, wy + 10*wsc, wz, wsc, winwd, (int)dlog->data ) || drawMMButton("CancelString", 0, 0, wx + winwd/2, wy + winht - 30*wsc, wz, 100*wsc, wsc, MMBUTTON_SMALL, 0 ) )
             {
                mouseClear();
                dialogRemove(0, missionMakerDialogDummyFunction, 0);
                collisions_off_for_rest_of_frame = TRUE;
            }break;
        case DIALOG_NO_RESPONSE:
        default:
            break;
    }

    BuildCBox( &box, wx, wy, winwd, winht );

    if( mouseCollision( &box ) || dlog->flags & DLGFLAG_LIMIT_MOUSE_TO_DIALOG )
        collisions_off_for_rest_of_frame = TRUE;
}

//-----------------------------------------------------------------------------------------------

char *dialogGetTextEntry(void)
{
    return dialogEdit ? dialogEdit->outputString : NULL;
}

void dialogClearTextEntry(void)
{
    if (dialogEdit)
    {
        smfBlock_Destroy(dialogEdit);
        dialogEdit = 0;
    }
    dialogEntryText[0] = 0;
    // DO NOT CLEAR dialogEditLastSetText here!
}

void dialogSetTextEntry( char * str )
{
    if( !dialogEdit )
    {
        dialogEdit = smfBlock_Create();
        smf_SetFlags(dialogEdit, SMFEditMode_ReadWrite, SMFLineBreakMode_SingleLine, 
            SMFInputMode_AnyTextNoTagsOrCodes, 255,
            SMFScrollMode_InternalScrolling, SMFOutputMode_StripAllTags, SMFDisplayMode_AllCharacters,
            SMFContextMenuMode_None, SMAlignment_Left, 0, 0, 0);
        smf_SetCharacterInsertSounds(dialogEdit, "type2", "mouseover");
    }
    if (!dialogEditLastSetText)
    {
        estrCreate(&dialogEditLastSetText);
    }
    smf_SetRawText(dialogEdit, str, false);
    estrPrintCharString(&dialogEditLastSetText, str);
}

// returns true if first character is special '@' symbol and handles are allowed
bool nameEditDialogCleanup(SMFBlock *pSMFBlock, bool allowHandle)
{
    bool isHandle = false;
    char *text = pSMFBlock->outputString;
    if(text)
    {
         char space[2] = " ";
        char *pch;
        int len;
    
        if(allowHandle && text[0] == '@')
        {
            stripNonRegionChars(&text[1], getCurrentLocale());    // skip first '@'
            isHandle = true;
        }
        else
            stripNonRegionChars(text, getCurrentLocale());

        len = strlen(text);
        // if the string is only whitespace, delete it
        if( strspn(text, space ) == len )
        {
            text[0] = '\0';
        }

        while((pch=strstr(text, "  "))!=NULL)
        {
            strcpy(pch, pch+1);
        }
    }

    return isHandle;
}
