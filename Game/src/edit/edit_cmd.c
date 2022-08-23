#include <utilitieslib/stdtypes.h>
#include "string.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include "cmdparse/cmdgame.h"
#include "cmdparse/cmdcommon.h"
#include "edit/edit_cmd.h"
#include "edit/edit_library.h"
#include "UI/uiConsole.h"
#include "clientcomm/clientcomm.h"
#include "edit/edit_net.h"
#include "win/win_init.h"
#include "entity/entity.h"
#include "player/player.h"
#include "UI/uiKeybind.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/EString.h>

#include "gameComm/Npc.h"        // For NPC structure defintion
#include "graphics/seqgraphics.h"
#include "win/input.h"
#include "group/groupProperties.h"
#include "edit_info.h"
#include "edit/edit_select.h"
#include "edit_errcheck.h"
#include "render/texWords.h"
#include "group/groupnetrecv.h"
#include "render/tex.h"
#include "seq/seq.h"
#include <utilitieslib/utils/fileutil.h>

#include "group/group.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include "seq/anim.h"

#include <utilitieslib/components/MRUList.h>

#include <utilitieslib/utils/sysutil.h>
#include "edit/ClickToSource.h"
#include "edit_lods.h"
#include "graphics/camera.h"

KeyBindProfile edit_binds_profile;
EditState    edit_state;
int            edit_mode;
extern Vec3 view_pos;

void editTexWord(void);
void editGetSelectedLodParams(char *buffer);
void editTrickFile(const char *filename, const char *trickname, int is_texture);

enum
{
    CMD_GRIDSIZE = 1,
    CMD_GRIDSHRINK,
    CMD_SINGLEAXIS,
    CMD_PLANE,
    CMD_FREEMOUSE,
    CMD_EXITEDIT,
    CMD_EXITEDITNOMOVE,
    CMD_EDITLIB,
    CMD_EDITTEXWORD,
    CMD_COPYLODPARAMS,
    CMD_LOADRECENT,
    CMD_EDITMODEL,
    CMD_EDITTRICK,
    CMD_EDITTEXBIND,
    CMD_EDITMATERIAL,
    CMD_MAKEDESTRUCTIBLEOBJECT,
    CMD_EDITLODS,
    CMD_EDITCAMPYR,
    CMD_FINDLIBPIECE,
};

static int temp_int;
static Vec3 temp_vec3;
#define TEMP_VEC3 {PARSETYPE_FLOAT,&temp_vec3[0]},{PARSETYPE_FLOAT,&temp_vec3[1]},{PARSETYPE_FLOAT,&temp_vec3[2]}

Cmd edit_cmds[] =
{
    { 0, "sel", 0, {{ PARSETYPE_S32, &edit_state.sel }} },
    { 0, "unsel", 0, {{ PARSETYPE_S32, &edit_state.unsel }} },
    { 0, "copy", 0, {{ PARSETYPE_S32, &edit_state.copy }} },
    { 0, "cut", 0, {{ PARSETYPE_S32, &edit_state.cut }} },
    { 0, "paste", 0, {{ PARSETYPE_S32, &edit_state.paste }} },
    { 0, "undo", 0, {{ PARSETYPE_S32, &edit_state.undo }} },
    { 0, "redo", 0, {{ PARSETYPE_S32, &edit_state.redo }} },
    { 0, "groundsnap", 0, {{ PARSETYPE_S32, &edit_state.groundsnap }} },
    { 0, "slopesnap", 0, {{ PARSETYPE_S32, &edit_state.slopesnap }} },
    { 0, "groundoffset", 0, {{ PARSETYPE_FLOAT, &edit_state.groundoffset }} },
    { 0, "selectall", 0, {{ PARSETYPE_S32, &edit_state.selectall }} },
    { 0, "selectallinstancesoffthisobject", 0, 
        {{ PARSETYPE_S32, &edit_state.selectAllInstancesOfThisObject }} },
    { 0, "copyProperties", 0, {{ PARSETYPE_S32, &edit_state.copyProperties }} },
    { 0, "pasteProperties", 0, {{ PARSETYPE_S32, &edit_state.pasteProperties }} },

    { 0, "libdel", 0, {{ PARSETYPE_S32, &edit_state.libdel }} },
    { 0, "ungroup", 0, {{ PARSETYPE_S32, &edit_state.ungroup }} },
    { 0, "group", 0, {{ PARSETYPE_S32, &edit_state.group }} },
    { 0, "attach", 0, {{ PARSETYPE_S32, &edit_state.attach }} },
    { 0, "detach", 0, {{ PARSETYPE_S32, &edit_state.detach }} },
    { 0, "setparent", 0, {{ PARSETYPE_S32, &edit_state.setparent }} },
    { 0, "setactivelayer", 0, {{ PARSETYPE_S32, &edit_state.setactivelayer }} },
    { 0, "editorig", 0, {{ PARSETYPE_S32, &edit_state.editorig }} },
    { 0, "editlib", CMD_EDITLIB, },
    { 0, "noerrcheck", 0, {{ PARSETYPE_S32, &edit_state.noerrcheck }} },
    { 0, "del", 0, {{ PARSETYPE_S32, &edit_state.del }} },
    { 0, "settype", 0, {{ PARSETYPE_S32, &edit_state.settype }} },
    { 0, "setfx", 0, {{ PARSETYPE_S32, &edit_state.setfx }} },
    { 0, "name", 0, {{ PARSETYPE_S32, &edit_state.name }} },
    { 0, "makelibs", 0, {{ PARSETYPE_S32, &edit_state.makelibs }} },
    { 0, "trayswap", 0, {{ PARSETYPE_S32, &edit_state.trayswap }} },

    { 0, "speed", 0, {{ PARSETYPE_S32, &edit_state.burningBuildingSpeedChange }} },
    { 0, "destroy", 0, {{ PARSETYPE_S32, &edit_state.burningBuildingDestroy }} },
    { 0, "repair", 0, {{ PARSETYPE_S32, &edit_state.burningBuildingRepair }} },
    { 0, "controller", 0, {{ PARSETYPE_S32, &edit_state.destroyableObjectController }} },
    { 0, "stop", 0, {{ PARSETYPE_S32, &edit_state.burningBuildingStop }} },

    { 0, "setambient", 0, {{ PARSETYPE_S32, &edit_state.setambient }} },
    { 0, "unsetambient", 0, {{ PARSETYPE_S32, &edit_state.unsetambient }} },
    { 0, "maxbright", 0, {{ PARSETYPE_S32, &edit_state.maxbright }} },
    { 0, "lightcolor", 0, {{ PARSETYPE_S32, &edit_state.lightcolor }} },
    { 0, "lightsize", 0, {{ PARSETYPE_S32, &edit_state.lightsize, 2 }} },
    { 0, "colorbynumber", 0, {{ PARSETYPE_S32, &edit_state.colorbynumber }} },
    { 0, "colormemory", 0, {{ PARSETYPE_S32, &edit_state.colormemory }} },
    { 0, "boxscale", 0, {{ PARSETYPE_S32, &edit_state.boxscale}} },
    { 0, "adjustCubemap", 0, {{ PARSETYPE_S32, &edit_state.adjustCubemap}} },


    { 0, "soundvol", 0, {{ PARSETYPE_S32, &edit_state.soundvol }} },
    { 0, "soundname", 0, {{ PARSETYPE_S32, &edit_state.soundname }} },
    { 0, "soundsize", 0, {{ PARSETYPE_S32, &edit_state.soundsize, 2 }} },
    { 0, "soundramp", 0, {{ PARSETYPE_S32, &edit_state.soundramp }} },
    { 0, "soundfind", 0, {{ PARSETYPE_S32, &edit_state.soundfind }} },
    { 0, "soundexclude", 0, {{ PARSETYPE_S32, &edit_state.soundexclude }} },
    { 0, "soundscript", 0, {{ PARSETYPE_S32, &edit_state.soundscript}} },

    { 0, "fognear", 0, {{ PARSETYPE_S32, &edit_state.fognear }} },
    { 0, "fogfar", 0, {{ PARSETYPE_S32, &edit_state.fogfar }} },
    { 0, "fogcolor1", 0, {{ PARSETYPE_S32, &edit_state.fogcolor1 }} },
    { 0, "fogcolor2", 0, {{ PARSETYPE_S32, &edit_state.fogcolor2 }} },
    { 0, "fogsize", 0, {{ PARSETYPE_S32, &edit_state.fogsize, 2 }} },
    { 0, "fogspeed", 0, {{ PARSETYPE_S32, &edit_state.fogspeed }} },

    { 0, "setview", 0, {{ PARSETYPE_S32, &edit_state.setview }} },
    { 0, "camview", 0, {{ PARSETYPE_S32, &edit_state.camview }} },
    { 0, "viewpos", 0, {{ PARSETYPE_FLOAT, &view_pos[0]}, {PARSETYPE_FLOAT, &view_pos[1]}, {PARSETYPE_FLOAT, &view_pos[2]}}},
    { 0, "look", CMD_FREEMOUSE, {{ PARSETYPE_S32, &edit_state.look }} },
    { 0, "pan", CMD_FREEMOUSE, {{ PARSETYPE_S32, &edit_state.pan }} },
    { 0, "zoom", CMD_FREEMOUSE, {{ PARSETYPE_S32, &edit_state.zoom }} },

    { 0, "findNextProperty", 0, {{ PARSETYPE_S32, &edit_state.findNextProperty }} },

    { 0, "showfog", 0, {{ PARSETYPE_S32, &edit_state.showfog }} },
    { 0, "showvis", 0, {{ PARSETYPE_S32, &edit_state.showvis }} },
    { 0, "polycount", 0, {{ PARSETYPE_S32, &edit_state.polycount }} },
    { 0, "gridsize", CMD_GRIDSIZE,  },
    { 0, "gridshrink", CMD_GRIDSHRINK,  },
    { 0, "rotsize", CMD_GRIDSIZE,  },
    { 0, "plane", CMD_PLANE, {{ PARSETYPE_S32, &edit_state.plane, 2 }} },
    { 0, "posrot", 0, {{ PARSETYPE_S32, &edit_state.posrot }} },
    { 0, "zerogrid", 0, {{ PARSETYPE_S32, &edit_state.zerogrid }} },
    { 0, "snaptype", 0, {{ PARSETYPE_S32, &edit_state.snaptype }} },
    { 0, "snaptovert", 0, {{ PARSETYPE_S32, &edit_state.snaptovert }} },

    { 0, "localrot", 0, {{ PARSETYPE_S32, &edit_state.localrot }} },
    { 0, "snap3", 0, {{ PARSETYPE_S32, &edit_state.snap3 }} },
    { 0, "nosnap", 0, {{ PARSETYPE_S32, &edit_state.nosnap }} },
    { 0, "open", 0, {{ PARSETYPE_S32, &edit_state.open }} },
    { 0, "close", 0, {{ PARSETYPE_S32, &edit_state.close }} },
    { 0, "mousewheelopenclose", 0, {{ PARSETYPE_S32, &edit_state.mousewheelopenclose }} },
    { 0, "closeinst", 0, {{ PARSETYPE_S32, &edit_state.closeinst }} },
    { 0, "scale", 0, {{ PARSETYPE_S32, &edit_state.scale }} },
    { 0, "singleaxis", CMD_SINGLEAXIS, {{ PARSETYPE_S32, &edit_state.singleaxis }} },
    { 0, "useObjectAxes", 0, {{ PARSETYPE_S32, &edit_state.useObjectAxes }} },
    { 0, "load", 0, {{ PARSETYPE_S32, &edit_state.load }} },
    { 0, "import", 0, {{ PARSETYPE_S32, &edit_state.import }} },
    { 0, "new", 0, {{ PARSETYPE_S32, &edit_state.editstate_new }} },
    { 0, "save", 0, {{ PARSETYPE_S32, &edit_state.save }} },
    { 0, "autosave", 0, {{ PARSETYPE_S32, &edit_state.autosave }} },
    { 0, "scenefile", 0, {{ PARSETYPE_S32, &edit_state.scenefile }} },
    { 0, "loadscreen", 0, {{ PARSETYPE_S32, &edit_state.loadscreen }} },
    { 0, "savesel", 0, {{ PARSETYPE_S32, &edit_state.savesel }} },
    { 0, "exportsel", 0, {{ PARSETYPE_S32, &edit_state.exportsel }} },
    { 0, "savelibs", 0, {{ PARSETYPE_S32, &edit_state.savelibs }} },
    { 0, "saveas", 0, {{ PARSETYPE_S32, &edit_state.saveas }} },
    { 0, "clientsave", 0, {{ PARSETYPE_S32, &edit_state.clientsave }} },
    { 0, "select_notselectable", 0, {{ PARSETYPE_S32, &edit_state.select_notselectable }} },


    { 0, "selcontents", 0, {{ PARSETYPE_S32, &edit_state.selcontents }} },
    { 0, "showall", 0, {{ PARSETYPE_S32, &edit_state.showall }} },
    { 0, "setpivot", 0, {{ PARSETYPE_S32, &edit_state.setpivot }} },
    { 0, "hide", 0, {{ PARSETYPE_S32, &edit_state.hide }} },
    { 0, "unhide", 0, {{ PARSETYPE_S32, &edit_state.unhide }} },
    { 0, "hideothers", 0, {{ PARSETYPE_S32, &edit_state.hideothers }} },
    { 0, "freeze", 0, {{ PARSETYPE_S32, &edit_state.freeze }} },
    { 0, "unfreeze", 0, {{ PARSETYPE_S32, &edit_state.unfreeze }} },
    { 0, "freezeothers", 0, {{ PARSETYPE_S32, &edit_state.freezeothers }} },
    { 0, "invertSelection", 0, {{ PARSETYPE_S32, &edit_state.invertSelection}} },
    { 0, "search", 0, {{ PARSETYPE_S32, &edit_state.search}} },
    { 0, "lassoAdd", 0, {{ PARSETYPE_S32, &edit_state.lassoAdd}} },
    { 0, "lassoInvert", 0, {{ PARSETYPE_S32, &edit_state.lassoInvert}} },
    { 0, "lassoExclusive", 0, {{ PARSETYPE_S32, &edit_state.lassoExclusive}} },
    { 0, "ignoretray", 0, {{ PARSETYPE_S32, &edit_state.ignoretray }} },
    { 0, "objinfo", 0, {{ PARSETYPE_S32, &edit_state.objinfo }} },
    { 0, "hideCollisionVolumes", 0, {{ PARSETYPE_S32, &edit_state.hideCollisionVolumes }} },
    { 0, "findlibpiece", CMD_FINDLIBPIECE },

    { 0, "lodscale", 0, {{ PARSETYPE_S32, &edit_state.lodscale }} },
    { 0, "lodfar", 0, {{ PARSETYPE_S32, &edit_state.lodfar }} },
    { 0, "lodfarfade", 0, {{ PARSETYPE_S32, &edit_state.lodfarfade }} },
    { 0, "loddisable", 0, {{ PARSETYPE_S32, &edit_state.loddisable }} },
    { 0, "lodenable", 0, {{ PARSETYPE_S32, &edit_state.lodenable }} },
    { 0, "lodfadenode", 0, {{ PARSETYPE_S32, &edit_state.lodfadenode }} },
    { 0, "lodfadenode2", 0, {{ PARSETYPE_S32, &edit_state.lodfadenode2 }} },

    { 0, "beaconname", 0, {{ PARSETYPE_S32, &edit_state.beaconName}} },
    { 0, "beaconsize", 0, {{ PARSETYPE_S32, &edit_state.beaconSize, 2 }} },
    { 0, "showconnection", 0, {{ PARSETYPE_S32, &edit_state.showBeaconConnection }} },
    { 0, "beaconpathstart", 0, {{ PARSETYPE_S32, &edit_state.showBeaconPathStart }} },
    { 0, "beaconpathcontinue", 0, {{ PARSETYPE_S32, &edit_state.showBeaconPathContinue }} },
    { 0, "shownbeacon", 0, {{ PARSETYPE_S32, &edit_state.shownBeaconType }} },
    { 0, "beaconselect", 0, {{ PARSETYPE_S32, &edit_state.beaconSelect}} },
    { 0, "quickplacement", 0, {{ PARSETYPE_S32, &edit_state.quickPlacement }} },
    { 0, "quickrotate", 0, {{ PARSETYPE_S32, &edit_state.quickRotate }} },
    { 0, "quickorient", 0, {{ PARSETYPE_S32, &edit_state.quickOrient }} },
    { 0, "selecteditonly", 0, {{ PARSETYPE_S32, &edit_state.selectEditOnly }} },
    { 0, "openonselection", 0, {{ PARSETYPE_S32, &edit_state.openOnSelection }} },
    { 0, "togglequickplace", 0, {{ PARSETYPE_S32, &edit_state.toggleQuickPlacement }} },
    { 0, "quickplacementobject", 0, {{ CMDSTR(edit_state.quickPlacementObject) }} },
    { 0, "setquickobject", 0, {{ PARSETYPE_S32, &edit_state.setQuickPlacementObject }} },

    { 0, "tintcolor1", 0, {{ PARSETYPE_S32, &edit_state.tintcolor1 }} },
    { 0, "tintcolor2", 0, {{ PARSETYPE_S32, &edit_state.tintcolor2 }} },

    { 0, "replacetex1", 0, {{ PARSETYPE_S32, &edit_state.replacetex1 }} },
    { 0, "replacetex2", 0, {{ PARSETYPE_S32, &edit_state.replacetex2 }} },
    { 0, "removetex", 0, {{ PARSETYPE_S32, &edit_state.removetex }} },
    { 0, "textureSwap", 0, {{ PARSETYPE_S32, &edit_state.textureSwap }} },

    { 0, "addProp", 0, {{ PARSETYPE_S32, &edit_state.addProperty}} },
    { 0, "removeProp", 0, {{ PARSETYPE_S32, &edit_state.removeProperty}} },
    { 0, "showAsRadius", 0, {{ PARSETYPE_S32, &edit_state.showPropertyAsRadius}} },
    { 0, "showAsString", 0, {{ PARSETYPE_S32, &edit_state.showPropertyAsString}} },
    { 0, "editTexWord", CMD_EDITTEXWORD},

    { 0, "exitedit", CMD_EXITEDIT},
    { 0, "exiteditnomove", CMD_EXITEDITNOMOVE},

    { 0, "ungroupall",0, {{ PARSETYPE_S32, &edit_state.ungroupall}} },
    { 0, "groupall",0, {{ PARSETYPE_S32, &edit_state.groupall}} },

    { 0, "editmousescale", 0, {{ PARSETYPE_FLOAT, &edit_state.mouseScale }} },
    { 0, "useOldMenu", 0, {{ PARSETYPE_S32, &edit_state.toggleMenu }} },
    { 0, "hideMenu", 0, {{ PARSETYPE_S32, &edit_state.hideMenu }} },
    { 0, "profiler", 0, {{ PARSETYPE_S32, &edit_state.profiler }} },
    { 0, "cameraPlacement", 0, {{ PARSETYPE_S32, &edit_state.cameraPlacement }} },
    { 0, "liveTrayLink", 0, {{ PARSETYPE_S32, &edit_state.liveTrayLink }} },
    { 0, "showGroupBounds", 0, {{ PARSETYPE_S32, &edit_state.showBounds, 3 }} },

    { 0, "copyAutoLodParams", CMD_COPYLODPARAMS},

    { 0, "noFogClip", 0, {{ PARSETYPE_S32, &edit_state.nofogclip }} },
    { 0, "editNoColl", 0, {{ PARSETYPE_S32, &edit_state.nocoll }} },

    { 0, "makeDestructibleObject", CMD_MAKEDESTRUCTIBLEOBJECT },

    { 0, "nudgeLeft", 0, {{ PARSETYPE_S32, &edit_state.nudgeLeft }} },
    { 0, "nudgeRight", 0, {{ PARSETYPE_S32, &edit_state.nudgeRight }} },
    { 0, "nudgeUp", 0, {{ PARSETYPE_S32, &edit_state.nudgeUp }} },
    { 0, "nudgeDown", 0, {{ PARSETYPE_S32, &edit_state.nudgeDown }} },

    { 0, "loadRecent", CMD_LOADRECENT, {{ PARSETYPE_S32, &temp_int }} },

    { 0, "editModel", CMD_EDITMODEL, {{ 0 }} },
    { 0, "editTrick", CMD_EDITTRICK, {{ 0 }} },
    { 0, "editTexBind", CMD_EDITTEXBIND, {{ PARSETYPE_S32, &temp_int }} },
    { 0, "editMaterial", CMD_EDITMATERIAL, {{ PARSETYPE_S32, &temp_int }} },

    { 0, "editLODs", CMD_EDITLODS, {{ 0 }} },

    { 0, "editcampyr", CMD_EDITCAMPYR, { TEMP_VEC3 } },

    { 0 },
};

extern Cmd game_cmds[];

CmdList edit_cmdlist = 
{
    {{ edit_cmds },
    { control_cmds },
    { 0}}
};

int editGetCmdValue(char *str)
{
    Cmd            *cmd;
    CmdContext    output = {0};
    char        *s;

    output.access_level = cmdAccessLevel();

    cmd = cmdOldRead(&edit_cmdlist,str,&output);
    if (s=strrchr(output.msg,' '))
        return atoi(s);
    return 0;
}

int editCmdParse(char *str, int x, int y)
{
    Cmd            *cmd;
    CmdContext    output = {0};
    static int    save_plane = -1;

    output.found_cmd = false;
    output.access_level = cmdAccessLevel();
    cmd = cmdOldRead(&edit_cmdlist,str,&output);
    if (output.msg[0])
    {
        if (strncmp(output.msg,"Unknown",7)==0)
            return 0;//cmdGameParse(str);

        conPrintf("%s",output.msg);
        return 1;
    }

    if (!cmd)
        return 0;

    switch(cmd->num)
    {
        xcase CMD_GRIDSIZE:
            if (!edit_state.quickPlacement && edit_state.posrot)
            {
                if (++edit_state.rotsize > 8)
                    edit_state.rotsize = 0;
            }
            else
            {
                if (++edit_state.gridsize > 8)
                    edit_state.gridsize = 0;
            }

        xcase CMD_GRIDSHRINK:
            if (!edit_state.quickPlacement && edit_state.posrot)
            {
                if (--edit_state.rotsize < 0)
                    edit_state.rotsize = 8;
            }
            else
            {
                if (--edit_state.gridsize < 0)
                    edit_state.gridsize = 8;
            }

        xcase CMD_SINGLEAXIS:
            save_plane = edit_state.plane;
            edit_state.plane = edit_state.singleaxis - 1;
            edit_state.singleaxis = 1;

        xcase CMD_PLANE:
            if (save_plane >= 0)
            {
                edit_state.plane = save_plane;
                save_plane = -1;
            }
            edit_state.singleaxis = 0;

        xcase CMD_EXITEDIT:
            editSetMode(0, 1);

        xcase CMD_EXITEDITNOMOVE:
            editSetMode(0, 0);

        xcase CMD_EDITLIB:
            if (!edit_state.editorig)
                edit_state.editorig = 2;
            else
                edit_state.editorig = 0;

        xcase CMD_EDITTEXWORD:
            editTexWord();

        xcase CMD_COPYLODPARAMS:
        {
            char buffer[1024];
            editGetSelectedLodParams(buffer);
            winCopyToClipboard(buffer);
        }

        xcase CMD_LOADRECENT:
        {
            extern MRUList * recentMaps;
            if (recentMaps && temp_int < recentMaps->count)
            {
                char name[128];
                strcpy(name, recentMaps->values[temp_int]);
                editLoad(name, 0);
            }
        }

        xcase CMD_EDITMODEL:
            if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model)
            {
                static char buffer[1024];
                char *s;
                fileLocateWrite(sel_list[0].def_tracker->def->model->filename, buffer);
                strstriReplace(buffer, "/data/", "/src/");
                s = strrchr(buffer, '/');
                if (s)
                    *s = 0;
                ShellExecuteA(NULL, "explore", buffer, NULL, NULL, SW_SHOW);
            }

        xcase CMD_EDITTRICK:
            if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model)
            {
                TrickNode *trick = sel_list[0].def_tracker->def->model->trick;
                if (trick && trick->info)
                {
                    char filename[MAX_PATH];
                    fileLocateWrite(trick->info->file_name, filename);
                    if (filename[0])
                        editTrickFile(filename, trick->info->name, 0);
                }
            }

        xcase CMD_EDITTEXBIND:
            if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && temp_int < sel_list[0].def_tracker->def->model->tex_count)
            {
                TexBind *bind;
                int i, tri_idx = 0;
                for (i = 0; i < temp_int; i++)
                    tri_idx += sel_list[0].def_tracker->def->model->tex_idx[i].count;
                bind = getTexWithReplaces(sel_list[0].def_tracker, tri_idx);
                if (bind && bind->texopt && bind->texopt->file_name)
                {
                    char filename[MAX_PATH];
                    fileLocateWrite(bind->texopt->file_name, filename);
                    if (filename[0])
                        editTrickFile(filename, bind->texopt->name, 1);
                }
            }

        xcase CMD_EDITMATERIAL:
        {
            int idx = temp_int % 10, layer = temp_int / 10;
            if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->model && idx < sel_list[0].def_tracker->def->model->tex_count)
            {
                TexBind *bind;
                int i, tri_idx = 0;
                for (i = 0; i < idx; i++)
                    tri_idx += sel_list[0].def_tracker->def->model->tex_idx[i].count;
                bind = getTexWithReplaces(sel_list[0].def_tracker, tri_idx);
                if (bind->tex_layers[layer])
                {
                    char buffer[1024], buffer2[1024];
                    BasicTexture *tex = bind->tex_layers[layer]->actualTexture;
                    sprintf(buffer, "texture_library/%s", tex->dirname);
                    fileLocateWrite(buffer, buffer2);
                    strstriReplace(buffer2, "/data/", "/src/");
                    ShellExecuteA(NULL, "explore", buffer2, NULL, buffer2, SW_SHOW);
                }
            }
        }

        xcase CMD_MAKEDESTRUCTIBLEOBJECT:
            if( sel_count > 1 || sel_count < 1 )
            {
                Errorf( "Must only have exactly one library piece selected" );
            }
            else
            {
                GroupDef * def = sel_list[0].def_tracker->def;
                if( !groupInLib(def) )
                {
                    Errorf( "Must have library piece selected" );
                }
                else
                {
                    char buf[200];
                    const char * libraryPieceName = def->name;
                    if( WriteAnObject( libraryPieceName ) )
                    {
                        //TO DO MDO make sure it's reloaded
                        sprintf( buf, "spawnvillain %s", libraryPieceName );
                        cmdParse( buf );
                    }
                }
            }

        xcase CMD_EDITLODS:
            editLODsUI();

        xcase CMD_EDITCAMPYR:
            createMat3YPR(cam_info.mat, temp_vec3);
//            copyVec3(temp_vec3, control_state.pyr.cur);
//            control_state.pyr.cur[0] *= -1;
//            control_state.pyr.cur[1] = addAngle(control_state.pyr.cur[1], RAD(180));

        xcase CMD_FINDLIBPIECE:
            if(sel_count == 1)
                libOpenTo(sel_list[0].def_tracker);
    }
    return 1;
}

void editStateInit()
{
    cmdOldInit(edit_cmds);
    edit_state.isDirty=0;
    edit_state.snap3 = 0;
    edit_state.rotsize = 8;
    edit_state.polycount = 1;
    edit_state.colormemory = 1;
    edit_state.shownBeaconType = -1;
    edit_state.autosave=1;
    edit_state.autosavewait=10*60*CLK_TCK;
    edit_state.autosavetime=0;
    edit_state.lassoAdd=0;
    edit_state.lassoExclusive=0;
    edit_state.lassoInvert=0;
    edit_state.drawingLasso=0;
    edit_state.lassoX=-1;
    edit_state.lassoY=-1;
    edit_state.useObjectAxes=0;
    edit_state.objectAxesMovement=0;
    edit_state.objectAxesUnspecified=1;
    copyMat4(unitmat,edit_state.objectMatrix);
    copyMat4(unitmat,edit_state.objectMatrixInverse);
    strcpy(edit_state.quickPlacementObject, "NAV");
    copyMat3(unitmat, edit_state.quickPlacementMat3);
    edit_state.propertyClipboardMaxSize=256;    //this is decided in edit_cmd.h

    bindKeyProfile(&edit_binds_profile);
    memset(&edit_binds_profile, 0, sizeof(KeyBindProfile));
    edit_binds_profile.name = "Edit Commands";
    edit_binds_profile.parser = editCmdParse;
    edit_binds_profile.trickleKeys = 1;

    bindKey("a","addProp 1",0);
    bindKey("b","++beaconsize",0);
    bindKey("c","copy 1",0);
    bindKey("e","exitedit",0);
    bindKey("g","group 1",0);
    bindKey("h","hide 1",0);
    bindKey("i","selectall 1",0);
    bindKey("j","unhide 1",0);
    bindKey("l","beaconload",0);
    bindKey("m","gridsize",0);
    bindKey("n","gridshrink",0);
    bindKey("o","++objinfo",0);
    bindKey("p","paste 1",0);
    bindKey("q","++posrot",0);
    bindKey("r","exiteditnomove",0);
    bindKey("s","++snaptype",0);
    bindKey("u","ungroup 1",0);
    bindKey("v","paste 1",0);
    bindKey("w","hideMenu 1",0);
    bindKey("x","cut 1",0);
    bindKey("z","groundsnap 1",0);

    bindKey("f5","++nosnap",0);
    bindKey("f6","++snaptovert",0);
    bindKey("f7","save 1",0);
    bindKey("f8","setpivot 1",0);
    bindKey("f9","++oldvid",0);
    bindKey("f11","redo 1",0);
    bindKey("f12","undo 1",0);

    bindKey("1","singleaxis 1",0);
    bindKey("2","singleaxis 2",0);
    bindKey("3","singleaxis 3",0);
    bindKey("4","++snap3",0);
    bindKey("5","++camview",0);
    bindKey("6","setquickobject 1",0);
    bindKey("7","togglequickplace 1",0);
//    bindKey("8","+quickrotate",0);
    bindKey("[","open 1",0);
    bindKey("]","close 1",0);
    bindKey("rbutton","+look",0);
    bindKey("lbutton","sel 1",0);
    bindKey("mbutton","+pan",0);
    bindKey("delete","del 1",0);
    bindKey("esc","unsel 1",0);
    bindKey("tab","++plane",0);
    bindKey("lalt","setview 1",0);
    bindKey("space","+zoom",0);
    bindKey("lshift","+quickplacement",0);
    bindKey("capital","+openonselection",0);
    bindKey("mousewheel","mousewheelopenclose 1",0);
    bindKey("ctrl+lbutton","lassoExclusive 1",0);
    bindKey("left","nudgeLeft 1",0);
    bindKey("right","nudgeRight 1",0);
    bindKey("up","nudgeUp 1",0);
    bindKey("down","nudgeDown 1",0);
    //    bindKey("shift+lbutton","lassoAdd 1",0);        //functionality for these 
    //    bindKey("alt+lbutton","lassoInvert 1",0);        //two lassos is not complete

    //bindKey("f3","findNextProperty 2",0);
    //bindKey("f","findNextProperty 1",0);

    if(isDevelopmentMode() && fileExists("c:\\config_edit.txt"))
    {
        cmdAccessOverride(9);
        cmdParse("exec c:\\config_edit.txt");
        cmdAccessOverride(0);
    }

    unbindKeyProfile(&edit_binds_profile);
}

int editMode()
{
    return edit_mode;
}

void editSetMode(int on, int move)
{
    static int override = 0;

    on = on ? 1 : 0;

    if(on == edit_mode)
    {
        return;
    }

    if(!override && !edit_mode && comm_link.addr.sin_addr.S_un.S_addr != getHostLocalIp())
    {
        if(game_state.nodebug || !winMsgYesNo("You are not currently connected to a local mapserver. Are you REALLY sure you want to do this?"))
            return;
        override = 1;
    }
    edit_mode = on;
    if (on)
    {
        extern Vec3    view_pos;
        extern F32    view_dist;

        if(!edit_binds_profile.parser)
            editStateInit();

        copyVec3(cam_info.cammat[3],view_pos);
        getMat3YPR(cam_info.cammat, control_state.pyr.cur);
        control_state.pyr.cur[0] *= -1;
        control_state.pyr.cur[1] = addAngle(control_state.pyr.cur[1], RAD(180));
        view_dist = 0;

        bindKeyProfile(&edit_binds_profile);
        cmdParse("camview 1");
    }
    else
    {
        Entity* player = playerPtr();

        if(move)
        {
            editSendPlayerPos();
        }

        if(player)
        {
            Vec3 cam_pyr;

            if(move)
            {
                entUpdatePosInstantaneous(player, cam_info.cammat[3]);
                player->seq->moved_instantly = 1;
            }

            control_state.pyr.cur[1] = getVec3Yaw(ENTMAT(player)[2]);

            copyVec3(control_state.pyr.cur, cam_pyr);

            cam_pyr[0] *= -1;
            cam_pyr[1] = addAngle(cam_pyr[1], RAD(180));

            copyVec3(cam_pyr, cam_info.targetPYR);
            copyVec3(cam_pyr, cam_info.pyr);
        }

        unbindKeyProfile(&edit_binds_profile);
        game_state.edit = 0;
        control_state.ignore = 0;
    }
    MouseClearButtonState();
}

void editTexWord(void)
{
    GroupDef* def=NULL;
    PropertyEnt* prop;
    bool bFromDef=false;

    if(sel_count != 1)
        goto NotFromDef;

    // Property can only be added to a single item.
    if (!isNonLibSingleSelect(0,0))
        goto NotFromDef;

    // Make sure the properties table exists.
    def = sel_list[0].def_tracker->def;
    if(!def->properties)
        goto NotFromDef;

    stashFindPointer( def->properties, "TexWordLayout", &prop );
    if(!prop)
        goto NotFromDef;

    bFromDef = true;
    estrPrintEString(&game_state.texWordEdit, prop->value_str);
    estrConcatStaticCharArray(&game_state.texWordEdit, ".texword");
NotFromDef:
    if (!bFromDef) {
        estrPrintCharString(&game_state.texWordEdit, "LAST");
    }
    texWordsEditor(1);

    // Reset texbinds
    if (bFromDef)
        setClientFlags(def, def->file->fullname);
}

char ** textureSwapNames;
int        textureSwapNameIndex;

static FileScanAction randomTextureCallback(char * dir,struct _finddata32_t * fileData)
{
    if (strchr(fileData->name,'.')!=NULL)
        strcpy(textureSwapNames[textureSwapNameIndex++],fileData->name);
    return FSA_NO_EXPLORE_DIRECTORY;
}

void editTextureMenuCallback(MenuEntry *me, ClickInfo *ci)
{
    DefTexSwap *** def_tex_swaps;
    DefTexSwap * dts;
    DefTexSwap * replacedTexture=NULL;
    int replacedIndex=-1;
    char fileName[1024];
    char * s;
    int i;
    int size;
    int replaceFlag;
    char originalTexture[TEXT_DIALOG_MAX_STRLEN];
    int composite;
    if (strcmp(me->parent->name,"Materials")==0)    //determine what kind of swap it is
        composite=1;
    else
        composite=0;
    edit_state.textureSwapTracker=sel_list[0].def_tracker;
    if (edit_state.textureSwapTracker==NULL || edit_state.textureSwapTracker->def==NULL) {
        edit_state.textureSwap=0;
        return;
    }
    me->info=&edit_state.textureSwapTracker->def->def_tex_swaps;
    def_tex_swaps=(DefTexSwap ***)me->info;
    strncpy(originalTexture, me->name, TEXT_DIALOG_MAX_STRLEN - 1);
    originalTexture[TEXT_DIALOG_MAX_STRLEN - 1] = '\0';
    s=strstr(originalTexture," ^3");
    if (s!=NULL)
        *s='\0';
    //if we are removing textures, take out the old texture, shrink down the array in an intelligent way
    if(edit_state.textureSwapRemove)
    {
        for(i = eaSize(def_tex_swaps)-1; i >= 0; --i)
        {
            if(strcmp((*def_tex_swaps)[i]->tex_name, originalTexture)==0)
            {
                deleteDefTexSwap(eaRemove(def_tex_swaps, i));
                if((s = strstr(me->name," ^3")) != NULL)
                    *s='\0';
            }
        }
        editUpdateTracker(edit_state.textureSwapTracker);
        edit_state.textureSwap=0;
        edit_state.textureSwapReopen=1;
        return;
    }
    strcpy(fileName, "c:/game/src/texture_library/");
    if (composite)
        strcat(fileName, texFindComposite(originalTexture)?texFindComposite(originalTexture)->dirname:"");
    else
        strcat(fileName, texFind(originalTexture)?texFind(originalTexture)->dirname:"");
    backSlashes(fileName);

    if (edit_state.textureSwapRandom) {
        int pick;
        textureSwapNames=(char **)_alloca(sizeof(char *)*500);
        textureSwapNameIndex=0;
        for (i=0;i<500;i++)
            textureSwapNames[i]=(char *)_alloca(sizeof(char)*256);
        fileScanAllDataDirs(fileName+12,randomTextureCallback);
        if (textureSwapNameIndex>0) {
            pick=rand()%textureSwapNameIndex;
            sprintf(fileName,textureSwapNames[pick]);
            if (strrchr(fileName,'.')!=NULL)
                *strrchr(fileName,'.')='\0';
            s=fileName;
        } else {
            s=NULL;
        }
    } else {
        if (strEndsWith(fileName, "\\"))
            strcat(fileName, "*.tga");
        else
            strcat(fileName, "\\*.tga");
        s = winGetFileName("Targa files (.tga)",fileName,0);
    }
    if (s==NULL) return;
    if (fileName[0]=='\0') return;

    {
        char tex[MAX_PATH];

        strncpy(tex, me->name, MAX_PATH - 1);
        tex[MAX_PATH - 1] = '\0';
        if(s = strchr(tex, ' ')) // assignment
            *s = '\0';

        // replace with 'filename', with the extension and path trimmed
        if(s = strrchr(fileName, '.')) // assignment
            *s = '\0';
        s = strrchr(fileName, '\\');
        if (!s++)
            s = fileName;

        dts = createDefTexSwap(tex, s, composite);
        if (!dts)
        {
            return;
        }
        if(dts->composite ? !dts->comp_replace_bind : !dts->replace_bind) // since this is a union now, a single check would suffice :P... but I'll leave that as an exercise for the compiler
        {
            winMsgAlert("No texture by that name exists.");
            deleteDefTexSwap(dts);
            return;
        }
    }

    s=strstr(me->name," ^3");
    if (s!=NULL)
    {
        int index = s - me->name;
        estrRemove(&me->name, index, estrLength(&me->name) - index);
    }
    size=eaSize(def_tex_swaps);
    replaceFlag=false;
    for (i=0;i<size;i++)
        if (strcmp((*def_tex_swaps)[i]->tex_name,dts->tex_name)==0) {
            replacedTexture=(*def_tex_swaps)[i];
            (*def_tex_swaps)[i]=dts;
            replaceFlag=true;
            replacedIndex=i;
            break;
        }
    if (!replaceFlag)
        eaPush(def_tex_swaps,dts);
    estrConcatStaticCharArray(&me->name, " ^3");
    estrConcatCharString(&me->name, dts->replace_name);

    editUpdateTracker(edit_state.textureSwapTracker);
    if (!replaceFlag)
        eaSetSize(def_tex_swaps,eaSize(def_tex_swaps)-1);
    else
        (*def_tex_swaps)[replacedIndex]=replacedTexture;
    deleteDefTexSwap(dts);
    edit_state.textureSwap=0;
    edit_state.textureSwapReopen=1;
}

void editCmdAddBasicTexturesToMenu(Menu * textureMenu,DefTracker * tracker) {
    int i;
    if (tracker==NULL || tracker->def==NULL) return;
    if (tracker->def && tracker->def->model && tracker->def->model->gld) {
        Model * model=tracker->def->model;
        for (i=0;i<model->tex_count;i++) {
            char texName[256];
            char path[512];
            TexBind * texBind;
            int j;
            strcpy(texName,model->gld->texnames.strings[model->tex_idx[i].id]);
            texBind=texFindComposite(texName);
            if (texBind) {
                for (j=0;j<ARRAY_SIZE(texBind->tex_layers);j++) {
                    if (texBind->tex_layers[j]==NULL) continue;
                    strcpy(texName,texBind->tex_layers[j]->name);
                    if (strcmp(texName,"white")!=0 && strcmp(texName,"grey")!=0 && strcmp(texName,"invisible")!=0 && strcmp(texName,"dummy_bump")!=0) {
                        int k;
                        for (k=0;k<eaSize(&edit_state.textureSwapTracker->def->def_tex_swaps);k++) {
                            DefTexSwap * dts=edit_state.textureSwapTracker->def->def_tex_swaps[k];
                            if (dts->composite)
                                continue;
                            if (strcmp(dts->tex_name,texName)==0) {
                                strcat(texName," ^3");
                                strcat(texName,edit_state.textureSwapTracker->def->def_tex_swaps[k]->replace_name);
                                break;
                            }
                        }
                        strcpy(path,"Textures/");
                        strcat(path,texName);
                        addEntryToMenu(textureMenu,path,editTextureMenuCallback,(void *)(&edit_state.textureSwapTracker->def->def_tex_swaps));
                    }
                }
            }
        }
    }
    trackerOpenEdit(tracker);
    for (i=0;i<tracker->count;i++)
        editCmdAddBasicTexturesToMenu(textureMenu,&tracker->entries[i]);
    trackerCloseEdit(tracker);
    return;
}

void editCmdAddCompositeTexturesToMenu(Menu * textureMenu,DefTracker * tracker) {
    int i;
    int error=0;
    if (tracker==NULL || tracker->def==NULL) return;
    if (tracker->def && tracker->def->model && tracker->def->model->gld) {
        Model * model=tracker->def->model;
        for (i=0;i<model->tex_count;i++) {
            char texName[256];
            char path[512];
            TexBind * texBind;
            strcpy(texName,model->gld->texnames.strings[model->tex_idx[i].id]);
            texBind=texFindComposite(texName);
            if (strcmp(texName,"white")!=0 && strcmp(texName,"grey")!=0 && strcmp(texName,"invisible")!=0 && strcmp(texName,"dummy_bump")!=0) {
                int k;
                MenuEntry * me;
                for (k=0;k<eaSize(&edit_state.textureSwapTracker->def->def_tex_swaps);k++) {
                    DefTexSwap * dts=edit_state.textureSwapTracker->def->def_tex_swaps[k];
                    if (dts->composite && strcmp(dts->tex_name,texName)==0) {
                        if (dts->comp_tex_bind && dts->comp_replace_bind)
                            error=(dts->comp_replace_bind->bind_blend_mode.intval!=dts->comp_tex_bind->bind_blend_mode.intval);
                        strcat(texName," ^3");
                        strcat(texName,dts->replace_name);
                        if (error)
                            strcat(texName," ^2MISMATCH!");

                        break;
                    }
                }
                strcpy(path,"Materials/");
                strcat(path,texName);
                me=addEntryToMenu(textureMenu,path,editTextureMenuCallback,(void *)(&edit_state.textureSwapTracker->def->def_tex_swaps));
                if (error)
                    me->color=0xFF0000FF;
                error=0;
            }
        }
    }
    trackerOpenEdit(tracker);
    for (i=0;i<tracker->count;i++)
        editCmdAddCompositeTexturesToMenu(textureMenu,&tracker->entries[i]);
    trackerCloseEdit(tracker);
    return;
}

void editTrickFile(const char *filename, const char *trickname, int is_texture)
{
    const char *ultraedit_path = "C:\\Program Files\\IDM Computer Solutions\\UltraEdit-32\\uedit32.exe";
    const char *editplus_path = "C:\\Program Files\\EditPlus 2\\editplus.exe";

    if(fileExists(ultraedit_path) || fileExists(editplus_path))
    {
        char *sys_call = estrTemp();
        int flen, trickline = 1;
        char *trickfile = fileAlloc(filename, &flen);
        char fixedpath[MAX_PATH];
        if (trickfile)
        {
            char trickstr[1024];
            char *s;
            if (is_texture)
                sprintf(trickstr, "texture %s%s", trickname, (trickname[0]=='x'||trickname[0]=='X')?"":".");
            else
                sprintf(trickstr, "trick %s", trickname);
            s = strstri(trickfile, trickstr);
            if (s)
            {
                for (; s >= trickfile && *s; s--)
                {
                    if (*s == '\n')
                        trickline++;
                }
            }
            free(trickfile);
        }
        strcpy(fixedpath, filename);
        backSlashes(fixedpath);
        if(fileExists(ultraedit_path))
            estrPrintf(&sys_call, "\"%s\" \"%s/%d\"", ultraedit_path, fixedpath, trickline);
        else
            estrPrintf(&sys_call, "\"%s\" \"%s\" -cursor %d:1", editplus_path, fixedpath, trickline);
        system_detach(sys_call, 0);
        estrDestroy(&sys_call);
    }
    else
    {
        ShellExecuteA(NULL, "open", filename, NULL, NULL, SW_SHOW);
    }
}
