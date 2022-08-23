#include <utilitieslib/stdtypes.h>
#include "testTeam.h"
#include "testClientInclude.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/components/ArrayOld.h>
#include "entity/entity.h"
#include "clientcomm/clientcomm.h"
#include "player/player.h"
#include "gameData/randomName.h"
#include "gameData/randomCharCreate.h"
#include <utilitieslib/components/earray.h>
#include "UI/uiNet.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/mathutil.h>
#include "entity/Supergroup.h"
#include "testUtil.h"
#include "cmdparse/cmdcommon.h"
#include "UI/uiTurnstile.h"

#define commAddInput(s) printf("commAddInput(\"%s\");\n", s); commAddInput(s);


int wantToInvite(StashTable offer_type, Entity* e, const char* invite_type, const char* invitee_name, bool useHashTable, bool useSgMembers)
{
    int i;
    char buf[1024];
    bool invite = false;

    // maybe just get rid of useSgMembers entirely?
    if(useSgMembers)
    {
        bool found = false;
        for(i = 0; i < eaSize(&e->sgStats);i++)
        {
            if(!stricmp(invitee_name, e->sgStats[i]->name))
            {
                found = true;
                break;
            }
        }
        if(!found)
            invite = true;
    }
    else if ((!useHashTable && rand()%10==0) || (useHashTable && !stashFindInt(offer_type, invitee_name, NULL))) {
        invite = true;

        if (useHashTable) {
            stashAddInt(offer_type, invitee_name, 1, false);
        }
    }

    if(invite)
    {
        sprintf(buf, "%s %s", invite_type, invitee_name);
        commAddInput(buf);
    }

    return invite;
}

static StashTable htHts = 0;

void clearInviteHashTable(char* invite_type)
{
    StashTable htSentOffer;
    if (stashFindPointer(htHts, invite_type, &htSentOffer)) {
        stashTableClear(htSentOffer);
    }
}

typedef struct InviteData {
    Entity* e;
    char* invite_type;
    StashTable htSentOffer;
}InviteData;

int stashCheckInvite(void* data, StashElement element)
{
    InviteData* invite_data = data;
    const char* inviting = stashElementGetStringKey(element);
    wantToInvite(invite_data->htSentOffer, invite_data->e, invite_data->invite_type,
        stashElementGetStringKey(element), !!invite_data->htSentOffer, false);
    return 1;
}

void checkInvite(char *invite_type, int mode_to_check, int modeType, bool useHashTable, bool useSgMembers) {
    static DWORD firsttime=0;
    static U32 lastReset = 0;
    StashTable htSentOffer = {0};
    static char **hashTableList = NULL;
    int i;
    Entity* e = playerPtr();

    if (!htHts) {
        htHts = stashTableCreateWithStringKeys(4, StashDeepCopyKeys);
    }
    if (useHashTable) {
        if (!stashFindPointer(htHts, invite_type, &htSentOffer)) {
            char *hashTableName = strdup(invite_type);
            htSentOffer = stashTableCreateWithStringKeys(128, StashDeepCopyKeys);
            stashAddPointer(htHts, invite_type, htSentOffer, false);
            lastReset = ABS_TIME;
            eaPush(&hashTableList, hashTableName);
        }
        
        if (ABS_TIME_SINCE(lastReset) > SEC_TO_ABS(30))
        {
            int i;
            for (i = 0; i < eaSize(&hashTableList); ++i)
            {
                clearInviteHashTable(hashTableList[i]);
            }
            lastReset = ABS_TIME;
        }
    }
    if (!firsttime) {
        firsttime = timeGetTime();
    }
    if (timeGetTime() < firsttime + 2000)
        return;
    switch (modeType)
    {
    case 2:
        if (!(g_testMode2 & mode_to_check))
            return;
        break;
    case 0:                    //    intentional fall through
    case 1:
    default:
        if (!(g_testMode & mode_to_check))
            return;
    }

    // check all nearby players
    for (i=1; i<entities_max; i++) {
        if (!(entity_state[i] & ENTITY_IN_USE ) )
            continue;
        if ( ENTTYPE_BY_ID(i) != ENTTYPE_PLAYER )
            continue;

        if(!wantToInvite(htSentOffer, e, invite_type, entities[i]->name, useHashTable, useSgMembers) && !useHashTable)
            break;
    }

    if(playerNames)
    {
        InviteData data;

        data.e = e;
        data.htSentOffer = htSentOffer;
        data.invite_type = invite_type;
        stashForEachElementEx(playerNames, stashCheckInvite, &data);
    }
}

void checkTeamUp(void) {
    checkInvite("i", TEST_TEAMINVITE, 0, true, false);
    {
        static int countdown = 10;
        if (0>=countdown--) {
            countdown = 10;
            checkInvite("i", TEST_TEAMTHRASH, 0, false, false);
            checkInvite("kick", TEST_TEAMTHRASH, 0, false, false);
            if (rand()%20==0 && (g_testMode & TEST_TEAMTHRASH)) {
                commAddInput("leaveTeam");
            }
        }
    }
}

void checkTeamUpDialog(char *txt) 
{
    if (stricmp(txt, "OfferTeamup")==0) 
    {
        if (g_testMode & TEST_TEAMACCEPT) {
            sendTeamAccept(0);
        } else {
            sendTeamDecline(0);
        }
    }
}


void checkSuperGroupDialog(char *txt) {
    if (stricmp(txt, "OfferSuperGroup")==0 && (g_testMode & TEST_SUPERGROUPACCEPT)) {
        char buf[1024];
        extern char offererName[64];
        extern int teamOfferingDbId;
        sprintf( buf, "sg_accept \"%s\" %i %i \"%s\"", offererName, teamOfferingDbId, playerPtr()->db_id, playerPtr()->name );
        commAddInput(buf);
        // clear this out so someone else can offer
        teamOfferingDbId = 0;
    }
}

static char *test_actions[] = {
    "promote",
    "demote",
    "sgkick",
    "sginvite",
    "sg_accept TEST 1 2", // just to throw wrenches into the system
    "sg_decline TEST 1", 
    "sgkick",
    "nameLeader",
    "nameLieutenant",
    "nameCaptain",
    "nameCommander",
    "nameMember",
    "sgleave",
    "sgstats",
};

void checkSuperGroupMisc() {
    // Choose a target in the supergroup
    Entity *e = playerPtr();
    int target;
    char *sTarget;
    int action;
    char *sAction;
    char buf[1024];
    
    if (!e || !e->supergroup || !e->sgStats)
        return;
    target = randInt(eaSize(&e->sgStats));
    sTarget = e->sgStats[target]->name;

    action = randInt2(ARRAY_SIZE(test_actions));
    sAction = test_actions[action];
    if (strcmp(sAction, "sgleave")==0 ||
        strcmp(sAction, "sgstats")==0)
    {
        sTarget = "";
    }

    sprintf(buf, "%s %s%s%s", sAction, sTarget[0] ? "\"" : "", sTarget, sTarget[0] ? "\"" : "");
    commAddInput(buf);
}

void checkSuperGroup() {
    if (playerPtr())
    {
        if (playerPtr()->supergroup) {
            // Invite people
            checkInvite("sginvite", TEST_SUPERGROUPINVITE, 0, true, false);
            if (g_testMode & TEST_SUPERGROUPMISC) {
                checkSuperGroupMisc();
            }
            if (g_testMode & TEST_SUPERGROUPLEAVE) {
                if(!(rand()%30)) {
                    commAddInput("sgleave");
                    clearInviteHashTable("sginvite");
                }
            }
        } else {
            if (g_testMode & TEST_SUPERGROUPINVITE) {
                // Create a superGroup
                //sprintf(buf, "sgcreate %s", randomName());
                //commAddInput(buf);
                Supergroup sg;
                strncpyt(sg.name, randomName(), ARRAY_SIZE(sg.name));
                strncpyt(sg.description, randomName(), ARRAY_SIZE(sg.description));
                strncpyt(sg.motto, randomName(), ARRAY_SIZE(sg.motto));
                strncpyt(sg.msg, randomName(), ARRAY_SIZE(sg.msg));
                strncpyt(sg.rankList[0].name, randomName(), ARRAY_SIZE(sg.rankList[0].name));
                strncpyt(sg.rankList[1].name, randomName(), ARRAY_SIZE(sg.rankList[1].name));
                strncpyt(sg.rankList[2].name, randomName(), ARRAY_SIZE(sg.rankList[2].name));
                strncpyt(sg.rankList[3].name, randomName(), ARRAY_SIZE(sg.rankList[3].name));
                strncpyt(sg.rankList[4].name, randomName(), ARRAY_SIZE(sg.rankList[4].name));
                sg.rankList[0].permissionInt[0] = rand();
                sg.rankList[1].permissionInt[0] = rand();
                sg.rankList[2].permissionInt[0] = rand();
                sg.rankList[3].permissionInt[0] = rand();
                sg.rankList[4].permissionInt[0] = rand();
                strncpyt(sg.emblem, "Atom_02", ARRAY_SIZE(sg.emblem));
                sgroup_sendCreate(&sg );
            }
        }
    }
}

void checkLeague(void) {
    checkInvite("li", TEST2_LEAGUE_INVITE, 2, true, false);
    {
        static int countdown = 10;
        if (0>=countdown--) {
            countdown = 10;
            checkInvite("li", TEST2_LEAGUE_THRASH, 2, false, false);
            checkInvite("league_kick", TEST2_LEAGUE_THRASH, 2, false, false);
            if (rand()%20==0 && (g_testMode2 & TEST2_LEAGUE_THRASH)) {
                commAddInput("leaveTeam");
            }
        }
    }
}

void checkLeagueDialog(char *txt) 
{
    if (stricmp(txt, "OfferLeagueup")==0) 
    {
        if (g_testMode2 & TEST2_LEAGUE_ACCEPT) {
            sendLeagueAccept(0);
        } else {
            sendLeagueDecline(0);
        }
    }
}

void checkTurnstileJoin(void) {
    if (g_testMode2 & TEST2_TURNSTILE_JOIN)
    {
        static unsigned int lastTime = 0;
        if (timerSecondsSince2000() > (lastTime + 15)) {
            int eventCount = 1;
            int eventList[1] = {-1};
            lastTime = timerSecondsSince2000();
            turnstileGame_generateQueueForEvents(eventList, eventCount);
        }
    }
}
