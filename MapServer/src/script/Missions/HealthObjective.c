
//////////////////////////////////////////////////////////////////////////
//
// MISSION SCRIPT
// Encounter Script
//
//
//
//////////////////////////////////////////////////////////////////////////
//
// HealthObjective
// A HealthObjective Boss is very much like a ChattyBoss.
// The differences are;
// HealthObjective allows you to set the exact health at which the things happen,
// not just 75%, 50%, and 25%
// In addition, the health objective can have a required objective be finished first
// HealthObjective has a RANDOMIZE_MIDDLE flag, which if set makes the middle
// Designers can also specify an objective as fixed so that it doesn't get randomized
// points of dialog/objective happen in a random order.
// HealthObject allows up to 5 mid points.
// HealthObject Has a teleport option, and a wait_for option,
// which allows the boss to teleport away and wait for something to happen.
// Health objective has objective complete ai which will cause the ai to use a specific ai upon return

// 
// Finer points;
//  if a mid point's health isn't specified, then it's determined by
//  interpolating the values inbetween.
//  For example, if only two pieces of dialog are specified; 1ST_DIALOG and 2ND_DIALOG,
//  then they happen at 66% and 33% health.
//   if 1st is speced at 50%, 2nd would happen at 25%
//
// 

#include "script/scriptutil.h"
#include "script/scriptengine.h"
#include "entity/entity.h"
#include "storyarc/mission.h"
#include "script/Scripthook/ScriptHookInternal.h"

#define MAX_MID_POINTS 5
#define MAX_POINTS (MAX_MID_POINTS+2)
#define END_POINT (MAX_MID_POINTS+1)
static struct health_name {
    char *health;
    char *prereq_obj;
    char *is_fixed;
    char *dialog;
    char *objective;
    char *port_to;
    char *do_ai;
    char *wait_for;
    char *obj_complete_ai;
    char *done;
} health_names[MAX_POINTS] = {
    {"",                    "",                    "",                    "BOSS_1STBLOOD_DIALOG",    "",                        "",                    "",                "",                "",                        "DONE_1STBLOOD"},
    {"BOSS_1ST_HEALTH",        "1ST_PREREQ_OBJ",    "1ST_IS_FIXED",        "BOSS_1ST_DIALOG",        "BOSS_1ST_Objective",    "1ST_TELEPORT_TO",    "1ST_DO_AI",    "1ST_WAIT_FOR",    "1ST_OBJ_COMPLETE_AI",    "DONE_1ST"},
    {"BOSS_2ND_HEALTH",        "2ND_PREREQ_OBJ",    "2ND_IS_FIXED",        "BOSS_2ND_DIALOG",        "BOSS_2ND_Objective",    "2ND_TELEPORT_TO",    "2ND_DO_AI",    "2ND_WAIT_FOR",    "2ND_OBJ_COMPLETE_AI",    "DONE_2ND"},
    {"BOSS_3RD_HEALTH",        "3RD_PREREQ_OBJ",    "3RD_IS_FIXED",        "BOSS_3RD_DIALOG",        "BOSS_3RD_Objective",    "3RD_TELEPORT_TO",    "3RD_DO_AI",    "3RD_WAIT_FOR",    "3RD_OBJ_COMPLETE_AI",    "DONE_3RD"},
    {"BOSS_4TH_HEALTH",        "4TH_PREREQ_OBJ",    "4TH_IS_FIXED",        "BOSS_4TH_DIALOG",        "BOSS_4TH_Objective",    "4TH_TELEPORT_TO",    "4TH_DO_AI",    "4TH_WAIT_FOR",    "4TH_OBJ_COMPLETE_AI",    "DONE_4TH"},
    {"BOSS_5TH_HEALTH",        "5TH_PREREQ_OBJ",    "5TH_IS_FIXED",        "BOSS_5TH_DIALOG",        "BOSS_5TH_Objective",    "5TH_TELEPORT_TO",    "5TH_DO_AI",    "5TH_WAIT_FOR",    "5TH_OBJ_COMPLETE_AI",    "DONE_5TH"},
    {"",                    "",                    "",                    "BOSS_DEAD_DIALOG",        "BOSS_DEAD_Objective",    "",                    "",                "",                "",                        "DONE_DEAD"}
};

static char WAIT_FOR_IT[] = "WAIT_FOR_IT";
static char PORT_LOC[] = "PORT_LOC";
static char DO_AI[] = "DO_AI";
static char OBJ_COMPLETE_AI[] = "OBJ_COMPLETE_AI";


static int find_next_health(int mid_point)
{
    for (;;)
    {
        mid_point++;
        if (mid_point == END_POINT) 
            break;
        if (VarGetFraction(health_names[mid_point].health) > 0.0)
        {
            return mid_point;
        }
    }
    return END_POINT;
}

static int find_last_point(void)
{
    int j;

    for (j=MAX_MID_POINTS;j>0;j--)
    {
        if ( !VarIsEmpty(health_names[j].health) || 
             !VarIsEmpty(health_names[j].dialog) ||
             !VarIsEmpty(health_names[j].objective) ||
             !VarIsEmpty(health_names[j].port_to) ||
             !VarIsEmpty(health_names[j].do_ai) ||
             !VarIsEmpty(health_names[j].wait_for) ||
             !VarIsEmpty(health_names[j].obj_complete_ai))
        {
            return j;
        }
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////
// Helper function.
//

static void do_mid_point(int n, ENTITY boss)
{
    const char *obj;
    const char *order;
    const char *port_to;
    const char *wait_for;
    const char *do_ai;
    const char *obj_complete_ai;
    int nn;

    // The order might have been randomized.
    nn = n;
    order = VarGet("BOSS_HEALTH_ORDER");
    if (strlen(order)>MAX_MID_POINTS) {
        nn = order[n]-'0';
        if (nn < 0)
            nn=0;
        if (nn > MAX_MID_POINTS)
            nn=MAX_MID_POINTS;
    }

    //Say won't do anything if passed ""
    Say( boss, VarGet(health_names[nn].dialog), 2 );

    // complete objective if specified
    obj = VarGet(health_names[nn].objective);
    if (obj[0])
    {
        if (OnMissionMap())
        {
            SetMissionObjectiveComplete(OBJECTIVE_SUCCESS, obj);
        }
        else
        {
            SetInstanceVar(obj, NumberToString(ZONE_OBJECTIVE_SUCCESS));
        }
    }

    // teleport away if specified
    port_to = VarGet(health_names[nn].port_to);
    if (port_to[0])
    {
        //save current location for later return.
        VarSet(PORT_LOC, EntityPos(boss));
        // And away we go.
        SetPosition(boss, StringAdd("marker:", port_to));
    }

    //Sit and do nothing, if wait for is specified.
    wait_for = VarGet(health_names[nn].wait_for);
    if (wait_for[0])
    {
        VarSet(WAIT_FOR_IT, wait_for);

    }

    //Do something, if told to.
    do_ai = VarGet(health_names[nn].do_ai);
    if (do_ai[0])
    {
        AddAIBehavior(boss, do_ai);
        VarSet(DO_AI, do_ai);
    }

    obj_complete_ai = VarGet(health_names[nn].obj_complete_ai);
    if (obj_complete_ai[0])
    {
        VarSet(OBJ_COMPLETE_AI, obj_complete_ai);
    }

}

static int objectiveMatchesStatus(const char *obj_name, int status )
{
    if (OnMissionMap())
    {
        return MissionObjectiveIsComplete(obj_name, status);
    }
    else
    {
        const char *currentStatus = GetInstanceVar(obj_name);
        // This allows the status parameter as a true/false to select between completion with success or failure.
        // We can't use the value directly, since status == 0 => failure, but an instance var with a value of zero is
        // not yet set.
        int desiredStatus = status ? ZONE_OBJECTIVE_SUCCESS : ZONE_OBJECTIVE_FAIL;
        return (desiredStatus == StringToNumber(currentStatus));
    }
}

void HealthObjective(void)
{  
    char cbuff[512];

    INITIAL_STATE 
    {
        //////////////////////////////////////////////////////////////////////////
        ON_ENTRY
        {
            ENTITY boss;
            int i, j;
            char c;
            FRACTION health_amount;
            FRACTION next_health_amount;
            FRACTION delta;
            int mid_point;
            int next_mid_point;
            int fill_point;
            int num_mid_points;
            int n;

            // First, count up how many midpoints there are.
            num_mid_points = find_last_point();

            // set mid_points defaults for those not specified.
            // pick an even spread of values between each two specified values.
            health_amount = 1.0;    //Start at full health
            mid_point = 0;

            while(mid_point < END_POINT)
            {
                next_mid_point = find_next_health(mid_point);
                if (next_mid_point == END_POINT)    //no more defined
                {
                    fill_point = num_mid_points+1; //fill to end.
                    next_health_amount = 0.0;
                } else {
                    fill_point = next_mid_point;
                    next_health_amount = VarGetFraction(health_names[fill_point].health);
                }

                // Set defaults between two defined values
                n = (fill_point - mid_point);    //n = number of points between those + 1
                if (n>0)
                    delta = (health_amount-next_health_amount)/n;
                for(j=mid_point+1; j<fill_point; j++)
                {
                    health_amount -= delta;
                    VarSetFraction(health_names[j].health, health_amount);
                }
                mid_point = next_mid_point;
                health_amount = next_health_amount;
            }
            //-- 

            boss = ActorFromEncounterActorName(MyEncounter(), VarGet( "Actor" ) );

            if( !boss || StringEqual(boss, TEAM_NONE))
                EndScript();

            SayOnKillHero(boss, VarGet("BOSS_KILLEDHERO_DIALOG") );

            VarSet( "HealthObjectiveBoss", boss );
            VarSetFraction( "LastHealth", GetHealth( boss ) );

            strcpy(cbuff, "0123456");

            if ( VarIsTrue("RANDOMIZE_MIDDLE") )
            {
                //swap middle around
                for (i=1; i<=num_mid_points; i++)
                {
                    j = (rand()%num_mid_points)+1;
                    if (VarIsTrue(health_names[i].is_fixed) || VarIsTrue(health_names[j].is_fixed)    )        //    If this position is fixed, skip it
                    {                                                                                        //    this isn't very random, but hopefully it's good enough
                        continue;
                    }
                    c = cbuff[i];
                    cbuff[i]=cbuff[j];
                    cbuff[j]=c;
                }
            }

            VarSet("BOSS_HEALTH_ORDER", cbuff);

        }
        END_ENTRY

        {
            ENTITY boss;
            FRACTION health;
            FRACTION lastHealth;
            FRACTION tp;
            int j;
            const char *wait_for;
            const char *do_ai;
            int stopWaiting = 0;

            boss = VarGet( "HealthObjectiveBoss" );

            wait_for = VarGet(WAIT_FOR_IT);


            if (wait_for[0])
            {
                if (objectiveMatchesStatus(wait_for, 1))
                {
                    const char *obj_complete_ai;
                    const char *pl;
                    //Once complete, clear flags.
                    VarSet(WAIT_FOR_IT, "");

                    do_ai = VarGet(DO_AI);
                    if (do_ai[0])
                    {
                        RemoveAllBehaviors(boss);
                    }
                    //if port_to was set, then go back there
                    pl = VarGet(PORT_LOC);
                    if (pl[0])
                    {
                        SetPosition(boss, pl);
                        VarSet(PORT_LOC, "");
                    }

                    obj_complete_ai = VarGet(OBJ_COMPLETE_AI);
                    if (obj_complete_ai[0])
                    {
                        AddAIBehavior(boss, obj_complete_ai);
                    }
                }
            }
            else
            {
                health = GetHealth( boss );
                lastHealth = VarGetFraction( "LastHealth" );

                if (( lastHealth >= 1.00 && 1.00 > health ) && !VarGetNumber( "DONE_1STBLOOD" ) )
                {
                    do_mid_point(0, boss);
                    VarSetNumber( "DONE_1STBLOOD", 1 ); 
                }

                for (j=1; (health > 0.0f) && (j<=MAX_MID_POINTS); j++)
                {
                    tp = VarGetFraction(health_names[j].health);
                    if ((tp > 0.0) && (health < tp) && !VarGetNumber( health_names[j].done ) && VarGetNumber(health_names[j-1].done) )
                    {
                        const char *prerequisite = VarGet(health_names[j].prereq_obj);
                        if (prerequisite[0] && !objectiveMatchesStatus(prerequisite, 1))
                        {
                            continue;
                        }
                        else
                        {
                            do_mid_point(j, boss);
                            VarSetNumber( health_names[j].done, 1 );
                            break;                                                //    don't trigger multiple in one tick
                        }
                    }
                }

                if( lastHealth > 0.00 && 0.00 >= health )//&& !VarGetNumber( "DoneDead" ) )
                {
                    Say( boss, VarGet( "BOSS_DEAD_DIALOG" ), 2 ); 
                    VarSetNumber( "DoneDead", 1 ); 
                }

                VarSetFraction( "LastHealth", health );
            }
        }
    }
    END_STATES 
}

void HealthObjectiveInit()
{
    int j;

    SetScriptName( "HealthObjective" );     
    SetScriptFunc( HealthObjective );
    SetScriptType( SCRIPT_ENCOUNTER );        

    SetScriptVar( "Actor",                                "",        0 );

    SetScriptVar( "BOSS_1STBLOOD_DIALOG",                "",        SP_MULTIVALUE | SP_OPTIONAL ); 

    for (j=1; j<=MAX_MID_POINTS; j++)
    {
        SetScriptVar( health_names[j].health,        "",        SP_OPTIONAL ); 
        SetScriptVar( health_names[j].prereq_obj,        "",        SP_OPTIONAL ); 
        SetScriptVar( health_names[j].is_fixed,        "",        SP_OPTIONAL ); 
        SetScriptVar( health_names[j].dialog,        "",        SP_MULTIVALUE | SP_OPTIONAL ); 
        SetScriptVar( health_names[j].objective,    "",        SP_OPTIONAL ); 
        SetScriptVar( health_names[j].wait_for,        "",        SP_OPTIONAL ); 
        SetScriptVar( health_names[j].do_ai,        "",        SP_OPTIONAL ); 
        SetScriptVar( health_names[j].port_to,        "",        SP_OPTIONAL ); 
        SetScriptVar( health_names[j].obj_complete_ai,    "",        SP_OPTIONAL ); 
    }

    SetScriptVar( "BOSS_DEAD_DIALOG",                    "",        SP_MULTIVALUE | SP_OPTIONAL ); 

    SetScriptVar( "BOSS_KILLEDHERO_DIALOG",                "",        SP_MULTIVALUE | SP_OPTIONAL );
    SetScriptVar( "RANDOMIZE_MIDDLE",                    "",        SP_OPTIONAL );
}
