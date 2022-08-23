#include <utilitieslib/stdtypes.h>
#include "render/renderssao.h"
#include "render/thread/rt_ssao.h"
#include "render/thread/rt_tune.h"
#include "render/thread/rt_queue.h"
#include "render/thread/rt_init.h"
#include "win/win_init.h"
#include "cmdparse/cmdgame.h"
#include "graphics/gfxSettings.h"
#include "win/input.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/file.h> // for isDevelopmentOrQAMode

static RdrSsaoParams ssao_params = {0,0,0.0f,0,0,0,0,0};
#define MAX_TUNE_COMMANDS 32
static TuneInputCommand ssao_debug_input[MAX_TUNE_COMMANDS+1];

void rdrAmbientStart()
{
    // Initial parameters
    windowSize(&ssao_params.width, &ssao_params.height);
    ssao_params.strength = (rdr_caps.features & GFXF_AMBIENT) ? game_state.ambientStrength : AMBIENT_OFF;
    ssao_params.blur = game_state.ambientBlur;
    switch(game_state.ambientResolution) {
        case AMBIENT_RES_HIGH_PERFORMANCE:
            ssao_params.ambientDownsample = 2;
            ssao_params.blurDownsample = 2;
            ssao_params.depthDownsample = 4;
            break;
        case AMBIENT_RES_PERFORMANCE:
            ssao_params.ambientDownsample = 2;
            ssao_params.blurDownsample = 2;
            ssao_params.depthDownsample = 2;
            break;
        case AMBIENT_RES_QUALITY:
            ssao_params.ambientDownsample = 2;
            ssao_params.blurDownsample = 1;
            ssao_params.depthDownsample = 2;
            break;
        case AMBIENT_RES_HIGH_QUALITY:
            ssao_params.ambientDownsample = 1;
            ssao_params.blurDownsample = 1;
            ssao_params.depthDownsample = 2;
            break;
        case AMBIENT_RES_SUPER_HIGH_QUALITY:
            ssao_params.ambientDownsample = 1;
            ssao_params.blurDownsample = 1;
            ssao_params.depthDownsample = 1;
            break;
    }    
    rdrQueue(DRAWCMD_SSAOSETPARAMS, &ssao_params, sizeof(ssao_params));
}

void rdrAmbientDeInit()
{
    ssao_params.width = 0;
    ssao_params.height = 0;
    ssao_params.strength = AMBIENT_OFF;
    rdrQueue(DRAWCMD_SSAOSETPARAMS, &ssao_params, sizeof(ssao_params));
}

void rdrAmbientOcclusion()
{
    rdrQueueCmd(DRAWCMD_SSAORENDER);
}

void rdrAmbientUpdate()
{
    const float epsilon = 0.001f;
    // Blend AO scene file in gradually
    float diff = scene_info.ambientOcclusionWeight - ssao_params.sceneAmbientOcclusionWeight;
    if(fabs(diff) < epsilon || !_finite(ssao_params.sceneAmbientOcclusionWeight)) {
        ssao_params.sceneAmbientOcclusionWeight = scene_info.ambientOcclusionWeight;
    } else if(TIMESTEP_NOSCALE > epsilon) {
        ssao_params.sceneAmbientOcclusionWeight += diff * 0.5f * MIN((1.0/30.0f) * TIMESTEP_NOSCALE, 1.0f);
    }
}

void rdrAmbientDebug()
{
    if (!isDevelopmentOrQAMode()) {
        return;
    }

    rdrQueueCmd(DRAWCMD_SSAORENDERDEBUG);
    tuneDraw();
}

void rdrAmbientDebugInput()
{
    int num_input = 0;
    KeyInput* input;

    if (!isDevelopmentOrQAMode()) {
        return;
    }

    memset(ssao_debug_input, 0, sizeof(ssao_debug_input));

    for (input = inpGetKeyBuf(); input; inpGetNextKey(&input))
    {
        if(input->type != KIT_EditKey) {
            continue;
        }

        // Shift + Ctrl + T to toggle it on and off
        if((input->attrib & KIA_SHIFT) && (input->attrib & KIA_CONTROL) && (input->key == 0x3A)) {
            ssao_debug_input[num_input++].cmd = tuneInputToggle;
            continue;
        }

        ssao_debug_input[num_input].shift = (input->attrib & KIA_SHIFT) != 0;
        ssao_debug_input[num_input].ctrl = (input->attrib & KIA_CONTROL) != 0;
        ssao_debug_input[num_input].alt = (input->attrib & KIA_ALT) != 0;

        switch (input->key) {
            case INP_BACK: ssao_debug_input[num_input++].cmd = tuneInputBack; break;
            case INP_UP: ssao_debug_input[num_input++].cmd = tuneInputUp; break;
            case INP_DOWN: ssao_debug_input[num_input++].cmd = tuneInputDown; break;
            case INP_LEFT: ssao_debug_input[num_input++].cmd = tuneInputLeft; break;
            case INP_RIGHT: ssao_debug_input[num_input++].cmd = tuneInputRight; break;
        }
    }

    if(num_input) {
        rdrQueue(DRAWCMD_SSAODEBUGINPUT, &ssao_debug_input, sizeof(ssao_debug_input) * num_input);
    }
}
