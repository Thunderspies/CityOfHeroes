#include "group/group.h"
#include <utilitieslib/components/StashTable.h>
#include "edit/edit_cmd.h"
#include "group/groupProperties.h"
#include "win/input.h"
#include "cmdparse/cmdcommon.h"
#include "edit/edit_library.h"
#include "graphics/font.h"
#include "win/win_init.h"
#include <utilitieslib/utils/mathutil.h>
#include "graphics/camera.h"
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdgame.h"
#include "gridcoll/gridcoll.h"
#include "graphics/sun.h"
#include "entity/entDebug.h"
#include "edit_info.h"
#include "edit/edit_pickcolor.h"
#include "edit/edit_net.h"
#include "edit/edit_select.h"
#include "edit/edit_drawlines.h"
#include "edit_errcheck.h"
#include "edit/edit_select.h"
#include "edit/edit_cmd_file.h"
#include "edit_cmd_group.h"
#include "edit/edit_cmd_adjust.h"
#include "edit/edit_cmd_select.h"
#include "gridcoll/gridcache.h"
#include "UI/uiInput.h"
#include "UI/uiScrollBar.h"
#include <utilitieslib/utils/timing.h>
#include "edit/MapGenerator.h"
#include "edit_context_menu.h"
#include "editorUI.h"
#include "edit_transformToolbar.h"

GroupDef    save_def;
StashTable    save_properties;
int            properties_saved;

char    map_loadsave_name[1024];

static int select_took_focus;

extern Menu * libraryMenu;
extern Menu * commandMenu;
extern EditListView * trackerListView;

static void saveCurrData(void)
{
    if (sel_count)
        save_def = *sel_list[0].def_tracker->def;

    if(save_def.has_properties)
    {
        properties_saved = 1;
        if(!save_properties)
            save_properties = stashTableCreateWithStringKeys(4, StashDeepCopyKeys);
        else
            stashTableClearEx(save_properties, NULL, destroyPropertyEnt);

        copyPropertyStashTable(save_def.properties, save_properties);
    }
    else
    {
        properties_saved = 0;
    }
}

static void calcDefaultMapName()
{
    char    *s=0,buf[1000] = "n:/game/data/maps/test.txt";

    if (game_state.world_name[0]) {
        s = fileLocateWrite(game_state.world_name, buf);
    }
    if (!s)
    {
        s = fileLocateWrite_s("maps/", NULL, 0);
        if (s) {
            sprintf(buf,"%stest.txt",s);
            fileLocateFree(s);
        }
    }
    Strcpy(map_loadsave_name, buf);
    for(s=map_loadsave_name;*s;s++)
    {
        if (*s == '/')
            *s = '\\';
    }

}

static int editHousekeeping(int *lost_focus_ptr)
{
    int    took_focus,lost_focus;

    PERFINFO_AUTO_START("editHousekeeping", 1);

    if (edit_state.select_notselectable)
        g_edit_collide_flags |= COLL_NOTSELECTABLE;
    else
        g_edit_collide_flags &= ~COLL_NOTSELECTABLE;

    PERFINFO_AUTO_START("editResetCmdChecker", 1);

    editResetCmdChecker();
    if (sel_count && !edit_state.lightsize && !edit_state.soundsize && !edit_state.fogsize && !edit_state.beaconSize && !edit_state.editPropRadius)
    {
        PERFINFO_AUTO_START("saveCurrData", 1);
        saveCurrData();
    }
    took_focus = edit_state.look | edit_state.zoom | edit_state.pan;
    inpLockMouse(took_focus);
    if (control_state.ignore)
        took_focus = 0;
    if (!map_loadsave_name[0])
    {
        PERFINFO_AUTO_STOP_START("calcDefaultMapName", 1);
        calcDefaultMapName();
    }

    PERFINFO_AUTO_STOP_START("editShowLibrary", 1);

    lost_focus = editShowLibrary(took_focus | select_took_focus | !GetFocus());

    if (lost_focus)
        edit_state.zoom = 0;
    if (sel.lib_load)
    {
        xyprintf(30,20,"LOADING LIBS");
    }
    if (editIsWaitingForServer())
    {
        PERFINFO_AUTO_STOP_START("Sleep(1)", 1);
        Sleep(1);
        PERFINFO_AUTO_STOP();
        return 0;
    }
    if (sel.lib_load)
    {
        PERFINFO_AUTO_STOP_START("libScrollCallback", 1);
        libScrollCallback(sel.lib_name,0);
    }
    *lost_focus_ptr = lost_focus;

    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_STOP();

    return 1;
}

void editCancelTextureSwap(MenuEntry * me,ClickInfo * ci) {
    edit_state.textureSwap=0;
    edit_state.textureSwapRandom=0;
    edit_state.textureSwapRemove=0;
    if (edit_state.textureSwapTracker!=NULL)
        selAdd(edit_state.textureSwapTracker,trackerRootID(edit_state.textureSwapTracker),2,EDITOR_LIST);
}

void editRemoveTextureSwap(MenuEntry * me,ClickInfo * ci) {
    if (edit_state.textureSwapRemove) {
        edit_state.textureSwapRemove=0;
        estrPrintCharString(&me->name, "Adding Swaps (click here to remove swaps)");
    } else {
        edit_state.textureSwapRemove=1;
        estrPrintCharString(&me->name, "Removing Swaps (click here to add swaps)");
    }
}

void editRandomizeTextureSwap(MenuEntry * me,ClickInfo * ci) {
    if (edit_state.textureSwapRandom==0) {
        edit_state.textureSwapRandom=1;
        estrPrintCharString(&me->name, "New Swaps are Random (click here to change)");
    } else {
        edit_state.textureSwapRandom=0;
        estrPrintCharString(&me->name, "New Swaps are Not Random (click here to change)");
    }
}

void destroyableObjectOnClose() {
    edit_state.destroyableObjectController=0;
    edit_state.destroyableObjectControllerID=0;
}

void destroyableObjectDone() {
    editorUIDestroyWindow(edit_state.destroyableObjectControllerID);
}

void destroyableObjectPlay(char * str) {
    edit_state.destroyableObjectPlay^=1;
}

void destroyableObjectSetObject() {
    if (sel_count!=1) return;
    edit_state.destroyableObject=sel_list[0].def_tracker;
}

void editProcess()
{
    DefTracker *tracker;
    Vec3    start,end;
    int        lost_focus;
    int        old_cache_disabled;
    static ScrollBar * scrollBar;
    static Menu * textureMenu;

    if(entDebugMenuVisible())
        edit_state.sel = 0;

    if (!editHousekeeping(&lost_focus))
    {
        return;
    }

    PERFINFO_AUTO_START("mouseOverSel", 1);

    old_cache_disabled = gridSetCacheDisabled(1);
    tracker = mouseOverSel(start,end,lost_focus);
    gridSetCacheDisabled(old_cache_disabled);

    PERFINFO_AUTO_STOP_START("file", 1);

    // file
    if (edit_state.load || edit_state.import)
        editLoad(winGetFileName("*.txt",map_loadsave_name,0),edit_state.import);
    if (edit_state.editstate_new)
        editNew();
    if (edit_state.savelibs)
        editSaveLibs();
    if (edit_state.save || edit_state.saveas || edit_state.savesel)
        editCmdSaveFile();
    if (edit_state.exportsel)
        editCmdExportToVrml();
    if (edit_state.clientsave)
        editCmdClientSave();
    if (edit_state.autosave)
        editCmdAutosave(NULL, NULL);
    else
        edit_state.autosavewait=0;
    if (edit_state.scenefile)
        editCmdSceneFile();
    if (edit_state.loadscreen)
        editCmdLoadScreen();
    /*MW removed because nobody uses it, and I think it's broken
    if (edit_state.checkin)
    editCmdCheckin();*/

    PERFINFO_AUTO_STOP_START("copy/paste", 1);

    if (edit_state.copyProperties)
        editCmdCopyProperties();
    if (edit_state.pasteProperties)
        editCmdPasteProperties();

    if (edit_state.profiler)
        editProfiler();

    PERFINFO_AUTO_STOP_START("group", 1);

    // group
    if (edit_state.ungroup)
        editCmdUngroup();
    if (edit_state.group)
        editCmdGroup();
    if (edit_state.attach)
        editCmdAttach();
    if (edit_state.detach)
        editCmdDetach();
    if (edit_state.open)
        editCmdOpen();
    if (edit_state.close || edit_state.closeinst)
        editCmdClose();
    if (edit_state.mousewheelopenclose)
        editCmdMouseWheelOpenClose();
    if (edit_state.setparent)
        editCmdSetParent();
    if (edit_state.setactivelayer)
        editCmdSetActiveLayer();
    if (edit_state.name)
        editName();
    if (edit_state.makelibs)
        editNameNewLibraryPiece();
    if (edit_state.settype || edit_state.setfx)
        editCmdSetFxName();
    if (edit_state.ungroupall)
        editCmdUngroupAll();
    if (edit_state.groupall)
        editCmdGroupAll();
    if (edit_state.trayswap)
        editCmdTraySwap();
    if (edit_state.toggleMenu) {
        edit_state.useOldMenu=0;
    }

    PERFINFO_AUTO_STOP_START("hideMenu", 1);

    if (edit_state.hideMenu) {
        hideAllELV();
        edit_state.hideMenu=0;
    }

    PERFINFO_AUTO_STOP_START("group->LOD", 1);

    // group->LOD
    if (edit_state.lodscale || edit_state.lodfar || edit_state.lodfarfade)
        editCmdSetLod();
    if (edit_state.loddisable)
        editCmdLodDisable();
    if (edit_state.lodenable)
        editCmdLodEnable();
    if (edit_state.lodfadenode)
        editCmdLodFadeNode();
    if (edit_state.lodfadenode2)
        editCmdLodFadeNode2();

    if (edit_state.nofogclip)
        editCmdNoFogClip();
    if (edit_state.nocoll)
        editCmdNoColl();

    PERFINFO_AUTO_STOP_START("beacons", 1);

    // adjust->beacons
    if (edit_state.beaconName)
        editCmdBeaconName();
    if (edit_state.beaconSize)
        editCmdBeaconSize();
    if (edit_state.showBeaconConnection)
        editCmdShowBeaconConnection();
    if (edit_state.showBeaconPathStart)
        editCmdShowBeaconPath(1);
    else if (edit_state.showBeaconPathContinue)
        editCmdShowBeaconPath(0);

    PERFINFO_AUTO_STOP_START("adjust/light", 1);

    if (edit_state.unsetambient)
        editCmdUnsetAmbient();

    // adjust->light
    if (edit_state.setambient || edit_state.lightcolor || edit_state.maxbright) {
        if (edit_state.lightHandle)
        {
            if (!editIsWaitingForServer())
            {
                if (sel_count==0)
                    selAdd(trackerFromHandle(edit_state.lightHandle), edit_state.lightHandle->ref_id, 1, EDITOR_LIST);
            }
            editCmdLightColor();
        }
        else
            editCmdLightColor();
    }

    if (edit_state.lightsize)
        editCmdLightSize();

    PERFINFO_AUTO_STOP_START("adjust/cubemap", 1);

    if (edit_state.adjustCubemap)
        editCmdAdjustCubemap();

    PERFINFO_AUTO_STOP_START("adjust/fog", 1);

    // adjust->fog
    if (edit_state.fogcolor1 || edit_state.fogcolor2)
        editCmdFogColor();
    if (edit_state.fognear || edit_state.fogfar)
        getFogDist();
    if (edit_state.fogsize)
        getFogSize();
    if (edit_state.fogspeed)
        getFogSpeed();

    PERFINFO_AUTO_STOP_START("adjust/tinttex", 1);

    // adjust->tinttex
    if (edit_state.tintcolor1 || edit_state.tintcolor2)
        editCmdTintColor();
    if (edit_state.removetex)
        editCmdRemoveTex();
    if (edit_state.replacetex1 || edit_state.replacetex2)
        editCmdReplaceTex();

    {
        static int x=400;
        static int y=50;
        static int width=400;
        static int height=400;
        static int scrolled=0;
        //we don't want to bring up the texture menu unless you have a single non-library piece selected,
        //and we prevent them from cleverly switching what object is selected by checking the currently
        //selected object against the textureSwapTracker and making sure they are the same
        if (edit_state.textureSwap) {
            if ((sel_count!=1 && textureMenu!=NULL) || !isNonLibSingleSelect(0,0) || (textureMenu!=NULL && sel_list[0].def_tracker!=edit_state.textureSwapTracker)) {
                editCancelTextureSwap(0,0);
            } else {
                if (textureMenu==NULL) {
                    textureMenu=newMenu(x,y,width,height,"Pick a texture to swap");
                    edit_state.textureSwapTracker=sel_list[0].def_tracker;
                    edit_state.textureSwapModTime=sel_list[0].def_tracker->def_mod_time;
                    addEntryToMenu(textureMenu,"Done",editCancelTextureSwap,NULL);
                    if (edit_state.textureSwapRandom)
                        addEntryToMenu(textureMenu,"New Swaps are Random (click here to change)",editRandomizeTextureSwap,0);
                    else
                        addEntryToMenu(textureMenu,"New Swaps are Not Random (click here to change)",editRandomizeTextureSwap,0);
                    if (edit_state.textureSwapRemove)
                        addEntryToMenu(textureMenu,"Removing Swaps (click here to add swaps)",editRemoveTextureSwap,NULL);
                    else
                        addEntryToMenu(textureMenu,"Adding Swaps (click here to remove swaps)",editRemoveTextureSwap,NULL);
                    addEntryToMenu(textureMenu," ",NULL,NULL);
                    editCmdAddBasicTexturesToMenu(textureMenu,edit_state.textureSwapTracker);
                    editCmdAddCompositeTexturesToMenu(textureMenu,edit_state.textureSwapTracker);
                    //open up the menus for those lazy artists :-P
                    {
                        MenuEntry * me=textureMenu->root->child;
                        while (me!=NULL) {
                            if (strcmp(me->name,"Textures"))
                                me->opened=1;
                            if (strcmp(me->name,"Materials"))
                                me->opened=1;
                            me=me->sibling;
                        }
                        textureMenu->root->opened=1;
                    }
                    textureMenu->lv->amountScrolled=scrolled;
                }
            }
        }
        if ((!edit_state.textureSwap) || (!edit_state.textureSwapTracker || !edit_state.textureSwapTracker->def)) {
            if (textureMenu && textureMenu->lv) {
                x=textureMenu->lv->x;
                y=textureMenu->lv->y;
                width=textureMenu->lv->width;
                height=textureMenu->lv->height;
                scrolled=textureMenu->lv->amountScrolled;
            }
            if (textureMenu!=NULL)
                destroyMenu(textureMenu);
            textureMenu=0;
            if (edit_state.textureSwapTracker==sel_list[0].def_tracker && sel_list[0].def_tracker &&
                edit_state.textureSwapModTime<sel_list[0].def_tracker->def_mod_time) {
                    if (edit_state.textureSwapReopen) {
                        edit_state.textureSwapReopen=0;
                        edit_state.textureSwap=1;
                    } else {
                        edit_state.textureSwapRemove=0;
                        edit_state.textureSwapRandom=0;
                        edit_state.textureSwapReopen=0;
                    }
                }
        }
    }

    mapGeneratorUpkeep(&edit_state.mapGeneration);



    PERFINFO_AUTO_STOP_START("adjust/sound", 1);

    // adjust->sound
    if (edit_state.soundname)
        editCmdSoundName();
    if (edit_state.soundscript)
        editCmdSoundScript();
    if (edit_state.soundvol)
        editCmdSoundVol();
    if (edit_state.soundramp)
        editCmdSoundRamp();
    if (edit_state.soundsize)
        editCmdSoundSize();
    if (edit_state.soundfind)
        editCmdSoundFind();
    if (edit_state.soundexclude)
        editCmdSoundExclude();

    if (edit_state.boxscale)
        editCmdBoxScale();

    PERFINFO_AUTO_STOP_START("adjust/properties", 1);

    // adjust->properties
    if (edit_state.addProperty)
    {
        extern void addPropertyCreateMenu(void *,void *);
        addPropertyCreateMenu(NULL,NULL);
    }

    PERFINFO_AUTO_STOP_START("select", 1);

    if (game_state.transformToolbar)
        editDrawTransformToolbar(&lost_focus);

    // select
    if (edit_state.useObjectAxes && (edit_state.sel || (edit_state.objectAxesMovement && isDown(MS_LEFT))))
        edit_state.objectAxesMovement=1;
    else
        edit_state.objectAxesMovement=0;
    if (edit_state.lassoAdd || edit_state.lassoInvert || edit_state.lassoExclusive)
        edit_state.drawingLasso=1;
    if (edit_state.drawingLasso)
        editCmdLasso();
    if (edit_state.sel)                //this function steals edit_state.sel, which is why lassoMode is before it
        editCmdSelect(tracker);
    if (edit_state.selectall)
        editCmdSelectAll();
    if (edit_state.selectAllInstancesOfThisObject)
        editCmdSelectAllInstancesOfThisObject();
    if (edit_state.setview )    // special behavior in the mission editor
        editCmdSnapDist();
    if (edit_state.unsel)
        unSelect(0);
    if (edit_state.selcontents)
        editSelContents();
    if (edit_state.setpivot)
        editCmdSetPivot();
    if (edit_state.hide)
        editCmdHide();
    if (edit_state.invertSelection)
        editCmdInvertSelection();
    if (edit_state.search)
        editCmdSearch();
    if (edit_state.freeze)
        editCmdFreeze();
    if (edit_state.unfreeze)
        editCmdUnfreeze();
    if (edit_state.freezeothers)
        editCmdFreezeOthers();
    if (edit_state.hideothers)
        editCmdHideOthers();
    if (edit_state.unhide)
        editCmdUnhide();
    if (edit_state.groundsnap || edit_state.slopesnap)
        editCmdGroundSnap();
    if (edit_state.setQuickPlacementObject)
        editCmdSetQuickPlacementObject();
    if (edit_state.toggleQuickPlacement)
    {
        edit_state.toggleQuickPlacement = 0;
        if(!edit_state.quickPlacement)
        {
            unSelect(0);
        }
        edit_state.quickPlacement = !edit_state.quickPlacement;
    }

    if (edit_state.useObjectAxes) {
        if (edit_state.objectAxesUnspecified) {
            if (sel_count>0) {
                copyMat4(sel_list[0].def_tracker->mat,edit_state.objectMatrix);
                invertMat4Copy(edit_state.objectMatrix,edit_state.objectMatrixInverse);
            }
            edit_state.objectAxesUnspecified=0;
        }
    } else {
        edit_state.objectAxesUnspecified=1;
    }

    if (edit_state.burningBuildingRepair) {
        edit_state.burningBuildingRepair=0;
        edit_state.burningBuildingRealRate=edit_state.burningBuildingSpeed;
        if (sel_count)
            edit_state.burningBuilding=sel_list[0].def_tracker;
    }

    if (edit_state.burningBuildingDestroy) {
        edit_state.burningBuildingDestroy=0;
        edit_state.burningBuildingRealRate=-edit_state.burningBuildingSpeed;
        if (sel_count)
            edit_state.burningBuilding=sel_list[0].def_tracker;
    }

    if (edit_state.destroyableObjectController) {
        if (!edit_state.destroyableObjectControllerID) {
            edit_state.destroyableObjectSpeed=0;
            edit_state.destroyableObject=NULL;
            edit_state.destroyableObjectControllerID=editorUICreateWindow("Destroyable Object");
            editorUISetCloseCallback(edit_state.destroyableObjectControllerID,destroyableObjectOnClose);
            editorUIAddSlider(edit_state.destroyableObjectControllerID,&edit_state.destroyableObjectHealth,0,100,0,NULL,"Health");
            editorUIAddSlider(edit_state.destroyableObjectControllerID,&edit_state.destroyableObjectSpeed,-100,100,0,NULL,"Destroy/Repair Speed");
            editorUIAddButtonRow(edit_state.destroyableObjectControllerID,NULL,"Set Destroyable Object",destroyableObjectSetObject,"Start/Stop",destroyableObjectPlay,"Done",destroyableObjectDone,NULL);
        }
        if (edit_state.destroyableObjectPlay) {
            StashElement e;
            if (edit_state.destroyableObject==NULL)
                return;
            edit_state.destroyableObjectHealth+=edit_state.destroyableObjectSpeed/100.0;
            if (!stashFindElement(edit_state.destroyableObject->def->properties,"Health", &e))
                return;
            estrPrintf(&((PropertyEnt *)stashElementGetPointer(e))->value_str,
                        "%f%c", edit_state.destroyableObjectHealth, (edit_state.destroyableObjectSpeed<0) ? '-' : '+');
        }
    }

    if (edit_state.burningBuildingSpeedChange)
        editCmdBurningBuildingSpeed();

    if (edit_state.burningBuildingRealRate)
        editCmdBurningBuildingModifyHealth();

    if (edit_state.burningBuildingStop) {
        edit_state.burningBuildingStop=0;
        edit_state.burningBuildingRealRate=0;
        edit_state.burningBuildingLastClockTick=0;
    }

    PERFINFO_AUTO_STOP_START("search", 1);

    //Search for places where this property is found
    if (edit_state.findNextProperty)
    {
        editCmdFindNextProperty(edit_state.findNextProperty);
        edit_state.findNextProperty = 0; //redundant?
    }

    PERFINFO_AUTO_STOP_START("edit", 1);

    // edit
    if (edit_state.copy || edit_state.cut)
        editCmdCutCopy();
    if (edit_state.paste)
        editCmdPaste();
    if (edit_state.del)
        editCmdDelete();
    if (edit_state.libdel)
        editCmdLibDelete();
    if (edit_state.undo)
        editUndo();
    if (edit_state.redo)
        editRedo();

    if (edit_state.reload) // MUST be after others (specifically Save and SaveLibs)
        editLoad(game_state.world_name, 0);

    PERFINFO_AUTO_STOP_START("debug things", 1);

    // various debug things
    select_took_focus = selUpdate(start,end,lost_focus);
    selShow();
    printEditStats();
    if (game_state.sound_info)
        editDrawSoundSpheres();

    {
        extern int collisions_off_for_rest_of_frame;
        int x, y;
        collisions_off_for_rest_of_frame = 0;
        displayCurrentContextMenu();
        if (rightClickCoords(&x, &y))
        {
            edit_state.drawingLasso = edit_state.lassoAdd = edit_state.lassoExclusive = edit_state.lassoInvert = 0;
            editContextMenuShow(x, y, !!inpLevel(INP_CONTROL));
        }
    }

    //JW: my own testing stuff goes here
    /*    {
    extern void soundDebugHelper(DefTracker *,int *,int);
    if (edit_state.soundDebug) {
    int line=0;
    soundDebugHelper(sel_list[0].def_tracker,&line,0);
    }
    }
    */

    PERFINFO_AUTO_STOP();
}

