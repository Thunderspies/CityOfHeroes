// ZONE SCRIPT
//

// Runs on supergroup bases during a raid - provides overall game control, and UI for the raid

#include "script/scriptutil.h"
#include <utilitieslib/utils/timing.h>

#define DISRUPTER_PYLON_POWER "Auto Pets.Raid_Disruptor_Pylon_Create.Create"
#define WRAPUP_LENGTH    1.0        // in minutes
#define ATTACKER_CLOCK    5.0        // in minutes

void CreateForceFields(void);
void DestroyForceFields(void);
NUMBER ForceFieldHealth(void);
NUMBER NumActiveAnchors(void);
int AttackerCount(void);
void RaidComplete(int attackerswin, const char* why, const char * nameOfItemOfPowerCaptured, int creationTimeOfItemOfPowerCaptured );
void RaidStop(void);
int RaidTimeRemaining(void);
int RaidPastEntryPhase(void);

static int IsPylon(ENTITY ent)
{
    return StringEqual(EntityPowerCreatedMe(ent), DISRUPTER_PYLON_POWER);
}

static void DestroyDisruptors(void)
{
    int i, count;
    count = NumEntitiesInTeam(ALL_CRITTERS);
    for (i = count; i >= 1; i--)
    {
        if (IsPylon(GetEntityFromTeam(ALL_CRITTERS, i)))
            Kill(GetEntityFromTeam(ALL_CRITTERS, i), 0);
    }
}

static void UpdateUI(void)
{
    int i, count;
    int UI = VarGetNumber("UI");
    int disruptors = 0;

    count = NumEntitiesInTeam(ALL_CRITTERS);
    for (i = 1; i <= count; i++)
    {
        if (IsPylon(GetEntityFromTeam(ALL_CRITTERS, i)))
            disruptors++;
    }
    VarSetNumber("DisruptorsRemaining", disruptors);
    VarSetNumber("StabilizersRemaining", NumActiveAnchors());
    VarSetNumber("ForceFields", ForceFieldHealth());

    if (disruptors >= 5)
    {
        VarSet("Why", "5 disruptors built");
        SET_STATE("AttackersWin");
    }
    if (NumActiveAnchors() <= 1)
    {
        VarSet("Why", "No anchors remain");
        SET_STATE("AttackersWin");
    }
}

static void End(int attackerswin)
{
    DestroyForceFields();
    DestroyDisruptors();
    RaidComplete(attackerswin, VarGet("Why"), VarGet("NameOfItemOfPowerCaptured"), VarGetNumber( "CreationTimeOfItemOfPowerCaptured"  ) );
    SET_STATE("WrapupTime");
    VarSet("Title", StringLocalizeStatic(attackerswin?"BaseRaidAttackersWin":"BaseRaidDefendersWin"));
}

void BaseRaid(void)
{
    INITIAL_STATE
        ON_ENTRY 
            TimerSet("AttackerClock", ATTACKER_CLOCK);
            TimerSet("Clock", (F32)RaidTimeRemaining()/60.0);
            VarSetNumber("RaidEndTime", RaidTimeRemaining() + timerSecondsSince2000());
            VarSet("Title", StringLocalizeStatic("BaseRaidTitle"));
            ScriptUITitle(ONENTER("TitleUI"), "Title", StringLocalizeStatic("BaseRaidDesc"));
            ScriptUITimer(ONENTER("TimerUI"), "RaidEndTime", StringLocalizeStatic("BaseRaidTimer"), StringLocalizeStatic("BaseRaidTimerDesc"));
            ScriptUIMeter(ONENTER("DisruptorUI"), "DisruptorsRemaining", StringLocalizeStatic("BaseRaidDisruptors"), "1", "0", "5", "dd0000ff", "2255ffff",
                StringLocalizeStatic("BaseRaidDisruptorsDesc"));
            ScriptUIMeter(ONENTER("StabilizerUI"), "StabilizersRemaining", StringLocalizeStatic("BaseRaidStabilizers"), "1", "1", "6", "2255ffff", "dd0000ff",
                StringLocalizeStatic("BaseRaidStabilizersDesc"));
            ScriptUIMeter(ONENTER("TrophyUI"), "ForceFields", StringLocalizeStatic("BaseRaidForceField"), "1", "0", "100", "2255ffff", "dd0000ff",
                StringLocalizeStatic("BaseRaidForceFieldDesc"));
            CreateForceFields();
        END_ENTRY

        UpdateUI();
        if (TimerElapsed("Clock"))
        {
            VarSet("Why", "Time ran out");
            SET_STATE("DefendersWin");
        }

        if (AttackerCount() > 0)
            TimerSet("AttackerClock", ATTACKER_CLOCK);
        else if (TimerElapsed("AttackerClock"))
        {
            VarSet("Why", "No attackers on map");
            SET_STATE("DefendersWin");
        }

    STATE("AttackersWin")
        BroadcastAttentionMessage(StringLocalizeStatic("BaseRaidAttackersWin"), NULL, 0.0);
        End(1);

    STATE("DefendersWin")
        BroadcastAttentionMessage(StringLocalizeStatic("BaseRaidDefendersWin"), NULL, 0.0);
        End(0);

    STATE("WrapupTime")
        ON_ENTRY
            TimerSet("Clock", WRAPUP_LENGTH);
        END_ENTRY

        if (TimerElapsed("Clock"))
        {
            RaidStop();
            EndScript();
        }
    END_STATES

    ON_SIGNAL("ItemCaptured")
        VarSet("Why", "Item of Power captured");
        SET_STATE("AttackersWin")
    END_SIGNAL

    ON_SIGNAL("AttackersWin")
        SET_STATE("AttackersWin")
    END_SIGNAL

    ON_SIGNAL("DefendersWin")
        SET_STATE("DefendersWin")
    END_SIGNAL

    ON_STOPSIGNAL
        End(0);
        EndScript();
    END_SIGNAL
}

void BaseRaidInit()
{
    SetScriptName( "BaseRaid" );
    SetScriptFunc( BaseRaid );
    SetScriptType( SCRIPT_ZONE );        

    SetScriptVar( "BaseRaidTitle",                    "BaseRaidTitle",            SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidDesc",                    "BaseRaidDesc",                SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidTimer",                    "BaseRaidTimer",            SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidTimerDesc",                "BaseRaidTimerDesc",        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidDisruptors",                "BaseRaidDisruptors",        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidDisruptorsDesc",            "BaseRaidDisruptorsDesc",    SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidStabilizers",            "BaseRaidStabilizers",        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidStabilizersDesc",        "BaseRaidStabilizersDesc",    SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidForceField",                "BaseRaidForceField",        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidForceFieldDesc",            "BaseRaidForceFieldDesc",    SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidAttackersWin",            "BaseRaidAttackersWin",        SP_DONTDISPLAYDEBUG );
    SetScriptVar( "BaseRaidDefendersWin",            "BaseRaidDefendersWin",        SP_DONTDISPLAYDEBUG );

}

// ************************************************************************************
// hooks that are specific to base raiding - better to group them here

#include "bases/bases.h"
#include "bases/basesystems.h"
#include "bases/basedata.h"
#include <utilitieslib/components/earray.h>
#include "gameComm/VillainDef.h"
#include "gameSys/sgraid.h"
#include <utilitieslib/utils/error.h>
#include "ai/entai.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include "script/scriptengine.h"
#include "gameComm/dooranimcommon.h"
#include "svr/svr_player.h"
#include "gameSys/raidmapserver.h"

NUMBER NumActiveAnchors(void)
{
    return baseCountActiveAnchors(&g_base);
}

// keeping track of our force fields in some static vars here
static RoomDetailPosition** positions;
static EntityRef** forcefields;

void ItemCaptured(STRING name, int creation_time)
{
    char buf[100];
    //Tell script which Item of Power was Captured
    ScriptEnvironment* se = ScriptFromRunId(ScriptRunIdFromName("BaseRaid", SCRIPT_ZONE));
    SetEnvironmentVar(se, "NameOfItemOfPowerCaptured", name);
    SetEnvironmentVar(se, "CreationTimeOfItemOfPowerCaptured", itoa( creation_time, buf, 10) );
    //Signal Item Captured
    ScriptSendSignal(ScriptRunIdFromName("BaseRaid", SCRIPT_ZONE), "ItemCaptured");
}

void RaidItemInteract(Entity* player, RoomDetail* det)
{
    BaseRoom* room = baseGetRoom(&g_base, det->parentRoom);
    Vec3 pos;
    int i, found = 0;

    if (!room)
    {
        Errorf("Couldn't get room in RaidItemInteract");
        return;
    }

    // check distance
    addVec3(room->pos, det->mat[3], pos);
    if (distance3Squared(pos, ENTPOS(player)) > SERVER_INTERACT_DISTANCE*SERVER_INTERACT_DISTANCE)
        return;

    // check force field
    for (i = 0; i < eaSize(&forcefields); i++)
    {
        Entity* ff = erGetEnt(*forcefields[i]);
        if (ff && nearSameVec3(ENTPOS(ff), pos) && ff->pchar->attrCur.fHitPoints > 0)
            found = 1;
    }

    if (!found)
    {
        ItemCaptured(det->info->pchName, det->creation_time);
    }
}

void CreateForceFields(void)
{
    int i;

    positions = baseGetItemsOfPower(&g_base);
    if (!positions) return;
    eaCreate(&forcefields);
    for (i = 0; i < eaSize(&positions); i++)
    {
        EntityRef* ref = calloc(sizeof(EntityRef), 1);
        Entity* ff = villainCreateByName("Objects_Raid_Force_Field", BASE_FORCED_LEVEL, NULL, 0, NULL, 0, NULL);
        if (ff)
        {
            entUpdatePosInstantaneous(ff, positions[i]->pos);
            aiInit(ff, NULL);
            aiPriorityQueuePriorityList(ff, "PL_BeGoodAndStill", 0, NULL);
            *ref = erGetRef(ff);
            eaPush(&forcefields, ref);
        }
        else
            Errorf("Base raid couldn't create force fields?");
    }
}

void DestroyForceFields(void)
{
    int i;

    for (i = 0; i < eaSize(&forcefields); i++)
    {
        Entity* ff = erGetEnt(*forcefields[i]);
        if (ff)
            entFree(ff);
    }
    eaDestroyEx(&forcefields, NULL);
    forcefields = 0;
    positions = 0;
}

// returns lowest force field
NUMBER ForceFieldHealth(void)
{
    int i;
    int min = 100;

    for (i = 0; i < eaSize(&forcefields); i++)
    {
        Entity* ff = erGetEnt(*forcefields[i]);
        if (ff)
        {
            int percent = 100 * ff->pchar->attrCur.fHitPoints / ff->pchar->attrMax.fHitPoints;
            if (percent < min)
                min = percent;
        }
        else
            min = 0;
    }
    return min;
}

// how many persons of another supergroup are on the map right now
static PlayerEnts* activeplayers = &player_ents[PLAYER_ENTS_ACTIVE];
int AttackerCount(void)
{
    int i, count = 0;
    U32 sgid = SGBaseId();
    for (i = 0; i < activeplayers->count; i++)
    {
        Entity* e = activeplayers->ents[i];
        if (e->supergroup_id && e->supergroup_id != sgid)
            count++;
    }
    return count;
}

