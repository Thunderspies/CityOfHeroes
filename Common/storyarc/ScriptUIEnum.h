#ifndef SCRIPTUIENUM_H
#define SCRIPTUIENUM_H

#include <utilitieslib/utils/structDefines.h>

//these enums cover all the widgets that have
//data associated with them that can be updated
typedef enum ScriptUIType
{
    BadWidget,
    MeterWidget,
    TimerWidget,
    CollectItemsWidget,
    TitleWidget,
    ListWidget,
    DistanceWidget,

    // New for the Zone event UI
    MeterExWidget,            // Meter widget using custom art resources
    TextMeterWidget,        // Meter widget with text on the left
    ProgressWidget,            // "Text" on the left "nn/mm" on the right - shows progress
    TextWidget,                // Simple text widget
    KarmaWidget,
} ScriptUIType;

//------------------------------------------------------------------------
// Team-Up Teleporter Trial Status info
//------------------------------------------------------------------------

typedef enum TrialStatus
{
    trialStatus_None = 0,
    trialStatus_Success,
    trialStatus_Failure,
    trialStatus_Count,
} TrialStatus;

extern StaticDefineInt TrialStatusEnum[];

#endif
