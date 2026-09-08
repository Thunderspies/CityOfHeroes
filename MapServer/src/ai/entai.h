#include "entity/entity_enum.h"
#ifndef ENTAI_H
#define ENTAI_H

typedef struct ScoreBoard ScoreBoard;
typedef struct GeneratedEntDef GeneratedEntDef;
typedef struct BasePower BasePower;
typedef struct Entity Entity;
typedef struct Power Power;
typedef struct ClientLink ClientLink;
typedef struct Packet Packet;
typedef struct AIPriorityManager AIPriorityManager;

#include "entaivars.h"

//----------------------------------------------------------------
// entaiMessage.c
//----------------------------------------------------------------

void* aiSendMessage(Entity* e, AIMessage msg);

//----------------------------------------------------------------
// entaiInstance.c
//----------------------------------------------------------------

void aiInitLoadConfigFiles(int forceReload);
void aiInitSystem(void);
void aiSetConfigByName(Entity* e, AIVars* ai, const char* configName);
void aiInit(Entity* e, GeneratedEntDef* generatedType);
void aiDestroy(Entity* e);
void aiReInitCoh(Entity* e);
void aiAddPower(Entity* e, Power* power);
void aiRemovePower(Entity* e, Power* power);

//----------------------------------------------------------------
// entaiPriority.c
//----------------------------------------------------------------

void aiPriorityLoadFiles(int forceReload);
void aiPriorityQueuePriorityList(Entity* e, const char* listName, int forceStart, AIPriorityManager* manager);
int aiPriorityAddFlag(Entity* e, char* flag_str);

//----------------------------------------------------------------
// entaiEvent.c
//----------------------------------------------------------------

typedef enum AIEventNotifyType {
    // Power stuff.
    
    AI_EVENT_NOTIFY_POWER_SUCCEEDED,
    AI_EVENT_NOTIFY_POWER_FAILED,
    AI_EVENT_NOTIFY_POWER_ENDED,
    
    // Other stuff.

    AI_EVENT_NOTIFY_ENTITY_KILLED,
    AI_EVENT_NOTIFY_PET_CREATED,
} AIEventNotifyType;

typedef struct AIEventNotifyParams {
    AIEventNotifyType        notifyType;

    Entity*                    source;
    Entity*                    target;

    union {
        struct {
            const BasePower*    ppowBase;
            S32                offAttrib;
            S32                offAspect;
            F32                fAmount;
            F32                fRadius;
            S32                revealSource;
            U32                uiInvokeID;
        } powerSucceeded;

        struct {
            Power*            ppow;
            S32                revealSource;
            S32                targetNotOnGround;
        } powerFailed;

        struct {
            const BasePower*    ppowBase;
            S32                offAttrib;
            S32                offAspect;
            U32                uiInvokeID;
        } powerEnded;
    };
} AIEventNotifyParams;

void aiEventNotify(AIEventNotifyParams* params);
void aiEventUpdateEvents(AIVars* ai);

//----------------------------------------------------------------
// entaiDebug.c
//----------------------------------------------------------------

void aiResetEntSend(void);
void aiEntSendAddLine(const Vec3 p1, int argb1, Vec3 p2, int argb2);
void aiEntSendAddText(Vec3 pos, const char* text, int flags);
void aiGetEntDebugInfo(Entity* e, ClientLink* clientLink, Packet* pak);
void aiSendGlobalEntDebugInfo(ClientLink *client, Packet *pak);
void aiDebugSendEntDebugMenu(ClientLink* client);
void aiEntSendMatrix(Mat4 mat, const char* textString, int flags);

//----------------------------------------------------------------
// entaiDoor.c
//----------------------------------------------------------------

void aiDoorInit(Entity* e);
void aiDoorSetOpen(Entity* e, int open);

//----------------------------------------------------------------
// entaiNPC.c
//----------------------------------------------------------------

void aiNPCResetBeaconRadius(void);

//----------------------------------------------------------------
// entaiPriority.c
//----------------------------------------------------------------

bool aiHasPriorityList(Entity *e);

//----------------------------------------------------------------
// entai.c
//----------------------------------------------------------------

void aiCritterDeathHandler(Entity* e);
void aiStopJumping(Entity* e);
void aiSetActivity(Entity* e, AIVars* ai, AIActivity activity);
void aiSetShoutRadius(Entity* e, float radius);
void aiSetShoutChance(Entity* e, int chance);

//----------------------------------------------------------------
// entaiExecute.c
//----------------------------------------------------------------

void aiInitMain(void);
void aiMain(Entity* e, AIVars* ai, EntType entType);

//----------------------------------------------------------------
// entaiPath.c
//----------------------------------------------------------------

void aiFindPathLimitedVolumes();
int aiCheckIfInPathLimitedVolume(const Vec3 pos);
int aiGetPathLimitedVolumeCount();

#endif