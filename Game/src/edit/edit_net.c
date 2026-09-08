#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/network/netio.h>
#include "win/win_init.h"
#include "comm_game.h"
#include "clientcomm/clientcomm.h"
#include <utilitieslib/utils/memcheck.h>
#include "group/group.h"
#include "edit/edit.h"
#include <utilitieslib/network/netcomp.h>
#include "group/groupProperties.h"
#include "edit/edit_net.h"
#include "edit/edit_cmd.h"
#include "edit/edit_select.h"
#include "cmdparse/cmdgame.h"
#include "graphics/camera.h"
#include "group/grouputil.h"
#include "edit/edit_cmd_file.h"
#include <utilitieslib/utils/fileutil.h>
#include "seq/gfxtree.h"
#include <utilitieslib/utils/utils.h>
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include "edit/edit_cmd_select.h"
#include "bases/bases.h"
#include "edit/edit_library.h"
#include "edit/DebugLocation.h"
#include <utilitieslib/components/StashTable.h>
#include <string.h>

/*
    ref idx1 idx2 idx3 -> uniquely identifies def inside of ref
    new def info
*/

static void sendDefId(Packet *pak,GroupDef *def)
{
    pktSendBitsPack(pak,1,def->file->svr_idx);
    pktSendBitsPack(pak,1,def->id);
}

void sendTracker(Packet *pak,DefTracker *tracker)
{
    int        i,depth,idxs[100];
    char *names[100];

    depth = trackerPack(tracker,idxs,names,0);
    pktSendBitsPack(pak,1,depth);
    for(i=0;i<depth;i++)
    {
        pktSendBitsPack(pak,1,idxs[i]);
        pktSendString(pak, names[i]);
    }
}

void editUngroup(int edit_open)
{
    Packet        *pak;
    int            i;

    if (!sel_count)
        return;
    for (i=0;i<sel_count;i++)
    {
        if (groupRefAtomic(sel_list[i].def_tracker->def))
            return;
    }
    editSelSort();
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"ungroup");
    pktSendBitsPack(pak,1,edit_state.editorig);
    pktSendBitsPack(pak,1,sel_count);
    for(i=0;i<sel_count;i++)
    {
        pktSendBitsPack(pak,1,sel_list[i].id);
        sendTracker(pak,sel_list[i].def_tracker);
    }
    editWaitingForServer(1, 1);
    commSendPkt(pak);
    unSelect(2);
}

// finds the tracker with the greatest depth that is a parent
// to all selected trackers, returns NULL if there isn't any
DefTracker * selectedTrackersYoungestCommonAncestor()
{
    int * depth=NULL;
    DefTracker ** trackers=NULL;
    int i;
    int highest;
    bool matching;
    DefTracker * ret;
    if (sel_count==0)
        return NULL;
    if (sel_count==1)
        return sel_list[0].def_tracker->parent;
    for (i=0;i<sel_count;i++) {
        eaPush(&trackers,sel_list[i].def_tracker);
        eaiPush(&depth,trackerDepth(sel_list[i].def_tracker));
    }
    highest=depth[0];
    for (i=0;i<eaiSize(&depth);i++)
        if (depth[i]<highest)
            highest=depth[i];

    do {
        for (i=0;i<eaiSize(&depth);i++) {
            while (depth[i]>highest) {
                trackers[i]=trackers[i]->parent;
                depth[i]--;
            }
        }
        matching=true;
        for (i=1;i<eaiSize(&depth);i++) {
            if (trackers[i]!=trackers[i-1]) {
                matching=false;
                break;
            }
        }
        highest--;
    } while (!matching);
    ret=trackers[0];
    eaDestroy(&trackers);
    return ret;
}

// 
int illegalOperation(void)
{
    DefTracker **trackers = NULL;
    int i;

    for(i = 0; i < sel_count; i++)
    {
        DefTracker *tracker = sel_list[i].def_tracker;
        while(tracker->parent)
        {
            tracker = tracker->parent;
            eaPush(&trackers, tracker);
        }
    }

    for(i = 0; i < sel_count; i++)
    {
        DefTracker *tracker = sel_list[i].def_tracker;
        while(tracker)
        {
            int j;
            for(j = 0; j < eaSize(&trackers); j++)
            {
                if(    strcmp(tracker->def->name, trackers[j]->def->name) == 0 &&
                    tracker != trackers[j])
                {
                    eaDestroy(&trackers);
                    return 1;
                }
            }
            tracker = tracker->parent;
        }
    }

    eaDestroy(&trackers);
    return 0;
}


//attach==0 if the operation is 'group'
//attach==1 if the operation is 'attach'
void editGroup(int attach)
{
    Packet    *pak;
    int        i,idx,pivot_idx=-1;

    if (!sel_count)
        return;
    if (edit_state.editorig && illegalOperation()) {
        winMsgAlert("As far as I can tell this sort of operation doesn't make any sense at all.  Maybe you meant to turn off editlib/editorig?  Talk to Jonathan if you still think you mean to do what you're trying to do.");
        return;
    }
    editSelSort();
    for(i=0;i<sel_count;i++)
    {
        if (sel_list[i].pick_pivot)
            pivot_idx = i;
    }
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    if (attach)
        pktSendString(pak,"attach");
    else
        pktSendString(pak,"group");
    pktSendBitsPack(pak,1,edit_state.editorig);
    pktSendBitsPack(pak,1,sel_count);
    pktSendBitsPack(pak,1,pivot_idx);
    idx = sel_count-1;
    for(i=0;i<sel_count;i++) 
    {
        if (sel_list[i].def_tracker == sel.group_parent_def)
        {
            winMsgAlert("Object can't be it's own parent!");
            return;
        }
    } 

    if (sel.group_parent_def!=NULL && sel.group_parent_def->def==NULL)
        sel.group_parent_def=NULL;
    //MW Set Parent should override the editGroup's preference for keeping a def in place.
    //But active layer only wants to act like set parent on paste, 
    //so if the setParent is the ActiveLayer, don't do it.
    //(But if you explicitly hit the attach button, then go ahead and use the active layer)
    if ( sel.group_parent_def && (sel.group_parent_def != sel.activeLayer || attach == 1 ) )
    {
        printf("Group under: %s\n",sel.group_parent_def->def->name);
        pktSendBitsPack(pak,1,sel.group_parent_refid);
        sendTracker(pak,sel.group_parent_def);//->parent);
    }
    else
    {
        if (selectedTrackersYoungestCommonAncestor()==NULL)
            printf("Group Under: NULL\n");
        else
            printf("Group under: %s\n",selectedTrackersYoungestCommonAncestor()->def->name);
        pktSendBitsPack(pak,1,sel_list[idx].id);
        sendTracker(pak,selectedTrackersYoungestCommonAncestor());
    }


    for(idx=0;idx<sel_count;idx++)
    {
        i = idx;
        pktSendBitsPack(pak,1,sel_list[i].id);
        sendTracker(pak,sel_list[i].def_tracker);
        printf("%s\n",sel_list[i].def_tracker->def->name);
    }
    printf("\n");
    editWaitingForServer(1, 1);
    commSendPkt(pak);
    unSelect(2);
}

void editGroupOld(int attach)
{
    Packet    *pak;
    int        i,idx,pivot_idx=-1;

    if (!sel_count)
        return;
    if (edit_state.editorig && illegalOperation()) {
        winMsgAlert("As far as I can tell this sort of operation doesn't make any sense at all.  Maybe you meant to turn off editlib/editorig?  Talk to Jonathan if you still think you mean to do what you're trying to do.");
        return;
    }
    editSelSort();
    for(i=0;i<sel_count;i++)
    {
        if (sel_list[i].pick_pivot)
            pivot_idx = i;
    }
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    if (attach)
        pktSendString(pak,"attach");
    else
        pktSendString(pak,"group");
    pktSendBitsPack(pak,1,edit_state.editorig);
    pktSendBitsPack(pak,1,sel_count);
    pktSendBitsPack(pak,1,pivot_idx);
    idx = sel_count-1;
    for(i=0;i<sel_count;i++) 
    {
        if (sel_list[i].def_tracker == sel.group_parent_def)
        {
            winMsgAlert("Object can't be it's own parent!");
            return;
        }
        if ((sel_list[i].def_tracker && sel.group_parent_def && sel.group_parent_def->parent) && sel_list[i].def_tracker->parent == sel.group_parent_def->parent)
        {
            winMsgAlert("Sorry. Can't do this yet. Let me know if you need it.");
            return;
        }
    } 

    //MW Set Parent should override the editGroup's preference for keeping a def in place.
    //But active layer only wants to act like set parent on paste, 
    //so if the setParent is the ActiveLayer, don't do it.
    //(But if you explicitly hit the attach button, then go ahead and use the active layer)
    if ( sel.group_parent_def && (sel.group_parent_def != sel.activeLayer || attach == 1 ) )
    {
        printf("Group under: %s\n",sel.group_parent_def->def->name);
        pktSendBitsPack(pak,1,sel.group_parent_refid);
        sendTracker(pak,sel.group_parent_def);//->parent);
    }
    else
    {
        if (selectedTrackersYoungestCommonAncestor()==NULL)
            printf("Group Under: NULL\n");
        else
            printf("Group under: %s\n",selectedTrackersYoungestCommonAncestor()->def->name);
        pktSendBitsPack(pak,1,sel_list[idx].id);
        sendTracker(pak,selectedTrackersYoungestCommonAncestor());
    }


    // Grouping subgroups of a group that is instanciated multiple times can cause problems
    // so first we update the trackers that we are going to change, this causes them to 
    // become their own objects

    if (0) {
        DefTracker ** trackers=NULL;
        for (i=0;i<sel_count;i++) {
            int j;
            if (sel_list[i].def_tracker->parent==NULL)
                continue;
            for (j=0;j<eaSize(&trackers);j++) {
                if (sel_list[i].def_tracker->parent==trackers[j])
                    break;
            }
            if (j<eaSize(&trackers)) continue;
            eaPush(&trackers,sel_list[i].def_tracker->parent);
        }
        editUpdateTrackers(trackers);
        eaDestroy(&trackers);
    }

    for(idx=0;idx<sel_count;idx++)
    {
        i = idx;
        pktSendBitsPack(pak,1,sel_list[i].id);
        sendTracker(pak,sel_list[i].def_tracker);
        printf("%s\n",sel_list[i].def_tracker->def->name);
    }
    printf("\n");
    editWaitingForServer(1, 1);
    commSendPkt(pak);
    unSelect(2);
}

void editDetach()
{
    Packet * pak;
    int i;
    {
        DefTracker * dt=sel_list[0].def_tracker;
        while (dt->parent)
            dt=dt->parent;
        if (stashFindPointerReturnPointer(dt->def->properties,"Layer")) {
            DefTracker * temp=sel.group_parent_def;
            int tempi = sel.group_parent_refid;
            sel.group_parent_def = sel.activeLayer;
            sel.group_parent_refid = sel.activeLayer_refid;
            editGroup(1);
            sel.group_parent_def=temp;
            sel.group_parent_refid=tempi;
            return;
        }
    }
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"detach");

    edit_state.cut=1;
    editCmdCutCopy();

    {
        int        one_deeper=0;
        Mat4    mat,matx;

        sel.use_pick_point = 0;

        if (!sel.parent)
            return;

        copyMat4(unitmat,mat);
        pktSendBitsPack(pak,1,edit_state.noServerUpdate);
        pktSendBitsPack(pak,1,edit_state.editorig);
        pktSendBits(pak,1,sel.cut && sel_count);

        pktSendBitsPack(pak,1,MAX(1,sel_count));
        editSelSort();
        for(i=0;!i || i<sel_count;i++)
        {
            int        ref_id;
            DefTracker    *tracker;
            Mat4    tracker_mat,inv_tracker,mat2;

            // where to put it in the tree
            if (!sel_count)
            {
                if (sel.group_parent_def)
                {
                    ref_id = sel.group_parent_refid;
                    tracker = sel.group_parent_def;
                    one_deeper = 1;
                }
                else
                {
                    ref_id = -1;
                    tracker = 0;
                }
            }
            else
            {
                //MW added if the selected thing is a layer, and it's the set parent, then you didn't mean it to be the 
                //sibling of this, you meant it to be the parent...
                if( (sel_count != 1 || sel.fake_ref.def) && //if I've just got me, I want to act like normal
                    sel_list[i].def_tracker && 
                    sel_list[i].def_tracker->def && 
                    groupDefFindPropertyValue( sel_list[i].def_tracker->def, "Layer") && 
                    sel.group_parent_def == sel_list[i].def_tracker )
                {
                    ref_id = sel.group_parent_refid;
                    tracker = sel.group_parent_def;
                    one_deeper = 1;
                }
                //end MW added 
                else
                {
                    ref_id = sel_list[i].id;
                    tracker = sel_list[i].def_tracker;
                }
            }

            pktSendBitsPack(pak,1,-1);
            sendTracker(pak,NULL);

            // what it is
            if (sel.fake_ref.def)
            {
                sendDefId(pak,sel.fake_ref.def);
                pktSendIfSetF32(pak,0);
            }
            else
            {
                sendDefId(pak,tracker->def);
                pktSendIfSetF32(pak,tracker->def->light_radius);
            }

            // where it is, local to its tree position
            mulMat4(mat,sel_list[i].mat,matx);
            if (tracker && (one_deeper || tracker->parent))
            {
                if (sel_count)
                    ref_id = sel_list[i].id;
                if (one_deeper)
                    trackerGetMat(tracker,groupRefFind(ref_id),tracker_mat);
                else
                    trackerGetMat(tracker->parent,groupRefFind(ref_id),tracker_mat);
                invertMat4Copy(tracker_mat,inv_tracker);
                mulMat4(inv_tracker,matx,mat2);
                copyMat4(mat2,matx);
            }
            sendMat4(pak,sel_list[0].def_tracker->mat);
        }
        if (sel_count)
        {
            sel_count = 1;
            updatePropertiesMenu = true; // if it was 1, it will be 0 in 20 lines...
        }
        {
            extern GroupInfo * group_ptr;

            if (!sel_count)
                return;

            pktSendBitsPack(pak,1,edit_state.editorig);
            pktSendBitsPack(pak,1,sel_count);
            editSelSort();
            for(i=0;i<sel_count;i++)
            {
                DefTracker *ref;

                ref = groupRefFind(sel_list[i].id);
                pktSendBitsPack(pak,1,ref->id);
                sendTracker(pak,sel_list[i].def_tracker);
            }
            editWaitingForServer(1, 1);
            unSelect(2);
            sel_count = 0; // updatePropertiesMenu = true is set above
        }

        editWaitingForServer(1, 1);
        commSendPkt(pak);
        gfxTreeDelete(sel.parent);
        gridFreeAll(&sel.grid);
        sel.parent = 0;
    }
    edit_state.detach = 0;
}

void editDelete()
{
    Packet    *pak;
    int        i;
    extern GroupInfo * group_ptr;
    
    if (!sel_count)
        return;

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"delete");
    pktSendBitsPack(pak,1,edit_state.editorig);
    pktSendBitsPack(pak,1,sel_count);
    editSelSort();
    for(i=0;i<sel_count;i++)
    {
        DefTracker *ref;

        ref = groupRefFind(sel_list[i].id);
        pktSendBitsPack(pak,1,ref->id);
        sendTracker(pak,sel_list[i].def_tracker);
    }
    editWaitingForServer(1, 1);
    commSendPkt(pak);
    unSelect(2);
    sel_count = 0;
    updatePropertiesMenu = true;
}

void editPaste()
{
    int        i,one_deeper=0;
    Packet    *pak;
    Mat4    mat,matx;

    sel.use_pick_point = 0;

    if (!sel.parent)
        return;

    mulMat4(sel.parent->mat,sel.offsetnode->mat,matx);
    mulMat4(matx,sel.scalenode->mat,mat);
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"paste");                            //
    pktSendBitsPack(pak,1,edit_state.editorig);            // what mode of updating to apply
    pktSendBitsPack(pak,1,MAX(1,sel_count));
    pktSendBitsPack(pak,1,sel.cut && sel_count);

    if (sel.group_parent_def!=NULL && sel.group_parent_def->def==NULL)
        sel.group_parent_def=NULL;
    if (sel_count==0 || (sel_count==1 && sel.fake_ref.def!=NULL)) 
    {    // this means we're pasting a new object from the library
        if (sel_count==1)
        {
            pktSendBitsPack(pak,1,trackerRootID(sel_list[0].def_tracker->parent));
            sendTracker(pak,sel_list[0].def_tracker->parent);    // where to paste
        } 
        else
        {
            pktSendBitsPack(pak,1,trackerRootID(sel.group_parent_def));
            sendTracker(pak,sel.group_parent_def);    // where to paste
        }
        pktSendBitsPack(pak,1,trackerRootID(sel_list[0].def_tracker));
        sendTracker(pak,sel_list[0].def_tracker);    // where to paste
        sendDefId(pak,sel.fake_ref.def);        // def to paste
        sendMat4(pak,mat);
    } 
    else 
    {
        for(i=0;i<sel_count;i++)
        {
            pktSendBitsPack(pak,1,trackerRootID(sel_list[i].def_tracker->parent));
            sendTracker(pak,sel_list[i].def_tracker->parent);    // where to paste
            pktSendBitsPack(pak,1,trackerRootID(sel_list[i].def_tracker));
            sendTracker(pak,sel_list[i].def_tracker);            // tracker to paste
            sendDefId(pak,sel_list[i].def_tracker->def);        // def to paste
            mulMat4(mat,sel_list[i].mat,matx);                    // absolute matrix of the object
            sendMat4(pak,matx);
        }
    }
    if (!edit_state.noServerUpdate)
        editWaitingForServer(1, 1);
    commSendPkt(pak);
    gfxTreeDelete(sel.parent);
    gridFreeAll(&sel.grid);
    sel.parent = 0;
    unSelect(2);
}

void editNew()
{
    Packet    *pak;

    edit_state.editstate_new = 0;

    if (!winMsgYesNo("OK to delete world?"))
        return;

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"new");
    commSendPkt(pak);
    editWaitingForServer(1, 1);
    unSelect(2);
}
#include "clientcomm/clientcomm.h"
#include <utilitieslib/utils/timing.h>

void saveOnExit() {
//    Packet    *pak;
    char    *fname;
    char buf[1024];
     int timeout=0;
    int timer=timerAlloc();

    if (g_base.rooms || !isDevelopmentMode() || !edit_state.isDirty || !winMsgYesNo("Do you want to save before exiting?")) {
        timerFree(timer);
        return;
    }

    if (!game_state.world_name[0] || edit_state.saveas || edit_state.savesel)
        fname = winGetFileName("*.txt",buf,1);
    else
        fname = game_state.world_name;
    if (!fname) {
        winMsgAlert("Unable to save: No file name provided.");
        timerFree(timer);
        return;
    }
//    pak = pktCreate();
//    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
//    pktSendString(pak,"save");
//    pktSendString(pak,fname);
//    commSendPkt(pak);
    editCmdSaveFile();            //why on earth do i have to do this?
    timerStart(timer);
    while (!commCheck(SERVER_UPDATE) && commConnected()) {
        if (!control_state.notimeout && timerElapsed(timer)>60) {    //DEFAULT_TIMEOUT==60, just stating it explicitly
            timeout=1;
        } else {
            netIdle(&comm_link, 1, 0.5);
        }
    }
    if (!commConnected())
        winMsgAlert("Server disconnected");
    if (timeout)
        winMsgAlert("Unable to save, server timeout.");
    timerFree(timer);
}

static FileScanAction deleteAutosaveFunc(char * dir,struct _finddata32_t * fileData)
{
    char target[256];

    forwardSlashes(game_state.world_name);
    strcpy(target,strrchr(game_state.world_name,'/')+1);
    if (strstr(target,".txt")) {
        strcpy(strstr(target,".txt"),".autosave.txt");
    } else {
        strcat(target,".autosave.txt");    //i hope this line never gets reached, it means someone is being stupid
    }

    if (strcmp(fileData->name,target)==0) {
        char buf[255];
        strcpy(buf,"C:/game/data/");
        strcat(buf,dir);
        strcat(buf,"/");
        strcat(buf,fileData->name);
        fileForceRemove(buf);
    }
    return FSA_NO_EXPLORE_DIRECTORY;
}

void editSave(char *fname)
{
    Packet    *pak;

    if (!fname)
        return;
    pak = pktCreate();
    commandMenuUpdateRecentMaps(fname);
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"save");
    pktSendString(pak,fname);
    commSendPkt(pak);

    //handle the deletion of autosaved files:
    {
        char dir[256]="/";
        strcpy(dir, fname);
        forwardSlashes(dir);
        if (strchr(dir,'/')) {    //make sure to mess with the root directory (C:\game\data\)
            if (strstri(dir,"game/data/"))
                strcpy(dir,strstri(dir,"game/data/")+9);    //chop off the "C:\game\data\", because it should be assumed
            if (strrchr(dir,'/')!=NULL)
                dir[(int)(strrchr(dir,'/')-dir)]=0;        //chop off the file name, so we're just left with the directory
            else
                dir[0]='\0';
            if (dir[0]=='\0') 
                strcpy(dir,"/");
        }
        if (strcmp(dir,"/")!=0)                            //only do this if we are not in C:\game\data
            fileScanAllDataDirs(dir,deleteAutosaveFunc);    //because we are not supposed to mess with files there
    }
    //end handling of autosaved files

    edit_state.isDirty=0;
}

void editSaveSelection(char *fname)
{
    Packet    *pak;
    int        i;

    if (!fname)
        return;
    editSelSort();
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"savesel");
    pktSendString(pak,fname);

    pktSendBitsPack(pak,1,sel_count);
    for(i=0;i<sel_count;i++)
    {
        pktSendBitsPack(pak,1,sel_list[i].id);
        sendTracker(pak,sel_list[i].def_tracker);
    }

    editWaitingForServer(1, 1);
    commSendPkt(pak);
    unSelect(2);
    edit_state.isDirty=0;
}

void editSaveLibs()
{
    Packet    *pak;

    edit_state.savelibs = 0;
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"savelibs");
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editLoad(char *fname,int import)
{
    Packet    *pak;

    edit_state.reload = edit_state.load = edit_state.import = 0;

    if (!fname || isProductionMode())
        return;

    if (!import)
        commandMenuUpdateRecentMaps(fname);
    else if(strstr(fname, "_layer_"))
        winMsgAlert("It looks like you are importing a layer.\n"
                    "Imported layers are shared by default, meaning it will stay with the map you imported from.\n"
                    "If you would rather have a local copy of the layer for editing, remove the SharedFrom property.");
    loadScreenResetBytesLoaded();
    DebugLocationShutdown();

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"load");
    pktSendString(pak,fname);
    pktSendBits(pak,1,import);
    editWaitingForServer(1, 1);
    commSendPkt(pak);
    unSelect(2);
}

static void sendGroupDef(Packet *pak,GroupDef *def,char *fullpath,char *objname)
{
    pktSendBitsPack(pak,1,sizeof(GroupDef));
    pktSendBitsArray(pak,sizeof(GroupDef) * 8,def);
    pktSendString(pak,fullpath ? fullpath : def->file->fullname);
    pktSendString(pak,objname ? objname : def->name);
    pktSendString(pak,def->type_name);
    pktSendString(pak,def->beacon_name);
    pktSendString(pak,def->sound_name);
    pktSendString(pak,def->sound_script_filename);
    pktSendString(pak,def->tex_names[0]);
    pktSendString(pak,def->tex_names[1]);
    if(def->has_properties)
        WriteGroupPropertiesToPacket(pak, def->properties);
    
    //new texture swapping stuff
    {
        int k;
        pktSendBitsPack(pak,1,eaSize(&def->def_tex_swaps));
        for (k=0;k<eaSize(&def->def_tex_swaps);k++) {
            pktSendString(pak,def->def_tex_swaps[k]->tex_name);
            pktSendString(pak,def->def_tex_swaps[k]->replace_name);
            pktSendBits(pak,1,def->def_tex_swaps[k]->composite);
        }
    }

}

int cmpNames(const void* aData, const void* bData)
{
    const DefTracker ** a = (const DefTracker **)aData;
    const DefTracker ** b = (const DefTracker **)bData;
    return strcmp((*a)->def->name,(*b)->def->name);
}

void editUpdateTrackers(DefTracker ** trackers)
{
    Packet    *pak;
    int i;
    if (eaSize(&trackers)==0)
        return;
    pak = pktCreate();
    eaQSort(trackers,cmpNames);
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"updatetrackers");
    pktSendBitsPack(pak,1,edit_state.editorig);
    pktSendBitsPack(pak,1,eaSize(&trackers));
    for (i=0;i<eaSize(&trackers);i++)
    {
        pktSendString(pak,trackers[i]->def->name);
    }
    for (i=0;i<eaSize(&trackers);i++)
    {
        pktSendBitsPack(pak,1,groupRefFindID(trackers[i]));
        sendTracker(pak,trackers[i]);
        sendDefId(pak,trackers[i]->def);
        sendGroupDef(pak,trackers[i]->def,0,0);
    }
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editUpdateTracker(DefTracker *tracker)
{
    Packet    *pak;
    extern GroupDef    save_def;
    extern StashTable save_properties;
    extern int properties_saved;

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"updatetracker");
    pktSendBitsPack(pak,1,edit_state.editorig);
    pktSendBitsPack(pak,1,groupRefFindID(tracker));
    sendTracker(pak,tracker);
    sendDefId(pak,tracker->def);
    sendGroupDef(pak,tracker->def,0,0);
    editWaitingForServer(1, 1);
    commSendPkt(pak);
    if (tracker == sel_list[0].def_tracker)
    {
        int curTrackerHasProperties;
        DefTexSwap **def_tex_swaps;

        // Does the temporarily altered tracker have properties?
        curTrackerHasProperties = tracker->def->has_properties;

        // Copy the saved group def back.
        def_tex_swaps = tracker->def->def_tex_swaps; // sigh...
        *tracker->def = save_def;
        tracker->def->def_tex_swaps = def_tex_swaps; // ...sigh

        if(properties_saved)
        {
            tracker->def->has_properties = 1;

            // If the altered tracker does not have properties, the saved off group def has a stale, invalid
            // hash table reference.  Create a new table in this case.
            if(curTrackerHasProperties)
                stashTableClear(tracker->def->properties);
            copyPropertyStashTable(save_properties, tracker->def->properties);
        }
    }
}


void editUpdateDef(GroupDef *def,char *fullpath,char *objname)
{
    Packet    *pak;

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"updatedef");
    sendDefId(pak,def);
    sendGroupDef(pak,def,fullpath,objname);
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editUndo()
{
    Packet    *pak;

    edit_state.undo = 0;
    unSelect(1);
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"undo");
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editRedo()
{
    Packet    *pak;

    edit_state.redo = 0;
    unSelect(1);
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"redo");
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editSendSceneFile(char *scenefile)
{
    Packet    *pak;

    unSelect(1);
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"scenefile");
    pktSendString(pak,scenefile);
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editSendLoadScreen(char *loadscreen)
{
    Packet    *pak;

    unSelect(1);
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"loadscreen");
    pktSendString(pak,loadscreen);
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editNetUpdate()
{
    editWaitingForServer(0, 1);
}

void editSendPlayerPos()
{
    char buffer[1000];

    sprintf(buffer,
            "setpospyr %f %f %f %f %f %f",
            vecParamsXYZ(cam_info.cammat[3]),
            vecParamsXYZ(control_state.pyr.cur));
            
    cmdParse(buffer);
}

void editDefLoad(char *name)
{
    Packet    *pak;

    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"defload");
    pktSendString(pak,name);
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}

void editTraySwap(char *fname)
{
    Packet    *pak;
    int        i;

    if (!fname)
        return;
    editSelSort();
    pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,"trayswap");
    pktSendString(pak,fname);

    pktSendBitsPack(pak,1,sel_count);
    for(i=0;i<sel_count;i++)
    {
        pktSendBitsPack(pak,1,sel_list[i].id);
        sendTracker(pak,sel_list[i].def_tracker);
    }

    editWaitingForServer(1, 1);
    commSendPkt(pak);

    unSelect(2);
}

void editCmd(char *cmd)
{
    Packet    *pak = pktCreate();
    pktSendBitsPack(pak,1,CLIENT_WORLDCMD);
    pktSendString(pak,cmd);
    editWaitingForServer(1, 1);
    commSendPkt(pak);
}
