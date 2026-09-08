#ifndef _EDIT_LIBRARY_H
#define _EDIT_LIBRARY_H

typedef struct PropertyDefList PropertyDefList;
typedef struct MenuEntry MenuEntry;
typedef struct ClickInfo ClickInfo;
void libraryMenuClickFunc(MenuEntry * entry, ClickInfo * click);

extern PropertyDefList g_propertyDefList;

int cmdScrollCallback(char *name,int idx);
void libUpdateList();
int libScrollCallback(char *name,int idx);
int trackerScrollCallback(char *name,int idx);
void trackerUpdateList(int ref_id,DefTracker *tracker,int depth);
void propertyUpdateList();
int propNameScrollCallback(char* oldPropName,int idx);
int propValueScrollCallback(char* oldValue,int idx);
void editSetTrackerOffset(DefTracker *tracker);
int editShowLibrary(int lost_focus);
void commandMenuUpdateRecentMaps(char * name);
int promptUserForString(const char *oldString, char *newString);
int editRemoveProperty(StashTable properties, char* propName);
int editChangePropertyName(StashTable properties, char* oldPropName, int * deleteProperty );
void loadObject(char * name);
void libOpenTo(DefTracker *tracker);
#endif
