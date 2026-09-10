#include "script.h"
#include "scriptengine.h"
#include "scriptutil.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/timing.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "script/LuaLibs/ScriptLuaCommon.h"
#include "script/LuaLibs/ScriptLuaAI.h"
#include "script/LuaLibs/ScriptLuaCallback.h"
#include "script/LuaLibs/ScriptLuaEncounter.h"
#include "script/LuaLibs/ScriptLuaEntity.h"
#include "script/LuaLibs/ScriptLuaGlowie.h"
#include "script/LuaLibs/ScriptLuaKarma.h"
#include "script/LuaLibs/ScriptLuaLocation.h"
#include "script/LuaLibs/ScriptLuaMessage.h"
#include "script/LuaLibs/ScriptLuaMission.h"
#include "script/LuaLibs/ScriptLuaReward.h"
#include "script/LuaLibs/ScriptLuaScript.h"
#include "script/LuaLibs/ScriptLuaTeam.h"
#include "script/LuaLibs/ScriptLuaUI.h"
#include "script/LuaLibs/ScriptLuaVar.h"
#include "script/LuaLibs/ScriptLuaWaypoint.h"

typedef struct LuaLibrary
{
    const char * libName;
    const luaL_Reg * lib ;
} LuaLibrary;

static LuaLibrary COHLuaLibraries [] =
{
    {"AI",            scriptAILib},
    {"Callback",    scriptCallbackLib},
    {"Door",        scriptDoorLib},
    {"Encounter",    scriptEncounterLib},
    {"Entity",        scriptEntityLib},
    {"Glowie",        scriptGlowieLib},
    {"Karma",        scriptKarmaLib},
    {"Location",    scriptLocationLib},
    {"Message",        scriptMessageLib},
    {"Mission",        scriptMissionLib},
    {"Reward",        scriptRewardLib},
    {"Script",        scriptScriptLib},
    {"Team",        scriptTeamLib},
    {"Turnstile",    scriptTurnstileLib},
    {"UI",            scriptUILib},
    {"Var",            scriptVarLib},
    {"Waypoint",    scriptWaypointLib}
};

int LuaMemoryUsed(ScriptEnvironment* script)
{
    if(!script || !script->lua_interpreter)
        return 0;

    return lua_gc(script->lua_interpreter, LUA_GCCOUNT, 0);
}

static int l_StackDump (lua_State *L) {
      int i;
      int top = lua_gettop(L);
      for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
    
          case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;
    
          case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
    
          case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
    
          default:  /* other values */
            printf("%s", lua_typename(L, t));
            break;
    
        }
        printf("  ");  /* put a separator */
      }
      printf("\n");  /* end the listing */

      return 0;
}

static void loadLuaFile(lua_State* L, const char *luaFile)
{
      char path[MAX_PATH];
      char *mem;
      int len;

      if(!luaFile || !luaFile[0])
          return;
 
      strncpyt(path, "SCRIPTS.LOC", MAX_PATH);
      strncatt(path, "/");
      strncatt(path, luaFile);
 
      mem = fileAlloc(path, &len);
 
      if(mem)
      {
            if(luaL_loadstring(L, mem))
            {
                  ScriptError("Lua: Error parsing lua script %s\n", luaFile);
            }
            else
            {
                // Call this file
                if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
                {
                    const char *errMsg = lua_tostring(L, -1);
                    if(errMsg)
                        ScriptError("Lua: Error in lua script %s initalization: %s\n", luaFile, lua_tostring(L, -1));
                    else
                        ScriptError("Lua: Error in lua script %s initialization\n", luaFile);
                }
            }
     
            fileFree(mem);            
      }
      else
            ScriptError("Lua: Can't locate lua script %s\n", luaFile);
}

static int l_LoadLuaFile (lua_State *L) {
    const char *file = luaL_checkstring(L, 1);
    if(file && file[0])
        loadLuaFile(L, file);
    return 0;
}

static void loadLuaString(lua_State* L, const char *luaString)
{
    if(!luaString || !luaString[0])
          return;

    if(luaL_loadstring(L, luaString))
    {
        ScriptError("Lua: Error parsing lua string %s\n", luaString);
    }
    else
    {
        // Call the string
        if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
        {
            const char *errMsg = lua_tostring(L, -1);
            if(errMsg)
                ScriptError("Lua: Error in lua string %s initalization: %s\n", luaString, lua_tostring(L, -1));
            else
                ScriptError("Lua: Error in lua string %s initialization\n", luaString);
        }
    }
}

static void loadCOHLuaLibraries(lua_State * L)
{
    int i, size = sizeof(COHLuaLibraries)/sizeof(LuaLibrary);

    for (i = 0; i < size; ++i)
    {
        lua_newtable(L);
        // luaL_setfuncs(L, COHLuaLibraries[i].lib, 0);                                //lua 5.2
        // lua_setglobal(L, COHLuaLibraries[i].libName);                            //lua 5.2 
        luaL_register(L, COHLuaLibraries[i].libName, COHLuaLibraries[i].lib);        //lua 5.1
    }
}

void printCOHLuaLibraries(const char * libName, char * output)
{
    int i, size = sizeof(COHLuaLibraries)/sizeof(LuaLibrary);


    for (i = 0; i < size; ++i)
    {
        int libFound;

        if (libName)
        {
            libFound = strcmp(COHLuaLibraries[i].libName, libName) == 0;
        }
        else
        {
            libFound = 0;
        }

        if (!libName || libFound)
        {
            //print everything from this library
            int j = 0;
            const luaL_Reg * curLib = COHLuaLibraries[i].lib;

            while (curLib[j].name) //loop until sentinel
            {
                strcat(output, COHLuaLibraries[i].libName);
                strcat(output, ".");
                strcat(output, curLib[j].name);
                strcat(output, "\n");
                ++j;
            }
        }

        if (libFound)
        {
            //all done
            break;
        }
    }
}

//void EndScript(void)
static int l_EndScript (lua_State *L)
{
    EndScript();
    return 0;
}

static void LuaPanic (lua_State *L, lua_Debug *ar)
{
    //We need to make sure this is the correct script environment to end the script
    if(g_currentScript && g_currentScript->lua_interpreter == L)
        EndScript();

    ScriptError("Lua: Maximum number of instructions reached - script is being terminated\n");
    luaL_error(L, "Instruction limit reached");
}

#define MAX_LUA_INSTRUCTIONS    100000
#define MAX_LUA_MEMORY            2048

void scriptLua(void)
{
    unsigned int saved_fpu_control;
    int count, i;
    /* the Lua interpreter */
    lua_State* L = NULL;
    if(g_currentScript->lua_interpreter)
        L = g_currentScript->lua_interpreter;

    // make sure we have FPU in full precision or Lua will screw up
    _controlfp_s(&saved_fpu_control, 0, 0);    // retrieve current control word
    _controlfp_s(NULL, _PC_64, _MCW_PC);    // set the fpu to full precision

    PERFINFO_AUTO_START("scriptLua", 1);

    INITIAL_STATE

        ON_ENTRY

            if(VarIsEmpty("LuaFiles") && VarIsEmpty("LuaString"))
            {
                // No actual Lua code to run - early out
                // Restore FPU Precision
                _controlfp_s(NULL, saved_fpu_control, _MCW_PC);
                EndScript();
                return;
            }
        
            if(L)
            {
                ScriptError("Lua: Interpreter already initialized\n");
                lua_close(L);
            }

            /* initialize Lua */
            g_currentScript->lua_interpreter = L = luaL_newstate();
            lua_sethook(L, LuaPanic, LUA_MASKCOUNT, MAX_LUA_INSTRUCTIONS);

            /* load various Lua libraries */
            luaL_openlibs(L);
            loadCOHLuaLibraries(L);
            
            lua_register(L, "LoadLuaFile",    l_LoadLuaFile);
            lua_register(L, "StackDump",    l_StackDump);
            lua_register(L, "EndScript",    l_EndScript);

            luaL_newmetatable(L, "GLOWIEDEF");

            PERFINFO_AUTO_START("scriptLua Load", 1);
            // Load Lua Files from the Var specification
            count = VarGetArrayCount("LuaFiles");
            for (i = 0; i < count; i++)
            {
                STRING file = VarGetArrayElement("LuaFiles", i);

                if (!StringEmpty(file))
                    loadLuaFile(L, file);
            }

            if(!VarIsEmpty("LuaString"))
                loadLuaString(L, VarGet("LuaString"));
            
            PERFINFO_AUTO_STOP();

        END_ENTRY

    END_STATES

    PERFINFO_AUTO_START("scriptLua Tick", 1);
    lua_sethook(L, LuaPanic, LUA_MASKCOUNT, MAX_LUA_INSTRUCTIONS);
    lua_getglobal(L, "tick");
    if (lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
        ScriptError("Lua: Error in script tick: %s\n", lua_tostring(L, -1));

    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_START("scriptLua GC", 1);
    
    lua_gc(L, LUA_GCSTEP, 1);

    PERFINFO_AUTO_STOP();


    ON_SIGNAL("EndScript")
        EndScript();
    END_SIGNAL

    ON_STOPSIGNAL
        EndScript();
    END_SIGNAL

    PERFINFO_AUTO_STOP();

    if(lua_gc(L, LUA_GCCOUNT, 0) > MAX_LUA_MEMORY)
    {
        ScriptError("Lua: Memory limit exceeded - script is being terminated\n");
        EndScript();
    }

    // Restore FPU Precision
    _controlfp_s(NULL, saved_fpu_control, _MCW_PC);
}

static void scriptLuaInit(STRING scriptName, NUMBER scriptType)
{
    SetScriptName(scriptName);
    SetScriptFunc(scriptLua);
    SetScriptType(scriptType);

    SetScriptVar( "LuaFiles",                                            NULL,        SP_OPTIONAL );
    SetScriptVar( "LuaString",                                            NULL,        SP_OPTIONAL );

    SetScriptSignal( "End", "EndScript" );
}

void scriptLuaInitMission()        { scriptLuaInit("LuaMission",    SCRIPT_MISSION); }
void scriptLuaInitEncounter()    { scriptLuaInit("LuaEncounter",    SCRIPT_ENCOUNTER); }
void scriptLuaInitLocation()    { scriptLuaInit("LuaLocation",    SCRIPT_LOCATION); }
void scriptLuaInitZone()        { scriptLuaInit("LuaZone",        SCRIPT_ZONE); }
void scriptLuaInitEntity()        { scriptLuaInit("LuaEntity",    SCRIPT_ENTITY); }
