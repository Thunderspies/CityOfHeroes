/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include "sendToClient.h"
#include <utilitieslib/utils/wininclude.h>
#include <stdio.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include "svr/svr_base.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/network/netio.h>
#include "entity/entserver.h"
#include "varutils.h"
#include <utilitieslib/components/StringCache.h>
#include "entity/entVarUpdate.h"
#include "comm_backend.h"
#include "dbcomm/dbdoor.h"
#include "dbcomm/dbcomm.h"
#include "comm_game.h"
#include "language/langServerUtil.h"
#include "entity/friends.h"
#include "svr_chat.h"
#include "gameSys/arenamap.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/components/EString.h>
#include "gameComm/Npc.h"
#include <utilitieslib/components/earray.h>
#include "entity/character_base.h"
#include "cmdparse/cmdenum.h"
#include "entity/teamCommon.h"
#include "entity/character_target.h"
#include <utilitieslib/utils/scriptvars.h>
#include "storyarc/storyarcutil.h"
#include <utilitieslib/language/MessageStoreUtil.h>

static char *xlateConfirmText(Entity *e, int translateName, const char* messageID, ...);

void convPrintf( ClientLink* client, const char *s, va_list va )
{
    int size;
    char *buffer, *cursor;
    bool redirect=false;
    char *format = "CSR:%s> ";

    if (!client || !client->entity)
        return;

    if (client->message_dbid && client->entity->db_id != client->message_dbid)
        redirect = true;

    size = _vscprintf(s, va);
    if (redirect) 
        size += (int)(strlen(client->entity->name) + strlen(format));

    buffer = _alloca(size+1);
    if (redirect)
    {
        sprintf(buffer, format, client->entity->name);
        cursor = buffer + strlen(buffer);
    }
    else 
        cursor = buffer;
    vsprintf_s(cursor, size+1 - (cursor-buffer), s, va);

    if (redirect)
    {
         cmdOldServerParsef( client, "relay_conprintf %i \"%s\"", client->message_dbid, escapeString(buffer) );
    } 
    else
    {
        START_PACKET( pak1, client->entity, SERVER_CON_PRINTF );
        pktSendString( pak1, buffer );
        END_PACKET
    }
}

//
void conPrintf( ClientLink* client, const char *s, ... )
{
    va_list va;

    va_start( va, s );
    convPrintf( client, s, va );
    va_end(va);
}

static char *SAFE_PLAYER_TAGS[] =
{
    "&lt;bordercolor ",
    "&lt;bgcolor ",
    "&lt;color ",
    "&lt;scale ",
    "&lt;duration ",
};
static void decodeSafeChatBubbleTagsInPlace(char **eString)
{
    unsigned int index;
    int *openTagIndices = NULL;
    for (index = 0; index < estrLength(eString); index++)
    {
        int i = 0;
        //    search for any open tags
        for (i = 0; i < ARRAY_SIZE(SAFE_PLAYER_TAGS); ++i)
        {
            if (!strnicmp((*eString)+index, SAFE_PLAYER_TAGS[i], strlen(SAFE_PLAYER_TAGS[i])))
            {
                eaiPush(&openTagIndices, index);
                break;
            }
        }

        //    only replace the '>' if there is an open tag somewhere
        if (eaiSize(&openTagIndices) && !strnicmp((*eString) + index, "&gt;", 4))
        {
            int openTagIndex = eaiPop(&openTagIndices);
            //    insert the closing tag first
            estrRemove(eString, index, 4);
            estrInsert(eString, index, ">", 1);

            //    insert the opening tag after so that the indexes don't get messed up
            estrRemove(eString, openTagIndex, 4);
            estrInsert(eString, openTagIndex, "<", 1);

            //    move the index back because we lost 3 characters from replacing the opening tag
            index -= (4-1);    //    strlen(&lt;) - strlen(<)
        }
    }
    eaiDestroy(&openTagIndices);
}

// this function will check to see if the msg should be ignored,
// then send the message to the client
void sendChatMsgEx( Entity *e, Entity *sender, const char *srcmsg, int type, F32 duration, int senderID, int sourceID, int enemy, F32* position )
{
    static Vec2 defaultPosition = {0.f, 0.f};
    char *msg;

    ClientLink* client = e->client;

    if (senderID && srcmsg && (type == INFO_PRIVATE_COM || type == INFO_PRIVATE_NOREPLY_COM))
    {
        //    check for to leader request
        char *dupMessage = strdup(srcmsg);
        char *toLeaderTypeStr = strtok(dupMessage,":");
        int toLeaderType = atoi(toLeaderTypeStr);
        srcmsg = srcmsg + (toLeaderType ? strlen(toLeaderTypeStr) : 1);
        //    bounce the request if its a leader message
        free(dupMessage);
        switch (toLeaderType)
        {
        case CMD_CHAT_PRIVATE_TEAM_LEADER:
            if (e->teamup && e->db_id != e->teamup->members.leader)
            {
                chatSendToPlayer(e->teamup->members.leader, srcmsg, type, senderID);
                return;
            }
            break;
        case CMD_CHAT_PRIVATE_LEAGUE_LEADER:
            if (e->league && e->db_id != e->league->members.leader)
            {
                chatSendToPlayer(e->league->members.leader, srcmsg, type, senderID);
                return;
            }
            break;
        }
    }

    if (!position)
        position = defaultPosition;

     if( OnArenaMap() && type != INFO_GMTELL ) 
    {
         if( type == INFO_PRIVATE_COM || type == INFO_PRIVATE_NOREPLY_COM) // bounce back message on privates
        {
            chatSendToPlayer( senderID, localizedPrintf(e,"PlayerIsInEvent"), INFO_USER_ERROR, 0 );
            return;
        }
        if( senderID && type != INFO_NEARBY_COM && type != INFO_SHOUT_COM && // arena players can only recieve these channels
            type != INFO_TEAM_COM && type != INFO_GMTELL && type != INFO_PET_SAYS )
        {
            return;
        }

        if( !ArenaMapIsParticipant(e) && type == INFO_TEAM_COM ) // observers can't even get team chat
            return;
    }

    if (client)
    {
        if ((type == INFO_SVR_COM || type == INFO_USER_ERROR) // Only redirect these
            && (client->message_dbid && client->entity->db_id != client->message_dbid))
        {
            char *format = "CSR:%s> %s";
            size_t size = strlen(srcmsg) + strlen(client->entity->name) + strlen(format);
            msg = _alloca(size + 1);
            sprintf(msg, format, client->entity->name, srcmsg);
            chatSendToPlayer(client->message_dbid, msg, INFO_DEBUG_INFO, 0);
            return;
        }
    }

    if( !isWatchedChannel(e, type) )
        return;

    // Check ignore list
    if( senderID && type != INFO_VILLAIN_SAYS && type != INFO_NPC_SAYS && 
        type != INFO_ADMIN_COM && type != INFO_GMTELL && type != INFO_CAPTION )
    {
        if(isIgnored(e,senderID))
            return;

        if( enemy && !server_state.allowHeroAndVillainPlayerTypesToTeamUp )
        {
            if( !e->pl->showEnemyBroadcast && 
                (type == INFO_SHOUT_COM || type == INFO_ARENA_GLOBAL || type == INFO_REQUEST_COM || type == INFO_HELP || type == INFO_ARCHITECT_GLOBAL) )
            {
                return;
            }
            if( !e->pl->showEnemyTells && !(e->pl->hidden&(1<<HIDE_TELL)) && type == INFO_PRIVATE_COM )
            {
                chatSendToPlayer( senderID, localizedPrintf(e,"PlayerIsNotSeeingEnemyChat",e->name), INFO_USER_ERROR, 0 );
                return;
            }
            if( e->pl->hideEnemyLocal && 
                (type == INFO_NEARBY_COM || type == INFO_NPC_SAYS || 
                type == INFO_EMOTE || type == INFO_VILLAIN_SAYS 
                || type == INFO_PET_COM ) )
            {
                return;
            }
        }
    }

    if (sender
        && (type == INFO_NEARBY_COM || type == INFO_NPC_SAYS 
            || type == INFO_EMOTE || type == INFO_VILLAIN_SAYS 
            || type == INFO_PET_COM || type == INFO_CAPTION)
        && !entity_TargetIsInVisionPhase(e, sender))
    {
        return;
    }

    // Record last sender for /rply
    if( (type == INFO_PRIVATE_COM || type == INFO_GMTELL) && senderID != e->db_id && senderID != 0 )
        e->pl->lastPrivateSender = senderID;

    //<-----------------------------------------------

    estrCreate(&msg);
    estrConcatCharString(&msg, srcmsg);
    decodeSafeChatBubbleTagsInPlace(&msg);

    // otherwise send message
    START_PACKET( pak1, e, SERVER_SEND_CHAT_MSG );
    pktSendBitsPack( pak1, 10, sourceID );
    pktSendBitsPack( pak1, 3, type );
    pktSendF32( pak1, duration );
    pktSendString( pak1, msg );
    pktSendF32(pak1, position[0]);
    pktSendF32(pak1, position[1]);
    END_PACKET

        if( e->pl->csrListener )
        {
            Entity * listener = entFromDbId( e->pl->csrListener );

            if( listener )
            {
                START_PACKET( pak1, listener, SERVER_SEND_CHAT_MSG );
                pktSendBitsPack( pak1, 10, senderID );
                pktSendBitsPack( pak1, 3, type );
                pktSendF32( pak1, duration );
                pktSendString( pak1, msg );
                pktSendF32(pak1, position[0]);
                pktSendF32(pak1, position[1]);
                END_PACKET
            }
            else
                e->pl->csrListener = 0;
        }
    estrDestroy(&msg);
}

void sendChatMsg( Entity *e, const char *srcmsg, int type, int senderID )
{
    sendChatMsgEx(e, 0, srcmsg, type, DEFAULT_BUBBLE_DURATION, senderID, senderID, 0, 0);
}

void sendChatMsgFrom(Entity *ent, Entity *eSrc, int type, F32 duration, const char *s )
{
    int id, id_src;
    Entity * parent = 0;

    if(type==INFO_VILLAIN_SAYS || type==INFO_NPC_SAYS || type==INFO_PET_COM )
        id = id_src = eSrc->owner;
    else
        id = id_src = eSrc->db_id;

    if(type==INFO_PET_COM)
    {
        parent = erGetEnt(eSrc->erOwner);
        if( parent )
            id = parent->db_id;
    }

    sendChatMsgEx(ent, eSrc, s, type, duration, id, id_src, ENT_IS_ON_RED_SIDE(ent) != ENT_IS_ON_RED_SIDE((parent?parent:eSrc)), 0 );
}

// Only sends the caption to the specified player
// since captions must be attached to an Entity, I have the player's Entity say it to himself
void sendPrivateCaptionMsg(Entity *player, const char *captionStr)
{
    // this code has been yanked out of saUtilEntSays
    // for whatever reason, captions were initially implemented as being entity dependent
    // so that makes this code that doesn't really have an entity a bit hacky
    char animation[100];
    char face[100];
    char * dialogToSayNow = 0;
    char * dialogToSayLater = 0;
    F32    time = 0;
    F32    duration = 0;
    int flags = 0;
    int shout = 0;
    int isCaption = 0;
    char gotoLoc[100];
    int goingToLoc = 0;
    char animList[1000];
    F32 captionPosition[] = {0.5, 1.0};
    ScriptVarsTable vars = {0};
    int dialogStartsWithTag = parseDialogForBubbleTags( captionStr, animation, &dialogToSayNow, &dialogToSayLater, &time, &duration, face, 
        &shout, &isCaption, captionPosition, gotoLoc, animList );

    player->queuedChatTimeStop = MAX( player->queuedChatTimeStop, ABS_TIME + SEC_TO_ABS(time) );
    player->IAmInEmoteChatMode = 1;

    // Skip all the movement related stuff from saUtilEntSays, since we shouldn't control
    // where the player entity goes...

    sendChatMsgEx(player, player, dialogToSayNow, INFO_CAPTION, duration, 
        player->db_id, player->db_id, 0, captionPosition);

    ///// Add the chat you're supposed to say in the future to the queue
    if( dialogToSayLater )
    {
        QueuedChat * addedQueuedChat;
        int i;

        //We've decided if you get another thing to say, you dump what you were saying
        for( i = 0 ; i < player->queuedChatCount ; i++)
            free(player->queuedChat[i].string);
        player->queuedChatCount = 0;
        //comment the above out to resume multiples.

        addedQueuedChat = dynArrayAdd(&player->queuedChat,sizeof(player->queuedChat[0]),&player->queuedChatCount,&player->queuedChatCountMax,1);
        addedQueuedChat->string = malloc(strlen(dialogToSayLater)+1);
        addedQueuedChat->privateMsg = 1;
        strcpy(addedQueuedChat->string, dialogToSayLater);

        addedQueuedChat->timeToSay = ABS_TIME + SEC_TO_ABS(time);
    }

    if( dialogToSayNow && dialogToSayLater ) //slightly ungainly, we only made a copy of the string if we needed to null terminate it
    {
        free(dialogToSayNow);
        free(dialogToSayLater);
    }
}

/*************************************************************************************
 *    sendDialog function family
 *        Send an "OK" dialog with the specified message to the specified player entity.
 */
//TO DO I removed the send to all ents functionality because it was unused and obviously buggy

void sendDialog( Entity    *ent, const char *message )
{
    if( ENTTYPE(ent) != ENTTYPE_PLAYER )
        return;

    START_PACKET( pak1, ent, SERVER_RECEIVE_DIALOG );
    pktSendString( pak1, message );
    END_PACKET
}

void sendDialogPowerRequest(Entity *e, const char *str, int translateName, const char *name, int id, int timeout)
{
    char *msg = xlateConfirmText(e, translateName, str, name);

    START_PACKET( pak1, e, SERVER_RECEIVE_DIALOG_POWREQ );
    pktSendString( pak1, msg );
    pktSendBitsPack( pak1, 16, id );
    pktSendBitsPack( pak1, 10, timeout );
    END_PACKET
}

void sendDialogLevelChange( Entity *e, int new_level, int player_level )
{
    START_PACKET( pak1, e, SERVER_RECEIVE_DIALOG_TEAMLEVEL );
    pktSendBitsAuto( pak1, new_level );
    pktSendBitsAuto( pak1, player_level );
    END_PACKET
}

void sendDialogIgnore( Entity    *ent, const char *message, int type )
{
    if( ENTTYPE(ent) != ENTTYPE_PLAYER )
        return;

    START_PACKET( pak1, ent, SERVER_RECEIVE_DIALOG_IGNORABLE );
    pktSendString( pak1, message );
    pktSendBitsPack( pak1, 16, type );
    END_PACKET
}


/*
 *    sendDialog function family
 *************************************************************************************/

//************************************************************************************
// Moral Choice dialogs

void sendMoralChoice(Entity * e, const char *leftText, const char *rightText, const char *leftWatermark, const char *rightWatermark, int requireConfirmation)
{
    ScriptVarsTable vars = {0};

    START_PACKET(pak, e, SERVER_SEND_MORAL_CHOICE);
    pktSendString(pak,textStd(saUtilTableLocalize(leftText, &vars)));
    pktSendString(pak,textStd(saUtilTableLocalize(rightText, &vars)));
    pktSendString(pak,leftWatermark);
    pktSendString(pak,rightWatermark);
    pktSendBitsAuto(pak,requireConfirmation);
    END_PACKET
}

// Moral Choice dialogs
//************************************************************************************

// type does not matter unless you are sending an info message
void doSendCmdAndString( Entity *e, int cmd, int type, const char *s )
{
    if( !isWatchedChannel(e, type) )
        return;

    START_PACKET( pak1, e, cmd );

    if ( cmd == SERVER_RECEIVE_INFO_BOX )
    {
        pktSendBitsPack( pak1, 2, type ); // send the info type
    }

    pktSendString( pak1, s );
    END_PACKET
}

/*************************************************************************************
 *    sendInfoBox function family
 *        Send a message to the specified player entity's chat window.
 */
static int petChannelConversion[][2] =
{
        {INFO_COMBAT,            INFO_PET_COMBAT},
        {INFO_DAMAGE,            INFO_PET_DAMAGE},
        {INFO_COMBAT_SPAM,        INFO_PET_COMBAT_SPAM},
        {INFO_HEAL,                INFO_PET_HEAL},
        {INFO_HEAL_OTHER,        INFO_PET_HEAL_OTHER},
        {INFO_COMBAT_SPAM,        INFO_PET_COMBAT_SPAM},
        {INFO_COMBAT_HITROLLS,    INFO_PET_COMBAT_HITROLLS},
};

int getPetChannel( int channel )
{
    int i;
    for( i = ARRAY_SIZE(petChannelConversion)-1; i>=0; i-- )
    {
        if( petChannelConversion[i][0]==channel)
            return petChannelConversion[i][1];
    }
    return channel;
}


void sendInfoBox( Entity *e, int type, const char *s, ... )
{
    va_list    va;
    char* message;

    // Only send messages to players
    if( ENTTYPE(e) != ENTTYPE_PLAYER )
        return;

    va_start( va, s );
    message = localizedPrintfVA(e, s, va);
    va_end( va );

    doSendCmdAndString( e, SERVER_RECEIVE_INFO_BOX, type, message);
}

void sendInfoBoxAlreadyTranslated(Entity *e, int type, const char *message)
{
    doSendCmdAndString(e, SERVER_RECEIVE_INFO_BOX, type, message);
}

// isModHit true + isVictimMessage true = pmod->ptemplate->pchDisplayVictimHit
// isModHit true + isVictimMessage false = pmod->ptemplate->pchDisplayAttackerHit
// isModHit false + isVictimMessage true = ppow->ppowBase->pchDisplayVictimHit
// isModHit false + isVictimMessage false = ppow->ppowBase->pchDisplayAttackerHit
void sendCombatMessage(Entity* e, int chatType, CombatMessageType messageType, int isVictimMessage, int powerCRC, int modIndex, const char *otherEntityName, float float1, float float2)
{
    Entity * sendToMe = erGetEnt(e->erOwner);
    const char *petName = 0;

    if (sendToMe)
    {
          if (e->petName && e->petName[0])
             petName = e->petName;
         else if(e->name && *e->name)
             petName = e->name;
         else
             petName = localizedPrintf(sendToMe, npcDefList.npcDefs[e->npcIndex]->displayName );

        chatType = getPetChannel(chatType);
    }
    else
    {
        sendToMe = e;
    }

    if (!isWatchedChannel(sendToMe, chatType))
    {
        return;
    }

    START_PACKET(pak, sendToMe, SERVER_COMBAT_MESSAGE);

    pktSendBitsAuto(pak, chatType);
    pktSendBitsAuto(pak, messageType);

    if (isVictimMessage)
    {
        pktSendBits(pak, 1, 1);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    pktSendBitsAuto(pak, powerCRC);
    pktSendBitsAuto(pak, modIndex);
    pktSendString(pak, otherEntityName);
    pktSendF32(pak, float1);
    pktSendF32(pak, float2);

    if (petName)
    {
        pktSendBits(pak, 1, 1);
        pktSendString(pak, petName);
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }

    END_PACKET;
}

static char *xlateConfirmText(Entity *e, int translateName, const char* messageID, ...)
{
    static int initted = 0;
    static Array* ConfirmMessage[2];
    static char message[1024];        // WARNING! Watch for overflow!
    int messageLength;

    translateName = translateName ? 1 : 0;
    message[0] = 0;

    // If no messages are given, do nothing.
    if(!messageID || messageID[0] == '\0')
        return message;

    if(!initted)
    {
        ConfirmMessage[0] = msCompileMessageType("{PlayerSource, %s}");
        ConfirmMessage[1] = msCompileMessageType("{PlayerSource, %r}");
        initted = 1;
    }

    VA_START(va, messageID);
    messageLength = msxPrintfVA(svrMenuMessages, message, ARRAY_SIZE(message), e->playerLocale, messageID, ConfirmMessage[translateName], NULL, translateFlag(e), va);
    VA_END();

    return message;
}

void sendServerWaypoint(Entity* e, int id, int set, int compass, int pulse, const char* icon, const char* iconB, const char* name, const char* mapname, Vec3 loc, float angle)
{
    START_PACKET(pak, e, SERVER_SET_WAYPOINT);
    pktSendBits(pak, 1, set);
    pktSendBitsPack(pak, 1, id);
    if (set)
    {
        pktSendBits(pak, 1, compass);
        pktSendBits(pak, 1, pulse);
        if (icon) {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, icon);
        } else {
            pktSendBits(pak, 1, 0);
        }
        if (iconB) {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, iconB);
        } else {
            pktSendBits(pak, 1, 0);
        }
        if (name) {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, name);
        } else {
            pktSendBits(pak, 1, 0);
        }
        if (mapname) {
            pktSendBits(pak, 1, 1);
            pktSendString(pak, mapname);
        } else {
            pktSendBits(pak, 1, 0);
        }
        if (angle != 0.0f) {
            pktSendBits(pak, 1, 1);
            pktSendF32(pak, angle);
        } else {
            pktSendBits(pak, 1, 0);
        }


        pktSendF32(pak, loc[0]);
        pktSendF32(pak, loc[1]);
        pktSendF32(pak, loc[2]);
    }
    END_PACKET
}

void sendPopHelp( Entity * e, const char *tag )
{
    START_PACKET(pak, e, SERVER_SEND_POP_HELP_TAG);
    pktSendString(pak, tag);
    END_PACKET
}

void sendPopHelpReset( Entity * e )
{
    START_PACKET(pak, e, SERVER_SEND_POP_HELP_RESET);
    END_PACKET
}

void sendIncarnateTrialStatus(Entity *e, TrialStatus status)
{
    START_PACKET(pak, e, SERVER_INCARNATE_TRIAL_STATUS);
    pktSendBitsAuto(pak, status);
    END_PACKET
}

/* End of File */
