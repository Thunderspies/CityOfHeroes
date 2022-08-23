#include "ScriptUI.h"
#include "entity/entity.h"
#include "svr/svr_base.h"
#include "comm_game.h"
#include "TeamReward.h"
#include "storyarc/storyarcutil.h"
#include "script/scriptengine.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/strings_opt.h>
#include "svr/svr_player.h"
#include <utilitieslib/utils/timing.h>

typedef struct ScriptUICollection
{
    char** widgetKeys;
    char* collectionName;
    int scriptID;
} ScriptUICollection;

extern Entity* EntTeamInternal(TEAM team, int index, int* num);
extern ENTITY EntityNameFromEnt(Entity* e);
extern mSTRING EntVar(STRING entName, STRING var);
extern char* TimerName(const char* timer);

#define SCRIPTUI_UPDATE_CLOCKS    (5)    //number of seconds between sending updates that only affect timers

// ScriptTracker, keeps a list of all ScriptUIs
static StashTable scriptUIList = 0;
static ScriptUICollection** collectionTracker = 0;
static char** widgetTracker = 0;
unsigned int currentUniqueID = 0;

// Converts a var to the string that is supposed to be used on the client
#define VTOS(var)    (var)->data?(var)->data:(var)->varName

// Adds PerEnt: to the begining of the string so that the scriptUI knows it is Ent Specific
STRING PERPLAYER(STRING var)
{
    char buf[256];
    sprintf(buf, "PerEnt:%s", var);
    return StringDupe(buf);
}

// Makes the widget automatically assigned when entering the zone
STRING ONENTER(STRING var)
{
    char buf[256];
    sprintf(buf, "OnEnter:%s", var);
    return StringDupe(buf);
}

// Checks vars to see if they are a special type
#define ISTOKEN(var)                (!strncmp(var, "Token:", 6))
#define ISPERPLAYER(var)            (!strncmp(var, "PerEnt:", 7))
#define ISGLOBAL(var)                (!ISPERPLAYER(var) && !ISTOKEN(var))
#define ISONENTER(widget)            (!strncmp(widget, "OnEnter:", 8))
#define ISCOLLECTION(widget)        (!strncmp(widget, "Collection:", 11))

// Removes any beginning flag and gives the basic name for a widget
STRING BASICNAME(STRING widgetName)
{
    if (ISONENTER(widgetName))
        return widgetName + 8;// <-- strlen("OnEnter:");
    return widgetName;
}

// Converts ui name to a lookup key format "script:scriptID"
static STRING hashKey(STRING uiCollection, int scriptID)
{
    char lookupKey[256];
    sprintf(lookupKey, "%s:%i", BASICNAME(uiCollection), scriptID);
    return StringDupe(lookupKey);
}

static STRING ScriptUIGetGlobalVar(STRING varName)
{
    if (!ISGLOBAL(varName))
        return NULL;
    return GetEnvironmentVar(g_currentScript, varName);    
}

// Gets a var based on what type it is, stores this in data as a global update to all
static STRING ScriptUIGetPlayerVar(ScriptEnvironment* scriptEnv, STRING varName, Entity* player)
{
    if (ISTOKEN(varName))
    {
        RewardToken* token = getRewardToken(player, varName + 6); // <-- strlen("Token:");
        int val = token?token->val:0;
        return NumberToString(val);
    }
    else if (ISPERPLAYER(varName))
    {
        if (player)
            return GetEnvironmentVar(scriptEnv, EntVar(EntityNameFromEnt(player), varName + 7));// <-- strlen("PerEnt:");
        else
            return NULL;
    }
    return NULL;
}

static ScriptUIWidget* ScriptUIHashLookup(STRING key)
{
    if (!scriptUIList)
        scriptUIList = stashTableCreateWithStringKeys(10, StashDeepCopyKeys | StashCaseSensitive );
    return stashFindPointerReturnPointer(scriptUIList, key);
}

// Returns a widget based on a widget name and current script ID
static void* ScriptUIHandle(WIDGET widgetName)
{
    const char* key = hashKey(widgetName, g_currentScript->id);
    return ScriptUIHashLookup(key);
}

// Creates a new widget(if it doesnt exist) based on the name and current script id
static ScriptUIWidget* ScriptUICreate(WIDGET widgetName)
{
    ScriptUIWidget* widget = ScriptUIHandle(widgetName);
    if (!widget)
    {
        widget = calloc(sizeof(ScriptUIWidget), 1);
        widget->name = strdup(BASICNAME(widgetName));
        widget->scriptID = g_currentScript->id;
        stashAddPointer(scriptUIList, hashKey(widgetName, widget->scriptID), widget, false);
        eaPush(&g_currentScript->widgets, widget);
    }
    return widget;
}

// Creates a new ScriptUI Var, saves the lookup and the var name
static ScriptUIVar* ScriptUICreateVar(STRING varName)
{
    ScriptUIVar* newVar = calloc(sizeof(ScriptUIVar), 1);
    newVar->varName = strdup(varName);
    if (ISGLOBAL(varName))
        newVar->data = strdup(ScriptUIGetGlobalVar(varName));
    return newVar;
}

// Cleans up all of the widget's scriptui vars
static void ScriptUIDestroyVars(ScriptUIWidget* widget)
{
    int i, n = eaSize(&widget->varCache);
    for (i = 0; i < n; i++)
    {
        ScriptUIVar* freeme = widget->varCache[i];
        free(freeme->data);
        free(freeme->varName);
    }
    eaDestroy(&widget->varCache);
}

// Destroys a script widget
static void ScriptUIDestroyWidget(ScriptUIWidget* widget)
{
    int i, numWidgets = eaSize(&widgetTracker);
    const char* key = hashKey(widget->name, widget->scriptID);

    // Send a hide message to all players, will be ignored by users without the UI
    ScriptUIHide(widget->name, ALL_PLAYERS);

    // Take out the garbage
    stashRemovePointer(scriptUIList, key, NULL);
    ScriptUIDestroyVars(widget);
    free(widget->name);
    free(widget);
    widget = NULL;

    // Remove it from the tracker list
    for (i = 0; i < numWidgets; i++)
    {
        if (!strcmp(widgetTracker[i], key))
        {
            free(widgetTracker[i]);
            eaRemove(&widgetTracker, i);
            break;
        }
    }
}

// Sends the full ScriptUI Widget to the client, also creates the player Var Cache
static void ScriptUISend(ScriptUIWidget* widget, Entity* player)
{
    int i, numVars = eaSize(&widget->varCache);
    int entInfoIndex = eaSize(&player->scriptUIData)-1;

    START_PACKET(pak, player, SERVER_SCRIPT_UI)
    pktSendBitsPack(pak, 1, widget->uniqueID);
    pktSendBits(pak, 1, 1); // Means create a new widget
    pktSendBitsPack(pak, 1, widget->type);
    pktSendBits(pak, 1, widget->hidden ? 1 : 0);
    pktSendBitsPack(pak, 1, numVars);
    for (i = 0; i < numVars; i++)
    {
        if (ISGLOBAL(widget->varCache[i]->varName))
        {
            pktSendString(pak, saUtilScriptLocalize(VTOS(widget->varCache[i])));
            eaPush(&player->scriptUIData[entInfoIndex]->perPlayerVars, NULL);
        }
        else
        {
            const char* personal = ScriptUIGetPlayerVar(g_currentScript, widget->varCache[i]->varName, player);
            personal = personal?personal:widget->varCache[i]->varName;
            eaPush(&player->scriptUIData[entInfoIndex]->perPlayerVars, strdup(personal));
            pktSendString(pak, saUtilScriptLocalize(personal));
        }
    }
    END_PACKET
}

// Sends a kill signal to the client letting it know to shutdown the widget
static void ScriptUISendKillSignal(Entity* player, int widgetID)
{
    START_PACKET(pak, player, SERVER_SCRIPT_UI)
        pktSendBitsPack(pak, 1, widgetID);
    pktSendBits(pak, 1, 0); // Means kill the widget
    END_PACKET
}

// Attach a script UI to a player
static void ScriptUIAttachEnt(ScriptUIWidget* widget, Entity* player)
{
    const char* widgetKey = hashKey(widget->name, widget->scriptID);
    int i, alreadyHas = 0;

    for (i = 0; i < eaSize(&player->scriptUIData); i++)
        if (!strcmp(widgetKey, player->scriptUIData[i]->widgetKey))
            alreadyHas = 1;

    if (!alreadyHas)
    {
        ScriptUIEntInfo* newInfo = calloc(sizeof(ScriptUIEntInfo), 1);
        newInfo->widgetKey = strdup(widgetKey);
        newInfo->id = widget->uniqueID;
        newInfo->scriptEnv = g_currentScript;
        newInfo->dirty = widget->dirty;
        eaPush(&player->scriptUIData, newInfo);
        ScriptUISend(widget, player);
    }
}

static void ScriptUIRemoveScriptData(Entity* player, int index)
{
    if (player && index >=0 && index < eaSize(&player->scriptUIData))
    {
        int j, numPersonal = eaSize(&player->scriptUIData[index]->perPlayerVars);
        for (j = 0; j < numPersonal; j++)
            free(player->scriptUIData[index]->perPlayerVars[j]);
        ScriptUISendKillSignal(player, player->scriptUIData[index]->id);
        eaDestroy(&player->scriptUIData[index]->perPlayerVars);
        free(player->scriptUIData[index]->widgetKey);
        free(player->scriptUIData[index]);
        eaRemove(&player->scriptUIData, index);
    }
}

// Detach a script UI from a player
static void ScriptUIDetachEnt(ScriptUIWidget* widget, Entity* player)
{
    const char* widgetKey = hashKey(widget->name, widget->scriptID);
    int i, n = eaSize(&player->scriptUIData);

    for (i = n-1; i >= 0; i--)
        if (!strcmp(widgetKey, player->scriptUIData[i]->widgetKey))
            ScriptUIRemoveScriptData(player, i);
}

// Generic function that takes a widget name, type, and list of parameters and creates a new widget
static void ScriptUIInitializeWidget(WIDGET widgetName, ScriptUIType type, int nParams, ...)
{
    va_list    arg;
    ScriptUIWidget* widget = ScriptUICreate(widgetName);
    
    // Reinitialize everything
    ScriptUIDestroyVars(widget);
    widget->type = type;
    widget->dirty = 0;
    widget->uniqueID = ++currentUniqueID;
    widget->hidden = 0;

    // Read in all the parameters and add them to the varCache
    va_start(arg, nParams);
    while (nParams-- > 0)
    {
        char* param = va_arg(arg, char*);
        eaPush(&widget->varCache, ScriptUICreateVar(param));
    }
    va_end(arg);
}

void ScriptUIActivate(WIDGET widgetName)
{
    ScriptUIWidget* widget = ScriptUIHandle(widgetName);
    if (widget && ISONENTER(widgetName))
    {
        int i;
        PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
        for (i = 0; i < players->count; i++)
        {
            Entity* player = players->ents[i];
            if (player)
                ScriptUIAttachEnt(widget, player);
        }
        eaPush(&widgetTracker, strdup(hashKey(widgetName, widget->scriptID)));
    }
}

// Generic function that takes an extra va_list as arguements
// It just appends the arguements in the va_list to the current varCache of the widget
static void ScriptUIInitializeWidgetEx(WIDGET widgetName, ScriptUIType type, int nParams, va_list arg)
{
    ScriptUIWidget* widget = ScriptUIHandle(widgetName);
    if (widget)
    {
        while (nParams-- > 0)
        {
            char* param = va_arg(arg, char*);
            eaPush(&widget->varCache, ScriptUICreateVar(param));
        }
    }
}

// All new ScriptUI Widgets just call the generic ScriptUIInitializeWidget
// They basically define an interface for how to create a title, meter etc..
// The bulk of the work for creating a new script UI needs to be done on the client
void ScriptUITitle(WIDGET widgetName, STRING titleVar, STRING infoText)
{
    ScriptUIInitializeWidget(widgetName, TitleWidget, 2, titleVar, infoText);
    ScriptUIActivate(widgetName);
}

void ScriptUIMeter(WIDGET widgetName, STRING meterVar, STRING meterName, STRING showValues, STRING min, STRING max, STRING color, STRING color2, STRING tooltip)
{
    ScriptUIInitializeWidget(widgetName, MeterWidget, 8, meterVar, meterName, showValues, min, max, color, color2, tooltip);
    ScriptUIActivate(widgetName);
}

void ScriptUICollectItems(WIDGET widgetName, NUMBER numCollectItems, STRING itemVar, STRING itemText, STRING itemImage, ...)
{
    va_list ap;
    ScriptUIInitializeWidget(widgetName, CollectItemsWidget, 3, itemVar, itemText, itemImage);
    va_start(ap, itemImage);
    ScriptUIInitializeWidgetEx(widgetName, CollectItemsWidget, (numCollectItems-1)*3, ap);
    va_end(ap);
    ScriptUIActivate(widgetName);
}

void ScriptUIList(WIDGET widgetName, STRING tooltip, STRING item, STRING itemColor, STRING value, STRING valueColor)
{
    ScriptUIInitializeWidget(widgetName, ListWidget, 5, tooltip, item, itemColor, value, valueColor);
    ScriptUIActivate(widgetName);
}

void ScriptUITimer(WIDGET widgetName, STRING timerVar, STRING timerText, STRING tooltip)
{
    ScriptUIInitializeWidget(widgetName, TimerWidget, 3, timerVar, timerText, tooltip);
    ScriptUIActivate(widgetName);
}

void ScriptUIDistance(WIDGET widgetName, STRING tooltip, STRING item, STRING itemColor, STRING value, STRING valueColor)
{
    ScriptUIInitializeWidget(widgetName, DistanceWidget, 5, tooltip, item, itemColor, value, valueColor);
    ScriptUIActivate(widgetName);
}

// New zone event widgets
void ScriptUIMeterEx(WIDGET widgetName, STRING meterName, STRING shortText, STRING current, STRING target, STRING color, STRING tooltip, ENTITY entID)
{
    ScriptUIInitializeWidget(widgetName, MeterExWidget, 7, meterName, shortText, current, target, color, tooltip, entID);
    ScriptUIActivate(widgetName);
}

void ScriptUIText(WIDGET widgetName, STRING text, STRING format, STRING tooltip)
{
    ScriptUIInitializeWidget(widgetName, TextWidget, 3, text, format, tooltip);
    ScriptUIActivate(widgetName);
}

void ScriptUIShow(COLLECTION collection, TEAM player)
{
    if (ISCOLLECTION(collection))
    {
        ScriptUICollection* coll = ScriptUIHandle(collection);
        if (coll)
        {
            int i, n = eaSize(&coll->widgetKeys);
            for (i = 0; i < n; i++)
            {
                ScriptUIWidget* widget = ScriptUIHashLookup(coll->widgetKeys[i]);
                if (widget)
                {
                    int j, numPlayers;
                    EntTeamInternal(player, -1, &numPlayers);
                    for (j = 0; j < numPlayers; j++)
                    {
                        // DGNOTE 7/21/2010
                        // Changed ScriptUIShow to unilaterally reattach the script UI.  That way it'll always default to attached.
                        // If you want it detached, you'll need to explicitly call ScriptUISendDetachCommand(player, 1);
                        // immediately after the call to here.
                        Entity *e = EntTeamInternal(player, j, NULL);

                        if (e)
                        {
                            ScriptUIAttachEnt(widget, e);
                            START_PACKET(pak, e, SERVER_DETACH_SCRIPT_UI);
                            pktSendBits(pak, 1, 0);
                            END_PACKET;
                        }
                    }
                }
            }
        }
    }
    else if (collection)
    {
        ScriptUIWidget* widget = ScriptUIHandle(collection);
        if (widget)
        {
            int i, numPlayers;
            EntTeamInternal(player, -1, &numPlayers);
            for (i = 0; i < numPlayers; i++)
            {
                Entity *e = EntTeamInternal(player, i, NULL);

                if (e)
                {
                    ScriptUIAttachEnt(widget, e);
                    START_PACKET(pak, e, SERVER_DETACH_SCRIPT_UI);
                    pktSendBits(pak, 1, 0);
                    END_PACKET;
                }
            }
        }
    }
}
void ScriptUIHide(COLLECTION collection, TEAM player)
{
    if (ISCOLLECTION(collection))
    {
        ScriptUICollection* coll = ScriptUIHandle(collection);
        if (coll)
        {
            int i, n = eaSize(&coll->widgetKeys);
            for (i = 0; i < n; i++)
            {
                ScriptUIWidget* widget = ScriptUIHashLookup(coll->widgetKeys[i]);
                if (widget)
                {
                    int j, numPlayers;
                    EntTeamInternal(player, -1, &numPlayers);
                    for (j = 0; j < numPlayers; j++)
                        ScriptUIDetachEnt(widget, EntTeamInternal(player, j, 0));
                }
            }
        }
    }
    else if (collection)
    {
        ScriptUIWidget* widget = ScriptUIHandle(collection);
        if (widget)
        {
            int i, numPlayers;
            EntTeamInternal(player, -1, &numPlayers);
            for (i = 0; i < numPlayers; i++)
                ScriptUIDetachEnt(widget, EntTeamInternal(player, i, 0));
        }
    }
}
void ScriptUIWidgetHidden(WIDGET widgetName, int hiddenStatus)
{
    ScriptUIWidget* widget = ScriptUIHandle(widgetName);
    if (widget && widget->hidden != hiddenStatus)
    {
        widget->hidden = hiddenStatus;
        widget->dirty++;
    }
}
// Takes a list of widgetNames and creates a collection out of them
// This collection can now be hidden/shown and it will apply to all the widgets
void ScriptUICreateCollection(COLLECTION collection, int nParams, ...)
{
    char collName[256];
    va_list    arg;
    ScriptUICollection* newColl = calloc(sizeof(ScriptUICollection), 1);
    sprintf(collName, "Collection:%s", collection);
    newColl->scriptID = g_currentScript->id;
    newColl->collectionName = strdup(collName);

    va_start(arg, nParams);
    while (nParams-- > 0)
    {
        char* widgetName = va_arg(arg, char*);
        eaPush(&newColl->widgetKeys, strdup(hashKey(widgetName, g_currentScript->id)));
    }
    va_end(arg);
    stashAddPointer(scriptUIList, hashKey(collName, g_currentScript->id), newColl,false);
    eaPush(&collectionTracker, newColl);
}

// Takes a list of widgetNames and creates a collection out of them
// This collection can now be hidden/shown and it will apply to all the widgets
void ScriptUICreateCollectionEx(COLLECTION collection, int nParams, const char **params)
{
    char collName[256];
    ScriptUICollection* newColl = calloc(sizeof(ScriptUICollection), 1);
    sprintf(collName, "Collection:%s", collection);
    newColl->scriptID = g_currentScript->id;
    newColl->collectionName = strdup(collName);

    while (nParams-- > 0)
    {
        const char *widgetName = *params++;
        eaPush(&newColl->widgetKeys, strdup(hashKey(widgetName, g_currentScript->id)));
    }
    stashAddPointer(scriptUIList, hashKey(collName, g_currentScript->id), newColl,false);
    eaPush(&collectionTracker, newColl);
}

// Checks if a Script UI collection exists
int ScriptUICollectionExists(COLLECTION collection)
{
    return ISCOLLECTION(collection) && ScriptUIHandle(collection);
}

// Controls whether the player's script UI is detached
void ScriptUISendDetachCommand(TEAM team, int detach)
{
    int i;
    int numPlayers;
    Entity *e;

    // DGNOTE 7/21/2010
    // Should have done this when I first wrote this: it now takes a TEAM rather than a PLAYER
    // and applies the setting to the whole TEAM.
    EntTeamInternal(team, -1, &numPlayers);
    for (i = 0; i < numPlayers; i++)
    {
        e = EntTeamInternal(team, i, NULL);
        if (e)
        {
            START_PACKET(pak, e, SERVER_DETACH_SCRIPT_UI);
            pktSendBits(pak, 1, !!detach);
            END_PACKET;
        }
    }
}

// Goes through all the vars of the current script's widgets and makes sure they are up to date
void ScriptUIUpkeep()
{
    int i, j, n = eaSize(&g_currentScript->widgets);
    for (i = 0; i < n; i++)
    {
        int updated = 0;
        ScriptUIWidget* widget = g_currentScript->widgets[i];
        for (j = 0; j < eaSize(&widget->varCache); j++)
        {
            ScriptUIVar* suiVar = widget->varCache[j];
            const char* latest = ScriptUIGetGlobalVar(suiVar->varName);
            if (latest && (!suiVar->data || strcmp(latest, suiVar->data)))
            {
                free(suiVar->data);
                suiVar->data = strdup(latest);
                suiVar->dirty = widget->dirty+1;
                widget->lastTime = timerSecondsSince2000();
                updated = 1;
            }
        }
        if (updated)
            widget->dirty++;
    }
}

static void ScriptUIDestroyCollection(ScriptUICollection *collection)
{
    int i, numWidgets = eaSize(&collection->widgetKeys);
    for (i = 0; i < numWidgets; i++)
        free(collection->widgetKeys[i]);
    eaDestroy(&collection->widgetKeys);

    stashRemovePointer(scriptUIList, hashKey(collection->collectionName, collection->scriptID), NULL);
    free(collection->collectionName);
    free(collection);
}
// Cleans up all scriptUIs of the current script
void ScriptUICleanup()
{
    int i, n = eaSize(&g_currentScript->widgets);
    for (i = 0; i < n; i++)
    {
        ScriptUIWidget* widget = g_currentScript->widgets[i];
        ScriptUIDestroyWidget(widget);
    }
    eaDestroy(&g_currentScript->widgets);
    n = eaSize(&collectionTracker);
    for (i = n-1; i >= 0; i--)
    {
        ScriptUICollection* collection = collectionTracker[i];
        if (collection->scriptID == g_currentScript->id)
        {
            ScriptUIDestroyCollection(collection);
            eaRemove(&collectionTracker, i);
        }
    }
}

// Clear out Destroyed Widgets and alert player if he needs any updates
int ScriptUIPrepEntUpdate(Entity* player)
{
    int ret = 0;
    int i, n = eaSize(&player->scriptUIData);
    for (i = n-1; i >= 0; i--)
    {
        ScriptUIWidget* widget = ScriptUIHashLookup(player->scriptUIData[i]->widgetKey);
        if (!widget)
            ScriptUIRemoveScriptData(player, i);
        else if (widget->dirty > player->scriptUIData[i]->dirty)
            ret = 1;
        else
        {    
            int j, numVars = eaSize(&widget->varCache);
            int updated = 0;
            for (j = 0; j < numVars; j++)
            {
                if (player->scriptUIData[i]->perPlayerVars[j])
                {
                    const char* latest = ScriptUIGetPlayerVar(player->scriptUIData[i]->scriptEnv, widget->varCache[j]->varName, player);
                    widget->varCache[j]->dirty = 0;
                    if (latest && strcmp(latest, player->scriptUIData[i]->perPlayerVars[j]))
                    {
                        free(player->scriptUIData[i]->perPlayerVars[j]);
                        player->scriptUIData[i]->perPlayerVars[j] = strdup(latest);
                        widget->varCache[j]->dirty = widget->dirty+1;
                        updated = ret = 1;
                    }
                }
            }
            if (updated)
                widget->dirty++;
        }
    }
    return ret;
}

void ScriptUISendUpdate(Entity* player)
{
    if (ScriptUIPrepEntUpdate(player))
    {
        int i, j, n = eaSize(&player->scriptUIData);
        START_PACKET(pak, player, SERVER_SCRIPT_UI_UPDATE)
        for (i = 0; i < n; i++)
        {
            ScriptUIWidget* widget = ScriptUIHashLookup(player->scriptUIData[i]->widgetKey);
            if (player->scriptUIData[i]->dirty < widget->dirty)
            {
                pktSendBits(pak, 1, 1);
                pktSendBitsPack(pak, 1, widget->uniqueID);
                pktSendBits(pak, 1, widget->hidden ? 1 : 0);
                
                for (j = 0; j < eaSize(&widget->varCache); j++)
                {
                    if (player->scriptUIData[i]->dirty < widget->varCache[j]->dirty)
                    {
                        pktSendBits(pak, 1, 1);
                        pktSendBitsPack(pak, 1, j);
                        if (player->scriptUIData[i]->perPlayerVars[j])
                            pktSendString(pak, saUtilScriptLocalize(player->scriptUIData[i]->perPlayerVars[j]));
                        else
                            pktSendString(pak, saUtilScriptLocalize(VTOS(widget->varCache[j])));
                    }
                }
                pktSendBits(pak, 1, 0);
                player->scriptUIData[i]->dirty = widget->dirty;
            }
        }
        pktSendBits(pak, 1, 0);
        END_PACKET
    }
}

// Attachs player to all global running scriptUIs
// Widget tracker only keeps track of OnEnter widgets
void ScriptUIPlayerEnteredMap(Entity* player)
{
    int i;
    for (i = 0; i < eaSize(&widgetTracker); i++)
    {
        ScriptUIWidget* widget = ScriptUIHashLookup(widgetTracker[i]);
        ScriptUIAttachEnt(widget, player);
    }
}

// Removes player from all scriptUIs
void ScriptUIPlayerExitedMap(Entity* player)
{
    int i, n = eaSize(&player->scriptUIData);
    for (i = n-1; i >= 0; i--)
    {
        ScriptUISendKillSignal(player, player->scriptUIData[i]->id);
        free(player->scriptUIData[i]->widgetKey);
        eaRemove(&player->scriptUIData, i);
    }
    eaDestroy(&player->scriptUIData);
}

void SetUpEntityTrackingForUI(VARIABLE findVar, ENTITY target)
{
    if (target)
    {
        Entity *e = EntTeamInternal(target, 0, NULL);
        if (e && e->owner)
        {
            VarSetNumber(findVar, e->owner);
            return;
        }
    }
    VarSetNumber(findVar, 0);
}