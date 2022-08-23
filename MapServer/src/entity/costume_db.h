

#ifndef COSTUME_DB_H
#define COSTUME_DB_H

#include "entity/power_customization.h"
#include "entity/costume.h"
#include "entity/entPlayer.h"

typedef struct DBCostume
{
    CostumePart part[MAX_COSTUMES*MAX_COSTUME_PARTS];
}DBCostume;

typedef struct DBAppearance
{
    Appearance appearance[MAX_COSTUMES];
}DBAppearance;

typedef struct DBPowerCustomizationList
{
    DBPowerCustomization powerCustomization[MAX_COSTUMES * MAX_POWERS];
}DBPowerCustomizationList;

void packageCostumes( Entity * e, StuffBuff *psb, StructDesc *desc);
void unpackCostumes( Entity * e, DBCostume *dbcostumes);

void packageAppearances( Entity *e, StuffBuff *psb, StructDesc *desc );
void unpackAppearance( Entity *e, DBAppearance * dba );

void packagePowerCustomizations( Entity *e, StuffBuff *psb, StructDesc *desc );
void unpackPowerCustomizations( Entity *e, DBPowerCustomizationList * dbpc );

#endif
