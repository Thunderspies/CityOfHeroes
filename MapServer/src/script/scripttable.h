/*\
 *
 *    scripttable.h - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides the link table for all the script functions available
 *
 */

#ifndef SCRIPTTABLE_H
#define SCRIPTTABLE_H

#include "gameComm/debugCommon.h"

#define MAX_SCRIPT_PARAMS    90
#define MAX_SCRIPT_SIGNALS    20
#define MAX_SCRIPT_NAMELEN    50


typedef struct ScriptParam
{
    const char*        param;
    const char*        defaultvalue;
    unsigned        flags;
} ScriptParam;

typedef struct ScriptSignal
{
    const char*        display;
    const char*        signal;
} ScriptSignal;

typedef struct ScriptFunction
{
    const char*        name;
    void            (*function)(void);
    ScriptType        type;
    ScriptParam        params[MAX_SCRIPT_PARAMS];        // list of params required in vars before executing
    ScriptSignal    signals[MAX_SCRIPT_SIGNALS];    // list of signals the script understands
    void            (*functionToRunOnInitialization)(void);
} ScriptFunction;

extern ScriptFunction g_scriptFunctions[];

ScriptFunction* ScriptFindFunction(const char* name);

#endif // SCRIPTTABLE_H