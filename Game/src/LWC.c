
#include "LWC.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/scriptvars.h>
#include <utilitieslib/language/MessageStore.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "UI/uiDialog.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/file.h>
#include "graphics/font.h"  // for xyprintf
#include "cmdparse/cmdgame.h"
#include "player/player.h"
#include "entity/entity.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/piglib.h>
#include <utilitieslib/utils/utils.h>
#include "UI/Hybrid/uiWebStoreFrame.h"
#include "clientcomm/clientcomm.h"
#include "game.h"

#include <windows.h>


#define LWC_STAGE_SEND_TO_MAPSERVER_INTERVAL 10
#define LWC_STAGE_ADVANCE_DELAY 1

static bool            s_Enabled = false;
static LWC_STAGE    s_CurrentStage = LWC_STAGE_INVALID;
static LWC_STAGE    s_DesiredStage = LWC_STAGE_INVALID;
static F32            s_DesiredStageTime = 0.0f;
static F32             s_NextStageETA = 0;
static F32             s_NextStagePercentDone = 0.0f; // 0.0f to 1.0f
static F32             s_NextStageDownloadedSizeMB = 0.0f;
static F32             s_NextStageTotalSizeMB = 0.0f;

static F32            s_TotalETA = 0;
static F32            s_TotalPercentDone = 0.0f;

static U32            s_NextTimeToSendLWCStage = 0;

static LWC_DOWNLOAD_RATE    s_download_rate = LWC_DOWNLOAD_FASTEST;

static bool s_pipeInited = false;
static int  s_debugMode = 0;

static HANDLE s_pipe_handle = INVALID_HANDLE_VALUE;
static bool s_pipe_connected = false;

// Initialize system at startup (before we know if it will be used)
void LWC_Init()
{
    // TODO: If using staged data, determine what stage we are at
    // fpe -- added LWC_IsActive function which needs to differentiate between final stage and LWC not being active, so set cur stage to invalid when not in use
    s_CurrentStage = LWC_STAGE_INVALID;

    // Update the crash report string
    game_setAssertSystemExtraInfo();
}

static void LWC_InitPipe()
{
    if (s_pipeInited)
        return;

    s_pipe_handle = CreateNamedPipeA("\\\\.\\pipe\\Coh_NCLauncher_Pipe",
                                    PIPE_ACCESS_DUPLEX | FILE_FLAG_FIRST_PIPE_INSTANCE,
                                    PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
                                    PIPE_UNLIMITED_INSTANCES ,
                                    10000,
                                    10000,
                                    1,    // A value of zero will result in a default time-out of 50 milliseconds
                                    NULL);

    if (s_pipe_handle == INVALID_HANDLE_VALUE)
    {
        char cBuf[1000];
        char cFullErrorMessage[1000];
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, cBuf, 1000, NULL);
        strcpy(cFullErrorMessage, "Failed to open named pipe . Windows system error message: ");
        strcat(cFullErrorMessage, cBuf);
        Errorf(cFullErrorMessage);

        return;
    }
    else
    {
        printf("Successfully opened pipe\n");
    }

    s_pipeInited = true;
}

static bool LWC_PiggFilterCB(const char *filename)
{
    // Sanity check, this should never happen
    assert(s_CurrentStage != LWC_STAGE_INVALID && s_CurrentStage <= LWC_STAGE_LAST);
    if (s_CurrentStage == LWC_STAGE_INVALID || s_CurrentStage >= LWC_STAGE_LAST)
        return true;

    // Stage 1 piggs
    // TODO: get this list of piggs from someplace else instead of hard-coded here?
    if (stricmp(filename, "bin.pigg") == 0 ||
        stricmp(filename, "fonts.pigg") == 0 ||
        stricmp(filename, "misc.pigg") == 0 ||
        stricmp(filename, "texts.pigg") == 0 ||
        strstriConst(filename, "stage1") )
        return true;

    if (s_CurrentStage >= LWC_STAGE_2 && strstriConst(filename, "stage2"))
        return true;

    if (s_CurrentStage >= LWC_STAGE_3 && strstriConst(filename, "stage3"))
        return true;

    return false;
}

// Called when we find '-lwc' on the command line
void LWC_Enable()
{
    s_CurrentStage = LWC_STAGE_1;
    s_DesiredStage = LWC_STAGE_1;

    // Disable version check and verify step when LWC is active
    game_state.no_version_check = 1;
    game_state.noVerify = 1;
    
    s_Enabled = true;
    LWC_SetThrottled(false); // default to full speed, we will throttle if mapserver/dbserver connection

    PigFileSetFilter(LWC_PiggFilterCB);

    LWC_InitPipe();

    // Update the crash report string
    game_setAssertSystemExtraInfo();
}


bool LWC_IsActive()
{
    // Note: after last stage is loaded, and all client callbacks have been called, stage gets set to invalid
    return (s_Enabled && s_CurrentStage != LWC_STAGE_INVALID);
}

char *LWC_GetStageString()
{
    static char stage_string[10];

    if (!LWC_IsActive())
        return "Not Active";

    sprintf(stage_string, "%d", s_CurrentStage);

    return stage_string;
}

// Call this once the last data stage is complete
static void LWC_Disable()
{
    s_CurrentStage = LWC_STAGE_INVALID;
    s_DesiredStage = LWC_STAGE_INVALID;

    if (s_pipeInited)
    {
        if (s_pipe_handle != INVALID_HANDLE_VALUE)
        {
            if (s_pipe_connected)
            {
                DisconnectNamedPipe(s_pipe_handle);
                s_pipe_connected = false;
            }

            CloseHandle(s_pipe_handle);
            s_pipe_handle = INVALID_HANDLE_VALUE;
            printf("Pipe disconnected\n");
        }
        s_pipeInited = false;
    }


    PigFileSetFilter(NULL);
    s_Enabled = false;
}


static char *sGetDownloadRateString(LWC_DOWNLOAD_RATE rate)
{
    if (rate == LWC_DOWNLOAD_FASTEST)
        return "Fastest";
    else if (rate == LWC_DOWNLOAD_PAUSED)
        return "Paused";

    return "Throttled";
}

static void printDebugging()
{
    int y = 5;
    LWC_STAGE currentStage = LWC_GetLastCompletedDataStage();

    xyprintf(10, y++, "LWC Current Data Stage = %d (desired = %d)", currentStage, s_DesiredStage);
    xyprintf(10, y++, "LWC Remaining Seconds Next Stage: %d", LWC_GetRemainingSecondsForNextStage());
    xyprintf(10, y++, "LWC %% Done Next Stage: %5.2f%%", LWC_GetPercentDoneNextStage() * 100.0f);
    xyprintf(10, y++, "LWC Pipe Connected: %d", s_pipe_connected);
    xyprintf(10, y++, "LWC Download Rate: %s", sGetDownloadRateString(s_download_rate));
    xyprintf(10, y++, "LWC Remaining Seconds Total: %d", LWC_GetRemainingSecondsTotal());
    xyprintf(10, y++, "LWC %% Done Total: %5.2f%%", LWC_GetPercentDoneTotal() * 100.0f);
}

static void sendReply(const char *message)
{
    DWORD bytesWritten;
    if (WriteFile(s_pipe_handle, message, strlen(message) + 1, &bytesWritten, 0))
    {
        if (s_debugMode > 1)
        {
            printf("Successfully wrote %d bytes: %s\n", bytesWritten, message);
        }
    }
    else
    {
        char cBuf[1000];
        char cFullErrorMessage[1000];
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, cBuf, 1000, NULL);
        strcpy(cFullErrorMessage, "Failed to write to named pipe . Windows system error message: ");
        strcat(cFullErrorMessage, cBuf);
        Errorf(cFullErrorMessage);
    }
}

static void UpdateDownloadRate()
{
    // dynamically set download rate based on game activity
    bool bThrottle = false;

    // check to see if recently had slow tick to dbserver or mapserver, or lost packets
    if( commIsInTroubledMode() )
        bThrottle = true;
    
    LWC_SetThrottled(bThrottle);
}

#define DOWNLOAD_APPLY_WEIGHTING 0.9f // for calculating stage % complete, weight download this amount, apply = 1- this amount.

static void handleMessage(const char *message, int msgLen)
{
    if (s_debugMode > 1)
    {
        printf("Received pipe message: %s\n", message);
    }

    if (strncmp(message, "echo ", 5) == 0)
    {
        char outBuffer[10000];
        sprintf(outBuffer, "reply %s", message + 5);

        sendReply(outBuffer);

        return;
    }

    if (strncmp(message, "Download Speed Check", 20) == 0)
    {
        UpdateDownloadRate();
        if (s_download_rate == LWC_DOWNLOAD_FASTEST)
            sendReply("Download Fastest");
        else if (s_download_rate == LWC_DOWNLOAD_PAUSED)
            sendReply("Download Paused");
        else
            sendReply("Download Normal");

        return;
    }

    if (strcmp(message, "END") == 0)
    {
        return;
    }

    // eg: "Downloading Stage 2 - 61 744 of 1219.69 ETA: 54"
#define IDENTIFIER "Downloading Stage "
    if (strnicmp(message, IDENTIFIER, strlen(IDENTIFIER)) == 0)
    {
        int stage, fields_read;
        F32 percent, size1, size2, duration;

        // Skip past the initial part of the message that we've already checked
        message += strlen(IDENTIFIER);

        fields_read = sscanf(message, "%d - %f %f of %f ETA: %f", &stage, &percent, &size1, &size2, &duration);
        
        if (fields_read == 5 &&
            stage >= LWC_STAGE_2 && stage <= LWC_STAGE_LAST &&
            percent >= 0.0f && percent <= 100.0f &&
            size1 > 0.0f && size1 < 10240 && 
            size2 > 0.0f && size2 < 10240 && 
            duration >= 0.0f)
        {
            U32 currentTime = timerCpuSeconds();

            if (stage - 1 != s_CurrentStage)
                LWC_SetCurrentDataStage(stage-1);

            s_NextStageETA = currentTime + duration;
            s_NextStagePercentDone = DOWNLOAD_APPLY_WEIGHTING * (percent / 100.0f);

            s_NextStageDownloadedSizeMB = size1;
            s_NextStageTotalSizeMB = size2;
        }

        return;
    }
#undef IDENTIFIER


    // eg: "Applying Stage 2 - 27 ETA: 17"
#define IDENTIFIER "Applying Stage "
    if (strnicmp(message, IDENTIFIER, strlen(IDENTIFIER)) == 0)
    {
        int stage, fields_read;
        F32 percent, duration;

        // Skip past the initial part of the message that we've already checked
        message += strlen(IDENTIFIER);

        fields_read = sscanf(message, "%d - %f ETA: %f", &stage, &percent, &duration);

        if (fields_read == 3 &&
            stage >= LWC_STAGE_2 && stage <= LWC_STAGE_LAST &&
            percent >= 0.0f && percent <= 100.0f &&
            duration >= 0.0f)
        {
            U32 currentTime = timerCpuSeconds();

            if (percent == 100.0f && stage == LWC_STAGE_LAST)
                LWC_SetCurrentDataStage(LWC_STAGE_LAST);
            else if (stage - 1 != s_CurrentStage)
                LWC_SetCurrentDataStage(stage-1);

            // Go from 90 to 100% while applying
            s_NextStagePercentDone = DOWNLOAD_APPLY_WEIGHTING + (1.0f - DOWNLOAD_APPLY_WEIGHTING) * (percent / 100.0f);
            //s_NextStageETA = currentTime + duration;
        }

        return;
    }
#undef IDENTIFIER

    // eg: "Total Progress - 0 ETA: 85
#define IDENTIFIER "Total Progress - "
    if (strnicmp(message, IDENTIFIER, strlen(IDENTIFIER)) == 0)
    {
        int fields_read;
        F32 percent, duration;

        // Skip past the initial part of the message that we've already checked
        message += strlen(IDENTIFIER);

        fields_read = sscanf(message, "%f ETA: %f", &percent, &duration);

        if (fields_read == 2 &&
            percent >= 0.0f && percent <= 100.0f &&
            duration >= 0.0f)
        {
            U32 currentTime = timerCpuSeconds();

            s_TotalETA = currentTime + duration;
            s_TotalPercentDone = percent / 100.0f;
        }

        return;
    }
#undef IDENTIFIER

    
    {
        //MessageBox(NULL, message, "Pipe Message", MB_OK);
        //dialogStd(DIALOG_OK, (char *) message, NULL, NULL, NULL, NULL, 0);
    }
}

// Quick hack, tbd have clients register callback
extern void sndOnLWCAdvanceStage(LWC_STAGE stage);
extern void trickOnLWCAdvanceStage(LWC_STAGE stage);
extern void geoLoadResetNonExistent();

void LWC_Tick(void)
{
    U32 currentTime = timerCpuSeconds();
    Entity *e = playerPtr();

    if (s_debugMode > 0)
    {
        if (s_Enabled)
            printDebugging();
        else
            xyprintf(10, 5, "LWC is disabled (run with -lwc)");
    }

    if (!s_Enabled)
        return;

    if (s_pipe_handle == INVALID_HANDLE_VALUE)
    {
        // TODO: Retry creating pipe?
        return;
    }
    else
    {
        BOOL connectStatus = ConnectNamedPipe(s_pipe_handle, NULL);
        DWORD lastError = GetLastError();

        if (lastError == ERROR_PIPE_CONNECTED)
        {
            if (!s_pipe_connected)
                printf("Pipe connected\n");
            s_pipe_connected = true;
        }
        else
        {
            if (s_pipe_connected == true)
            {
                DisconnectNamedPipe(s_pipe_handle);
                printf("Pipe disconnected\n");
            }

            s_pipe_connected = false;
        }

        if (s_pipe_connected)
        {
            char inBuffer[10000];
            S32 bytesRead;
            while(ReadFile(s_pipe_handle, inBuffer, ARRAY_SIZE(inBuffer) - 1, &bytesRead, NULL))
            {

                if (bytesRead)
                {
                    // Make sure it is NULL terminated
                    if (inBuffer[bytesRead - 1] != 0)
                        inBuffer[bytesRead] = 0;

                    handleMessage(inBuffer, bytesRead);
                }
            }

        }
    }

    if (s_DesiredStage > s_CurrentStage && s_DesiredStage <= LWC_STAGE_LAST && currentTime > s_DesiredStageTime)
    {
        int newPiggCount;
        LWC_STAGE prevStage = s_CurrentStage;

        // Increment s_CurrentStage here so the LWC_PiggFilterCB() will find new piggs
        s_CurrentStage++;
        newPiggCount = fileCheckForNewPiggs();

        if (s_debugMode)
            printf("LWC: Advanced from stage %d to stage %d, loaded %d new piggs\n", prevStage, s_CurrentStage, newPiggCount);

        if (s_debugMode > 1)
        {
            printf("  s_DesiredStage = %d\n", s_DesiredStage);
        }

        if (newPiggCount) 
        {
            // If we stopped finding new piggs, go ahead and read all of the new piggs
            
            // callbacks to let modules know new piggs have been loaded        
            sndOnLWCAdvanceStage(s_CurrentStage);
            trickOnLWCAdvanceStage(s_CurrentStage);
            geoLoadResetNonExistent();

            s_NextStageETA = currentTime;
            s_NextStagePercentDone = 0.0f;
            s_NextTimeToSendLWCStage = 0;
        
            // once all callbacks have been called after final stage loads, LWC is no longer considered active, so set current stage to invalid
            if (s_CurrentStage == LWC_STAGE_LAST)
            {
                LWC_Disable();
            }

            // Update the crash report string
            game_setAssertSystemExtraInfo();
        }
        else
        {
            // We haven't found any new piggs yet, continue to wait
            s_CurrentStage--;
        }

        // Do this so we can check again if we did not find any piggs, or check again if advancing more than one stage
        s_DesiredStageTime = currentTime + LWC_STAGE_ADVANCE_DELAY;
    }

    // Do the entity and mapserver update after the stage is advanced
    // since LWC may be disabled and we would not get another tick to do this
    if (e && e->pl && e->pl->lwc_stage != s_CurrentStage)
        e->pl->lwc_stage = s_CurrentStage;

    if (currentTime > s_NextTimeToSendLWCStage)
    {
        if (e && e->pl)
        {
            LWC_SendEntityStageToMapServer(e);
        }

        s_NextTimeToSendLWCStage = currentTime + LWC_STAGE_SEND_TO_MAPSERVER_INTERVAL;
    }
}

LWC_STAGE LWC_GetLastCompletedDataStage(void)
{
    return s_CurrentStage;
}

U32 LWC_GetRemainingSecondsForNextStage()
{
    U32 currentTime = timerCpuSeconds();

    if (s_NextStageETA < currentTime)
        return 0;

    return s_NextStageETA - currentTime;
}

F32 LWC_GetPercentDoneNextStage()
{
    return s_NextStagePercentDone;
}

U32 LWC_GetRemainingSecondsTotal()
{
    U32 currentTime = timerCpuSeconds();

    if (s_TotalETA < currentTime)
        return 0;

    return s_TotalETA - currentTime;
}

F32 LWC_GetPercentDoneTotal()
{
    return s_TotalPercentDone;
}

static void LWC_ShowDataNotReadyMessage(LWC_STAGE stage)
{
    if (stage == LWC_STAGE_2)
        dialogStd(DIALOG_OK, "LWCDataNotReadyTutorial", 0, 0, 0, 0, 0);
    else
    {
        dialog( DIALOG_OK, -1,-1,-1,-1, textStd("LWCDataNotReadyMap", stage), NULL, NULL, NULL, NULL, 0, NULL, NULL, 1, 0, 0, 0 );
    }
}

bool LWC_CheckStageAndShowMessageIfNotReady(LWC_STAGE stage)
{
    if (LWC_IsActive() && LWC_GetLastCompletedDataStage() < stage)
    {
        LWC_ShowDataNotReadyMessage(stage);

        return false;
    }

    return true;
}


void LWCDebug_SetMode(int debugMode)
{
    s_debugMode = debugMode;
}

void LWC_SetCurrentDataStage(LWC_STAGE stage)
{
    if (stage != s_DesiredStage)
    {
        U32 currentTime = timerCpuSeconds();

        if (s_debugMode > 1)
        {
            printf("LWC_SetCurrentDataStage: %d\n", stage);
        }

        s_DesiredStage = MINMAX(stage, LWC_STAGE_1, LWC_STAGE_LAST);
        s_DesiredStageTime = currentTime + LWC_STAGE_ADVANCE_DELAY;
    }
}


void LWC_SetDownloadRate(LWC_DOWNLOAD_RATE newRate)
{
    if (newRate >= 0 && newRate <= 2)
        s_download_rate = newRate;
}

void LWC_SetThrottled(bool bThrottled)
{
    s_download_rate = bThrottled ? LWC_DOWNLOAD_NORMAL : LWC_DOWNLOAD_FASTEST;
}

LWC_DOWNLOAD_RATE LWC_GetDownloadRate()
{
    return s_download_rate;
}



