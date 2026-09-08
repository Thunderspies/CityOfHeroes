#ifndef TRAY_CLIENT_H
#define TRAY_CLIENT_H

#include <utilitieslib/stdtypes.h>
#include "UI/uiInclude.h"
#include "gameComm/trayCommon.h"

typedef struct Packet Packet;
typedef struct Entity Entity;
typedef struct Power Power;

extern U32 g_timeLastPower;
extern U32 g_timeLastAttack;

extern F32 gfTargetConfusion;

// TrayObj Functions
//------------------------------------------------------------------------------------------
void entity_CancelQueuedPower(Entity *e);
void entity_CancelWindupPower(Entity *e);

void macro_create( char *shortName, char *command, char *icon, int slot );

void trayobj_copy( TrayObj *dest, TrayObj *src, int update_server );
void trayobj_startDraggingEx(TrayObj *to, AtlasTex *icon, AtlasTex * overlay, float icon_scale);
void trayobj_startDragging(TrayObj *to, AtlasTex *icon, AtlasTex * overlay);
void trayobj_stopDragging();
int trayobj_IsDefault(TrayObj *pto);
void trayobj_SetDefaultPower(TrayObj *pto, Entity *e);
void trayobj_ExitStance(Entity *e);
void tray_dragPower( int i, int j, AtlasTex * icon );

void tray_HandleWorldCursor(Entity *e, Mat4 matCursor, Vec3 probe);
void tray_SetWorldCursorRange(void);

void tray_CancelQueuedPower(void);
void tray_AddObj( TrayObj *to );
void tray_AddObjTryAt(TrayObj *to, int trayIndex); // zero-based
void tray_init(void);

const char *traycm_PowerText( void *data );
const char *traycm_PowerInfo( void *data );
void traycm_Add( void *data );
void traycm_Remove( void * data );
void traycm_Info( void * data );
int traycm_InfoAvailable( void * data );
int traycm_showExtra(void *data);
const char *traycm_PowerExtra(void *data);
int traycm_DeletableTempPower( void * data );
void traycm_DeletePower( void * data );

void trayInfo_GenericContext( CBox *box, TrayObj * to);


void trayslot_select(Entity *e, TrayCategory trayCategory, int trayNumber, int slotNumber);
bool trayslot_istype( Entity *e, int slot_num, int tray_num, TrayItemType type);
void trayslot_drawIcon( TrayObj * ts, float xp, float yp, float zp, float scale, int inventory );

TrayObj *trayslot_find( Entity *e, const char *pch );
void trayslot_findAndSelect( Entity *e, const char *pch, int toggleonoff );
void trayslot_findAndSelectLocation( Entity *e, const char *locname, const char *pch );
void trayslot_findAndSetDefaultPower( Entity *e, const char *pch );

// Figure out the right tray to refer to if the Razer tray is open.
CurTrayType tray_razer_adjust( CurTrayType tray );

void tray_change( int alt, int forward );
void tray_goto( int tray );
void tray_goto_tray( int curtray, int traynum );

void tray_setMode(int mode);
void tray_momentary_alt_toggle( CurTrayMask mask, int toggle );
void tray_toggleMode();
void tray_setSticky( CurTrayType tray, int toggle );
void tray_toggleSticky( CurTrayType tray );

void trayslot_Timer( Entity *e, TrayObj * to, float * sc );
void trayslot_RemainingTime( Entity *e, TrayObj * to, char * remainingTime );
int trayslot_IsActive( Entity *e,  TrayObj * to );
void tray_Clear(void);

void clearServerTrayPriorities(void);
void serverTray_AddPower(Entity *e, Power *ppow, int priority);
void serverTray_AlterPriority(Entity *e, Power *ppow, int priority);
void serverTray_RemovePower(Entity *e, Power *ppow);
void serverTray_Rebuild();

int trayWindow();
int trayWindowSingle();
void trayslot_Display( Entity *e, int i, int current_tray, int curTrayType, float x, float y, float z, float scale, int color, int window );

extern int gMacroOnstack;

#define BOX_SIDE    40.f
#define TRAY_YOFF    5
#define DEFAULT_TRAY_HT     50
#define ARROW_OFFSET 7

CurTrayType get_cur_tray( Entity *e );
extern TrayObj *s_ptsActivated;

#endif
