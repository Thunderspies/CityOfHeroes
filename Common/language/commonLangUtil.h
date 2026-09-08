
#ifndef COMMONLANGUTIL_H
#define COMMONLANGUTIL_H

#include <utilitieslib/utils/structDefines.h>

typedef struct Entity            Entity;
typedef struct MessageStore        MessageStore;

extern StaticDefineInt ParseGender[];
extern StaticDefineInt ParseStatus[];
extern StaticDefineInt ParsePlayerType[];
extern StaticDefineInt ParsePlayerSubType[];
extern StaticDefineInt ParseAlignment[];
extern StaticDefineInt ParsePraetorianProgress[];
extern StaticDefineInt ParseTeamNames[];
extern StaticDefineInt ParseBodyType[];

char* printLocalizedEnt(const char *msg, ...);
char* printLocalizedEntFromEntLocale(const char *msg, ...);

void installCustomMessageStoreHandlers(MessageStore* store);

#endif
