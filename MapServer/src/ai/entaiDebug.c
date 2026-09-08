
#include <utilitieslib/stdtypes.h>
#include "entaiprivate.h"
#include "entaiPriorityPrivate.h"
#include "entaiPriority.h"
#include "entaiScript.h"
#include "entaiBehaviorCoh.h"
#include "ailib/aiStruct.h"
#include "ailib/aiBehaviorDebug.h"
#include "ai/entai.h"
#include "ai/entaiLog.h"

#include "beacon/beaconPath.h"
#include "beacon/beaconPrivate.h"
#include "beacon/beaconDebug.h"
#include "beacon/beaconConnection.h"

#include "seq/AnimBitList.h"
#include "entity/entserver.h"
#include "entity/entPlayer.h"
#include "player/clientEntityLink.h"
#include "generator/encounterPrivate.h"
#include "entity/character_base.h"
#include "entity/powers.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "gameComm/Npc.h"
#include "svr/svr_player.h"
#include "entity/motion.h"
#include "gameComm/VillainDef.h"
#include "cmdparse/entcon.h"
#include "dbcomm/dbcomm.h"
#include "dbcomm/staticMapInfo.h"
#include "language/langServerUtil.h"
#include "cmdparse/cmdcontrols.h"
#include "gameComm/debugCommon.h"
#include "generator/entgen.h"
#include "seq/seqstate.h"
#include "seq/seq.h"
#include "entity/entity.h"
#include <utilitieslib/utils/utils.h>
#include "comm_game.h"
#include "cmdparse/cmdserver.h"
#include "storyarc/storyarcprivate.h"
#include "NovodeX/NwWrapper.h"
#include "ClickToSourceFlags.h"
#include "group/group.h"
#include "entity/boostset.h"
#include "bases/DetailRecipe.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include "storyarc/missionMapCommon.h"
#include "entity/incarnate.h"

typedef struct EntSendLine
{
    Vec3 p1, p2;
    int argb1, argb2;
} EntSendLine;

typedef struct EntSendText
{
    Vec3    pos;
    char    text[50];
    int        flags;
} EntSendText;

static struct {
    EntSendLine*    lines;
    int                count;
    int                maxCount;
} ent_send_lines;

static struct {
    EntSendText*    texts;
    int                count;
    int                maxCount;
} ent_send_texts;

typedef struct AILogFlagToName{
    AILogFlag    flagBit;
    char*        name;
}AILogFlagToName;

AILogFlagToName aiLogFlagToName[] = {
    { AI_LOG_BASIC,                "Basic" },
    { AI_LOG_PATH,                "Path" },
    { AI_LOG_WAYPOINT,            "Waypoint" },
    { AI_LOG_COMBAT,            "Combat" },
    { AI_LOG_POWERS,            "Powers" },
    { AI_LOG_TEAM,                "Team" },
    { AI_LOG_FEELING,            "Feeling" },
    { AI_LOG_SPECIAL,            "Special" },
    { AI_LOG_PRIORITY,            "Priority" },
    { AI_LOG_CAR,                "Car" },
    { AI_LOG_THINK,                "Think" },
    { AI_LOG_ENCOUNTER,            "Encounter" },
    { AI_LOG_SEQ,                "Sequencer" },
    { AI_LOG_BEHAVIOR,            "Behavior"},
};

StaticDefineInt PriorityFlags[];

void aiResetEntSend(){
    ent_send_lines.count = 0;
    ent_send_texts.count = 0;
}

void aiEntSendAddLine(const Vec3 p1, int argb1, Vec3 p2, int argb2){
    EntSendLine* line = dynArrayAddStruct(    &ent_send_lines.lines,
                                            &ent_send_lines.count,
                                            &ent_send_lines.maxCount);

    copyVec3(p1, line->p1);
    line->argb1 = argb1;

    copyVec3(p2, line->p2);
    line->argb2 = argb2;
}

void aiEntSendAddText(Vec3 pos, const char* textString, int flags){
    EntSendText* text = dynArrayAddStruct(    &ent_send_texts.texts,
                                            &ent_send_texts.count,
                                            &ent_send_texts.maxCount);
    int len;

    len = (int)strlen(textString);

    copyVec3(pos, text->pos);
    strncpy(text->text, textString, min(len, ARRAY_SIZE(text->text) - 1) + 1);
    text->flags = flags;
}

void aiEntSendMatrix(Mat4 mat, const char* textString, int flags)
{
    int idx;
    Vec3 p1;
    Vec3 p1x;
    F32 rad = 1;
    int colors[3];

    colors[0] = 0xff000000;
    colors[1] = 0xff00ffff;
    colors[2] = 0xff0000ff;

    for( idx = 0 ; idx < 3 ; idx++ )
    {
        zeroVec3( p1 ); 
        p1[idx] += rad;
        mulVecMat4( p1, mat, p1x );
        aiEntSendAddLine(p1x, colors[idx], mat[3], colors[idx]);
    }

    if( textString )
        aiEntSendAddText(mat[3], textString, 0);

}

static void aiGetClippedString(const char* inString, int maxLength, char* outString){
    const char* curPos = inString;
    int curLength = maxLength;

    while(curLength-- && *curPos)
        *outString++ = *curPos++;

    while(curLength < maxLength && isspace((unsigned char)outString[-1]))
        outString--;

    *outString = 0;
}


static const char* aiGetGroupFlags(int groupFlags)
{
    static char* buff;
    static struct {
        int *flag;
        char *desc;
    } mapping[] = {
        {&AI_POWERGROUP_BASIC_ATTACK,    "BasicAttack"    },
        {&AI_POWERGROUP_FLY_EFFECT,        "FlyFx"            },
        {&AI_POWERGROUP_RESURRECT,        "Resurrect"        },
        {&AI_POWERGROUP_BUFF,            "Buff"            },
        {&AI_POWERGROUP_DEBUFF,            "DeBuff"        },
        {&AI_POWERGROUP_ROOT,            "Root"            },
        {&AI_POWERGROUP_VICTOR,            "Victor"        },
        {&AI_POWERGROUP_POSTDEATH,        "PostDeath"        },
        {&AI_POWERGROUP_HEAL_SELF,        "HealSelf"        },
        {&AI_POWERGROUP_TOGGLEHARM,        "ToggleHarm"    },
        {&AI_POWERGROUP_TOGGLEAOEBUFF,    "ToggleAoEBuff"    },
        {&AI_POWERGROUP_HEAL_ALLY,        "HealAlly"        },
        {&AI_POWERGROUP_SUMMON,            "Summon"        },
        {&AI_POWERGROUP_KAMIKAZE,        "Kamikaze"        },
        {&AI_POWERGROUP_PLANT,            "Plant"            },
        {&AI_POWERGROUP_TELEPORT,        "Teleport"        },
        {&AI_POWERGROUP_EARLYBATTLE,    "EarlyBattle"    },
        {&AI_POWERGROUP_MIDBATTLE,        "MidBattle"        },
        {&AI_POWERGROUP_ENDBATTLE,        "EndBattle"        },
        {&AI_POWERGROUP_FORCEFIELD,        "ForceField"    },
        {&AI_POWERGROUP_HELLIONFIRE,    "HellionFire"    },
        {&AI_POWERGROUP_PET,            "Pet"            },
        {&AI_POWERGROUP_SECONDARY_TARGET,    "SecondaryTarget"        },
    };
    int i;

#define BUFF_SIZE 1000
    if(!buff)
        buff = malloc(BUFF_SIZE);

    buff[0]=0;
    for (i=0; i<ARRAY_SIZE(mapping); i++) {
        if (groupFlags & *mapping[i].flag) {
            groupFlags -= *mapping[i].flag;
            if (*mapping[i].flag & AI_BUFF_FLAGS) {
                strcat(buff, "^2");
            } else if (*mapping[i].flag & AI_DEBUFF_FLAGS) {
                strcat(buff, "^1");
            } else {
                strcat(buff, "^8");
            }
            strcat(buff, mapping[i].desc);
            if (groupFlags) {
                strcat(buff, "^0|");
            }
        }
    }
    if (groupFlags)
        strcatf_s(buff, BUFF_SIZE, "^4%d", groupFlags);

    return buff;
}
#undef BUFF_SIZE

static const char* aiGetPowerInfoText(Entity* e, AIVars* ai, AIPowerInfo* info){
    static char* buffer;
    char* pos;
    char timerText[200] = "";
    Character* pchar = e->pchar;
    Power* power = info->power;
    
    if(!buffer){
        buffer = malloc(1000);
    }
    
    pos = buffer;

    if(power->fTimer){
        char* cur = timerText;

        cur += sprintf(cur, ", ^%s%1.1f^0s", power->fTimer ? "r" : "4", power->fTimer);

        if(    power == pchar->ppowCurrent &&
            power->fTimer < power->ppowBase->fInterruptTime)
        {
            cur += sprintf(    cur,
                            " ^rINT ^4%1.1f^0s ^2^b%d/15b^0",
                            power->ppowBase->fInterruptTime - power->fTimer,
                            (int)(15 * (power->ppowBase->fInterruptTime - power->fTimer) / power->ppowBase->fInterruptTime));
        }
    }
    else if(power->ppowBase->fInterruptTime){
        sprintf(timerText,
                ", int ^4%1.1f^0s",
                power->ppowBase->fInterruptTime);
    }

    if(info->doNotChoose.duration && ABS_TIME_SINCE(info->doNotChoose.startTime) < info->doNotChoose.duration){
        sprintf(timerText + strlen(timerText),
                ", ^1doNotChoose ^r%1.1f^0s",
                ABS_TO_SEC((F32)(info->doNotChoose.duration - ABS_TIME_SINCE(info->doNotChoose.startTime))));
    }

    pos += sprintf(    pos,
                    "%s%s%s%s^0(^s^4%d^0:^4%s^0,^4%1.1f^0ft,^4%1.1f^0p,^4%1.1f^0op,^4%1.1f^0cp,^4%1.1f^0tc%s%s^n),",
                    ai->doNotChangePowers && info == ai->power.current ? "^1KEEP:" : "",
                    pchar->ppowCurrent == power ? "^7Cur:" :
                        pchar->ppowQueued == power ? "^9Queue:" :
                            "",
                    power->bActive ? "^r" :
                        info== ai->power.current ? "^2" :
                            info== ai->power.preferred ? "^1" :
                                power->fTimer ? "^9" :
                                    "^5",
                    power->ppowBase->pchName,
                    info->stanceID,
                    aiGetGroupFlags(info->groupFlags),
                    aiGetPowerRange(pchar, info),
                    info->power->ppowBase->fPreferenceMultiplier,
                    info->critterOverridePreferenceMultiplier,
                    info->debugPreference,
                    info->debugTargetCount,
                    info->power->bDontSetStance ? ", DontSetStance" :"",
                    timerText);

    return buffer;
}

static const char* getEncounterStateName(EGState state){
    switch (state){
        case EG_ASLEEP:        return "Asleep";  //Starting state, and after it has been cleaned up.
                                              //MISSION: if not in tray, will never change from this state
        case EG_AWAKE:        return "Awake";   //
        case EG_WORKING:    return "Working"; //Entities spawned
        case EG_INACTIVE:    return "Inactive";  //Entities set to INACTIVE  (triggered by player proximity check, usually)
        case EG_ACTIVE:        return "Active";    //Entities set to ACTIVE (triggerd by some ent in ecounter acquiring you as a target, usually)
        case EG_COMPLETED:    return "Completed"; //You have completed the encounter -- it will get cleaned up 
        case EG_OFF:        return "Off";
    }
    return "ERROR";
}

static const char* getActorStateName(ActorState state){
    switch(state){
        case ACT_WORKING:
            return "Working";
        case ACT_INACTIVE:
            return "Inactive";
        case ACT_ACTIVE:
            return "Active";
        case ACT_ALERTED:
            return "Alerted";
        case ACT_COMPLETED:
            return "Completed";
        case ACT_SUCCEEDED:
            return "Succeeded";
        case ACT_FAILED:
            return "Failed";
        case ACT_THANK:
            return "Thank";
        default:
            return "Unknown";
    }
}

static const char* getEntTypeName(EntType entType){
    switch(entType){
        case ENTTYPE_CAR:
            return "CAR";
        case ENTTYPE_DOOR:
            return "DOOR";
        case ENTTYPE_NPC:
            return "NPC";
        case ENTTYPE_CRITTER:
            return "CRITTER";
        case ENTTYPE_PLAYER:
            return "PLAYER";
        case ENTTYPE_HERO:
            return "HERO";
        case ENTTYPE_MOBILEGEOMETRY:
            return "MOBILEGEOM";
        case ENTTYPE_MISSION_ITEM:
            return "MISSION_ITEM";
        default:
            return "Other";
    }
}

static const char* getCamelCaseEntTypeName(EntType entType){
    switch(entType){
        case ENTTYPE_CAR:
            return "Car";
        case ENTTYPE_DOOR:
            return "Door";
        case ENTTYPE_NPC:
            return "NPC";
        case ENTTYPE_CRITTER:
            return "Critter";
        case ENTTYPE_PLAYER:
            return "Player";
        case ENTTYPE_HERO:
            return "Hero";
        case ENTTYPE_MOBILEGEOMETRY:
            return "MobileGeom";
        case ENTTYPE_MISSION_ITEM:
            return "MissionItem";
        default:
            return "Other";
    }
}

// This function is called from sendEntity to get the info crap that appears over an entity's head.

void aiGetEntDebugInfo(Entity* e, ClientLink* client, Packet* pak){
    Entity* controlledEntity = clientGetControlledEntity(client);
    AIVars* ai = ENTAI(e);
    Character tempCharacter;
    Character* pchar = e->pchar ? e->pchar : &tempCharacter;
    char debugInfoString[100000];
    char tempBuffer[10000];
    char* tempBufferPos;
    char* curPos = debugInfoString;
    int sendExtraInfo = 0;
    EntType entType = ENTTYPE(e);
    int i;
    int entDebugInfo = client->entDebugInfo;

    // Clear the temp Character struct.

    ZeroStruct(&tempCharacter);
    tempCharacter.entParent = e;

    // Clear the debug info string.

    debugInfoString[0] = '\0';

    // Get the type-specific info.

    #define GET_BOOL_STRING(x) (x ? "^2TRUE" : "^1FALSE")
    #define PRINT curPos += sprintf
    #define SECTION_TITLE(title) PRINT(curPos, "^9^l50l ^7"title" ^9^l250l\n")

    if(    (entType == ENTTYPE_NPC && (entDebugInfo & ENTDEBUGINFO_NPC)) ||
        (entType == ENTTYPE_CRITTER && (entDebugInfo & ENTDEBUGINFO_CRITTER)) ||
        (e != controlledEntity && entType == ENTTYPE_PLAYER && (entDebugInfo & ENTDEBUGINFO_PLAYER)) ||
        (e == controlledEntity && (entDebugInfo & ENTDEBUGINFO_SELF)) ||
        (entType == ENTTYPE_DOOR && (entDebugInfo & ENTDEBUGINFO_DOOR)) ||
        (entType == ENTTYPE_CAR && (entDebugInfo & ENTDEBUGINFO_CAR)) ||
        (entType == ENTTYPE_MISSION_ITEM && (entDebugInfo & ENTDEBUGINFO_NPC)))
    {
        sendExtraInfo = 1;

        SECTION_TITLE("START OF INFO");

        // First lines.

        if(e->access_level > 0){
            PRINT(curPos, "^r**** ACCESS LEVEL %d ****\n", e->access_level);
        }

        if(ENTPAUSED(e)){
            PRINT(curPos, "^2**** ENTITY PAUSED ****\n");
        }

        if(e->pchar && e->pchar->attrCur.fHitPoints <= 0){
            PRINT(curPos, "^1**** I AM DEAD ****\n");
        }

        if(ENTSLEEPING(e)){
            PRINT(curPos, "^2**** ENTITY SLEEPING ****\n");
        }

        // Next line.

        PRINT(curPos, "^3%s", getEntTypeName(entType));

        PRINT(curPos, "^dSvrID ^4%d^d, ", entGetId(e));

        if(entType == ENTTYPE_PLAYER){
            PRINT(curPos, "DB ^4%d^d, ", e->db_id);
        }

        PRINT(curPos, "Name: %s", AI_LOG_ENT_NAME(e));

        PRINT(    curPos,
                " ^d(Lvl ^4%d^d/^4%d ^d%s%s%s^2%s(rank %s)^d)",
                pchar->iLevel + 1,
                pchar->iCombatLevel + 1,
                e->villainDef ? "^1" : "",
                e->villainDef ? e->villainDef->name : "",
                e->villainDef ? " ^d/ " : "",
                pchar->pclass ?
                    strnicmp(pchar->pclass->pchName, "class_", 6) == 0 ?
                        pchar->pclass->pchName + 6 :
                        pchar->pclass->pchName :
                    "",
                e->villainDef ? StaticDefineIntRevLookup(ParseVillainRankEnum, e->villainDef->rank) : "none"
                        );

        PRINT(    curPos,
                "^d, Svr Tick: ^4%s\n",
                getCommaSeparatedInt(global_state.global_frame_count));

        if(controlledEntity){
            float yaw1, yaw2;
            Vec3 toMe;

            subVec3(ENTPOS(controlledEntity), ENTPOS(e), toMe);
            yaw1 = getVec3Yaw(ENTMAT(e)[2]);
            yaw2 = getVec3Yaw(toMe);

            PRINT(    curPos,
                    "Relative To You: ^4%1.2f^0ft(-^4%1.2f^0-^4%1.2f^0=^4%1.2f^0) away, ^4%1.2f ^0height diff, ^4%1.2f ^0deg in FOV\n",
                    distance3(ENTPOS(e), ENTPOS(controlledEntity)),
                    ai->collisionRadius,
                    ENTAI(controlledEntity)->collisionRadius,
                    distance3(ENTPOS(e), ENTPOS(controlledEntity)) -
                        ai->collisionRadius -
                        ENTAI(controlledEntity)->collisionRadius,
                    ENTPOSY(e) - ENTPOSY(controlledEntity),
                    fabs(DEG(subAngle(yaw1, yaw2))));
        }

        if(    entType == ENTTYPE_PLAYER &&
            entDebugInfo & ENTDEBUGINFO_PLAYER_ONLY &&
            e->client)
        {
            F32 timestep_acc = 0;
            F32 timestep_noscale_acc = 0;
            ControlState* controls = &e->client->controls;

            SECTION_TITLE("PLAYER-ONLY");

            PRINT(    curPos,
                    "sendrad: ^4%1.f^0, "
                    "phys.acc: ^4%1.2f^0, "
                    "playback.ms_acc: ^4%1.0f^0, "
                    "csc_count: ^4%d^0\n"
                    "hit_end: ^4%d^0, "
                    "ms_total: ^4%d^0, "
                    "err_corr: ^4%d^0"
                    "\n",
                    sqrt(e->client->ent_send.max_radius_SQR),
                    controls->phys.acc,
                    controls->time.playback.ms_acc,
                    controls->csc_list.count,
                    controls->connStats.hit_end_count,
                    controls->time.total_ms,
                    controls->net_error_correction);

            if(e->client->link)
            {
                PRINT(    curPos,
                        "Net(s/r): bps: ^4%1.f^d/^4%1.f^d, pps: ^4%1.1f^d/^4%1.1f^d\n"
                        "   total: ^4%d^0/^4%d^d, lost: ^4%d^0/^4%d^d, dupes: ^4%d^d, ping: ^4%d^dms\n",
                        e->client->link->sendHistory.lastByteSum / e->client->link->sendHistory.lastTimeSum,
                        e->client->link->recvHistory.lastByteSum / e->client->link->recvHistory.lastTimeSum,
                        ARRAY_SIZE(e->client->link->sendHistory.times) / e->client->link->sendHistory.lastTimeSum,
                        ARRAY_SIZE(e->client->link->recvHistory.times) / e->client->link->recvHistory.lastTimeSum,
                        e->client->link->totalPacketsSent,
                        e->client->link->totalPacketsRead,
                        e->client->link->lost_packet_sent_count,
                        e->client->link->lost_packet_recv_count,
                        e->client->link->duplicate_packet_recv_count,
                        pingAvgRate(&e->client->link->pingHistory));
            }

            PRINT(curPos, "^t70t");

            for(i = 0; i < min(50, ARRAY_SIZE(e->client->controls.controlLog.entries)); i++)
            {
                int size = ARRAY_SIZE(e->client->controls.controlLog.entries);
                int index = (e->client->controls.controlLog.cur - i - 1 + size) % size;
                ControlLogEntry* entry = e->client->controls.controlLog.entries + index;
                ControlLogEntry* lastEntry = e->client->controls.controlLog.entries + (index + size - 1) % size;
                struct tm* theTime = localtime(&entry->actualTime);
                char* timeString = asctime(theTime);

                if(!entry->actualTime)
                    continue;

                timeString[strlen(timeString)-1] = 0;

                PRINT(    curPos,
                        "%2.2d:\t^%d%s\t^%d%d(%d)\t",
                        index,
                        entry->actualTime % 10,
                        timeString,
                        (entry->mmTime / 1000) % 10,
                        entry->mmTime,
                        entry->mmTime - lastEntry->mmTime);

                PRINT(    curPos,
                        "^%d%1.3f",
                        (int)(timestep_acc / 30) % 10,
                        entry->timestep);

                if(entry->timestep != entry->timestep_noscale){
                    PRINT(    curPos,
                            "^0/^%d%1.3f",
                            (int)(timestep_noscale_acc / 30) % 10,
                            entry->timestep_noscale);
                }

                PRINT(    curPos,
                        "  %I64u",
                        entry->qpc - (i ? lastEntry->qpc : 0));

                if(i == 0 || entry->qpcFreq != lastEntry->qpcFreq){
                    PRINT(    curPos,
                            "(%I64u)",
                            entry->qpcFreq);
                }

                PRINT(curPos, "\n");

                timestep_acc += entry->timestep;
                timestep_noscale_acc += entry->timestep_noscale;
            }
#if NOVODEX
            if (nx_state.initialized)
            {
                PRINT(
                    curPos,
                    "Novodex: meshShapes: ^4%d^0 actorSpores: ^4%d^0 staticActors: ^4%d^0\n          dynamicActors: ^4%d^0 scenes(ragdolls): ^4%d^0\n",
                    nx_state.meshShapeCount,
                    nx_state.actorSporeCount,
                    nx_state.staticActorCount,
                    nx_state.dynamicActorCount,
                    nx_state.sceneCount
                    );
            }
#endif
        }

        if(!(entDebugInfo & ENTDEBUGINFO_DISABLE_BASIC)){
            SECTION_TITLE("BASIC INFO");

            PRINT(    curPos,
                    "Pos: (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0), "
                    "Spawn: (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0), "
                    "Dir: (^4%1.1f^0,^4%1.1f^0,^4%1.1f^0), "
                    "^4%1.2f^0ft\n",
                    ENTPOSPARAMS(e),
                    vecParamsXYZ(ai->spawn.pos),
                    vecParamsXYZ(ENTMAT(e)[2]),
                    distance3(ENTPOS(e), ai->spawn.pos));

            PRINT(    curPos,
                    "HP: ^s^4%1.1f^0/^4%1.1f ^n^2^b%d/25b^0, "
                    "End: ^s^4%1.1f^0/^4%1.1f ^n^5^b%d/25b^0, "
                    "Spd: ^4%1.1f^0ft/s(^slim:^4%1.1f^0^n), "
                    "Jmp: ^4%1.1f^0%s, "
                    "Vis: ^4%1.0f^0(^4%1.0f^0)+^4%1.0f^0ft^0@^4%1.0f^0deg, "
                    "LOD: ^4%1.0f^0ft, "
                    "Dist: ^4%d^0ft"
                    "\n",
                    pchar->attrCur.fHitPoints, pchar->attrMax.fHitPoints,
                    pchar->attrMax.fHitPoints ? (int)ceil(25 * pchar->attrCur.fHitPoints / pchar->attrMax.fHitPoints) : 0,
                    pchar->attrCur.fEndurance, pchar->attrMax.fEndurance,
                    pchar->attrMax.fEndurance ? (int)ceil(25 * pchar->attrCur.fEndurance / pchar->attrMax.fEndurance) : 0,
                    e->motion->input.surf_mods[SURFPARAM_GROUND].max_speed * 30, ai->inputs.limitedSpeed * 30,
                    entType != ENTTYPE_PLAYER ? aiGetJumpHeight(e) : pchar->attrCur.fJumpHeight,
                    entType != ENTTYPE_PLAYER ? "ft" : "x",
                    pchar->attrCur.fPerceptionRadius, ai->overridePerceptionRadius,
                    ai->targetedSightAddRange,
                    DEG(ai->fov.angle),
                    e->seq->type->fade[1],
                    (int)e->dist_history.total);

            {
                char buffer[1000];
                char* pos = buffer;

                if(!ai->teamMemberInfo.alerted)
                    pos += sprintf(pos, "^1NotAlerted^0, ");

                if(ai->canFly)
                    pos += sprintf(pos, "^2CanFly^0, ");

                if(ai->isFlying)
                    pos += sprintf(pos, "^2IsFlying^0, ");

                if(e->noPhysics)
                    pos += sprintf(pos, "^2NoPhysics^0, ");

                if(e->invincible)
                    pos += sprintf(pos, "^2Invincible^0%s, ", e->invincible & INVINCIBLE_DBFLAG ? "(^1db^0)" : "");

                if(e->unstoppable)
                    pos += sprintf(pos, "^2Unstoppable^0, ");

                if(pchar->attrCur.fUntouchable)
                    pos += sprintf(pos, "^2Untouchable^0, ");

                if(e->alwaysHit)
                    pos += sprintf(pos, "^2Always Hit^0, ");

                if(ENTHIDE(e))
                    pos += sprintf(pos, "^2Invisible^0%s, ", ENTHIDE(e) & 2 ? "(^1db^0)" : "");

                if(e->untargetable)
                    pos += sprintf(pos, "^2Untargetable^0%s, ", e->untargetable & UNTARGETABLE_DBFLAG ? "(^1db^0)" : "");

                if(ai->doNotChangePowers)
                    pos += sprintf(pos, "^2DoNotChangePowers^0, ");

                if(ai->doNotMove)
                    pos += sprintf(pos, "^2DoNotMove^0, ");

                if(ai->doNotUsePowers)
                    pos += sprintf(pos, "^2DoNotUsePowers^0, ");

                if(pchar->attrCur.fHeld > 0)
                    pos += sprintf(pos, "^2Held^0, ");

                if(pchar->attrCur.fImmobilized > 0)
                    pos += sprintf(pos, "^2Immobilized^0, ");

                if(pchar->attrCur.fStunned > 0)
                    pos += sprintf(pos, "^2Stunned^0, ");

                if(character_IsConfused(pchar) > 0) 
                    pos += sprintf(pos, "^2Confused^0, ");

                if(pchar->attrCur.fSleep > 0)
                    pos += sprintf(pos, "^2Sleeping^0, ");

                if(ai->isAfraidFromAI) 
                    pos += sprintf(pos, "^2AfraidFromAI^0, ");

                if(ai->isAfraidFromPowers) 
                    pos += sprintf(pos, "^2AfraidFromPowers^0, ");

                if(ai->isTerrorized)
                    pos += sprintf(pos, "^2Terrorized^0, ");

                if(ai->teamMemberInfo.underGriefedHPRatio)
                    pos += sprintf(pos, "^2UnderGriefHP^0, ");

                if(ai->isGriefed)
                    pos += sprintf(pos, "^2Griefed^0, ");

                if(pchar && pchar->iAllyID==kAllyID_Good)
                    pos += sprintf(pos, "^2Hero^0, ");

                if(pchar && pchar->iAllyID==kAllyID_Evil)
                    pos += sprintf(pos, "^2Monster^0, ");

                if(pchar && pchar->iAllyID==kAllyID_Foul)
                    pos += sprintf(pos, "^2Villain^0, ");

                if(pchar && pchar->iAllyID==kAllyID_Villain2)
                    pos += sprintf(pos, "^2Villain2^0, ");

                if(pchar && pchar->iGangID)
                    pos += sprintf(pos, "^2Gang^0:^4%d^0, ", pchar->iGangID);

                if(pchar && pchar->iAllyID!=kAllyID_Evil && pchar->iAllyID!=kAllyID_Good && pchar->iAllyID!=kAllyID_Foul && pchar->iAllyID!=kAllyID_Villain2)
                {
                    pos += sprintf(pos, "^2Free Agent^0:^4%d^0, ", pchar->iAllyID);
                }

                if(pchar->bIgnoreCombatMods)
                    pos += sprintf(pos, "^2IgnoreCombatMods^0, ");

                if(e->adminFrozen)
                    pos += sprintf(pos, "^2AdminFrozen^0, ");

                if(e->doorFrozen)
                    pos += sprintf(pos, "^2DoorFrozen^0, ");

                if(e->controlsFrozen)
                    pos += sprintf(pos, "^2ControlsFrozen^0, ");

                if(e->client && e->client->controls.nocoll)
                    pos += sprintf(pos, "^2NoColl^0, ");
                    
                if( getMoveFlags(e->seq, SEQMOVE_CANTMOVE))
                    pos += sprintf(pos, "^2CantMove^0, ");

                if(!ai->findTargetInProximity)
                    pos += sprintf(pos, "^2DoNotFindTarget^0, ");

                if(ai->spawnPosIsSelf) 
                    pos += sprintf(pos, "^2SpawnPosIsSelf^0, ");

                if(!ai->considerEnemyLevel)   
                    pos += sprintf(pos, "^2DontConsiderEnemyLevel^0, ");

                if(!ai->doAttackAI)
                    pos += sprintf(pos, "^2DontDoAttackAI^0, ");

                if(ai->waitForThink)
                    pos += sprintf(pos, "^2WaitForThink^0, ");

                if(e->cutSceneActor)
                    pos += sprintf(pos, "^1CutSceneActor^0, ");

                if(e->erCreator){
                    Entity* creator = erGetEnt(e->erCreator);

                    if(creator){
                        pos += sprintf(pos, "^2Created^0(%s^0:^4%d^0), ", AI_LOG_ENT_NAME(creator), creator->owner);
                    }else{
                        pos += sprintf(pos, "^2Created^0(^1na^0), ");
                    }
                }

                if(!ai->useEnterable)
                    pos += sprintf(pos, "^1NOT UseEnterable^0, ");

                if(ai->pathLimited)
                    pos += sprintf(pos, "^2PathLimited^0, ");

                if(pos != buffer)
                    PRINT(curPos, "Flags: %s\n", buffer);
            }

            if (entType!= ENTTYPE_PLAYER)
                PRINT(curPos, "AOE Usage:(^s^4AOEPref^0 %1.2f^4 per target^0,^4 SingleTargetPref ^0%1.2f^0)\n", ai->AOEUsage.fAOEPreference, ai->AOEUsage.fSingleTargetPreference);

            if(entType != ENTTYPE_PLAYER && ai->power.count){
                int count;
                AIPowerInfo* info;

                // Print attack powers.

                tempBuffer[0] = 0;
                tempBufferPos = tempBuffer;

                for(info = ai->power.list, count = 0; info; info = info->next){
                    if(!info->isAttack)
                        continue;

                    tempBufferPos += sprintf(    tempBufferPos,
                                                "%s%s",
                                                count && !(count % 2) ? "\n^t100t\t" : "",
                                                aiGetPowerInfoText(e, ai, info));

                    count++;
                }

                if(tempBufferPos != tempBuffer){
                    PRINT(curPos, "Attack Powers: %s\n", tempBuffer);
                }

                // Print auto powers.

                if(entDebugInfo & ENTDEBUGINFO_AUTO_POWERS){
                    tempBuffer[0] = 0;
                    tempBufferPos = tempBuffer;

                    for(info = ai->power.list, count = 0; info; info = info->next){
                        Power* power = info->power;

                        if(!info->isAuto)
                            continue;

                        tempBufferPos += sprintf(    tempBufferPos,
                                                    "%s%s",
                                                    count && !(count % 2) ? "\n^t80t\t" : "",
                                                    aiGetPowerInfoText(e, ai, info));

                        count++;
                    }

                    if(tempBufferPos != tempBuffer){
                        PRINT(curPos, "Auto Powers: %s\n", tempBuffer);
                    }
                }

                // Print toggle powers.

                tempBuffer[0] = 0;
                tempBufferPos = tempBuffer;

                for(info = ai->power.list, count = 0; info; info = info->next){
                    Power* power = info->power;
                    char timerText[100] = "";

                    if(!info->isToggle)
                        continue;

                    tempBufferPos += sprintf(    tempBufferPos,
                                                "%s%s",
                                                count && !(count % 2) ? "\n^t80t\t" : "",
                                                aiGetPowerInfoText(e, ai, info));

                    count++;
                }

                if(tempBufferPos != tempBuffer){
                    PRINT(curPos, "Toggle Powers: %s\n", tempBuffer);
                }

                // Print other powers.

                tempBuffer[0] = 0;
                tempBufferPos = tempBuffer;

                for(info = ai->power.list, count = 0; info; info = info->next){
                    Power* power = info->power;
                    char timerText[100] = "";

                    if(info->isAttack || info->isAuto || info->isToggle)
                        continue;

                    tempBufferPos += sprintf(    tempBufferPos,
                                                "%s%s",
                                                count && !(count % 2) ? "\n^t80t\t" : "",
                                                aiGetPowerInfoText(e, ai, info));

                    count++;
                }

                if(tempBufferPos != tempBuffer){
                    PRINT(curPos, "Other Powers: %s\n", tempBuffer);
                }
            }

            // Encounter info.

            if(e->encounterInfo){
                assert(e->encounterInfo->parentGroup->active);
                PRINT(curPos, "SpawnDef: ^8%s\n", e->encounterInfo->parentGroup->active->spawndef->fileName);
            }
            
            // Tracker info.
            
            if(e->coll_tracker){
                PRINT(    curPos,
                        "Tracker Pos: ^4%1.3f^0, ^4%1.3f^0, ^4%1.3f\n",
                        posParamsXYZ(e->coll_tracker));
            }
        }

        if(entType != ENTTYPE_PLAYER && entDebugInfo & ENTDEBUGINFO_PATH){
            SECTION_TITLE("PATH INFO");

            if(ai->followTarget.type){
                AIFollowTargetType type = ai->followTarget.type;

                PRINT(    curPos,
                        "Follow %s/%s: (^4%1.1f^0, ^4%1.1f^0, ^4%1.1f^0), ",
                        type == AI_FOLLOWTARGET_BEACON ? "Beacon" :
                            type == AI_FOLLOWTARGET_POSITION ? "Position" :
                                type == AI_FOLLOWTARGET_ENTERABLE ? "Enterable" :
                                    type == AI_FOLLOWTARGET_MOVING_POINT ? "MovingPoint" :
                                        (type == AI_FOLLOWTARGET_ENTITY && ai->followTarget.entity) ?
                                            ENTTYPE(ai->followTarget.entity) == ENTTYPE_PLAYER ? "Player" :
                                            ENTTYPE(ai->followTarget.entity) == ENTTYPE_CRITTER ? "Critter" :
                                            ENTTYPE(ai->followTarget.entity) == ENTTYPE_NPC ? "NPC" :
                                                "Entity" :
                                        "Unknown",
                        aiGetFollowTargetReasonText(ai->followTarget.reason),
                        vecParamsXYZ(ai->followTarget.pos));

                PRINT(    curPos,
                        "Since Shortcut: ^4%d^0s%s%s^0, ",
                        ABS_TO_SEC(ABS_TIME_SINCE(ai->time.checkedForShortcut)),
                        ai->shortcutWasTaken ? " ^0- ^2SHORTCUT TAKEN" : "",
                        ai->preventShortcut ? " ^0- ^1PREVENTED" : "");

                if(    type == AI_FOLLOWTARGET_ENTITY &&
                    ai->followTarget.entity)
                {
                    PRINT(    curPos,
                            "Target: %s",
                            AI_LOG_ENT_NAME(ai->followTarget.entity));
                }
                
                PRINT(curPos, "\n");
                
            }else{
                PRINT(    curPos,
                        "Follow Target: ^1NONE\n");
            }

            PRINT(    curPos,
                    "Movement: "
                    "doNotRun=%s^0, "
                    "MoveType: ^4%d^0, "
                    "AIMotion: ^4%d^0, "
                    "\n",
                    GET_BOOL_STRING(ai->doNotRun),
                    e->move_type,
                    ai->motion.type);

            PRINT(curPos, "Standoff: ^4%1.1f^0ft^s(^4%1.1f^0)^n\n",
                ai->followTarget.standoffDistance, ai->inputs.standoffDistance);


            if(ai->navSearch){
                PRINT(    curPos, "Found Path Time: ^4%d^ds\n",
                        ABS_TO_SEC(ABS_TIME_SINCE(ai->time.foundPath)));

                if(ai->navSearch->path.head){
                    NavPath* path = &ai->navSearch->path;

                    if(path->curWaypoint){
                        NavPathWaypoint* waypoint = path->curWaypoint;
                        Vec3 pos;
                        Beacon* beacon = waypoint->beacon;

                        if(beacon){
                            copyVec3(posPoint(beacon), pos);
                        }else{
                            copyVec3(waypoint->pos, pos);
                        }

                        PRINT(    curPos,
                                "Path Waypoint (^4%d ^0total): (^4%1.f^0,^4%1.f^0,^4%1.f^0) - ^4%1.f ^0feet away\n",
                                path->waypointCount,
                                vecParamsXYZ(pos),
                                distance3(pos, ENTPOS(e)));
                    }
                }
            }
        }

        if(entDebugInfo & ENTDEBUGINFO_AI_STUFF){
            SECTION_TITLE("AI STUFF");

            //PRINT(    curPos,
            //        "AI Tick: ^4%s ^0(^4%1.3f^0), "
            //        "Life time: ^4%1.2f^0s, ^4%s ^0processes\n",
            //        getCommaSeparatedInt(ENTINFO(e).tick),
            //        ENTINFO(e).accProcess,
            //        ENTINFO(e).lifeTime / 30.0,
            //        getCommaSeparatedInt(ENTINFO(e).processCount));

            if(aiGetAttackerCount(e)){
                PRINT(curPos, "Attackers: ^4%d^d, ", aiGetAttackerCount(e));
            }else{
                PRINT(curPos, "Attackers: ^2NONE^d, ");
            }

            PRINT(curPos, "Threat Level: ^4%1.3f\n", pchar ? pchar->attrCur.fThreatLevel : 0);

            {
                Beacon* b = entGetNearestBeacon(e);

                if(b){
                    PRINT(    curPos,
                            "Nearest Beacon: ^4%1.f^d, ^4%1.f^d, ^4%1.f^d\n",
                            posParamsXYZ(b));
                }
            }
            
            if(ai->avoid.list){
                AIAvoidInstance* avoid;
                BeaconAvoidNode* node = ai->avoid.placed.nodes;
                int nodeCount = 0;
                
                for(; node; node = node->entityList.next, nodeCount++);
                
                PRINT(curPos, "Avoids (^4%d ^0nodes): ", nodeCount);
                
                for(avoid = ai->avoid.list; avoid; avoid = avoid->next){
                    PRINT(    curPos,
                            "%s^4%s%d ^0lvl @ ^4%1.1f^0ft",
                            avoid == ai->avoid.list ? "" : ", ",
                            avoid->maxLevelDiff >= 0 ? "+" : "",
                            avoid->maxLevelDiff,
                            avoid->radius);
                }

                PRINT(curPos, "\n");
            }

            if(entType != ENTTYPE_PLAYER){
                PRINT(    curPos,
                    "Brain: ^8%s^0, "
                    "Config: ^8%s ^0(^s^8%s^0^n)\n",
                    aiBrainTypeName[ai->brain.type] ? aiBrainTypeName[ai->brain.type] : "Unknown",
                    ai->brain.configName ? ai->brain.configName : "Unknown",
                    e->aiConfig ? e->aiConfig : ((e->villainDef && e->villainDef->aiConfig) ? e->villainDef->aiConfig : "--"));

                PRINT(    curPos,
                        "AI Timeslice: ^4%d ^0/ ^4%d ^0= ^4%1.2f^0%%\n",
                        ai->timeslice,
                        ai_state.totalTimeslice,
                        ai_state.totalTimeslice ? 100 * (float)ai->timeslice / (float)ai_state.totalTimeslice : 0);

                PRINT(    curPos,
                        "Memory: ^4%1.1f^0s, "
                        "Nearby Players: ^4%d ^0(Checked: ^4%1.1f^0s), "
                        "Shout: ^4%d^0%% @ ^4%1.2f^0ft\n",
                        ABS_TO_SEC((F32)ai->eventMemoryDuration),
                        ai->nearbyPlayerCount,
                        ABS_TO_SEC((F32)ABS_TIME_SINCE(ai->time.checkedProximity)),
                        ai->shoutChance,
                        ai->shoutRadius);

                if(entType == ENTTYPE_NPC){
                    PRINT(curPos, "Time Since Scared: ^4%d^0s\n", ABS_TO_SEC(ABS_TIME_SINCE(ai->time.begin.afraidState)));
                }

                PRINT(    curPos,
                        "Spawn: Protect ^4%1.0f^0ft in, ^4^4%1.0f^0ft out, ^4%1.1f^0s\n",
                        ai->protectSpawnRadius,
                        ai->protectSpawnOutsideRadius,
                        ABS_TO_SEC((F32)ai->protectSpawnDuration));

                PRINT(    curPos,
                        "Times: "
                        "setTarget=^4%d^ds, "
                        "didAttack=^4%d^ds, "
                        "wasAttacked=^4%d^ds\n",
                        ABS_TO_SEC(ABS_TIME_SINCE(ai->attackTarget.time.set)),
                        ABS_TO_SEC(ABS_TIME_SINCE(ai->time.didAttack)),
                        ABS_TO_SEC(ABS_TIME_SINCE(ai->time.wasAttacked)));

                PRINT(    curPos,
                        "%sGrief^d: ^4%1.1f^ds - ^4%1.1f^ds @ ^4%1.1f^d%% hp (cur=^4%1.1f^d%%)\n",
                        ai->isGriefed ? "^r" : "",
                        ABS_TO_SEC((float)ai->griefed.time.begin),
                        ABS_TO_SEC((float)ai->griefed.time.end),
                        100 * ai->griefed.hpRatio,
                        pchar->attrMax.fHitPoints ? 100 * pchar->attrCur.fHitPoints / pchar->attrMax.fHitPoints : 0);

                if(e->encounterInfo){
                    EncounterGroup* group = e->encounterInfo->parentGroup;
                    int ai_group = -1;

                    if (e->encounterInfo->actorID >= 0 && e->encounterInfo->actorID < eaSize(&group->active->spawndef->actors))
                        ai_group = group->active->spawndef->actors[e->encounterInfo->actorID]->ai_group;

                    PRINT(    curPos,
                            "Encounter: ^2%s^0, "
                            "AI Group: ^4%d^0, "
                            "Actor state: ^8%s",
                            getEncounterStateName(e->encounterInfo->parentGroup? e->encounterInfo->parentGroup->state: -1),
                            ai_group,
                            getActorStateName(e->encounterInfo->state));

                    if(group->active && group->active->whokilledme){
                        Entity* entWhoKilled = erGetEnt(group->active->whokilledme);

                        PRINT(    curPos,
                                "^0, WhoKilled: %s",
                                entWhoKilled ? AI_LOG_ENT_NAME(entWhoKilled) : "^1gone");
                    }

                    PRINT(curPos, "\n");
                }

                if(ai->attackTarget.entity){
                    PRINT(    curPos,
                            "\n"
                            "Target: %s %s^0- Distance ^4%1.2f\n"
                            "LOS: %s ^0- ^4%d^0s (^4%1.1f^0s)\n"
                            //"Attack List Pos: ^4%d%s\n"
                            //"Velocity: ( ^4%1.3f^0, ^4%1.3f^0, ^4%1.3f ^0)\n"
                            //"Motion: ^4%d ^0- ^4%1.2f\n"
                            "Waiting For Target To Move: %s\n"
                            "magicallyKnowTargetPos: %s\n",
                            AI_LOG_ENT_NAME(ai->attackTarget.entity),
                            ai->brain.type!=AI_BRAIN_COT_BEHEMOTH?"":(ai->brain.cotbehemoth.statusPrimary&&ai->brain.cotbehemoth.statusPrimary==ai->attackTarget.status)?"^1(PRIMARY) ":"^2(non-primary) ",
                            distance3(ENTPOS(e), ENTPOS(ai->attackTarget.entity)),
                            ai->canSeeTarget ? "^2VISIBLE" : ai->canHitTarget ? "^2HITABLE^0, ^1NOT VISIBLE" : "^1NOT VISIBLE",
                            ABS_TO_SEC(ABS_TIME_SINCE(ai->time.beginCanSeeState)),
                            ABS_TO_SEC((F32)ABS_TIME_SINCE(ai->time.checkedLOS)),
                            //ai->attackerListIndex,
                            //ai->attackerListIndex ? "" : " ^0- ^2LEADER",
                            //vecParamsXYZ(ai->attackTarget.entity),
                            //ai->motion.type,
                            //ai->motion.time,
                            GET_BOOL_STRING(ai->waitingForTargetToMove),
                            GET_BOOL_STRING(ai->magicallyKnowTargetPos));
                }
            }
        }

        if(clientLinkGetDebugVar(client, "Ai.Behavior"))
        {
             char* debugStr;
            SECTION_TITLE("BEHAVIOR");

            PRINT(curPos, "Current Override: %s\n",
                ai->base->behaviorCombatOverride == AIB_COMBAT_OVERRIDE_AGGRESSIVE ? "Aggressive" :
                ai->base->behaviorCombatOverride == AIB_COMBAT_OVERRIDE_COLLECTIVE ? "Collective" :
                ai->base->behaviorCombatOverride == AIB_COMBAT_OVERRIDE_DEFENSIVE ? "Defensive" :
                ai->base->behaviorCombatOverride == AIB_COMBAT_OVERRIDE_PASSIVE ? "Passive" : "");

            PRINT(curPos, "BehaviorData Chunks Allocated: ^4%i ^0(^sx^4%i ^0bytes^n)\n", ai->base->behaviorDataAllocated, aiBehaviorGetDataChunkSize());
            PRINT(curPos, "BehaviorTeamData Chunks Allocated: ^4%i ^0(^sx^4%i ^0bytes^n)\n", ai->teamMemberInfo.team ? ai->teamMemberInfo.team->base->behaviorTeamDataAllocated : 0, aiBehaviorGetTeamDataChunkSize());

            debugStr = aiBehaviorDebugPrint(e, ENTAI(e)->base, 0);
            PRINT(curPos, "%s", debugStr);
//             for(i = 0; i < eaSize(&ai->behaviors); i++)
//             {
//                 PRINT(curPos, "%i ", i);
//                 if(ai->behaviors[i]->running)
//                     PRINT(curPos, "^2");
//                 else if(!ai->behaviors[i]->activated)
//                     PRINT(curPos, "^8");
//                 else if(ai->behaviors[i]->finished)
//                     PRINT(curPos, "^1");
//                 else
//                     PRINT(curPos, "^5");
//                 name = aiBehaviorStringFromIndex(ai->behaviors[i]->index);
//                 PRINT(curPos, "%s^0 %s ", name ? name : "", aiBehaviorOverrideStringFromEnum(ai->behaviors[i]->combatOverride));
//                 PRINT(curPos, "%s\n", aiBehaviorGetDebugString(e, ai->behaviors[i], ""));
//             }
        }

        if(clientLinkGetDebugVar(client, "Ai.Behavior.History"))
        {
            char* histString;
            SECTION_TITLE("BEHAVIOR HISTORY");

            histString = aiBehaviorDebugPrintPrevMods(e, ENTAI(e)->base);
            PRINT(curPos, "%s", histString ? histString : "");
            histString = aiBehaviorDebugPrintPendingMods(e, ENTAI(e)->base);
            PRINT(curPos, "%s", histString ? histString : "");
        }

        if(entDebugInfo & ENTDEBUGINFO_SEQUENCER){
            SECTION_TITLE("SEQUENCER");

            PRINT(curPos, "SeqName: ^3%s\n", e->seq->type->name);

            PRINT(curPos, "SeqState: ^3%s\n", seqGetStateString(e->seq->state_lastframe));

            PRINT(curPos, "SeqStickyState: ^3%s\n", seqGetStateString(e->seq->sticky_state));

            if(e->pchar)
            {
                char *stance = NULL;
                estrCreate(&stance);

                for(i = eaiSize(&e->pchar->piStickyAnimBits) - 1; i >= 0; i--)
                {
                    estrConcatf(&stance, "%s, ", seqGetStateNameFromNumber(e->pchar->piStickyAnimBits[i]));
                }
                PRINT(curPos, "Power Stance Bits: ^3%s\n", stance);
                estrDestroy(&stance);
            }

            //PRINT(curPos, "MoveSeqState: ^3%s\n", seqGetStateString(getMoveFlags(e->seq))); //TO DO need a list of set seq->animation.move->flags instead

            PRINT(curPos, "MoveLastFrame: ^3%s\n", e->seq->animation.move_lastframe->name);

            PRINT(curPos, "Move: ^3%s ^0(^4%d^0)\n", e->seq->animation.move->name, e->move_idx);

            PRINT(curPos, "FinishCycle: %s\n", GET_BOOL_STRING(e->seq->animation.move->flags & SEQMOVE_FINISHCYCLE));
        }

        if(entDebugInfo & ENTDEBUGINFO_EVENT_HISTORY){
            int size = ARRAY_SIZE(ai->eventHistory.eventSet);
            int totalHits = 0;
            int totalMisses = 0;
            float totalDamageDone = 0;

            SECTION_TITLE("EVENT HISTORY");

            aiEventUpdateEvents(ai);

            for(i = 0; i < size; i++){
                int index = (ai->eventHistory.curEventSet - i + size) % size;
                AIEventSet* set = ai->eventHistory.eventSet + index;
                AIEvent* event = set->events;

                PRINT(curPos, "%d:", i);

                totalHits += set->attackedCount.enemy;
                totalMisses += set->wasMissedCount.enemy;
                totalDamageDone += set->damageTaken.enemy;

                if(set->eventCount){
                    PRINT(curPos, " ^4%d ^0events (", set->eventCount);

                    if(set->attackedCount.me)
                        PRINT(curPos, "^4%d ^1attacked^0, ", set->attackedCount.me);

                    if(set->attackedCount.myFriend)
                        PRINT(curPos, "^4%d ^1friend attacked^0, ", set->attackedCount.myFriend);

                    if(set->damageTaken.me)
                        PRINT(curPos, "^4%1.2f ^1damage taken^0, ", set->damageTaken.me);

                    PRINT(curPos, ")");
                }

                PRINT(curPos, "\n");
            }

            if(totalHits)
                PRINT(curPos, "^2Damage Done^0: ^4%1.2f^0hp/^4%d^0hits = ^t200t\t^4%1.2f^0hp/hit\n", totalDamageDone, totalHits, totalDamageDone / totalHits);

            if(totalHits + totalMisses)
                PRINT(curPos, "^2Attack Percent^0: ^4%d^0/^4%d^0 hits = ^t200t\t^4%1.2f^0%%\n", totalHits, totalHits + totalMisses, 100.0 * (float)totalHits / (totalHits + totalMisses));
        }

        if(    entDebugInfo & ENTDEBUGINFO_FEELING_INFO &&
            entType != ENTTYPE_PLAYER)
        {
            SECTION_TITLE("FEELING INFO");

            aiFeelingUpdateTotals(e, ai);

            PRINT(curPos, "^t100t");

            PRINT(curPos, "^7FEELING\tTOTAL\tMODS\tBASE\tPROXIMITY\tPERMANENT\n");

            for(i = 0; i < AI_FEELING_COUNT; i++){
                int j;
                int mod_level = (int)ceil(ai->feeling.mods.level[i] * 40);

                if(mod_level > 40)
                    mod_level = 40;
                else if (mod_level < -40)
                    mod_level = -40;

                PRINT(    curPos,
                        "^%d%s : \t^h^b%d/90b\t^b0/%db^b4b^b0/%db",
                        (i + 1) % 10,
                        aiFeelingName[i],
                        (int)ceil(ai->feeling.total.level[i] * 90),
                        40 + mod_level,
                        40 - mod_level);

                for(j = 0; j < AI_FEELINGSET_COUNT; j++){
                    PRINT(curPos, "\t^b%d/90b", (int)ceil(ai->feeling.sets[j].level[i] * 90));
                }

                PRINT(curPos, "\n");
            }
        }

        if(entDebugInfo & ENTDEBUGINFO_STATUS_TABLE){
            char clippedName[20];

            SECTION_TITLE("MY STATUS TABLE");

            if(ai->proxEntStatusTable){
                AIProxEntStatus* status = ai->proxEntStatusTable->statusHead;

                if(status){
                    float maxDanger = -FLT_MAX;
                    float maxDamageToMe = -FLT_MAX;
                    float maxDamageToMyFriends = -FLT_MAX;
                    float maxDamageFromMe = -FLT_MAX;

                    while(status){
                        if(status->entity){
                            if(status->dangerValue > maxDanger)
                                maxDanger = status->dangerValue;

                            if(status->damage.toMe > maxDamageToMe)
                                maxDamageToMe = status->damage.toMe;

                            if(status->damage.toMyFriends > maxDamageToMyFriends)
                                maxDamageToMyFriends = status->damage.toMyFriends;

                            if(status->damage.toMe > maxDamageToMe)
                                maxDamageFromMe = status->damage.fromMe;
                        }

                        status = status->tableList.next;
                    }

                    PRINT(curPos, "^t150t");

                    PRINT(curPos, "^sENTITY:ID\tDMG TO ME/FRIENDS\tDMG FROM ME\tDANGER\tSEEN/HIT ME/FRIEND\tFLAGS\n^n");

                    status = ai->proxEntStatusTable->statusHead;

                    while(status){
                        if(status->entity){
                            Entity* target = status->entity;
                            AIVars* aiTarget = ENTAI(target);
                            char flagsText[200] = "";
                            char* flagsCur = flagsText;
                            char timeSeen[100];
                            char timeHitMe[100];
                            char timeHitFriend[100];

                            aiGetClippedString(AI_LOG_ENT_NAME(target), 13, clippedName);

                            if(ABS_TIME_SINCE(status->taunt.begin) < status->taunt.duration){
                                flagsCur += sprintf(flagsCur,
                                    "^1taunt^0(^4%1.1f^0s),",
                                    ABS_TO_SEC((float)(status->taunt.duration - ABS_TIME_SINCE(status->taunt.begin))));
                            }

                            if(ABS_TIME_SINCE(status->placate.begin) < status->placate.duration){
                                flagsCur += sprintf(flagsCur,
                                    "^1placate^0(^4%1.1f^0s),",
                                    ABS_TO_SEC((float)(status->placate.duration - ABS_TIME_SINCE(status->placate.begin))));
                            }

                            if(ABS_TIME_SINCE(status->invalid.begin) < status->invalid.duration){
                                flagsCur += sprintf(flagsCur,
                                                    "^1invalid^0(^4%1.1f^0s),",
                                                    ABS_TO_SEC((float)(status->invalid.duration - ABS_TIME_SINCE(status->invalid.begin))));
                            }

                            if(status->neverForgetMe){
                                flagsCur += sprintf(flagsCur, "^1never forget^0,");
                            }
                            
                            if(aiTarget->avoid.list){
                                flagsCur += sprintf(flagsCur, "^1avoid %1.1fft^0,", aiTarget->avoid.maxRadius);
                            }

                            if(ABS_TO_SEC(ABS_TIME_SINCE(status->time.posKnown)) < 10000)
                                sprintf(timeSeen, "^4%d^0s", (int)ABS_TO_SEC(ABS_TIME_SINCE(status->time.posKnown)));
                            else
                                sprintf(timeSeen, "^1na^0");

                            if(ABS_TO_SEC(ABS_TIME_SINCE(status->time.attackMe)) < 10000)
                                sprintf(timeHitMe, "^4%d^0s", (int)ABS_TO_SEC(ABS_TIME_SINCE(status->time.attackMe)));
                            else
                                sprintf(timeHitMe, "^1na^0");

                            if(ABS_TO_SEC(ABS_TIME_SINCE(status->time.attackMyFriend)) < 10000)
                                sprintf(timeHitFriend, "^4%d^0s", (int)ABS_TO_SEC(ABS_TIME_SINCE(status->time.attackMyFriend)));
                            else
                                sprintf(timeHitFriend, "^1na^0");

                            PRINT(    curPos,
                                    "%s%s^0:^s^4%d^0^n"
                                    "\t^1^b%d/50b ^b%d/50b"
                                    "\t^2^b%d/100b"
                                    "\t^3^b%d/50b ^s^4%1.2f^0/^4%1.2f^n"
                                    "\t%s/%s/%s"
                                    "\t%s"
                                    "\t\n",
                                    target->pchar && target->pchar->attrCur.fHitPoints <= 0.0 ? "^7^h" :
                                        target == ai->attackTarget.entity ? "^2^h" :
                                            status->hasAttackedMe ? "^1^h" :
                                                ai->teamMemberInfo.orders.targetStatus && ai->teamMemberInfo.orders.targetStatus->entity == target ? "^5^h" : "",
                                    clippedName,
                                    target->owner,
                                    maxDamageToMe ? (int)ceil(100 * status->damage.toMe / maxDamageToMe) : 0,
                                    maxDamageToMyFriends ? (int)ceil(100 * status->damage.toMyFriends / maxDamageToMyFriends) : 0,
                                    maxDamageFromMe ? (int)ceil(100 * status->damage.fromMe / maxDamageFromMe) : 0,
                                    maxDanger ? (int)ceil(50 * status->dangerValue / maxDanger) : 0,
                                    target->pchar ? target->pchar->attrCur.fThreatLevel : 0,
                                    status->dangerValue,
                                    timeSeen,
                                    timeHitMe,
                                    timeHitFriend,
                                    flagsText);
                        }
                        status = status->tableList.next;
                    }
                }else{
                    PRINT(curPos, " ^1Empty\n");
                }
            }
        }

        if(    entDebugInfo & ENTDEBUGINFO_TEAM_INFO &&
            entType != ENTTYPE_PLAYER)
        {
            if(!ai->teamMemberInfo.team){
                SECTION_TITLE("TEAM MEMBERS");

                PRINT(curPos, "No Team\n");
            }else{
                AITeam* team = ai->teamMemberInfo.team;
                AITeamMemberInfo* member;

                if(team->proxEntStatusTable->statusHead){
                    AIProxEntStatus* status = team->proxEntStatusTable->statusHead;

                    SECTION_TITLE("TEAM STATUS TABLE");

                    if(status){
                        float maxDanger = -FLT_MAX;
                        float totalDamageToTeam = 0;

                        while(status){
                            if(status->entity && status->dangerValue > maxDanger)
                                maxDanger = status->dangerValue;

                            status = status->tableList.next;
                        }

                        PRINT(curPos, "^t150t");

                        status = team->proxEntStatusTable->statusHead;

                        while(status){
                            totalDamageToTeam += status->damage.toMe;
                            status = status->tableList.next;
                        }

                        PRINT(    curPos,
                                "^sENTITY:ID\tDMG TO/FROM US\tDANGER\tSEEN/HIT BY\tMEMBERS ASSIGNED/ACTUAL\n^n");

                        status = team->proxEntStatusTable->statusHead;

                        while(status){
                            if(    status->entity && pchar && status->entity->pchar &&
                                (character_IsConfused(pchar) || ENTAI(status->entity)->teamMemberInfo.team != ai->teamMemberInfo.team))
                            {
                                AIProxEntStatus* myStatus = aiGetProxEntStatus(ai->proxEntStatusTable, status->entity, 0);
                                char clippedName[20];
                                char timeSeen[100];
                                char timeHitMe[100];
                                AITeamMemberInfo* teammate;
                                int attackersCount = 0;

                                for(teammate = team->members.list; teammate; teammate = teammate->next){
                                    if(    teammate->entity &&
                                        ENTAI(teammate->entity) &&
                                        ENTAI(teammate->entity)->attackTarget.entity == status->entity)
                                    {
                                        attackersCount++;
                                    }
                                }

                                aiGetClippedString(AI_LOG_ENT_NAME(status->entity), 13, clippedName);

                                PRINT(    curPos,
                                        "%s%s^0:^4^s%d^n\t",
                                        ai->teamMemberInfo.orders.targetStatus &&
                                            status->entity == ai->teamMemberInfo.orders.targetStatus->entity ? "^2^h^d" :
                                                status->hasAttackedMe ? "^1^h^d" : "",
                                        clippedName,
                                        status->entity->owner);

                                if(ABS_TO_SEC(ABS_TIME_SINCE(status->time.posKnown)) < 10000)
                                    sprintf(timeSeen, "^4%d^0s", (int)ABS_TO_SEC(ABS_TIME_SINCE(status->time.posKnown)));
                                else
                                    sprintf(timeSeen, "^1na^0");

                                if(ABS_TO_SEC(ABS_TIME_SINCE(status->time.posKnown)) < 10000)
                                    sprintf(timeHitMe, "^4%d^0s", (int)ABS_TO_SEC(ABS_TIME_SINCE(status->time.attackMe)));
                                else
                                    sprintf(timeHitMe, "^1na^0");

                                PRINT(    curPos,
                                        "^1^b%d/50b "
                                        "^2^b%d/50b"
                                        "\t^3^b%d/50b ^4%1.2f"
                                        "\t%s/%s"
                                        "\t^4%d^0/^4%d"
                                        "\t\n",
                                        totalDamageToTeam ? (int)ceil(50 * status->damage.toMe / totalDamageToTeam) : 0,
                                        (int)ceil(50 * status->damage.fromMe / status->entity->pchar->attrMax.fHitPoints),
                                        (int)ceil(50 * status->dangerValue / maxDanger),
                                        status->entity->pchar->attrCur.fThreatLevel,
                                        timeSeen,
                                        timeHitMe,
                                        status->assignedAttackers,
                                        attackersCount);
                            }
                            status = status->tableList.next;
                        }
                    }else{
                        PRINT(curPos, " ^1Empty\n");
                    }
                }

                SECTION_TITLE("TEAM MEMBERS");

                PRINT(    curPos,
                        "Members Alive: ^4%d^0, Killed: ^4%d^0, UnderGriefHP: ^4%d^0, First Attacked: ^4%d^0s, Last Attacked: ^4%d^0s, Did Attack: ^4%d^0s,\n",
                        team->members.aliveCount,
                        team->members.killedCount,
                        team->members.underGriefedHPCount,
                        (int)ABS_TO_SEC(ABS_TIME_SINCE(team->time.firstAttacked)),
                        (int)ABS_TO_SEC(ABS_TIME_SINCE(team->time.wasAttacked)),
                        (int)ABS_TO_SEC(ABS_TIME_SINCE(team->time.didAttack)));

                PRINT(curPos, "Time since recalc: ^4%1.1f ^0s\n", ABS_TO_SEC((float)ABS_TIME_SINCE(team->time.lastSetRoles)));

                PRINT(curPos, "^t150t");

                PRINT(curPos, "^sMEMBER:ID\tHP^t105t\tEND^t150t\tORDERS\tATTACK TARGET\tFLAGS\n^n");

                for(i = 1, member = team->members.list; member; member = member->next, i++){
                    AIVars* aiMember = ENTAI(member->entity);
                    float pctHP = member->entity->pchar->attrCur.fHitPoints / member->entity->pchar->attrMax.fHitPoints;
                    float pctEND = member->entity->pchar->attrCur.fEndurance / member->entity->pchar->attrMax.fEndurance;
                    char clippedName[20];

                    aiGetClippedString(AI_LOG_ENT_NAME(member->entity), 13, clippedName);

                    PRINT(    curPos,
                            "%s%d. %s^0:^4^s%d^n\t",
                            e == member->entity ? "^5^h^d" : "",
                            i,
                            clippedName,
                            member->entity->owner);

                    if(pctHP > 0){
                        PRINT(    curPos,
                                "%s^b%d/50b %s^b%d/50b ",
                                pctHP > 0.4 ? "^2" : "^1",
                                (int)ceil(50.0 * pctHP),
                                pctEND > 0.4 ? "^5" : "^1",
                                (int)ceil(50.0 * pctEND));
                    }else{
                        PRINT(curPos, "^1DEAD!");
                    }

                    PRINT(curPos, "\t");

                    switch(member->orders.role){
                        case AI_TEAM_ROLE_ATTACKER_MELEE:
                            PRINT(curPos, "^2M");
                            break;
                        case AI_TEAM_ROLE_ATTACKER_RANGED:
                            PRINT(curPos, "^1R");
                            break;
                        case AI_TEAM_ROLE_HEALER:
                            PRINT(curPos, "^5H");
                            break;
                    }

                    if(member->orders.targetStatus && member->orders.targetStatus->entity){
                        aiGetClippedString(AI_LOG_ENT_NAME(member->orders.targetStatus->entity), 13, clippedName);

                        PRINT(curPos, "^0:%s", clippedName);
                    }

                    if(aiMember->attackTarget.entity){
                        aiGetClippedString(AI_LOG_ENT_NAME(aiMember->attackTarget.entity), 13, clippedName);
                    }else{
                        clippedName[0] = 0;
                    }

                    PRINT(    curPos,
                            aiMember->attackTarget.entity ? "\t%s^0(^4%d^0)" : "\t%s",
                            clippedName,
                            aiMember->attackTarget.hitCount);

                    PRINT(curPos, "\t^0");

                    if(member->entity->pchar->attrCur.fSleep > 0)
                        PRINT(curPos, "^1sleep^0(^4%1.2f^0),", member->entity->pchar->attrCur.fSleep);

                    if(member->entity->pchar->attrCur.fHeld > 0)
                        PRINT(curPos, "^1held^0,");

                    if(member->entity->pchar->attrCur.fImmobilized > 0)
                        PRINT(curPos, "^1immobile^0,");

                    if(character_IsConfused(member->entity->pchar) > 0)
                        PRINT(curPos, "^1confused^0,");

                    if(ENTAI(member->entity)->isAfraidFromAI)
                        PRINT(curPos, "^1afraid.ai^0,");

                    if(ENTAI(member->entity)->isAfraidFromPowers)
                        PRINT(curPos, "^1afraid.powers^0,");

                    if(!ENTAI(member->entity)->teamMemberInfo.alerted)
                        PRINT(curPos, "^1not alerted^0,");

                    if(member->currentlyRunningOutOfDoor)
                        PRINT(curPos, "^1running out of door,");

                    PRINT(curPos, "\t\n");
                }

                if(entDebugInfo & ENTDEBUGINFO_TEAM_POWERS){
                    AIBasePowerInfo* info;

                    SECTION_TITLE("TEAM POWERS");

                    for(info = team->power.list; info; info = info->next){
                        PRINT(    curPos,
                                "^5%s ^d(^4%d^dx,^4%d^ds,^4%d^duses)\n",
                                info->basePower->pchName,
                                info->refCount,
                                ABS_TO_SEC(ABS_TIME_SINCE(info->time.lastUsed)),
                                info->usedCount);
                    }
                }
            }
        }

        if(entDebugInfo & ENTDEBUGINFO_PHYSICS){
            char buffer[1000];
            char* pos = buffer;

            SECTION_TITLE("PHYSICS");

            // Flags.

            if(e->motion->falling)
                pos += sprintf(pos, "^2falling^0, ");

            if(e->motion->bigfall)
                pos += sprintf(pos, "^2bigfall^0, ");

            if(e->motion->input.flying)
                pos += sprintf(pos, "^2flying^0, ");

            if(e->motion->jumping)
                pos += sprintf(pos, "^2jumping^0, ");

            if(e->motion->input.no_ent_collision)
                pos += sprintf(pos, "^2no_ent_collision^0, ");

            if(e->motion->input.no_slide_physics)
                pos += sprintf(pos, "^2no_slide_physics^0, ");

            if(e->motion->on_poly_edge)
                pos += sprintf(pos, "^2on_poly_edge^0, ");

            if(e->motion->bouncing)
                pos += sprintf(pos, "^2bouncing^0, ");

            if(pos != buffer)
                PRINT(curPos, "Flags: %s\n", buffer);

            // Other things.

            PRINT(    curPos,
                    "vel: (^4%f^0, ^4%f^0, ^4%f^0)\n",
                    vecParamsXYZ(e->motion->vel));

            PRINT(    curPos,
                    "inp_vel: (^4%f^0, ^4%f^0, ^4%f^0)\n",
                    vecParamsXYZ(e->motion->input.vel));

            PRINT(    curPos,
                    "last_pos: (^4%f^0, ^4%f^0, ^4%f^0)\n",
                    vecParamsXYZ(e->motion->last_pos));

            PRINT(    curPos,
                    "surf_normal: (^4%f^0, ^4%f^0, ^4%f^0)\n",
                    vecParamsXYZ(e->motion->surf_normal));

            PRINT(    curPos,
                    "stuck: ^4%d^0, "
                    "stuck_head: ^4%d^0, "
                    "move_type: ^4%d^0,"
                    "move_time: ^4%f^0,"
                    "\n",
                    e->motion->stuck,
                    e->motion->stuck_head,
                    e->move_type,
                    e->motion->move_time);

            PRINT(    curPos,
                    "jump_height: ^4%f^0, "
                    "max_jump_height: ^4%f^0, "
                    "highest_height: ^4%f^0, "
                    "jump_time: ^4%d^0, "
                    "\n",
                    e->motion->jump_height,
                    e->motion->input.max_jump_height,
                    e->motion->highest_height,
                    e->motion->jump_time);

            PRINT(    curPos,
                    "inp_vel: (^4%f^0, ^4%f^0, ^4%f^0)\n",
                    vecParamsXYZ(e->motion->input.vel));

            for(i = 0; i < ARRAY_SIZE(e->motion->input.surf_mods); i++){
                PRINT(    curPos,
                        "%s"
                        "surf_mods[^4%d^0]: "
                        "traction: ^4%1f^0, "
                        "friction: ^4%1f\n"
                        "%s"
                        "bounce: ^4%1f^0, "
                        "gravity: ^4%1f^0, "
                        "max_speed: ^4%1f^0, "
                        "\n",
                        (e->motion->input.flying || e->motion->falling) == i ? "^2^h^0" : "",
                        i,
                        e->motion->input.surf_mods[i].traction,
                        e->motion->input.surf_mods[i].friction,
                        (e->motion->input.flying || e->motion->falling) == i ? "^2^h^0" : "",
                        e->motion->input.surf_mods[i].bounce,
                        e->motion->input.surf_mods[i].gravity,
                        e->motion->input.surf_mods[i].max_speed);
            }

            PRINT(    curPos,
                    "Capsule: ^4%1.3f^0, ^4%1.3f^0, ^4%d\n",
                    e->motion->capsule.length,
                    e->motion->capsule.radius,
                    e->motion->capsule.dir);

            if(e->client){
                // ServerControlState.

                ServerControlState* scs = e->client->controls.server_state;

                buffer[0] = 0;
                pos = buffer;

                // Flags.

                if(scs->controls_input_ignored)
                    pos += sprintf(pos, "^2controls_input_ignored^0, ");

                if(scs->no_ent_collision)
                    pos += sprintf(pos, "^2no_ent_collision^0, ");

                if(scs->fly)
                    pos += sprintf(pos, "^2fly^0, ");

                if(scs->stun)
                    pos += sprintf(pos, "^2stun^0, ");

                if(scs->jumppack)
                    pos += sprintf(pos, "^2jumppack^0, ");

                if(scs->glide)
                    pos += sprintf(pos, "^2glide^0, ");

                if(scs->ninja_run)
                    pos += sprintf(pos, "^2ninja_run^0, ");

                if(scs->walk)
                    pos += sprintf(pos, "^2walk^0, ");

                if(scs->beast_run)
                    pos += sprintf(pos, "^2beast_run^0, ");

                if(scs->steam_jump)
                    pos += sprintf(pos, "^2steam_jump^0, ");

                if(scs->hover_board)
                    pos += sprintf(pos, "^2hover_board^0, ");

                if(scs->magic_carpet)
                    pos += sprintf(pos, "^2magic_carpet^0, ");

                if(scs->parkour_run)
                    pos += sprintf(pos, "^2parkour_run^0, ");

                PRINT(    curPos,
                        "SCS: "
                        "Speed: ^4%1.3f^0, ^4%1.3f^0, "
                        "Back: ^4%1.3f^0, "
                        "Jump: ^4%1.3f^0, "
                        "%s%s"
                        "\n",
                        scs->speed[0],
                        scs->speed[1],
                        scs->speed_back,
                        scs->jump_height,
                        buffer[0] ? "Flags: " : "",
                        buffer[0] ? buffer : "");
            }
        }

        if(entDebugInfo & ENTDEBUGINFO_POWERS){
            // SHANNON

            SECTION_TITLE("POWERS");

            if(!character_IsInitialized(pchar))
            {
                PRINT(curPos, "^1Character not initialized^0\n");
            }
            else
            {
                int iset;

                PRINT(curPos, "Powers:\n");
                for(iset=0; iset<eaSize(&pchar->ppPowerSets); iset++)
                {
                    int ipow;
                    PowerSet *pset = pchar->ppPowerSets[iset];
                    PRINT(curPos, "   %s\n", pset->psetBase->pchName);

                    for(ipow=0; ipow<eaSize(&pset->ppPowers); ipow++)
                    {
                        Power *ppow = pset->ppPowers[ipow];
                        
                        if(!ppow)
                            continue;

                        if(ppow == pchar->ppowStance)
                            PRINT(curPos, "^2");
                        PRINT(curPos, "      %s", ppow->ppowBase->pchName);
                        if(ppow->ppowBase->eType == kPowerType_Auto)
                            PRINT(curPos, " (auto)");
                        if(ppow == pchar->ppowStance)
                            PRINT(curPos, "^0");

                        if(eaSize(&ppow->ppowBase->ppchAIGroups))
                        {
                            int i;

                            PRINT(curPos, "(");

                            for(i = 0; i < eaSize(&ppow->ppowBase->ppchAIGroups); i++)
                            {
                                PRINT(curPos, "%s^6%s", i ? "^0," : "", ppow->ppowBase->ppchAIGroups[i]);
                            }

                            PRINT(curPos, "^0)");
                        }
                        PRINT(curPos, "\n");
                    }
                }
            }
        }

        if(entDebugInfo & ENTDEBUGINFO_COMBAT){
            // SHANNON

            if(!character_IsInitialized(pchar))
            {
                SECTION_TITLE("COMBAT");
                PRINT(curPos, "^1Character not initialized^0\n");
            }
            else
            {
                // Print player-specific stuff.

                if(entType == ENTTYPE_PLAYER && clientLinkGetDebugVar(client, "Combat.Stats")){
                    SECTION_TITLE("COMBAT STATS");

                    PRINT(curPos, "Level: ^4%d^0\n", pchar->iLevel);
                    PRINT(curPos, "Skill: ^4%d^0\n", pchar->iWisdomLevel);
                    PRINT(curPos, "XP: ^4%d^0\n", pchar->iExperiencePoints);
                    PRINT(curPos, "Debt: ^4%d^0\n", pchar->iExperienceDebt);
                    PRINT(curPos, "Rest: ^4%d^0\n", pchar->iExperienceRest);
                    PRINT(curPos, "Wisdom: ^4%d^0\n", pchar->iWisdomPoints);
                    PRINT(curPos, "Influence: ^4%d^0\n", pchar->iInfluencePoints);
                    PRINT(curPos, "Origin: %s\n", pchar->porigin->pchName);
                    PRINT(curPos, "Class: %s\n", pchar->pclass->pchName);

                    PRINT(curPos, "Timers Regen: ^4%6.2f^0 Recover: ^4%6.2f^0 Idea: ^4%6.2f^0\n", pchar->fRegenerationTimer, pchar->fRecoveryTimer, pchar->fInsightRecoveryTimer);

                    PRINT(    curPos,
                            "^t150t\tDamage^t70t\tuses^t30t\thits^t30t\tmiss\n");

                    PRINT(    curPos,
                            "%s:^t150t\t^4%1.2f^0\t^4%1.2f^0^t70t\t^4%d^0^t30t\t^4%d^0^t30t\t^4%d^0\n",
                            "Given",
                            pchar->stats.fDamageGiven,
                            pchar->stats.fHealingGiven,
                            pchar->stats.iCntUsed,
                            pchar->stats.iCntHits,
                            pchar->stats.iCntMisses);

                    PRINT(    curPos,
                            "%s:^t150t\t^4%1.2f^0\t^4%1.2f^0^t70t\t^t30t\t^4%d^0^t30t\t^4%d^0\n",
                            "Received",
                            pchar->stats.fDamageReceived,
                            pchar->stats.fHealingReceived,
                            pchar->stats.iCntBeenHit,
                            pchar->stats.iCntBeenMissed);

                    PRINT(    curPos,
                            "%s:^t150t\t^t70t\t^4%d^0^t30t\n",
                            "Knocks",
                            pchar->stats.iCntKnockbacks);

                    PRINT(    curPos,
                            "%s:^t150t\t^t70t\t^4%d^0^t30t\n",
                            "Kills",
                            pchar->stats.iCntKills);

                    for(i=eaSize(&pchar->ppPowerSets)-1; i>=0; i--)
                    {
                        int j;
                        for(j=eaSize(&pchar->ppPowerSets[i]->ppPowers)-1; j>=0; j--)
                        {
                            if(pchar->ppPowerSets[i]->ppPowers[j] &&
                                pchar->ppPowerSets[i]->ppPowers[j]->stats.iCntUsed!=0)
                            {
                                PRINT(curPos, "^5%s.%s^0:^t150t\t^4%1.2f^0\t^4%1.2f^0^t70t\t^4%d^0^t30t\t^4%d^0^t30t\t^4%d^0\n",
                                    pchar->ppPowerSets[i]->psetBase->pchName,
                                    pchar->ppPowerSets[i]->ppPowers[j]->ppowBase->pchName,
                                    pchar->ppPowerSets[i]->ppPowers[j]->stats.fDamageGiven,
                                    pchar->ppPowerSets[i]->ppPowers[j]->stats.fHealingGiven,
                                    pchar->ppPowerSets[i]->ppPowers[j]->stats.iCntUsed,
                                    pchar->ppPowerSets[i]->ppPowers[j]->stats.iCntHits,
                                    pchar->ppPowerSets[i]->ppPowers[j]->stats.iCntMisses);
                            }
                        }
                    }
                }
                
                if(clientLinkGetDebugVar(client, "Combat.Mods")){
                    AttribMod *pmod;
                    char *savedcurPos=curPos;
                    AttribModListIter iter;

                    SECTION_TITLE("COMBAT MODS");

                    pmod = modlist_GetFirstMod(&iter, &pchar->listMod);
                    while(pmod!=NULL)
                    {
                        extern TokenizerParseInfo ParseCharacterAspects[];
                        extern TokenizerParseInfo ParseCharacterAttributes[];
                        extern char *dbg_GetParseName(TokenizerParseInfo atpi[] , int offset);

                        const AttribModTemplate *t = pmod->ptemplate;
                        Entity *eSrc = erGetEnt(pmod->erSource);
                        const char *pchSrc = "(unknown)";

                        if(t->ppowBase->eType==kPowerType_Auto && eSrc==e)
                        {
                            if(entDebugInfo & ENTDEBUGINFO_AUTO_POWERS)
                            {
                                PRINT(curPos, "^3%s.%s^0 from ^7%s^0", t->ppowBase->pchName, t->pchName, "(auto)");
                                PRINT(curPos, "   changes ^8%s.%s^0\n",
                                    dbg_GetParseName(ParseCharacterAspects, (int)pmod->ptemplate->offAspect),
                                    dbg_GetParseName(ParseCharacterAttributes, (int)pmod->ptemplate->offAttrib));
                            }
                        }
                        else
                        {
                            if(eSrc)
                            {
                                pchSrc = eSrc->name;
                                if(ENTTYPE(eSrc)==ENTTYPE_CRITTER && (pchSrc==NULL || pchSrc[0]=='\0'))
                                {
                                    pchSrc = npcDefList.npcDefs[eSrc->npcIndex]->displayName;
                                }
                            }

                            PRINT(curPos, "^3%s.%s^0 from ^7%s^0\n", t->ppowBase->pchName, t->pchName, pchSrc);

                            PRINT(curPos, "^t15t \t^8%s.%s^0^t47t \tTime:^t25t \t^4%6.2f^0\n",
                                dbg_GetParseName(ParseCharacterAspects, (int)pmod->ptemplate->offAspect),
                                dbg_GetParseName(ParseCharacterAttributes, (int)pmod->ptemplate->offAttrib),
                                pmod->fTimer);

                            if (pmod->ptemplate->offAttrib == kSpecialAttrib_SetMode)
                            {
                                PRINT(curPos, "^t15t \t\tMode %2.0f\n", pmod->ptemplate->fMagnitude);
                            }
                            if(pmod->fDuration<ATTRIBMOD_DURATION_FOREVER)
                            {
                                PRINT(curPos, "^t15t \tMag:^t30t \t^4%6.2f^0^t70t \tDur:^t25t \t^4%6.2f^0\n", pmod->fMagnitude, pmod->fDuration);
                            }
                            else
                            {
                                PRINT(curPos, "^t15t \tMag:^t30t \t^4%6.2f^0^t70t \tDur:^t25t \t^4until shut off^0\n", pmod->fMagnitude);
                            }

                            PRINT(curPos, "^t15t \tChance:^t30t\t^4%6.2f^0, %scancel on miss\n", pmod->fChance, t->bCancelOnMiss ? "" : "does not ");
                            PRINT(curPos, "^t25t \t%sallow str, %sallow res, %srequire near ground\n",
                                t->bAllowStrength ? "" : "dis", t->bAllowResistance ? "" : "dis", t->bNearGround ? "" : "not ");
                        }

                        pmod = modlist_GetNextMod(&iter);
                    }

//                    AI_LOG(AI_LOG_COMBAT, (e, "%s", savedcurPos));
                }

                if(clientLinkGetDebugVar(client, "Combat.Attribs")){
                    SECTION_TITLE("COMBAT ATTRIBS");

                    curPos = DumpAttribs(pchar, curPos);
                }
            }
        }

        if(entDebugInfo & ENTDEBUGINFO_ENHANCEMENT){
            // SHANNON

            SECTION_TITLE("ENHANCEMENTS");

            if(!character_IsInitialized(pchar))
            {
                PRINT(curPos, "^1Character not initialized^0\n");
            }
            else
            {
                if(pchar->stats.ppowLastAttempted)
                {
                    PRINT(curPos, "^3%s^0\n", pchar->stats.ppowLastAttempted->ppowBase->pchName);
                    curPos = DumpStrAttribs(pchar, pchar->stats.ppowLastAttempted->pattrStrength, curPos);
                }
                else
                {
                    PRINT(curPos, "^1Execute power to see enhancement info^0\n");
                }
            }
        }
    }
    else if(e == clientGetViewedEntity(client) && entDebugInfo & ENTDEBUGINFO_COMBAT)
    {
        // No entities are selected, dump all the power stats

        int icat;
        const PowerDictionary *pdict = &g_PowerDictionary;

        SECTION_TITLE("MY PLAYER COMBAT");

        PRINT(curPos, "^t150t\tDamage^t70t\tuses^t30t\thits^t30t\tmiss\n");
        for(icat=0; icat<eaSize(&pdict->ppPowerCategories); icat++)
        {
            int iset;
            bool bPrintedCat = false;

            const PowerCategory *pcat = g_PowerDictionary.ppPowerCategories[icat];

            for(iset=0; iset<eaSize(&pcat->ppPowerSets); iset++)
            {
                int ipow;
                bool bPrintedSet = false;

                const BasePowerSet *pset = pcat->ppPowerSets[iset];

                for(ipow=0; ipow<eaSize(&pset->ppPowers); ipow++)
                {
                    if(pset->ppPowers[ipow]->stats.iCntUsed > 0)
                    {
                        if(!bPrintedCat)
                        {
                            PRINT(curPos, "^5%s^0\n", pcat->pchName);
                            bPrintedCat = true;
                        }
                        if(!bPrintedSet)
                        {
                            PRINT(curPos, "   ^5%s^0\n", pset->pchName);
                            bPrintedSet = true;
                        }

                        PRINT(curPos, "      ^5%s^0:^t150t\t^4%1.2f^0\t^4%1.2f^0^t70t\t^4%d^0^t30t\t^4%d^0^t30t\t^4%d^0\n",
                            pset->ppPowers[ipow]->pchName,
                            pset->ppPowers[ipow]->stats.fDamageGiven,
                            pset->ppPowers[ipow]->stats.fHealingGiven,
                            pset->ppPowers[ipow]->stats.iCntUsed,
                            pset->ppPowers[ipow]->stats.iCntHits,
                            pset->ppPowers[ipow]->stats.iCntMisses);
                    }
                }
            }
        }

    }

    // Send the AI log.

    if(sendExtraInfo && ai->log && ai->log->text[0]){ 
        int i, end;
        char* log = ai->log->text;

        SECTION_TITLE("AI LOG");

        //PRINT(    curPos, "AI Log: ^4%d ^0chars\n",
        //                    strlen(log));

        //PRINT(curPos, "Flags: ");

        //for(i = 0; i < ARRAY_SIZE(aiLogFlagToName); i++){
        //    PRINT(    curPos, "%s%s^0(^4%d^0), ",
        //            (server_state.aiLogFlags & (1 << aiLogFlagToName[i].flagBit)) ? "^2" : "^1",
        //            aiLogFlagToName[i].name,
        //            aiLogFlagToName[i].flagBit);

        //    if((i % 8) == 7){
        //        PRINT(curPos, "\n     ");
        //    }
        //}

        //PRINT(curPos, "\n");

        for(i = end = (int)strlen(log) - 1; i >= -1; i--){
            if(i == -1 || log[i] == '\n'){
                char temp = log[end];
                log[end] = '\0';
                PRINT(curPos, "  ");
                PRINT(curPos, log + i + 1);
                PRINT(curPos, "\n");
                log[end] = temp;
                end = i;
            }
        }
    }

    if(debugInfoString[0]){
        SECTION_TITLE("END OF INFO");
    }

    // Send the debug info.

    pktSendString(pak, debugInfoString);

    // Send the path.

    if(    !sendExtraInfo ||
        !(entDebugInfo & ENTDEBUGINFO_PATH) ||
        entType == ENTTYPE_PLAYER ||
        !ai->navSearch || !ai->navSearch->path.head)
    {
        pktSendBitsPack(pak, 1, 0);
    }else{
        NavPathWaypoint* cur;
        NavPath* path = &ai->navSearch->path;

        pktSendBitsPack(pak, 1, ai->navSearch->path.waypointCount);

        for(cur = path->head; cur; cur = cur->next){
            Beacon* beacon = cur->beacon;
            BeaconConnection* conn = cur->connectionToMe;

            pktSendF32(pak, vecX(cur->pos));
            pktSendF32(pak, vecY(cur->pos));
            pktSendF32(pak, vecZ(cur->pos));

            if(conn && cur->connectType == NAVPATH_CONNECT_JUMP){
                pktSendBits(pak, 1, 1);
                pktSendIfSetF32(pak, conn->minHeight ? conn->minHeight : 0);
                pktSendIfSetF32(pak, conn->minHeight ? conn->maxHeight : 0);
            }else{
                pktSendBits(pak, 1, 0);
                pktSendBits(pak, 1, cur->connectType == NAVPATH_CONNECT_FLY ? 1 : 0);
            }
        }
    }

    // Send the point log.

    if(!sendExtraInfo || !ai->log){
        pktSendBitsPack(pak, 1, 0);
    }else{
        int i;
        int count = 0;

        // Count how many points there are.

        for(i = 0; i < AI_POINTLOG_LENGTH; i++){
            AIPointLogPoint* p = ai->log->pointLog.points + i;

            if(p->argb){
                count++;
            }
        }

        pktSendBitsPack(pak, 1, count);

        // Send the points.

        i = ai->log->pointLog.curPoint;

        do{
            AIPointLogPoint* p = ai->log->pointLog.points + i;

            if(p->argb){
                pktSendBits(pak, 32, p->argb);

                pktSendF32(pak, p->pos[0]);
                pktSendF32(pak, p->pos[1]);
                pktSendF32(pak, p->pos[2]);

                pktSendString(pak, p->tag);
            }
            i = (i + 1) % AI_POINTLOG_LENGTH;
        }while(i != ai->log->pointLog.curPoint);
    }

    // Send the movement target position.

    if(!sendExtraInfo){
        pktSendBits(pak, 1, 0);
    }else{
        if(entType == ENTTYPE_DOOR){
            Beacon* beacon = entGetNearestBeacon(e);

            if(beacon){
                pktSendBits(pak, 1, 1);
                pktSendF32(pak, posX(beacon));
                pktSendF32(pak, posY(beacon));
                pktSendF32(pak, posZ(beacon));
                pktSendBits(pak, 32, 0xffff8800);
            }
        }
        else if(ai->followTarget.type && entDebugInfo & ENTDEBUGINFO_PATH){
            pktSendBits(pak, 1, 1);

            if(ai->motion.type == AI_MOTIONTYPE_JUMP || ai->motion.type == AI_MOTIONTYPE_FLY){
                pktSendF32(pak, ai->motion.target[0]);
                pktSendF32(pak, ai->motion.target[1]);
                pktSendF32(pak, ai->motion.target[2]);
                pktSendBits(pak, 32, 0xff0088ff);
            }
            else{
                pktSendF32(pak, vecX(ai->followTarget.pos));
                pktSendF32(pak, vecY(ai->followTarget.pos));
                pktSendF32(pak, vecZ(ai->followTarget.pos));
                pktSendBits(pak, 32, 0xff00ff00);
            }
        }
                        
        if(ai->avoid.placed.nodes){
            BeaconAvoidNode* node;
            
            for(node = ai->avoid.placed.nodes; node; node = node->entityList.next){
                pktSendBits(pak, 1, 1);
                pktSendF32(pak, posX(node->beacon));
                pktSendF32(pak, posY(node->beacon));
                pktSendF32(pak, posZ(node->beacon));
                pktSendBits(pak, 32, 0xffff3333);
            }
        }
        
        pktSendBits(pak, 1, 0);
    }

    // Send the team members.

    if(    !sendExtraInfo ||
        !ai->teamMemberInfo.team ||
        ai->teamMemberInfo.team->members.totalCount == 1 ||
        !(entDebugInfo & ENTDEBUGINFO_TEAM_INFO))
    {
        pktSendBitsPack(pak, 1, 0);
    }else{
        AITeamMemberInfo* member;

        pktSendBitsPack(pak, 1, ai->teamMemberInfo.team->members.totalCount - 1);

        for(member = ai->teamMemberInfo.team->members.list; member; member = member->next){
            if(member->entity != e){
                pktSendBitsPack(pak, 8, member->entity->owner);
            }
        }
    }

    // Send the net_ipos.

    //if(e != client->entity){
    //    printf("%d\t%d\t%d\n", e->net_ipos[0], e->net_ipos[1], e->net_ipos[2]);
    //}

    pktSendBits(pak, 24, e->net_ipos[0]);
    pktSendBits(pak, 24, e->net_ipos[1]);
    pktSendBits(pak, 24, e->net_ipos[2]);
}

static void aiSendDebugPoint(Packet* pak, Vec3 pos, int argb, int connect, int drawupright)
{
    pktSendBits(pak, 1, 1);
    pktSendF32(pak, pos[0]);
    pktSendF32(pak, pos[1]);
    pktSendF32(pak, pos[2]);
    pktSendBits(pak, 32, argb);
    pktSendBits(pak, 1, connect ? 1 : 0);
    pktSendBits(pak, 1, drawupright ? 1 : 0);
}

void aiSendGlobalEntDebugInfo(ClientLink *client, Packet *pak) {
    char buffer[1000];
    char buffer2[1000];
    int i;

    // Send lines.

    if(    !server_visible_state.pause &&
        //client->controls.selected_ent_server_index == interp_state.store_entity_id &&
        client->entDebugInfo & ENTDEBUGINFO_ENTSEND_LINES)
    {
        Entity* e = entFromId(interp_state.store_entity_id);

        if(e)
        {
            int pos_to_binpos[7] = { 3, 1, 4, 0, 5, 2, 6 };

            if(client->entDebugInfo & ENTDEBUGINFO_INTERP_LINE_CURVE)
            {
                // Send the interp curve as a RED line.

                aiSendDebugPoint(pak, interp_state.last_pos, 0xff0000ff, 0, 0);

                for(i = 0; i < 7; i++)
                {
                    aiSendDebugPoint(pak, interp_state.interp_positions[i], 0xffff0000, 1, 1);
                }

                aiSendDebugPoint(pak, e->net_pos, 0xff00ff00, 1, 1);
            }

            if(client->entDebugInfo & ENTDEBUGINFO_INTERP_LINE_GUESS)
            {
                // Send the guess curve as a PINK line.

                aiSendDebugPoint(pak, interp_state.last_pos, 0xff0000ff, 0, 0);

                for(i = 0; i < 7; i++)
                {
                    float* pos = interp_state.guess_pos[pos_to_binpos[i]];
                    if(pos[0] || pos[1] || pos[2])
                    {
                        aiSendDebugPoint(pak, pos, 0xffff00ff, 1, 1);
                    }
                }

                aiSendDebugPoint(pak, e->net_pos, 0xff00ff00, 1, 1);
            }

            // Send the pos history points as a AQUA line.

            if(client->entDebugInfo & ENTDEBUGINFO_INTERP_LINE_HISTORY)
            {
                int size = e->pos_history.end + 1 - e->pos_history.net_begin;

                if(size < 0)
                    size += ENT_POS_HISTORY_SIZE;

                for(i = 0; i < size; i++)
                {
                    aiSendDebugPoint(pak, e->pos_history.pos[(e->pos_history.net_begin + i) % ENT_POS_HISTORY_SIZE], 0xff00ffff, i > 0, i > 0);
                }
            }
        }

        // Send the list of lines.

        for(i = 0; i < ent_send_lines.count; i++){
            EntSendLine* line = ent_send_lines.lines + i;
            aiSendDebugPoint(pak, line->p1, line->argb1, 0, 0);
            aiSendDebugPoint(pak, line->p2, line->argb2, 1, 0);
        }
    }

    // Terminator bit.

    pktSendBits(pak, 1, 0);

    // Send the texts.

    if(    !server_visible_state.pause &&
    (    client->controls.selected_ent_server_index == interp_state.store_entity_id &&
        client->entDebugInfo & ENTDEBUGINFO_INTERP_TEXT ||
        client->entDebugInfo & ENTDEBUGINFO_FORCE_INTERP_TEXT))
    {
        for(i = 0; i < ent_send_texts.count; i++){
            EntSendText* text = ent_send_texts.texts + i;
            pktSendBits(pak, 1, 1);
            pktSendF32(pak, text->pos[0]);
            pktSendF32(pak, text->pos[1]);
            pktSendF32(pak, text->pos[2]);
            pktSendString(pak, text->text);
            pktSendBitsPack(pak, 1, text->flags);
        }
    }

    pktSendBits(pak, 1, 0);

    // active encounter info
    if(    !server_visible_state.pause &&
        //client->controls.selected_ent_server_index == interp_state.store_entity_id &&
        client->entDebugInfo & ENTDEBUGINFO_ENCOUNTERS)
    {
        int playercount, running, active, panicplayers, panic;
        int playerradius, villainradius, adjustprob, defaultplayer, defaultvillain, defaultadjust;
        int curcritters, mincritters, belowmin, maxcritters, abovemax;
        F32 success, playerratio, activeratio, paniclow, panichigh;
        EncounterGetStats(&playercount, &running, &active, &success, &playerratio, &activeratio, &panic);
        EncounterGetTweakNumbers(&playerradius, &villainradius, &adjustprob, &defaultplayer, &defaultvillain, &defaultadjust);
        EncounterGetPanicThresholds(&paniclow, &panichigh, &panicplayers);
        EncounterGetCritterNumbers(&curcritters, &mincritters, &belowmin, &maxcritters, &abovemax);
        pktSendBits(pak, 1, 1);
        pktSendBitsPack(pak, 1, playercount);
        pktSendBitsPack(pak, 1, running);
        pktSendBitsPack(pak, 1, active);
        pktSendF32(pak, success);
        pktSendF32(pak, playerratio);
        pktSendF32(pak, activeratio);
        pktSendBitsPack(pak, 1, panic);
        pktSendBitsPack(pak, 1, playerradius);
        pktSendBitsPack(pak, 1, villainradius);
        pktSendBitsPack(pak, 1, adjustprob);
        pktSendBitsPack(pak, 1, defaultplayer);
        pktSendBitsPack(pak, 1, defaultvillain);
        pktSendBitsPack(pak, 1, defaultadjust);
        pktSendF32(pak, paniclow);
        pktSendF32(pak, panichigh);
        pktSendBitsPack(pak, 1, panicplayers);
        pktSendBitsPack(pak, 1, curcritters);
        pktSendBitsPack(pak, 1, mincritters);
        pktSendBits(pak, 1, belowmin);
        pktSendBitsPack(pak, 1, maxcritters);
        pktSendBits(pak, 1, abovemax);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    // encounter beacon info
    if(    !server_visible_state.pause &&
        //client->controls.selected_ent_server_index == interp_state.store_entity_id &&
        client->entDebugInfo & ENTDEBUGINFO_ENCOUNTERBEACONS)
    {
        pktSendBits(pak, 1, 1);
        EncounterSendBeacons(pak, client->entity);
    }
    else
        pktSendBits(pak, 1, 0);


    // Send the debug buttons.

    #define DEBUG_BUTTON(display,command) {pktSendBits(pak,1,1);pktSendString(pak,display);pktSendString(pak,command);}

        if(client->debugMenuFlags & DEBUGMENUFLAGS_QUICKMENU){
            Entity* player = client->entity;
            Entity* selected_ent = validEntFromId(client->controls.selected_ent_server_index);
            EntType entType = selected_ent ? ENTTYPE(selected_ent) : ENTTYPE_NOT_SPECIFIED;
            int entDebugInfo = client->entDebugInfo;
            int entDebugInfoSent = 0;
            AIVars* aiSelected = selected_ent ? ENTAI(selected_ent) : NULL;

            sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_QUICKMENU);
            DEBUG_BUTTON("^1DISABLE ^0QuickMenu", buffer);

            DEBUG_BUTTON("^1DISABLE ALL", "entdebuginfo 0\nailog 0");
            DEBUG_BUTTON("^7SUPER MENU", "mmm");
            DEBUG_BUTTON("^2CLEAR LOGS", "clearailog");
            DEBUG_BUTTON("^2CLEAR LINES", "cleardebuglines");

            for(i = 0; i < ARRAY_SIZE(aiLogFlagToName); i++){
                int bit = aiLogFlagToName[i].flagBit;
                int enabled = AI_LOG_ENABLED(bit);
                sprintf(buffer, "%s ^0LOG: ^5%s", enabled ? "^2[x]" : "^1[  ]", aiLogFlagToName[i].name);
                sprintf(buffer2, enabled ? "&ailog !%d" : "|ailog %d", 1 << bit);
                DEBUG_BUTTON(buffer, buffer2);
            }

            #define CREATE_COMMAND(name,bit){                                                                    \
                if(!(entDebugInfoSent & (bit))){                                                                \
                entDebugInfoSent |= (bit);                                                                    \
                sprintf(buffer, "%s ^0INFO: ^6%s",                                                            \
                entDebugInfo & (bit) ? "^2[x]" : "^1[  ]", name);                                            \
                sprintf(buffer2, entDebugInfo & (bit) ? "&entdebuginfo !%d" : "|entdebuginfo %d", (bit));    \
                DEBUG_BUTTON(buffer, buffer2);                                                                \
                }                                                                                                \
            }

            #define CREATE_COMMAND_DEBUGVAR(name,var){                                                                    \
                sprintf(buffer, "%s ^0INFO: ^6%s",                                                            \
                clientLinkGetDebugVar(client, (var)) ? "^2[x]" : "^1[  ]", name);                            \
                sprintf(buffer2, clientLinkGetDebugVar(client, (var)) ? "setdebugvar %s 0" : "setdebugvar %s 1", (var));    \
                DEBUG_BUTTON(buffer, buffer2);                                                                \
            }

            CREATE_COMMAND("No Basic Info",                ENTDEBUGINFO_DISABLE_BASIC);
            CREATE_COMMAND("Sequencer",                    ENTDEBUGINFO_SEQUENCER);
            CREATE_COMMAND("AI Stuff",                    ENTDEBUGINFO_AI_STUFF);
            CREATE_COMMAND_DEBUGVAR("Behavior",            "Ai.Behavior");
            CREATE_COMMAND_DEBUGVAR("Behavior Hist",    "Ai.Behavior.History");
            CREATE_COMMAND("Path",                        ENTDEBUGINFO_PATH);
            CREATE_COMMAND("Physics",                    ENTDEBUGINFO_PHYSICS);
            CREATE_COMMAND("Combat",                    ENTDEBUGINFO_COMBAT);
            CREATE_COMMAND("Auto Powers",                ENTDEBUGINFO_AUTO_POWERS);

             switch(entType){
                case ENTTYPE_CRITTER:
                    CREATE_COMMAND("Team Powers",        ENTDEBUGINFO_TEAM_POWERS);
                    CREATE_COMMAND("Team Info",            ENTDEBUGINFO_TEAM_INFO);
                    CREATE_COMMAND("Status Table",        ENTDEBUGINFO_STATUS_TABLE);
                    CREATE_COMMAND("Feeling Info",        ENTDEBUGINFO_FEELING_INFO);
                    CREATE_COMMAND("Event History",        ENTDEBUGINFO_EVENT_HISTORY);

                    CREATE_COMMAND("Enhancements",        ENTDEBUGINFO_ENHANCEMENT);
                    CREATE_COMMAND("Powers",            ENTDEBUGINFO_POWERS);
                    CREATE_COMMAND("Critter",            ENTDEBUGINFO_CRITTER);

                    break;
                case ENTTYPE_PLAYER:
                    CREATE_COMMAND("Status Table",        ENTDEBUGINFO_STATUS_TABLE);
                    CREATE_COMMAND("Enhancements",        ENTDEBUGINFO_ENHANCEMENT);
                    CREATE_COMMAND("Powers",             ENTDEBUGINFO_POWERS);
                    CREATE_COMMAND("Player-Only",        ENTDEBUGINFO_PLAYER_ONLY);

                    if(selected_ent == player){
                        CREATE_COMMAND("Self",            ENTDEBUGINFO_SELF);
                    }else{
                        CREATE_COMMAND("Player",        ENTDEBUGINFO_PLAYER);
                    }

                    break;
                case ENTTYPE_NPC:
                    CREATE_COMMAND("NPC",                ENTDEBUGINFO_NPC);
                    break;
                case ENTTYPE_CAR:
                    CREATE_COMMAND("Car",                ENTDEBUGINFO_CAR);
                    break;
                case ENTTYPE_DOOR:
                    CREATE_COMMAND("Door",                ENTDEBUGINFO_DOOR);
                    break;
            }

            #define CREATE_USED(x, bit)            \
                if(entDebugInfo & (bit)){        \
                    CREATE_COMMAND(x, (bit));    \
                }

            CREATE_USED("Self",        ENTDEBUGINFO_SELF);
            CREATE_USED("Player",    ENTDEBUGINFO_PLAYER);
            CREATE_USED("Critter",    ENTDEBUGINFO_CRITTER);
            CREATE_USED("NPC",        ENTDEBUGINFO_NPC);
            CREATE_USED("Car",        ENTDEBUGINFO_CAR);
            CREATE_USED("Door",        ENTDEBUGINFO_DOOR);

            #undef CREATE_USED
            #undef CREATE_COMMAND
            #undef CREATE_COMMAND_DEBUGVAR

            if(selected_ent){
                DEBUG_BUTTON(ENTINFO(selected_ent).paused ? "^1UNPAUSE ENT" : "^2PAUSE ENT", "ec 0 pause toggle");
            }
        }
        else if(client->entDebugInfo){
            sprintf(buffer, "|debugmenuflags %d", DEBUGMENUFLAGS_QUICKMENU);
            DEBUG_BUTTON("^2ENABLE ^0QuickMenu", buffer);

            if(client->entDebugInfo || server_state.aiLogFlags){
                DEBUG_BUTTON("^1DISABLE ALL", "entdebuginfo 0\nailog 0\nclearailog");
            }
        }

        // End of buttons bit.

        pktSendBits(pak, 1, 0);

    #undef DEBUG_BUTTON
}

static void aiDebugSendMenuItem(Packet* pak,
                                const char* displayText,
                                const char* commandText,
                                int opened,
                                int hasSubItems,
                                char* rolloverText)
{
    if(!displayText)
    {
        // This is the end of group marker.
        pktSendBits(pak, 1, 0);
        return;
    }

    // It isn't end of group, so send a 1 bit.
    pktSendBits(pak, 1, 1);

    // Send a 1 bit if there's no sub-items.
    pktSendBits(pak, 1, hasSubItems);

    // Send bit for whether to pre-open a sub-group.
    pktSendBits(pak, 1, opened);

    // Send the rollover text.
    if(hasSubItems || !opened)
    {
        if(rolloverText)
        {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, rolloverText);
        }
        else
            pktSendBits(pak, 1, 0);
    }

    // Send the display string.
    pktSendString(pak, displayText);

    // If there's no sub-items, send command string.
    if(!hasSubItems)
        pktSendString(pak, commandText);
}

#if 0 // not called
static int aiConfigGetRefCount(AIConfig* config){
    int refCount = 0;
    int count;
    int i;

    // Check if a villaindef references it specifically.
    count = eaSize(&villainDefList.villainDefs);
    for(i = 0; i < count; i++)
    {
        if(villainDefList.villainDefs[i]->aiConfig && !stricmp(config->name, villainDefList.villainDefs[i]->aiConfig))
            return 2;
    }

    // Now if it's just the same name as a villaindef.
    for(i = 0; i < count; i++)
    {
        if(    villainDefList.villainDefs[i]->name && !stricmp(config->name, villainDefList.villainDefs[i]->name))
        {
            return 1;
        }
    }

    // Now if it's referenced by another config.
    count = eaSize(&aiAllConfigs.configs);
    for(i = 0; i < count; i++)
    {
        int baseCount = eaSize(&aiAllConfigs.configs[i]->baseConfigs);
        int j;
        for(j = 0; j < baseCount; j++)
        {
            if(!stricmp(config->name, aiAllConfigs.configs[i]->baseConfigs[j]->name))
                return 5;
        }
    }

    return 0;
}
#endif

static const char* getNavSearchResultText(int result)
{
    switch(result)
    {
        xcase NAV_RESULT_BAD_POSITIONS:
            return "^1BadPos";
        xcase NAV_RESULT_NO_SOURCE_BEACON:
            return "^1NoSourceBeacon";
        xcase NAV_RESULT_NO_TARGET_BEACON:
            return "^1NoTargetBeacon";
        xcase NAV_RESULT_BLOCK_ERROR:
            return "^1NoBeaconBlock";
        xcase NAV_RESULT_NO_BLOCK_PATH:
            return "^1NoBlockPath";
        xcase NAV_RESULT_NO_BEACON_PATH:
            return "^1NoBeaconPath";
        xcase NAV_RESULT_CLUSTER_CONN_BLOCKED:
            return "^1ClusterConnBlocked";
        xcase NAV_RESULT_SUCCESS:
            return "^2Success";
        xcase NAV_RESULT_PARTIAL:
            return "^2Partial";
        xdefault:
            return "^1Unknown";
    }
}

static const char* removePrefix(const char* s, const char* prefix)
{
    if(s && !strnicmp((char*)s, prefix, strlen(prefix)))
    {
        return s + strlen(prefix);
    }
    return s;
}

static const char* getShortServerCmdName(int cmd){    return removePrefix(getServerCmdName(cmd), "SERVER_"); }

const char* getShortClientInpCmdName(int cmd)
{
    const char* getClientInpCmdName(int cmd);
    return removePrefix(getClientInpCmdName(cmd), "cmd:CLIENTINP_");
}

#define BEGIN_GROUP(x, opened) aiDebugSendMenuItem(pak, (x), NULL, (opened), 1, NULL);
#define BEGIN_GROUP_ROLLOVER(x, opened, rollover) aiDebugSendMenuItem(pak, (x), NULL, (opened), 1, rollover);
#define SET_PREFIX(x, y) aiDebugSendMenuItem(pak, (x), (y), 1, 0, NULL)
#define CLEAR_PREFIX() aiDebugSendMenuItem(pak, "", "", 1, 0, NULL)
#define NEW_ITEM(x,y) aiDebugSendMenuItem(pak, (x), (y), 0, 0, NULL)
#define NEW_ITEM_ROLLOVER(x,y,rollover) aiDebugSendMenuItem(pak, (x), (y), 0, 0, rollover)
#define END_GROUP() aiDebugSendMenuItem(pak, NULL, NULL, 0, 0, NULL);
#define COMMANDBUFFERSIZE 200

// storyArcName name should be NULL if it is not a storyarc task
static void showTask(const StoryTask* task, const ContactDef* contactdef, const char* storyArcName, Packet* pak )
{
    char displaybuffer[COMMANDBUFFERSIZE];
    char commandbuffer[COMMANDBUFFERSIZE];
    char taskDescription[COMMANDBUFFERSIZE];
    int i;

    if( task->deprecated )
        return;

    _snprintf(taskDescription, COMMANDBUFFERSIZE, "MinLevel: %i\nMax Level: %i\nVillain Group: %s", task->minPlayerLevel, task->maxPlayerLevel, localizedPrintf(NULL, villainGroupGetPrintName(getVillainGroupFromTask(task))));

    BEGIN_GROUP_ROLLOVER( task->logicalname, 0, taskDescription);
    // StoryArc task
    if (storyArcName)
    {
        _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Get StoryArc Task");
        _snprintf( commandbuffer, COMMANDBUFFERSIZE, "GetStoryArcTask %s %s", storyArcName, task->logicalname );
        NEW_ITEM(displaybuffer, commandbuffer);

        if (task->type == TASK_MISSION)
        {
            const MissionDef *pDef = NULL;
            
            if (task->missiondef != NULL)
                pDef = task->missiondef[0];

            _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Get StoryArc Task And Goto Door");
            _snprintf( commandbuffer, COMMANDBUFFERSIZE, "GetStoryArcTask %s %s$$GotoMission", storyArcName, task->logicalname );
            NEW_ITEM(displaybuffer, commandbuffer);

            // see if we have a map list and not a single specified map
            if (pDef && !pDef->mapfile)
            {
                MissionMap **pMapList = MissionMapGetList(pDef->mapset, pDef->maplength);

                if (pMapList != NULL) 
                {
                    int nummaps = eaSize(&pMapList);
                    BEGIN_GROUP_ROLLOVER( "Map List", 0, "Select Map");

                    for (i = 0; i < nummaps; i++)
                    {
                        _snprintf( displaybuffer, COMMANDBUFFERSIZE, "%s", pMapList[i]->mapfile);
                        _snprintf( commandbuffer, COMMANDBUFFERSIZE, "startstoryarcmission %s %s %s", 
                                    storyArcName, task->logicalname, pMapList[i]->mapfile );
                        NEW_ITEM(displaybuffer, commandbuffer);
                    }
                    END_GROUP()
                }
            }
        }
    }
    else // regular task
    {
        _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Get Task");
        _snprintf( commandbuffer, COMMANDBUFFERSIZE, "GetContactTask %s %s", contactdef->filename, task->logicalname );
        NEW_ITEM(displaybuffer, commandbuffer);
        if (task->type == TASK_MISSION)
        {
            const MissionDef *pDef = NULL;
            
            if (task->missiondef != NULL)
                pDef = task->missiondef[0];

            _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Get Task And Goto Door");
            _snprintf( commandbuffer, COMMANDBUFFERSIZE, "GetContactTask %s %s$$GotoMission", contactdef->filename, task->logicalname );
            NEW_ITEM(displaybuffer, commandbuffer);

            // see if we have a map list and not a single specified map
            if (pDef && !pDef->mapfile)
            {
                MissionMap **pMapList = MissionMapGetList(pDef->mapset, pDef->maplength);

                if (pMapList != NULL) 
                {
                    int nummaps = eaSize(&pMapList);
                    BEGIN_GROUP_ROLLOVER( "Map List", 0, "Select Map");

                    for (i = 0; i < nummaps; i++)
                    {
                        _snprintf( displaybuffer, COMMANDBUFFERSIZE, "%s", pMapList[i]->mapfile);
                        _snprintf( commandbuffer, COMMANDBUFFERSIZE, "startmission %s %s", task->logicalname, pMapList[i]->mapfile );
                        NEW_ITEM(displaybuffer, commandbuffer);
                    }
                    END_GROUP()
                }
            }
        }
    }
    END_GROUP()
}

static void showStoryArc( const StoryArcRef * storyarcref, const ContactDef * contactdef, Packet* pak )
{
    int i, j;
    const StoryArc* storyarc = StoryArcDefinition(&storyarcref->sahandle);
    char storyArcLogical[COMMANDBUFFERSIZE];
    char storyArcDescription[COMMANDBUFFERSIZE];

    if( !storyarc || (storyarc->deprecated && !(storyarc->flags & STORYARC_FLASHBACK)))
        return;

    saUtilFilePrefix(storyArcLogical, storyarc->filename);
    _snprintf(storyArcDescription, COMMANDBUFFERSIZE, "MinLevel: %i\nMax Level: %i\n", storyarc->minPlayerLevel, storyarc->maxPlayerLevel);

    BEGIN_GROUP_ROLLOVER( storyArcLogical, 0, storyArcDescription);
    {
        char displaybuffer[COMMANDBUFFERSIZE];
        char commandbuffer[COMMANDBUFFERSIZE];

        _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Get StoryArc" );
        _snprintf( commandbuffer, COMMANDBUFFERSIZE, "GetContactStoryArc %s %s", contactdef->filename, storyarc->filename );

        NEW_ITEM(displaybuffer, commandbuffer);

        //Display All My Children
        for(i = 0; i < eaSize(&storyarc->episodes); i++)
            for (j = 0; j < eaSize(&storyarc->episodes[i]->tasks); j++)
                showTask(storyarc->episodes[i]->tasks[j], contactdef, storyarc->filename, pak);
    }
    END_GROUP()
}

static int cmpLogicalName(const ContactDef **va,const ContactDef **vb)
{
    int x;
    char a[100];
    char b[100];

    ScriptVarsTable vars = {0};

    if((*va)->tipType != (*vb)->tipType)
        return (*va)->tipType < (*vb)->tipType ? -1 : 1;

    ScriptVarsTablePushScope(&vars, &(*va)->vs);
    strcpy_s(SAFESTR(a), saUtilTableLocalize( (*va)->displayname, &vars ));
    ScriptVarsTablePopScope(&vars);

    ScriptVarsTablePushScope(&vars, &(*vb)->vs);
    strcpy_s(SAFESTR(b), saUtilTableLocalize( (*vb)->displayname, &vars ));

    x = stricmp( a, b );
    if( !x )
        x = stricmp( (*va)->filename, (*vb)->filename );
    return x;
}

static int comparePowerCats(const void* p1Data, const void* p2Data){
    const PowerCategory ** p1 = (const PowerCategory **)p1Data;
    const PowerCategory ** p2 = (const PowerCategory **)p2Data; return stricmp( (*p1)->pchName, (*p2)->pchName ); }
static int comparePowerSets(const void* p1Data, const void* p2Data){
    const BasePowerSet ** p1 = (const BasePowerSet **)p1Data;
    const BasePowerSet ** p2 = (const BasePowerSet **)p2Data; return stricmp( (*p1)->pchName, (*p2)->pchName ); }

static int cmpLocationName(const void* vaData, const void* vbData)
{
    const ContactDef ** va = (const ContactDef **)vaData;
    const ContactDef ** vb = (const ContactDef **)vbData;
    int x;
    char a[100];
    char b[100];

    if((*va)->tipType != (*vb)->tipType)
        return (*va)->tipType < (*vb)->tipType ? -1 : 1;
 
    // Compare the location names
    strcpy_s(SAFESTR(a), saUtilScriptLocalize( (*va)->locationName?(*va)->locationName:"" ));
    strcpy_s(SAFESTR(b), saUtilScriptLocalize( (*vb)->locationName?(*vb)->locationName:"" ));
    x = stricmp( a, b );

    // If the same, then go to logical name
    if( !x )
        return cmpLogicalName(va, vb);
    return x;
}

static int cmpLogicalName(const ContactDef **va,const ContactDef **vb);
static int cmpLogicalNameCallback(const void* arg0, const void* arg1)
{
    return cmpLogicalName((const ContactDef **)arg0, (const ContactDef **)arg1);
}

static showContactsAndMissions(Packet* pak, int sortByZone)
{
    int i,k,n,p, currentTipType;
    char currentLocation[256], currentChar[2]={0};
    const ContactDef ** pContacts;
    const ContactDef * contact;
    const ContactDef ** sortedContacts;

    //Show All the contacts
    pContacts = g_contactlist.contactdefs; 
    n = eaSize(&pContacts);

    sortedContacts = calloc( n, sizeof( void* ) );   
    for (i = 0; i < n; i++)
        sortedContacts[i] = pContacts[i];

    if (sortByZone) 
        qsort((void**)sortedContacts,n,sizeof(void*),cmpLocationName);
    else
        qsort((void**)sortedContacts,n,sizeof(void*),cmpLogicalNameCallback);

    currentLocation[0] = '\0';
    currentTipType = 0;
    
    BEGIN_GROUP("Regular Contact",0)

    for (i = 0; i < n; i++) 
    {
        char displaybuffer[COMMANDBUFFERSIZE];
        char commandbuffer[COMMANDBUFFERSIZE];
        char locationName[256];
        char name[256];
 
        contact = sortedContacts[i];
        strcpy_s(SAFESTR(name), ContactDisplayName(contact, NULL));

        // don't display auto-dismissed contacts, they will just be immediately removed anyways
        if (CONTACT_IS_AUTODISMISSED(contact))
            continue;

        // new tip type heading
        if(currentTipType != contact->tipType)
        {
            END_GROUP() // end the current location/character group
            END_GROUP() // end the current tip group
            if(contact->tipTypeStr && contact->tipTypeStr[0])
                BEGIN_GROUP(contact->tipTypeStr, 0)
            else
                BEGIN_GROUP("Unknown", 0)
            currentTipType = contact->tipType;

            currentLocation[0] = '\0'; // reset these headings
            currentChar[0] = '\0';
        }

        // Sort by zone grouping
        if (sortByZone)
        {
            strcpy_s(SAFESTR(locationName), contact->locationName?saUtilScriptLocalize(contact->locationName):"");
            if (!locationName[0])
                strcpy(locationName, "Miscellaneous(No Location)");

            // New location, end the old group, start a new one
            if (!currentLocation[0] || stricmp(locationName, currentLocation))
            {
                if (currentLocation[0])
                    END_GROUP()

                strcpy_s(SAFESTR(currentLocation), locationName);
                BEGIN_GROUP(currentLocation, 0)
            }
        }
        else
        {
            if( name[0] != currentChar[0] )
            {
                if( currentChar[0] )
                    END_GROUP()
                currentChar[0] = name[0];
                BEGIN_GROUP(currentChar, 0)
            }
        }

        BEGIN_GROUP( name, 0);
        {
            // Debug commands
            _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Add to Contact List" );
            _snprintf( commandbuffer, COMMANDBUFFERSIZE, "AddContact %s", contact->filename );
            NEW_ITEM(displaybuffer, commandbuffer);

            _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Fill Contact Relationship" );
            _snprintf( commandbuffer, COMMANDBUFFERSIZE, "ContactCXP %s %i", contact->filename, contact->completeCP );
            NEW_ITEM(displaybuffer, commandbuffer);

            _snprintf( displaybuffer, COMMANDBUFFERSIZE, "Talk to Contact" );
            _snprintf( commandbuffer, COMMANDBUFFERSIZE, "ContactDialog %s", contact->filename );
            NEW_ITEM(displaybuffer, commandbuffer);

            // Show all my Tasks
            if (p = eaSize(&contact->tasks))
            {
                BEGIN_GROUP("Tasks", 0)
                for(k = 0; k < p; k++)
                    showTask(contact->tasks[k], contact, NULL, pak);
                END_GROUP()
            }

            // Show all my Story Arcs
            if (p = eaSize(&contact->storyarcrefs))
            {
                BEGIN_GROUP("StoryArcs", 0)
                for(k = 0; k < p; k++)
                    if (contact->storyarcrefs[k])
                        showStoryArc( contact->storyarcrefs[k], contact, pak);
                END_GROUP()
            }
        }
        END_GROUP()
    }

    END_GROUP()
    END_GROUP()

    free((void**)sortedContacts);
}

static int getClusterBeaconCount(BeaconBlock* cluster){
    int count = 0;
    int i;
    
    for(i = 0; i < cluster->subBlockArray.size; i++)
    {
        BeaconBlock* galaxy = cluster->subBlockArray.storage[i];
        int j;
        
        for(j = 0; j < galaxy->subBlockArray.size; j++)
        {
            BeaconBlock* subBlock = galaxy->subBlockArray.storage[j];
            count += subBlock->beaconArray.size;
        }
    }
    
    return count;
}

typedef struct ClientPacketLogEntrySortable
{
    ClientSubPacketLogEntry*    entry;
    int                            oop;
} ClientPacketLogEntrySortable;

void aiDebugSendEntDebugMenu(ClientLink* client)
{
    char buffer[1000];
    char buffer2[1000];
    Packet* pak = pktCreate();
    Entity* e = NULL;
    Entity* player = clientGetControlledEntity(client);
    int entID = client->controls.selected_ent_server_index;
    int i;
    EntType entType;
    int open;
    int entDebugInfo = client->entDebugInfo;
    int accesslevel = 0;

    if( player )
        accesslevel = (player->access_level>=9);

    PERFINFO_AUTO_START("sendEntDebugMenu", 1);

    pktSetCompression(pak, 1);

    pktSendBitsPack(pak,1,SERVER_SENDENTDEBUGMENU);

    e = validEntFromId(entID);

    entType = e ? ENTTYPE(e) : ENTTYPE_NOT_SPECIFIED;



    BEGIN_GROUP("Debug Menu", 1)
        if(entDebugInfo || server_state.aiLogFlags){
            strcpy(buffer,"");
            strcat(buffer, entDebugInfo ? "entdebuginfo 0\n" : "");
            strcat(buffer, server_state.aiLogFlags ? "ailog 0\n" : "");

            if(buffer[0])
                buffer[strlen(buffer)-1] = '\0';

            if(accesslevel)
                NEW_ITEM("^1DISABLE ^dALL Server Debugging", buffer);
        }

        if(accesslevel){
            if(server_state.aiLogFlags){
                    NEW_ITEM("^1DISABLE ^dALL AI Logs", "ailog 0");
            }

            if(entDebugInfo || server_state.aiLogFlags){
                if(accesslevel)
                    NEW_ITEM("Clear AI Logs", "clearailog");
            }
        }
        if(player){
            sprintf(buffer, "Me (%s^d)", AI_LOG_ENT_NAME(player));
            BEGIN_GROUP(buffer, 0)
                sprintf(buffer, "%s ^6Self ^dINFO",
                        entDebugInfo & ENTDEBUGINFO_SELF ? "^1DISABLE" : "^2ENABLE");
                sprintf(buffer2, entDebugInfo & ENTDEBUGINFO_SELF ? "&entdebuginfo !%d" : "|entdebuginfo %d", ENTDEBUGINFO_SELF);

                if(accesslevel)
                    NEW_ITEM(buffer, buffer2);

                NEW_ITEM(    client->controls.nocoll ? "^1DISABLE ^dNo Collisions Movement" : "^2ENABLE ^dNo Collisions Movement",
                            client->controls.nocoll ? "nocoll 0" : "nocoll 1");

                sprintf(buffer,
                        "ec me sethealth %1.3f\nec me setendurance %1.3f\nec me setinsight %1.3f",
                        player->pchar->attrMax.fHitPoints,
                        player->pchar->attrMax.fEndurance,
                        player->pchar->attrMax.fInsight);
                NEW_ITEM("REFILL ^2HP^d, ^5Endurance ^d and Insight", buffer);

                NEW_ITEM("RECHARGE My Powers", "ec me rechargepower -1");

                BEGIN_GROUP("Flags", 1)
                    NEW_ITEM(    player->invincible ? "^1DISABLE ^dInvincible" : "^2ENABLE ^dInvincible",
                                player->invincible ? "invincible 0" : "invincible 1");

                    NEW_ITEM(    player->unstoppable ? "^1DISABLE ^dUnstoppable" : "^2ENABLE ^dUnstoppable",
                                player->unstoppable ? "unstoppable 0" : "unstoppable 1");

                    NEW_ITEM(    player->doNotTriggerSpawns ? "^1DISABLE ^dDoNotTriggerSpawns" : "^2ENABLE ^dDoNotTriggerSpawns",
                                player->doNotTriggerSpawns ? "donottriggerspawns 0" : "donottriggerspawns 1");

                    NEW_ITEM(    player->alwaysHit ? "^1DISABLE ^dAlways Hit" : "^2ENABLE ^dAlways Hit",
                                player->alwaysHit ? "alwayshit 0" : "alwayshit 1");

                    NEW_ITEM(    player->untargetable ? "^1DISABLE ^dUntargetable" : "^2ENABLE ^dUntargetable",
                                player->untargetable ? "untargetable 0" : "untargetable 1");

                    NEW_ITEM(    ENTHIDE(player) ? "^1DISABLE ^dInvisibility to others" : "^2ENABLE ^dInvisibility to others",
                                ENTHIDE(player) ? "invisible 0" : "invisible 1");

                    NEW_ITEM(    player->pchar->bIgnoreCombatMods ? "^1DISABLE ^dIgnore Combat Mods" : "^2ENABLE ^dIgnore Combat Mods",
                                player->pchar->bIgnoreCombatMods ? "ec me ignorecombatmods 0" : "ec me ignorecombatmods 1");

                    if (accesslevel && player->pl)
                    {
                        if (ENT_IS_HERO(player)) {
                            sprintf( buffer, "ec %d setplayertype %d", entGetId(player), kPlayerType_Villain );
                            NEW_ITEM("^1BECOME ^0Villain", buffer);
                        } else {
                            sprintf( buffer2, "ec %d setplayertype %d", entGetId(player), kPlayerType_Hero );
                            NEW_ITEM("^1BECOME ^0Hero", buffer2);
                        }
                    }
                    if (accesslevel && player->pl)
                    {
                        if (ENT_IS_HERO(player)) {
                            if (player->pl->playerSubType != kPlayerSubType_Paragon) {
                                sprintf( buffer, "ec %d setplayersubtype %d", entGetId(player), kPlayerSubType_Paragon );
                                NEW_ITEM("^1BECOME ^0Paragon", buffer);
                            }

                            if (player->pl->playerSubType != kPlayerSubType_Normal) {
                                sprintf( buffer, "ec %d setplayersubtype %d", entGetId(player), kPlayerSubType_Normal );
                                NEW_ITEM("^1BECOME ^0Normal Hero", buffer);
                            }

                            if (player->pl->playerSubType != kPlayerSubType_Rogue) {
                                sprintf( buffer, "ec %d setplayersubtype %d", entGetId(player), kPlayerSubType_Rogue );
                                NEW_ITEM("^1BECOME ^0Vigilante", buffer);
                            }
                        } else {
                            if (player->pl->playerSubType != kPlayerSubType_Paragon) {
                                sprintf( buffer, "ec %d setplayersubtype %d", entGetId(player), kPlayerSubType_Paragon );
                                NEW_ITEM("^1BECOME ^0Tyrant", buffer);
                            }

                            if (player->pl->playerSubType != kPlayerSubType_Normal) {
                                sprintf( buffer, "ec %d setplayersubtype %d", entGetId(player), kPlayerSubType_Normal );
                                NEW_ITEM("^1BECOME ^0Normal Villain", buffer);
                            }

                            if (player->pl->playerSubType != kPlayerSubType_Rogue) {
                                sprintf( buffer, "ec %d setplayersubtype %d", entGetId(player), kPlayerSubType_Rogue );
                                NEW_ITEM("^1BECOME ^0Rogue", buffer);
                            }
                        }
                    }
                    if (accesslevel && player->pl) {
                        if (player->pl->praetorianProgress != kPraetorianProgress_PrimalBorn) {
                            sprintf(buffer, "ec %d setpraetorian %d", entGetId(player), kPraetorianProgress_PrimalBorn);
                            NEW_ITEM("^1BECOME ^0Primal Earth Born", buffer);
                        }
                        if (player->pl->praetorianProgress != kPraetorianProgress_NeutralInPrimalTutorial) {
                            sprintf(buffer, "ec %d setpraetorian %d", entGetId(player), kPraetorianProgress_NeutralInPrimalTutorial);
                            NEW_ITEM("^1BECOME ^0Neutral in Primal tutorial", buffer);
                        }
                        if (player->pl->praetorianProgress != kPraetorianProgress_Tutorial) {
                            sprintf(buffer, "ec %d setpraetorian %d", entGetId(player), kPraetorianProgress_Tutorial);
                            NEW_ITEM("^1BECOME ^0Praetorian in tutorial", buffer);
                        }
                        if (player->pl->praetorianProgress != kPraetorianProgress_Praetoria) {
                            sprintf(buffer, "ec %d setpraetorian %d", entGetId(player), kPraetorianProgress_Praetoria);
                            NEW_ITEM("^1BECOME ^0Praetorian before Rift Encl.", buffer);
                        }
                        if (player->pl->praetorianProgress != kPraetorianProgress_PrimalEarth) {
                            sprintf(buffer, "ec %d setpraetorian %d", entGetId(player), kPraetorianProgress_PrimalEarth);
                            NEW_ITEM("^1BECOME ^0Ex-Praetorian", buffer);
                        }
                    }
                    if(accesslevel && player->pchar)
                    {
                        sprintf( buffer, "ec %d setgang 0", entGetId(player) );
                        sprintf( buffer2,"Leave Gang %d", player->pchar->iGangID );
                        NEW_ITEM(player->pchar->iGangID ? buffer2 : "No Gang", buffer);

                        sprintf(buffer, "ec %d setteam 0 %d", entGetId(player), player->pchar->iAllyID!=kAllyID_Good);
                        NEW_ITEM(player->pchar->iAllyID==kAllyID_Good ? "^1DISABLE ^0Ally Hero/Good" : "^2ENABLE ^0Ally Hero/Good", buffer);

                        sprintf(buffer, "ec %d setteam 1 %d", entGetId(player), player->pchar->iAllyID!=kAllyID_Evil);
                        NEW_ITEM(player->pchar->iAllyID==kAllyID_Evil ? "^1DISABLE ^0Ally Monster/Evil" : "^2ENABLE ^0Ally Monster/Evil", buffer);

                        sprintf(buffer, "ec %d setteam 2 %d", entGetId(player), player->pchar->iAllyID!=kAllyID_Foul);
                        NEW_ITEM(player->pchar->iAllyID==kAllyID_Foul ? "^1DISABLE ^0Ally Villain/Foul" : "^2ENABLE ^0Ally Villain/Foul", buffer);

                        for(i = 3; i <= 10; i++){
                            sprintf(buffer, "ec %d setteam %d 1", entGetId(player), i);
                            sprintf(buffer2, "Set team to %d%s", i, player->pchar->iAllyID==i?" <-- current team":"");
                            NEW_ITEM(buffer2, buffer);
                        }

                        if (player->pchar->playerTypeByLocation != kPlayerType_Hero)
                        {
                            sprintf(buffer, "ec %d setinfluencetype %d", entGetId(player), kPlayerType_Hero);
                            NEW_ITEM("Set Influence Type to 'Influence'", buffer);
                        }

                        if (player->pchar->playerTypeByLocation != kPlayerType_Villain)
                        {
                            sprintf(buffer, "ec %d setinfluencetype %d", entGetId(player), kPlayerType_Villain);
                            NEW_ITEM("Set Influence Type to 'Infamy'", buffer);
                        }
                    }
                END_GROUP()

                BEGIN_GROUP("Permanent Flags", 1)
                    NEW_ITEM(    player->invincible == INVINCIBLE_DBFLAG ? "^1DISABLE ^dInvincible" : "^2ENABLE ^dInvincible",
                                player->invincible == INVINCIBLE_DBFLAG ? "invincible 0" : "invincible 2");
                    NEW_ITEM(    player->untargetable == UNTARGETABLE_DBFLAG ? "^1DISABLE ^dUntargetable" : "^2ENABLE ^dUntargetable",
                                player->untargetable == UNTARGETABLE_DBFLAG ? "untargetable 0" : "untargetable 2");
                    NEW_ITEM(    ENTHIDE(player) == 2 ? "^1DISABLE ^dInvisibility to others" : "^2ENABLE ^dInvisibility to others",
                                ENTHIDE(player) == 2 ? "invisible 0" : "invisible 2");
                END_GROUP()

                sprintf(buffer,
                        "sethp$$Set ^2HP^t100t\t^b%d/50b ^d(^4%1.1f^d)",
                        (int)ceil(50 * player->pchar->attrCur.fHitPoints / player->pchar->attrMax.fHitPoints),
                        player->pchar->attrCur.fHitPoints);
                BEGIN_GROUP(buffer, 0)
                    sprintf(buffer, "ec me sethealth ");
                    SET_PREFIX("", buffer);
                    sprintf(buffer, "Max (^4%1.3f^d)", player->pchar->attrMax.fHitPoints);
                    sprintf(buffer2, "%f", player->pchar->attrMax.fHitPoints);
                    NEW_ITEM(buffer, buffer2);
                    NEW_ITEM("1", "1");
                    NEW_ITEM("0", "0");
                END_GROUP()

                sprintf(buffer,
                        "setend$$Set ^5Endurance^t100t\t^b%d/50b ^d(^4%1.1f^d)",
                        (int)ceil(50 * player->pchar->attrCur.fEndurance / player->pchar->attrMax.fEndurance),
                        player->pchar->attrCur.fEndurance);
                BEGIN_GROUP(buffer, 0)
                    sprintf(buffer, "ec me setendurance ");
                    SET_PREFIX("", buffer);
                    sprintf(buffer, "Max (^4%1.3f^d)", player->pchar->attrMax.fEndurance);
                    sprintf(buffer2, "%f", player->pchar->attrMax.fEndurance);
                    NEW_ITEM(buffer, buffer2);
                    NEW_ITEM("1", "1");
                    NEW_ITEM("0", "0");
                END_GROUP()

                sprintf(buffer,
                        "setins$$Set ^5Insight^t100t\t^b%d/50b ^d(^4%1.1f^d)",
                        (int)ceil(50 * player->pchar->attrCur.fInsight / player->pchar->attrMax.fInsight),
                        player->pchar->attrCur.fInsight);
                BEGIN_GROUP(buffer, 0)
                    sprintf(buffer, "ec me setinsight ");
                    SET_PREFIX("", buffer);
                    sprintf(buffer, "Max (^4%1.3f^d)", player->pchar->attrMax.fInsight);
                    sprintf(buffer2, "%f", player->pchar->attrMax.fInsight);
                    NEW_ITEM(buffer, buffer2);
                    NEW_ITEM("1", "1");
                    NEW_ITEM("0", "0");
                END_GROUP()

                BEGIN_GROUP("Speed Scale", 0)
                    SET_PREFIX("", "speed_scale ");
                    NEW_ITEM("1", "1");
                    NEW_ITEM("5", "5");
                    NEW_ITEM("10", "10");
                END_GROUP()

                BEGIN_GROUP("Combat Mod Shift", 0)
                    SET_PREFIX("", "set_combat_mod_shift ");
                    NEW_ITEM("0", "0");
                    NEW_ITEM("+1", "1");
                    NEW_ITEM("+2", "2");
                    NEW_ITEM("+3", "3");
                    NEW_ITEM("+4", "4");
                    NEW_ITEM("+5", "5");
                    NEW_ITEM("+6", "6");
                END_GROUP()
            END_GROUP()
        }

        BEGIN_GROUP("Entities", 0)
             if( accesslevel ){ 

                BEGIN_GROUP("Misc", 0)
                    NEW_ITEM("^1PAUSE ^dAll Entities", "ec -1 pause 1");
                    NEW_ITEM("^2UNPAUSE ^dAll Entities", "ec -1 pause 0");

                    NEW_ITEM("Reload Priority Lists", "reloadpriority");
                    NEW_ITEM("Reload AI Configs", "reloadaiconfig");
                    NEW_ITEM("Set All Health to 1", "ec -1 sethealth 1");
                    NEW_ITEM( server_state.disableConsiderEnemyLevel ? "^2ENABLE ^dAI Considers Enemy Level" : "^1DISABLE ^dAI Considers Enemy Level",
                              server_state.disableConsiderEnemyLevel ? "disableConsiderEnemyLevel 0"          : "disableConsiderEnemyLevel 1"        );

                    NEW_ITEM(    entgenIsPaused() ? "^2UNPAUSE ^dGenerators" : "^1PAUSE ^dGenerators",
                                entgenIsPaused() ? "ec -1 entgens resume" : "ec -1 entgens pause");

                    if(entSendGetInterpStoreEntityId()){
                        NEW_ITEM("Unset As Debug Entity On Server", "setdebugentity 0");
                        NEW_ITEM("Clear Debug Lines", "cleardebuglines");
                    }

                    if(e){
                        sprintf(buffer, "setdebugentity %d", e->owner);
                        NEW_ITEM("Set As Debug Entity On Server", buffer);
                    }
                END_GROUP()

                sprintf(buffer, "edi$$Entity Debug Info (^0Cur = ^4%d^d)", entDebugInfo);
                BEGIN_GROUP(buffer, 0)
                    NEW_ITEM("^1DISABLE ^dALL", "entdebuginfo 0");

                    #define CREATE_COMMAND(name,bit){                                                                \
                        sprintf(buffer, "%s ^d%s",                                                                    \
                                entDebugInfo & (bit) ? "^1DISABLE" : "^2ENABLE", name);                                \
                        sprintf(buffer2, entDebugInfo & (bit) ? "&entdebuginfo !%d" : "|entdebuginfo %d", (bit));    \
                        NEW_ITEM(buffer, buffer2);                                                                    \
                    }

                    BEGIN_GROUP("Entity Types", 0)
                        CREATE_COMMAND("Critters",            ENTDEBUGINFO_CRITTER);
                        CREATE_COMMAND("Players",            ENTDEBUGINFO_PLAYER);
                        CREATE_COMMAND("Self",                ENTDEBUGINFO_SELF);
                        CREATE_COMMAND("Cars",                ENTDEBUGINFO_CAR);
                        CREATE_COMMAND("Doors",                ENTDEBUGINFO_DOOR);
                        CREATE_COMMAND("NPCs",                ENTDEBUGINFO_NPC);
                    END_GROUP()

                    BEGIN_GROUP("Visible Data", 0)
                        BEGIN_GROUP("Combat", 0)
                            CREATE_COMMAND("Combat",        ENTDEBUGINFO_COMBAT);
                            i = (int)(intptr_t)clientLinkGetDebugVar(client, "Combat.Stats");
                            NEW_ITEM(    i ? "^1DISABLE ^dCombat Stats" : "^2ENABLE ^dCombat Stats",
                                        i ? "setdebugvar Combat.Stats 0" : "setdebugvar Combat.Stats 1");
                            i = (int)(intptr_t)clientLinkGetDebugVar(client, "Combat.Mods");
                            NEW_ITEM(    i ? "^1DISABLE ^dCombat Mods" : "^2ENABLE ^dCombat Mods",
                                        i ? "setdebugvar Combat.Mods 0" : "setdebugvar Combat.Mods 1");
                            i = (int)(intptr_t)clientLinkGetDebugVar(client, "Combat.Attribs");
                            NEW_ITEM(    i ? "^1DISABLE ^dCombat Attribs" : "^2ENABLE ^dCombat Attribs",
                                        i ? "setdebugvar Combat.Attribs 0" : "setdebugvar Combat.Attribs 1");
                        END_GROUP()
                        
                        CREATE_COMMAND("No Basic Info",        ENTDEBUGINFO_DISABLE_BASIC);
                        CREATE_COMMAND("Powers",            ENTDEBUGINFO_POWERS);
                        CREATE_COMMAND("Enhancements",        ENTDEBUGINFO_ENHANCEMENT);
                        CREATE_COMMAND("Sequencer",            ENTDEBUGINFO_SEQUENCER);
                        CREATE_COMMAND("Status Table",        ENTDEBUGINFO_STATUS_TABLE);
                        CREATE_COMMAND("Event History",        ENTDEBUGINFO_EVENT_HISTORY);
                        CREATE_COMMAND("Feeling Info",        ENTDEBUGINFO_FEELING_INFO);
                        CREATE_COMMAND("AI Stuff",            ENTDEBUGINFO_AI_STUFF);
                        i = (int)(intptr_t)clientLinkGetDebugVar(client, "Ai.Behavior");
                        NEW_ITEM(    i ? "^1DISABLE ^dBehavior" : "^2ENABLE ^dBehavior",
                            i ? "setdebugvar Ai.Behavior 0" : "setdebugvar Ai.Behavior 1");
                        i = (int)(intptr_t)clientLinkGetDebugVar(client, "Ai.Behavior.History");
                        NEW_ITEM(    i ? "^1DISABLE ^dBehavior History" : "^2ENABLE ^dBehavior History",
                            i ? "setdebugvar Ai.Behavior.History 0" : "setdebugvar Ai.Behavior.History 1");
                        CREATE_COMMAND("Team Info",            ENTDEBUGINFO_TEAM_INFO);
                        CREATE_COMMAND("Team Power Info",    ENTDEBUGINFO_TEAM_POWERS);
                        CREATE_COMMAND("Path",                ENTDEBUGINFO_PATH);
                         CREATE_COMMAND("Physics",            ENTDEBUGINFO_PHYSICS);
                        CREATE_COMMAND("Auto Powers",        ENTDEBUGINFO_AUTO_POWERS);
                        CREATE_COMMAND("Player-Only",        ENTDEBUGINFO_PLAYER_ONLY);
                    END_GROUP()

                    BEGIN_GROUP("World Lines & Text",0)
                        CREATE_COMMAND("Interp Curve",                ENTDEBUGINFO_INTERP_LINE_CURVE);
                        CREATE_COMMAND("Interp Guess Curve",        ENTDEBUGINFO_INTERP_LINE_GUESS);
                        CREATE_COMMAND("Interp Position History",    ENTDEBUGINFO_INTERP_LINE_HISTORY);
                        CREATE_COMMAND("Interp Text",                ENTDEBUGINFO_INTERP_TEXT);
                        CREATE_COMMAND("Force Interp Text",            ENTDEBUGINFO_FORCE_INTERP_TEXT);
                        CREATE_COMMAND("Misc Lines",                ENTDEBUGINFO_MISC_LINES);
                        NEW_ITEM("Clear Debug Lines", "cleardebuglines");
                    END_GROUP()

                    #undef CREATE_COMMAND
                END_GROUP()

                BEGIN_GROUP("Client Entity Debugging", 0)
                    SET_PREFIX("", "entdebugclient ");
                    NEW_ITEM("^1NONE", "0");
                    NEW_ITEM("Position Lines", "1");
                    NEW_ITEM("Lines To All Ents", "2");
                    NEW_ITEM("Lines To All Players", "32");
                    NEW_ITEM("Hide All Entities", "4");
                    NEW_ITEM("Show Crazy Prediction Lines", "8");
                    NEW_ITEM("Show My Server Position", "16");
                    NEW_ITEM("Show My Orientation", "32");
                    NEW_ITEM("Show Nearby Capsules", "64");
                END_GROUP()

                sprintf(buffer, "ailog$$AI Log (^0Cur = ^4%d^d)", server_state.aiLogFlags);
                BEGIN_GROUP(buffer, 0)
                {
                    int i;

                    NEW_ITEM("Clear AI Logs", "clearailog");
                    NEW_ITEM("^1DISABLE ^dALL", "ailog 0");
                    NEW_ITEM("^2ENABLE ^dALL", "ailog -1");

                    #define CREATE_COMMAND(name,bit){                                                            \
                        sprintf(buffer, "%s ^d%s (^4%d^d)",                                                        \
                                server_state.aiLogFlags & (bit) ? "^1DISABLE" : "^2ENABLE", name, (bit));        \
                        sprintf(buffer2, server_state.aiLogFlags & (bit) ? "&ailog !%d" : "|ailog %d", (bit));    \
                        NEW_ITEM(buffer, buffer2);                                                                \
                    }

                    for(i = 0; i < ARRAY_SIZE(aiLogFlagToName); i++){
                        CREATE_COMMAND(aiLogFlagToName[i].name, 1 << aiLogFlagToName[i].flagBit);
                    }

                    #undef CREATE_COMMAND
                }
                END_GROUP()
            }

            BEGIN_GROUP("Goto", 0)
                SET_PREFIX(    "Next ",        "next");
                NEW_ITEM(    "Critter",        "critter");
                NEW_ITEM(    "Player",        "player");
                NEW_ITEM(    "NPC",            "npc");
                NEW_ITEM(    "Hostage",        "hostage");
                NEW_ITEM(    "Car",            "car");
                CLEAR_PREFIX();

                NEW_ITEM("Go back to first item",    "nextreset");
            END_GROUP()

            if( accesslevel ){
                BEGIN_GROUP("Select Any Entity", 0)
                    SET_PREFIX("", "selectanyentity ");
                    NEW_ITEM("On", "1");
                    NEW_ITEM("Off", "0");
                END_GROUP()
            }
        END_GROUP()

        if(client->debugMenuFlags & DEBUGMENUFLAGS_PLAYERLIST && (accesslevel || OnInstancedMap()) ){
            sprintf(buffer, "plrs$$Players (^s^4%d^d, ^2ON^d^n)", player_ents[PLAYER_ENTS_ACTIVE].count);
            BEGIN_GROUP(buffer, 0)
            {
                int i;
                sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_PLAYERLIST);
                NEW_ITEM("^1DISABLE ^dPlayer Info", buffer);
                
                if(clientLinkGetDebugVar(client, "PlayerList.ShowNetStats")){
                    NEW_ITEM("^1DISABLE ^dNet Stats", "setdebugvar PlayerList.ShowNetStats 0");
                }else{
                    NEW_ITEM("^2ENABLE ^dNet Stats", "setdebugvar PlayerList.ShowNetStats 1");
                }

                for(i = 0; i < player_ents[PLAYER_ENTS_ACTIVE].count; i++){
                    Entity* e = player_ents[PLAYER_ENTS_ACTIVE].ents[i];
                    NetLink* link = e->client ? e->client->link : NULL;
                    
                    sprintf(buffer,
                            "^%s%s ^d(^s^0Lvl ^4%d^0/^4%d^d^n) (%s)",
                            e->client ? "7" : "r[NoConn]",
                            e->name,
                            e->pchar ? e->pchar->iLevel + 1 : 0,
                            e->pchar ? e->pchar->iCombatLevel + 1 : 0,
                            ENT_IS_HERO(e) ? (ENT_IS_IN_PRAETORIA(e) ? "Resistance" : "Hero") : (ENT_IS_IN_PRAETORIA(e) ? "Loyalist" : "Villain"));

                    if(e->access_level){
                        sprintf(buffer + strlen(buffer), "^r(Access %d)", e->access_level);
                    }

                    BEGIN_GROUP(buffer, 0)
                        sprintf(buffer, "gotoent %d", e->owner);
                        NEW_ITEM("Go", buffer);
                        sprintf(buffer, "who %s", e->name);
                        NEW_ITEM("Who", buffer);
                        sprintf(buffer, "teleport %s", e->name);
                        NEW_ITEM("Teleport", buffer);

                        if(link && clientLinkGetDebugVar(client, "PlayerList.ShowNetStats")){
                            sprintf(buffer,
                                    "Net (s/r): ^4%1.f ^d/ ^4%1.f ^dbps",
                                    (F32)link->sendHistory.lastByteSum / link->sendHistory.lastTimeSum,
                                    (F32)link->recvHistory.lastByteSum / link->recvHistory.lastTimeSum);
                            
                            NEW_ITEM(buffer, " ");

                            if(clientPacketLogIsEnabled(e->db_id)){
                                sprintf(buffer, "clientpacketlog 0 %d", e->db_id);
                                NEW_ITEM("^1DISABLE ^dPacket Log", buffer);
                            }else{
                                sprintf(buffer, "clientpacketlog 1 %d", e->db_id);
                                NEW_ITEM("^2ENABLE ^dPacket Log", buffer);
                            }
                        }
                    END_GROUP()
                }
            }
            END_GROUP()
        }else if ( accesslevel || OnInstancedMap() ){
            sprintf(buffer, "plrs$$Players (^s^4%d^d, ^1OFF^d^n)", player_ents[PLAYER_ENTS_ACTIVE].count);
            BEGIN_GROUP(buffer, 0)
                sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_PLAYERLIST);
                NEW_ITEM("^2ENABLE ^dPlayer Info", buffer);
            END_GROUP()
        }
        else if( client->debugMenuFlags & DEBUGMENUFLAGS_PLAYERLIST )
        {
            client->debugMenuFlags &= ~DEBUGMENUFLAGS_PLAYERLIST;
        }

        if(accesslevel){
            sprintf(buffer,
                    "time$$Time (^s^4%d^0:^4%2.2d^0:^4%2.2d^0%sm^d^n)",
                    (int)floor(server_visible_state.time) % 12 ? (int)floor(server_visible_state.time) % 12 : 12,
                    (int)floor(server_visible_state.time * 60) % 60,
                    (int)floor(server_visible_state.time * 60 * 60) % 60,
                    server_visible_state.time < 12 ? "a" : "p");
            BEGIN_GROUP(buffer, 0)
                SET_PREFIX("Timescale ", "timescale ");
                NEW_ITEM("Stopped + Time = ^5NOON", "0\ntimeset 12");
                NEW_ITEM("Normal", "0.000444");
                NEW_ITEM("Stopped", "0");

                SET_PREFIX("Time = ^5", "timeset ");
                NEW_ITEM("MIDNIGHT",    "0");
                NEW_ITEM("6am",            "6");
                NEW_ITEM("NOON",        "12");
                NEW_ITEM("6pm",            "18");
            END_GROUP()

            BEGIN_GROUP("Encounters", 0)

                NEW_ITEM("Reset",    "encounterreset all");
                NEW_ITEM("Reload",    "encounterreload");


                sprintf(buffer, "%s Stats Info",
                    entDebugInfo & (ENTDEBUGINFO_ENCOUNTERS) ? "^1DISABLE" : "^2ENABLE");
                sprintf(buffer2, entDebugInfo & (ENTDEBUGINFO_ENCOUNTERS) ? "&entdebuginfo !%d" : "|entdebuginfo %d", (ENTDEBUGINFO_ENCOUNTERS));
                NEW_ITEM(buffer, buffer2);

                sprintf(buffer, "%s Beacons",
                    entDebugInfo & (ENTDEBUGINFO_ENCOUNTERBEACONS) ? "^1DISABLE" : "^2ENABLE");
                sprintf(buffer2, entDebugInfo & (ENTDEBUGINFO_ENCOUNTERBEACONS) ? "&entdebuginfo !%d" : "|entdebuginfo %d", (ENTDEBUGINFO_ENCOUNTERBEACONS));
                NEW_ITEM(buffer, buffer2);

                NEW_ITEM(EncounterGetDebugInfo()? "^1DISABLE ^dDebug Info": "^2ENABLE ^dDebug Info",
                    EncounterGetDebugInfo()? "encounterdebug 0": "encounterdebug 1");

                NEW_ITEM(EncounterGetSpawnAlways()? "^1DISABLE ^dAlways Spawn": "^2ENABLE ^dAlways Spawn",
                    EncounterGetSpawnAlways()? "encounteralwaysspawn 0": "encounteralwaysspawn 1");

                NEW_ITEM(EncounterAllowProcessing(-1)? "^1DISABLE ^dEncounters": "^2ENABLE ^dEncounters",
                    EncounterAllowProcessing(-1)? "encounterprocessing 0": "encounterprocessing 1");

                NEW_ITEM(EncounterInMissionMode()? "Set Mode ^2CITY": "Set Mode ^1MISSION",
                    EncounterInMissionMode()? "encountermode city": "encountermode mission");

                NEW_ITEM("List by Neighborhood", "encounterneighborhoodlist");

                NEW_ITEM("Spawn Closest Encounter", "encounterspawnclosest");

                BEGIN_GROUP("Force Team Size", 0)
                    SET_PREFIX("", "encounterteamsize ");
                    NEW_ITEM("No Force", "0");
                    NEW_ITEM("3", "3");
                    NEW_ITEM("5", "5");
                    NEW_ITEM("8", "8");
                    BEGIN_GROUP("Extended Sizes", 0)
                        SET_PREFIX("", "encounterteamsize ");
                        NEW_ITEM("12", "12");
                        NEW_ITEM("16", "16");
                        NEW_ITEM("24", "24");
                    END_GROUP()
                END_GROUP()

                BEGIN_GROUP("Autostart spawns", 0)
                    NEW_ITEM("Minimum Autostart Time = 2 min", "encounterminautostart 120");
                    NEW_ITEM("Minimum Autostart Time = 30 seconds", "encounterminautostart 30");
                    NEW_ITEM("Minimum Autostart Time = default", "encounterminautostart 0");
                END_GROUP()
            END_GROUP()

            BEGIN_GROUP("Story arcs", 0)
                NEW_ITEM("Status",    "storyarcprint");
                NEW_ITEM("Succeed current task", "completetask 0");
                NEW_ITEM("Fail current task", "failtask 0");
                NEW_ITEM("Reset my info", "storyinforeset");
                NEW_ITEM((player->db_flags&DBFLAG_ALT_CONTACT)?"Make Kalinda my contact":"Make Burke my contact", "storytogglecontact");
                NEW_ITEM("Restart mission", "initmission");
                NEW_ITEM("Exit mission", "exitmission");
                NEW_ITEM("Reload Persistent NPCs", "pnpcreload");
                NEW_ITEM("Reload Storyarcs, Tasks, Contacts, Missions", "storyarcreload");
                BEGIN_GROUP("Difficulty", 0)
                    BEGIN_GROUP("Level Adjust", 0)
                        NEW_ITEM("(-1) Minus 1", "setdifficultylevel -1");
                        NEW_ITEM("( 0) Equal", "setdifficultylevel -1");
                        NEW_ITEM("(+1) Plus One", "setdifficultylevel 1");
                        NEW_ITEM("(+2) Plus One", "setdifficultylevel 2");
                        NEW_ITEM("(+3) Plus One", "setdifficultylevel 3");
                        NEW_ITEM("(+4) Plus One", "setdifficultylevel 4");
                    END_GROUP()
                    BEGIN_GROUP("Team Size", 0)
                        NEW_ITEM("1", "setdifficultyteamsize 1");
                        NEW_ITEM("2", "setdifficultyteamsize 2");
                        NEW_ITEM("3", "setdifficultyteamsize 3");
                        NEW_ITEM("4", "setdifficultyteamsize 4");
                        NEW_ITEM("5", "setdifficultyteamsize 5");
                        NEW_ITEM("6", "setdifficultyteamsize 6");
                        NEW_ITEM("7", "setdifficultyteamsize 7");
                        NEW_ITEM("8", "setdifficultyteamsize 8");
                    END_GROUP()
                    NEW_ITEM("Always Use AV", "setdifficultyav 1");
                    NEW_ITEM("Always Use EB", "setdifficultyav 0");
                    NEW_ITEM("Downgrade Boss to Lt Solo", "setdifficultyboss 0");
                    NEW_ITEM("Don't Downgrade Boss to Lt Solo", "setdifficultyboss 1");
                END_GROUP()
                BEGIN_GROUP("Debugging", 0)
                    NEW_ITEM("Detailed info",  "storyarcdetail");
                    NEW_ITEM("Show mission info", "showmissioninfo");
                    NEW_ITEM("Show mission contact", "showmissioncontact 1");
                    NEW_ITEM("List all contacts (client)", "showcontactlist");
                    NEW_ITEM("List all tasks (client)", "showtasklist");
                    NEW_ITEM("Show mission doors", "showmissiondoors");
                    NEW_ITEM("Refresh contact list", "contactgetall");
                    NEW_ITEM("Refresh task list", "taskgetall");
                    NEW_ITEM("Show client contact list", "showcontactlist");
                END_GROUP()
            END_GROUP()

            BEGIN_GROUP("Scripts", 0)
                NEW_ITEM(client->scriptDebugFlags & SCRIPTDEBUGFLAG_DEBUGON? "^1DISABLE ^dDebug Info": "^2ENABLE ^dDebug Info",
                    client->scriptDebugFlags & SCRIPTDEBUGFLAG_DEBUGON? "scriptdebug 0": "scriptdebug 1");
                NEW_ITEM("Show script locations", "scriptlocationprint");
                //NEW_ITEM("Start TestScript", "zoneeventstart TestScript");
                //NEW_ITEM("Stop TestScript", "zoneeventstop TestScript");
            END_GROUP()

            if (server_state.clickToSource)
            {
                BEGIN_GROUP("ClickToSource", 0)
                    sprintf(buffer2, "ctstoggle %i", CTS_SINGLECLICK);
                    NEW_ITEM("Toggle Single Click To Source", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_STORY);
                    NEW_ITEM("Toggle Story System Display", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_FX);
                    NEW_ITEM("Toggle Fx Display", buffer2);
                    sprintf(buffer2, entDebugInfo & (ENTDEBUGINFO_ENCOUNTERBEACONS) ? "&entdebuginfo !%d" : "|entdebuginfo %d", (ENTDEBUGINFO_ENCOUNTERBEACONS));
                    NEW_ITEM("Toggle Encounters Display", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_NPC);
                    NEW_ITEM("Toggle NPCDef Display", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_VILLAINDEF);
                    NEW_ITEM("Toggle VillainDef Display", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_BADGES);
                    NEW_ITEM("Toggle Badge Display", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_SEQUENCER);
                    NEW_ITEM("Toggle Sequencer Display", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_ENTTYPE);
                    NEW_ITEM("Toggle Ent Type Display", buffer2);
                    sprintf(buffer2, "ctstoggle %i", CTS_POWERS);
                    NEW_ITEM("Toggle Powers Display", buffer2);
                END_GROUP()
            }

            BEGIN_GROUP("Combat", 0)
                NEW_ITEM("Reset Stats",    "combatstatsreset");
                NEW_ITEM("Dump Stats to combat.log",    "combatstatsdump");
                NEW_ITEM("Reload class and power defs",    "defsreload");
            END_GROUP()

            BEGIN_GROUP("TestClients", 0)
                if(!(client->debugMenuFlags & DEBUGMENUFLAGS_TESTCLIENTS)){
                    sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_TESTCLIENTS);
                    NEW_ITEM("^2ENABLE ^dTestClient Menu", buffer);
                }else{
                    sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_TESTCLIENTS);
                    NEW_ITEM("^1DISABLE ^dTestClient Menu", buffer);

                    NEW_ITEM("Come to me",    "cometome\ncamera_shake 1\ncamera_shake_falloff 0.1");
                    BEGIN_GROUP("Modes", 1)
                        NEW_ITEM("^1DISABLE ^0Move",    "b @mode -move");
                        NEW_ITEM("^2ENABLE ^0Move",        "b @mode +move");
                        NEW_ITEM("^1DISABLE ^0Fight",    "b @mode -Fight");
                        NEW_ITEM("^2ENABLE ^0Fight",    "b @mode +Fight");
                        NEW_ITEM("^1DISABLE ^0Chat",    "b @mode -Chat");
                        NEW_ITEM("^2ENABLE ^0Chat",        "b @mode +Chat");
                        NEW_ITEM("^1DISABLE ^0Follow",    "b @mode -Follow");
                        NEW_ITEM("^2ENABLE ^0Follow",    "b @mode +Follow");
                        NEW_ITEM("^1DISABLE ^0Team invites","b @mode -teaminvite");
                        NEW_ITEM("^2ENABLE ^0Team invites",    "b @mode +teaminvite");
                        NEW_ITEM("^1DISABLE ^0Team accepts","b @mode -teamaccept");
                        NEW_ITEM("^2ENABLE ^0Team accepts",    "b @mode +teamaccept");
                        NEW_ITEM("^1DISABLE ^0Team thrashing",    "b @mode -teamthrash");
                        NEW_ITEM("^2ENABLE ^0Team thrashing",    "b @mode +teamthrash");
                        NEW_ITEM("^1DISABLE ^0Leveling Up","b @mode -levelup");
                        NEW_ITEM("^2ENABLE ^0Leveling Up",    "b @mode +levelup");
                        NEW_ITEM("^1DISABLE ^0Brawling","b @nobrawl");
                        NEW_ITEM("^2ENABLE ^0Brawling",    "b @brawl");
                        NEW_ITEM("^1DISABLE ^0Supergroup invites",    "b @mode -supergroupinvite");
                        NEW_ITEM("^2ENABLE ^0Supergroup invites",    "b @mode +supergroupinvite");
                        NEW_ITEM("^1DISABLE ^0Supergroup accepts",    "b @mode -supergroupaccept");
                        NEW_ITEM("^2ENABLE ^0Supergroup accepts",    "b @mode +supergroupaccept");
                        NEW_ITEM("^1DISABLE ^0Supergroup promote/demote/leave",    "b @mode -supergroupmisc");
                        NEW_ITEM("^2ENABLE ^0Supergroup promote/demote/leave",    "b @mode +supergroupmisc");
                        NEW_ITEM("^1DISABLE ^0Auto-resurrect",    "b @mode -autorez");
                        NEW_ITEM("^2ENABLE ^0Auto-resurrect",    "b @mode +autorez");
                        NEW_ITEM("^1DISABLE ^0Ignore List",    "b @mode +noignore");
                        NEW_ITEM("^2ENABLE ^0Ignore List",    "b @mode -noignore");
                        NEW_ITEM("^1DISABLE ^0Mission Testing",    "b @mode -missions");
                        NEW_ITEM("^2ENABLE ^0Mission Testing",    "b @mode +missions");
                        NEW_ITEM("^1DISABLE ^0Email",    "b @mode -email");
                        NEW_ITEM("^2ENABLE ^0Email",    "b @mode +email");
                        NEW_ITEM("^1DISABLE ^0League invites","b @mode -leagueinvite");
                        NEW_ITEM("^2ENABLE ^0League invites",    "b @mode +leagueinvite");
                        NEW_ITEM("^1DISABLE ^0Turnstile queueing","b @mode -turnstile");
                        NEW_ITEM("^2ENABLE ^0Turnstile queueing",    "b @mode +turnstile");
                        NEW_ITEM("^1DISABLE ^0League accepts","b @mode -leagueaccept");
                        NEW_ITEM("^2ENABLE ^0League accepts",    "b @mode +leagueaccept");
                        NEW_ITEM("^1DISABLE ^0League thrashing",    "b @mode -leaguethrash");
                        NEW_ITEM("^2ENABLE ^0League thrashing",    "b @mode +leaguethrash");
                    END_GROUP()
                    NEW_ITEM("EvilChat",    "b @evilchat\nb @mode +Chat");
                    NEW_ITEM("GoodChat",    "b @goodchat");
                    NEW_ITEM("Print Stats",    "b @printstats");
                }
            END_GROUP()
        }

        BEGIN_GROUP("Design Testing", 0)
            if(!(client->debugMenuFlags & DEBUGMENUFLAGS_INSP_ENHA))
            {
                sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_INSP_ENHA);
                NEW_ITEM("^2ENABLE ^dDesign Testing Menu", buffer);
            }
            else 
            {
                sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_INSP_ENHA);
                NEW_ITEM("^1DISABLE ^dDesign Testing Menu", buffer);
                NEW_ITEM("Auto Enhance SO", "autoenhance" );
                NEW_ITEM("Auto Enhance IO", "autoenhanceio");
                NEW_ITEM("Auto Enhance Sets", "autoenhanceset" );
                NEW_ITEM("Get Max Slots", "maxslots" );
                BEGIN_GROUP("EnhanceAndInspire", 0)
                    BEGIN_GROUP("Set Enhancement Level", 0)    
                    SET_PREFIX("Level ^4", "entcontrol me setenhancelvl ");
                    for(i = 1; i <= 53; i++){
                        sprintf(buffer, "%d",    i);
                        NEW_ITEM(buffer, buffer);
                    }
                    END_GROUP()
                    BEGIN_GROUP("Enhancements", 0)
                    {
                        sprintf(buffer, "Set Enhancement Origin (%s)", client->entity->pchar->porigin->pchName);
                        NEW_ITEM(buffer, NULL);
                        {
                            int level = client->entity->pchar->iCombatLevel + 1;
                            char descbuffer[256];
                            if (clientLinkGetDebugVar(client, "EnhanceLevel"))
                            {
                                level = (int)(intptr_t)clientLinkGetDebugVar(client, "EnhanceLevel");
                            }

                            sprintf(buffer, "boosts_setlevel %d", level );
                            sprintf(descbuffer, "Set All Boosts to Level %d", level);
                            NEW_ITEM(descbuffer, buffer);
                        }

                        {
                            int i; //, j;
                            int NumTypes = 29;
                            char *Types[29] =
                            {
                                "Damage",
                                "Range",
                                "Endurance_Discount",
                                "Accuracy",
                                "Recharge",
                                "Recovery",
                                "Heal",
                                "Stun",
                                "Sleep",
                                "Immobilize",
                                "Knockback",
                                "Confuse",
                                "Fear",
                                "Snare",
                                "Hold",
                                "Intangible",
                                "Interrupt",
                                "Drain_Endurance",
                                "ToHit_Buff",
                                "Defense_Buff",
                                "ToHit_DeBuff",
                                "Defense_DeBuff",
                                "Res_Damage",
                                "Taunt",
                                "Cone",
                                "Range",
                                "Run",
                                "Jump",
                                "Fly",
                            };
                            for (i = 0; i < NumTypes; i++)
                            {
                                BEGIN_GROUP(Types[i], 0)
                                {
                                    int level = client->entity->pchar->iCombatLevel + 1;
                                    if (clientLinkGetDebugVar(client, "EnhanceLevel"))
                                    {
                                        level = (int)(intptr_t)clientLinkGetDebugVar(client, "EnhanceLevel");
                                    }

                                    sprintf(buffer, "boost generic_%s generic_%s %d", Types[i], Types[i], level );
                                    NEW_ITEM("Training", buffer);


                                    // brute force and ULGY!
                                    if (!stricmp(client->entity->pchar->porigin->pchName, "science") || !stricmp(client->entity->pchar->porigin->pchName, "technology"))
                                    {
                                        sprintf(buffer, "boost science_technology_%s science_technology_%s %d", Types[i], Types[i],level );
                                    } 
                                    else if (!stricmp(client->entity->pchar->porigin->pchName, "magic") || !stricmp(client->entity->pchar->porigin->pchName, "natural")) 
                                    {
                                        sprintf(buffer, "boost natural_magic_%s natural_magic_%s %d", Types[i], Types[i],level );
                                    }
                                    else if (!stricmp(client->entity->pchar->porigin->pchName, "mutation"))
                                    {
                                        sprintf(buffer, "boost mutation_science_%s mutation_science_%s %d", Types[i], Types[i],level );
                                    }
                                    NEW_ITEM("DO", buffer);

                                    sprintf(buffer, "boost %s_%s %s_%s %d", client->entity->pchar->porigin->pchName, Types[i],
                                        client->entity->pchar->porigin->pchName, Types[i], level );
                                    NEW_ITEM("SO", buffer);

                                    if( stricmp( Types[i], "Drain_Endurance")!=0 || stricmp(Types[i], "Cone")!=0 )
                                    { 
                                        sprintf(buffer, "boost crafted_%s crafted_%s %d", Types[i], Types[i], level );
                                        NEW_ITEM("IO", buffer);
                                    }

                                }
                                END_GROUP()
                            }
                        }
                    }
                    END_GROUP()

                    BEGIN_GROUP("Hamidons", 0)     
                    {                            
                        int NumTypes = 11;
                        char *Types[11] =
                        {
                            "Hamidon_Accuracy_Mez",
                            "Hamidon_Buff_Endurance_Discount",
                            "Hamidon_Buff_Recharge",
                            "Hamidon_Damage_Accuracy",
                            "Hamidon_Damage_Mez",
                            "Hamidon_Damage_Range",
                            "Hamidon_Debuff_Accuracy",
                            "Hamidon_Debuff_Endurance_Discount",
                            "Hamidon_Heal_Endurance_Discount",
                            "Hamidon_Res_Damage_Endurance_Discount",
                            "Hamidon_Travel_Endurance_Discount"
                        };

                        int level = client->entity->pchar->iCombatLevel + 1;
                        if (clientLinkGetDebugVar(client, "EnhanceLevel"))
                        {
                            level = (int)(intptr_t)clientLinkGetDebugVar(client, "EnhanceLevel");
                        }

                        for (i = 0; i < NumTypes; i++) 
                        {
                            sprintf(buffer, "boost %s %s %d", Types[i], Types[i], level );
                            NEW_ITEM(Types[i], buffer);
                        }
                    }
                    END_GROUP()

                    BEGIN_GROUP("Invention", 0)    
                    {
                        int level = client->entity->pchar->iCombatLevel + 1;
                        const BoostSet **ppBoostSets = g_BoostSetDict.ppBoostSets;
                        BoostSet **ppSortedSets = NULL;

                        if (clientLinkGetDebugVar(client, "EnhanceLevel"))
                        {
                            level = (int)(intptr_t)clientLinkGetDebugVar(client, "EnhanceLevel");
                        }

                        if(!(client->debugMenuFlags & DEBUGMENUFLAGS_IOSORT))
                        {
                            sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_IOSORT);
                            NEW_ITEM("^dSort Alphabetically", buffer);
                        }
                        else
                        {
                            sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_IOSORT);
                            NEW_ITEM("^dSort by Set", buffer);
                        }


                        eaCopy(&ppSortedSets, &ppBoostSets);
                        stableSort(ppSortedSets, eaSize(&ppSortedSets), sizeof(BoostSet *), NULL, (client->debugMenuFlags&DEBUGMENUFLAGS_IOSORT)?boostset_CompareName:boostset_Compare );
                        NEW_ITEM("Get Invention Salvage", "rw_invention_all");

                        if(!(client->debugMenuFlags & DEBUGMENUFLAGS_IOSORT))
                        {
                            BEGIN_GROUP("Boost Sets", 0)    
                            {

                                for (i = 0; i < eaSize(&ppSortedSets); i++)
                                {
                                    if (i == 0)
                                    {
                                        if (ppSortedSets[i]->pchGroupName != NULL)
                                            sprintf(buffer, "%s", textStd(ppSortedSets[i]->pchGroupName) );
                                        else
                                            sprintf(buffer, "%s", "Uncategorized" );
                                        BEGIN_GROUP(buffer, 0)    
                                    } 
                                    else 
                                    {
                                        if (ppSortedSets[i]->pchGroupName != NULL && 
                                            ppSortedSets[i-1]->pchGroupName != NULL && 
                                            stricmp(ppSortedSets[i]->pchGroupName, ppSortedSets[i-1]->pchGroupName) != 0)
                                        {
                                            END_GROUP()
                                            sprintf(buffer, "%s", textStd(ppSortedSets[i]->pchGroupName) );
                                            BEGIN_GROUP(buffer, 0)    
                                        } 
                                        else if (ppSortedSets[i]->pchGroupName == NULL && 
                                                    ppSortedSets[i-1]->pchGroupName != NULL)
                                        {
                                            END_GROUP()
                                            sprintf(buffer, "%s", "Uncategorized" );
                                            BEGIN_GROUP(buffer, 0)    
                                        }
                                    }
                                    sprintf(buffer, "%s", textStd(ppSortedSets[i]->pchDisplayName) );
                                    BEGIN_GROUP(buffer, 0)    
                                    {
                                        int j;

                                        // get whole set
                                        sprintf(buffer, "boostset %s %d", ppSortedSets[i]->pchName, level );
                                        NEW_ITEM("Get Set", buffer);

                                        for (j = 0; j < eaSize(&ppSortedSets[i]->ppBoostLists); j++)
                                        {
                                            int k;
                                            for(k = 0; k < eaSize(&ppSortedSets[i]->ppBoostLists[j]->ppBoosts); k++)
                                            {
                                                const BasePower *pBase = ppSortedSets[i]->ppBoostLists[j]->ppBoosts[k];
                                                BEGIN_GROUP(pBase->pchName, 0)    
                                                {
                                                    const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel(pBase->pchFullName, level);
                                                    sprintf(buffer, "boost %s %s %d", pBase->pchName, pBase->pchName, level );
                                                    NEW_ITEM("Get Enhancement", buffer); 
                                                    if (pRecipe)
                                                    {
                                                        sprintf(buffer, "rw_recipe %s", pRecipe->pchName);
                                                        NEW_ITEM("Get Recipe", buffer); 
                                                    }
                                                }
                                                END_GROUP() // enhancement in boostset
                                            }
                                        }
                                    }
                                    END_GROUP() // boostset
                                }
                                END_GROUP() // matches i == 0 case
                            }
                            END_GROUP()
                        }
                        else
                        {
                            for (i = 0; i < eaSize(&ppSortedSets); i++)
                            {
                                sprintf(buffer, "%s", textStd(ppSortedSets[i]->pchDisplayName) );
                                BEGIN_GROUP(buffer, 0)    
                                {
                                    int j;

                                    // get whole set
                                    sprintf(buffer, "boostset %s %d", ppSortedSets[i]->pchName, level );
                                    NEW_ITEM("Get Set", buffer);
                                    for (j = 0; j < eaSize(&ppSortedSets[i]->ppBoostLists); j++)
                                    {
                                        int k;
                                        for(k = 0; k < eaSize(&ppSortedSets[i]->ppBoostLists[j]->ppBoosts); k++)
                                        {
                                            const BasePower *pBase = ppSortedSets[i]->ppBoostLists[j]->ppBoosts[k];
                                            BEGIN_GROUP(pBase->pchName, 0)    
                                            {
                                                const DetailRecipe *pRecipe = detailrecipedict_RecipeFromEnhancementAndLevel(pBase->pchFullName, level);
                                                sprintf(buffer, "boost %s %s %d", pBase->pchName, pBase->pchName, level );
                                                NEW_ITEM("Get Enhancement", buffer); 
                                                if (pRecipe)
                                                {
                                                    sprintf(buffer, "rw_recipe %s", pRecipe->pchName);
                                                    NEW_ITEM("Get Recipe", buffer); 
                                                }
                                            }
                                            END_GROUP() // enhancement in boostset
                                        }
                                    }
                                }
                                END_GROUP() // boostse
                            }
                        }
                    }
                    END_GROUP()
                    BEGIN_GROUP("Inspiration", 0)
                    {
                        const PowerCategory *pcat = powerdict_GetCategoryByName(&g_PowerDictionary, "Inspirations");
                        if(pcat!=NULL)
                        {
                            int i;

                            for (i = 0; i < eaSize(&pcat->ppPowerSets); i++) {
                                const BasePowerSet *pset = pcat->ppPowerSets[i];

                                if(pset!=NULL)
                                {
                                    int j;

                                    for (j = 0; j < eaSize(&pset->ppPowers); j++) {
                                        const BasePower *ppow = pset->ppPowers[j];
                                        if(ppow!=NULL)
                                        {
                                            sprintf(buffer, "inspire %s %s", pset->pchName, ppow->pchName );
                                            NEW_ITEM(ppow->pchName, buffer);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    END_GROUP() // inspirations
                END_GROUP() // enhance and inspire

                BEGIN_GROUP("Incarnate", 0)
                {
                    IncarnateSlot slotIndex;
                    char descbuffer[256];

                    for (slotIndex = 0; slotIndex < kIncarnateSlot_Count; slotIndex++)
                    {
                        char slotname[256];

                        sprintf(slotname, "%s", StaticDefineIntRevLookup(ParseIncarnateSlotNames,slotIndex));

                        BEGIN_GROUP(slotname, 0)    
                        
                        sprintf(descbuffer, "Unlock ^4%s ^0Slot", slotname);
                        sprintf(buffer, "badgegrant %s", incarnateEnableBadgeNames[slotIndex]);
                        NEW_ITEM(descbuffer, buffer);

                        sprintf(descbuffer, "Grant All ^4%s ^0Powers", slotname);
                        sprintf(buffer, "incarnate_grant_all_by_slot %s", slotname);
                        NEW_ITEM(descbuffer, buffer);

                        sprintf(descbuffer, "Revoke All ^4%s ^0Powers", slotname);
                        sprintf(buffer, "incarnate_revoke_all_by_slot %s", slotname);
                        NEW_ITEM(descbuffer, buffer);

                        sprintf(descbuffer, "Unequip Current ^4%s ^0Power", slotname);
                        sprintf(buffer, "incarnate_force_unequip_by_slot %s", slotname);
                        NEW_ITEM(descbuffer, buffer);

                        {
                            const BasePowerSet *incarnateBaseSet = IncarnateSlot_getBasePowerSet(slotIndex);
                            int powerIndex;
                            char prefix[256] = "";
                            char oldPrefix[256] = "";

                            for (powerIndex = 0; powerIndex < eaSize(&incarnateBaseSet->ppPowers); powerIndex++)
                            {    
                                strcpy(prefix, incarnateBaseSet->ppPowers[powerIndex]->pchName);
                                strtok(prefix, "_");
                                if (strnicmp(prefix, oldPrefix, strlen(prefix)) != 0)
                                {
                                    if(powerIndex > 0)
                                    {
                                        END_GROUP()
                                    }
                                    BEGIN_GROUP(prefix, 0)
                                    strcpy(oldPrefix, prefix);
                                }
                                
                                sprintf(descbuffer, "Buy ^1%s",    incarnateBaseSet->ppPowers[powerIndex]->pchName);
                                sprintf(buffer, "incarnate_grant %s %s", slotname, incarnateBaseSet->ppPowers[powerIndex]->pchName);
                                NEW_ITEM(descbuffer, buffer);

                                sprintf(descbuffer, "--Force Equip ^1%s",    incarnateBaseSet->ppPowers[powerIndex]->pchName);
                                sprintf(buffer, "incarnate_force_equip %s %s", slotname, incarnateBaseSet->ppPowers[powerIndex]->pchName);
                                NEW_ITEM(descbuffer, buffer);
                            }
                            END_GROUP()
                        }
                        END_GROUP()
                    }

                    BEGIN_GROUP("Components",0)

                    NEW_ITEM("Grant 50 Incarnate Shards", "salvagegrant S_IncarnateShard 50");
                    NEW_ITEM("Grant 50 Incarnate Threads", "salvagegrant S_IncarnateThread 50");
                    NEW_ITEM("Grant 50 Astral Merits", "salvagegrant S_EndgameMerit01 50");
                    NEW_ITEM("Grant 50 Empyrean Merits", "salvagegrant S_EndgameMerit02 50");

                    END_GROUP()

                    NEW_ITEM("Grant 50K Psychic Incarnate XP", "incarnateslot_reward_xp A_Points 50000");
                    NEW_ITEM("Grant 50K Physical Incarnate XP", "incarnateslot_reward_xp B_Points 50000");
                    NEW_ITEM("Grant All Incarnate Slots", "incarnateslot_activate_all");
                    NEW_ITEM("Grant All Incarnate Powers", "incarnate_grant_all");
                    NEW_ITEM("Remove All Incarnate Powers", "incarnate_revoke_all");
                }
                END_GROUP() // incarnate

                BEGIN_GROUP("Character Change", 0)    // Character switching
                    // Select character level
                    BEGIN_GROUP("Set Level", 0)    
                        SET_PREFIX("Level ^4", "entcontrol me setplayermorphlvl ");
                        for(i = 1; i <= 50; i++){
                            sprintf(buffer, "%d",    i);
                            NEW_ITEM(buffer, buffer);
                        }
                    END_GROUP()
                    // Select character AT/Primary/Secondary powersets
                    BEGIN_GROUP("Select AT/Primary/Secondary", 0)
                        // Find all of the ATs
                        for(i=eaSize(&g_CharacterClasses.ppClasses)-1; i>=0; i--)
                        {
                            int j;
                            sprintf(buffer, "%s",    g_CharacterClasses.ppClasses[i]->pchName);
                            BEGIN_GROUP(buffer, 0);
                                // Loop through all of the primary powersets
                                for(j=eaSize(&g_CharacterClasses.ppClasses[i]->pcat[kCategory_Primary]->ppPowerSets)-1; j>=0; j--)
                                {
                                    int k;
                                    const BasePowerSet *pPowerSetPri = g_CharacterClasses.ppClasses[i]->pcat[kCategory_Primary]->ppPowerSets[j];
                                    sprintf(buffer, "%s",    pPowerSetPri->pchName);
                                    BEGIN_GROUP(buffer, 0);
                                        // Loop through all of the secondary powersets
                                        for(k=eaSize(&g_CharacterClasses.ppClasses[i]->pcat[kCategory_Secondary]->ppPowerSets)-1; k>=0; k--)
                                        {
                                            char tempBuffer[100];
                                            const BasePowerSet *pPowerSetSec = g_CharacterClasses.ppClasses[i]->pcat[kCategory_Secondary]->ppPowerSets[k];
                                            strcpy(tempBuffer, &(g_CharacterClasses.ppClasses[i]->pchName[6]));
                                            sprintf(buffer, "%s",    pPowerSetSec->pchName);
                                            sprintf(buffer2, "playermorph %s %s %s 0",    tempBuffer, 
                                                pPowerSetPri->pchName,
                                                pPowerSetSec->pchName);
                                            NEW_ITEM(buffer, buffer2); 
                                        }
                                    END_GROUP()
                                }
                            END_GROUP()
                        }
                    END_GROUP()

                    // Save current character
                    NEW_ITEM("Save Character To Common Folder", "packageent");

                    if(dirExists("n:/nobackup/resource/characters/common/"))
                    {

                        // Load existing common characters
                        BEGIN_GROUP("Common Saved Characters",0)
                        {
                            int count,i;
                            char **dirs;
                            dirs = fileScanDirFolders("n:/nobackup/resource/characters/common/", &count, FSF_FOLDERS | FSF_NOHIDDEN);
                            for(i=0; i<count; i++)
                            {
                                char *group = strrchr(dirs[i],'/');
                                BEGIN_GROUP(group+1,0)
                                {
                                    char **files;
                                    int charcount,j;
                                    files = fileScanDirFolders(dirs[i], &charcount, FSF_FILES | FSF_NOHIDDEN);
                                    for(j=0; j<charcount; j++)
                                    {
                                        char *s = files[j];
                                        int len = (int)strlen(s);
                                        if(strcmp(&s[len-4],".txt"))
                                            continue;
                                        sprintf(buffer, "playermorph common %s null 0",files[j]);
                                        s = strrchr(s,'/')+1;
                                        len = (int)strlen(s);
                                        s[len-4] = '\0';
                                        NEW_ITEM(s, buffer);
                                        free(files[j]);
                                    }
                                    SAFE_FREE(files);
                                }
                                END_GROUP()
                                free(dirs[i]);
                            }
                            SAFE_FREE(dirs);
                        }
                        END_GROUP()
                    }

                END_GROUP()
            }
        END_GROUP()

        BEGIN_GROUP("Power", 0)
        if(!(client->debugMenuFlags & DEBUGMENUFLAGS_POWERS))
        {
                sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_POWERS);
                NEW_ITEM("^2ENABLE ^dPower Testing Menu", buffer);
        }
        else 
        {
            int j, k;

            static const PowerCategory ** ppSortedCats = 0;

            sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_POWERS);
            NEW_ITEM("^1DISABLE ^dPower Testing Menu", buffer);
            NEW_ITEM("Grant All Powers", "omnipotent");
            NEW_ITEM("Hide Power Target Info", "showPowerTargetInfo 0 0 0");

            if(!ppSortedCats)
            {
                eaCopyConst(&ppSortedCats, &g_PowerDictionary.ppPowerCategories);
                eaQSortConst(ppSortedCats, comparePowerCats);
            }

            for(i=0; i < eaSize(&ppSortedCats); i++)
            {
                const BasePowerSet ** ppSortedSets = 0;
                 eaCopyConst(&ppSortedSets, &ppSortedCats[i]->ppPowerSets);
                eaQSortConst(ppSortedSets, comparePowerSets);

                if( stricmp(ppSortedCats[i]->pchName, "Boosts")==0 )
                    continue; // Boosts as powers makes no sense

                sprintf(buffer, "%s", ppSortedCats[i]->pchName);
                BEGIN_GROUP(buffer, 0)
                for(j=0; j < eaSize(&ppSortedSets); j++ )
                {
                    const BasePowerSet *pSet = ppSortedSets[j];
                
                    sprintf(buffer, "%s", pSet->pchName);
                    BEGIN_GROUP(buffer, 0)
                    for(k=0; k<eaSize(&pSet->ppPowers); k++)
                    {    
                        sprintf(buffer, "Grant Power ^1%s",    pSet->ppPowers[k]->pchName);
                        sprintf(buffer2, "powers_buypower_dev %s %s %s", ppSortedCats[i]->pchName, pSet->pchName, pSet->ppPowers[k]->pchName);
                        NEW_ITEM(buffer, buffer2); 

                        sprintf(buffer, "--Show Targeting Info");
                        sprintf(buffer2, "showPowerTargetInfo %s %s %s", ppSortedCats[i]->pchName, pSet->pchName, pSet->ppPowers[k]->pchName);
                        NEW_ITEM(buffer, buffer2); 
                    }
                    END_GROUP()
                }
                END_GROUP()
            }
        }
        END_GROUP()
            
        if(accesslevel)
        {
            BEGIN_GROUP("Spawn", 0)
                if (server_state.levelEditor) {
                    NEW_ITEM("Spawns not loaded (running w/ -levelEditor)", "nop");
                } else {
                    if(!(client->debugMenuFlags & DEBUGMENUFLAGS_VILLAIN_SPAWNLIST)){
                        sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_VILLAIN_SPAWNLIST);
                        NEW_ITEM("^2ENABLE ^dVillain Spawn List", buffer);
                    }else{
                        sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_VILLAIN_SPAWNLIST);
                        NEW_ITEM("^1DISABLE ^dVillain Spawn List", buffer);

                        BEGIN_GROUP("Spawn Villain", 1)
                        {
                            VillainGroupEnum lastGroup;
                            int i;
                            int defaultSpawnLevel;

                            if(client->defaultSpawnLevel < 1){
                                defaultSpawnLevel = client->entity->pchar->iCombatLevel + 1;
                            }else{
                                defaultSpawnLevel = client->defaultSpawnLevel;
                            }

                            sprintf(buffer, "spawnlevel$$Set Spawn Level (^0^sCur = ^4%d^d^n)", defaultSpawnLevel);

                            BEGIN_GROUP(buffer, 0)
                            {
                                SET_PREFIX("Level ^4", "ec me setspawnlevel ");

                                for(i = 1; i <= 54; i++){
                                    sprintf(buffer, "%d%s%s%s",
                                            i,
                                            i == client->entity->pchar->iLevel + 1 ? " ^d(^2^sMy Security Lvl^d^n)" : "",
                                            i == client->entity->pchar->iCombatLevel + 1 ? " ^d(^2^sMy Combat Lvl^d^n)" : "",
                                            i == defaultSpawnLevel ? "^r <==(Cur)" : "");
                                    sprintf(buffer2, "%d", i);
                                    NEW_ITEM(buffer, buffer2);
                                }
                            }
                            END_GROUP()

                            for(i = 0; i < eaSize(&villainDefList.villainDefs); i++){
                                int j;
                                int minLevel = INT_MAX;
                                int maxLevel = INT_MIN;
                                int matchesDefault = 0;

                                if(!i || lastGroup != villainDefList.villainDefs[i]->group){
                                    if(i){
                                        END_GROUP()
                                    }

                                    BEGIN_GROUP(villainGroupGetName(villainDefList.villainDefs[i]->group), 0)

                                    if(client->debugMenuFlags & DEBUGMENUFLAGS_VILLAIN_COSTUME){
                                        SET_PREFIX("^6", "mmm$$editvillaincostume ");
                                    }else{
                                        SET_PREFIX("^6", "spawnvillain \"");
                                    }
                                }

                                for(j = 0; j < eaSize(&villainDefList.villainDefs[i]->levels); j++){
                                    int level = villainDefList.villainDefs[i]->levels[j]->level;

                                    if(level < minLevel)
                                        minLevel = level;
                                    if(level > maxLevel)
                                        maxLevel = level;
                                    if(level == defaultSpawnLevel)
                                        matchesDefault = 1;
                                }

                                sprintf(buffer,
                                    "%s%s ^d(^4%d^d-^4%d^d)",
                                    matchesDefault ? "^2" : "",
                                    villainDefList.villainDefs[i]->name,
                                    minLevel,
                                    maxLevel);

                                if(client->debugMenuFlags & DEBUGMENUFLAGS_VILLAIN_COSTUME){
                                    sprintf(buffer2, "%i %s", defaultSpawnLevel,  villainDefList.villainDefs[i]->name);
                                }else{
                                    sprintf(buffer2, "%s\"", villainDefList.villainDefs[i]->name);
                                }
                                

                                NEW_ITEM(buffer, buffer2);

                                lastGroup = villainDefList.villainDefs[i]->group;
                            }

                            if(i){
                                END_GROUP()
                            }
                        }
                        END_GROUP()
                    }

                    if(!(client->debugMenuFlags & DEBUGMENUFLAGS_COSTUME_SPAWNLIST)){
                        sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_COSTUME_SPAWNLIST);
                        NEW_ITEM("^2ENABLE ^dCostume Spawn List", buffer);
                    }else{
                        sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_COSTUME_SPAWNLIST);
                        NEW_ITEM("^1DISABLE ^dCostume Spawn List", buffer);

                        BEGIN_GROUP("Spawn Costume", 1)
                        {
                            char lastGroup[100] = "";
                            char* spawnCommand;
                            int spawnType = client->spawnCostumeType;
                            int i;

                            switch(spawnType){
                                case 0:
                                    spawnCommand = "spawnnpc";
                                    break;
                                case 1:
                                    spawnCommand = "spawnobjective";
                                    break;
                                case 2:
                                    spawnCommand = "spawnvillain";
                                    break;
                                default:
                                    spawnCommand = "spawnnpc";
                                    break;
                            }

                            sprintf(buffer, "setspawntype$$Set Type (^0Cur = ^1%s^d)", spawnCommand);

                            BEGIN_GROUP(buffer, 0)
                                SET_PREFIX("", "ec me setspawncostumetype ");
                                NEW_ITEM("spawnnpc", "0");
                                NEW_ITEM("spawnobjective", "1");
                                NEW_ITEM("spawnDummyVillain", "2");
                            END_GROUP()

                            for(i = 0; i < eaSize(&npcDefList.npcDefs); i++){
                                char name[100];
                                char group[100];
                                char* s;

                                strcpy(name, npcDefList.npcDefs[i]->name);

                                s = strchr(name, '_');

                                if(s){
                                    *s = 0;
                                    strcpy(group, name);
                                }else{
                                    group[0] = 0;
                                }

                                if(!i || stricmp(group, lastGroup)){
                                    if(i){
                                        END_GROUP()
                                    }

                                    BEGIN_GROUP(group[0] ? group : name, 0)
                                    strcpy(lastGroup, group);

                                    if(client->debugMenuFlags & DEBUGMENUFLAGS_VILLAIN_COSTUME){
                                        SET_PREFIX("^6", "mmm$$editnpccostume ");
                                    }else{
                                        sprintf(buffer, "%s ", spawnCommand);
                                        SET_PREFIX("^6", buffer);
                                    }
                                }

                                NEW_ITEM(npcDefList.npcDefs[i]->name, npcDefList.npcDefs[i]->name);
                            }

                            if(i){
                                END_GROUP()
                            }
                        }
                        END_GROUP()
                    }

                    if(!(client->debugMenuFlags & DEBUGMENUFLAGS_VILLAIN_COSTUME)){
                        sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_VILLAIN_COSTUME);
                        NEW_ITEM("^2ENABLE ^dEdit Costumes", buffer);
                    }else{
                        sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_VILLAIN_COSTUME);
                        NEW_ITEM("^1DISABLE ^dEdit Costumes", buffer);
                    }
                }

            END_GROUP() // Spawn

            if(client->debugMenuFlags & DEBUGMENUFLAGS_PATHLOG){
                BEGIN_GROUP("Path Log", 0)
                {
                    int count = aiPathLogGetSetCount();
                    S64 last = 0;

                    sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_PATHLOG);
                    NEW_ITEM("^1DISABLE ^dPath Log", buffer);

                    for(i = 0; i < count; i++){
                        AIPathLogEntrySet* set = aiPathLogSets[i];

                        if(set){
                            int j;
                            int size = ARRAY_SIZE(set->entries);
                            AIPathLogEntry* lastEntry = set->entries + set->cur;
                            AIPathLogEntry* prev = NULL;
                            S64 totalTimeDiffs = 0;
                            int timeDiffsCount = 0;
                            F32 averageTimeDiff = 0;

                            for(j = 0; j < size; j++){
                                int index = (set->cur - j + size) % size;
                                AIPathLogEntry* entry = set->entries + index;

                                if(entry->entityRef){
                                    if(prev){
                                        totalTimeDiffs += prev->abs_time - entry->abs_time;
                                        timeDiffsCount++;
                                    }

                                    prev = entry;
                                }
                            }

                            if(timeDiffsCount){
                                averageTimeDiff = ABS_TO_SEC((F32)totalTimeDiffs / timeDiffsCount);
                            }

                            if(i < count - 1){
                                sprintf(buffer,
                                        "%I64u$$(^s^%d%1.1f^0s^d, ^0avg ^4%1.2f^0s^d^n) ^4%s^dM - ^4%s^dM",
                                        last,
                                        ABS_TIME_SINCE(lastEntry->abs_time) < SEC_TO_ABS(10) ? 2 : 1,
                                        ABS_TO_SEC((F32)ABS_TIME_SINCE(lastEntry->abs_time)),
                                        averageTimeDiff,
                                        getCommaSeparatedInt(last/1000000),
                                        getCommaSeparatedInt(aiPathLogCycleRanges[i]/1000000));
                            }else{
                                sprintf(buffer,
                                        "%I64u$$(^s^%d%1.1f^0s^d, ^0avg ^4%1.2f^0s^d^n) ^4%s^dM and higher",
                                        last,
                                        ABS_TIME_SINCE(lastEntry->abs_time) < SEC_TO_ABS(10) ? 2 : 1,
                                        ABS_TO_SEC((F32)ABS_TIME_SINCE(lastEntry->abs_time)),
                                        averageTimeDiff,
                                        getCommaSeparatedInt(last/1000000));
                            }

                            BEGIN_GROUP(buffer, 0)
                                for(j = 0; j < size; j++){
                                    int index = (set->cur - j + size) % size;
                                    AIPathLogEntry* entry = set->entries + index;
                                    F32 ratio = (F32)(entry->cpuCycles - last) / (F32)(aiPathLogCycleRanges[i] - last);

                                    if(entry->entityRef){
                                        Entity* ent = erGetEnt(entry->entityRef);

                                        sprintf(buffer,
                                                "%d$$^4^b%d/25b ^s^4%1.1f^0s^t45t\t%s^%d%s ^4%d",
                                                index,
                                                (int)(ratio * 25),
                                                ABS_TO_SEC((F32)ABS_TIME_SINCE(entry->abs_time)),
                                                ent ? "" : "^1(X) ",
                                                entry->success ? 2 : 1,
                                                getEntTypeName(entry->entType),
                                                ent ? ent->owner : 0);

                                        sprintf(buffer2,
                                                "Cycles: ^4%s\n"
                                                "Jump: ^4%1.1f\n"
                                                "CanFly: %s\n"
                                                "Result: %s\n"
                                                ,
                                                getCommaSeparatedInt(entry->cpuCycles),
                                                entry->jumpHeight,
                                                entry->canFly ? "^2YES" : "^1NO",
                                                getNavSearchResultText(entry->searchResult));

                                        BEGIN_GROUP_ROLLOVER(buffer, 0, buffer2)
                                            if(ent){
                                                sprintf(buffer, "gotoent %d", ent->owner);
                                                NEW_ITEM("ent", buffer);
                                            }

                                            sprintf(buffer, "setpos %1.f %1.f %1.f", vecParamsXYZ(entry->sourcePos));
                                            NEW_ITEM("source", buffer);

                                            sprintf(buffer, "setpos %1.f %1.f %1.f", vecParamsXYZ(entry->targetPos));
                                            NEW_ITEM("target", buffer);
                                        END_GROUP()
                                    }
                                }
                            END_GROUP()
                        }

                        last = aiPathLogCycleRanges[i];
                    }
                }
                END_GROUP()
            }

            if(client->debugMenuFlags & DEBUGMENUFLAGS_PLAYER_NET_STATS){
                int clientSubPacketLogEntryAllocatedCount();
                
                sprintf(buffer, "plns$$Player Net Stats (^s^4%d ^dentries^n)", clientSubPacketLogEntryAllocatedCount());

                BEGIN_GROUP(buffer, 0)
                {
                    void clientPacketLogGetAllDBIDS(int** dbIDs);
                    
                    static int* dbIDs;
                    
                    int i;
                    
                    sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_PLAYER_NET_STATS);
                    NEW_ITEM("^1DISABLE ^dNet Stats Menu", buffer);
                    
                    if(clientPacketLogIsEnabled(0)){
                        NEW_ITEM("^1DISABLE ^0All Packet Logs", "clientpacketlog 0 -1");
                        NEW_ITEM("^1CLEAR ^0All Packet Logs", "clientpacketlog -1 -1");
                    }

                    NEW_ITEM(    clientPacketLogIsPaused() ? "^2UNPAUSE ^dPacket Log" : "^1PAUSE ^dPacket Log",
                                clientPacketLogIsPaused() ? "clientpacketlogpause 0" : "clientpacketlogpause 1");
                                
                    clientPacketLogGetAllDBIDS(&dbIDs);
            
                    if(e){
                        // Add the selected entity if it's not in the list.
                        
                        for(i = 0; i < eaiSize(&dbIDs); i++){
                            if(dbIDs[i] == e->db_id){
                                break;
                            }
                        }
                        
                        if(i == eaiSize(&dbIDs)){
                            eaiPush(&dbIDs, e->db_id);
                        }
                    }
                                                            
                    for(i = 0; i < eaiSize(&dbIDs); i++){
                        int db_id = dbIDs[i];
                        Entity* logEnt = entFromDbId(db_id);
                        ClientPacketLog* log = clientGetPacketLog(NULL, db_id, 0);
                        
                        if(logEnt){
                            sprintf(buffer,
                                    "ent%d$$%s%s ^d(ID: ^4%d^d, dbID: ^4%d^d)",
                                    db_id,
                                    logEnt == e ? "^rSelected^d: " : "",
                                    AI_LOG_ENT_NAME(logEnt),
                                    entGetId(logEnt),
                                    db_id);
                        }else{
                            sprintf(buffer,
                                    "ent%d$$%s ^d(dbID: ^4%d^d)",
                                    db_id,
                                    log ? log->name : "[No Name]",
                                    db_id);
                        }
                        BEGIN_GROUP(buffer, logEnt == e)
                        
                        if(log){
                            int showAllPackets;
                            
                            sprintf(buffer, "clientpacketlog 0 %d", db_id);
                            NEW_ITEM("^1DISABLE ^0Client's Packet Log", buffer);

                            sprintf(buffer, "clientpacketlog -1 %d", db_id);
                            NEW_ITEM("^1CLEAR ^0Client's Packet Log", buffer);

                            // Enable showing all packets for just this db_id.
                            
                            if(log){
                                {
                                    ClientSubPacketLogEntry* entry;
                                    int curFrame = -1;
                                    int min_bits = INT_MAX;
                                    int max_bits = -1;
                                    int newConnection = 0;
                                    
                                    for(entry = log->subPackets[CLIENT_PAKLOG_OUT]; entry; entry = entry->next){
                                        if(entry->parent_pak_bit_count){
                                            min_bits = min(min_bits, (int)entry->parent_pak_bit_count);
                                            max_bits = max(max_bits, (int)entry->parent_pak_bit_count);
                                        }
                                    }                                    
                                    
                                    sprintf(buffer,
                                            "out$$Outbound (^4%d^d'^4%d^dB - ^4%d^d'^4%d^dB^n)",
                                            min_bits / 8,
                                            min_bits % 8,
                                            max_bits / 8,
                                            max_bits % 8);
                                    BEGIN_GROUP(buffer, 0)

                                    sprintf(buffer2, "PacketLog.ShowAllPacketsOut.%d", db_id);
                                    showAllPackets = (int)(intptr_t)clientLinkGetDebugVar(client, buffer2);
                                    sprintf(buffer, "setdebugvar %s %d", buffer2, !showAllPackets);
                                    NEW_ITEM(showAllPackets ?
                                                "^1DISABLE ^dShowing All Packets" :
                                                "^2ENABLE ^dShowing All Packets",
                                            buffer);
                                    
                                    for(entry = log->subPackets[CLIENT_PAKLOG_OUT]; entry; entry = entry->next){
                                        if(!showAllPackets && entry->cmd == ~0){
                                            continue;
                                        }
                                        
                                        if(curFrame == -1 || entry->send_server_frame != curFrame){
                                            ClientSubPacketLogEntry* cur = entry;
                                            int bitCount = 0;
                                            
                                            if(curFrame != -1){
                                                END_GROUP()
                                            }
                                            
                                            // Count all the sub-packets from this same packet.
                                            
                                            for(; cur; cur = cur->next){
                                                if(    cur->send_server_frame != entry->send_server_frame ||
                                                    cur->parent_pak_bit_count != entry->parent_pak_bit_count)
                                                {
                                                    break;
                                                }
                                                
                                                bitCount += cur->bitCount;
                                            }
                                            
                                            if(newConnection){
                                                NEW_ITEM("^5New Connection", " ");
                                                newConnection = 0;
                                            }
                                            
                                            if(cur && entry->client_connect_time != cur->client_connect_time){
                                                newConnection = 1;
                                            }

                                            sprintf(buffer,
                                                    "^2^b%1.f/25b %s^%dF^4%d ^d(^4%d^d'^4%d^dB, ^4%d^d'^4%d^dB)",
                                                    max_bits >= min_bits && entry->parent_pak_bit_count ? 25 * (F32)(entry->parent_pak_bit_count - min_bits) / (max_bits - min_bits) : 0,
                                                    entry->cmd == ~0 ? "^s" : "",
                                                    entry->cmd == ~0 ? 1 : 2,
                                                    entry->send_server_frame,
                                                    entry->parent_pak_bit_count / 8,
                                                    entry->parent_pak_bit_count % 8,
                                                    bitCount / 8,
                                                    bitCount % 8);
                                            BEGIN_GROUP(buffer, 0)
                                        }
                                        
                                        curFrame = entry->send_server_frame;
                                        
                                        if(entry->cmd != ~0){
                                            sprintf(buffer,
                                                    "^4%d^d'^4%d^dB: ^5%s^d.^4^s%d^n",
                                                    entry->bitCount / 8,
                                                    entry->bitCount % 8,
                                                    getShortServerCmdName(entry->cmd),
                                                    entry->cmd);
                                            
                                            NEW_ITEM(buffer, " ");
                                        }
                                    }
                                    
                                    if(curFrame != -1){
                                        END_GROUP()
                                    }
                                }
                                END_GROUP()
                                
                                {
                                    static ClientPacketLogEntrySortable* entryArray;
                                    static int entryArrayMaxCount;
                                    
                                    ClientSubPacketLogEntry* entry = log->subPackets[CLIENT_PAKLOG_IN];
                                    int curFrame = -1;
                                    int min_bits = INT_MAX;
                                    int max_bits = -1;
                                    U32 max_pak_id = 0;
                                    int oop_count = 0;
                                    int entryCount = 0;
                                    int newConnection = 0;
                                    int i;
                                    
                                    // Make array of entries.
                                    
                                    for(entry = log->subPackets[CLIENT_PAKLOG_IN]; entry; entry = entry->next){
                                        ClientPacketLogEntrySortable* es;
                                        
                                        if(entry->parent_pak_bit_count){
                                            min_bits = min(min_bits, (int)entry->parent_pak_bit_count);
                                            max_bits = max(max_bits, (int)entry->parent_pak_bit_count);
                                        }

                                        es = dynArrayAddStruct(&entryArray, &entryCount, &entryArrayMaxCount);
                                        
                                        ZeroStruct(es);
                                        es->entry = entry;
                                    }

                                    // Mark OO Packets.

                                    for(i = entryCount - 1; i >= 0; i--){
                                        ClientSubPacketLogEntry* prev;
                                        
                                        entry = entryArray[i].entry;
                                        
                                        prev = i < entryCount - 1 ? entryArray[i+1].entry : NULL;

                                        if(prev && entry->client_connect_time != prev->client_connect_time){
                                            max_pak_id = 0;
                                        }
                                        entryArray[i].oop = entry->parent_pak_id < max_pak_id ? max_pak_id : 0;
                                        if(entryArray[i].oop){
                                            oop_count++;
                                        }
                                        max_pak_id = max(max_pak_id, entry->parent_pak_id);
                                        entry->next = prev;
                                        prev = entry;
                                    }
                                    
                                    sprintf(buffer,
                                            "in$$Inbound (^4%d^d'^4%d^dB - ^4%d^d'^4%d^dB, ^4%d^doop^n)",
                                            min_bits / 8,
                                            min_bits % 8,
                                            max_bits / 8,
                                            max_bits % 8,
                                            oop_count);
                                    BEGIN_GROUP(buffer, 0)
                                    
                                    sprintf(buffer2, "PacketLog.ShowAllPacketsIn.%d", db_id);
                                    showAllPackets = (int)(intptr_t)clientLinkGetDebugVar(client, buffer2);
                                    sprintf(buffer, "setdebugvar %s %d", buffer2, !showAllPackets);
                                    NEW_ITEM(    showAllPackets ?
                                                    "^1DISABLE ^dShowing All Packets" :
                                                    "^2ENABLE ^dShowing All Packets",
                                                buffer);
                                    
                                    for(i = 0; i < entryCount; i++){
                                        entry = entryArray[i].entry;
                                        
                                        if(!showAllPackets && entry->cmd == ~0){
                                            continue;
                                        }
                                        
                                        if(curFrame == -1 || entry->cmd == ~0 || entry->send_server_frame != curFrame){
                                            ClientSubPacketLogEntry* cur = entry;
                                            int bitCount = 0;
                                            
                                            if(curFrame != -1){
                                                END_GROUP()
                                            }
                                            
                                            // Count all the sub-packets from this same packet.
                                            
                                            for(; cur; cur = cur->next){
                                                if(    cur->parent_pak_id != entry->parent_pak_id ||
                                                    cur->send_server_frame != entry->send_server_frame ||
                                                    cur->parent_pak_bit_count != entry->parent_pak_bit_count)
                                                {
                                                    break;
                                                }
                                                
                                                bitCount += cur->bitCount;
                                            }
                                            
                                            if(    entry->parent_pak_id &&
                                                cur &&
                                                entry->client_connect_time == cur->client_connect_time &&
                                                entry->parent_pak_id - cur->parent_pak_id != 1)
                                            {
                                                sprintf(buffer2, "^r(%d)^d", entry->parent_pak_id - cur->parent_pak_id);
                                            }else{
                                                buffer2[0] = 0;
                                            }
                                            
                                            if(newConnection){
                                                NEW_ITEM("^5New Connection", " ");
                                                newConnection = 0;
                                            }
                                            
                                            if(cur && entry->client_connect_time != cur->client_connect_time){
                                                newConnection = 1;
                                            }

                                            sprintf(buffer,
                                                    "^2^b%1.f/25b %s^%sF^4%d %s^d(^4%d^d'^4%d^dB, ^4%d^d'^4%d^dB)",
                                                    max_bits >= min_bits && entry->parent_pak_bit_count ? 25 * (F32)(entry->parent_pak_bit_count - min_bits) / (max_bits - min_bits) : 0,
                                                    entry->cmd == ~0 ? "^s" : "",
                                                    entryArray[i].oop ? "r" : entry->cmd == ~0 ? "1" : "2",
                                                    entry->send_server_frame,
                                                    buffer2,
                                                    entry->parent_pak_bit_count / 8,
                                                    entry->parent_pak_bit_count % 8,
                                                    bitCount / 8,
                                                    bitCount % 8);
                                            BEGIN_GROUP(buffer, 0)
                                            
                                            sprintf(buffer, "PakID: ^4%d", entry->parent_pak_id);
                                            if(entryArray[i].oop){
                                                sprintf(buffer + strlen(buffer), " ^d(Max: ^4%d^d)", entryArray[i].oop);
                                            }
                                            NEW_ITEM(buffer, " ");
                                        }

                                        curFrame = entry->send_server_frame;
                                        
                                        if(entry->cmd != ~0){
                                            sprintf(buffer,
                                                    "^4%d^d'^4%d^dB: ^5%s^d.^4%d",
                                                    entry->bitCount / 8,
                                                    entry->bitCount % 8,
                                                    getShortClientInpCmdName(entry->cmd),
                                                    entry->cmd);
                                            
                                            NEW_ITEM(buffer, " ");
                                        }
                                    }
                                    
                                    if(curFrame != -1){
                                        END_GROUP()
                                    }
                                }
                                END_GROUP()
                                
                                {
                                    ClientSubPacketLogEntry* entry;
                                    int curFrame = -1;
                                    int min_bits = INT_MAX;
                                    int max_bits = -1;
                                    int newConnection = 0;
                                    int startTime;
                                    
                                    for(entry = log->subPackets[CLIENT_PAKLOG_RAW_OUT]; entry; entry = entry->next){
                                        if(entry->parent_pak_bit_count){
                                            min_bits = min(min_bits, (int)entry->parent_pak_bit_count);
                                            max_bits = max(max_bits, (int)entry->parent_pak_bit_count);
                                        }
                                    }                                    
                                    
                                    sprintf(buffer,
                                            "outraw$$Outbound Raw (^4%d^d'^4%d^dB - ^4%d^d'^4%d^dB^n)",
                                            min_bits / 8,
                                            min_bits % 8,
                                            max_bits / 8,
                                            max_bits % 8);
                                    BEGIN_GROUP(buffer, 0)

                                    sprintf(buffer2, "PacketLog.ShowPacketsOutRaw.%d", db_id);
                                    showAllPackets = (int)(intptr_t)clientLinkGetDebugVar(client, buffer2);
                                    sprintf(buffer, "setdebugvar %s %d", buffer2, !showAllPackets);
                                    NEW_ITEM(showAllPackets ?
                                                "^1DISABLE ^dShowing Packets" :
                                                "^2ENABLE ^dShowing Packets",
                                            buffer);
                                            
                                    entry = log->subPackets[CLIENT_PAKLOG_RAW_OUT];
                                    
                                    if(entry){
                                        startTime = entry->create_abs_time;
                                    }
                                    
                                    if(showAllPackets){
                                        for(entry = log->subPackets[CLIENT_PAKLOG_RAW_OUT]; entry; entry = entry->next){
                                            ClientSubPacketLogEntry* cur = entry;
                                            int bitCount = 0;
                                            
                                            if(curFrame != -1){
                                                END_GROUP()
                                            }
                                            
                                            // Count all the sub-packets from this same packet.
                                            
                                            for(; cur; cur = cur->next){
                                                if(cur->parent_pak_id != entry->parent_pak_id){
                                                    break;
                                                }
                                                
                                                bitCount += cur->bitCount;
                                            }
                                            
                                            if(newConnection){
                                                NEW_ITEM("^5New Connection", " ");
                                                newConnection = 0;
                                            }
                                            
                                            if(cur && entry->client_connect_time != cur->client_connect_time){
                                                newConnection = 1;
                                            }

                                            sprintf(buffer,
                                                    "^2^b%1.f/25b %s^%dP^4%d ^d@ ^4%1.2f^ds %s%s^d(^4%d^d'^4%d^dB)",
                                                    max_bits >= min_bits && entry->parent_pak_bit_count ? 25 * (F32)(entry->parent_pak_bit_count - min_bits) / (max_bits - min_bits) : 0,
                                                    entry->cmd == ~0 ? "^s" : "",
                                                    entry->cmd == ~0 ? 1 : 2,
                                                    entry->parent_pak_id,
                                                    timerSeconds(startTime - entry->create_abs_time),
                                                    entry->send_server_frame ? "^rx" : "",
                                                    entry->send_server_frame ? getCommaSeparatedInt(entry->send_server_frame) : "",
                                                    entry->parent_pak_bit_count / 8,
                                                    entry->parent_pak_bit_count % 8);
                                            BEGIN_GROUP(buffer, 0)
                                            
                                            curFrame = entry->send_server_frame;
                                        }
                                    }
                                    
                                    if(curFrame != -1){
                                        END_GROUP()
                                    }
                                }
                                END_GROUP()
                            }
                        }else{
                            sprintf(buffer, "clientpacketlog 1 %d", db_id);
                            NEW_ITEM("^2ENABLE ^0Packet Log", buffer);
                        }
                        END_GROUP()
                    }
                                        
                    BEGIN_GROUP("Global Stats", 0)
                        if(!clientLinkGetDebugVar(client, "PacketLog.ShowGlobalOut")){
                            NEW_ITEM("^2ENABLE ^dOut Stats", "setdebugvar PacketLog.ShowGlobalOut 1");
                        }else{
                            BEGIN_GROUP("Out (^5^sSERVER_*^d^n)", 1)
                                NEW_ITEM("^1DISABLE ^dOut Stats", "setdebugvar PacketLog.ShowGlobalOut 0");
                                
                                for(i = 0; i < getCmdStatsCount(CLIENT_PAKLOG_OUT); i++){
                                    CmdStats* stats = getCmdStats(CLIENT_PAKLOG_OUT, i, 0);
                                    
                                    if(stats && stats->sentCount){
                                        int bitAvg = (F64)stats->bitCountTotal / (F64)stats->sentCount;
                                        char* pos = buffer;

                                        pos += sprintf(pos, "^s");
                                        
                                        if(entFromDbId(stats->dbIDMin)){
                                            pos += sprintf(pos, "^1[min]");
                                        }
                                        
                                        if(entFromDbId(stats->dbIDMax)){
                                            pos += sprintf(pos, "^7[MAX]");
                                        }
                                                
                                        pos += sprintf(    pos,
                                                        "^5%s^d.^4%d^d:^t280t\t^t50t\t^4%d^dx^t50t\t^4%d^d'^4%d^dB^t50t\t^4%d^d'^4%d^dB",
                                                        getShortServerCmdName(i),
                                                        i,
                                                        stats->sentCount,
                                                        bitAvg / 8,
                                                        bitAvg % 8,
                                                        stats->bitCountMax / 8,
                                                        stats->bitCountMax % 8);
                                                
                                        BEGIN_GROUP(buffer, 0)
                                        {
                                            Entity* statsEnt;
                                            
                                            sprintf(buffer, "Min: ^4%d^d'^4%d^dB", stats->bitCountMin / 8, stats->bitCountMin % 8);
                                            statsEnt = entFromDbId(stats->dbIDMin);
                                            strcpy(buffer2, " ");
                                            if(statsEnt){
                                                sprintf(buffer + strlen(buffer), " ^s(^7%s^d)", statsEnt->name);
                                                sprintf(buffer2, "gotoent %d", statsEnt->owner);
                                            }else{
                                                sprintf(buffer + strlen(buffer), " ^s(^7dbID ^4%d^d)", stats->dbIDMin);
                                            }
                                            NEW_ITEM(buffer, buffer2[0] ? buffer2 : "");
                                            
                                            sprintf(buffer, "Max: ^4%d^d'^4%d^dB", stats->bitCountMax / 8, stats->bitCountMax % 8);
                                            statsEnt = entFromDbId(stats->dbIDMax);
                                            strcpy(buffer2, " ");
                                            if(statsEnt){
                                                sprintf(buffer + strlen(buffer), " ^s(^7%s^d)", statsEnt->name);
                                                sprintf(buffer2, "gotoent %d", statsEnt->owner);
                                            }else{
                                                sprintf(buffer + strlen(buffer), " ^s(^7dbID ^4%d^d)", stats->dbIDMax);
                                            }
                                            NEW_ITEM(buffer, buffer2[0] ? buffer2 : "");
                                        }
                                        END_GROUP()
                                    }
                                }
                            END_GROUP()
                        }

                        if(!clientLinkGetDebugVar(client, "PacketLog.ShowGlobalIn")){
                            NEW_ITEM("^2ENABLE ^dIn Stats", "setdebugvar PacketLog.ShowGlobalIn 1");
                        }else{
                            BEGIN_GROUP("In (^5^sCLIENTINP_*^d^n)", 1)
                                NEW_ITEM("^1DISABLE ^dIn Stats", "setdebugvar PacketLog.ShowGlobalIn 0");

                                for(i = 0; i < getCmdStatsCount(CLIENT_PAKLOG_IN); i++){
                                    CmdStats* stats = getCmdStats(CLIENT_PAKLOG_IN, i, 0);
                                    
                                    if(stats && stats->sentCount){
                                        int bitAvg = (F64)stats->bitCountTotal / (F64)stats->sentCount;
                                        char* pos = buffer;

                                        pos += sprintf(pos, "^s");
                                        
                                        if(entFromDbId(stats->dbIDMin)){
                                            pos += sprintf(pos, "^1[min]");
                                        }
                                        
                                        if(entFromDbId(stats->dbIDMax)){
                                            pos += sprintf(pos, "^7[MAX]");
                                        }
                                                
                                        pos += sprintf(    pos,
                                                        "^5%s^d.^4%d^d:^t250t\t^t50t\t^4%d^dx^t50t\t^4%d^d'^4%d^dB^t50t\t^4%d^d'^4%d^dB",
                                                        getShortClientInpCmdName(i),
                                                        i,
                                                        stats->sentCount,
                                                        bitAvg / 8,
                                                        bitAvg % 8,
                                                        stats->bitCountMax / 8,
                                                        stats->bitCountMax % 8);
                                                        
                                        BEGIN_GROUP(buffer, 0)
                                        {
                                            Entity* statsEnt;
                                            
                                            sprintf(buffer, "Min: ^4%d^d'^4%d^dB", stats->bitCountMin / 8, stats->bitCountMin % 8);
                                            statsEnt = entFromDbId(stats->dbIDMin);
                                            strcpy(buffer2, " ");
                                            if(statsEnt){
                                                sprintf(buffer + strlen(buffer), " ^s(^7%s^d)", statsEnt->name);
                                                sprintf(buffer2, "gotoent %d", statsEnt->owner);
                                            }else{
                                                sprintf(buffer + strlen(buffer), " ^s(^7dbID ^4%d^d)", stats->dbIDMin);
                                            }
                                            NEW_ITEM(buffer, buffer2[0] ? buffer2 : "");
                                            
                                            sprintf(buffer, "Max: ^4%d^d'^4%d^dB", stats->bitCountMax / 8, stats->bitCountMax % 8);
                                            statsEnt = entFromDbId(stats->dbIDMax);
                                            strcpy(buffer2, " ");
                                            if(statsEnt){
                                                sprintf(buffer + strlen(buffer), " ^s(^7%s^d)", statsEnt->name);
                                                sprintf(buffer2, "gotoent %d", statsEnt->owner);
                                            }else{
                                                sprintf(buffer + strlen(buffer), " ^s(^7dbID ^4%d^d)", stats->dbIDMax);
                                            }
                                            NEW_ITEM(buffer, buffer2[0] ? buffer2 : "");
                                        }
                                        END_GROUP()
                                    }
                                 }
                            END_GROUP()
                        }                            
                    END_GROUP();
                }
                END_GROUP()
            }
            
            BEGIN_GROUP("Contacts And Missions", 0)
                if(!(client->debugMenuFlags & DEBUGMENUFLAGS_CONTACTLIST)){
                        sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_CONTACTLIST);
                        NEW_ITEM("^2ENABLE ^dShow Contacts And Missions", buffer);
                    }else{
                        sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_CONTACTLIST);
                        NEW_ITEM("^1DISABLE ^dHide Contacts And Missions", buffer);

                        if (client->debugMenuFlags & DEBUGMENUFLAGS_CONTACTZONESORT)
                        {
                            sprintf(buffer, "&debugmenuflags !%d$$mmm", DEBUGMENUFLAGS_CONTACTZONESORT);
                            NEW_ITEM("Sort Contacts By Name", buffer);
                        }
                        else
                        {
                            sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_CONTACTZONESORT);
                            NEW_ITEM("Sort Contacts By Zone", buffer);
                        }

                        showContactsAndMissions(pak, client->debugMenuFlags & DEBUGMENUFLAGS_CONTACTZONESORT);

                    }
            END_GROUP()



            if(client->debugMenuFlags & DEBUGMENUFLAGS_BEACON_DEV){ 
                BEGIN_GROUP("Beacons", 0)
                    sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_BEACON_DEV);
                    NEW_ITEM("^1DISABLE ^dBeacon Menu", buffer);
                    sprintf(buffer, "ec me beacon 14 0");
                    NEW_ITEM("Show beacon connections near me", buffer);                    

                    beaconCheckBlocksNeedRebuild();
                    
                    sprintf(buffer, "%d Clusters", combatBeaconClusterArray.size);
                    
                    BEGIN_GROUP(buffer, 0)
                        BEGIN_GROUP("Goto", 0)
                            SET_PREFIX("Cluster ", "beacongotocluster ");
                            
                            for(i = 0; i < combatBeaconClusterArray.size; i++){
                                sprintf(buffer, "^4%d ^d(^4%d ^0beacons^d)", i, getClusterBeaconCount(combatBeaconClusterArray.storage[i]));
                                sprintf(buffer2, "%d", i);
                                NEW_ITEM(buffer, buffer2);
                            }
                        END_GROUP()

                        BEGIN_GROUP("Show", 0)
                            SET_PREFIX("Cluster ", "ec me beacon 13 ");
                            
                            for(i = 0; i < combatBeaconClusterArray.size; i++){
                                sprintf(buffer, "^4%d ^d(^4%d ^0beacons^d)", i, getClusterBeaconCount(combatBeaconClusterArray.storage[i]));
                                sprintf(buffer2, "%d", i);
                                NEW_ITEM(buffer, buffer2);
                            }
                        END_GROUP()
                    END_GROUP()

                    BEGIN_GROUP("Generator", 0)
                        if(beaconGetDebugVar("sendlines")){
                            NEW_ITEM("^1DISABLE ^dSending Lines", "beaconsetvar sendlines 0");
                        }else{
                            NEW_ITEM("^2ENABLE ^dSending Lines", "beaconsetvar sendlines 1");
                        }

                        if(beaconGetDebugVar("sendbeacons")){
                            NEW_ITEM("^1DISABLE ^dSending Beacons", "beaconsetvar sendbeacons 0");
                        }else{
                            NEW_ITEM("^2ENABLE ^dSending Beacons", "beaconsetvar sendbeacons 1");
                        }

                        if(beaconGetDebugVar("nocenterfill")){
                            NEW_ITEM("^2ENABLE ^dFill Centers", "beaconsetvar nocenterfill 0");
                        }else{
                            NEW_ITEM("^1DISABLE ^dFill Centers", "beaconsetvar nocenterfill 1");
                        }

                        if(beaconGetDebugVar("showhull")){
                            NEW_ITEM("^1DISABLE ^dConvex Hull", "beaconsetvar showhull 0");
                        }else{
                            NEW_ITEM("^2ENABLE ^dConvex Hull", "beaconsetvar showhull 1");
                        }

                        if(beaconGetDebugVar("showbestcorners")){
                            NEW_ITEM("^1DISABLE ^dBest Corners", "beaconsetvar showbestcorners 0");
                        }else{
                            NEW_ITEM("^2ENABLE ^dBest Corners", "beaconsetvar showbestcorners 1");
                        }
                    END_GROUP()
                END_GROUP()
            }
        }

        BEGIN_GROUP("Static Maps", 0)
        {
            if(!(client->debugMenuFlags & DEBUGMENUFLAGS_STATIC_MAPS)){
                sprintf(buffer, "|debugmenuflags %d$$mmm", DEBUGMENUFLAGS_STATIC_MAPS);
                NEW_ITEM("^2ENABLE ^dStatic Map List", buffer);
            }else{
                extern StaticMapInfo** staticMapInfos;
                int count = eaSize(&staticMapInfos);

                sprintf(buffer, "&debugmenuflags !%d", DEBUGMENUFLAGS_STATIC_MAPS);
                NEW_ITEM("^1DISABLE ^dStatic Map List", buffer);

                BEGIN_GROUP("Empty Maps", 0)
                    for(i = 0; i < count; i++){
                        StaticMapInfo* info = staticMapInfos[i];

                        if (info) // && 
//                            info->baseMapID != 68 && // Vince - removed at the request of Matt M.
//                            info->mapID != 68)
                        {
                            const char* name;

                            if(info->instanceID > 1)
                                sprintf(buffer, "%s_%d", info->name, info->instanceID);
                            else
                                strcpy(buffer, info->name);

                            name = clientPrintf(client,buffer);

                            if(info->playerCount)
                                continue;
                            sprintf(buffer, "%d$$^4%d^d:^t50t\t^1%s^d", info->mapID, info->mapID, name);

                            BEGIN_GROUP(buffer, 0)
                                sprintf(buffer2, "mapmove %d", info->mapID);

                                NEW_ITEM("mapmove", buffer2);
                            END_GROUP()
                        }
                    }
                END_GROUP()

                for(i = 0; i < count; i++){
                    StaticMapInfo* info = staticMapInfos[i];

                    if(info){
                        const char* name;

                        if(info->instanceID > 1)
                            sprintf(buffer, "%s_%d", info->name, info->instanceID);
                        else
                            strcpy(buffer, info->name);

                        name = clientPrintf(client,buffer); 

                        if (!info->playerCount)
                            continue;
                        sprintf(buffer, "^4%d^d:^t50t\t^2%s^d^t50t\t(^4%d^0 (^5%d^0/^1%d^0) plrs^d)", info->mapID, name, info->playerCount, info->heroCount, info->villainCount);

                        BEGIN_GROUP(buffer, 0)
                            sprintf(buffer2, "mapmove %d", info->mapID);

                            NEW_ITEM("mapmove", buffer2);
                        END_GROUP()
                    }
                }
            }
        }
        END_GROUP()

        open =    client->sendAutoTimers && timing_state.runperfinfo ||
                client->controls.controldebug;

        BEGIN_GROUP("Misc Debug", open)
            NEW_ITEM("Copy Map And Pos To Clipboard",    "copydebugpos");

            if(accesslevel){
                NEW_ITEM("Toggle See Everything", "++see_everything");

                NEW_ITEM("Show static maps", "maplist");

                NEW_ITEM(    ai_state.noNPCGroundSnap ? "^2ENABLE ^dNPC Ground Snap" : "^1DISABLE ^dNPC Ground Snap",
                            ai_state.noNPCGroundSnap ? "ec me setnpcgroundsnap 0" : "ec me setnpcgroundsnap 1");

                NEW_ITEM(    global_state.no_file_change_check ? "^2ENABLE ^0File Change Check" : "^1DISABLE ^0File Change Check",
                            global_state.no_file_change_check ? "nofilechangecheck 0" : "nofilechangecheck 1");

                NEW_ITEM(    ai_state.alwaysKnockPlayers ? "^1DISABLE ^dAlways Knock Players" : "^2ENABLE ^dAlways Knock Players",
                            ai_state.alwaysKnockPlayers ? "alwaysknockplayers 0" : "alwaysknockplayers 1");

                //BEGIN_GROUP("Beacon Connections", 0)
                //    NEW_ITEM("NPC Beacons", "shownbeacon 0");
                //    NEW_ITEM("Combat Beacons", "shownbeacon 1");
                //    NEW_ITEM("Traffic Beacons", "shownbeacon 2");
                //    NEW_ITEM("Block Connections", "shownbeacon 3");
                //    NEW_ITEM("5 Levels Of Combat Connections", "shownbeacon 4");
                //    NEW_ITEM("Show Unconnected Things", "shownbeacon 5");
                //END_GROUP()

                NEW_ITEM("Toggle Artist Cost Bar", "simplestats");

                BEGIN_GROUP("Programmer Stuff", open)
                    BEGIN_GROUP("Performance Monitor", open)
                        sprintf(buffer,
                                "svr$$Server (%s^d, ^0Depth = ^4%d^d)",
                                (timing_state.runperfinfo || client->sendAutoTimers) ? "^2On" : "^1Off",
                                timing_state.autoStackMaxDepth);
                        BEGIN_GROUP(buffer, 0)
                            if(timing_state.runperfinfo || client->sendAutoTimers){
                                NEW_ITEM("Server ^1OFF", "runperfinfo_server 0\n&sendautotimers 0");
                            }else{
                                NEW_ITEM("Server ^2ON", "runperfinfo_server -1\nresetperfinfo_server 15\nsendautotimers 1");
                            }

                            NEW_ITEM(    server_state.time_all_ents ? "^1DISABLE ^dEntity Timers" : "^2ENABLE ^dEntity Timers",
                                        server_state.time_all_ents ? "perfinfo_time_all_ents 0" : "perfinfo_time_all_ents 1");

                            NEW_ITEM("^1CLOSE ^dAll Entity Timers", "perfinfo_disable_ent_timers");

                            SET_PREFIX("Depth ^4", "perfinfomaxstack_server ");

                            for(i = 1; i < 20; i++){
                                sprintf(buffer, "%d", i);
                                NEW_ITEM(buffer, buffer);
                            }

                            NEW_ITEM("100", "100");
                        END_GROUP()

                        BEGIN_GROUP("Client", 0)
                            NEW_ITEM("Client ^1OFF", "runperfinfo_game 0");
                            NEW_ITEM("Client ^2ON", "runperfinfo_game -1\nresetperfinfo_game 15");

                            BEGIN_GROUP("FxTimers", 0)
                                NEW_ITEM("FxTimers ^1OFF", "fxtimers 0");
                                NEW_ITEM("FxTimers ^2ON", "fxtimers 1");
                            END_GROUP()

                            SET_PREFIX("Depth ^4", "perfinfomaxstack_game ");

                            for(i = 1; i < 20; i++){
                                sprintf(buffer, "%d", i);
                                NEW_ITEM(buffer, buffer);
                            }

                            NEW_ITEM("100", "100");
                        END_GROUP()
                    END_GROUP()

                    BEGIN_GROUP("Render Stats", 0)
                        BEGIN_GROUP("Add Card Stats", 0)
                            NEW_ITEM("Geforce 6", "addcardstats \"GeForce 6800\"");
                            NEW_ITEM("Geforce 5", "addcardstats \"GeForce 5700\"");
                            NEW_ITEM("Geforce 4", "addcardstats \"GeForce 4600\"");
                            NEW_ITEM("Geforce 4 MX", "addcardstats \"GeForce 4 MX\"");
                            NEW_ITEM("Geforce 2", "addcardstats \"GeForce 2\"");
                            NEW_ITEM("Radeon 9800", "addcardstats \"Radeon 9800\"");
                            NEW_ITEM("Radeon 9700", "addcardstats \"Radeon 9700\"");
                            NEW_ITEM("Radeon 9600", "addcardstats \"Radeon 9600\"");
                            NEW_ITEM("Radeon 8500", "addcardstats \"Radeon 8500\"");
                            NEW_ITEM("Radeon 7200", "addcardstats \"Radeon 7200\"");
                        END_GROUP()
                        BEGIN_GROUP("Remove Card Stats", 0)
                            NEW_ITEM("Geforce 6", "remcardstats \"GeForce 6800\"");
                            NEW_ITEM("Geforce 5", "remcardstats \"GeForce 5700\"");
                            NEW_ITEM("Geforce 4", "remcardstats \"GeForce 4600\"");
                            NEW_ITEM("Geforce 4 MX", "remcardstats \"GeForce 4 MX\"");
                            NEW_ITEM("Geforce 2", "remcardstats \"GeForce 2\"");
                            NEW_ITEM("Radeon 9800", "remcardstats \"Radeon 9800\"");
                            NEW_ITEM("Radeon 9700", "remcardstats \"Radeon 9700\"");
                            NEW_ITEM("Radeon 9600", "remcardstats \"Radeon 9600\"");
                            NEW_ITEM("Radeon 8500", "remcardstats \"Radeon 8500\"");
                            NEW_ITEM("Radeon 7200", "remcardstats \"Radeon 7200\"");
                        END_GROUP()
                        NEW_ITEM("Enable Detailed Bars", "costbars 1");
                        NEW_ITEM("Disable Detailed Bars", "costbars 0");
                        NEW_ITEM("Enable Cost Graph", "costgraph 1");
                        NEW_ITEM("Disable Cost Graph", "costgraph 0");
                        NEW_ITEM("Show Scene Complexity Info", "scomp 1");
                        NEW_ITEM("Hide Scene Complexity Info", "scomp 0");
                    END_GROUP()

                    sprintf(buffer, "dmf$$Debug Menu Flags");
                    BEGIN_GROUP(buffer, 0)
                        #define CREATE_COMMAND(name,bit){                                                                            \
                            sprintf(buffer, "%s ^d%s", client->debugMenuFlags & (bit) ? "^1DISABLE" : "^2ENABLE", name);            \
                            sprintf(buffer2, client->debugMenuFlags & (bit) ? "&debugmenuflags !%d" : "|debugmenuflags %d", (bit));    \
                            NEW_ITEM(buffer, buffer2);                                                                                \
                        }

                        CREATE_COMMAND("Player Info",        DEBUGMENUFLAGS_PLAYERLIST);
                        CREATE_COMMAND("Quick Menu",        DEBUGMENUFLAGS_QUICKMENU);
                        CREATE_COMMAND("Path Log",            DEBUGMENUFLAGS_PATHLOG);
                        CREATE_COMMAND("Beacon Dev",        DEBUGMENUFLAGS_BEACON_DEV);
                        CREATE_COMMAND("Player Net Stats",    DEBUGMENUFLAGS_PLAYER_NET_STATS);

                        #undef CREATE_COMMAND
                    END_GROUP()

                    BEGIN_GROUP("Entity Interpolation", 0)
                        NEW_ITEM(    client->controls.controldebug ? "Control Debugging ^1OFF" : "Control Debugging ^2ON",
                                    client->controls.controldebug ? "controldebug 0" : "controldebug 1");

                        NEW_ITEM(    server_state.disableMeleePrediction ? "^2ENABLE ^0Melee Prediction" : "^1DISABLE ^0Melee Prediction",
                                    server_state.disableMeleePrediction ? "nomeleeprediction 0" : "nomeleeprediction 1");

                        NEW_ITEM("Default Levels", "interpdatalevel 2\ninterpdataprecision 1");

                        if(e){
                            sprintf(buffer, "setdebugentity %d", e->owner);
                            NEW_ITEM("Set Interp Debug Entity", buffer);
                        }

                        if(entSendGetInterpStoreEntityId()){
                            NEW_ITEM("Clear Interp Debug Entity", "setdebugentity 0");
                        }

                        BEGIN_GROUP("Level", 0)
                            SET_PREFIX("Level ^4", "interpdatalevel ");
                            for(i = 0; i <= 3; i++){
                                sprintf(buffer, "%d%s", i, client->interpDataLevel == i ? " ^3[CURRENT]" : "");
                                sprintf(buffer2, "%d", i);
                                NEW_ITEM(buffer, buffer2);
                            }
                        END_GROUP()

                        BEGIN_GROUP("Precision", 0)
                            SET_PREFIX("^4", "interpdataprecision ");
                            for(i = 0; i <= 3; i++){
                                sprintf(buffer, "%d ^dBit%s", 5 + i, client->interpDataPrecision == i ? " ^3[CURRENT]" : "");
                                sprintf(buffer2, "%d", i);
                                NEW_ITEM(buffer, buffer2);
                            }
                        END_GROUP()
                    END_GROUP()
                END_GROUP()
            }
        END_GROUP()

        if(e && accesslevel){
            AIVars* ai = ENTAI(e);
            Character* pchar = e->pchar;

            sprintf(buffer, "selent$$Selected Entity: %s ^d(ID: ^4%d^d)", AI_LOG_ENT_NAME(e), entGetId(e));
            BEGIN_GROUP(buffer, 1)
                BEGIN_GROUP("Entity-Specific Debug Info", 1)
                    #define CREATE_COMMAND(name,bit){                                                                \
                        sprintf(buffer, "%s  ^6%s ^dINFO",                                                            \
                        entDebugInfo & (bit) ? "^1DISABLE" : "^2ENABLE", name);                                        \
                        sprintf(buffer2, entDebugInfo & (bit) ? "&entdebuginfo !%d" : "|entdebuginfo %d", (bit));    \
                        NEW_ITEM(buffer, buffer2);                                                                    \
                    }

                    #define CREATE_COMMAND_DEBUGVAR(name,var){                                                                \
                        sprintf(buffer, "%s  ^6%s ^dINFO",                                                            \
                        clientLinkGetDebugVar(client, (var)) ? "^1DISABLE" : "^2ENABLE", name);                        \
                        sprintf(buffer2, clientLinkGetDebugVar(client, (var)) ? "setdebugvar %s 0" : "setdebugvar %s 1", (var));    \
                        NEW_ITEM(buffer, buffer2);                                                                    \
                    }

                    switch(entType){
                        xcase ENTTYPE_CRITTER:
                            CREATE_COMMAND("Critter",                ENTDEBUGINFO_CRITTER);
                            CREATE_COMMAND("Powers",                ENTDEBUGINFO_POWERS);
                            CREATE_COMMAND("Combat",                ENTDEBUGINFO_COMBAT);
                            CREATE_COMMAND("Enhancements",            ENTDEBUGINFO_ENHANCEMENT);

                            BEGIN_GROUP("AI Flags", 0)
                                CREATE_COMMAND("AI Stuff",            ENTDEBUGINFO_AI_STUFF);
                                CREATE_COMMAND_DEBUGVAR("Behavior", "Ai.Behavior");
                                CREATE_COMMAND_DEBUGVAR("Behavior History", "Ai.Behavior.History");
                                CREATE_COMMAND("Event History",     ENTDEBUGINFO_EVENT_HISTORY);
                                CREATE_COMMAND("Feeling Info",        ENTDEBUGINFO_FEELING_INFO);
                                CREATE_COMMAND("Status Table",        ENTDEBUGINFO_STATUS_TABLE);
                                CREATE_COMMAND("Team Info",            ENTDEBUGINFO_TEAM_INFO);
                                CREATE_COMMAND("Team Powers Info",    ENTDEBUGINFO_TEAM_INFO);
                                CREATE_COMMAND("Path",                ENTDEBUGINFO_PATH);
                            END_GROUP()

                        xcase ENTTYPE_PLAYER:
                            if(e == player){
                                CREATE_COMMAND("Self",                ENTDEBUGINFO_SELF);
                            }else{
                                CREATE_COMMAND("Player",            ENTDEBUGINFO_PLAYER);
                            }

                            CREATE_COMMAND("Player-Only",            ENTDEBUGINFO_PLAYER_ONLY);
                            CREATE_COMMAND("Powers",                 ENTDEBUGINFO_POWERS);
                            CREATE_COMMAND("Combat",                 ENTDEBUGINFO_COMBAT);
                            CREATE_COMMAND("Enhancements",            ENTDEBUGINFO_ENHANCEMENT);

                        xcase ENTTYPE_NPC:
                            CREATE_COMMAND("NPC",                    ENTDEBUGINFO_NPC);

                        xcase ENTTYPE_CAR:
                            CREATE_COMMAND("Car",                    ENTDEBUGINFO_CAR);

                        xcase ENTTYPE_DOOR:
                            CREATE_COMMAND("Door",                    ENTDEBUGINFO_DOOR);
                    }

                    BEGIN_GROUP("Common Flags", 0)
                        CREATE_COMMAND("No Basic Info",                ENTDEBUGINFO_DISABLE_BASIC);
                        CREATE_COMMAND("Sequencer",                    ENTDEBUGINFO_SEQUENCER);
                        CREATE_COMMAND("AI Stuff",                    ENTDEBUGINFO_AI_STUFF);
                        CREATE_COMMAND("Path",                        ENTDEBUGINFO_PATH);
                        CREATE_COMMAND("Physics",                    ENTDEBUGINFO_PHYSICS);
                        CREATE_COMMAND("Auto Powers",                ENTDEBUGINFO_AUTO_POWERS);
                    END_GROUP()

                    #undef CREATE_COMMAND
                    #undef CREATE_COMMAND_DEBUGVAR
                END_GROUP()

                BEGIN_GROUP("Misc Stuff", 0)
                    sprintf(buffer,
                            "ec %d pause %d",
                            e->owner,
                            !ENTPAUSED(e));
                    NEW_ITEM(ENTPAUSED(e) ? "Unpause Entity" : "Pause Entity", buffer);

                    NEW_ITEM("Move To Me", "moveentitytome");
                    NEW_ITEM("Follow With Camera", "camerafollowentity");

                    sprintf(buffer, "perfinfo_enable_ent_timer %d", e->owner);
                    NEW_ITEM("Open Entity Timer", buffer);

                    if(entType != ENTTYPE_PLAYER){
                        AIPriorityManager* manager;
                        if(ai && (manager = aiBehaviorGetPriorityManager(e, 0, 0)) && manager->list){
                            sprintf(buffer, "Restart Priority List: ^6%s", manager->list->name);
                            sprintf(buffer2, "ec %d setprioritylist %s", e->owner, manager->list->name);
                            NEW_ITEM(buffer, buffer2);
                        }

                        if(e->encounterInfo){
                            const char* name = e->encounterInfo->parentGroup->active->spawndef->fileName;
                            if (!name) name = "";
                            sprintf(buffer, "Restart Encounter: ^5...%s", name + strlen(name) - min(strlen(name), 30));
                            sprintf(buffer2, "entityencounterspawn %d", e->owner);
                            NEW_ITEM(buffer, buffer2);
                        }
                    }
                END_GROUP()

                if(pchar){
                    sprintf(buffer,
                            "sethp$$Set ^2HP^t100t\t^b%d/50b ^d(^4%1.1f^d)",
                            (int)ceil(50 * pchar->attrCur.fHitPoints / pchar->attrMax.fHitPoints),
                            pchar->attrCur.fHitPoints);
                    BEGIN_GROUP(buffer, 0)
                        sprintf(buffer, "ec %d sethealth ", entGetId(e));
                        SET_PREFIX("", buffer);
                        sprintf(buffer, "Max (^4%1.3f^d)", pchar->attrMax.fHitPoints);
                        sprintf(buffer2, "%f", pchar->attrMax.fHitPoints);
                        NEW_ITEM(buffer, buffer2);
                        NEW_ITEM("^41 ^dhp", "1");

                        for(i = 0; i <= 100; i += 10){
                            sprintf(buffer, "^4%d^d%%", i);
                            sprintf(buffer2, "%d", (int)ceil(i * pchar->attrMax.fHitPoints / 100));
                            NEW_ITEM(buffer, buffer2);
                        }
                    END_GROUP()

                    sprintf(buffer,
                            "setend$$Set ^5Endurance^t100t\t^b%d/50b ^d(^4%1.1f^d)",
                            (int)ceil(50 * pchar->attrCur.fEndurance / pchar->attrMax.fEndurance),
                            pchar->attrCur.fEndurance);
                    BEGIN_GROUP(buffer, 0)
                        sprintf(buffer, "ec %d setendurance ", entGetId(e));
                        SET_PREFIX("", buffer);
                        sprintf(buffer, "Max (^4%1.3f^d)", pchar->attrMax.fEndurance);
                        sprintf(buffer2, "%f", pchar->attrMax.fEndurance);
                        NEW_ITEM(buffer, buffer2);
                        NEW_ITEM("1", "1");
                        NEW_ITEM("0", "0");
                    END_GROUP()

                    sprintf(buffer,
                            "setins$$Set ^5Insight^t100t\t^b%d/50b ^d(^4%1.1f^d)",
                            (int)ceil(50 * pchar->attrCur.fInsight / pchar->attrMax.fInsight),
                            pchar->attrCur.fInsight);
                    BEGIN_GROUP(buffer, 0)
                        sprintf(buffer, "ec %d setinsight ", entGetId(e));
                        SET_PREFIX("", buffer);
                        sprintf(buffer, "Max (^4%1.3f^d)", pchar->attrMax.fInsight);
                        sprintf(buffer2, "%f", pchar->attrMax.fInsight);
                        NEW_ITEM(buffer, buffer2);
                        NEW_ITEM("1", "1");
                        NEW_ITEM("0", "0");
                    END_GROUP()
                }

                BEGIN_GROUP("Flags", 0)
                    sprintf(buffer, "ec %d setinvincible %d", e->owner, e->invincible ? 0 : 1);
                    NEW_ITEM(e->invincible ? "^1DISABLE ^dInvincible" : "^2ENABLE ^dInvincible", buffer);

                    sprintf(buffer, "ec %d setunstoppable %d", e->owner, e->unstoppable ? 0 : 1);
                    NEW_ITEM(e->unstoppable ? "^1DISABLE ^dUnstoppable" : "^2ENABLE ^dUnstoppable", buffer);

                    sprintf(buffer, "ec %d setalwayshit %d", e->owner, e->alwaysHit ? 0 : 1);
                    NEW_ITEM(e->alwaysHit ? "^1DISABLE ^dAlways Hit" : "^2ENABLE ^dAlways Hit", buffer);

                    sprintf(buffer, "ec %d setuntargetable %d", e->owner, e->untargetable ? 0 : 1);
                    NEW_ITEM(e->untargetable ? "^1DISABLE ^dUntargetable" : "^2ENABLE ^dUntargetable", buffer);

                    sprintf(buffer, "ec %d setinvisible %d", e->owner, ENTHIDE(e) ? 0 : 1);
                    NEW_ITEM(ENTHIDE(e) ? "^1DISABLE ^dInvisible" : "^2ENABLE ^dInvisible", buffer);

                    if(e && e->pchar)
                    {
                        sprintf(buffer, "ec %d ignorecombatmods %d", e->owner, e->pchar->bIgnoreCombatMods?0:1);
                        NEW_ITEM(e->pchar->bIgnoreCombatMods ? "^1DISABLE ^dIgnore Combat Mods" : "^2ENABLE ^dIgnore Combat Mods", buffer);

                        sprintf( buffer, "ec %d setgang 0", entGetId(e) );
                        sprintf( buffer2,"Leave Gang %d", e->pchar->iGangID );
                        NEW_ITEM(e->pchar->iGangID ? buffer2 : "No Gang", buffer);

                        sprintf(buffer, "ec %d setteam 0 %d", entGetId(e), e->pchar->iAllyID!=kAllyID_Good);
                        NEW_ITEM(e->pchar->iAllyID==kAllyID_Good ? "^1DISABLE ^0Ally Hero/Good" : "^2ENABLE ^0Ally Hero/Good", buffer);

                        sprintf(buffer, "ec %d setteam 1 %d", entGetId(e), e->pchar->iAllyID!=kAllyID_Evil);
                        NEW_ITEM(e->pchar->iAllyID==kAllyID_Evil ? "^1DISABLE ^0Ally Monster/Evil" : "^2ENABLE ^0Ally Monster/Evil", buffer);

                        sprintf(buffer, "ec %d setteam 2 %d", entGetId(e), e->pchar->iAllyID!=kAllyID_Foul);
                        NEW_ITEM(e->pchar->iAllyID==kAllyID_Foul ? "^1DISABLE ^0Ally Villain/Foul" : "^2ENABLE ^0Ally Villain/Foul", buffer);

                        for(i = 3; i <= 10; i++){
                            sprintf(buffer, "ec %d setteam %d 1", entGetId(e), i);
                            sprintf(buffer2, "Set team to %d%s", i, e->pchar->iAllyID==i?" <-- current team":"");
                            NEW_ITEM(buffer2, buffer);
                        }
                    }

                    if(entType != ENTTYPE_PLAYER)
                    {
                        sprintf(buffer, "ec %d setdonotusepowers %d", e->owner, ai->doNotUsePowers ? 0 : 1);
                        NEW_ITEM(ai->doNotUsePowers ? "^1DISABLE ^dDo Not Use Powers" : "^2ENABLE ^dDo Not Use Powers", buffer);
                        
                        sprintf(buffer, "ec %d setdonotmove %d", e->owner, ai->doNotMove ? 0 : 1);
                        NEW_ITEM(ai->doNotMove ? "^1DISABLE ^dDo Not Move" : "^2ENABLE ^dDo Not Move", buffer);
                    }
                END_GROUP()

                if(entType != ENTTYPE_PLAYER){
                    AIPriorityManager* manager = aiBehaviorGetPriorityManager(e, 0, 0);
                    BEGIN_GROUP("Set ^8AI Config", 0)
                    {
                        NEW_ITEM("^2RELOAD ^dAI Configs", "reloadaiconfig");

                        sprintf(buffer, "ec %d setaiconfig ", e->owner);
                        SET_PREFIX("", buffer);

                        for(i = 0; i < eaSize(&aiAllConfigs.configs); i++){
                            int refType = 0;//aiConfigGetRefCount(aiAllConfigs.configs[i]);

                            sprintf(buffer, "^%d%s", refType, aiAllConfigs.configs[i]->name);
                            sprintf(buffer2, "%s", aiAllConfigs.configs[i]->name);
                            NEW_ITEM(buffer, buffer2);
                        }
                    }
                    END_GROUP()

//                     BEGIN_GROUP("Set ^3Priority List", 0)
//                         NEW_ITEM("Reload Priority Lists", "reloadpriority");
// 
//                         sprintf(buffer, "ec %d behavioradd ", e->owner);
//                         NEW_ITEM(manager && manager->list ? "NONE" : "^6NONE", buffer);
// 
//                         SET_PREFIX("", buffer);
// 
//                         for(i = 0; i < aiGetPriorityListCount(); i++){
//                             int color = 3;
// 
//                             if(    manager && manager->list &&
//                                 manager->list->name &&
//                                 !strcmp(aiGetPriorityListNameByIndex(i), manager->list->name))
//                             {
//                                 color = 6;
//                             }
// 
//                             sprintf(buffer, "^%d%s", color, aiGetPriorityListNameByIndex(i));
// 
//                             NEW_ITEM(buffer, aiGetPriorityListNameByIndex(i));
//                         }
//                     END_GROUP()


                    BEGIN_GROUP("Set ^3Behavior Alias", 0)
                        sprintf(buffer, "ec %d behavioradd ", e->owner);
                        SET_PREFIX("", buffer);

                        for(i = 0; i < aiBehaviorDebugGetAliasCount(); i++){
                            int color = 3;
                            const char* alias = aiBehaviorDebugGetAliasEntry(i);

                            sprintf(buffer, "^%d%s", color, alias);

                            NEW_ITEM(buffer, alias);
                        }
                    END_GROUP()

                        
                    BEGIN_GROUP("Set ^3Anim List", 0)
                        sprintf(buffer, "ec %d behavioradd ", e->owner);
                        SET_PREFIX("", buffer);

                        for(i = 0; i < alDebugGetAnimListCount(); i++){
                            int color = 3;
                            char* list = alDebugGetAnimListStr(i);

                            sprintf(buffer, "^%d%s", color, list);

                            NEW_ITEM(buffer, list);
                        }
                    END_GROUP()
                        
                        
                        
//                     BEGIN_GROUP("^9Behaviors", 0)
//                         sprintf(buffer, "ec %d behaviorprioritylist", e->owner);
//                         NEW_ITEM("Run Priority Lists as Behaviors",buffer);
//                     END_GROUP()
// 
                    BEGIN_GROUP("^5Powers", 0)
                    {
                        int i;

                        if(entType != ENTTYPE_PLAYER)
                        {
                            sprintf(buffer, "ec %d setdonotusepowers %d", e->owner, ai->doNotUsePowers ? 0 : 1);
                            NEW_ITEM(ai->doNotUsePowers ? "^1DISABLE ^dDo Not Use Powers" : "^2ENABLE ^dDo Not Use Powers", buffer);
                        }
                        sprintf(buffer, "ec %d rechargepower -1", e->owner);
                        NEW_ITEM("Recharge Powers", buffer);

                        BEGIN_GROUP("Set ^5Attack Power", 0)
                        {
                            AIPowerInfo* info;

                            sprintf(buffer, "ec %d setpower ", entGetId(e));

                            NEW_ITEM("^6NONE", buffer);

                            SET_PREFIX("", buffer);

                            for(info = ai->power.list, i = 0; info; info = info->next, i++){
                                Power* power = info->power;

                                if(info->isAttack){
                                    sprintf(buffer, "%s%s ^d(^4%1.2fs^d) ^d(^41.2fp, ^41.2fop^d)",
                                            info == ai->power.current ? "^6" : "^5",
                                            power->ppowBase->pchName,
                                            power->fTimer,
                                            power->ppowBase->fPreferenceMultiplier,
                                            info->critterOverridePreferenceMultiplier);
                                    sprintf(buffer2, "%d", i);

                                    NEW_ITEM(buffer, buffer2);
                                }
                            }
                        }
                        END_GROUP()

                        BEGIN_GROUP("Use/Toggle ^5Power", 0)
                        {
                            AIPowerInfo* info;

                            sprintf(buffer, "ec %d usepower ", entGetId(e));

                            SET_PREFIX("", buffer);

                            for(info = ai->power.list, i = 0; info; info = info->next, i++){
                                Power* power = info->power;

                                if(info->isAuto)
                                    continue;

                                sprintf(buffer, "%s%s ^d(^4%1.2fs^d) ^d(^4%1.2fp, ^4%1.2fop^d)",
                                        power->bActive ? "^r" : info == ai->power.current ? "^6" : "^5",
                                        power->ppowBase->pchName,
                                        power->fTimer,
                                        power->ppowBase->fPreferenceMultiplier,
                                        info->critterOverridePreferenceMultiplier);
                                sprintf(buffer2, "%d", i);

                                NEW_ITEM(buffer, buffer2);
                            }
                        }
                        END_GROUP()
                    }
                    END_GROUP()

                    BEGIN_GROUP("Dangerous", 0)
                    {
                        sprintf(buffer,
                                "ec %d kill",
                                e->owner);
                        NEW_ITEM("Destroy This Entity", buffer);
                    }
                    END_GROUP()
                }
            END_GROUP()
        }

        //BEGIN_GROUP("Packet Info", 0)
        //    sprintf(buffer, "Size: %d", pak->stream.size);
        //    NEW_ITEM(buffer, "");
        //END_GROUP()
    END_GROUP()

    // Send the MOTD.

    {
        char buffer[10000];
        char* curPos = buffer;
        int total = 0;
        int availableCount = 0;
        int unavailableCount = 0;
        U32 curTime = dbSecondsSince2000();

        struct {
            int total;
            int sleeping;
            int paused;
        } counts[ENTTYPE_COUNT + 1];

        curPos += sprintf(curPos, "^8Message Of The Day\n^9^l300l\n");
        
        ZeroArray(counts);

        for(i = 0; i < entities_max; i++){
            Entity* e = validEntFromId(i);

            if(e){
                EntType entType = ENTTYPE(e);

                counts[ENTTYPE_COUNT].total++;

                counts[entType].total++;

                if(ENTSLEEPING_BY_ID(i)){
                    counts[ENTTYPE_COUNT].sleeping++;
                    counts[entType].sleeping++;
                }

                if(ENTPAUSED_BY_ID(i)){
                    counts[ENTTYPE_COUNT].paused++;
                    counts[entType].paused++;
                }
            }
            else if(entities[i]){
                if(abs(curTime - entities[i]->freed_time) > 5){
                    availableCount++;
                }else{
                    unavailableCount++;
                }
            }
        }

        curPos += sprintf(curPos, "^t100tEntType\tCount\tSleeping\tPaused\n");

        {
            int rowCount = 0;

            for(i = 1; i < ARRAY_SIZE(counts); i++){
                const char* name;

                if(i == ENTTYPE_HERO)
                    continue;

                name = i == ENTTYPE_COUNT ? "Total" : getCamelCaseEntTypeName(i);

                if(!counts[i].total){
                    curPos += sprintf(curPos, "^n%s:\t^s^40\n", name);
                }else{
                    curPos += sprintf(    curPos,
                                        "^n^%d^h%s^0:\t%s^4%d\t%s%d\t%s%s%d\t\n",
                                        i == ENTTYPE_COUNT ? 5 : 1 + rowCount % 2,
                                        name,
                                        counts[i].total ? "" : "^s",
                                        counts[i].total,
                                        counts[i].sleeping ? "^n" : "^s",
                                        counts[i].sleeping,
                                        counts[i].paused ? "^n" : "^s",
                                        counts[i].paused ? "^r" : "^4",
                                        counts[i].paused);
                }

                rowCount++;
            }
        }

        curPos += sprintf(    curPos,
                            "^t100t"
                            //"Players:\t^4%d\n"
                            //"Critters:\t^4%d^0, ^4%d ^0awake\n"
                            //"NPCs:\t^4%d\n"
                            //"Cars:\t^4%d\n"
                            //"Doors:\t^4%d\n"
                            "^nAvailable:\t^4%d\n"
                            "Unavailable:\t^4%d\n"
                            //"Total:\t^4%d\n"
                            "",
                            //entCount[ENTTYPE_PLAYER],
                            //entCount[ENTTYPE_CRITTER],
                            //awakeCritterCount,
                            //entCount[ENTTYPE_NPC],
                            //entCount[ENTTYPE_CAR],
                            //entCount[ENTTYPE_DOOR],
                            availableCount,
                            unavailableCount//,
                            //total
                            );

        pktSendString(pak, buffer);
    }

    pktSend(&pak, server_state.curClient->link);

    PERFINFO_AUTO_STOP();
}
