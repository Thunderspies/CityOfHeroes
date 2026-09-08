#include "gridcoll/gridcoll.h"
#include "edit/edit_select.h"
#include <utilitieslib/utils/mathutil.h>
#include "win/win_init.h"
#include "group/grouptrack.h"
#include "graphics/camera.h"
#include "edit/edit_cmd.h"
#include "win/input.h"
#include "graphics/font.h"
#include "edit/edit_drawlines.h"
#include "graphics/groupdraw.h"
#include "cmdparse/cmdgame.h"
#include "graphics/gfx.h"
#include "edit_info.h"
#include "edit/edit_net.h"
#include "graphics/gfxwindow.h"
#include "edit/edit_cmd_select.h"
#include "sound/sound.h"
#include "group/grouputil.h"
#include "edit_cmd_group.h"
#include "UI/uiInput.h"
#include "seq/gfxtree.h"
#include "group/groupgrid.h"
#include "seq/anim.h"
#include "edit_errcheck.h"
#include "edit/edit_cmd_adjust.h"
#include "resource.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/components/StashTable.h>
#include "editorUI.h"
#include "Properties.h"
#include "edit/edit_library.h"
#include "group/groupfileload.h"
#include <utilitieslib/components/EString.h>

Vec3        quickPlacementRotateNormal;


EditSelect sel;


DefTracker    sel_refs[12000];
int            sel_ref_count;

SelInfo    sel_list[12000];
int        sel_count;

int            mouseover_ctri_idx = -1;
DefTracker    *mouseover_tracker = 0;

int            g_edit_collide_flags;
int            g_needRelock;
Vec3        snapped_pyr;

const F32 lock_angs[] = {  .25, .5, 1, 5, 10, 11.25, 22.5, 45, 90 };

int editCollNodeCallback(void *nd,int facing)
{
    DefTracker    *tracker = nd;

    if (edit_state.ignoretray && tracker->def->is_tray)
        return 0;

    for(;tracker;tracker = tracker->parent)
    {
        Model* model=0;

        // MS: Prevent clicking on a door's invisible world-geometry collision.
        //     They don't have names, and show up as "(null)/(null)" when selected in the editor.
        //     This is annoying and confusing and no one could possibly ever want to select them.
        
        if(!tracker->def->name)
            return 0;
        
        if (tracker->def->model)
            model = tracker->def->model;
#if 1
        // BR: this should be handled in the lower level code now - tell bruce if it doesn't
        // MS: Put this back in for special case (TRICK_NOT_SELECTABLE|TRICK_NOCOLL) where COLL_NOTSELECTABLE doesn't get set. 
        //     See comments in groupSetTrickFlags(...).
        if (!edit_state.select_notselectable && model && model->trick && (model->trick->flags1 & TRICK_NOT_SELECTABLE))
            return 0;
#endif    
        if (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE))
            return 0;
        if (edit_state.hideCollisionVolumes && tracker->def->volume_trigger)
            return 0;
    }

    if(edit_state.selectEditOnly){
        for(tracker = nd;tracker;tracker = tracker->parent)
        {
        Model* model=0;

        if (tracker->def->model)
            model = tracker->def->model;

            if (model && model->trick && (model->trick->flags1 & TRICK_EDITORVISIBLE))
                return 1;
        }
        return 0;
    }else{
        return 1;
    }
}


void setScale()
{
    copyMat4(unitmat,sel.scalenode->mat);
    scaleMat3Vec3(sel.scalenode->mat,sel.scale);
    sel.scalenode->mat[3][1] = (sel.max[1] - sel.min[1])/2 * (sel.scale[1] - 1.f);
}


void selCopy(const Vec3 pos)
{
int            i,ref_id,count;
DefTracker    *ref,*selref;
Mat4        mat,inv_mat,tracker_mat;
DefTracker    *tracker;

    if (sel.fake_ref.def && sel_count > 1)
    {
        winMsgAlert("You can only do this with one item selected!");
        sel.fake_ref.def = 0;
        return;
    }
    if (sel.parent)
    {
        gfxTreeDelete(sel.parent);
        for(i=0;i<sel_ref_count;i++)
            groupRefGridDelEx(&sel_refs[i],&sel.grid);
        sel_ref_count = 0;
        gridFreeAll(&sel.grid);
        sel.parent = 0;
    }
    if (sel.fake_ref.entries)
    {
        trackerClose(&sel.fake_ref);
        //sel.fake_ref.def_tracker = 0;
    }
    if (!sel_count && !sel.fake_ref.def)
        return;

    sel.parent = gfxTreeInsert(0);
    sel.offsetnode = gfxTreeInsert(sel.parent);
    sel.scalenode = gfxTreeInsert(sel.offsetnode);
    copyMat4(unitmat,sel.scalenode->mat);
    if (!sel_count)
    {
    Vec3    dv;
    F32        size;

        ref = &sel.fake_ref;
        subVec3(ref->def->max,ref->def->min,dv);
        size = lengthVec3(dv);
        if(pos)
        {
            copyVec3(pos, ref->mat[3]);
        }
        else
        {
            copyVec3(cam_info.cammat[3],ref->mat[3]);
            moveVinZ(ref->mat[3],cam_info.cammat,-size);
        }
        tracker = ref;//trackerFind(ref,0,0);
        trackerGetMat(tracker,ref,tracker_mat);
    }
    else
    {
        ref = groupRefFind(sel_list[0].id);
        tracker = sel_list[0].def_tracker;
        trackerGetMat(tracker,ref,tracker_mat);
    }
    zeroVec3(sel.offsetnode->mat[3]);
    copyMat3(unitmat,sel.parent->mat);
    copyMat3(tracker_mat,sel.offsetnode->mat);
    copyVec3(tracker_mat[3],sel.parent->mat[3]);
    copyVec3(tracker->def->min,sel.min);
    copyVec3(tracker->def->max,sel.max);

    if (sel.use_pick_point)
    {
        sel.use_pick_point = 0;
        subVec3(sel.parent->mat[3],sel.pick_point,sel.offsetnode->mat[3]);
        copyVec3(sel.pick_point,sel.parent->mat[3]);
    }

    getMat3YPR(sel.offsetnode->mat,sel.pyr);
    zeroVec3(sel.pyr);
    extractScale(sel.offsetnode->mat,sel.scale);
    setVec3(sel.scale,1,1,1);
    for(i=0;i<3;i++)
        sel.scale_inv[i] = 1.f / sel.scale[i];

    mulMat4(sel.parent->mat,sel.offsetnode->mat,mat);
    invertMat4Copy(mat,inv_mat);
    //setVec3(sel.min,8e16,8e16,8e16);
    if (sel.fake_ref.def)
        count = 1;
    else
        count = sel_count;
    for(i=0;i<count;i++)
    {
        if (sel_count)
        {
            ref = groupRefFind(sel_list[i].id);
            tracker = sel_list[i].def_tracker;
            trackerGetMat(tracker,ref,tracker_mat);
            if (sel.fake_ref.def)
            {
                ref = &sel.fake_ref;
                tracker = ref;//trackerFind(ref,0,0);
            }
        }
        mulMat4(inv_mat,tracker_mat,sel_list[i].mat);
        if (sel.fake_ref.def)
            ref = &sel.fake_ref;
        if (ref != &sel.fake_ref)
            ref_id = ref->id;
        else
            ref_id = 0;
        //groupActivate(ref->def,sel_list[i].mat,ref_id,sel.scalenode,&sel.grid);

        selref = &sel_refs[i];
        memset(selref,0,sizeof(DefTracker));
        selref->def = tracker->def;
        selref->id = -1;
        copyMat4(sel_list[i].mat,selref->mat);
        collGridAddGroup(&sel.grid,selref,selref);
        if(sel_count && i == 0)
        {
            copyMat3(sel_list[i].def_tracker->mat, sel.fake_ref.mat);
        }
    }
    sel_ref_count = count;
}

void edit_relock() 
{
    Mat4    mat;

    g_needRelock = 0;
    if (!edit_state.scale)
    {
        mulMat4(sel.parent->mat,sel.offsetnode->mat,mat);
        if (!edit_state.nosnap && edit_state.snaptype == SNAP_CENTER)
            copyVec3(mat[3],sel.pick_point);
        subVec3(mat[3],sel.pick_point,sel.offsetnode->mat[3]);
        copyVec3(sel.pick_point,sel.parent->mat[3]);
        copyMat3(mat,sel.offsetnode->mat);
        copyMat3(unitmat,sel.parent->mat);
        zeroVec3(sel.pyr);
        zeroVec3(snapped_pyr);
    }
}

//abandon all hope, ye who enter here...
//selUpdate modifies the currently selected objects according to this input
//parameters and the current mode, specified by 20 variables in edit_state
//start is the 3d coordinates of where the user has clicked (on the screen)
//end is the 3d coordinates of the line that extends from the camera through
//        start, for about 10000 feet or so
//i don't know what lost_focus is, perhaps it is true if the selected objects are
//no longer selected?

//BIG TO DO: lots of commented out code in this function.  if things have been working properly
//in here for a few weeks, get rid of it all
int selUpdate(Vec3 start,Vec3 end,int lost_focus)
{
    extern int mouse_dx,mouse_dy;
    F32        x=0,z=0,y=0;
    Vec3    dv;
    Mat4    mat;                        //stores the axes and origin of the coordinate system
                                        //that the object is being modified in (translated, rotated...)

    int        edit_axis[3] = {0,0,0},i;    //this variable is supposed to determine which axes the object
                                        //can be modified along, it does not work correctly with the rest
                                        //of the code, and i think its only purpose now is to tell
                                        //showGrid which lines to draw
    F32        rotval;
    static    int last_plane;
    static int        old_edit_axis[3];
    Vec3        oldPosition;            //position of the object *now*, which will be old by the
                                        //end of the function
    static Vec3    localPyr;                //store the snapped_pyr for local rotations
    static int    localRotation=0;        //when we first start to localrot, we need to store the matrix
                                        //this will flag when we first enter localrot
    static Mat4    localRotationMatrix;    //and this is the matrix

    if(edit_state.localrot && isDown(MS_LEFT))
    {
        if(    (localRotation==0 || localRotation!=edit_state.plane+1) &&
            sel.offsetnode != NULL)
        {
            localRotation=edit_state.plane+1;
            copyMat4(sel.offsetnode->mat,localRotationMatrix);
            zeroVec3(localPyr);
        }
    }
    else
    {
        localRotation=0;
    }

    if (sel.parent)
        copyVec3(sel.parent->mat[3],oldPosition);

    if (edit_state.lightsize || edit_state.soundsize || edit_state.fogsize || edit_state.beaconSize || edit_state.editPropRadius)
    {
        F32            scaleval,size;
        int            i;
        GroupDef    *def;
        U32            color;
        extern float propRadius;

        //extractScale(sel.parent->mat,sel.scale);
        scaleval = -mouse_dy * 0.1f;

        for(i=0;i<sel_count;i++)
        {
            def = sel_list[i].def_tracker->def;
            if (edit_state.lightsize && !def->has_light)
                continue;
            if (edit_state.soundsize && !def->has_sound)
                continue;
            if (edit_state.fogsize && !def->has_fog)
                continue;
            if (edit_state.beaconSize && !def->has_beacon)
                continue;

            if (edit_state.beaconSize)
                size = def->beacon_radius;
            else if (edit_state.fogsize)
                size = def->fog_radius;
            else if (edit_state.lightsize)
                size = def->light_radius;
            else if (edit_state.soundsize)
                size = def->sound_radius;
            else if (edit_state.editPropRadius)
                size = propRadius;

            if (!(lost_focus || !inpLevel(INP_LBUTTON) || inpIsMouseLocked()))
            {
                scaleval = size * 0.01f;
                if (scaleval < 0.01)
                    scaleval = 0.01;
                size -= mouse_dy * scaleval;
                if (size < 0)
                    size = 0;

                if (edit_state.beaconSize)
                    def->beacon_radius = size;
                else if (edit_state.fogsize)
                    def->fog_radius = size;
                else if (edit_state.lightsize)
                    def->light_radius = size;
                else if (edit_state.soundsize)
                    def->sound_radius = size;
                else if(edit_state.editPropRadius)
                    propRadius = size;
            }
            xyprintf(1,windowScaleY(45),"Radius %f",size);
            color = 0xffff00;
            if (sel_count == 1 && sel_list[0].def_tracker->def->sound_exclude)
                color = 0xff0000;
            drawSphere(sel_list[i].def_tracker->mat,size,20,color);
        }
    }

    //i don't think anyone uses zerogrid.  i think if we get rid of it we can also get
    //rid of showGrid2.  honestly, why do we need a showGrid2?
    if (edit_state.zerogrid)
    {
    int        draw_axis[3] = {1,0,1},zero_axis[3] = {0,0,0};

        showGrid2(unitmat,draw_axis,zero_axis);
    }

    //!sel.parent means that nothing is selected
    if (!sel.parent || (edit_state.tintcolor1 || edit_state.tintcolor2))
    {
        g_needRelock = 1;
        return 0;
    }

retry:
    setScale();
    copyMat4(unitmat,mat);

    if (edit_state.posrot)
    {
//        if (sel.fake_ref.def!=NULL) {
//            if (edit_state.localrot)
//                mulMat4(sel.fake_ref.mat,sel.offsetnode->mat,mat);
//            else
//                copyMat3(sel.fake_ref.mat,mat);
//        } else {
            if (edit_state.localrot)
                mulMat4(sel.parent->mat,sel.offsetnode->mat,mat);
            else
                copyMat3(sel.parent->mat,mat);
//        }
//        if (edit_state.useObjectAxes)
//            copyMat3(edit_state.objectMatrix,mat);
        if (edit_state.localrot) {
            drawRotationGuide(mat[3],mat,edit_state.plane);
        } else {
            Mat4 buf,buf2;
            mulMat4(sel.parent->mat,sel.offsetnode->mat,buf);
            mulMat4(buf,sel.scalenode->mat,buf2);
            if (edit_state.useObjectAxes)
                drawRotationGuide(buf2[3],edit_state.objectMatrix,edit_state.plane);
            else if (edit_state.snaptype == SNAP_VERT)
            {
                drawRotationGuide(sel.parent->mat[3],unitmat,edit_state.plane);
            }
            else
                drawRotationGuide(buf2[3],unitmat,edit_state.plane);
        }
    }

    copyVec3(sel.parent->mat[3],mat[3]);

    switch(edit_state.plane)            //this code changes the axes so that we are translating
    {                                    //and rotating along the proper axes.  Each does two swaps
        Vec3 buf;                        //so that the determinate of the matrix remains 1, and not -1
        case 0:
        xcase 1:
            copyVec3(mat[0],buf);
            copyVec3(mat[1],mat[0]);
            copyVec3(buf,mat[1]);
            copyVec3(mat[1],buf);
            copyVec3(mat[2],mat[1]);
            copyVec3(buf,mat[2]);
        xcase 2:
            copyVec3(mat[0],buf);
            copyVec3(mat[2],mat[0]);
            copyVec3(buf,mat[2]);
            copyVec3(mat[1],buf);
            copyVec3(mat[2],mat[1]);
            copyVec3(buf,mat[2]);
    }
    if (edit_state.posrot)
    {
        switch(edit_state.plane)
        {
            case 0:
                edit_axis[0] = 1;
            xcase 1:
                edit_axis[1] = 1;
            xcase 2:
                edit_axis[2] = 1;
        }
    }
    else
    {
        switch(edit_state.plane)    //you'll notice that this code looks silly, if you can fix it, awesome
        {
            case 0:
                edit_axis[1] = 1;
                if (!edit_state.singleaxis)
                    edit_axis[2] = 1;
            xcase 1:
                edit_axis[1] = 1;
                if (!edit_state.singleaxis)
                    edit_axis[2] = 1;
            xcase 2:
                edit_axis[1] = 1;
                if (!edit_state.singleaxis)
                    edit_axis[2] = 1;
        }
    }
    if (edit_state.quickPlacement || lost_focus || !inpLevel(INP_LBUTTON) || inpIsMouseLocked())
    {
        if (edit_state.useObjectAxes && !edit_state.posrot) {
            Mat4 buf;
            mulMat3(edit_state.objectMatrix,mat,buf);
            copyMat3(buf,mat);
            showGrid(mat,edit_axis,old_edit_axis,0);
        } else
        if(edit_state.quickPlacement){
            edit_axis[0] = 1;
            edit_axis[1] = 0;
            edit_axis[2] = 1;
            copyMat3(unitmat, mat);
            copyVec3(sel.parent->mat[3], mat[3]);
            showGrid(mat,edit_axis,old_edit_axis,0);
        } else {
            if (!edit_state.posrot) {
                if (edit_state.singleaxis && edit_state.plane==0)
                {
                    Vec3 buf;
                    Mat4 mbuf;
                    edit_axis[0]=1;
                    edit_axis[1]=0;
                    copyMat4(mat,mbuf);
                    copyVec3(mat[0],buf);
                    copyVec3(mat[2],mat[0]);
                    copyVec3(buf,mat[2]);
                    copyVec3(mat[1],buf);
                    copyVec3(mat[2],mat[1]);
                    copyVec3(buf,mat[2]);
                    edit_state.plane=2;
                    showGrid(mat,edit_axis,old_edit_axis,edit_state.posrot);
                    copyMat4(mbuf,mat);
                    edit_axis[0]=0;
                    edit_axis[1]=1;
                    edit_state.plane=0;
                } else 
                    showGrid(mat,edit_axis,old_edit_axis,edit_state.posrot);
            }
        }

        if(lost_focus || !inpLevel(INP_LBUTTON) || inpIsMouseLocked()){
            if (edit_state.nudgeLeft || edit_state.nudgeRight || edit_state.nudgeUp || edit_state.nudgeDown)
                goto nudger;
            g_needRelock = 1;
            return 0;
        }
    }
    if(!edit_state.quickPlacement)
    {
        if (g_needRelock)
        {
            edit_relock();
            goto retry;
        }
    }
//    add3dCursor(sel.parent->mat[3],NULL);
    {
        if (edit_state.posrot || edit_state.quickPlacement)
        {
            if(edit_state.quickPlacement && edit_state.quickRotate || !edit_state.quickPlacement && edit_state.posrot)
            {
                rotval = RAD(mouse_dy);
                if (last_plane != edit_state.plane)
                {
                    last_plane = edit_state.plane;
                    copyVec3(snapped_pyr,sel.pyr);
                }
                if (edit_state.localrot)
                    zeroVec3(sel.pyr);
                
                if(edit_state.quickPlacement && edit_state.quickRotate && edit_state.quickOrient){
                    static F32 yaw_acc;
                    static F32 cur_yaw;
                    
                    F32 yaw;
                    Vec3 rot_pyr = {0,0,0};
                    Mat3 rot_mat;
                    F32 lock_angle;
                    Mat4 mat_orig;
                    
                    yaw_acc += rotval;
                    lock_angle = lock_angs[edit_state.rotsize];
                    
                    yaw = (int)(DEG(yaw_acc) / lock_angle) * lock_angle;
                    
                    yaw = RAD(yaw);
                    
                    getVec3YP(quickPlacementRotateNormal, &rot_pyr[1], &rot_pyr[0]);
                    
                    //printf("normal: %f\t%f\t%f\n", vecParamsXYZ(quickPlacementRotateNormal));
                    //printf("pyr:    %f\t%f\t%f\n", vecParamsXYZ(rot_pyr));

                    // I have no idea why this works, the multiply order is totally backwards.
                    
                    copyMat3(unitmat, rot_mat);
                    scaleVec3(rot_pyr, -1, rot_pyr);
                    createMat3PYR(rot_mat, rot_pyr);
                    copyMat4(sel.parent->mat, mat_orig);
                    mulMat3(rot_mat, mat_orig, sel.parent->mat);

                    copyMat3(unitmat, rot_mat);
                    rollMat3(yaw - cur_yaw, rot_mat);
                    copyMat4(sel.parent->mat, mat_orig);
                    mulMat3(rot_mat, mat_orig, sel.parent->mat);

                    copyMat3(unitmat, rot_mat);
                    scaleVec3(rot_pyr, -1, rot_pyr);
                    createMat3YPR(rot_mat, rot_pyr);
                    copyMat4(sel.parent->mat, mat_orig);
                    mulMat3(rot_mat, mat_orig, sel.parent->mat);

                    cur_yaw = yaw;
                }else{
                    switch(edit_state.quickPlacement ? 0 : edit_state.plane)
                    {
                        case 0:
                            sel.pyr[1] += rotval;
                        xcase 1:
                            sel.pyr[2] += rotval;
                        xcase 2:
                            sel.pyr[0] += rotval;
                    }
                    copyVec3(sel.pyr,snapped_pyr);
                    if (edit_state.localrot)
                        rotval=0;
                    {
                        F32 lock_angle;

                        lock_angle = lock_angs[edit_state.rotsize];
                        for(i=0;i<3;i++)
                        {
                            int        t;

// this line probably needs to be here in some fashion or another
//                            if (!edit_state.quickPlacement || edit_state.quickPlacement && i != 1)
//                                continue;
                            t = DEG(sel.pyr[i]) / lock_angle;
                            snapped_pyr[i] = RAD(lock_angle * t);
                        }

                    }
                    if (edit_state.localrot)
                    {
                        Mat4    mat,matx;
                        Vec3 buf;
                        addVec3(sel.pyr,localPyr,buf);
                        copyVec3(buf,localPyr);
                        zeroVec3(sel.pyr);
                        if (!edit_state.nosnap)    {
                            F32 lock_angle;
                            lock_angle = lock_angs[edit_state.rotsize];
                            for(i=0;i<3;i++)
                            {
                                int        t;
                                t = DEG(localPyr[i]) / lock_angle;
                                buf[i] = RAD(lock_angle * t);
                            }
                        }
                        createMat3YPR(mat,buf);
                        mulMat3(localRotationMatrix,mat,matx);
                        copyMat3(matx,sel.offsetnode->mat);
                    } else if (edit_state.useObjectAxes)
                    {
                        Mat4    currentMatrix;
                        Mat4    currentPyrMatrix;
                        Mat4    revisedMatrix;
                        mulMat3(sel.parent->mat,edit_state.objectMatrix,currentMatrix);
                        copyMat3(edit_state.objectMatrix,currentMatrix);
                        createMat3YPR(currentPyrMatrix,snapped_pyr);
                        mulMat3(currentMatrix,currentPyrMatrix,revisedMatrix);
                        mulMat3(revisedMatrix,edit_state.objectMatrixInverse,sel.parent->mat);
                    }
                    else {
                        createMat3YPR(sel.parent->mat,snapped_pyr);
                    }
                }
            }
        }
        else
        {
            if (edit_state.snaptovert)
            {
                copyVec3(sel.snap_pos,sel.parent->mat[3]);
                copyVec3(sel.snap_pos,mat[3]);
            }

            if (edit_state.useObjectAxes) {
                Mat4 buf;
                mulMat3(edit_state.objectMatrix,mat,buf);
                copyMat3(buf,mat);
            }
nudger:
            if (!planeIntersect(start,end,mat,dv))
                return !g_needRelock;
            {    //handle which axes the object can be moved along
                int i;
                Vec3 move;
                Vec3 buf,buf2;
                subVec3(dv,sel.parent->mat[3],move);
                setVec3(dv,0,0,0);
                for (i=0;i<3;i++){//(edit_state.singleaxis?1:2);i++) {
                    double d=dotVec3(move,mat[i]);
                    if (edit_state.singleaxis && !i)
                        d=0;
                    scaleVec3(mat[i],d,buf);
                    addVec3(buf,dv,buf2);
                    copyVec3(buf2,dv);
                }
                copyVec3(dv,buf);
                addVec3(sel.parent->mat[3],buf,dv);
                if (!edit_state.nosnap) {
                    if(edit_state.snaptype != SNAP_VERT)
                    {
                        Vec3 snapped;
                        double gridsize;
                        if (edit_state.gridsize==0)
                            gridsize=.5;
                        else
                            gridsize=(double)(1<<(edit_state.gridsize-1));
                        if (edit_state.useObjectAxes) {
                            Vec3 buffer;
                            subVec3(dv,edit_state.objectMatrix[3],buffer);
                            mulVecMat3(buffer,edit_state.objectMatrixInverse,snapped);
                        } else
                            copyVec3(dv,snapped);
                        for (i=0;i<3;i++)
                            if ((i+1)%3==edit_state.plane || (!edit_state.singleaxis && i==edit_state.plane))
                                snapped[i]=((int)((snapped[i])/gridsize))*gridsize;
                        if (edit_state.useObjectAxes) {
                            Vec3 buffer;
                            mulVecMat3(snapped,edit_state.objectMatrix,buffer);
                            addVec3(buffer,edit_state.objectMatrix[3],dv);
                        } else
                            copyVec3(snapped,dv);
                    }
                    else
                    {
                        copyVec3(sel.pick_point,dv);
                    }
                }
            }

//            copyVec3(dv,mat[3]);
//            copyVec3(mat[3],dv);
            copyVec3(dv,mat[3]);
            if (edit_state.nudgeLeft || edit_state.nudgeRight || edit_state.nudgeUp || edit_state.nudgeDown)
                copyVec3(sel.parent->mat[3],mat[3]);
            if (edit_state.singleaxis && edit_state.plane==0)
            {
                Vec3 buf;
                Mat4 mbuf;
                edit_axis[0]=1;
                edit_axis[1]=0;
                copyMat4(mat,mbuf);
                copyVec3(mat[0],buf);
                copyVec3(mat[2],mat[0]);
                copyVec3(buf,mat[2]);
                copyVec3(mat[1],buf);
                copyVec3(mat[2],mat[1]);
                copyVec3(buf,mat[2]);
                edit_state.plane=2;
                showGrid(mat,edit_axis,old_edit_axis,edit_state.posrot);
                copyMat4(mbuf,mat);
                edit_axis[0]=0;
                edit_axis[1]=1;
                edit_state.plane=0;
            } else 
                showGrid(mat,edit_axis,old_edit_axis,edit_state.posrot);
            if (edit_state.nudgeLeft || edit_state.nudgeRight || edit_state.nudgeUp || edit_state.nudgeDown)
            {
                float amt;
                Vec3 tv;
                amt=(1<<edit_state.gridsize)/2.0;
                if (edit_state.plane==1) {
                    int buf;
                    buf=edit_state.nudgeLeft;
                    edit_state.nudgeLeft=edit_state.nudgeDown;
                    edit_state.nudgeDown=buf;
                    buf=edit_state.nudgeUp;
                    edit_state.nudgeUp=edit_state.nudgeRight;
                    edit_state.nudgeRight=buf;
                }
                if (!edit_state.sel && edit_state.nudgeLeft) {
                    scaleVec3(mat[0],amt,tv);
                    subVec3(sel.parent->mat[3],tv,sel.parent->mat[3]);
                }
                if (!edit_state.sel && edit_state.nudgeRight) {
                    scaleVec3(mat[0],amt,tv);
                    addVec3(sel.parent->mat[3],tv,sel.parent->mat[3]);
                }
                if (!edit_state.sel && edit_state.nudgeUp) {
                    scaleVec3(mat[2],amt,tv);
                    addVec3(sel.parent->mat[3],tv,sel.parent->mat[3]);
                }
                if (!edit_state.sel && edit_state.nudgeDown) {
                    scaleVec3(mat[2],amt,tv);
                    subVec3(sel.parent->mat[3],tv,sel.parent->mat[3]);
                }
                if (!edit_state.sel)
                    edit_state.nudgeLeft=edit_state.nudgeRight=edit_state.nudgeUp=edit_state.nudgeDown=0;
                return 0;
            }

            sel.parent->mat[3][0] = dv[0];
            sel.parent->mat[3][1] = dv[1];
            sel.parent->mat[3][2] = dv[2];
        }
    }
    copyVec3(sel.parent->mat[3],edit_stats.cursor_pos);
    if (edit_state.cameraPlacement)
        copyMat4(cam_info.cammat,sel.parent->mat);
    return !g_needRelock;
}

static void selDrawHelper(DefTracker * tracker,Mat4 mat)
{
    int i;
    Mat4 matx;
    if (tracker->frozen || (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE)))
        return;
    for (i=0;i<tracker->count;i++)
    {
        mulMat4(tracker->mat,mat,matx);
        selDrawHelper(&tracker->entries[i],matx);
    }
    if (tracker->count==0)
        groupDrawDefTracker(tracker->def,0,mat,0,0,1,0,-1);
}

#include "graphics/groupMiniTrackers.h"
#include "graphics/groupdrawinline.h"
void selDraw(void)
{
    Mat4        mat,matx,viewmat;
    int            i;

    if (!sel.parent)
        return;

    mulMat4(sel.parent->mat,sel.offsetnode->mat,matx);
    mulMat4(matx,sel.scalenode->mat,mat);
    gfxSetViewMat(cam_info.cammat,viewmat,0);
    mulMat4(viewmat,mat,matx);

    // this might be a bit too brutal, creating and destroying minitrackers as the selection changes would be faster
    group_draw.globFxLight = NULL;
    group_draw.globFxMiniTracker = NULL;
    group_draw.do_welding = 0;
    for(i=0;i<sel_ref_count;i++)
    {
        Mat4        matxx;
        DrawParams    draw = {0};
        GroupEnt    ent = {0};

        DefTracker    *tracker = &sel_refs[i];
        GroupDef    *def = tracker->def;

        draw.view_scale = 1.0f;
        draw.alpha = 1;

        groupDrawPushMiniTrackers();
        groupDrawBuildMiniTrackersForDef(def, 0);

        mulMat4(matx, tracker->mat, matxx);
        makeAGroupEnt(&ent, def, unitmat, draw.view_scale, 0);
        drawDefInternal(&ent, matxx, tracker, VIS_DRAWALL, &draw, 0);
        groupDrawPopMiniTrackers();
    }
}

static int pointInBox(Vec3 pt,Vec3 min,Vec3 max)
{
int        i;

    for(i=0;i<3;i++)
    {
        if (pt[i] > max[i])
            return 0;
        if (pt[i] < min[i])
            return 0;
    }
    return 1;
}

void editSelContents()
{
    int            i;
    DefTracker    *ref;
    Vec3        dv,min,max;

    edit_state.selcontents = 0;
    if (!sel_count)
        return;
    setVec3(min,8e16,8e16,8e16);
    setVec3(max,-8e16,-8e16,-8e16);
    for(i=0;i<sel_count;i++)
    {
        ref = groupRefFind(sel_list[i].id);
        mulVecMat4(ref->def->min,ref->mat,dv);
        MINVEC3(dv,min,min);
        mulVecMat4(ref->def->max,ref->mat,dv);
        MAXVEC3(dv,max,max);
    }
    for(i=0;i<group_info.ref_count;i++)
    {
        ref = groupRefFind(i);
        copyVec3(ref->mat[3],dv);
        if (pointInBox(dv,min,max))
            selAdd(ref,i,0,0);
    }
}

int cmpDepth(SelInfo *a,SelInfo *b)
{
    int i;
    for (i=0;i<a->depth && i<b->depth;i++)
        if (a->idxs[i]!=b->idxs[i])
            return b->idxs[i]-a->idxs[i];
    return a->depth-b->depth;
}

int editSelSort()
{
    int        i;
    int        idxs[100];

    for(i=0;i<sel_count;i++)
    {
        int j;
        sel_list[i].depth = trackerPack(sel_list[i].def_tracker,idxs,NULL,0);
        if (sel_list[i].depth)
            sel_list[i].ent_idx = idxs[sel_list[i].depth-1];
        else
            sel_list[i].ent_idx = 0;
        sel_list[i].idxs=_alloca((sel_list[i].depth+1)*sizeof(int));
        for (j=0;j<sel_list[i].depth;j++)
            sel_list[i].idxs[j]=idxs[j];
    }
    for (i=0;i<sel_count;i++)
    {
        int j;
        for (j=i+1;j<sel_count;j++)
        {
            int k;
            if (sel_list[i].id!=sel_list[j].id)
                continue;
            for (k=0;k<sel_list[i].depth && k<sel_list[j].depth;k++)
                if (sel_list[i].idxs[k]!=sel_list[j].idxs[k])
                    break;
            if (k==sel_list[i].depth || k==sel_list[j].depth)
                return 0;    //trying to 
        }
    }
    qsort(sel_list,sel_count,sizeof(sel_list[0]),(int (*) (const void *, const void *)) cmpDepth);
    return 1;
}

void closeSelGroups()
{
    int            i;
    DefTracker    *tracker_parents[ARRAY_SIZE(sel_list)];

    editSelSort();
    for(i=0;i<sel_count;i++)
        tracker_parents[i] = sel_list[i].def_tracker->parent;
    for(i=0;i<sel_count;i++)
        trackerCloseEdit(tracker_parents[i]);
}

void unSelect(int all)
{
    int        i;

    edit_stats.poly_recount = 1;
    edit_state.unsel = 0;
    edit_state.lightsize = 0;
    edit_state.soundsize = 0;
    edit_state.fogsize = 0;
    edit_state.beaconSize = 0;
    edit_state.editPropRadius = 0;
    if (sel.parent)
    {
        if( gfx_tree_root )
            gfxTreeDelete(sel.parent);
        gridFreeAll(&sel.grid);
        sel.parent = 0;
        sel.fake_ref.def = 0;
    }
    else if (sel_count)
    {
        for(i=sel_count-1;i>=0;i--)
        {
            selAdd(sel_list[i].def_tracker,sel_list[i].id,2,0);
            if (!all)
                break;
        }
    }
    if (all)
    {
        if (!edit_state.noServerUpdate && all == 2)
            editWaitingForServer(1, 1);
    }
    sound_editor_count = 0;
}

void trackerUnselect(DefTracker *tracker)
{
    int        i;

    for(i=0;i<sel_count;i++)
    {
        if (sel_list[i].def_tracker == tracker)
        {
            unSelect(2);
            return;
        }
    }
}


/*
static void editDrawNxShape(DefTracker* nxTracker)
{
    while(nxTracker && !nxTracker->novodex.actor)
    {
        nxTracker = nxTracker->parent;
    }
    
    if(nxTracker)
    {
        GroupDefNxInfo* novodex = &nxTracker->def->novodex;
        Vec3* mat = nxTracker->mat;
        int k;
        
        for(k = 0; k < novodex->shapes.size; k++)
        {
            GroupDefNxShapeInfo* shape = novodex->shapes.shape + k;
            int j;
            
            for(j = 0; j < shape->tri_count; j++){
                void drawLineCheckOrient(Mat3 orient_mat,Vec3 base_pos,Vec3 la,Vec3 lb,U8 c[4],F32 width);
                Vec3 v0[3];
                Vec3 v1[3];
                U8 c[4] = {255,255,255,255};
                
                copyVec3(shape->verts[shape->tris[j*3+0]], v0[0]);
                copyVec3(shape->verts[shape->tris[j*3+1]], v0[1]);
                copyVec3(shape->verts[shape->tris[j*3+2]], v0[2]);
                
                mulVecMat4(v0[0], mat, v1[0]);
                mulVecMat4(v0[1], mat, v1[1]);
                mulVecMat4(v0[2], mat, v1[2]);
                
                drawLineCheckOrient(NULL,NULL,v1[0],v1[1],c,3);
                drawLineCheckOrient(NULL,NULL,v1[1],v1[2],c,3);
                drawLineCheckOrient(NULL,NULL,v1[2],v1[0],c,3);
            }
        }
    }
}
*/

DefTracker *mouseOverSel(Vec3 start,Vec3 end,int lose_focus)
{
    CollInfo    coll;
    DefTracker        *tracker = 0;
    Mat4    matx;
    Mat4    temp;
    Model    *model;
    int            x,y;
    Vec3        dv;
    int            hit=0,hit_currsel=0,vsnap=0;
    int            mouseLocked = inpIsMouseLocked();
    int snapVertSelect = edit_state.snaptype == SNAP_VERT && !inpLevel(INP_LDRAG);

    mouseover_ctri_idx = -1;
    mouseover_tracker = 0;
    inpMousePos(&x,&y);


    gfxCursor3d(x,y,10000,start,end);
    if (edit_state.nosnap)
        sprintf(edit_stats.snap,"NoSnap");
    else if (edit_state.snaptype == SNAP_VERT)
    {
        sprintf(edit_stats.snap,"VertSnap");
        vsnap = COLL_NEARESTVERTEX;
    }
    else if (edit_state.snaptype == SNAP_CENTER)
        sprintf(edit_stats.snap,"CenterSnap");

    coll.node_callback = editCollNodeCallback;

    if (sel.parent)
    {
        Vec3    startx,endx;
        Mat4    mat,inv_mat;
        if (edit_state.snaptovert)
        {
            hit = (collGrid(0,start,end,&coll,0,g_edit_collide_flags | COLL_HITANYSURF | COLL_NEARESTVERTEX | COLL_DISTFROMSTART | COLL_NODECALLBACK));
            if (hit)
                copyVec3(coll.mat[3],sel.snap_pos);
        }
        mulMat4(sel.parent->mat,sel.offsetnode->mat,matx);
        mulMat4(matx,sel.scalenode->mat,mat);
        invertMat4Copy(mat,inv_mat);
        mulVecMat4(start,inv_mat,startx);
        mulVecMat4(end,inv_mat,endx);

        if(sel.parent && edit_state.last_selected_tracker && snapVertSelect)
        {
            copyMat4(edit_state.last_selected_tracker->mat, temp);
            //copyMat4(sel.parent->mat, temp);
            copyMat4(matx,edit_state.last_selected_tracker->mat);
        }
        
        if(!sel.parent || !edit_state.sel || !edit_state.quickPlacement)
        {
            if (collGrid(&sel.grid,startx,endx,&coll,0,g_edit_collide_flags | vsnap | COLL_HITANYSURF | COLL_DISTFROMSTART | COLL_NODECALLBACK))
            {
                hit_currsel = hit = 1;
                mulVecMat4(coll.mat[3],mat,dv);
                copyVec3(dv,coll.mat[3]);
            }
        }
    }
    edit_stats.geo[0] = 0;
    edit_stats.group[0] = 0;
    if ((!hit && !mouseLocked) || (hit && edit_state.snaptype == SNAP_VERT && inpLevel(INP_LBUTTON)))    //don't select ghost vertices if you're dragging with vert snap
    {
        int extra_flags = 0;
        
        if(edit_state.selectEditOnly){
            extra_flags |= COLL_EDITONLY;
        }
        else if(!edit_state.sel || !edit_state.quickPlacement){
            extra_flags |= COLL_HITANYSURF;
        }
        
        hit = (collGrid(0,start,end,&coll,0,g_edit_collide_flags | vsnap | COLL_DISTFROMSTART | COLL_NODECALLBACK | extra_flags));
    }
    if (lose_focus)
        sel.use_pick_point = 0;
    if (hit && !lose_focus)
    {
        copyVec3(coll.mat[3],sel.pick_point);
        sel.use_pick_point = 1;

        tracker = coll.node;
        model = tracker->def->model;
        mouseover_ctri_idx = coll.tri_idx;
        mouseover_tracker = tracker;
        
        if(edit_state.sel)
        {
            setVec3(quickPlacementRotateNormal, 0, 1, 0);
        }
        
        if(edit_state.quickPlacement && edit_state.sel)
        {
            copyVec3(coll.mat[1], quickPlacementRotateNormal);
        
            if(edit_state.quickPlacementObject[0]){
                Vec3 pos;
                sel.lib_load = 0;

                if(!sel.parent)
                    copyMat4(unitmat, sel.fake_ref.mat);
                    
                copyVec3(sel.pick_point, pos);
                if(!edit_state.nosnap)
                {
                    float gridsize = 0.5f * (1 << edit_state.gridsize);
                    float x, z;

                    x = pos[0];
                    
                    pos[0] -= fmod(x, gridsize);
                    
                    if(fabs(fmod(x, gridsize)) > gridsize / 2.0)
                    {
                        if(x < 0)
                            pos[0] -= gridsize;
                        else
                            pos[0] += gridsize;
                    }
                    
                    z = pos[2];
                    
                    pos[2] -= fmod(z, gridsize);
                    
                    if(fabs(fmod(z, gridsize)) > gridsize / 2.0)
                    {
                        if(z < 0)
                            pos[2] -= gridsize;
                        else
                            pos[2] += gridsize;
                    }

                    copyVec3(pos, sel.pick_point);
                }
                
                if(edit_state.quickOrient)
                {
                    Vec3 offset;
                    scaleVec3(coll.mat[1], edit_state.groundoffset, offset);
                    addVec3(offset, pos, pos);
                }
                else
                {
                    pos[1] += edit_state.groundoffset;
                }

                if(!sel.parent){
                    sel.fake_ref.def = groupDefFind(edit_state.quickPlacementObject);
                    if (sel.fake_ref.def)// && !sel.lib_load)
                    {
                        if(edit_state.quickOrient)
                        {
                            copyMat3(coll.mat, sel.fake_ref.mat);
                        }
                        else
                        {
                            copyMat3(edit_state.quickPlacementMat3, sel.fake_ref.mat);
                        }
                        unSelect(1);
                        selCopy(pos);
                        if(edit_state.quickPlacement == 2){
                            editCmdPaste();
                        }
                    }
                    else
                    {
                        editDefLoad(edit_state.quickPlacementObject);
                        sel.lib_load = 1;
                    }
                }else{
                    //Mat4 mat;
                    //printf("%1.2f, %1.2f, %1.2f\n", sel.parent->mat[2][0], sel.parent->mat[2][1], sel.parent->mat[2][2]);
                    //mulMat4(sel.parent->mat,sel.offsetnode->mat,mat);
                    //copyMat3(sel.parent->mat,sel.offsetnode->mat);
                    
                    if(edit_state.quickOrient)
                    {
                        copyMat3(coll.mat, sel.offsetnode->mat);
                    }

                    zeroVec3(sel.offsetnode->mat[3]);

                    //copyMat3(unitmat, sel.parent->mat);
                    copyVec3(pos, sel.parent->mat[3]);
                }
            }
            edit_state.sel = 0;
        }
        else if(edit_state.beaconSelect && edit_state.sel)
        {
            if(tracker)
            {
                if(!tracker->def->has_beacon || !tracker->def->has_sound)
                {
                    tracker = tracker->parent;
                    if(!tracker || !(tracker->def->has_beacon || tracker->def->has_sound))
                    {
                        tracker = NULL;
                    }
                    else
                    {
                        DefTracker* parent = tracker->parent;
                        for(;parent; parent = parent->parent)
                        {
                            parent->edit = 1;
                        }
                    }
                }
            }
        }
        else if(edit_state.openOnSelection && edit_state.sel)
        {
            if(tracker)
            {
                tracker = tracker->parent;
                if(!tracker)
                {
                    tracker = NULL;
                }
                else
                {
                    DefTracker* parent = tracker->parent;
                    for(;parent; parent = parent->parent)
                    {
                        parent->edit = 1;
                    }
                }
            }
        }
        else 
        {
            edit_state.tracker_under_mouse = tracker;
            
            for(;tracker && tracker->parent;tracker = tracker->parent)
            {
                if (tracker->parent->edit)
                    break;
            }
        }

        if ((!hit_currsel || snapVertSelect) && (tracker && tracker->def->name))
        {
        DefTracker    *ref;
        Mat4        mat;

            subVec3(coll.mat[3],start,dv);
            ref = trackerRoot(tracker);
#if 0
            xyprintf(4,31,"POS: %f %f %f",tracker->mat[3][0],tracker->mat[3][1],tracker->mat[3][2]);
            xyprintf(4,32,"NAME %s",model->name);
#endif
            strcpy(edit_stats.geo,model->name);

        //    copyVec3(coll.mat[3],edit_stats.cursor_pos);
            copyVec3(coll.mat[3],mat[3]);
            strcpy(edit_stats.group,tracker->def->name);
            if (edit_state.quickPlacement || edit_state.snaptype == SNAP_CENTER && !edit_state.nosnap)
                trackerGetMat(tracker,ref,mat);
            add3dCursor(mat[3], edit_state.quickPlacement ? mat : NULL);
            
            /*
            if(edit_state.quickPlacement && edit_state.tracker_under_mouse)
            {
                editDrawNxShape(edit_state.tracker_under_mouse);
            }
            */
            
//                xyprintf(11,11,"%s",tracker->def->name);
        }

        if(sel.parent && edit_state.quickPlacement && edit_state.quickRotate)
        {
            Mat4 matx;
            mulMat4(sel.parent->mat, sel.fake_ref.mat, matx);
            add3dCursor(sel.parent->mat[3], matx);
        }
    }
    if(sel.parent && edit_state.last_selected_tracker && snapVertSelect)
    {
        copyMat4(temp, edit_state.last_selected_tracker->mat);
    }
    return tracker;
}
void editRefSelect(DefTracker *tracker,U32 color)
{
    TrickNode    *trick;

    if (!tracker)
        return;
    trick = allocTrackerTrick(tracker);
    if (color && !edit_state.tintcolor1 && !edit_state.tintcolor2)
    {
        trick->flags2 |= TRICK2_WIREFRAME;
        trick->flags1 &= ~TRICK_HIDE;
        trick->trick_rgba[0] = (color >> 16) & 255;
        trick->trick_rgba[1] = (color >>  8) & 255;
        trick->trick_rgba[2] = (color >>  0) & 255;
        trick->trick_rgba[3] = 255;
    }
    else
        trick->flags2 &= ~TRICK2_WIREFRAME;
}

int getSelIndex(DefTracker* tracker)
{
    int i;
    
    for(i=0;i<sel_count;i++)
    {
        if (tracker == sel_list[i].def_tracker)
            return i;
    }
    
    return -1;
}

extern Menu * propertiesMenu;
bool updatePropertiesMenu = false;
char  propertiesList[64][256];
int propertiesListIndex;

int addPropertiesToList(StashElement e)
{
    PropertyEnt * prop=stashElementGetPointer(e);
    sprintf(propertiesList[propertiesListIndex++],"%s^3%s",prop->name_str,prop->value_str);
    return 1;
}

void propertyUpdateTrackers()
{
    DefTracker ** trackers=0;
    int i;
    for (i=0;i<sel_count;i++)
    {
        eaPush(&trackers,sel_list[i].def_tracker);
    }
    editUpdateTrackers(trackers);
    eaDestroy(&trackers);
}

void getAllPropertyValues2Sub(GroupDef * def,char * name,char *** values) {
    PropertyEnt * prop;
    int i;
    if (!def)
        return;
    stashFindPointer(def->properties,name,&prop);
    if (prop!=NULL) {
        int good=true;
        for (i=0;i<eaSize(values);i++)
            if (strcmp((*values)[i],prop->value_str)==0) {
                good=false;
                break;
            }
            if (good)
                eaPush(values,prop->value_str);
    }
    for (i=0;i<def->count;i++)
        getAllPropertyValues2Sub(def->entries[i].def,name,values);
}

void getAllPropertyValues2(char * name,char *** values) {
    int i;
    for (i=0;i<group_info.ref_count;i++)
        getAllPropertyValues2Sub(group_info.refs[i]->def,name,values);
}

// this function will not prompt the user in any way, so it
// will overwrite properties that were already there, be careful!
void addPropertiesToSelectedTrackers(char * name1,char * value1,...) {
    va_list args;
    int i;
    PropertyEnt ** props = NULL;
    char *name = name1;
    char *value = value1;

    if(!sel_count)
        return;
    if(!isNonLibSelect(0, WARN_CHECKPARENT)) // this causes instancing, warn them
        return;

    updatePropertiesMenu = true;

    va_start(args,value1);
    do {
        if(stricmp(name, "SharedFrom")==0)
        {
            winMsgAlert("You can not add a SharedFrom property directly. Use Import instead.");
        }
        else
        {
            PropertyEnt *prop=createPropertyEnt();
            sprintf(prop->name_str ,"%s",name );
            estrPrintCharString(&prop->value_str, value);
            eaPush(&props,prop);
        }
    } while( (name = va_arg(args,char *)) && (value = va_arg(args,char *)) );
    va_end(args);

    /* if there is going to be a warning it needs to go here, and the rest of the
    function should be called as a response to clicking OK in the dialog
    */

    for (i=0;i<sel_count;i++)
    {
        int j;
        GroupDef * def = sel_list[i].def_tracker->def;
        if(!def->properties){
            def->properties = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys);
            def->has_properties = 1;
        }
        for (j=0;j<eaSize(&props);j++)
        {
            PropertyEnt *prop;
            if(stashRemovePointer(def->properties,props[j]->name_str, &prop))
                destroyPropertyEnt(prop);
            stashAddPointer(def->properties, props[j]->name_str, props[j], 0);

            if (stricmp(props[j]->name_str, "VisTray")==0 ||
                stricmp(props[j]->name_str, "VisOutside")==0 ||
                stricmp(props[j]->name_str, "VisOutsideOnly")==0 ||
                stricmp(props[j]->name_str, "SoundOccluder")==0 ||
                stricmp(props[j]->name_str, "DoWelding")==0)
            {
                groupSetTrickFlags(def);
            }
        }
    }
    propertyUpdateTrackers();

    for (i=0;i<sel_count;i++)
    {
        int j;
        for (j=0;j<eaSize(&props);j++)
        {
            PropertyEnt *prop;
            DefTracker * tracker=sel_list[i].def_tracker;
            stashFindPointer( tracker->def->properties,props[j]->name_str, &prop );
            if (prop==NULL)
                continue;    //this means is was already deleted from a previous tracker with the same def
            stashRemovePointer(tracker->def->properties,props[j]->name_str, NULL);
            destroyPropertyEnt(prop);
            if (stashGetValidElementCount(tracker->def->properties)==0) {
                stashTableDestroy(tracker->def->properties);
                tracker->def->properties=0;
                tracker->def->has_properties=0;
            }
        }
    }

    eaDestroy(&props);
}

// WARNING: This is really only supposed to be used in conjuction with
// addPropertiesToSelectedTrackers, it does not update information on the
// server, so make sure to call the above function first so it gets updated
void removePropertiesFromSelectedTrackers(char * name1,...) {
    va_list args;
    int i;
    char * name;

    updatePropertiesMenu = true;

    for (i=0;i<sel_count;i++)
    {
        GroupDef * def = sel_list[i].def_tracker->def;
        if (def->properties) {
            stashRemovePointer(def->properties,name1, NULL);
            if (stashGetValidElementCount(def->properties)==0) {
                stashTableDestroy(def->properties);
                def->properties=0;
                def->has_properties=0;
            }
        }
    }
    va_start(args,name1);
    name=va_arg(args,char *);
    while (name!=NULL) {
        for (i=0;i<sel_count;i++)
        {
            GroupDef * def = sel_list[i].def_tracker->def;
            if (def->properties==NULL)
                continue;
            stashRemovePointer(def->properties,name,NULL);
            if (stashGetValidElementCount(def->properties)==0) {
                stashTableDestroy(def->properties);
                def->properties=0;
                def->has_properties=0;
            }
        }
        name=va_arg(args,char *);
    }
    va_end(args);

    /* if there is going to be a warning it needs to go here, and the rest of the
    function should be called as a response to clicking OK in the dialog
    */

}


/****************************************************************************************************/

typedef enum AddPropertyPropertyType {
    AP_ALL_PROPERTIES,
    AP_DOORS,
    AP_DESTROYABLE_OBJECTS,
} AddPropertyPropertyType;

typedef struct {
    int ID;
    AddPropertyPropertyType propertyType;
    int subMenuWidgets;
    char name[256];
    int whichName;
    char value[256];
    float valueFloat;
    int whichValue;        // for combo boxes and radio buttons
    int startingWidgetID;
} AddPropertyMenuInfo;

AddPropertyMenuInfo addPropInfo;

/******************************************************************************************/

char * addPropertyDoorMenuTypes[10] = {    "SpawnLocation",
                                        "LairType",
                                        "GotoSpawn",
                                        "OpenDoor",
                                        "LockedDoor",
                                        "KeyDoor",
                                        "SGBase",
                                        "RadiusOverride",
                                        ""                };

typedef struct {
    char ** spawnLocations;
} AddDoorPropertyMenuInfo;

AddDoorPropertyMenuInfo addDoorPropInfo;

void addDoorPropertyCopyValue(void* notUsed) {
    sprintf(addPropInfo.value,"%s",addDoorPropInfo.spawnLocations[addPropInfo.whichValue]);
}

void addPropertyClear(void* notUsed) {
    addPropInfo.whichValue=0;
    addPropInfo.whichName=0;
    addPropInfo.value[0]=0;
    addPropInfo.name[0]=0;
}

int addGotoSpawnShowComboBox(int widgetID) {
    return stricmp(addDoorPropInfo.spawnLocations[addPropInfo.whichValue],"Other")!=0;
}

int addGotoSpawnShowTextEntry(int widgetID) {
    return stricmp(addDoorPropInfo.spawnLocations[addPropInfo.whichValue],"Other")==0;
}

void addDoorProperties() {
    int i=0;
    editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
        editorUIAddComboBoxFromArray(addPropInfo.ID,&addPropInfo.whichName,"Door Property",NULL,addPropertyDoorMenuTypes);
        while (addPropertyDoorMenuTypes[i][0]) {
            editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
                if (stricmp(addPropertyDoorMenuTypes[i],"GotoSpawn")==0) {
                    addPropInfo.whichValue=0;
                    editorUIAddSubWidgets(addPropInfo.ID,addGotoSpawnShowComboBox,0,0);
                        eaDestroy(&addDoorPropInfo.spawnLocations);
                        addDoorPropInfo.spawnLocations=NULL;
                        getAllPropertyValues2("SpawnLocation",&addDoorPropInfo.spawnLocations);
                        eaPush(&addDoorPropInfo.spawnLocations,"Other");
                        editorUIAddComboBoxFromEArray(addPropInfo.ID,&addPropInfo.whichValue,"Value",addDoorPropertyCopyValue,addDoorPropInfo.spawnLocations);
                    editorUIEndSubWidgets(addPropInfo.ID);
                    editorUIAddSubWidgets(addPropInfo.ID,addGotoSpawnShowTextEntry,0,0);
                        editorUIAddTextEntry(addPropInfo.ID,addPropInfo.value,256,NULL,"Value");
                    editorUIEndSubWidgets(addPropInfo.ID);
                } else
                    editorUIAddTextEntry(addPropInfo.ID,addPropInfo.value,256,NULL,"Value");
            editorUIEndSubWidgets(addPropInfo.ID);
            i++;
        }
    editorUIEndSubWidgets(addPropInfo.ID);
}

void addDoorPropertiesToTrackers() {
    addPropertiesToSelectedTrackers(addPropertyDoorMenuTypes[addPropInfo.whichName],addPropInfo.value,NULL);
}

/******************************************************************************************/

typedef struct {
    float min,max,health;
    int repairing;
    int state;
} AddDestroyablePropertiesMenuInfo;

AddDestroyablePropertiesMenuInfo addDestroyablePropInfo;

void addDestroyablePropertiesConstrainMinHealth(void* notUsed) {
    if (addDestroyablePropInfo.min>addDestroyablePropInfo.max)
        addDestroyablePropInfo.min=addDestroyablePropInfo.max;
}

void addDestroyablePropertiesConstrainMaxHealth(void* notUsed) {
    if (addDestroyablePropInfo.max<addDestroyablePropInfo.min)
        addDestroyablePropInfo.max=addDestroyablePropInfo.min;
}

void addDestroyableProperties() {
    ZeroStruct(&addDestroyablePropInfo);
    editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
        editorUIAddRadioButtons(addPropInfo.ID,0,&addDestroyablePropInfo.state,NULL,"Not Destroyable","Parent","Child",NULL);
        editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
        editorUIEndSubWidgets(addPropInfo.ID);
        editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
            editorUIAddSlider(addPropInfo.ID,&addDestroyablePropInfo.health,0,100,1,NULL,"Health");
        editorUIEndSubWidgets(addPropInfo.ID);
        editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
            editorUIAddSlider(addPropInfo.ID,&addDestroyablePropInfo.min,0,100,1,addDestroyablePropertiesConstrainMaxHealth,"Min Health");
            editorUIAddSlider(addPropInfo.ID,&addDestroyablePropInfo.max,0,100,1,addDestroyablePropertiesConstrainMinHealth,"Max Health");
            editorUIAddCheckBox(addPropInfo.ID,&addDestroyablePropInfo.repairing,NULL,"Repairing");
        editorUIEndSubWidgets(addPropInfo.ID);
    editorUIEndSubWidgets(addPropInfo.ID);
    if (stashFindElement(sel_list[0].def_tracker->def->properties,"Health",NULL)) {
        PropertyEnt * prop;
        stashFindPointer(sel_list[0].def_tracker->def->properties,"Health",&prop);
        addDestroyablePropInfo.health=prop?atoi(prop->value_str):0;
        addDestroyablePropInfo.state=1;
    } else
    if (stashFindElement(sel_list[0].def_tracker->def->properties,"MinHealth",NULL) ||
    stashFindElement(sel_list[0].def_tracker->def->properties,"MaxHealth",NULL) ||
    stashFindElement(sel_list[0].def_tracker->def->properties,"Repairing",NULL)) {
        PropertyEnt * prop;
        stashFindPointer(sel_list[0].def_tracker->def->properties,"MinHealth",&prop);
        addDestroyablePropInfo.min=prop?atoi(prop->value_str):0;
        stashFindPointer(sel_list[0].def_tracker->def->properties,"MaxHealth",&prop);
        addDestroyablePropInfo.max=prop?atoi(prop->value_str):0;
        stashFindPointer(sel_list[0].def_tracker->def->properties,"Repairing",&prop);
        addDestroyablePropInfo.repairing=prop?atoi(prop->value_str):0;
        addDestroyablePropInfo.state=2;
    } else
        addDestroyablePropInfo.state=0;
}

void addDestroyablePropertiesToTrackers() {
    removePropertiesFromSelectedTrackers("Health","MinHealth","MaxHealth","Repairing",NULL);
    if (addDestroyablePropInfo.state==0) {    // do nothing, leave those properties removed

    } else
    if (addDestroyablePropInfo.state==1) {    // parent object
        char health[32];
        sprintf(health,"%d",(int)(addDestroyablePropInfo.health+.5));
        addPropertiesToSelectedTrackers("Health",health,NULL);
    } else
    if (addDestroyablePropInfo.state==2) {    // child object
        char min[32];
        char max[32];
        sprintf(min,"%d",(int)(addDestroyablePropInfo.min+.5));
        sprintf(max,"%d",(int)(addDestroyablePropInfo.max+.5));
        addPropertiesToSelectedTrackers(    "MaxHealth",max,
                                            "MinHealth",min,
                                            "Repairing",(addDestroyablePropInfo.repairing?"1":"0"),NULL);
    }
}

/******************************************************************************************/

void addPropertyDestroyWindow() {
    editorUIDestroyWindow(addPropInfo.ID);
    addPropInfo.ID=0;
    editWaitingForServer(0, 0);
}

//quotes in properties are bad, mkay?
static void clean_copy(char *dest, char *src) {
    while(*src)    {
        if (*src != '"')
        {
            *dest = *src;
            dest++;
        }
        src++;
    }
    *dest = 0;
}

static void clean_value(void *str) {
    clean_copy(addPropInfo.value,addPropInfo.value);
    clean_copy(addPropInfo.name,addPropInfo.name);
}


void addPropertyValueChange(void* notUsed) {
    sprintf(addPropInfo.value,"%s",g_propertyDefList.list[addPropInfo.whichName]->texts[addPropInfo.whichValue]);
}

void addPropertySliderValueChanged(void* notUsed) {
    addPropInfo.valueFloat=atof(addPropInfo.value);
}

void addPropertySliderSliderChanged(void* widgetIndex) {
    int index = (int)widgetIndex;
    if (g_propertyDefList.list[index]->type==PROPTYPE_SLIDER)
        sprintf(addPropInfo.value,"%f",addPropInfo.valueFloat);
    else
        sprintf(addPropInfo.value,"%d",(int)(addPropInfo.valueFloat+.5));
}

void addPropertyPropertiesChanged(void* notUsed) {
    addPropInfo.value[0]=0;
}

void addPropertiesToTrackers() {
    if (addPropInfo.whichName<eaSize(&g_propertyDefList.list)-1)
        sprintf(addPropInfo.name,"%s",g_propertyDefList.list[addPropInfo.whichName]->name);
    if (stricmp(addPropInfo.name," ")==0)
        return;
    addPropertiesToSelectedTrackers(addPropInfo.name,(addPropInfo.value[0])?addPropInfo.value:"0",NULL);
}

void addPropertyToTrackers(char * button) {
    if (addPropInfo.propertyType==0) {    // All Properties
        addPropertiesToTrackers();
    } else
    if (addPropInfo.propertyType==1) {    // Door Properties
        addDoorPropertiesToTrackers();
    } else
    if (addPropInfo.propertyType==2) {    // Destroyable Properties
        addDestroyablePropertiesToTrackers();
    }
    if (stricmp(button,"ok")==0)
        editorUIDestroyWindow(addPropInfo.ID);
}

int addPropertyNormalProperty(int widgetID) {
    if (addPropInfo.whichName==eaSize(&g_propertyDefList.list)-1)
        return 0;
    return 1;
}


int addPropertyOtherProperty(int widgetID) {
    if (addPropertyNormalProperty(widgetID))
        return 0;
    return 1;
}

void addPropertyCreateMenu(MenuEntry * me,ClickInfo * ci) {
    static int temp=1;
    char ** list=0;
    int i=0;
    edit_state.addProperty = 0;
    if (addPropInfo.ID)
        return;
    if (sel_count==0)
        return;
    editWaitingForServer(1, 0);
    ZeroStruct(&addPropInfo);
    for (i=0;i<eaSize(&g_propertyDefList.list);i++)
        eaPush(&list,g_propertyDefList.list[i]->name);
    editorUIDestroyWindow(addPropInfo.ID);
    addPropInfo.ID=editorUICreateWindow("Add Property");
    editorUISetCloseCallback(addPropInfo.ID,addPropertyDestroyWindow);
    editorUIAddComboBox(addPropInfo.ID,(int *)&addPropInfo.propertyType,"Properties",addPropertyClear,"All Properties","Doors","Destroyable Objects",NULL);
        editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
            editorUIAddSubWidgets(addPropInfo.ID,addPropertyNormalProperty,0,0);
                editorUIAddComboBoxFromEArray(addPropInfo.ID,&addPropInfo.whichName,"Name",addPropertyPropertiesChanged,list);

                    for (i=0;i<eaSize(&g_propertyDefList.list);i++) {
                        int temp=editorUIAddSubWidgets(addPropInfo.ID,editorUIShowByParentSelection,0,0);
                        if (i==0)
                            addPropInfo.startingWidgetID=temp;
                        if (g_propertyDefList.list[i]->type==PROPTYPE_TEXTBOX)
                            editorUIAddTextEntry(addPropInfo.ID,addPropInfo.value,256,clean_value,"Value");
                        else
                        if (g_propertyDefList.list[i]->type==PROPTYPE_COMBOBOX)
                            editorUIAddComboBoxFromEArray(addPropInfo.ID,&addPropInfo.whichValue,"Value",addPropertyValueChange,g_propertyDefList.list[i]->texts);
                        else
                        if (g_propertyDefList.list[i]->type==PROPTYPE_SLIDER) {
                            int sliderID;
                            editorUIAddTextEntry(addPropInfo.ID,addPropInfo.value,256,addPropertySliderValueChanged,"Value");
                            sliderID = editorUIAddSlider(addPropInfo.ID,&addPropInfo.valueFloat,g_propertyDefList.list[i]->min,g_propertyDefList.list[i]->max,0,addPropertySliderSliderChanged,"");
                            editorUISetWidgetCallbackParam(sliderID, (void*)i);
                        } else
                        if (g_propertyDefList.list[i]->type==PROPTYPE_INTEGERSLIDER) {
                            int sliderID;
                            editorUIAddTextEntry(addPropInfo.ID,addPropInfo.value,256,addPropertySliderValueChanged,"Value");
                            sliderID = editorUIAddSlider(addPropInfo.ID,&addPropInfo.valueFloat,g_propertyDefList.list[i]->min,g_propertyDefList.list[i]->max,1,addPropertySliderSliderChanged,"");
                            editorUISetWidgetCallbackParam(sliderID, (void*)i);
                        } else
                        if (g_propertyDefList.list[i]->type==PROPTYPE_RADIOBUTTONS)
                            editorUIAddRadioButtonsFromEArray(addPropInfo.ID,&addPropInfo.whichValue,addPropertyValueChange,g_propertyDefList.list[i]->texts);

                        editorUIEndSubWidgets(addPropInfo.ID);
                    }


                editorUIEndSubWidgets(addPropInfo.ID);
                editorUIAddSubWidgets(addPropInfo.ID,addPropertyOtherProperty,0,0);
                    editorUIAddTextEntry(addPropInfo.ID,addPropInfo.name,256,clean_value,"Name");
                    editorUIAddTextEntry(addPropInfo.ID,addPropInfo.value,256,clean_value,"Value");
                editorUIEndSubWidgets(addPropInfo.ID);
            editorUIEndSubWidgets(addPropInfo.ID);

        addDoorProperties();

        addDestroyableProperties();

        editorUIAddButtonRow(addPropInfo.ID,NULL,"Ok",addPropertyToTrackers,"Add Another",addPropertyToTrackers,"Cancel",addPropertyDestroyWindow,NULL);
    eaDestroy(&list);
}

/****************************************************************************************************/

void addProperty2(MenuEntry * me,ClickInfo * ci){
    GroupDef* def;
    PropertyEnt* prop;
    char buffer[1024];
    char buffer2[1024];
    char another = true;
    int i;
    PropertyEnt ** props=0;

    edit_state.addProperty = 0;

    if (me!=NULL && ci->x/me->theMenu->lv->columnWidth>=2)
        return;

    if(!sel_count)
        return;

    // Properties can only be added to groups, not to library pieces
    if (!isNonLibSelect(0,WARN_CHECKPARENT))
        return;

    updatePropertiesMenu = true;

    while (another) {
        // Prompt the user for the new property name.
        {
            PropertyDialogParams ptd;
            char * cursor;
            extern LRESULT CALLBACK editPropertyDialog(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);
            sprintf(buffer, "NewPropertyName");
            sprintf(buffer2, "0");
            ptd.property = buffer;
            ptd.value = buffer2;
            another = DialogBoxParam(glob_hinstance, MAKEINTRESOURCE(IDD_NEWPROP), hwnd, editPropertyDialog, (LPARAM)&ptd);
            inpClear();
            // check for cancel
            if (another == 2) {
                break;
            }

            cursor = strrchr(buffer, '\\');
            if(cursor){
                cursor++;
                strcpy(buffer, cursor);
            }
        }

        // Verify that the property does not already exist.
        {
            StashElement element;
            int count=0;
            for (i=0;i<sel_count;i++)
            {
                // Make sure the properties table exists.
                def = sel_list[i].def_tracker->def;
                if(!def->properties){
                    def->properties = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys);
                    def->has_properties = 1;
                }
                // Try to find the property with the new name
                stashFindElement(def->properties, buffer, &element);
                if(element)
                    count++;
            }
            if (count>0)
            {
                char msg[256];
                if (sel_count>1)
                    sprintf(msg,"%d group%s already have this property.  If you continue it will erase the old value from %s, are you sure you want to do this?",count,(count>1)?"s":"",(count>1)?"that group":"those groups");
                else
                    sprintf(msg,"This group already has this property.  If you continue it will erase the old value, are you sure you want to do this?");
                if (!winMsgYesNo(msg))
                    return;
                for (i=0;i<sel_count;i++)
                {
                    DefTracker * tracker=sel_list[i].def_tracker;
                    stashFindPointer( tracker->def->properties,buffer, &prop );
                    if (prop==NULL)
                        continue;
                    stashRemovePointer(tracker->def->properties,buffer, NULL);
                    destroyPropertyEnt(prop);
                }
            }
        }
        for (i=0;i<sel_count;i++)
        {
            def = sel_list[i].def_tracker->def;
            prop = createPropertyEnt();
            strcpy(prop->name_str, buffer);    // Give the property the user given name
            estrPrintCharString(&prop->value_str, buffer2);    // Give the property a default value of "0".

            stashAddPointer(def->properties, prop->name_str, prop, false);

            if (stricmp(prop->name_str, "VisTray")==0 ||
                stricmp(prop->name_str, "VisOutside")==0 ||
                stricmp(prop->name_str, "VisOutsideOnly")==0 ||
                stricmp(prop->name_str, "SoundOccluder")==0 ||
                stricmp(prop->name_str, "DoWelding")==0)
                groupSetTrickFlags(def);

            eaPush(&props,prop);
        }
        propertyUpdateTrackers();
    }
    for (i=0;i<sel_count;i++)
    {
        int j;
        for (j=0;j<eaSize(&props);j++)
        {
            DefTracker * tracker=sel_list[i].def_tracker;
            stashFindPointer( tracker->def->properties,props[j]->value_str, &prop );
            if (prop==NULL)
                continue;    //this means is was already deleted from a previous tracker with the same def
            stashRemovePointer(tracker->def->properties,buffer, NULL);
            destroyPropertyEnt(prop);
            if (stashGetValidElementCount(tracker->def->properties)==0) {
                stashTableDestroy(tracker->def->properties);
                tracker->def->properties=0;
                tracker->def->has_properties=0;
            }
        }
    }
}

void propertiesMenuCallback(MenuEntry * me,ClickInfo * ci)
{
    int column=ci->x/me->theMenu->lv->columnWidth;
    char *buf = NULL;
    char *oldbuf = NULL;
    int i;
    if (column<3)
    {

    }
    else
    if (column<6)
    {
        const char *caretPtr;
        PropertyEnt * prop;
        DefTracker * tracker;
        char propertyName[TEXT_DIALOG_MAX_STRLEN];
        strncpy(propertyName, me->name, TEXT_DIALOG_MAX_STRLEN - 1);
        propertyName[TEXT_DIALOG_MAX_STRLEN - 1] = '\0';
        if (strstr(me->name, "^"))
            estrPrintCharString(&buf, strstr(me->name, "^") + 2);
        if (caretPtr = strstr(buf, "^"))
            estrRemove(&buf, caretPtr - buf, estrLength(&buf) - (caretPtr - buf));
        if (strchr(propertyName,'^')!=0)
            *strchr(propertyName,'^')=0;

        tracker=sel_list[0].def_tracker;
        assert(stashFindPointer(tracker->def->properties, propertyName, &prop) && prop);

        if(stricmp(propertyName, "SharedFrom") == 0)
        {
            char msg[512];
            sprintf(msg,    "You cannot change the SharedFrom property, you can only remove it.\n"
                            "If you do, the layer will be saved to the current map's directory instead of:\n"
                            "%s\n"
                            "\n"
                            "!! THIS CANNOT BE UNDONE !!\n"
                            "Do you want to remove the SharedFrom property?",
                            prop->value_str);
            if(winMsgYesNo(msg))
            {
                stashRemovePointer(tracker->def->properties, propertyName, NULL);
                destroyPropertyEnt(prop);
                if(!stashGetValidElementCount(tracker->def->properties))
                    tracker->def->has_properties = 0;
                propertyUpdateTrackers();
            }
            return;
        }

        if (!winGetEString("Enter new value.", &buf) || !buf[0])
            return;

        estrPrintEString(&oldbuf, prop->value_str);

        for (i=0;i<sel_count;i++)
        {
            tracker=sel_list[i].def_tracker;
            stashFindPointer(tracker->def->properties, propertyName, &prop);
            assert(prop);
            estrPrintEString(&prop->value_str, buf);
            estrRemoveDoubleQuotes(&prop->value_str);
        }
        propertyUpdateTrackers();
        
        // ARM NOTE: What does this do?  I don't get it...
        for (i=0;i<sel_count;i++)
        {
            tracker=sel_list[i].def_tracker;
            stashFindPointer(tracker->def->properties, propertyName, &prop);
            assert(prop);
            estrPrintEString(&prop->value_str, oldbuf);
        }
    }
    else
    if (column>=6)
    {
        PropertyEnt * prop;
        DefTracker * tracker;
        char propertyName[TEXT_DIALOG_MAX_STRLEN];
        char *propertyValue = NULL;
        strncpy(propertyName, me->name, TEXT_DIALOG_MAX_STRLEN - 1);
        propertyName[TEXT_DIALOG_MAX_STRLEN - 1] = '\0';
        if (strchr(propertyName,'^')!=0)
            *strchr(propertyName,'^')=0;
        estrPrintf(&buf, "Remove the property '%s' from %d groups?", propertyName, sel_count);
        if (winMsgYesNo(buf))
        {
            for (i=0;i<sel_count;i++)
            {
                tracker = sel_list[i].def_tracker;
                stashFindPointer(tracker->def->properties, propertyName, &prop);
                if (prop == NULL)
                    continue;    //this means is was already deleted from a previous tracker with the same def
                estrPrintEString(&propertyValue, prop->value_str);
                stashRemovePointer(tracker->def->properties, propertyName, NULL);
                destroyPropertyEnt(prop);
                if (stashGetValidElementCount(tracker->def->properties)==0)
                    tracker->def->has_properties = 0;
            }
            propertyUpdateTrackers();
            for (i=0;i<sel_count;i++)
            {
                GroupDef * def = sel_list[i].def_tracker->def;
                prop = createPropertyEnt();
                strcpy(prop->name_str, propertyName);    // Give the property the user given name
                estrPrintCharString(&prop->value_str, propertyValue);    // Give the property a default value of "0".
                stashAddPointer(def->properties, prop->name_str, prop, false);
            }
        }
    }
}

void propertiesMenuCreate()
{
    int i;
    StashTable properties;
    char propertiesListFinal[64][TEXT_DIALOG_MAX_STRLEN];
    int propertiesListIndexFinal;
    static int x,y,width,height;
    static int sticky=1;        //keep the menu around for a tick or two even if sel_count goes to zero
//    PERFINFO_AUTO_START("propertiesCreateMenu",1);
    if (sel_count==0)
    {
        if (propertiesMenu!=NULL && sticky<=0)
        {
            x=propertiesMenu->lv->x;
            y=propertiesMenu->lv->y;
            width=propertiesMenu->lv->width;
            height=propertiesMenu->lv->height;
            destroyMenu(propertiesMenu);
            propertiesMenu=NULL;
        }
        sticky--;
//        PERFINFO_AUTO_STOP();
        return;
    }
    sticky=1;
    if (height==0)
    {
        x=15;
        y=-45;
        width=430;
        height=100;
    }
    propertiesListIndex=0;
    propertiesListIndexFinal=0;
    properties=sel_list[0].def_tracker->def->properties;
    stashForEachElement(properties,addPropertiesToList);
    for (i=0;i<propertiesListIndex;i++)
        strcpy(propertiesListFinal[i],propertiesList[i]);
    propertiesListIndexFinal=propertiesListIndex;
    for (i=1;i<sel_count;i++)
    {
        int j;
        propertiesListIndex=0;
        properties=sel_list[i].def_tracker->def->properties;
        stashForEachElement(properties,addPropertiesToList);
        for (j=0;j<propertiesListIndexFinal;j++)
        {
            int k;
            for (k=0;k<propertiesListIndex;k++)
            {
                if (strcmp(propertiesListFinal[j],propertiesList[k])==0)
                    break;
            }
            if (k==propertiesListIndex)
            {
                strcpy(propertiesListFinal[j],propertiesListFinal[propertiesListIndexFinal-1]);
                propertiesListIndexFinal--;
                j--;
            }
        }
        if (propertiesListIndexFinal==0)
            break;
    }

    if (updatePropertiesMenu)
    {
        if (propertiesMenu != NULL)
        {
            x = propertiesMenu->lv->x;
            y = propertiesMenu->lv->y;
            width = propertiesMenu->lv->width;
            height = propertiesMenu->lv->height;
            destroyMenu(propertiesMenu);
            propertiesMenu = NULL;
        }
        updatePropertiesMenu = false;
    }

    if (propertiesMenu==NULL && sel_count>0)
    {
        propertiesMenu=newMenu(x,y,width,height,"Properties");
        propertiesMenu->lv->theCorner=EDIT_LIST_VIEW_LOWERLEFT;
        propertiesMenu->lv->theWindowCorner=EDIT_LIST_VIEW_LOWERLEFT;
        addEntryToMenu(propertiesMenu,"Add property^2",addPropertyCreateMenu,NULL);
        for (i=0;i<propertiesListIndexFinal;i++)
        {
            MenuEntry * me;
            char smallBuffer[100];
            backSlashes(propertiesListFinal[i]); // this part is important, make sure addEntryToMenu doesn't do any recursive shenanigans

            // ARM NOTE:  I'm removing this line because it's truncating something 
            //   I don't want it to, but there's a decent chance this may be needed for some
            //   obscure reason...
            //if (strlen(propertiesListFinal[i])>90)
            //    propertiesListFinal[i][90]=0;

            snprintf(smallBuffer, 91, "%s", propertiesListFinal[i]);
            sprintf(smallBuffer + 91, "^6Remove");
            smallBuffer[99] = '\0';
            me=addEntryToMenu(propertiesMenu,smallBuffer,propertiesMenuCallback,NULL);
            estrPrintf(&me->name, "%s^6Remove", propertiesListFinal[i]);
        }    
        propertiesMenu->root->opened=1;
        propertiesMenu->lv->colorColumns=1;
        propertiesMenu->lv->columnWidth=60;
    }
    estrPrintf(&propertiesMenu->root->name, "Properties (%d group%s)", sel_count, (sel_count > 1 ? "s" : ""));
//    PERFINFO_AUTO_STOP();
}

void selAdd(DefTracker *tracker,int group_id,int toggle, int selectionSrc)
{
    int        i,cidx=-1;
    U32        color;
    int selectLayer = 0;
    static U32        colors[] = {
        0x80ff00, 0x8000ff, 0x0080ff,
        0xff8000, 0xff0080, 0x00ff80,
        0xffff80, 0xff80ff, 0x80ffff,
        0xff0000, 0x00ff00, 0x0000ff,
        0xffff00, 0xff00ff, 0x00ffff,};
    U8        used[ARRAY_SIZE(colors)];
 
    if (tracker) {
        DefTracker * temp=tracker;
        while (temp) {
            if (temp->frozen) return;
            temp=temp->parent;
        }
    }

    updatePropertiesMenu = true;

    //go ahead and set it as unselected, if it is supposed to be selected it
    //will get reflagged later
    tracker->selected=0;

    /////Layers : If you are selecting a Layer, unselect everything else first.  
    if( tracker && tracker->def && groupDefFindPropertyValue( tracker->def, "Layer") ) 
    {
        selectLayer = 1;

        //Don't know why all this
        edit_stats.poly_recount = 1;
        edit_state.unsel = 0;
        edit_state.lightsize = 0;
        edit_state.soundsize = 0; 
        edit_state.fogsize = 0;
        edit_state.beaconSize = 0;
        edit_state.editPropRadius = 0;
        sound_editor_count = 0;
        //If you are unselecting a layer, don't do anything special
        for( i=0 ; i<sel_count ; i++ )
        {
            if( sel_list[i].def_tracker == tracker )
                selectLayer = 0;
        }
        if( selectLayer )
        {
            for(i=sel_count-1;i>=0;i--)
            {
                DefTracker * t;
                t = sel_list[i].def_tracker;
                sel_list[i].id = 0; 
                t->inherit = 0;
                memmove(sel_list+i,sel_list+i+1,(sel_count-i) * sizeof(sel_list[0]));
                sel_count--;
                editRefSelect(t,0);
            }
        }
    
    }
    //If you are selecting anything else, and you have a layer selected, unselect it.
    else 
    {
        for(i=sel_count-1;i>=0;i--)
        {
            if( groupDefFindPropertyValue( sel_list[i].def_tracker->def, "Layer") )
            {
                DefTracker * t1;
                t1 = sel_list[i].def_tracker;
                sel_list[i].id = 0;
                t1->inherit = 0;
                memmove(sel_list+i,sel_list+i+1,(sel_count-i) * sizeof(sel_list[0]));
                sel_count--;
                editRefSelect(t1,0);
            }
        }
    }

    //Set this layer as the active layer
    //ONly when you explicitly select this in the editor should you make this your active layer
    if( selectionSrc == EDITOR_LIST && selectLayer )      
    {
        edit_state.setactivelayer = 1;
        //editCmdSetActiveLayer();
    }

    // End Layers         ///////////////////////////////////////////////////////

    edit_stats.poly_recount = 1;
    if (toggle != 2 && tracker && tracker->edit)
    {
        trackerCloseEdit(tracker);
        unSelect(1);
    }
    memset(used,0,sizeof(used));
    for(i=0;i<sel_count;i++)
    {
        if (tracker == sel_list[i].def_tracker)
        {
            if (toggle)
            {
                sel_list[i].id = 0;
                tracker->inherit = 0;
                memmove(sel_list+i,sel_list+i+1,(sel_count-i) * sizeof(sel_list[0]));
                sel_count--;
                editRefSelect(tracker,0);
            }
            return;
        }
        else
        {
            // Check if this is a parent of tracker somewhere.
            DefTracker* parent = tracker->parent;
            
            for(;parent;parent = parent->parent)
            {
                if(parent == sel_list[i].def_tracker)
                {
                    sel_list[i].id = 0;
                    parent->inherit = 0;
                    memmove(sel_list+i,sel_list+i+1,(sel_count-i) * sizeof(sel_list[0]));
                    sel_count--;
                    editRefSelect(parent,0);
                    break;
                }
            }
            
            if(parent)
                break;
        }
        if (tracker->parent && tracker->parent == sel_list[i].def_tracker->parent)
            cidx = sel_list[i].color_idx;
        if (sel_list[i].def_tracker->parent)
            used[sel_list[i].color_idx] = 1;
    }
    if (cidx < 0)
    {
        for(i=0;i<ARRAY_SIZE(colors);i++)
        {
            if (!used[i])
                break;
        }
        if (i >= ARRAY_SIZE(colors))
            i = randInt(ARRAY_SIZE(colors));
        cidx = i;
    }

    if (sel_count >= ARRAY_SIZE(sel_list))
        return;
    if (tracker)
        tracker->inherit = 1;
    sel_list[sel_count].id = group_id;
    sel_list[sel_count].color_idx = cidx;
    {
    extern void goterr();

        if (!tracker)
            goterr();
    }
    sel_list[sel_count].def_tracker = tracker;
    sel_count++;
    tracker->selected=1;

    color = colors[cidx];
    if (tracker && !tracker->parent)
        color = 0xffff00;
    if (sel_list[sel_count-1].pick_pivot)
        color = 0xff0000;
    editRefSelect(tracker,color);

}

void clearSetParentAndActiveLayer()
{
    sel.activeLayer            = 0;
    sel.activeLayer_refid    = 0;
    sel.group_parent_def    = 0;
    sel.group_parent_refid    = 0;
}


//////////////////////////////////////////////////////////////////////////
// Status Window

static int status_ui_id=-1;
static char status_text[1024]="";
static float status_percent = 0;

static void getEditStatusWindowText(char *buffer)
{
    strcpy(buffer, status_text);
}

static float getEditStatusPercenDone(void)
{
    return status_percent;
}

static void updateStatusBar(void* notUsed)
{
    status_percent += 0.01f * TIMESTEP;
    if (status_percent > 1.f)
        status_percent = 0;

    if (status_ui_id >= 0)
    {
        int screen_width, screen_height;
        float window_width, window_height;
        windowClientSize(&screen_width, &screen_height);
        editorUIGetSize(status_ui_id, &window_width, &window_height);
        editorUISetPosition(status_ui_id, (screen_width-window_width)*0.5f, (screen_height-window_height)*0.5f);
    }
}

void setEditStatusWindowPercent(float percentDone)
{
    status_percent = percentDone;
    if (status_ui_id >= 0)
        editorUICenterWindow(status_ui_id);
}

void setEditStatusWindowText(char *text)
{
    if (text)
        strncpy(status_text, text, ARRAY_SIZE(status_text)-1);
    else
        status_text[0] = 0;
}

void showEditStatusWindow(char *title, int auto_bar)
{
    if (status_ui_id < 0)
    {
        status_ui_id = editorUICreateWindow(title);
        if (status_ui_id < 0)
            return;

        status_percent = 0;
        editorUISetModal(status_ui_id, 1);
        editorUIAddDynamicText(status_ui_id, getEditStatusWindowText);
        editorUIAddProgressBar(status_ui_id, getEditStatusPercenDone);
        if (auto_bar)
            editorUIAddDrawCallback(status_ui_id, updateStatusBar);

        setEditStatusWindowText("");
    }

    status_percent = 0;
    setEditStatusWindowText("");
}

void closeEditStatusWindow(void)
{
    editorUIDestroyWindow(status_ui_id);
    status_ui_id = -1;
}


//////////////////////////////////////////////////////////////////////////
// waiting for server

void editWaitingForServer(int waiting, int showStatus)
{
    sel.waitingforserver = waiting;
    if (showStatus)
    {
        if (waiting)
        {
            showEditStatusWindow("Please Wait", 1);
            setEditStatusWindowText("Waiting for server...");
        }
        else
        {
            closeEditStatusWindow();
        }
    }
}

int editIsWaitingForServer(void)
{
    return sel.waitingforserver;
}




