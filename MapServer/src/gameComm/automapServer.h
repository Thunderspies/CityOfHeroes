#ifndef _AUTOMAP_SERVER_H
#define _AUTOMAP_SERVER_H

#define MVC_NEW_MAP (2)
#define MVC_MARKED  (1)

typedef struct Entity Entity;
typedef struct VisitedMap VisitedMap;
VisitedMap *visitedMap_Create();
int mapperVisitCell(Entity *e,int cell_idx);
void mapperSend(Entity *e, int clearFog);
void mapperClearFogForCurrentMap(Entity *e);

#endif
