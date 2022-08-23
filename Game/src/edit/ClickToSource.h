#ifndef CLICKTOSOURCE_H
#define CLICKTOSOURCE_H

#include <utilitieslib/stdtypes.h>
#include "ClickToSourceFlags.h"

typedef struct Packet Packet;

extern unsigned int g_ctsstate;

#define CTS_SINGLE_CLICK        (g_ctsstate & CTS_SINGLECLICK)
#define CTS_SHOW_STORY            (g_ctsstate & CTS_STORY)
#define CTS_SHOW_FX                (g_ctsstate & CTS_FX)
#define CTS_SHOW_NPC            (g_ctsstate & CTS_NPC)
#define CTS_SHOW_VILLAINDEF        (g_ctsstate & CTS_VILLAINDEF)
#define CTS_SHOW_BADGES            (g_ctsstate & CTS_BADGES)
#define CTS_SHOW_SEQUENCER        (g_ctsstate & CTS_SEQUENCER)
#define CTS_SHOW_ENTTYPE        (g_ctsstate & CTS_ENTTYPE)
#define CTS_SHOW_POWERS            (g_ctsstate & CTS_POWERS)

typedef enum CTSDisplayType
{
    CTS_TEXT_REGULAR,
    CTS_TEXT_DEBUG,
    CTS_TEXT_REGULAR_3D,
    CTS_TEXT_DEBUG_3D,
} CTSDisplayType;

int ClickToSourceDisplay(F32 x, F32 y, F32 z, F32 yShift, int color, const char* filename, const char* displayString, CTSDisplayType displayType);

int WriteAnObject( const char * libraryPieceName );

#endif
