#ifndef _CLIENTCOMM_H
#define _CLIENTCOMM_H

#include <utilitieslib/network/net_structdefs.h>
#include <utilitieslib/utils/timing.h>

typedef struct EntPlayer EntPlayer;

typedef enum{
    NST_None,
    NST_ReceivedUnreliable,
    NST_ReceivedReliable,        // Impossible to determine for now.
    NST_LostIncomingPacket,
    NST_LostOutgoingPacket,
} NetStatType;

typedef enum{
    TroubledComm_None,
    TroubledComm_Mapserver,
    TroubledComm_Dbserver,    
    TroubledComm_DroppedPackets,
    TroubledComm_Count
} TroubledCommReason;

typedef enum
{
    CS_NONE,
    CS_CONNECTED,
    CS_NOT_RESPONSIVE,
    CS_DISCONNECTED,
} ConnectionStatus;

typedef struct
{
    NetStatType type;
    int        bytes;
    F32        time;
} NetStat;

extern Packet *client_input_pak;
extern NetLink comm_link;
extern int g_showMapLoadingMessage;

#define START_INPUT_PACKET(pakx, cmdNumber) {            \
    Packet* pakx = client_input_pak;                    \
    START_BIT_COUNT(client_input_pak, #cmdNumber);        \
    pktSendBitsPack(client_input_pak, 1, cmdNumber);
    
#define END_INPUT_PACKET                                \
    STOP_BIT_COUNT(client_input_pak);                    \
}

#define EMPTY_INPUT_PACKET(cmdNumber){                    \
    START_BIT_COUNT(client_input_pak, #cmdNumber);        \
    pktSendBitsPack(client_input_pak, 1, cmdNumber);    \
    STOP_BIT_COUNT(client_input_pak);                    \
}

#define RECV_HISTORY_SIZE 512 // must be power of 2
extern NetStat recv_hist[RECV_HISTORY_SIZE];
extern int recv_idx;

void commInit(void);
void checkit2(void);
void commSendPkt(Packet *pak);
U32 commID(void);
U32 commAck(void);
void commResetControlState(void);
int commConnected(void);
int commDisconnect(void);
void commAddInput(char *s);
void commFreeInputPak(void);
void commNewInputPak(void);
void commSendInput(void);
void commGetStats(NetLink *link, Packet* pak);
void commServerWaitOK(char *mapname);
void commServerMapXfer(Packet *pak_in);
void handleForcedLogout(Packet *pak);
int commStart(char *ip_str,int port,int connect_cookie);
void commGetData(void);
int commCheck(int lookfor);
int commReqScene(int isInitialLogin);
int commReqShortcuts(void);
int commConnect(char *addr,int port,int cookie);
int tryConnect(void);
int commTimeSinceLastReceive(void);
ConnectionStatus commCheckConnection(void);
void commSendQuitGame(int abort_quit);
void handleClientArenaPlayerUpdate(Packet * pak);
void clearCutScene( void );
void sendShardCmd(char * cmd, char *fmt,...);
int commGetCharacterCountsAsync(void);
int commGetAccountServerCatalogAsync(void);
int commGetAccountServerInventory(void);
void commSendPopHelp(unsigned int pop_help_event, unsigned int new_state);
void commSendPopHelpLatest(EntPlayer *pl);
void commLogPackets(int value);

void commTickTroubledMode();
bool commIsInTroubledMode();
void commSetTroubledMode(TroubledCommReason reason);

#endif
