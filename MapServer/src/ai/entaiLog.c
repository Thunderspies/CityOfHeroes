
#include "entaiprivate.h"
#include "gameComm/Npc.h"
#include "entity/entity.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/mathutil.h>

int aiLogCurFlagBit = 0;

//**************************************************************************************
// AI Logging.
//**************************************************************************************

static int aiLogInit(AIVars* ai){
    // Create log struct if necessary.
    
    if(!ai->log){
        ai->log = calloc(1, sizeof(*ai->log));
        
        if(!ai->log)
            return 0;
    }
    
    return 1;
}

static void aiLogLine(Entity* e, char* txt){
    AIVars* ai = ENTAI(e);
    int addLen;
    char* pos;
    int logLen = ai->log ? (int)strlen(ai->log->text) : 0;
    char buffer[AI_LOG_LENGTH + 1];
    char* log;
    
    log = ai->log->text;
    
    if(!logLen || log[logLen - 1] == '\n')
    {
        __int64 curTime = timerCpuTicks64();
        addLen = sprintf(    buffer,
                            "^%d%3.3d^0(^4%1.2f^0)(^2%d^0) : %s",
                            ai->log->logTickColor + 1,
                            global_state.global_frame_count % 1000,
                            fmod((float)curTime / timerCpuSpeed64(), 60),
                            aiLogCurFlagBit, txt);
    }else{
        addLen = sprintf(buffer, "%s", txt);
    }

    pos = buffer;

    if(addLen > AI_LOG_LENGTH){
        pos += addLen - AI_LOG_LENGTH;
        
        // Find the next newline, if this isn't the beginning of a line.
        
        if(*(pos-1) != '\n')
            for(; *pos && *pos != '\n'; pos++);
            
        if(*pos == '\n'){
            pos++;
        }
        
        strcpy(log, pos);
    }else{
        int newHead = 0;
        
        // Find the first newline character that leaves enough extra space for the new text.
        
        if(logLen + addLen > AI_LOG_LENGTH){
            int i;
            for(i = 0; i < logLen; i++){
                if(log[i] == '\n' && logLen - (i + 1) + addLen <= AI_LOG_LENGTH){
                    newHead = i + 1;
                    break;
                }
            }
            
            if(i == logLen){
                newHead = logLen;
            }
        }
        
        if(newHead)
            memmove(log, log + newHead, logLen - newHead);
            
        strcpy(log + logLen - newHead, buffer);
    }
}

static void aiLogUpdateColor(Entity* e){
    AIVars* ai = ENTAI(e);
    
    // Check if this is a different tick than the last logging, if so then increment the color.
    
    if(global_state.global_frame_count != ai->log->logTick){
        ai->log->logTick = global_state.global_frame_count;
        ai->log->logTickColor = (ai->log->logTickColor + 1) % 9;
        aiLogLine(e, "^5^l400l\n");
    }
}

void aiLog(Entity* e, char* txt, ...){
    AIVars* ai = ENTAI(e);
    va_list va;
    char buffer[10000];
    char* cursor;
    char* token;
    char delim;

    if(!ai || !aiLogInit(ai)){
        return;
    }
    
    // Get string from variable length params.

    va_start(va, txt);
    vsprintf(buffer, txt, va);
    va_end(va);
    
    if(!buffer[0])
        return;
    
    aiLogUpdateColor(e);
    
    for(cursor = buffer, token = strsep2(&cursor, "\n", &delim); token; token = strsep2(&cursor, "\n", &delim)){
        char temp;

        if(delim == '\n'){
            temp = *cursor;
            cursor[-1] = '\n';
            *cursor = '\0';
        }

        aiLogLine(e, token);

        if(delim == '\n'){
            *cursor = temp;
        }
    }
}

void aiLogClear(Entity* e){
    if(!e || !ENTAI(e) || !ENTAI(e)->log)
        return;

    free(ENTAI(e)->log);
    
    ENTAI(e)->log = NULL;
}

void aiPointLog(Entity* e, Vec3 pos, int argb, char* txt, ...){
    AIVars* ai = ENTAI(e);
    va_list va;
    char buffer[1000];
    AIPointLogPoint* p;

    if(!ai || !aiLogInit(ai)){
        return;
    }

    // Get string from variable length params.

    va_start(va, txt);
    vsprintf(buffer, txt, va);
    va_end(va);
    
    if(strlen(buffer) >= AI_POINTLOGPOINT_TAG_LENGTH){
        buffer[AI_POINTLOGPOINT_TAG_LENGTH] = '\0';
    }
    
    p = ai->log->pointLog.points + ai->log->pointLog.curPoint;

    strcpy(p->tag, buffer);
    copyVec3(pos, p->pos);
    p->argb = argb;

    ai->log->pointLog.curPoint = (ai->log->pointLog.curPoint + 1) % AI_POINTLOG_LENGTH;
}

const char* aiLogEntName(Entity* e){
    static int curBuffer = 0;
    static char buffers[10][100];
    char* buffer = buffers[curBuffer = (curBuffer + 1) % 10];
    
    sprintf(buffer,
            "^%s%s",
            e ?
                ENTTYPE(e) == ENTTYPE_PLAYER ?
                    "7" :
                    "6" :
                "1",
            e ?
                e->name[0] ?
                    e->name :
                    ENTTYPE(e) == ENTTYPE_PLAYER ?
                        "Unnamed Player" :
                        npcDefList.npcDefs[e->npcIndex]->name :
                "NOBODY");
                        
    return buffer;
}