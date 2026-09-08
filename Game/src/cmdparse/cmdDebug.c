#include <utilitieslib/stdtypes.h>
#include "cmdDebug.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils/position.h"
#include <utilitieslib/components/StashTable.h>
#include <limits.h>
#include <utilitieslib/assert/assert.h>
#include "group/grouptrack.h"
#include "gridcoll/gridcoll.h"
#include "edit/edit_select.h"
#include "edit/edit_drawlines.h"
#include <utilitieslib/network/netio.h>
#include "entity/entDebug.h"
#include <utilitieslib/utils/utils.h>
#include "render/model.h"

BeaconDebugLine**    beaconConnection = NULL;
int                    beacConn_showWhenMouseDown;

MP_DEFINE(BeaconDebugLine);

static BeaconDebugLine* getNewLine(){
    BeaconDebugLine* newLine;
    
    MP_CREATE(BeaconDebugLine,1000);
    newLine = MP_ALLOC(BeaconDebugLine);

    eaPush(&beaconConnection, newLine);
    
    return newLine;
}

void destroyBeaconDebugLine(void* lineData)
{
    BeaconDebugLine* line = (BeaconDebugLine*)lineData;
    MP_FREE(BeaconDebugLine, line);
}

void ShowBeaconDebugInfo(Packet* pak){
    Vec3 sourcePos;
    Vec3 targetPos;
    CollInfo info;
    DefTracker* tracker;
    int infoType;
    U32 color;
    U32 color2;
    BeaconDebugLine* newLine;
    
    while(!pktEnd(pak)){
        // Get the operation type.
        
        infoType = pktGetBitsPack(pak, 2);

        switch(infoType){
            case 0:{
                Vec3 tempLocation;
                
                // Highlight a beacon.
                
                sourcePos[0] = pktGetF32(pak);
                sourcePos[1] = pktGetF32(pak);
                sourcePos[2] = pktGetF32(pak);
                color = pktGetBits(pak, 32);
                
                copyVec3(sourcePos, tempLocation);

                tempLocation[1] += 5;
                
                if(collGrid(NULL, sourcePos, tempLocation, &info, 0, COLL_EDITONLY | COLL_BOTHSIDES)){
                    tracker = info.node;
                    
                    if(tracker->parent){
                        tracker = tracker->parent;
                        
                        if(tracker->def->has_beacon){
                            selAdd(tracker,trackerRootID(tracker),0,0);
                            editRefSelect(sel_list[getSelIndex(tracker)].def_tracker, color & 0xffffff);
                        }
                    }
                }
                else if(0){
                    int i;
                    
                    newLine = getNewLine();
                    
                    copyVec3(sourcePos, newLine->a);
                    copyVec3(sourcePos, newLine->b);
                    
                    newLine->b[1] += 5;

                    newLine->colorARGB1 = newLine->colorARGB2 = 0xff0000ff;

                    for(i = 0; i < 100; i++){
                        newLine = getNewLine();
                        
                        copyVec3(beaconConnection[eaSize(&beaconConnection)-2]->b, newLine->a);
                        copyVec3(sourcePos, newLine->b);
                        
                        newLine->b[0] += i * sin(i/5.f) * cos(i) / 10.f;
                        newLine->b[1] += i + 6;
                        newLine->b[2] += i * sin(i/5.f) * sin(i) / 10.f;

                        newLine->colorARGB1 = newLine->colorARGB2 = 0xff0000ff;
                    }
                }
                break;
            }

            case 1:{
                // Draw a line.
                
                sourcePos[0] = pktGetF32(pak);
                sourcePos[1] = pktGetF32(pak);
                sourcePos[2] = pktGetF32(pak);
                targetPos[0] = pktGetF32(pak);
                targetPos[1] = pktGetF32(pak);
                targetPos[2] = pktGetF32(pak);
                color = pktGetBits(pak, 32);
                color2 = pktGetBits(pak, 1) ? pktGetBits(pak, 32) : color;

                // Add the line to the list.
                
                newLine = getNewLine();
                
                copyVec3(sourcePos, newLine->a);
                copyVec3(targetPos, newLine->b);

                newLine->colorARGB1 = color;
                newLine->colorARGB2 = color2;

                break;
            }
            
            case 2:{
                sourcePos[0] = pktGetF32(pak);
                sourcePos[1] = pktGetF32(pak);
                sourcePos[2] = pktGetF32(pak);
                color = pktGetIfSetBits(pak, 32);
                
                {
                    #define ADDLINE(pt1, pt2, clr){                            \
                        newLine = getNewLine();                                \
                        copyVec3(pt1, newLine->a);                            \
                        copyVec3(pt2, newLine->b);                            \
                        newLine->colorARGB1 = clr;                            \
                        newLine->colorARGB2 = clr;                            \
                    }

                    F32 height = 2;
                    F32 size = 0.2;
                    Vec3 o;
                    Vec3 a, b, c;
                    
                    //sourcePos[1] += height;

                    copyVec3(sourcePos, o);

                    copyVec3(o, a);
                    copyVec3(o, b);
                    a[1] -= height;
                    b[1] -= size;
                    ADDLINE(a, b, color);

                    copyVec3(o, a);
                    copyVec3(o, b);
                    a[1] -= height;
                    b[1] -= height - 0.2;
                    b[0] += 0.1;
                    ADDLINE(a, b, color);
                    a[1] += 0.2;
                    ADDLINE(a, b, color);

                    //b[1] -= 0.2;
                    //ADDLINE(a, b, 0xffff00ff);

                    if(0){
                        // Draw the little knob on top.
                        
                        copyVec3(o, a);
                        copyVec3(o, b);
                        a[0] += size;
                        b[0] -= size;
                        ADDLINE(a, b, 0xffffaaaa);
                        copyVec3(o, c);
                        c[2] += size;
                        ADDLINE(a, c, 0xffffaaaa);
                        ADDLINE(b, c, 0xffffaaaa);
                        c[2] -= 2 * size;
                        ADDLINE(a, c, 0xffffaaaa);
                        ADDLINE(b, c, 0xffffaaaa);


                        copyVec3(o, a);
                        copyVec3(o, b);
                        a[1] += size;
                        b[1] -= size;
                        ADDLINE(a, b, 0xffaaffaa);
                        copyVec3(o, c);
                        c[0] += size;
                        ADDLINE(a, c, 0xffaaffaa);
                        ADDLINE(b, c, 0xffaaffaa);
                        c[0] -= 2 * size;
                        ADDLINE(a, c, 0xffaaffaa);
                        ADDLINE(b, c, 0xffaaffaa);

                        copyVec3(o, a);
                        copyVec3(o, b);
                        a[2] += size;
                        b[2] -= size;
                        ADDLINE(a, b, 0xffaaaaff);
                        copyVec3(o, c);
                        c[1] += size;
                        ADDLINE(a, c, 0xffaaaa0ff);
                        ADDLINE(b, c, 0xffaaaaff);
                        c[1] -= 2 * size;
                        ADDLINE(a, c, 0xffaaaaff);
                        ADDLINE(b, c, 0xffaaaaff);
                    }
                }
                
                break;
            }
            
            default:{
                assert(0 && "Bad operation code in ShowBeaconDebugInfo.");
                break;
            }
        }
    }
}

void cmdOldDebugHandle(Packet* pak){
    char* command = pktGetString(pak);
    
    if(!stricmp(command, "ShowBeaconDebugInfo")){
        ShowBeaconDebugInfo(pak);
    }
    else if(!stricmp(command, "AStarRecording")){
        entDebugReceiveAStarRecording(pak);
    }
}
