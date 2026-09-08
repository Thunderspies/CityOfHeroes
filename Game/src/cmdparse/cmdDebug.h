/* File cmdDebug.h
 *    Defines an interface for calling command handlers via text messages.
 *
 */

#ifndef CMDDEBUG_H
#define CMDDEBUG_H

typedef struct Packet Packet;

typedef struct BeaconDebugLine {
    Vec3    a;
    Vec3    b;
    U32        colorARGB1;
    U32        colorARGB2;
} BeaconDebugLine;

extern BeaconDebugLine**    beaconConnection;
extern int                    beacConn_showWhenMouseDown;

void cmdOldDebugHandle(Packet* pak);
void destroyBeaconDebugLine(void* lineData);

#endif
