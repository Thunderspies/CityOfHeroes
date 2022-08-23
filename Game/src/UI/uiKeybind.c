
#include "UI/uiKeybind.h"
#include "win/input.h"
#include "UI/uiNet.h"
#include "time.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include "player/player.h"
#include "entity/entPlayer.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiConsole.h"
#include "UI/uiKeymapping.h"
#include "entity/entVarUpdate.h"
#include "UI/uiChat.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/sysutil.h>
#include "UI/uiChat.h"
#include "UI/sprite/sprite_text.h"
#include "entity/entity.h"
#include <utilitieslib/utils/file.h>
#include "winuser.h"
#include "UI/uiFocus.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/EString.h>
#include "formatter/smf_main.h"

// holder for parsed data list - bad structure naming here
KeyBindProfiles data_kbp = {0};

 // key binding profile stack
KeyBindProfile **kbProfiles = 0;

// counter so we know the order things were bound
U32 s_ID = 0;

U32 keybindGetNextID()
{
    return (++s_ID)?s_ID:++s_ID;
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

// Instructions for the parse on how to fill in a defaultKeyBind structure
TokenizerParseInfo ParseKeyBind[] = {
    { "key",        TOK_STRING(defaultKeyBind, key,0)        },
    { "command",    TOK_STRING(defaultKeyBind, command,0)    },
    { "End",        TOK_END,        0                                    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseKeyBindList[] = {
    { "DisplayName",    TOK_STRING(KeyBindList,displayName,0)        },
    { "Name",            TOK_STRING(KeyBindList,name,0)            },
    { "KeyBind",        TOK_STRUCT(KeyBindList,binds, ParseKeyBind) },
    { "End",            TOK_END,      0                                        },
    { "", 0, 0 }
};

TokenizerParseInfo ParseKeyProfiles[] = {
    { "KeyProfile",        TOK_STRUCT(KeyBindProfiles,profiles,ParseKeyBindList) },
    { "", 0, 0 }
};

// reads in the keybinding files
void ParseBindings()
{
    clock_t start, finish;
    double duration;
    start = clock();
    
    if (game_state.create_bins)
    {
        // If we're creating bins, load both of them

        if (!ParserLoadFiles("menu/Koreakey", ".kb", "kbkorea.bin", 0, ParseKeyProfiles, &data_kbp, NULL, NULL, NULL))
        {
            FatalErrorf("Couldn't load english keybindings!\n");
        }

        ParserDestroyStruct(ParseKeyProfiles, &data_kbp);
        memset(&data_kbp,0,sizeof(KeyBindProfiles));

        if (!ParserLoadFiles("menu/Defaultkey", ".kb", "kb.bin", 0, ParseKeyProfiles, &data_kbp, NULL, NULL, NULL))
        {
            FatalErrorf("Couldn't load korean keybindings!\n");
        }
    }
    else if (getCurrentLocale() == 3) 
    {
        if (!ParserLoadFiles("menu/Koreakey", ".kb", "kbkorea.bin", 0, ParseKeyProfiles, &data_kbp, NULL, NULL, NULL))
        {
            printf("Couldn't load default keybindings!\n");
        }
    }
    else
    {
        if (!ParserLoadFiles("menu/Defaultkey", ".kb", "kb.bin", 0, ParseKeyProfiles, &data_kbp, NULL, NULL, NULL))
        {
            printf("Couldn't load default keybindings!\n");
        }
    }

    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    writeConsole(OUTPUT_VERBOSE, "Loaded keybinds in %2.3fs", duration);
}


//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

typedef struct
{
    int        value;
    char    *name;
    char    *name2;
} SwitchToken;

#define     SW_NAME(x) {x,#x},
#define     SW_NAME2(x,c) {x,#x,c},

// Mapping between key value and keyname.
SwitchToken    key_names[] =
{
    SW_NAME(INP_1)
    SW_NAME(INP_2)
    SW_NAME(INP_3)
    SW_NAME(INP_5)
    SW_NAME(INP_6)
    SW_NAME(INP_7)
    SW_NAME(INP_8)
    SW_NAME(INP_MULTIPLY)
    SW_NAME(INP_0)
    SW_NAME(INP_BACKSPACE)
    SW_NAME(INP_Q)
    SW_NAME(INP_W)
    SW_NAME(INP_E)
    SW_NAME(INP_R)
    SW_NAME(INP_Y)
    SW_NAME(INP_U)
    SW_NAME(INP_I)
    SW_NAME(INP_O)
    SW_NAME(INP_P)
    SW_NAME(INP_A)
    SW_NAME(INP_S)
    SW_NAME(INP_D)
    SW_NAME(INP_F)
    SW_NAME(INP_G)
    SW_NAME(INP_H)
    SW_NAME(INP_J)
    SW_NAME(INP_K)
    SW_NAME(INP_L)
    SW_NAME(INP_LCONTROL)
    SW_NAME(INP_LSHIFT)
    SW_NAME(INP_Z)
    SW_NAME(INP_X)
    SW_NAME(INP_C)
    SW_NAME(INP_V)
    SW_NAME(INP_B)
    SW_NAME(INP_N)
    SW_NAME(INP_M)
    SW_NAME(INP_RSHIFT)
    SW_NAME(INP_SPACE)
    SW_NAME(INP_SUBTRACT)
    SW_NAME(INP_CAPITAL)
    SW_NAME(INP_F1)
    SW_NAME(INP_F2)
    SW_NAME(INP_F3)
    SW_NAME(INP_F4)
    SW_NAME(INP_F5)
    SW_NAME(INP_F6)
    SW_NAME(INP_F7)
    SW_NAME(INP_F8)
    SW_NAME(INP_F9)
    SW_NAME(INP_F10)
    SW_NAME(INP_NUMLOCK)
    SW_NAME(INP_SCROLL)
    SW_NAME(INP_NUMPAD7)
    SW_NAME(INP_NUMPAD8)
    SW_NAME(INP_NUMPAD9)
    SW_NAME(INP_NUMPAD4)
    SW_NAME(INP_NUMPAD5)
    SW_NAME(INP_NUMPAD6)
    SW_NAME(INP_ADD)
    SW_NAME(INP_NUMPAD1)
    SW_NAME(INP_NUMPAD2)
    SW_NAME(INP_NUMPAD3)
    SW_NAME(INP_NUMPAD0)
    SW_NAME(INP_DECIMAL)
    SW_NAME(INP_F11)
    SW_NAME(INP_F12)
    SW_NAME(INP_F13)
    SW_NAME(INP_F14)
    SW_NAME(INP_F15)
    SW_NAME(INP_KANA)
    SW_NAME(INP_CONVERT)
    SW_NAME(INP_KANJI)
    SW_NAME(INP_RCONTROL)
    SW_NAME(INP_4)
    SW_NAME(INP_T)
    SW_NAME(INP_DIVIDE)
    SW_NAME(INP_9)
    SW_NAME(INP_TAB)
    SW_NAME(INP_HOME)
    SW_NAME(INP_UP)
    SW_NAME(INP_LEFT)
    SW_NAME(INP_RIGHT)
    SW_NAME(INP_END)
    SW_NAME(INP_DOWN)
    SW_NAME(INP_INSERT)
    SW_NAME(INP_DELETE)
    SW_NAME(INP_LWIN)
    SW_NAME(INP_RWIN)
    SW_NAME(INP_APPS)
    SW_NAME(INP_LBUTTON)
    SW_NAME(INP_RBUTTON)
    SW_NAME(INP_MBUTTON)
    SW_NAME(INP_BUTTON4)
    SW_NAME(INP_BUTTON5)
    SW_NAME(INP_BUTTON6)
    SW_NAME(INP_BUTTON7)
    SW_NAME(INP_BUTTON8)
    SW_NAME(INP_MOUSEWHEEL)
    SW_NAME(INP_CONTROL)
    SW_NAME(INP_PAUSE)
    SW_NAME(INP_SYSRQ)
    SW_NAME(INP_COMMA)
    SW_NAME(INP_EQUALS)
    SW_NAME(INP_NUMPADENTER) 

    SW_NAME2(INP_PRIOR,"PAGEUP")
    SW_NAME2(INP_NEXT,"PAGEDOWN")
    SW_NAME2(INP_RBRACKET,"]")
    SW_NAME2(INP_TILDE,"`")
    SW_NAME2(INP_LBRACKET,"[")
    SW_NAME2(INP_MINUS,"-")
    SW_NAME2(INP_SEMICOLON,";")
    SW_NAME2(INP_APOSTROPHE,"'")
    SW_NAME2(INP_BACKSLASH,"\\")
    SW_NAME2(INP_SLASH,"/")
    SW_NAME2(INP_PERIOD,".")
    SW_NAME2(INP_RETURN,"ENTER")
    SW_NAME2(INP_ESCAPE,"ESC")
    SW_NAME2(INP_LMENU,"LALT")
    SW_NAME2(INP_RMENU,"RALT")
    SW_NAME2(INP_ALT,"ALT")
    SW_NAME2(INP_CONTROL,"CTRL")
    SW_NAME2(INP_LCONTROL,"LCTRL")
    SW_NAME2(INP_RCONTROL,"RCTRL")
    SW_NAME2(INP_SHIFT,"SHIFT")
    SW_NAME2(INP_UP,"UPARROW")
    SW_NAME2(INP_DOWN,"DOWNARROW")
    SW_NAME2(INP_LEFT,"LEFTARROW")
    SW_NAME2(INP_RIGHT,"RIGHTARROW")

    SW_NAME(INP_JOY1)    
    SW_NAME(INP_JOY2)
    SW_NAME(INP_JOY3)
    SW_NAME(INP_JOY4)
    SW_NAME(INP_JOY5)
    SW_NAME(INP_JOY6)
    SW_NAME(INP_JOY7)
    SW_NAME(INP_JOY8)
    SW_NAME(INP_JOY9)
    SW_NAME(INP_JOY10)

    SW_NAME(INP_JOY11)
    SW_NAME(INP_JOY12)
    SW_NAME(INP_JOY13)
    SW_NAME(INP_JOY14)
    SW_NAME(INP_JOY15)
    SW_NAME(INP_JOY16)
    SW_NAME(INP_JOY17)
    SW_NAME(INP_JOY18)
    SW_NAME(INP_JOY19)
    SW_NAME(INP_JOY20)
    SW_NAME(INP_JOY21)
    SW_NAME(INP_JOY22)
    SW_NAME(INP_JOY23)
    SW_NAME(INP_JOY24)
    SW_NAME(INP_JOY25)

    SW_NAME(INP_JOYPAD_UP)
    SW_NAME(INP_JOYPAD_DOWN)
    SW_NAME(INP_JOYPAD_LEFT)
    SW_NAME(INP_JOYPAD_RIGHT)

    SW_NAME(INP_POV1_UP)
    SW_NAME(INP_POV1_DOWN)
    SW_NAME(INP_POV1_LEFT)
    SW_NAME(INP_POV1_RIGHT)

    SW_NAME(INP_POV2_UP)
    SW_NAME(INP_POV2_DOWN)
    SW_NAME(INP_POV2_LEFT)
    SW_NAME(INP_POV2_RIGHT)

    SW_NAME(INP_POV3_UP)
    SW_NAME(INP_POV3_DOWN)
    SW_NAME(INP_POV3_LEFT)
    SW_NAME(INP_POV3_RIGHT)


    SW_NAME(INP_JOYSTICK1_UP)
    SW_NAME(INP_JOYSTICK1_DOWN)
    SW_NAME(INP_JOYSTICK1_LEFT)
    SW_NAME(INP_JOYSTICK1_RIGHT)

    SW_NAME(INP_JOYSTICK2_UP)
    SW_NAME(INP_JOYSTICK2_DOWN)
    SW_NAME(INP_JOYSTICK2_LEFT)
    SW_NAME(INP_JOYSTICK2_RIGHT)

    SW_NAME(INP_JOYSTICK3_UP)
    SW_NAME(INP_JOYSTICK3_DOWN)
    SW_NAME(INP_JOYSTICK3_LEFT)
    SW_NAME(INP_JOYSTICK3_RIGHT)

    SW_NAME2(INP_MOUSE_CHORD,            "MouseChord")
    SW_NAME2(INP_LCLICK,                "LeftClick")
    SW_NAME2(INP_MCLICK,                "MiddleClick")
    SW_NAME2(INP_RCLICK,                "RightClick")
    SW_NAME2(INP_LDRAG,                    "LeftDrag")
    SW_NAME2(INP_MDRAG,                    "MiddleDrag")
    SW_NAME2(INP_RDRAG,                    "RightDrag")
    SW_NAME2(INP_MOUSEWHEEL_FORWARD,    "wheelplus")
    SW_NAME2(INP_MOUSEWHEEL_BACKWARD,    "wheelminus")  
    SW_NAME2(INP_LDBLCLICK,                "LeftDoubleClick")
    SW_NAME2(INP_MDBLCLICK,                "MiddleDoubleClick")
    SW_NAME2(INP_RDBLCLICK,                "RightDoubleClick")    
    //     SW_NAME(INP_CIRCUMFLEX) SW_NAME(INP_NUMPADCOMMA) SW_NAME(INP_NUMPADEQUALS) SW_NAME(INP_GRAVE)
    // SW_NAME(INP_NOCONVERT) SW_NAME(INP_AT)    SW_NAME(INP_COLON )    SW_NAME(INP_UNLABELED)    SW_NAME(INP_UNDERLINE)
    // SW_NAME(INP_SYSRQ)        SW_NAME(INP_YEN)    SW_NAME(INP_AX)
    // SW_NAME(INP_STOP)
};

//
//
static int __cdecl compareKeyNames(const SwitchToken* t1, const SwitchToken* t2)
{
    return stricmp(    t1->name2 ? t1->name2 : (t1->name + 4),
                    t2->name2 ? t2->name2 : (t2->name + 4));
}

//
//
static void sortKeyNames()
{
    static int sorted = 0;

    if(!sorted)
    {
        sorted = 1;

        qsort(key_names, ARRAY_SIZE(key_names), sizeof(key_names[0]), compareKeyNames);
    }
}

//
//
char *keyName(int num)
{
    int        i;

    sortKeyNames();

    for( i = 0; i < ARRAY_SIZE(key_names); i++ )
    {
        if (key_names[i].value == num)
            return &key_names[i].name[4];
    }
    return 0;
}

//
//
static int keyValue(char *name)
{
    int        i;

    if( !name )
        return 0;

    sortKeyNames();

    for( i = 0; i < ARRAY_SIZE(key_names); i++ )
    {
        if( stricmp( name, &key_names[i].name[4] ) == 0 ||
            ( key_names[i].name2 && stricmp( name, key_names[i].name2 ) == 0 ) )
        {
            return key_names[i].value;
        }
    }

    return 0;
}

int keybind_modifier( int keyvalue )
{
    if( keyvalue == INP_LCONTROL || keyvalue == INP_RCONTROL || keyvalue == INP_CONTROL)
        return MODIFIER_CTRL;
    else if( keyvalue == INP_LSHIFT || keyvalue == INP_RSHIFT || keyvalue == INP_SHIFT )
        return MODIFIER_SHIFT;
    else if( keyvalue == INP_LMENU || keyvalue == INP_RMENU || keyvalue == INP_ALT )
        return MODIFIER_ALT;
    else
        return MODIFIER_NONE;
}

static int keybind_idFromModifier( int id )
{
    if( id == MODIFIER_CTRL )
        return INP_CONTROL;
    else if( id == MODIFIER_SHIFT )
        return INP_SHIFT;
    else if( id == MODIFIER_ALT )
        return INP_ALT;
    else
        return 0;
}


// these function mimic the server side saving of the keybinding list
void keybind_set( const char *profile, int key, int mod, int secondary, const char *command )
{
    int i;
    Entity *e = playerPtr();

    if( stricmp( profile, e->pl->keyProfile ) != 0 )
        return;

    if (!e->pl->keybinds) {
        // Happens at character selection screen
        conPrintf("Failure to bind key");
        return;
    }

    // first see if we have this key saved already
    for( i = 0; i < BIND_MAX; i++ )
    {
        if( e->pl->keybinds[i].key == (key & 0xff) && e->pl->keybinds[i].modifier == mod )
        {
            if( command )
                strncpyt( e->pl->keybinds[i].command[0], command, BIND_MAX);
            else
                e->pl->keybinds[i].command[0][0] = '\0';

            e->pl->keybinds[i].secondary = secondary;
            e->pl->keybinds[i].uID = keybindGetNextID();
            return;
        }
    }

    if( command && e->pl->keybind_count <= BIND_MAX-1 )
    {
        e->pl->keybinds[e->pl->keybind_count].key = key;
        e->pl->keybinds[e->pl->keybind_count].modifier = mod;
        e->pl->keybinds[e->pl->keybind_count].secondary = secondary;
        e->pl->keybinds[e->pl->keybind_count].uID = keybindGetNextID();

        if( !e->pl->keybinds[e->pl->keybind_count].command[0] )
            e->pl->keybinds[e->pl->keybind_count].command[0] = malloc( sizeof(char)*BIND_MAX );
        strncpyt( e->pl->keybinds[e->pl->keybind_count].command[0], command, BIND_MAX);

        e->pl->keybind_count++;
    }
}

void keybind_setClear( const char *profile, int key, int mod )
{
    int i;
    Entity *e = playerPtr();

    // first see if we have this key saved already
    for( i = 0; i < e->pl->keybind_count; i++ )
    {
        if( e->pl->keybinds[i].key == (key & 0xff) && e->pl->keybinds[i].modifier == mod )
        {
            strncpyt( e->pl->keybinds[i].command[0], "nop", BIND_MAX );
            e->pl->keybinds[i].uID = 0;//keybindGetNextID();
            return;
        }
    }

    // otherwise, explicitly un-bind it
    if( e->pl->keybind_count && e->pl->keybind_count <= BIND_MAX-1 )
    {
        e->pl->keybinds[e->pl->keybind_count].key = key;
        e->pl->keybinds[e->pl->keybind_count].modifier = mod;
        e->pl->keybinds[e->pl->keybind_count].uID = 0;//keybindGetNextID();

        if( !e->pl->keybinds[e->pl->keybind_count].command[0] )
            e->pl->keybinds[e->pl->keybind_count].command[0] = malloc( sizeof(char)*BIND_MAX );
        strncpyt( e->pl->keybinds[e->pl->keybind_count].command[0], "nop", BIND_MAX );

        e->pl->keybind_count++;
    }
}

static void s_keybind_bindkey(KeyBindProfile *kbp, int keyvalue, int modvalue, const char *command, int secondary, int save)
{
    Entity *e = playerPtr(); 
    assert((modvalue < MOD_TOTAL) && (modvalue >= 0 ));
    if ( (keyvalue < ARRAY_SIZE(kbp->binds)) && (keyvalue > 0 ) )
    {
        if ((modvalue < MOD_TOTAL) && (modvalue >= 0 ))
        {
            if(kbp->binds[keyvalue].command[modvalue] && command 
                && stricmp(kbp->binds[keyvalue].command[modvalue], command)==0)
            {
                kbp->binds[keyvalue].uID = keybindGetNextID();
                // We're about to bind the same thing again.
                // Don't bother (reduces warning messages).
                return;
            }

            if( secondary < 0 )
            {
                addSystemChatMsg( textStd("CommandHasTwoBinds"), INFO_USER_ERROR, 0 );
                return;
            }

            if( command )
            {
                if( !kbp->binds[keyvalue].command[modvalue] )
                    kbp->binds[keyvalue].command[modvalue] = calloc(1, sizeof(char)*BIND_MAX);

                strncpyt( kbp->binds[keyvalue].command[modvalue], command, BIND_MAX );
            }
            else
                SAFE_FREE(kbp->binds[keyvalue].command[modvalue]);

            kbp->binds[keyvalue].secondary = secondary;
            kbp->binds[keyvalue].uID = keybindGetNextID();

            if( save && e && e->pl )
            {
                keybind_send( e->pl->keyProfile, keyvalue, modvalue, secondary, command );
                keybind_set( e->pl->keyProfile, keyvalue, modvalue, secondary, command );
            }
        }
    }
}
//
//
void keybind_setkey( KeyBindProfile *kbp, const char *key, const char *command, int save, int secondary )
{
    int keyvalue, modvalue;
    int keyvaluealt = 0;
    char *keyname, *modname;
    char tmp[BIND_MAX];

    // temporary holder for the key string
    strncpyt( tmp, key, BIND_MAX );

    // get tokens
    modname = strtok( tmp, "+" );
    keyname = strtok( NULL, "+" );

    // get the value
    modvalue = keyValue( modname );
    keyvalue = keyValue( keyname );
    
    if (!keyvalue)
    {
        //    if there is no key, the modkey is the key 
        keyvalue = modvalue;
        modvalue = 0;
    }

    modvalue = keybind_modifier(modvalue);
    
    //    make sure we don't assign the key to an out of bounds key
    switch (keyvalue)
    {
    case INP_CONTROL:
        keyvalue = INP_LCONTROL;
        keyvaluealt = INP_RCONTROL;
        break;
    case INP_SHIFT:
        keyvalue = INP_LSHIFT;
        keyvaluealt = INP_RSHIFT;
        break;
    case INP_ALT:
        keyvalue = INP_LALT;
        keyvaluealt = INP_RALT;
        break;
    }

    s_keybind_bindkey(kbp, keyvalue, modvalue, command, secondary, save);
    if (keyvaluealt)
        s_keybind_bindkey(kbp, keyvaluealt, modvalue, command, secondary, save);
}

void displayKeybind(const char *key)
{
    KeyBindProfile *curProfile = eaGet( &kbProfiles, eaSize(&kbProfiles)-1 );
    char tmp[BIND_MAX];
    char *modname;
    char *keyname;
    int keyvalue, modvalue;
    

    // temporary holder for the key string
    strncpyt( tmp, key, BIND_MAX );

    // get tokens
    modname = strtok( tmp, "+" );
    keyname = strtok( NULL, "+" );

    // get the value
    keyvalue = keyValue( keyname );
    modvalue = keyValue( modname );

    if( !keyvalue )
    {
        keyvalue = modvalue;
        modvalue = 0;
    }

    modvalue = keybind_modifier( modvalue );

    if (keyvalue > 0 && keyvalue < BIND_MAX)
    {
        char *estr = NULL;
        estrCreate(&estr);
        if (curProfile->binds[keyvalue].command[modvalue])
        {    
            char *commandString = NULL;
            estrCreate(&commandString);
            estrConcatCharString(&commandString, curProfile->binds[keyvalue].command[modvalue]);//textStd("", curProfile->binds[keyvalue].command[modvalue]));
            smf_EncodeAllCharactersInPlace(&commandString);
            estrConcatCharString(&estr, textStd("ShowBindString", key, commandString));
            estrDestroy(&commandString);
        }
        else
        {
            estrConcatCharString(&estr, textStd("NothingString"));
        }
        addSystemChatMsg(estr, INFO_SVR_COM, 0);
        estrDestroy(&estr);
    }
    else
    {
        addSystemChatMsg("InvalidKeyString", INFO_SVR_COM, 0);
    }
}
//
//
void keybinds_init( const char *profile, KeyBindProfile *kbp, KeyBind *kb )
{
    int i, j, k, m, profileIndex;
    Entity *e = playerPtr();

    profileIndex = -1;
    // determine which profile I should use
    for( i = 0; i < eaSize( &data_kbp.profiles ); i++ )
    {
        if(stricmp(profile, data_kbp.profiles[i]->name) == 0 || 
            (stricmp("default", data_kbp.profiles[i]->name) == 0 && profileIndex == -1))
        {
            profileIndex = i;
        }
    }

    for( k = 0; k < BIND_MAX; k++ )
    {
        for( m = 0; m < MOD_TOTAL; m++ ) {
            if (kbp->binds[k].command[m])
            {
                free(kbp->binds[k].command[m]);
            }
        }
    }
    memset( kbp, 0, sizeof(KeyBind)*BIND_MAX );

    for( j = 0; j < eaSize( &data_kbp.profiles[profileIndex]->binds ); j++ )
    {
        int count = 0;
        // see if its already been added
        for( k = 0; k < BIND_MAX; k++ )
        {
            for( m = 0; m < MOD_TOTAL; m++ )
            {
                if( kbp->binds[k].command[m] && stricmp( kbp->binds[k].command[m], data_kbp.profiles[profileIndex]->binds[j]->command ) == 0 )
                    count++;
            }
        }

        if( count < 2 )
            keybind_setkey( kbp, data_kbp.profiles[profileIndex]->binds[j]->key, data_kbp.profiles[profileIndex]->binds[j]->command, 0, count );
    }

    if( kb )
    {
        // now overwrite with any saved keybinds
        for( i = 0; i < BIND_MAX; i++ )
        {
            // skip corrupted keybinds
            if( kb[i].modifier >= MOD_TOTAL || kb[i].modifier < 0 )
                continue;

            if( kb[i].key )
            {
                if( !kbp->binds[kb[i].key].command[kb[i].modifier] )
                    kbp->binds[kb[i].key].command[kb[i].modifier] = calloc( 1, sizeof(char)*BIND_MAX);

                strncpyt( kbp->binds[kb[i].key].command[kb[i].modifier], kb[i].command[0], BIND_MAX );
                kbp->binds[kb[i].key].secondary = kb[i].secondary;
                kbp->binds[kb[i].key].uID = keybindGetNextID();
            }
        }
    }
    keybind_clearChanges();
}

void keybind_initKey( KeyBindProfile *kbp, const char * key )
{
    int i,j;
    Entity * e = playerPtr();

    for( i = 0; i < eaSize( &data_kbp.profiles ); i++ )
    {
        // use the current profile
        if( stricmp( e->pl->keyProfile, data_kbp.profiles[i]->name ) == 0 )
        {
            for( j = 0; j < eaSize( &data_kbp.profiles[i]->binds ); j++ )
            {
                if( stricmp(data_kbp.profiles[i]->binds[j]->key,key) == 0 )
                     keybind_setkey( kbp, data_kbp.profiles[i]->binds[j]->key, data_kbp.profiles[i]->binds[j]->command, 1, 0 );
            }
        }
    }
}


void keybind_reset( const char *profile, KeyBindProfile *kbp )
{
     Entity *e = playerPtr();

    memset( e->pl->keybinds, 0, sizeof(KeyBind)*BIND_MAX );
    memset( kbp->binds, 0, sizeof(KeyBind)*BIND_MAX );
    keybinds_init( e->pl->keyProfile, kbp, e->pl->keybinds );

    keybind_sendReset();
}


int keybind_executeCommand( KeyBind * kb, int mod, int state, int timeStamp, int x, int y )
{
    char *cmdstr = kb->command[mod];

    if (cmdstr && cmdstr[0])
    {
        cmdSetTimeStamp(timeStamp);

        if (cmdstr[0] == '+' && cmdstr[1] != '+')
        {
            kb->pressed[mod] = state;

            if(state)
            {
                cmdParseEx(cmdstr, x, y );
            }
            else
            {
                char buf[BIND_MAX];
                strcpy(buf,cmdstr);
                buf[0] = '-';
                cmdParseEx(buf, x, y );
            }
        }
        else if(state)
        {
            // have the profile's associated command parser parse the command
            //curProfile->parser(cmdstr);
            cmdParseEx(cmdstr, x, y );
        }

        return TRUE;
    }

    return FALSE;
}


int keybind_findCommand( KeyBindProfile *curProfile, int keyScanCode, int state, int timeStamp, int x, int y )
{
    int i, mod, retVal = 0;

    // try modifier versions if necessary
    if( inpLevel( INP_CONTROL ) )
        retVal |= keybind_executeCommand( &curProfile->binds[keyScanCode], MODIFIER_CTRL, state, timeStamp, x, y );
    if( inpLevel( INP_SHIFT ) )
        retVal |= keybind_executeCommand( &curProfile->binds[keyScanCode], MODIFIER_SHIFT, state, timeStamp, x, y );
    if( inpLevel( INP_ALT ) )
        retVal |= keybind_executeCommand( &curProfile->binds[keyScanCode], MODIFIER_ALT, state, timeStamp, x, y );

    if(!retVal)
    {
        retVal = keybind_executeCommand( &curProfile->binds[keyScanCode], MODIFIER_NONE, state, timeStamp, x, y );
    }

    // is the key ctrl/shift/alt?
    mod = keybind_modifier( keyScanCode );

    if( mod )
    {
        // now search keybinds for any command with this modifier
        for( i = 0; i < BIND_MAX; i++ )
        {
            if( curProfile->binds[i].command[mod] && curProfile->binds[i].command[mod][0] ) // is there a command?
            {
                if( inpLevel( i ) )
                    retVal |= keybind_executeCommand( &curProfile->binds[i], mod, state, timeStamp, x, y );
            }
        }
    }

    return retVal;
}

//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------


void bindKeyProfile(KeyBindProfile *profile)
{
    eaPush( &kbProfiles, profile );
}

// FIXME!!!
// Simply popping the binding stack might cause some problems.
// Example: push profiles 1, 2, and 3 onto the stack.  Now somehow the mode associated with profile 2
// tries to exit and performs a pop.  Profile 3 is popped in error.
//
// Using toggles to enter/exit different modes will definitely cause a problem when there are more than
// two modes.
//
// If this should ever become a problem,  it can be solved issuing a profile ID when it is "bound" or
// pushed onto the stack.
//
//
void unbindKeyProfile(KeyBindProfile *profile)
{
    if(!profile)
    {
        eaPop(&kbProfiles);
    }
    else
    {
        int i;

        for(i = eaSize(&kbProfiles) - 1; i >= 0 ; i--)
        {
            if( kbProfiles[i] == profile)
            {
                eaRemove( &kbProfiles, i );
                break;
            }
        }
    }

    // Always keep at least the game binding profile on the stack so
    // some key bindings and commands will be available.
    if( 0 == eaSize(&kbProfiles) && isDevelopmentMode() )
    {
        conPrintf("Error! KeyBinding Profile stack emptied.");
        bindKeyProfile(&game_binds_profile);
    }
}

// binds key/command pair to the topmost profile in the kbProfile stack
void bindKey( const char *keyname, const char *command, int save )
{
    KeyBindProfile *curProfile = eaGet( &kbProfiles, eaSize(&kbProfiles)-1 );
    char *estr;

    estrCreate(&estr);
    estrConcatCharString(&estr, command);
    smf_EncodeAllCharactersInPlace(&estr);

    keybind_setkey( curProfile, keyname, estr, save, keybind_CheckBinds( estr ) );
     if( !command )
        keybind_initKey( curProfile, keyname );

    estrDestroy(&estr);
}

// FIXME!!!
//  Messed up function.
//    Trickle rules don't do what they are supposed to do.
void bindKeyExec(int keyScanCode, int state, U32 timeStamp)
{
    int x, y;
    inpMousePos( &x, &y );
    bindKeyExecEx(keyScanCode, state, timeStamp, x, y );
}

void bindKeyExecEx(int keyScanCode, int state, U32 timeStamp, int x, int y )
{
    int        edit=0;
    int        i, foundCommand = FALSE;
    KeyBindProfile *curProfile = NULL;

    // Starting from the profile at the top of the kbProfile stack,
    // give all key binding profiles a chance to handle the keypress.
    for(i = eaSize(&kbProfiles)-1; i >= 0; i--)
    {
        curProfile = eaGet(&kbProfiles, i);

        // If the command has been handled, break..
        // Or, if the current profile doesn't allow key-presses to trickle
        // down to lower level of the profile stack, break...
        if( keybind_findCommand( curProfile, keyScanCode, state, timeStamp, x, y ) )
            foundCommand = TRUE;

        if( foundCommand || !curProfile->trickleKeys )
            break;

        // Otherwise, the command isn't handled in the current binding profile && the
        // current profile allows keypresses to trickle down to lower
        // level profiles.
        // Continue down the stack to try to find a valid command
    }

    if( !foundCommand && keyScanCode < INP_MOUSE_BUTTONS && curProfile && state && curProfile->allOtherKeyCallback )
        curProfile->allOtherKeyCallback();
}

int isGameReturnKeybind(int keyScanCode)
{
    int        i;
    KeyBindProfile *curProfile;

    // Starting from the profile at the top of the kbProfile stack,
    // check all all key binding profiles.
    for(i = eaSize(&kbProfiles)-1; i >= 0; i--)
    {
        char * command;
        curProfile = eaGet(&kbProfiles, i);

        if( keyScanCode < 0 || keyScanCode >= BIND_MAX )
            break;

        if( inpLevel( INP_CONTROL ) )
            command = curProfile->binds[keyScanCode].command[MODIFIER_CTRL];
        if( inpLevel( INP_SHIFT ) )
            command = curProfile->binds[keyScanCode].command[MODIFIER_SHIFT];
        if( inpLevel( INP_ALT ) )
            command = curProfile->binds[keyScanCode].command[MODIFIER_ALT];
        else
            command = curProfile->binds[keyScanCode].command[MODIFIER_NONE];

        if( command )
        {
             if( strstri( command, "gamereturn" ) || strstri( command, "windowcloseextra" ) )
                return true;
        }

        if( !curProfile->trickleKeys )
            break;

        // Otherwise, the command isn't handled in the current binding profile && the
        // current profile allows keypresses to trickle down to lower
        // level profiles.
        // Continue down the stack to try to find a valid command
    }

    return false;
}

int isMovementKeybind(void)
{
    int        i;
    KeyBindProfile *curProfile;

    // Starting from the profile at the top of the kbProfile stack,
    // check all all key binding profiles.
    for(i = eaSize(&kbProfiles)-1; i >= 0; i--)
    {
        KeyInput* input = inpGetKeyBuf();
        curProfile = eaGet(&kbProfiles, i);

        while(input)
        {
            int keyScanCode = OemKeyScan(input->key);
            char * command;

            if( keyScanCode < 0 || keyScanCode >= BIND_MAX )
                break;

            if( inpLevel( INP_CONTROL ) )
                command = curProfile->binds[keyScanCode].command[MODIFIER_CTRL];
            if( inpLevel( INP_SHIFT ) )
                command = curProfile->binds[keyScanCode].command[MODIFIER_SHIFT];
            if( inpLevel( INP_ALT ) )
                command = curProfile->binds[keyScanCode].command[MODIFIER_ALT];
            else
                command = curProfile->binds[keyScanCode].command[MODIFIER_NONE];

            if( command )
            {
                if((strstri( command, "+forward" ) || strstri( command, "+backward" ) ||
                    strstri( command, "+left" )    || strstri( command, "+right" )))
                    return keyScanCode;
            }

            inpGetNextKey(&input);
        }

        if( !curProfile->trickleKeys )
            break;

        // Otherwise, the command isn't handled in the current binding profile && the
        // current profile allows keypresses to trickle down to lower
        // level profiles.
        // Continue down the stack to try to find a valid command
    }

    return false;
}


static void bindCheckKeysReleasedSub(KeyBind *key_binds)
{
    int        i, j;
    char    buf[BIND_MAX];


    for(i=0;i<BIND_MAX;i++)
    {
        if (key_binds[i].pressed[0] && !inpLevel(i))
        {
            key_binds[i].pressed[0] = 0;

            if( key_binds[i].command[0] )
            {
                strcpy(buf,key_binds[i].command[0]);
                buf[0] = '-';
                cmdParse(buf);
            }
        }

        for( j = 1; j < MOD_TOTAL; j++ )
        {
            if (key_binds[i].pressed[j] && (!inpLevel(i) || !inpLevel( keybind_idFromModifier(j)) ) )
            {
                key_binds[i].pressed[j] = 0;
                if( key_binds[i].command[j] )
                {
                    strcpy(buf,key_binds[i].command[j]);
                    buf[0] = '-';
                    cmdParse(buf);
                }
            }
        }
    }
}
//static float emergency_break = 0;
void bindCheckKeysReleased()
{
    int i, scan = 0;
    KeyBindProfile *curProfile;

    for(i = eaSize(&kbProfiles)-1; i >= 0; i--)
    {
        curProfile = eaGet(&kbProfiles, i);

        // Skip profiles that do not have any keys marked as "pressed"
        //if(!curProfile->havePressed)
        //    continue;

        bindCheckKeysReleasedSub(curProfile->binds);
    }
}

void bindListBinds(void)
{
    int i, j, k, m;
    KeyBindProfile *curProfile;

    conPrintf("\nHere's your keybind profiles:\n");

    for(i = 0; i < eaSize(&kbProfiles); i++)
    {
        curProfile = eaGet(&kbProfiles, i);

        conPrintf(    "  %d. Profile Name: %s%s\n",
                    i,
                    curProfile->name ? curProfile->name : "Unnamed",
                    curProfile->trickleKeys ? "" : " (Key Trickle Disabled)");

        for(j = 0; j < ARRAY_SIZE(key_names); j++)
        {
            for(k = 0; k < ARRAY_SIZE(curProfile->binds); k++)
            {
                if(k == key_names[j].value)
                {
                    for(m = 0; m < ARRAY_SIZE(curProfile->binds[k].command); m++)
                    {
                        if(curProfile->binds[k].command[m]){
                            char buffer[1000];
                            sprintf(buffer, "%s%s", 
                                    m == MODIFIER_CTRL ? "CTRL+" :
                                        m == MODIFIER_SHIFT ? "SHIFT+" :
                                            m == MODIFIER_ALT ? "ALT+" :
                                                "",
                                    key_names[j].name2 ? key_names[j].name2 : key_names[j].name + 4);
                            
                            conPrintf("    %-10s : %s", buffer, curProfile->binds[k].command[m]);
                        }
                    }
                }
            }
        }
    }
}

void bindListSave(char * pch, int silent)
{
    int i, j, k, m;
    KeyBindProfile *curProfile;
    FILE *file;
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];

     if(pch)
        strcpy( filename, pch);
    else
    {
        if(isDevelopmentMode())
            strcpy(filename, "C:/keybinds.txt");
        else
            sprintf(filename, "%s/keybinds.txt", getExecutableDir(dir));
    }

    file = fileOpen( filename, "wt" );
    if (!file && !silent)
    {
        addSystemChatMsg("UnableKeybindSave", INFO_USER_ERROR, 0);
        return;
    }
    


    for(i = 0; i < eaSize(&kbProfiles); i++)
    {
        curProfile = eaGet(&kbProfiles, i);

        for(j = 0; j < ARRAY_SIZE(key_names); j++)
        {
            for(k = 0; k < ARRAY_SIZE(curProfile->binds); k++)
            {
                if(k == key_names[j].value)
                {
                    for(m = 0; m < ARRAY_SIZE(curProfile->binds[k].command); m++)
                    {
                        if(curProfile->binds[k].command[m] && *curProfile->binds[k].command[m] )
                        {
                            fprintf(file, "%s%s \"%s\"\n",    m == MODIFIER_CTRL ? "CTRL+" :    m == MODIFIER_SHIFT ? "SHIFT+" : m == MODIFIER_ALT ? "ALT+" :"",
                                        key_names[j].name2 ? key_names[j].name2 : key_names[j].name + 4, curProfile->binds[k].command[m]);
                        }
                    }
                }
            }
        }
    }
    fclose(file);
    if(!silent)
        addSystemChatMsg( textStd("KeybindSaved", filename), INFO_SVR_COM, 0);
}

void bindListLoad(char *pch, int silent)
{
    FILE *file;
    char buf[1000];
    char *dest;
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];
    static int recursion = 0;

    recursion++;

    if(pch)
        strcpy( filename, pch);
    else if(isDevelopmentMode())
        strcpy(filename, "C:/keybinds.txt");
    else
        sprintf(filename, "%s/keybinds.txt", getExecutableDir(dir));

    file = fileOpen( filename, "rt" );
    if (!file)
    {
        if(pch && !silent)
            addSystemChatMsg(textStd("CantReadKeybind", filename), INFO_USER_ERROR, 0);
        recursion = 0;
        return;
    }

    strcpy(buf, "bind ");
    dest = buf+strlen(buf);
    while(fgets(dest,sizeof(buf)-5,file))
    {
        if (recursion <= 5)
        {
            if(!cmdParse(buf) && !silent)
            {
                addSystemChatMsg(buf, INFO_USER_ERROR, 0);
            }
        }
    }
    fclose(file);
    if(!silent)
        addSystemChatMsg( textStd("KeybindLoaded", filename), INFO_SVR_COM, 0);
    recursion--;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------


