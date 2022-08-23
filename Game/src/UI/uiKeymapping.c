//
// keymapping.c -- the name says it all
//----------------------------------------------------------------------------------

#include "UI/uiNet.h"
#include "UI/uiUtil.h"
#include "UI/uiComboBox.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiKeybind.h"
#include "UI/uiKeymapping.h"
#include "UI/uiBox.h"

#include "win/input.h"
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include "entity/entPlayer.h"

#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_base.h"

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>
#include "entity/entity.h"
#include "UI/uiInput.h"
#include <utilitieslib/language/MessageStoreUtil.h>

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//

int g_isBindingKey;
typedef struct entry_type
{
    int     entry_code;
    char name[ MAX_NAME_LEN ];
}EntryType;

enum
{
    ENTRY_NONE,
    ENTRY_READY,
    ENTRY_MODIFIER,
    ENTRY_READY2,
    ENTRY_MODIFIER2,
};

#define SW_NAME(x) {x,#x},

// these are the valid input types and their corresponding strings (which need to be translated of course)
// Compressed for space....MUAHAHAHAHAHAHAH!!!!
static EntryType allowable_inputs[] =
{
    { 0, "NotSet" }, 
        SW_NAME(INP_1) SW_NAME(INP_2) SW_NAME(INP_3) SW_NAME(INP_4)
        SW_NAME(INP_5) SW_NAME(INP_6) SW_NAME(INP_7) SW_NAME(INP_8) SW_NAME(INP_9) SW_NAME(INP_0) SW_NAME(INP_MINUS)
        SW_NAME(INP_EQUALS) SW_NAME(INP_TAB) SW_NAME(INP_Q) SW_NAME(INP_W) SW_NAME(INP_E) SW_NAME(INP_R) SW_NAME(INP_T) 
        SW_NAME(INP_Y) SW_NAME(INP_U) SW_NAME(INP_I) SW_NAME(INP_O) SW_NAME(INP_P) SW_NAME(INP_LBRACKET) SW_NAME(INP_RBRACKET) 
        SW_NAME(INP_RETURN) SW_NAME(INP_LCONTROL) SW_NAME(INP_A) SW_NAME(INP_S) SW_NAME(INP_D) SW_NAME(INP_F)
        SW_NAME(INP_G) SW_NAME(INP_H) SW_NAME(INP_J) SW_NAME(INP_K) SW_NAME(INP_L) SW_NAME(INP_SEMICOLON) SW_NAME(INP_APOSTROPHE) 
        SW_NAME(INP_LSHIFT) SW_NAME(INP_BACKSLASH) SW_NAME(INP_Z) SW_NAME(INP_X) SW_NAME(INP_C) SW_NAME(INP_V)
        SW_NAME(INP_B) SW_NAME(INP_N) SW_NAME(INP_M) SW_NAME(INP_COMMA) SW_NAME(INP_PERIOD) SW_NAME(INP_SLASH) SW_NAME(INP_RSHIFT)
        SW_NAME(INP_SPACE) SW_NAME(INP_F1) SW_NAME(INP_F2) SW_NAME(INP_F3) SW_NAME(INP_F4) SW_NAME(INP_F5) SW_NAME(INP_F6)
        SW_NAME(INP_F7)    SW_NAME(INP_F8)    SW_NAME(INP_F9) SW_NAME(INP_F10) SW_NAME(INP_NUMLOCK) SW_NAME(INP_SCROLL)
        SW_NAME(INP_NUMPAD7) SW_NAME(INP_NUMPAD8) SW_NAME(INP_NUMPAD9) SW_NAME(INP_NUMPAD4) SW_NAME(INP_NUMPAD5)
        SW_NAME(INP_NUMPAD6) SW_NAME(INP_NUMPAD1) SW_NAME(INP_NUMPAD2) SW_NAME(INP_NUMPAD3) SW_NAME(INP_NUMPAD0)
        SW_NAME(INP_OEM_102) SW_NAME(INP_F11) SW_NAME(INP_F12) SW_NAME(INP_F13) SW_NAME(INP_F14) SW_NAME(INP_F15)
        SW_NAME(INP_NUMPADEQUALS) SW_NAME(INP_ABNT_C1) SW_NAME(INP_ABNT_C2) SW_NAME(INP_COLON) SW_NAME(INP_UNDERLINE) 
        SW_NAME(INP_NEXTTRACK) SW_NAME(INP_NUMPADENTER) SW_NAME(INP_RCONTROL) SW_NAME(INP_MUTE) SW_NAME(INP_CALCULATOR)
        SW_NAME(INP_PLAYPAUSE) SW_NAME(INP_MEDIASTOP) SW_NAME(INP_VOLUMEDOWN) SW_NAME(INP_VOLUMEUP) SW_NAME(INP_WEBHOME) 
        SW_NAME(INP_NUMPADCOMMA) SW_NAME(INP_SYSRQ) SW_NAME(INP_PAUSE) SW_NAME(INP_HOME) SW_NAME(INP_END)
        SW_NAME(INP_INSERT) SW_NAME(INP_DELETE) SW_NAME(INP_APPS) SW_NAME(INP_WEBSEARCH) SW_NAME(INP_WEBFAVORITES)
        SW_NAME(INP_WEBREFRESH) SW_NAME(INP_WEBSTOP) SW_NAME(INP_WEBFORWARD) SW_NAME(INP_WEBBACK) SW_NAME(INP_MYCOMPUTER) 
        SW_NAME(INP_MAIL) SW_NAME(INP_MEDIASELECT) SW_NAME(INP_BACKSPACE) SW_NAME(INP_NUMPADSTAR) SW_NAME(INP_LALT) 
        SW_NAME(INP_CAPSLOCK) SW_NAME(INP_NUMPADMINUS) SW_NAME(INP_NUMPADPLUS) SW_NAME(INP_NUMPADPERIOD)
        SW_NAME(INP_NUMPADSLASH) SW_NAME(INP_RALT) SW_NAME(INP_UPARROW) SW_NAME(INP_PGUP) SW_NAME(INP_LEFTARROW)
        SW_NAME(INP_RIGHTARROW) SW_NAME(INP_DOWNARROW) SW_NAME(INP_PGDN) SW_NAME(INP_MBUTTON) SW_NAME(INP_RBUTTON)
        SW_NAME(INP_BUTTON4) SW_NAME(INP_BUTTON5) SW_NAME(INP_BUTTON6) SW_NAME(INP_BUTTON7) SW_NAME(INP_BUTTON8) SW_NAME(INP_TILDE)
        SW_NAME(INP_JOY1)    SW_NAME(INP_JOY2) SW_NAME(INP_JOY3) SW_NAME(INP_JOY4) SW_NAME(INP_JOY5) SW_NAME(INP_JOY6)
        SW_NAME(INP_JOY7) SW_NAME(INP_JOY8) SW_NAME(INP_JOY9) SW_NAME(INP_JOY10) SW_NAME(INP_JOY11) SW_NAME(INP_JOY12) SW_NAME(INP_JOY13)
        SW_NAME(INP_JOY14) SW_NAME(INP_JOY15) SW_NAME(INP_JOY16) SW_NAME(INP_JOY17) SW_NAME(INP_JOY18) SW_NAME(INP_JOY19) SW_NAME(INP_JOY20)
        SW_NAME(INP_JOY21) SW_NAME(INP_JOY22) SW_NAME(INP_JOY23) SW_NAME(INP_JOY24) SW_NAME(INP_JOY25) SW_NAME(INP_JOYPAD_UP) SW_NAME(INP_JOYPAD_DOWN)
        SW_NAME(INP_JOYPAD_LEFT) SW_NAME(INP_JOYPAD_RIGHT) SW_NAME(INP_POV1_UP) SW_NAME(INP_POV1_DOWN) SW_NAME(INP_POV1_LEFT)
        SW_NAME(INP_POV1_RIGHT) SW_NAME(INP_POV2_UP) SW_NAME(INP_POV2_DOWN) SW_NAME(INP_POV2_LEFT) SW_NAME(INP_POV2_RIGHT)
        SW_NAME(INP_POV3_UP) SW_NAME(INP_POV3_DOWN) SW_NAME(INP_POV3_LEFT) SW_NAME(INP_POV3_RIGHT) SW_NAME(INP_JOYSTICK1_UP)
        SW_NAME(INP_JOYSTICK1_DOWN) SW_NAME(INP_JOYSTICK1_LEFT) SW_NAME(INP_JOYSTICK1_RIGHT) SW_NAME(INP_JOYSTICK2_UP)
        SW_NAME(INP_JOYSTICK2_DOWN) SW_NAME(INP_JOYSTICK2_LEFT) SW_NAME(INP_JOYSTICK2_RIGHT) SW_NAME(INP_JOYSTICK3_UP)
        SW_NAME(INP_JOYSTICK3_DOWN) SW_NAME(INP_JOYSTICK3_LEFT) SW_NAME(INP_JOYSTICK3_RIGHT)
        SW_NAME(INP_MOUSE_CHORD)
};    

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

ComboBox gProfileCB = {0};
ComboBox gProfileWindowCB = {0};

enum
{
    BIND_PRIMARY,
    BIND_SECONDARY,
};

typedef struct Bind
{
    int key;
    int mod;
    U32 uID;
}Bind;

// commands are the "Things" that can be bound to keys and remapped,
// they all need to be game commands
typedef struct Command
{
    char    *str;
    char    *name;

    int        state;
    Bind    bind[2];

} Command;

typedef struct CommandCategory
{
    char        *displayName;
    Command        **commands;
} CommandCategory;

typedef struct CommandList
{
    CommandCategory **category;
} CommandList;

CommandList comList = {0};


enum 
{
    KEY_NOCHANGE    = 0,
    KEY_SENDPRIM    = 1,
    KEY_SENDSEC        = 2,
    KEY_CLEAR        = 4,
};

// List of keybind changes to commit to server. Both clears and sets are in same array, although
// index means something different in each case.
char keybindChanged[BIND_MAX][MOD_TOTAL];

int oldBindCount = 0;
KeyBind oldBinds[BIND_MAX];


//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

// Instructions for the parse on how to fill in a defaultKeyBind structure
TokenizerParseInfo ParseCommand[] = {
    { "CmdString",        TOK_STRING(Command, str,0)        },
    { "DisplayName",    TOK_STRING(Command, name,0)        },    
    { "End",            TOK_END,        0                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParseCommandCategory[] = {
    { "DisplayName",    TOK_STRING(CommandCategory, displayName,0)                            },
    { "Command",        TOK_STRUCT(CommandCategory, commands, ParseCommand) },
    { "End",            TOK_END,      0                                                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseAllCommands[] = {
    { "CommandCategory",    TOK_STRUCT(CommandList,category, ParseCommandCategory)    },
    { "End",                TOK_END,      0                                                                                },
    { "", 0, 0 }
};

// reads in the keybinding files 
void ParseCommandList()
{
    if (!ParserLoadFiles("menu", ".cat", "command.bin", 0, ParseAllCommands, &comList, NULL, NULL, NULL))
    {
        printf("Couldn't load default keybindings!\n");
    }
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

//
//
static char* keymap_getName( int mod, int keyindex )
{
    int i;
    static char keyname[128] = {0};

    keyname[0] = '\0';

    if( mod )
    {
        if( mod == MODIFIER_CTRL )
            strcpy( keyname, textStd( "INP_CONTROL" ));
        if( mod == MODIFIER_SHIFT )
            strcpy( keyname, textStd( "INP_SHIFT" ));
        if( mod == MODIFIER_ALT )
            strcpy( keyname, textStd( "INP_ALT" ));
    }

    for( i = 0; i < sizeof(allowable_inputs)/sizeof(EntryType); i++ )
    {
        if( allowable_inputs[i].entry_code == keyindex )
        {
            strcat( keyname, textStd(allowable_inputs[i].name) );
            break;
        }
    }

    if( keyname[0] == '\0' )
        strcpy( keyname, "NotSet" );

    return keyname;
}

//
//
void keymap_getBindValues(void)
{
    int i, j, k, m, slot, foundPri, foundSec;
    Command * cmd;

     for( i = 0; i < eaSize(&comList.category); i++ )
    {
        for( j = 0; j < eaSize(&comList.category[i]->commands); j++)
         {
            foundPri = foundSec = 0;
            cmd = comList.category[i]->commands[j];
             
            // now check for a command of that same type in our bind table
            for( k = 0; k < BIND_MAX; k++ )
            {
                for( m = 0; m < MOD_TOTAL; m++ )
                {
                    if ( game_binds_profile.binds[k].command[m] && stricmp( cmd->str, game_binds_profile.binds[k].command[m] ) == 0 )
                    {
                        if( game_binds_profile.binds[k].secondary )
                        {
                            slot = BIND_SECONDARY;
                            foundSec = 1;
                        }
                        else
                        {
                            slot = BIND_PRIMARY;
                            foundPri = 1;
                        }

                        cmd->bind[slot].key = k;
                        cmd->bind[slot].mod = m;
                        cmd->bind[slot].uID = game_binds_profile.binds[k].uID;
                    }
                }
            }

            if( !foundPri )
            {
                cmd->bind[BIND_PRIMARY].key = 0;
                cmd->bind[BIND_PRIMARY].mod = 0;
                cmd->bind[BIND_PRIMARY].uID = keybindGetNextID();
            }

            if( !foundSec )
            {
                cmd->bind[BIND_SECONDARY].key = 0;
                cmd->bind[BIND_SECONDARY].mod = 0; 
                cmd->bind[BIND_SECONDARY].uID = keybindGetNextID();
            }
        }
    }
}

//
//
int keybind_CheckBinds( char* command )
{
    int i, j;
    Command *cmd;

    if(!command)
        return BIND_PRIMARY;

    keymap_getBindValues();

    for( i = 0; i < eaSize(&comList.category); i++ )
    {
        for( j = 0; j < eaSize(&comList.category[i]->commands); j++)
        {
            cmd = comList.category[i]->commands[j];

            if( stricmp( cmd->str, command ) == 0 )
            {
                if( cmd->bind[0].key && cmd->bind[1].key  )
                {
                    // overwrite the older bind
                    if( cmd->bind[0].uID < cmd->bind[1].uID )
                        return BIND_PRIMARY;
                    else
                        return BIND_SECONDARY;
                }
                else if( cmd->bind[0].key )
                    return BIND_SECONDARY;
                else
                    return BIND_PRIMARY;
            }
        }
    }

    return BIND_PRIMARY;
}

//
//
void keymap_setKey( Command * cmd, int slot )
{
    int i, j;
    KeyBind *kb;
    Command *command;

    // now check for a command of that same type in our bind table
     for( i = 0; i < BIND_MAX; i++ )
    {
        for( j = 0; j < MOD_TOTAL; j++ )
        {
            // found a bind with same command, make sure its one of our two binds or clear it
            if ( game_binds_profile.binds[i].command[j] && stricmp( cmd->str, game_binds_profile.binds[i].command[j] ) == 0 )
            {
                 if( (cmd->bind[0].key != i || cmd->bind[0].mod != j) &&
                    (cmd->bind[1].key != i || cmd->bind[1].mod != j ) )
                {
                    memset(game_binds_profile.binds[i].command[j], 0, sizeof(char)*BIND_MAX );
                    keybindChanged[i][j] |= KEY_CLEAR;
                    keybind_setClear( playerPtr()->pl->keyProfile, i, j );
                }
            }
        }
    }

    // same key can't bind to two differnt things
    for( i = 0; i < eaSize(&comList.category); i++ )
    {
        for( j = 0; j < eaSize(&comList.category[i]->commands); j++)
        {
            command = comList.category[i]->commands[j];

            if( command == cmd )
                continue;

            if( cmd->bind[slot].key == command->bind[slot].key  &&
                cmd->bind[slot].mod == command->bind[slot].mod )
            {
                command->bind[slot].key = 0;
                command->bind[slot].mod = 0;
                command->bind[slot].uID = keybindGetNextID();
            }
        }
    }

    kb = &game_binds_profile.binds[cmd->bind[slot].key];

    // copy the new one in
    if( !kb->command[cmd->bind[slot].mod] )
        kb->command[cmd->bind[slot].mod] = calloc( 1, sizeof(char)*BIND_MAX);
    strncpyt( kb->command[cmd->bind[slot].mod], cmd->str, BIND_MAX );
    kb->secondary = slot;
    kb->uID = cmd->bind[slot].uID;

    // tell the server so this can persist
    if (slot)
        keybindChanged[cmd->bind[slot].key][cmd->bind[slot].mod] |= KEY_SENDSEC;
    else
        keybindChanged[cmd->bind[slot].key][cmd->bind[slot].mod] |= KEY_SENDPRIM;
    keybind_set( playerPtr()->pl->keyProfile, cmd->bind[slot].key, cmd->bind[slot].mod, slot, cmd->str );

    cmd->state = ENTRY_NONE;
    keymap_getBindValues();

}

void keymap_clearCommands( Command * cmd )
{
    int i, j;

    for( i = 0; i < eaSize(&comList.category); i++ )
    {
        for( j = 0; j < eaSize(&comList.category[i]->commands); j++)
        {
            comList.category[i]->commands[j]->state = ENTRY_NONE;
        }
    }
    g_isBindingKey = false;
}
//
//
void drawKeyChoice( Command * cmd, float x, float y, float z, float sc, float x2, float x3)
{
    float tsc = sc;
     int i, color = CLR_WHITE; 
    static int slot = 0;

    // Name and primary slot
    //-------------------------------------------------------------------------------------
       if ( (cmd->state == ENTRY_READY || cmd->state == ENTRY_MODIFIER) && slot == BIND_PRIMARY )
    {
        tsc *= 1.1f;
        color = CLR_GREEN;
    }
    else if( cmd->bind[0].key == 0  )
        color = CLR_GREY;

    if( selectableText(x, y, z, tsc, cmd->name, CLR_WHITE, CLR_WHITE, FALSE, TRUE ) ||
         selectableText(x2, y, z, tsc, keymap_getName( cmd->bind[0].mod, cmd->bind[0].key ), color, color, FALSE, TRUE ) )
    {
        if( !cmd->state )
        {
            keymap_clearCommands( cmd );
            cmd->state = ENTRY_READY;
            g_isBindingKey = true;
        }
        else
        {
            keymap_clearCommands( cmd );
            g_isBindingKey = false;
        }

        slot = BIND_PRIMARY;
    }

    // secondary slot
    //-------------------------------------------------------------------------------------
    if ( (cmd->state == ENTRY_READY || cmd->state == ENTRY_MODIFIER) && slot == BIND_SECONDARY )
    {
        tsc *= 1.1f;
        color = CLR_GREEN;
    }
    else if( cmd->bind[1].key == 0  )
        color = CLR_GREY;
    else
        color = CLR_WHITE;

    if( selectableText(x3, y, z, tsc, keymap_getName( cmd->bind[1].mod, cmd->bind[1].key ), color, color, FALSE, TRUE ) )
    {
        if( !cmd->state )
        {
            keymap_clearCommands( cmd );
            cmd->state = ENTRY_READY;
            g_isBindingKey = true;
        }
        else {
            keymap_clearCommands( cmd );
            g_isBindingKey = false;
        }

        slot = BIND_SECONDARY;
    }

    //if they clicked on either bind they are now in the ready state
    // first input
    //-------------------------------------------------------------------------------------

    // check for input
    if( cmd->state == ENTRY_READY )
     {
        for( i = 0; i < sizeof(allowable_inputs)/sizeof(EntryType); i++ )
        {
            if( inpEdge(allowable_inputs[i].entry_code) )
            {
                cmd->bind[slot].key = allowable_inputs[i].entry_code;
                cmd->state = ENTRY_MODIFIER;
            }
        }
    }

    // secondary input
    //-------------------------------------------------------------------------------------
    // now that we have input, either wait for a keyup or a secondary button

    if( cmd->state == ENTRY_MODIFIER )
    {
        // the key they pressed was released, save as bind without modifier
        if( !inpLevel(cmd->bind[slot].key) )
        {
            cmd->bind[slot].mod = 0;
            cmd->bind[slot].uID = keybindGetNextID();
            keymap_setKey( cmd, slot );
            g_isBindingKey = false;
            mouseClear();
        }
        else
        {
            // if the first key they pressed was a modifier, wait for non-modifier input
            if(  keybind_modifier( cmd->bind[slot].key ) )
            {
                for( i = 0; i < sizeof(allowable_inputs)/sizeof(EntryType); i++ )
                {
                    if( allowable_inputs[i].entry_code != cmd->bind[slot].key && inpEdge(allowable_inputs[i].entry_code) )
                    {
                        if( !keybind_modifier(allowable_inputs[i].entry_code) )
                        {
                            cmd->bind[slot].mod = keybind_modifier(cmd->bind[slot].key);
                             cmd->bind[slot].key = allowable_inputs[i].entry_code;
                            cmd->bind[slot].uID = keybindGetNextID();
                            keymap_setKey( cmd, slot );
                            g_isBindingKey = false;
                            mouseClear();
                        }
                    }
                }
            }
            else // the first key they pressed was not a modifier, wait for a modifier key
            {
                for( i = 0; i < sizeof(allowable_inputs)/sizeof(EntryType); i++ )
                {
                    if( allowable_inputs[i].entry_code != cmd->bind[slot].key && inpEdge(allowable_inputs[i].entry_code) )
                    {
                        if( keybind_modifier(allowable_inputs[i].entry_code) )
                        {
                            cmd->bind[slot].mod = keybind_modifier(allowable_inputs[i].entry_code);
                            cmd->bind[slot].uID = keybindGetNextID();
                            keymap_setKey( cmd, slot );
                            g_isBindingKey = false;    
                            mouseClear();
                        }
                        else
                        {
                            cmd->bind[slot].key = allowable_inputs[i].entry_code;
                            cmd->bind[slot].uID = keybindGetNextID();
                            keymap_setKey( cmd, slot );
                            g_isBindingKey = false;
                            mouseClear();
                        }
                    }
                }
            } // end else !modifier
        }// end else released
    }// end if modify state
}

const char* keymap_getProfileName(const char* displayName)
{
    int i;

    for( i = 0; i < eaSize( &data_kbp.profiles ); i++ )
    {
        if( stricmp(data_kbp.profiles[i]->displayName, displayName) == 0 )
            return data_kbp.profiles[i]->name;
    }

    return NULL;
}

void keymap_drawCBox(float x, float y, float z, float sc) {
    Entity *e = playerPtr();
    const char *newProfile = 0;
    const char *strProfileName = NULL;
    combobox_setLoc(&gProfileWindowCB,x,y,z);
    newProfile = combobox_display( &gProfileWindowCB );


    if( newProfile 
        && (strProfileName = keymap_getProfileName(newProfile)) 
        && stricmp( strProfileName, e->pl->keyProfile ) != 0 )
    {
        keyprofile_send( strProfileName );
        strcpy( e->pl->keyProfile, strProfileName );
        keybinds_init( strProfileName, &game_binds_profile, e->pl->keybinds );
        keymap_getBindValues();
    }
}

void keymap_drawReset(float x, float y, float z, float sc) {
    Entity *e = playerPtr();
    if( D_MOUSEHIT == drawStdButton( x, y, z, 100*sc, 20*sc, CLR_DARK_RED, "ResetKeyBinds", sc, 0 ) )
    {
        keybind_reset( e->pl->keyProfile, &game_binds_profile );
        keymap_getBindValues();
    }
}

float keymap_drawBinds(float x, float y, float z, float sc, float width) {
    float oldy = y;
    int i, j;
    Entity *e = playerPtr();

    for( i = 0; i < eaSize(&comList.category); i++ )
    {
        font( &game_12 );
        font_color( CLR_YELLOW, CLR_RED );
        prnt( x, y, z, sc, sc, comList.category[i]->displayName );
        y += 20*sc;

        for( j = 0; j < eaSize(&comList.category[i]->commands); j++)
        {
            drawKeyChoice(comList.category[i]->commands[j], x, y, z,sc,x+width/10*4,x+width/10*7);
            y += 15*sc; 
        }

        y += 20*sc;
    }

    return y -oldy;
}


void keymap_initProfileChoice(void)
{
    int i, count = 0, curr = 0;
    Entity *e = playerPtr();
    static char **strings;

    eaCreate( &strings );
    for( i = 0; i < eaSize( &data_kbp.profiles ); i++ )
    {
        if( e->access_level < 9 && stricmp(data_kbp.profiles[i]->name, "Dev") == 0 )
            continue;

        eaPush( &strings, data_kbp.profiles[i]->displayName);

        if( stricmp(e->pl->keyProfile, data_kbp.profiles[i]->name) == 0 )
            curr = count;

        count++;
    }
    comboboxClear( &gProfileCB);
    combobox_init( &gProfileCB, 741, 100, 25, 150, 20, 100, strings, 0 );
    gProfileCB.currChoice = curr;
    comboboxClear( &gProfileWindowCB);
    combobox_init( &gProfileWindowCB, 741, 100, 25, 150, 20, 100, strings, WDW_OPTIONS );
    gProfileWindowCB.currChoice = curr;

}

// Save a copy of your keybinds to undo to later
void keybind_saveUndo()
{
    int i;
    Entity *e = playerPtr();

    // In the front end, no entity
    if (!e || !e->pl || !e->pl->keybinds)
        return;

    for( i = 0; i < BIND_MAX; i++ )
    {
        oldBinds[i].key = e->pl->keybinds[i].key;
        oldBinds[i].modifier = e->pl->keybinds[i].modifier;
        oldBinds[i].secondary = e->pl->keybinds[i].secondary;
        if (e->pl->keybinds[i].command[0])
        {
            if (!oldBinds[i].command[0])
                oldBinds[i].command[0] = malloc( sizeof(char)*BIND_MAX );
            strncpyt(oldBinds[i].command[0], e->pl->keybinds[i].command[0], BIND_MAX);
        }
    }
    oldBindCount = e->pl->keybind_count;
}

// Undo any changes made to keybinds
void keybind_undoChanges()
{
    int changed = 0, i, j;
    Entity *e = playerPtr();

    // In the front end, no entity
    if (!e || !e->pl || !e->pl->keybinds)
        return;

    for( i = 0; i < BIND_MAX; i++ )
    {
        for( j = 0; j < MOD_TOTAL; j++ )
        {
            changed = 1;
            keybindChanged[i][j] = 0;
        }
    }
    if (changed)
    {
        for( i = 0; i < BIND_MAX; i++ )
        {
            e->pl->keybinds[i].key = oldBinds[i].key;
            e->pl->keybinds[i].modifier = oldBinds[i].modifier;
            e->pl->keybinds[i].secondary = oldBinds[i].secondary;
            if (oldBinds[i].command[0])
            {
                if (!e->pl->keybinds[i].command[0])
                    e->pl->keybinds[i].command[0] = malloc( sizeof(char)*BIND_MAX );
                strncpyt(e->pl->keybinds[i].command[0], oldBinds[i].command[0], BIND_MAX);
            }
            else if (e->pl->keybinds[i].command[0])
            { //unbind if it it was bound
                strcpy(e->pl->keybinds[i].command[0],"nop");
            }
        }
        e->pl->keybind_count = oldBindCount;
        
        keybinds_init( e->pl->keyProfile, &game_binds_profile, e->pl->keybinds );        
    }
}
// Clear out any pending changes to keybinds
void keybind_clearChanges()
{
    int i,j;
    for( i = 0; i < BIND_MAX; i++ )
    {
        for( j = 0; j < MOD_TOTAL; j++ )
        {
            keybindChanged[i][j] = 0;
        }
    }
}

// Send all of the changed keybinds to the server.
void keybind_saveChanges()
{
    int i, j, k;
    Entity *e = playerPtr();
    for( i = 0; i < BIND_MAX; i++ )
    {
        for( j = 0; j < MOD_TOTAL; j++ )
        {            
            if (keybindChanged[i][j] & KEY_CLEAR)
            {
                keybind_sendClear( e->pl->keyProfile, i, j );
            }
        }
    }
    for( i = 0; i < BIND_MAX; i++ )
    {
        for( j = 0; j < MOD_TOTAL; j++ )
        {
            int slot = -1;
            if (keybindChanged[i][j] & KEY_SENDPRIM)
            {
                for( k = 0; k < BIND_MAX; k++ )
                {
                    if( e->pl->keybinds[k].key == (i & 0xff) && e->pl->keybinds[k].modifier == j 
                        && e->pl->keybinds[k].secondary == 0 && e->pl->keybinds[k].command)
                    {
                        keybind_send( e->pl->keyProfile, i, j, 0, e->pl->keybinds[k].command[0] );
                    }
                }

            }
            if (keybindChanged[i][j] & KEY_SENDSEC)
            {
                for( k = 0; k < BIND_MAX; k++ )
                {
                    if( e->pl->keybinds[k].key == (i & 0xff) && e->pl->keybinds[k].modifier == j 
                        && e->pl->keybinds[k].secondary == 1 && e->pl->keybinds[k].command)
                    {
                        keybind_send( e->pl->keyProfile, i, j, 1, e->pl->keybinds[k].command[0] );
                    }
                }

            }
            keybindChanged[i][j] = 0;
        }
    }
}
