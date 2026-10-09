#ifndef _CONTAINERLOADSAVE_H
#define _CONTAINERLOADSAVE_H

typedef struct Entity Entity;
typedef struct ClientLink ClientLink;
typedef struct StructDesc StructDesc;
typedef struct StuffBuff StuffBuff;
typedef struct KarmaEventHistory KarmaEventHistory;
typedef enum PetitionCategory PetitionCategory; // not really a legal typedef
typedef void (*AttributeNamesCallback)(StuffBuff *sb);

char *entTemplate();
char *packageEnt(Entity *e);
char *packageEntAll(Entity *e);
void unpackEnt( Entity *e, char *buff );
char *teamupTemplate();
char *packageTeamup(Entity *e);
void unpackTeamup(Entity *e,char *mem);
void unpackLeague(Entity *e,char *mem);
char *packageEventHistory(KarmaEventHistory *event);
KarmaEventHistory *unpackEventHistory(char *mem);
char *taskForceTemplate();
char *packageTaskForce(Entity *ent);
void unpackTaskForce(Entity *e,char *mem,int send_to_client);
void unpackMapTaskForce(int dbid, char *mem);
void containerWriteTemplates(char *dir);
char *packagePetition(Entity *e, PetitionCategory cat, const char *summary, const char *msg);

void AddAttributeNamesCallback(AttributeNamesCallback newCallback);

void morphCharacter(ClientLink* client, Entity *e, char *at, char *pri_power, char *sec_power, int level);
void fixSuperGroupFields(Entity* e);
void fixSuperGroupMode(Entity* e);
StructDesc *getStructDesc(int type, char* table);
#endif
