#include "win/input.h"
#include "cmdparse/cmdgame.h"
#include "UI/uiInput.h"
#include "UI/uiKeybind.h"
#include "UI/uiOptions.h"
#include <utilitieslib/utils/mathutil.h>
#include "graphics/camera.h"
#include "UI/uiConsole.h"

KeyBindProfile        detachedCam_binds_profile;
S32                    detachedCam_input_state;
CameraInfo*            detachedCam_info;
S32                    detachedCam_move_state[6];

static void rotateDetachedCamera(F32 pitch, F32 yaw, F32 roll);
static void setDetatchedCamera(int axis, F32 angle);
static int detachedCamCmdParse(char *str, int x, int y);

enum
{
    CMD_DETACHEDCAM_MOVE_FORWARD= 1,
    CMD_DETACHEDCAM_MOVE_BACK,
    CMD_DETACHEDCAM_MOVE_LEFT,
    CMD_DETACHEDCAM_MOVE_RIGHT,
    CMD_DETACHEDCAM_MOVE_ASCEND,
    CMD_DETACHEDCAM_MOVE_DESCEND,
    CMD_DETACHEDCAM_TURN_UP,
    CMD_DETACHEDCAM_TURN_DOWN,
    CMD_DETACHEDCAM_TURN_LEFT,
    CMD_DETACHEDCAM_TURN_RIGHT,
    CMD_DETACHEDCAM_TURN_CLOCKWISE,
    CMD_DETACHEDCAM_TURN_COUNTERCLOCKWISE,
    CMD_DETACHEDCAM_RESET_ROLL,
    CMD_DETACHEDCAM_EXIT,
};

Cmd detachedCam_cmds[] =
{
    { 0, "detachedCam_move_forward",            CMD_DETACHEDCAM_MOVE_FORWARD, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_move_back",                CMD_DETACHEDCAM_MOVE_BACK, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_move_left",                CMD_DETACHEDCAM_MOVE_LEFT, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_move_right",                CMD_DETACHEDCAM_MOVE_RIGHT, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_move_ascend",                CMD_DETACHEDCAM_MOVE_ASCEND, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_move_descend",            CMD_DETACHEDCAM_MOVE_DESCEND, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_turn_left",                CMD_DETACHEDCAM_TURN_LEFT, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_turn_right",                CMD_DETACHEDCAM_TURN_RIGHT, {{ PARSETYPE_S32, &detachedCam_input_state }} },
    
    { 0, "detachedCam_turn_up",                    CMD_DETACHEDCAM_TURN_UP, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_turn_down",                CMD_DETACHEDCAM_TURN_DOWN, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_turn_clockwise",            CMD_DETACHEDCAM_TURN_CLOCKWISE, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_turn_counterclockwise",    CMD_DETACHEDCAM_TURN_COUNTERCLOCKWISE, {{ PARSETYPE_S32, &detachedCam_input_state }} },

    { 0, "detachedCam_reset_roll",                CMD_DETACHEDCAM_RESET_ROLL, {{ 0 }} },

    { 0, "detachedCam_exit",                    CMD_DETACHEDCAM_EXIT, {{ 0 }} },
    { 0 },
};

CmdList detachedCam_cmdlist =
{
    {{ detachedCam_cmds },
    { 0 }}
};

void toggleDetachedCamControls(bool on, CameraInfo *ci)
{
    static int detachedCamControlsInitted = 0;

    if(!detachedCamControlsInitted)
    {
        detachedCamControlsInitted = 1;

        cmdOldInit(detachedCam_cmds);

        bindKeyProfile(&detachedCam_binds_profile);
        detachedCam_binds_profile.name = "Detached Camera Commands";
        detachedCam_binds_profile.parser = detachedCamCmdParse;
        detachedCam_binds_profile.trickleKeys = 1;

        bindKey("w",        "+detachedCam_move_forward",    0);
        bindKey("s",        "+detachedCam_move_back",        0);
        bindKey("a",        "+detachedCam_move_left",        0);
        bindKey("d",        "+detachedCam_move_right",        0);
        bindKey("space",    "+detachedCam_move_ascend",        0);
        bindKey("z",        "+detachedCam_move_descend",    0);
        bindKey("q",        "+detachedCam_turn_left",        0);
        bindKey("e",        "+detachedCam_turn_right",        0);
        bindKey("pageup",    "+detachedCam_turn_up",            0);
        bindKey("pagedown",    "+detachedCam_turn_down",        0);
        bindKey("n",        "+detachedCam_turn_counterclockwise",    0);
        bindKey("m",        "+detachedCam_turn_clockwise",            0);
        bindKey("b",        "detachedCam_reset_roll",        0);
        bindKey("esc",        "detachedCam_exit",                0);
        unbindKeyProfile(&detachedCam_binds_profile);
    }

    if(on)
    {
        control_state.detached_camera = true;
        detachedCam_info = ci;
        bindKeyProfile(&detachedCam_binds_profile);
    }
    else
    {
        control_state.detached_camera = false;
        detachedCam_info = NULL;
        unbindKeyProfile(&detachedCam_binds_profile);
    }
    return;
}

static int detachedCamCmdParse(char *str, int x, int y)
{
    Cmd            *cmd;
    CmdContext    output = {0};

    
    output.found_cmd = false;
    output.access_level = cmdAccessLevel();
    cmd = cmdOldRead(&detachedCam_cmdlist,str,&output);

    if (output.msg[0])
    {
        if (strncmp(output.msg,"Unknown",7)==0)
            return 0;

        conPrintf("%s",output.msg);
        return 1;
    }

    if (!cmd)
        return 0;

    if(!detachedCam_info)
        return 0;

    switch(cmd->num)
    {
        xcase CMD_DETACHEDCAM_MOVE_FORWARD:
            detachedCam_move_state[2] = -detachedCam_input_state;
        xcase CMD_DETACHEDCAM_MOVE_BACK:
            detachedCam_move_state[2] = detachedCam_input_state;
        xcase CMD_DETACHEDCAM_MOVE_LEFT:
            detachedCam_move_state[0] = detachedCam_input_state;
        xcase CMD_DETACHEDCAM_MOVE_RIGHT:
            detachedCam_move_state[0] = -detachedCam_input_state;
        xcase CMD_DETACHEDCAM_MOVE_ASCEND:
            detachedCam_move_state[1] = detachedCam_input_state;
        xcase CMD_DETACHEDCAM_MOVE_DESCEND:
            detachedCam_move_state[1] = -detachedCam_input_state;
        xcase CMD_DETACHEDCAM_TURN_UP:
            detachedCam_move_state[3] = -detachedCam_input_state;
        xcase CMD_DETACHEDCAM_TURN_DOWN:
            detachedCam_move_state[3] = detachedCam_input_state;
        xcase CMD_DETACHEDCAM_TURN_LEFT:
            detachedCam_move_state[4] = -detachedCam_input_state;
        xcase CMD_DETACHEDCAM_TURN_RIGHT:
            detachedCam_move_state[4] = detachedCam_input_state;
        xcase CMD_DETACHEDCAM_TURN_CLOCKWISE:
            detachedCam_move_state[5] = -detachedCam_input_state;
        xcase CMD_DETACHEDCAM_TURN_COUNTERCLOCKWISE:
            detachedCam_move_state[5] = detachedCam_input_state;
        xcase CMD_DETACHEDCAM_RESET_ROLL:
            setDetatchedCamera(2, 0.0f);
        xcase CMD_DETACHEDCAM_EXIT:
            toggleDetachedCamControls(false, NULL);
    }
    return 1;
}

void camDetached(CameraInfo *ci)
{
    extern int mouse_dx,mouse_dy;
    int i;

    // Check for mouse look - bit of a hack
    if (gMouseInpCur.right == MS_DOWN)
    {
        if(!!mouse_dy)
            ci->pyr[0] = addAngle(ci->pyr[0], mouse_dy * optionGetf(kUO_MouseSpeed) / 500.0F);

        if(!!mouse_dx)
            ci->pyr[1] = addAngle(ci->pyr[1], mouse_dx * optionGetf(kUO_MouseSpeed) / 500.0F);
    }

    // Apply translation inputs
    for(i = 0; i <= 2; i++)
    {
        if(!!detachedCam_move_state[i])
        {
            F32 *cam_direction = ci->cammat[i];
            F32 scale = TIMESTEP * control_state.speed_scale * detachedCam_move_state[i];
            scaleAddVec3(cam_direction, scale, ci->cammat[3], ci->cammat[3]);
        }
    }

    // Apply rotation inputs
    {
        F32 scale = RAD(2) * TIMESTEP * optionGetf(kUO_MouseSpeed);

        for(i = 0; i <= 2; i++)
        {
            if(!!detachedCam_move_state[i + 3])
                ci->pyr[i] = addAngle(ci->pyr[i], detachedCam_move_state[i + 3] * scale);
        }
    }

    // Apply changes to the camera
    createMat3YPR(ci->cammat, ci->pyr);
}

static void setDetatchedCamera(int axis, F32 angle)
{
    Vec3 pyr;
    getMat3YPR(detachedCam_info->cammat, pyr);

    detachedCam_info->pyr[axis] = pyr[axis] = fixAngle(angle);

    createMat3YPR(detachedCam_info->cammat, pyr);
}