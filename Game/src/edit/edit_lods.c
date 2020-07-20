#include <utilitieslib/stdtypes.h>
#include "assert.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/utils/FolderCache.h>
#include "editorUI.h"
#include "edit/edit_select.h"
#include "seq/anim.h"
#include "edit/edit_cmd_select.h"
#include "group/groupfileload.h"
#include "seq/modelReload.h"
#include <utilitieslib/version/AppRegCache.h>
#include "group/groupfilelib.h"
#include "seq/AutoLOD.h"
#include "cmdparse/cmdgame.h"

//////////////////////////////////////////////////////////////////////////
// LOD Editor Window

#define MAX_EDIT_AUTOLODS 4

static void editLODsSetLODValues(Model *model, GroupDef *def);

enum
{
    LOD_MODE_SPECIFY=0,
    LOD_MODE_TRICOUNT=1,
};

typedef struct EditAutoLOD
{
    float distance;
    float fade_in;
    float fade_out;
    int mode;
    int use_fallback_material;
    float tri_percent;
    char modelname[128];
} EditAutoLOD;

typedef struct EditLODs
{
    int ui_id;

    DefTracker *tracker;
    Model *model;
    ModelLODInfo *new_lod_info;

    int automatic;
    int show_current;
    int current_lod_display;

    float num_lods;
    EditAutoLOD lods[MAX_EDIT_AUTOLODS];

} EditLODs;

static EditLODs edit_lods = {0};

enum 
{
    APPLY_STATE_NONE=0,
    APPLY_STATE_START,
    APPLY_STATE_WAIT1,
    APPLY_STATE_CHECKOUT,
    APPLY_STATE_WRITELOD,
    APPLY_STATE_WAIT2,
    APPLY_STATE_CANCEL,
    APPLY_STATE_DONE,
};


static void updateLODInfo(int reset_tricks)
{
    float near_dist = 0;
    int i;

    if (edit_lods.new_lod_info)
        freeModelLODInfoData(edit_lods.new_lod_info);
    else
        edit_lods.new_lod_info = ParserAllocStruct(sizeof(ModelLODInfo));

    edit_lods.new_lod_info->modelname = ParserAllocString(edit_lods.model->name);
    edit_lods.new_lod_info->force_auto = !!edit_lods.automatic;

    if (!edit_lods.automatic)
    {
        for (i = 0; i < edit_lods.num_lods; i++)
        {
            AutoLOD *lod = allocAutoLOD();
            lod->lod_nearfade = edit_lods.lods[i].fade_in;
            lod->lod_farfade = edit_lods.lods[i].fade_out;
            lod->lod_near = near_dist;
            near_dist += edit_lods.lods[i].distance;
            lod->lod_far = near_dist;
            if (edit_lods.lods[i].mode == LOD_MODE_TRICOUNT)
            {
                lod->flags = LOD_ERROR_TRICOUNT;
                lod->max_error = 100.f - edit_lods.lods[i].tri_percent;
            }
            else if (edit_lods.lods[i].modelname[0])
            {
                lod->modelname_specified = 1;
                lod->lod_modelname = ParserAllocString(edit_lods.lods[i].modelname);
                lod->lod_filename = objectLibraryPathFromObj(lod->lod_modelname);
                lod->lod_filename = lod->lod_filename?ParserAllocString(lod->lod_filename):0;
            }
            if (edit_lods.lods[i].use_fallback_material)
                lod->flags |= LOD_USEFALLBACKMATERIAL;
            eaPush(&edit_lods.new_lod_info->lods, lod);
        }
    }
    else
    {
        lodinfoFillInAutoData(edit_lods.new_lod_info, edit_lods.model->name, edit_lods.model->filename, edit_lods.model->radius, edit_lods.model->tri_count, edit_lods.model->autolod_dists);
    }

    if (!edit_lods.model->gld->lod_infos)
        edit_lods.model->lod_info = edit_lods.new_lod_info;

    // reset trick flags
    if (reset_tricks)
        groupResetOne(edit_lods.tracker, RESET_TRICKS|RESET_BOUNDS);
    else
        groupResetOne(edit_lods.tracker, RESET_BOUNDS);
}

static int apply_state=APPLY_STATE_NONE;
static void (*apply_done_callback)(void) = NULL;
static void (*apply_canceled_callback)(void) = NULL;

static void editLODsApply(void (*done_callback)(void), void (*canceled_callback)(void))
{
    apply_state = APPLY_STATE_START;
    apply_done_callback = done_callback;
    apply_canceled_callback = canceled_callback;
}

static void editLODsApplyStateMachine(void)
{
    static char outputname[MAX_PATH];

    switch (apply_state)
    {
        xcase APPLY_STATE_START:
        {
            if (edit_lods.model->gld->file_format_version < 7)
            {
                // make sure getvrml will reprocess the file
                if (!isGetVrmlRunning())
                {
                    Errorf("GetVrml is not running, unable to make changes!");
                    apply_state = APPLY_STATE_CANCEL;
                }
                else
                {
                    char vrmlname[MAX_PATH];
                    fileLocateWrite(edit_lods.model->filename, vrmlname);
                    strstriReplace(vrmlname, "/data/", "/src/");
                    changeFileExt(vrmlname,".wrl",vrmlname);
                    if (!fileExists(vrmlname))
                    {
                        Errorf("You do not have %s checked out, GetVrml will not reprocess file!", vrmlname);
                        apply_state = APPLY_STATE_CANCEL;
                    }
                    else
                    {
                        // pop up status window
                        showEditStatusWindow("Please Wait", 1);
                        apply_state = APPLY_STATE_WAIT1;
                        setEditStatusWindowText("Waiting for GetVrml...");
                    }
                }
            }
            else
            {
                // pop up status window
                showEditStatusWindow("Please Wait", 1);
                apply_state = APPLY_STATE_WAIT1;
                setEditStatusWindowText("Waiting for GetVrml...");
            }
        }

        xcase APPLY_STATE_WAIT1:
        {
            // wait for getvrml
            if (tryToLockGetVrml())
            {
                apply_state = APPLY_STATE_CHECKOUT;
                setEditStatusWindowText("Checking out LOD file...");
            }
        }

        xcase APPLY_STATE_CHECKOUT:
        {
            fileLocateWrite(getLODFileName(edit_lods.model->filename), outputname);
            apply_state = APPLY_STATE_WRITELOD;
            setEditStatusWindowText("Saving LOD file...");
        }

        xcase APPLY_STATE_WRITELOD:
        {
            writeLODInfo(edit_lods.new_lod_info, edit_lods.model->filename);

            apply_state = APPLY_STATE_WAIT2;
            setEditStatusWindowText("Waiting for GetVrml...");

            // let getvrml do its thing
            releaseGetVrmlLock();
            Sleep(500);
        }

        xcase APPLY_STATE_WAIT2:
        {
            // wait for getvrml to finish
            if (tryToLockGetVrml())
            {
                // release getvrml lock
                releaseGetVrmlLock();
                apply_state = APPLY_STATE_DONE;
            }
        }

        xcase APPLY_STATE_CANCEL:
        {
            // close status window
            closeEditStatusWindow();
            apply_state = APPLY_STATE_NONE;

            if (apply_canceled_callback)
                apply_canceled_callback();
        }

        xcase APPLY_STATE_DONE:
        {
            // close status window
            closeEditStatusWindow();
            apply_state = APPLY_STATE_NONE;

            if (apply_done_callback)
                apply_done_callback();
        }
    }
}

static int browse_ui_id=-1;
static void browseDestroyWindow(void)
{
    editorUIDestroyWindow(browse_ui_id);
}

static int isOkNode(FolderNode *node)
{
    if (strEndsWith(node->name, ".geo"))
        return 1;

    if (!node->is_dir)
        return 0;

    node = node->contents;
    while (node)
    {
        if (isOkNode(node))
            return 1;
        node = node->next;
    }

    return 0;
}

static int findPath(const char *path, TreeElementAddress *elem)
{
    int i, firsttime = 1;
    char pathstr[MAX_PATH], *str=pathstr, *s;
    FolderNode *node = FolderNodeFind(folder_cache->root, "object_library");

    strcpy(pathstr, path);
    forwardSlashes(pathstr);
    fixDoubleSlashes(pathstr);

    freeTreeAddressData(elem);

    while (str)
    {
        s = strchr(str, '/');
        if (s)
            *s = 0;

        if (node)
        {
            // find first ok node
            while (node && !isOkNode(node))
                node = node->next;

            // search for address of the desired path
            i = 0;
            while (node && stricmp(node->name, str))
            {
                i++;

                // find next ok node
                node = node->next;
                while (node && !isOkNode(node))
                    node = node->next;
            }

            if (!node)
                i = -1;
        }
        else
        {
            // inside geo file
            GeoLoadData *gld;
            *(str-1) = 0;
            gld = geoLoad(pathstr, LOAD_HEADER, GEO_INIT_FOR_DRAWING|GEO_USED_BY_WORLD);
            if (gld)
            {
                for (i = 0; i < gld->modelheader.model_count; i++)
                {
                    if (stricmp(gld->modelheader.models[i]->name, str)==0)
                        break;
                }

                if (i == gld->modelheader.model_count)
                    i = -1;
            }
            else
            {
                i = -1;
            }
        }

        if (i < 0)
        {
            freeTreeAddressData(elem);
            return 0;
        }

        if (!firsttime)
            treeAddressPush(elem, i);
        firsttime = 0;

        if (node)
            node = node->contents;

        if (s)
        {
            *s = '/';
            str = s+1;
        }
        else
        {
            str = 0;
        }
    }
    
    return 1;
}

static int findObject(char *namebuf, char *path, const TreeElementAddress *elem)
{
    int i, j, child_count=0;
    FolderNode *node = FolderNodeFind(folder_cache->root, "object_library");
    
    path[0]=0;

    if (!node || !isOkNode(node))
        return 0;

    for (i = 0; i < eaiSize(&elem->address); i++)
    {
        assert(node);

        strcat(path, node->name);
        if (node->is_dir)
        {
            strcat(path, "/");

            node = node->contents;

            while (node && !isOkNode(node))
                node = node->next;
            for (j = 0; j < elem->address[i]; j++)
            {
                do {
                    node = node->next;
                } while (node && !isOkNode(node));
            }
        }
        else
        {
            // geo file
            GeoLoadData *gld = geoLoad(path, LOAD_HEADER, GEO_INIT_FOR_DRAWING|GEO_USED_BY_WORLD);
            assert(elem->address[i] < gld->modelheader.model_count);
            strcpy(namebuf, gld->modelheader.models[elem->address[i]]->name);
            node = 0;
        }
    }

    if (node)
        strcpy(namebuf, node->name);

    if (node && node->is_dir)
    {
        node = node->contents;
        while (node)
        {
            if (isOkNode(node))
                child_count++;
            node = node->next;
        }
    }
    else if (node)
    {
        // geo file
        GeoLoadData *gld;
        strcat(path, node->name);
        gld = geoLoad(path, LOAD_HEADER, GEO_INIT_FOR_DRAWING|GEO_USED_BY_WORLD);
        assertmsgf(gld, "Invalid lod name \"%s\"", path);
        if(gld) child_count = gld->modelheader.model_count;
    }

    return child_count;
}

static int browseElementCallback(char *namebuf, const TreeElementAddress *elem)
{
    char path[MAX_PATH];
    return findObject(namebuf, path, elem);
}

static int browse_tree_id = -1;
static TreeElementAddress browse_tree_address={0};
static char browse_tree_selected[MAX_PATH]="", browse_tree_selected_path[MAX_PATH]="";
static int change_model_button[MAX_EDIT_AUTOLODS];

static void browseGetSelected(char *namebuf)
{
    if (editorUITreeControlGetSelected(browse_tree_id, &browse_tree_address, 1))
        findObject(browse_tree_selected, browse_tree_selected_path, &browse_tree_address);
    else
        strcpy(browse_tree_selected, "");

    sprintf(namebuf, "Selected: %s", browse_tree_selected);
}

static void copyLODsOkButton(char *unused)
{
    if (browse_tree_selected[0])
    {
        Model *model = modelFind(browse_tree_selected, browse_tree_selected_path, LOAD_NOW, GEO_INIT_FOR_DRAWING|GEO_USED_BY_WORLD);
        if (model && model->lod_info && eaSize(&model->lod_info->lods))
        {
            GroupDef *def;
            char *s, name[1024];
            strcpy(name, model->name);
            s = strstri(name, "__");
            if (s)
                *s = 0;
            def = groupDefFindWithLoad(name);
            editLODsSetLODValues(model, def);
            edit_lods.automatic = 0;
            updateLODInfo(1);
        }
        else
        {
            Errorf("No model selected or selected model does not have LOD information.");
        }
    }

    browseDestroyWindow();
}

static void setModelNameButton(char *unused)
{
    if (browse_tree_selected[0])
    {
        char *s;
        strcpy(edit_lods.lods[edit_lods.current_lod_display].modelname, browse_tree_selected);
        s = strstri(edit_lods.lods[edit_lods.current_lod_display].modelname, "__");
        if (s)
            *s = 0;
        editorUIButtonChangeText(change_model_button[edit_lods.current_lod_display], edit_lods.lods[edit_lods.current_lod_display].modelname);
        updateLODInfo(1);
    }

    browseDestroyWindow();
}

static void editLODsChangeModel(char *unused)
{
    char path[MAX_PATH];

    if (!edit_lods.tracker)
        return;

    // open browse window
    browse_ui_id = editorUICreateWindow("Browse For Model");
    if (browse_ui_id < 0)
        return;

    editorUISetModal(browse_ui_id, 1);
    editorUISetCloseCallback(edit_lods.ui_id, browseDestroyWindow);

    browse_tree_selected[0] = 0;
    browse_tree_selected_path[0] = 0;

    freeTreeAddressData(&browse_tree_address);
    if (edit_lods.lods[edit_lods.current_lod_display].modelname[0])
    {
        char *filename = objectLibraryPathFromObj(edit_lods.lods[edit_lods.current_lod_display].modelname);
        strcpy(path, filename);
        strcat(path, "/");
        strcat(path, edit_lods.lods[edit_lods.current_lod_display].modelname);
        findPath(path, &browse_tree_address);
    }
    if (!eaiSize(&browse_tree_address.address))
    {
        strcpy(path, edit_lods.model->filename);
        strcat(path, "/");
        strcat(path, edit_lods.model->name);
        findPath(path, &browse_tree_address);
    }

    browse_tree_id = editorUIAddTreeControl(browse_ui_id, NULL, browseElementCallback, 0, 0, eaiSize(&browse_tree_address.address)?&browse_tree_address:NULL);
    editorUIAddDynamicText(browse_ui_id, browseGetSelected);
    editorUIAddButtonRow(browse_ui_id, NULL, "Ok", setModelNameButton, "Cancel", browseDestroyWindow, NULL);

    editorUICenterWindow(browse_ui_id);
}

static void editLODsCopyParams(char *unused)
{
    if (!edit_lods.tracker)
        return;

    // open browse window
    browse_ui_id = editorUICreateWindow("Browse For Model");
    if (browse_ui_id < 0)
        return;

    editorUISetModal(browse_ui_id, 1);
    editorUISetCloseCallback(edit_lods.ui_id, browseDestroyWindow);

    browse_tree_selected[0] = 0;
    browse_tree_selected_path[0] = 0;

    if (!eaiSize(&browse_tree_address.address))
    {
        char path[MAX_PATH];
        strcpy(path, edit_lods.model->filename);
        strcat(path, "/");
        strcat(path, edit_lods.model->name);
        findPath(path, &browse_tree_address);
    }

    browse_tree_id = editorUIAddTreeControl(browse_ui_id, NULL, browseElementCallback, 0, 0, eaiSize(&browse_tree_address.address)?&browse_tree_address:NULL);
    editorUIAddDynamicText(browse_ui_id, browseGetSelected);
    editorUIAddButtonRow(browse_ui_id, NULL, "Ok", copyLODsOkButton, "Cancel", browseDestroyWindow, NULL);

    editorUICenterWindow(browse_ui_id);
}

static void editLODsDestroyWindow(void)
{
    if (edit_lods.tracker)
        edit_lods.tracker->lod_override = 0;
    if (edit_lods.ui_id)
    {
        float x, y;
        RegReader rr = createRegReader();
        initRegReader(rr, regGetAppKey());
        editorUIGetPosition(edit_lods.ui_id, &x, &y);
        rrWriteInt(rr, "LODEditorX", x);
        rrWriteInt(rr, "LODEditorY", y);
        destroyRegReader(rr);

        editorUIDestroyWindow(edit_lods.ui_id);
    }
    if (edit_lods.new_lod_info)
    {
        freeModelLODInfoData(edit_lods.new_lod_info);
        ParserFreeStruct(edit_lods.new_lod_info);
    }
    ZeroStruct(&edit_lods);
    groupResetAll(RESET_TRICKS|RESET_BOUNDS|RESET_LODINFOS);
    editWaitingForServer(0, 0);
}

static void editLODsOkButton(char *unused)
{
    editLODsApply(editLODsDestroyWindow, 0);
}

static int editLODsGetNumLODs(void)
{
    return edit_lods.num_lods;
}

static void editLODsGetDistStatusText(char *text)
{
    float near_dist = 0, far_dist = 0;
    int i;
    
    if (!edit_lods.tracker)
        return;

    if (edit_lods.current_lod_display < 0)
    {
        text[0] = 0;
        return;
    }

    for (i = 0; i < edit_lods.current_lod_display; i++)
        near_dist += edit_lods.lods[i].distance;
    far_dist = near_dist + edit_lods.lods[edit_lods.current_lod_display].distance;

    sprintf(text, "LOD Near: %4d    LOD Far: %4d", (int)near_dist, (int)far_dist);
}

static void editLODsGetTriStatusText(char *text)
{
    int num_tris = 0;

    if (!edit_lods.tracker)
        return;

    if (edit_lods.current_lod_display < 0)
    {
        text[0] = 0;
        return;
    }

    if (edit_lods.tracker->def->auto_lod_models && edit_lods.current_lod_display < eaSize(&edit_lods.tracker->def->auto_lod_models) && edit_lods.tracker->def->auto_lod_models[edit_lods.current_lod_display])
        num_tris = edit_lods.tracker->def->auto_lod_models[edit_lods.current_lod_display]->tri_count;

    sprintf(text, "Triangles: %4d", num_tris);
}

static int editLODsShowWarning1(int unused)
{
    if (!edit_lods.model)
        return 0;

    if (edit_lods.model->gld->file_format_version < 7)
        return 1;

    return 0;
}

static void editLODsGetWarning1(char *text)
{
    strcpy(text, "WARNING: geo is old, changes will not be visible");
}

static int editLODsShowWarning2(int unused)
{
    return game_state.vis_scale != 1;
}

static void editLODsGetWarning2(char *text)
{
    strcpy(text, "WARNING: vis scale not set to 1");
}

static void editLODsDistanceChanged(void *unused)
{
    if (!edit_lods.tracker)
        return;

    updateLODInfo(0);
}

static void editLODsLODsChanged(void *unused)
{
    if (!edit_lods.tracker)
        return;

    updateLODInfo(1);
}

static void editLODsFallbackChanged(void *unused)
{
    if (!edit_lods.tracker)
        return;

    updateLODInfo(0);
}

static float editLODsGetBreakdown(int lod_num, int *selected)
{
    float dist = 0;
    int i;

    if (!edit_lods.tracker)
        return 0;

    if (lod_num >= edit_lods.num_lods)
    {
        *selected = 0;
        return 0;
    }

    for (i = 0; i < edit_lods.num_lods; i++)
        dist += edit_lods.lods[i].distance;

    *selected = lod_num == edit_lods.current_lod_display;

    return edit_lods.lods[lod_num].distance / dist;
}

static void editLODsShowCurrentChanged(void *widget)
{
    if (!edit_lods.tracker)
        return;

    if (edit_lods.show_current)
        edit_lods.tracker->lod_override = edit_lods.current_lod_display + 1;
    else
        edit_lods.tracker->lod_override = 0;
}

static float new_num_lods = 0;
static void editLODsNumLODsChanged(void *widget)
{
    int i;
    float last_percent = 100;

    if (!edit_lods.tracker)
        return;

    for (i = 0; i < edit_lods.num_lods; i++)
    {
        if (edit_lods.lods[i].mode == LOD_MODE_TRICOUNT)
            last_percent = edit_lods.lods[i].tri_percent;
    }

    for (i = edit_lods.num_lods; i < new_num_lods; i++)
    {
        edit_lods.lods[i].distance = edit_lods.lods[i-1].distance;
        edit_lods.lods[i].fade_in = edit_lods.lods[i-1].fade_in;
        edit_lods.lods[i].fade_out = edit_lods.lods[i-1].fade_out;
        edit_lods.lods[i].mode = LOD_MODE_TRICOUNT;
        edit_lods.lods[i].modelname[0] = 0;
        edit_lods.lods[i].use_fallback_material = 1;
        if (last_percent <= 25)
            edit_lods.lods[i].tri_percent = last_percent;
        else
            last_percent = edit_lods.lods[i].tri_percent = last_percent - 25;
    }

    edit_lods.num_lods = new_num_lods;

    updateLODInfo(1);
}

static void editLODsSetLODValues(Model *model, GroupDef *def)
{
    int i, firstLodIsSrc = 0;

    edit_lods.num_lods = eaSize(&model->lod_info->lods);
    MIN1(edit_lods.num_lods, MAX_EDIT_AUTOLODS);
    new_num_lods = edit_lods.num_lods;

    edit_lods.automatic = model->lod_info->bits.is_automatic || model->lod_info->force_auto;

    for (i = 0; i < edit_lods.num_lods; i++)
    {
        AutoLOD *lod = model->lod_info->lods[i];
        edit_lods.lods[i].distance = lod->lod_far - lod->lod_near;
        edit_lods.lods[i].fade_in = (i==0)?0:lod->lod_nearfade;
        edit_lods.lods[i].fade_out = lod->lod_farfade;
        edit_lods.lods[i].use_fallback_material = !!(lod->flags & LOD_USEFALLBACKMATERIAL);
        if (lod->modelname_specified)
        {
            edit_lods.lods[i].mode = LOD_MODE_SPECIFY;
            strcpy(edit_lods.lods[i].modelname, lod->lod_modelname);
            edit_lods.lods[i].tri_percent = 100;
        }
        else if (lod->flags & LOD_ERROR_TRICOUNT || lod->max_error == 0)
        {
            edit_lods.lods[i].mode = LOD_MODE_TRICOUNT;
            edit_lods.lods[i].tri_percent = 100 - lod->max_error;
            edit_lods.lods[i].modelname[0] = 0;
            if (i == 0 && lod->max_error == 0)
                firstLodIsSrc = 1;
        }
        else if (def && firstLodIsSrc && eaSize(&def->auto_lod_models) > 0 && def->auto_lod_models[0] && i < eaSize(&def->auto_lod_models) && def->auto_lod_models[i])
        {
            edit_lods.lods[i].mode = LOD_MODE_TRICOUNT;
            edit_lods.lods[i].tri_percent = 100.f * def->auto_lod_models[i]->tri_count / def->auto_lod_models[0]->tri_count;
            edit_lods.lods[i].modelname[0] = 0;
        }
        else
        {
            if (i == 0)
                firstLodIsSrc = 1; // not entirely accurate, but better than setting all LODs to 100
            edit_lods.lods[i].mode = LOD_MODE_TRICOUNT;
            edit_lods.lods[i].tri_percent = 100;
            edit_lods.lods[i].modelname[0] = 0;
        }
    }

    for (i = edit_lods.num_lods; i < MAX_EDIT_AUTOLODS; i++)
    {
        edit_lods.lods[i].distance = 10;
        edit_lods.lods[i].fade_in = 0;
        edit_lods.lods[i].fade_out = 0;
        edit_lods.lods[i].mode = 1;
        edit_lods.lods[i].tri_percent = 100;
        edit_lods.lods[i].modelname[0] = 0;
    }

    if (edit_lods.current_lod_display >= edit_lods.num_lods)
        edit_lods.current_lod_display = edit_lods.num_lods-1;

    updateLODInfo(0);
}

static void editLODsInit(DefTracker *tracker)
{
    Model *model = tracker->def->model;

    edit_lods.tracker = tracker;
    edit_lods.model = model;

    editLODsSetLODValues(model, tracker->def);
}

static void editLODsUnsetAutomatic(void)
{
    edit_lods.automatic = 0;
}

static void editLODsAutomaticPressed(void *widget)
{
    if (!edit_lods.automatic)
        return;

    if (edit_lods.model->gld->file_format_version < 7)
    {
        updateLODInfo(0);
        editLODsApply(0, editLODsUnsetAutomatic);
    }
    else
    {
        updateLODInfo(1);
        editLODsSetLODValues(edit_lods.model, edit_lods.tracker->def);
    }
}

static int alwayslock=1;
static void addLODPage(int lod_num)
{
    editorUIAddSubWidgets(edit_lods.ui_id, editorUIShowByParentSelection,1,0);
        editorUIAddEditSlider(edit_lods.ui_id, &edit_lods.lods[lod_num].distance, 10, 2000, 0, 1000000, 1, editLODsDistanceChanged, "Distance");
        if (lod_num==0)
            editorUIAddSubWidgets(edit_lods.ui_id,0,0,&alwayslock);
        editorUIAddEditSlider(edit_lods.ui_id, &edit_lods.lods[lod_num].fade_in, 0, 100, 0, 1000, 1, editLODsDistanceChanged, "Fade In");
        if (lod_num==0)
            editorUIEndSubWidgets(edit_lods.ui_id);
        editorUIAddEditSlider(edit_lods.ui_id, &edit_lods.lods[lod_num].fade_out, 0, 100, 0, 1000, 1, editLODsDistanceChanged, "Fade Out");
        editorUIAddDynamicText(edit_lods.ui_id,editLODsGetDistStatusText);
        editorUIAddSpace(edit_lods.ui_id);
        editorUIAddCheckBox(edit_lods.ui_id, &edit_lods.lods[lod_num].use_fallback_material, editLODsFallbackChanged, "Use Fallback Material");
        editorUIAddSpace(edit_lods.ui_id);
        editorUIAddRadioButtons(edit_lods.ui_id, 1, &edit_lods.lods[lod_num].mode, NULL, "Specify LOD", "Generate LOD", NULL);
            editorUIAddSubWidgets(edit_lods.ui_id, editorUIShowByParentSelection,0,0);
                change_model_button[lod_num] = editorUIAddButton(edit_lods.ui_id, "Model Name", edit_lods.lods[lod_num].modelname, editLODsChangeModel);
            editorUIEndSubWidgets(edit_lods.ui_id);
            editorUIAddSubWidgets(edit_lods.ui_id, editorUIShowByParentSelection,0,0);
                editorUIAddSlider(edit_lods.ui_id, &edit_lods.lods[lod_num].tri_percent, 0, 100, 1, editLODsLODsChanged, "Triangles (%)");
            editorUIEndSubWidgets(edit_lods.ui_id);
            editorUIAddDynamicText(edit_lods.ui_id,editLODsGetTriStatusText);
    editorUIEndSubWidgets(edit_lods.ui_id);
}

static int last_num_reloads = 0, last_num_lod_reloads = 0;
static void editLODsPerFrame(void *widget)
{
    if (!edit_lods.tracker)
        return;

    if (getNumModelReloads() != last_num_reloads || getNumLODReloads() != last_num_lod_reloads)
    {
        DefTracker *tracker = edit_lods.tracker;
        last_num_reloads = getNumModelReloads();
        last_num_lod_reloads = getNumLODReloads();
        editLODsInit(tracker);
    }

    editLODsApplyStateMachine();
}

void editLODsUI(void)
{
    char buffer[MAX_PATH],name[MAX_PATH], *s;
    RegReader rr;
    int x = 0, y = 0;

    if (edit_lods.ui_id)
        return;
    if (sel_count != 1 || !sel_list[0].def_tracker || !sel_list[0].def_tracker->def || !sel_list[0].def_tracker->def->model || !sel_list[0].def_tracker->def->model->lod_info)
        return;

    strcpy(name, sel_list[0].def_tracker->def->model->name);
    s = strstri(name, "__");
    if (s)
        *s = 0;
    sprintf(buffer, "LOD Editor (%s)", name);
    edit_lods.ui_id = editorUICreateWindow(buffer);
    if (edit_lods.ui_id < 0)
        return;

    last_num_reloads = getNumModelReloads();
    last_num_lod_reloads = getNumLODReloads();
    edit_lods.show_current = 0;
    edit_lods.current_lod_display = 0;
    editLODsInit(sel_list[0].def_tracker);

    editCmdSnapDist();
    unSelect(1);
    editWaitingForServer(1, 0);

    rr = createRegReader();
    initRegReader(rr, regGetAppKey());
    if (!rrReadInt(rr, "LODEditorX", &x))
        x = 0;
    if (!rrReadInt(rr, "LODEditorY", &y))
        y = 0;
    destroyRegReader(rr);

    editorUISetPosition(edit_lods.ui_id, x, y);
    editorUISetCloseCallback(edit_lods.ui_id,editLODsDestroyWindow);

    editorUIAddDrawCallback(edit_lods.ui_id, editLODsPerFrame);

    editorUIAddCheckBox(edit_lods.ui_id, &edit_lods.automatic, editLODsAutomaticPressed, "Automatic LODs");
    editorUIAddCheckBox(edit_lods.ui_id, &edit_lods.show_current, editLODsShowCurrentChanged, "Show Selected LOD");
    editorUIAddSubWidgets(edit_lods.ui_id,0,0,&edit_lods.automatic);
        editorUIAddSlider(edit_lods.ui_id, &new_num_lods, 1, MAX_EDIT_AUTOLODS, 1, editLODsNumLODsChanged, "Number of LODs");
        editorUIAddBreakdownBar(edit_lods.ui_id, editLODsGetBreakdown, MAX_EDIT_AUTOLODS);
        editorUIAddTabControl(edit_lods.ui_id, &edit_lods.current_lod_display, editLODsShowCurrentChanged, editLODsGetNumLODs, "LOD 0", "LOD 1", "LOD 2", "LOD 3", NULL);
        addLODPage(0);
        addLODPage(1);
        addLODPage(2);
        addLODPage(3);
    editorUIEndSubWidgets(edit_lods.ui_id);
    editorUIAddSubWidgets(edit_lods.ui_id, editLODsShowWarning1, 0, 0);
        editorUIAddDynamicText(edit_lods.ui_id, editLODsGetWarning1);
    editorUIEndSubWidgets(edit_lods.ui_id);
    editorUIAddSubWidgets(edit_lods.ui_id, editLODsShowWarning2, 0, 0);
        editorUIAddDynamicText(edit_lods.ui_id, editLODsGetWarning2);
    editorUIEndSubWidgets(edit_lods.ui_id);
    editorUIAddButtonRow(edit_lods.ui_id, NULL, "Copy From Other Model", editLODsCopyParams, NULL);
    editorUIAddButtonRow(edit_lods.ui_id, NULL, "Ok", editLODsOkButton, "Cancel", editLODsDestroyWindow, NULL);
}



