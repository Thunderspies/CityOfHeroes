//
// clientcommreceive.h 1/2/03
//
#ifndef _CLIENTCOMMRECIEVE_H
#define _CLIENTCOMMRECIEVE_H

typedef struct Packet Packet;

void receiveVar( Packet *pak );
void receiveControlPlayer( Packet *pak );
void receivePrintEnemyStats( Packet *pak );
void receiveTimeDownTimer( Packet *pak );
void receiveInfoBox( Packet *pak );
void receiveDoorMsg( Packet *pak );
void receiveEnterStore( Packet *pak );
void receiveEnterVault( Packet *pak );
void receiveContactDialog( Packet *pak );
void receiveContactDialogClose( Packet *pak );
void receiveContactDialogOk( Packet *pak );
void receiveContactDialogYesNo( Packet *pak );
void receiveMissionEntryText( Packet *pak );
void receiveMissionExitText( Packet *pak );
void receiveTeamCompletePrompt( Packet *pak );
void receiveBrokerAlert( Packet *pak );
void receiveDeadNoGurney( Packet *pak );
void receiveFaceEntity( Packet *pak );
void receiveFaceLocation( Packet* pak );
void receiveVisitLocations(Packet *pak);
void receiveLevelUp(Packet *pak);
void receiveLevelUpRespec(Packet *pak);
void receiveNewTitle(Packet *pak);
void receiveVisitMapCells(Packet *pak);
void receiveResendWaypointRequest(Packet* pak);
void receiveWaypointUpdate(Packet* pak);
void handleFloatingInfo(int iOwnerShownOver,char *pch,int iStyle,float fDelay, U32 color);
void receiveBugReport(Packet * pak);
void receiveSelectBuild(Packet * pak);
void receiveCutSceneUpdate( Packet *pak );
void receiveShardVisitorData(Packet *pak);
void receiveShardVisitorJump(Packet *pak);
void receiveShowContactFinderWindow(Packet *pak);
void receiveWebStoreOpenProduct(Packet *pak);

#endif