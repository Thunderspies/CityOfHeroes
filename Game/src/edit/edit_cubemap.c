
#include <utilitieslib/stdtypes.h>
#include "edit_cubemap.h"

#include <utilitieslib/version/AppRegCache.h>
#include "edit/edit_net.h"
#include "edit/edit_select.h"
#include "editorUI.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/RegistryReader.h>

static int s_dialogID = -1;

static enum {
    DIALOG_NONE,
    DIALOG_ACTIVE,
    DIALOG_CANCEL,
    DIALOG_OK,
} s_dialogState = DIALOG_NONE;

static char s_genSizeBuffer[5];
static char s_captureSizeBuffer[5];
static char s_angleBuffer[16];
static char s_timeBuffer[6];
static int s_genSize;
static int s_captureSize;
static F32 s_angle;
static F32 s_time;
static bool s_validGenSize;
static bool s_validCaptureSize;
static bool s_validAngle;
static bool s_validTime;

void validateSize(char *buffer, const char *sizeBuffer, int *size, bool *valid) {
    *valid = sscanf(sizeBuffer, "%d", size) == 1 && isPower2(*size);

    if (*valid)
        buffer[0] = '\0';
    else
        sprintf(buffer, "Error: Must be a power of 2");
}

void validateGenSize(char *buffer) {
    validateSize(buffer, s_genSizeBuffer, &s_genSize, &s_validGenSize);
}

void validateCaptureSize(char *buffer) {
    validateSize(buffer, s_captureSizeBuffer, &s_captureSize, &s_validCaptureSize);
}

bool isAngleExcessive() {
    float filterSupport;

    if (!s_validAngle || ! s_validCaptureSize)
        return false;

    // This is only approximate, since a texel's subtended angle varies across the cube face, but
    //  it should serve the purpose.
    filterSupport = s_captureSize * s_angle / 90.f;
    return filterSupport > 20.0;
}

void validateAngle(char *buffer) {
    s_validAngle = sscanf(s_angleBuffer, "%f", &s_angle) == 1 && s_angle >= 0.0f &&
        s_angle <= 360.0f;

    if (s_validAngle)
    {
        if (isAngleExcessive())
            sprintf(buffer, "Warning: Capture resolution is very high for this angle.");
        else
            buffer[0] = '\0';
    }
    else
        sprintf(buffer, "Error: Must be a number between 0.0 and 360.0");
}

void validateTime(char *buffer) {
    s_validTime = sscanf(s_timeBuffer, "%f", &s_time) == 1 && s_time >= 0.0f && s_time < 24.0f;

    if (s_validTime)
        buffer[0] = '\0';
    else
        sprintf(buffer, "Error: Must be a number greater than or equal to 0.0, and less than 24.0");
}

static void onDialogOK() {
    if (s_validGenSize && s_validCaptureSize && s_validAngle && s_validTime)
        s_dialogState = DIALOG_OK;
}

static void onDialogCancel() {
    s_dialogState = DIALOG_CANCEL;
}

void edit_openAdjustCubemapDialog() {
    int x = 0, y = 0;
    RegReader rr;

    if (s_dialogID >= 0)
        return;

    s_dialogID = editorUICreateWindow("Adjust Cubemap");

    if (s_dialogID < 0)
        s_dialogState = DIALOG_CANCEL;

    rr = createRegReader();
    initRegReader(rr, regGetAppKey());
    if (!rrReadInt(rr, "AdjustCubemapDialogX", &x))
        x = 0;
    if (!rrReadInt(rr, "AdjustCubemapDialogY", &y))
        y = 0;
    destroyRegReader(rr);

    editorUISetPosition(s_dialogID, x, y);

    editorUISetModal(s_dialogID, 1);

    sprintf_s(s_genSizeBuffer,sizeof(s_genSizeBuffer),"%d", sel_list[0].def_tracker->def->cubemapGenSize);
    sprintf_s(s_captureSizeBuffer,sizeof(s_captureSizeBuffer),"%d", sel_list[0].def_tracker->def->cubemapCaptureSize);
    sprintf_s(s_angleBuffer,sizeof(s_angleBuffer),"%1.1f", sel_list[0].def_tracker->def->cubemapBlurAngle);
    sprintf_s(s_timeBuffer,sizeof(s_timeBuffer),"%1.1f", sel_list[0].def_tracker->def->cubemapCaptureTime);
    editorUIAddTextEntry(s_dialogID, s_genSizeBuffer, sizeof(s_genSizeBuffer), NULL, "Final Size:");
    editorUIAddDynamicText(s_dialogID, validateGenSize);
    editorUIAddTextEntry(s_dialogID, s_captureSizeBuffer, sizeof(s_captureSizeBuffer), NULL, "Capture Size:");
    editorUIAddDynamicText(s_dialogID, validateCaptureSize);
    editorUIAddTextEntry(s_dialogID, s_angleBuffer, sizeof(s_angleBuffer), NULL, "Blur Angle:");
    editorUIAddDynamicText(s_dialogID, validateAngle);
    editorUIAddTextEntry(s_dialogID, s_timeBuffer, sizeof(s_timeBuffer), NULL, "Time of Day:");
    editorUIAddDynamicText(s_dialogID, validateTime);
    editorUIAddButtonRow(s_dialogID, NULL,
        "OK", onDialogOK,
        "Cancel", onDialogCancel,
        NULL);
    s_dialogState = DIALOG_ACTIVE;
}

bool edit_isAdjustCubemapDialogFinished() {
    return s_dialogState == DIALOG_CANCEL || s_dialogState == DIALOG_OK;
}

bool edit_isCubemapTrackerModified() {
    return s_dialogState == DIALOG_OK;
}

void edit_submitCubemapTrackerChanges() {
    DefTracker *cubemapTracker = sel_list[0].def_tracker;
    GroupDef *savedDef = cubemapTracker->def;
    GroupDef modifiedDef = *savedDef;
    cubemapTracker->def = &modifiedDef;
    cubemapTracker->def->cubemapGenSize = s_genSize;
    cubemapTracker->def->cubemapCaptureSize = s_captureSize;
    cubemapTracker->def->cubemapBlurAngle = s_angle;
    cubemapTracker->def->cubemapCaptureTime = s_time;
    editUpdateTracker(cubemapTracker);
    cubemapTracker->def = savedDef;
}

void edit_closeAdjustCubemapDialog() {
    if (s_dialogID >= 0) {
        float x, y;
        RegReader rr = createRegReader();
        initRegReader(rr, regGetAppKey());
        editorUIGetPosition(s_dialogID, &x, &y);
        rrWriteInt(rr, "AdjustCubemapDialogX", x);
        rrWriteInt(rr, "AdjustCubemapDialogY", y);
        destroyRegReader(rr);

        s_dialogState = DIALOG_NONE;
        editorUIDestroyWindow(s_dialogID);
        s_dialogID = -1;
    }
}
