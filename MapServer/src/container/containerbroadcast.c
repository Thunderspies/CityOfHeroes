#include <utilitieslib/stdtypes.h>
#include "containerbroadcast.h"
#include "containerEmail.h"
#include "string.h"
#include "entity/entserver.h"
#include "gameComm/sendToClient.h"
#include "entity/entVarUpdate.h"
#include "comm_game.h"
#include "dbcomm/dbcontainer.h"
#include "team.h"
#include "league.h"
#include "svr/svr_player.h"
#include "storyarc/storyarcprivate.h"
#include "entity/entGameActions.h"
#include "storyarc/TeamTask.h"
#include "storyarc/contact.h"
#include "language/langServerUtil.h"
#include "cmdparse/cmdserver.h"
#include "cmdparse/cmdchat.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "entity/SgrpServer.h"
#include "entity/TaskforceParams.h"
#include "storyarc/taskforce.h"
#include "player/badges_server.h"
#include "storyarc/playerCreatedStoryarcServer.h"
#include "storyarc/task.h"
#include "entity/character_combat.h"

//
// String commands are sent in the form of "Command:rest of data".
//
void dealWithBroadcast(int container,int *members,int count,char *msg,int msg_type,int senderID,int container_type)
{
    char    *msg_text;
    int        i,len;
    Entity    *e;
    char    tmp[512];

    msg_text = strchr(msg,':');
    assert(msg_text);
    msg_text++;
    len = msg_text - msg;

    if (strnicmp(msg,DBMSG_EMAIL_MSG,len)==0)
    {
        for(i=0; i<count; i++ )
        {
            e = entFromDbIdEvenSleeping(members[i]);
            if (e)
                emailSendNewHeader( e->db_id, msg_text, senderID );
        }
    }
    if (strnicmp(msg,DBMSG_MAPLIST_REFRESH,len)==0)
    {
        for(i=0; i<count; i++ )
        {
            e = entFromDbIdEvenSleeping(members[i]);
            if (e)
            {
                teamUpdateMaplist( e, msg_text, senderID );
                leagueUpdateMaplist( e, msg_text, senderID );
            }
        }
    }
    if (strnicmp(msg,DBMSG_CHAT_MSG,len)==0)
    {
        for(i=0; i<count; i++ )
        {
            e = entFromDbIdEvenSleeping(members[i]);
            if (e)
                sendChatMsg( e, msg_text, msg_type, senderID );
        }
    }
    if (strnicmp(msg,DBMSG_HERO_CHAT_MSG,len)==0)
    {
        for(i=0; i<count; i++ )
        {
            e = entFromDbIdEvenSleeping(members[i]);
            if (e)
                sendChatMsgEx( e, 0, msg_text, msg_type, DEFAULT_BUBBLE_DURATION, senderID, senderID, ENT_IS_VILLAIN(e), 0 );
        }
    }
    if (strnicmp(msg,DBMSG_VILLAIN_CHAT_MSG,len)==0)
    {
        for(i=0; i<count; i++ )
        {
            e = entFromDbIdEvenSleeping(members[i]);
            if (e)
                sendChatMsgEx( e, 0, msg_text, msg_type, DEFAULT_BUBBLE_DURATION, senderID, senderID, !ENT_IS_VILLAIN(e), 0 );
        }
    }
    else if (strnicmp(msg,DBMSG_ALLIANCECHAT_MSG,len)==0)
    {
        int sender_sg_id = 0, sender_sg_rank = 0, filtered = 0, j;

        // extract sender sg dbid and sg rank
        {
            char *msg_text2;
            msg_text2 = strchr(msg_text,':');
            if (msg_text2)
            {
                *msg_text2 = 0;
                sender_sg_id = atoi(msg_text);
                msg_text = msg_text2 + 1;

                msg_text2 = strchr(msg_text,':');
                if (msg_text2)
                {
                    *msg_text2 = 0;
                    sender_sg_rank = atoi(msg_text);
                    msg_text = msg_text2 + 1;
                }
            }
        }

        for(i=0; i<count; i++ )
        {
            e = entFromDbIdEvenSleeping(members[i]);
            if (e)
            {
                filtered = 0;
                if (e->supergroup_id)
                {
                    for (j = 0; j < MAX_SUPERGROUP_ALLIES; j++)
                    {
                        if (e->supergroup->allies[j].db_id == 0)
                            break;
                        if ((e->supergroup->allies[j].db_id == sender_sg_id) && (sender_sg_rank < e->supergroup->allies[j].minDisplayRank))
                        {
                            filtered = 1;
                            break;
                        }
                    }
                }
                if (!filtered)
                    sendChatMsg( e, msg_text, msg_type, senderID );
            }
        }
    }
    else if (strnicmp(msg,DBMSG_STORYARC_TASK_REFRESH,len)==0)
    {
        for(i=0; i<count; i++ )
        {
            e = entFromDbId(members[i]);
            if (e)
                StoryArcSendTaskRefresh(e);
        }
    }
    else if (strnicmp(msg,DBMSG_STORYARC_FULL_REFRESH,len)==0)
    {
        for(i=0; i<count; i++ )
        {
            e = entFromDbId(members[i]);
            if (e)
            {
                StoryArcSendFullRefresh(e);
            }
        }
    }
    else if (strnicmp(msg,DBMSG_MISSION_COMPLETE,len)==0)
    {
        int owner;
        StoryTaskHandle sahandle;

        for(i=0; i<count; i++ )
        {
            e = entFromDbId(members[i]);
            if (e)
            {
                serveFloater(e, e, "FloatMissionComplete", 0.0f, kFloaterStyle_Attention, 0);
                TaskForceMissionCompleteDesaturate(e);
            }
        }

        e = entFromDbIdEvenSleeping(members[0]);  // only need a single member to deal with this message
        if (e && e->teamup && // verify this team is still attached to the mission that sent the message
            MissionDeconstructIdString(msg_text, &owner, &sahandle) &&
            e->teamup->activetask->assignedDbId == owner &&
            isSameStoryTaskPos( &e->teamup->activetask->sahandle, &sahandle ) )
        {
            MissionSetTeamupComplete(e, 1);
        }
    }
    else if (strnicmp(msg,DBMSG_MISSION_FAILED,len)==0)
    {
        int owner;
        StoryTaskHandle sahandle;

        for(i=0; i<count; i++ )
        {
            e = entFromDbId(members[i]);
            if (e)
                serveFloater(e, e, "FloatMissionFailed", 0.0f, kFloaterStyle_Attention, 0);
        }

        e = entFromDbIdEvenSleeping(members[0]);  // only need a single member to deal with this message
        if (e && e->teamup && // verify this team is still attached to the mission that sent the message
            MissionDeconstructIdString(msg_text, &owner, &sahandle) &&
            e->teamup->activetask->assignedDbId == owner &&
            isSameStoryTaskPos( &sahandle, &e->teamup->activetask->sahandle ) )
        {
            MissionSetTeamupComplete(e, 0);
        }
    }
    else if (strnicmp(msg,DBMSG_MISSION_STOPPED_RUNNING,len)==0)
    {
        int owner;
        StoryTaskHandle sahandle;

        e = entFromDbIdEvenSleeping(members[0]);  // only need a single member to deal with this message
        if (e && e->teamup && // verify this team is still attached to the mission that sent the message
            MissionDeconstructIdString(msg_text, &owner, &sahandle) &&
            e->teamup->activetask->assignedDbId == owner &&
            isSameStoryTaskPos( &sahandle, &e->teamup->activetask->sahandle ) )
        {
            teamHandleMapStoppedRunning(e);
        }
    }
    else if (strnicmp(msg,DBMSG_MISSION_OBJECTIVE,len)==0)
    {
        int owner, objnum, success;
        StoryTaskHandle sahandle;

        e = entFromDbIdEvenSleeping(members[0]);  // only need a single member to deal with this message
        if (e && e->teamup && // verify this team is still attached to the mission that sent the message
            MissionDeconstructObjectiveString(msg_text, &owner, &sahandle, &objnum, &success) &&
            (e->taskforce_id || (e->teamup->activetask->assignedDbId == owner)) && // MAK - if on taskforce, owner can change easily.  we can safely ignore check here
            isSameStoryTaskPos( &sahandle, &e->teamup->activetask->sahandle ) )
        {
            MissionHandleObjectiveInfo(e, sahandle.compoundPos, objnum, success);
        }
    }
    else if(strnicmp(msg, DBMSG_TEAM_TASK_SELECTED, len)==0)
    {
        for(i=0; i<count; i++ )
        {
            Entity* teammate;
            teammate = entFromDbId(members[i]);
            if (teammate && teammate->teamup && teammate->teamup_id == container)
            {
                teammate->pl->pendingArchitectTickets = 0;

                character_InterruptMissionTeleport(teammate->pchar);
                START_PACKET(pak, teammate, SERVER_TASK_SELECT);
                    pktSendBitsAuto(pak, 0);
                END_PACKET
                if(teammate->db_id != teammate->teamup->members.leader)
                    sendChatMsg(teammate, localizedPrintf(teammate, "TeamTaskChanged"), INFO_SVR_COM, 0);
            }
        }
    }
    else if (strnicmp(msg, DBMSG_TASKFORCE_RESUMETEAM, len)==0)
    {
        // look for first taskforce member on this server who isn't sender
        //  - he needs to add sender to his team
        int sender_id = atoi(msg_text);
        for (i = 0; i < count; i++)
        {
            Entity* leader;

            if (members[i] == sender_id) continue;
            leader = entFromDbIdEvenSleeping(members[i]);
            if (leader)
            {
                if (!leader->teamup_id)
                {
                    teamCreate(leader, CONTAINER_TEAMUPS);
                    TaskForceSelectTask(leader);
                }
                sprintf(tmp, "team_accept_relay %i %i %i %i 0", sender_id, leader->teamup_id, 0, leader->pl->pvpSwitch);
                serverParseClient(tmp, NULL);
                break;
            }
        }
    }
    else if (strnicmp(msg, DBMSG_TFCHALLENGE_FAILED, len)==0)
    {
        for (i = 0; i < count; i++)
        {
            Entity* e = entFromDbId(members[i]);
            if (e)
                TaskForceShowChallengeFailedMsg(e);
        }
    }
    else if (strnicmp(msg, DBMSG_TFCHALLENGE_SHOWSUMMARY, len)==0)
    {
        for (i = 0; i < count; i++)
        {
            Entity* e = entFromDbId(members[i]);
            if (e)
                TaskForceShowChallengeSummary(e);
        }
    }
    else if (strnicmp(msg, DBMSG_FLASHBACK_GETCREDIT, len)==0)
    {
        for (i = 0; i < count; i++)
        {
            Entity* e = entFromDbId(members[i]);
            if (e)
                badge_StatAdd(e, "FlashbacksComplete", 1);
        }
    }
    else if (strnicmp(msg, DBMSG_SUPERGROUP_REFRESH, len)==0)
    {
        teamlogPrintf(__FUNCTION__, "supergroup refresh %i, sender %i, count %i", container, senderID, count);
        for (i = 0; i < count; i++)
        {
            Entity* e = entFromDbId(members[i]);
            if (e)
                sgroup_getStats(e);
        }
    }
    else if (strnicmp(msg, DBMSG_PAYMENT_REMAIN_TIME, len)==0 && count == 1)
    {
        Entity* e = entFromDbIdEvenSleeping(members[0]);

        sendChatMsg( e, msg_text, msg_type, senderID );
    }
    else if (strnicmp(msg, DBMSG_ARCHITECT_COMPLETE, len)==0)
    {
        for (i = 0; i < count; i++)
        {
            Entity* e = entFromDbIdEvenSleeping(members[i]);
            if (e)
                playerCreatedStoryArc_Reward(e, 1);
        }
    }
    else if (strnicmp(msg, DBMSG_ARCHITECT_COMPLETE_MISSION, len)==0)
    {
        for (i = 0; i < count; i++)
        {
            Entity* e = entFromDbIdEvenSleeping(members[i]);
            if (e)
                playerCreatedStoryArc_Reward(e, 0);
        }
    }
    else if (strnicmp(msg, DBMSG_TASKFORCE_COMPLETE, len)==0)
    {
        int success;
        char *msg_text2;
        msg_text2 = strchr(msg_text,':');
        if (msg_text2)
        {
            *msg_text2 = 0;
            success = atoi(msg_text);
            msg_text = msg_text2 + 1;

            for (i = 0; i < count; i++)
            {
                Entity* e = entFromDbIdEvenSleeping(members[i]);
                if (e)
                    StoryArcRewardMerits(e, msg_text, success);
            }
        }
    }
}

//
// Send this message to all in group.  Comes with a DB Server style container header.
//
void sendEntsMsg(int container_type,int container_num, int msg_type, int senderID, char *msgs,...)
{
    int size;
    char *buffer;
    va_list va;

    va_start( va, msgs );
    size = _vscprintf(msgs, va);
    buffer = _alloca(size+1);
    vsprintf_s(buffer, size+1, msgs, va);
    va_end(va);

    dbBroadcastMsg(    container_type, &container_num, msg_type, senderID, 1, buffer );
}


