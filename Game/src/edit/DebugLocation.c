#include "edit/DebugLocation.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/earray.h>
#include "render/renderprim.h"
#include <utilitieslib/network/net_packet.h>
#include "graphics/camera.h"
#include <utilitieslib/utils/rgb_hsv.h>

static DebugLocation** debugLocations = 0;
static U32 currentId = 0;

U32 DebugLocationCreate(Vec3 pos, U32 col)
{
    DebugLocation* newLoc = malloc(sizeof(DebugLocation));
    Vec3 rgb;

    newLoc->uniqueId = ++currentId;
    copyVec3(pos, newLoc->position);

    newLoc->alpha = col & 255;
    newLoc->rgb[0] = (col >> 24);
    newLoc->rgb[1] = ((col >> 16) & 0xff);
    newLoc->rgb[2] = ((col >> 8) & 0xff);

    rgb[0] = newLoc->rgb[0]/255.0;
    rgb[1] = newLoc->rgb[1]/255.0;
    rgb[2] = newLoc->rgb[2]/255.0;
    rgbToHsv(rgb, newLoc->hsv);
    newLoc->vModifier = (newLoc->hsv[2] < 190/255.0)?2:-2;
    
    eaPush(&debugLocations, newLoc);
    return currentId;
}

int DebugLocationDestroy(U32 id)
{
    int i, n = eaSize(&debugLocations);
    for (i = n-1; i >= 0; i--)
    {
        if (debugLocations[i]->uniqueId == id)
        {
            free(debugLocations[i]);
            eaRemove(&debugLocations, i);
            return 1;
        }
    }
    return 0;
}

#define DL_SIDES 12
#define DL_RADIUS 2
#define DL_ANGLE 2*PI/DL_SIDES
#define DL_HEIGHT 10
#define DL_ALPHADISTSQ 25000
#define DL_MINALPHA 50
#define DL_MAXALPHA 255
#define DL_GLOWTICK 60

static int dlDisplayTick = 0;

static void DebugLocationDisplay(DebugLocation* location)
{
    Vec3 p1, p2, p3, p4, hsv, rgb;
    int i, color;
    int alpha = location->alpha;
    int colorAdjust = abs(DL_GLOWTICK/2 - (dlDisplayTick%DL_GLOWTICK));
    int distSq = distance3Squared(cam_info.cammat[3], location->position);

    // As you get closer, turn down the alpha
    if (distSq <= DL_ALPHADISTSQ)
        alpha = MAX(DL_MINALPHA, DL_MAXALPHA*distSq/(float)DL_ALPHADISTSQ);

    // Make the location seem to brighten
    copyVec3(location->hsv, hsv);
    hsv[2] += (colorAdjust*location->vModifier)/255.0;
    hsvToRgb(hsv, rgb);
    color = (alpha << 24) + ((int)(255*rgb[0]) << 16) + ((int)(255*rgb[1]) << 8) + (int)(255*rgb[2]);

    for (i = 0; i < DL_SIDES; i++)
    {
        p1[0] = location->position[0] + DL_RADIUS*sin(DL_ANGLE*(i+1));
        p1[1] = location->position[1] + DL_HEIGHT;
        p1[2] = location->position[2] + DL_RADIUS*cos(DL_ANGLE*(i+1));

        p2[0] = p1[0];
        p2[1] = location->position[1];
        p2[2] = p1[2];

        p3[0] = location->position[0] + DL_RADIUS*sin(DL_ANGLE*i);
        p3[1] = location->position[1];
        p3[2] = location->position[2] + DL_RADIUS*cos(DL_ANGLE*i);

        p4[0] = p3[0];
        p4[1] = p1[1];
        p4[2] = p3[2];

        drawTriangle3D_3(p1, 0x00ffffff, p2, color, p3, color);
        drawTriangle3D_3(p1, 0x00ffffff, p3, color, p4, 0x00ffffff);
    }
}

void DebugLocationDisplayAll()
{
    int i, n = eaSize(&debugLocations);
    for (i = 0; i < n; i++)
        DebugLocationDisplay(debugLocations[i]);
    dlDisplayTick++;
}

void DebugLocationShutdown()
{
    int i, n = eaSize(&debugLocations);
    for (i = n-1; i >= 0; i--)
    {
        free(debugLocations[i]);
        eaRemove(&debugLocations, i);
    }
    eaDestroy(&debugLocations);
}

int DebugLocationCount()
{
    return eaSize(&debugLocations);
}

void DebugLocationReceive(Packet* pak)
{
    Vec3 newPosition;
    int removeOld = pktGetBits(pak, 1);
    int i, color, count = pktGetBitsPack(pak, 1);

    if (removeOld)
        DebugLocationShutdown();

    for (i = 0; i < count; i++)
    {
        newPosition[0] = pktGetF32(pak);
        newPosition[1] = pktGetF32(pak);
        newPosition[2] = pktGetF32(pak);
        color = pktGetBits(pak, 32);
        DebugLocationCreate(newPosition, color);
    }
}