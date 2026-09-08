#ifndef UIREGISTER_H
#define UIREGISTER_H

#include "entity/entity.h"
extern int gWaitingToEnterGame;
extern int gLoggingIn;

void resetRegisterMenu();
void registerMenuEnterCode();
void registerMenuExitCode();
void registerMenu();
void uiRegisterCopyToEntity(Entity* e, int copyName);

#endif