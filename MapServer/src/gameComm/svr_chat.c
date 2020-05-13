#include <stdio.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/log.h>
#include "dbcomm/logcomm.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/network/netio.h>
#include "combat/combat.h"
#include "entity/entVarUpdate.h"
#include "dbcomm/dbcontainer.h"
#include "dbcomm/dbcomm.h"
#include "varutils.h"
#include "sendToClient.h"
#include "container/containerbroadcast.h"
#include "entity/entserver.h"
#include "language/langServerUtil.h"
#include "svr_chat.h"
#include "svr/svr_base.h"
#include "dbcomm/dbquery.h"
#include "cmdparse/chat_emote.h"
#include "cmdparse/cmdchat.h"
#include "entity/character_animfx.h"
#include "svr/svr_player.h"
#include <utilitieslib/utils/utils.h>
#include "dbcomm/shardcomm.h"
#include "entity/SgrpServer.h"
#include "gameSys/arenamap.h"
#include "entity/friendCommon.h"
#include "dbcomm/dbnamecache.h"
#include "gameSys/arenamapserver.h"
#include "cmdparse/cmdserver.h"
#include "entity/teamCommon.h"
#include "entity/Supergroup.h"
#include "entity/costume.h"
#include "comm_backend.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/timing.h>
#include "dbcomm/staticMapInfo.h"
#include "container/containerloadsave.h" // packagePetition
#include "comm_game.h"
#include "auth/authUserData.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include "entity/character_eval.h"
#include "cmdparse/cmdenum.h"


#define CHAT_SPAM_FALLOF_TIME     (30)
#define CHAT_SPAM_ALLOWED        (5)
#define CHAT_SPAM_BAN_TIME         (2*60)

void chatRateLimiter(Entity *e)
{
    U32 time = dbSecondsSince2000();

    if( e->pl->lastChatSent == time )
        e->pl->chatSpams++;
    else
    {
        e->pl->chatSpams -= (time - (e->pl->lastChatSent+1));
        if(e->pl->chatSpams<0)
            e->pl->chatSpams = 0;
    }

    if( e->pl->chatSpams > CHAT_SPAM_ALLOWED)  
    {
        e->chat_ban_expire = MAX( e->chat_ban_expire, (U32)(time + CHAT_SPAM_BAN_TIME) );
        e->pl->chatSpams = 0;
    }

    e->pl->lastChatSent = time;
}


int chatBanned(Entity *e, const char *msg)
{
    int dt;

    if(SAFE_MEMBER(e->pl,send_spam_petition))
    {
        U32 time = dbSecondsSince2000();

        const char *str = packagePetition(e,PETITIONCATEGORY_CONDUCT,"Auto-Silenced Chat, Last Message in Description",(msg?msg:""));
        dbAsyncContainerUpdate(CONTAINER_PETITIONS,-1,CONTAINER_CMD_CREATE,str,0);
        free((void*)str);

        e->pl->is_a_spammer = 1;
        LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "Player auto-silenced for being ignored too many times on global");
        e->pl->send_spam_petition = 0;
    }

    dt = e->chat_ban_expire - dbSecondsSince2000();
    if ( dt <= 0)
    {
        if(e->chat_ban_expire)
            shardCommSendf(e, false, "unSpamMe" );
        e->chat_ban_expire = 0;
        e->pl->is_a_spammer = 0;
        
        return 0;
    }
    else if(e->pl && e->pl->is_a_spammer)
    {
        // spammers don't get the polite message, but record what they said
        LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s","[spamblocked]",msg);
        return 2;
    }
    else
    {
        char* message;
        int hour, min, sec;
        hour = dt / 3600;
        min = (dt % 3600) / 60;
        sec = (dt - hour*3600 - min*60);

        if( !hour )
        {
              if( min )
                message = localizedPrintf(e, "BannedFromChatMinSec", min, sec);
            else
                message = localizedPrintf(e, "BannedFromChatSec", sec);
        }
        else
            message = localizedPrintf(e, "BannedFromChat", hour, min, sec);
    
        if (!e->pl->afk) // Prevent an infinite message bounce condition
            chatSendToPlayer(e->db_id, message, INFO_GMTELL, 0);
        return 1;
    }
}


//-------------------------------------------------------------------------------------------------------

/* Function chatSendToPlayer()
 *  Send a message directly to a player.
 *  This function either:
 *      1) sends the message directly to a player connected to this mapserver.
 *      or
 *      2) asks the dbserver to send the message to the mapserver where the player
 *      is connect.  The mapserver can then relay the message to the player.
 */
void chatSendToPlayer(int container, const char *msg, int type, int senderID)
{
    Entity  *rec;
    Entity *e = entFromDbId(senderID);
    int typebit = 1 << type;

    if( rec = entFromDbId( container ) )
    {
        // 8/4/04 MJP: Disabled server-side filtering due to unintended effects (turning off channels like PRIVATE on client 
        // would take away speech bubbles). If this becomes a problem, and need to filter some stuff, simply 
        // modify chat_settings.systemChannels when they are received from client in chatSettings.c

            //  don't send if filtered out
            //if( type == INFO_GMTELL || ((rec->pl->chat_settings.systemChannels & typebit)))
            //{
        if(e)
            sendChatMsgFrom(rec, e, type, DEFAULT_BUBBLE_DURATION, msg);
        else
            sendChatMsg( rec, msg, type, senderID );
            //}
    }
    else
    {
        Entity *sleepingEnt = entFromDbIdEvenSleeping( container );
        if (sleepingEnt && sleepingEnt->client && sleepingEnt->client->message_dbid)
        {
            // This player's chat is being redirected from a CSR command
            sendChatMsg( sleepingEnt, msg, type, senderID );
        } 
        else 
        {
            sendEntsMsg( CONTAINER_ENTS, container, type, senderID, "%s%s", DBMSG_PLAYER_MSG(e), msg );
        }
    }
}

//
//
void chatSendToSupergroup( Entity *e, const char *msg, int type)
{
    // Trial accounts only get team, local, friend and limited tells.
    if(entIsTrial(e))
        return;

    if(type != INFO_SVR_COM)
    {
        chatRateLimiter(e);
        if(chatBanned(e,msg))
            return;
    }

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "supergroupString", "messageString", "emptyString", "supergroupSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    if( !e->supergroup_id )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"youAreNotOnASgroup"), INFO_USER_ERROR, e->db_id);
    }
    else
    {
        if(e->db_id != -1)
        {
            if( type == INFO_SUPERGROUP_COM )
            {
                char buf[CHAT_TEMP_BUFFER_LENTH];
                sprintf( buf, "%s: %s", e->name, msg );
                LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[supergroup]", msg );
                sendEntsMsg(CONTAINER_SUPERGROUPS, e->supergroup_id, INFO_SUPERGROUP_COM, e->db_id, "%s%s", DBMSG_CHAT_MSG, buf);
            }
            else
            {
                sendEntsMsg(CONTAINER_SUPERGROUPS, e->supergroup_id, type, e->db_id, "%s%s", DBMSG_CHAT_MSG, msg);
            }
        }
    }
}

void chatSendToLevelingpact( Entity *e, const char *msg, int type)
{
    // Trial accounts only get team, local, friend and limited tells.
    if(entIsTrial(e))
        return;

    if(type != INFO_SVR_COM)
    {
        chatRateLimiter(e);
        if(chatBanned(e,msg))
            return;
    }

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "levelingpactChatString", "messageString", "emptyString", "levelingpactChatSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    if( !e->levelingpact_id )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"LevelingPactNotAMember"), INFO_USER_ERROR, e->db_id);
    }
    else
    {
        if(e->db_id != -1)
        {
            if( type == INFO_LEVELINGPACT_COM )
            {
                char buf[CHAT_TEMP_BUFFER_LENTH];
                sprintf( buf, "%s: %s", e->name, msg );
                LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[levelingpact]", msg );
                sendEntsMsg(CONTAINER_LEVELINGPACTS, e->levelingpact_id, INFO_LEVELINGPACT_COM, e->db_id, "%s%s", DBMSG_CHAT_MSG, buf);
            }
            else
            {
                sendEntsMsg(CONTAINER_LEVELINGPACTS, e->levelingpact_id, type, e->db_id, "%s%s", DBMSG_CHAT_MSG, msg);
            }
        }
    }
}

//
//
void chatSendToAlliance( Entity *e, const char *msg)
{
    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    chatRateLimiter(e);
    if(chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "allianceString", "messageString", "emptyString", "allianceSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    if( !e->supergroup_id )
        chatSendToPlayer(e->db_id, localizedPrintf(e,"youAreNotOnASgroup"), INFO_USER_ERROR, e->db_id);
    else
    {
        if(e->db_id != -1)
        {
            char buf[CHAT_TEMP_BUFFER_LENTH + 64];
            char buf2[CHAT_TEMP_BUFFER_LENTH + 128];
            int i;

            if (!sgroup_hasPermission(e, SG_PERM_ALLIANCECHAT))
            {
                chatSendToPlayer( e->db_id, localizedPrintf(e,"AllianceNotAllowedToChat"), INFO_USER_ERROR, 0);
                return;
            }

            sprintf( buf, "%s: %s", e->name, msg );
            LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[alliance]", msg );

            sendEntsMsg(CONTAINER_SUPERGROUPS, e->supergroup_id, INFO_ALLIANCE_OWN_COM, e->db_id, "%s%s", DBMSG_CHAT_MSG, buf);

            for (i = 0; i < MAX_SUPERGROUP_ALLIES; i++)
            {
                if (e->supergroup->allies[i].db_id == 0)
                    break;

                if (e->supergroup->allies[i].dontTalkTo)
                    continue;

                sprintf(buf2, " (%s)%s", e->supergroup->name, buf);
                sendEntsMsg(CONTAINER_SUPERGROUPS, e->supergroup->allies[i].db_id, INFO_ALLIANCE_ALLY_COM, e->db_id, "%s%d:%d:%s", DBMSG_ALLIANCECHAT_MSG, e->supergroup_id, sgroup_rank(e, e->db_id), buf2);
            }
        }
    }
}

void chatSendToLeague( Entity *e, const char *msg, int type)
{
    // Trial accounts only get team, local, friend and limited tells.
    if(entIsTrial(e))
        return;

    if(type != INFO_SVR_COM)
    {
        chatRateLimiter(e);
        if(chatBanned(e,msg))
            return;
    }

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "leagueString", "messageString", "emptyString", "leagueSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    if( !e->league_id )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"youAreNotOnALeague"), INFO_USER_ERROR, e->db_id);
    }
    else
    {
        if(e->db_id != -1)
        {
            if( type == INFO_LEAGUE_COM )
            {
                char buf[CHAT_TEMP_BUFFER_LENTH];
                sprintf( buf, "%s: %s", e->name, msg );
                LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[league]", msg );
                sendEntsMsg(CONTAINER_LEAGUES, e->league_id, INFO_LEAGUE_COM, e->db_id, "%s%s", DBMSG_CHAT_MSG, buf);
            }
            else
            {
                sendEntsMsg(CONTAINER_LEAGUES, e->league_id, type, e->db_id, "%s%s", DBMSG_CHAT_MSG, msg);
            }
        }
    }
}

void chatSendPetSay( Entity * pet, const char * msg )
{
    Entity *e = erGetEnt(pet->erOwner);

    if( !e || !msg || msg[0] == 0 )
        return;

    if( !e->teamup_id )
        chatSendToPlayer(e->db_id, msg, INFO_PET_SAYS, pet->owner );
    else
    {
        int i;
        for( i = 0; i < e->teamup->members.count; i++ )
        {
            Entity * teammate = entFromDbId( e->teamup->members.ids[i] );
            if( teammate )
                chatSendToPlayer(teammate->db_id, msg, INFO_PET_SAYS, pet->owner );
        }
    }
}
//
//
void chatSendToTeamup( Entity *e, const char *msg, int type)
{
    if( (type != INFO_SVR_COM) && chatBanned(e,msg) )
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "teamString", "messageString", "emptyString", "teamSynonyms" ), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() && !ArenaMapIsParticipant(e) )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    if( !e->teamup_id )
        chatSendToPlayer(e->db_id, localizedPrintf(e,"youAreNotOnATeam"), INFO_USER_ERROR, e->db_id);
    else
    {
        if(e->db_id != -1)
        {
            if( type == INFO_TEAM_COM )
            {
                char buf[CHAT_TEMP_BUFFER_LENTH];
                sprintf( buf, "%s: %s", e->name, msg );
                LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[team]", msg );
                sendEntsMsg(CONTAINER_TEAMUPS, e->teamup_id, INFO_TEAM_COM, e->db_id, "%s%s", DBMSG_CHAT_MSG, buf);
            }
            else
            {
                sendEntsMsg(CONTAINER_TEAMUPS, e->teamup_id, type, e->db_id, "%s%s", DBMSG_CHAT_MSG, msg);
            }
        }
    }
}

void chatSendToArena( Entity *e, const char *msg, int type)
{
    chatRateLimiter(e);
    if( chatBanned(e,msg) )
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat", 
            "arenaLocalString", "messageString", "emptyString", "arenaLocalSynonyms" ), INFO_ARENA_ERROR, 0);
        return;
    }

     if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_ARENA_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_ARENA_ERROR, 0 );
        return;
    }

    if( !e->pl->arenaActiveEvent.eventid )
        chatSendToPlayer(e->db_id, localizedPrintf(e,"youAreNotInArenaEvent"), INFO_USER_ERROR, e->db_id);
    else
    {
        int i;
        ArenaEvent *event = FindEventDetail( e->pl->arenaActiveEvent.eventid, 0, NULL );

        for( i = eaSize(&event->participants)-1; i >= 0; i-- )
        {
            if( event->participants[i]->dbid )
                chatSendToPlayer( event->participants[i]->dbid, msg, INFO_ARENA, e->db_id );
        }
    }
}

void chatSendToArenaChannel( Entity *e, const char *msg )
{
    char buf[CHAT_TEMP_BUFFER_LENTH];

    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    if(e && e->pl && !AccountCanPublicChannelChat(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"InvalidChatChannelTier4"), INFO_USER_ERROR, 0);
        return;
    }

    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "arenaCommandString", "messageString", "emptyString", "arenaSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[arena]", msg );

    sendEntsMsg( CONTAINER_ENTS, -1, INFO_ARENA_GLOBAL, e->db_id, "%s%s: %s",DBMSG_PLAYER_MSG(e), e->name, msg );
}

void chatSendToLookingForGroup( Entity *e, const char *msg )
{
    char buf[CHAT_TEMP_BUFFER_LENTH];

    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "lfgCommandString", "messageString", "emptyString", "lfgSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[looking_for_group]", msg );

    sendEntsMsg( CONTAINER_ENTS, -1, INFO_LOOKING_FOR_GROUP, e->db_id, "%s%s: %s",DBMSG_PLAYER_MSG(e), e->name, msg );
}

void chatSendToArchitectGlobal( Entity *e, const char *msg )
{
    char buf[CHAT_TEMP_BUFFER_LENTH];

    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    // HYBRID : Add AH License lock here
    if(e && e->pl && (!AccountCanPublicChannelChat(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags) || !AccountHasStoreProduct(ent_GetProductInventory( e ), SKU("coliauch"))))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"InvalidChatChannelTier4"), INFO_USER_ERROR, 0);
        return;
    }


    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "architectCommandString", "messageString", "emptyString", "arenaSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[architect]", msg );

    sendEntsMsg( CONTAINER_ENTS, -1, INFO_ARCHITECT_GLOBAL, e->db_id, "%s%s: %s",DBMSG_PLAYER_MSG(e), e->name, msg );
}

void chatSendToHelpChannel( Entity *e, const char *msg )
{
    char buf[CHAT_TEMP_BUFFER_LENTH];

    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"usefulHelpMessage"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[help]", msg );

    sendEntsMsg( CONTAINER_ENTS, -1, INFO_HELP, e->db_id, "%s%s: %s",DBMSG_PLAYER_MSG(e), e->name, msg );
}

//
//
void chatSendToAll( Entity *e, const char *msg)
{
    if( e->access_level <= 0 )
        return;

    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s: %s", "[admin]", e->name, msg );

    sendEntsMsg( CONTAINER_ENTS, -1, INFO_ADMIN_COM, e->db_id, "%s %s: %s",DBMSG_CHAT_MSG, e->name, msg );
}

void chatSendTrialMessage(Entity *e, char *toName)
{
    if(e && toName && entIsTrial(e))
    {
        char msg[CHAT_TEMP_BUFFER_LENTH];
        //int ban_level;
        int id        = dbPlayerIdFromName(toName);
        Entity *toPlayer    = (id>=0)?entFromDbId( id ):NULL;
        int online    = player_online( toName );
        char * message = localizedPrintf(e,"AutoReplyTrial", e->name);


#if 0    //if we end up seeing people spamming this message, which would be admittedly weird, chat limit this.
        //ALTERNATIVELY, only allow this to send when the client has sent this message.
        //UNCOMMENT OUT THE int ban_level; above!
        chatRateLimiter(e);
        ban_level = chatBanned(e,message);
        if(ban_level && (!toPlayer || !toPlayer->access_level))
        {
            if(ban_level > 1) // a spammer
            {
                // pretend we sent the tell
                sprintf( msg, "-->%s: %s", toName, chatMsg );
                chatSendToPlayer(e->db_id, message, INFO_PRIVATE_COM, 0);
            }
            return;
        }
#endif

        if  (id < 0 || // Character doesn't exist or
            (!toPlayer && !online) || // He's not online and he's not on this mapserver or
            (toPlayer && toPlayer->logout_timer) || // He's on this mapserver, and online, but he's currently marked as "quitting"
            entIsTrial(toPlayer)) //don't send to trials since you can't validly respond to trials.
        {
            //just ignore, don't send anything.
        }
        else if( message )
        {
            e->pl->lastPrivateSendee = id;
            //we don't care about access level for this.
            sprintf( msg, "%s: %s", e->name, message );
            chatSendToPlayer(id, msg, INFO_PRIVATE_NOREPLY_COM, e->db_id);

            if( !(toPlayer && toPlayer->pl->hidden&(1<<HIDE_TELL) ))
            {
                sprintf( msg, "-->%s: %s", toName, message );
                chatSendToPlayer(e->db_id, msg, INFO_PRIVATE_NOREPLY_COM, 0);
            }

            LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[tellTrialMessage]", msg );
        }
    }
}

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
//
//
void chatSendPrivateMsg( Entity *e, char *incmsg, int toLeader, int allowReply, int isReply )
{
    char *name=NULL;
    char* chatMsg;
    char msg[CHAT_TEMP_BUFFER_LENTH];
    char seps[] = ",";
    const char * handle=NULL;
    int id, online;
    Entity *player;

    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    if(!isReply && e && e->pl && !AccountCanWhisper(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"InvalidChatWhisperTier3"), INFO_USER_ERROR, 0);
        return;
    }

    strncpyt( msg, incmsg, ARRAY_SIZE(msg));
    if (incmsg[0]=='"')
    {
        // JE: added this so I can send /Ts to my testclients, which often have spaces in their names
        char *c = strchr(incmsg+1, '"');
        if (c) {
            strcpy(incmsg, incmsg+1);
            name = incmsg;
            c--; // strcpy moved it left by one
            *c=0; // Remove quote
            strcpy(c+1, c+2); // Remove space
        }
    }
    if (!name)
    {
        name = strtok( incmsg, seps );
    }

    if( !name || stricmp( incmsg, msg ) == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "privateString", "playerWithCommaString", "messageString", "privateSynonyms" ), INFO_USER_ERROR, 0);
        return;
    }


    chatMsg = &incmsg[strlen(name)+1];
    // If the first character of the message is a space (i.e. "/t TEST, Hi!"), skip it
    if (chatMsg && chatMsg[0]==' ')
        chatMsg++;

      if( strstri( chatMsg, localizedPrintf(e,"[AUTO-REPLY -- User is away]"/*"AutoReplyMatch"*/) ) )
    {
        e->pl->afk = 1;
        e->pl->send_afk_string = 0;
    }

    handle = getHandleFromString(name);
    if(handle)
    {
        chatRateLimiter(e);
        if(!chatBanned(e,msg))
        {
            // user specified a handle -- use global chat
             shardCommSendf(e, true, "SendUser \"%s\" \"%s\"", handle, escapeString(chatMsg));
        }
    }
    else    // use normal private chat
    {
        int ban_level;
        char *leaderTypeMsg = NULL;
        id        = dbPlayerIdFromName(name);
        player    = entFromDbId( id );
        online    = player_online( name );

        if (toLeader)
        {
            if (name && name[0] == '@')
            {
                chatSendToPlayer(e->db_id, localizedPrintf(e, "notValidForGlobals"), INFO_USER_ERROR, 0);
                return;
            }
            switch (toLeader)
            {
            case CMD_CHAT_PRIVATE_TEAM_LEADER:
                leaderTypeMsg = "toTeamLeader";
                break;
            case CMD_CHAT_PRIVATE_LEAGUE_LEADER:
                leaderTypeMsg = "toLeagueLeader";
                break;
            }
        }
        chatRateLimiter(e);
        ban_level = chatBanned(e,msg);
        if(ban_level && (!player || !player->access_level))
        {
            if(ban_level > 1) // a spammer
            {
                // pretend we sent the tell
                sprintf( msg, "-->%s: %s", name, chatMsg );
                chatSendToPlayer(e->db_id, msg, allowReply?INFO_PRIVATE_COM:INFO_PRIVATE_NOREPLY_COM, 0);
            }
            return;
        }

        if  (id < 0 || // Character doesn't exist or
            (!player && !online) || // He's not online and he's not on this mapserver or
            (player && player->logout_timer) ) // He's on this mapserver, and online, but he's currently marked as "quitting"
        {
            chatSendToPlayer(e->db_id, localizedPrintf(e,"playerNotFound", name), INFO_USER_ERROR, 0);
        }
        else
        {
            if( chatMsg )
            {
                if (!toLeader)
                    e->pl->lastPrivateSendee = id;
                if( e->access_level > 0 && e->access_level < 9 )
                {
                    sprintf( msg, "%s: %s", e->name, chatMsg );
                    chatSendToPlayer(id, msg, INFO_GMTELL, e->db_id);

                    sprintf( msg, "-->%s: %s", name, chatMsg );
                    chatSendToPlayer(e->db_id, msg, INFO_GMTELL, 0);

                    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[GMprivate]", msg );
                }
                else
                {
                    sprintf( msg, "%i:%s: %s", toLeader, e->name, chatMsg );
                    chatSendToPlayer(id, msg, allowReply?INFO_PRIVATE_COM:INFO_PRIVATE_NOREPLY_COM, e->db_id);

                    if( player && player->pl->hidden&(1<<HIDE_TELL) )
                    {
                        chatSendToPlayer(e->db_id, localizedPrintf(e,"playerNotFound", name), INFO_USER_ERROR, 0);
                    }
                    else
                    {
                        sprintf( msg, "-->%s: %s", leaderTypeMsg ? localizedPrintf(e, leaderTypeMsg, name): name, chatMsg );
                        chatSendToPlayer(e->db_id, msg, allowReply?INFO_PRIVATE_COM:INFO_PRIVATE_NOREPLY_COM, 0);
                    }

                    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[private]", msg );
                }
            }
        }
    }
}

//
//
void chatSendReply( Entity *e, const char * msg )
{
    char buf[CHAT_TEMP_BUFFER_LENTH];

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e, "incorrectFormat",
            "replyString", "messageString", "emptyString", "replySynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if ( e->pl->lastPrivateSender <= 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"noPlayerToReply"), INFO_USER_ERROR, 0);
    }
    else
    {
        char *destName = dbPlayerNameFromId(e->pl->lastPrivateSender);
        if (!destName)
        {
            // Player you're replying too has been deleted?
            chatSendToPlayer(e->db_id, localizedPrintf(e,"noPlayerToReply"), INFO_USER_ERROR, 0);
        } 
        else
        {
            if (strlen(msg) + strlen(destName) +2> sizeof(buf)) 
            {
                // Stop buffer overflow!
            }
            else 
            {
                sprintf(buf, "%s,%s", destName, msg);
                chatSendPrivateMsg(e, buf, 0, 1, 1);
            }
        }
    }
}

//
//
void chatSendTellLast( Entity *e, const char * msg )
{
    char buf[CHAT_TEMP_BUFFER_LENTH];

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "tellLastString", "messageString", "emptyString", "tellLastSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if ( e->pl->lastPrivateSendee <= 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"noTellsSent" ), INFO_USER_ERROR, 0);
    }
    else
    {
        char *destName = dbPlayerNameFromId(e->pl->lastPrivateSendee);
        if (!destName) 
        {
            // Player you're replying too has been deleted?
            chatSendToPlayer(e->db_id, localizedPrintf(e,"noPlayerToTell" ), INFO_USER_ERROR, 0);
        }
        else
        {
            if (strlen(msg) + strlen(destName) +2> sizeof(buf))
            {
                // Stop buffer overflow!
            } 
            else 
            {
                sprintf(buf, "%s,%s", destName, msg);
                chatSendPrivateMsg(e, buf, 0, 1, 1);
            }
        }
    }
}

//
//
void chatSendBroadCast( Entity *e, const char * msg )
{
    int i;
    char buf[CHAT_TEMP_BUFFER_LENTH];

    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    if(e && e->pl && !AccountCanPublicChannelChat(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"InvalidChatChannelTier4"), INFO_USER_ERROR, 0);
        return;
    }

    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "broadcastString", "messageString", "emptyString", "broadcastSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() && !ArenaMapIsParticipant(e) )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[broadcast]", msg );
    for( i = 1; i < entities_max; i++)
    {
        if( entity_state[i] & ENTITY_IN_USE && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER )
        {
            sendChatMsgFrom(entities[i], e, INFO_SHOUT_COM, DEFAULT_BUBBLE_DURATION, buf);
        }
    }
}

// Like /a (admin chat) but only to current map
void chatSendMapAdmin( Entity *e, const char * msg )
{
    int i;
    char buf[CHAT_TEMP_BUFFER_LENTH];

    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "broadcastString", "messageString", "emptyString", "broadcastSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[broadcast]", msg );
    for( i = 1; i < entities_max; i++)
    {
        if( entity_state[i] & ENTITY_IN_USE && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER )
        {
            sendChatMsg( entities[i], buf, INFO_ADMIN_COM, e->db_id );
        }
    }
}


//
// broadcast to all people with accesslevel >= 9
void chatSendDebug( const char * msg )
{
    int i;

    for( i = 1; i < entities_max; i++)
    {
        if( entity_state[i] & ENTITY_IN_USE && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER && entities[i]->access_level >= 9)
        {
            sendChatMsg( entities[i], msg, INFO_DEBUG_INFO, 0);
        }
    }
}

//
// server communication to everyone on map
void chatSendServerBroadcast( const char * msg )
{
    int i;

    for( i = 1; i < entities_max; i++)
    {
        if( entity_state[i] & ENTITY_IN_USE && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER)
        {
            sendChatMsg( entities[i], msg, INFO_SVR_COM, 0);
        }
    }
}

//
//
void chatSendRequest( Entity *e, const char * msg )
{
    int i;
    char buf[CHAT_TEMP_BUFFER_LENTH];

    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    if(e && e->pl && !AccountCanPublicChannelChat(ent_GetProductInventory( e ), e->pl->loyaltyPointsEarned, e->pl->account_inventory.accountStatusFlags))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"InvalidChatChannelTier4"), INFO_USER_ERROR, 0);
        return;
    }

    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "requestString", "messageString", "emptyString", "requestSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[request]", msg );
    for( i = 1; i < entities_max; i++)
    {
        if( entity_state[i] & ENTITY_IN_USE && ENTTYPE_BY_ID(i) == ENTTYPE_PLAYER )
        {
            sendChatMsgFrom(entities[i], e, INFO_REQUEST_COM, DEFAULT_BUBBLE_DURATION, buf);
        }
    }
}

//
//
void chatSendLocal( Entity *e, const char * msg )
{
    int     i;
    char buf[CHAT_TEMP_BUFFER_LENTH];
    static chatDistance = 0;


    if (chatDistance == 0)
    {
        StaticMapInfo* mapinfo = staticMapGetBaseInfo(db_state.map_id);
        if (mapinfo)
        {
            chatDistance = mapinfo->chatRadius;
        } else {
            chatDistance = 100;
        }
    }


    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "localString", "messageString", "emptyString", "localSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() && !ArenaMapIsParticipant(e) )
    {
        chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s %s", "[local]", msg );
    for (i=0; i<player_ents[PLAYER_ENTS_ACTIVE].count; i++)
    {
        const F32 * hearerPos;
        if( server_state.viewCutScene )
            hearerPos = server_state.cutSceneCameraMat[3];
        else
            hearerPos = ENTPOS(player_ents[PLAYER_ENTS_ACTIVE].ents[i]);

        if( ent_close( chatDistance, ENTPOS(e), hearerPos ) )
            sendChatMsgFrom(player_ents[PLAYER_ENTS_ACTIVE].ents[i], e, INFO_NEARBY_COM, DEFAULT_BUBBLE_DURATION, buf);
    }
}

//
//
void chatSendEmote( Entity *e, const char * msg )
{
    int     i;
    char buf[CHAT_TEMP_BUFFER_LENTH];
    bool    bDev = e->access_level > ACCESS_USER;
    static chatDistance = 0;

    if (chatDistance == 0)
    {
        StaticMapInfo* mapinfo = staticMapGetBaseInfo(db_state.map_id);
        if (mapinfo)
        {
            chatDistance = mapinfo->chatRadius;
        } else {
            chatDistance = 100;
        }
    }
    
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
     { 
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "localString", "messageString", "emptyString", "localSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    gIsCCemote = 0;
    // Let's see if this is one of the animated emotes
    for(i=eaSize(&g_EmoteAnims.ppAnims)-1; i>=0; i--)
    {
        const EmoteAnim *anim = g_EmoteAnims.ppAnims[i];
        if( matchEmoteName(anim->pchDisplayName, msg) && chatemote_CanDoEmote(anim,e) )
        {
            Entity *whoWeCheck = (e && e->erOwner) ? erGetEnt(e->erOwner) : e;
            character_EnterStance(e->pchar, NULL, 1);
            character_SetAnimBits(e->pchar, anim->piModeBits, 0, 0);
            

            // Should I reward a power?
            if (anim->pchPowerRequires && whoWeCheck->pchar && chareval_Eval(whoWeCheck->pchar, anim->pchPowerRequires, "defs/emotes.def"))
            {
                character_AddRewardPower(whoWeCheck->pchar, powerdict_GetBasePowerByFullName(&g_PowerDictionary, anim->pchPowerReward));
            }

            return;
        }
    }

    chatRateLimiter(e);

    sprintf( buf, "%s %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[emote]", msg );
    for (i=0; i<player_ents[PLAYER_ENTS_ACTIVE].count; i++)
    {
        const F32 * hearerPos;
        if( server_state.viewCutScene )
            hearerPos = server_state.cutSceneCameraMat[3];
        else
            hearerPos = ENTPOS(player_ents[PLAYER_ENTS_ACTIVE].ents[i]);

        if( ent_close( chatDistance, ENTPOS(e), hearerPos ) )
            sendChatMsgFrom(player_ents[PLAYER_ENTS_ACTIVE].ents[i], e, INFO_EMOTE, DEFAULT_BUBBLE_DURATION, buf);
    }
}

//
//
void chatSendEmoteCostumeChange( Entity *e, const char *emote, int costumeNum )
{
    int     i;
    bool    bDev = e->access_level > ACCESS_USER;
    static chatDistance = 0;

    if (chatDistance == 0)
    {
        StaticMapInfo* mapinfo = staticMapGetBaseInfo(db_state.map_id);
        if (mapinfo)
        {
            chatDistance = mapinfo->chatRadius;
        } else {
            chatDistance = 100;
        }
    }
    
    if( chatBanned(e,emote))
        return;

    if( !emote || emote[0] == 0 )
     { 
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "localString", "messageString", "emptyString", "localSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    gIsCCemote = 1;

    // Let's see if this is one of the animated emotes
    for(i=eaSize(&g_EmoteAnims.ppAnims)-1; i>=0; i--)
    {
        const EmoteAnim *anim = g_EmoteAnims.ppAnims[i];
        if( matchEmoteName(anim->pchDisplayName, emote) && chatemote_CanDoEmote(anim,e) )
        {
            // By the time we get here, we know that the emote string is legit.
            // TODO: Lock this to costume change type emotes only.  How do we check this?
            // Now do the necessary checks to ensure that the costume change is also OK.
            Costume *costume;

            gIsCCemote = 0;

            // This check prohibits using an emote without actually changing costumes.  If it's
            // considered OK to use them as emotes w/o doing a costume change, just remove this test
            // and the return;
            if( e->pl->current_costume == costumeNum )
            {
                return;
            }

            // Try and do the costume change 
            if (!costume_change( e, costumeNum ))
            {
                return;
            }
                    
            costume = costume_current(e);
            e->pl->pendingCostumeForceTime = timerSecondsSince2000() + CC_EMOTE_DEFER_FLAG_TIMEOUT;

            if (costume)
            {
                int sgColorIndex = costume->appearance.currentSuperColorSet;
                if (sgColorIndex < 0 || sgColorIndex >= NUM_SG_COLOR_SLOTS)
                {
                    sgColorIndex = 0;
                    costume->appearance.currentSuperColorSet = 0;
                }
                e->pl->superColorsPrimaryU    = costume->appearance.superColorsPrimaryU[sgColorIndex];
                e->pl->superColorsSecondaryU  = costume->appearance.superColorsSecondaryU[sgColorIndex];
                e->pl->superColorsPrimary2U   = costume->appearance.superColorsPrimary2U[sgColorIndex];
                e->pl->superColorsSecondary2U = costume->appearance.superColorsSecondary2U[sgColorIndex];
                e->pl->superColorsTertiaryU   = costume->appearance.superColorsTertiaryU[sgColorIndex];
                e->pl->superColorsQuaternaryU = costume->appearance.superColorsQuaternaryU[sgColorIndex];
                costume_sendSGColors(e);
            }

            character_EnterStance(e->pchar, NULL, 1);
            character_SetAnimBits(e->pchar, anim->piModeBits, 0, 0);
            return;
        }
    }
    gIsCCemote = 0;
}


void chatSendToFriends( Entity *e, const char * msg )
{
    int i, sent = 0;
    char buf[CHAT_TEMP_BUFFER_LENTH];

    // Trial accounts only get team, local, help and limited tells.
    if(entIsTrial(e))
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"invalidTrialChatChannel"), INFO_USER_ERROR, 0);
        return;
    }

    chatRateLimiter(e);
    if( chatBanned(e,msg))
        return;

    if( !msg || msg[0] == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"incorrectFormat",
            "friendString", "messageString", "emptyString", "friendSynonyms"), INFO_USER_ERROR, 0);
        return;
    }

    if( OnArenaMap() )
    {
        if(ArenaMapIsParticipant(e))
            chatSendToPlayer( e->db_id, localizedPrintf(e,"NotAllowedInArena"), INFO_USER_ERROR, 0 );
        else
            chatSendToPlayer( e->db_id, localizedPrintf(e,"ObserverCannotSpeak"), INFO_USER_ERROR, 0 );
        return;
    }

    if( e->friendCount == 0 )
    {
        chatSendToPlayer(e->db_id, localizedPrintf(e,"NoFriends" ), INFO_USER_ERROR, 0);
        return;
    }

    sprintf( buf, "%s: %s", e->name, msg );
    LOG_ENT( e, LOG_CHAT, LOG_LEVEL_IMPORTANT, 0, "%s%s", "[friends]", msg );
    for( i = 0; i < e->friendCount; i++ )
    {
        if( e->friends[i].online )
        {
            chatSendToPlayer(e->friends[i].dbid, buf, INFO_FRIEND_COM, e->db_id);
            sent = TRUE;
        }
    }

    if( !sent )
        chatSendToPlayer(e->db_id, localizedPrintf(e,"NoFriendsOnline"), INFO_USER_ERROR, 0);
    else
        chatSendToPlayer( e->db_id, buf, INFO_FRIEND_COM, e->db_id);
}


//
//
void recvChatMsg( Entity *e, Packet *pak )
{
    char msg[1000];

    strncpyt(msg, pktGetString(pak), ARRAY_SIZE(msg));

    if (db_state.silence_all)
    {
        char* message;
        message = localizedPrintf(e, "ChatSilenced");
        chatSendToPlayer(e->db_id, message, INFO_GMTELL, 0);
        return;
    }


    switch( e->pl->chatSendChannel )
    {
        case INFO_PRIVATE_COM:
            chatSendPrivateMsg(e, msg, 0, 1, 0);
        break;

        case INFO_PRIVATE_NOREPLY_COM:
            chatSendPrivateMsg(e, msg, 0, 0, 0);
        break;

        case INFO_TEAM_COM:
            chatSendToTeamup( e, msg, INFO_TEAM_COM );
        break;

        case INFO_SUPERGROUP_COM:
             chatSendToSupergroup( e, msg, INFO_SUPERGROUP_COM );
        break;

        case INFO_LEVELINGPACT_COM:
            chatSendToLevelingpact(e, msg, INFO_LEVELINGPACT_COM);
        break;

        case INFO_ALLIANCE_OWN_COM:
        case INFO_ALLIANCE_ALLY_COM:
            chatSendToAlliance( e, msg );
            break;

        case INFO_NEARBY_COM:
            chatSendLocal( e, msg );
        break;

        case INFO_SHOUT_COM:
            chatSendBroadCast( e, msg );
        break;

        case INFO_REQUEST_COM:
            chatSendRequest( e, msg );
        break;

        case INFO_FRIEND_COM:
            chatSendToFriends( e, msg );
            break;

        case INFO_ARENA_GLOBAL:
            chatSendToArenaChannel( e, msg );

        case INFO_ARCHITECT_GLOBAL:
            chatSendToArchitectGlobal( e, msg );

        case INFO_HELP:
            chatSendToHelpChannel( e, msg );

        case INFO_ADMIN_COM:
            chatSendToAll( e, msg );
        break;

        case INFO_LEAGUE_COM:
            chatSendToLeague(e, msg, INFO_LEAGUE_COM);
        break;

        case INFO_EMOTE:
            chatSendEmote( e, msg );
        break;

    }
}

void setChatChannel( Entity * e, int newChannel, int userChannelIdx )
{
    e->pl->chatSendChannel = newChannel;
    e->pl->chat_settings.userSendChannel = userChannelIdx;
}

//------------------------------------------------------------------------------------------------------
// Ignore list stuff
//------------------------------------------------------------------------------------------------------

int isIgnoredAuth( Entity *e, int authId )
{
    int i;
    // if the name is found in the list, leave
    for ( i = 0; i < MAX_IGNORES; i++ )
    {
        if ( e->ignoredAuthId[i] == authId )
            return TRUE;
    }
    return FALSE;
}

int isIgnored( Entity *e, int db_id )
{
    int auth = authIdFromDbId(db_id);
    return (auth && isIgnoredAuth(e, auth));
    // if there is no auth id, assume they are ok
}



void addUserToIgnoreList(ClientLink *client, char* nameOfUserToIgnore, int authToIgnore, Entity* userIgnoring, bool spammer )
{
    if(nameOfUserToIgnore && nameOfUserToIgnore[0]=='@') // Global Name Passed
    {
        if( spammer )
            handleGenericShardCommCmd(client, userIgnoring, "ignoreSpammer", nameOfUserToIgnore );
        else
            handleGenericShardCommCmd(client, userIgnoring, "ignore", nameOfUserToIgnore );
    }
    else // Local Name
    {
        char* message;
        int authId = authIdFromName(nameOfUserToIgnore);
        if( authToIgnore )
            authId = authToIgnore;

        if(  authId == 0 ) // -1 indicates name was not found
        {
            message = localizedPrintf(userIgnoring, "InvalidPlayerName", nameOfUserToIgnore );
            sendChatMsg( userIgnoring, message, INFO_USER_ERROR, 0 );
        }
        else if ( authId == userIgnoring->auth_id )
        {
            message = localizedPrintf(userIgnoring, "Can'tIgnoreSelf");
            sendChatMsg( userIgnoring, message, INFO_USER_ERROR, 0 );
        }
        else
        {
            if ( isIgnoredAuth( userIgnoring, authId ) )
            {
                message = localizedPrintf(userIgnoring, "AlreadyIgnored", nameOfUserToIgnore);
                sendChatMsg(userIgnoring, message, INFO_USER_ERROR, 0 );
                return;
            }

            // add them to list (this will be overwritten later by chatserver which is authority on ignores)
            memmove( userIgnoring->ignoredAuthId + 1, userIgnoring->ignoredAuthId, sizeof(int)*(MAX_IGNORES-1) );
            userIgnoring->ignoredAuthId[0] = authId;

            // let the user know he won't be annoyed by the jerk anymore
            message = localizedPrintf(userIgnoring, "PlayerIgnored", nameOfUserToIgnore);
            sendChatMsg(userIgnoring, message, INFO_SVR_COM, 0 );

            // Next add to global ignore also
            if( spammer )
                handleGenericAuthShardCommCmd(client, userIgnoring, "ignoreAuthSpammer", authId ); 
            else
                handleGenericAuthShardCommCmd(client, userIgnoring, "ignoreAuth", authId ); 
        }
    }
}

// this removes an ignored person from an ignore list
void removeUserFromIgnoreList( ClientLink * client, char* nameOfUserToRemove, Entity* userRemoving )
{
    if(nameOfUserToRemove[0]=='@') // Global Name Passed
    {
        handleGenericShardCommCmd(client, userRemoving, "UnIgnore", nameOfUserToRemove );
    }
    else // Local Name
    {
        char* message;
        int authId = authIdFromName(nameOfUserToRemove);

        if(!authId) // -1 indicates name was not found
        {
            message = localizedPrintf(userRemoving, "InvalidPlayerName", nameOfUserToRemove);
            sendChatMsg(userRemoving, message, INFO_USER_ERROR, 0 );
        }
        else
        {
            int i;
            for( i = 0; i < MAX_IGNORES; i++ )
            {
                if( authId == userRemoving->ignoredAuthId[i] )
                    userRemoving->ignoredAuthId[i] = 0; // zero out entry to allow messages immediately
            }

            // remove from global ignore
            handleGenericAuthShardCommCmd(client, userRemoving, "UnignoreAuth", authId );
        }
    }
} 

// shows a user his ignore list
void ignoreList( Entity* e )
{
    // TODO: Pass command chatserver
}
