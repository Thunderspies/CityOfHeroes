#ifndef _POPHELP_H
#define _POPHELP_H

#include <utilitieslib/stdtypes.h>

typedef struct Entity Entity;

typedef struct PopHelpItem
{
    char *name;
    char *text;
    char *tag;
    char *soundName;
    bool inYourFace;
    S64 timeTriggered;
} PopHelpItem;

typedef enum PopHelpState
{
    PopHelpState_Untriggered,
    PopHelpState_Unread,
    PopHelpState_Read,
    PopHelpState_Hidden,
    PopHelpState_Dismissed
} PopHelpState;

PopHelpItem *getPopHelpItem(int event);
int getPopHelpEvent(char *tag);
void loadPopHelp(char *def_filename, char *attribute_filename);
int loadedPopHelp(void);
void reloadPopHelp(void);
char *popHelpTags(void);
PopHelpState getPopHelpState(struct Entity *e, unsigned int n);
PopHelpState getPopHelpStateByTag(struct Entity *e, char *tag);
void setPopHelpState(struct Entity *e, unsigned int n, PopHelpState new_state);

#ifdef SERVER
void triggerPopHelpEventHappenedByTag(Entity *e, const char *tag);
#endif

#endif // _POPHELP_H
