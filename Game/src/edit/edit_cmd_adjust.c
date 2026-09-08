#include "group/group.h"
#include "edit/edit_cmd.h"
#include "edit/edit_select.h"
#include <utilitieslib/utils/mathutil.h>
#include "group/grouptrack.h"
#include "win/win_init.h"
#include <utilitieslib/utils/utils.h>
#include "edit/edit_net.h"
#include "edit_errcheck.h"
#include "edit/edit_pickcolor.h"
#include "edit/edit_drawlines.h"
#include "win/input.h"
#include "graphics/camera.h"
#include "cmdparse/cmdgame.h"
#include "gridcoll/gridcoll.h"
#include "group/groupProperties.h"
#include "graphics/sun.h"
#include "edit/edit_cmd_select.h"
#include "utils/position.h"
#include "sound/sound.h"
#include "player/player.h"
#include "resource.h"
#include "group/grouputil.h"
#include "render/tex.h"
#include "entity/entity.h"
#include "edit/edit_cmd_adjust.h"
#include <utilitieslib/components/StashTable.h>
#include "editorUI.h"
#include <utilitieslib/utils/Color.h>
#include "bases/basedraw.h"
#include "group/groupfileload.h"
#include "edit_cubemap.h"

static EditCmdBoxScaleInfo boxScaleInfo;

void getFogDist()
{
    F32            *fogdist;
    char        buf[TEXT_DIALOG_MAX_STRLEN];

    if (!sel_count || !selIsFog(0) || !isNonLibSingleSelect(0,0))
    {
        edit_state.fognear = edit_state.fogfar = 0;
        return;
    }
    fogdist = &sel_list[0].def_tracker->def->fog_near;
    if (edit_state.fogfar)
        fogdist = &sel_list[0].def_tracker->def->fog_far;
    edit_state.fognear = edit_state.fogfar = 0;

    sprintf(buf,"%f",*fogdist);
    if( !winGetString("Fog Distance",buf) )
        return; // user cancelled

    *fogdist = atof(buf);
    editUpdateTracker(sel_list[0].def_tracker);
    unSelect(2);

}

void getFogSize()
{
    if (!selIsFog(1) || !isNonLibSingleSelect(&edit_state.fogsize,0))
    {
        edit_state.fogsize = 0;
        return;
    }
    if (edit_state.fogsize == 2)
    {
        edit_state.fogsize = 0;
        editUpdateTracker(sel_list[0].def_tracker);
        unSelect(2);
    }
}

void getFogSpeed()
{
    F32 *fogspeed;
    char buf[TEXT_DIALOG_MAX_STRLEN];

    if (!sel_count || !selIsFog(0) || !isNonLibSingleSelect(0,0))
    {
        edit_state.fogspeed = 0;
        return;
    }
    fogspeed = &sel_list[0].def_tracker->def->fog_speed;
    edit_state.fogspeed = 0;

    sprintf(buf,"%f",*fogspeed);
    if( !winGetString("Fog Speed",buf) )
        return; // user cancelled

    *fogspeed = atof(buf);
    MAX1(*fogspeed, 0.001);
    editUpdateTracker(sel_list[0].def_tracker);
    unSelect(2);
}

void editCmdFogColor()
{
    int    *memp = &edit_state.fogcolor1;

    if (!*memp)
        memp = &edit_state.fogcolor2;
    get2Color(edit_state.fogcolor2,0,memp);    //now we set this inside get2Color
    //*memp=0;
}


extern char *EnterTextStrings[];

LRESULT CALLBACK editPropertyDialog(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    static PropertyDialogParams* ptd;
    //    RECT rc;
    int i;
    switch (msg)
    {
    case WM_INITDIALOG:
        {
            // check for missing params
            if (!lp) {
                EndDialog(hDlg, 0); 
                return 0; 
            }

            ptd = (PropertyDialogParams*) lp;

            /*
            GetClientRect(GetDlgItem(hDlg, IDC_EDIT), &rc);
            MapWindowPoints(GetDlgItem(hDlg, IDC_EDIT), hDlg, (LPPOINT)&rc, 2);
            MoveWindow(GetDlgItem(hDlg, IDC_EDIT), rc.left, rc.top, rc.right-rc.left, 200, FALSE);


            if (etd->prompt && etd->prompt[0])
            SetWindowText(GetDlgItem(hDlg, IDC_HELPTEXT), etd->prompt);
            */
            // set up the drop-down
            for (i = 0; EnterTextStrings[i][0]; i++)
                SendMessageA(GetDlgItem(hDlg, IDC_EDIT), CB_ADDSTRING, 0, (LPARAM)EnterTextStrings[i]);

            //ActivateScenarios;Generator;Generator Group;MarkerAI;MarkerItem;Marke
            if (ptd->property && ptd->property[0])
                SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT), ptd->property);

            if (ptd->value && ptd->value[0])
                SetWindowTextA(GetDlgItem(hDlg, IDC_EDIT1), ptd->value);

            return 1;
        }
    case WM_COMMAND:
        {
            if (LOWORD(wp) == IDCANCEL)
            {
                EndDialog(hDlg, 2);
                ptd->property = NULL;
                ptd->value = NULL;
                return 1;
            }
            else if (LOWORD(wp) == IDOK || LOWORD(wp) == IDC_ANOTHER)
            {
                GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT), ptd->property, 256); 
                GetWindowTextA(GetDlgItem(hDlg, IDC_EDIT1), ptd->value, 256); 
                // 256 == HACK, but similar to WinGetFileName
                EndDialog(hDlg, 0);
                if (LOWORD(wp) == IDOK ) {
                    EndDialog(hDlg, 0);
                } else {
                    EndDialog(hDlg, 1);
                }
                return 1;
            }
        }
    }
    return 0;
}


void editAddProperty(){
    GroupDef* def;
    PropertyEnt* prop;
    char buffer[1024];
    char buffer2[1024];
    char another = true;

    edit_state.addProperty = 0;

    if(!sel_count)
        return;

    // Property can only be added to a single item.
    if (!isNonLibSingleSelect(0,0))
        return;

    while (another) {
        // Prompt the user for the new property name.
        {

            PropertyDialogParams ptd;
            char* cursor;

            sprintf(buffer, "NewPropertyName");
            sprintf(buffer2, "0");
            ptd.property = buffer;
            ptd.value = buffer2;
            another = DialogBoxParamA(glob_hinstance, MAKEINTRESOURCEA(IDD_NEWPROP), hwnd, (DLGPROC)editPropertyDialog, (LPARAM)&ptd);
            inpClear();
            // check for cancel
            if (another == 2) {
                break;
            }

            /*

            sprintf(buffer, "NewPropertyName");

            // If the user hit the "cancel" button, don't add the property.
            if(!winGetString("Property Name", buffer)){
            return;
            }

            */

            cursor = strrchr(buffer, '\\');
            if(cursor){
                cursor++;
                strcpy(buffer, cursor);
            }
        }

        // Verify that the property does not already exist.
        {
            StashElement element;

            // Make sure the properties table exists.
            def = sel_list[0].def_tracker->def;
            if(!def->properties){
                def->properties = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys);
                def->has_properties = 1;
            }

            // Try to find the property with the new name
            stashFindElement(def->properties, buffer, &element);
            if(element){
                winMsgAlert("The property already exists!");
                return;
            }
        }

        // Add the property
        {
            prop = createPropertyEnt();
            strcpy(prop->name_str, buffer);    // Give the property the user given name
            estrPrintCharString(&prop->value_str, buffer2);    // Give the property a default value of "0".

            stashAddPointer(def->properties, prop->name_str, prop, false);

            if (stricmp(prop->name_str, "VisTray")==0 ||
                stricmp(prop->name_str, "VisOutside")==0 ||
                stricmp(prop->name_str, "VisOutsideOnly")==0 ||
                stricmp(prop->name_str, "SoundOccluder")==0 ||
                stricmp(prop->name_str, "DoWelding")==0)
            {
                groupSetTrickFlags(def);
            }
        }
    }

    // Send the server the updated group def.
    editUpdateTracker(sel_list[0].def_tracker);

    //    unSelect(2);
}


void editCmdSoundSize()
{
    int        i;

    if (!selIsSounds(1) || !isNonLibSingleSelect(0,0))
        edit_state.soundsize = 0;
    else if (edit_state.soundsize == 2)
    {
        edit_state.soundsize = 0;
        for(i=0;i<sel_count;i++)
            editUpdateTracker(sel_list[i].def_tracker);
        unSelect(2);
    }
}

void editCmdSoundVol()
{
    edit_state.soundvol = 0;
    if (sel_count && selIsSounds(1) && isNonLibSingleSelect(0,0))
    {
        char    buf[TEXT_DIALOG_MAX_STRLEN];
        int        i,t;

        sprintf(buf,"%f",sel_list[0].def_tracker->def->sound_vol / 128.f);
        if( winGetString("Sound Volume",buf) )
        {
            t = atof(buf) * 128;
            if (t > 255)
                t = 255;
            if (t < 0)
                t = 0;
            for(i=0;i<sel_count;i++)
            {
                sel_list[i].def_tracker->def->sound_vol = t;
                editUpdateTracker(sel_list[i].def_tracker);
            }
        }
    }
}

void editCmdSoundRamp()
{
    edit_state.soundramp = 0;
    if (sel_count && selIsSounds(1) && isNonLibSingleSelect(0,0))
    {
        char    buf[TEXT_DIALOG_MAX_STRLEN];
        int        i;

        sprintf(buf,"%f",sel_list[0].def_tracker->def->sound_ramp);
        if( winGetString("Sound Ramp in feet",buf) )
        {
            F32 val = (F32)atof(buf);
            for(i=0;i<sel_count;i++)
            {
                sel_list[i].def_tracker->def->sound_ramp = val;
                editUpdateTracker(sel_list[i].def_tracker);
            }
        }
    }
}

void editCmdSoundName()
{
    int fail = 0;

    if (!sel_count)
        fail = 1;
    else if (!selIsSounds(0) || !isNonLibSingleSelect(0,0))
        fail = 1;
    if (fail)
    {
        edit_state.soundname = 0;
    }
    else
    {
        int            i;
        char        *s,*s2,buf[1000];

        sprintf(buf,"%s/%s",fileDataDir(),sndGetFullPath(sel_list[0].def_tracker->def->sound_name));
        backSlashes(buf);
        s = winGetFileName("*",buf,0);
        if (s)
        {
            GroupDef    *def;

            s2 = strrchr(s,'\\');
            if (!s2++)
                s2 = s;
            for(i=0;i<sel_count;i++)
            {
                def = sel_list[i].def_tracker->def;
                strcpy(def->sound_name,s2);
                s = strrchr(def->sound_name,'.');
                if (s)
                    *s = 0;
                def->has_sound = 1;
                editUpdateTracker(sel_list[i].def_tracker);
                unSelect(2);
            }
        }
    }
    edit_state.soundname = 0;
}

void editCmdSoundScript()
{
    int fail = 0;

    if (!sel_count)
        fail = 1;
    else if (!selIsSounds(0) || !isNonLibSingleSelect(0,0))
        fail = 1;
    if (fail)
    {
        edit_state.soundscript = 0;
    }
    else
    {
        int            i;
        char        *s,*s2,buf[1000];
        char        *curname=sel_list[0].def_tracker->def->sound_script_filename;
        sprintf(buf,"%s\\sound\\scripts\\%s",fileDataDir(),(curname)?curname:"*");
        backSlashes(buf);
        s = winGetFileName("*",buf,0);
        if (s)
        {
            GroupDef    *def;

            s2 = strrchr(s,'\\');
            if (!s2++)
                s2 = s;
            for(i=0;i<sel_count;i++)
            {
                def = sel_list[i].def_tracker->def;
                groupAllocString(&def->sound_script_filename);
                strcpy(def->sound_script_filename,s2);
                s = strrchr(def->sound_name,'.');
                if (s)
                    *s = 0;
                def->has_sound = 1;
                editUpdateTracker(sel_list[i].def_tracker);
                unSelect(2);
            }
        }
    }
    edit_state.soundscript = 0;
}

void editCmdSoundFind()
{
    int            i,idx;
    static int    curr_sound_select;
    DefTracker    *tracker;

    edit_state.soundfind = 0;
    curr_sound_select++;
    for(i=0;i<sound_editor_count;i++)
    {
        idx = (i + curr_sound_select) % sound_editor_count;
        tracker = sound_editor_infos[idx].def_tracker;
        if (!tracker || groupTrackerHidden(tracker))
            continue;
        unSelect(1);
        selAdd(tracker,trackerRootID(tracker),1,0);
    }
}

void editCmdSoundExclude()
{
    edit_state.soundexclude = 0;
    if (!selIsSounds(0) || !isNonLibSingleSelect(0,0))
        return;
    sel_list[0].def_tracker->def->sound_exclude = !sel_list[0].def_tracker->def->sound_exclude;
    editUpdateTracker(sel_list[0].def_tracker);
    unSelect(2);
}

void editCmdUnsetAmbient() {
    int i;
    DefTracker ** defs=0;
    edit_state.unsetambient=0;
    for (i=0;i<sel_count;i++)
        if (sel_list[i].def_tracker->def->has_ambient) {
            sel_list[i].def_tracker->def->has_ambient=0;
            eaPush(&defs,sel_list[i].def_tracker);
        }
    if (eaSize(&defs)>0) {
        editUpdateTrackers(defs);
        eaDestroy(&defs);
    }
}

void editCmdLightColor()
{
    int fail = 0,*memp;
    int            rgb;
    GroupDef    *def;
    if (sel_count==0) {
        edit_state.lightcolor = edit_state.maxbright = edit_state.setambient = 0;
        edit_colorPicker_close();
        return;
    }
    def = sel_list[0].def_tracker->def;
    if (def==NULL) {
        edit_state.lightcolor = edit_state.maxbright = edit_state.setambient = 0;
        edit_colorPicker_close();
        return;
    }
    rgb = (def->color[0][0] << 16) | (def->color[0][1] << 8)
        | (def->color[0][2] << 0) | (def->color[0][3] << 24);

    memp = &edit_state.lightcolor;
    if (!*memp)
        memp = &edit_state.setambient;
    if (!*memp)
        memp = &edit_state.maxbright;
    if (!sel_count || !isNonLibSingleSelect(memp,0)) {
        fail = 1;
        trackerHandleDestroy(edit_state.lightHandle);
        edit_state.lightHandle = NULL;
    }
    else if (edit_state.lightcolor && !selIsLights(0)) {
        fail = 1;
        trackerHandleDestroy(edit_state.lightHandle);
        edit_state.lightHandle = NULL;
    }
    if (!fail && edit_state.maxbright && !sel_list[0].def_tracker->def->has_ambient)
    {
        winMsgAlert("You cant use maxbright unless you've already set an ambient");
        fail = 1;
        trackerHandleDestroy(edit_state.lightHandle);
        edit_state.lightHandle = NULL;
    }

    if (fail)
    {
        if (!edit_state.lightHandle) {
            edit_state.lightcolor = edit_state.maxbright = edit_state.setambient = 0;
            pickColor(&rgb,0);    //fake like there is a color picker up
        }
    }
    else
    {
        int            i,done,idx;

        int oldRGB = rgb;

        done = pickColor(&rgb,0);

        trackerHandleDestroy(edit_state.lightHandle);
        edit_state.lightHandle = trackerHandleCreate(sel_list[0].def_tracker);
    
        if (oldRGB != rgb)
        {
            for (i=0;i<sel_count;i++)
            {
                idx = edit_state.maxbright;
                def = sel_list[i].def_tracker->def;
                def->color[idx][0] = (rgb >> 16) & 255;
                def->color[idx][1] = (rgb >> 8) & 255;
                def->color[idx][2] = rgb & 255;
                def->color[idx][3] = (rgb >> 24) & 255;
            }
            
            colorTrackerDeleteAll();
        }

        if (done || edit_state.changedColor)
        {
            editSelSort();
            edit_state.sel = 0;
            if (done != -2)
            {
                for (i=0;i<sel_count;i++)
                {
                    idx = edit_state.maxbright;
                    def = sel_list[i].def_tracker->def;
                    def->color[idx][0] = (rgb >> 16) & 255;
                    def->color[idx][1] = (rgb >> 8) & 255;
                    def->color[idx][2] = rgb & 255;
                    def->color[idx][3] = (rgb >> 24) & 255;
                    if (edit_state.setambient)
                    {
                        if (done < 0)
                            def->has_ambient = 0;
                        else
                            def->has_ambient = 1;
                    }
                    if (edit_state.lightcolor)
                    {
                        if (done < 0)
                            def->has_light = 0;
                        else
                            def->has_light = 1;
                    }
                    editUpdateTracker(sel_list[i].def_tracker);
                    unSelect(2);
                }
            }
            if (done) {
                edit_state.lightcolor = edit_state.maxbright = edit_state.setambient = 0;
            }
        }
        //return;
    }
}

void editCmdLightSize()
{
    int        i;

    if (!selIsLights(1) || !isNonLibSingleSelect(&edit_state.lightsize,0))
        edit_state.lightsize = 0;
    else if (edit_state.lightsize == 2)
    {
        edit_state.lightsize = 0;
        for(i=0;i<sel_count;i++)
            editUpdateTracker(sel_list[i].def_tracker);
        unSelect(2);
    }
}


void editCmdAdjustCubemap()
{
    if (!selIsCubemap())
        return;
        
    edit_openAdjustCubemapDialog();

    if (edit_isAdjustCubemapDialogFinished()) {
        if (edit_isCubemapTrackerModified())
            edit_submitCubemapTrackerChanges();

        edit_closeAdjustCubemapDialog();
        edit_state.adjustCubemap = 0;
    }
}

void editCmdRemoveTex()
{
    edit_state.removetex = 0;
    if (isNonLibSingleSelect(0,0))
    {
        GroupDef    *def;

        def = sel_list[0].def_tracker->def;
        groupFreeString(&def->tex_names[0]);
        groupFreeString(&def->tex_names[1]);
        def->has_tex = 0;
        editUpdateTracker(sel_list[0].def_tracker);
        unSelect(2);
    }
}

void editCmdReplaceTex()
{
    int        idx;

    idx = edit_state.replacetex2;
    edit_state.replacetex1 = edit_state.replacetex2 = 0;

    if (isNonLibSingleSelect(0,0))
    {
        GroupDef    *def;
        char        *s;
        static char    buf[1000] = "c:\\game\\src\\texture_library\\*";
        BasicTexture *bind = 0;

        def = sel_list[0].def_tracker->def;

        if (def->tex_names[idx])
            bind = texFind(def->tex_names[idx]);
        if (bind)
            sprintf(buf,"c:\\game\\src\\texture_library\\%s\\%s.tga",bind->dirname,bind->name);
        backSlashes(buf);
        s = winGetFileName("Targa files (.tga)",buf,0);
        if (s)
        {
            s = strrchr(s,'\\');
            if (!s++)
                s = buf;
            groupAllocString(&def->tex_names[idx]);
            strcpy(def->tex_names[idx],s);
            def->has_tex |= 1 << idx;
            editUpdateTracker(sel_list[0].def_tracker);
            unSelect(2);
        }
    }
}

void editCmdTintColor()
{
    int    *memp = &edit_state.tintcolor1;

    if (!*memp)
        memp = &edit_state.tintcolor2;
    get2Color(edit_state.tintcolor2,1,memp);    //now we set this inside get2Color
    //        *memp = 0;
}

void editCmdSetLod()
{
    DefTracker    *ref;
    Vec3        dv;
    F32            d,dist;
    DefTracker    *tracker;
    Mat4        mat;
    int            i;

    if (!(sel_count == 1 || ((edit_state.lodfar||edit_state.lodfarfade) && sel_count)))
        winMsgAlert("Must only select one group for this operation");
    else if (!edit_state.lodscale && !sel_list[0].def_tracker->def->model)
        winMsgAlert("Cant use this on non-root group. Try lodscale instead.");
    else if (edit_state.lodscale && !isNonLibSingleSelect(0,0))
        ;
    else
    {
        ref = groupRefFind(sel_list[0].id);
        tracker = sel_list[0].def_tracker;
        trackerGetMat(tracker,ref,mat);

        subVec3(cam_info.cammat[3],mat[3],dv);
        dist = lengthVec3(dv);

        if (edit_state.lodscale)
        {
            tracker->def->lod_scale = dist / (tracker->def->vis_dist * 1.f/1.5f);
            editUpdateDef(tracker->def,0,0);
        }
        else if (edit_state.lodfarfade)
        {
            d = dist - tracker->def->lod_far;
            if (d < 0)
                d = 0;
            tracker->def->lod_farfade = d;
            for(i=0;i<sel_count;i++)
            {
                sel_list[i].def_tracker->def->lod_farfade = d;
                sel_list[i].def_tracker->def->lod_autogen = 0;
                editUpdateDef(sel_list[i].def_tracker->def,0,0);
            }
        }
        else if (edit_state.lodfar)
        {
            for(i=0;i<sel_count;i++)
            {
                sel_list[i].def_tracker->def->lod_far = dist;
                sel_list[i].def_tracker->def->lod_autogen = 0;
                editUpdateDef(sel_list[i].def_tracker->def,0,0);
            }
        }
        unSelect(2);
    }
    edit_state.lodscale = edit_state.lodfarfade = edit_state.lodfar = 0;
}
void editCmdLodFadeNode()
{
    if (sel_count != 1)
    {
        winMsgAlert("Must only select one group for this operation");
        edit_state.lodfadenode = 0;
        return;
    }
    edit_state.lodfadenode = 0;
    sel_list[0].def_tracker->def->lod_fadenode = !sel_list[0].def_tracker->def->lod_fadenode;
    editUpdateTracker(sel_list[0].def_tracker);
    unSelect(2);
}
void editCmdLodFadeNode2()
{
    if (sel_count != 1)
    {
        winMsgAlert("Must only select one group for this operation");
        edit_state.lodfadenode2 = 0;
        return;
    }
    edit_state.lodfadenode2 = 0;
    sel_list[0].def_tracker->def->lod_fadenode = (!sel_list[0].def_tracker->def->lod_fadenode)*2;
    editUpdateTracker(sel_list[0].def_tracker);
    unSelect(2);
}

void editCmdNoFogClip()
{
    int        i;

    edit_state.nofogclip = 0;
    for(i=0;i<sel_count;i++)
    {
        sel_list[0].def_tracker->def->no_fog_clip = !sel_list[0].def_tracker->def->no_fog_clip;
        editUpdateTracker(sel_list[0].def_tracker);
    }
    unSelect(2);
}

void editCmdNoColl()
{
    int        i;

    edit_state.nocoll = 0;
    for(i=0;i<sel_count;i++)
    {
        sel_list[0].def_tracker->def->no_coll = !sel_list[0].def_tracker->def->no_coll;
        editUpdateTracker(sel_list[0].def_tracker);
    }
    unSelect(2);
}

//MW 5-30-03 Disabled this; it was slowing engine down, and no one seems to use it 
void editCmdLodDisable()
{
    int        i;

    for(i=0;i<sel_count;i++)
    {
        if (!sel_list[i].def_tracker->tricks)
            sel_list[i].def_tracker->tricks = calloc(sizeof(TrickInfo),1);
        //sel_list[i].def_tracker->tricks->flags |= TRICK_NOLODFADE;  //Disabled
    }
    edit_state.loddisable = 0;
}
void editCmdLodEnable()
{
    /*
    for(i=0;i<group_info.ref_count;i++)
    editUnhide(group_info.refs[i],TRICK_NOLODFADE);
    */
    edit_state.lodenable = 0;
}
//End Disabled

void editCmdBeaconName()
{

    if (isNonLibSingleSelect(0,0))
    {
        GroupDef    *def;
        char        *s, *s2;

        // show a "open file" dialog for the user to enter some text as "filename"
        def = sel_list[0].def_tracker->def;
        if(def->has_beacon || edit_state.noerrcheck){
            groupAllocString(&def->beacon_name);
            s = def->beacon_name;
            if( winGetString("Beacon Name",s) )
            {
                // extract the user entered text from fullpath
                s2 = strrchr(s,'\\');
                if (!s2++)
                    s2 = s;

                // set the beacon name
                strcpy(def->beacon_name,s2);
                def->has_beacon = 1;

                // tell the server the world has been modified
                editUpdateTracker(sel_list[0].def_tracker);
                unSelect(2);
            }
        }
    }

    edit_state.beaconName = 0;
}

void editCmdBeaconSize()
{
    GroupDef* def = sel_count > 0 ? sel_list[0].def_tracker->def : NULL;

    // Do not proceed unless a beacon has been selected
    if (!isNonLibSingleSelect(0,0) || !selIsBeacon())
    {
        edit_state.beaconSize = 0;
    }
    else if (edit_state.beaconSize == 2)
    {
        int i;

        edit_state.beaconSize = 0;
        for(i = 0; i < sel_count; i++)
            editUpdateTracker(sel_list[i].def_tracker);
        unSelect(2);
    }
}

void editCmdShowBeaconConnection()
{
    char buffer[1024];

    if(edit_state.shownBeaconType >= 0)
    {
        // Do not proceed unless a beacon has been selected
        if(sel_count)
        {
            int i;
            char* pos = buffer;

            // Create a string containing the coord of the origin beacon

            pos += sprintf(pos, "entcontrol -1 beacon %i ", edit_state.shownBeaconType);

            for(i = 0; i < min(sel_count, 2); i++){
                pos += sprintf(pos, "%f %f %f ", posParamsXYZ(sel_list[i].def_tracker));
            }

            cmdGameParse(buffer, 0, 0);
        }
        else if(!sel_count)
        {
            Entity* player = playerPtr();
            if(player)
            {
                sprintf(buffer,
                    "entcontrol -1 beacon %i %f %f %f",
                    edit_state.shownBeaconType,
                    ENTPOSPARAMS(player));
                cmdGameParse(buffer, 0, 0);
            }
        }
    }
    edit_state.showBeaconConnection = 0;
}

void editCmdShowBeaconPath(int start)
{
    char buffer[1024];

    edit_state.showBeaconPathStart = 0;
    edit_state.showBeaconPathContinue = 0;

    if(!start || (sel_count == 2 && sel_list[0].def_tracker->def->has_beacon && sel_list[1].def_tracker->def->has_beacon)){
        if(start){
            sprintf(buffer,
                "beaconpathdebug 1 %f %f %f %f %f %f",
                posParamsXYZ(sel_list[0].def_tracker),
                posParamsXYZ(sel_list[1].def_tracker));
        }else{
            sprintf(buffer, "beaconpathdebug 0 0 0 0 0 0 0");
        }

        cmdGameParse(buffer, 0, 0);
    }
}


static char g_propertyName[128];
static int g_propertyIdxToGet;
static int g_propertyIdx = 0;
static DefTracker * g_propertyTracker = 0;
extern Vec3 view_pos;

int myPropertyFinder(GroupDefTraverser* traverser)
{
    PropertyEnt* property;
    DefTracker* tracker = 0;

    // find the mission room marker
    if (traverser->def->properties)
    {
        stashFindPointer(  traverser->def->properties, g_propertyName , &property );
        if (property) //I found this property
        {
            //TO DO ok, how do I get a tracker out of this?
            //copyVec3(traverser->mat[3], roomPos);

            if( g_propertyIdx == g_propertyIdxToGet )
            {
                g_propertyTracker = (void*)1;//TO DO temp tracker;
                copyVec3(traverser->mat[3],view_pos); //TO DO temp
            }
            g_propertyIdx++;
        }
    }

    if (!traverser->def->child_properties)    // if none of our children have props, don't go further
        return 0;
    return 1; // traverse children
}

void editCmdFindNextProperty( int findType )
{
    //1 = new property, 2 = use old property
    if( findType == 1 )
    {    
        int result;
        char buffer[1024];
        char buffer2[1024];
        PropertyDialogParams ptd = {0};

        sprintf(buffer, "PropertyToLookFor");
        sprintf(buffer2, "ValueSearchNotYetImplemented");

        ptd.property = buffer;
        ptd.value = buffer2;
        result = DialogBoxParam(glob_hinstance, MAKEINTRESOURCE(IDD_NEWPROP), hwnd, (DLGPROC)editPropertyDialog, (LPARAM)&ptd);
        //inpClear();
        // check for cancel
        //if (another == 2)
        //    break;
        strncpy( g_propertyName, ptd.property, 127 );

        g_propertyIdxToGet = 0;
    }

    if( g_propertyName[0] )
    {
        GroupDefTraverser traverser = {0};
        g_propertyTracker = 0;
        groupProcessDefExBegin(&traverser, &group_info, myPropertyFinder);    // first rooms
        g_propertyIdxToGet++; //next time get the next one
    }

    if( g_propertyTracker )
    {
        //TO DO comment back in
        //editCmdSelect( g_propertyTracker );
        //editCmdSnapDist();
    }
    else
    {
        g_propertyIdxToGet = 0; //no more to find, next time you search, loop from the start
    }
}

void editCmdBurningBuildingSpeed() {
    double speed;
    char buf[TEXT_DIALOG_MAX_STRLEN];
    char msg[255];
    edit_state.burningBuildingSpeedChange=0;
    if( !winGetString("Enter new speed in %/second.",buf) )
        return;
    speed=atof(buf);
    sprintf(msg,"speed %f%%/s",speed);
    estrPrintCharString(&edit_state.burningBuildingMenuEntry->name, msg);
    edit_state.burningBuildingSpeed=speed;
    edit_state.burningBuildingSpeedChange=0;
};

void editCmdBurningBuildingModifyHealth() {
    StashElement e;
    PropertyEnt *propertyEnt;
    DefTracker * def=edit_state.burningBuilding;
    double currentHealth;
    int clockTick=clock();
    char sign;
    if (def==NULL) return;
    if (edit_state.burningBuildingLastClockTick==0) {
        edit_state.burningBuildingLastClockTick=clock();
        return;
    }
    stashFindElement(def->def->properties,"Health", &e);
    if (e==NULL) return;
    propertyEnt = ((PropertyEnt *)stashElementGetPointer(e));
    currentHealth = atof(propertyEnt->value_str);
    currentHealth+=((double)(clockTick-edit_state.burningBuildingLastClockTick)/(double)CLK_TCK)*edit_state.burningBuildingRealRate;
    edit_state.burningBuildingLastClockTick=clockTick;
    if (edit_state.burningBuildingRealRate<0)
        sign='-';
    else
        sign='+';
    if (currentHealth<0 || currentHealth>100) {
        edit_state.burningBuildingRealRate=0;
        edit_state.burningBuildingLastClockTick=0;
    }
    if (currentHealth<0) currentHealth=0;
    if (currentHealth>100) currentHealth=100;
    estrPrintf(&propertyEnt->value_str, "%f%c", currentHealth,sign);
}

void boxScaleChanged(void* notUsed) {
    boxScaleInfo.def->box_scale[0]=atof(boxScaleInfo.x);
    boxScaleInfo.def->box_scale[1]=atof(boxScaleInfo.y);
    boxScaleInfo.def->box_scale[2]=atof(boxScaleInfo.z);
}

void editCmdBoxScaleDestroyed() {
    if (boxScaleInfo.tracker==NULL)
        return;
    boxScaleInfo.tracker->invisible=0;
    ZeroStruct(&boxScaleInfo);
    edit_state.boxscale=0;
}

void editCmdBoxScaleDone(char * button) {
    if (stricmp(button,"OK")==0)
        editUpdateTracker(boxScaleInfo.tracker);
    copyVec3(boxScaleInfo.oldScale,boxScaleInfo.def->box_scale);
    editorUIDestroyWindow(boxScaleInfo.ID);
}

void editCmdBoxScale() {
    Color cc[2];
    if (boxScaleInfo.ID) return;
    if (sel_count!=1) return;
    if (!sel_list[0].def_tracker->def->has_volume) return;
    cc[0].integer=0xff9f0000;
    cc[1].integer=0xff806f5f;
    boxScaleInfo.tracker=sel_list[0].def_tracker;
    boxScaleInfo.tracker->invisible=1;
    boxScaleInfo.def=sel_list[0].def_tracker->def;
    copyMat4(sel_list[0].def_tracker->mat,boxScaleInfo.mat);
    copyVec3(boxScaleInfo.def->box_scale,boxScaleInfo.oldScale);
    boxScaleInfo.ID=editorUICreateWindow("Volume Size");
    editorUISetCloseCallback(boxScaleInfo.ID,editCmdBoxScaleDestroyed);
    sprintf(boxScaleInfo.x,"%.2f",sel_list[0].def_tracker->def->box_scale[0]);
    sprintf(boxScaleInfo.y,"%.2f",sel_list[0].def_tracker->def->box_scale[1]);
    sprintf(boxScaleInfo.z,"%.2f",sel_list[0].def_tracker->def->box_scale[2]);
    editorUIAddTextEntry(boxScaleInfo.ID,boxScaleInfo.x,256,boxScaleChanged,"X");
    editorUIAddTextEntry(boxScaleInfo.ID,boxScaleInfo.y,256,boxScaleChanged,"Y");
    editorUIAddTextEntry(boxScaleInfo.ID,boxScaleInfo.z,256,boxScaleChanged,"Z");
    editorUIAddButtonRow(boxScaleInfo.ID,NULL,"OK",editCmdBoxScaleDone,"Cancel",editCmdBoxScaleDone,NULL);
}

// same as drawScaledBox, except it works properly for non-orthogonally aligned boxes
void drawScaledBoxArbitrary(Mat4 mat,Vec3 size,GroupDef *box,Color tint[2])
{
    Vec3 scalevec;
    Mat4 boxmat;
    Mat4 buffer;
    int i;

    if (!box)
        return;
    for(i=0;i<3;i++)
        scalevec[i] = (size[i]) / (box->max[i] - box->min[i]);

    scaleMat3Vec3Xfer(unitmat,scalevec,buffer);
    mulMat3(mat,buffer,boxmat);
    setVec3(scalevec,0,0,0);
    mulVecMat4(scalevec,mat,boxmat[3]);
    drawDefSimple(box,boxmat,tint);
}

void scalableVolumesDraw() {
    if (editMode()) {
        Color cc[2];
        cc[0].integer=0xff9f0000;
        cc[1].integer=0xff806f5f;
        if (boxScaleInfo.def!=NULL)
        {
            GroupDef *newdef = (boxScaleInfo.def->entries && boxScaleInfo.def->entries[0].def && boxScaleInfo.def->entries[0].def->is_tray)?groupDefFindWithLoad("tray_collision_box_16x16"):groupDefFindWithLoad("collision_box_16x16");
            drawScaledBoxArbitrary(boxScaleInfo.mat,boxScaleInfo.def->box_scale,newdef,cc);
        }
    }
}
