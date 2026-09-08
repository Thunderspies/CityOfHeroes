/*\
 *
 *    scriptengine.h/c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides the execution environment for CoH scripts
 *
 */

#include "scriptengine.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/error.h>
#include "storyarc/storyarcprivate.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/staticMapInfo.h"
#include "container/mapgroup.h"
#include "entity/entity.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "dbcomm/logcomm.h"

int    g_scriptCombatLevel = 0;
int    g_scriptMinCombatLevel = 0;
int    g_scriptMaxCombatLevel = 0;
int    g_scriptNPCsScared = 0;
ScriptEnvironment** g_runningScripts = 0;
ScriptEnvironment* g_currentScript = 0;
U32 g_currentTime = 0;
U32 g_currentKarmaTime = 0;        //    this is the most recent value of dbSecondsSince2000 that the script set
U32 g_nextScriptId = 1;        // TODO - make a real reference system?  Currently we rely on this not overflowing
                            // before server quits.
StashTable g_instanceVars = NULL;

TokenizerParseInfo ParseScriptDef[] = {
    { "{",                TOK_START,        0},
    { "",                TOK_CURRENTFILE(ScriptDef, filename) },
    { "ScriptName",        TOK_STRING(ScriptDef, scriptname,0) },
    { "}",                TOK_END,            0},
    SCRIPTVARS_STD_PARSE(ScriptDef)
    { "", 0, 0 }
};

typedef struct ScriptDefList
{
    const ScriptDef** scripts;
} ScriptDefList;

TokenizerParseInfo ParseScriptDefList[] = {
    { "ScriptDef",        TOK_STRUCT(ScriptDefList,scripts,ParseScriptDef) },
    { "", 0, 0 }
};

SHARED_MEMORY ScriptDefList g_scriptdeflist = {0};

// *********************************************************************************
//  Script defs
// *********************************************************************************

// Search for a recently reloaded contact def that still needs to be preprocessed
// Different search because the def has not yet been processed
static ScriptDef* ScriptDefFindUnprocessed(const char* relpath)
{
    char cleanedName[MAX_PATH];
    int i, n = eaSize(&g_scriptdeflist.scripts);
    strcpy(cleanedName, strstri((char*)relpath, SCRIPTS_DIR));
    forwardSlashes(_strupr(cleanedName));
    for (i = 0; i < n; i++)
        if (!stricmp(cleanedName, g_scriptdeflist.scripts[i]->filename))
            return (ScriptDef*)g_scriptdeflist.scripts[i];
    return NULL;
}

static void ScriptRestartActiveScripts(ScriptDef* script)
{
    int i, n = eaSize(&g_runningScripts);
    int numScriptsStopped = 0;
    for (i = n-1; i >= 0; i--)
        if (!stricmp(g_runningScripts[i]->function->name, script->filename))
            ScriptReset(g_runningScripts[i]->id);
}

// Reloads the latest version of a def, finds it, then processes
// MAKE SURE to do all the pre/post/etc stuff here that gets done during regular loading
static void ScriptDefReloadCallback(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if (ParserReloadFile(relpath, ParseScriptDefList, sizeof(ScriptDefList), (void*)&g_scriptdeflist, NULL, NULL))
    {
        ScriptDef* script = ScriptDefFindUnprocessed(relpath);
        saUtilPreprocessScripts(ParseScriptDef, script);
        ScriptRestartActiveScripts(script);
    }
    else
    {
        Errorf("Error reloading scriptdef: %s\n", relpath);
    }
}

static void ScriptDefSetupCallbacks()
{
    if (!isDevelopmentMode())
        return;
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "scripts.loc/*.scriptdef", ScriptDefReloadCallback);
}

void ScriptDefLoad(void)
{
    int flags = PARSER_SERVERONLY;
    if (g_scriptdeflist.scripts) // Reload!
        flags |= PARSER_FORCEREBUILD;

    ParserUnload(ParseScriptDefList, &g_scriptdeflist, sizeof(g_scriptdeflist));

    loadstart_printf("Loading script definitions.. ");
    if (!ParserLoadFilesShared("SM_scriptdefs.bin", SCRIPTS_DIR, ".scriptdef", "scriptdefs.bin",
        flags, ParseScriptDefList, &g_scriptdeflist, sizeof(g_scriptdeflist), NULL, NULL, saUtilPreprocessScripts, NULL, NULL))
    {
        log_printf("scripterr", "Error loading script definitions\n");
        printf("ScriptDefLoad: Error loading script definitions\n");
    }
    ScriptDefSetupCallbacks();
    loadend_printf("%i script definitions", eaSize(&g_scriptdeflist.scripts));
}

const ScriptDef* ScriptDefFromFilename(const char* filename)
{
    char buf[MAX_PATH];
    int i, n;

    if (!filename) return NULL;

    saUtilCleanFileName(buf, filename);
    n = eaSize(&g_scriptdeflist.scripts);
    for (i = 0; i < n; i++)
    {
        if (g_scriptdeflist.scripts[i]->filename && !stricmp(g_scriptdeflist.scripts[i]->filename, buf))
        {
            return g_scriptdeflist.scripts[i];
        }
    }
    return NULL;
}

U32 ScriptBeginDef(const ScriptDef* def, int type, void* user, const char* errname, const char *dataFilename)
{
    ScriptVarsTable vars = {0};
    ScriptFunction* func = ScriptFindFunction(def->scriptname);
    if (!func) {
        ScriptError("Unable to find ScriptDef (%s)\n", def->scriptname);
        return 0;
    }

    if (func->type != type)
    {
        ScriptError("ScriptDef started as wrong script type (%i)\n", type);
        return 0;
    }
    ScriptVarsTablePushScope(&vars, &def->vs);
    return ScriptBegin(def->scriptname, &vars, type, user, errname, dataFilename);
}

// *********************************************************************************
//  Script Environment
// *********************************************************************************

MP_DEFINE(ScriptEnvironment);
ScriptEnvironment* ScriptEnvironmentAlloc(const char *dataFilename)
{
    ScriptEnvironment *script;

    MP_CREATE(ScriptEnvironment, 10);
    
    script = MP_ALLOC(ScriptEnvironment);
    estrPrintCharString(&script->pchSourceFile, dataFilename);

    return script;
}
void ScriptEnvironmentFree(ScriptEnvironment* script)
{
    estrDestroy(&script->pchSourceFile);
    MP_FREE(ScriptEnvironment, script);
}

void ScriptError(char* s, ...)
{
    int flag = isDevelopmentMode()?LOG_CONSOLE_ALWAYS:0;
    va_list arg;

    va_start(arg, s);
    log_va( LOG_ERROR, LOG_LEVEL_VERBOSE, flag, s, arg );
    va_end(arg);
}

// kind of a hack, but if I hit the SE_DYNAMIC_LOOKUP sentinal value, I need to look
// up a different value each time from the initialvars table

// don't try to free our sentinel
static void seFree(char* value)
{
    if (value && value != SE_DYNAMIC_LOOKUP)
        free(value);
}

///////////////////////////////////////////////////////////////////////////////////////////

void SetEnvironmentVar(ScriptEnvironment* script, const char* key, const char* value)
{
    char* prev;

    stashRemovePointer(script->vars, key, &prev);
    if (prev)
        seFree(prev);
    if (value == SE_DYNAMIC_LOOKUP)
        stashAddPointer(script->vars, key, SE_DYNAMIC_LOOKUP, false);
    else if (value) 
        stashAddPointer(script->vars, key, strdup(value), false);
}

const char* GetEnvironmentVar(ScriptEnvironment* script, const char* key)
{
    char* result = stashFindPointerReturnPointer(script->vars, key);
    if(result == NULL)
    {
        // Attempt to look up Var from the vars table
        // This is a pass-through for scriptLua scripts which do not register their vars at InitEnvironmentVars
        const char *lookup = ScriptVarsTableLookup(&script->initialvars, key);
        if(lookup == key)
            return NULL;
        else
            return lookup;
    }
    else if (result == SE_DYNAMIC_LOOKUP)
    {
        // Do not reseed the table if the player is on a mission map.  This fixes problems with 
        //    scripts on newspaper/police band missions not using the correct seed.
        if (!OnMissionMap())
            ScriptVarsTableSetSeed(&script->initialvars, rule30Rand());
        return ScriptVarsTableLookup(&script->initialvars, key);
    }
    else
        return result;
}

void RemoveEnvironmentVar(ScriptEnvironment* script, char* key)
{
    stashRemovePointer(script->vars, key, NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////

void SetInstanceVar(const char* key, const char* value)
{
    char* prev;

    stashRemovePointer(g_instanceVars, key, &prev);
    if (prev) 
        free(prev);

    if (value) 
        stashAddPointer(g_instanceVars, key, strdup(value), false);
}

const char* GetInstanceVar(const char* key)
{
    char* result = stashFindPointerReturnPointer(g_instanceVars, key);
    return result;
}

void RemoveInstanceVar(const char* key)
{
    char* prev;

    stashRemovePointer(g_instanceVars, key, &prev);

    if (prev) 
        free(prev);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// notify everybody else that the script is stopping..
void NotifyScriptStopping(ScriptEnvironment* script)
{
    switch (script->function->type)
    {
    case SCRIPT_ENCOUNTER:
        if (script->encounter)
            EncounterNotifyScriptStopping(script->encounter, script->id);
        break;
    case SCRIPT_LOCATION:
        if (script->location)
            LocationNotifyScriptStopping(script->location, script->id);
    }
}

static void seFreeAdapter(void* arg0)
{
    seFree((char*)arg0);
}

void ScriptEnvironmentDestroy(ScriptEnvironment* script)
{
    EncounterGroup* group = script->encounter;

    //Check to see if there was a time when the encounter would have closed but for the script's control
    if( group )
    {
        if( group->scriptControlsCompletion && group->active ) //is this check redundant?
        {
            if ( eaSize( &group->active->ents ) == 0)
                EncounterClose( group );
        }
        group->scriptControlsCompletion = 0; //give up control od this encounter
    }

    //Temp hack till we get global script vars done right
    {
        extern int gMinerStrikeRunning;
        extern void MinerStrike(void);
        if( 0 == stricmp( script->function->name, "MinerStrike" ) )
            gMinerStrikeRunning--;
    }

    if(script->lua_interpreter)
    {
        /* cleanup Lua */
        lua_close(script->lua_interpreter);
        script->lua_interpreter = NULL;
    }

    ClearEncounterGroupOverRides();
    NotifyScriptStopping(script);
    ScriptTeamDestroyAll(script);
    EncounterStopReservingAll(script);
    EncounterReenableManualSpawnAll(script); //should come after StopReservingAll
    stashTableClearEx(script->vars, NULL, seFreeAdapter);
    stashTableDestroy(script->vars);
    stashTableDestroy(script->varsFlags);
    ScriptVarsTableClear(&script->initialvars);
    ScriptEnvironmentFree(script);
}

// *********************************************************************************
//  Begin execution functions
// *********************************************************************************

// we're just using a hash table for the local vars.  It gets initialized with parameters
// from the vars table.  We also keep a copy of the vars table we were passed, 
// although we can only copy the scopes correctly.
static int InitEnvironmentVars(const char* errname, ScriptEnvironment* script, ScriptFunction* function, ScriptVarsTable* varsparam)
{
    ScriptVarsTable* vars = &script->initialvars;
    int i;
    int ret = 1;

    if (varsparam)
    {
        // changed from ScriptVarsTableCopy because this wasn't using or clearing its references and strings properly
        ScriptVarsTableScopeCopy(vars, varsparam);
    }
    script->vars = stashTableCreateWithStringKeys(50, StashDeepCopyKeys);
    script->varsFlags = stashTableCreateWithStringKeys(50, StashDeepCopyKeys);

    for (i = 0; i < MAX_SCRIPT_PARAMS && (function->params[i].param && function->params[i].param[0]); i++)
    {
        const char* result = ScriptVarsTableLookup(vars, function->params[i].param);
        if (result == function->params[i].param) // wasn't actually replaced by table
        {
            if (varsparam && (result != ScriptVarsTableLookup(varsparam, function->params[i].param)))
            {
                ScriptError("InitEnvironmentVars called with temporary string! - ask Mark how to rectify this");
                // This shouldn't happen.  If it does, the script is getting a temp var like {HeroName} 
                // as a parameter, and I would need to make the initialvars table make a full copy.  The
                // hack of just not copying it to the initialvars table will basically work though:
                SetEnvironmentVar(script, function->params[i].param, ScriptVarsTableLookup(varsparam, function->params[i].param));
            }
            else if (function->params[i].defaultvalue)
                SetEnvironmentVar(script, function->params[i].param, function->params[i].defaultvalue);
            else
            {
                if (errname && !(function->params[i].flags & SP_OPTIONAL) )
                {
                    ScriptError("%s requested script %s, but did not provide param %s", 
                    errname, function->name, function->params[i].param);
                    ret = 0;
                }
            }
        }
        else
        {
            // if I was able to replace, and I want a dynamic multivalue reference, let the environment vars know.
            // they will reference the initialvars table later
            if (function->params[i].flags & SP_MULTIVALUE)
                SetEnvironmentVar(script, function->params[i].param, SE_DYNAMIC_LOOKUP);
            else
                SetEnvironmentVar(script, function->params[i].param, result);
        }
        stashAddInt(script->varsFlags, function->params[i].param, function->params[i].flags, false); // store flags for easier access
    }

    if (!ret)
    {
        stashTableDestroy(script->vars);
        script->vars = 0;
    }
    return ret;
}

U32 ScriptBegin(const char* scriptName, ScriptVarsTable* vars, ScriptType type, void* user, const char* errname, const char *dataFilename)
{
    ScriptEnvironment* script = ScriptEnvironmentAlloc(dataFilename);
    ScriptFunction* function = ScriptFindFunction(scriptName);
    int error = 0;

    //clear out the script--solves problems when we look for portions of the script that are never filled out.
    memset(script, 0, sizeof(ScriptEnvironment));

    if (!g_instanceVars)
    {
        g_instanceVars = stashTableCreateWithStringKeys(50, StashDeepCopyKeys);
    }

    if (!function)
    {
        if (errname) ScriptError("%s requested script %s, but it doesn't exist", errname, scriptName);
        goto MSE_Err;
    }
    if (function->type != type)
    {
        if (errname) ScriptError("%s requested script %s, but this is not the right type of script", errname, scriptName);
        goto MSE_Err;
    }
    if (error) goto MSE_Err;
    
    script->function = function;

    switch (type)
    {
    case SCRIPT_MISSION:    script->mission = (const MissionDef*)user;        break;
    case SCRIPT_ENCOUNTER:    script->encounter = (EncounterGroup*)user;    break;
    case SCRIPT_LOCATION:    script->location = (ScriptLocation*)user;    break;
    case SCRIPT_ENTITY:        script->entity = (Entity*)user;                break;
    }
    if (!InitEnvironmentVars(errname, script, function, vars))
        goto MSE_Err;
    script->timeDelta = dbSecondsSince2000();
    script->runState = SCRIPTSTATE_RUNNING;
    script->id = g_nextScriptId++;                // need to worry about overflow sometime?

    if (!g_runningScripts)
        eaCreate(&g_runningScripts);
    eaPush(&g_runningScripts, script);

    if( script->function->functionToRunOnInitialization )
    {
        g_currentScript = script;
        g_currentTime    = dbSecondsSince2000() - g_currentScript->timeDelta;
        g_currentScript->function->functionToRunOnInitialization();
        g_currentScript = 0;
        g_currentTime    = 0;
    }

    //Temp hack till we get real global script vars
    {
        extern int gMinerStrikeRunning;
        extern void MinerStrike(void);
        if( 0 == stricmp( script->function->name, "MinerStrike" ) )
            gMinerStrikeRunning++;
    }
    //End hack

    return script->id;
MSE_Err:
    ScriptEnvironmentFree(script);
    return 0;
}

int ScriptEnd(U32 scriptId, int immediate)
{
    int i, n;
    n = eaSize(&g_runningScripts);
    for (i = n-1; i >= 0; i--)
    {
        if (g_runningScripts[i]->id == scriptId)
        {
            if (immediate)
            {
                // this should NEVER be called when in the loop
                assert(g_currentScript == NULL);

                ScriptEnvironmentDestroy(g_runningScripts[i]);
                eaRemove(&g_runningScripts, i);
            }
            else
            {
                // script runs for one more tick and we tell it to stop
                ScriptSendSignal(scriptId, SIGNAL_STOP);
                g_runningScripts[i]->deleteMe = 1;
            }
            return 1;
        }
    }
    return 0;
}

int ScriptSendSignal(U32 scriptId, char* signal)
{
    char buf[2000];
    ScriptEnvironment* script = ScriptFromRunId(scriptId);
    if (!script) return 0;

    strcpy(buf, PREFIX_SIGNAL);
    strncatt(buf, signal);
    SetEnvironmentVar(script, buf, "True");
    return 1;
}

int ScriptPause(U32 scriptId, int on)
{
    ScriptEnvironment* script = ScriptFromRunId(scriptId);
    if (!script) return 0;

    if (on)
    {
        if (script->runState == SCRIPTSTATE_PAUSED) return 1;

        script->runState = SCRIPTSTATE_PAUSED;
        script->pauseTime = dbSecondsSince2000();
    }
    else // off, or resume
    {
        if (script->runState == SCRIPTSTATE_RUNNING) return 1;

        script->runState = SCRIPTSTATE_RUNNING;
        script->timeDelta += dbSecondsSince2000() - script->pauseTime;
    }
    return 1;
}

int ScriptReset(U32 scriptId)
{
    ScriptEnvironment* script = ScriptFromRunId(scriptId);
    if (!script) return 0;

    script->timeDelta = script->pauseTime = dbSecondsSince2000();
    stashTableClear(script->vars);
    stashTableDestroy(script->vars);
    InitEnvironmentVars("command", script, script->function, NULL);
    return 1;
}

int ScriptRunIdFromName(char* scriptName, ScriptType type)
{
    int i, n;
    n = eaSize(&g_runningScripts);
    for (i = n-1; i >= 0; i--)
    {
        ScriptFunction* f = g_runningScripts[i]->function;
        if (stricmp(f->name, scriptName) == 0 && f->type == type)
            return g_runningScripts[i]->id;
    }
    return 0;
}

ScriptEnvironment* ScriptFromRunId(U32 scriptId)
{
    int i, n;
    n = eaSize(&g_runningScripts);
    for (i = n-1; i >= 0; i--)
    {
        if (g_runningScripts[i]->id == scriptId)
            return g_runningScripts[i];
    }
    return NULL;
}

//
void ScriptEndAllRunningScripts(bool immediate)
{
    int i, n;

    n = eaSize(&g_runningScripts);

    if(!immediate)
    {
        for (i = n-1; i >= 0; i--) // reverse so we can delete scripts while executing
        {
            ScriptEnd(g_runningScripts[i]->id, false);
        }

        // Allow the the stop signals to fire before deleting the script
        ScriptTick(FORCE_SCRIPT_TICK);
    }

    for (i = n-1; i >= 0; i--) // reverse so we can delete scripts while executing
    {
        ScriptEnd(g_runningScripts[i]->id, true);
    }

    g_currentScript = NULL;
}

int ScriptCountRunningInstances(STRING name)
{
    int i, n, count = 0;

    n = eaSize(&g_runningScripts);
    for (i = n-1; i >= 0; i--) // reverse so we can delete scripts while executing
    {
        if (stricmp(g_runningScripts[i]->function->name, name) == 0)
            count++;
    }
    return count;
}

// *********************************************************************************
//  Global hook
// *********************************************************************************

// scripts can be executed here and at ScriptClosureListCall
void ScriptTick(F32 timestep)
{
    int i, n;
    const F32 framesPerTick = (30.0 / SCRIPT_TICKS_PER_SECOND);
    static F32 frametime = 0.0;
    U32 dbseconds;

#ifndef DISABLE_ASSERTIONS
    // scripts ignore any divide-by-zero errors
    int prev_ignoredivzero = g_ignoredivzero;
    if (isProductionMode())
        g_ignoredivzero = 1;
#endif

    ScriptLocationTick();
    ScriptGlowiesTick();
    ScriptSkyFadeTick(timestep);

    if (timestep == FORCE_SCRIPT_TICK)
        timestep = framesPerTick;

    // run at SCRIPT_TICKS_PER_SECOND
    frametime += timestep;
    if (frametime >= framesPerTick)
    {
        frametime -= framesPerTick;

        dbseconds = dbSecondsSince2000();
        g_currentKarmaTime = dbseconds;
        n = eaSize(&g_runningScripts);
        for (i = n-1; i >= 0; i--) // reverse so we can delete scripts while executing
        {
            EncounterGroup * myEncounterToDelete = 0;
            bool encounterScriptWithNoEncounter = false;

            g_currentScript = g_runningScripts[i];
            g_currentTime = dbseconds - g_currentScript->timeDelta;

            //If another script shut down an encounter that itself has a script, that encounter's script
            //of course doesn't get a final tick because his encounter has been blown away
            if (g_currentScript->deleteMe &&
                g_currentScript->function->type == SCRIPT_ENCOUNTER &&
                (!g_currentScript->encounter || !g_currentScript->encounter->active ) )
                encounterScriptWithNoEncounter = true;
            
            if (g_currentScript->runState != SCRIPTSTATE_PAUSED && encounterScriptWithNoEncounter == false )
                g_currentScript->function->function();

            if( g_currentScript->closeMyEncounter ) 
            {
                myEncounterToDelete = g_currentScript->encounter;
                myEncounterToDelete->scriptControlsCompletion = 0;
            }

            if (g_currentScript->encounter)
                g_currentScript->encounter->scriptHasNotHadTick = false;

            PERFINFO_AUTO_START("ScriptUIUpkeep", 1);
            ScriptUIUpkeep();
            PERFINFO_AUTO_STOP();

            if (g_currentScript->deleteMe) // can be set by script or stop function
            {
                ScriptUICleanup();
                ScriptEnvironmentDestroy(g_runningScripts[i]);
                eaRemove(&g_runningScripts, i);
            }

            if( myEncounterToDelete ) //If you tried to close your own encounter, defer closure to here.
            {
                EncounterCleanup(myEncounterToDelete);
            }


            FreeAllTempStrings();
        }
        g_currentScript = 0;
    }

#ifndef DISABLE_ASSERTIONS
    g_ignoredivzero = prev_ignoredivzero;
#endif
}

// *********************************************************************************
//  Script Closures
// *********************************************************************************

typedef struct ScriptClosure
{
    U32            scriptid;
    void*        func;
    const char*    lua_func;
} ScriptClosure;
typedef ScriptClosure** ScriptClosureList;

MP_DEFINE(ScriptClosure);
ScriptClosure* ScriptClosureCreate(void) { MP_CREATE(ScriptClosure, 100); return MP_ALLOC(ScriptClosure); }
void ScriptClosureDestroy(ScriptClosure* sc) { SAFE_FREE(sc->lua_func); MP_FREE(ScriptClosure, sc); }

// also clears if f == NULL
void SetScriptClosure(ScriptClosureList* list, U32 scriptid, void* f, const char* lua_f)
{
    int i;

    for (i = eaSize(list)-1; i >= 0; i--)
    {
        if ((*list)[i]->scriptid == scriptid)
        {
            if (!f)
            {
                eaRemoveAndDestroy(list, i, ScriptClosureDestroy);
            }
            else
            {
                (*list)[i]->func = f;
                SAFE_FREE((*list)[i]->lua_func);
                if(lua_f && lua_f[0])
                    (*list)[i]->lua_func = strdup(lua_f);
            }
            return;
        }
    }
    if (f)
    {
        ScriptClosure* sc = ScriptClosureCreate();
        sc->scriptid = scriptid;
        sc->func = f;
        if(lua_f && lua_f[0])
            sc->lua_func = strdup(lua_f);
        eaPush(list, sc);
    }
}

// ScriptClosureListCall calls passed function UNTIL it returns true.
// returns true if this happened
// NOTE - this now means that scripts are run at other times than ScriptTick()
int ScriptClosureListCall(ScriptClosureList* list, SCLC_Function func, void* param1, void* param2)
{
    int i, ret = 0;
    U32 dbseconds;
    void * preservePrevCurrentScript = 0;
    F32 preservePrevCurrentScriptTime;

    if (!*list)
        return 0;
    dbseconds = dbSecondsSince2000();

    //Preserve old script environment
    if( g_currentScript )
    {
        preservePrevCurrentScript = g_currentScript;
        preservePrevCurrentScriptTime = g_currentTime;
    }

    for (i = eaSize(list)-1; i >= 0; i--)
    {
        ScriptClosure* sc = (*list)[i];
        ScriptEnvironment* script = ScriptFromRunId(sc->scriptid);
        if (!script) // script has closed in meantime
        {
            eaRemoveAndDestroy(list, i, ScriptClosureDestroy);
            continue;
        }
        g_currentScript = script;
        g_currentTime = dbseconds - g_currentScript->timeDelta;

        if(sc->lua_func && sc->func)
            ret = ((SCLC_Function)sc->func)(cpp_const_cast(void*)(sc->lua_func), param1, param2);
        else if(sc->func)
            ret = func(sc->func, param1, param2);
        
        if (script->deleteMe)
            ScriptError("Script %s tried to delete itself during closure execution", script->function->name);
        if (script->closeMyEncounter)
            ScriptError("Script %s tried to close its encounter during closure execution", script->function->name);
        g_currentScript = NULL;
        if (ret) break; // any callback that handled this message breaks execution
    }

    //Restore previous script environment
    if( preservePrevCurrentScript )
    {
        g_currentScript = preservePrevCurrentScript;
        g_currentTime = preservePrevCurrentScriptTime;
    }
    else
    {
        FreeAllTempStrings(); //If there is not a stack of script environments, it's OK, otherwise just let them pile up
    }

    return ret;
}

static void ScriptClosureDestroyAdapter(void* arg0)
{
    ScriptClosureDestroy((ScriptClosure*)arg0);
}

void ScriptClosureListDestroy(ScriptClosureList* list)
{
    eaClearEx(list, ScriptClosureDestroyAdapter);
}

// *********************************************************************************
//  Shard events
// *********************************************************************************

static char g_activeEvent[64];
static U32 g_activeScriptID;
static U32 g_eventSignalTS;

// received update to g_mapgroup
void ScriptHandleMapGroup(void)
{
    if (db_state.is_static_map) 
    {
        // shard events only happen on static maps!

        if (stricmp(g_activeEvent, g_mapgroup.activeEvent) != 0)
        {
            // shutdown any running event
            if (g_activeScriptID)
            {
                ScriptEnd(g_activeScriptID, 0);
                g_activeScriptID = 0;
            }

            // start up new event if there is one
            if (g_mapgroup.activeEvent[0])
            {
                if (ScriptFindFunction(g_mapgroup.activeEvent)) 
                {
                    g_activeScriptID = ScriptBegin(g_mapgroup.activeEvent, NULL, SCRIPT_ZONE, NULL, "Shard Event", "");
                } else {
                    const ScriptDef *pScriptDef = ScriptDefFromFilename(g_mapgroup.activeEvent);

                    if (pScriptDef)
                        g_activeScriptID = ScriptBeginDef(pScriptDef, SCRIPT_ZONE, NULL, "Shard Event", "");
                }
            }
            strcpy(g_activeEvent, g_mapgroup.activeEvent);
        }
        if (g_eventSignalTS != g_mapgroup.eventSignalTS)
        {
            if (g_mapgroup.eventSignal[0])
                ScriptSendSignal(g_activeScriptID, g_mapgroup.eventSignal);
            g_eventSignalTS = g_mapgroup.eventSignalTS;
        }
    }
}

void ShardEventBegin(ClientLink* client, char* scriptName)
{
    ScriptFunction* function;

    if (!scriptName) return;
    function = ScriptFindFunction(scriptName);
    if (!function)
    {
        // its not a function, perhaps its a scriptdef file
        const ScriptDef *pScriptDef = ScriptDefFromFilename(scriptName);

        if (!pScriptDef)
        {
            // not a function or a scriptdef file
            conPrintf(client, "Script %s does not exist", scriptName);
            return;
        } else {
            function = ScriptFindFunction(pScriptDef->scriptname);
            if (!function)
            {
                // can't find script specified in scriptdef
                conPrintf(client, "Script function %s does not exist", pScriptDef->scriptname);
                return;
            }
        }
    }


    if (function->type != SCRIPT_ZONE)
    {
        conPrintf(client, clientPrintf(client,"ZENotEvent", scriptName));
        return;
    }

    mapGroupLock();
    if (g_mapgroup.activeEvent[0])
    {
        conPrintf(client, clientPrintf(client,"ZEStopCurrentErr", g_mapgroup.activeEvent));
        mapGroupUpdateUnlock();
        return;
    }
    strncpyt(g_mapgroup.activeEvent, scriptName, sizeof(g_mapgroup.activeEvent));
    g_mapgroup.eventSignal[0] = 0;
    g_mapgroup.eventSignalTS = 0;
    mapGroupUpdateUnlock();
    conPrintf(client, "Shard event %s started", g_mapgroup.activeEvent);
}

void ShardEventEnd(ClientLink* client)
{
    mapGroupLock();
    if (!g_mapgroup.activeEvent[0])
    {
        conPrintf(client, clientPrintf(client,"ZENoEvent"));
    }
    else
    {
        conPrintf(client, "Shard event %s stopped", g_mapgroup.activeEvent);
        g_mapgroup.activeEvent[0] = 0;
        g_mapgroup.eventSignal[0] = 0;
        g_mapgroup.eventSignalTS = 0;
    }
    mapGroupUpdateUnlock();
}

void ShardEventSignal(ClientLink* client, char* signal)
{
    mapGroupLock();
    if (!g_mapgroup.activeEvent[0])
    {
        conPrintf(client, clientPrintf(client,"ZENoEvent"));
    }
    else
    {
        U32 prevTS = g_mapgroup.eventSignalTS;
        strncpyt(g_mapgroup.eventSignal, signal, sizeof(g_mapgroup.eventSignal));
        g_mapgroup.eventSignalTS = dbSecondsSince2000();
        if (g_mapgroup.eventSignalTS == prevTS) g_mapgroup.eventSignalTS++; // just a little extra protection
        conPrintf(client, clientPrintf(client,"ZESendSignal", g_mapgroup.eventSignal, g_mapgroup.activeEvent));
    }
    mapGroupUpdateUnlock();
}

// *********************************************************************************
//  Z O N E    S C R I P T S
// *********************************************************************************

// ZoneScriptsStart - Starts up all zone scripts specified in maps.spec for this map.
void ZoneScriptsStart()
{
    const MapSpec* spec = MapSpecFromMapId(db_state.base_map_id);
    int i;
    int count;
    
    if (spec) {
        count = eaSize(&spec->zoneScripts);

        for (i = 0; i < count; i++)
        {
            const ScriptDef *pScriptDef = ScriptDefFromFilename(spec->zoneScripts[i]);

            if (pScriptDef)
            {
                ScriptBeginDef(pScriptDef, SCRIPT_ZONE, NULL, "ZoneScript start", spec->zoneScripts[i]);
            } else {
                printf("Unable to find map script %s\n", spec->zoneScripts[i]);
            }
        }
    }
}

// EntityScriptsBegin - Begin all scripts on an entity from a provided array of ScriptDefs and optional vars table
void EntityScriptsBegin(Entity *entity, const ScriptDef **scripts, int numScripts, ScriptVarsTable *vars, const char *dataFilename)
{
    int i;
    static ScriptVarsTable vartable;
    ScriptVarsTable *initialvars = vars ? vars : &vartable;

    if(!entity)
    {
        Errorf("Attempted to start scripts on a NULL entity");
        return;
    }

    for (i = 0; i < numScripts; i++)
    {
        U32 scriptId = 0;
        const ScriptDef* script = scripts[i];

        ScriptVarsTablePushScope(initialvars, &script->vs);
        scriptId = ScriptBegin(script->scriptname, initialvars, SCRIPT_ENTITY, entity, "EntityScriptsBegin", dataFilename);
        if(scriptId > 0)
            eaiPush(&entity->scriptids, scriptId);
        else
            Errorf("Error starting script %s on entity %s", script->scriptname, entity->name);
        ScriptVarsTablePopScope(initialvars);
    }
}

// Slash command helper function to start an Entity Script from a ScriptDef on a targeted entity, using its encounter vartable if it has any
void EntityScriptBeginScriptDefFromCommand(Entity *e, const ScriptDef *pScriptDef, const char *dataFilename)
{
    if(!e || !pScriptDef)
    {
        Errorf("Invalid arguments to EntityScriptBeginScriptDefFromCommand\n");
        return;
    }
    else
    {
        EncounterGroup *group = e->encounterInfo ? e->encounterInfo->parentGroup : 0;

        if(group && group->active)
            EntityScriptsBegin(e, &pScriptDef, 1, &group->active->vartable, dataFilename);
        else
            EntityScriptsBegin(e, &pScriptDef, 1, NULL, dataFilename);
    }
}

U32 EntityScriptBeginScriptFunctionFromCommand(Entity *e, const char *scriptFunction, const char *dataFilename)
{
    U32 scriptId = 0;
    if(!e || !scriptFunction)
    {
        Errorf("Invalid arguments to EntityScriptBeginScriptDefFromCommand\n");
    }
    else
    {
        EncounterGroup *group = e->encounterInfo ? e->encounterInfo->parentGroup : 0;

        if(group && group->active)
            scriptId = ScriptBegin(scriptFunction, &group->active->vartable, SCRIPT_ENTITY, e, "EntityScriptBeginScriptFunctionFromCommand", dataFilename);
        else
            scriptId = ScriptBegin(scriptFunction, NULL, SCRIPT_ENTITY, e, "EntityScriptBeginScriptFunctionFromCommand", dataFilename);
    }
    return scriptId;
}

extern Entity* EntTeamInternalEx(TEAM team, int index, int* num, int onlytargetable, int countDead);
// Scripthook to start an Entity Script from a scriptDef reference
//  Adds the scopes of the initialvars table of the parent script to the var table but otherwise is untied to the parent script
void EntityScriptBeginScriptDefFromScript(ENTITY entity, STRING scriptDef)
{
    Entity *e = EntTeamInternalEx(entity, 0, NULL, 0, 1);
    const ScriptDef *pScriptDef = ScriptDefFromFilename(scriptDef);

    if(!e || !pScriptDef || !g_currentScript)
    {
        ScriptError("Invalid arguments to EntityScriptBeginScriptDefFromScript\n");
        return;
    }

    EntityScriptsBegin(e, &pScriptDef, 1, &g_currentScript->initialvars, g_currentScript->pchSourceFile);
}

// Scripthook to start an Entity Script from a script function reference
//  Copies the scopes of the initialvars table of the parent script but otherwise is untied to the parent script
void EntityScriptBeginFunctionFromScript(ENTITY entity, STRING script)
{
    Entity *e = EntTeamInternalEx(entity, 0, NULL, 0, 1);

    if(!e || !script || !g_currentScript)
    {
        ScriptError("Invalid arguments to EntityScriptBeginScriptDefFromScript\n");
        return;
    }
    else
    {
        U32 scriptId = 0;
        scriptId = ScriptBegin(script, &g_currentScript->initialvars, SCRIPT_ENTITY, e, "EntityScriptBeginFunctionFromScript", g_currentScript->pchSourceFile);
        if(scriptId > 0)
            eaiPush(&e->scriptids, scriptId);
        else
            Errorf("Error starting script %s on entity %s", script, e->name);
    }
}

void EntityScriptsStop(Entity *entity)
{
    int i, n;

    n = eaiSize(&entity->scriptids);
    for (i = n-1; i >= 0; i--)
    {
        ScriptEnd(entity->scriptids[i], 0);
    }
    eaiSetSize(&entity->scriptids, 0);
}

// *********************************************************************************
//  T E S T I N G   F E A T U R E
// *********************************************************************************

void SetStartZone(NUMBER mapID)
{
    dbOverrideStartZone(mapID);
}

// Force the start of a scriptDef using the relevant expected variables for the type of script
void CmdScriptDefStart(const ScriptDef* pScriptDef, Entity *e)
{
    ScriptFunction* function = NULL;

    if(!pScriptDef || !pScriptDef->scriptname)
    {
        Errorf("CmdScriptDefStart called with invalid scriptDef");
        return;
    }
    
    function = ScriptFindFunction(pScriptDef->scriptname);

    if(!function)
    {
        Errorf("CmdScriptDefStart called with invalid script function");
        return;
    }

    switch(function->type)
    {
        xcase SCRIPT_ZONE:
        {
            ScriptBeginDef(pScriptDef, SCRIPT_ZONE, NULL, "ScriptDefStartCmd", "*slash command*");
        }
        xcase SCRIPT_MISSION:
        {
            if(!OnMissionMap())
                Errorf("CmdScriptDefStart tried to start mission script on non-mission map");
            else
                StartMissionScriptDef(pScriptDef, g_activemission->def, &g_activemission->vars);
        }
        xcase SCRIPT_ENCOUNTER:
        {
            if(!e || !e->encounterInfo || !e->encounterInfo->parentGroup)
                Errorf("CmdScriptDefStart tried to start encounter script without an finding an encounter");
            else
                StartEncounterScriptFromDef(pScriptDef, e->encounterInfo->parentGroup);
        }
        xcase SCRIPT_LOCATION:
        {
            Errorf("CmdScriptDefStart doesn't currently support location scripts, use scriptlocationstart instead");
        }
        xcase SCRIPT_ENTITY:
        {
            if(!e)
                Errorf("CmdScriptDefStart tried to start entity script without targeted entity");
            else
                EntityScriptBeginScriptDefFromCommand(e, pScriptDef, "*slash command*");
        }
    }
}

// Fake the start of a blank lua script of the specified function, then add in the LuaFiles or LuaString var with the relevant data
void CmdScriptLuaStart(const char* luaFunc, const char* luaFile, const char* luaString, Entity *e)
{
    U32 scriptId = 0;
    ScriptFunction* function = NULL;

    if(!luaFunc || (!luaFile && !luaString))
    {
        Errorf("CmdScriptLuaStart called with invalid arguments");
        return;
    }
    
    function = ScriptFindFunction(luaFunc);

    if(!function)
    {
        Errorf("CmdScriptLuaStart called with invalid script function");
        return;
    }

    switch(function->type)
    {
        xcase SCRIPT_ZONE:
        {
            scriptId = ScriptBegin(luaFunc, NULL, SCRIPT_ZONE, NULL, "CmdScriptLuaStart", luaFile);
        }
        xcase SCRIPT_MISSION:
        {
            if(!OnMissionMap())
                Errorf("CmdScriptLuaStart tried to start mission script on non-mission map");
            else
                scriptId = ScriptBegin(luaFunc, &g_activemission->vars, SCRIPT_MISSION, (void*)g_activemission->def, "CmdScriptLuaStart", luaFile);
        }
        xcase SCRIPT_ENCOUNTER:
        {
            if(!e || !e->encounterInfo || !e->encounterInfo->parentGroup)
                Errorf("CmdScriptLuaStart tried to start encounter script without an finding an encounter");
            else
                scriptId = StartEncounterScriptFromFunction(luaFunc, e->encounterInfo->parentGroup);
        }
        xcase SCRIPT_LOCATION:
        {
            Errorf("CmdScriptLuaStart doesn't currently support location scripts, use scriptlocationstart instead");
        }
        xcase SCRIPT_ENTITY:
        {
            if(!e)
                Errorf("CmdScriptLuaStart tried to start entity script without targeted entity");
            else
                scriptId = EntityScriptBeginScriptFunctionFromCommand(e, luaFunc, luaFile);
        }
    }

    if(scriptId > 0)
    {
        // Inject the LuaFiles or LuaString into the vars of the new script before it has a chance to start
        ScriptEnvironment *newLuaScript = ScriptFromRunId(scriptId);
        if(luaFile)
            SetEnvironmentVar(newLuaScript, "LuaFiles", luaFile);
        if(luaString)
            SetEnvironmentVar(newLuaScript, "LuaString", luaString);
    }
    else
        Errorf("CmdScriptLuaStart failed to start the script");
}

// *********************************************************************************
//  P V P   M A P S 
// *********************************************************************************

int OnPvPMap()
{
    return server_visible_state.isPvP;
}

void SetScriptCombatLevel(NUMBER combatLevel)
{
    g_scriptCombatLevel = combatLevel;
}

void SetScriptMinCombatLevel(NUMBER combatLevel)
{
    g_scriptMinCombatLevel = combatLevel;
}

void SetScriptMaxCombatLevel(NUMBER combatLevel)
{
    g_scriptMaxCombatLevel = combatLevel;
}


// p->iCombatLevel is set to experience level before calling this function.
// returns true if I'm going to handle setting this players combat level.
int ScriptLevelOverride(Character* p)
{
    Entity* e;

    if (!g_scriptCombatLevel && !g_scriptMinCombatLevel && !g_scriptMaxCombatLevel) 
        return 0;

    e = p->entParent;

    if (!e || !e->pl) 
        return 0;

    if (g_scriptCombatLevel > 0)
        p->iCombatLevel = g_scriptCombatLevel - 1;
    else if (g_scriptMinCombatLevel > 0 && p->iCombatLevel < g_scriptMinCombatLevel - 1)
        p->iCombatLevel = g_scriptMinCombatLevel - 1;
    else if (g_scriptMaxCombatLevel > 0 && p->iCombatLevel > g_scriptMaxCombatLevel - 1)
        p->iCombatLevel = g_scriptMaxCombatLevel - 1;

    return 1;
}

int ScriptGetEffectiveLevel(int level)
{
    if (g_scriptCombatLevel > 0)
        return g_scriptCombatLevel - 1;
    else if (g_scriptMinCombatLevel > 0 && level < g_scriptMinCombatLevel - 1)
        return g_scriptMinCombatLevel - 1;
    else if (g_scriptMaxCombatLevel > 0 && level > g_scriptMaxCombatLevel - 1)
        return g_scriptMaxCombatLevel - 1;
    else
        return level;
}

bool ScriptUsingLevelOverride()
{
    return (g_scriptCombatLevel != 0);
}

void SetAreAllNPCsScared(NUMBER flag)
{
    g_scriptNPCsScared = flag;
}

int AreAllNPCsScared(void)
{
    return g_scriptNPCsScared;
}

ScriptType GetScriptType(void)
{
    if(g_currentScript && g_currentScript->function)
        return g_currentScript->function->type;
    else
        return -1;
}