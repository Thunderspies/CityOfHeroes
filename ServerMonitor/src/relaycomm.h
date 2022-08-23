#ifndef _RELAYCOMM_H
#define _RELAYCOMM_H

#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/textparser.h>
#include "comm_backend.h"
#include "serverMonitorCmdRelay.h"


typedef struct
{
    NetLink        *link;
    CmdRelayCon * relayCon;
} CmdRelayClientLink;

void cmdRelayInit(void);
void cmdRelayUpdate(void);


extern TokenizerParseInfo CmdRelayConNetInfo[]; 

#ifdef RELAYCOMM_PARSE_INFO_DEFS


#include <utilitieslib/utils/ListView.h>
#include "container.h"
#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/utils/structNet.h>




TokenizerParseInfo CmdRelayConNetInfo[] = 
{
    { "Hostname",            TOK_FIXEDSTR(CmdRelayCon, hostname), 0,    TOK_FORMAT_LVWIDTH(83)},
    { "IpAddress",            TOK_FIXEDSTR(CmdRelayCon, ipAddress), 0, TOK_FORMAT_LVWIDTH(90)},
    { "RelayType",            TOK_FIXEDSTR(CmdRelayCon, typeStr), 0, TOK_FORMAT_LVWIDTH(70)},
    { "Version",            TOK_FIXEDSTR(CmdRelayCon, version), 0, TOK_FORMAT_LVWIDTH(83)},
    { "Protocol",            TOK_INT(CmdRelayCon, protocol, 0), 0,        TOK_FORMAT_LVWIDTH(83)},
    { "Last Update (sec)",    TOK_INT(CmdRelayCon, lastUpdate, 0), 0,        TOK_FORMAT_LVWIDTH(60)},
    { "Crashed Maps",        TOK_INT(CmdRelayCon, crashedMapCount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Mapservers",            TOK_INT(CmdRelayCon, mapserverCount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Launchers",            TOK_INT(CmdRelayCon, launcherCount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "DbServers",            TOK_INT(CmdRelayCon, dbserverCount, 0),    0, TOK_FORMAT_LVWIDTH(40)},
    { "ArenaServers",        TOK_INT(CmdRelayCon, arenaserverCount, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "Crashed ArenaServers",TOK_INT(CmdRelayCon, crashedArenaCount, 0), 0, TOK_FORMAT_LVWIDTH(40)},
    { "LogServers",            TOK_INT(CmdRelayCon, logserverCount, 0), 0,    TOK_FORMAT_LVWIDTH(40)},
    { "Status",                TOK_FIXEDSTR(CmdRelayCon, statusStr), 0,        TOK_FORMAT_LVWIDTH(83)},
    { "Message",            TOK_FIXEDSTR(CmdRelayCon, lastMsg), 0,        TOK_FORMAT_LVWIDTH(255)},
    { 0 }
};




#endif    // RELAYCOMM_PARSE_INFO_DEFS

enum {    CMDRELAY_ACTION_STATUS_NOTASK = 0,
        CMDRELAY_ACTION_STATUS_BUSY,
        CMDRELAY_ACTION_STATUS_SUCCESS,
        CMDRELAY_ACTION_STATUS_FAILURE};

void receiveProtocolFromClient(Packet * pak, NetLink * link);
void requestProtocolFromClient(NetLink * link);
void sendRelayExe(CmdRelayClientLink * client, char * data, int size);

#endif    // _RELAYCOMM_H
