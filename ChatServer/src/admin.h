#ifndef __ADMIN_H__
#define __ADMIN_H__

#include <utilitieslib/network/netio.h>
#include "shardnet.h"


enum
{
    UPLOAD_NONE = 0,
    UPLOAD_READY                    = (1 << 0),
    UPLOAD_USERLIST_INPROGRESS        = (1 << 1),
    UPLOAD_USERLIST_COMPLETE        = (1 << 2),
    UPLOAD_CHANNELLIST_INPROGRESS    = (1 << 3),
    UPLOAD_CHANNELLIST_COMPLETE        = (1 << 4),
    UPLOAD_WAITING_FOR_ACK            = (1 << 5),
    UPLOAD_COMPLETE                    = (UPLOAD_READY | UPLOAD_USERLIST_COMPLETE | UPLOAD_CHANNELLIST_COMPLETE),
};

extern ClientLink ** admin_clients;
extern ClientLink * admin_upload;        // ChatAdmin that is currently being sent full channel/user info


int adminHandleClientMsg(Packet *pak,int cmd, NetLink *link);
void adminTick();
bool adminUserOnline(User * user);


#endif //__ADMIN_H__
