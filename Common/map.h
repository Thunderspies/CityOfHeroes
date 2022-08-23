#ifndef	_map_h
#define	_map_h

#include "entity/entity.h" // This is needed only for MAX_NAME_LEN, dammit
typedef struct Entity Entity;
					

extern	char	curr_map_name[256];

int missionMap();
void setMapName( char * s );


#endif


