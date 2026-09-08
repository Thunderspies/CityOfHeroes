#ifndef AUTOCOMMANDS_H_
#define AUTOCOMMANDS_H_

#include "container/dbcontainerpack.h"

typedef struct ContainerInfo ContainerInfo;
typedef struct Entity Entity;
typedef struct ClientLink ClientLink;

#define AUTO_COMMAND_STRING_LENGTH 256

typedef struct AutoCommand
{
    int id;
    int executionTime;
    char command[AUTO_COMMAND_STRING_LENGTH];
    char creator[32]; // length from Entity::auth_name
    int creationTime;
} AutoCommand;

extern StructDesc autocommand_desc[];

void containerHandleAutoCommand(ContainerInfo *ci);
void autoCommand_add(Entity *creator, char *commandName, int days, int hours, int minutes);
void autoCommand_delete(Entity *deleter, int id);
void autoCommand_showAll(Entity *user);
void autoCommand_showByCommand(Entity *user, char *commandName);
void autoCommand_run(ClientLink *link, Entity *e, int testRun);

#endif //AUTOCOMMANDS_H_