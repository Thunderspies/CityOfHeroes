
#include "UI/uiGame.h"            // for start_menu
#include "UI/uiUtil.h"
#include "UI/uiFx.h"
#include "UI/uiInput.h"
#include "UI/uiGender.h"
#include "uiSlider.h"
#include "UI/uiAvatar.h"
#include "UI/uiUtilMenu.h"
#include "uiPCCCreationNLM.h"
#include "UI/uiUtilGame.h"
#include "uiPowers.h"

#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "graphics/textureatlas.h"
#include "render/tex.h"

#include "graphics/font.h"
#include "graphics/ttFont.h"
#include "graphics/ttFontUtil.h"

#include "cmdparse/cmdgame.h"    // for editnpc
#include "cmdparse/cmdcommon.h"  // for timestep
#include "entity/character_base.h"

#include "graphics/FX/fx.h"
#include "player/player.h"
#include "entity/entity.h"
#include "win/win_init.h"     // for windowClientSize
#include "gameComm/initClient.h"
#include "entity/costume_client.h"   // for setGender
#include "sound/sound.h"
#include <utilitieslib/utils/textparser.h>

// for temporary default body type selections (development only)
#include "UI/uiDialog.h"    
#include "UI/uiComboBox.h"
#include <utilitieslib/utils/utils.h>
#include "UI/uiCostume.h"
#include "UI/uiTailor.h"

#include "win/input.h"
#include "seq/seqstate.h"
#include "gameData/costume_critter.h"

#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/file.h>
#include "entity/PCC_Critter.h"
#include "UI/uiNet.h"
#include "entity/entPlayer.h"
#include "UI/uiSupercostume.h"
#include "auth/authUserData.h"
#include "entity/LoadDefCommon.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>

extern int gLoadRandomPresetCostume;
extern int pccCritterRank;

#ifdef TEST_CLIENT
Costume *pccFixupCostume = NULL;
int pccCritterFirstGenderPass = 0;
#else
extern Costume *pccFixupCostume;
extern int pccCritterFirstGenderPass;
#endif

static Costume *prevCostume = NULL;
static int prevGender = 0;
static int prevBodyType = 0;
static int gender_inCostume = FALSE;

static GenderChangeMode genderEditingMode = 0;




typedef struct BodyConfigList{

    BodyConfig ** configs;

}BodyConfigList;

static BodyConfigList gBodyConfigList;

BodyConfig sBodyConfig;
static BodyConfig sDestBodyConfig;

TokenizerParseInfo parseBodyConfigScales[] = {
    { "Global",            TOK_F32(BodyConfigScales,global,0)    },
    { "Bone",            TOK_F32(BodyConfigScales,bone,0)        },
    { "Head",            TOK_F32(BodyConfigScales,head,0)        },
    { "Chest",            TOK_F32(BodyConfigScales,chest,0)    },
    { "Shoulder",        TOK_F32(BodyConfigScales,shoulder,0)    },
    { "Waist",            TOK_F32(BodyConfigScales,waist,0)    },
    { "Hip",            TOK_F32(BodyConfigScales,hip,0)        },
    { "Leg",            TOK_F32(BodyConfigScales,leg,0)        },
    { "End",            TOK_END,        0},
    { "", 0, 0 }
};


TokenizerParseInfo parseBodyConfig[] = {
//    { "",                TOK_STRING | TOK_STRUCTPARAM,        offsetof(BodyConfig,)},
    { "DisplayName",    TOK_STRING(BodyConfig,displayName,0)    },
    { "BodyType",        TOK_STRING(BodyConfig,bodyType,0)        },
    { "Build",            TOK_STRING(BodyConfig,bodyBuild,0)        },
    { "Scales",            TOK_EMBEDDEDSTRUCT(BodyConfig,scales,parseBodyConfigScales)   },
    { "End",            TOK_END,        0},
    { "", 0, 0 }
};

TokenizerParseInfo parseBodyConfigList[] = {
    { "BodyConfig",        TOK_STRUCT(BodyConfigList,configs,parseBodyConfig) },
    { "", 0, 0 }
};


//---------------------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------------------

char * genderBuildKey[] = {"slim","average","athletic","heavy"};

float fSpeeds[MAX_BODY_SCALES] = {0};

int gCurrentGender = AVATAR_GS_MALE; // default
extern int gCurrentBodyType;
int gCurrentBuild = BUILD_ATHLETIC;

int gOffsetX, gOffsetY; // position of spin sliders

void gender_reset()
{
    gCurrentGender = AVATAR_GS_MALE;
}

#define GENDER_MENU_WD    910 // width of the police line up area
#define GENDER_MENU_X    57  // x coordinate
#define FOOT_OFFSET        66  // width of a foot in pixels
#define CM_OFFSET        2.17
#define LINE_OFFSET        10  // offset from bottom of frame to start the lines
#define GENDER_SPACE    282 // space between gender selection buttons
//
//
static void drawGenderBackground(float screenScaleX, float screenScaleY)
{
       AtlasTex * lineL    = atlasLoadTexture( "bodytype_heightline_end_L.tga" );
       AtlasTex * line    = atlasLoadTexture( "bodytype_heightline_mid.tga" );
    AtlasTex * lineR    = atlasLoadTexture( "bodytype_heightline_end_R.tga" );
    float wd = (GENDER_MENU_WD - LINE_OFFSET*2 - 600) * screenScaleX;
    float x = (600 + GENDER_MENU_X + LINE_OFFSET) * screenScaleX;
    float w = lineL->width;
     int i;

    font( &game_9 );
      font_color( CLR_WHITE, CLR_WHITE );

    // draw 8ft of lines
       if( !getCurrentLocale() )
     {
           for( i = 0; i < 9; i++ )
        {

               float y = (620 - (i*FOOT_OFFSET))* screenScaleY;
     
               display_sprite( lineL, x,            y, 21, 1.f, 1.f, CLR_WHITE );
            display_sprite( line,  x + w,        y, 21, (wd - w*2)/line->width, 1.f, CLR_WHITE );
            display_sprite( lineR, x + wd - w,    y, 21, 1.f, 1.f, CLR_WHITE );

              cprntEx( x,                y + 2, 22, 1.f, 1.f, CENTER_Y, "%i'", i );
            cprntEx( x + wd - 10,    y + 2, 22, 1.f, 1.f, CENTER_Y, "%i'", i );
        }
    }
    else
    {
          for( i = 0; i <= 240; i+=20 )
        {
            float y = (620 - (i*CM_OFFSET))*screenScaleY;

             display_sprite( lineL, x,            y, 21, 1.f, 1.f, CLR_WHITE );
            display_sprite( line,  x + w,        y, 21, (wd - w*2)/line->width, 1.f, CLR_WHITE );
            display_sprite( lineR, x + wd - w,    y, 21, 1.f, 1.f, CLR_WHITE );

              cprntEx( x - 20,                y + 2, 22, 1.f, 1.f, CENTER_Y, "%i cm", i );
            cprntEx( x + wd - 10,    y + 2, 22, 1.f, 1.f, CENTER_Y, "%i cm", i );
        }    
    }
}

int green_blobby_fx_id=0; // fx variables
static FxParams fxp;
// turns on green blobby
//
static void greenBlobbyFx()
{

    Entity *e = playerPtrForShell(1);

    if (green_blobby_fx_id) {
        fxDelete(green_blobby_fx_id, HARD_KILL);
    }

    // turn on fx for the green blobby
    fxInitFxParams(&fxp);
    fxp.keys[0].seq = e->seq;
    fxp.keycount++;
    fxp.fxtype = FX_ENTITYFX;
    green_blobby_fx_id = fxCreate("ENEMYFX/greenblobby/greenblobby.fx", &fxp);
}

#define GS_Y        603 // the y coordinate of the gender selection buttons
#define GS_WD        130 // the width of a normal button
#define ZOOM_SPEED  20  // the speed the bbobby zooms in from nowhere

void SelectConfig(int idx, int prevIdx);
//
//
static void genderSelect(float screenScaleX, float screenScaleY)
{
       Entity *e = playerPtr();

       float x = 50*screenScaleX;
       float y = 100*screenScaleY;
       float space = 180*screenScaleX;
    float wd = 550*screenScaleX;
    float scale = MIN(screenScaleX, screenScaleY);
    int id;

    // Draw a large paddle and bounding frame
    drawMenuBar( x + wd/2, y, 20, scale, wd/scale, "Type", 0, 1, 0, 0, 1, SELECT_FROM_UISKIN( CLR_FRAME_BLUE, CLR_FRAME_RED, CLR_WHITE ) );
    drawFrame( PIX2, R6, x, y, 10, wd, 100*screenScaleY, 1.f, 0xffffff88, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

      x += 40*screenScaleX;
    y += 60*screenScaleY;

    // the female check bar
       if( D_MOUSEHIT == drawCheckBar( x, y, 20, scale, GS_WD*screenScaleX, "Female", gCurrentGender == AVATAR_GS_FEMALE, 1, 0, 0, NULL )
          && gCurrentGender != AVATAR_GS_FEMALE )
     {
        electric_clearAll();
         electric_init( x/screenScaleX, y/screenScaleY, 1000, MIN(1.f, scale), 0, 30, 1, 0 );
          sndPlay( "powerselect2", SOUND_GAME );

        // clear costume
        costume_init( costume_as_mutable_cast(e->costume) );
        clearCostumeStack();

        // change the model
        setGender( e, "fem" );
        greenBlobbyFx();
        gender_inCostume = false;

        // set current gender state
        gCurrentGender = AVATAR_GS_FEMALE;

        id = buildMatchConfig();
        if( id >=0 )
            SelectConfig( id, -1 );

        gLoadRandomPresetCostume = 1;

    }

    x += space;

    // Male
    if( D_MOUSEHIT == drawCheckBar( x, y, 20, scale, GS_WD*screenScaleX, "Male", gCurrentGender == AVATAR_GS_MALE, 1, 0, 0, NULL )
         && gCurrentGender != AVATAR_GS_MALE )
    {
        electric_clearAll();
        electric_init(  x/screenScaleX, y/screenScaleY, 1000, MIN(1.f, scale), 0, 30, 1, 0 );
        sndPlay( "powerselect2", SOUND_GAME );
        costume_init( costume_as_mutable_cast(e->costume) );
        clearCostumeStack();
        setGender( e, "male" );
        greenBlobbyFx();
        gender_inCostume = false;

        gCurrentGender = AVATAR_GS_MALE;

        id = buildMatchConfig();
        if( id >=0 )
            SelectConfig( id, -1 );

        gLoadRandomPresetCostume = 1;
    }

    x += space;

    // Huge
    if( D_MOUSEHIT == drawCheckBar( x, y, 20, scale, GS_WD*screenScaleX, "Huge", gCurrentGender == AVATAR_GS_HUGE, 1, 0, 0, NULL )
          && gCurrentGender != AVATAR_GS_HUGE )
    {
         electric_clearAll();
           electric_init(x/screenScaleX, y/screenScaleY, 1000, MIN(1.f, scale), 0, 30, 1, 0 );
         sndPlay( "powerselect2", SOUND_GAME );
        costume_init( costume_as_mutable_cast(e->costume) );
        clearCostumeStack();
        setGender( e, "huge" );
        greenBlobbyFx();
        gender_inCostume = false;

        gCurrentGender = AVATAR_GS_HUGE;

        id = buildMatchConfig();
        if( id >=0 )
            SelectConfig( id, -1 );

        gLoadRandomPresetCostume = 1;
    }

       if( game_state.editnpc ) // if the edit npc flag is set
    {
        int npcOffset = 100;
        x -= space*2;
        // female npc
         if( D_MOUSEHIT == drawCheckBar( x, y+npcOffset, 20, scale, 120*screenScaleX, "Female NPC", gCurrentGender == AVATAR_GS_BF, 1, 0, 0, NULL )
               && gCurrentGender != AVATAR_GS_BF )
        {
            costume_init( costume_as_mutable_cast(e->costume) );
            setGender( e, "bf" );
            costume_Apply( e );
    
            gCurrentGender = AVATAR_GS_BF;
        }

        x += space;
        // male npc
        if( D_MOUSEHIT == drawCheckBar( x, y+npcOffset, 20, scale, 150*screenScaleX, "Male NPC", gCurrentGender == AVATAR_GS_BM, 1, 0, 0, NULL )
             && gCurrentGender != AVATAR_GS_BM )
        {
            costume_init( costume_as_mutable_cast(e->costume) );
            setGender( e, "bm" );
            costume_Apply( e );
            gCurrentGender = AVATAR_GS_BM;
        }
        x+= space;

        // villain
        if( D_MOUSEHIT == drawCheckBar( x, y+npcOffset-60, 20, scale, GS_WD*screenScaleX, "Enemy", gCurrentGender == AVATAR_GS_ENEMY, 1, 0, 0, NULL )
             && gCurrentGender != AVATAR_GS_ENEMY )
        {
            costume_init( costume_as_mutable_cast(e->costume) );
            setGender(e, "enemy" ); // sets bodytype
            costume_Apply( e );
            gCurrentGender = AVATAR_GS_ENEMY;
        }


        if( D_MOUSEHIT == drawCheckBar( x, y+npcOffset-20, 20, scale, GS_WD*screenScaleX, "Enemy2", gCurrentGender == AVATAR_GS_ENEMY2, 1, 0, 0, NULL )
             && gCurrentGender != AVATAR_GS_ENEMY2 )
        {
            costume_init( costume_as_mutable_cast(e->costume) );
            setGender(e, "enemy2" ); // sets bodytype
            costume_Apply( e );
            gCurrentGender = AVATAR_GS_ENEMY2;
        }

        if( D_MOUSEHIT == drawCheckBar( x, y+npcOffset+20, 40, scale, GS_WD*screenScaleX, "Enemy3", gCurrentGender == AVATAR_GS_ENEMY3, 1, 0, 0, NULL )
               && gCurrentGender != AVATAR_GS_ENEMY3 )
        {
            costume_init( costume_as_mutable_cast(e->costume) );
            setGender(e, "enemy3" ); // sets bodytype
            costume_Apply( e );
            gCurrentGender = AVATAR_GS_ENEMY3;
        }
    }
}



int buildMatchConfig(void)
{
    int i;
    for( i = 0; i < eaSize(&gBodyConfigList.configs); i++)
    {
        if( stricmp( gBodyConfigList.configs[i]->bodyBuild, genderBuildKey[gCurrentBuild] ) == 0 )
        {
            if(( AVATAR_GS_FEMALE == gCurrentGender && stricmp( gBodyConfigList.configs[i]->bodyType, "Female" ) == 0 ) ||
               ( AVATAR_GS_MALE == gCurrentGender && stricmp( gBodyConfigList.configs[i]->bodyType, "Male" ) == 0 ) ||
               ( AVATAR_GS_HUGE == gCurrentGender && stricmp( gBodyConfigList.configs[i]->bodyType, "Huge" ) == 0 ) )
            {
                return i;
            }
        }
    }

    return -1;
}



static void genderBuildSelect( float x, float y, float screenScaleX, float screenScaleY)
{
    int i;
    float scale = MIN(screenScaleX, screenScaleY);
     for( i = 0; i < BUILD_NUM; i++ )
    {
        if( D_MOUSEHIT == drawCheckBar( x, y + (i*45*screenScaleY), 20, scale, GS_WD *screenScaleX, genderBuildKey[i], gCurrentBuild == i, 1, 0, 0, NULL )
            && gCurrentBuild != i )
        {
            int id;
            electric_clearAll();
            electric_init( x/screenScaleX, (y/screenScaleY) + i*45, 1000, MIN(1.f, scale), 0, 30, 1, 0 );
            sndPlay( "powerselect2", SOUND_GAME );

            gCurrentBuild = i;

            id = buildMatchConfig();

            if( id >=0 )
                SelectConfig( id, -1 );
        }
    }
}

// scale slider
//
void genderScale(float x, float y, float screenScaleX, float screenScaleY)
{
    F32 * pScale = &sBodyConfig.scales.global;
    float actual = 0;
       float wd = 300*screenScaleX;
    float screenScale = MIN(screenScaleX, screenScaleY);
    Entity *e = playerPtr();

     font( &game_12 );
    font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0xaaaaffff, 0xaaaaaaff, CLR_WHITE ) );
       prnt( x - wd/2 - str_wd(font_grp, screenScale, screenScale, "short"), y, 60, screenScale, screenScale, "Short" );
     prnt( x + wd/2, y, 60, screenScale, screenScale, "Tall" );

    if(game_state.editnpc)
        prnt(740*screenScaleX, (663*screenScaleY) + y, 60, screenScale, screenScale, "%.2f", *pScale);

    wd -= (20*screenScaleX);
     *pScale = doSlider( x, y-(5*screenScaleY), 5000, wd/screenScale, screenScale, screenScale, *pScale, SLIDER_SCALE, uiColors.standard.scrollbar, 0 );

    // pre-built offsets for each charchter type 
       if( gCurrentGender == AVATAR_GS_FEMALE ) // she is scaled up to male size, but is allowed to be shorter
    {
        if( (*pScale) > 0 )
            actual = 36*(*pScale);
        else
            actual = 27*(*pScale);
    }
      else if( gCurrentGender == AVATAR_GS_MALE )
    {
        if ( (*pScale) > 0 )
            actual = 25*(*pScale);
        else
            actual = 33*(*pScale);
    }
     else if( gCurrentGender == AVATAR_GS_HUGE ) // he is scaled to be initially taller, he can be taller overall, but not as short
    {
        if ( (*pScale) > 0 )
            actual = 25*(*pScale);
        else
            actual = 33*(*pScale);
    }
    else
        actual = ((*pScale)*25);

    if( gCostumeCritter )
        actual = 100*(*pScale);

    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fScale = actual;
    }
    scaleHero( e, actual );
}
 

static F32 previewNetworkScale(F32 f)
{
    return f; // enable this to have the UI show an arbitrary precision, I have problems noticing it with even 4 bits
//    int precision = 5;
//    F32 min=-1, max=1;
//    int val = (f - min)/(max - min) * ((1 << precision) - 1);
//    return val * (max - min) / ((1<<precision)-1) + min;
}




//---------------------------------------------------------------------------------------------------------
// Temporary Stuff for loading/saving body type (slider) configs
//---------------------------------------------------------------------------------------------------------

static ComboBox comboDefaults = {0};
ComboBox comboAnims = {0};
static bool converge = false;

char BODY_CONFIG_FILENAME[] = "menu/Costume/defaultBodyCfg.txt";

enum {DONT_SAVE = -1};
#define BONE_SPEED  .1f

void convergeConfigs()
{
    int i;
    bool changed = false;

    if( !converge )
        return;

       for( i = 1; i < MAX_BODY_SCALES; i++ )
    {
        if( (sBodyConfig.fScales[i] - sDestBodyConfig.fScales[i]) > .001f )
        {
            changed = true;
            sBodyConfig.fScales[i] -= fSpeeds[i] *TIMESTEP;
            if( sBodyConfig.fScales[i] < sDestBodyConfig.fScales[i] )
                sBodyConfig.fScales[i] = sDestBodyConfig.fScales[i];
        }

        if( (sDestBodyConfig.fScales[i] - sBodyConfig.fScales[i]) > .001f  )
        {
            changed = true;
            sBodyConfig.fScales[i] += fSpeeds[i] *TIMESTEP;
            if( sBodyConfig.fScales[i] > sDestBodyConfig.fScales[i] )
                sBodyConfig.fScales[i] = sDestBodyConfig.fScales[i];
        }
    }

    if( !changed )
    {
        converge = false;
        sndStop(SOUND_UI_ALTERNATE, 0.f);
    }
    else
    {
        sndPlay("N_Slider_loop", SOUND_UI_ALTERNATE);
    }

}

void setConfigs()
{
    int i;
    for (i = 1; i < MAX_BODY_SCALES; ++i)
    {
        sBodyConfig.fScales[i] = sDestBodyConfig.fScales[i];
    }
}

void SelectConfig(int idx, int prevIdx)
{
    if(prevIdx != DONT_SAVE)
    {
        assert(prevIdx >= 0 && prevIdx < eaSize(&gBodyConfigList.configs));
        memcpy(&gBodyConfigList.configs[ prevIdx ]->scales, &sBodyConfig.scales, sizeof(BodyConfigScales));
    }

    if(idx != prevIdx)
    {
        Entity * e = playerPtr();
        int i;

        assert(idx >= 0 && idx < eaSize(&gBodyConfigList.configs));
        memcpy(&sDestBodyConfig.scales, &gBodyConfigList.configs[ idx ]->scales, sizeof(BodyConfigScales));
        converge = true;

        for( i = 1; i < MAX_BODY_SCALES; i++ )
        {
            fSpeeds[i] = ABS(sDestBodyConfig.fScales[i]-sBodyConfig.fScales[i])*BONE_SPEED/2;
        }

        // set gender
         if(e)
        {
            if( gCurrentGender != AVATAR_GS_FEMALE && !stricmp(gBodyConfigList.configs[ idx ]->bodyType, "Female"))
            {
                gCurrentGender = AVATAR_GS_FEMALE;
                setGender(e, "fem");
            }
            else if( gCurrentGender != AVATAR_GS_HUGE && !stricmp(gBodyConfigList.configs[ idx ]->bodyType, "Huge"))
            {
                gCurrentGender = AVATAR_GS_HUGE;
                setGender(e, "huge");
            }
            else if ( gCurrentGender != AVATAR_GS_MALE && !stricmp(gBodyConfigList.configs[ idx ]->bodyType, "Male") )
            {
                gCurrentGender = AVATAR_GS_MALE;
                setGender(e, "male");
            }
        }
    }
}

void ResetCurrentConfig()
{
    memset(&sBodyConfig.scales, 0, sizeof(BodyConfigScales));
}

// These are total hack jobs to get default body types working ASAP
void SaveConfigs(int type)
{      
    SelectConfig( type, type);    // just save it

    if(!ParserWriteTextFile(BODY_CONFIG_FILENAME, parseBodyConfigList, &gBodyConfigList, 0, 0))
    {
        char buf[1000];
        sprintf(buf, "Error writing to file %s. Have you checked it out?", BODY_CONFIG_FILENAME);
        dialogStd(DIALOG_OK, buf, NULL, NULL, NULL, NULL, 0);
        return;
    }
}


void LoadConfigs()
{
    int                i, size, prev;
    static char**    strings = NULL;

    if(gBodyConfigList.configs)
    {
        ParserDestroyStruct(parseBodyConfigList, &gBodyConfigList);
    }

    // copy old settings
     if(!ParserLoadFiles(NULL, BODY_CONFIG_FILENAME, "defaultBodyCfg.bin", 0, parseBodyConfigList, &gBodyConfigList, 0, NULL, NULL))
    {
        dialogStd(DIALOG_OK, "Could not find config file", NULL, NULL, NULL, NULL, 0);
        return;
    }

    SelectConfig(comboDefaults.currChoice, DONT_SAVE);

    // update combo box (strings stick around as static variable)

    size = eaSize(&gBodyConfigList.configs);

     if(strings)
    {
        eaDestroyEx(&strings, 0);
    }

    for(i=0;i<size;i++)
        eaPush(&strings, strdup(gBodyConfigList.configs[i]->displayName));

    prev = comboDefaults.currChoice;

    comboboxClear( &comboDefaults);
    combobox_init( &comboDefaults, 25,  560, 200, 150, 23, (size + 1)*18, strings, 0 );
    comboDefaults.currChoice = min(prev, (size-1));
}


// re-scale orginal scale to value relative to overall bone scale and within a given range (specified in ent_type file)
F32 adjustScale(F32 orig, F32 range)
{
    F32 r = MINMAX(range, 0, 2);
    //F32 f = ((orig + 1) / 2) + ((sBodyConfig.scales.bone - 1)/2);
    
    //  OFFSET INTO RANGE                 RANGE START
    F32 offset = ((orig + 1) * (range / 2));
    F32 start = (((sBodyConfig.scales.bone + 1) * (2 - r))/2) - 1;
 
    F32 f = start + offset;

    F32 res = MINMAX(f, -1, 1);

    return res;
}

F32 reverseAdjustScale( F32 appearance, F32 range, F32 boneScale )
{
    F32 r = MINMAX(range, 0, 2);

    return ((appearance+ 1 - (((boneScale+1)*(2-r))/2))/(r/2))-1;

}

static void genderSetScalesFromSpecifiedAppearance(Entity *e, const Appearance *appearance )
{
    Costume *costume = costume_current(e);
        if( gCurrentGender == AVATAR_GS_FEMALE ) // she is scaled up to male size, but is allowed to be shorter
        {
            if( appearance->fScale > 0 )
                sBodyConfig.scales.global = appearance->fScale/36.0f;
            else
                sBodyConfig.scales.global = appearance->fScale/27.0f;
        }
        else if( gCurrentGender == AVATAR_GS_MALE )
        {
            if ( appearance->fScale > 0 )
                sBodyConfig.scales.global = appearance->fScale/25.0f;
            else
                sBodyConfig.scales.global = appearance->fScale/33.0f;
        }
        else if( gCurrentGender == AVATAR_GS_HUGE ) // he is scaled to be initially taller, he can be taller overall, but not as short
        {
            if ( appearance->fScale > 0 )
                sBodyConfig.scales.global = appearance->fScale/25.0f;
            else
                sBodyConfig.scales.global = appearance->fScale/33.0f;
        }
        else
            sBodyConfig.scales.global = (appearance->fScale/25.0f);

    sBodyConfig.scales.bone        = appearance->fBoneScale;
    sBodyConfig.scales.shoulder = reverseAdjustScale( appearance->fShoulderScale,e->seq->type->shoulderScaleRange, appearance->fBoneScale ); 
    sBodyConfig.scales.chest    = reverseAdjustScale( appearance->fChestScale,e->seq->type->chestScaleRange, appearance->fBoneScale ); 
    sBodyConfig.scales.head        = reverseAdjustScale( appearance->fHeadScale,e->seq->type->headScaleRange, appearance->fBoneScale ); 
    sBodyConfig.scales.hip        = reverseAdjustScale( appearance->fHipScale,e->seq->type->hipScaleRange, appearance->fBoneScale ); 
    sBodyConfig.scales.waist    = reverseAdjustScale( appearance->fWaistScale,e->seq->type->waistScaleRange, appearance->fBoneScale ); 
    sBodyConfig.scales.leg        = ( appearance->fLegScale  > 0.0)
        ? appearance->fLegScale/max(0, ABS(e->seq->type->legScaleMax))
        : appearance->fLegScale/max(0, ABS(e->seq->type->legScaleMin));
    sBodyConfig.scales.arm        = (appearance->fArmScale >= 0.0)
        ? appearance->fArmScale / ARM_GROW_SCALE
        : appearance->fArmScale / ARM_SHRINK_SCALE;

    memcpy( costume->appearance.f3DScales, appearance->f3DScales, sizeof(Vec3)*NUM_3D_BODY_SCALES );

}

void genderSetScalesFromApperance( Entity * e )
{
    genderSetScalesFromSpecifiedAppearance(e, &(e->costume->appearance));
}

void tailorGenderInit( Entity * e )
{
    genderSetScalesFromSpecifiedAppearance(e, &(costume_current(e)->appearance));
    converge = 0;
    gCurrentBuild = BUILD_ATHLETIC;
}

F32 drawGenderSlider(float x, float y, float screenScaleX, float screenScaleY, float scale, int sliderType, char * minTitle, char * maxTitle, bool isAdjusted, F32 adjusted, F32 range )
{
    float screenScale = MIN(screenScaleX, screenScaleY);
    font( &game_12 );
      font_color( CLR_WHITE, SELECT_FROM_UISKIN( 0xaaaaffff, 0xaaaaaaff, CLR_WHITE ) );
      prnt( x - ((110*screenScaleX)+ str_wd(font_grp, screenScaleX, screenScaleY, minTitle)), y, 60, screenScaleX, screenScaleY, minTitle );
      //prnt( x + 110, y, 60, 1.f, 1.f, maxTitle);

      if( game_state.editnpc)
    {
  //         if(isAdjusted)
  //            prnt(x-50, y+200, 60, 1.f, 1.f, "%.2f (%.2f adjusted) Range: %.2f", scale, adjusted, range);
//        else
//            prnt(x-50, y+200, 60, 1.f, 1.f, "%.2f", scale);
    }

    return doSlider( x, y-5, 60, 200*screenScaleX/screenScale, screenScale, screenScale, scale, sliderType, uiColors.standard.scrollbar, 0 );
}

// change the bone scale
//
void genderBoneScale(float x, float y, float screenScaleX, float screenScaleY, float uiScale, int costume)
{
    F32 * pScale = &sBodyConfig.scales.bone;
    Entity *e = playerPtr();
    float screenScale = MIN(screenScaleX, screenScaleY);

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( !costume )
    {
        if(game_state.editnpc)
            prnt(740*screenScaleX, y + (5*screenScaleY), 60, uiScale*screenScale, uiScale*screenScale, "%.2f", *pScale);

         *pScale = drawGenderSlider( x, y, screenScaleX, screenScaleY, *pScale, SLIDER_BONE_SCALE, "Physique", "", true, e->costume->appearance.fBoneScale, 2 );

    }
    else
    {
         prnt(x - 140*screenScaleX, y+5*screenScaleY, 5000, uiScale*screenScale, uiScale*screenScale, "Physique" );
        if( uiScale >= 1.f )
            *pScale = doSlider( x+20*screenScaleX, y, 5000, 200*screenScaleX/screenScale, screenScale, screenScale, *pScale, SLIDER_BONE_SCALE, uiColors.standard.scrollbar, 0 );
        else
            doSlider( x+20*screenScaleX, y, 5000, 200*screenScaleX/screenScale, screenScale, screenScale, *pScale, SLIDER_BONE_SCALE, uiColors.standard.scrollbar, 0 );
    }

    
    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fBoneScale = *pScale;
    }

    changeBoneScale( e->seq, previewNetworkScale(*pScale) );
}

void genderShoulderScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume)
{
    F32 adjusted;
    F32 * pScale = &sBodyConfig.scales.shoulder;
    Entity *e = playerPtr();
    F32 range = e->seq->type->shoulderScaleRange;

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( costume )
    {
        if( uiscale == 1.f )
            *pScale = costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_SHOULDER_SCALE, "Shoulders", e->costume->appearance.fShoulderScale, range, uiscale, screenScaleX, screenScaleY);
        else
            costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_SHOULDER_SCALE, "Shoulders", e->costume->appearance.fShoulderScale, range, uiscale, screenScaleX, screenScaleY);
    }
    else
        *pScale = drawGenderSlider( x, y, screenScaleX, screenScaleY, *pScale, SLIDER_CHARACTER_SHOULDER_SCALE, "Shoulders", "Max Shoulders", true, e->costume->appearance.fShoulderScale, range);

    adjusted = adjustScale(*pScale, range);

    e->seq->shoulderScale = adjusted;
    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fShoulderScale = adjusted;
    }
}

void genderHipScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume)
{
    F32 adjusted;
    F32 * pScale = &sBodyConfig.scales.hip;
    Entity *e = playerPtr();
    F32 range = e->seq->type->hipScaleRange;

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( costume )
    {
        if( uiscale == 1.f )
            *pScale = costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_HIP_SCALE, "Hips", e->costume->appearance.fHipScale, range, uiscale, screenScaleX, screenScaleY);
        else
            costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_HIP_SCALE, "Hips", e->costume->appearance.fHipScale, range, uiscale, screenScaleX, screenScaleY);
    }
    else
        *pScale = drawGenderSlider(x, y, screenScaleX, screenScaleY, *pScale, SLIDER_CHARACTER_HIP_SCALE, "Hips", "Max Hips", true, e->costume->appearance.fHipScale, range);

    adjusted = adjustScale(*pScale, range);
    //    adjusted = *pScale;

    e->seq->hipScale = adjusted;
    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fHipScale = adjusted;
    }
}

void genderHeadScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume)
{
    F32 * pScale = &sBodyConfig.scales.head;
    Entity *e = playerPtr();
    F32 range = e->seq->type->headScaleRange;

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( costume )
    {
        if( uiscale == 1.f )
            *pScale = costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_HEAD_SCALE, "Head", e->costume->appearance.fHeadScale, range, uiscale, screenScaleX, screenScaleY);
        else
            costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_HEAD_SCALE, "Head", e->costume->appearance.fHeadScale, range, uiscale, screenScaleX, screenScaleY);
    }
    else
        *pScale = drawGenderSlider(x, y, screenScaleX, screenScaleY, *pScale, SLIDER_CHARACTER_HEAD_SCALE, "Head", "Max Head", true, e->costume->appearance.fHeadScale, range);

    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fHeadScale = adjustScale(*pScale, range);
    }
}


void genderWaistScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume)
{
    F32 * pScale = &sBodyConfig.scales.waist;
    Entity *e = playerPtr();
    F32 range = e->seq->type->waistScaleRange;

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( costume )
    {
        if( uiscale == 1.f )
            *pScale = costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_WAIST_SCALE, "Waist", e->costume->appearance.fWaistScale, range, uiscale, screenScaleX, screenScaleY);
        else
            costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_WAIST_SCALE, "Waist", e->costume->appearance.fWaistScale, range, uiscale, screenScaleX, screenScaleY);
    }
    else
        *pScale = drawGenderSlider(x, y, screenScaleX, screenScaleY, *pScale, SLIDER_CHARACTER_WAIST_SCALE, "Waist", "Max Waist", true, e->costume->appearance.fWaistScale, range);

    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fWaistScale = adjustScale(*pScale, range);
    }
}

void genderChestScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume)
{
    F32 * pScale = &sBodyConfig.scales.chest;
    Entity *e = playerPtr();
    F32 range = e->seq->type->chestScaleRange;

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( costume )
    {
        if( uiscale == 1.f )
            *pScale = costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_CHEST_SCALE, "Chest", e->costume->appearance.fChestScale, range, uiscale, screenScaleX, screenScaleY);
        else
            costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_CHEST_SCALE, "Chest", e->costume->appearance.fChestScale, range, uiscale, screenScaleX, screenScaleY);
    }
    else
        *pScale = drawGenderSlider(x, y, screenScaleX, screenScaleY, *pScale, SLIDER_CHARACTER_CHEST_SCALE, "Chest", "Max Chest", true, e->costume->appearance.fChestScale, range);

    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fChestScale = adjustScale(*pScale, range);
    }
}


void genderLegScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume)
{
    F32 * pScale = &sBodyConfig.scales.leg;
    Entity *e = playerPtr();
    F32 adjusted;

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( costume )
    {
        if( uiscale == 1.f )
            *pScale = costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_LEG_SCALE, "Legs", 0, 0, uiscale, screenScaleX, screenScaleY);
        else
            costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_LEG_SCALE, "Legs", 0, 0, uiscale, screenScaleX, screenScaleY);
    }
    else
        *pScale = drawGenderSlider(x, y, screenScaleX, screenScaleY, *pScale, SLIDER_CHARACTER_LEG_SCALE, "Legs", "Max Legs", false, 0, 0);

    if(*pScale > 0)
        adjusted = (*pScale) * max(0, ABS(e->seq->type->legScaleMax));
    else
        adjusted = (*pScale) * max(0, ABS(e->seq->type->legScaleMin));    

    e->seq->legScale = adjusted;
    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fLegScale = adjusted;
    }
}

void genderArmScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume)
{
    F32 adjusted;
    F32 * pScale = &sBodyConfig.scales.arm;
    Entity *e = playerPtr();

    font( &game_12 );
    font_color( uiColors.standard.text, uiColors.standard.text2 );

    if( costume )
    {
        if( uiscale == 1.f )
            *pScale = costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_ARM_SCALE, "Arms", e->costume->appearance.fArmScale, 0, uiscale, screenScaleX, screenScaleY);
        else
            costumeDrawSlider(x, y, *pScale, SLIDER_CHARACTER_ARM_SCALE, "Arms", e->costume->appearance.fArmScale, 0, uiscale, screenScaleX, screenScaleY);
    }
    else
        *pScale = drawGenderSlider(x, y, screenScaleX, screenScaleY, *pScale, SLIDER_CHARACTER_ARM_SCALE, "Arms", "Max Forearms", false, 0, 0);

    if((*pScale) >= 0.0)
        adjusted = (*pScale)*ARM_GROW_SCALE;
    else
        adjusted = (*pScale)*ARM_SHRINK_SCALE;

    e->seq->armScale = adjusted;
    {
        Costume* mutable_costume = costume_as_mutable_cast(e->costume);
        mutable_costume->appearance.fArmScale = adjusted;
    }
}

void genderFaceScale(float x, float y, float screenScaleX, float screenScaleY, float uiscale, int costume, int part)
{

     Entity *e = playerPtr();
    int i;
    float screenScale = MIN(screenScaleX, screenScaleY);
    static char* titles[] = {"Head", "Brow", "Cheek", "Chin", "Cranium", "Jaw", "Nose"};

      if( costume || game_state.editnpc )
    {
        font( &game_12 );
        font_color( uiColors.standard.text, uiColors.standard.text2 );
        prnt((x - 140*uiscale)*screenScaleX, (y+5*uiscale)*screenScaleY, 5000, uiscale*screenScale, uiscale*screenScale, titles[part]);
    }
     x -= 50;

    for(i=0;i<3;i++)
    {
        int type = SLIDER_CHARACTER_HEADX_SCALE + i + (part * 3);
        int wd = 60;
        char * title = (i) ? "" : titles[part];
        
          F32 part_scale = e->costume->appearance.f3DScales[part][i];
        F32 tmpsc = part_scale;

          if( costume )
        {
            if( uiscale >= 1.f )
            {
                tmpsc = costumeDrawSliderTriple(x*screenScaleX, y*screenScaleY, wd*screenScaleX/screenScale, tmpsc, type, 0, 0, uiscale*screenScale);
            }
            else
                costumeDrawSliderTriple(x*screenScaleX, y*screenScaleY, wd*screenScaleX/screenScale, tmpsc, type, 0, 0, uiscale*screenScale);
        }
        else if ( game_state.editnpc )
             tmpsc = costumeDrawSliderTriple(x*screenScaleX, y*screenScaleY, wd*screenScaleX/screenScale, tmpsc, type, 0, 0, uiscale*screenScale);

        if( ABS(tmpsc-part_scale) > .005f )
        {
            Costume* mutable_costume = costume_as_mutable_cast(e->costume);
            mutable_costume->appearance.f3DScales[part][i] = tmpsc;
        }

        x += (wd+10);
    }
}

typedef struct DemoAnim
{
    const char * name;
    const char ** stateBits;
    const char ** flashBits;
    const char * filename;
}DemoAnim;

typedef struct DemoAnimList
{
    DemoAnim **animList;
    
}DemoAnimList;
DemoAnimList gAnimList = { 0 };
TokenizerParseInfo ParseDemoAnim[] =
{
    { "{",          TOK_START,                      0    },
    { "Name",        TOK_STRING(DemoAnim, name, 0)        },
    { "StateBit",    TOK_STRINGARRAY(DemoAnim, stateBits)},
    { "FlashBit",    TOK_STRINGARRAY(DemoAnim, flashBits)},
    { "filename",    TOK_CURRENTFILE(DemoAnim,filename)    },
    { "}",          TOK_END,                        0    },
    { 0 }
};

TokenizerParseInfo ParseDemoAnimList[] =
{
    { "Animation",    TOK_STRUCT(DemoAnimList, animList, ParseDemoAnim)        },
    { "", 0, 0 }
};

void initAnimComboBox(int rebuild);
/**********************************************************************func*
* load_MenuAnimations
*
*/
static void GenderRebuildAnimList(const char *relpath, int when)
{
    if (strEndsWith(relpath, ".bak"))
        return;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    StructClear(ParseDemoAnimList, &gAnimList);
    if (ParserReloadFile(relpath, ParseDemoAnimList, sizeof(DemoAnimList), &gAnimList, NULL, NULL))
    {
        initAnimComboBox(1);
    }
}
void load_MenuAnimations()
{
    const char *pchFilename = "defs/menuAnimations.def";
    const char *pchBinFilename = MakeBinFilename(pchFilename);

    ParserLoadFiles(0,pchFilename, pchBinFilename, 0, ParseDemoAnimList, &gAnimList, NULL, NULL, NULL);
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, pchFilename, GenderRebuildAnimList);
}

void setAnim( int i, int* flashBits )
{
    seqClearState(playerPtrForShell(1)->seq->state);
    setAnims(gAnimList.animList[i]->stateBits);
    if(*flashBits)
    {
        setAnims(gAnimList.animList[i]->flashBits);
        *flashBits = 0;
    }
}

void setAnims(const char **bitnames)
{
    int i, count = eaSize(&bitnames);
    for(i = 0; i < count; ++i)
    {
        int state_number = seqGetStateNumberFromName(bitnames[i]);
        if(state_number >= 0)
            seqSetState(playerPtrForShell(1)->seq->state, TRUE, state_number);
    }
}

static void genderBuild(float screenScaleX, float screenScaleY)
{

    float x = 50*screenScaleX;
    float slider_off = 400*screenScaleX;
      float y = 300*screenScaleY;
    float space = 180*screenScaleX;
    float wd = 550*screenScaleX;
    int i = 0;
     const int SPACING = 25*screenScaleY;
    float scale = MIN(screenScaleX, screenScaleY);
    Entity * e = playerPtr();

    // Draw a large paddle and bounding frame
    drawMenuBar( x + wd/2, y, 20, scale, wd/scale, "Build", 0, 1, 0, 0, 1, CLR_WHITE );
      drawFrame( PIX2, R6, x, y, 10, wd, 300*screenScaleY, 1.f, 0xffffff88, SELECT_FROM_UISKIN( 0x0000ff44, 0xff000044, 0x00000066 ) );

     y += 60*screenScaleY;
       genderScale( x + wd/2, y, screenScaleX, screenScaleY);    // scale slider (specialer)
     y += 45 *screenScaleY;
 
      genderBuildSelect( x+(40*screenScaleX), y+(5*screenScaleY), screenScaleX, screenScaleY );
       genderBoneScale(        x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);
    //genderHeadScale(        x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);
    genderShoulderScale(    x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);
    if(game_state.editnpc)
        genderArmScale(        x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);
    genderChestScale(        x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);
    genderWaistScale(        x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);
    genderHipScale(            x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);
    genderLegScale(            x+slider_off, y + (SPACING * i++), screenScaleX, screenScaleY, 1.f, 0);

    changeBoneScaleEx(e->seq, &e->costume->appearance);
}

//---------------------------------------------------------------------------------------------------------
// Lord of the Gender Menu Functions //////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------------------------------------

// 
//

static int gender_init = FALSE;
static int gender_init2 = FALSE;    // for demo only

void resetGenderUI()
{
    gender_inCostume = FALSE;
    gender_init = FALSE;
    gender_init2 = FALSE;
    gender_reset();
}

void initAnimComboBox(int rebuild)
{
    int i=0;
    static char ** strings = 0;
    if (rebuild)
    {
        eaDestroy(&strings);
        strings = NULL;
    }
    if(!strings)
    {
        for (i = 0; i < eaSize(&gAnimList.animList); ++i)
        {
            eaPushConst(&strings, gAnimList.animList[i]->name);
        }

        comboboxClear( &comboAnims);
        combobox_init( &comboAnims, 570,  75, 5000, 110, 23, i*18 + 1, strings, 0 );
    }
}

//void genderChangeMenuExit(int continueWithChanges)
//{
//    Entity *e = playerPtr();
//    if (continueWithChanges)
//    {
//        int genderMatch = 0;
//        if(prevCostume)
//        {
//            switch (prevCostume->appearance.bodytype)
//            {
//                case kBodyType_Male:
//                    if ( gCurrentGender == AVATAR_GS_MALE )
//                    {
//                        genderMatch = 1;
//                    }
//                    break;
//                case kBodyType_Female:
//                    if ( gCurrentGender == AVATAR_GS_FEMALE )
//                    {
//                        genderMatch = 1;
//                    }
//                    break;
//                case kBodyType_Huge:
//                    if ( gCurrentGender == AVATAR_GS_HUGE )
//                    {
//                        genderMatch = 1;
//                    }
//                    break;            
//            }    
//        }
//
//        if (!genderMatch)
//        {
//            //    since this is a new gender, we reset the costume
//            //    this will reset EVERY time they hit the next button, not just the first time. 
//            //    If it becomes a problem, I can have it only reset the first time
//            //    set the current costume to new reset costume (for loading)
//            genderEditingMode = GENDER_CHANGE_MODE_DIFFERENT_GENDER;
//            costume_destroy(costume_current(e));
//            e->pl->costume[e->pl->current_costume] = costume_clone(e->costume);
//            costume_setCurrentCostume(e, 0);
//            resetCostume(0);
//            costume_destroy(costume_current(e));
//            e->pl->costume[e->pl->current_costume] = costume_clone(e->costume);
//        }
//        else
//        {
//            //    This is the old costume, set the current costume to the original costume for the tailor cost stuff
//            //    set the current costume to original (for loading)
//            genderEditingMode = GENDER_CHANGE_MODE_BASIC;
//            costume_destroy(costume_current(e));
//            e->pl->costume[e->pl->current_costume] = costume_clone( costume_as_const(prevCostume) );        
//            costume_setCurrentCostume(e, 0);
//            updateCostume();
//        }
//        
//        costume_Apply(e);
//        start_menu(MENU_TAILOR);
//    }
//    else
//    {
//        //    revert
//        costume_destroy(costume_current(e));
//        e->pl->costume[e->pl->current_costume] = prevCostume;
//        prevCostume = NULL;
//        tailor_exit(0);
//        genderEditingMode = 0;
//        gCurrentGender = prevGender;
//        gCurrentBodyType = prevBodyType;
//
//        NLM_incrementTransactionID(NLM_TailorMenu());
//        start_menu(MENU_GAME);
//    }
//}

static void genderMenuExitCode(void)
{
    gender_init = FALSE;
    gender_inCostume = TRUE;
    texEnableThreadedLoading();

    toggle_3d_game_modes(SHOW_NONE);

    electric_clearAll();
    // kill blobby fx
    fxDelete(green_blobby_fx_id, HARD_KILL);
    green_blobby_fx_id = 0;
}
static void genderMenuTailorExitCode(void)
{
    genderMenuExitCode();
    setDefaultCostume(false);
    //genderChangeMenuExit(1);
}
static int genderMenuTailorHiddenCode(void)
{
    Entity *e = playerPtr();
    return !((isDevelopmentMode() || AccountCanAccessGenderChange(&e->pl->account_inventory, e->pl->auth_user_data, e->access_level)) && genderEditingMode);
}
NonLinearMenuElement cc_gender_NLME = { MENU_GENDER, { "CC_GenderString", 0, 0 }, 
                                                0, 0, 0, 0, genderMenuExitCode };
NonLinearMenuElement tailor_gender_NLME = { MENU_GENDER, { "CC_GenderString", 0, 0 },
                                                genderMenuTailorHiddenCode, 0, 0, 0, genderMenuTailorExitCode };
NonLinearMenuElement PCC_gender_NLME = { MENU_GENDER, { "CC_GenderString", 0, 0 },
                                                0, 0, 0, 0, genderMenuExitCode };

static int genderFromBodyType(BodyType bodytype)
{
    int gender;
    switch (bodytype)
    {
        case kBodyType_Male:
            gender = AVATAR_GS_MALE;
            break;
        case kBodyType_Female:
            gender = AVATAR_GS_FEMALE;
            break;
        case kBodyType_Huge:
            gender = AVATAR_GS_HUGE;
            break;
        default:
            gender = AVATAR_GS_MALE;
            break;
    }

    return gender;
}

static const char* genderTextFromGender(int gender)
{
    if( gender == AVATAR_GS_FEMALE || gender == AVATAR_GS_BF )
        return "fem";
    else if ( gender == AVATAR_GS_HUGE || gender == AVATAR_GS_ENEMY || gender == AVATAR_GS_ENEMY2 )
        return "huge";
    else
        return "male";
}

void genderMenu()
{
    Entity *e, *pe;
     AtlasTex * layover = NULL; //atlasLoadTexture( "_Layout_bodytype.tga" );
    int w, h;
    int rawCost = 0;
    int finalCost = 0;
    // for new bone scale slider test
     int yOffset = 0;
     float yScale = 0;
    int bGreyPay = 0;
    int screenScalingDisabled = is_scrn_scaling_disabled();
    float screenScaleX, screenScaleY, scale;

       windowClientSizeThisFrame( &w, &h );
    calc_scrn_scaling(&screenScaleX, &screenScaleY);
    scale = MIN(screenScaleX, screenScaleY);

     drawBackground( layover );
    set_scrn_scaling_disabled(1);
    pe = playerPtr();
    if (getPCCEditingMode() < 0)
        setPCCEditingMode(getPCCEditingMode()*-1);

    e = playerPtr();
    // title bar
    //if (e != pe)
    //    drawMenuTitle( 10*screenScaleX, 30*screenScaleY, 20, "ChooseBodyType", scale, !gender_init );

    if( !gTailoredCostume  && (bUltraTailor || genderEditingMode) )
    {
        prevGender = gCurrentGender;
        prevBodyType = gCurrentBodyType;
        tailor_init(0);

        if (prevCostume)
            costume_destroy(prevCostume);
        prevCostume = costume_current(e);
        e->pl->costume[e->pl->current_costume] = costume_clone( costume_as_const(prevCostume) );

        gCurrentGender = genderFromBodyType(prevCostume->appearance.bodytype);
        set_scrn_scaling_disabled(screenScalingDisabled);
        gender_init = FALSE; // to force set gender next time thru
        return;
    }

    setAnim( comboAnims.currChoice, &comboAnims.newlySelectedItem );

    if( !gender_init )
    {
        int isNewTransaction = 0;
        int id;

        gender_init = TRUE;

        toggle_3d_game_modes(SHOW_TEMPLATE);    // show costume 
        texDisableThreadedLoading();

        // set initial values
        {
            Costume* mutable_costume = costume_as_mutable_cast(e->costume);
            costume_SetScale(mutable_costume, 0);
            scaleHero(e, 0);
            costume_SetBoneScale( mutable_costume, 0 );
            changeBoneScale( e->seq, 0 );
            costume_init(mutable_costume);
        }
        if (isNewTransaction)
            gender_inCostume = FALSE;

        setGender( e, genderTextFromGender(gCurrentGender) );

        if( !gender_inCostume )
        {
            greenBlobbyFx();

            id = buildMatchConfig();
            if( id >=0 )
                SelectConfig( id, -1 );

            if (genderEditingMode)
            {
                genderSetScalesFromSpecifiedAppearance(e, &(costume_current(e)->appearance));
                converge = 0;
                gCurrentBuild = BUILD_ATHLETIC;
            }
        }
        initAnimComboBox(0);
        if (pccFixupCostume && pccCritterFirstGenderPass)
        {
            pccCritterFirstGenderPass = 0;
            costume_current(e)->appearance = pccFixupCostume->appearance;
            genderSetScalesFromSpecifiedAppearance( e, &(e->costume->appearance));
            converge = 0;
            gCurrentBuild = BUILD_ATHLETIC;
        }
    }

    // NEW SETTINGS/SLIDERS
     if(!gBodyConfigList.configs)
    {
        LoadConfigs();
        gender_init2 = true;
    }
     convergeConfigs(); // migrate slider positions toward preset


       drawGenderBackground(screenScaleX, screenScaleY);    // draw the police line up

      genderSelect(screenScaleX, screenScaleY);            // the gender selection check bars
    genderBuild(screenScaleX, screenScaleY);            // the body presets and sliders

    if( game_state.editnpc)
    {
           if( D_MOUSEHIT == drawStdButton( 220*screenScaleX, 580 * screenScaleY, 20, 90*screenScaleX, 30*screenScaleY, 0x4466ffff, "Load All", 1.5f, 0 ) )
             LoadConfigs();

        if( D_MOUSEHIT == drawStdButton( 220*screenScaleX, 620*screenScaleY, 20, 90*screenScaleX, 30*screenScaleY, 0x4466ffff, "Save All", 1.5f, 0 ) )
            SaveConfigs(comboDefaults.currChoice);

        if( D_MOUSEHIT == drawStdButton( 220*screenScaleX, 660*screenScaleY, 20, 90*screenScaleX, 30*screenScaleY, 0x4466ffff, "Reset Current", 1.5f, 0 ) )
            ResetCurrentConfig(comboDefaults.currChoice);

        combobox_display( &comboAnims );
    }

      drawSpinButtons(screenScaleX, screenScaleY);

//       drawMenuFrame( R12, 632, 64, 10, 360, 620, CLR_WHITE, 0x00000088, FALSE );
    moveAvatar( gCurrentGender );
/*
    if( D_MOUSEHIT == drawStdButton( 512, 742, 20, 80, 30, 0x4466ffff, "HelpString", 1.5f, 0 ) )
        setHelpOverlay( "Help_Gender.tga" );
*/

    if( bUltraTailor )
    {
        if ( drawNextButton( 40*screenScaleX,  740*screenScaleY, 10, scale, 0, 0, 0, "BackString" ))
        {
            // revert to previous costume
            if(prevCostume && genderEditingMode)
            {
                costume_destroy(costume_current(e));
                e->pl->costume[e->pl->current_costume] = prevCostume;
                prevCostume = NULL;
            }
            tailor_exit(0);
        }
    }
    else
    {
           if ( !game_state.e3screenshot && drawNextButton( 40*screenScaleX,  740*screenScaleY, 10, scale, 0, 0, 0, "BackString" ))
        {
            genderMenuExitCode();
            sndPlay( "back", SOUND_GAME );
            if (genderEditingMode)
            {
                //genderChangeMenuExit(0);
            }
            else
            {
                if (getPCCEditingMode())
                {
                    if (pccCritterRank == PCC_RANK_CONTACT)
                    {
                        start_menu(MENU_PCC_RANK);
                    }
                    else
                    {
                        start_menu(MENU_PCC_POWERS);
                    }
                }
            }
            
        }
    }

    if ( drawNextButton( 980*screenScaleX, 740*screenScaleY, 10, scale, 1, bGreyPay, 0, "NextString" ) || inpEdgeNoTextEntry(INP_RETURN) )
    {
        sndPlay( "next", SOUND_GAME );
        genderMenuExitCode();

        if( bUltraTailor )
        {
            setDefaultCostume(false);
            start_menu( MENU_TAILOR );
        }
        else if (genderEditingMode)
        {
            setDefaultCostume(false);
            //genderChangeMenuExit(1);
        }
        else
        {
            costumeMenuEnterCode();
            start_menu( MENU_COSTUME );
        }
    }

     drawHelpOverlay();
    set_scrn_scaling_disabled(0);
    if (e != pe)
    {
        drawNonLinearMenu( NLM_PCCCreationMenu(), 20, 10, 20, 1000,20, 1.0f, PCC_gender_NLME.listId, PCC_gender_NLME.listId, 0 );
    }
    if (getPCCEditingMode() > 0)
        setPCCEditingMode(getPCCEditingMode()*-1);
    set_scrn_scaling_disabled(screenScalingDisabled);
}