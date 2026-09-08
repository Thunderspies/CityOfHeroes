#ifndef UIPOWERINVENTORY_H
#define UIPOWERINVENTORY_H

#include "entity/powers.h"

typedef struct TrayObj TrayObj;
// This is the new power inventory window
//
int powersWindow();
int skillsWindow();

TrayObj * power_getVisibleTrayObj( TrayObj * obj, int *tray, int *slot );
char * masterMindHackGetHelpFromCommandName( char * cmd );

extern BasePower newbMastermindMacros[3];
void initnewbMasterMindMacros(void);

#endif