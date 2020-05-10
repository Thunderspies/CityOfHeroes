#include <utilitieslib/components/Earray.h>
#include "UI/uiKeymapping.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiGame.h"
#include "UI/uiChat.h"
#include "UI/uiToolTip.h"
#include <utilitieslib/components/StringCache.h>
#include "win/input.h"
#include "UI/uiKeybind.h"
#include "cmdparse/cmdgame.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "player/player.h"
#include "edit/edit_pickcolor.h"
#include "UI/uiWindows.h"
#include "UI/uiNet.h"
#include "UI/uiOptions.h"
#include "UI/uiReticle.h"
#include "UI/uidialog.h"
#include "UI/sprite/sprite_text.h"
#include "entity/EntPlayer.h"
#include "graphics/gfx.h"
#include "graphics/gfxSettings.h"
#include "uislider.h"
#include "UI/uiWindows_init.h"
#include "UI/uiComboBox.h"
#include "sound/sound.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "graphics/truetype/ttFontDraw.h"
#include "win/win_init.h" //for function to get supported resolutions
#include "UI/uiScrollBar.h"
#include "UI/uiChatUtil.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/utils.h>
#include "render/thread/rt_init.h"
#include "auth/authUserData.h"
#include "clientcomm/dbclient.h"
#include "UI/uiPet.h"
#include "UI/uiInput.h"
#include "entity/entity.h"
#include "NovodeX/NwWrapper.h"
#include "render/renderUtil.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/osdependent.h>
#include "UI/uiBuff.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/sysutil.h>
#include "UI/uiClipper.h"
#include "uiHelpButton.h"
#include "win/hwlight.h"
#include "UI/uiEmail.h"
#include "player/inventory_client.h"

typedef enum
{
    OPTION_MISC,        // General
    OPTION_CONTROLS,    // Controls
    OPTION_BINDS,        // Keymapping
    OPTION_GRAPHICS,    // Graphics and Audio
    OPTION_WINDOWS,        // Windows
} OptionTab;

static int s_CurTab = OPTION_MISC;
extern AudioState g_audio_state;

/**************************************************************************/

#include "uiOptions_Type.c"

static int s_petSay;
static int status_hideauto, status_blink, status_stack, status_nonum, status_nosend;
static int group_hideauto,  group_blink,  group_stack,  group_nonum, group_nosend;
static int pet_hideauto,    pet_blink,    pet_stack,    pet_nonum, pet_nosend;

int mapOptionGet(int id)
{
    UserOption *pUO;
    initGameOptionStash();

    if (id < 32)
    {
        stashIntFindPointer(st_GameOptions, kUO_MapOptions, &pUO);
        if (pUO)
            return pUO->iVal & (1 << id);
    }
    else if (id < 64)
    {
        stashIntFindPointer(st_GameOptions, kUO_MapOptions2, &pUO);
        if (pUO)
            return pUO->iVal & (1 << (id - 32));
    }
    else
    {
        assert(0 && "need more map option bitfields!");
    }

    return 0;
}

void mapOptionSet(int id, int val)
{
    UserOption *pUO;
    initGameOptionStash();

    if (id < 32)
    {
        stashIntFindPointer(st_GameOptions, kUO_MapOptions, &pUO);
        if (pUO)
        {
            if (val)
            {
                pUO->iVal |= (1 << id);
            }
            else
            {
                pUO->iVal &= ~(1 << id);
            }
        }
    }
    else if (id < 64)
    {
        stashIntFindPointer(st_GameOptions, kUO_MapOptions2, &pUO);
        if (pUO)
            if (val)
            {
                pUO->iVal |= (1 << (id - 32));
            }
            else
            {
                pUO->iVal &= ~(1 << (id - 32));
            }
    }
    else
    {
        assert(0 && "need more map option bitfields!");
    }
}

void mapOptionSetBitfield(int id, int val)
{
    UserOption *pUO;
    initGameOptionStash();

    if (id == 0)
    {
        stashIntFindPointer(st_GameOptions, kUO_MapOptions, &pUO);
        if (pUO)
            pUO->iVal = val;
    }
    else if (id == 1)
    {
        stashIntFindPointer(st_GameOptions, kUO_MapOptions2, &pUO);
        if (pUO)
            pUO->iVal = val;
    }
    else
    {
        assert(0 && "need more map option bitfields!");
    }
}

int optionGet( int id )
{
    UserOption * pUO;
    initGameOptionStash();
    stashIntFindPointer( st_GameOptions, id, &pUO );
    if( pUO )
        return pUO->iVal;
    return 0;
}

F32 optionGetf( int id )
{
    UserOption * pUO;
    initGameOptionStash();
    stashIntFindPointer( st_GameOptions, id, &pUO );
    if( pUO )
        return pUO->fVal;
    return 0;
}

static void optionSetEx(UserOption *pUO, int val, bool save)
{
    if(!pUO)
    {
        return;
    }

    if( !pUO->doNotWrite)
        pUO->iVal = val; // TODO: bounds check?
    if( pUO->data && !pUO->doNotWrite )
        *(int*)(pUO->data) = val;

    if( save || pUO->delaySave)
    {
        if (game_state.game_mode == SHOW_LOAD_SCREEN)
        {
            pUO->delaySave = 1;
        }
        else
        {
            sendOptions(0);
            pUO->delaySave = 0;
        }
    }
}

void optionSet( int id, int val, bool save )
{
    UserOption * pUO;
    initGameOptionStash();
    stashIntFindPointer( st_GameOptions, id, &pUO );
    optionSetEx(pUO, val, save);
}

void optionSetf( int id, F32 val, bool save )
{
    UserOption * pUO;
    initGameOptionStash();
    stashIntFindPointer( st_GameOptions, id, &pUO );
    if( pUO && !pUO->doNotWrite)
        pUO->fVal = val; // TODO: bounds check?
    if( pUO->data && !pUO->doNotWrite )
        *(float*)(pUO->data) = val;

    if( save )
        sendOptions(0);
}

void optionToggle( int id, bool save )
{
    UserOption * pUO;
     initGameOptionStash();
    stashIntFindPointer( st_GameOptions, id, &pUO );
    if (pUO)
        optionSetEx(pUO, !pUO->iVal, save);
}

void optionToggleByName( char * name )
{
    int i;
    for( i =ARRAY_SIZE(game_options)-1; i>=0; i-- )
    {
        if( !game_options[i].doNotWrite && game_options[i].bitSize>0 &&
            (stricmp( name, game_options[i].pchDisplayName)==0 ||
             stricmp( name, textStd(game_options[i].pchDisplayName))==0 ))
        {
            char val[16];
            optionSetEx(&game_options[i], !game_options[i].iVal, 1);
            itoa(game_options[i].iVal, val, 10);
            addSystemChatMsg( textStd("OptionSet", name, val), INFO_SVR_COM, 0 );
            return;
        }
    }

    addSystemChatMsg( textStd("OptionNotFound", name), INFO_USER_ERROR, 0 );    
}

UserOption *userOptionGetByName( char * name )
{
    int i;
    for( i =ARRAY_SIZE(game_options)-1; i>=0; i-- )
    {
        if( !game_options[i].doNotWrite &&
            (stricmp( name, game_options[i].pchDisplayName)==0 ||
            stricmp( name, textStd(game_options[i].pchDisplayName))==0 ))
        {
            return &game_options[i];
        }
    }

    return NULL;
}

void optionSetByName( char * name, char *val )
{
    UserOption * pUO = userOptionGetByName(name);
    if(pUO)
    {
        if( pUO->bitSize < 0 )
        {
            optionSetf( pUO->id, atof(val), 1 );
        }
        else
        {
            optionSet( pUO->id, atoi(val), 1 ); 
        }
        addSystemChatMsg( textStd("OptionSet", name, val ), INFO_SVR_COM, 0 );    
    }
    else
    {
        addSystemChatMsg( textStd("OptionNotFound", name), INFO_USER_ERROR, 0 );    
    }
}

void optionList()
{
    int i;
    for( i =ARRAY_SIZE(game_options)-1; i>=0; i-- )
    {
        if( !game_options[i].doNotWrite )
            addSystemChatMsg( textStd("OptionName", game_options[i].pchDisplayName), INFO_SVR_COM, 0 );    
    }
}


void optionSave(char * pch)
{
    int i, count = 0;
    FILE *file;
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];

    if(pch) 
        strcpy(filename,pch);
    else if(isDevelopmentMode())
        strcpy(filename, "C:/options.txt");
    else
        sprintf(filename, "%s/options.txt", getExecutableDir(dir));

    file = fileOpen( filename, "wt" );
    if (!file)
    {
        addSystemChatMsg("UnableOptionSave", INFO_USER_ERROR, 0);
        return;
    }

    for( i = 0; i < ARRAY_SIZE(game_options); i++ )
    {
        if(game_options[i].doNotWrite)
            continue;

        if( game_options[i].bitSize < 0 )
            fprintf(file, "%s %f\n", game_options[i].pchDisplayName, game_options[i].fVal );
        else if( game_options[i].bitSize == 32 )
            fprintf(file, "%s %x\n", game_options[i].pchDisplayName, game_options[i].iVal );
        else
            fprintf(file, "%s %i\n", game_options[i].pchDisplayName, game_options[i].iVal );

    }

    addSystemChatMsg( textStd("OptionsSaved", filename), INFO_SVR_COM, 0);
    fclose(file);
}

void optionLoad(char *pch)
{
    FILE *file;
    char buf[1000];
    char filename[FILENAME_MAX];
    char dir[FILENAME_MAX];

    if(pch)
        strcpy(filename, pch);
    else if(isDevelopmentMode())
        strcpy(filename, "C:/options.txt");
    else
        sprintf(filename, "%s/options.txt", getExecutableDir(dir));

    file = fileOpen( filename, "rt" );
    if (!file )
    {
        if(pch)
            addSystemChatMsg(textStd("DidNotLoadOptions", filename), INFO_USER_ERROR, 0);
        return;
    }

    while(fgets(buf,sizeof(buf),file))
    {
        char *args[10];
        int count = tokenize_line_safe( buf, args, ARRAY_SIZE(args), 0 );
        if( count != 2 )
            addSystemChatMsg( textStd("InvalidOptionLine", buf ), INFO_USER_ERROR, 0 );
        else
        {
            UserOption * pUO = userOptionGetByName( args[0] );
            if(!pUO)
            {
                addSystemChatMsg( textStd("InvalidOption", args[0] ), INFO_USER_ERROR, 0 );
                continue;
            }
                
            if( pUO->bitSize < 0 )
                pUO->fVal = atof( args[1] );
            else if( pUO->bitSize == 32 )
            {
                int hex_int;
                sscanf(args[1],"%x", &hex_int);
                pUO->iVal = hex_int;
            }
            else 
                pUO->iVal = atoi( args[1] );
        }
    }

    // Extra hackery
    if( optionGet(kUO_ChatDisablePetSay) )
        s_petSay = 0;
    else if( !optionGet(kUO_ChatEnablePetTeamSay) )
        s_petSay = 1;
    else
        s_petSay = 2; 

    // set local variable for managing buff icon options
    status_hideauto = isBuffSetting(kBuff_Status, kBuff_HideAuto);
    status_blink = isBuffSetting(kBuff_Status, kBuff_NoBlink);
    if( isBuffSetting(kBuff_Status,kBuff_NoStack) )
        status_stack = 1;
    else if( isBuffSetting(kBuff_Status,kBuff_NumStack) )
        status_stack = 2;
    else
        status_stack = 0;

    group_hideauto = isBuffSetting(kBuff_Group, kBuff_HideAuto);
    group_blink = isBuffSetting(kBuff_Group, kBuff_NoBlink);
    if( isBuffSetting(kBuff_Group,kBuff_NoStack) )
        group_stack = 1;
    else if( isBuffSetting(kBuff_Group,kBuff_NumStack) )
        group_stack = 2;
    else
        group_stack = 0;

    pet_hideauto = isBuffSetting(kBuff_Pet, kBuff_HideAuto);
    pet_blink = isBuffSetting(kBuff_Pet, kBuff_NoBlink);
    if( isBuffSetting(kBuff_Pet,kBuff_NoStack) )
        pet_stack = 1;
    else if( isBuffSetting(kBuff_Pet,kBuff_NumStack) )
        pet_stack = 2;
    else
        pet_stack = 0;

    addSystemChatMsg( textStd("OptionsLoaded", filename), INFO_SVR_COM, 0);
    sendOptions((void*)1);
    fclose(file);

}

/**************************************************************************/

static ComboBox s_cbRefreshRates = {0};

typedef struct SliderParam
{
    float fMin;
    float fNormal;
    float fMax;
    float fSize;
} SliderParam;

typedef struct SnapParam
{
    float fMin;
    float fNormal;
    float fMax;
    float fSize;
    int count;
    float values[16];
} SnapParam;

static GfxSettings s_gfx;
static float s_fR;
static float s_fG;
static float s_fB;
static float s_fA;

static float s_fRbub[BUB_COLOR_TOTAL];
static float s_fGbub[BUB_COLOR_TOTAL];
static float s_fBbub[BUB_COLOR_TOTAL];

static float s_fWindowScale;
static int   s_iFontSize;
static float s_renderScale;

static int s_iLevelAbove;
static int s_iLevelBelow;

static int s_antialiasing;

static int s_razerTray;

#define IS_AN_ADVANCED_OPTION if (!s_gfx.showAdvanced) return OPT_HIDE;

#define SLIDER_EPSILON 0.01f

SliderParam s_paramWorldDetail      = { 0.5f,        1.0f,     2.0f,    1.0f };
SliderParam s_paramCharacterQuality = { 0.3f,        1.0f,     2.0f,    1.0f };
SliderParam s_paramGamma            = { 0.3f,        1.0f,     3.0f,    1.0f };
SliderParam s_paramMaxParticles     = { 100.0f, 50000.0f, 50000.0f,    1.0f };
SliderParam s_paramMaxParticleFill  = { 1.0f,        10.0f,    10.0f,    1.0f };
SliderParam s_paramMaxDebris        = { 0.0f, 1500.0f, 1500.0f,    1.0f };
SliderParam s_paramFXSoundVolume    = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramMusicSoundVolume = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramVOSoundVolume    = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramZeroOne          = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramAlpha            = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramMouseSensitivity = { 0.5f,        1.0f,     2.0f,    1.0f };
SliderParam s_paramTurnSensitivity  = { 1.0f,        3.0f,     10.0f,    1.0f };
SliderParam s_paramScrollSensitivity= { 1.0f,        20.0f,    400.0f,    1.0f };
SliderParam s_paramChatFont         = { 5.0f,        12.0f,    20.0f,    1.0f };
SliderParam s_paramTeamLevelAbove   = { 0.0f,        0.0f,     50.0f,    1.0f };
SliderParam s_paramTeamLevelBelow   = { 0.0f,        0.0f,     50.0f,    1.0f };
SliderParam s_paramRed              = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramGreen            = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramBlue             = { 0.0f,        1.0f,     1.0f,    1.0f };
SliderParam s_paramWindowScale      = { 0.64f,       1.0f,     1.02f,    1.0f };
SliderParam s_paramCursorScale        = { 1.0f,        1.0f,     2.0f,    1.0f };
SliderParam s_paramToolTipScale     = { 0.01f,         .6,     1.00f,    1.0f };
SliderParam s_paramDOF              = { 0.1f,        1.0f,     2.0f,    1.0f };
SliderParam s_paramBloom            = { 0.1f,        1.0f,     2.0f,    1.0f };
SliderParam s_paramRenderScale        = { 0.25f,         1.0f,        1.0f,    1.0f};
SliderParam s_paramFieldOfView      = { FIELDOFVIEW_MIN, FIELDOFVIEW_MAX, FIELDOFVIEW_MAX, 1.0f };


SliderParam s_paramWhite      = { 0.004f,       1.0f,     1.0f,    1.0f };
SliderParam s_paramBlack      = { 0.004f,       0.004f,     1.0f,    1.0f };
SliderParam s_paramNull       = { 0.0f,            0.0f,        0.0f,    1.0f };

SnapParam s_paramSlowUglyScale    = { 0.0f,         0.5f,        1.0f,    0.8f, 
5, {0.0f, 0.25, 0.5f, 0.75f, 1.0f}};

SnapParam s_paramAmbientOptionScale = { 0.0f, 0.5f, 1.0f, 0.8f, 8, {0.0f, 0.125f, 0.25f, 0.4f, 0.55f, 0.7f, 0.85f, 1.0f}};

SnapParam s_paramShadowModeScale = { 0.0f, 0.0f, 5.0f, 0.8f, 6, {0, 1, 2, 3, 4, 5}};

SnapParam s_paramShadowMapShaderScale = { 1.0f, 1.0f, 3.0f, 0.8f, 3, {1, 2, 3}};

SnapParam s_paramShadowMapSizeScale = { 0.0f, 0.0f, 2.0f, 0.8f, 3, {0, 1, 2}};

SnapParam s_paramShadowMapDistanceScale = { 0.0f, 0.0f, 2.0f, 0.8f, 3, {0, 1, 2}};

SnapParam s_paramCubemapModeScale = { 0.0f, 0.0f, 4.0f, 0.8f, 5, {0, 1, 2, 3, 4}};

SnapParam s_paramWaterModeScale = { 0.0f, 0.0f, 4.0f, 0.8f, 5, {0, 1, 2, 3, 4}};

// This allows up to 32x.  If you change this, also update the list of values for s_paramAntiAliasingScale.
#define MAX_OPTIONS_FSAA_LOG2 6
SnapParam s_paramAntiAliasingScale = { 1.0f, 1.0f, 6.0f, 0.8f, MAX_OPTIONS_FSAA_LOG2, {1, 2, 3, 4, 5, 6}};

// Callback functions to determine if display
OptionDisplayType alwaysDisplay(GameOptions *go) 
{
    return OPT_DISPLAY;
}

OptionDisplayType neverDisplay(GameOptions *go) 
{
    return OPT_HIDE;
}

OptionDisplayType displayIfVillain(GameOptions *go)
{
    if (ENT_IS_VILLAIN(playerPtr())) 
        return OPT_DISPLAY;
    else 
        return OPT_HIDE;
}

OptionDisplayType displayIfMasterMind(GameOptions *go)
{
    if (playerIsMasterMind()) 
        return OPT_DISPLAY;
    else 
        return OPT_HIDE;
}

OptionDisplayType displayIfVip(GameOptions *go)
{
    if (db_info.vip || AccountIsVIP(inventoryClient_GetAcctInventorySet(), inventoryClient_GetAcctStatusFlags())) 
        return OPT_DISPLAY;
    else 
        return OPT_HIDE;
}



static char *DisplayShowOption(UserOption *pv);
static char *DisplayStackOption(void *pv);
static char *DisplayPetSayOption(void *unused);
static char *DisplayTeamCompleteOption(void *unused);

static void ToggleLimitedShowOption(UserOption *pv);
static void ToggleShowOption(UserOption *pv);
static void ToggleCertShowClaimOption(UserOption *pv);
static void ToggleShowOnOff(UserOption *pv);
static void ToggleStackOption(void *pv);
static void TogglePetSayOption(void *unused);
static void ToggleTeamCompleteOption(void *unused);

static void ResetWindows(void *pv);
//----------------------------------------------------------------------------------------------------------------

static GameOptions s_optListGeneral[] =
{
    { 0,                       kOptionType_Title,  "OptionsTeam",             0,                     NULL, kOptionType_Nop,       NULL,                     NULL,                   NULL,            NULL             },
    { kUO_TeamLevelAbove,      kOptionType_String, "OptionAcceptTeamAbove",   "AcceptAboveHelp",     NULL, kOptionType_IntSlider, &s_iLevelAbove,           &s_paramTeamLevelAbove, NULL,            NULL             },
    { kUO_TeamLevelBelow,      kOptionType_String, "OptionAcceptTeamBelow",   "AcceptBelowHelp",     NULL, kOptionType_IntSlider, &s_iLevelBelow,           &s_paramTeamLevelBelow, NULL,            NULL             },
    { kUO_PromptTeamTeleport,  kOptionType_Bool,   "PromptTeamTeleport",      "PromptTeleportHelp",  NULL, kOptionType_Bool,      NULL,                     NULL,                   "OptionEnabled", "OptionDisabled" },
    { kUO_TeamComplete,        kOptionType_Func,   DisplayTeamCompleteOption, "MissionCompleteHelp", NULL, kOptionType_Func,      ToggleTeamCompleteOption, NULL,                   NULL,            NULL,            },

    { 0,                            kOptionType_Title,  "OptionsEmail",          0,                         NULL, kOptionType_Nop,  NULL,                       NULL, NULL,             NULL             },
    { kUO_DisableEmail,             kOptionType_Bool,   "OptionDisableEmail",    "DisableEmailHelp",        NULL, kOptionType_Bool, NULL,                       NULL, "OptionDisabled", "OptionEnabled"  },
    { kUO_FriendSGEmailOnly,        kOptionType_Bool,   "OptionFriendEmailOnly", "FriendEmailHelp",         NULL, kOptionType_Bool, NULL,                       NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_GMailFriendOnly,          kOptionType_Bool,   "OptionFriendGmailOnly", "FriendGmailHelp",         NULL, kOptionType_Bool, NULL,                       NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideUnclaimableCerts,     kOptionType_Bool,   "HideUnclaimableCert",   "HideUnclaimableCertHelp", NULL, kOptionType_Func, ToggleCertShowClaimOption,  NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_BlinkCertifications,      kOptionType_Bool,   "BlinkCertifications",   "BlinkCertificationsHelp", NULL, kOptionType_Func, ToggleShowOnOff,            NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_VoucherPrompt,            kOptionType_Bool,   "HideVoucherPrompt",     "HideVoucherPromptHelp",   NULL, kOptionType_Bool, NULL,                       NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_NewCertificationPrompt,   kOptionType_Bool,   "HideNewCertPrompt",     "HideNewCertPromptHelp",   NULL, kOptionType_Bool, NULL,                       NULL, "OptionEnabled",  "OptionDisabled" },

    { 0,                                kOptionType_Title,  "OptionsPrompts",               0,                         NULL, kOptionType_Nop,  NULL, NULL, NULL,             NULL             },
    { kUO_DeclineSuperGroupInvite,      kOptionType_Bool,   "DeclineSuperGroupInvite",      "DeclineSGHelp",           NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_DeclineTradeInvite,           kOptionType_Bool,   "DeclineTradeInvite",           "DeclineTradeHelp",        NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HidePlacePrompt,              kOptionType_Bool,   "HidePlacePrompt",              "HidePlaceHelp",           NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideDeletePrompt,             kOptionType_Bool,   "HideDeletePrompt",             "HideDeleteHelp",          NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideUnslotPrompt,             kOptionType_Bool,   "HideUnslotPrompt",             "HideUnslotHelp",          NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideDeleteSalvagePrompt,      kOptionType_Bool,   "HideDeleteSalvagePrompt",      "HideDeleteSalvageHelp",   NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideUsefulSalvagePrompt,      kOptionType_Bool,   "HideUsefulSalvagePrompt",      "HideUsefulSalvageHelp",   NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideOpenSalvagePrompt,        kOptionType_Bool,   "HideOpenSalvagePrompt",        "HideOpenSalvageHelp",     NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideConvertConfirmPrompt,     kOptionType_Bool,    "HideConvertConfirmPrompt",    "HideConvertConfirmHelp",  NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideDeleteRecipePrompt,       kOptionType_Bool,   "HideDeleteRecipePrompt",       "HideDeleteRecipeHelp",    NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideInspirationFull,          kOptionType_Bool,   "HideInspirationFull",          "HideInspirationFullHelp", NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideSalvageFull,              kOptionType_Bool,   "HideSalvageFull",              "HideSalvageFullHelp",     NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideRecipeFull,               kOptionType_Bool,   "HideRecipeFull",               "HideRecipeFullHelp",      NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideEnhancementFull,          kOptionType_Bool,   "HideEnhancementFull",          "HideEnhancementFullHelp", NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideCoopPrompt,               kOptionType_Bool,   "HideCoopPrompt",               "HideCoopHelp",            NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_PopHelpDisable,               kOptionType_Bool,   "PopHelpPrompt",                "PopHelpHelp",             NULL, kOptionType_Bool, NULL, NULL, "OptionDisabled", "OptionEnabled"  },

    { kUO_ForcedPopHelpDisable,         kOptionType_Bool,   "ForcedPopHelpPrompt",          "ForcedPopHelpHelp",            NULL, kOptionType_Bool, NULL, NULL, "OptionDisabled", "OptionEnabled" },
    { kUO_HideFeePrompt,                kOptionType_Bool,   "HideFeeWarning",               "HideFeeHelp",                  NULL, kOptionType_Bool, NULL, NULL, "OptionDisabled", "OptionEnabled" },
    { kUO_HideLoyaltyTreeAccessButton,  kOptionType_Bool,   "HideLoyaltyTreeAccessButton",  "HideLoyaltyTreeAccessHelp",    NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_HideStoreAccessButton,        kOptionType_Bool,   "HideStoreAccessButton",        "HideStoreAccessHelp",          NULL, kOptionType_Bool, NULL, NULL, "OptionEnabled",  "OptionDisabled", displayIfVip },

    //{ 0,                            kOptionType_Title,  "OptionsWebSettings",      NULL, kOptionType_Nop,       NULL,               NULL,                 NULL,             NULL             },
    //{ kUO_WebHideStats,             kOptionType_Bool,   "HideWebStats",            NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    //{ kUO_WebHidePowers,            kOptionType_Bool,   "HideWebPowers",           NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    //{ kUO_WebHideBadges,            kOptionType_Bool,   "HideWebBadges",           NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    //{ kUO_WebHideFriends,           kOptionType_Bool,   "HideWebFriends",          NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },

    { 0,                            kOptionType_Title,  "OptionsTray",         0,                         NULL, kOptionType_Nop,         NULL,                     NULL,                  NULL,             NULL             },
    { kUO_DisableDrag,              kOptionType_Bool,   "LockPowerTray",       "LockPowerTrayHelp",       NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    { kUO_ShowTimer,                kOptionType_Bool,   "ShowTimer",           "ShowTimerHelp",           NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },

    { 0,                            kOptionType_Title,  "OptionsMisc",         0,                         NULL, kOptionType_Nop,         NULL,                     NULL,                  NULL,             NULL             },
    { kUO_UseToolTips,              kOptionType_Bool,   "OptionToolTips",      "ToolTipHelp",             NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    { kUO_ToolTipDelay,             kOptionType_String, "TooltipDelay",        "ToolTipDelayHelp",        NULL, kOptionType_FloatSlider, NULL,                     &s_paramToolTipScale,  NULL,             NULL             },
    { kUO_DeclineGifts,             kOptionType_Bool,   "DeclineNonTeamGifts", "DeclineNonTeamGiftsHelp", NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    { kUO_DeclineTeamGifts,         kOptionType_Bool,   "DeclineTeamGifts",    "DeclineTeamGiftsHelp",    NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    { kUO_UseOldTeamUI,             kOptionType_Bool,   "UseOldTeamUI",        "UseOldTeamUIHelp",        NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionEnabled",  "OptionDisabled" },
    { kUO_DisableLoadingTips,       kOptionType_Bool,   "DisableLoadingTip",   "DisableLoadingTipsHelp",  NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionDisabled", "OptionEnabled"  },
    { kUO_NoXP,                     kOptionType_Bool,   "NoXPPrompt",           "NoXPHelp",               NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionNoXP",     "OptionEarnXP"   },
    { kUO_NoXPExemplar,             kOptionType_Bool,   "NoXPExemplarPrompt",  "NoXPExemplarHelp",        NULL, kOptionType_Bool,        NULL,                     NULL,                  "OptionNoXPIng",  "OptionEarnXP"   },
    
    { 0 },
};

static GameOptions s_optListWindows[] =
{
    { 0, kOptionType_Title,  "OptionsUI",         0, NULL, kOptionType_Nop,         NULL,            NULL,                NULL, NULL },
    { 0, kOptionType_String, "OptionWindowScale", 0, NULL, kOptionType_FloatSlider, &s_fWindowScale, &s_paramWindowScale, NULL, NULL },
    { 0, kOptionType_String, "OptionBorderRed",   0, NULL, kOptionType_FloatSlider, &s_fR,           &s_paramRed,         NULL, NULL },
    { 0, kOptionType_String, "OptionBorderGreen", 0, NULL, kOptionType_FloatSlider, &s_fG,           &s_paramGreen,       NULL, NULL },
    { 0, kOptionType_String, "OptionBorderBlue",  0, NULL, kOptionType_FloatSlider, &s_fB,           &s_paramBlue,        NULL, NULL },
    { 0, kOptionType_String, "OptionBorderAlpha", 0, NULL, kOptionType_FloatSlider, &s_fA,           &s_paramAlpha,       NULL, NULL },
    { kUO_CursorScale,            kOptionType_String,    "OptionCursorScale",    "OptionCursorScaleHelp",    NULL, kOptionType_FloatSlider, NULL, &s_paramCursorScale, NULL, NULL },


    { 0,                       kOptionType_Title,  "OptionsReticle",  0,                    NULL, kOptionType_Nop,  NULL,                    NULL, NULL, NULL },
    { kUO_ShowVillainName,     kOptionType_Func,   DisplayShowOption, "VillainNameHelp",    NULL, kOptionType_Func, ToggleLimitedShowOption, NULL, NULL, NULL },
    { kUO_ShowVillainBars,     kOptionType_Func,   DisplayShowOption, "VillainHealthHelp",  NULL, kOptionType_Func, ToggleLimitedShowOption, NULL, NULL, NULL },
    { kUO_ShowVillainReticles, kOptionType_Func,   DisplayShowOption, "VillainReticleHelp", NULL, kOptionType_Func, ToggleLimitedShowOption, NULL, NULL, NULL },
    { kUO_ShowPlayerName,      kOptionType_Func,   DisplayShowOption, "PlayerNameHelp",     NULL, kOptionType_Func, ToggleShowOption,        NULL, NULL, NULL },
    { kUO_ShowPlayerBars,      kOptionType_Func,   DisplayShowOption, "PlayerHealthHelp",   NULL, kOptionType_Func, ToggleShowOption,        NULL, NULL, NULL },
    { kUO_ShowArchetype,       kOptionType_Func,   DisplayShowOption, "PlayerArchHelp",     NULL, kOptionType_Func, ToggleShowOption,        NULL, NULL, NULL },
    { kUO_ShowSupergroup,      kOptionType_Func,   DisplayShowOption, "PlayerSGHelp",       NULL, kOptionType_Func, ToggleShowOption,        NULL, NULL, NULL },
    { kUO_ShowPlayerReticles,  kOptionType_Func,   DisplayShowOption, "PlayerReticalHelp",  NULL, kOptionType_Func, ToggleLimitedShowOption, NULL, NULL, NULL },
    { kUO_ShowAssistReticles,  kOptionType_Func,   DisplayShowOption, "AutoAssistHelp",     NULL, kOptionType_Func, ToggleLimitedShowOption, NULL, NULL, NULL },
    { kUO_ShowOwnerName,       kOptionType_Func,   DisplayShowOption, "PersonalNameHelp",   NULL, kOptionType_Func, ToggleShowOnOff,         NULL, NULL, NULL },
    { kUO_ShowPlayerRating,    kOptionType_Func,   DisplayShowOption, "PlayerRatingHelp",   NULL, kOptionType_Func, ToggleShowOption,        NULL, NULL, NULL },

    { 0,                  kOptionType_Title,  "OptionsWindow",  0,                 NULL, kOptionType_Nop,  NULL,         NULL, NULL,             NULL             },
    { 0,                  kOptionType_String, "ResetWindows",   "ResetWindowHelp", NULL, kOptionType_Func, ResetWindows, NULL, NULL,             NULL             },
    { kUO_ChatFade,       kOptionType_Bool,   "OptionDimChat",  "DimChatHelp",     NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_Chat1Fade,      kOptionType_Bool,   "OptionDimChat1", "DimChat1Help",    NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_Chat2Fade,      kOptionType_Bool,   "OptionDimChat2", "DimChat2Help",    NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_Chat3Fade,      kOptionType_Bool,   "OptionDimChat3", "DimChat3Help",    NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_Chat4Fade,      kOptionType_Bool,   "OptionDimChat4", "DimChat4Help",    NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_CompassFade,    kOptionType_Bool,   "OptionDimNav",   "DimNavHelp",      NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_FadeExtraTrays, kOptionType_Bool,   "OptionDimTrays", "DimTrayHelp",     NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_WindowFade,     kOptionType_Bool,   "OptionDimAll",   "DimAllHelp",      NULL, kOptionType_Bool, NULL,         NULL, "OptionEnabled",  "OptionDisabled" },

    
    { 0,                           kOptionType_Title,  "OptionsChat",             0,                        NULL, kOptionType_Nop,       NULL,         NULL,             NULL,             NULL             },
    { kUO_AllowProfanity,          kOptionType_Bool,   "OptionAllowProfanity",    "ProfanityHelp",          NULL, kOptionType_Bool,      NULL,         NULL,             "OptionDisabled", "OptionEnabled", },
    { kUO_ShowBallons,             kOptionType_Bool,   "OptionChatBalloons",      "ChatBallonsHelp",        NULL, kOptionType_Bool,      NULL,         NULL,             "OptionEnabled",  "OptionDisabled" },
    { kUO_ChatFontSize,            kOptionType_String, "ChatFontSize",            "ChatFontSizeHelp",       NULL, kOptionType_IntSlider, &s_iFontSize, &s_paramChatFont, NULL,             NULL             },
    { kUO_LogChat,                 kOptionType_Bool,   "LogChat",                 "LogChatHelp",            NULL, kOptionType_Bool,      NULL,         NULL,             "OptionEnabled",  "OptionDisabled" },
    { kUO_LogPrivateMessages,      kOptionType_Bool,   "LogPrivateMsgs",          "LogPrivatMsgsHelp",      NULL, kOptionType_Bool,      NULL,         NULL,             "OptionEnabled",  "OptionDisabled" },
    { kUO_ShowEnemyTells,          kOptionType_Bool,   "ShowEnemyPrivate",        "ShowEnemyPrivateHelp",   NULL, kOptionType_Bool,      NULL,         NULL,             "OptionEnabled",  "OptionDisabled" },
    { kUO_ShowEnemyBroadcast,      kOptionType_Bool,   "ShowEnemyBroadcast",      "ShowEnemyBroadcastHelp", NULL, kOptionType_Bool,      NULL,         NULL,             "OptionEnabled",  "OptionDisabled" },
    { kUO_HideEnemyLocal,          kOptionType_Bool,   "HideEnemyLocal",          "ShowEnemyLocalHelp",     NULL, kOptionType_Bool,      NULL,           NULL,             "OptionDisabled", "OptionEnabled", },
    { kUO_StaticColorsPerName,     kOptionType_Bool,   "IndividualNameColors",    "NameColorHelp",          NULL, kOptionType_Bool,      NULL,           NULL,             "OptionEnabled",  "OptionDisabled" },

    { 0,                           kOptionType_Title,  "OptionsPets",       0,                     NULL,      kOptionType_Nop,  NULL,               NULL,      NULL,             NULL             },
    { kUO_ShowPets,                kOptionType_Bool,   "ShowPetsOption",    "ShowPetHelp",         NULL,      kOptionType_Bool, NULL,               NULL,      "OptionEnabled",  "OptionDisabled" },
    { 0,                           kOptionType_Func,   DisplayPetSayOption, "PetSayHelp",          &s_petSay, kOptionType_Func, TogglePetSayOption, &s_petSay, NULL,              NULL             },
    { kUO_PreventPetIconDrag,      kOptionType_Bool,   "LockPetWindow",     "LockPetHelp",         NULL,      kOptionType_Bool, NULL,               NULL,       "OptionEnabled",  "OptionDisabled", displayIfMasterMind },
    { kUO_AdvancedPetControls,     kOptionType_Bool,   "PetWindowMode",     "PetWindowHelp",       NULL,      kOptionType_Bool, NULL,               NULL,       "OptionEnabled",  "OptionDisabled", displayIfMasterMind },
    { kUO_DisablePetNames,         kOptionType_Bool,   "DisablePetNames",   "DisablePetNamesHelp", NULL,      kOptionType_Bool, NULL,               NULL,       "OptionEnabled",  "OptionDisabled" },

    { 0, kOptionType_Title, "StatusWindowBuffs", 0, NULL,              kOptionType_Nop,  NULL,            NULL,               NULL,          NULL             },
    { 0, kOptionType_Bool,  "HideAutoPowers",    "HideAutoPowersHelp", &status_hideauto, kOptionType_Bool, &status_hideauto,  NULL,          "OptionEnabled",  "OptionDisabled" },
    { 0, kOptionType_Bool,  "DisableBlinking",   "DisableBlinkHelp",   &status_blink,    kOptionType_Bool, &status_blink,      NULL,          "OptionDisabled", "OptionEnabled", },
    { 0, kOptionType_Func,  DisplayStackOption,  "IconStackHelp",      &status_stack,    kOptionType_Func, ToggleStackOption, &status_stack, NULL,             NULL             },
    { 0, kOptionType_Bool,  "HideNumbers",         "HideNumbersHelp",    &status_nonum,     kOptionType_Bool, &status_nonum,     NULL,          "OptionEnabled",  "OptionDisabled" },
    { 0, kOptionType_Bool,  "HideAllBuffs",         "StopAllBuffsHelp",   &status_nosend,   kOptionType_Bool, &status_nosend,    NULL,          "OptionEnabled",  "OptionDisabled" },
    
    { 0, kOptionType_Title, "GroupWindowBuffs", 0, NULL,              kOptionType_Nop,  NULL,             NULL,              NULL,          NULL             },
    { 0, kOptionType_Bool,  "HideAutoPowers",   "HideAutoPowersHelp", &group_hideauto,  kOptionType_Bool, &group_hideauto,   NULL,         "OptionEnabled",  "OptionDisabled" },
    { 0, kOptionType_Bool,  "DisableBlinking", " DisableBlinkHelp",   &group_blink,     kOptionType_Bool, &group_blink,      NULL,         "OptionDisabled", "OptionEnabled",  },
    { 0, kOptionType_Func,  DisplayStackOption, "IconStackHelp",      &group_stack,     kOptionType_Func, ToggleStackOption, &group_stack, NULL,             NULL             },
    { 0, kOptionType_Bool,  "HideNumbers",        "HideNumbersHelp",    &group_nonum,        kOptionType_Bool, &group_nonum,      NULL,         "OptionEnabled",  "OptionDisabled" },
    { 0, kOptionType_Bool,  "HideAllBuffs",        "StopAllBuffsHelp",   &group_nosend,    kOptionType_Bool, &group_nosend,     NULL,         "OptionEnabled",  "OptionDisabled" },

    { 0, kOptionType_Title, "PetWindowBuffs",   0, NULL,              kOptionType_Nop, NULL,             NULL,              NULL,       NULL             },
    { 0, kOptionType_Bool,  "HideAutoPowers",   "HideAutoPowersHelp", &pet_hideauto,   kOptionType_Bool, &pet_hideauto,     NULL,       "OptionEnabled",  "OptionDisabled" },
    { 0, kOptionType_Bool,  "DisableBlinking",  "DisableBlinkHelp",   &pet_blink,      kOptionType_Bool, &pet_blink,        NULL,       "OptionDisabled", "OptionEnabled",   },
    { 0, kOptionType_Func,  DisplayStackOption, "IconStackHelp",      &pet_stack,      kOptionType_Func, ToggleStackOption, &pet_stack, NULL,             NULL             },
    { 0, kOptionType_Bool,  "HideNumbers",        "HideNumbersHelp",    &pet_nonum,       kOptionType_Bool, &pet_nonum,        NULL,       "OptionEnabled",  "OptionDisabled" },
    { 0, kOptionType_Bool,  "HideAllBuffs",        "StopAllBuffsHelp",   &pet_nosend,     kOptionType_Bool, &pet_nosend,       NULL,       "OptionEnabled",  "OptionDisabled" },

    { 0, kOptionType_Title,  "ChatBubbleColor",       0, NULL,    kOptionType_Nop,         NULL,                     NULL,          NULL, NULL },
    { 0, kOptionType_String, "OptionBubbleTextRed",   0, NULL,  kOptionType_FloatSlider, &s_fRbub[BUB_COLOR_TEXT], &s_paramBlack, NULL, NULL },
    { 0, kOptionType_String, "OptionBubbleTextGreen", 0, NULL,    kOptionType_FloatSlider, &s_fGbub[BUB_COLOR_TEXT], &s_paramBlack, NULL, NULL },
    { 0, kOptionType_String, "OptionBubbleTextBlue",  0, NULL,    kOptionType_FloatSlider, &s_fBbub[BUB_COLOR_TEXT], &s_paramBlack, NULL, NULL },
    { 0, kOptionType_String, "OptionBubbleBackRed",   0, NULL,    kOptionType_FloatSlider, &s_fRbub[BUB_COLOR_BACK], &s_paramWhite, NULL, NULL },
    { 0, kOptionType_String, "OptionBubbleBackGreen", 0, NULL,    kOptionType_FloatSlider, &s_fGbub[BUB_COLOR_BACK], &s_paramWhite, NULL, NULL },
    { 0, kOptionType_String, "OptionBubbleBackBlue",  0, NULL,    kOptionType_FloatSlider, &s_fBbub[BUB_COLOR_BACK], &s_paramWhite, NULL, NULL },

    { 0 },
};

static OptionDisplayType ShouldDisplayAdvancedOption(GameOptions *go);
static OptionDisplayType ShouldNotDisplayAdvancedOption(GameOptions *go);
static OptionDisplayType ShouldDisplayVBOsOption(GameOptions *go);
static OptionDisplayType ShouldDisplayTextureAnisoOption(GameOptions *go);
static OptionDisplayType ShouldDisplayAntialiasingOption(GameOptions *go);
static OptionDisplayType ShouldDisplayCubemapOption(GameOptions *go);
static OptionDisplayType ShouldDisplayCursorOption(GameOptions *go);
static OptionDisplayType ShouldDisplayShaderDetailOption(GameOptions *go);
static OptionDisplayType ShouldDisplayWaterOption(GameOptions *go);
static OptionDisplayType ShouldDisplayBuildingPROption(GameOptions *go);
static OptionDisplayType ShouldDisplayShadowModeOption(GameOptions *go);
static OptionDisplayType ShouldDisplayShadowMapOption(GameOptions *go);
static OptionDisplayType ShouldDisplayShadowMapShaderAdvancedOption(GameOptions *go);
static OptionDisplayType ShouldDisplayBloomAmountOption(GameOptions *go);
static OptionDisplayType ShouldDisplayDesaturateOption(GameOptions *go);
static OptionDisplayType ShouldDisplayBloomOption(GameOptions *go);
static OptionDisplayType ShouldDisplayDOFOption(GameOptions *go);
static OptionDisplayType ShouldDisplayDOFAmountOption(GameOptions *go);
static OptionDisplayType ShouldDisplayRefreshRateOption(GameOptions *go);
static OptionDisplayType ShouldDisplayAgeiaSupportOption(GameOptions *go);
static OptionDisplayType ShouldDisplayAmbient(GameOptions *go);
static OptionDisplayType ShouldDisplayAmbientOption(GameOptions *go);
static OptionDisplayType ShouldDisplayAmbientAdvanced(GameOptions *go);
static OptionDisplayType ShouldDisplayAmbientAdvancedSubOption(GameOptions *go);
static OptionDisplayType ShouldDisplay3DSoundOption(GameOptions *go);
static OptionDisplayType ShouldDisplayHardwareLightOption(GameOptions *go);

static char *DisplayShadowModeOption(void *pv);
static char *DisplayShadowMapShaderOption(void *pv);
static char *DisplayShadowMapSizeOption(void *pv);
static char *DisplayShadowMapDistanceOption(void *pv);
static char *DisplayCubemapModeOption(void *pv);
static char *DisplayVBOsOption(void *pv);
static char *DisplayTextureQualityOption(void *pv);
static char *DisplayPhysicsQualityOption(void *pv);
static char *DisplayEntityTextureQualityOption(void *pv);
static char *DisplayTextureAnisoOption(void *pv);
static char *DisplayAntialiasingOption(void *pv);
static char *DisplayTextureLodBiasOption(void *pv);
static char *DisplayShaderDetailOption(void *pv);
static char *DisplayBloomOption(void *pv);
static char *DisplayDesaturateOption(void *pv);
static char *DisplayDOFOption(void *pv);
static char *DisplayLightmapOption(void *pv);
static char *DisplayWaterOption(void *pv);
static char *DisplayEffResolutionOption(void *pv);
static char *DisplaySlowUglyOption(void *pv);
static char *DisplayRefreshRateOption(void *pv);
static char *DisplayAmbientOption(void *pv);
static char *DisplayAmbientStrength(void *pv);
static char *DisplayAmbientResolution(void *pv);
static char *DisplayAmbientBlur(void *pv);

static void ToggleVBOsOption(void *pv);
static void ToggleTextureQualityOption(void *pv);
static void TogglePhysicsQualityOption(void *pv);
static void ToggleEntityTextureQualityOption(void *pv);
static void ToggleTextureLodBiasOption(void *pv);
static void ToggleTextureAnisoOption(void *pv);
static void ToggleShaderDetailOption(void *pv);
static void ToggleBloomOption(void *pv);
static void ToggleDesaturateOption(void *pv);
static void ToggleDOFOption(void *pv);
static void ToggleLightmapOption(void *pv);
static void ToggleAmbientStrength(void *pv);
static void ToggleAmbientResolution(void *pv);
static void ToggleAmbientBlur(void *pv);


static void getDebrisSliderMinMax(float* fMin, float* fMax);

static GameOptions s_optListGraphics[] =
{
    { 0, kOptionType_Title,  "OptionsGraphics",             0,                            NULL,                kOptionType_Nop,                NULL,                    NULL,                        NULL,                NULL,                    NULL },
    { 0, kOptionType_String, "OptionResolution",         "OptionResolutionHelp",    NULL,                kOptionType_Nop,                NULL,                    NULL,                        NULL,                NULL,                    NULL },
    { 0, kOptionType_Func,   DisplayEffResolutionOption, "Option3DResolutionHelp",    NULL,                kOptionType_FloatSlider,        &s_renderScale,            &s_paramRenderScale,        NULL,                NULL,                    NULL },
    { 0, kOptionType_Func,     DisplayRefreshRateOption,   "OptionRefreshRateHelp",    NULL,                kOptionType_Nop,                NULL,                    NULL,                        NULL,                NULL,                     ShouldDisplayRefreshRateOption },
    { 0, kOptionType_String, "OptionGamma",                 "OptionGammaHelp",            NULL,                kOptionType_FloatSlider,        &s_gfx.gamma,            &s_paramGamma,                NULL,                NULL,                    NULL },
    { 0, kOptionType_Func,   DisplayAntialiasingOption,  "AntialiasingHelp",        NULL,                 kOptionType_IntSnapSlider,        &s_antialiasing,        &s_paramAntiAliasingScale,    NULL,                NULL,                    ShouldDisplayAntialiasingOption },
    { 0, kOptionType_Bool,   "OptionShowAdvanced",       "OptionShowAdvancedHelp",    &s_gfx.showAdvanced, kOptionType_Bool,                &s_gfx.showAdvanced,    NULL,                        "OptionEnabled",    "OptionDisabledUltra",    NULL},
    { 0, kOptionType_Func,   DisplaySlowUglyOption,      "OptionSlowOrUglyHelp",    NULL,                 kOptionType_SnapSliderUltra,    &s_gfx.slowUglyScale,    &s_paramSlowUglyScale,        NULL,                NULL,                    ShouldNotDisplayAdvancedOption },
    { 0, kOptionType_String, "OptionFieldOfView",        "OptionFieldOfViewHelp",    &s_gfx.fieldOfView,  kOptionType_IntSlider,            &s_gfx.fieldOfView,        &s_paramFieldOfView,        NULL,                NULL,                    NULL },

    // Ultra Mode Section
    { 0, kOptionType_Title,  "OptionsUltraMode",                 0,                                NULL,                                   kOptionType_Nop,         NULL,                        NULL,                     NULL,                NULL,         ShouldDisplayAdvancedOption },
    { 0, kOptionType_Func,     DisplayCubemapModeOption,            "OptionCubemapModeHelp",        &s_gfx.advanced.cubemapMode,         kOptionType_IntSnapSlider, &s_gfx.advanced.cubemapMode,         &s_paramCubemapModeScale,       NULL, NULL, ShouldDisplayCubemapOption },
    { 0, kOptionType_Func,   DisplayWaterOption,                "WaterEffectsHelp",             &s_gfx.advanced.useWater,             kOptionType_IntSnapSlider, &s_gfx.advanced.useWater,            &s_paramWaterModeScale,         NULL, NULL, ShouldDisplayWaterOption },
    { 0, kOptionType_Bool,   "OptionShowShadowMapAdvanced",     "OptionShowShadowMaptAdvancedHelp", &s_gfx.advanced.shadowMap.showAdvanced, kOptionType_Bool,   &s_gfx.advanced.shadowMap.showAdvanced, NULL, "OptionEnabled", "OptionDisabled", ShouldDisplayShadowMapOption },
    { 0, kOptionType_Func,     DisplayShadowModeOption,            "OptionShadowModeHelp",            &s_gfx.advanced.shadowMode,             kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMode,         &s_paramShadowModeScale,        NULL, NULL, ShouldDisplayShadowModeOption },
    { 0, kOptionType_Bool,   "OptionShowAmbientAdvanced",       "OptionShowAmbientAdvancedHelp", &s_gfx.advanced.ambient.showAdvanced, kOptionType_Bool,        &s_gfx.advanced.ambient.showAdvanced, NULL, "OptionEnabled", "OptionDisabled", ShouldDisplayAmbient },
    { 0, kOptionType_Func,   DisplayAmbientOption,              "AmbientOptionHelp",            &s_gfx.advanced.ambient.optionScale, kOptionType_SnapSlider,    &s_gfx.advanced.ambient.optionScale, &s_paramAmbientOptionScale,    NULL, NULL, ShouldDisplayAmbientOption },

    // Advanced shadow map options
    { 0, kOptionType_TitleUltra,  "OptionShadowMapAdvanced",    0,                              NULL,                                kOptionType_Nop,         NULL, NULL, NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },
    { 0, kOptionType_Func,   DisplayShadowMapShaderOption,        "OptionShadowMapShaderHelp",    &s_gfx.advanced.shadowMap.shader,     kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMap.shader,   &s_paramShadowMapShaderScale,   NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },
    { 0, kOptionType_Func,   DisplayShadowMapSizeOption,        "OptionShadowMapSizeHelp",      &s_gfx.advanced.shadowMap.size,          kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMap.size,     &s_paramShadowMapSizeScale,     NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },
    { 0, kOptionType_Func,   DisplayShadowMapDistanceOption,    "OptionShadowMapDistanceHelp",    &s_gfx.advanced.shadowMap.distance,     kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMap.distance, &s_paramShadowMapDistanceScale, NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },

    // Advanced ambient options
    { 0, kOptionType_TitleUltra,  "OptionAmbientAdvanced",      0,                              NULL,                                kOptionType_Nop,         NULL, NULL, NULL, NULL, ShouldDisplayAmbientAdvanced },
    { 0, kOptionType_Func,   DisplayAmbientStrength,            "AmbientStrengthHelp",          &s_gfx.advanced.ambient.strength,    kOptionType_Func,        ToggleAmbientStrength,             &s_gfx.advanced.ambient.strength,   NULL, NULL, ShouldDisplayAmbientAdvanced },
    { 0, kOptionType_Func,   DisplayAmbientResolution,          "AmbientResolutionHelp",        &s_gfx.advanced.ambient.resolution,  kOptionType_Func,        ToggleAmbientResolution,           &s_gfx.advanced.ambient.resolution, NULL, NULL, ShouldDisplayAmbientAdvancedSubOption },
    { 0, kOptionType_Func,   DisplayAmbientBlur,                "AmbientBlurHelp",              &s_gfx.advanced.ambient.blur,        kOptionType_Func,        ToggleAmbientBlur,                 &s_gfx.advanced.ambient.blur,       NULL, NULL, ShouldDisplayAmbientAdvancedSubOption },

    { 0, kOptionType_TitleUltra,  "OptionsGraphicsAdvanced",    0,                              NULL,                                 kOptionType_Nop,         NULL,                                 NULL,                             NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_Bool,   "OptionSuppressCloseFx",           "OptionSuppressCloseFxHelp",    &s_gfx.advanced.suppressCloseFx,     kOptionType_Bool,        &s_gfx.advanced.suppressCloseFx,     NULL,                           "OptionEnabled",  "OptionDisabled", ShouldDisplayAdvancedOption },
    { 0, kOptionType_Bool,   "OptionAgeiaOn",                   "OptionAgeiaOnHelp",            &s_gfx.advanced.ageiaOn,             kOptionType_Bool,        &s_gfx.advanced.ageiaOn,             NULL,                           "OptionEnabled",  "OptionDisabled", ShouldDisplayAgeiaSupportOption },
    { 0, kOptionType_Func,   DisplayPhysicsQualityOption,       "PhysicsQualityHelp",           &s_gfx.advanced.physicsQuality,         kOptionType_Func,        TogglePhysicsQualityOption,         &s_gfx.advanced.physicsQuality, NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_Func,   DisplayTextureQualityOption,        "TextureQualityHelp",           &s_gfx.advanced.mipLevel,             kOptionType_Func,        ToggleTextureQualityOption,         &s_gfx.advanced.mipLevel,       NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_Func,   DisplayEntityTextureQualityOption, "CharacterTextureQualityHelp",  &s_gfx.advanced.entityMipLevel,      kOptionType_Func,        ToggleEntityTextureQualityOption,     &s_gfx.advanced.entityMipLevel, NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_String, "OptionWorldDetail",                "OptionWorldDetailHelp",        NULL,                                 kOptionType_FloatSlider, &s_gfx.advanced.worldDetailLevel,     &s_paramWorldDetail,             NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_String, "OptionCharacterDetail",            "OptionCharacterDetailHelp",    NULL,                                 kOptionType_FloatSlider, &s_gfx.advanced.entityDetailLevel, &s_paramCharacterQuality,         NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_String, "OptionMaxParticleCount",          "OptionMaxParticleCountHelp",   NULL,                                 kOptionType_IntSlider,   &s_gfx.advanced.maxParticles,      &s_paramMaxParticles,           NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_Bool,   "OptionVSync",                     "OptionVSyncHelp",              &s_gfx.advanced.useVSync,             kOptionType_Bool,        &s_gfx.advanced.useVSync,             NULL,                             "OptionEnabled",  "OptionDisabled", ShouldDisplayAdvancedOption },
    { 0, kOptionType_Bool,   "OptionCursorMode",                "OptionCursorModeHelp",         &s_gfx.advanced.colorMouseCursor,     kOptionType_Bool,        &s_gfx.advanced.colorMouseCursor,     NULL,                             "OptionCursorColor",  "OptionCursorCompatible", ShouldDisplayCursorOption },
    { 0, kOptionType_Func,     DisplayCubemapModeOption,            "OptionCubemapModeHelp",        &s_gfx.advanced.cubemapMode,         kOptionType_IntSnapSlider,   &s_gfx.advanced.cubemapMode,      &s_paramCubemapModeScale,       NULL, NULL, ShouldDisplayCubemapOption },
    { 0, kOptionType_Func,   DisplayVBOsOption,                 "OptionUseGeometryBuffersHelp", &s_gfx.advanced.enableVBOs,             kOptionType_Func,        ToggleVBOsOption,                     &s_gfx.advanced.enableVBOs,     NULL, NULL, ShouldDisplayVBOsOption },
    { 0, kOptionType_Func,   DisplayTextureAnisoOption,         "TextureAnisoHelp",             &s_gfx.advanced.texAniso,             kOptionType_Func,        ToggleTextureAnisoOption,             &s_gfx.advanced.texAniso,         NULL, NULL, ShouldDisplayTextureAnisoOption },
    { 0, kOptionType_Func,   DisplayTextureLodBiasOption,       "TextureLODBiasToolHelp",       &s_gfx.advanced.texLodBias,             kOptionType_Func,        ToggleTextureLodBiasOption,         &s_gfx.advanced.texLodBias,     NULL, NULL, ShouldDisplayAdvancedOption },
    { 0, kOptionType_Func,   DisplayShaderDetailOption,         "ShaderQualityHelp",            &s_gfx.advanced.shaderDetail,         kOptionType_Func,        ToggleShaderDetailOption,             &s_gfx.advanced.shaderDetail,     NULL, NULL, ShouldDisplayShaderDetailOption },
    { 0, kOptionType_Func,   DisplayWaterOption,                "WaterEffectsHelp",             &s_gfx.advanced.useWater,             kOptionType_IntSnapSlider,   &s_gfx.advanced.useWater,         &s_paramWaterModeScale,         NULL, NULL, ShouldDisplayWaterOption },
//    { 0, kOptionType_Bool,   "OptionBuildingPR",                "BuildingPREffectsHelp",        &s_gfx.advanced.buildingPlanarReflections,
//                                                                                                                                        kOptionType_Bool,        &s_gfx.advanced.buildingPlanarReflections,
//                                                                                                                                                                                                  NULL,                             "OptionEnabled",  "OptionDisabled", ShouldDisplayBuildingPROption },
    { 0, kOptionType_Func,   DisplayDOFOption,                  "DOFEffectsHelp",               &s_gfx.advanced.useDOF,                 kOptionType_Func,        ToggleDOFOption,                     &s_gfx.advanced.useDOF,         NULL, NULL, ShouldDisplayDOFOption },
    { 0, kOptionType_Func,   DisplayBloomOption,                "BloomEffectsHelp",             &s_gfx.advanced.useBloom,             kOptionType_Func,        ToggleBloomOption,                 &s_gfx.advanced.useBloom,         NULL, NULL, ShouldDisplayBloomOption },
    { 0, kOptionType_String, "OptionBloomAmount",               "OptionBloomAmountHelp",        NULL,                                 kOptionType_FloatSlider, &s_gfx.advanced.bloomMagnitude,     &s_paramBloom,                     NULL, NULL, ShouldDisplayBloomAmountOption },
    { 0, kOptionType_Func,   DisplayDesaturateOption,           "DesaturateEffectsHelp",        &s_gfx.advanced.useDesaturate,         kOptionType_Func,        ToggleDesaturateOption,             &s_gfx.advanced.useDesaturate,     NULL, NULL, ShouldDisplayDesaturateOption },

    { 0, kOptionType_Bool,   "OptionShowShadowMapAdvanced",     "OptionShowShadowMaptAdvancedHelp", &s_gfx.advanced.shadowMap.showAdvanced, kOptionType_Bool,   &s_gfx.advanced.shadowMap.showAdvanced, NULL, "OptionEnabled", "OptionDisabled", ShouldDisplayShadowMapOption },
    { 0, kOptionType_Func,     DisplayShadowModeOption,            "OptionShadowModeHelp",            &s_gfx.advanced.shadowMode,             kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMode,      &s_paramShadowModeScale,        NULL, NULL, ShouldDisplayShadowModeOption },

    { 0, kOptionType_Bool,   "OptionShowAmbientAdvanced",       "OptionShowAmbientAdvancedHelp", &s_gfx.advanced.ambient.showAdvanced, kOptionType_Bool,      &s_gfx.advanced.ambient.showAdvanced, NULL, "OptionEnabled", "OptionDisabled", ShouldDisplayAmbient },
    { 0, kOptionType_Func,   DisplayAmbientOption,              "AmbientOptionHelp",            &s_gfx.advanced.ambient.optionScale, kOptionType_SnapSlider,  &s_gfx.advanced.ambient.optionScale, &s_paramAmbientOptionScale,   NULL, NULL, ShouldDisplayAmbientOption },


    // Advanced shadow map options
    { 0, kOptionType_Title,  "OptionShadowMapAdvanced",         0,                              NULL,                                kOptionType_Nop,         NULL, NULL, NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },
    { 0, kOptionType_Func,   DisplayShadowMapShaderOption,        "OptionShadowMapShaderHelp",    &s_gfx.advanced.shadowMap.shader,     kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMap.shader,   &s_paramShadowMapShaderScale,   NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },
    { 0, kOptionType_Func,   DisplayShadowMapSizeOption,        "OptionShadowMapSizeHelp",      &s_gfx.advanced.shadowMap.size,          kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMap.size,     &s_paramShadowMapSizeScale,     NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },
    { 0, kOptionType_Func,   DisplayShadowMapDistanceOption,    "OptionShadowMapDistanceHelp",    &s_gfx.advanced.shadowMap.distance,     kOptionType_IntSnapSlider, &s_gfx.advanced.shadowMap.distance, &s_paramShadowMapDistanceScale, NULL, NULL, ShouldDisplayShadowMapShaderAdvancedOption },

    // Advanced ambient options
    { 0, kOptionType_Title,  "OptionAmbientAdvanced",            0,                              NULL,                                kOptionType_Nop,         NULL, NULL, NULL, NULL, ShouldDisplayAmbientAdvanced },
    { 0, kOptionType_Func,   DisplayAmbientStrength,            "AmbientStrengthHelp",          &s_gfx.advanced.ambient.strength,    kOptionType_Func,        ToggleAmbientStrength,             &s_gfx.advanced.ambient.strength,   NULL, NULL, ShouldDisplayAmbientAdvanced },
    { 0, kOptionType_Func,   DisplayAmbientResolution,          "AmbientResolutionHelp",        &s_gfx.advanced.ambient.resolution,  kOptionType_Func,        ToggleAmbientResolution,           &s_gfx.advanced.ambient.resolution, NULL, NULL, ShouldDisplayAmbientAdvancedSubOption },
    { 0, kOptionType_Func,   DisplayAmbientBlur,                "AmbientBlurHelp",              &s_gfx.advanced.ambient.blur,        kOptionType_Func,        ToggleAmbientBlur,                 &s_gfx.advanced.ambient.blur,       NULL, NULL, ShouldDisplayAmbientAdvancedSubOption },



    { 0, kOptionType_Title,  "OptionsSound",        0,                         NULL,                        kOptionType_Nop,         NULL,                       NULL,                     NULL, NULL },
    { 0, kOptionType_String, "OptionSoundFXVolume", "OptionSoundFXVolumeHelp", NULL,                        kOptionType_FloatSlider, &s_gfx.fxSoundVolume,       &s_paramFXSoundVolume,    NULL, NULL },
    { 0, kOptionType_String, "OptionMusicVolume",   "OptionMusicVolumeHelp",   NULL,                        kOptionType_FloatSlider, &s_gfx.musicSoundVolume,    &s_paramMusicSoundVolume, NULL, NULL },
    { 0, kOptionType_String, "OptionSoundVOVolume",    "OptionSoundVOVolumeHelp", NULL,                        kOptionType_FloatSlider, &s_gfx.voSoundVolume,        &s_paramVOSoundVolume, NULL, NULL },
    { 0, kOptionType_Bool,   "OptionSoundQuality",  "OptionSoundQualityHelp",  &s_gfx.forceSoftwareAudio,    kOptionType_Bool,        &s_gfx.forceSoftwareAudio,  NULL,                     "CompatibilityString", "PerformanceString" },
    { 0, kOptionType_Bool,     "Option3DSound",       "Option3DSoundHelp",       &s_gfx.enable3DSound,        kOptionType_Bool,         &s_gfx.enable3DSound,         NULL,                       "OptionEnabled", "OptionDisabled", ShouldDisplay3DSoundOption },

    { 0, kOptionType_Title,  "OptionsAlienFX",       0,                         NULL,                        kOptionType_Nop,         NULL,                       NULL,                     NULL, NULL },
    { 0, kOptionType_Bool,     "OptionHardwareLights", "OptionHardwareLightsHelp",       &s_gfx.enableHWLights,        kOptionType_Bool,         &s_gfx.enableHWLights,         NULL,                       "OptionEnabled", "OptionDisabled", ShouldDisplayHardwareLightOption },

    // Non-displayed options, used to save state for undo
    { 0,kOptionType_String, "Thiswillneverdisplay", 0, NULL, kOptionType_Bool,      &s_gfx.fullScreen,  NULL,         NULL, NULL, neverDisplay },
    { 0,kOptionType_String, "Thiswillneverdisplay", 0, NULL, kOptionType_IntSlider, &s_gfx.screenX,     &s_paramNull, NULL, NULL, neverDisplay },
    { 0,kOptionType_String, "Thiswillneverdisplay", 0, NULL, kOptionType_IntSlider, &s_gfx.screenY,     &s_paramNull, NULL, NULL, neverDisplay },
    { 0,kOptionType_String, "Thiswillneverdisplay", 0, NULL, kOptionType_IntSlider, &s_gfx.refreshRate, &s_paramNull, NULL, NULL, neverDisplay },        

    { 0 },
};



static char *DisplayMousePitchOption(UserOption *pv);
static void ToggleMousePitchOption(UserOption *pv);
static void ToggleJoystickEnable(void *pv);

static GameOptions s_optListControls[] =
{
    { 0,               kOptionType_Title,  "ControlSensitivity", 0, NULL, kOptionType_Nop,         NULL, NULL,                     NULL, NULL },
    { kUO_MouseSpeed,  kOptionType_String, "MouseLook",          "MouseLookHelp", NULL, kOptionType_FloatSlider, NULL, &s_paramMouseSensitivity, NULL, NULL },
    { kUO_SpeedTurn,   kOptionType_String, "TurningSpeed",       "TurningSpeedHelp", NULL, kOptionType_FloatSlider, NULL, &s_paramTurnSensitivity,  NULL, NULL },
    
    { 0,                  kOptionType_Title, "Joystick",      0, NULL,                       kOptionType_Nop,  NULL,                 NULL,                       NULL, NULL },
    { kUO_EnableJoystick, kOptionType_Bool,  "JoystickInput", "JoystickHelp", NULL, kOptionType_Func, ToggleJoystickEnable, NULL, "OptionEnabled",  "OptionDisabled" },
    
    { 0,                       kOptionType_Title,  "Mouse",                     0,  NULL, kOptionType_Nop,  NULL,                   NULL, NULL,             NULL             },
    { kUO_MouseInvert,         kOptionType_Bool,   "OptionMouseMovement",    "MouseMovementHelp",  NULL, kOptionType_Bool, NULL,                   NULL, "OptionInverted", "OptionNormal"   },
    { kUO_ClickToMove,         kOptionType_Bool,   "ClickToMove",            "ClickToMoveHelp",  NULL, kOptionType_Bool, NULL,                   NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_MousePitchOption,    kOptionType_Func,   DisplayMousePitchOption,  "MousePitchHelp",  NULL, kOptionType_Func, ToggleMousePitchOption, NULL, NULL,             NULL             },
    { kUO_ReverseMouseButtons, kOptionType_Bool,   "ReverseMouseButtons",    "ReverseMouseButtonsHelp",  NULL, kOptionType_Bool, NULL,                   NULL, "OptionReversed", "OptionNormal"   },
    { kUO_DisableMouseScroll,  kOptionType_Bool,   "DisableMouseScrollText", "MouseScrollTextHelp",  NULL, kOptionType_Bool, NULL,                   NULL, "OptionDisabled", "OptionEnabled",  },
    { kUO_MouseScrollSpeed,    kOptionType_String, "MouseScrollSpeedText",   "MouseScrollSpeedHelp",  NULL, kOptionType_IntSlider, NULL, &s_paramScrollSensitivity,  NULL, NULL },
    { kUO_CamFree,             kOptionType_Bool,   "FreeCamera",             "FreeCameraHelp",  NULL, kOptionType_Bool, NULL,                   NULL, "OptionEnabled",  "OptionDisabled" },
    { kUO_DisableCameraShake,  kOptionType_Bool,   "DisableCameraShakeText", "CameraShakeHelp",  NULL, kOptionType_Bool, NULL,                   NULL, "OptionDisabled", "OptionEnabled"  },
    { 0,                       kOptionType_Bool,   "OptionRazerNagaTray",     "RazerMouseTrayHelp", &s_razerTray, kOptionType_Bool, &s_razerTray, NULL, "OptionEnabled", "OptionDisabled" },

    { 0 },
};

//----------------------------------------------------------------------------------------------------------------

static char *DisplayShowOption(UserOption *pv)
{
    static char ach[128];
    int val = pv->iVal;

    switch(pv->id)
    {
        xcase kUO_ShowArchetype:        strcpy(ach, "PlayerArchetype");
        xcase kUO_ShowSupergroup:        strcpy(ach, "PlayerSupergroup");
        xcase kUO_ShowPlayerName:        strcpy(ach, "PlayerName");
        xcase kUO_ShowVillainName:        strcpy(ach, "VillainName");
        xcase kUO_ShowPlayerBars:        strcpy(ach, "PlayerHealthEndBar");
        xcase kUO_ShowVillainBars:        strcpy(ach, "VillainHealthBar");
        xcase kUO_ShowPlayerReticles:    strcpy(ach, "PlayerReticles");
        xcase kUO_ShowVillainReticles:    strcpy(ach, "VillainReticles");
        xcase kUO_ShowAssistReticles:    strcpy(ach, "AssistReticles");
        xcase kUO_ShowOwnerName:        strcpy(ach, "OwnerName");
        xcase kUO_ShowPlayerRating:        strcpy(ach, "PlayerRating");
    }
    strcat(ach,"\n");

    if(val&kShow_Selected && val&kShow_OnMouseOver)
        strcat(ach, "ShowWhenSelectedOrMouseOver");
    else if(val & kShow_Always)
        strcat(ach, "ShowAlways");
    else if(val & kShow_OnMouseOver)
        strcat(ach, "ShowOnMouseOver");
    else if(val & kShow_Selected)
        strcat(ach, "ShowWhenSelected");
    else
        strcat(ach, "ShowHidden");

    return ach;
}

static char *DisplayTeamCompleteOption(void *unused)
{
    static char ach[128];
    strcpy(ach, textStd("TeamCompleteOption"));
    strcat(ach,"\n");

    if(optionGet(kUO_TeamComplete) == 0)
        strcat( ach, "OptionAlwaysAsk");
    else if(optionGet(kUO_TeamComplete) == 1)
        strcat( ach, "OptionEnabled");
    else
        strcat( ach, "OptionDisabled");

    return ach;
}

static char *DisplayPetSayOption(void *unused)
{
    static char ach[128];
    strcpy(ach, textStd("PetSayOption"));
    strcat(ach,"\n");

    if(s_petSay == 0)
        strcat( ach, "OptionDisabled");
    else if(s_petSay == 1)
        strcat( ach, "ListenOwnedPets");
    else
        strcat( ach, "ListenTeamPets");

    return ach;
}

static char *DisplayStackOption(void *pv)
{
    static char ach[128];
    strcpy(ach, textStd("StackingOption"));
    strcat(ach,"\n");

    if( *((int*)pv) == 2)
        strcat( ach, "StackNumeric");
    else if( *((int*)pv) == 1)
        strcat( ach, "NoStacking");
    else
        strcat( ach, "DefaultStacking");

    return ach;
}

static void ToggleJoystickEnable(void *pv)
{
    // toggle true/false
    optionToggle(kUO_EnableJoystick, 1);
    JoystickEnable();
}

static void ToggleShowOption(UserOption *pv)
{
    if(pv->iVal & kShow_Selected && pv->iVal & kShow_OnMouseOver)
        pv->iVal = kShow_HideAlways;
    else if(pv->iVal & kShow_Always)
        pv->iVal = kShow_OnMouseOver;
    else if(pv->iVal & kShow_OnMouseOver)
        pv->iVal = kShow_Selected;
    else if(pv->iVal & kShow_Selected)
        pv->iVal = kShow_Selected|kShow_OnMouseOver;
    else
        pv->iVal = kShow_Always;
}

static void TogglePetSayOption(void *unused)
{
    s_petSay++;
    if (s_petSay > 2) s_petSay = 0;
    if(s_petSay == 0)
    {
        optionSet(kUO_ChatDisablePetSay, 1, 0);
        optionSet(kUO_ChatEnablePetTeamSay, 0, 0);
    }  
    else if (s_petSay == 1)
    {
        optionSet(kUO_ChatDisablePetSay, 0, 0);
        optionSet(kUO_ChatEnablePetTeamSay, 0, 0);
    } 
    else
    {
        optionSet(kUO_ChatDisablePetSay, 0, 0);
        optionSet(kUO_ChatEnablePetTeamSay, 1, 0);    
    }
}
static void ToggleCertShowClaimOption(UserOption *pv)
{
    pv->iVal = !pv->iVal;
    updateCertifications(0);
}
static void ToggleStackOption( void * pv )
{
    int *stack = (int*)pv;

    (*stack)++;
     if(*stack>2)
        *stack = 0;
}

static void ToggleTeamCompleteOption(void *unused)
{
    optionSet( kUO_TeamComplete, (optionGet(kUO_TeamComplete)+1)%3, 0 );
}

static void ToggleLimitedShowOption(UserOption *pv)
{
    if(pv->iVal&kShow_Selected && pv->iVal&kShow_OnMouseOver)
        pv->iVal = kShow_HideAlways;
    else if(pv->iVal&kShow_OnMouseOver)
        pv->iVal= kShow_Selected;
    else if(pv->iVal&kShow_Selected)
        pv->iVal = kShow_Selected|kShow_OnMouseOver;
    else
        pv->iVal = kShow_OnMouseOver;
}

static void ToggleShowOnOff(UserOption *pv)
{
    if(pv->iVal & kShow_Always )
        pv->iVal = kShow_HideAlways;
    else
        pv->iVal = kShow_Always;
}


static char *DisplayEffResolutionOption(void *pv)
{
    static char ach[128];
    int effx, effy;
    int i, j;
#define CLOSE(x,v) (ABS((x)-(v))/((float)(x)) <= 0.02)

    // Update renderscale in s_gfx
    if (s_renderScale>=0.99) {
        s_gfx.useRenderScale = RENDERSCALE_OFF;
        s_gfx.renderScaleX = s_gfx.renderScaleY = 1;
    } else {
        s_gfx.useRenderScale = RENDERSCALE_SCALE;
        s_gfx.renderScaleX = s_gfx.renderScaleY = s_renderScale;
    }
    effx = s_gfx.screenX*s_gfx.renderScaleX;
    effy = s_gfx.screenY*s_gfx.renderScaleY;

    // Snap to good resolutions
    for (i=0; i<11; i++) {
        if (CLOSE(1<<i, effx))
            s_gfx.renderScaleX = (1<<i)/(float)s_gfx.screenX;
        if (CLOSE(1<<i, effy))
            s_gfx.renderScaleY = (1<<i)/(float)s_gfx.screenY;
    }
    for (j=2; j<=4; j++) {
        for (i=1; i<j; i++) {
            F32 ratio = i/(float)j;
            if (ABS(ratio - s_gfx.renderScaleX) <= 0.02)
                s_gfx.renderScaleX = ratio;
            if (ABS(ratio - s_gfx.renderScaleY) <= 0.02)
                s_gfx.renderScaleY = ratio;
        }
    }

    s_renderScale = s_gfx.renderScaleX;
    strcpy(ach, "Option3DResolution");
    strcatf(ach, "\n%dx%d", (int)(s_gfx.screenX*s_gfx.renderScaleX), (int)(s_gfx.screenY*s_gfx.renderScaleY));
    return ach;
}

static char *DisplaySlowUglyOption(void *pv)
{
    static char ach[128];
    F32 save = s_gfx.slowUglyScale;
    strcpy(ach, "OptionSlowOrUgly\n");
    if (s_gfx.slowUglyScale < s_paramSlowUglyScale.values[0]+SLIDER_EPSILON) {
        strcat(ach, "OptionUgly");
    } else if (s_gfx.slowUglyScale < s_paramSlowUglyScale.values[1]+SLIDER_EPSILON) {
        strcat(ach, "OptionRecommendedLowEnd");
    } else if (s_gfx.slowUglyScale < s_paramSlowUglyScale.values[2]+SLIDER_EPSILON) {
        strcat(ach, "OptionRecommended");
    } else if (s_gfx.slowUglyScale < s_paramSlowUglyScale.values[3]+SLIDER_EPSILON) {
        strcat(ach, "OptionSlow");
    } else {
        strcat(ach, "OptionUltra");
    }

    // Do work here!
    if (!s_gfx.showAdvanced)
    {
        // Apply this value
        gfxGetScaledAdvancedSettings(&s_gfx, s_gfx.slowUglyScale);
        s_gfx.slowUglyScale = save;
    }

    return ach;
}

static char *DisplayTextureQualityOption(void *pv)
{
    static char ach[128];
    int iMipLevel = *(int *)pv;

    strcpy(ach, "TextureQuality");

    strcat(ach,"\n");

    if(iMipLevel==3)
    {
        strcat(ach, "TextureQualityVeryLow");
    }
    if(iMipLevel==2)
    {
        strcat(ach, "TextureQualityLow");
    }
    else if(iMipLevel==1)
    {
        if (systemSpecs.maxPhysicalMemory <= 512*1024*1024)
            strcat(ach, "TextureQualityMediumNotRecommended");
        else
            strcat(ach, "TextureQualityMedium");
    }
    else if(iMipLevel==0)
    {
        if (systemSpecs.maxPhysicalMemory <= 768*1024*1024)
            strcat(ach, "TextureQualityHighNotRecommended");
        else
            strcat(ach, "TextureQualityHigh");
    }
    else if (iMipLevel==-1)
    {
        if (systemSpecs.maxPhysicalMemory <= 768*1024*1024)
            strcat(ach, "TextureQualityVeryHighNotRecommended");
        else
            strcat(ach, "TextureQualityVeryHigh");
    }

    return ach;
}

static char *DisplayPhysicsQualityOption(void *pv)
{
    static char ach[128];
    int iPQLevel = *(int*)pv;

    strcpy(ach, "PhysicsQuality");

    strcat(ach,"\n");
    #if NOVODEX
    switch (iPQLevel)
    {
        xcase ePhysicsQuality_None:
        {
            strcat(ach, "PhysicsQualityNone");
        }
        xcase ePhysicsQuality_Low:
        {
            strcat(ach, "PhysicsQualityLow");
        }
        xcase ePhysicsQuality_Medium:
        {
            strcat(ach, "PhysicsQualityMedium");
        }
        xcase ePhysicsQuality_High:
        {
            strcat(ach, "PhysicsQualityHigh");
        }
        xcase ePhysicsQuality_VeryHigh:
        {
            if ( s_gfx.advanced.ageiaOn )
                strcat(ach, "PhysicsQualityVeryHigh");
            else
                strcat(ach, "PhysicsQualityVeryHighNoPPU");
        }
    }
    #endif
    return ach;
}

static char *DisplayRefreshRateOption(void *pv)
{
    static char ach[128];
    strcpy(ach, "OptionRefreshRate\n");
//    if (!s_gfx.fullScreen || s_cbRefreshRates.currChoice < 0)
//        strcat(ach, "N/A");
    return ach;
}

static OptionDisplayType ShouldDisplayRefreshRateOption(GameOptions *go)
{
    if (!s_gfx.fullScreen || s_cbRefreshRates.currChoice < 0)
        return OPT_HIDE; //OPT_DISABLE
    return OPT_DISPLAY;
}

static char *DisplayEntityTextureQualityOption(void *pv)
{
    static char ach[128];
    int iMipLevel = *(int *)pv;

    strcpy(ach, "CharacterTextureQuality");

    strcat(ach,"\n");

    if(iMipLevel==4)
    {
        strcat(ach, "TextureQualityVeryLow");
    }
    if(iMipLevel==3)
    {
        strcat(ach, "TextureQualityLow");
    }
    else if(iMipLevel==2)
    {
        strcat(ach, "TextureQualityMedium");
    }
    else if(iMipLevel==1)
    {
        if (systemSpecs.maxPhysicalMemory <= 512*1024*1024)
            strcat(ach, "TextureQualityHighNotRecommended");
        else
            strcat(ach, "TextureQualityHigh");
    }
    else if(iMipLevel==0)
    {
        if (systemSpecs.maxPhysicalMemory <= 512*1024*1024)
            strcat(ach, "TextureQualityVeryHighNotRecommended");
        else
            strcat(ach, "TextureQualityVeryHigh");
    }

    return ach;
}

static char *DisplayTextureLodBiasOption(void *pv)
{
    static char ach[128];
    int texLodBias = *(int *)pv;

    strcpy(ach, "TextureLODBias");

    strcat(ach,"\n");

    if(texLodBias==2)
    {
        strcat(ach, "TextureLODBiasSmooth");
    }
    else if(texLodBias==1)
    {
        if (s_gfx.advanced.texAniso>1)
            strcat(ach, "TextureLODBiasMediumNotWithAniso");
        else
            strcat(ach, "TextureLODBiasMedium");

    }
    else
    {
        if (s_gfx.advanced.texAniso>1)
            strcat(ach, "TextureLODBiasCrispNotWithAniso");
        else
            strcat(ach, "TextureLODBiasCrisp");
    }

    return ach;
}

static OptionDisplayType ShouldDisplayVBOsOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!rdr_caps.use_vertshaders)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

void gfxUpdateShadowMapAdvanced( GfxSettings * gfxSettings )
{
    if (gfxSettings->advanced.shadowMode < SHADOW_SHADOWMAP_LOW)
        return;

    if(gfxSettings->showAdvanced && gfxSettings->advanced.shadowMap.showAdvanced) {
        return;
    } else if(!gfxSettings->showAdvanced) {
        gfxSettings->advanced.shadowMap.showAdvanced = false;
    }

    switch (gfxSettings->advanced.shadowMode)
    {
        case SHADOW_SHADOWMAP_HIGHEST:
            gfxSettings->advanced.shadowMap.shader = SHADOWSHADER_HIGHQ;
            gfxSettings->advanced.shadowMap.size = SHADOWMAPSIZE_LARGE;
            gfxSettings->advanced.shadowMap.distance = SHADOWDISTANCE_FAR;
            break;

        case SHADOW_SHADOWMAP_HIGH:
            gfxSettings->advanced.shadowMap.shader = SHADOWSHADER_HIGHQ;
            gfxSettings->advanced.shadowMap.size = SHADOWMAPSIZE_MEDIUM;
            gfxSettings->advanced.shadowMap.distance = SHADOWDISTANCE_FAR;
            break;

        case SHADOW_SHADOWMAP_MEDIUM:
            gfxSettings->advanced.shadowMap.shader = SHADOWSHADER_HIGHQ;
            gfxSettings->advanced.shadowMap.size = SHADOWMAPSIZE_MEDIUM;
            gfxSettings->advanced.shadowMap.distance = SHADOWDISTANCE_MIDDLE;
            break;

        default:
            gfxSettings->advanced.shadowMap.shader = SHADOWSHADER_FAST;    // aka low
            gfxSettings->advanced.shadowMap.size = SHADOWMAPSIZE_MEDIUM;
            gfxSettings->advanced.shadowMap.distance = SHADOWDISTANCE_MIDDLE;
    }
}

static OptionDisplayType ShouldDisplayShadowModeOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if( rdr_caps.disable_stencil_shadows )
        return OPT_DISABLE;
    if (s_gfx.advanced.shadowMap.showAdvanced)
        return OPT_CUSTOMIZED;
    return OPT_DISPLAY;
}

static char *DisplayShadowModeOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;

    strcpy(ach, "OptionShadowMode\n");

    if (iValue==SHADOW_STENCIL)
        strcat(ach, "ShadowMode_Lowest");
    else if(iValue==SHADOW_SHADOWMAP_LOW)
        strcat(ach, "ShadowMode_Low");
    else if (iValue==SHADOW_SHADOWMAP_MEDIUM)
        strcat(ach, "ShadowMode_Medium");
    else if (iValue==SHADOW_SHADOWMAP_HIGH)
        strcat(ach, "ShadowMode_High");
    else if (iValue==SHADOW_SHADOWMAP_HIGHEST)
        strcat(ach, "ShadowMode_Highest");
    else
        strcat(ach, "OptionDisabled");

    if (iValue >= SHADOW_SHADOWMAP_LOW)
        gfxUpdateShadowMapAdvanced( &s_gfx );

    return ach;
}

static OptionDisplayType ShouldDisplayShadowMapOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!(rdr_caps.allowed_features & GFXF_SHADOWMAP))
        return OPT_DISABLE;
    if (s_gfx.advanced.shadowMode < SHADOW_SHADOWMAP_LOW)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayShadowMapShaderAdvancedOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!s_gfx.advanced.shadowMap.showAdvanced)
        return OPT_HIDE;
    if (s_gfx.advanced.shadowMode < SHADOW_SHADOWMAP_LOW || !(rdr_caps.allowed_features & GFXF_SHADOWMAP))
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static char *DisplayShadowMapShaderOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;

    strcpy(ach, "OptionShadowMapShader\n");

    if (iValue==SHADOWSHADER_HIGHQ)
        strcat(ach, "ShadowMapShader_HighQuality");
    else if (iValue==SHADOWSHADER_MEDIUMQ)
        strcat(ach, "ShadowMapShader_Medium");
    else
        strcat(ach, "ShadowMapShader_Fast"); // fast is default when nothing is set previously

    return ach;
}

static char *DisplayShadowMapSizeOption(void *pv)
{
    static char ach[128];

    strcpy(ach, "OptionShadowMapSize\n");

    // Apply restrictions
    // Note that we check against 2x width and height since the actual texture for
    // the shadow cascades will be 2x witdh and height of each cascade.
    if (s_gfx.advanced.shadowMap.size > SHADOWMAPSIZE_SMALL && rdr_caps.max_texture_size < 2048)
    {
        s_gfx.advanced.shadowMap.size = SHADOWMAPSIZE_SMALL;
    }
    else if (s_gfx.advanced.shadowMap.size > SHADOWMAPSIZE_MEDIUM && rdr_caps.max_texture_size < 4096)
    {
        s_gfx.advanced.shadowMap.size = SHADOWMAPSIZE_MEDIUM;
    }

    if (s_gfx.advanced.shadowMap.size==SHADOWMAPSIZE_LARGE)
        strcat(ach, "OptionShadowMapSize_Large");
    else if (s_gfx.advanced.shadowMap.size==SHADOWMAPSIZE_MEDIUM)
        strcat(ach, "OptionShadowMapSize_Medium");
    else
        strcat(ach, "OptionShadowMapSize_Small");

    return ach;
}

static char *DisplayShadowMapDistanceOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;

    strcpy(ach, "OptionShadowMapDistance\n");

    if (iValue==SHADOWDISTANCE_FAR)
        strcat(ach, "OptionShadowMapDistance_Far");
    else if (iValue==SHADOWDISTANCE_MIDDLE)
        strcat(ach, "OptionShadowMapDistance_Middle");
    else
        strcat(ach, "OptionShadowMapDistance_Close");

    return ach;
}

static char *DisplayCubemapModeOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;

    strcpy(ach, "OptionCubemapMode\n");

    if(iValue>=4)
        strcat(ach, "CubemapMode_UltraQuality");
    else if (iValue==3)
        strcat(ach, "CubemapMode_HighQuality");
    else if (iValue==2)
        strcat(ach, "CubemapMode_MediumQuality");
    else if (iValue==1)
        strcat(ach, "CubemapMode_LowQuality");
    else
        strcat(ach, "OptionDisabled");

    return ach;
}

static char *DisplayVBOsOption(void *pv)
{
    static char ach[128];
    int value = *(int *)pv;

    strcpy(ach, "OptionUseGeometryBuffers");
    strcat(ach,"\n");
    if(value==0 || !rdr_caps.use_vertshaders)
        strcat(ach, "OptionDisabled");
    else
        strcat(ach, "OptionEnabled");

    return ach;
}

static void ToggleVBOsOption(void *pv)
{
    int value = *(int *)pv;
    if(value==1 || !rdr_caps.use_vertshaders)
        value = 0;
    else
        value = 1;
    *(int*)pv = value;
}


static OptionDisplayType ShouldDisplayTextureAnisoOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (rdr_caps.maxTexAnisotropic<=1 || !rdr_caps.supports_anisotropy)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static char *DisplayTextureAnisoOption(void *pv)
{
    static char ach[128];
    int texAniso = *(int *)pv;

    strcpy(ach, "TextureAniso");

    strcat(ach,"\n");

    if(texAniso==1)
    {
        strcat(ach, "TextureAnisoOff");
    }
    else
    {
        strcatf(ach, "%dx", texAniso);
    }

    return ach;
}

static OptionDisplayType ShouldDisplayAdvancedOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldNotDisplayAdvancedOption(GameOptions *go)
{
    if (!s_gfx.showAdvanced)
        return OPT_DISPLAY;
    return OPT_CUSTOMIZED;
}

static int GetMaxMultisample()
{
    if (rdr_caps.supports_fbos)
        return rdr_caps.max_fbo_samples;
    else
        return rdr_caps.max_samples;
}

static bool SupportsMultisample()
{
    return GetMaxMultisample() > 1;
}

static OptionDisplayType ShouldDisplayAntialiasingOption(GameOptions *go)
{
    if (!SupportsMultisample())
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayCubemapOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!(rdr_caps.allowed_features & GFXF_CUBEMAP))
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayCursorOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
//    if (IsUsingWin9x() || isDevelopmentMode())
//        return OPT_DISPLAY;
//    return OPT_HIDE;
    return OPT_DISPLAY;
}


static void InitAntialiasingOption()
{
    int max_fsaa = GetMaxMultisample();
    int max_fsaa_log2 = logf(max_fsaa) / logf(2) + 1;

    if (s_gfx.antialiasing > 1)
        s_antialiasing = logf(s_gfx.antialiasing) / logf(2) + 1;
    else
        s_antialiasing = 1;

    s_paramAntiAliasingScale.count = MIN(max_fsaa_log2, MAX_OPTIONS_FSAA_LOG2);
    s_paramAntiAliasingScale.fMax = s_paramAntiAliasingScale.count;
}

static char *DisplayAntialiasingOption(void *pv)
{
    static char ach[128];

    strcpy(ach, "Antialiasing");

    strcat(ach,"\n");

    if (!SupportsMultisample())
    {
        s_gfx.antialiasing = 1;
        strcat(ach, "DisabledOptionDisplay");
    }
    else if(s_antialiasing<=1)
    {
        s_gfx.antialiasing = 1;
        strcat(ach, "AntialiasingOff");
    }
    else
    {
        s_gfx.antialiasing = 1 << (s_antialiasing - 1);
        strcatf(ach, "%dx", s_gfx.antialiasing);
    }

    return ach;
}

static void ToggleTextureQualityOption(void *pv)
{
    int iValue = *(int *)pv;

    iValue--;
    if (iValue<-1 || iValue>3)
        iValue=3;

    *(int *)pv = iValue;
}

static void TogglePhysicsQualityOption(void *pv)
{
    int iValue = *(int *)pv;

    iValue++;

    #if NOVODEX
        if (iValue<-1 || iValue>ePhysicsQuality_VeryHigh)
            iValue=ePhysicsQuality_None;
    #endif

    *(int *)pv = iValue;
}

static void ToggleEntityTextureQualityOption(void *pv)
{
    int iValue = *(int *)pv;

    iValue--;
    if (iValue<0 || iValue>4)
        iValue=4;

    *(int *)pv = iValue;
}

static void ToggleTextureLodBiasOption(void *pv)
{
    int iValue = *(int *)pv;

    iValue--;
    if (iValue<0 || iValue>2)
        iValue=2;

    *(int *)pv = iValue;
}

static void ToggleTextureAnisoOption(void *pv)
{
    static int saved_texLodBias=-1;
    int texAniso = *(int *)pv;

    if(texAniso>=rdr_caps.maxTexAnisotropic)
    {
        if (saved_texLodBias != -1)
            s_gfx.advanced.texLodBias = saved_texLodBias;
        texAniso = 1;
    }
    else
    {
        if (s_gfx.advanced.texLodBias!=2)
            saved_texLodBias = s_gfx.advanced.texLodBias;
        s_gfx.advanced.texLodBias = 2;
        texAniso *= 2;
    }

    *(int *)pv = texAniso;
}

static OptionDisplayType ShouldDisplayShaderDetailOption(GameOptions *go)
{
    OptionGraphicsPreset preset;

    IS_AN_ADVANCED_OPTION
    
    preset = gfxGetPresetEnumFromScale(s_gfx.slowUglyScale);

    // Don't allow changing of the shader detail if we are in ultra mode
    if (preset == GRAPHICSPRESET_ULTRA_LOW || preset == GRAPHICSPRESET_ULTRA_HIGH)
        return OPT_DISABLE;

    if (shaderDetailIncAllowed(SHADERDETAIL_NOBUMPS, NULL) == SHADERDETAIL_NOBUMPS)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static char *DisplayShaderDetailOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;

    strcpy(ach, "ShaderQuality");

    strcat(ach,"\n");

    if (shaderDetailIncAllowed(SHADERDETAIL_NOBUMPS, NULL) == SHADERDETAIL_NOBUMPS) {
        // No options
        strcat(ach, "DisabledOptionDisplay");
    } else {
        if (iValue==SHADERDETAIL_GF4MODE)
            strcat(ach, "ShaderQuality0");
        else if (iValue==SHADERDETAIL_SINGLE_MATERIAL)
            strcat(ach, "ShaderQuality1");
        else if (iValue==SHADERDETAIL_DUAL_MATERIAL || iValue==SHADERDETAIL_HIGHEST_AVAILABLE)
            strcat(ach, "ShaderQuality2");
        else if (iValue==SHADERDETAIL_NOBUMPS)
            strcat(ach, "ShaderQuality4");
        else if (iValue==SHADERDETAIL_NOWORLDBUMPS)
            strcat(ach, "ShaderQuality5");
        else
            strcat(ach, "BadShaderQuality");
    }
    return ach;
}


static void ToggleShaderDetailOption(void *pv)
{
    int iValue = *(int *)pv;

    iValue = shaderDetailIncAllowed(iValue, NULL);

    *(int *)pv = iValue;
}


static OptionDisplayType ShouldDisplayWaterOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (gfxGetMaxWaterMode()==0)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static char *DisplayWaterOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;
    int max=gfxGetMaxWaterMode();

    strcpy(ach, "WaterEffects");

    strcat(ach,"\n");

    if (max==0) {
        // Ideally if max == 0, this option should not be displayed
        strcat(ach, "DisabledOptionDisplay");
    } else {
        if (iValue==0)
            strcat(ach, "WaterEffectsOff");
        else if (iValue==1)
            strcat(ach, "WaterEffectsLow");
        else if (iValue==2)
            strcat(ach, "WaterEffectsMedium");
        else if (iValue==3)
            strcat(ach, "WaterEffectsHigh");
        else //if (iValue==4)
            strcat(ach, "WaterEffectsUltra");
    }
    return ach;
}

static OptionDisplayType ShouldDisplayBuildingPROption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION

    // Planar Reflections require cubemaps to be enabled
    if (ShouldDisplayCubemapOption(go) != OPT_DISPLAY || s_gfx.advanced.cubemapMode == CUBEMAP_OFF) 
        return OPT_DISABLE;

    // Determine what levels are allowed by their card
    if (rdr_caps.features & GFXF_WATER)
        return OPT_DISPLAY;

    return OPT_DISABLE;
}


int maxDOFSetting(void)
{
    int max=0;
    // Determine what levels are allowed by their card
    if (rdr_caps.allowed_features & GFXF_DOF) {
        max = 1;
    }
    return max;
}

static OptionDisplayType ShouldDisplayDOFOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (maxDOFSetting()==0)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayDOFAmountOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (maxDOFSetting()==0)
        return OPT_DISABLE;
    if (s_gfx.advanced.useDOF==0)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static char *DisplayDOFOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;
    int max=maxDOFSetting();

    strcpy(ach, "DOFEffects");

    strcat(ach,"\n");

    if (iValue > max)
        iValue = max;

    if (max==0) {
        // Ideally if max == 0, this option should not be displayed
        strcat(ach, "DisabledOptionDisplay");
    } else {
        if (iValue==0)
            strcat(ach, "OptionDisabled");
        else
            strcat(ach, "OptionEnabled");
    }
    return ach;
}


static void ToggleDOFOption(void *pv)
{
    int iValue = *(int *)pv;
    int max = maxDOFSetting();

    iValue++;
    if (iValue<0)
        iValue=0;
    if (iValue > max)        
        iValue=0;

    *(int *)pv = iValue;
}

int maxBloomSetting(void)
{
    int max=0;
    // Determine what levels are allowed by their card
    if (rdr_caps.allowed_features & GFXF_BLOOM) {
        max = 2;
    }
    return max;
}

static OptionDisplayType ShouldDisplayBloomOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (maxBloomSetting()==0)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayBloomAmountOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (maxBloomSetting()==0)
        return OPT_DISABLE;
    if (s_gfx.advanced.useBloom==BLOOM_OFF)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}


static OptionDisplayType ShouldDisplayAgeiaSupportOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    #if NOVODEX
        if (!nwHardwarePresent())
            return OPT_DISABLE;
        return OPT_DISPLAY;
    #else
        return OPT_DISABLE;
    #endif
}

typedef struct debrisMinMaxSliderParams
{
    F32 fMin;
    F32 fMaxHW;
    F32 fMaxSW;
} debrisMinMaxSliderParams;

static debrisMinMaxSliderParams dMMParams = { -1.0f, 1.0f, -0.3f };

static void getDebrisSliderMinMax(float* fMin, float* fMax)
{
    *fMin = dMMParams.fMin;
    if ( s_gfx.advanced.ageiaOn )
        *fMax = dMMParams.fMaxHW;
    else
        *fMax = dMMParams.fMaxSW;

}

static char *DisplayBloomOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;
    int max=maxBloomSetting();

    strcpy(ach, "BloomEffects");

    strcat(ach,"\n");

    if (iValue > max)
        iValue = max;

    if (max==0) {
        // Ideally if max == 0, this option should not be displayed
        strcat(ach, "DisabledOptionDisplay");
    } else {
        if (iValue==0)
            strcat(ach, "BloomEffectsOff");
        else if (iValue==1)
            strcat(ach, "BloomEffectsRegular");
        else //if (iValue==2)
            strcat(ach, "BloomEffectsHeavy");
    }
    return ach;
}

static void ToggleBloomOption(void *pv)
{
    int iValue = *(int *)pv;
    int max = maxBloomSetting();

    iValue++;
    if (iValue<0)
        iValue=0;
    if (iValue > max)        
        iValue=0;

    *(int *)pv = iValue;
}


static int desaturateEnabled(void)
{
    return (rdr_caps.allowed_features & (GFXF_DESATURATE));
}

static OptionDisplayType ShouldDisplayDesaturateOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (desaturateEnabled()==0)
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static char *DisplayDesaturateOption(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;
    int enabled=desaturateEnabled();

    strcpy(ach, "DesaturateEffects\n");

    if (!enabled) {
        strcat(ach, "DisabledOptionDisplay");
    } 
    else
    {
        if (iValue)
            strcat(ach, "OptionEnabled");
        else
            strcat(ach, "OptionDisabled");
    }
    return ach;
}


static void ToggleDesaturateOption(void *pv)
{
    int iValue = *(int *)pv;
    int allowed = desaturateEnabled();

    if(allowed)
        iValue = !iValue;
    *(int *)pv = iValue;
}

static OptionDisplayType ShouldDisplayAmbient(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!(rdr_caps.allowed_features & GFXF_AMBIENT))
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayAmbientOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!(rdr_caps.allowed_features & GFXF_AMBIENT))
        return OPT_DISABLE;
    if (s_gfx.advanced.ambient.showAdvanced)
        return OPT_CUSTOMIZED;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayAmbientAdvanced(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!s_gfx.advanced.ambient.showAdvanced)
        return OPT_HIDE;
    if (!(rdr_caps.allowed_features & GFXF_AMBIENT))
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayAmbientAdvancedSubOption(GameOptions *go)
{
    IS_AN_ADVANCED_OPTION
    if (!s_gfx.advanced.ambient.showAdvanced)
        return OPT_HIDE;
    if (s_gfx.advanced.ambient.strength==AMBIENT_OFF)
        return OPT_DISABLE;
    if (!(rdr_caps.allowed_features & GFXF_AMBIENT))
        return OPT_DISABLE;
    return OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplay3DSoundOption(GameOptions *go)
{
    // This is the most common failure case. Checking here saves us from a more expensive check
    //  and an incorrectly labeled option. Revisit when the sound issue is solved (see sound_sys.c,
    //  createTheListener()).
    if (IsUsingVistaOrLater())
        g_audio_state.surroundFailed = 1;

    return g_audio_state.surroundFailed ? OPT_DISABLE : OPT_DISPLAY;
}

static OptionDisplayType ShouldDisplayHardwareLightOption(GameOptions *go)
{
    return hwlightIsPresent() ? OPT_DISPLAY : OPT_DISABLE;
}

static char *DisplayAmbientOption(void *pv)
{
    static char ach[128];
    float fValue = *(float *)pv;

    strcpy(ach, "AmbientOption\n");

    if(fValue < s_paramAmbientOptionScale.values[0]+SLIDER_EPSILON) {
        strcat(ach, "AmbientOptionDisable");
        s_gfx.advanced.ambient.option = AMBIENT_DISABLE;
    } else if (fValue < s_paramAmbientOptionScale.values[1]+SLIDER_EPSILON) {
        strcat(ach, "AmbientOptionHighPerformance");
        s_gfx.advanced.ambient.option = AMBIENT_HIGH_PERFORMANCE;
    } else if (fValue < s_paramAmbientOptionScale.values[2]+SLIDER_EPSILON) {
        strcat(ach, "AmbientOptionPerformance");
        s_gfx.advanced.ambient.option = AMBIENT_PERFORMANCE;
    } else if (fValue < s_paramAmbientOptionScale.values[3]+SLIDER_EPSILON) {
        strcat(ach, "AmbientOptionBalanced");
        s_gfx.advanced.ambient.option = AMBIENT_BALANCED;
    } else if (fValue < s_paramAmbientOptionScale.values[4]+SLIDER_EPSILON) {
        strcat(ach, "AmbientOptionQuality");
        s_gfx.advanced.ambient.option = AMBIENT_QUALITY;
    } else if (fValue < s_paramAmbientOptionScale.values[5]+SLIDER_EPSILON) {
        strcat(ach, "AmbientOptionHighQuality");
        s_gfx.advanced.ambient.option = AMBIENT_HIGH_QUALITY;
    } else if (fValue < s_paramAmbientOptionScale.values[6]+SLIDER_EPSILON) {
        strcat(ach, "AmbientOptionSuperHighQuality");
        s_gfx.advanced.ambient.option = AMBIENT_SUPER_HIGH_QUALITY;
    } else {
        strcat(ach, "AmbientOptionUltra");
        s_gfx.advanced.ambient.option = AMBIENT_ULTRA_QUALITY;
    }

    gfxUpdateAmbientAdvanced( &s_gfx );
    return ach;
}

static int maxAmbientStrength()
{
    int max = 0;
    if (rdr_caps.allowed_features & GFXF_AMBIENT)
        max = AMBIENT_ULTRA;
    return max;
}

static char *DisplayAmbientStrength(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;
    int max = maxAmbientStrength();

    strcpy(ach, "AmbientStrength\n");

    if (max==0) {
        strcat(ach, "DisabledOptionDisplay");
    } else {
        if (iValue==AMBIENT_OFF)
            strcat(ach, "AmbientStrengthOff");
        else if (iValue==AMBIENT_LOW)
            strcat(ach, "AmbientStrengthLow");
        else if (iValue==AMBIENT_MEDIUM)
            strcat(ach, "AmbientStrengthMedium");
        else if (iValue==AMBIENT_HIGH)
            strcat(ach, "AmbientStrengthHigh");
        else //if (iValue==AMBIENT_ULTRA)
            strcat(ach, "AmbientStrengthUltra");
    }
    return ach;
}

static void ToggleAmbientStrength(void *pv)
{
    int iValue = *(int *)pv;
    int max = maxAmbientStrength();

    iValue++;
    if (iValue<0)
        iValue=0;
    if (iValue > max)
        iValue=0;

    *(int *)pv = iValue;
}

static int maxAmbientResolution()
{
    int max = 0;
    if (rdr_caps.allowed_features & GFXF_AMBIENT)
        max = AMBIENT_RES_SUPER_HIGH_QUALITY;
    return max;
}

static char *DisplayAmbientResolution(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;
    int max = maxAmbientResolution();

    strcpy(ach, "AmbientResolution\n");

    if (max==0) {
        strcat(ach, "DisabledOptionDisplay");
    } else {
        if (iValue==AMBIENT_RES_HIGH_PERFORMANCE)
            strcat(ach, "AmbientOptionHighPerformance");
        else if (iValue==AMBIENT_RES_PERFORMANCE)
            strcat(ach, "AmbientOptionPerformance");
        else if (iValue==AMBIENT_RES_QUALITY)
            strcat(ach, "AmbientOptionQuality");
        else if (iValue==AMBIENT_RES_HIGH_QUALITY)
            strcat(ach, "AmbientOptionHighQuality");
        else //if (iValue==AMBIENT_RES_SUPER_HIGH_QUALITY)
            strcat(ach, "AmbientOptionSuperHighQuality");
    }
    return ach;
}

static void ToggleAmbientResolution(void *pv)
{
    int iValue = *(int *)pv;
    int max = maxAmbientResolution();

    iValue++;
    if (iValue<0)
        iValue=0;
    if (iValue > max)
        iValue=0;

    *(int *)pv = iValue;
}

static int maxAmbientBlur()
{
    int max = 0;
    if (rdr_caps.allowed_features & GFXF_AMBIENT)
        max = AMBIENT_TRILATERAL;
    return max;
}

static char *DisplayAmbientBlur(void *pv)
{
    static char ach[128];
    int iValue = *(int *)pv;
    int max = maxAmbientBlur();

    strcpy(ach, "AmbientBlur\n");

    if (max==0) {
        strcat(ach, "DisabledOptionDisplay");
    } else {
        if (iValue==AMBIENT_NO_BLUR)
            strcat(ach, "AmbientNoBlur");
        else if (iValue==AMBIENT_FAST_BLUR)
            strcat(ach, "AmbientFastBlur");
        else if (iValue==AMBIENT_GAUSSIAN)
            strcat(ach, "AmbientGaussian");
        else if (iValue==AMBIENT_BILATERAL)
            strcat(ach, "AmbientBilateral");
        else if (iValue==AMBIENT_GAUSSIAN_DEPTH)
            strcat(ach, "AmbientGaussianDepth");
        else if (iValue==AMBIENT_BILATERAL_DEPTH)
            strcat(ach, "AmbientBilateralDepth");
        else //if (iValue==AMBIENT_TRILATERAL)
            strcat(ach, "AmbientTrilateral");
    }
    return ach;
}

static void ToggleAmbientBlur(void *pv)
{
    int iValue = *(int *)pv;
    int max = maxAmbientBlur();

    iValue++;
    if (iValue<0)
        iValue=0;
    if (iValue > max)
        iValue=0;

    *(int *)pv = iValue;
}

void gfxUpdateAmbientAdvanced( GfxSettings * gfxSettings )
{
    if(gfxSettings->showAdvanced && gfxSettings->advanced.ambient.showAdvanced) {
        return;
    } else if(!gfxSettings->showAdvanced) {
        gfxSettings->advanced.ambient.showAdvanced = false;
    }

    gfxSettings->advanced.ambient.optionScale = s_paramAmbientOptionScale.values[gfxSettings->advanced.ambient.option];

    switch(gfxSettings->advanced.ambient.option)
    {
        case AMBIENT_DISABLE:
            gfxSettings->advanced.ambient.strength = AMBIENT_OFF;
            gfxSettings->advanced.ambient.blur = AMBIENT_BILATERAL;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_PERFORMANCE;
            break;
        case AMBIENT_HIGH_PERFORMANCE:
            gfxSettings->advanced.ambient.strength = AMBIENT_LOW;
            gfxSettings->advanced.ambient.blur = AMBIENT_FAST_BLUR;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_HIGH_PERFORMANCE;
            break;
        case AMBIENT_PERFORMANCE:
            gfxSettings->advanced.ambient.strength = AMBIENT_LOW;
            gfxSettings->advanced.ambient.blur = AMBIENT_BILATERAL;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_PERFORMANCE;
            break;
        case AMBIENT_BALANCED:
            gfxSettings->advanced.ambient.strength = AMBIENT_MEDIUM;
            gfxSettings->advanced.ambient.blur = AMBIENT_BILATERAL;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_PERFORMANCE;
            break;
        case AMBIENT_QUALITY:
            gfxSettings->advanced.ambient.strength = AMBIENT_MEDIUM;
            gfxSettings->advanced.ambient.blur = AMBIENT_TRILATERAL;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_QUALITY;
            break;
        case AMBIENT_HIGH_QUALITY:
            gfxSettings->advanced.ambient.strength = AMBIENT_HIGH;
            gfxSettings->advanced.ambient.blur = AMBIENT_TRILATERAL;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_QUALITY;
            break;
        case AMBIENT_SUPER_HIGH_QUALITY:
            gfxSettings->advanced.ambient.strength = AMBIENT_HIGH;
            gfxSettings->advanced.ambient.blur = AMBIENT_TRILATERAL;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_HIGH_QUALITY;
            break;
        case AMBIENT_ULTRA_QUALITY:
            gfxSettings->advanced.ambient.strength = AMBIENT_ULTRA;
            gfxSettings->advanced.ambient.blur = AMBIENT_TRILATERAL;
            gfxSettings->advanced.ambient.resolution = AMBIENT_RES_QUALITY;
            break;
    }
}

static char *DisplayMousePitchOption(UserOption *pv)
{
    static char ach[128];
    MousePitchOption option = pv->iVal;

    strcpy(ach, "MousePitchOption");
    strcat(ach,"\n");

    if(option==MOUSEPITCH_FIXED)
        strcat(ach, "MousePitchFixed");
    else if(option==MOUSEPITCH_SPRING)
        strcat(ach, "MousePitchSpring");
    else
        strcat(ach, "MousePitchFree");
    return ach;
}

static void ToggleMousePitchOption(UserOption *pv)
{
    if (pv->iVal == MOUSEPITCH_FIXED)
        pv->iVal = MOUSEPITCH_FREE;
    else
        pv->iVal++;
}

static void ResetWindows(void *pv)
{
    windows_ResetLocations();
    gWindowRGBA[0] = (winDefs[WDW_STAT_BARS].loc.color>>24 & 0xff);
    gWindowRGBA[1] = (winDefs[WDW_STAT_BARS].loc.color>>16 & 0xff);
    gWindowRGBA[2] = (winDefs[WDW_STAT_BARS].loc.color>>8  & 0xff);
    gWindowRGBA[3] = (winDefs[WDW_STAT_BARS].loc.back_color & 0xff);
    s_fR = gWindowRGBA[0]/255.0f;
    s_fG = gWindowRGBA[1]/255.0f;
    s_fB = gWindowRGBA[2]/255.0f;
    s_fA = gWindowRGBA[3]/255.0f;
    s_fWindowScale = winDefs[WDW_STAT_BARS].loc.sc;
    s_paramWindowScale.fMax = window_getMaxScale();
}


typedef char *(*DispFunc)(void *);
typedef void (*ToggleFunc)(void *);

void initToolTips(GameOptions *popt)
{ //see if there's a tooltip string, if so add tooltip
    CBox box;
     BuildCBox( &box, 0,0,0,0 );
    while(popt->pvDisplay!=NULL)
    {
        if (!popt->pToolTip)
        {
            char *pch;
            char *temp;
            char *buffer;
            if (popt->eTypeDisplay == kOptionType_Func)
            {
                int len;
                temp = ((DispFunc)popt->pvDisplay)(popt->id?userOptionGet(popt->id):popt->pvDisplayParam);
                if (!temp) continue;
                pch = strchr(temp, '\n'); //find first line
                if (!pch)
                { //if no newline, skip
                    popt++;
                    continue;
                }
                len = pch - temp;
                buffer = malloc(sizeof(char) * (len+8+4));
                strncpyt(buffer,temp,len+1);
                strcpy(buffer+len,"ToolTip"); //construct basestring+ToolTip
            }
            else
            { //Otherwise base string = the paramater
                temp = (char*)popt->pvDisplay;
                if (!temp) continue;
                buffer = malloc(sizeof(char) * (strlen(temp)+8+4));
                sprintf(buffer,"%sToolTip",temp);
            }
            pch=NULL;
            if (rdr_caps.chip & R200) 
            {
                // Look for ATI-specific tool-tips.  We hate ATI.
                strcat(buffer, "ATI");
                pch = textStd(buffer);
                if (strcmp(buffer,pch) == 0) 
                {
                    // Didn't find a lookup
                    buffer[strlen(buffer)-3]='\0';
                    pch = NULL;
                }
            }
            if (!pch)
            {
                pch = textStd(buffer);
            }
            if (strcmp(buffer,pch) == 0)
            {
                free(buffer);
                popt++;
                continue;
            }
            popt->pToolTip = (ToolTip *)calloc(sizeof(ToolTip),1);
            setToolTipEx(popt->pToolTip,&box,pch,NULL,MENU_GAME,WDW_OPTIONS,1,TT_NOTRANSLATE);
            addToolTip(popt->pToolTip);
            free(buffer);
        }
        popt++;
    }
}



void clearToolTips(GameOptions *popt)
{ //disable tooltips in other tab
    while(popt->pvDisplay!=NULL)
    {
        if (popt->pToolTip)
            clearToolTip(popt->pToolTip);
        popt++;
    }
}

void saveInitial(GameOptions *popt)
{ //save all values for undo
    while(popt->pvDisplay!=NULL)
    {
        switch(popt->eTypeToggle)
        {
        case kOptionType_Bool:
            if (!popt->pSave) 
                popt->pSave = calloc(1, sizeof(bool));
            if( popt->id )
                *(bool *)popt->pSave = optionGet(popt->id);
            else if (popt->pvToggle)
                *(bool *)popt->pSave = *(bool *)popt->pvToggle;
            break;
        case kOptionType_Func:
            if (!popt->pSave)
                popt->pSave = calloc(1, sizeof(int));

            if( popt->id )
                *(int *)popt->pSave = optionGet(popt->id);
            else if (popt->pvToggleParam)
                *(int *)popt->pSave = *(int *)popt->pvToggleParam;
            break;
        case kOptionType_IntSlider:
        case kOptionType_IntMinMaxSlider:
        case kOptionType_IntSnapSlider:
            if (!popt->pSave)
                popt->pSave = calloc(1, sizeof(int));

            if( popt->id )
            {
                UserOption *pUO = userOptionGet(popt->id);
                if( pUO->bitSize < 0 )
                    *(int *)popt->pSave = (int)pUO->fVal;
                else
                    *(int *)popt->pSave = pUO->iVal;
            }
            else if (popt->pvToggle)
                *(int *)popt->pSave = *(int *)popt->pvToggle;
            break;
        case kOptionType_FloatSlider:
        case kOptionType_SnapSlider:
        case kOptionType_SnapSliderUltra:
        case kOptionType_MinMaxSlider:
        case kOptionType_SnapMinMaxSlider:
            if (!popt->pSave)
                popt->pSave = calloc(1, sizeof(float));

            if( popt->id )
                *(float *)popt->pSave = optionGetf(popt->id);
            if (popt->pvToggle)
                *(float *)popt->pSave = *(float *)popt->pvToggle;
            break;
        }
        popt++;
    }
}

void loadInitial(GameOptions *popt)
{
    while(popt->pvDisplay!=NULL)
    {
        if (!popt->pSave) 
        { //skip unsaved ones
            popt++;
            continue;
        }
        switch(popt->eTypeToggle)
        {
        case kOptionType_Bool:
            if(popt->id)
                optionSet(popt->id,*(bool *)popt->pSave, 0);
            else if (popt->pvToggle)
                *(bool *)popt->pvToggle = *(bool *)popt->pSave;
            break;
        case kOptionType_Func:
            if(popt->id)
                optionSet(popt->id,*(int *)popt->pSave, 0);
            else if (popt->pvToggleParam)
                *(int *)popt->pvToggleParam = *(int *)popt->pSave;
            break;
        case kOptionType_IntSlider:
        case kOptionType_IntMinMaxSlider:
        case kOptionType_IntSnapSlider:
            if(popt->id)
            {
                UserOption *pUO = userOptionGet(popt->id);
                if( pUO->bitSize < 0 )
                    pUO->fVal = (int)(*(int *)popt->pSave);
                else
                    pUO->iVal = *(int *)popt->pSave;
            }
            else if (popt->pvToggle)
                 *(int *)popt->pvToggle = *(int *)popt->pSave;
            break;
        case kOptionType_FloatSlider:
        case kOptionType_SnapSlider:
        case kOptionType_SnapSliderUltra:
        case kOptionType_MinMaxSlider:
        case kOptionType_SnapMinMaxSlider:
            if(popt->id)
                optionSetf(popt->id,*(float *)popt->pSave, 0);
            else if (popt->pvToggle)
                *(float *)popt->pvToggle = *(float *)popt->pSave;
            break;
        }
        popt++;
    }
}

void *getOldValue(GameOptions *popt, void * curValue)
{
    while(popt->pvDisplay!=NULL)
    {
        if (!popt->pSave) 
        { //skip unsaved ones
            popt++;
            continue;
        }
        switch(popt->eTypeToggle) 
        {
        case kOptionType_Bool:
        case kOptionType_IntSlider:
        case kOptionType_IntMinMaxSlider:
        case kOptionType_IntSnapSlider:
        case kOptionType_FloatSlider:
        case kOptionType_SnapSlider:
        case kOptionType_SnapSliderUltra:
        case kOptionType_SnapMinMaxSlider:
        case kOptionType_MinMaxSlider:
            if (popt->pvToggle == curValue)
                return popt->pSave;
            break;
        case kOptionType_Func:
            if (popt->pvToggleParam == curValue)
                return popt->pSave;
            break;
        }
        popt++;
    }
    return NULL;
}


/**************************************************************************/



#define LINE_HEIGHT 22

int HandleOptionList(GameOptions *popt, float start_x, float y, float xVals, float yVals, float start_wd, int z, float sc, CBox *windowBounds, int color, int bcolor)
{
    float starty = y;
    UIBox uibox;

      int i=0, fontcolor;
    const int HilightColor = (color & 0xffffff00) | 0x44;
    const int frameBorderColor = (color & 0xffffff00) | 0x44;
    const int frameBgColor = (color & 0xffffff00) | 0x22;
    const int helpFgColor = ((color >> 1) & 0x7f7f7f00) + 0x404040FF;
    const int helpBgColor = ((color >> 1) & 0x7f7f7f00) | 0xFF;

    const F32 x = start_x + 25*sc;
    const F32 width = start_wd - 25*sc;

    while(popt->pvDisplay!=NULL)
    {
        int visible = 1;
        char *pch = NULL;
        char *pchTmp = NULL;
        bool bRet = false;
        bool custDisplay = false;
        OptionDisplayType dis;
        CBox box;
        
        font(&game_12);
        
        if(popt->doDisplay)
            dis = popt->doDisplay(popt);
        else
            dis = OPT_DISPLAY;

        if(dis == OPT_HIDE) 
        {
            popt++;
            continue;
        }

        i++;
        if (dis == OPT_DISABLE)
            fontcolor = CLR_GREY;
        else if(dis == OPT_CUSTOMIZED)
            fontcolor = CLR_GREYED_RED;
        else
            fontcolor = CLR_WHITE;

        if (windowBounds && (y+(i*LINE_HEIGHT)*sc < windowBounds->top || y+(i*LINE_HEIGHT+15)*sc > windowBounds->bottom))
            visible = 0;

         BuildCBox( &box, start_x,y+(i*LINE_HEIGHT-12)*sc,start_wd,(LINE_HEIGHT-10)*sc );
        if(popt->pToolTip) 
        {
            if (visible) 
                setToolTipEx(popt->pToolTip,&box,popt->pToolTip->sourcetxt,NULL,MENU_GAME,WDW_OPTIONS,optionGetf(kUO_ToolTipDelay)*50,TT_NOTRANSLATE);
            else 
                clearToolTip(popt->pToolTip);
        }

         if( popt->pchHelp )
               helpButtonFullyConfigurable( &popt->pHelpButton, start_x + 10*sc, y+(i*LINE_HEIGHT*sc) - 6*sc, z, sc, popt->pchHelp, &box, 0,
                  CLR_MM_HELP_TEXT, CLR_MM_HELP_DARK, CLR_MM_HELP_MID, helpFgColor, helpBgColor );

           uiBoxDefine(&uibox, start_x,y+(i*LINE_HEIGHT-12)*sc,start_wd/2-15*sc,(LINE_HEIGHT-10)*sc );
        

        if( kOptionType_Title != popt->eTypeDisplay && kOptionType_TitleUltra != popt->eTypeDisplay ) 
        {
            CBox box;
            BuildCBox(&box, x - 10*sc, y+(i*LINE_HEIGHT - 15)*sc, width, (LINE_HEIGHT-4)*sc);
            if ( mouseCollision( &box ))
                drawStdButtonMeat( x - 10*sc, y+(i*LINE_HEIGHT - 16)*sc, z, width, (LINE_HEIGHT-4)*sc, HilightColor );
        }
        
        switch(popt->eTypeDisplay)
        {
            case kOptionType_Bool:
                assert(popt->pchTrue && popt->pchFalse);
                if((pch=(char *)popt->pvDisplay)!=NULL)
                {
                    font_color(fontcolor, fontcolor);
                    if (visible) prnt(x, y+(i*LINE_HEIGHT)*sc, z+1, sc, sc, pch);
                }

                if( popt->id )
                    pch = optionGet(popt->id)? popt->pchTrue:popt->pchFalse;
                else if(popt->pvDisplayParam)
                    pch = *(bool *)popt->pvDisplayParam ? popt->pchTrue : popt->pchFalse;
                break;

            case kOptionType_Func:
                pch = ((DispFunc)popt->pvDisplay)(popt->id?userOptionGet(popt->id):popt->pvDisplayParam);
                break;

            case kOptionType_String:
                pch = (char *)popt->pvDisplay;
                font_color(fontcolor, fontcolor);
                if (visible)
                {
                    clipperPushRestrict(&uibox); 
                    prnt(x, y+(i*LINE_HEIGHT*sc), z+1, sc, sc, pch);
                    clipperPop();
                }
                break;

            case kOptionType_Title:
            case kOptionType_TitleUltra:
                pch = (char *)popt->pvDisplay;
                if (starty != y) 
                {
                    if( popt->eTypeDisplay == kOptionType_TitleUltra)
                         drawFrame( PIX2, R6, x-37*sc, starty, z, width+35*sc,y+(i*(LINE_HEIGHT)- LINE_HEIGHT/2)*sc-starty,sc, frameBorderColor, 0xffff0044 );
                    else
                          drawFrame( PIX2, R6, x-37*sc, starty, z, width+35*sc,y+(i*(LINE_HEIGHT)- LINE_HEIGHT/2)*sc-starty,sc, frameBorderColor, frameBgColor);
                }
                starty = y+(i*LINE_HEIGHT + LINE_HEIGHT/4)*sc;
                y+=LINE_HEIGHT/2*sc; // wild cheat here
                yVals+=LINE_HEIGHT/2*sc; // and here.

                font( &title_14 );
                font_color(CLR_YELLOW, CLR_RED);
                if (visible) 
                    prnt(x, y+(i*LINE_HEIGHT)*sc, z+1, sc, sc, pch);
                break;
        }

        if(pch && (pchTmp=strchr(pch, '\n'))!=NULL)
        {

            *pchTmp='\0';
            font_color(fontcolor, fontcolor);
            if (visible) 
            {
                prnt(x, y+i*LINE_HEIGHT*sc, z+1, sc, sc, pch);
            }
            *pchTmp='\n';
            pchTmp++;
            custDisplay = true;
        }
        else
        {
            pchTmp = pch;
        }


        if (dis == OPT_DISABLE || dis == OPT_CUSTOMIZED)
        {
            if (visible) {
                if(dis == OPT_DISABLE)
                    prnt(xVals, yVals + i*LINE_HEIGHT*sc, z+1, sc,sc, "DisabledOptionDisplay");
                else
                    prnt(xVals, yVals + i*LINE_HEIGHT*sc, z+1, sc,sc, "CustomizedOptionDisplay");
            }
        } 
        else 
        {
            UserOption *pUO = NULL;
            if( popt->id )
                pUO = userOptionGet(popt->id);

            switch(popt->eTypeToggle)
            {
            case kOptionType_Bool:
                if(selectableText(xVals, yVals + i*LINE_HEIGHT*sc, z+1, sc, pchTmp, CLR_WHITE, CLR_WHITE, FALSE, TRUE))
                {
                    if( popt->id )
                        optionToggle( popt->id, 1 );
                    else
                        *(bool *)popt->pvToggle = !*(bool *)popt->pvToggle;
                }

                if(  stricmp( pch, "OptionDisabledUltra" ) == 0 ) // special hack until ultra mode is no longer special
                {
                       drawFlatFrame( PIX3, R4, xVals + 75*sc , yVals + i*LINE_HEIGHT*sc - 16*sc, z, 200*sc, 20*sc, sc, 0xffff0066, 0xffff0066 );
                     if(selectableText(xVals + 83*sc, yVals + i*LINE_HEIGHT*sc, z+1, sc*.9, "ClickForUltra", CLR_WHITE, CLR_WHITE, FALSE, TRUE))
                    {
                        if( popt->id )
                            optionToggle( popt->id, 1 );
                        else
                            *(bool *)popt->pvToggle = !*(bool *)popt->pvToggle;
                    }
                }
                
                break;

            case kOptionType_Func:
                if(selectableText(xVals, yVals + i*LINE_HEIGHT*sc, z+1, sc, pchTmp, CLR_WHITE, CLR_WHITE, FALSE, TRUE))
                {
                    if (popt->pvToggle)
                        ((ToggleFunc)popt->pvToggle)(popt->id?userOptionGet(popt->id):popt->pvToggleParam);
                }
                break;

            case kOptionType_FloatSlider:
            case kOptionType_IntSlider:
            case kOptionType_IntMinMaxSlider:
            case kOptionType_IntSnapSlider:
            case kOptionType_SnapSlider:
            case kOptionType_SnapSliderUltra:
            case kOptionType_SnapMinMaxSlider:
            case kOptionType_MinMaxSlider:
                {
                    SliderParam *pparam = (SliderParam *)popt->pvToggleParam;
                    float fVal = 0;
                    int iWidth = 190 * pparam->fSize;
                    int iOffset = 190 - iWidth;
                    char ach[128] = { 0 };
                    
                    if(popt->eTypeToggle==kOptionType_FloatSlider || 
                        popt->eTypeToggle==kOptionType_SnapSlider ||
                        popt->eTypeToggle==kOptionType_SnapSliderUltra ||
                        popt->eTypeToggle==kOptionType_SnapMinMaxSlider ||
                        popt->eTypeToggle==kOptionType_MinMaxSlider )
                    {
                        if( pUO && pUO->bitSize < 0 )
                            fVal = pUO->fVal;
                        else if( pUO )
                            fVal = (float)pUO->iVal;
                        else    
                            fVal = *(float *)popt->pvToggle;
                    }
                    else
                    {
                        if( pUO && pUO->bitSize < 0 )
                            fVal = pUO->fVal;
                        else if( pUO )
                            fVal = (float)pUO->iVal;
                        else
                            fVal = *(int *)popt->pvToggle+0.001f;
                    }

                    // translate fVal from a value between fMin and fMax
                    // to a value between 0.0f and 1.0f.
                    if (pparam->fMax == pparam->fMin)
                    {
                        fVal = 0.0f;
                    }
                    else if (popt->eTypeToggle != kOptionType_SnapSlider
                        && popt->eTypeToggle != kOptionType_SnapSliderUltra
                        && popt->eTypeToggle != kOptionType_SnapMinMaxSlider)
                    {                    
                        if(pparam->fNormal==pparam->fMax || pparam->fNormal==pparam->fMin)
                        {
                            fVal = (fVal-pparam->fMin)/(pparam->fMax-pparam->fMin);
                        }
                        else if(fVal<pparam->fNormal)
                        {
                            fVal = (fVal-pparam->fMin)/(pparam->fNormal-pparam->fMin)*0.8f;
                        }
                        else
                        {
                            fVal = ((fVal-pparam->fNormal)/(pparam->fMax-pparam->fNormal)*0.2f)+0.8f;
                        }
                    }

                    fVal = (fVal*2.0f)-1.0f;
                    if (popt->eTypeToggle == kOptionType_SnapSlider || popt->eTypeToggle == kOptionType_SnapSliderUltra || popt->eTypeToggle == kOptionType_IntSnapSlider)
                    {
                        SnapParam *snap = (SnapParam *)popt->pvToggleParam;
                        int j;
                        float ticks[50];
                        float min, max, invRange;

                        onlydevassert(snap->count <= 50);
                        onlydevassert(snap->fMin < snap->fMax);

                        // doSliderTicks() assumes the range of the tick values is from -1 to 1
                        // doSliderAll seems to not respect the minval parameter
                        min = snap->fMin;
                        max = snap->fMax;
                        invRange = 1.0f / (max - min);

                        for (j = 0; j < snap->count; j++)
                        {
                            ticks[j] = (snap->values[j] - min) * invRange * 2.0f - 1.0f;
                        }

                        fVal = doSliderTicks(xVals+(80+iWidth/2+iOffset)*sc, yVals + (i*LINE_HEIGHT-7)*sc, z+1, iWidth, sc, sc, fVal, i, color, popt->eTypeToggle==kOptionType_SnapSliderUltra?BUTTON_ULTRA:0, snap->count, ticks);
                    }
                    else if (popt->eTypeToggle == kOptionType_SnapMinMaxSlider)
                    {    
                        SnapParam *snap = (SnapParam *)popt->pvToggleParam;
                        int j;
                        float min, max;

                        // Odd, this assumes the snap values are from 0 to 1 but still calls getSliderMinMax()
                        // This could probably be combined with the above slider logic
                        for (j = 0; j < snap->count; j++)
                        {
                            snap->values[j] = (snap->values[j]*2.0f)-1.0f;
                        }
                        popt->getSliderMinMax(&min, &max);
                        fVal = doSliderAll(xVals+(80+iWidth/2+iOffset)*sc, yVals + (i*LINE_HEIGHT-7)*sc, z+1, iWidth, sc, sc, fVal, min, max, i, color, 0, snap->count,snap->values);
                        for (j = 0; j < snap->count; j++)
                        {
                            snap->values[j] = (snap->values[j]+1.0f)/2.0f;
                        }
                    }
                    else if (popt->eTypeToggle == kOptionType_MinMaxSlider)
                    {    
                        float min, max;
                        popt->getSliderMinMax(&min, &max);
                        fVal = doSliderAll(xVals+(80+iWidth/2+iOffset)*sc, yVals + (i*LINE_HEIGHT-7)*sc, z+1, iWidth, sc, sc, fVal, min, max, i, color, 0, 0, 0);
                    }
                    else if(popt->eTypeToggle == kOptionType_IntMinMaxSlider)
                    {        
                        float min, max;
                        popt->getSliderMinMax(&min, &max);
                        fVal = doSliderAll(xVals+(80+iWidth/2+iOffset)*sc, yVals + (i*LINE_HEIGHT-7)*sc, z+1, iWidth, sc, sc, fVal, min, max, i, color, 0, 0, 0);
                    }
                    else
                    {                    
                        fVal = doSlider(xVals+(80+iWidth/2+iOffset)*sc, yVals + (i*LINE_HEIGHT-7)*sc, z+1, iWidth, sc, sc, fVal, i, color, 0);
                    }

                    fVal = (fVal+1.0f)/2.0f;
                    if (popt->eTypeToggle != kOptionType_SnapSlider &&
                        popt->eTypeToggle != kOptionType_SnapSliderUltra &&
                        popt->eTypeToggle != kOptionType_SnapMinMaxSlider)
                    {
                        if(pparam->fNormal==pparam->fMax)
                        {
                            if( fVal > .98f )
                                fVal = pparam->fNormal;
                            else
                                fVal = (fVal*(pparam->fMax-pparam->fMin))+pparam->fMin;
                        }
                        else if(pparam->fNormal==pparam->fMin)
                        {
                            if( fVal < .02f )
                                fVal = pparam->fNormal;
                            else
                                fVal = (fVal*(pparam->fMax-pparam->fMin))+pparam->fMin;
                        }
                        else if(fVal>0.78f && fVal<0.82f)
                        {

                            // This makes the slider "snap" to the normal value
                            fVal = pparam->fNormal;
                        }
                        else if(fVal<0.8f)
                        {
                            fVal = (fVal/0.8f)*(pparam->fNormal-pparam->fMin)+pparam->fMin;
                        }
                        else
                        {
                            fVal = ((fVal-0.8f)/0.2f)*(pparam->fMax-pparam->fNormal)+pparam->fNormal;
                        }
                    }
                    else
                    {
                        // After everything else, snap the value to one of the presets
                        int i;
                        int closest = -1;
                         float bestValue = 100000.0;
                        SnapParam *snap = (SnapParam *)popt->pvToggleParam;
                        for (i = 0; i < snap->count; i++)
                        {
                            if( popt->eTypeToggle == kOptionType_SnapMinMaxSlider )
                            {
                                float min, max;
                                popt->getSliderMinMax(&min, &max);
                                
                                if(snap->values[i] < min || snap->values[i] > max)
                                {
                                    continue;
                                }
                            }
                            if (fabs(fVal - snap->values[i]) < bestValue)
                            {
                                bestValue = fabs(fVal - snap->values[i]);
                                closest = i;
                            }
                        }
                        if (closest != -1)
                        {
                            fVal = snap->values[closest];
                        }
                        else
                        {
                            fVal = snap->fNormal;
                        }
                    }
                    if(popt->eTypeToggle==kOptionType_FloatSlider || popt->eTypeToggle==kOptionType_SnapSlider || popt->eTypeToggle == kOptionType_SnapSliderUltra ||
                        popt->eTypeToggle==kOptionType_MinMaxSlider  || popt->eTypeToggle==kOptionType_SnapMinMaxSlider )
                    {
                        if( pUO && pUO->bitSize < 0 )
                            pUO->fVal = fVal;
                        else if(pUO)
                            pUO->iVal = round(fVal); 
                        else
                            *(float *)popt->pvToggle = fVal;
                        sprintf(ach, "%.0f%%%%", fVal*100);
                    }
                    else
                    {
                        if( pUO && pUO->bitSize < 0 )
                            pUO->fVal = fVal;
                        else if(pUO)
                             pUO->iVal = round(fVal); 
                        else
                            *(int *)popt->pvToggle = (int)(fVal + 0.5f);

                        sprintf(ach, "%d", (int)(fVal + 0.5f));
                    }

                    if(fVal==pparam->fNormal)
                    {
                        font_color(CLR_GREEN, CLR_GREEN);
                    }
                    else if(pparam->fMin!=pparam->fNormal && fVal>pparam->fNormal)
                    {
                        font_color(CLR_RED, CLR_RED);
                    }
                    if (visible) {
                        if (custDisplay) {
                            font_color(fontcolor,fontcolor);
                            prnt(xVals, yVals+i*LINE_HEIGHT*sc, z+1, sc, sc, pchTmp);
                        } else {                    
                            prnt(xVals, yVals+i*LINE_HEIGHT*sc, z+1, sc, sc, ach);
                        }
                    }
                }
                break;
            }
        }

        popt++;
    }
    if (starty != y) {
        drawFrame( PIX2, R6, x-37*sc, starty, z, width+35*sc, y+(i*LINE_HEIGHT+ LINE_HEIGHT/2)*sc-starty,sc, frameBorderColor, frameBgColor);
    }


    return i;
}

int countTitlesInOptList(GameOptions *popt)
{
    int title_count = 0;

    while(popt->pvDisplay!=NULL)
    {
        if( kOptionType_Title != popt->eTypeDisplay && kOptionType_TitleUltra != popt->eTypeDisplay )
        {
            popt++;
            continue;
        }

        if(popt->doDisplay && popt->doDisplay(popt) == OPT_HIDE)
        {
            popt++;
            continue;
        }

        popt++;
        title_count++;
    }

    return title_count;
}

void setNewOptions() 
{
    optionSet( kUO_ShowPets, playerIsMasterMind(),0);
    optionSet(kUO_WebHideFriends, 1, 0);

    if (getCurrentLocale() == 3) 
    { //korean defaults
        optionSet(kUO_ShowPlayerBars, kShow_Always, 0);
        optionSet(kUO_ShowArchetype, kShow_Selected | kShow_OnMouseOver, 0);
        optionSet(kUO_ShowOwnerName, kShow_Always, 0);
        optionSetf(kUO_ToolTipDelay,0.3f, 0);
        optionSet(kUO_ClickToMove, 1, 0);
        optionSet(kUO_MouseInvert, 1, 0);
        optionSet(kUO_CamFree,1, 0);
    }

    if(!game_state.safemode)
    {
        optionLoad(0);
        wdwLoad(0);
        loadChatSettings(0);
        bindListLoad(0,0);
    }
}

void optionsSelectTab(void*data)
{
    s_CurTab = (OptionTab)data;
    clearToolTips(s_optListGeneral);
    clearToolTips(s_optListControls);
    clearToolTips(s_optListGraphics);
    clearToolTips(s_optListWindows);
    keymap_clearCommands(NULL);
}

#define TAB_INSET       20 // amount to push tab in from edge of window
#define TAB_HT        20
#define BACK_TAB_OFFSET 3  // offset to kee back tab on the bar
#define OVERLAP         5  // amount to overlap tabs

extern int g_isBindingKey;

static void freeFunc(char *str)
{
    free(str);
}

static int fillRefreshRates(char ***strings, int x, int y, GfxResolution *supported, int supportedCount, int refreshRate)
{
    int i, j, ret = -1;
    char buf[1024];

    if (*strings)
        eaDestroyEx(strings, freeFunc);
    eaCreate(strings);

    for (i = 0; i < supportedCount; i++)
    {
        if (supported[i].x == x && supported[i].y == y)
        {
            for (j = 0; j < eaiSize(&supported[i].refreshRates); j++)
            {
                if (!supported[i].refreshRates[j])
                    continue;
                if (ret < 0)
                    ret = j;
                if (supported[i].refreshRates[j] == refreshRate)
                    ret = j;
                sprintf(buf, "%d", supported[i].refreshRates[j]);
                eaPush(strings, strdup(buf));
            }
            break;
        }
    }

    return ret;
}


// This determines if the "Apply Now" button needs to be animated 
// to let the player know that the "Apply Now" button needs to be
// pressed before all of the changes will be in effect.
static bool needsApply()
{
    GfxSettings applied_gfx;
    OptionShaderDetail new_shaderDetail;
    OptionShaderDetail applied_shaderDetail;

    gfxGetSettings(&applied_gfx);

    // if shadow maps become enabled
    if ((s_gfx.advanced.shadowMode >= SHADOW_SHADOWMAP_LOW) && (applied_gfx.advanced.shadowMode < SHADOW_SHADOWMAP_LOW))
        return true;

    // if shadow maps become disabled but shadow map shaders are still being used
    if ((s_gfx.advanced.shadowMode < SHADOW_SHADOWMAP_LOW) && (game_state.shadowShaderSelection != SHADOWSHADER_NONE))
        return true;

    // if shadow maps are enabled and shadow map shader changed
    if ((s_gfx.advanced.shadowMode >= SHADOW_SHADOWMAP_LOW) &&
        (s_gfx.advanced.shadowMap.shader != applied_gfx.advanced.shadowMap.shader))
        return true;

    // if shadow map mode changed
    if ((s_gfx.advanced.shadowMode >= SHADOW_SHADOWMAP_LOW) && (applied_gfx.advanced.shadowMode >= SHADOW_SHADOWMAP_LOW)
            && s_gfx.advanced.shadowMode != applied_gfx.advanced.shadowMode)
        return true;

    // if the shader detail changed
    new_shaderDetail = s_gfx.advanced.shaderDetail;
    applied_shaderDetail = applied_gfx.advanced.shaderDetail;

    // Compensate for SHADERDETAIL_DUAL_MATERIAL and SHADERDETAIL_HIGHEST_AVAILABLE being the same
    if (new_shaderDetail == SHADERDETAIL_HIGHEST_AVAILABLE)
        new_shaderDetail = shaderDetailHighestAvailable();

    if (applied_shaderDetail == SHADERDETAIL_HIGHEST_AVAILABLE)
        applied_shaderDetail = shaderDetailHighestAvailable();

    if (new_shaderDetail != applied_shaderDetail)
        return true;

    // Mip level changes
    if ((s_gfx.advanced.mipLevel != applied_gfx.advanced.mipLevel) ||
        (s_gfx.advanced.entityMipLevel != applied_gfx.advanced.entityMipLevel))
        return true;

#if NOVODEX
    // Ageia changed
    if (s_gfx.advanced.ageiaOn != applied_gfx.advanced.ageiaOn)
        return true;

    // Physics Quality changed
    if (s_gfx.advanced.physicsQuality != applied_gfx.advanced.physicsQuality)
        return true;
#endif

    if (s_gfx.screenX != applied_gfx.screenX || s_gfx.screenY != applied_gfx.screenY ||
        s_gfx.refreshRate != applied_gfx.refreshRate || s_gfx.fullScreen != applied_gfx.fullScreen)
        return true;

    return false;
}

int optionsWindow()
{
    float x, y, z, wd, ht, sc, oldy;
    int apply_now=0;
    Entity *e = playerPtr();
    static CBox clipbox;
    static int init = false;
    static int initonce = false;
    static int initonce2 = false;

    static int colorPick = 0;
    static int color, bcolor;
    static int frameBorderColor;
    static int frameBgColor;

    static char **s_ppchRes;
    static char **s_ppchRefreshRates = 0;
    static ComboBox s_cbRes = {0};
    static char s_achCurRes[128];
    static char s_achChoice[128];
    static char s_achWindowed[128];
    static int currRefreshRate;

    static GfxResolution desktop;
    static GfxResolution supported[MAX_SUPPORTED_GFXRESOLUTIONS] = {0};
    static char desktopResolution[128];
    static char supportedStrings[MAX_SUPPORTED_GFXRESOLUTIONS][128];
    static int supportedCount;
    static ScrollBar optionsSB = { WDW_OPTIONS, 0 };
    static ScrollBar keyBindsSB = { WDW_OPTIONS, 0 };
    static ScrollBar controlsSB = { WDW_OPTIONS, 0 };
    static ScrollBar graphicsSB = { WDW_OPTIONS, 0 };
    static uiTabControl *tc;
    static float lR = -1.f, lG = -1.f, lB = -1.f, lA = -1.f;
    U32 applyNowButtonColor;

    GfxSettings temp_gfx;
    static int s_lastScreenX = -1;
    static int s_lastScreenY = -1;
    static int s_lastRefresRate = -1;
    static int s_lastFullscreen = -1;

    if(!window_getDims(WDW_OPTIONS, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
    {
        init = false;
        g_isBindingKey = false;
        return 0;
    }

    frameBorderColor = (color & 0xffffff00) | 0x44;
    frameBgColor = (color & 0xffffff00) | 0x22;

    // In the front end, always start (and stay) on the graphics options tab
    if (!isMenu(MENU_GAME))
    {
        s_CurTab = OPTION_GRAPHICS;
        if (tc)
        {
            uiTabControlSelect(tc,(int*)OPTION_GRAPHICS);
        }
    }

    // Only call keymap_initProfileChoice() once we are in the game
    if(!initonce2 && window_getMode(WDW_OPTIONS) == WINDOW_DISPLAYING && isMenu(MENU_GAME))
    {
        initonce2 = true;
        keymap_initProfileChoice();
    }

    if(!initonce && window_getMode(WDW_OPTIONS) == WINDOW_DISPLAYING)
    {
        int i;
        char * cdr;

        initonce = true;

        eaCreate(&s_ppchRes);

        eaPush(&s_ppchRes, s_achCurRes);

        winGetSupportedResolutions( &desktop, supported, &supportedCount );

        cdr = textStd("CurrentDesktopResolution");
        sprintf( desktopResolution, "%d x %d %s", desktop.x, desktop.y, cdr );

        eaPush(&s_ppchRes, desktopResolution);
        for( i = 0 ; i < supportedCount ; i++ )
        {
            sprintf( supportedStrings[i], "%d x %d", supported[i].x, supported[i].y );
            eaPush(&s_ppchRes, supportedStrings[i]);
        }

        // Windowed mode not supported under Cider.
        if (!IsUsingCider())
        {
            strcpy(s_achWindowed, textStd("OptionsWindowed"));
            eaPush(&s_ppchRes, s_achWindowed);
        }

        combobox_init(&s_cbRes, x+ 420, y+ 97, 120, 190, 20, 510, s_ppchRes, WDW_OPTIONS);

        if(gWindowRGBA[0]==0 && gWindowRGBA[1]==0 && gWindowRGBA[2]==0 && gWindowRGBA[3]==0)
        {
            gWindowRGBA[0] = (winDefs[WDW_STAT_BARS].loc.color>>24 & 0xff);
            gWindowRGBA[1] = (winDefs[WDW_STAT_BARS].loc.color>>16 & 0xff);
            gWindowRGBA[2] = (winDefs[WDW_STAT_BARS].loc.color>>8  & 0xff);
            gWindowRGBA[3] = (winDefs[WDW_STAT_BARS].loc.back_color & 0xff);
        }
        tc = uiTabControlCreate(TabType_Undraggable, optionsSelectTab, 0, 0, 0, 0 );
        uiTabControlAdd(tc,"OptionsGeneral",(int*)OPTION_MISC);
        uiTabControlAdd(tc,"OptionsControls",(int*)OPTION_CONTROLS);
        uiTabControlAdd(tc,"Keymapping",(int*)OPTION_BINDS);
        uiTabControlAdd(tc,"OptionsGraphicsAndSound",(int*)OPTION_GRAPHICS);
        uiTabControlAdd(tc,"OptionsWindows",(int*)OPTION_WINDOWS);

        initToolTips(s_optListGeneral);
        initToolTips(s_optListControls);
        initToolTips(s_optListGraphics);
        initToolTips(s_optListWindows);
        uiTabControlSelect(tc,(int*)OPTION_MISC);
    }

    if(!init && window_getMode(WDW_OPTIONS) == WINDOW_DISPLAYING)
    {
        int i = 0;
        init = true;

        gfxGetSettings(&s_gfx);
        s_razerTray = game_state.razerMouseTray;

        InitAntialiasingOption();

        s_lastScreenX = -1;
        s_lastScreenY = -1;
        s_lastRefresRate = -1;
        s_lastFullscreen = -1;

        gWindowRGBA[0] = (winDefs[WDW_STAT_BARS].loc.color>>24 & 0xff);
        gWindowRGBA[1] = (winDefs[WDW_STAT_BARS].loc.color>>16 & 0xff);
        gWindowRGBA[2] = (winDefs[WDW_STAT_BARS].loc.color>>8  & 0xff);
        gWindowRGBA[3] = (winDefs[WDW_STAT_BARS].loc.back_color & 0xff);
        lR = s_fR = gWindowRGBA[0]/255.0f;
        lG = s_fG = gWindowRGBA[1]/255.0f;
        lB = s_fB = gWindowRGBA[2]/255.0f;
        lA = s_fA = gWindowRGBA[3]/255.0f;

        s_fRbub[0] = ((optionGet(kUO_ChatBubbleColor1)>>24) & 0xff)/255.0f;
        s_fGbub[0] = ((optionGet(kUO_ChatBubbleColor1)>>16) & 0xff)/255.0f;
        s_fBbub[0] = ((optionGet(kUO_ChatBubbleColor1)>>8) & 0xff)/255.0f;  
        s_fRbub[1] = ((optionGet(kUO_ChatBubbleColor2)>>24) & 0xff)/255.0f;
        s_fGbub[1] = ((optionGet(kUO_ChatBubbleColor2)>>16) & 0xff)/255.0f;
        s_fBbub[1] = ((optionGet(kUO_ChatBubbleColor2)>>8) & 0xff)/255.0f;  

        s_iFontSize = optionGet(kUO_ChatFontSize);
        s_iLevelAbove = optionGet(kUO_TeamLevelAbove);
        s_iLevelBelow = optionGet(kUO_TeamLevelBelow);

        s_fWindowScale = winDefs[WDW_STAT_BARS].loc.sc;
        s_paramWindowScale.fMax = window_getMaxScale();

        s_renderScale = game_state.renderScaleX;
        if( optionGet(kUO_ChatDisablePetSay) )
            s_petSay = 0;
        else if( !optionGet(kUO_ChatEnablePetTeamSay) )
            s_petSay = 1;
        else
            s_petSay = 2;

        saveInitial(s_optListGeneral);
        saveInitial(s_optListControls);
        saveInitial(s_optListGraphics);
        saveInitial(s_optListWindows);
        keybind_clearChanges();
        keybind_saveUndo();

        // set local variable for managing buff icon options
        status_hideauto = isBuffSetting(kBuff_Status, kBuff_HideAuto);
        status_blink = isBuffSetting(kBuff_Status, kBuff_NoBlink);
        status_nonum = isBuffSetting(kBuff_Status, kBuff_NoNumbers);
        status_nosend = isBuffSetting(kBuff_Status, kBuff_NoSend);
        if( isBuffSetting(kBuff_Status,kBuff_NoStack) )
            status_stack = 1;
        else if( isBuffSetting(kBuff_Status,kBuff_NumStack) )
            status_stack = 2;
        else
            status_stack = 0;

        group_hideauto = isBuffSetting(kBuff_Group, kBuff_HideAuto);
        group_blink = isBuffSetting(kBuff_Group, kBuff_NoBlink);
        group_nonum = isBuffSetting(kBuff_Group, kBuff_NoNumbers);
        group_nosend = isBuffSetting(kBuff_Group, kBuff_NoSend);
        if( isBuffSetting(kBuff_Group,kBuff_NoStack) )
            group_stack = 1;
        else if( isBuffSetting(kBuff_Group,kBuff_NumStack) )
            group_stack = 2;
        else
            group_stack = 0;

        pet_hideauto = isBuffSetting(kBuff_Pet, kBuff_HideAuto);
        pet_blink = isBuffSetting(kBuff_Pet, kBuff_NoBlink);
        pet_nonum = isBuffSetting(kBuff_Pet, kBuff_NoNumbers);
        pet_nosend = isBuffSetting(kBuff_Pet, kBuff_NoSend);
        if( isBuffSetting(kBuff_Pet,kBuff_NoStack) )
            pet_stack = 1;
        else if( isBuffSetting(kBuff_Pet,kBuff_NumStack) )
            pet_stack = 2;
        else
            pet_stack = 0;
    }

    // Get current window state to see if the user manually changed from windowed to fullscreen
    // or changed the window size
    gfxGetWindowSettings(&temp_gfx);

    if (temp_gfx.screenX != s_lastScreenX || temp_gfx.screenY != s_lastScreenY ||
        temp_gfx.refreshRate != s_lastRefresRate || temp_gfx.fullScreen != s_lastFullscreen)
    {
        int choice;

        s_lastRefresRate = temp_gfx.refreshRate;
        s_lastScreenX = temp_gfx.screenX;
        s_lastScreenY = temp_gfx.screenY;
        s_lastFullscreen = temp_gfx.fullScreen;

        // Do this to keep the "Apply Now" button from blinking when the user does a manual change
        s_gfx.refreshRate = temp_gfx.refreshRate;
        s_gfx.screenX = temp_gfx.screenX;
        s_gfx.screenY = temp_gfx.screenY;
        s_gfx.fullScreen = temp_gfx.fullScreen;

        if(temp_gfx.fullScreen)
        {
            sprintf(s_achCurRes, "%d x %d", temp_gfx.screenX, temp_gfx.screenY);
        }
        else
        {
            strcpy(s_achCurRes, textStd("OptionsWindowed"));
        }

        currRefreshRate = temp_gfx.refreshRate;

        strcpy(s_achChoice, s_achCurRes);
        s_cbRes.currChoice = 0;

        comboboxClear(&s_cbRefreshRates);
        choice = fillRefreshRates(&s_ppchRefreshRates, temp_gfx.screenX, temp_gfx.screenY, supported, supportedCount, temp_gfx.refreshRate);
        if (choice >= 0)
            combobox_init(&s_cbRefreshRates, x+ 420, y+ 132, 132, 190, 20, 510, s_ppchRefreshRates, WDW_OPTIONS);
        s_cbRefreshRates.currChoice = choice;
    }
    
    oldy = y;
    drawFrame( PIX3, R10, x, y, z, wd, ht, sc, color, bcolor );

    if (window_getMode(WDW_OPTIONS) == WINDOW_SHRINKING && init)
    {
        gfxApplySettings(&s_gfx, true, false);
        init = false;
    }


    if (window_getMode(WDW_OPTIONS) != WINDOW_DISPLAYING)
        return 0;

    // Only show all of the tabs with the in-game options.
    // Don't show the tabs (always stay on graphics tab) in the front-end
    if (isMenu(MENU_GAME))
        drawTabControl(tc, x+TAB_INSET*sc, y, z, wd - TAB_INSET*2*sc, 15, sc, color, color, TabDirection_Horizontal);

    ht -= TAB_HT*sc;
    y+= TAB_HT*sc;
    BuildCBox(&clipbox, x,y,wd,ht);

    set_scissor(true);
    scissor_dims(x, y, wd-(PIX3)*2*sc, ht-(PIX3+R10)*2*sc);

    // Draw the right thing given the current "tab"
     if (s_CurTab == OPTION_BINDS)
    {
        float dht;

        keymap_drawCBox(30,45+TAB_HT-keyBindsSB.offset,2,sc);
           keymap_drawReset(x+.53*wd,y+55*sc-keyBindsSB.offset,z,sc);
         if( D_MOUSEHIT == drawStdButton(x+.8*wd,y+45*sc-keyBindsSB.offset,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"SaveToFile",sc,0) )
            bindListSave(0,0);
         if( D_MOUSEHIT == drawStdButton(x+.8*wd,y+70*sc-keyBindsSB.offset,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"LoadFromFile",sc,0) )
            bindListLoad(0,0);

        dht = keymap_drawBinds(x+30*sc,y+135*sc-keyBindsSB.offset,z,sc,wd-35*sc);
        font( &title_14 );
        font_color(CLR_YELLOW, CLR_RED);
        prnt( x+25*sc, y+33*sc-keyBindsSB.offset, z+1, sc, sc, "KeyProfile" );
        prnt( x+25*sc, y+110*sc-keyBindsSB.offset, z+1, sc, sc, "Keymapping" );
        drawFrame( PIX2, R6, x+13*sc, y+27*sc-keyBindsSB.offset, z, wd-23*sc,  61*sc, sc, frameBorderColor, frameBgColor ); //keyprifile frame
        drawFrame( PIX2, R6, x+13*sc,  y+104*sc-keyBindsSB.offset, z, wd-23*sc, dht+20*sc, sc, frameBorderColor, frameBgColor ); //keybind frame
        dht += 140*sc;
        set_scissor(false);
        doScrollBar( &keyBindsSB,ht - (PIX3+R10)*2*sc, dht, wd, (PIX3+R10)*sc, z, &clipbox, 0);
    }
    else if(s_CurTab == OPTION_CONTROLS)
    {
        int dht = 0, count;
        count = HandleOptionList(s_optListControls, x+25*sc, y-controlsSB.offset, x+wd-290*sc, y+dht-controlsSB.offset, wd - 33*sc,z+1,sc,&clipbox, color, bcolor);
        dht = (count*LINE_HEIGHT+70)*sc;
        set_scissor(false);
        doScrollBar( &controlsSB, ht - (PIX3+R10)*2*sc, dht, wd, (PIX3+R10)*sc, z, &clipbox, 0 );
        game_state.razerMouseTray = s_razerTray;
    }
    else if(s_CurTab == OPTION_MISC)
    {
        float dht;
        int count;
         count = HandleOptionList(s_optListGeneral, x+25*sc, y-optionsSB.offset, x+wd-290*sc, y-optionsSB.offset, wd - 33*sc,z+1,sc,&clipbox, color, bcolor);
        // Each normal option line is LINE_HEIGHT pixels
        dht = (count*LINE_HEIGHT+countTitlesInOptList(s_optListGeneral)*14)*sc;

        set_scissor(false);

         doScrollBar( &optionsSB, ht - (PIX3+R10)*2*sc, dht, wd, (PIX3+R10)*sc, z, &clipbox, 0 );
    }
    else if(s_CurTab == OPTION_WINDOWS)
    {
        float dht;
        int i, bub_color[BUB_COLOR_TOTAL], count;
         count = HandleOptionList(s_optListWindows, x+25*sc, y-optionsSB.offset, x+wd-290*sc, y-optionsSB.offset, wd - 33*sc,z+1,sc,&clipbox, color, bcolor);
        // Each normal option line is LINE_HEIGHT pixels
        dht = (count*LINE_HEIGHT+countTitlesInOptList(s_optListWindows)*14)*sc;

        dht += 75*sc;
        for( i = 0; i < BUB_COLOR_TOTAL; i++ )
            bub_color[i] = (((int)(s_fRbub[i]*255.f) << 24)|((int)(s_fGbub[i]*255.f) << 16)|((int)(s_fBbub[i]*255.f) << 8)|0xff);
        drawChatBubble( kBubbleType_Chat, kBubbleLocation_Bottom, textStd("BubbleLook"), x+wd/2, y+dht-optionsSB.offset, z, sc, bub_color[BUB_COLOR_TEXT], bub_color[BUB_COLOR_BACK], CLR_BLACK, 0 );
        dht += LINE_HEIGHT*sc;
           
        font( &title_14 );
        font_color(CLR_YELLOW, CLR_RED);
        prnt( x+25*sc, y+dht+5*sc-optionsSB.offset, z+1, sc, sc, "ChatSettings" );
        prnt( x+25*sc, y+dht+83*sc-optionsSB.offset, z+1, sc, sc, "WindowSettings" );
        drawFrame( PIX2, R6, x+13*sc, y+dht*sc-optionsSB.offset, z, wd-23*sc,  61*sc, sc, frameBorderColor, frameBgColor ); 
        drawFrame( PIX2, R6, x+13*sc,  y+dht+77*sc-optionsSB.offset, z, wd-23*sc, 61*sc, sc, frameBorderColor, frameBgColor ); 
        if( D_MOUSEHIT == drawStdButton(x+.33*wd,y+dht+30*sc-optionsSB.offset,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"SaveToFile",sc,0) )
            saveChatSettings(0);
        if( D_MOUSEHIT == drawStdButton(x+.66*wd,y+dht+30*sc-optionsSB.offset,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"LoadFromFile",sc,0) )
            loadChatSettings(0);
        if( D_MOUSEHIT == drawStdButton(x+.33*wd,y+dht+107*sc-optionsSB.offset,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"SaveToFile",sc,0) )
            wdwSave(0);
        if( D_MOUSEHIT == drawStdButton(x+.66*wd,y+dht+107*sc-optionsSB.offset,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"LoadFromFile",sc,0) )
            wdwLoad(0);
        dht += 140*sc;

        set_scissor(false);

         doScrollBar( &optionsSB, ht - (PIX3+R10)*2*sc, dht, wd, (PIX3+R10)*sc, z, &clipbox, 0 );

        if (!nearf(lR,s_fR) || !nearf(lG,s_fG) || !nearf(lB,s_fB) || !nearf(lA,s_fA)){ //only change window colors on change
            gWindowRGBA[0] = MAX(1, (int)(s_fR*255));
            gWindowRGBA[1] = MAX(1, (int)(s_fG*255));
            gWindowRGBA[2] = MAX(1, (int)(s_fB*255));
            gWindowRGBA[3] = MAX(1, (int)(s_fA*255));
            windows_ChangeColor();
            lR = s_fR;
            lG = s_fG;
            lB = s_fB;
            lA = s_fA;
        }
    }
    else if(s_CurTab == OPTION_GRAPHICS)
    {
        const char *pchRes;
        AtlasTex *ptex = white_tex_atlas;
        AtlasTex *backing = atlasLoadTexture( "Options_windowtint_backdrop.tga" );
        float dht;
        int count;

        // resolution combo box
        combobox_setLoc(&s_cbRes,wd/sc-210,TAB_HT+37-graphicsSB.offset,2);
        pchRes = combobox_display(&s_cbRes);

        if(pchRes)
        {
            strcpy(s_achChoice, pchRes);
            if(stricmp(textStd("OptionsWindowed"), pchRes)==0)
            {
                s_gfx.fullScreen = 0;
                windowSize(&s_gfx.screenX, &s_gfx.screenY);
                comboboxClear(&s_cbRefreshRates);
                s_cbRefreshRates.currChoice = -1;
            }
            else if(stricmp(pchRes, s_achCurRes)!=0)
            {
                int choice;
                sscanf(pchRes, "%d x %d", &s_gfx.screenX, &s_gfx.screenY);
                s_gfx.fullScreen = 1;
                choice = fillRefreshRates(&s_ppchRefreshRates, s_gfx.screenX, s_gfx.screenY, supported, supportedCount, s_gfx.refreshRate);
                comboboxClear(&s_cbRefreshRates);
                if (choice >= 0)
                    combobox_init(&s_cbRefreshRates, x+ 420, y+ 132, 120, 190, 20, 510, s_ppchRefreshRates, WDW_OPTIONS);
                s_cbRefreshRates.currChoice = choice;
            }
        }

        // refresh rate combo box
        if (s_gfx.fullScreen && s_cbRefreshRates.currChoice >= 0)
        {
            combobox_setLoc(&s_cbRefreshRates,wd/sc-210,TAB_HT+81-graphicsSB.offset,2);
            pchRes = combobox_display(&s_cbRefreshRates);
        }
        else
            pchRes = 0;
        if(pchRes)
        {
            sscanf(pchRes, "%d", &s_gfx.refreshRate);
        }

        // other stuff
        count = HandleOptionList(s_optListGraphics, x+25*sc, y-graphicsSB.offset, x+wd-290*sc, y-graphicsSB.offset, wd - 33*sc,z+1,sc,&clipbox, color, bcolor);
        sndVolumeControl( SOUND_VOLUME_FX,      s_gfx.fxSoundVolume );
        sndVolumeControl( SOUND_VOLUME_MUSIC,   s_gfx.musicSoundVolume );
        sndVolumeControl( SOUND_VOLUME_VOICEOVER, s_gfx.voSoundVolume );
        g_audio_state.software = s_gfx.forceSoftwareAudio;

        game_state.gamma = s_gfx.gamma;
        if (g_audio_state.surroundFailed)
        {
            g_audio_state.uisurround = s_gfx.enable3DSound = false;
            g_audio_state.surroundFailed = false;
        }
        else
        {
            g_audio_state.uisurround = s_gfx.enable3DSound;
        }

        if (!hwlightIsPresent())
            s_gfx.enableHWLights = 0;
        
        game_state.enableHardwareLights = s_gfx.enableHWLights;

        dht = (count*LINE_HEIGHT + 55)*sc;
        if (s_gfx.showAdvanced)
            dht += LINE_HEIGHT;
        if (s_gfx.advanced.shadowMap.showAdvanced)
            dht += LINE_HEIGHT;
        if (s_gfx.advanced.ambient.showAdvanced)
            dht += LINE_HEIGHT;
        set_scissor(0);

        doScrollBar( &graphicsSB, ht - (PIX3+R10)*2*sc, dht, wd, (PIX3+R10)*sc, z, &clipbox, 0 );
        
        gfxApplySettings(&s_gfx, true, true);
    }
    else
    {
        onlydevassert("invalid or unhandled s_CurTab value" == 0);
    }

    // Animate the "Apply Now" button if something needs to be applied
    applyNowButtonColor = window_GetColor(WDW_OPTIONS);

    if (needsApply())
    {
        // Bounces from black->original color->white->original color->black-> ...
        // Guarantees that at least two colors will be visible.
        Color c;
        F32 scale = sinf(fmodf(global_state.cur_timeGetTime/1000.0f,1.0f) * 2 * PI);

        c.integer = applyNowButtonColor;

        // Modifies the GBA of the Color structure because the colors in the U32 are stored in memory as A B G R,
        // but the Color structure stores them as R G B A
        if (scale < 0.0f)
        {
            c.g += (float)c.g * scale;
            c.b += (float)c.b * scale;
            c.a += (float)c.a * scale;
        }
        else
        {
            c.g += (float)(255 - c.g) * scale;
            c.b += (float)(255 - c.b) * scale;
            c.a += (float)(255 - c.a) * scale;
        }

        applyNowButtonColor = c.integer;
    }

       if( D_MOUSEHIT == drawStdButton(x+60*sc,y+ht-(TAB_HT/2+PIX3)*sc,z,100*sc,TAB_HT*sc,applyNowButtonColor,"ApplyNow",sc,0) )
    {
         gfxApplySettings(&s_gfx, true, false);

        if (isMenu(MENU_GAME))
        {
            int i, tmp=0;

            keymap_clearCommands(NULL);

            s_iFontSize = optionGet(kUO_ChatFontSize);
            s_iLevelAbove = optionGet(kUO_TeamLevelAbove);
            s_iLevelBelow = optionGet(kUO_TeamLevelBelow);

            for(i=0;i<eaSize(&gChatFilters);i++)
                gChatFilters[i]->reformatText = true;

            gWindowRGBA[0] = MAX(1, (int)(s_fR*255));
            gWindowRGBA[1] = MAX(1, (int)(s_fG*255));
            gWindowRGBA[2] = MAX(1, (int)(s_fB*255));
            gWindowRGBA[3] = MAX(1, (int)(s_fA*255));

            windows_ChangeColor();
            if (s_fWindowScale != *(float *)getOldValue(s_optListWindows,&s_fWindowScale))
                window_SetScaleAll( s_fWindowScale );

            ttSetCacheableFontScaling(0.0,0.0); //clear the fontsize cache
            optionSet( kUO_ChatBubbleColor1, (((int)(s_fRbub[0]*255.f) << 24)|((int)(s_fGbub[0]*255.f) << 16)|((int)(s_fBbub[0]*255.f) << 8)|0xff), 0 );        
            optionSet( kUO_ChatBubbleColor2, (((int)(s_fRbub[1]*255.f) << 24)|((int)(s_fGbub[1]*255.f) << 16)|((int)(s_fBbub[1]*255.f) << 8)|0xff), 0 );  

            if (s_petSay == 0)
            {
                optionSet(kUO_ChatDisablePetSay, 1, 0);
                optionSet(kUO_ChatEnablePetTeamSay, 0, 0);
            } else if (s_petSay == 1)
            {
                optionSet(kUO_ChatDisablePetSay, 0, 0);
                optionSet(kUO_ChatEnablePetTeamSay, 0, 0);
            } 
            else 
            {
                optionSet(kUO_ChatDisablePetSay, 0, 0);
                optionSet(kUO_ChatEnablePetTeamSay, 1, 0);    
            }

            if(status_hideauto)
                tmp |= (1<<kBuff_HideAuto);
            if(status_blink)
                tmp |= (1<<kBuff_NoBlink);
            if(status_stack == 1)
                tmp |= (1<<kBuff_NoStack);
            if(status_stack == 2)
                tmp |= (1<<kBuff_NumStack);
            if(status_nonum)
                tmp |= (1<<kBuff_NoNumbers);
            if(status_nosend)
                tmp |= (1<<kBuff_NoSend);

            if(group_hideauto)
                tmp |= (1<<(kBuff_HideAuto+8));
            if(group_blink)
                tmp |= (1<<(kBuff_NoBlink+8));
            if(group_stack == 1 )
                tmp |= (1<<(kBuff_NoStack+8));
            if(group_stack == 2 )
                tmp |= (1<<(kBuff_NumStack+8));
            if(group_nonum)
                tmp |= (1<<(kBuff_NoNumbers+8));
            if(group_nosend)
                tmp |= (1<<(kBuff_NoSend+8));

            if(pet_hideauto)
                tmp |= (1<<(kBuff_HideAuto+16));
            if(pet_blink)
                tmp |= (1<<(kBuff_NoBlink+16));
            if(pet_stack == 1 )
                tmp |= (1<<(kBuff_NoStack+16));
            if(pet_stack == 2 )
                tmp |= (1<<(kBuff_NumStack+16));
            if(pet_nonum)
                tmp |= (1<<(kBuff_NoNumbers+16));
            if(pet_nosend)
                tmp |= (1<<(kBuff_NoSend+16));

            optionSet(kUO_BuffSettings, tmp, 0);
            sendOptions(0);

            game_state.options_have_been_saved = 1;
            keybind_saveChanges();

            collisions_off_for_rest_of_frame = true;
        }
    }
     else if (D_MOUSEHIT == drawStdButton(x+wd-52*sc,y+ht-(TAB_HT/2+PIX3)*sc,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"OptionsCancel",sc,0))
    {
        int i;
        loadInitial(s_optListGeneral);
        loadInitial(s_optListControls);
        loadInitial(s_optListGraphics);
        loadInitial(s_optListWindows);
        if (s_renderScale>=0.99) {
            s_gfx.useRenderScale = RENDERSCALE_OFF;
            s_gfx.renderScaleX = s_gfx.renderScaleY = 1;
        } else {
            s_gfx.useRenderScale = RENDERSCALE_SCALE;
            s_gfx.renderScaleX = s_gfx.renderScaleY = s_renderScale;
        }
        game_state.gamma = s_paramGamma.fNormal;

        gfxApplySettings(&s_gfx, true, false);

        if (isMenu(MENU_GAME))
        {
            gWindowRGBA[0] = MAX(1, (int)(s_fR*255));
            gWindowRGBA[1] = MAX(1, (int)(s_fG*255));
            gWindowRGBA[2] = MAX(1, (int)(s_fB*255));
            gWindowRGBA[3] = MAX(1, (int)(s_fA*255));

            windows_ChangeColor();

            keymap_clearCommands(NULL);

            optionSet(kUO_ChatFontSize,s_iFontSize, 0);
            optionSet(kUO_TeamLevelAbove,s_iLevelAbove, 0);
            optionSet(kUO_TeamLevelBelow,s_iLevelBelow, 0);

            for(i=0;i<eaSize(&gChatFilters);i++)
                gChatFilters[i]->reformatText = true;

            if (s_fWindowScale != *(float *)getOldValue(s_optListWindows,&s_fWindowScale))
                window_SetScaleAll( s_fWindowScale );
            ttSetCacheableFontScaling(0.0,0.0); //clear the fontsize cache    

            optionSet( kUO_ChatBubbleColor1, (((int)(s_fRbub[0]*255.f) << 24)|((int)(s_fGbub[0]*255.f) << 16)|((int)(s_fBbub[0]*255.f) << 8)|0xff), 0 );        
            optionSet( kUO_ChatBubbleColor2, (((int)(s_fRbub[1]*255.f) << 24)|((int)(s_fGbub[1]*255.f) << 16)|((int)(s_fBbub[1]*255.f) << 8)|0xff), 0 );   

            if (s_petSay == 0) 
            {
                optionSet(kUO_ChatDisablePetSay, 1, 0);
                optionSet(kUO_ChatEnablePetTeamSay, 0, 0);
            } 
            else if (s_petSay == 1)
            {
                optionSet(kUO_ChatDisablePetSay, 0, 0);
                optionSet(kUO_ChatEnablePetTeamSay, 0, 0);
            } 
            else 
            {
                optionSet(kUO_ChatDisablePetSay, 0, 0);
                optionSet(kUO_ChatEnablePetTeamSay, 1, 0);    
            }
            sendOptions(0);

            game_state.options_have_been_saved = 1;
            keybind_undoChanges();
        }

        init = false;
            
        window_setMode(WDW_OPTIONS,WINDOW_SHRINKING);
    } 

    if (s_CurTab == OPTION_GRAPHICS)
    {
        if( D_MOUSEHIT == drawStdButton(x+.667*wd,y+ht-(TAB_HT/2+PIX3)*sc,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"OptionsSetToDefaults",sc,0))
        {
            int i;
            gfxGetInitialSettings(&s_gfx);
            if(s_gfx.fullScreen)
            {
                sprintf(s_achChoice, "%d x %d", s_gfx.screenX, s_gfx.screenY);
            }
            else
            {
                strcpy(s_achChoice, textStd("OptionsWindowed"));
            }
            for (i = 0; i < eaSize(&s_cbRes.strings); i++)
            {
                if (stricmp(s_achChoice,s_cbRes.strings[i]) == 0)
                {
                    s_cbRes.currChoice = i;
                    s_cbRes.changed = 1;
                }
            }
            if(stricmp(textStd("OptionsWindowed"), s_achChoice)==0)
            {
                s_gfx.fullScreen = 0;
                windowSize(&s_gfx.screenX, &s_gfx.screenY);
                comboboxClear(&s_cbRefreshRates);
                s_cbRefreshRates.currChoice = -1;
            }
            else if(stricmp(s_achChoice, s_achCurRes)!=0)
            {
                int choice;
                sscanf(s_achChoice, "%d x %d", &s_gfx.screenX, &s_gfx.screenY);
                s_gfx.fullScreen = 1;
                choice = fillRefreshRates(&s_ppchRefreshRates, s_gfx.screenX, s_gfx.screenY, supported, supportedCount, s_gfx.refreshRate);
                comboboxClear(&s_cbRefreshRates);
                if (choice >= 0)
                    combobox_init(&s_cbRefreshRates, x+ 420, y+ 132, 120, 190, 20, 510, s_ppchRefreshRates, WDW_OPTIONS);
                s_cbRefreshRates.currChoice = choice;
            }

            game_state.gamma = s_paramGamma.fNormal;
            s_renderScale = s_gfx.renderScaleX+0.0005;
            s_fWindowScale = s_paramWindowScale.fNormal;
            s_fRbub[BUB_COLOR_TEXT] = s_paramBlack.fNormal;
            s_fGbub[BUB_COLOR_TEXT] = s_paramBlack.fNormal;
            s_fBbub[BUB_COLOR_TEXT] = s_paramBlack.fNormal;

            s_fRbub[BUB_COLOR_BACK] = s_paramWhite.fNormal;
            s_fGbub[BUB_COLOR_BACK] = s_paramWhite.fNormal;
            s_fBbub[BUB_COLOR_BACK] = s_paramWhite.fNormal;
        }
    }
    else
    {
           if( D_MOUSEHIT == drawStdButton(x+.38*wd,y+ht-(TAB_HT/2+PIX3)*sc,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"OptionsLoad",sc,0))
            optionLoad(0);
        if( D_MOUSEHIT == drawStdButton(x+.63*wd,y+ht-(TAB_HT/2+PIX3)*sc,z,100*sc,TAB_HT*sc,window_GetColor(WDW_OPTIONS),"OptionsSave",sc,0))
            optionSave(0);
    }

    return 0;
}

/* End of File */

