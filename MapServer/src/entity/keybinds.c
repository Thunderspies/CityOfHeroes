#include "keybinds.h"
#include <utilitieslib/network/netio.h>
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include "win/input.h"
#include "time.h"
#include <utilitieslib/utils/error.h>
#include "entity/entVarUpdate.h"
#include "language/langServerUtil.h"
#include "gameComm/sendToClient.h"
#include <utilitieslib/utils/utils.h>

//-------------------------------------------------------------------------------

void keybinds_send( Packet *pak, char * profile, ServerKeyBind * kbp )
{
    int i;

    pktSendString( pak, profile );

    for( i = 0; i < BIND_MAX; i++ )
    {
        pktSendString( pak, kbp[i].command );
        pktSendBits( pak, 32, kbp[i].key );
        pktSendBits( pak, 32, kbp[i].modifier );
    }
}

void keybind_receive( Packet *pak, Entity *e )
{
    char profile[32], command[256] = {0};
    int i, key, mod;

    Strncpyt( profile, pktGetString( pak ));
    key    = pktGetBits( pak, 32 );
    mod    = pktGetBits( pak, 32 );

    if( pktGetBits(pak, 1) )
        Strncpyt( command, pktGetString( pak ));

    if( profile[0] == '\0' || stricmp( profile, e->pl->keyProfile ) != 0 )
    {
        if( profile[0] != '\0' )
            strcpy( e->pl->keyProfile, profile );

        sendChatMsg( e, localizedPrintf( e, "CouldNotActionReason", "MapKey", "WrongProfileTryAgain" ), INFO_USER_ERROR, 0 );

        return;
    }

    // first see if we have this key saved already
    for( i = 0; i < BIND_MAX; i++ )
    {
        if( e->pl->keybinds[i].key == (key & 0xff) && e->pl->keybinds[i].modifier == mod )
        {
            if( command )
                strcpy( e->pl->keybinds[i].command, command);
            else
                e->pl->keybinds[i].command[0] = '\0';

            return;
        }
    }

    // guess we have to add it to list, make sure we have room
    if( e->pl->keybind_count >= 255 )
    {
        sendChatMsg( e, localizedPrintf(e,"MaxKeybinds"), INFO_USER_ERROR, 0 );
        return;
    }

    e->pl->keybinds[e->pl->keybind_count].key = key;
    e->pl->keybinds[e->pl->keybind_count].modifier = mod;
    strcpy( e->pl->keybinds[e->pl->keybind_count].command, command);
    e->pl->keybind_count++;
}


void keybind_receiveClear( Packet *pak, Entity *e )
{
    char profile[32];
    int i, key, mod;

    Strncpyt( profile, pktGetString( pak ));
    key    = pktGetBits( pak, 32 );
    mod    = pktGetBits( pak, 32 );

    // if we already have this key saved as bound, overwrite it
    for( i = 0; i < e->pl->keybind_count; i++ )
    {
        if( e->pl->keybinds[i].key == (key & 0xff) && e->pl->keybinds[i].modifier == mod )
        {
            strcpy( e->pl->keybinds[i].command, "nop" );
            return;
        }
    }

    // otherwise, explicitly un-bind it
    if( e->pl->keybind_count >= 255 )
    {
        sendChatMsg( e, localizedPrintf(e,"MaxKeybinds"), INFO_USER_ERROR, 0 );
        return;
    }

    e->pl->keybinds[e->pl->keybind_count].key = key;
    e->pl->keybinds[e->pl->keybind_count].modifier = mod;
    strcpy( e->pl->keybinds[e->pl->keybind_count].command, "nop" );
    e->pl->keybind_count++;
}

void keybind_receiveReset( Entity *e )
{
    e->pl->keybind_count = 0;
    memset( e->pl->keybinds, 0, sizeof(ServerKeyBind)*BIND_MAX );
    sendChatMsg( e, localizedPrintf(e,"KeybindsReset"), INFO_USER_ERROR, 0 );
}


//
//
void keyprofile_receive( Packet *pak, Entity *e )
{
    Strncpyt( e->pl->keyProfile, pktGetString( pak ));
}




