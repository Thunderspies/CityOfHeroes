#include "win/win_init.h"     // for windowClientSize
#include "entity/SgrpBasePermissions.h"
#include "UI/uiGame.h"   // for start_menu
#include "player/player.h"
#include "entity/costume_client.h"   // for setGender
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/sprite/sprite_text.h"
#include "UI/uiUtilMenu.h"
#include "UI/uiUtilGame.h"
#include "UI/uiAvatar.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "graphics/ttFont.h"
#include "entity/powers.h"
#include "entity/origins.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "cmdparse/cmdgame.h"    // for editnpc
#include "cmdparse/cmdcommon.h"  // for timestep
#include "entity/entclient.h"
#include "UI/uiEditText.h"
#include "UIEdit.h"
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "UI/uiDialog.h"
#include "graphics/font.h"
#include "entity/entVarUpdate.h"
#include "language/langClientUtil.h"
#include "UI/uiCostume.h"
#include "gameData/costume_data.h"
#include "UI/uiSupercostume.h"
#include "graphics/ttFont.h" 
#include "UI/uiBox.h"
#include "sound/sound.h"
#include "UI/uiNet.h"
#include "entity/teamup.h"
#include "UI/uiClipper.h"
#include "graphics/ttFontUtil.h"
#include "entity/entity.h"
#include "graphics/textureatlas.h"
#include <utilitieslib/utils/StringUtil.h>
#include "entity/Supergroup.h"
#include "UI/uiChat.h"
#include "UI/uiFocus.h"
#include "filter/profanity.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#define EMBLEM_DRAW_SIZE 45.f

typedef enum
{
    SUPER_NAME,
    SUPER_MOTTO,
    SUPER_LEADER,
    SUPER_COMMANDER,
    SUPER_GENERAL,
    SUPER_CAPTAIN,
    SUPER_LIEUTENANT,
    SUPER_MEMBER,
    SUPER_MESSAGE,
    SUPER_INFLUENCE,
    SUPER_DEMOTETIMEOUT,
    SUPER_DESCRIPTION,
    SUPER_TOTAL_TEXT_FIELDS,
    SUPER_EMBLEM,
    SUPER_COLOR,
    SUPER_RANKS,
    SRFIELDS_COUNT,
} SRFields;

static char const *s_strFromSRField[] = 
{
"SUPER_NAME",
"SUPER_MOTTO",
"SUPER_LEADER",
"SUPER_COMMANDER",
"SUPER_GENERAL",
"SUPER_CAPTAIN",
"SUPER_LIEUTENANT",
"SUPER_MEMBER",
"SUPER_MESSAGE",
"SUPER_INFLUENCE",
"SUPER_DEMOTETIMEOUT",
"SUPER_DESCRIPTION",
"SUPER_TOTAL_TEXT_FIELDS",
"SUPER_EMBLEM",
"SUPER_COLOR",
"SUPER_RANKS",
};
STATIC_ASSERT(SRFIELDS_COUNT == ARRAY_SIZE(s_strFromSRField));

static UIBox tbox[SUPER_TOTAL_TEXT_FIELDS];                        // Where are the text blocks?    
static UIEdit* superRegBlock[SUPER_TOTAL_TEXT_FIELDS];    // Edit boxes for each text block.
Supergroup fieldTextBuffer;
SRFields activeField = SUPER_NAME;

// fields that are stored as numbers in SG, but we use text fields to display
#define NUMBER_FIELD_LEN 20
static char demoteText[NUMBER_FIELD_LEN];
static char influenceText[NUMBER_FIELD_LEN];

// Which mode should the screen be in?
typedef enum
{
    SRM_MODIFY,        // User is modifying existing data.
    SRM_CREATE,        // User is creating new supergroup
} SRMode;
SRMode srMode;

// How far along are we in the creation state?
typedef enum
{
    SRCS_NONE,
    SRCS_ENTERING_DATA,            // User is entering data
    SRCS_WAITING_FOR_RESULT,    // User has hit the "accept" button and the game is waitng for a SG creation result.
    SRCS_CREATION_SUCCEEDED,    // SG creation succeeded.
    SRCS_CREATION_FAILED,        // SG creation failed.
} SRCreationState;
SRCreationState srCreationState;

static void srFlashField(SRFields field);
static int srFieldDeselecting(int field);

void srClearText(void)
{
    memset( &fieldTextBuffer, 0, sizeof(Supergroup) );
}

//----------------------------------------
//  uiEditClear
//----------------------------------------
void srClearAll(void)
{
    int i;

    for( i = 0; i < SUPER_TOTAL_TEXT_FIELDS; i++ )
    {
        uiEditClear( superRegBlock[i] );
    }
    srClearText();
}

// Given the current state of the screen, can the user modify the said field.
static int srCanModifyField(int userRank, SRFields field)
{
    Entity* e = playerPtr();

    // In creation mode, the user can modify all fields.
    if(srMode == SRM_CREATE)
    {
        if ( SUPER_INFLUENCE == field ) // read-only
            return 0;
        return 1;
    }
    else if(srMode == SRM_MODIFY && e)
    {
        switch(field)
        {
        case SUPER_MESSAGE:
            return TSTB(e->supergroup->rankList[userRank].permissionInt, SG_PERM_MOTD);
        case SUPER_MOTTO:
            return TSTB(e->supergroup->rankList[userRank].permissionInt, SG_PERM_MOTTO);
        case SUPER_LEADER:
        case SUPER_COMMANDER:
        case SUPER_GENERAL:
        case SUPER_CAPTAIN:
        case SUPER_LIEUTENANT:
        case SUPER_MEMBER:
        case SUPER_RANKS:
            return TSTB(e->supergroup->rankList[userRank].permissionInt, SG_PERM_RANKS);
        case SUPER_DEMOTETIMEOUT:
            return TSTB(e->supergroup->rankList[userRank].permissionInt, SG_PERM_AUTODEMOTE);
        case SUPER_DESCRIPTION:
            return TSTB(e->supergroup->rankList[userRank].permissionInt, SG_PERM_DESCRIPTION);
        case SUPER_EMBLEM:
        case SUPER_COLOR:
            return TSTB(e->supergroup->rankList[userRank].permissionInt, SG_PERM_COSTUME);
        }

        return 0;
    }
    return 0;
}

static int srGetPlayerRank(void)
{
    Entity* e = playerPtr();
    int i;

    if(srMode == SRM_CREATE)
        return NUM_SG_RANKS_VISIBLE - 1;

    for(i = 0; i < eaSize(&e->supergroup->memberranks); i++)
        if(e->supergroup->memberranks[i]->dbid == e->db_id)
            return e->supergroup->memberranks[i]->rank;

    return 0;
}

// helper for setting/getting sgrp  fields
struct FieldSgOffsets
{
    SRFields field;
    int oStr;
} s_FieldSgrpOffsetPairs[] = 
{
    { SUPER_NAME,            offsetof(Supergroup,name)},
    { SUPER_MOTTO,            offsetof(Supergroup,motto)},
    { SUPER_LEADER,            offsetof(Supergroup,rankList[5].name)},
    { SUPER_COMMANDER,        offsetof(Supergroup,rankList[4].name)},
    { SUPER_GENERAL,        offsetof(Supergroup,rankList[3].name)},
    { SUPER_CAPTAIN,        offsetof(Supergroup,rankList[2].name)},
    { SUPER_LIEUTENANT,        offsetof(Supergroup,rankList[1].name)},
    { SUPER_MEMBER,            offsetof(Supergroup,rankList[0].name)},
    { SUPER_MESSAGE,        offsetof(Supergroup,msg)},
    { SUPER_DESCRIPTION,    offsetof(Supergroup,description)},
};

//----------------------------------------
//  mimic behavior of uiedittext
//----------------------------------------
static char* s_GetFieldUIEdit(SRFields field)
{
    char *str = AINRANGE( field, superRegBlock ) ? uiEditGetUTF8Text(superRegBlock[field]) : NULL;
    return str ? str : "";
}

static void s_SetFieldUIEdit(SRFields field, char *txt)
{
    if(verify( AINRANGE( field, superRegBlock ) && superRegBlock[field]))
    {
        uiEditSetUTF8Text(superRegBlock[field], txt);
    }
}

static void s_SetFieldTextBufFromUIEdit(SRFields field)
{
    int i;
    for( i = 0; i < ARRAY_SIZE( s_FieldSgrpOffsetPairs ); ++i ) 
    {
        if( s_FieldSgrpOffsetPairs[i].field == field )
        {
            break;
        }
    }
    if(INRANGE0( i, ARRAY_SIZE( s_FieldSgrpOffsetPairs )))
    {
        char *dest = ((char*)&fieldTextBuffer + s_FieldSgrpOffsetPairs[i].oStr);
        strcpy(dest,s_GetFieldUIEdit(s_FieldSgrpOffsetPairs[i].field));
    }
}


// Issue commands to begin the creation of a super group on the
// server. this happens when the user is done with this screen.
static void srBeginSGCreate()
{
    Entity* e = playerPtr();
    int i;
    
    if(    srCreationState == SRCS_WAITING_FOR_RESULT ||
        srCreationState == SRCS_CREATION_SUCCEEDED ||
        srCreationState == SRCS_CREATION_FAILED)
        return;

    // send the create supergroup message
    strncpyt(fieldTextBuffer.emblem, gSuperEmblem.info[gSuperEmblem.currentInfo]->texName2,
        ARRAY_SIZE(fieldTextBuffer.emblem));
    fieldTextBuffer.tithe = SGROUP_TITHE_MIN;
    srFieldDeselecting(SUPER_DEMOTETIMEOUT);
    fieldTextBuffer.demoteTimeout = atoi(s_GetFieldUIEdit(SUPER_DEMOTETIMEOUT)) * SGROUP_DEMOTE_FACTOR;

    // --------------------
    // set text fields

    for( i = 0; i < ARRAY_SIZE(s_FieldSgrpOffsetPairs); ++i ) 
    {
        char *dest = ((char*)&fieldTextBuffer + s_FieldSgrpOffsetPairs[i].oStr);
        strcpy(dest,s_GetFieldUIEdit(s_FieldSgrpOffsetPairs[i].field));
    }

    // --------------------
    // send creation

    // Special hack for ".tga" suffixes on emblems
    i = strlen(fieldTextBuffer.emblem);
    if (i > 4 && stricmp(&fieldTextBuffer.emblem[i - 4], ".tga") == 0)
    {
        fieldTextBuffer.emblem[i - 4] = 0;
    }

    sgroup_sendCreate(&fieldTextBuffer);
    srCreationState = SRCS_WAITING_FOR_RESULT;

}

// Perform any clean up before exiting this screen.
static void srExitScreen(void)
{
    Entity* e = playerPtr();

    // If the supergroup creation failed for the user, kill the supergroup structure.
    if(srMode == SRM_CREATE && srCreationState != SRCS_CREATION_SUCCEEDED && e->supergroup)
    {
        destroySupergroup(e->supergroup);
        e->supergroup = NULL;
    }

    srCreationState = SRCS_NONE;
    uiReturnAllFocus();
}

// Move on to the supergroup costume screen.
static void srShowSGCostumeScreen()
{
    srExitScreen();
    toggle_3d_game_modes( SHOW_TEMPLATE );
    start_menu( MENU_SUPERCOSTUME );
}

static void srReturnToGame()
{
    srExitScreen();
    start_menu( MENU_GAME );
}

void srReportSGCreationResult(int success)
{
    if(success)
    {
        srCreationState = SRCS_CREATION_SUCCEEDED;
    }
    else
    {
        srCreationState = SRCS_CREATION_FAILED;
    }

    //dialogRemove("srWaitingSGCreation", NULL, NULL);
}

//----------------------------------------
//  uiEditGetUTF8Text, remember to clean up
//----------------------------------------
static int srFieldDeselecting(int field)
{
    int min = 0, max = 0;
    int failed = 0;
    int i;
    char* str = uiEditGetUTF8Text(superRegBlock[field]);

    if (field == SUPER_DEMOTETIMEOUT)
    {
        min = SGROUP_DEMOTE_MIN / SGROUP_DEMOTE_FACTOR;
        max = SGROUP_DEMOTE_MAX / SGROUP_DEMOTE_FACTOR;
    }
    else
        return 1; // other fields ok

    // verify all characters are numbers
    if(str)
    {
        for (i = strlen(str)-1; i >= 0; i--)
        {
            if (str[i] < '0' || str[i] > '9')
            {
                failed = 1;
                break;
            }
        }
        if (!failed)
        {
            i = atoi(str);
            if (i < min || i > max)
                failed = 1;
        }

        if (failed)
        {
            sprintf(str, "%i", min);
            uiEditSetCursorPos(superRegBlock[field], UTF8GetLength( str ));
            srFlashField(field);
        }
    }
    return 1; // allow deselect to continue regardless
}

static int srFindModifiableField(int startIndex)
{
    int i;

    startIndex = MAX(startIndex, 0);
    if(startIndex == SUPER_TOTAL_TEXT_FIELDS)
        startIndex = 0;
    // Starting from the current active box, search for the next accessible field.
    for(i = startIndex; i < SUPER_TOTAL_TEXT_FIELDS; i++)
    {
        if(srCanModifyField(srGetPlayerRank(), i))
            break;
    }
    if(i == SUPER_TOTAL_TEXT_FIELDS)
    {
        for(i = 0; i < startIndex; i++)
        {
            if(srCanModifyField(srGetPlayerRank(), i))
                break;
        }
    }

    // Can't find any modifiable fields?
    if(i == startIndex && !srCanModifyField(srGetPlayerRank(), startIndex))
        return -1;

    return i;
}

static void s_FieldUiEditReturnFocus(SRFields field)
{
    if(verify( INRANGE0( field, ARRAY_SIZE( superRegBlock ) )))
    {
        s_SetFieldTextBufFromUIEdit(field); // keep supergroup text buffer up to date
        uiEditReturnFocus(superRegBlock[field]);        
    }
}


static void srSelectFirstModifiableField(int startIndex)
{
    int i;

    // Make sure all fields are unselected.
    for(i = 0; i < SUPER_TOTAL_TEXT_FIELDS-1; i++)
    {
        if (superRegBlock[i] && superRegBlock[i]->inEditMode)
        {
            s_FieldUiEditReturnFocus(i);
            if (!srFieldDeselecting(i))
                return;
        }
    }

    activeField = srFindModifiableField(startIndex);
    if(activeField != -1 && superRegBlock[activeField])
    {
        uiEditTakeFocus(superRegBlock[activeField]);
    }
}
//--------------------------------------------------------------------------------------------------

static void processMouseOverBox(UIBox* box, float z)
{
    AtlasTex* tex = white_tex_atlas;
    if(!box)
        return;

    if(uiMouseCollision(box))
    {
        display_sprite(tex, box->x, box->y, z, box->width/tex->width, box->height/tex->height, CLR_SELECTION_BACKGROUND);
    }
}

static void darkBackgroundDisplay(int z)
{
    // Display a dark background.
    AtlasTex* background = white_tex_atlas;
     display_sprite(background, 0, 0, z, (float)DEFAULT_SCRN_WD/background->width, (float)DEFAULT_SCRN_HT/background->height, game_state.skin == UISKIN_HEROES ? CLR_CONSTRUCT_EX(0, 0, 0, 0.92) : CLR_CONSTRUCT_EX(0, 0, 0, 0.52));
}

static void drawRegistrationForm(void)
{
    // This is a terrible hack.
    // The problem is that way deep in the guts of atlasLoadTexture(...) we look at game_state.skin to determine if we should prepend a "v_" for villains,
    // and a "p_" for Praetorians.  Rather than try passing down an override, and generally screwing around with that code which is stable, works, and
    // hasn't been touched in forever, I do a very simple, but extremely grotesque hack.  If the player is dark, we take a look at game_state.skin and flip
    // its sense for just this call.
    AtlasTex* form;
    UISkin original_skin;
    Entity *player;
    
    player = playerPtr();
    original_skin = game_state.skin;
    if (player && sgroup_isPlayerDark(player))
    {
        if (original_skin == UISKIN_HEROES)
        {
            game_state.skin = UISKIN_VILLAINS;
        }
        else if (original_skin == UISKIN_VILLAINS)
        {
            game_state.skin = UISKIN_HEROES;
        }
    }

    form = atlasLoadTexture("registration_form_background.tga");
    display_sprite( form, 0, 0, 9, DEFAULT_SCRN_WD/form->width, DEFAULT_SCRN_HT/form->height, CLR_WHITE );

    game_state.skin = original_skin;
}


static void handleTextBlocks(void)
{
    Entity *e = playerPtr();
    static float t = 0;
    int i;
    TTDrawContext* textFont = &smfDefault;
    TTFontRenderParams oldParam = textFont->renderParams;
    int fontHeight = ttGetFontHeight(textFont, 1.0, 1.0);
    AtlasTex * back      = white_tex_atlas;
    float z = 1005;
    int allowCR;

    // --------------------
    // set positions
    // ab: this shouldn't happen every frame

    uiBoxDefine(&tbox[SUPER_NAME],        272, 193, 302, 29);        // supergroup name
    uiBoxDefine(&tbox[SUPER_MOTTO],        272, 244, 302, 43);        // group motto
    uiBoxDefine(&tbox[SUPER_LEADER],    398, 320, 158, 20);        // leader
    uiBoxDefine(&tbox[SUPER_COMMANDER],    398, 346, 158, 20);        // commander
    uiBoxDefine(&tbox[SUPER_GENERAL],    398, 372, 158, 20);        // general
    uiBoxDefine(&tbox[SUPER_CAPTAIN],    398, 398, 158, 20);        // captain
    uiBoxDefine(&tbox[SUPER_LIEUTENANT],398, 424, 158, 20);        // lieutenant
    uiBoxDefine(&tbox[SUPER_MEMBER],    398, 450, 158, 20);        // member
    uiBoxDefine(&tbox[SUPER_MESSAGE],    575, 193, 203, 94);        // message
    uiBoxDefine(&tbox[SUPER_DESCRIPTION], 272, 503, 505, 99);    // description
    uiBoxDefine(&tbox[SUPER_INFLUENCE], 575, 309, 203, 20);        // influence
    uiBoxDefine(&tbox[SUPER_DEMOTETIMEOUT], 627, 461, 106, 21);    // demote timeout

    for( i = 0; i < SUPER_TOTAL_TEXT_FIELDS; ++i ) 
    {
        UIBox box;
        uiBoxDefine(&box,tbox[i].x+PIX3,tbox[i].y-3,tbox[i].width-PIX3*2,tbox[i].height);
        uiEditSetBounds(superRegBlock[i], box);
        uiEditSetZ(superRegBlock[i],z);
    }

    textFont->renderParams.outline = 0;
    
    t += TIMESTEP*.1; // timer for the pulsing background

    // enter text into the current box

    allowCR = activeField == SUPER_MESSAGE || activeField == SUPER_DESCRIPTION;
//    allowCR = activeField == SUPER_MESSAGE;
    if(activeField != -1)
    {
        if(srCanModifyField(srGetPlayerRank(), activeField))
        {
            superRegBlock[activeField]->canEdit = true;
        }
        else
        {
            superRegBlock[activeField]->canEdit = false;
        }
    }
    

    // cycle boxes with the tab key
     if( inpEdge( INP_TAB ) )
    {
        srSelectFirstModifiableField(activeField+1);
    }

    for( i = 0; i < SUPER_TOTAL_TEXT_FIELDS; i++ )
    {
        char space[2] = " ";

        if(srCanModifyField(srGetPlayerRank(), i) && activeField != -1)
        {
            char *str = uiEditGetUTF8Text(superRegBlock[i]);
            
            if( str )
            {
                // if the string is only whitespace, delete it
                if(strspn( str, space ) == strlen( str ) )
                {
                    uiEditClear(superRegBlock[i]);
                }
            }
            else
            {
                 font( &profileFont );
                 font_color( 0x00000033, 0x00000033 );
                 cprntEx( tbox[i].x + tbox[i].width/2, tbox[i].y + tbox[i].height/2, 50, 1.f, 1.f, CENTER_X|CENTER_Y, "ClickToEdit" );
            }

            // check for a mouse click
            if(uiMouseUpHit( &tbox[i], MS_LEFT ) )
            {
                // unselect current box
                if (!srFieldDeselecting(activeField))
                    continue;

                s_FieldUiEditReturnFocus(activeField);

                // Select new box.
                activeField = i;
                uiEditTakeFocus(superRegBlock[i]);
            }

            // Display mouseover highlight
            processMouseOverBox(&tbox[i], z);
        }

        uiEditProcess(superRegBlock[i]);
    }

    if(srCanModifyField(srGetPlayerRank(), activeField) && activeField != -1)
    {
        // display selection
        display_sprite( back, tbox[activeField].x, tbox[activeField].y, z-1, 
            tbox[activeField].width/back->width, 
            tbox[activeField].height/back->height, CLR_SELECTION_BACKGROUND);
    }

    textFont->renderParams = oldParam;

    // weird hack because fieldTextBuffer keeps being used to init fields at odd times
    fieldTextBuffer.demoteTimeout = atoi(demoteText)*SGROUP_DEMOTE_FACTOR;

}

//-------------------------------------------------------------------------------------------------

//
//
static int drawEmblem()
{
    char buf[256];
    AtlasTex * emblem;
    AtlasTex * back = white_tex_atlas;
    UIBox box;
    Entity *e = playerPtr();
    float z = 2000;

    uiBoxDefine(&box, 414, 624, 84, 67);

    if (gSuperEmblem.info[gSuperEmblem.currentInfo]->texName2[0] == '!')
    {
          sprintf( buf, "%s.tga", gSuperEmblem.info[gSuperEmblem.currentInfo]->texName2 );
    }
    else
    {
          sprintf( buf, "emblem_%s.tga", gSuperEmblem.info[gSuperEmblem.currentInfo]->texName2 );
    }
    emblem = atlasLoadTexture( buf );

    if(srCanModifyField(srGetPlayerRank(), SUPER_EMBLEM))
    {
        processMouseOverBox(&box, z);
        if(uiMouseUpHit(&box, MS_LEFT))
            return 1;
    }

    display_sprite( emblem, 
        box.x + (box.width - EMBLEM_DRAW_SIZE)/2, 
        box.y + (box.height - EMBLEM_DRAW_SIZE)/2, 
        z, EMBLEM_DRAW_SIZE/emblem->width, EMBLEM_DRAW_SIZE/emblem->height, CLR_WHITE);

    return 0;
}


#define EMBLEM_PER_ROW 16
#define EMBLEM_VISIBLE_ROWS 8
#define EMBLEM_HORIZ_SPACING 10
#define EMBLEM_VERT_SPACING 11
#define EMBLEM_BUTTON_AREA 80
#define CP_SPEED    7
//
//
static int drawEmblemPicker(int* emblemIndexInOut)
{
     AtlasTex * back = white_tex_atlas;
      int i, x = 100, y = 10, z = 6000;
    int ht, sby, count;
    int ht1;
    UIBox baseFrame, emblemDrawingArea, buttonBox;
    char emblemName[256];
       float emblemDrawSizeX = EMBLEM_DRAW_SIZE, emblemDrawSizeY = EMBLEM_DRAW_SIZE;
    static ScrollBar sb = {0};

     //reverseToScreenScalingFactorf(&emblemDrawSizeX, &emblemDrawSizeY);

    // Draw the background.
      darkBackgroundDisplay(z);

    // Draw the base frame.
    uiBoxDefine(&baseFrame, 0, 0, DEFAULT_SCRN_WD, DEFAULT_SCRN_HT);
    uiBoxAlter(&baseFrame, UIBAT_SHRINK, UIBAD_LEFT | UIBAD_RIGHT, 55); 
    uiBoxAlter(&baseFrame, UIBAT_SHRINK, UIBAD_TOP | UIBAD_BOTTOM, EMBLEM_BUTTON_AREA); 
      drawMenuFrame(R12, baseFrame.x, baseFrame.y, z, baseFrame.width, baseFrame.height, CLR_WHITE, SELECT_FROM_UISKIN(0x04295aff, 0x5a0004ff, 0x000044ff), 0);

    // Draw the emblems.
    uiBoxDefine(&emblemDrawingArea, 0, 0, 
        EMBLEM_PER_ROW * emblemDrawSizeX + (EMBLEM_PER_ROW-1)*EMBLEM_HORIZ_SPACING,
        EMBLEM_VISIBLE_ROWS * emblemDrawSizeY + (EMBLEM_VISIBLE_ROWS - 1) * EMBLEM_VERT_SPACING + 32);
    emblemDrawingArea.x = baseFrame.x + (baseFrame.width - emblemDrawingArea.width)/2;
    emblemDrawingArea.y = baseFrame.y + (baseFrame.height - 40 - emblemDrawingArea.height)/2;
    set_scissor(true);

    scissor_dims( emblemDrawingArea.x - 20,emblemDrawingArea.y, emblemDrawingArea.width + 40, emblemDrawingArea.height );
    sby = emblemDrawingArea.y;
    emblemDrawingArea.y -= sb.offset;

    ht1 = 0;
    count = eaSize(&gSuperEmblem.info);
    //count = 12 * 16;
    for(i = 0; i < count; i++)
    {
        UIBox emblemBox;
        UIBox* box = &emblemDrawingArea;
        AtlasTex * emblem;
        int column =    (i%EMBLEM_PER_ROW);
        int row =        (i/EMBLEM_PER_ROW);
        int x    = box->x + column    * emblemDrawSizeX    + column* EMBLEM_HORIZ_SPACING;
        int y    = box->y + row        * emblemDrawSizeY    + row    * EMBLEM_VERT_SPACING - sb.offset + 18;
        int selected =    (emblemIndexInOut ? *emblemIndexInOut == i : 0);
        int mouseover;
        int l;
        char emblemTex[256];

        uiBoxDefine(&emblemBox, x, y, emblemDrawSizeX, emblemDrawSizeY);
        if (y + emblemDrawSizeY > ht1)
        {
            ht1 = y + emblemDrawSizeY;
        }
        mouseover = uiMouseCollision(&emblemBox);

        strcpy(emblemTex, gSuperEmblem.info[i]->texName2);
        l = strlen(emblemTex);
        if (l > 4 && stricmp(&emblemTex[l - 4], ".tga") == 0)
        {
            emblemTex[l - 4] = 0;
        }
        
        if (gSuperEmblem.info[i]->texName2[0] != '!')
        {
            sprintf(emblemName, "emblem_%s.tga", emblemTex);
        }
        else
        {
            sprintf(emblemName, "%s.tga", emblemTex);
        }
        emblem = atlasLoadTexture(emblemName);
        if(!emblem)
            continue;

        display_sprite(emblem, emblemBox.x, emblemBox.y, z, emblemBox.width/emblem->width, emblemBox.height/emblem->height,  CLR_WHITE);
        if(selected || mouseover)
        {
            AtlasTex* selectionRing = atlasLoadTexture("select_logo_ring.tga");
            UIBox ringBox = emblemBox;
            int color = (selected ? CLR_WHITE : CLR_CONSTRUCT_EX(255, 255, 255, 0.5));
            uiBoxAlter(&ringBox, UIBAT_GROW, UIBAD_ALL, 19);
            display_sprite(selectionRing, ringBox.x, ringBox.y, z, ringBox.width/selectionRing->width, ringBox.height/selectionRing->height, color);    
        }

        if(uiMouseUpHit(&emblemBox, MS_LEFT))
        {
            if(emblemIndexInOut)
                *emblemIndexInOut = i;
        }
    }
    ht = ((count + EMBLEM_PER_ROW - 1) / EMBLEM_PER_ROW) * emblemDrawSizeY + ((count - 1) / EMBLEM_PER_ROW) * EMBLEM_VERT_SPACING;
    // I have no idea why this is necessary.  Obviously something is scaled differently from I'd expect it to be.  So we kluge it.
    // If we ever get complaints after adding more chest emblems and converting them to SG logos, come and fiddle with this.
    if (count > 8 * EMBLEM_PER_ROW)
    {
        int rows = (count + EMBLEM_PER_ROW - 1) / EMBLEM_PER_ROW - 9; 
        ht = ht + 8 - (rows * 28);
    }
    set_scissor(false);

    doScrollBar( &sb, emblemDrawingArea.height, ht, emblemDrawingArea.x + emblemDrawingArea.width + 23, sby, z + 10, 0, &emblemDrawingArea );

    // Draw the accept button
    buttonBox = baseFrame;
    uiBoxAlter(&buttonBox, UIBAT_SHRINK, UIBAD_TOP, baseFrame.height-EMBLEM_BUTTON_AREA);
    if(D_MOUSEHIT == drawStdButton(buttonBox.x+buttonBox.width/2, buttonBox.y+buttonBox.height/2, z, 120, 50, CLR_GREEN, "AcceptString", 2.0, 0))
    {
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------

#define PALETTE_RINGS_PER_ROW 10
#define PALETTE_HORIZ_SPACING 10
#define PALETTE_VERT_SPACING 10
#define CP_SPEED    7
//
//
static int colorPaletteDraw(int* colorInOut)
{
    int i;
    ColorPalette *palette = &gSuperPalette;
    float z = 5000;
    UIBox box, frameBox, buttonBox;
    UIBox ringDimension = {0};
    int pickedColor = (colorInOut ? *colorInOut : 0);

    darkBackgroundDisplay(z);

    // Which region should we draw the color rings in?
    ringDimension.width = 25;
    ringDimension.height = 25;
    //reverseToScreenScalingFactorf(&ringDimension.width, &ringDimension.height);

    uiBoxDefine(&box, 0, 0, 
        PALETTE_RINGS_PER_ROW * ringDimension.width + (PALETTE_RINGS_PER_ROW-1) * PALETTE_HORIZ_SPACING,
        (eaSize((F32***)&palette->color)+9) / PALETTE_RINGS_PER_ROW * ringDimension.height + eaSize((F32***)&palette->color) / PALETTE_RINGS_PER_ROW * PALETTE_VERT_SPACING);

    // Center the drawing region on screen.
    box.x = (DEFAULT_SCRN_WD - box.width)/2;
    box.y = (DEFAULT_SCRN_HT - box.height)/2;

    frameBox = box;
    uiBoxAlter(&frameBox, UIBAT_GROW, UIBAD_ALL, 20);
    uiBoxAlter(&frameBox, UIBAT_GROW, UIBAD_BOTTOM, 40);
    drawMenuFrame(R12, frameBox.x, frameBox.y, z, frameBox.width, frameBox.height, CLR_WHITE, CLR_BLACK, 0);
    for(i = 0; i < eaSize( (F32***) &palette->color ); i++)
    {
        int clr = getColorFromVec3(*(palette->color[i]));
        int column =    (i%PALETTE_RINGS_PER_ROW);
        int row =        (i/PALETTE_RINGS_PER_ROW);
        int x    = box.x + column * ringDimension.width    + column* PALETTE_HORIZ_SPACING;
        int y    = box.y + row * ringDimension.height    + row    * PALETTE_VERT_SPACING;
        CRDMode mode = CRD_MOUSEOVER | CRD_SELECTABLE;
        mode |= (colorInOut && *colorInOut == getColorFromVec3(*palette->color[i]) ? CRD_SELECTED : 0);

        if(colorRingDraw( x+ringDimension.width/2, y+ringDimension.height/2, z, mode, clr, 1.f))
        {
            if(colorInOut)
            {
                *colorInOut = getColorFromVec3(*palette->color[i]);
            }
        }
    }

    buttonBox = frameBox;
    uiBoxAlter(&buttonBox, UIBAT_SHRINK, UIBAD_TOP, box.height);
    if(D_MOUSEHIT == drawStdButton(buttonBox.x + buttonBox.width/2, buttonBox.y + buttonBox.height/2, z, 80, 30, CLR_GREEN, "AcceptString", 1.0, 0))
    {
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------

////
////
#define COLUMN_WIDTH        120
#define ROW_HEIGHT            25
#define MIN_TITLE_SCALE        0.85f

static int permissionEditorDraw()
{
    int i, usedPermissions = 0, curPermission = 0;
    int yLast = 0; // the last drawn y position (add 3 for cprnt)
    ColorPalette *palette = &gSuperPalette;
    float z = 5000;
    char titleBuf[20];
    UIBox box, frameBox;
    CBox cbox;
    SupergroupRank* ranks = &fieldTextBuffer.rankList[0];
    int playerRank = srGetPlayerRank();
    int rank;
    int xTxt = 0;
    int permissionNameWidth = 0;
    float titleScale = 1.0f;
    AtlasTex * filled = atlasLoadTexture( "Context_checkbox_filled.tga" );
    AtlasTex * empty  = atlasLoadTexture( "Context_checkbox_empty.tga" );
    AtlasTex * glow   = atlasLoadTexture("costume_button_link_glow.tga");

    darkBackgroundDisplay(z);

    for(i = 0; i < SG_PERM_COUNT; i++)
    {
         if(sgroupPermissions[i].used)
        {
            usedPermissions++;
            permissionNameWidth = MAX(permissionNameWidth, 2*R10+str_wd(&game_12, 1, 1, sgroupPermissions[i].name));
        }
    }

    uiBoxDefine(&box, 0, 0, 
        COLUMN_WIDTH * NUM_SG_RANKS_VISIBLE + permissionNameWidth + 2 *(PIX3+R10),
         ROW_HEIGHT * (usedPermissions + kSgrpBaseEntryPermission_Count + 4));

    // Center the drawing region on screen.
    box.x = (DEFAULT_SCRN_WD - box.width)/2;
    box.y = (DEFAULT_SCRN_HT - box.height)/2 - 40; // account for the bottom grow below

    // set text drawing offset
     xTxt = box.x + R10 + PIX3 + permissionNameWidth / 2;

    frameBox = box;
     uiBoxAlter(&frameBox, UIBAT_GROW, UIBAD_BOTTOM, 20);
       drawMenuFrame(R12, frameBox.x, frameBox.y, z, frameBox.width, frameBox.height, CLR_WHITE, CLR_BLACK, 0);

     uiBoxAlter(&box, UIBAT_SHRINK, UIBAD_LEFT|UIBAD_RIGHT, R10 );

    setHeadingFontFromSkin(0);
       cprntEx(box.x + R10*2, box.y+2*R10, z+1, 1, 1, CENTER_Y, "SupergroupPermissionsStr");

    font( &title_9 );

    titleScale = 1.0f;

    // First try and buy enough room by reducing the font
     for(rank = 0; rank < NUM_SG_RANKS_VISIBLE; rank++)
    {
        strncpyt(titleBuf, ranks[rank].name, 20);

          while(titleScale > MIN_TITLE_SCALE && str_wd( &title_9, titleScale, titleScale, titleBuf) > COLUMN_WIDTH-2*R10 )
        {
            titleScale -= 0.025f;
        }
    }
    // Now, try and fit what we have
      for(rank = 0; rank < NUM_SG_RANKS_VISIBLE; rank++)
    {
        int displayColumn = NUM_SG_RANKS_VISIBLE - rank - 1;
        strncpyt(titleBuf, ranks[rank].name, 20);

          while(str_wd( &title_9, titleScale, titleScale, titleBuf) > COLUMN_WIDTH-2*R10 )
        {
            strncpyt(titleBuf+strlen(titleBuf)-4, "...", 4);
        }

           cprnt(box.x + PIX3 + displayColumn * COLUMN_WIDTH + COLUMN_WIDTH / 2 + permissionNameWidth, box.y + PIX3 + ROW_HEIGHT*2, z+1, titleScale, titleScale, titleBuf);
    //    drawVerticalLine(box.x + PIX3 + i * COLUMN_WIDTH + permissionNameWidth,
    //        box.y + PIX3 + ROW_HEIGHT + ROW_HEIGHT / 2, ROW_HEIGHT * usedPermissions, z+1, 1, CLR_GREY);
    }

     font( &game_12 );
     font_color( CLR_WHITE, CLR_WHITE );
     for(i = 0; i < SG_PERM_COUNT; i++)
    {
        if(sgroupPermissions[i].used)
        {
            yLast = box.y + PIX3 + (curPermission+3) * ROW_HEIGHT;

               BuildCBox(&cbox, box.x + R10, yLast- ROW_HEIGHT/2, box.width-2*R10, (ROW_HEIGHT-1));
             if ( mouseCollision( &cbox ))
                    drawStdButtonMeat( box.x+R10-PIX3, yLast-ROW_HEIGHT/2, z, box.width-R10-PIX3, (ROW_HEIGHT-1), 0xffffff22 );

            cprntEx( box.x + permissionNameWidth - str_wd(&game_12, 1.f, 1.f,sgroupPermissions[i].name), yLast+1, z+1, 1, 1, CENTER_Y, sgroupPermissions[i].name);

              for(rank = 0; rank < NUM_SG_RANKS_VISIBLE; rank++)
            {
                int displayColumn = NUM_SG_RANKS_VISIBLE - rank - 1;
                 float sc = 1.2f;
                 float tx = box.x + PIX3 + displayColumn * COLUMN_WIDTH + COLUMN_WIDTH / 2 + permissionNameWidth - empty->width*sc/2;
                int set = TSTB(ranks[rank].permissionInt, i);
                int allowed = srCanISetThisPermission(ranks, playerRank, rank, i);

                BuildCBox(&cbox, tx - ABS(empty->width-glow->width*.5f)*sc/2, yLast-ROW_HEIGHT/2, (glow->width*.5f*sc), ROW_HEIGHT-1 );

                if( mouseCollision(&cbox) && allowed )
                {
                       display_sprite( glow, tx+(empty->width-glow->width*.5f)*sc/2, yLast-glow->height*.5f*sc/2, z+1, .5f*sc, .5f*sc, CLR_WHITE );
                    if( mouseClickHit(&cbox, MS_LEFT) )
                    {
                        if(set)
                            CLRB(ranks[rank].permissionInt, i);
                        else
                            SETB(ranks[rank].permissionInt, i);
                        set = !set;
                    }
                }

                display_sprite( empty, tx, yLast-empty->height*sc/2, z+1, sc, sc, allowed?CLR_WHITE:0xffffff88 );

                 if( set )
                    display_sprite( filled, tx, yLast-empty->height*sc/2, z+1, sc, sc, allowed?CLR_WHITE:0xffffff88 );
            }
            curPermission++;
        }
    }

       drawFrame( PIX2, R4, box.x+R10/2, box.y+2*R10, z, box.width-R10, (curPermission+2)*ROW_HEIGHT, 1.f, 0xffffff22, SELECT_FROM_UISKIN(0x0000ff22, 0xff000022, 0x11115522 ) );

    // ----------
     // base entry permissions

     yLast += ROW_HEIGHT*2;
    uiBoxAlter(&box, UIBAT_SHRINK, UIBAD_RIGHT, 200 );

    setHeadingFontFromSkin(0);
     cprntEx(box.x + R10*2, yLast, z+1, 1, 1, CENTER_Y, "SgBaseEntryPermissionTitle");
    drawFrame( PIX2, R4, box.x+R10/2, yLast, z, box.width-R10, (kSgrpBaseEntryPermission_Count)*ROW_HEIGHT, 1.f, 0xffffff22, SELECT_FROM_UISKIN(0x0000ff22, 0xff000022, 0x11115522 ) );

    font(&game_12);
     font_color( CLR_WHITE, CLR_WHITE );
    
     for( i = 1; i < kSgrpBaseEntryPermission_Count; ++i )  
    {
        char *entryMsg = sgrpbaseentrypermission_ToMenuMsg(i);
        
        yLast += ROW_HEIGHT;

        if( verify(entryMsg ))
         {
            bool canModify = TSTB(ranks[playerRank].permissionInt, SG_PERM_CAN_SET_BASEENTRY_PERMISSION);
            bool isSet = fieldTextBuffer.entryPermission&(1<<i);
            char *pchTmp;

               cprntEx( box.x + permissionNameWidth - str_wd(&game_12, 1.f, 1.f,entryMsg), yLast+1, z+3, 1, 1, CENTER_Y, entryMsg);

             if( isSet )
                pchTmp = textStd("OptionEnabled");
            else
                pchTmp = textStd("OptionDisabled");
            
             if( canModify )
            { 
                  if(selectableText(box.x + box.width/2, yLast+5, z+1, 1.f, pchTmp, CLR_WHITE, CLR_WHITE, FALSE, TRUE))
                {
                    if(isSet)
                        fieldTextBuffer.entryPermission &= ~(1<<i);
                    else
                        fieldTextBuffer.entryPermission |= (1<<i);
                }    

                BuildCBox(&cbox, box.x + R10, yLast- ROW_HEIGHT/2, box.width-2*R10, (ROW_HEIGHT-1));
                if ( mouseCollision( &cbox ))
                    drawStdButtonMeat( box.x+R10-PIX3, yLast-ROW_HEIGHT/2, z, box.width-R10-PIX3, (ROW_HEIGHT-1), 0xffffff22 );

            }
            else
                cprntEx( box.x + box.width/2, yLast+1, z+1, 1, 1, CENTER_Y, pchTmp );
        }
    }

    // ----------
    // draw the buttons
       if(D_MOUSEHIT == drawStdButton(frameBox.x + frameBox.width - (frameBox.width-box.width)/2, yLast - ROW_HEIGHT-10, z, 120, 30, 0x888888ff, "OptionsSetToDefaults", 1.0, 
                                   srGetPlayerRank() != NUM_SG_RANKS_VISIBLE - 1 ? BUTTON_LOCKED : 0)) // only leader gets to reset permissions
     {
        sgroup_setDefaultPermissions(&fieldTextBuffer);
    }


       if(D_MOUSEHIT == drawStdButton(frameBox.x + frameBox.width - (frameBox.width-box.width)/2, yLast + ROW_HEIGHT - 15, z, 120, 30, CLR_GREEN, "AcceptString", 1.0, 0))
    {
        return 1;
     }

    return 0;
}

static bool s_fieldEmpty(SRFields field)
{
    char *str = AINRANGE( field, superRegBlock ) ? uiEditGetUTF8Text(superRegBlock[field]) : NULL;
    return str && str[0];
}

static int s_requiredFields[] = {
    SUPER_NAME, 
    SUPER_LEADER,
    SUPER_COMMANDER,
    SUPER_GENERAL,
    SUPER_CAPTAIN,
    SUPER_LIEUTENANT,
    SUPER_MEMBER,
};

static int srAllRequiredFieldsPresent(void)
{
    bool res = true;
    int i;
    for( i = 0; i < ARRAY_SIZE( s_requiredFields ) && res; ++i ) 
    {
        res = res && s_fieldEmpty(s_requiredFields[i]);
    }
    return res;
}

static float fieldFlashTimers[SUPER_TOTAL_TEXT_FIELDS];
static void srProcessFieldFlashes()
{
    AtlasTex* white = white_tex_atlas;
    int i;

    for(i = 0; i < ARRAY_SIZE(fieldFlashTimers); i++)
    {
        UIBox* box = &tbox[i];
        if(fieldFlashTimers[i])
        { 
            fieldFlashTimers[i] = max(0.f, fieldFlashTimers[i]-TIMESTEP*0.1);
            display_sprite(white, box->x, box->y, 2000, box->width/white->width, box->height/white->height, CLR_CONSTRUCT_EX(255, 0, 0, 0.2*fabs(sin(fieldFlashTimers[i]))));
        }
    }
}

static void srFlashField(SRFields field)
{
    fieldFlashTimers[field] = PI*2;
}

static void srFlashRequiredFields()
{
    int i;
    for( i = 0; i < ARRAY_SIZE( s_requiredFields ); ++i ) 
    {
        srFlashField(s_requiredFields[i]);
    }
}


static int rosterSgStatCompare(const SupergroupStats** stat1, const SupergroupStats** stat2)
{
    const SupergroupStats* s1 = *stat1;
    const SupergroupStats* s2 = *stat2;

    if(s1->rank != s2->rank)
    {
        return s2->rank - s1->rank;
    }
    
    return stricmp(s1->name, s2->name);
}

void showSRScreen()
{
    Entity *e = playerPtr();
    int w, h;
    static int epicker = FALSE, c1picker = FALSE, c2picker = FALSE, textBoxes = FALSE, permpicker = FALSE;
    static int colorSend = 3;

     toggle_3d_game_modes( SHOW_NONE );
    drawBackground( NULL );

     windowClientSizeThisFrame( &w, &h );

    if( !e->supergroup )
    {
        if( srMode == SRM_CREATE )
        {
            int i;
            e->supergroup = createSupergroup();
            msPrintf( menuMessages, SAFESTR(fieldTextBuffer.rankList[5].name), "defaultSuperLeaderTitle" );
            msPrintf( menuMessages, SAFESTR(fieldTextBuffer.rankList[4].name), "defaultLeaderTitle" );
            msPrintf( menuMessages, SAFESTR(fieldTextBuffer.rankList[3].name), "defaultCommanderTitle" );
            msPrintf( menuMessages, SAFESTR(fieldTextBuffer.rankList[2].name), "defaultCaptainTitle" );
            msPrintf( menuMessages, SAFESTR(fieldTextBuffer.rankList[1].name), "defaultLieutenantTitle" );
            msPrintf( menuMessages, SAFESTR(fieldTextBuffer.rankList[0].name), "defaultMemberTitle" );
            sgroup_setDefaultPermissions(&fieldTextBuffer);
            fieldTextBuffer.colors[0] = getColorFromVec3(*(gSuperPalette.color[0]));
            fieldTextBuffer.colors[1] = getColorFromVec3(*(gSuperPalette.color[0]));
            fieldTextBuffer.prestige = 0;
            fieldTextBuffer.demoteTimeout = SGROUP_DEMOTE_MIN;
            
            for( i = 0; i < ARRAY_SIZE( s_FieldSgrpOffsetPairs ); ++i ) 
            {
                char *txt = ((char*)&fieldTextBuffer + s_FieldSgrpOffsetPairs[i].oStr);
                s_SetFieldUIEdit(s_FieldSgrpOffsetPairs[i].field, txt);
            }

            sprintf( demoteText, "%i", fieldTextBuffer.demoteTimeout / SGROUP_DEMOTE_FACTOR );
            s_SetFieldUIEdit(SUPER_DEMOTETIMEOUT, demoteText);

            sprintf( influenceText, "%i", fieldTextBuffer.prestige);
            s_SetFieldUIEdit(SUPER_INFLUENCE, influenceText);
        }
        else
            start_menu( MENU_GAME );
    }

    if(!textBoxes)
    {
        textBoxes = TRUE;
    }

    if (colorSend)
    {
        if (--colorSend == 0)
        {
            requestSGColorData();
        }
    }

    // draw static text
    //drawStaticText();

    drawRegistrationForm();

    // draw text blocks
    handleTextBlocks();

    // draw pickers
    if(epicker)
    {
        int l;
        if(drawEmblemPicker(&gSuperEmblem.currentInfo))
            epicker = FALSE;
        strncpyt(e->supergroup->emblem, gSuperEmblem.info[gSuperEmblem.currentInfo]->texName2,ARRAY_SIZE(e->supergroup->emblem));
        l = strlen(e->supergroup->emblem);
        if (l > 4 && stricmp(&e->supergroup->emblem[l - 4], ".tga") == 0)
        {
            e->supergroup->emblem[l - 4] = 0;
        }
        collisions_off_for_rest_of_frame = 1;
    }
    else if( c1picker )
    {
          if(colorPaletteDraw(&fieldTextBuffer.colors[0]))
            c1picker = FALSE;
        e->supergroup->colors[0] = fieldTextBuffer.colors[0];
        collisions_off_for_rest_of_frame = 1;
    }
    else if( c2picker )
    {
        if(colorPaletteDraw(&fieldTextBuffer.colors[1]))
            c2picker = FALSE;
        e->supergroup->colors[1] = fieldTextBuffer.colors[1];
        collisions_off_for_rest_of_frame = 1;
    }
    else if( permpicker )
    {
        if(permissionEditorDraw())
            permpicker = FALSE;

        collisions_off_for_rest_of_frame = 1;
    }
    //---------------------------
    // Draw supergroup emblem

    if(drawEmblem())
         epicker = TRUE;

    //---------------------------
    // Draw supergroup primary color ring
    {
        UIBox box;
        int selectable = srCanModifyField(srGetPlayerRank(), SUPER_COLOR); 
        CRDMode mode = 0;

        uiBoxDefine(&box, 708, 624, 33, 35);

        if(selectable)
        {
            processMouseOverBox(&box, 50);
            mode |= CRD_SELECTABLE;
        }

        if(colorRingDraw(box.x+box.width/2, box.y+box.height/2, 50, mode, fieldTextBuffer.colors[0], 1.f))
        {
            c1picker = TRUE;
        }
    }

    //---------------------------
    // Draw supergroup secondary color ring
    {
        UIBox box;
        int selectable = srCanModifyField(srGetPlayerRank(), SUPER_COLOR); 
        CRDMode mode = 0;

        uiBoxDefine(&box, 708, 660, 33, 31);

        if(selectable)
        {
            processMouseOverBox(&box, 50);
            mode |= CRD_SELECTABLE;
        }

        if( colorRingDraw(box.x+box.width/2, box.y+box.height/2, 50, mode, fieldTextBuffer.colors[1], 1.f ))
        {
            c2picker = TRUE;
        }
    }

    //---------------------------
    // Draw custom rank box
    {
        UIBox box;
        int selectable = 1;//srCanModifyField(srGetPlayerRank(), SUPER_COLOR); 
        CRDMode mode = 0;

         uiBoxDefine(&box, 575, 402, 202, 36);
        if(selectable)
        {
             font( &profileFont );
            font_color( 0x00000033, 0x00000033 );
            cprntEx( 575 + 202/2, 402 + 36/2, 50, 1.f, 1.f, CENTER_X|CENTER_Y, "ClickToEdit" );
            processMouseOverBox(&box, 50);
            mode |= CRD_SELECTABLE;
        }

        if(uiMouseUpHit( &box, MS_LEFT ) )
        {
            s_SetFieldTextBufFromUIEdit(activeField);    // save the active field.
            permpicker = TRUE;
        }
    }

/*    //---------------------------
    // Draw supergroup roster
    {
        UIBox rosterBox, nameBox, titleBox;
        int i;
        Entity* player = playerPtr();
        int fontHeight;
        AtlasTex* titleIcon[3] = {NULL, atlasLoadTexture("registration_roster_captain.tga"), atlasLoadTexture("registration_roster_leader.tga")};
        float z = 50;
        int oldFontSize;
        int maxMembers = 15;
        float nameSpacing;

        eaQSort(player->sgStats, rosterSgStatCompare);
        font(&profileFont);
        oldFontSize = font_grp->renderParams.renderSize;
         font_grp->renderParams.renderSize = 13;
        fontHeight = ttGetFontHeight(font_grp, 1.0, 1.0);

        uiBoxDefine(&rosterBox, 575, 193, 202, 266);
        uiBoxDefine(&nameBox, rosterBox.x, rosterBox.y, rosterBox.width, fontHeight);
        clipperPush( &rosterBox );

        titleBox = nameBox;
        titleBox.width = 13;
        uiBoxAlter(&nameBox, UIBAT_SHRINK, UIBAD_LEFT, titleBox.width);

        nameSpacing = (rosterBox.height - maxMembers * fontHeight)/(maxMembers-1) - 0.5;
         font_color(CLR_BLACK, CLR_BLACK);
        for(i = 0; i < eaSize(&player->sgStats); i++)
        {
            SupergroupStats* s = player->sgStats[i];
            int rank;

            rank = MINMAX(s->rank, 0, 2);
            if(titleIcon[rank])
            {
                AtlasTex* icon = titleIcon[rank];
                display_sprite(icon, titleBox.x, titleBox.y+(fontHeight-icon->height)/2+3, z, 1.0, 1.0, CLR_BLACK);
            }
            
            cprntEx(nameBox.x, nameBox.y+fontHeight, z, 1.0, 1.0, NO_MSPRINT, s->name);

            titleBox.y += nameBox.height + nameSpacing;
            nameBox.y += nameBox.height + nameSpacing;
        }
        font_grp->renderParams.renderSize = oldFontSize;
        clipperPop();
    }*/

    srProcessFieldFlashes();

    if(!epicker && !c1picker && !c2picker && !permpicker)
    {
        ACButtonResult result = 0;
        ACButton grayedButtons = 0;

        if(!srAllRequiredFieldsPresent())
        {
            grayedButtons |= ACB_ACCEPT;
        }

        result = drawAcceptCancelButton(grayedButtons, 1.f, 1.f);
        if(ACBR_CANCEL & result)
        {
            srReturnToGame();
            textBoxes = FALSE;
            // Set colorSend to 3 so we delay a couple of ticks.  Since the start_menu call appears to take a tick or two to strike, this means
            // we wind up making one last call through here.  I want the color send request to happen at the top of the next go round, not on the 
            // last run through here of this go round.  Thus the delay.
            colorSend = 3;
        }
        else if(ACBR_ACCEPT_LOCKED & result)
        {
            // If the user tried to click on the accept button while some required
            // fields are not filled out, flash those fields.
            srFlashRequiredFields();    
            textBoxes = FALSE;
        }
        else if(ACBR_ACCEPT & result)
        {
            int i;
            for( i = 0; i < ARRAY_SIZE( s_requiredFields ); ++i ) 
            {
                char * txt = uiEditGetUTF8Text(superRegBlock[s_requiredFields[i]]);
                if( IsAnyWordProfane( txt ) )
                {
                     dialogStd( DIALOG_OK, textStd( "InvalidRankName", txt ), NULL, NULL, NULL, NULL, 0 );
                    return;
                }    
            }

            srFieldDeselecting(activeField);
            textBoxes = FALSE;
            colorSend = 3;
            srBeginSGCreate();
        }
    }
}


void superRegMenu(void)
{
    static float timer = 0.f;

    // Is the player here to modify existing supergroup stuff?
    if(srCreationState == SRCS_NONE)
        srCreationState = SRCS_ENTERING_DATA;

    switch(srCreationState)
    {
    case SRCS_ENTERING_DATA:
    case SRCS_WAITING_FOR_RESULT:
        showSRScreen();
        break;
    
    case SRCS_CREATION_SUCCEEDED:
        if( !playerPtr()->sgStats )
        {
            if( timer < 0.f )
            {
                cmdParse( "sgstats" );
                showSRScreen();
                timer += 30;
            }
        }
        else
        {
            if( playerPtr()->supergroup)
            {
                srShowSGCostumeScreen();
            }
            else
                showSRScreen();
        }
        break;

    case SRCS_CREATION_FAILED:
        dialogStd( DIALOG_OK, "SupergroupInvalidName", NULL, NULL, NULL, NULL, 0 );
        srCreationState = SRCS_ENTERING_DATA;
        showSRScreen();
        timer = 0.f;
        break;
    }

    timer -= TIMESTEP/30;

    return;
}


static void s_initEdits()
{
    int rgba[4];

    struct 
    {
        SRFields field;
        int len;
        bool noNewlines;
    } editInits[] = 
        {
            { SUPER_NAME, SG_NAME_LEN, true },
            { SUPER_MOTTO, SG_MOTTO_LEN, true },
            { SUPER_LEADER, SG_TITLE_LEN, true },
            { SUPER_COMMANDER, SG_TITLE_LEN, true },
            { SUPER_GENERAL, SG_TITLE_LEN, true },
            { SUPER_CAPTAIN, SG_TITLE_LEN, true },
            { SUPER_LIEUTENANT, SG_TITLE_LEN, true },
            { SUPER_MEMBER, SG_TITLE_LEN, true },
            { SUPER_MESSAGE, SG_MESSAGE_LEN, false },
            { SUPER_DESCRIPTION, SG_DESCRIPTION_LEN, false},
            { SUPER_INFLUENCE, NUMBER_FIELD_LEN, true },
            { SUPER_DEMOTETIMEOUT, NUMBER_FIELD_LEN, true },
        };
    int i;

    if(!verify(!superRegBlock[0]))
    {
        // already initted
        return;
    }

    GetTextColorForType(INFO_PROFILE_TEXT, rgba);
    
    for( i = 0; i < ARRAY_SIZE( editInits ); ++i ) 
    {
        
        superRegBlock[editInits[i].field] = uiEditCreateSimple(NULL, editInits[i].len , &smfDefault, rgba[0], uiEditDefaultInputHandler );
        uiEditDisallowReturns( superRegBlock[SUPER_NAME], editInits[i].noNewlines );
    }
}


void srEnterRegistrationScreen()
{
    static once = false;
     Entity* e = playerPtr();
    
    if( e->npcIndex )
    {
        dialogStd( DIALOG_OK, "CantChangeSGShapshifted", NULL, NULL, NULL, NULL, 0 );
        return;
    }

    if( !once )
    {
        s_initEdits();
        once = true;
    }

    if(e->supergroup)
    {
        int i;        
        srMode = SRM_MODIFY;
        memcpy(&fieldTextBuffer, e->supergroup, sizeof(Supergroup));
        
        for( i = 0; i < ARRAY_SIZE( s_FieldSgrpOffsetPairs ); ++i ) 
        {
            char *txt = ((char*)&fieldTextBuffer + s_FieldSgrpOffsetPairs[i].oStr);
            s_SetFieldUIEdit(s_FieldSgrpOffsetPairs[i].field, txt);
        }

        sprintf( demoteText, "%i", fieldTextBuffer.demoteTimeout / SGROUP_DEMOTE_FACTOR );
        s_SetFieldUIEdit(SUPER_DEMOTETIMEOUT, demoteText);

        sprintf( influenceText, "%i", fieldTextBuffer.prestige);
        s_SetFieldUIEdit(SUPER_INFLUENCE, influenceText);

        if( e->supergroup )
        {
            int i, count = eaSize(&gSuperEmblem.info);
            for( i = 0; i < count; i++ )
            {
                if( stricmp( gSuperEmblem.info[i]->texName2, e->supergroup->emblem ) == 0 )
                    gSuperEmblem.currentInfo = i;
            }

        }
        srRequestPermissionsSetBit();
    }
    else
    {
        srMode = SRM_CREATE;
        srOverridePermissionsSetBit(1);
    }

    srCreationState = SRCS_NONE;

    srSelectFirstModifiableField(0);

    start_menu(MENU_SUPER_REG);
}

bool creatingSupergroup(void)
{
    if( srMode == SRM_CREATE )
        return true;
    else
        return false;

}
