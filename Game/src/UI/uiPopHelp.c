#include "gameComm/wdwbase.h"

#include <utilitieslib/language/AppLocale.h>

#include "uiPopHelp.h"
#include "player/pophelp.h"
#include "uiUtil.h"
#include "uiUtilGame.h"
#include "uiUtilMenu.h"
#include "uiWindows.h"
#include "uiInput.h"
#include "uiEditText.h"
#include "uiTabControl.h"
#include <utilitieslib/utils/mathutil.h>
#include "uiChat.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"
#include "entity/entPlayer.h"
#include "player/player.h"
#include "entity/character_base.h"
#include "entity/entity.h"

#include "entity/entVarUpdate.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>

#include "uiSMFView.h"
#include "formatter/smf_main.h"
#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/StashTable.h>
#include "uiOptions.h"
#include <utilitieslib/utils/scriptvars.h>
#include "clientcomm/clientcomm.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/error.h>
#include "group/groupnetrecv.h"
#include "sound/sound.h"
#include "uiGame.h"

#include "cmdparse/cmdgame.h" //cfg_LockActiveForGR() for the issue 17-18 transition

char *unread_texture_name = "Pop_Help_Icon_On_Alert";
char *glow_texture_name = "Pop_Help_Icon_On_Alert_Glow";
char *read_texture_name = "Pop_Help_Icon_Off_Blue";

char *new_pop_help_sound = "clue1";
char *pop_help_window_open_sound = "help2";
char *pop_help_window_close_sound = "next";

static TextAttribs gTextAttr =
{
    /* piBold        */  (int *)0,
    /* piItalic      */  (int *)0,
    /* piColor       */  (int *)0xffffffff,
    /* piColor2      */  (int *)0,
    /* piColorHover  */  (int *)0xffffffff,
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

SMFView *gPopHelpView;

// which pop help is currently displayed in the pop help text window?
// index into the array of pop help ! icons in the pop help UI
static int g_pop_help_displayed = -1;

static int *delayedInYourFaces = 0;

static void client_translate(char *dest, int dsize, char *from)
{
    Entity *e;
    ScriptVarsTable clientvars = {0};

    e = playerPtr();
    if (e && e->pchar)
    {
        ScriptVarsTablePushVar(&clientvars, "HeroName", e->name);
        ScriptVarsTablePushVar(&clientvars, "Hero", e->name);
    }

    msPrintfVars(menuMessages, dest, dsize, from, &clientvars, 0, 0);
    ScriptVarsTableClear(&clientvars);
}

#define MAX_POP_HELP_SHOWN 5

//static int latest_pop_helps[MAX_POP_HELP_SHOWN];
static int num_pop_helps_up_currently = 0;

// add to the number of pop help icons shown
//
void showPopHelp(int pop_help_event)
{
    Entity *e;

    e = playerPtr();
    if (!e || !e->pl)
        return;

    if(num_pop_helps_up_currently<MAX_POP_HELP_SHOWN)
    {
        e->pl->popHelpLatest[num_pop_helps_up_currently] = pop_help_event;
        num_pop_helps_up_currently++;
    }
}

static int pop_help_anim_point;
static int pop_help_anim_ticks;
#define TICKS_IN_ANIM 6

void closePopHelp(int clicked)
{
    int j;
    Entity *e;

    e = playerPtr();
    if (!e || !e->pl)
        return;

    //slide 'em up
    g_pop_help_displayed = -1;
    num_pop_helps_up_currently--;
    for(j=clicked; j<num_pop_helps_up_currently; j++)
    {
        e->pl->popHelpLatest[j] = e->pl->popHelpLatest[j+1];
    }
    e->pl->popHelpLatest[num_pop_helps_up_currently] = 0;

    if (eaiSize(&delayedInYourFaces))
    {
        int newInYourFace = eaiRemove(&delayedInYourFaces, 0);
        int overwrite;

        if (num_pop_helps_up_currently >= MAX_POP_HELP_SHOWN && !optionGet(kUO_ForcedPopHelpDisable))
        {
            num_pop_helps_up_currently--;
            overwrite = e->pl->popHelpLatest[num_pop_helps_up_currently];
            setPopHelpState(e, overwrite, PopHelpState_Hidden);
            commSendPopHelp(overwrite, PopHelpState_Hidden);
        }

        showPopHelp(newInYourFace);
        g_pop_help_displayed = num_pop_helps_up_currently-1;
        windows_Show("pophelptext");
        setPopHelpState(e, newInYourFace, PopHelpState_Unread);
        commSendPopHelp(newInYourFace, PopHelpState_Unread);
    }
    else
    {
        // check for unshown pop helps to add
        for( j = 0; j < MAX_POP_HELP; j++)
        {
            if (getPopHelpState(e, j) == PopHelpState_Hidden)
            {
                showPopHelp(j);
                setPopHelpState(e, j, PopHelpState_Unread);
                commSendPopHelp(j, PopHelpState_Unread);
                break;
            }
        }
    }
    // for the fancy anim.
    pop_help_anim_point = clicked;
    pop_help_anim_ticks = TICKS_IN_ANIM;
}

// sets the state to UNREAD, unless state was already higher.
// returns 1 if state was unread, 0 in all other cases.
int popHelpEventHappened(int pop_help_event)
{
    int current_state;
    PopHelpItem *phi;
    int overwrite;
    Entity *e;

    e = playerPtr();
    if (!e || !e->pl)
        return 0;

    if (pop_help_event < 0 || pop_help_event >= MAX_POP_HELP)
        return 0;
    current_state = getPopHelpState(e, pop_help_event);
    if (current_state == PopHelpState_Untriggered)
    {
        if (!optionGet(kUO_PopHelpDisable))
        {
            phi = getPopHelpItem(pop_help_event);

            if (!phi)
                return 0;

            phi->timeTriggered = timerMsecsSince2000();

            // if it's inyourface, and there's no room, ditch the last one.
            if (phi && phi->inYourFace && (num_pop_helps_up_currently >= MAX_POP_HELP_SHOWN) && !optionGet(kUO_ForcedPopHelpDisable))
            {
                num_pop_helps_up_currently--;
                overwrite = e->pl->popHelpLatest[num_pop_helps_up_currently];
                setPopHelpState(e, overwrite, PopHelpState_Hidden);
                commSendPopHelp(overwrite, PopHelpState_Hidden);
            }

            // add it to the list, if there's room
            if (num_pop_helps_up_currently < MAX_POP_HELP_SHOWN)
            {
                if (phi && phi->inYourFace && !optionGet(kUO_ForcedPopHelpDisable))
                {
                    if (g_pop_help_displayed == -1)
                    {
                        showPopHelp(pop_help_event);
                        g_pop_help_displayed = num_pop_helps_up_currently-1;
                        windows_Show("pophelptext");
                        setPopHelpState(e, pop_help_event, PopHelpState_Read);
                        commSendPopHelp(pop_help_event, PopHelpState_Read);
                        if (phi->soundName)
                        {
                            sndPlay(phi->soundName, SOUND_VOICEOVER);
                        }
                    }
                    else
                    {
                        eaiPush(&delayedInYourFaces, pop_help_event);
                        setPopHelpState(e, pop_help_event, PopHelpState_Hidden);
                        commSendPopHelp(pop_help_event, PopHelpState_Hidden);
                    }
                }
                else
                {
                    showPopHelp(pop_help_event);
                    setPopHelpState(e, pop_help_event, PopHelpState_Unread);
                    commSendPopHelp(pop_help_event, PopHelpState_Unread);
                }
                commSendPopHelpLatest(e->pl);
            } else {
                setPopHelpState(e, pop_help_event, PopHelpState_Hidden);
                commSendPopHelp(pop_help_event, PopHelpState_Hidden);
            }
            sndPlay( new_pop_help_sound, SOUND_GAME );
            addSystemChatMsg(textStd("PopHelpReceived", textStd(phi->name)), INFO_SVR_COM, 0);
        }
        return 1;
    }
    return 0;
}


int popHelpEventHappenedByTag(char *tag)
{
    int v;

    v = getPopHelpEvent(tag);
    if (v<0)
    {
        if (strStartsWith(tag, "CODEPH"))
        {
            ErrorFilenamef( "pophelp.xls", "POP HELP:  Missing pop help '%s' ", tag );
            return 0;
        }
        ErrorFilenamef( gMapName, "POP HELP:  Bad pop help '%s'", tag );
        return 0;
    }

    return popHelpEventHappened(v);
}


int popHelpWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;
    int j;
    static AtlasTex *unread_texture;
    static AtlasTex *read_texture;
    static AtlasTex *glow_texture;
    AtlasTex *ttex;
    int clicked;
    int phs;
    int dh;
    int anim_offset;
    int o1;
    int mouse_state;
    PopHelpItem *phi;
    Entity *e;
    #define LEADING 10

    if (!unread_texture)
        unread_texture = atlasLoadTexture(unread_texture_name);
    if (!read_texture)
        read_texture = atlasLoadTexture(read_texture_name);
    if (!glow_texture)
        glow_texture = atlasLoadTexture(glow_texture_name);
 
    devassert(loadedPopHelp());

    e = playerPtr();
    if (!e || !e->pl)
        return 0;

    if (optionGet(kUO_PopHelpDisable))
        return 0;

    if( !window_getDims( WDW_POP_HELP, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    //drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );

    if( window_getMode( WDW_POP_HELP ) != WINDOW_DISPLAYING )
        return 0;

    clicked = -1;
    dh = 0;
    if (pop_help_anim_ticks > 0)
    {
        anim_offset = pop_help_anim_ticks * unread_texture->height * scale / TICKS_IN_ANIM;
        pop_help_anim_ticks--;
    }

    o1 = 0;
    for( j = 0; j < num_pop_helps_up_currently; j++)
    {
        float glow_scale;
        S64 current_time = timerMsecsSince2000();
        float timeDiff;

        phi = getPopHelpItem(e->pl->popHelpLatest[j]);

        if (!phi) // shouldn't happen, but it's here to fail gracefully
            continue;

        timeDiff = (current_time - phi->timeTriggered) / 1000.0f;
        glow_scale = MAX(3.0f - (timeDiff * 2), 1.0f);

        phs = getPopHelpState(e, e->pl->popHelpLatest[j]);
        if (phs == PopHelpState_Unread)
        {
            ttex = unread_texture;
        }
        else
        {
            ttex = read_texture;
        }

        if (pop_help_anim_ticks > 0 && j >= pop_help_anim_point)
            o1 = anim_offset;

        mouse_state = drawGenericButton(ttex, x - (ttex->width * scale / 2) + (wd / 2), 
                                              y + dh*(ttex->height+LEADING)*scale + o1, 
                                              z, scale, 0xffffffff, 0xffffffff, 1 );

        if (timeDiff < 2.0f)
        {
            display_sprite( glow_texture, x - (glow_texture->width * scale * glow_scale / 2) + (wd / 2), 
                y - (glow_texture->height * scale * glow_scale / 2) + (ttex->height * scale / 2) + dh*(ttex->height+LEADING)*scale + o1, 
                z, scale * glow_scale, scale * glow_scale, (0xffffff00) | (int) (0xff * MIN(2.0f - timeDiff, 1.0f)));
        }

        if( D_MOUSEHIT == mouse_state)
        {
            clicked = j;
        }
        if (D_MOUSEOVER == mouse_state)
        {
            static SMFBlock *sm = 0;

            if (!sm)
            {
                sm = smfBlock_Create();
            }

            if (phi)
            {
                char *s = textStd(phi->name);
                int twd = str_wd(&game_12, 1.f, 1.f, s);
                int mouseoverX = x - twd;
                int mouseoverY = y + (dh*(ttex->height+LEADING)+ttex->height/2-12)*scale;
                CBox cbox;
                int tempZ = z;

                if (windowUp(WDW_POP_HELP_TEXT))
                {
                    tempZ += 10000;
                    BuildCBox(&cbox, mouseoverX - 3, mouseoverY + 2, twd + 4, 17);

                    drawBox(&cbox, tempZ - 1, color, bcolor|0x000000ff);
                }

                smf_ParseAndDisplay(sm, s, mouseoverX, mouseoverY, tempZ, twd, 0, true, false, &gTextAttr, NULL, 0, false);
            }
        }
        dh++;
    }
    if (clicked >= 0)
    {
        PopHelpItem *phi = getPopHelpItem(e->pl->popHelpLatest[clicked]);
        if (g_pop_help_displayed == clicked)
        {
            windows_Toggle("pophelptext");
            if (windowUp(WDW_POP_HELP_TEXT)) 
            {
                sndPlay(pop_help_window_open_sound, SOUND_GAME);
                if (phi && phi->soundName)
                {
                    sndPlay(phi->soundName, SOUND_VOICEOVER);
                }
            }
            else
            {
                sndPlay(pop_help_window_close_sound, SOUND_GAME);
            }
            g_pop_help_displayed = -1;
        } else 
        {
            windows_Show("pophelptext");
            sndPlay(pop_help_window_open_sound, SOUND_GAME);
            if (phi && phi->soundName)
            {
                sndPlay(phi->soundName, SOUND_VOICEOVER);
            }
            g_pop_help_displayed = clicked;
        }

        
        setPopHelpState(e, e->pl->popHelpLatest[clicked], PopHelpState_Read);
        commSendPopHelp(e->pl->popHelpLatest[clicked], PopHelpState_Read);
    }

    return 0;
}

int popHelpTextWindow()
{
    float x, y, z, wd, ht, scale;
    int color, bcolor;
    static AtlasTex *bang_texture;
    static AtlasTex *dot_texture;
    PopHelpItem *phi;
    int current_pop_help;
    static char *dismiss;
    static int dismiss_wd = 0;
    char translated_text[2000];
    Entity *e;

    e = playerPtr();
    if (!e || !e->pl)
        return 0;

    devassert(loadedPopHelp());
    
    if(!gPopHelpView)
    {
        gPopHelpView = smfview_Create(WDW_POP_HELP_TEXT);
        smfview_SetAttribs(gPopHelpView, &gTextAttr);
    }

    if( !window_getDims( WDW_POP_HELP_TEXT, &x, &y, &z, &wd, &ht, &scale, &color, &bcolor ) )
        return 0;

    if (optionGet(kUO_PopHelpDisable))
        return 0;

    if (g_pop_help_displayed < 0 || g_pop_help_displayed >= MAX_POP_HELP_SHOWN)
        return 0;

    current_pop_help = e->pl->popHelpLatest[g_pop_help_displayed];
    phi = getPopHelpItem(current_pop_help);
    if (phi) {
        client_translate(translated_text, sizeof(translated_text), phi->text);
    } else {
        strcpy(translated_text, "internal error");
        if (isDevelopmentMode())
        {
            snprintf(translated_text, sizeof(translated_text), "Error - popHelp %d not found", current_pop_help );
        }
    }
    drawFrame( PIX3, R10, x, y, z, wd, ht, scale, color, bcolor );

    if( window_getMode( WDW_POP_HELP_TEXT ) != WINDOW_DISPLAYING )
        return 0;

    gTextAttr.piScale = (int*)((int)(1.1f*SMF_FONT_SCALE*scale));
    smfview_SetLocation(gPopHelpView, PIX3*scale, PIX3*scale, 0);
    smfview_SetSize(gPopHelpView, wd - 2*PIX3*scale, ht - (2*PIX3+22)*scale);
    smfview_SetText(gPopHelpView, translated_text);
    smfview_Draw(gPopHelpView);

    dismiss = textStd("ClosePopHelpButton");
    dismiss_wd = str_wd(&game_12, scale, scale, dismiss);

    if( drawStdButton( x+(wd*2/3), y+ht-16*scale, z, dismiss_wd+24*scale, 20*scale, color, dismiss, 1.f, 0 ) == D_MOUSEHIT )
    {
        setPopHelpState(e, current_pop_help, PopHelpState_Dismissed);
        commSendPopHelp(current_pop_help, PopHelpState_Dismissed);
        windows_Hide("pophelptext");
        closePopHelp(g_pop_help_displayed);
        sndPlay( pop_help_window_close_sound, SOUND_GAME );
        commSendPopHelpLatest(e->pl);
        collisions_off_for_rest_of_frame = 1;
    }

//     dismiss = textStd("DisablePopHelpButton");
//     dismiss_wd = str_wd(&game_12, scale, scale, dismiss);
// 
//     if( drawStdButton( x+wd-dismiss_wd/2-22*scale, y+ht-16*scale, z, dismiss_wd+24*scale, 20*scale, color, dismiss, 1.f, 0 ) == D_MOUSEHIT )
//     {
//         setPopHelpState(e, current_pop_help, PopHelpState_Dismissed);
//         commSendPopHelp(current_pop_help, PopHelpState_Dismissed);
//         windows_Hide("pophelptext");
//         hidePopHelp(g_pop_help_clicked);
//         sndPlay( pop_help_window_close_sound, SOUND_GAME );
//         commSendPopHelpLatest(e->pl);
//         optionSet(kUO_PopHelpDisable, true, 0);
//     }

    dismiss = textStd("HidePopHelpButton");
    dismiss_wd = str_wd(&game_12, scale, scale, dismiss);

    if( drawStdButton( x+(wd*1/3), y+ht-16*scale, z, dismiss_wd+24*scale, 20*scale, color, dismiss, 1.f, 0 ) == D_MOUSEHIT )
    {
        windows_Hide("pophelptext");
        g_pop_help_displayed = -1;
        collisions_off_for_rest_of_frame = 1;
    }

    return 0;
}

static void recalcPopHelpVariables(void)
{
    Entity *e;

    e = playerPtr();
    if (!e || !e->pl)
        return;

    // calculate num_pop_helps_up_currently
    for (num_pop_helps_up_currently = 0; 
        num_pop_helps_up_currently < MAX_POP_HELP_SHOWN;
        num_pop_helps_up_currently++)
    {
        if (e->pl->popHelpLatest[num_pop_helps_up_currently] == 0)
            break;
    }

    e->pl->popHelpIsValid = 1;
}

void popHelpTick(void)
{
    Entity *e;
    // This variable is used to ensure that we only try to grant new pop helps once per map.
    static int lastGrantedBaseMapId = -1;
    static StaticMapInfo* info;
    static int myHP = 0;
    static int myDBID = 0;
    // There are two ticks of showBGAlpha() that happen after the Enter Game button
    // has been pressed on the comic load screen.  I need to delay more than one frame after
    // the last call to the load screen before starting a sound, or the sound won't play.
    // Adding one more tick above that in case someone has three ticks in showBGAlpha() for
    // some reason.
    static int thisDelaysNewPopHelpsForSomeTicks = 0;
    
    e = playerPtr();

    if (!loadedPopHelp()
        || current_menu() != MENU_GAME
        || !e || !e->pchar || !e->pl)
    {
        lastGrantedBaseMapId = -1;
        thisDelaysNewPopHelpsForSomeTicks = 0;
        return;
    }

#ifndef TEST_CLIENT
    if (e->pl->popHelpIsValid == 0)
        recalcPopHelpVariables();
#endif

    if (getPopHelpStateByTag(e, "CODEPH_HP") == PopHelpState_Untriggered)
    {
        if (myDBID != e->db_id)
        {
            myHP = e->pchar->attrCur.fHitPoints;
            myDBID = e->db_id;
        }
        else if (e->pchar->attrCur.fHitPoints >= myHP)
        {
            myHP = e->pchar->attrCur.fHitPoints;
        }
        else 
        {
            popHelpEventHappenedByTag("CODEPH_HP");
        }
    }

    if (lastGrantedBaseMapId != game_state.base_map_id)
    {
        // ULTRA HACK to get pop help sounds to play
        switch (thisDelaysNewPopHelpsForSomeTicks)
        {
        case 5: // the number of ticks I need to delay...
            if (e->pl->praetorianProgress == kPraetorianProgress_Tutorial ) 
            {
                int phph = getPopHelpEvent("CODEPH_POP_HELP");
                popHelpEventHappenedByTag("CODEPH_New_Praetorian");
                setPopHelpState(e, phph, PopHelpState_Dismissed);
                commSendPopHelp(phph, PopHelpState_Dismissed);
            }
            else if (game_state.base_map_id == 41) // 41 is the map_id of Destroyed Galaxy City
            {
                popHelpEventHappenedByTag("CODEPH_NTUTORIAL1");
            }
            else
            {
                popHelpEventHappenedByTag("CODEPH_POP_HELP");
            }

            thisDelaysNewPopHelpsForSomeTicks = 0;
            lastGrantedBaseMapId = game_state.base_map_id;
            break;
        default:
            thisDelaysNewPopHelpsForSomeTicks++;
        }
    }

}

//Debug command to reset pop help to it's initial state.
//
void resetPopHelp(void)
{
    Entity *e;

    e = playerPtr();
    if (!e || !e->pl)
        return;

    memset(e->pl->popHelpStatus, 0, sizeof(e->pl->popHelpStatus));
    memset(e->pl->popHelpLatest, 0, sizeof(e->pl->popHelpLatest));
    g_pop_help_displayed = -1;
    num_pop_helps_up_currently = 0;
}

// Called when a character is selected, includes new characters
void initializePopHelp(void)
{
    windows_Hide("pophelptext");
    windows_Show("pophelp");
    g_pop_help_displayed = -1;
}
