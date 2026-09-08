#include "time.h"
#include "edit/edit_select.h"
#include "edit/edit_cmd.h"
#include "edit_info.h"
#include "seq/anim.h"
#include "UI/uiContextMenu.h"
#include <utilitieslib/components/MRUList.h>
#include "render/tex.h"
#include <utilitieslib/utils/textparser.h>
#include "cmdparse/cmdgame.h"
#include "render/thread/rt_model.h"

#define BORDER_COLOR 0xafafafff
#define BACK_COLOR 0x000000ff

typedef struct ContextCommand
{
    char *name;
    char *cmd_name;
    struct ContextCommand *submenu;
    int (*is_available_func)(void *);
    const char * (*name_append_func)(struct ContextCommand *);
    int *toggle_var;
    int param;
} ContextCommand;

typedef struct ContextCommandList
{
    ContextCommand **commands;
} ContextCommandList;

TokenizerParseInfo parseContextCommand[] = {
    { "",        TOK_STRUCTPARAM|TOK_STRING(ContextCommand, name, 0)},
    { "",        TOK_STRUCTPARAM|TOK_STRING(ContextCommand, cmd_name, 0)},
    { "\n",        TOK_END,                            0 },
    { "", 0, 0 }
};

TokenizerParseInfo parseContextCommandList[] = {
    { "Entry",            TOK_STRUCT(ContextCommandList, commands, parseContextCommand) },
    { "", 0, 0 }
};
//////////////////////////////////////////////////////////////////////////

static int editSelOne(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def;
}

static int editSelOneModel(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model;
}

static int editSelMaterial(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && cmd && cmd->param < sel_list[0].def_tracker->def->model->tex_count;
}

static int editSelMaterialTex(ContextCommand *cmd)
{
    int idx, layer;
    if (!cmd)
        return 0;
    idx = cmd->param % 10;
    layer = cmd->param / 10;
    if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && idx < sel_list[0].def_tracker->def->model->tex_count)
    {
        TexBind *bind;
        int i, tri_idx = 0;
        for (i = 0; i < idx; i++)
            tri_idx += sel_list[0].def_tracker->def->model->tex_idx[i].count;
        if (!(excludedLayer(sel_list[0].def_tracker->def->model->blend_modes[idx], layer)&EXCLUDE_BIND))
        {
            bind = getTexWithReplaces(sel_list[0].def_tracker, tri_idx);
            return bind && !!bind->tex_layers[layer];
        }
    }

    return 0;
}

static int editSelLight(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && (sel_list[0].def_tracker->def->has_ambient || sel_list[0].def_tracker->def->has_light);
}

static int editSelCubemap(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->has_cubemap;
}

static int editSelTexOld(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->has_tex;
}

static int editSelTexNew(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && eaSize(&sel_list[0].def_tracker->def->def_tex_swaps);
}

static int editSelTint(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->has_tint_color;
}

static int editSelFog(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->has_fog;
}

static int editSelSound(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->has_sound;
}

static int editSelFX(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->type_name && sel_list[0].def_tracker->def->type_name[0];
}

static int editSelVolume(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->has_volume;
}

static int editLods(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && sel_list[0].def_tracker->def->model->lod_info;
}

static int editLibPiece(ContextCommand *cmd)
{
    return sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && groupInLib(sel_list[0].def_tracker->def);
}

static const char *editSelName(ContextCommand *cmd)
{
    if (sel_count == 1)
    {
        DefTracker    *tracker;
        tracker = sel_list[0].def_tracker;

        if (tracker->def && tracker->def->name)
            return tracker->def->name;
    }

    return "NONE";
}

static const char *editSelMaterialName(ContextCommand *cmd)
{
    if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && cmd && cmd->param < sel_list[0].def_tracker->def->model->tex_count)
    {
        static char buffer[1024];
        int i, tri_idx = 0;
        for (i = 0; i < cmd->param; i++)
            tri_idx += sel_list[0].def_tracker->def->model->tex_idx[i].count;
        strcpy(buffer, getTexNameWithReplaces(sel_list[0].def_tracker, tri_idx));
        return buffer;
    }

    return "";
}

static const char *editSelMaterialBlendMode(ContextCommand *cmd)
{
    if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && cmd && cmd->param < sel_list[0].def_tracker->def->model->tex_count)
    {
        static char buffer[1024];
        BlendModeType blend_mode; // = sel_list[0].def_tracker->def->model->blend_modes[cmd->param];
        TexBind *bind;
        int idx = cmd->param;
        int i, tri_idx = 0;
        for (i = 0; i < idx; i++)
            tri_idx += sel_list[0].def_tracker->def->model->tex_idx[i].count;
        bind = getTexWithReplaces(sel_list[0].def_tracker, tri_idx);
        if (!bind)
            return "";
        blend_mode = bind->bind_blend_mode;
        strcpy(buffer, blend_mode_names[blend_mode.shader]);
        if (blend_mode.blend_bits & BMB_SINGLE_MATERIAL)
            strcat(buffer, " (SINGLE MATERIAL)");
        if (blend_mode.blend_bits & BMB_BUILDING)
            strcat(buffer, " (BUILDING)");
        if (blend_mode.blend_bits & BMB_OLDTINTMODE)
            strcat(buffer, " (OLD TINT)");

        // currently BMB_CUBEMAP is set on the fly based on ReflectTex trick flag
        //if (blend_mode.blend_bits & BMB_CUBEMAP)
        //    strcat(buffer, " (CUBEMAP)");

        // currently BMB_PLANAR_REFLECTION is set on the fly
        //if (blend_mode.blend_bits & BMB_PLANAR_REFLECTION)
        //    strcat(buffer, " (PLANAR REFLECTION)");

        return buffer;
    }

    return "";
}

static const char *editSelMaterialTexName(ContextCommand *cmd)
{
    int idx, layer;
    if (!cmd)
        return "";
    idx = cmd->param % 10;
    layer = cmd->param / 10;
    if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && idx < sel_list[0].def_tracker->def->model->tex_count)
    {
        static char buffer[1024];
        TexBind *bind;
        int i, tri_idx = 0;
        for (i = 0; i < idx; i++)
            tri_idx += sel_list[0].def_tracker->def->model->tex_idx[i].count;
        bind = getTexWithReplaces(sel_list[0].def_tracker, tri_idx);
        if (!bind || !bind->tex_layers[layer])
            return "";
        strcpy(buffer, bind->tex_layers[layer]->actualTexture->name);
        return buffer;
    }

    return "";
}

static const char *editSelModelName(ContextCommand *cmd)
{
    if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model)
    {
        static char buffer[1024];
        char *s = strrchr(sel_list[0].def_tracker->def->model->filename, '/');
        if (s)
            s++;
        else
            s = sel_list[0].def_tracker->def->model->filename;
        sprintf(buffer, "%s (%s)", sel_list[0].def_tracker->def->model->name, s);
        return buffer;
    }

    return "";
}

static const char *editSelTrickName(ContextCommand *cmd)
{
    if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model)
    {
        TrickNode *trick = sel_list[0].def_tracker->def->model->trick;
        if (trick && trick->info)
            return trick->info->name;
        return "NONE";
    }

    return "";
}

static const char *autosaveAppend(ContextCommand *cmd)
{
    static char append[1024];
    if (edit_state.autosave)
        sprintf(append," ON: %dm",edit_state.autosavewait/(60*CLK_TCK));
    else
        sprintf(append," OFF...");

    return append;
}

static const char *burningSpeedAppend(ContextCommand *cmd)
{
    static char append[1024];
    sprintf(append," %f%%%%/s",edit_state.burningBuildingSpeed);
    return append;
}

static const char *loadRecentAppend(ContextCommand *cmd)
{
    extern MRUList * recentMaps;
    if (!recentMaps || cmd->param >= recentMaps->count)
        return "";

    if (strrchr(recentMaps->values[cmd->param],'/')==NULL)
        return recentMaps->values[cmd->param];
    return strrchr(recentMaps->values[cmd->param],'/')+1;
}

static int loadRecentAvailable(ContextCommand *cmd)
{
    extern MRUList * recentMaps;
    return recentMaps && cmd->param < recentMaps->count;
}

//////////////////////////////////////////////////////////////////////////

static int editSelOneCallback(void *data)
{
    return editSelOne((ContextCommand*)data);
}

static int editSelOneModelCallback(void *data)
{
    return editSelOneModel((ContextCommand*)data);
}

static int editSelMaterialCallback(void *data)
{
    return editSelMaterial((ContextCommand*)data);
}

static int editSelMaterialTexCallback(void *data)
{
    return editSelMaterialTex((ContextCommand*)data);
}

static int editSelLightCallback(void *data)
{
    return editSelLight((ContextCommand*)data);
}

static int editSelCubemapCallback(void *data)
{
    return editSelCubemap((ContextCommand*)data);
}

static int editSelTexOldCallback(void *data)
{
    return editSelTexOld((ContextCommand*)data);
}

static int editSelTexNewCallback(void *data)
{
    return editSelTexNew((ContextCommand*)data);
}

static int editSelTintCallback(void *data)
{
    return editSelTint((ContextCommand*)data);
}

static int editSelFogCallback(void *data)
{
    return editSelFog((ContextCommand*)data);
}

static int editSelSoundCallback(void *data)
{
    return editSelSound((ContextCommand*)data);
}

static int editSelFXCallback(void *data)
{
    return editSelFX((ContextCommand*)data);
}

static int editSelVolumeCallback(void *data)
{
    return editSelVolume((ContextCommand*)data);
}

static int editLodsCallback(void *data)
{
    return editLods((ContextCommand*)data);
}

static int editLibPieceCallback(void *data)
{
    return editLibPiece((ContextCommand*)data);
}

static int loadRecentAvailableCallback(void *data)
{
    return loadRecentAvailable((ContextCommand*)data);
}

static ContextCommand fileRecentCommands[] =
{
    { "", "loadRecent 4", 0, loadRecentAvailableCallback, loadRecentAppend, 0, 4 },
    { "", "loadRecent 3", 0, loadRecentAvailableCallback, loadRecentAppend, 0, 3 },
    { "", "loadRecent 2", 0, loadRecentAvailableCallback, loadRecentAppend, 0, 2 },
    { "", "loadRecent 1", 0, loadRecentAvailableCallback, loadRecentAppend, 0, 1 },
    { "", "loadRecent 0", 0, loadRecentAvailableCallback, loadRecentAppend, 0, 0 },
    { 0 },
};

static ContextCommand fileCommands[] = 
{
    { "Load Recent", 0, fileRecentCommands },
    { "Load...", "load 1" },
    { "Import...", "import 1" },
    { "New", "new 1" },
    { "Save (F7)", "save 1" },
    { "Save Selection...", "savesel 1" },
    { "Export Selection To VRML...", "exportsel 1" },
    { "Save As...", "saveas 1" },
    { "Scene File...", "scenefile 1" },
    { "Loadscreen...", "loadscreen 1" },
    { "Client Save", "clientsave 1" },
    { "Autosave", "++autosave", 0, 0, autosaveAppend },
    { 0 },
};

static ContextCommand groupLodCommands[] = 
{
    { "Copy AutoLOD Trick", "copyAutoLodParams" },
    { "Scale...", "lodscale 1" },
    { "Far Distance...", "lodfar 1" },
    { "Farfade...", "lodfarfade 1" },
    { "Disable", "loddisable 1" },
    { "Enable", "lodenable 1" },
    { "Fade Node", "lodfadenode 1" },
    { "Fade Node 2", "lodfadenode2 1" },
    { 0 },
} ;

static ContextCommand groupCommands[] =
{
    { "LOD Parameters", 0, groupLodCommands },
    { "Group (&g)", "group 1" },
    { "Ungroup (&u)", "ungroup 1" },
    { "Delete (del)", "del 1" },
    { "libdel", "libdel 1" },
    { "Open (&[)", "open 1" },
    { "Close (&])", "close 1" },
    { "Set As Parent", "setparent 1" },
    { "Attach", "attach 1" },
    { "Detach", "detach 1" },
    { "Name...", "name 1" },
    { "Make Libs", "makelibs 1" },
    { "Save Libs...", "savelibs 1" },
    { "Set Pivot (F8)", "setpivot 1" },
    { "Set Type...", "settype 1" },
    { "Set FX...", "setfx 1" },
    { "Tray Swap", "trayswap 1" },
    { "Set NoFogClip", "noFogClip 1" },
    { "Set NoColl", "editNoColl 1" },
    { "Make Lib Piece into Costume", "makeDestructibleObject"  },
    { 0 },
};

static ContextCommand adjustBeaconCommands[] = 
{
    { "Name", "beaconname 1" },
    { "Size (&b)", "++beaconsize", 0, 0, 0, &edit_state.beaconSize },
    { "Show Connection", "showconnection 1" },
    { "Radii", "++beaconradii", 0, 0, 0, &edit_state.beaconradii },
    { "Select (&0)", "++beaconselect", 0, 0, 0, &edit_state.beaconSelect },
    { 0 },
};

static ContextCommand adjustLightCommands[] = 
{
    { "Ambient", "setambient 1" },
    { "Remove Ambient", "unsetambient 1" },
    { "maxbright", "maxbright 1" },
    { "Color", "lightcolor 1" },
    { "Size", "++lightsize", 0, 0, 0, &edit_state.lightsize },
    { 0 },
};

static ContextCommand adjustVolumeCommands[] = 
{
    { "Set Volume Size...", "boxscale 1" },
    { 0 },
};

static ContextCommand adjustFogCommands[] = 
{
    { "Near Distance", "fognear 1" },
    { "Far Distance", "fogfar 1" },
    { "Color 1", "fogcolor1 1" },
    { "Color 2", "fogcolor2 1" },
    { "Size", "++fogsize", 0, 0, 0, &edit_state.fogsize },
    { "Fade Speed", "fogspeed 1" },
    { 0 },
};

static ContextCommand adjustTintCommands[] = 
{
    { "Color 1", "tintcolor1 1" },
    { "Color 2", "tintcolor2 1" },
    { 0 },
};

static ContextCommand adjustTexCommands[] = 
{
    { "Replace Tex1", "replacetex1 1" },
    { "Replace Tex2", "replacetex2 1" },
    { "Remove Tex", "removetex 1" },
    { "Texture Swap", "textureswap 1" },
    { "Edit TexWord", "editTexWord" },
    { 0 },
};

static ContextCommand adjustSoundCommands[] = 
{
    { "Volume", "soundvol 1" },
    { "Size", "++soundsize", 0, 0, 0, &edit_state.soundsize },
    { "Ramp", "soundramp 1" },
    { "Name", "soundname 1" },
    { "Find", "soundfind 1" },
    { "Exclude", "soundexclude 1" },
    { "Script", "soundscript 1" },
    { 0 },
};

static ContextCommand adjustPropCommands[] = 
{
    { "Add (&a)", "addProp 1" },
    { "Remove", "removeProp 1" },
    { "Show As Radius", "showAsRadius 1" },
    { "Show As String", "showAsString 1" },
    { "Copy", "copyProperties 1" },
    { "Paste", "pasteProperties 1" },
    { 0 },
};

static ContextCommand adjustBurningBuildingsCommands[] = 
{
    { "Destroy", "destroy 1" },
    { "Repair", "repair 1" },
    { "Speed", "speed 1", 0, 0, burningSpeedAppend },
    { "Stop", "stop 1" },
    { 0 },
};

static ContextCommand adjustCommands[] =
{
    { "Beacons", 0, adjustBeaconCommands },
    { "Light", 0, adjustLightCommands },
    { "Volume", 0, adjustVolumeCommands },
    { "Fog", 0, adjustFogCommands },
    { "Tint", 0, adjustTintCommands },
    { "Texture", 0, adjustTexCommands },
    { "Sound", 0, adjustSoundCommands },
    { "Properties", 0, adjustPropCommands },
    { "Burning Buildings", 0, adjustBurningBuildingsCommands },
    { 0 },
};

static ContextCommand selectCommands[] =
{
    { "Cut (&x)", "cut 1" },
    { "Copy (&c)", "copy 1" },
    { "Paste (&v)", "paste 1" },
    { "Undo (F12)", "undo 1" },
    { "Select All", "selectall 1" },
    { "Select All Instances Of This Object", "selectallinstancesoffthisobject 1", 0, editSelOneModelCallback },
    { "Unselect (ESC)", "unsel 1" },
    { "Hide (&h)", "hide 1" },
    { "Hide Others", "hideothers 1" },
    { "Unhide All (&j)", "unhide 1" },
    { "Freeze", "freeze 1" },
    { "Freeze Others", "freezeothers 1" },
    { "Unfreeze All", "unfreeze 1" },
    { "Invert Selection", "invertSelection 1" },
    { "Search", "search 1" },
    { "Set View (ALT)", "setview 1" },
    { "Ground Snap (&z)", "groundsnap 1" },
    { "Slope Snap", "slopesnap 1" },
    { "Select Contents", "selcontents 1" },
    { "Set Quick Object (&6)", "setquickobject 1" },
    { "Toggle Quickplace (&7)", "togglequickplace 1" },
    { 0 },
};

static ContextCommand knobsEditCommands[] =
{
    { "Position/Rotation (&q)", "++posrot", 0, 0, 0, &edit_state.posrot },
    { "Increase Grid Size (&m)", "gridsize" },
    { "Decrease Grid Size (&n)", "gridshrink" },
    { "Working Plane (TAB)", "++plane" },
    { "Snap Type (&s)", "++snaptype", 0, 0, 0, &edit_state.snaptype },
    { "NoSnap (F5)", "++nosnap", 0, 0, 0, &edit_state.nosnap },
    { "SnapToVert (F6)", "++snaptovert", 0, 0, 0, &edit_state.snaptovert },
    { "Axis (1/2/3)", "++singleaxis" },
    { "Local Rotation", "++localrot", 0, 0, 0, &edit_state.localrot },
    { "Use Object Axes", "++useObjectAxes", 0, 0, 0, &edit_state.useObjectAxes },
    { "Zero Grid", "++zerogrid", 0, 0, 0, &edit_state.zerogrid },
    { "Snap3", "++snap3", 0, 0, 0, &edit_state.snap3 },
    { 0 },
};

static ContextCommand knobsMiscCommands[] =
{
    { "Show All", "++showall", 0, 0, 0, &edit_state.showall },
    { "Edit Original", "++editorig", 0, 0, 0, &edit_state.editorig },
    { "Edit Lib", "editlib" },
    { "Toggle Show Fog", "++showfog", 0, 0, 0, &edit_state.showfog },
    { "Toggle Show Vis", "++showvis", 0, 0, 0, &edit_state.showvis },
    { "Ignore Tray", "++ignoretray", 0, 0, 0, &edit_state.ignoretray },
    { "Color By Number", "++colorbynumber", 0, 0, 0, &edit_state.colorbynumber },
    { "Color Memory", "++colormemory", 0, 0, 0, &edit_state.colormemory },
    { "Toggle Object Info (&o)", "++objinfo", 0, 0, 0, &edit_state.objinfo },
    { "Toggle No Error Check", "++noerrcheck", 0, 0, 0, &edit_state.noerrcheck },
    { "Profiler", "++profiler", 0, 0, 0, &edit_state.profiler },
    { "Camera Placement", "++cameraPlacement", 0, 0, 0, &edit_state.cameraPlacement },
    { "Live Tray Linking", "++liveTrayLink", 0, 0, 0, &edit_state.liveTrayLink },
    { "Show Group Bounds", "++showGroupBounds", 0, 0, 0, &edit_state.showBounds },
    { "Hide Collision Volumes", "++hideCollisionVolumes", 0, 0, 0, &edit_state.hideCollisionVolumes },
    { 0 },
};

static ContextCommand knobsCommands[] =
{
    { "Edit", 0, knobsEditCommands },
    { "Misc", 0, knobsMiscCommands },
    { 0 },
};

//////////////////////////////////////////////////////////////////////////

static ContextCommand contextCommands[] =
{
    { "Set Ambient", "setambient 1", 0, editSelLightCallback },
    { "Remove Ambient", "unsetambient 1", 0, editSelLightCallback },
    { "maxbright", "maxbright 1", 0, editSelLightCallback },
    { "Light Color", "lightcolor 1", 0, editSelLightCallback },
    { "Light Size", "++lightsize", 0, editSelLightCallback, 0, &edit_state.lightsize },
    { "Adjust Cubemap", "adjustcubemap 1", 0, editSelCubemapCallback },
    { "-", 0, 0, editSelLightCallback },

    { "Replace Tex1", "replacetex1 1", 0, editSelTexOldCallback },
    { "Replace Tex2", "replacetex2 1", 0, editSelTexOldCallback },
    { "Remove Tex", "removetex 1", 0, editSelTexOldCallback },
    { "-", 0, 0, editSelTexOldCallback },

    { "Texture Swap", "textureswap 1", 0, editSelTexNewCallback },
    { "-", 0, 0, editSelTexNewCallback },

    { "Tint Color 1", "tintcolor1 1", 0, editSelTintCallback },
    { "Tint Color 2", "tintcolor2 1", 0, editSelTintCallback },
    { "-", 0, 0, editSelTintCallback },

    { "Fog Near Distance", "fognear 1", 0, editSelFogCallback },
    { "Fog Far Distance", "fogfar 1", 0, editSelFogCallback },
    { "Fog Color 1", "fogcolor1 1", 0, editSelFogCallback },
    { "Fog Color 2", "fogcolor2 1", 0, editSelFogCallback },
    { "Fog Size", "++fogsize", 0, editSelFogCallback, 0, &edit_state.fogsize },
    { "Fog Fade Speed", "fogspeed 1", 0, editSelFogCallback },
    { "-", 0, 0, editSelFogCallback },

    { "Sound Volume", "soundvol 1", 0, editSelSoundCallback },
    { "Sound Size", "++soundsize", 0, editSelSoundCallback, 0, &edit_state.soundsize },
    { "Sound Ramp", "soundramp 1", 0, editSelSoundCallback },
    { "Sound Name", "soundname 1", 0, editSelSoundCallback },
    { "Sound Find", "soundfind 1", 0, editSelSoundCallback },
    { "Sound Exclude", "soundexclude 1", 0, editSelSoundCallback },
    { "Sound Script", "soundscript 1", 0, editSelSoundCallback },
    { "-", 0, 0, editSelSoundCallback },

    { "Set Type...", "settype 1", 0, editSelFXCallback },
    { "Set FX...", "setfx 1", 0, editSelFXCallback },
    { "-", 0, 0, editSelFXCallback },

    { "Set Volume Size...", "boxscale 1", 0, editSelVolumeCallback },
    { "-", 0, 0, editSelVolumeCallback },

    { "Edit LODs...", "editlods", 0, editLodsCallback },
    { "-", 0, 0, editLodsCallback },

    { "Find In Library", "findlibpiece", 0, editLibPieceCallback },
    { "-", 0, 0, editLibPieceCallback },

    { 0 },
};

static ContextCommand rootCommands[] = 
{
    { "File", 0, fileCommands },
    { "Group", 0, groupCommands },
    { "Adjust", 0, adjustCommands },
    { "Select", 0, selectCommands },
    { "Knobs", 0, knobsCommands },
    { 0 },
};

static ContextCommand materialCommands0[] = 
{
    { "Edit ", "editTexBind 0", 0, editSelMaterialCallback, editSelMaterialName, 0, 0 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 0 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 0 },
    { "Base1: ", "editMaterial 00", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 00 },
    { "Mul1:  ", "editMaterial 10", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 10 },
    { "Bump1: ", "editMaterial 20", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 20 },
    { "Dual1: ", "editMaterial 30", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 30 },
    { "AddGlow: ", "editMaterial 90", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 90 },
    { "Mask:  ", "editMaterial 40", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 40 },
    { "Base2: ", "editMaterial 50", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 50 },
    { "Mul2:  ", "editMaterial 60", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 60 },
    { "Bump2: ", "editMaterial 70", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 70 },
    { "Dual2: ", "editMaterial 80", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 80 },
    { "Cubemap: ", "editMaterial 100", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 100 },
    { 0 },
};

static ContextCommand materialCommands1[] = 
{
    { "Edit ", "editTexBind 1", 0, editSelMaterialCallback, editSelMaterialName, 0, 1 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 1 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 1 },
    { "Base1: ", "editMaterial 01", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 01 },
    { "Mul1:  ", "editMaterial 11", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 11 },
    { "Bump1: ", "editMaterial 21", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 21 },
    { "Dual1: ", "editMaterial 31", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 31 },
    { "AddGlow: ", "editMaterial 91", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 91 },
    { "Mask:  ", "editMaterial 41", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 41 },
    { "Base2: ", "editMaterial 51", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 51 },
    { "Mul2:  ", "editMaterial 61", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 61 },
    { "Bump2: ", "editMaterial 71", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 71 },
    { "Dual2: ", "editMaterial 81", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 81 },
    { "Cubemap: ", "editMaterial 101", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 101 },
    { 0 },
};

static ContextCommand materialCommands2[] = 
{
    { "Edit ", "editTexBind 2", 0, editSelMaterialCallback, editSelMaterialName, 0, 2 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 2 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 2 },
    { "Base1: ", "editMaterial 02", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 02 },
    { "Mul1:  ", "editMaterial 12", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 12 },
    { "Bump1: ", "editMaterial 22", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 22 },
    { "Dual1: ", "editMaterial 32", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 32 },
    { "AddGlow: ", "editMaterial 92", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 92 },
    { "Mask:  ", "editMaterial 42", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 42 },
    { "Base2: ", "editMaterial 52", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 52 },
    { "Mul2:  ", "editMaterial 62", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 62 },
    { "Bump2: ", "editMaterial 72", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 72 },
    { "Dual2: ", "editMaterial 82", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 82 },
    { "Cubemap: ", "editMaterial 102", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 102 },
    { 0 },
};

static ContextCommand materialCommands3[] = 
{
    { "Edit ", "editTexBind 3", 0, editSelMaterialCallback, editSelMaterialName, 0, 3 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 3 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 3 },
    { "Base1: ", "editMaterial 03", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 03 },
    { "Mul1:  ", "editMaterial 13", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 13 },
    { "Bump1: ", "editMaterial 23", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 23 },
    { "Dual1: ", "editMaterial 33", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 33 },
    { "AddGlow: ", "editMaterial 93", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 93 },
    { "Mask:  ", "editMaterial 43", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 43 },
    { "Base2: ", "editMaterial 53", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 53 },
    { "Mul2:  ", "editMaterial 63", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 63 },
    { "Bump2: ", "editMaterial 73", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 73 },
    { "Dual2: ", "editMaterial 83", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 83 },
    { "Cubemap: ", "editMaterial 103", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 103 },
    { 0 },
};

static ContextCommand materialCommands4[] = 
{
    { "Edit ", "editTexBind 4", 0, editSelMaterialCallback, editSelMaterialName, 0, 4 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 4 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 4 },
    { "Base1: ", "editMaterial 04", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 04 },
    { "Mul1:  ", "editMaterial 14", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 14 },
    { "Bump1: ", "editMaterial 24", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 24 },
    { "Dual1: ", "editMaterial 34", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 34 },
    { "AddGlow: ", "editMaterial 94", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 94 },
    { "Mask:  ", "editMaterial 44", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 44 },
    { "Base2: ", "editMaterial 54", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 54 },
    { "Mul2:  ", "editMaterial 64", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 64 },
    { "Bump2: ", "editMaterial 74", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 74 },
    { "Dual2: ", "editMaterial 84", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 84 },
    { "Cubemap: ", "editMaterial 104", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 104 },
    { 0 },
};

static ContextCommand materialCommands5[] = 
{
    { "Edit ", "editTexBind 5", 0, editSelMaterialCallback, editSelMaterialName, 0, 5 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 5 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 5 },
    { "Base1: ", "editMaterial 05", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 05 },
    { "Mul1:  ", "editMaterial 15", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 15 },
    { "Bump1: ", "editMaterial 25", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 25 },
    { "Dual1: ", "editMaterial 35", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 35 },
    { "AddGlow: ", "editMaterial 95", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 95 },
    { "Mask:  ", "editMaterial 45", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 45 },
    { "Base2: ", "editMaterial 55", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 55 },
    { "Mul2:  ", "editMaterial 65", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 65 },
    { "Bump2: ", "editMaterial 75", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 75 },
    { "Dual2: ", "editMaterial 85", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 85 },
    { "Cubemap: ", "editMaterial 105", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 105 },
    { 0 },
};

static ContextCommand materialCommands6[] = 
{
    { "Edit ", "editTexBind 6", 0, editSelMaterialCallback, editSelMaterialName, 0, 6 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 6 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 6 },
    { "Base1: ", "editMaterial 06", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 06 },
    { "Mul1:  ", "editMaterial 16", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 16 },
    { "Bump1: ", "editMaterial 26", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 26 },
    { "Dual1: ", "editMaterial 36", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 36 },
    { "AddGlow: ", "editMaterial 96", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 96 },
    { "Mask:  ", "editMaterial 46", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 46 },
    { "Base2: ", "editMaterial 56", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 56 },
    { "Mul2:  ", "editMaterial 66", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 66 },
    { "Bump2: ", "editMaterial 76", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 76 },
    { "Dual2: ", "editMaterial 86", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 86 },
    { "Cubemap: ", "editMaterial 106", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 106 },
    { 0 },
};

static ContextCommand materialCommands7[] = 
{
    { "Edit ", "editTexBind 7", 0, editSelMaterialCallback, editSelMaterialName, 0, 7 },
    { "-", 0, 0, editSelMaterialCallback, 0, 0, 7 },
    { "", 0, 0, editSelMaterialCallback, editSelMaterialBlendMode, 0, 7 },
    { "Base1: ", "editMaterial 07", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 07 },
    { "Mul1:  ", "editMaterial 17", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 17 },
    { "Bump1: ", "editMaterial 27", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 27 },
    { "Dual1: ", "editMaterial 37", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 37 },
    { "AddGlow: ", "editMaterial 97", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 97 },
    { "Mask:  ", "editMaterial 47", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 47 },
    { "Base2: ", "editMaterial 57", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 57 },
    { "Mul2:  ", "editMaterial 67", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 67 },
    { "Bump2: ", "editMaterial 77", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 77 },
    { "Dual2: ", "editMaterial 87", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 87 },
    { "Cubemap: ", "editMaterial 107", 0, editSelMaterialTexCallback, editSelMaterialTexName, 0, 107 },
    { 0 },
};

static ContextCommand modelCommands[] = 
{
    { "Group" },
    { "", "setview 1", 0, 0, editSelName },
    { "Model", 0, 0, editSelOneModelCallback },
    { "", "editModel", 0, editSelOneModelCallback, editSelModelName },
    { "Trick", 0, 0, editSelOneModelCallback },
    { "", "editTrick", 0, editSelOneModelCallback, editSelTrickName },
    { "Materials", 0, 0, editSelOneModelCallback },
    { "", 0, materialCommands0, editSelMaterialCallback, editSelMaterialName, 0, 0 },
    { "", 0, materialCommands1, editSelMaterialCallback, editSelMaterialName, 0, 1 },
    { "", 0, materialCommands2, editSelMaterialCallback, editSelMaterialName, 0, 2 },
    { "", 0, materialCommands3, editSelMaterialCallback, editSelMaterialName, 0, 3 },
    { "", 0, materialCommands4, editSelMaterialCallback, editSelMaterialName, 0, 4 },
    { "", 0, materialCommands5, editSelMaterialCallback, editSelMaterialName, 0, 5 },
    { "", 0, materialCommands6, editSelMaterialCallback, editSelMaterialName, 0, 6 },
    { "", 0, materialCommands7, editSelMaterialCallback, editSelMaterialName, 0, 7 },
    { 0 },
};

//////////////////////////////////////////////////////////////////////////

static int editContextCommandVisible(void *param)
{
    ContextCommand *cmd = param;

    if (!cmd->is_available_func || cmd->is_available_func(cmd))
    {
        if (cmd->cmd_name || cmd->submenu)
            return CM_AVAILABLE;
        return CM_VISIBLE;
    }

    return CM_HIDE;
}

static void editContextCommand(void *param)
{
    ContextCommand *cmd = param;
    if (cmd->submenu)
        return;

    if (cmd->cmd_name)
    {
        if (!editCmdParse(cmd->cmd_name, 0, 0))
            cmdParse(cmd->cmd_name);
    }
}

static const char *editContextCommandName(void *param)
{
    static char buffer[1024];
    const char *append_str = 0;
    ContextCommand *cmd = param;

    if (cmd->name_append_func)
        append_str = cmd->name_append_func(cmd);

    if (!append_str && cmd->toggle_var)
    {
        if (*cmd->toggle_var)
            append_str = "    < ON >";
    }

    if (!append_str)
        return cmd->name;

    sprintf(buffer, "%s%s", cmd->name, append_str);

    return buffer;
}

static void editAddContextCommands(ContextMenu *parent_menu, ContextCommand *commands)
{
    int i;
    for (i = 0; commands[i].name; i++)
    {
        if (commands[i].submenu)
        {
            ContextMenu *child_menu = contextMenu_Create(0);
            contextMenu_SetCustomColors(child_menu, BORDER_COLOR, BACK_COLOR);
            editAddContextCommands(child_menu, commands[i].submenu);
            contextMenu_addVariableTextCode(parent_menu, editContextCommandVisible, (void *)(&commands[i]), 0, 0, editContextCommandName, (void *)(&commands[i]), child_menu);
        }
        else if (strcmp(commands[i].name, "-")==0)
        {
            contextMenu_addDividerVisible(parent_menu, commands[i].is_available_func);
        }
        else if (!commands[i].cmd_name)
        {
            contextMenu_addVariableTitleVisible(parent_menu, editContextCommandVisible, (void *)(&commands[i]), editContextCommandName, (void *)(&commands[i]));
        }
        else
        {
            contextMenu_addVariableTextCode(parent_menu, editContextCommandVisible, (void *)(&commands[i]), editContextCommand, (void *)(&commands[i]), editContextCommandName, (void *)(&commands[i]), 0);
        }
    }

    contextMenu_SetNoTranslate(parent_menu, 1);
}

void editAddCustomCommands(ContextMenu *parent_menu, char *filename)
{
    int i;
    ContextMenu *child_menu;
    TokenizerHandle tok;
    ContextCommandList *clist;

    tok = TokenizerCreate(filename);
    if( !tok )
    {
        return;
    }
    else
    {
        clist = malloc(sizeof(ContextCommandList));
        eaCreate(&clist->commands);
        TokenizerParseList( tok, parseContextCommandList, clist, TokenizerErrorCallback );
        TokenizerDestroy(tok);
    }

    if (!eaSize(&clist->commands))
    {
        eaDestroy(&clist->commands);
        free(clist);
        return;
    }

    child_menu = contextMenu_Create(0);
    contextMenu_SetCustomColors(child_menu, BORDER_COLOR, BACK_COLOR);
    for (i = 0; i < eaSize(&clist->commands); i++)
        contextMenu_addVariableTextCode(child_menu, editContextCommandVisible, (void *)(clist->commands[i]), editContextCommand, (void *)(clist->commands[i]), editContextCommandName, (void *)(clist->commands[i]), 0);
    contextMenu_SetNoTranslate(child_menu, 1);

    contextMenu_addCode(parent_menu, 0, 0, 0, 0, "Custom", child_menu);
    contextMenu_SetNoTranslate(parent_menu, 1);
}

//////////////////////////////////////////////////////////////////////////

void editContextMenuShow(int x, int y, int model_menu)
{
    static ContextMenu *edit_cm = 0;
    static ContextMenu *model_cm = 0;
    if (!edit_cm)
    {
        edit_cm = contextMenu_Create(0);
        contextMenu_SetCustomColors(edit_cm, BORDER_COLOR, BACK_COLOR);
        editAddContextCommands(edit_cm, contextCommands);
        editAddContextCommands(edit_cm, rootCommands);
        if (fileExists("C:/editmenu.txt"))
            editAddCustomCommands(edit_cm, "C:/editmenu.txt");
    }
    if (!model_cm)
    {
        model_cm = contextMenu_Create(0);
        contextMenu_SetCustomColors(model_cm, BORDER_COLOR, BACK_COLOR);
        editAddContextCommands(model_cm, modelCommands);
    }

    if (!model_menu)
        contextMenu_set(edit_cm, x, y, NULL);
    else if (editSelOne(0))
        contextMenu_set(model_cm, x, y, NULL);
}
