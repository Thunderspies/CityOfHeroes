/*\
 *
 *    scripthook.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Implementation for generic team references and generic location references.
 *    Implementation for all the basic hook functions available to scripts.
 *
 */

#include "script.h"
#include "scriptengine.h"
#include "scriptutil.h"

#include "ai/entai.h"
#include "ai/entaiScript.h"
#include "ai/entaivars.h"
#include "ai/entaiprivate.h"
#include "svr/svr_player.h"
#include "entity/entPlayer.h"
#include "entity/entGameActions.h"
#include "entity/character_base.h"
#include "entity/character_level.h"
#include "entity/character_target.h"
#include "entity/character_tick.h"
#include "entity/character_animfx.h"
#include "entity/entity.h"

#include "storyarc/storyarcprivate.h"
#include "generator/encounterPrivate.h"
#include "storyarc/missiongeoCommon.h"
#include "group/groupnetsend.h"
#include "generator/patrol.h"
#include "door.h"
#include "Reward.h"
#include "entity/entGameActions.h"
#include "group/groupdynsend.h"
#include "seq/seqstate.h"
#include "gameSys/dooranim.h"
#include "beacon/beacon.h"
#include "TeamReward.h"
#include "script/ZoneEvents/ScriptUI.h"
#include "cmdparse/cmdserver.h"
#include "gridcoll/megaGrid.h"
#include "player/badges_server.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "utils/position.h"
#include "gameComm/NpcServer.h"
#include "gameData/store_net.h"
#include "entity/teamCommon.h"
#include "dbcomm/dbmapxfer.h"
#include "entity/buddy_server.h"
#include "seq/seq.h"
#include "cmdparse/cmdserver.h"
#include "graphics/sun.h"
#include "container/containerbroadcast.h"
#include "comm_backend.h"
#include "entity/pvp.h"
#include "gameData/arenastruct.h"
#include "dbcomm/logcomm.h"
#include "player/pmotion.h"

#include "Scripthook/ScriptHookInternal.h"

///////////////////////////////////////////////////////////////////////////////////
// E X T E R N S
///////////////////////////////////////////////////////////////////////////////////
extern void MissionItemSetClientTimer(MissionObjectiveInfo* info, float expireTime);

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void AllowHeroAndVillainPlayerTypesToTeamUpOnThisMap( bool test )
{
    if( test )
        server_state.allowHeroAndVillainPlayerTypesToTeamUp = 1;
    else 
        server_state.allowHeroAndVillainPlayerTypesToTeamUp = 0;
}

void AllowPrimalAndPraetorianPlayersToTeamUpOnThisMap( bool test )
{
    if( test )
        server_state.allowPrimalAndPraetoriansToTeamUp = 1;
    else
        server_state.allowPrimalAndPraetoriansToTeamUp = 0;
}

void DisableGurneysOnThisMap( bool flag )
{
    char buf[100];
    
    server_visible_state.disablegurneys = flag;

    sprintf(buf,"disablegurneys %i",server_visible_state.disablegurneys);
    cmdOldServerReadInternal(buf);
}

// *********************************************************************************
//  Location Scripts - both named volumes and normal script markers are kept here
// *********************************************************************************

static void InitAllScriptLocations(void);

MP_DEFINE(ScriptLocation);

ScriptLocation** g_scriptLocations;

static int scriptLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* generatorProp;
    PropertyEnt* prop;

    // find an object with properties
    if (!traverser->def->properties) {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }

    // if we have a script marker
    stashFindPointer( traverser->def->properties, "ScriptLocation", &generatorProp );
    if (generatorProp)
    {
        Vec3 pos;
        const ScriptDef* script = ScriptDefFromFilename(generatorProp->value_str);
        copyVec3(traverser->mat[3], pos);
        if (!script)
            ErrorFilenamef(currentMapFile(), "ScriptLocation at (%0.2f,%0.2f,%0.2f) requests script %s.  This doesn't exist.",
                pos[0], pos[1], pos[2], generatorProp->value_str);
        else
        {
            ScriptLocation* ls = MP_ALLOC(ScriptLocation);
            ls->def = script;
            copyVec3(pos, ls->pos);
            copyMat4(traverser->mat, ls->mat);

            stashFindPointer( traverser->def->properties, "AlwaysRun", &prop );
            if (prop)
                ls->alwaysRun = 1;

            stashFindPointer( traverser->def->properties, "RunEvery", &prop );
            if (prop)
            {
                ls->timerControlled = 1;
                ls->timerBegin = (F32)atof(prop->value_str);
                stashFindPointer( traverser->def->properties, "RunEveryRange", &prop );
                if (prop)
                {
                    ls->timerRange = (F32)atof(prop->value_str);
                }
            }

            stashFindPointer( traverser->def->properties, "Name", &prop );
            if (!prop)
                stashFindPointer( traverser->def->properties, "LogicalName", &prop );
            if (prop && prop->value_str)
                ls->name = strdup(prop->value_str);

            ls->locationid = eaPush(&g_scriptLocations, ls);
        }
        return 0; // allow overrides based on grouping
    }

    if (!traverser->def->child_properties)
        return 0;
    return 1; // continue through children
}

void ScriptLocationLoad(void)
{
    int i, n;
    GroupDefTraverser traverser = {0};

    MP_CREATE(ScriptLocation, 10);
    if (!g_scriptLocations)
        eaCreate(&g_scriptLocations);
    n = eaSize(&g_scriptLocations);
    for (i = 0; i < n; i++)
    {
        if (g_scriptLocations[i]->scriptid)
            ScriptEnd(g_scriptLocations[i]->scriptid, 1);
        if (g_scriptLocations[i]->name)
            free(g_scriptLocations[i]->name);
        MP_FREE(ScriptLocation, g_scriptLocations[i]);
    }
    eaSetSize(&g_scriptLocations, 0);

    loadstart_printf("Loading script locations.. ");
    groupProcessDefExBegin(&traverser, &group_info, scriptLoader);
    loadend_printf("%i script locations", eaSize(&g_scriptLocations));

    InitAllScriptLocations();
}

static void InitScriptLocation(ScriptLocation* location)
{
    if (location->timerControlled)
    {
        F32 hours = location->timerBegin;
        F32 random = rule30Float();
        if (random < 0.0) random *= -1;
        hours += location->timerRange * random;

        location->timer = dbSecondsSince2000() + (U32)(hours * 60 * 60);
    }
}

static void InitAllScriptLocations(void)
{
    int i, n;
    n = eaSize(&g_scriptLocations);
    for (i = n-1; i >= 0; i--)
    {
        InitScriptLocation(g_scriptLocations[i]);
    }
}

// kick any scripts that should go off
void ScriptLocationTick(void)
{
    int i, n;
    n = eaSize(&g_scriptLocations);
    for (i = n-1; i >= 0; i--)
    {
        ScriptLocation* loc = g_scriptLocations[i];
        if (loc->scriptid) continue;
        if (loc->alwaysRun || (loc->timerControlled && dbSecondsSince2000() > loc->timer))
        {
            loc->scriptid = ScriptBeginDef(loc->def, SCRIPT_LOCATION, loc, "ScriptLocation start", loc->def->filename);
            if (!loc->scriptid) // don't try again
            {
                loc->alwaysRun = loc->timerControlled = 0;
            }
        }
    }
}

void LocationNotifyScriptStopping(ScriptLocation* location, int scriptid)
{
    if (!location)
    {
        Errorf("Internal error, script of type location without a location field\n");
        return;
    }
    if (scriptid != location->scriptid)
    {
        Errorf("Internal error in when stopping a location script, script gave %i, location has %i\n", scriptid, location->scriptid);
    }
    location->scriptid = 0;
    InitScriptLocation(location);
}

void ScriptLocationDebugPrint(ClientLink* client)
{
    char* str;
    int i, n;
    n = eaSize(&g_scriptLocations);
    estrCreate(&str);
    estrConcatCharString(&str, saUtilLocalize(0,"ScriptLocationHeader"));
    estrConcatChar(&str, '\n');
    for (i = 0; i < n; i++)
    {
        ScriptLocation* loc = g_scriptLocations[i];

        //"LogicalName (pos): ScriptName running (id)" or " will run in 42 mins" or ""
        estrConcatCharString(&str, loc->name? loc->name: saUtilLocalize(0,"UnnamedScriptLocation"));
        estrConcatf(&str, " (%0.1f,%0.1f,%0.1f): ", loc->pos[0], loc->pos[1], loc->pos[2]);
        estrConcatCharString(&str, loc->def->scriptname);
        if (loc->scriptid)
        {
            estrConcatCharString(&str, saUtilLocalize(0,"ScriptLocationRunning", loc->scriptid));
        }
        else if (loc->timerControlled)
        {
            int min = (loc->timer - dbSecondsSince2000()) / 60;
            estrConcatCharString(&str, saUtilLocalize(0,"ScriptLocationWillRun", min));
        }
        estrConcatChar(&str, '\n');
    }
    conPrintf(client, "%s", str);
    estrDestroy(&str);
}

ScriptLocation* ScriptLocationFromName(char* name)
{
    int i, n;
    n = eaSize(&g_scriptLocations);
    for (i = 0; i < n; i++)
    {
        if (g_scriptLocations[i]->name && !stricmp(name, g_scriptLocations[i]->name))
            return g_scriptLocations[i];
    }
    return NULL;
}

void ScriptLocationStart(ClientLink* client, char* name)
{
    ScriptLocation* loc = ScriptLocationFromName(name);
    if (!loc)
    {
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationDoesntExist", name));
        return;
    }
    if (loc->scriptid)
    {
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationRunningAlready", name, loc->scriptid));
        return;
    }
    loc->scriptid = ScriptBeginDef(loc->def, SCRIPT_LOCATION, loc, "ScriptLocation start", loc->def->filename);
    if (loc->scriptid)
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationStarted", name, loc->scriptid));
    else
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationErrorCommand", name));
}

void ScriptLocationStop(ClientLink* client, char* name)
{
    int id;
    ScriptLocation* loc = ScriptLocationFromName(name);
    if (!loc)
    {
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationDoesntExist", name));
        return;
    }
    id = loc->scriptid;
    if (!id)
    {
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationNotRunning", name));
    }
    else
    {
        if (ScriptEnd(id, 0))
            conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationStopped", name));
        else
            conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationErrorCommand", name));
    }
}

void ScriptLocationSignal(ClientLink* client, char* name, char* signal)
{
    ScriptLocation* loc = ScriptLocationFromName(name);
    if (!loc)
    {
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationDoesntExist", name));
        return;
    }
    if (!loc->scriptid)
    {
        conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationNotRunning", name));
    }
    else
    {
        if (ScriptSendSignal(loc->scriptid, signal))
            conPrintf(client, "%s", saUtilLocalize(0,"ScriptSentSignal", name, signal));
        else
            conPrintf(client, "%s", saUtilLocalize(0,"ScriptLocationErrorCommand", name));
    }
}

// *********************************************************************************
//  Script Markers - both named volumes and normal script markers are kept here
// *********************************************************************************

StashTable g_scriptMarkers = 0;
int g_scriptMarkerCount = 0;
int g_namedVolumeCount = 0;
MP_DEFINE(ScriptMarker);
MP_DEFINE(Position);

ScriptMarker* MarkerFind(const char* name, int namedvolume)
{
    char shortname[SCRIPTMARKER_NAMELEN+1];
    strncpyt(shortname, name, SCRIPTMARKER_NAMELEN);
    strcat(shortname, namedvolume? "1": "0");
    return stashFindPointerReturnPointer(g_scriptMarkers, shortname);
}

void ForEachScriptMarker(SCRIPTMARKERPROCESSOR f)
{
    stashForEachElement(g_scriptMarkers, f);
}

SCRIPTMARKER GetScriptMarkerFromScriptElement(SCRIPTELEMENT elem)
{
    return stashElementGetPointer(elem);
}

STRING FindMarkerProperty(SCRIPTMARKER marker, STRING name)
{
    PropertyEnt* generatorProp;
    ScriptMarker *m = (ScriptMarker *) marker;
    if (m && m->properties && stashFindPointer(m->properties, name, &generatorProp))
    {
        return StringDupe(generatorProp->value_str);    
    } else {
        return NULL;
    }
}

STRING FindMarkerPropertyByMarkerName(STRING markerName, STRING name)
{
    SCRIPTMARKER m = MarkerFind(markerName, 0);

    return FindMarkerProperty(m, name);
}

STRING FindMarkerGeoName(SCRIPTMARKER marker)
{
    ScriptMarker *m = (ScriptMarker *) marker;
    if (m) {
        return StringDupe(m->geoName);    
    } else {
        return NULL;
    }
}



static ScriptMarker* MarkerAdd(const char* name, int namedvolume)
{
    char shortname[SCRIPTMARKER_NAMELEN+1];

    ScriptMarker* result = MarkerFind(name, namedvolume);
    if (result) return result;

    result = MP_ALLOC(ScriptMarker);
    eaCreate(&result->locs);
    strncpyt(shortname, name, SCRIPTMARKER_NAMELEN);
    strcpy(result->name, shortname);
    strcat(shortname, namedvolume? "1": "0");
    stashAddPointer(g_scriptMarkers, shortname, result, false);
    result->properties = NULL;
    return result;
}

int markerLoader(GroupDefTraverser* traverser)
{
    PropertyEnt* generatorProp;

    // find an object with properties
    if (!traverser->def->properties) {
        if (!traverser->def->child_properties)    // if none of our children have props, don't go further
            return 0;
        return 1;
    }

    // if we have a script marker
    stashFindPointer( traverser->def->properties, "ScriptMarker", &generatorProp );
    if (generatorProp)
    {
        ScriptMarker* marker;
        if (StringEqual(generatorProp->value_str, "ScriptProperty"))
            marker = MarkerAdd(traverser->def->name, 0);
        else 
            marker = MarkerAdd(generatorProp->value_str, 0);
        if (marker)
        {
            Position* pos;
            pos = MP_ALLOC(Position);
            copyMat4(traverser->mat, pos->mat);
            eaPush(&marker->locs, pos);
            g_scriptMarkerCount++;
            marker->properties = stashTableClone(traverser->def->properties, NULL, NULL);
            marker->geoName = strdup(traverser->def->name);
        }
    } // if generatorProp

    // named volume triggers
    stashFindPointer( traverser->def->properties, "NamedVolume", &generatorProp );
    if (generatorProp)
    {
        ScriptMarker* marker = MarkerAdd(generatorProp->value_str, 1);
        if (marker)
        {
            Position* pos;
            pos = MP_ALLOC(Position);
            copyMat4(traverser->mat, pos->mat);
            eaPush(&marker->locs, pos);
            g_namedVolumeCount++;
        }
    }

    if (!traverser->def->child_properties)
        return 0;
    return 1; // continue through children
}

int MarkerDestroyIter(StashElement el)
{
    int i, n;
    ScriptMarker* marker = stashElementGetPointer(el);
    if (marker)
    {
        n = eaSize(&marker->locs);
        for (i = 0; i < n; i++)
        {
            MP_FREE(Position, marker->locs[i]);
        }
        eaDestroy(&marker->locs);

        if (marker->properties)
            stashTableDestroy(marker->properties);
        marker->properties = NULL;

        if (marker->geoName)
            free(marker->geoName);
        marker->geoName = NULL;

        MP_FREE(ScriptMarker, marker);
    }
    return 1;
}

void ScriptMarkerLoad(void)
{
    GroupDefTraverser traverser = {0};

    MP_CREATE(ScriptMarker, 10);
    MP_CREATE(Position, 10);
    if (!g_scriptMarkers)
        g_scriptMarkers = stashTableCreateWithStringKeys(10, StashDeepCopyKeys);
    stashForEachElement(g_scriptMarkers, MarkerDestroyIter);
    stashTableClear(g_scriptMarkers);
    g_scriptMarkerCount = 0;
    g_namedVolumeCount = 0;

    loadstart_printf("Loading script markers.. ");
    groupProcessDefExBegin(&traverser, &group_info, markerLoader);
    loadend_printf("%i markers, %i volumes", g_scriptMarkerCount, g_namedVolumeCount);
}

// *********************************************************************************
//  Playing arbitrary FX
// *********************************************************************************

void ScriptPlayFXOnEntity(ENTITY entity, STRING fxName)
{
    Entity *e;

    e = EntTeamInternal(entity, 0, NULL);
    if (e && e->pchar && fxName && fxName[0])
    {
        character_PlayFX(e->pchar, e->pchar, fxName, colorPairNone, 0, 0, PLAYFX_NO_TINT);
    }
}

// *********************************************************************************
//  Messages to players
// *********************************************************************************

void SendToAllMapsMessage(STRING localizedMessage )
{
    if (GetMapAllowsVillains())
        sendEntsMsg( CONTAINER_ENTS, -1, INFO_EVENT_VILLAIN, 0, "%s%s", DBMSG_CHAT_MSG, localizedMessage );
    if (GetMapAllowsHeroes())
        sendEntsMsg( CONTAINER_ENTS, -1, INFO_EVENT_HERO, 0, "%s%s", DBMSG_CHAT_MSG, localizedMessage );
    if ( GetMapAllowsPraetorians() )
        sendEntsMsg( CONTAINER_ENTS, -1, INFO_EVENT_PRAETORIAN, 0, "%s%s", DBMSG_CHAT_MSG, localizedMessage );
}

// sent to everyone on map, unless radius is nonzero
void BroadcastAttentionMessage(STRING localizedMessage, LOCATION location, FRACTION radius )
{
    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;
    Vec3 pos;

    if (localizedMessage) {
        if( radius )
        {
            if( !GetPointFromLocation( location, pos ) ) //don't do radius check if you can't figure out where from
                radius = 0;
        }

        for (i = 0; i < players->count; i++)
        {
            Entity * e = players->ents[i];
            // don't count admins spying as players on the map
            if (ENTINFO(e).hide) continue;
            if (!e->ready) continue;
            if( !radius || distance3Squared( ENTPOS(e), pos ) < radius * radius ) 
                serveFloater(e, e, saUtilScriptLocalize(localizedMessage), 0.0, kFloaterStyle_Attention, 0);
        }
    }
}

void SendPlayerAttentionMessageWithColor( ENTITY player, STRING message, NUMBER color, int alertType )
{
    int n, i;

    n = NumEntitiesInTeam(player);
    for (i = 0; i < n; i++)
    {
        Entity * e = EntTeamInternal(player, i, NULL);
        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            serveFloater(e, e, saUtilScriptLocalize(message), 0.0, alertType, color);
        }
    }
}


void SendPlayerAttentionMessage( ENTITY player, STRING message )
{
    SendPlayerAttentionMessageWithColor( player, message, 0, kFloaterStyle_Attention );
}

void SendPlayerCaption( ENTITY player, STRING message )
{
    int n, i;

    n = NumEntitiesInTeam(player);
    for (i = 0; i < n; i++)
    {
        Entity * e = EntTeamInternal(player, i, NULL);
        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            ScriptVarsTable vars = {0};
            ScriptVarsTableScopeCopy(&vars, &g_currentScript->initialvars);
            saBuildScriptVarsTable(&vars, e, e->db_id, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);
            sendPrivateCaptionMsg(e, saUtilTableAndEntityLocalize(message, &vars, e));
        }
    };
}

void SendPlayerPopHelp( ENTITY player, STRING popHelpMessage )
{
    int n, i;

    n = NumEntitiesInTeam(player);
    for (i = 0; i < n; i++)
    {
        Entity * e = EntTeamInternal(player, i, NULL);
        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            sendPopHelp(e, popHelpMessage);
        }
    };
}

void SendPlayerPopHelpReset( ENTITY player )
{
    int n, i;

    n = NumEntitiesInTeam(player);
    for (i = 0; i < n; i++)
    {
        Entity * e = EntTeamInternal(player, i, NULL);
        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            sendPopHelpReset(e);
        }
    };
}

void SendPlayerFadeSound( ENTITY player, NUMBER channel, FRACTION fadeTime )
{
    int n, i;

    n = NumEntitiesInTeam(player);
    for (i = 0; i < n; i++)
    {
        Entity * e = EntTeamInternal(player, i, NULL);
        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            serveFadeSound(e, channel, fadeTime);
        }
    }
}

void SendPlayerSound( ENTITY player, STRING sound, NUMBER channel, FRACTION volumeLevel )
{
    int n, i;

    n = NumEntitiesInTeam(player);
    for (i = 0; i < n; i++)
    {
        Entity * e = EntTeamInternal(player, i, NULL);
        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            servePlaySound(e, sound, channel, volumeLevel);
        }
    }
}

// moved from ScriptedZoneEvent.c
void ResetPlayerMusic(TEAM team)
{
    int n, i;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity * e = EntTeamInternal(team, i, NULL);
        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            char *nhoodSound;
            pmotionGetNeighborhoodProperties(e, 0, &nhoodSound);
            if (nhoodSound && strstri(nhoodSound, "_loop"))
            {
                SendPlayerSound(EntityNameFromEnt(e), nhoodSound, SOUND_MUSIC, 1.0f); // this time we want neighborhood music to interrupt the ZE music
            }
            else
            {
                SendPlayerFadeSound(EntityNameFromEnt(e), SOUND_MUSIC, 1.0f);
            }
        }
    }
}

void SendPlayerSystemMessage( TEAM player, STRING message )
{
    int n, i;

    n = NumEntitiesInTeam(player);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(player, i, NULL);

        if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
        {
            sendChatMsg(e, saUtilScriptLocalize(message), INFO_SVR_COM, 0);
        }
    }
}

void SendPlayerDialog( ENTITY player, STRING message )
{
    Entity * e = EntTeamInternal(player, 0, NULL);
    if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
    {
        sendDialog(e, saUtilScriptLocalize(message));
    }
}

void SendPlayerDialogWithIgnore( ENTITY player, STRING message, NUMBER type )
{
    Entity * e = EntTeamInternal(player, 0, NULL);
    if (e && ENTTYPE_PLAYER == ENTTYPE(e) )
    {
        sendDialogIgnore(e, saUtilScriptLocalize(message), type);
    }
}

FRACTION GetDayNightTime()
{
    return server_visible_state.time;
}


// sent to everyone on map, unless radius is nonzero
//Disabled for now
void ShowTimer(FRACTION time, LOCATION location, FRACTION radius )
{
/*    PlayerEnts* players = &player_ents[PLAYER_ENTS_ACTIVE];
    int i;
    Vec3 pos;

    if( radius )
    {
        if( !GetPointFromLocation( location, pos ) ) //don't do radius check if you can't figure out where from
            radius = 0;
    }

    for (i = 0; i < players->count; i++)
    {
        Entity * e = players->ents[i];
        // don't count admins spying as players on the map
        if (ENTINFO(e).hide) continue;
        if (!e->ready) continue;
        if( !radius || distance3Squared( e->mat[3], pos ) < radius * radius ) 
            e->timer = dbSecondsSince2000() + (U32)(time * 60);
    }*/
}

// *********************************************************************************
//  Script util
// *********************************************************************************

void EndScript(void)
{
    g_currentScript->deleteMe = 1;
}

void DebugString(STRING string)
{
    if (isDevelopmentMode())
        printf("%s\n", string);
}

// *********************************************************************************
//  Timers
// *********************************************************************************

char* TimerName(const char* timer)
{
    static char timerBuf[1000];
    sprintf(timerBuf, PREFIX_TIMER "%s", timer);
    return timerBuf;
}

int DoEvery(STRING timer, FRACTION minutes, void(*func)(void))
{
    const char* var;
    char* timerName = TimerName(timer);
    U32 endTime, timeFromNow;

    // calc max time from now to go off
    timeFromNow = (int)(minutes * 60);
    if (timeFromNow <= 0) timeFromNow = 1;    // minimum resolution is one second
    endTime = g_currentTime + timeFromNow;

    // if timer unset or set to higher than time I want, set it
    var = GetEnvironmentVar(g_currentScript, timerName);
    if (!var || (U32)atoi(var) > endTime)
    {
        VarSetNumber(timerName, endTime);
    }
    // if timer elapsed, call function
    else if ((U32)atoi(var) <= g_currentTime)
    {
        VarSetNumber(timerName, endTime);
        if( func )
            func();
        return 1;
    }
    return 0;
}

void TimerSet(STRING timer, FRACTION minutes)
{
    char* timerName = TimerName(timer);
    U32 endTime, timeFromNow;

    timeFromNow = (int)(minutes * 60);
    endTime = g_currentTime + timeFromNow;
    VarSetNumber(timerName, endTime);
}

int TimerElapsed(STRING timer)
{
    char* timerName = TimerName(timer);
    const char* var;

    var = GetEnvironmentVar(g_currentScript, timerName);
    if (var) {
        return ((U32)atoi(var) <= g_currentTime);
    } else {
        return false;
    }
}

FRACTION TimerRemain(STRING timer)
{
    char* timerName = TimerName(timer);
    const char* var;
    U32 time, remain;

    var = GetEnvironmentVar(g_currentScript, timerName);
    if (!var)
    {
        ScriptError("Invalid timer name: %s", timer);
        return 0.0;
    }
    time = (U32)atoi(var);
    if (time <= g_currentTime)
        return 0.0; // elapsed
    remain = time - g_currentTime;
    return (F32)remain / 60.0;
}

void TimerRemove(STRING timer)
{
    char* timerName = TimerName(timer);

    RemoveEnvironmentVar(g_currentScript, timerName); 
}

NUMBER CurrentTime(void)
{
    return g_currentTime;
}

NUMBER SecondsSince2000(void)
{
    return timerSecondsSince2000();
}

FRACTION CpuTicksInSeconds(void)
{
    return timerSeconds64( timerCpuTicks64() );
}

FRACTION TimeSinceCpuTicksInSeconds(FRACTION start)
{
    return timerSeconds64( timerCpuTicks64() ) - start;
}

// *********************************************************************************
//  Script Controlled Doors
// *********************************************************************************

void LockDoor( LOCATION loc, STRING msg )
{
    Vec3 pos;
    if (!GetPointFromLocation(loc, pos) )
        return;

    ScriptLockOrUnlockDoor( pos, msg, DOOR_LOCKED );
}

void UnlockDoor( LOCATION loc )
{
    Vec3 pos;
    if (!GetPointFromLocation(loc, pos) )
        return;
    ScriptLockOrUnlockDoor( pos, 0, 0 );
}

//This might be crazy, but hey
void OpenDoor( LOCATION loc )
{
    Entity * doorent;
    Vec3 pos;

    if (!GetPointFromLocation(loc, pos) )
        return;

    doorent = getTheDoorThisGeneratorSpawned(pos);
    if( doorent )
        aiDoorSetOpen(doorent, 1);
}

void CloseDoor( LOCATION loc )
{
    Entity * doorent;
    Vec3 pos;

    if (!GetPointFromLocation(loc, pos) )
        return;

    doorent = getTheDoorThisGeneratorSpawned(pos);
    if( doorent )
        aiDoorSetOpen(doorent, 0);
}

// *********************************************************************************
//  Dynamic Sky Files
// *********************************************************************************

NUMBER SkyFileGetByName(STRING name)
{
    return serverFindSkyByName(name);
}

void SkyFileFade(NUMBER sky1, NUMBER sky2, FRACTION fade)
{
    serverSetSkyFade(sky1, sky2, fade); // 0.0 = all sky1
}

// *********************************************************************************
//  Dynamic Geometry
// *********************************************************************************

NUMBER DynamicGeometryFind(STRING name, LOCATION location, FRACTION radius)
{
    Vec3 pos;

    if (!GetPointFromLocation(location, pos) )
        return 0;

    return groupDynFind(name, pos, radius);
}

void DynamicGeometrySetHP(NUMBER id, NUMBER hp, int repair)
{
    groupDynSetHpAndRepair(id, hp, repair);
}

NUMBER DynamicGeometryGetHP(NUMBER id)
{
    return groupDynGetHp(id);
}


// *********************************************************************************
//  PvP Functions
// *********************************************************************************

void SetPvPMap(NUMBER flag)
{
    char buf[100];
    int n, i;

    server_visible_state.isPvP = flag;

    // set up default pvp settings
    server_state.enablePowerDiminishing = flag;
    server_state.enableTravelSuppressionPower = flag;
    server_state.enableHealDecay = flag;
    server_state.enablePassiveResists = flag;
    server_state.enableSpikeSuppression = false;
    server_state.inspirationSetting = ARENA_ALL_INSPIRATIONS;

    sprintf(buf,"pvpmap %i",server_visible_state.isPvP);
    cmdOldServerReadInternal(buf);

    // setup anyone who's already on the map
    n = NumEntitiesInTeam(ALL_PLAYERS_READYORNOT);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(ALL_PLAYERS_READYORNOT, i, NULL);

        pvp_ApplyPvPPowers(e);
    }
}



void SetBloodyBayTeamInternal( Entity * e )
{
    U32 gangID;
    U32 oldGangID;
    int i, petCount;
    static int gangIDBase = 100000000; // one hundred million - that should be high enough avoid conflicts with teamups, but not overflow

    if( !e || !e->pchar )
        return;

    oldGangID = e->pchar->iGangID;

    //TO DO needs to be a hash or something so firebases don't get confused? Maybe not.
    if( e->teamup_id )
    {
        gangID = e->teamup_id;
    }
    else if( e->BloodyBayGangID )
    {
        gangID = e->BloodyBayGangID;
    }
    else
    {
        gangID = gangIDBase++;
        e->BloodyBayGangID = gangID;
    }

    if( gangID & ( GANG_ID_IS_STATIC | GANG_ID_IS_TEAMUP_ID )  )
        ScriptError( "TeamUP ID or Db_ID %d too big to be used for gang ID!", gangID );

    if( e->teamup_id )
    {
        gangID |= GANG_ID_IS_TEAMUP_ID;
    }

    //If you are in the safezone, then clear your gang ID 
    for( i = 0 ; i < e->volumeList.volumeCount ; i++ )
    {

        char *name = 0;
        if( e->volumeList.volumes[i].volumePropertiesTracker )
            name = groupDefFindPropertyValue(e->volumeList.volumes[i].volumePropertiesTracker->def,"NamedVolume");
        if( name && StringEqual( name, "SafeZone" ) )
            gangID = 0;
    }


    if( oldGangID != gangID)
    {
        aiScriptSetGang(e, gangID);

        // need to set pets gang correctly too
        //        Any pets that have the same gang id as the parent originally did will be switched to the
        //        new gang id.
        petCount = e->petList_size;
        for( i = petCount-1 ; i >= 0 ; i-- )
        {
            Entity * pet = erGetEnt(e->petList[i]);
            if ( pet && pet->pchar && pet->pchar->iGangID == oldGangID ) 
            {
                aiScriptSetGang(pet, gangID);
            }
        }
    }
}


void SetBloodyBayTeam( ENTITY player )
{
    Entity * e = EntTeamInternal(player, 0, NULL);

    if (e)
        SetBloodyBayTeamInternal( e );
}

void RunBloodyBayTick()
{
    int i, n;
    n = NumEntitiesInTeam(ALL_PLAYERS);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(ALL_PLAYERS, i, NULL);

        if (e)
            SetBloodyBayTeamInternal(e);
    }
}


// *********************************************************************************
/// script init functions
// *********************************************************************************
extern ScriptFunction * g_currScriptFunction;
void SetScriptName( STRING name )
{
    if (!g_currScriptFunction)
    {
        ScriptError("SetScriptVar called outside initialization");
        return;
    }
    g_currScriptFunction->name = name;
}
void SetScriptFunc( void * func )
{
    if (!g_currScriptFunction)
    {
        ScriptError("SetScriptVar called outside initialization");
        return;
    }
    g_currScriptFunction->function = func;
}
void SetScriptFuncToRunOnInitialization( void * func )
{
    if (!g_currScriptFunction)
    {
        ScriptError("SetScriptVar called outside initialization");
        return;
    }
    g_currScriptFunction->functionToRunOnInitialization = func;
}
void SetScriptType( NUMBER scriptType )
{
    if (!g_currScriptFunction)
    {
        ScriptError("SetScriptVar called outside initialization");
        return;
    }
    g_currScriptFunction->type = scriptType;
}
void SetScriptVar( STRING var, STRING defaultVal, NUMBER flags )
{    
    ScriptParam * param = 0;
    int i;

    if (!g_currScriptFunction)
    {
        ScriptError("SetScriptVar called outside initialization");
        return;
    }

    if( !var )
    {
        FatalErrorf("Couldn't SetScriptVar");
        return;
    }

    for( i = 0 ; i < MAX_SCRIPT_PARAMS ; i++ )
    {
        if( !g_currScriptFunction->params[i].param )
        {
            param = &g_currScriptFunction->params[i];
            break;
        }
    }

    if( !param )
    {
        FatalErrorf("Exceeded MAX_SCRIPT_PARAMS");
        return;
    }
    param->param = var;
    param->defaultvalue = defaultVal;
    param->flags = flags;
}

void SetScriptSignal( STRING displayStr, STRING signalStr )
{    
    ScriptSignal * signal = 0;
    int i;

    if (!g_currScriptFunction)
    {
        ScriptError("SetScriptVar called outside initialization");
        return;
    }

    if( !displayStr || !signalStr )
    {
        FatalErrorf("Invalid param to SetScriptSignal");
        return;
    }

    for( i = 0 ; i < MAX_SCRIPT_SIGNALS ; i++ )
    {
        if( !g_currScriptFunction->signals[i].signal )
        {
            signal = &g_currScriptFunction->signals[i];
            break;
        }
    }

    if( !signal )
    {
        FatalErrorf("Error: exceeded MAX_SCRIPT_SIGNALS");
        return;
    }

    signal->display = displayStr;
    signal->signal  = signalStr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// G L O W I E   P L A C E M E N T   F U N C T I O N S
//////////////////////////////////////////////////////////////////////////////////////////////////

static ScriptVarsTable g_scriptGlowieVars = {0};
static int ScriptGlowieCount = 0;
static StashTable ScriptGlowieDeathList = NULL;

// returns true if interact should not be allowed
int ScriptGlowieInteract(Entity* player, MissionObjectiveInfo* info, int success)
{
    Entity *npc = erGetEnt(info->objectiveEntity);

     return ScriptClosureListCall(&npc->onClickHandler, npcClicked, player, npc);
}

void ScriptGlowieComplete(Entity* player, MissionObjectiveInfo* info, int success)
{
    Entity *npc = erGetEnt(info->objectiveEntity);

     ScriptClosureListCall(&info->onCompleteHandler, npcClicked, player, npc);

    if (info->reusable)
        MissionItemSetState(info, MOS_INITIALIZED);
}

void ScriptGlowiesTick()
{
    StashTableIterator iter;
    StashElement elem;
    int results;

    // clean up glowies
    if (ScriptGlowieDeathList) {
        stashGetIterator(ScriptGlowieDeathList, &iter);
        while (stashGetNextElement(&iter, &elem))
        {
            if (stashElementGetInt(elem) < SecondsSince2000())
            {
                entFree((void*)stashElementGetKey(elem));
                stashAddressRemoveInt(ScriptGlowieDeathList, stashElementGetKey(elem), &results);
            }
        }
    }
}

int ScriptGlowiesPlaced()
{
    return ScriptGlowieCount;
}

GLOWIEDEF GlowieCreateDef(STRING name, 
                          STRING model,
                            STRING InteractBeginString,
                            STRING InteractInterruptedString,
                            STRING InteractCompleteString,
                            STRING InteractActionString,
                            NUMBER delay)
{
    MissionObjective *def = malloc(sizeof(MissionObjective));

    memset(def, 0, sizeof(MissionObjective));
    def->count                        = 0;
    def->interactOutcome            = MIO_NONE;
    def->locationtype                = LOCATION_TROPHY;

    def->name                        = "Glowie";
    def->description                = "Glowies";
    def->singulardescription        = "Find Glowie";
    def->model                        = strdup(model);
    def->modelDisplayName            = strdup(name);
    def->filename                    = strdup(Script_GetCurrentBlame());

    def->effectInactive                = strdup(MISSION_ITEM_FX);
    def->effectRequires                = NULL;
    def->effectActive                = NULL;
    def->effectCooldown                = NULL;
    def->effectCompletion            = NULL;
    def->effectFailure                = NULL;

    def->interactBeginString        = strdup(InteractBeginString);
    def->interactInterruptedString    = strdup(InteractInterruptedString);
    def->interactCompleteString        = strdup(InteractCompleteString);
    def->interactActionString        = strdup(InteractActionString);
    def->interactWaitingString        = "";
    def->interactResetString        = "";
    def->interactDisallowString        = "";

    def->forceFieldVillain            = NULL;

    def->level = 1;
    def->interactDelay = delay;
    def->forceFieldRespawn = 0;
 
    return (void *) def;
}

void GlowieAddEffect(GLOWIEDEF glowdef, ScriptGlowieEffectType type, STRING fx)
{
    MissionObjective *def = (MissionObjective *) glowdef;
    if (glowdef)
    {
        switch (type)
        {
        case kGlowieEffectInactive:
            def->effectInactive = strdup(fx);
            break;
        case kGlowieEffectActive:
            def->effectActive = strdup(fx);
            break;
        case kGlowieEffectCooldown:
            def->effectCooldown = strdup(fx);
            break;
        case kGlowieEffectCompletion:
            def->effectCompletion = strdup(fx);
            break;
        case kGlowieEffectFailure:
            def->effectFailure = strdup(fx);
            break;
        default:
            break;
        }
    }
}

void GlowieSetDescriptions(GLOWIEDEF glowdef, STRING description, STRING singulardescription)
{
    MissionObjective *def = (MissionObjective *) glowdef;
    def->description = description;
    def->singulardescription = singulardescription;
}

void GlowieSetActivateRequires(GLOWIEDEF glowdef, STRING requiresParam)
{
    MissionObjective *def = (MissionObjective *) glowdef;
    mSTRING required;
    int i;
    int argc;
    char *argv[100];

    required = StringCopySafe(requiresParam);
    argc = tokenize_line(required, argv, 0);
    if (argc > 0)
    {
        eaCreate(&def->activateRequires);
        for (i = 0; i < argc; i++)
        {
            eaPush(&def->activateRequires, strdup(argv[i]));
        }
    }
}

void GlowieSetCharRequires(GLOWIEDEF glowdef, STRING requiresParam, STRING failText)
{
    MissionObjective *def = (MissionObjective *) glowdef;
    mSTRING required;
    int i;
    int argc;
    char *argv[100];

    required = StringCopySafe(requiresParam);
    argc = tokenize_line(required, argv, 0);
    if (argc > 0)
    {
        eaCreate(&def->charRequires);
        for (i = 0; i < argc; i++)
        {
            eaPush(&def->charRequires, strdup(argv[i]));
        }
    }

    def->charRequiresFailedText = failText;
}

ENTITY GlowiePlace(GLOWIEDEF glowdef, LOCATION position, NUMBER reusable, PlayerInteractWithNPCOrGlowieFunc click, PlayerInteractWithNPCOrGlowieFunc done)
{
    return GlowiePlaceEx(glowdef, position, reusable, click, NULL, done, NULL);
}

ENTITY GlowiePlaceEx(GLOWIEDEF glowdef, LOCATION position, NUMBER reusable, PlayerInteractWithNPCOrGlowieFunc click, STRING lua_click,
                        PlayerInteractWithNPCOrGlowieFunc done, STRING lua_done)
{
    MissionObjective *def = (MissionObjective *) glowdef;
    Entity* e = NULL;
    MissionObjectiveInfo* info;
    Mat4 mat;
    int state;

    // setting var table
    MissionItemSetVarsTable(&g_scriptGlowieVars);

    
    GetMatFromLocation( position, mat, 0 );

    // create the entity
    if (def->model)
        e = npcCreate(def->model, ENTTYPE_MISSION_ITEM);
    if( !e )
    {
        e = villainCreateByName(def->model, 1, NULL, false, NULL, 0, NULL);
        if ( e )
            e->notAttackedByJustAnyone = 1;
    }
    if (!e)
    {
        Errorf("Invalid costume or villainDef specified %s specified for script glowie, not placing", def->model);
        return 0;
    }

    // set the position
    entUpdateMat4Instantaneous(e, mat);

    // Attach mission objective information.
    info = MissionObjectiveInfoCreate();
    info->def = def;
    def->count++;
    info->name = strdup(info->def->name);
    info->script = 1;
    info->reusable = reusable;
    e->missionObjective = info;
    info->objectiveEntity = erGetRef(e);

    // Assign the item a name
//    name = saUtilTableLocalize(info->def->modelDisplayName, &g_raidvars);
    if (info->def->modelDisplayName)
    {
        strncpy(e->name, info->def->modelDisplayName, MAX_NAME_LEN);
        e->name[MAX_NAME_LEN-1] = 0;
    }

    // add to mission info table - we can find it back by our trophy flag later
    eaPush(&objectiveInfoList, info);
    MissionWaypointCreate(info);


    state = MOS_INITIALIZED;
    if (def->activateRequires && !(MissionObjectiveExpressionEval(def->activateRequires)))
    {
        state = MOS_REQUIRES; //Initialized, but not glowing (yet)
    }
    /* BUGME - this needs a Character * to work
    if (def->charRequires && !chareval_requires(pchar, *def->charRequires);
    {
        state = MOS_REQUIRES; //Initialized, but not glowing (yet)
    }
    */
    MissionItemSetState(info, state);


    aiInit(e, NULL);

    if(!lua_click)
        SetScriptClosure(&e->onClickHandler, g_currentScript->id, click, NULL);
    else if(lua_click)
        SetScriptClosure(&e->onClickHandler, g_currentScript->id, lua_callbackEntityEntity, lua_click);

    if(!lua_done)
        SetScriptClosure(&info->onCompleteHandler, g_currentScript->id, done, NULL);
    else if(lua_done)
        SetScriptClosure(&info->onCompleteHandler, g_currentScript->id, lua_callbackEntityEntity, lua_done);

    ScriptGlowieCount++;
    return EntityNameFromEnt(e);
}

Entity *ZowiePlace(GLOWIEDEF glowdef, Mat4 pos, NUMBER reusable)
{
    MissionObjective *def = (MissionObjective *) glowdef;
    Entity* e = NULL;
    MissionObjectiveInfo* info;
    int state;

    def->effectInactive = "";    //kill zowie sounds.

    // setting var table
    MissionItemSetVarsTable(&g_scriptGlowieVars);

    // create the entity
    if (def->model)
        e = npcCreate(def->model, ENTTYPE_MISSION_ITEM);
    if( !e )
    {
        e = villainCreateByName(def->model, 1, NULL, false, NULL, 0, NULL);
        if ( e )
            e->notAttackedByJustAnyone = 1;
    }
    if (!e)
    {
        Errorf("Invalid costume or villainDef specified %s specified for script glowie, not placing", def->model);
        return 0;
    }

    // set the position
    entUpdateMat4Instantaneous(e, pos);

    // Attach mission objective information.
    info = MissionObjectiveInfoCreate();
    info->def = def;
    def->count++;
    info->name = strdup(info->def->name);
    info->zowie = 1;
    info->reusable = reusable;
    e->missionObjective = info;
    info->objectiveEntity = erGetRef(e);

    // Assign the item a name
    //    name = saUtilTableLocalize(info->def->modelDisplayName, &g_raidvars);
    if (info->def->modelDisplayName)
    {
        strncpy(e->name, info->def->modelDisplayName, MAX_NAME_LEN);
        e->name[MAX_NAME_LEN-1] = 0;
    }

    // add to mission info table - we can find it back by our trophy flag later
    eaPush(&objectiveInfoList, info);
    MissionWaypointCreate(info);


    state = MOS_INITIALIZED;
    if (def->activateRequires && !(MissionObjectiveExpressionEval(def->activateRequires)))
    {
        state = MOS_REQUIRES; //Initialized, but not glowing (yet)
    }
    MissionItemSetState(info, state);

    aiInit(e, NULL);

    ScriptGlowieCount++;
    return e;
}

int GlowieGetState(ENTITY glowie)
{
    Entity* e = EntTeamInternal(glowie, 0, NULL);
    if (e)
    {
        if (ENTTYPE(e) == ENTTYPE_MISSION_ITEM)
        {
            if (e->missionObjective)
            {
                return e->missionObjective->state;
            }
        }
    }
    return 0;
}

void GlowieSetState(ENTITY glowie)
{
    Entity* e = EntTeamInternal(glowie, 0, NULL);
    if (e)
    {
        if (ENTTYPE(e) == ENTTYPE_MISSION_ITEM)
        {
            if (e->missionObjective)
            {
                MissionItemSetState(e->missionObjective, MOS_INITIALIZED);
            }
        }
    }
}

void GlowieClearState(ENTITY glowie)
{
    Entity* e = EntTeamInternal(glowie, 0, NULL);
    if (e)
    {
        if (ENTTYPE(e) == ENTTYPE_MISSION_ITEM)
        {
            if (e->missionObjective)
            {
                MissionItemSetState(e->missionObjective, MOS_SUCCEEDED);
            }
        }
    }
}

static void GlowieClean(int idx)
{
    MissionObjectiveInfo *pInfo = objectiveInfoList[idx];
    MissionObjective *def = cpp_const_cast(MissionObjective*)(pInfo->def); // Mutable because it was allocated by GlowieCreateDef()
    int i;

    // clean up any players that are interacting with this item
    if (pInfo->interactPlayerRef)
    {
        Entity *pPlayer = erGetEnt(pInfo->interactPlayerRef);

        if(pPlayer)
        {
            // Tell the player that the timer has stopped.
            MissionItemSetClientTimer(pInfo, 0.0);

            // Unlink the entity from the objective.
            pPlayer->interactingObjective = NULL;
        }
    }

    // update def count
    if (def) {
        // probably move to MissionObjectiveInfo
        def->count--;
        if (def->count <= 0)
        {
            if (def->model)
                free(def->model);
            if (def->modelDisplayName)
                free(def->modelDisplayName);
            if (def->filename)
                free(def->filename);
            if (def->interactBeginString)
                free(def->interactBeginString);
            if (def->interactInterruptedString)
                free(def->interactInterruptedString);
            if (def->interactCompleteString)
                free(def->interactCompleteString);
            if (def->interactActionString)
                free(def->interactActionString);
            if (def->effectInactive)
                free(def->effectInactive);
            if (def->effectActive)
                free(def->effectActive);
            if (def->effectCooldown)
                free(def->effectCooldown);
            if (def->effectCompletion)
                free(def->effectCompletion);
            if (def->effectFailure)
                free(def->effectFailure);
            if (def->activateRequires)
            {
                for (i = eaSize(&def->activateRequires) - 1; i >= 0; i--)
                {
                    free(def->activateRequires[i]);
                }
                eaDestroy(&def->activateRequires);
            }
            free(def);
        }
    }

    objectiveInfoList[idx]->def = NULL;
    objectiveInfoList[idx]->state = MOS_REMOVE;

}


void GlowieRemove(ENTITY glowie)
{
    Entity* e = EntTeamInternal(glowie, 0, NULL);
    if (e)
    {
        if (ENTTYPE(e) == ENTTYPE_MISSION_ITEM)
        {
            int idx = eaFind(&objectiveInfoList, e->missionObjective);
            if (idx != -1) {
                GlowieClean(idx);
            }

            if (!ScriptGlowieDeathList)
                ScriptGlowieDeathList = stashTableCreateAddress(10);

            stashAddressAddInt(ScriptGlowieDeathList, e, SecondsSince2000() + 1, true);
            e->missionObjective = NULL;


//            modStateBits(e, ENT_DEAD, e->seq->type->ticksToLingerAfterDeath, TRUE );//entFree(e);
            ScriptGlowieCount--;
        }
    }
}

extern void DestroyAllItemObjectives(void); 


void GlowieClearAll(void)
{

    int size = eaSize(&objectiveInfoList);
    int i;

    for (i = 0; i < size; i++)
    {
        if (objectiveInfoList[i]->script)
        {
            GlowieClean(i);
        }
    }

    if(ScriptGlowieDeathList)
    {
        stashTableDestroy(ScriptGlowieDeathList);
        ScriptGlowieDeathList = NULL;
    }

    ScriptGlowieCount = 0;
}

static int sSky1;
static int sSky2;
static F32 sSeconds = 0.0f;
static F32 sOrigSeconds;

void ScriptSkyFileSetFade(NUMBER sky1, NUMBER sky2, FRACTION seconds)
{
    sSky1 = sky1;
    sSky2 = sky2;
    if (seconds < 0.0f)
    {
        seconds = 1.0f;
    }
    sSeconds = seconds;
    sOrigSeconds = seconds;
}

void ScriptSkyFadeTick(F32 timestep)
{
    float lerp;

    if (sSeconds > 0.0f)
    {
        sSeconds -= timestep / 30.0f;
        if (sSeconds < 0.0f)
        {
            sSeconds = 0.0f;
        }
        // I'm not entirely convinced it's possible to reach here with sOrigSeconds == 0.0.
        // However people hate it when mapservers crash w/ divide zero errors
        if (sOrigSeconds)
        {
            lerp = sSeconds / sOrigSeconds;
            // yet more probably unnecessary defensive coding.
            if (lerp > 1.0f)
            {
                lerp = 1.0f;
            }
            else if (lerp < 0.0f)
            {
                lerp = 0.0f;
            }
            serverSetSkyFade(sSky1, sSky2, lerp);
        }
        else
        {
            lerp = 0.0f;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// U I    W R A P P E R   F U N C T I O N S
//////////////////////////////////////////////////////////////////////////////////////////////////

/*
int deUICreate()
{
    //return scriptUICreate(NULL);
    return 0;
}

void deUIdestroy(NUMBER ID)
{
//    scriptUIDestroy(ID);
}

void deUIEntStartUsing(NUMBER ID, ENTITY ent)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if (e)
    {
//        scriptUIEntStartUsing(ID, e);
    }
}

void deUIEntStopUsing(NUMBER ID, ENTITY ent)
{
    Entity* e = EntTeamInternal(ent, 0, NULL);
    if (e)
    {
//        scriptUIEntStopUsing(ID, e);
    }
}

void deUIAddTitleBar(NUMBER ID, STRING titleText, STRING infoText)
{
//    scriptUIAddTitleBar(ID, titleText, infoText);
}

void deUIAddCollectNItems(NUMBER ID,STRING itemImage,STRING itemBackground,STRING magic)
{
    //scriptUIAddCollectNItems(ID, itemImage, itemBackground, magic);
}

void deUIAddTimer(NUMBER ID, STRING text, FRACTION time)
{
//    scriptUIAddTimer(ID, 5, 0, (int) (time * 60.0f), text, "", "");
}//
*/

void GiveBadgeCredit( ENTITY player, ScriptBadgeType type )
{
    Entity * pPlayer = EntTeamInternal(player, 0, NULL);

    if (pPlayer) {
        switch (type)
        {
            case kScriptBadgeTypePresents:
                badge_RecordPresentsCollected(pPlayer, 1);
                break;
            case kScriptBadgeTypeRockets:
                badge_RecordRocketsLaunched(pPlayer, 1);
                break;
            case kScriptBadgeTypeRVPillbox:
                badge_RecordRVPillbox(pPlayer, 1);
                break;
            case kScriptBadgeTypeRVHeavyPet:
                badge_RecordRVHeavyPet(pPlayer, 1);
                break;
            case kScriptBadgeTypeBBOreConvert:
                badge_RecordBBOreConvert(pPlayer, 1);
                break;
            case kScriptBadgeTypeRWZBombPlace:
                badge_RecordRWZBombPlace(pPlayer, 1);
                break;
            default:
                // don't know... don't do anything
                break;
        }
    }
}

void GiveBadgeStat(TEAM team, STRING badgeStat, NUMBER count)
{
    int n;
    int i;
    Entity *e;

    n = NumEntitiesInTeam(team);
    if (badgeStat && badgeStat[0])
    {
        for (i = 0; i < n; i++)
        {
            e = EntTeamInternal(team, i, NULL);
            if (e)
            {
                badge_StatAdd(e, badgeStat, count);
            }
        }
    }
}

//TO DO this is very clunky, just add callback to death
void CountTowardGhostTrapBadge( ENTITY deadGhost )
{
    Entity * eVictim;
    int j;

    eVictim = EntTeamInternal(deadGhost, 0, NULL);

    if( eVictim && eVictim->pchar && eVictim->pchar->erLastDamage )
    {
        Entity * e = erGetEnt( eVictim->pchar->erLastDamage );
        if( e )
        {
            if(e->teamup_id)
            {
                Entity *eNearbyTeammate = NULL;
                // If there's an actual team see if anyone is close
                // enough to the victim get the credit.
                for(j=0; j<e->teamup->members.count; j++)
                {
                    Entity *teammate = entFromDbId(e->teamup->members.ids[j]);
                    if(teammate
                        && distance3Squared(ENTPOS(teammate), ENTPOS(eVictim))<=SQR(200))
                    {
                        eNearbyTeammate = teammate;
                        break;
                    }
                }

                // This updates for everyone on the team.
                if(eNearbyTeammate)
                {
                    for(j=0; j<e->teamup->members.count; j++)
                    {
                        Entity *teammate = entFromDbId(e->teamup->members.ids[j]);
                        if(teammate)
                            badge_RecordGhostTrapped(teammate);
                    }
                }
            }
            else
            {
                // An army of one.
                if(distance3Squared(ENTPOS(e), ENTPOS(eVictim))<=SQR(200))
                {
                    badge_RecordGhostTrapped(e);
                }
            }
        }
    }
}


//TO DO waypoints?
//Look for an encounter near but not too near that player
ENCOUNTERGROUP LaunchGenericWaveAttack( LOCATION target, STRING layout, STRING spawnName, STRING shout )
{
    ENCOUNTERGROUP group = LOCATION_NONE; 

    if( !spawnName )
        spawnName = GetGenericMissionSpawn();

    if( !layout )
        layout = "mission";

    if( !target || StringEqual(target, LOCATION_NONE ) ) 
        target = MissionOwner();

    if( !target || StringEqual(target, TEAM_NONE ) )
        target = GetEntityFromTeam(ALL_PLAYERS, 0);

    group = FindEncounter( 
        target,                                //Relative to this spot
        100,                                //No Nearer than this
        600,                                //No Farther than this
        0,                                    //Inside this AREA
        0,                                    //Assigned this MissionEncounter (the LogicalName)
        layout,                                //Layout (Many Named Demon. Called "EncounterSpawn" in Spawndef files and in Maps, "Layouts" in mission specs and scripts and often throughout the code, "Spawntype" in SpawnDef struct, "EncounterLayouts" in mapspec)
        0,                                    //Group Name
        SEF_UNOCCUPIED_ONLY | SEF_HAS_PATH_TO_LOC //Flags
        );

    if( !StringEqual( group, LOCATION_NONE ) )
    {
        //printf("LaunchGenericWaveAttack() is spawning...\n");
        group = CloneEncounter(group);
        Spawn( 1, "WaveAttackers", spawnName, group, layout, MissionLevel(), 0 );   
        SetEncounterInactive(group);
        AddAIBehavior(group, "Combat");
        SetAIMoveTarget( group, target, LOW_PRIORITY, 0, 1, 0.0f );
        if( shout && shout[0] )
            MinionSays( group, shout );
        DetachSpawn( "WaveAttackers" );
    }

    return group;
}

void ScriptDBLog(char *basis, ENTITY player, char *fmt, ...)
{
    va_list args;
    char buffer[4096];

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    buffer[sizeof(buffer) - 1] = 0;
    va_end(args);

    dbLog(basis, EntTeamInternal(player, 0, NULL), "%s", buffer);
}

// This is probably a disposable routine
void SendMessageToPlayer( ENTITY player, char *fmt, ... )
{
    Entity *e;
    va_list args;
    char buffer[4096];

    e = EntTeamInternal(player, 0, NULL);
    if (e)
    {
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        buffer[sizeof(buffer) - 1] = 0;
        va_end(args);

        sendInfoBox(e, INFO_REWARD, "%s", buffer);
    }
}

void TeamSZERewardLogMsg(TEAM team, STRING eventName, STRING message)
{
    int i, n;

    n = NumEntitiesInTeam(team);
    for (i = 0; i < n; i++)
    {
        Entity* e = EntTeamInternal(team, i, NULL);
        LOG_ENT( e, LOG_SZE_REWARDS, LOG_LEVEL_IMPORTANT, 0, "SZE:%s %s", eventName, message );
    }
}