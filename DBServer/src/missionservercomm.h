#pragma once

typedef struct Packet Packet;
typedef struct NetLink NetLink;

void missionserver_commInit(void);
int missionserver_secondsSinceUpdate(void);
NetLink* missionserver_getLink(void);

void missionserver_db_remoteCall(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_publishArc(Packet *pak_in, NetLink *link);
void missionserver_db_requestSearchPage(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_requestArcInfo(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_requestBanStatus(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_requestArcData(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_requestArcDataOtherUser(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_requestInventory(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_claimTickets(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_buyItem(int mapid, Packet *pak_in, NetLink *link);
void missionserver_db_requestAllArcs(int mapid, Packet *pak_in, NetLink *link);
void missionserver_forceLinkReset(void);


#define DB_MISSIONSERVER_MAX_SENDQUEUE_SIZE_DEFAULT            1000000
#define DB_MISSIONSERVER_MAX_SENDQUEUEPUBLISH_SIZE_DEFAULT    500000
