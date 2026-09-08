#include "group/group.h"
#include "group/groupfilelib.h" //added by HA as requested by Don Pham 4/11/12
#include "edit/edit_cmd.h"
#include "edit/edit_select.h"
#include <utilitieslib/utils/mathutil.h>
#include "group/grouptrack.h"
#include "win/win_init.h"
#include "gridcoll/gridcoll.h"
#include "edit/edit_drawlines.h"
#include "win/input.h"
#include "edit_errcheck.h"
#include "edit/edit_net.h"
#include "edit/edit_library.h"
#include "edit_cmd_group.h"
#include "graphics/camera.h"
#include <utilitieslib/components/StashTable.h>
#include "seq/gfxtree.h"
#include "graphics/gfxwindow.h"
#include "UI/uiInput.h"
#include <utilitieslib/utils/utils.h>
#include "seq/anim.h"
#include "editorUI.h"
#include "graphics/groupdraw.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/qsortG.h>
#include "graphics/groupdrawutil.h"
#include "graphics/groupMiniTrackers.h"
#include "render/tex.h"
#include <utilitieslib/utils/file.h>

void editSnapToTrackers(DefTracker** trackers)
{
    Vec3        min = {8e16,8e16,8e16}, max = {-8e16,-8e16,-8e16};
    Vec3        dv;
    Mat4        mat;
    extern Vec3 view_pos;
    extern F32 view_dist;
    extern CameraInfo cam_info;
    int i, numTrackers = eaSize(&trackers);
    for (i=0;i<numTrackers;i++)
    {
        DefTracker* tracker = trackers[i];
        trackerGetMat(tracker,groupRefFind(sel_list[i].id),mat);
        mulVecMat4(tracker->def->min,mat,dv);
        MINVEC3(min,dv,min);
        mulVecMat4(tracker->def->max,mat,dv);
        MAXVEC3(max,dv,max);
    }
    subVec3(max,min,dv);
    view_dist = lengthVec3(dv);
    scaleVec3(dv,0.5,dv);
    addVec3(dv,min,view_pos);
}

void editCmdSnapDist()
{
    int        i;
    extern Vec3 view_pos;
    extern F32 view_dist;
    extern CameraInfo cam_info;
    
    if (sel_count)
    {
        editSetTrackerOffset(sel_list[0].def_tracker);
    } else {
        view_dist=0;
        copyVec3(cam_info.cammat[3],view_pos);
    }
    edit_state.setview = 0;
    if (sel.parent)
    {
        copyVec3(sel.parent->mat[3],view_pos);
    }
    else if (sel_count)
    {
        DefTracker** trackerlist = NULL;
        for(i=0;i<sel_count;i++)
            eaPush(&trackerlist, sel_list[i].def_tracker);
        editSnapToTrackers(trackerlist);
        eaDestroy(&trackerlist);
    }
}




void editUnhide(DefTracker *tracker,U32 trick_flags)
{
    int        i;

    if (!tracker)
        return;
    if (tracker->tricks)
        tracker->tricks->flags1 &= ~trick_flags;
    for(i=0;i<tracker->count;i++)
        editUnhide(&tracker->entries[i],trick_flags);
}

void editCmdSetPivot()
{
    edit_state.setpivot = 0;
    if (sel_count != 1)
        winMsgAlert("Only works on first selection");
    else
    {
    int        i,id;
    DefTracker    *tracker;

        for(i=0;i<sel_count;i++)
            sel_list[i].pick_pivot = 0;
        id = sel_list[0].id;
        tracker = sel_list[0].def_tracker;
        selAdd(sel_list[0].def_tracker,sel_list[0].id,1,0);
        sel_list[0].pick_pivot = 1;
        selAdd(tracker,id,1,0);
    }
}

void editCmdHide()
{
    int        i;

    for(i=0;i<sel_count;i++)
    {
        if (!sel_list[i].def_tracker->tricks)
            sel_list[i].def_tracker->tricks = calloc(sizeof(TrickInfo),1);
        sel_list[i].def_tracker->tricks->flags1 |= TRICK_HIDE;

        //Hidden things can't be the active layer
        if( sel_list[i].def_tracker == sel.activeLayer  )
            editCmdUnsetActiveLayer();
    }
    edit_state.hide = 0;
    unSelect(1);
}

//reverses whether an object is hidden or not hidden
//used by editCmdHideOthers
void reverseHideTrackers(DefTracker * tracker) {
    if (tracker==NULL) return;
    if (tracker->tricks==NULL)        //if it has no tricks, its not hidden
        tracker->tricks=calloc(sizeof(TrickInfo),1);    //so allocate tricks, so we can hide it
    if (tracker->tricks->flags1 & TRICK_HIDE) {            //if it is hidden
        int i;
        bool lastChild=true;
        tracker->tricks->flags1 &= ~TRICK_HIDE;            //unhide it
        for (i=0;i<tracker->count;i++)
            if (tracker->entries[i].tricks && (tracker->entries[i].tricks->flags1 & TRICK_HIDE))
                lastChild=false;
        if (lastChild) return;
        for (i=0;i<tracker->count;i++)
            reverseHideTrackers(&tracker->entries[i]);    //and reverse hide all of its children
    } else                                                    //otherwise
        tracker->tricks->flags1 |= TRICK_HIDE;                //hide it (and by doing so, hide its children)
                                                            //and we're done
}


void editCmdUnhide()
{
    int        i;

    for(i=0;i<group_info.ref_count;i++)
        editUnhide(group_info.refs[i],TRICK_HIDE);
    edit_state.unhide = 0;
}

void editCmdHideOthers()
{
    DefTracker *tracker;
    int i;
    editCmdUnhide();
    for (i=0;i<sel_count;i++) {
        tracker=sel_list[i].def_tracker;
        while (tracker!=NULL) {
            if (tracker->tricks==NULL)
                tracker->tricks=calloc(sizeof(TrickInfo),1);
            tracker->tricks->flags1 |= TRICK_HIDE;
            tracker=tracker->parent;
        }
    }
    for (i=0;i<group_info.ref_count;i++)
        reverseHideTrackers(group_info.refs[i]);
    edit_state.hideothers = 0;
}

void editCmdFreeze() {
    int i;
    edit_state.freeze=0;
    for (i=sel_count-1;i>=0;i--) {
        DefTracker * tracker=sel_list[i].def_tracker;
        selAdd(sel_list[i].def_tracker,sel_list[i].id,2,0);
        tracker->frozen=1;
        editCmdUnsetActiveLayer();
    }
}

void editCmdUnfreezeRecursive(DefTracker * tracker) {
    int i;
    tracker->frozen=0;
    for (i=0;i<tracker->count;i++)
        editCmdUnfreezeRecursive(&tracker->entries[i]);
}

void editCmdUnfreeze() {
    int        i;
    edit_state.unfreeze=0;
    for(i=0;i<group_info.ref_count;i++)
        editCmdUnfreezeRecursive(group_info.refs[i]);
}

void reverseFreezeTrackers(DefTracker * tracker) {
    if (tracker==NULL) return;
    if (tracker->frozen) {                                    //if frozen
        int i;
        bool lastChild=true;
        tracker->frozen=0;                                    //unfreeze it
        for (i=0;i<tracker->count;i++)
            if (tracker->entries[i].frozen)
                lastChild=false;
        if (lastChild) return;                                //if none of it's children are frozen we're done
        for (i=0;i<tracker->count;i++)                        //if not...
            reverseFreezeTrackers(&tracker->entries[i]);    //reverse freeze all of its children
    } else                                                    //otherwise
        tracker->frozen=1;                                    //freeze it (and by doing so, freeze its children)
    //and we're done
}

void editCmdFreezeOthers()
{
    DefTracker *tracker;
    int i;
    editCmdUnfreeze();
    for (i=0;i<sel_count;i++) {
        tracker=sel_list[i].def_tracker;
        while (tracker!=NULL) {
            tracker->frozen=1;
            tracker=tracker->parent;
        }
    }
    for (i=0;i<group_info.ref_count;i++)
        reverseFreezeTrackers(group_info.refs[i]);
    edit_state.freezeothers = 0;
}

//selectLassoFrustrum
//figures out what objects are in the lasso volume and selects them
//returns 1 if everything in the tracker was in in the frustrum
//returns 0 if nothing in the tracker was in the frustrum
//returns -1 if something, but not everything, was in the frustrum
int selectLassoFrustrum(DefTracker * tracker,Vec4 planes[5],int layer,int selAddMode) {
    int i;
    Vec3 mid;
    if (tracker->def==NULL) return 0;
    if (tracker->tricks!=NULL && (tracker->tricks->flags1 & TRICK_HIDE))
        return 0;

    copyVec3(tracker->mat[3],mid);
    for (i=0;i<5;i++) {
        int j;
        float sum=0;
        for (j=0;j<3;j++)
            sum+=planes[i][j]*mid[j];
        sum+=planes[i][3];
        if (sum>0 && (tracker->count==0 || strstr(tracker->def->name,"grp")!=tracker->def->name))
            return 0;
    }
    if (tracker->count==0 || strstr(tracker->def->name,"grp")!=tracker->def->name)
        return 1;
    {
        int * ret=(int *)_alloca(tracker->count*sizeof(int));
        int opened=0;
        int allInside=1;
        int anySelected=0;
        int doNotClose=0;
//        if (tracker->entries==NULL) {
//        if (!tracker->edit || !tracker->parent) {
            trackerOpenEdit(tracker);
            opened=1;
//        }
//        }
        for (i=0;i<tracker->count;i++) {
            ret[i]=selectLassoFrustrum(&tracker->entries[i],planes,layer,selAddMode);
            if (ret[i]==1)
                anySelected=1;
            else
                allInside=0;
            if (ret[i]!=0)
                doNotClose=1;
        }
        if (allInside) {
            if (opened)
                trackerCloseEdit(tracker);
            return 1;
        } else {
            if (anySelected) {
                for (i=0;i<tracker->count;i++)
                    if (ret[i]==1)
                        selAdd(&tracker->entries[i],trackerRootID(&tracker->entries[i]),selAddMode,EDITOR_LIST);
            } else {
//                if (opened && !doNotClose)
//                    trackerCloseEdit(tracker);
            }
        }
        if (anySelected)
            return -1;
    }
    return 0;
}

void editCmdLasso() {
    int mx,my;                //mouse coordinates
    int urx,ury,llx,lly;    //(x,y) for the upper-left and lower-left corners of the lasso
    inpMousePos(&mx,&my);

    if (edit_state.lassoX!=-1 || edit_state.lassoY!=-1) {
        if (mx<edit_state.lassoX) {
            urx=edit_state.lassoX;
            llx=mx;
        } else {
            urx=mx;
            llx=edit_state.lassoX;
        }
        if (my<edit_state.lassoY) {
            ury=edit_state.lassoY;
            lly=my;
        } else {
            ury=my;
            lly=edit_state.lassoY;
        }
        drawLasso(urx,ury,llx,lly);
    }


    if ( (isDown(MS_LEFT)) && (edit_state.lassoX==-1 && edit_state.lassoY==-1)) {
        edit_state.lassoX=mx;
        edit_state.lassoY=my;
    }
    if ( (!isDown(MS_LEFT)) && (edit_state.lassoX!=-1 && edit_state.lassoY!=-1)) {
        //select things in lasso
        int i;
        Vec3 v1,v2,buff,buff2;
        Vec3 normals[5];    //normals of 5 planes
        Vec3 points[5];        //1 point in each, no far plane, since we might as well select
                            //everything all the way to infinity
        Vec3 farPoints[4];    //corresponding far points for each of the sides of the frustrum
        Vec4 planes[5];        //the actual equations of the 5 planes
        Mat4 cameraInverted;//inverted camera matrix
        Vec3 frustrum[8];    //8 points of the frustrum

        if (urx==llx || ury==lly) {            //if the lasso is just a point or a line then don't try to
            edit_state.lassoX=-1;            //make a frustrum out of it, it would just do silly things
            edit_state.lassoY=-1;
            edit_state.drawingLasso=0;
            edit_state.lassoAdd=0;
            edit_state.lassoExclusive=0;
            edit_state.lassoInvert=0;
            return;
        }

        invertMat4Copy(cam_info.cammat,cameraInverted);

        gfxCursor3d(urx,ury,10,buff,buff2);
        copyVec3(buff,points[0]);
        copyVec3(buff2,farPoints[0]);
        gfxCursor3d(urx,lly,10,buff,buff2);
        copyVec3(buff,points[1]);
        copyVec3(buff2,farPoints[1]);
        gfxCursor3d(llx,lly,10,buff,buff2);
        copyVec3(buff,points[2]);
        copyVec3(buff2,farPoints[2]);
        gfxCursor3d(llx,ury,10,buff,buff2);
        copyVec3(buff,points[3]);
        copyVec3(buff2,farPoints[3]);
        copyVec3(points[3],points[4]);                        //we can use one of the previous points for the
                                                                //near plane as well
        for (i=0;i<=4;i++) {
            int j=0;
            for (j=0;j<=2;j++)
                points[i][j]*=1;
        }
        for (i=0;i<4;i++) {
            int j=0;
            for (j=0;j<=2;j++)
                farPoints[i][j]*=1;
        }
        gfxCursor3d(urx,ury,-10,frustrum[0],frustrum[4]);
        gfxCursor3d(urx,lly,-10,frustrum[1],frustrum[5]);
        gfxCursor3d(llx,lly,-10,frustrum[2],frustrum[6]);
        gfxCursor3d(llx,ury,-10,frustrum[3],frustrum[7]);
                                                            //near plane as well

        for (i=0;i<=3;i++) {                                //this figures out the normal to each side
            subVec3(points[(i+1)%4],points[i],v1);            //of the lasso frustrum
            subVec3(farPoints[i],points[i],v2);
            crossVec3(v1,v2,normals[i]);
        }
        subVec3(points[0],points[4],v1);                        //the near plane is a slightly special case
        subVec3(points[2],points[4],v2);                    //since it is totally defined by points
        crossVec3(v2,v1,normals[4]);

        for (i=0;i<=4;i++) {                                //compute the equation for the plane of each side
            int j;                                            //ax+by+cx+d=0
            planes[i][3]=0;
            for (j=0;j<3;j++) {
                planes[i][j]=normals[i][j];                    //a,b,c computed here
                planes[i][3]-=normals[i][j]*points[i][j];    //d computed here
            }
        }

        //now that we have the planes, plugging a point into each equation will tell us on what side of that
        //plane it lies, if a point lies on the same side of every plane, then it is in the lasso frustrum
        if (edit_state.lassoExclusive)
            unSelect(1);
//            for (i=sel_count-1;i>=0;i--)
//                selAdd(sel_list[i].def_tracker,sel_list[i].id,2,EDITOR_LIST);
        for (i=0;i<group_info.ref_count;i++) {
            int opened=0;
            int selAddMode=2;
            int lassoVal;
            if (group_info.refs[i]->tricks!=NULL && (group_info.refs[i]->tricks->flags1 & TRICK_HIDE))
                continue;
            if (edit_state.lassoInvert)
                selAddMode=2;
            if (sel.activeLayer!=NULL && group_info.refs[i]!=sel.activeLayer)
                continue;
            if (group_info.refs[i]->count>0 && group_info.refs[i]->entries==NULL) {
                trackerOpen(group_info.refs[i]);
                opened=1;
            }
            lassoVal=selectLassoFrustrum(group_info.refs[i],planes,i,selAddMode);
            if (lassoVal==1) {
                selAdd(group_info.refs[i],i,2,EDITOR_LIST);
            }
            if (sel_count==0)
                trackerCloseEdit(group_info.refs[i]);
            //else if (lassoVal==0 && opened)
            //    trackerClose(group_info.refs[i]);
            
        }
        edit_state.lassoX=-1;
        edit_state.lassoY=-1;
        edit_state.drawingLasso=0;
        edit_state.lassoAdd=0;
        edit_state.lassoInvert=0;
        edit_state.lassoExclusive=0;
    }
    /* else {
        if (edit_state.lassoX!=-1 || edit_state.lassoY!=-1) {        //released after having drawn some lasso
        }
    }*/
}


//if sub is a case-insensitive substring of str, returns
//a point to the first occurence of sub in str
//otherwise returns NULL
//does not use KMP, runs in O(strlen(str)*strlen(sub)) time

char * istrstr(const char * str,const char * sub) {
    int i,j;
    int strLength;
    int subLength;

    if (str==NULL || sub==NULL) return NULL;

    strLength=strlen(str);
    subLength=strlen(sub);

    for (i=0;i<strLength-subLength+1;i++) {
        for (j=0;j<subLength;j++) {
            if (!(str[i+j]==sub[j] || (str[i+j]+'a'-'A'==sub[j]) || (str[i+j]==sub[j]+'a'-'A')))
                break;
        }
        if (j==subLength)
            return ((char *)(str+i));
    }
    return NULL;
}

int selectForProperties;    //1 iff a search string matches one of a def's properties
char * searchString;        //string being sought
                            //these are global so the callback function below can use them

int trackerSearchProperties(StashElement e)
{
    const char* key = stashElementGetStringKey(e);
    if (istrstr(key,searchString)) {
        selectForProperties=1;
        return 0;
    }
    return 1;
}

int selectByName(DefTracker * tracker,int layer,char * name) {
    int found=0;
    if (tracker==NULL) return 0;
    if (tracker->tricks!=NULL && (tracker->tricks->flags1 & TRICK_HIDE))
        return 0;
    selectForProperties=0;
    if (tracker->def && tracker->def->properties) {
        searchString=name;
        stashForEachElement(tracker->def->properties,trackerSearchProperties);
    }
    if (selectForProperties || (tracker->def!=NULL && istrstr(tracker->def->name,name)!=NULL)) {
        selAdd(tracker,layer,2,EDITOR_LIST);
        return 1;
    } else {
        int i;
        int opened=0;
        if (tracker->count && tracker->entries==NULL) {
            trackerOpen(tracker);
            opened=1;
        }
        for (i=0;i<tracker->count;i++)
            found+=selectByName(&tracker->entries[i],layer,name);
        if (opened && found==0)
            trackerClose(tracker);
        else if (found>0)
            tracker->edit=1;
    }
    return found;
}

typedef struct EditorSearch {
    int searchWindow;
    int searchType;
    int searchLocation;
    int partialMatches;
    char searchString[256];
} EditorSearch;

typedef struct EditorSearchResult {
    char name[64];
    int path[100];
} EditorSearchResult;

static EditorSearch editorSearch;
static int propertySearchSuccessful;
static EditorSearchResult ** searchResults;
int searchIndex;

int trackerPropertySearchFunc(StashElement e) {
    PropertyEnt * prop;
    char * target;
    prop=stashElementGetPointer(e);
    if (editorSearch.searchType==1)    // Property Names
        target=prop->name_str;
    else
    if (editorSearch.searchType==2) // Property Values
        target=prop->value_str;
    else
        return 1;
    if (editorSearch.partialMatches)
        propertySearchSuccessful|=(strstri(target,editorSearch.searchString)!=NULL);
    else
        propertySearchSuccessful|=(stricmp(target,editorSearch.searchString)==0);
    return 1;
}

static int trackerMatchString(DefTracker * tracker,int uid,int suid,char * str) {
    if (tracker->def==NULL)
        return 0;
    if (editorSearch.searchType==0) {    // Object Names
        if (editorSearch.partialMatches)
            return (strstriConst(tracker->def->name,str)!=NULL);
        else
            return (stricmp(tracker->def->name,str)==0);
    } else
    if (editorSearch.searchType==1) {    // Property Names
        if (tracker->def->properties==NULL)
            return 0;
        propertySearchSuccessful=0;
        stashForEachElement(tracker->def->properties,trackerPropertySearchFunc);
        return propertySearchSuccessful;
    } else
    if (editorSearch.searchType==2) {    // Property Values
        if (tracker->def->properties==NULL)
            return 0;
        propertySearchSuccessful=0;
        stashForEachElement(tracker->def->properties,trackerPropertySearchFunc);
        return propertySearchSuccessful;
    } else
    if (editorSearch.searchType==3) {    // UIDs
        return uid == suid;
    }
    return 0;
}

static int searchForTrackers(DefTracker * tracker,int uid,int suid,char * str,int * path,int depth) {
    int i;
    int opened=0;
    int found=0;
    if (tracker->def==NULL)
        return 0;
    if (trackerMatchString(tracker,uid,suid,str)) {
        EditorSearchResult * esr=malloc(sizeof(EditorSearchResult));
        int num=eaSize(&searchResults)+1;
        memcpy(esr->path,path,sizeof(esr->path));
        sprintf(esr->name,"%d. %s",num,tracker->def->name);
        eaPush(&searchResults,esr);
        return 1;
    }
    if (tracker->def->count && tracker->entries==NULL) {
        trackerOpenEdit(tracker);
        opened=1;
    }
    if (uid>=0)
        uid++;
    for (i=0;i<tracker->count;i++) {
        path[depth+1]=i;
        found+=searchForTrackers(&tracker->entries[i],uid,suid,str,path,depth+1);
        if (uid>=0 && tracker->entries[i].def)
            uid+=1+tracker->entries[i].def->recursive_count;
    }
    path[depth+1]=-1;
    if (opened)
        trackerCloseEdit(tracker);
//    else
        //if (found>0)
        //tracker->edit=1;

    return found;
}

void searchResultsCallback(void* notUsed) {
    EditorSearchResult * esr=searchResults[searchIndex];
    if(editorSearch.searchType==0 && editorSearch.searchLocation==2){//display clicked object added by HA as requested by Don Pham 4/11/12
        if(    esr->path[0] < objectLibraryCount()){
                libScrollCallback(objectLibraryNameFromIdx(esr->path[0]),0);
                edit_state.setview=1;
        }
    }else//end addition by HA as requested by Don Pham 4/11/12
    {
        DefTracker * tracker=group_info.refs[esr->path[0]];
        int i=1;
        if (tracker->def==NULL)
            return;
        selAdd(tracker,trackerRootID(tracker),1,0);
        edit_state.last_selected_tracker=tracker;
        while (tracker->def!=NULL && esr->path[i]!=-1) {
            editCmdOpen();
            tracker=&tracker->entries[esr->path[i]];
            selAdd(tracker,trackerRootID(tracker),1,0);
            edit_state.last_selected_tracker=tracker;
            i++;
        }        
        edit_state.setview=1;
    }
}

int searchResultWindowID;

void editCmdSearchCallback(char * str) {
    int i;
    int path[100];
    memset(path,-1,100*4);
    if (strcmp(str,"Search")==0) {
        eaDestroyEx(&searchResults,NULL);
        if (editorSearch.searchLocation==0) {    // Everywhere
            int uid=0;
            for (i=0;i<group_info.ref_count;i++) {
                path[0]=i;
                if (!group_info.refs[i]->tricks || !(group_info.refs[i]->tricks->flags1 & TRICK_HIDE))
                    searchForTrackers(group_info.refs[i],uid,atoi(editorSearch.searchString),editorSearch.searchString,path,0);
                if (group_info.refs[i]->def)
                    uid+=1+group_info.refs[i]->def->recursive_count;
            }
        } else
        if (editorSearch.searchLocation==1) {    // Current Selection
            for (i=0;i<sel_count;i++) {
                path[0]=i;
                searchForTrackers(sel_list[i].def_tracker,-1,atoi(editorSearch.searchString),editorSearch.searchString,path,0);
            }
        }else
        if (editorSearch.searchType==0 && editorSearch.searchLocation==2) {    // object Library: added by HA as requested by Don Pham 4/11/12
            int count=objectLibraryCount();
            for(i=0;i<count;i++){
                char *objLibName=objectLibraryNameFromIdx(i);
                path[0]=i;
                if(objLibName && strstriConst(objLibName,editorSearch.searchString)!=NULL){                
                    EditorSearchResult * esr=malloc(sizeof(EditorSearchResult));
                    int num=eaSize(&searchResults)+1;
                    memcpy(esr->path,path,sizeof(esr->path));
                    sprintf(esr->name,"%d. %s",num,objLibName);
                    eaPush(&searchResults,esr);
                }
            }//end addition by HA as requested by Don Pham 4/11/12
        } else        //err?
            return;
    } else
    if (strcmp(str,"Done")==0) {
        editorUIDestroyWindow(editorSearch.searchWindow);
        editorSearch.searchWindow=-1;
        return;
    }
    {
        static int firstPass=1;
        char buf[256];
        if (!firstPass)
            editorUIDestroyWindow(searchResultWindowID);
        firstPass=0;
        sprintf(buf,"Search Results: %d Hit%c",eaSize(&searchResults),eaSize(&searchResults)==1?' ':'s');
        searchResultWindowID=editorUICreateWindow(buf);
//        int i;
//        for (i=0;i<eaSize(&results);i++)
//            eaPush(&names,results[i]->def->name);
        sprintf(buf,"%d Hit%c",eaSize(&searchResults),eaSize(&searchResults)==1?' ':'s');
        editorUIAddListFromEArray(searchResultWindowID,&searchIndex,buf,searchResultsCallback,(char **)searchResults);
    }
}

void editCmdSearch() {
    char buf[TEXT_DIALOG_MAX_STRLEN];
    int i;
    int len;
    edit_state.search=0;
    editorSearch.searchWindow=editorUICreateWindow("Search");
    editorUIAddComboBox(editorSearch.searchWindow,&editorSearch.searchType,"Search For:",NULL,"Object Names","Property Names","Property Values","UIDs",0);    
    //editorUIAddComboBox(editorSearch.searchWindow,&editorSearch.searchLocation,"Search within:",NULL,"Everywhere","Current Selection",0); //before HA changes as requested by Don Pham 4/11/12
    editorUIAddComboBox(editorSearch.searchWindow,&editorSearch.searchLocation,"Search within:",NULL,"Everywhere","Current Selection","Object Library",0);//object Library added by HA as requested by Don Pham 4/11/12
    editorSearch.partialMatches=1;
    editorUIAddCheckBox(editorSearch.searchWindow,&editorSearch.partialMatches,NULL,"Partial Matches");
    editorUIAddTextEntry(editorSearch.searchWindow,editorSearch.searchString,256,NULL,"Search String:");
    editorUIAddButtonRow(editorSearch.searchWindow,NULL,"Search",editCmdSearchCallback,"Done",editCmdSearchCallback,NULL);
    return;
    buf[0]=0;
    if( !winGetString("Enter name to search for.", buf) || !buf[0] )
        return;
    unSelect(1);
    len=(int)strlen(buf);
//    for (i=0;i<len;i++)
//        if (buf[i]>='A' && buf[i]<='Z')
//            buf[i]+='a'-'A';
    for (i=0;i<group_info.ref_count;i++) {
        if (group_info.refs[i]->tricks!=NULL && (group_info.refs[i]->tricks->flags1 & TRICK_HIDE))
            continue;
        if (group_info.refs[i]==sel.activeLayer || sel.activeLayer==NULL)
            selectByName(group_info.refs[i],i,buf);
    }

}

void selectUnhashed(StashTable selected,StashTable ancestors,DefTracker * tracker,int layer) {
    if (tracker==NULL) return;
    if (stashAddressFindInt(selected,tracker, NULL)) return;
    if (!stashAddressFindInt(ancestors,tracker,NULL)) {
        selAdd(tracker,layer,2,EDITOR_LIST);
    } else {
        int i;
        for (i=0;i<tracker->count;i++)
            selectUnhashed(selected,ancestors,&tracker->entries[i],layer);
    }
}

void editCmdInvertSelection() {
    StashTable selected = stashTableCreateAddress(5000);
    StashTable ancestors = stashTableCreateAddress(5000);
    int i;

    //first hash all the (non-NULL) currently selected DefTrackers
    for (i=0;i<sel_count;i++) {
        DefTracker * tracker=sel_list[i].def_tracker;
        stashAddressAddInt(selected, tracker, 1, false);
        tracker=tracker->parent;
        while (tracker!=NULL) {
            stashAddressAddInt(ancestors, tracker, 1, false);
            tracker=tracker->parent;
        }
    }
    //unselect everything
    unSelect(1);

    //now go and select every DefTracker that has no descendant in the selected hash table
    //and make sure to only select things that are in the active layer
    for (i=0;i<group_info.ref_count;i++)
        if (group_info.refs[i]==sel.activeLayer || sel.activeLayer==NULL)
            selectUnhashed(selected,ancestors,group_info.refs[i],i);

    stashTableDestroy(selected);
    stashTableDestroy(ancestors);
    edit_state.invertSelection=0;
}

void editCmdGroundSnap()
{
    int        i,hit;
    Mat4    mat,matx,m;
    Vec3    start,end;
    CollInfo    coll;
    extern DefTracker    sel_refs[];
    extern int sel_ref_count;

    if (sel.parent)
    {
        mulMat4(sel.parent->mat,sel.offsetnode->mat,matx);
        mulMat4(matx,sel.scalenode->mat,mat);
        for(i=0;i<sel_ref_count;i++)
        {
            mulMat4(mat,sel_refs[i].mat,m);

            copyVec3(m[3],start);
            copyVec3(m[3],end);
            start[1] += 1;
            end[1] -= 1000;
            hit = (collGrid(0,start,end,&coll,0,g_edit_collide_flags | COLL_DISTFROMSTART));
            if (hit)
            {
                coll.mat[3][1] += edit_state.groundoffset;
                if (edit_state.slopesnap)
                {
                    Mat4    mx,orig_yaw,negate_yaw,mz;
                    Vec3    pyr;

                    getMat3YPR(coll.mat,pyr);
                    pyr[0] = pyr[2] = 0;
                    pyr[1] = -pyr[1];
                    createMat3RYP(negate_yaw,pyr);

                    getMat3YPR(sel.offsetnode->mat,pyr);
                    pyr[0] = pyr[2] = 0;
                    createMat3RYP(orig_yaw,pyr);

                    mulMat3(coll.mat,orig_yaw,mx);
                    mulMat3(mx,negate_yaw,mz);
                    copyMat3(mz,sel.offsetnode->mat);
                    //copyMat3(coll.mat,sel.offsetnode->mat);
                    sel.offsetnode->mat[3][1] -= m[3][1] - coll.mat[3][1];
                    //sel_list[i].mat[3][1] -= m[3][1] - coll.mat[3][1];
                    //sel.parent->mat[3][1] -= mat[3][1] - coll.mat[3][1];
                    //copyMat3(unitmat,sel.offsetnode->mat);
                }
                else
                {
                    sel.offsetnode->mat[3][1] -= m[3][1] - coll.mat[3][1];
                }
            }
        }
    }
    edit_state.slopesnap = edit_state.groundsnap = 0;
}

void editCmdSetQuickPlacementObject()
{
    edit_state.setQuickPlacementObject = 0;
    if(sel_count == 1){
        if(sel_list[0].def_tracker && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->name){
            strcpy(edit_state.quickPlacementObject, sel_list[0].def_tracker->def->name);
            
            if(edit_state.quickPlacement)
                copyMat3(sel_list[0].def_tracker->mat, edit_state.quickPlacementMat3);
            else
                copyMat3(unitmat, edit_state.quickPlacementMat3);
        }
    }
    if (strcmp(edit_state.quickPlacementObject,"Patrol_Circle")==0 ||
        strcmp(edit_state.quickPlacementObject,"Patrol_OneWay")==0 ||
        strcmp(edit_state.quickPlacementObject,"Patrol_PingPong")==0 )
        strcpy(edit_state.quickPlacementObject,"Patrol_2");
    else {
        int d;
        sscanf(edit_state.quickPlacementObject,"Patrol_%d",&d);
        if (d>=2 && d<20)
            sprintf(edit_state.quickPlacementObject,"Patrol_%d",++d);
    }
}

void editCmdSelect(DefTracker *tracker)
{
    edit_state.sel = 0;
    if (inpFocus() && tracker && !sel.parent && 
        !((edit_state.lightsize || edit_state.soundsize || 
        edit_state.fogsize || edit_state.beaconSize || 
        edit_state.editPropRadius) && sel_count))
    {
        edit_state.last_selected_tracker = edit_state.tracker_under_mouse;

        selAdd(tracker,trackerRootID(tracker),1,0);
    }
}

void editCmdSelectAll()
{
    DefTracker    **trackers,*tracker;
    int            count,i;

    edit_state.selectall = 0;

    if (sel_count)
        unSelect(1);
    else
    {
        trackers = group_info.refs;
        count = group_info.ref_count;
        for(i=0;i<count;i++)
        {
            tracker = trackers[i];
            if (tracker->def)
                selAdd(tracker,trackerRootID(tracker),1,0);
        }
    }
}

static void selectObjectInstances(DefTracker *tracker, Model *model, int layer)
{
    int i;

    if (!tracker)
        return;

    if (tracker->def && tracker->def->model == model)
        selAdd(tracker, layer, 0, 0);

    for (i = 0; i < tracker->count; ++i)
        selectObjectInstances(&tracker->entries[i], model, layer);
}

void editCmdSelectAllInstancesOfThisObject()
{
    edit_state.selectAllInstancesOfThisObject = 0;

    if (sel_count == 1 && sel_list[0].def_tracker && sel_list[0].def_tracker->def && 
        sel_list[0].def_tracker->def->model)
    {
        Model *model = sel_list[0].def_tracker->def->model;
        int layer;

        for(layer=0; layer < group_info.ref_count; ++layer)
            selectObjectInstances(group_info.refs[layer], model, layer);
    }
}

int copyProperties(StashElement e)
{
    const char * key  =stashElementGetStringKey(e);
    void * value=stashElementGetPointer(e);
    strcpy(edit_state.propertyNameClipboard[edit_state.propertyClipboardSize],key);
    memcpy(&edit_state.propertyValueClipboard[edit_state.propertyClipboardSize],value,sizeof(PropertyEnt));
    edit_state.propertyClipboardSize++;
    return 1;
}

void editCmdCopyProperties() {
    int numProperties;

    edit_state.copyProperties=0;
    if (sel_count==0 || sel_list[0].def_tracker->def==NULL || sel_list[0].def_tracker->def->properties==NULL)
        return;

    numProperties=stashGetValidElementCount(sel_list[0].def_tracker->def->properties);
    if (numProperties==0)
        return;

    edit_state.propertyClipboardSize=0;
    stashForEachElement(sel_list[0].def_tracker->def->properties,copyProperties);
};

void editCmdPasteProperties() {
    int i;
    DefTracker * tracker;
    edit_state.pasteProperties=0;
    if (sel_count==0)
        return;
    tracker=sel_list[0].def_tracker;
    if (tracker->def==NULL)
        return;
    if (edit_state.propertyClipboardSize==0)
        return;
    if (tracker->def->properties==NULL) {
        tracker->def->properties = stashTableCreateWithStringKeys(16,  StashDeepCopyKeys);
        tracker->def->has_properties = 1;
    }
    for (i=0;i<edit_state.propertyClipboardSize;i++) {
        PropertyEnt * property;
        stashRemovePointer(tracker->def->properties,edit_state.propertyNameClipboard[i], &property);
        if (property!=NULL)
            destroyPropertyEnt(property);
        property=createPropertyEnt();
        strcpy(property->name_str,edit_state.propertyValueClipboard[i].name_str);
        estrPrintEString(&property->value_str, edit_state.propertyValueClipboard[i].value_str);
        stashAddPointer(tracker->def->properties,property->name_str,property, false);
    }

    editUpdateTracker(sel_list[0].def_tracker);
};

void editCmdCutCopy()
{
    if (sel_count && isNonLibSelect(0,WARN_CHECKPARENT))
    {
        sel.fake_ref.def = 0;
        sel.cut = edit_state.cut;
        selCopy(NULL);
    }
    edit_state.cut = edit_state.copy = 0;
}

void editCmdPaste()
{
    edit_state.paste = 0;
    if (sel_count==0 && sel.fake_ref.def==NULL)
        return;
    if (isNonLibSelect(0,WARN_CHECKATTACH))
        editPaste();
    if (strcmp(edit_state.quickPlacementObject,"Patrol_Circle")==0 ||
        strcmp(edit_state.quickPlacementObject,"Patrol_OneWay")==0 ||
        strcmp(edit_state.quickPlacementObject,"Patrol_PingPong")==0 )
        strcpy(edit_state.quickPlacementObject,"Patrol_2");
    else {
        int d;
        sscanf(edit_state.quickPlacementObject,"Patrol_%d",&d);
        if (d>=2 && d<20)
            sprintf(edit_state.quickPlacementObject,"Patrol_%d",++d);
    }
    sel.fake_ref.def = 0;
}

void editCmdDelete()
{
    int i;
    edit_state.del = 0;
    if (!isNonLibSelect(0,WARN_CHECKPARENT))
        return;
    for (i=0;i<sel_count;i++)
        if (sel_list[i].def_tracker->def->name[0]=='_' && sel_list[i].def_tracker->def->count==0 && sel_list[i].def_tracker->parent!=NULL)
            break;
    if (i!=sel_count) {
        char buf[256];
        sprintf(buf,"You are about to delete a lowest-level underscore piece (%s).  Are you sure you want to do this?",sel_list[i].def_tracker->def->name);
        if (!winMsgYesNo(buf))
            return;
    }
    editDelete();
}

void editCmdLibDelete()
{
GroupDef    *def;
char        buf[1000];
int            i,count;

    sprintf(buf,"Warning! Deleting a library piece could that's still used can cause lots of problems.\n OK to permanently delete library piece?");
    if (winMsgYesNo(buf))
    {
        for (i=0;i<sel_count;i++)
        {
            def = sel_list[i].def_tracker->def;
            count = groupDefRefCount(def);
            if (count > 1)
            {
                sprintf(buf,"Cant delete %s from library because you have %d references to it in your map.",def->name,count);
                winMsgAlert(buf);
                continue;
            }
            def->in_use = 0;
            editUpdateDef(def,0,0);
        }
        editDelete();
    }
    edit_state.libdel = 0;
}

//////////////////////////////////////////////////////////////////////////

static int totaltrisfound;
static void editFoundVisible(DefTracker *tracker, Model *model, MiniTracker *minitracker)
{
    int i, num, revpath[100], revpathcount=0;
    EditorSearchResult *esr;
    DefTracker *dtrack;

    if (!tracker || !model)
        return;

    esr = malloc(sizeof(EditorSearchResult));

    for (dtrack = tracker; dtrack->parent; dtrack = dtrack->parent)
    {
        for (i = 0; i < dtrack->parent->count; i++)
        {
            if (&dtrack->parent->entries[i] == dtrack)
            {
                revpath[revpathcount++] = i;
                break;
            }
        }
        assert(i < dtrack->parent->count);
    }
    for (i = 0; i < group_info.ref_count; i++)
    {
        if (group_info.refs[i] == dtrack)
        {
            revpath[revpathcount++] = i;
            break;
        }
    }
    assert(i < group_info.ref_count);

    for (i = 0; i < revpathcount; i++)
        esr->path[i] = revpath[revpathcount - i - 1];
    esr->path[revpathcount] = -1;

    num = eaSize(&searchResults)+1;
    totaltrisfound += model->tri_count;
    sprintf(esr->name, "%d. (%d) %s", num, model->tri_count, tracker->def->name);
    eaPush(&searchResults, esr);
}

typedef struct VisModel
{
    Model *model;
    BasicTexture *tex;
    int count;
} VisModel;

MP_DEFINE(VisModel);
static StashTable visible_models = 0;
static VisModel **visible_model_list = 0;

static VisModel *visCreate(Model *model, BasicTexture *tex)
{
    VisModel *vis;
    MP_CREATE(VisModel, 1024);
    vis = MP_ALLOC(VisModel);
    vis->model = model;
    vis->tex = tex;
    return vis;
}

static void visFree(void* visData)
{
    VisModel * vis = (VisModel *)visData;
    MP_FREE(VisModel, vis);
}

static void editTabulateVisible(DefTracker *tracker, Model *model, MiniTracker *mini_tracker)
{
    VisModel *vis;

    if (!model)
        return;

    if (game_state.search_visible == 3 || game_state.search_visible == 5)
    {
        int i, j, count = model->tex_count;
        TexBind **binds = model->tex_binds;
        if(mini_tracker && mini_tracker->tracker_binds[0])
        {
            binds = mini_tracker->tracker_binds[0];
            count = eaSize(&binds);
        }
        for (i = 0; i < count; i++)
        {
            TexBind *tex = binds[i];
            if (!tex)
                continue;

            for (j = 0; j < TEXLAYER_MAX_LAYERS; j++)
            {
                BasicTexture *btex = tex->tex_layers[j];
                if (!btex)
                    continue;

                if (!stashFindPointer(visible_models, btex->name, &vis))
                {
                    vis = visCreate(NULL, btex);
                    stashAddPointer(visible_models, btex->name, vis, false);
                    eaPush(&visible_model_list, vis);
                }

                vis->count++;
            }
        }
    }
    else
    {
        if (!stashFindPointer(visible_models, model->name, &vis))
        {
            vis = visCreate(model, NULL);
            stashAddPointer(visible_models, model->name, vis, false);
            eaPush(&visible_model_list, vis);
        }

        vis->count++;
    }
}

static int visCmp(const void *a, const void *b)
{
    float t;
    VisModel *visA = *((VisModel **)a);
    VisModel *visB = *((VisModel **)b);

    t = visA->count - visB->count;
    if (!t)
        t = visA->model->radius - visB->model->radius;

    if (t < 0)
        return -1;
    return t > 0;
}

static int visCmp2(const void *a, const void *b)
{
    int t;
    VisModel *visA = *((VisModel **)a);
    VisModel *visB = *((VisModel **)b);

    t = visA->count - visB->count;

    if (!t)
        t = visA->tex->memory_use[0] - visB->tex->memory_use[0];

    return t;
}

static void displayTabulation(int for_textures)
{
    FILE *f = fopen("C:/search_tabulation.csv", "wt");
    if (!for_textures)
    {
        int i, total_models = 0, total_subs = 0;
        printf("\n\nCount, Radius, Model Name\n");
        fprintf(f, "Count, Radius, Model Name\n");
        qsortG(visible_model_list, eaSize(&visible_model_list), sizeof(visible_model_list[0]), visCmp);
        for (i = 0; i < eaSize(&visible_model_list); i++)
        {
            printf("%d, %.2f, %s\n", visible_model_list[i]->count, visible_model_list[i]->model->radius, visible_model_list[i]->model->name);
            fprintf(f, "%d, %.2f, %s\n", visible_model_list[i]->count, visible_model_list[i]->model->radius, visible_model_list[i]->model->name);
            total_models += visible_model_list[i]->count;
            total_subs += visible_model_list[i]->count * visible_model_list[i]->model->tex_count;
        }
        printf("Total Models: %d\n", total_models);
        fprintf(f, "Total Models: %d\n", total_models);
        printf("Total Subs: %d\n", total_subs);
        fprintf(f, "Total Subs: %d\n", total_subs);
        printf("Unique Models: %d\n", eaSize(&visible_model_list));
        fprintf(f, "Unique Models: %d\n", eaSize(&visible_model_list));
    }
    else
    {
        int i, total_textures = 0, total_memory = 0;
        printf("\n\nCount, Bytes Used, Texture Name\n");
        fprintf(f, "Count, Bytes Used, Texture Name\n");
        qsortG(visible_model_list, eaSize(&visible_model_list), sizeof(visible_model_list[0]), visCmp2);
        for (i = 0; i < eaSize(&visible_model_list); i++)
        {
            printf("%d, %d, %s\n", visible_model_list[i]->count, visible_model_list[i]->tex->memory_use[0], visible_model_list[i]->tex->name);
            fprintf(f, "%d, %d, %s\n", visible_model_list[i]->count, visible_model_list[i]->tex->memory_use[0], visible_model_list[i]->tex->name);
            total_textures += visible_model_list[i]->count;
            total_memory += visible_model_list[i]->tex->memory_use[0];
        }
        printf("Total Textures: %d\n", total_textures);
        fprintf(f, "Total Textures: %d\n", total_textures);
        printf("Unique Textures: %d\n", eaSize(&visible_model_list));
        fprintf(f, "Unique Textures: %d\n", eaSize(&visible_model_list));
        printf("Total Memory Usage: %d\n", total_memory);
        fprintf(f, "Total Memory Usage: %d\n", total_memory);
    }
    fclose(f);
}

static int editTabulateDef(GroupDef *def, DefTracker *tracker, Mat4 world_mat, TraverserDrawParams *draw)
{
    if (!def || def->editor_visible_only)
        return 0;

    if (def->model)
        editTabulateVisible(tracker, def->model, groupDrawGetMiniTracker(draw->uid, 0));

    return 1;
}

void editCmdTabulateAll(void)
{
    stashTableDestroy(visible_models);
    eaDestroyEx(&visible_model_list, visFree);
    visible_models = stashTableCreateWithStringKeys(2048, StashDeepCopyKeys);

    groupTreeTraverse(editTabulateDef, 0);

    displayTabulation(game_state.search_visible==5);
}

void editCmdSearchVisible(void)
{
    if (game_state.search_visible > 3)
    {
        editCmdTabulateAll();
        game_state.search_visible = 0;
        return;
    }

    if (group_draw.search_visible)
    {
        static int ID=-1;
        char buf[256];
        group_draw.search_visible = 0;
        group_draw.search_visible_callback = 0;

        if (game_state.search_visible > 1)
        {
            displayTabulation(game_state.search_visible==3);
        }
        else
        {
            editorUIDestroyWindow(ID);
            sprintf(buf,"Search Results: %d Hit%c, %d Tris",eaSize(&searchResults),eaSize(&searchResults)==1?' ':'s', totaltrisfound);
            ID=editorUICreateWindow(buf);
            sprintf(buf,"%d Hit%c, %d Tris",eaSize(&searchResults),eaSize(&searchResults)==1?' ':'s', totaltrisfound);
            editorUIAddListFromEArray(ID,0,buf,searchResultsCallback,(char **)searchResults);
        }

        game_state.search_visible = 0;
    }
    else
    {
        eaDestroyEx(&searchResults, 0);
        stashTableDestroy(visible_models);
        eaDestroyEx(&visible_model_list, visFree);
        visible_models = stashTableCreateWithStringKeys(2048, StashDeepCopyKeys);
        group_draw.search_visible = 1;
        group_draw.search_visible_callback = (game_state.search_visible == 1)?editFoundVisible:editTabulateVisible;
        totaltrisfound = 0;
    }
}

