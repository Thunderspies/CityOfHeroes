/*\
 *
 *    missiondef.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *     functions to handle static mission defs, and
 *    verify them on load
 *
 */

#include "mission.h"
#include "storyarcprivate.h"
#include "Reward.h"
#include "dbcomm/staticMapInfo.h"
#include "storyarc/missionMapCommon.h"
#include "entity/CustomVillainGroup.h"
// *********************************************************************************
//  Load and parse mission defs
// *********************************************************************************

// villain pacing
StaticDefineInt ParseVillainPacingEnum[] =
{
    DEFINE_INT
    { "Flat",                PACING_FLAT },
    { "SlowRampUp",            PACING_SLOWRAMPUP },
    { "SlowRampDown",        PACING_SLOWRAMPDOWN },
    { "Staggered",            PACING_STAGGERED },
    { "FrontLoaded",        PACING_FRONTLOADED },
    { "BackLoaded",            PACING_BACKLOADED },
    { "HighNotoriety",        PACING_HIGHNOTORIETY },
    { "Unmodifiable",        PACING_UNMODIFIABLE },
    DEFINE_END
};

StaticDefineInt ParseMissionInteractOutcomeEnum[] =
{
    DEFINE_INT
    { "None",            MIO_NONE },
    { "Remove",            MIO_REMOVE },
    DEFINE_END
};

StaticDefineInt ParseDayNightCycleEnum[] =
{
    DEFINE_INT
    { "Normal",            DAYNIGHT_NORMAL },
    { "AlwaysNight",    DAYNIGHT_ALWAYSNIGHT },
    { "AlwaysDay",        DAYNIGHT_ALWAYSDAY },
    { "FastCycle",        DAYNIGHT_FAST },
    DEFINE_END
};

StaticDefineInt ParseMissionFlags[] =
{
    DEFINE_INT
    { "NoTeleportOnComplete",    MISSION_NOTELEPORTONCOMPLETE },
    { "NoBaseHospital",            MISSION_NOBASEHOSPITAL },
    { "CoedTeamsAllowed",        MISSION_COEDTEAMSALLOWED },
    { "PraetorianTutorial",        MISSION_PRAETORIAN_TUTORIAL },
    { "CoUniverseTeamsAllowed",    MISSION_COUNIVERSETEAMSALLOWED },
    DEFINE_END
};

StaticDefineInt ParseMissionObjectiveFlags[] =
{
    DEFINE_INT
    { "ShowWaypoint",            MISSIONOBJECTIVE_SHOWWAYPOINT },
    { "RewardOnCompleteSet",    MISSIONOBJECTIVE_REWARDONCOMPLETESET },
    DEFINE_END
};

// mission objectives
TokenizerParseInfo ParseMissionObjective[] = {
    { "{",                    TOK_START,        0 },
    { "",TOK_STRUCTPARAM |    TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,name,0) },

    { "GroupName",            TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,groupName,0) },
    
    { "Filename",            TOK_CURRENTFILE(MissionObjective, filename) },

    { "ObjectiveName",TOK_REDUNDANTNAME | TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,name,0) },
    { "Model",                TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,model,0) },
    { "EffectInActive",        TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,effectInactive,0) },
    { "EffectRequires",        TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,effectRequires,0) },
    { "EffectActive",        TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,effectActive,0) },
    { "EffectCooldown",        TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,effectCooldown,0) },
    { "EffectCompletion",    TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,effectCompletion,0) },
    { "EffectFailure",        TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,effectFailure,0) },

    { "Description",                    TOK_STRING(MissionObjective, description, 0)                        },
    { "SingularDescription",            TOK_STRING(MissionObjective, singulardescription, 0)                },
    { "DescRequires",                    TOK_STRINGARRAY(MissionObjective, descRequires)                        },
    { "ActivateOnObjectiveComplete",    TOK_STRINGARRAY(MissionObjective, activateRequires)                    },
    { "CharRequires",                    TOK_STRINGARRAY(MissionObjective, charRequires)                        },
    { "CharRequiresFailedText",            TOK_STRING(MissionObjective, charRequiresFailedText, 0)                },
    { "ModelDisplayName",                TOK_STRING(MissionObjective, modelDisplayName, 0)                    },
    { "Number",                            TOK_INT(MissionObjective, count, 1)                                    },
    { "Reward",                            TOK_STRUCT(MissionObjective, reward, ParseStoryReward)                },
    { "Level",                            TOK_INT(MissionObjective, level, 0) }, // optional, can override the mission level - used for force fields currently
    { "Flags",                            TOK_FLAGS(MissionObjective, flags, 0), ParseMissionObjectiveFlags    }, 

    { "InteractDelay",            TOK_INT(MissionObjective,interactDelay,0) },
    { "SimultaneousObjective",    TOK_BOOLFLAG(MissionObjective,simultaneousObjective,0) },
    { "InteractBeginString",    TOK_STRING(MissionObjective,interactBeginString,0) },
    { "InteractCompleteString", TOK_STRING(MissionObjective,interactCompleteString,0) },
    { "InteractInterruptedString", TOK_STRING(MissionObjective,interactInterruptedString,0) },
    { "InteractActionString",    TOK_STRING(MissionObjective,interactActionString,0) },
    { "InteractResetString",    TOK_STRING(MissionObjective,interactResetString,0) },
    { "InteractWaitingString",    TOK_STRING(MissionObjective,interactWaitingString,0) },
    { "InteractOutcome",        TOK_INT(MissionObjective,interactOutcome, MIO_NONE), ParseMissionInteractOutcomeEnum },

    { "ForceFieldVillain",    TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,forceFieldVillain,0) },
    { "ForceFieldRespawn",    TOK_INT(MissionObjective,forceFieldRespawn,0) },
    { "ForceFieldObjective",TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,forceFieldObjective,0) },

    { "LocationName",        TOK_NO_TRANSLATE | TOK_STRING(MissionObjective,locationName,0) },
    { "Location",            TOK_INT(MissionObjective,locationtype,LOCATION_ITEMFLOOR),    ParseLocationTypeEnum },
    { "Room",TOK_REDUNDANTNAME | TOK_INT(MissionObjective,room,MISSION_OBJECTIVE),    ParseMissionPlaceEnum },
    { "MissionPlacement",    TOK_INT(MissionObjective,room,MISSION_OBJECTIVE),    ParseMissionPlaceEnum },
    { "SkillCheck",            TOK_BOOLFLAG(MissionObjective,skillCheck,0) },
    { "ScriptControlled",    TOK_BOOLFLAG(MissionObjective,scriptControlled,0) },

    { "ObjectiveReward",    TOK_REDUNDANTNAME | TOK_STRUCT(MissionObjective,reward,ParseStoryReward) },
    { "}",                    TOK_END,            0},
    { "", 0, 0 }
};

// mission objective sets
TokenizerParseInfo ParseMissionObjectiveSet[] = {
    { "",    TOK_STRUCTPARAM | TOK_NO_TRANSLATE | TOK_STRINGARRAY(MissionObjectiveSet,objectives) },
    { "\n",    TOK_END,    0 },
    { "", 0, 0 }
};

// mission key doors
TokenizerParseInfo ParseMissionKeyDoor[] = {
    { "",    TOK_STRUCTPARAM | TOK_NO_TRANSLATE | TOK_STRING(MissionKeyDoor,name,0) },
    { "",    TOK_STRUCTPARAM | TOK_NO_TRANSLATE | TOK_STRINGARRAY(MissionKeyDoor,objectives) },
    { "\n",    TOK_END,    0 },
    { "", 0, 0 }
};

extern StaticDefineInt ParseVillainType[];

TokenizerParseInfo ParseMissionDef[] = {
    { "{",                    TOK_START,        0},
    { "",                    TOK_CURRENTFILE(MissionDef,filename) },
    { "EntryString",        TOK_STRING(MissionDef,entrytext,0) },
    { "ExitStringFail",        TOK_STRING(MissionDef,exittextfail,0) },
    { "ExitStringSuccess",    TOK_STRING(MissionDef,exittextsuccess,0) },
    { "DefeatAllText",        TOK_STRING(MissionDef,pchDefeatAllText,0) },
    { "MapFile",            TOK_FILENAME(MissionDef,mapfile,0) },
    { "MapSet",                TOK_INT(MissionDef,mapset,MAPSET_NONE),    ParseMapSetEnum },
    { "MapLength",            TOK_INT(MissionDef,maplength,0),    },
    { "VillainGroup",        TOK_NO_TRANSLATE | TOK_STRING(MissionDef,villaingroup,"VG_NONE") },//        ParseVillainGroupEnum },
    { "VillainGroupVar",    TOK_NO_TRANSLATE | TOK_STRING(MissionDef,villaingroupVar,"VG_NONE") }, // field for VG_VAR type, not yet operational
    { "VillainGroupType",    TOK_INT(MissionDef,villainGroupType,0),    ParseVillainType },
    { "Flags",                TOK_FLAGS(MissionDef,flags,0),            ParseMissionFlags },

    // door
    { "CityZone",            TOK_NO_TRANSLATE | TOK_STRING(MissionDef,cityzone,0) },        // normal name of city zone, or "WhereIssued" or "WhereIssuedIfPossible"
    { "DoorType",            TOK_NO_TRANSLATE | TOK_STRING(MissionDef,doortype,0) },
    { "LocationName",        TOK_NO_TRANSLATE | TOK_STRING(MissionDef,locationname,0) },    // used for ZoneTransfer mission doors now
    { "FoyerType",            TOK_NO_TRANSLATE | TOK_STRING(MissionDef,foyertype,0) },    // deprecated
    { "DoorNPC",            TOK_NO_TRANSLATE | TOK_STRING(MissionDef,doorNPC,0) },    
    { "DoorNPCDialog",        TOK_NO_TRANSLATE | TOK_STRING(MissionDef,doorNPCDialog,0) },
    { "DoorNPCDialogStart",    TOK_NO_TRANSLATE | TOK_STRING(MissionDef,doorNPCDialogStart,0) },

    // Side Missions
    { "NumSideMissions",    TOK_INT(MissionDef,numSideMissions,0),    },
    { "SideMission",        TOK_NO_TRANSLATE | TOK_STRINGARRAY(MissionDef,sideMissions),    },

    { "VillainPacing",        TOK_INT(MissionDef,villainpacing,PACING_FLAT),    ParseVillainPacingEnum },
    { "TimeToComplete",        TOK_INT(MissionDef,missiontime,0) },
    { "RandomNPCs",            TOK_INT(MissionDef,numrandomnpcs,0) },
    { "MissionLevel",        TOK_INT(MissionDef, missionLevel, -1) },

    { "SpawnDef",            TOK_STRUCT(MissionDef,spawndefs,ParseSpawnDef) },
    { "Objective",            TOK_STRUCT(MissionDef,objectives,ParseMissionObjective) },
    { "CanCompleteWith",    TOK_STRUCT(MissionDef,objectivesets,ParseMissionObjectiveSet) },
    { "CanFailWith",        TOK_STRUCT(MissionDef,failsets,ParseMissionObjectiveSet) },

    // specialty stuff
    { "Race",                TOK_NO_TRANSLATE | TOK_STRING(MissionDef,race,0) },
    { "DayNightCycle",        TOK_INT(MissionDef,daynightcycle,DAYNIGHT_NORMAL),    ParseDayNightCycleEnum },
    { "KeyDoor",            TOK_STRUCT(MissionDef,keydoors,ParseMissionKeyDoor) },
    { "Script",                TOK_STRUCT(MissionDef,scripts,ParseScriptDef) },
    { "KeyClueDef",            TOK_STRUCT(MissionDef,keyclues,ParseStoryClue) },
    { "CustomVGIdx",        TOK_INT(MissionDef, CVGIdx, 0)    },

    { "FogDist",            TOK_F32(MissionDef, fFogDist, 0 ) },
    { "FogColor",            TOK_VEC3(MissionDef, fogColor) },

    { "ShowItemsOnMinimap",       TOK_INT(MissionDef, showItemThreshold, 1) },
    { "ShowCrittersOnMinimap", TOK_INT(MissionDef, showCritterThreshold, 1) },

    { "LoadScreenPages",    TOK_STRINGARRAY(MissionDef, loadScreenPages) },

    { "}",                    TOK_END,            0},
    SCRIPTVARS_STD_PARSE(MissionDef)
    { "", 0, 0 }
};

typedef struct MissionDefList
{
    MissionDef** missiondefs;
} MissionDefList;

TokenizerParseInfo ParseMissionDefList[] = {
    { "MissionDef",        TOK_STRUCT(MissionDefList,missiondefs,ParseMissionDef) },
    { "", 0, 0 }
};

MissionDefList g_missiondeflist = {0};

int AllObjectivesSimultaneous(const MissionDef* mission, const char* objectivename)
{
    int numobj, curobj;

    numobj = eaSize(&mission->objectives);
    for (curobj = 0; curobj < numobj; curobj++)
    {
        if (stricmp(mission->objectives[curobj]->name, objectivename)) continue;
        if (!mission->objectives[curobj]->simultaneousObjective) return 0;
    }
    return 1;
}

// just make sure the objective names we were given make sense
static int VerifyObjectiveSet(const MissionDef* mission, const char* logicalname, const char** objectives)
{
    int curspec, numspec;
    int curobj, numobj;
    int curspawn, numspawns;
    int ret = 1;

    numspec = eaSize(&objectives);
    for (curspec = 0; curspec < numspec; curspec++)
    {
        const char* spec = objectives[curspec];
        int found = 0;
        if (!stricmp(spec, "DefeatAllVillains"))
            found = 1;
        else if (!stricmp(spec, "DefeatAllVillainsInEndRoom"))
            found = 1;
        else if (!stricmp(spec, "EachPlayerLeavesMap"))
            found = 1;
        else if (!stricmp(spec, "ScriptControlled")) //Completion of entire mision is handles by the script
            found = 1;
        else if (!strnicmp(spec, "Script", 6) ) //If it's a script fulfilled objective, dont try to verify it
            found = 1;
        else
        {
            numobj = eaSize(&mission->objectives);
            for (curobj = 0; curobj < numobj; curobj++)
            {
                if (!stricmp(mission->objectives[curobj]->name, spec))
                {
                    found = 1;
                    break;
                }
            } // each a
            numspawns = eaSize(&mission->spawndefs);
            for (curspawn = 0; curspawn < numspawns; curspawn++)
            {
                if (cutscene_has_objective(mission->spawndefs[curspawn]->cutSceneDefs, spec))
                {
                    found = 1;
                    break;
                }

                if (!mission->spawndefs[curspawn]->objective)
                    continue;
                if (!mission->spawndefs[curspawn]->objective[0]->name) 
                    continue;
                if (!stricmp(mission->spawndefs[curspawn]->objective[0]->name, spec))
                {
                    found = 1;
                    break;
                }
            } // each spawn
        }

        // now we should have found the specification one way or another
        if (!found)
        {
            ErrorFilenamef(mission->filename, "Invalid mission objective specification: %s, mission %s", spec, logicalname);
            ret = 0;
        }
    } // each spec
    // MAK - we can't modify data once we get to here, so just let the screwed up spec go with a warning
    return ret;
}

// verify information for each mission
int VerifyAndProcessMissionDefinition(MissionDef* mission, const char* logicalname, int minlevel, int maxlevel)
{
    int ret = 1;
    int badspec = 0;
    int curset, numsets;
    int curobj, numobj;
    int i, n;

    // check mission strings
    saUtilVerifyContactParams(mission->entrytext, mission->filename, 1, 1, 0, 0, 0, 0);
    if (!VerifyMissionScripts(mission, logicalname)) ret = 0;

    // check spawns
    n = eaSize(&mission->spawndefs);
    for (i = 0; i < n; i++)
    {
        if (!VerifySpawnDef(mission->spawndefs[i], 1))
            ret = 0;
    }

    // check mission door location
    if (mission->doortype) 
    {
        if (!strnicmp(mission->doortype, "ZoneTransfer", 12 /*strlen("ZoneTransfer") */))
        {
            if (mission->cityzone)
            {
                ErrorFilenamef(mission->filename, "Mission %s can't specify a particular zone for a ZoneTransfer door", logicalname);
                ret = 0;
            }
        }
        else // normal mission door
        {
            if (mission->cityzone && stricmp(mission->cityzone, "WhereIssued") && stricmp(mission->cityzone, "WhereIssuedIfPossible")
                && stricmp(mission->cityzone, "AnyPrimalHero") && stricmp(mission->cityzone, "AnyPrimalVillain"))
            {
                const MapSpec* spec = MapSpecFromMapName(mission->cityzone);
                if (!spec)
                {
                    ErrorFilenamef(mission->filename, "Mission %s specifies an invalid city zone %s for the door", logicalname, mission->cityzone);
                    ret = 0;
                }
                else
                {
                    int i, n = 0;
                    int found = 0;
                    if (spec->missionDoorTypes) n = eaSize(&spec->missionDoorTypes);
                    for (i = 0; i < n; i++)
                    {
                        if (!stricmp(mission->doortype, spec->missionDoorTypes[i]))
                        {
                            found = 1;
// We are now okay with a mission specifying a map whose mission level range is invalid.
// Mission level range is now only applicable to "Any", "AnyPrimalHero", "AnyPrimalVillain".                            
//                             if (!MapSpecCanHaveMission(spec, minlevel)) 
//                             {
//                                 ErrorFilenamef(mission->filename, "Mission %s specifies a door in city zone %s, but this zone isn't accessible at level %i",
//                                     logicalname, mission->cityzone, minlevel);
//                                 ret = 0;
//                             }
//                             else if (!MapSpecCanHaveMission(spec, maxlevel))
//                             {
//                                 ErrorFilenamef(mission->filename, "Mission %s specifies a door in city zone %s, but this zone isn't accessible at level %i",
//                                     logicalname, mission->cityzone, maxlevel);
//                                 ret = 0;
//                             }
                            break;
                        }
                    }
                    if (!found)
                    {
                        ErrorFilenamef(mission->filename, "Mission %s specifies a %s door in city zone %s, but this doesn't exist", 
                            logicalname, mission->doortype, mission->cityzone);
                        ret = 0;
                    }
                }
            } 
            else // didn't specify a city zone
            {
                // our strategy is to check to see the door is available every 5 levels within the level range
                int level;
                if (minlevel < 1) minlevel = 1;
                if (maxlevel < minlevel || maxlevel > MAX_PLAYER_SECURITY_LEVEL) maxlevel = MAX_PLAYER_SECURITY_LEVEL;
                for (level = minlevel; level <= maxlevel; level += 5)
                {
                    if (!VerifyCanAssignMissionDoor(mission->doortype, level))
                    {
                        ErrorFilenamef(mission->filename, "Mission %s (levels %i-%i) can't get a %s door at level %i from any city zone",
                            logicalname, minlevel, maxlevel, mission->doortype, level);
                        ret = 0;
                    }
                }
            }
        }
    } else if (mission->doorNPC) {
        // validate door NPC exists
        const char* target = mission->doorNPC;
        int m, numm = eaSize(&mapSpecList.mapSpecs);
        int found = false;
        for (m = 0; m < numm; m++)
        {
            const MapSpec* spec = mapSpecList.mapSpecs[m];
            found = StringArrayFind(spec->persistentNPCs, target);
            if (found)
            {
                if (minlevel < spec->entryLevelRange.min)
                {
                    ErrorFilenamef(mission->filename, "Task %s requests delivery to %s, but this target is in city %s and not available at level %i",
                                        logicalname, target, spec->mapfile, minlevel);
                    ret = 0;
                }
                break;
            }
        } // each map
        if (!found)
        {
            ErrorFilenamef(mission->filename, "Task %s requests delivery to %s, but this target does not exist",
                                    logicalname, target);
        }
    } else {
        ErrorFilenamef(mission->filename, "Mission %s doesn't specify a door", logicalname);
        ret = 0;
    }


    if (mission->doortype 
        && !strnicmp(mission->doortype, "ZoneTransfer", 12 /*strlen("ZoneTransfer")*/)
        && !mission->locationname)
    {
        ErrorFilenamef(mission->filename, "Mission %s is a ZoneTransfer mission, but is missing LocationName", logicalname);
    }

    // check all objective rewards
    numobj = eaSize(&mission->objectives);
    for (curobj = 0; curobj < numobj; curobj++)
    {
        const MissionObjective* obj = mission->objectives[curobj];
        if (eaSize(&obj->reward) > 1) // verify not more than one reward
        {
            ErrorFilenamef(mission->filename, "More than one reward given for objective %s, mission %s", obj->name, logicalname);
            badspec = 1;
        }

        // verify objective strings
        saUtilVerifyContactParams(obj->interactActionString, mission->filename, 1, 1, 0, 0, 0, 0);
        saUtilVerifyContactParams(obj->interactBeginString, mission->filename, 1, 1, 0, 0, 0, 1);
        saUtilVerifyContactParams(obj->interactCompleteString, mission->filename, 1, 1, 0, 0, 0, 1);
        saUtilVerifyContactParams(obj->interactWaitingString, mission->filename, 1, 1, 0, 0, 0, 0);
        saUtilVerifyContactParams(obj->interactResetString, mission->filename, 1, 1, 0, 0, 0, 1);
        saUtilVerifyContactParams(obj->interactInterruptedString, mission->filename, 1, 1, 0, 0, 0, 1);
        saUtilVerifyContactParams(obj->modelDisplayName, mission->filename, 0, 0, 0, 0, 0, 0);
        saUtilVerifyContactParams(obj->description, mission->filename, 0, 0, 0, 0, 0, 0);
        saUtilVerifyContactParams(obj->singulardescription, mission->filename, 0, 0, 0, 0, 0, 0);

        // verify the right strings exist
        if (obj->simultaneousObjective)
        {
            if (obj->locationtype == LOCATION_PERSON) ErrorFilenamef(mission->filename, "Simultaneous objective %s is marked as a person objective", obj->name);
            if (!AllObjectivesSimultaneous(mission, obj->name)) ErrorFilenamef(mission->filename, "One %s objective is simultaneous, but not all are", obj->name);
            if (!obj->interactWaitingString) ErrorFilenamef(mission->filename, "Simultaneous objective %s doesn't have a waiting string", obj->name);
            if (!obj->interactResetString) ErrorFilenamef(mission->filename, "Simultaneous objective %s doesn't have a reset string", obj->name);
        }
        if (obj->locationtype == LOCATION_PERSON)
        {
            if (!obj->interactCompleteString) ErrorFilenamef(mission->filename, "Person objective %s doesn't have a complete string", obj->name);
        }
        else // objective item
        {
            if (!obj->interactCompleteString) ErrorFilenamef(mission->filename, "Item objective %s doesn't have a complete string", obj->name);
            if (!obj->interactBeginString) ErrorFilenamef(mission->filename, "Item objective %s doesn't have a begin string", obj->name);
            if (!obj->interactActionString) ErrorFilenamef(mission->filename, "Item objective %s doesn't have an action string", obj->name);
            if (!obj->interactInterruptedString) ErrorFilenamef(mission->filename, "Item objective %s doesn't have an interrupted string", obj->name);
        }

        // verify internals of reward
        rewardVerifyErrorInfo("Mission objective reward error", mission->filename);
        rewardVerifyMaxErrorCount(1);
        if (obj->reward)
        {
            // fpe merge comment 7/9/11:  using rewardSetsVerifyAndProcess instead of VerifyRewardBlock because the latter looks to contain bad logic, may need to revisit
            //badspec = VerifyRewardBlock(obj->reward[0], "objective reward", mission->filename, 1, 0, 1) && badspec;
            if (!rewardSetsVerifyAndProcess(cpp_const_cast(RewardSet**)(obj->reward[0]->rewardSets)))
                badspec = 1;

            // verify that contact points aren't given here
            if (obj->reward[0]->contactPoints)
            {
                ErrorFilenamef(mission->filename, "Contact points given for objective %s, missing %s", obj->name, logicalname);
                badspec = 1;
            }
        }
    }

    // check all objectivesets - verify that we can make sense of the objective specified
    numsets = eaSize(&mission->objectivesets);
    for (curset = 0; curset < numsets; curset++)
    {
        if (!VerifyObjectiveSet(mission, logicalname, mission->objectivesets[curset]->objectives))
            badspec = 0;
    } 
    numsets = eaSize(&mission->failsets);
    for (curset = 0; curset < numsets; curset++)
    {
        if (!VerifyObjectiveSet(mission, logicalname, mission->failsets[curset]->objectives))
            badspec = 0;
    }
    numsets = eaSize(&mission->keydoors);
    for (curset = 0; curset < numsets; curset++)
    {
        if (!VerifyObjectiveSet(mission, logicalname, mission->keydoors[curset]->objectives))
            badspec = 0;
    }

    // verify placement of objectives and spawns
    if (!VerifyAndProcessMissionPlacement(mission, logicalname))
        ret = 0;

    // verify we don't have too many key clues
    if (eaSize(&mission->keyclues) > KEYCLUES_PER_MISSION)
    {
        ErrorFilenamef(mission->filename, "Too many key clues for mission %s: has %i but only %i allowed", 
            logicalname, eaSize(&mission->keyclues), KEYCLUES_PER_MISSION);
        badspec = 1;
    }

    // Check the exit mission strings
    saUtilVerifyContactParams(mission->exittextfail, mission->filename, 1, 0, 0, 0, 0, 1);
    saUtilVerifyContactParams(mission->exittextsuccess, mission->filename, 1, 0, 0, 0, 0, 1);

    return !badspec && ret;
}

void MissionPreload()
{
    MissionMapPreload();
    MissionSpawnPreload();
}

int MissionIsZoneTransfer(const MissionDef* def)
{
    if (!def || !def->doortype) return 0;
    return strnicmp(def->doortype, "ZoneTransfer", 12 /*strlen("zonetransfer")*/ ) == 0;
}


