#ifndef SCRIPTUI_H
#define SCRIPTUI_H

#include "scripttypes.h"
#include "storyarc/ScriptUIEnum.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

typedef struct Entity Entity;
typedef struct ScriptEnvironment ScriptEnvironment;

typedef struct ScriptUIVar
{
    char* varName;                    // The name of the var, or value if 
    char* data;                        // The value of the var last lookup
    unsigned int    dirty;            // Marks that this var needs to be updated
} ScriptUIVar;

typedef struct ScriptUIWidget
{
    ScriptUIVar**    varCache;
    ScriptUIType    type;
    char*            name;
    unsigned int    scriptID;        // Script ID this widget belongs to
    unsigned int    uniqueID;        // ID to uniquely identify this widget
    unsigned int    dirty;            // Marks that this widget needs to be updated
    unsigned int    lastTime;        // The last time this widget was updated(timers)
    unsigned int    hidden;            // Marks that this widget is not visible to the player
} ScriptUIWidget;

typedef struct ScriptUIEntInfo
{
    char*                widgetKey;        // ScriptUI Widget hash so the player knows how to reference it
    char**                perPlayerVars;    // Player Specific Vars, maintained on the player rather than the Widget
    ScriptEnvironment*    scriptEnv;        // The script environment from which this came
    int                    id;                // Once the widget is destroyed, we need this to send a kill signal
    unsigned int        dirty;            // Stamp of the last time the widget changed
} ScriptUIEntInfo;

// Wrap this around any parameter of a widget and it means this variable will be taken from the player
// Ex: ScriptUIList("EnemyList", PERPLAYER("EnemyName"), "Most Wanted", "This is your hated enemy");
STRING PERPLAYER(STRING var);

// Wrap this around the widget to make it automatically show to all who enter the map
// Ex: ScriptUIList(ONENTER("EnemyList"), "EnemyName", "Most Wanted", "This is your hated enemy");
STRING ONENTER(STRING widgetName);

// PERPLAYER Widgets must be shown using ScriptUIShow
// Non PERPLAYER Widgets will be automatically shown to all who enter the map
void ScriptUITitle(WIDGET widgetName, STRING titleVar, STRING infoText);
// Meter Tooltips replace the following substrings with the appropriate value
// {MAX} = max value; {MIN} = min value; {VAL} = current value
void ScriptUIMeter(WIDGET widgetName, STRING meterVar, STRING meterName, STRING showValues, STRING min, STRING max, STRING color, STRING color2, STRING tooltip);
void ScriptUITimer(WIDGET widgetName, STRING timerVar, STRING timerText, STRING tooltip);
void ScriptUIList(WIDGET widgetName, STRING tooltip, STRING item, STRING itemColor, STRING value, STRING valueColor);
void ScriptUIDistance(WIDGET widgetName, STRING tooltip, STRING item, STRING itemColor, STRING value, STRING valueColor);
// Specify the number of collect items, where a collect item is "Var", "Text", "Image"
// 2 Collect items would be ScriptUICollectItems("collection", 2, "Item1", "Text1", "Image1", "Item2", "Text2", "Image2"
void ScriptUICollectItems(WIDGET widgetName, NUMBER numCollectItems, STRING itemVar, STRING itemText, STRING itemImage, ...);

// New widgets for zone events
void ScriptUIMeterEx(WIDGET widgetName, STRING meterName, STRING shortText, STRING current, STRING target, STRING color, STRING tooltip, ENTITY entID);
void ScriptUIText(WIDGET widgetName, STRING text, STRING format, STRING tooltip);

// collection can either be a collection, or it can be a single widget
// If it is a collection, all widgets within that collection are shown/hidden from the player
// If it is a widget, just that widget is toggled to be shown/hidden
void ScriptUIShow(COLLECTION collection, TEAM player);
void ScriptUIHide(COLLECTION collection, TEAM player);
void ScriptUIWidgetHidden(COLLECTION collection, int hiddenStatus);

// Takes a list of widgetNames and creates a collection out of them
// This collection can now be hidden/shown and it will apply to all the widgets
// A Collection is prefixed with the string "Collection:"
// So when referencing it with show hide, use Show("Collection:Name");
void ScriptUICreateCollection(COLLECTION collection, int nParams, ...);
// Same as the above, but recveives the params in an array.
void ScriptUICreateCollectionEx(COLLECTION collection, int nParams, const char **params);
// Checks if a Script UI collection exists
int ScriptUICollectionExists(COLLECTION collection);
// Control whether the players UI is detached or not
void ScriptUISendDetachCommand(ENTITY player, int detach);

// The following are utility functions, don't call them directly 
void ScriptUISendUpdate(Entity* player);
void ScriptUIUpkeep();
void ScriptUICleanup();
void ScriptUIPlayerEnteredMap(Entity* player);
void ScriptUIPlayerExitedMap(Entity* player);

// Function moved from ScriptedZoneEvents
void SetUpEntityTrackingForUI(VARIABLE findVar, ENTITY target);

#endif