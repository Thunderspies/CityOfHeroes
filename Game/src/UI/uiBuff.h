#ifndef UIBUFF_H
#define UIBUFF_H

#include "comm_game.h"

typedef struct ContextMenu ContextMenu;
extern ContextMenu *gPowerBuffcm;

typedef struct BasePower BasePower;

typedef enum BuffWindow
{
    kBuff_Status,
    kBuff_Group,
    kBuff_Pet,
}BuffWindow;


#define MAX_BUFF_ICONS 50  // no longer a hardcoded array restriction, but lets be pragmatic.

typedef struct ToolTipParent ToolTipParent;
typedef struct AttribDescription AttribDescription;
typedef struct Entity Entity;

float status_drawBuffs( Entity *e, float x, float y, float z, float wd, float ht, float scale, int flip, ToolTipParent *parent );
void linedrawBuffs( BuffWindow window, Entity *e, float x, float y, float z, float wd, float ht, float scale, int flip, ToolTipParent *parent );


void addUpdatePowerBuff(Entity * e, const BasePower *ppow, int iBlink, int uid, AttribDescription **ppDesc, F32 *ppfMag );
void freePowerBuff( void * buff );

const BasePower** basePowersFromPowerBuffs(void ** buff);

void markPowerBuff(Entity *e);
void clearMarkedPowerBuff(Entity *e);
void markBuffForDeletion(Entity *e, int uid);

void setBuffSetting( BuffWindow window, BuffSettings setting );
int isBuffSetting( BuffWindow window, BuffSettings setting );

#endif
