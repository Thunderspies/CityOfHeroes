#ifndef _EDIT_CMD_H
#define _EDIT_CMD_H

#include <utilitieslib/stdtypes.h>
//#include <utilitieslib/utils/cmdoldparse.h>
#include "group/groupProperties.h"
#include "Menu.h"

typedef struct DefTracker DefTracker;
typedef struct TrackerHandle TrackerHandle;

typedef struct
{
    U32        *packet_ids;
    int        sel,selectall,selectAllInstancesOfThisObject,groundsnap,slopesnap,selcontents,del,
        libdel,group,ungroup,unsel,open,editorig,noerrcheck, close,closeinst,setparent,attach,name,
        setpivot,hide,hideothers,unhide,invertSelection,settype,setfx,search,setactivelayer,
        makelibs;
    int        clientsave;
    int        autosave;    //0 iff autosave is turned off
    int        autosavetime;    //the last time a save happened
    int        autosavewait;    //time between each save
    int        mousewheelopenclose;
    int        trayswap;
    F32        groundoffset;
    int        setambient,unsetambient,maxbright,lightcolor,lightsize,colorbynumber,colormemory;
    int        adjustCubemap;
    int        soundsize,soundvol,soundname,soundramp,soundfind,soundexclude,soundscript;
    int        fognear,fogfar,fogcolor1,fogcolor2,fogsize,fogspeed;
    int        gridsize,rotsize,plane,posrot,scale,zerogrid;
    int        polycount,canhide,ignoretray,objinfo;
    int        cut,copy,paste,undo,redo;
    int        showfog,showvis;
    int        snaptype,nosnap,snap3,snaptovert,singleaxis,localrot;
    int        look,pan,zoom,lookat,setview,camview;
    int        load,import,editstate_new,save,savesel,saveas,savelibs,scenefile,loadscreen,exportsel;
    int        lodscale,lodfar,lodfarfade,loddisable,lodenable,lodfadenode,lodfadenode2;
    int        showall;
    int        beaconName, beaconSize, showBeaconConnection, showBeaconPathStart, showBeaconPathContinue, beaconradii, shownBeaconType, beaconSelect;
    int        toggleQuickPlacement, quickPlacement, quickRotate, quickOrient, setQuickPlacementObject, selectEditOnly, openOnSelection;
    Mat3    quickPlacementMat3;
    char    quickPlacementObject[256];
    int        tintcolor1,tintcolor2,tintremove;
    int        replacetex1,replacetex2,removetex;
    int        addProperty, removeProperty, showPropertyAsRadius, showPropertyAsString, editPropRadius;
    int        ungroupall,groupall;
    int        checkin;
    int        select_notselectable;
    DefTracker*    tracker_under_mouse;
    DefTracker* last_selected_tracker;
    int        useOldMenu;
    int        toggleMenu;
    int        focusStolen;
    F32        mouseScale;
    int        isDirty;        // 1 iff there are unsaved changes
    int     findNextProperty; // next property to find
    int        lassoAdd;
    int        lassoExclusive;
    int        lassoInvert;
    int        drawingLasso;
    int        lassoX,lassoY;    //coordinates of the starting point of the lasso, (-1,-1) when not in lasso mode
    int        useObjectAxes;    //1 iff the axes for rotation and translation are now specified by objectMatrix
    int        objectAxesUnspecified;
    int        objectAxesMovement;
    Mat4    objectMatrix;    //see above
    Mat4    objectMatrixInverse;
    int        copyProperties;
    int        pasteProperties;
    char        propertyNameClipboard [256][256];    //stores the names of the properties copied
    PropertyEnt propertyValueClipboard[256];        //stores the corresponding values of those properties
    int        propertyClipboardSize;        //number of elements in the property clipboard
    int        propertyClipboardMaxSize;    //number of elements there is room for in the current clipboard
    int        hideMenu;
    int        reload;    // Reload the map you're on, for re-syncing
    double    burningBuildingSpeed;
    int        burningBuildingSpeedChange;
    double    burningBuildingRealRate;        //actual rate, after multiplier
    int        burningBuildingDestroy;
    int        burningBuildingRepair;
    MenuEntry    *burningBuildingMenuEntry;
    DefTracker    *burningBuilding;            //the building that is actually being destroyed or repaired
    int        burningBuildingLastClockTick;    //the last clock tick that the building's health was changed on
    int        burningBuildingStop;
    int        destroyableObjectController;
    int        destroyableObjectControllerID;
    float    destroyableObjectHealth;
    int        destroyableObjectRepairing;
    float    destroyableObjectSpeed;
    int        destroyableObjectPlay;
    DefTracker * destroyableObject;
    int        textureSwap;
    int        textureSwapReopen;
    DefTracker * textureSwapTracker;
    int        textureSwapRemove;
    int        textureSwapRandom;
    unsigned int        textureSwapModTime;
    int        profiler;                        //Conor's cool new gizmo
    int        cameraPlacement;
    int        liveTrayLink;
    int        detach;
    int        hideCollisionVolumes;
    int        freeze;
    int        unfreeze;
    int        freezeothers;
    int        soundDebug;
    int        showBounds;
    int        nofogclip;
    int        nocoll;
    int        mapGeneration;
    int        changedColor;        //gross
    TrackerHandle *lightHandle;
    int        nudgeLeft,nudgeRight,nudgeUp,nudgeDown;
    int        editorTrashing;
    int        boxscale;
    int        noServerUpdate;
} EditState;

extern EditState edit_state;

// Generated by mkproto
int editCmdParse(char *str, int x, int y);
void editStateInit();
int editMode();
void editSetMode(int on, int move);
void editCmdAddBasicTexturesToMenu(Menu * textureMenu,DefTracker * tracker);
void editCmdAddCompositeTexturesToMenu(Menu * textureMenu,DefTracker * tracker);
void editDumpImages();

// End mkproto
#endif
