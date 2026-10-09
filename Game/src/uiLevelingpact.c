#include "UI/uiCursor.h"
#include "UI/uiTarget.h"
#include "uiLevelingpact.h"
#include "UI/uiChat.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "entity/entVarUpdate.h"
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include "storyarc/contactClient.h"
#include "entity/entity.h"
#include "entity/entity_enum.h"
#include "entity/teamCommon.h"
#include "entity/entPlayer.h"
#include "entity/character_level.h"
#include "comm_game.h"
#include "UI/uiContextMenu.h"
#include "UI/sprite/sprite_base.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include "graphics/ttFont.h"
#include "graphics/ttFontUtil.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_font.h"
#include "formatter/smf_main.h"
#include "UI/uiSMFView.h"
#include "UI/uiDialog.h"
#include "UI/uiFriend.h"




int levelingpact_IsInPact(void *foo)
{
    Entity *e = playerPtr();
    return (SAFE_MEMBER(e, levelingpact_id))?CM_AVAILABLE:CM_HIDE;
}

void levelingpact_openWindow(void *notused)
{
    selectChannelWindow(textStd("LevelingpactTab"));
    window_setMode( WDW_FRIENDS, WINDOW_GROWING ); 
}

void levelingpact_quitPact(void *notused)
{
    if(!strcmp(dialogGetTextEntry(),  SAFE_MEMBER(playerPtr(), namePtr)))
        cmdParse( "unlevelingpact_real" );
    else
        dialog(DIALOG_YES_NO, -1, -1, -1, -1, textStd("LevelingPactLeaveFailure",dialogGetTextEntry(),SAFE_MEMBER(playerPtr(), namePtr) ),
        NULL, levelingpact_quitWindow, NULL, NULL, 
        DLGFLAG_GAME_ONLY, NULL, NULL, 0, 0, 0, 0 );
}
void levelingpact_quitWindow(void *notused)
{
    dialog(DIALOG_OK_CANCEL_TEXT_ENTRY, -1, -1, -1, -1, textStd("LevelingPactLeaveWarning",LEVELINGPACT_MAXLEVEL,SAFE_MEMBER(playerPtr(), namePtr) ),
        NULL, levelingpact_quitPact, NULL, NULL, 
        DLGFLAG_GAME_ONLY, NULL, NULL, 0, 0, 256, 0 );
}



