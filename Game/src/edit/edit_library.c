#include <string.h>
#include <stdio.h>
#include "group/group.h"
#include "edit_uiscroll.h"
#include "graphics/font.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/mathutil.h>
#include "edit/edit_cmd.h"
#include "edit/edit_select.h"
#include "edit/edit.h"
#include "edit/edit_net.h"
#include "UI.h"
#include "UI/uiInput.h"
#include "group/groupproperties.h"
#include "win/win_init.h"
#include "group/groupfilelib.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include "edit_errcheck.h"
#include "edit/edit_drawlines.h"
#include "Menu.h"
#include "edit/edit_cmd_file.h"
#include "edit_cmd_group.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/MRUList.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include "filter/validate_name.h"
#include "MissionControl.h"
#include "edit/edit_library.h"
#include <utilitieslib/components/StashTable.h>
#include "win/input.h"

//dbg stuff
#include "gameComm/wdwbase.h"
#include "UI/uiArenaResult.h"
#include "entity/entVarUpdate.h"
#include "clientcomm/clientcomm.h"
#include "MissionControl.h"
//#include "kmeans.h"
#include <utilitieslib/utils/PigDir.h>
#include <utilitieslib/utils/tga.h>
//end dbgstuff

#include <utilitieslib/utils/Color.h>
#include "bases/basedraw.h"
#include "graphics/camera.h"

#include "properties.h"
#include "group/groupgrid.h"

ScrollInfo    lib_scroll;
ScrollInfo    cmd_scroll;
ScrollInfo    tracker_scroll;
ScrollInfo2    property_name_scroll;
ScrollInfo2    property_value_scroll;

Menu *        libraryMenu;
Menu *        favoritesMenu;
MRUList *    recentItems;            //stores the 100 most recent items for the favorites menu
int            recentItemColors[10];    //stores the corresponding colors from the library menu
Menu *        commandMenu;
Menu *        propertiesMenu;
EditListView * trackerListView;
MRUList * recentMaps;    //for the recentMaps in the command menu

PropertyDefList g_propertyDefList;

StashTable trackersSelectedHashTable;
typedef struct
{
    char    name[64];
    int        *val;
} CmdScrollEnt;

CmdScrollEnt cmd_scroll_ents[] = 
{
    { "file/load" },
    { "file/import" },
    { "file/new" },
    { "file/save      f7" },
    { "file/savesel" },
    { "file/exportsel" },
    { "file/saveas" },
    { "file/scenefile" },
    { "file/loadscreen" },
//    { "file/checkin" },    //MW removed, nobody ever uses this, I think it's broken, and I don't want to support it...

    { "group/group      g" },
    { "group/ungroup    u" },
    { "group/del        del" },
    { "group/libdel" },
    { "group/open       [" },
    { "group/close      ]" },
    { "group/setparent" },
    //{ "group/setactivelayer" }, dont offer, only clicking on a layer sets it as active
    { "group/attach" },
    { "group/name" },
    { "group/makelibs" },
    { "group/savelibs" },
    { "group/setpivot   f8" },
    { "group/settype" },
    { "group/setfx" },
    { "group/trayswap" },

    { "group/lod/copyAutoLodParams" },
    { "group/lod/lodscale" },
    { "group/lod/lodfar" },
    { "group/lod/lodfarfade" },
    { "group/lod/loddisable" },
    { "group/lod/lodenable" },
    { "group/lod/lodfadenode" },
    { "group/lod/lodfadenode2" },

    { "adjust/beacons/beaconname" },
    { "adjust/beacons/beaconsize      b" },
    { "adjust/beacons/showconnection  o" },
    { "adjust/beacons/beaconradii" },
    { "adjust/beacons/beaconselect    0" },

    { "adjust/light/setambient" },
    { "adjust/light/unsetambient" },
    { "adjust/light/maxbright" },
    { "adjust/light/lightcolor" },
    { "adjust/light/lightsize" },

    { "adjust/fog/fognear" },
    { "adjust/fog/fogfar" },
    { "adjust/fog/fogcolor1" },
    { "adjust/fog/fogcolor2" },
    { "adjust/fog/fogsize" },
    { "adjust/fog/fogspeed" },

    { "adjust/tinttex/tintcolor1" },
    { "adjust/tinttex/tintcolor2" },
    { "adjust/tinttex/replacetex1" },
    { "adjust/tinttex/replacetex2" },
    { "adjust/tinttex/removetex" },
    { "adjust/tinttex/textureSwap" },
    { "adjust/tinttex/editTexWord" },

    { "adjust/sound/soundvol" },
    { "adjust/sound/soundsize" },
    { "adjust/sound/soundramp" },
    { "adjust/sound/soundname" },
    { "adjust/sound/soundfind" },
    { "adjust/sound/soundexclude" },

    { "adjust/properties/addProp    a" },
    { "adjust/properties/removeProp" },
    { "adjust/properties/showAsRadius" },
    { "adjust/properties/showAsString" },

    { "select/cut              x" },
    { "select/copy             c" },
    { "select/paste            v" },
    { "select/undo             f12" },
    { "select/unsel            esc" },
    { "select/hide             h" },
    { "select/hideothers" },
    { "select/unhide           j" },
    { "select/invertSelection" },
    { "select/setview          alt" },
    { "select/groundsnap       z" },
    { "select/slopesnap" },
    { "select/selectall" },
    { "select/selcontents" },
    { "select/lasso" },
    { "select/setquickobject   6" },
    { "select/togglequickplace 7" },

    { "knobs/edit/posrot     q" },
    { "knobs/edit/gridsize   m" },
    { "knobs/edit/gridshrink n" },
    { "knobs/edit/plane      tab" },
    { "knobs/edit/snaptype   s" },
    { "knobs/edit/nosnap     f5" },
    { "knobs/edit/snaptovert f6" },
    { "knobs/edit/singleaxis 123" },
    { "knobs/edit/localrot" },
    { "knobs/edit/zerogrid" },
    { "knobs/edit/snap3" },

    { "knobs/misc/showall" },
    { "knobs/misc/editorig" },
    { "knobs/misc/editlib" },
    { "knobs/misc/showfog" },
    { "knobs/misc/showvis" },
    { "knobs/misc/ignoretray" },
    { "knobs/misc/colorbynumber" },
    { "knobs/misc/colormemory" },
    { "knobs/misc/objinfo" },
    { "knobs/misc/noerrcheck" },
    { "menu/useoldmenu" },
    { "menu/hideMenu" }
};

int cmdScrollCallback(char *name,int idx)
{
    char    *s,buf[128],cmd[128];
    int        knob=0;

    strcpy(buf,name);
    s = strchr(buf,' ');
    if (s)
        *s = 0;
    s = strrchr(buf,'/');
    if (s)
        s++;
    else
        s = buf;

    if (strstr(name,"knobs/gridsize") || strstr(name,"knobs/gridshrink") || strstr(name,"knobs/misc/editlib") || strstr(name,"editTexWord"))
        sprintf(cmd,"%s",s);
    else if (strstr(name,"knobs/") || 
        strstr(name,"fog/fogsize") || 
        strstr(name,"light/lightsize") || 
        strstr(name,"sound/soundsize") || 
        strstr(name,"beacons/beaconsize") || 
        strstr(name,"beacons/beaconradii") || 
        strstr(name,"useoldmenu") || 
        strstr(name,"beacons/beaconselect"))
    {
        sprintf(cmd,"++%s",s);
        knob = 1;
    }
    else
        sprintf(cmd,"%s 1",s);
    editCmdParse(cmd, 0, 0);
    if (knob)
    {
        char    *s2;
        int        val;
        extern int editGetCmdValue(char *s);

        val = editGetCmdValue(s);
        s2 = strstr(name," ON");
        if (s2)
            s2[0] = 0;
        if (val)
            strcatf_unsafe(name," ON");
    }
    return 0;
}

typedef struct
{
    char        name[128];
    int            idx;
} LibScrollEnt;

LibScrollEnt    *lib_list;
int                lib_list_count,lib_list_max;

void commandMenuClickFunc(MenuEntry * me,ClickInfo * ci) {
    char command[TEXT_DIALOG_MAX_STRLEN + 2];
    char name[TEXT_DIALOG_MAX_STRLEN];
    char * marker;
    int value=(int)me->info;

    strncpy(name, me->name, TEXT_DIALOG_MAX_STRLEN - 1);
    name[TEXT_DIALOG_MAX_STRLEN - 1] = '\0';

    marker=strchr(name,' ');
    if (marker)            //if there is any whitespace
        *marker='\0';    //get rid of it

    if (value==0)
        sprintf(command,"%s 1",name);
    else
    if (value==1)
        sprintf(command,"%s",name);
    else
    if (value==2)
        sprintf(command,"++%s",name);
        
    editCmdParse(command, 0, 0);

    if (value==2)    {
        char * s;
        int val;
        extern int editGetCmdValue(name);

        val=editGetCmdValue(name);
        s=strstr(me->name, " ON");
        if (s)
        {
            int index = s - me->name;
            estrRemove(&me->name, index, estrLength(&me->name) - index);
        }
        if (val)
            estrConcatStaticCharArray(&me->name, " ON");
    }
}

void useOldMenu(MenuEntry * m,void * info) {
    edit_state.toggleMenu=0;
    edit_state.useOldMenu=1;
}

#if 0 // unused debug
void makeSierpinskiTriangle(char * output,char * object,int depth,int sides,float length) {
    char buf[64];
    float f;
    int i;
    if (depth<0) return;
    if (depth>0)
        makeSierpinskiTriangle(output,object,depth-1,sides,length);
    sprintf(buf,"Def grp_Sierpinski%d\n",depth);
    strcat(output,buf);
    for (i=0;i<sides;i++)  {
        if (depth==0)
            sprintf(buf,"\tGroup %s\n",object);
        else
            sprintf(buf,"\tGroup grp_Sierpinski%d\n",depth-1);
        strcat(output,buf);
        f=length*(float)(1<<depth);
        sprintf(buf,"\t\tPos %f 0 %f\n",f*sin(360.0/(float)sides*(float)i*3.1415926535/180.0),f*cos(360.0/(float)sides*(float)i*3.1415926535/180.0));
        strcat(output,buf);
        strcat(output,"\tEnd\n");
    }
    strcat(output,"End\n\n");
}
#endif

void cancelLoadingLibs(MenuEntry * me,void * v) {
    sel.lib_load=0;
}
void soundDebugHelper(DefTracker * tracker,int * line,int tab) {
    char buf[512];
    int i;
    int r,g,b;
    
    if (!tracker)
        return;
    if (!tracker->def)
        return;
    if (tracker->def->has_sound)
        r=255;
    else
        r=0;
    g=255;
    b=0;
    buf[0]=0;
    if (tracker->def->sound_name)
        strcat(buf,tracker->def->sound_name);
    if ((*line)>35) {
        if ((*line)==35)
            xyprintfcolor(40+tab*5,25+*line,r,g,b,"more...");
        (*line)++;
        return;
    }
    xyprintfcolor(40+tab*5,25+*line,r,g,b,"%s",buf);
    if (buf[0])
        (*line)++;
    for (i=0;i<tracker->count;i++)
    soundDebugHelper(&tracker->entries[i],line,tab+1);
}

void soundDebug(MenuEntry * me,void * v) {
    if (edit_state.soundDebug) {
        estrRemove(&me->name, estrLength(&me->name) - 3, 3);
        edit_state.soundDebug=0;
    } else {
        estrConcatStaticCharArray(&me->name, " ON");
        edit_state.soundDebug=1;
    }

}

#if 0 // unused debug
void sierpinski(MenuEntry * me,void * v) {
    char prompt[256];
    char result[TEX_DIALOG_MAX_STRLEN];
    int depth;
    float length;
    int sides;
    char * output;
    FILE * fptr;
    strcpy(prompt,"Enter depth.");
    winGetString(prompt,result);
    depth=atoi(result);
    strcpy(prompt,"Enter num sides.");
    winGetString(prompt,result);
    sides=atoi(result);
    strcpy(prompt,"Enter side length.");
    winGetString(prompt,result);
    length=atof(result);
    if (depth<0) return;
    if (depth>10)
        depth=10;
    if (length<0)
        length=1;
    if (sides<1)
        sides=1;
    output=(char *)malloc((depth+1)*1000);
    if (!result) return;
    fptr=fopen("C:\\game\\data\\maps\\_jwills\\sierpinski.txt","wt");
    output[0]=0;
    strcat(output,"Version 2\n");
    if (sel_count && sel_list[0].def_tracker->def && sel_list[0].def_tracker->def->gf && sel_list[0].def_tracker->def->gf->name) {
        sprintf(prompt,"%s",sel_list[0].def_tracker->def->gf->name);
        strcpy(strrchr(prompt,'/')+1,sel_list[0].def_tracker->def->name);
        makeSierpinskiTriangle(output,(char *)(strchr(prompt,'/')+1),depth,sides,length);
    } else
        makeSierpinskiTriangle(output,"Nature/Bushes/bushes/bush_lrg1",depth,sides,length);
    sprintf(result,"Ref grp_Sierpinski%d\n\tPos 0 0 0\nEnd\n\n",depth);
    strcat(output,result);
    fprintf(fptr,output);
    fclose(fptr);
    free(output);
}
#endif

void loadRecent(MenuEntry * me,ClickInfo * ci)
{
    extern void editLoad(char *,int);
    char buf[256];
    if (me->info==NULL)
        return;
    strcpy(buf,me->info);
    editLoad(buf,0);
}

void toggleEditorTrashing(MenuEntry * me,ClickInfo * ci) {
    edit_state.editorTrashing=!edit_state.editorTrashing;
}

void commandMenuUpdateRecentMaps(char * name)
{
    int opened;
    int i;
    if (commandMenu==NULL || recentMaps==NULL)
        return;
    opened=addEntryToMenu(commandMenu,"file/loadRecent",NULL,NULL)->opened;

    deleteEntryFromMenu(commandMenu,"file/loadRecent");
    addEntryToMenu(commandMenu,"file/loadRecent",NULL,NULL)->opened=opened;
    if (name!=NULL)
    {
        char namebuf[256];
        strcpy(namebuf,name);
        forwardSlashes(namebuf);
        mruAddToList(recentMaps,namebuf);
    }

    for (i=recentMaps->count-1;i>=0;i--)
    {
        char name[256];
        if (strrchr(recentMaps->values[i],'/')==NULL)
            sprintf(name,"file/loadRecent/%s",recentMaps->values[i]);
        else
            sprintf(name,"file/loadRecent/%s",strrchr(recentMaps->values[i],'/'));
        addEntryToMenu(commandMenu,name,loadRecent,recentMaps->values[i]);
    }
}

int favoriteMenuColorFunc(MenuEntry * me,ClickInfo * ci) {
    static int recentLine=0;
    extern int libraryMenuColorFunc(MenuEntry *,ClickInfo *);
    if (me && strcmp(me->name,"Recent")==0 && me->parent==me->theMenu->root)
        recentLine=ci->line;
    if (me && me->parent && strcmp(me->parent->name,"Recent")==0)
        return recentItemColors[ci->line-recentLine];
    return libraryMenuColorFunc(me,ci);
}

void favoritesMenuUpdateRecentItems(char * name_param,MenuEntry * me)
{
    char local_name[TEXT_DIALOG_MAX_STRLEN];
    int i;
    extern void libraryMenuClickFunc(MenuEntry *,ClickInfo *);
    extern int libraryMenuColorFunc(MenuEntry *,ClickInfo *);
    int color=0;

    if (me)
        color=libraryMenuColorFunc(me,NULL);
    if (color==0)
        color=0x00ff00ff;
    
    if (commandMenu==NULL || recentMaps==NULL)
        return;

    if (!me || me->theMenu!=favoritesMenu)
    {
        for (i=recentItems->count-1;i>=0;i--)
        {
            snprintf(local_name, TEXT_DIALOG_MAX_STRLEN - 1, "%s", recentItems->values[i]);
            local_name[TEXT_DIALOG_MAX_STRLEN - 1] = '\0';
            if (strrchr(local_name,'/')!=NULL)
                *strrchr(local_name,'/')=0;
            deleteEntryFromMenu(favoritesMenu,local_name);
        }
    }

    if (name_param!=NULL)
    {
        snprintf(local_name, TEXT_DIALOG_MAX_STRLEN - 1, "Recent/%s/%x", name_param, color);
        local_name[TEXT_DIALOG_MAX_STRLEN - 1] = '\0';
        if (!(recentItems->count>0 && strcmp(recentItems->values[0],local_name)==0))
            mruAddToList(recentItems,local_name);
    }

    if (!me || me->theMenu!=favoritesMenu)
    {
        for (i=recentItems->count-1;i>=0;i--) 
        {
            snprintf(local_name, TEXT_DIALOG_MAX_STRLEN - 1, "%s", recentItems->values[i]);
            local_name[TEXT_DIALOG_MAX_STRLEN - 1] = '\0';
            if (strrchr(local_name,'/')!=NULL)
            {
                sscanf(strrchr(local_name,'/')+1,"%x",&color);
                *strrchr(local_name,'/')=0;
            } 
            else
            {
                color=0x00ff00ff;
            }
            recentItemColors[recentItems->count-i]=color;
            addEntryToMenu(favoritesMenu,local_name,libraryMenuClickFunc,recentItems->values[i]);
        }
    }
    recentItemColors[0]=0xffff00ff;
}
/*
void editorReplayWrapperStart(MenuEntry * me,ClickInfo * ci) {
    editorReplayStart();
}

void editorReplayWrapperStop(MenuEntry * me,ClickInfo * ci) {
    editorReplayStop();
}

void editorReplayWrapperPause(MenuEntry * me,ClickInfo * ci) {
    extern int editorReplayOn;
    if (editorReplayOn==1)
        editorReplayOn=2;
}

void editorReplayWrapperContinue(MenuEntry * me,ClickInfo * ci) {
    editorReplayContinue();
}
*/

void commandMenuCreate() {
    MenuEntry *autosaveMenuEntry;
#define COMMAND_MENU_FUNC_1        (void *)0        //toggles
#define COMMAND_MENU_FUNC        (void *)1        //one shot commands
#define COMMAND_MENU_FUNC_ON    (void *)2        //knobs (shows 'ON' in the menu)
    recentMaps=createMRUList("RecentMaps","RecentMapsMRU",5,1024);
    commandMenu=newMenu(-235,-76,222,215,"Command");
    commandMenu->lv->theWindowCorner=EDIT_LIST_VIEW_LOWERRIGHT;
    commandMenu->lv->growUp=1;
    addEntryToMenu(commandMenu,"file/load"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/import"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/new"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/save    ^2f7"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    //autosave needs a little extra processing
    autosaveMenuEntry=addEntryToMenu(commandMenu,"file/autosave",editCmdAutosave,COMMAND_MENU_FUNC_1);    //editCmdAutosave makes sure
                                                                        //that the message displayed
                                                                        //in the menu is consistent
                                                                        //with the time between autosaves
    estrPrintf(&autosaveMenuEntry->name, "autosave ON: %dm", edit_state.autosavewait/(60*CLK_TCK));

    addEntryToMenu(commandMenu,"file/savesel"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/exportsel"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/saveas"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/scenefile"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/loadscreen"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"file/clientsave"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    commandMenuUpdateRecentMaps(NULL);

    addEntryToMenu(commandMenu,"group/group      ^2g"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/ungroup    ^2u"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/del        ^2del",commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/libdel"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/open       ^2["    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/close      ^2]"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/setparent"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/attach"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/detach"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/name"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/makelibs"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/savelibs"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/setpivot   ^2f8"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/settype"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/setfx"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/trayswap"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/noFogClip"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/noColl"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/makeDestructibleObject",commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"group/lod/copyAutoLodParams",commandMenuClickFunc,COMMAND_MENU_FUNC);
    addEntryToMenu(commandMenu,"group/lod/lodscale"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/lod/lodfar"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/lod/lodfarfade"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/lod/loddisable"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/lod/lodenable"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/lod/lodfadenode"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"group/lod/lodfadenode2"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"adjust/beacons/beaconname"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/beacons/beaconsize     ^2b"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"adjust/beacons/showconnection ^2o"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/beacons/beaconradii"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"adjust/beacons/beaconselect   ^20"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);

    addEntryToMenu(commandMenu,"adjust/volumes/boxscale"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"adjust/light/setambient"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/light/unsetambient"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/light/maxbright"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/light/lightcolor"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/light/lightsize"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);

    addEntryToMenu(commandMenu,"adjust/fog/fognear"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/fog/fogfar"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/fog/fogcolor1"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/fog/fogcolor2"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/fog/fogsize"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"adjust/fog/fogspeed"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"adjust/tinttex/tintcolor1"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/tinttex/tintcolor2"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/tinttex/replacetex1"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/tinttex/replacetex2"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/tinttex/removetex"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/tinttex/textureSwap"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/tinttex/editTexWord"    ,commandMenuClickFunc,COMMAND_MENU_FUNC);

    addEntryToMenu(commandMenu,"adjust/sound/soundvol"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/sound/soundsize"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"adjust/sound/soundramp"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/sound/soundname"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/sound/soundfind"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/sound/soundexclude"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/sound/soundscript"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"adjust/properties/addProp      ^2a"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/properties/removeProp"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/properties/showAsRadius"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/properties/showAsString"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/properties/copyProperties"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/properties/pasteProperties"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"adjust/burningBuildings/controller"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/burningBuildings/destroy"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"adjust/burningBuildings/repair"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    edit_state.burningBuildingMenuEntry = addEntryToMenu(commandMenu,"adjust/burningBuildings/speed 0%",commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    estrPrintf(&edit_state.burningBuildingMenuEntry->name, "speed 0.000000%%/s");
    addEntryToMenu(commandMenu,"adjust/burningBuildings/stop"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"select/cut              ^2x"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/copy             ^2c"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/paste            ^2v"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/undo             ^2f12"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/redo"                    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/unsel            ^2esc"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/hide                ^2h"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/hideothers"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/unhide           ^2j"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/freeze"                    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/freezeothers"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/unfreeze"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/invertSelection"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/search"                    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/setview          ^2alt"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/groundsnap       ^2z"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/slopesnap"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/selectall"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/selcontents"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/setquickobject   ^26"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
    addEntryToMenu(commandMenu,"select/togglequickplace ^27"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);

    addEntryToMenu(commandMenu,"knobs/edit/posrot     ^2q"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/gridsize   ^2m"        ,commandMenuClickFunc,COMMAND_MENU_FUNC);
    addEntryToMenu(commandMenu,"knobs/edit/gridshrink ^2n"        ,commandMenuClickFunc,COMMAND_MENU_FUNC);
    addEntryToMenu(commandMenu,"knobs/edit/plane      ^2tab"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/snaptype   ^2s"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/nosnap     ^2f5"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/snaptovert ^2f6"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/singleaxis ^2123"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/localrot"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/useObjectAxes"        ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/zerogrid"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/edit/snap3"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);

    addEntryToMenu(commandMenu,"knobs/misc/showall"                    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/editorig"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/editlib"                    ,commandMenuClickFunc,COMMAND_MENU_FUNC);
    addEntryToMenu(commandMenu,"knobs/misc/showfog"                    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/showvis"                    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/ignoretray"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/colorbynumber"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/colormemory"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/objinfo    ^2o"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/noerrcheck"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/profiler"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/cameraPlacement"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/liveTrayLink"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    addEntryToMenu(commandMenu,"knobs/misc/showGroupBounds"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    {
        extern GameState game_state;
        if (game_state.hideCollisionVolumes) {
            addEntryToMenu(commandMenu,"knobs/misc/hideCollisionVolumes ON"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
            edit_state.hideCollisionVolumes=game_state.hideCollisionVolumes;
        } else
            addEntryToMenu(commandMenu,"knobs/misc/hideCollisionVolumes"    ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
    }

    //my own stuff, stay away!!! :-P
//    addEntryToMenu(commandMenu,"Jon's Stuff/tester"            ,foobarthundermonkey,COMMAND_MENU_FUNC_ON);
//    addEntryToMenu(commandMenu,"Jon's Stuff/missionControl"            ,commandMenuClickFunc,COMMAND_MENU_FUNC_ON);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Editor Thrashing"        ,toggleEditorTrashing,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Editor Replay/Start"    ,editorReplayWrapperStart,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Editor Replay/Stop"        ,editorReplayWrapperStop,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Editor Replay/Pause"    ,editorReplayWrapperPause,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Editor Replay/Continue"    ,editorReplayWrapperContinue,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/kmeans"                    ,testkmeans,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Sierpinski's Triangle"    ,sierpinski,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Cancel Loading Libs"    ,cancelLoadingLibs,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"Jon's Stuff/Sound Debug"            ,soundDebug,COMMAND_MENU_FUNC_1);
//    addEntryToMenu(commandMenu,"menu/useOldMenu"            ,useOldMenu,NULL);
//    addEntryToMenu(commandMenu,"menu/hideMenu"                ,commandMenuClickFunc,COMMAND_MENU_FUNC_1);
};

void loadObject(char * name) {
    sel.fake_ref.def = groupDefFind(name);
    if (!sel.fake_ref.def)// && !sel.lib_load)
    {
        if (!sel.lib_load)
        {
            sel.lib_load = 1;
            strncpy(sel.lib_name, name, 999);
            sel.lib_name[999] = '\0';
            editDefLoad(name);
        }
    }
    else
    {
        sel.lib_load = 0;
        copyMat4(unitmat,sel.fake_ref.mat);
        selCopy(NULL);
    }
}

//this function gets called whenever an item in the libraryMenu
//is clicked.  It loads the appropriate object into the editor
void libraryMenuClickFunc(MenuEntry * me,ClickInfo * ci) {
    favoritesMenuUpdateRecentItems(me->name,me);
    loadObject(me->name);
}

void libraryMenuCreate() {
//this function should exist and be used
}

int * lineNums;            //used for the EditListView callback function
int numLinesSelected;
int shiftTrackerStatus;
DefTracker * lastTrackerSelected;

void trackerPrint(DefTracker * tracker,int start,int end,int * current,char ** text,int * colors,int depth,int layer) {
    int i;
    int iResult;
    int actual=*current-start;
    if (tracker==NULL) return;
    if (tracker->def==NULL) return;
    if (tracker->def->name==NULL) {
        printf("No def should be without a name!\n");
        return;
    };
    if (tracker==lastTrackerSelected && inpLevel(INP_SHIFT))
        shiftTrackerStatus++;
    if (*current>=start && *current<=end)
    {
        estrClear(&text[actual]);
        for (i=0;i<depth;i++)
            estrConcatChar(&text[actual], ' ');
        estrConcatCharString(&text[actual], tracker->def->name);
        colors[actual]=-1;
        if (tracker->tricks!=NULL && (tracker->tricks->flags1 & TRICK_HIDE))
            colors[actual]=0x7F7F7FFF;    //grayish
        if (tracker->frozen)
            colors[actual]=0x1F1FF3FF;    //grayish
        if (tracker->def->has_properties)   
        {
            //If this is a Layer, 
            PropertyEnt* layerProp; 
            stashFindPointer(  tracker->def->properties, "Layer" , &layerProp );
            
            if( layerProp )
            {
                extern EditSelect sel;
                if( tracker == sel.activeLayer ) //maybe also check ids?
                {
                    estrConcatStaticCharArray(&text[actual], " Active");
                    colors[actual]=0xFF7F00FF;
                }
                estrConcatStaticCharArray(&text[actual], " Layer");        
                if (colors[actual]==-1)
                    colors[actual]=0xAC3FBFFF;    //purlish?
            }
            else
                estrConcatChar(&text[actual], '#');
        }

        if (tracker->def->count>0) {
            if (tracker->entries && tracker->edit)
                text[actual][depth-1]='-'; // okay hackery for estrings, we already have this character in the string.
            else
                text[actual][depth-1]='+'; // okay hackery for estrings, we already have this character in the string.
        }
//        if (tracker->def->has_properties && depth>2)
//            estrConcatChar(&text[actual], '#');
        if (tracker->def->has_ambient)
            estrConcatStaticCharArray(&text[actual], "-AMB");
        if (tracker->def->has_tint_color)
            estrConcatStaticCharArray(&text[actual], "-TINT");
        if (tracker->def->has_tex || eaSize(&tracker->def->def_tex_swaps))
            estrConcatStaticCharArray(&text[actual], "-TEX");
        if (tracker->def->lod_fadenode==1)
            estrConcatStaticCharArray(&text[actual], "-FADE");
        if (tracker->def->lod_fadenode==2)
            estrConcatStaticCharArray(&text[actual], "-FADE2");
        if (tracker->def->no_fog_clip)
            estrConcatStaticCharArray(&text[actual], "-NOFOGCLIP");
        if (tracker->def->no_coll)
            estrConcatStaticCharArray(&text[actual], "-NOCOLL");
        if (!(((tracker->def->lod_scale<1.0)?1.0-tracker->def->lod_scale:tracker->def->lod_scale-1.0)<1e-9))
            estrConcatStaticCharArray(&text[actual], "-LOD");
        if (actual==trackerListView->lineOver && trackerListView->lineOver!=-1 && trackerListView->hasFocus) {
            colors[actual]=EDIT_LIST_VIEW_MOUSEOVER;
            if (tracker)
                add3dCursor(tracker->mat[3], NULL);
            if (colors[actual]==-1) {
                if (tracker->def->count>0)
                    colors[actual]=EDIT_LIST_VIEW_DIRECTORY;
            }
        } else if (colors[actual]==-1)
            colors[actual]=EDIT_LIST_VIEW_STANDARD;

    }


    if (stashAddressFindInt(trackersSelectedHashTable,tracker, &iResult) && iResult == 1 )
    {
        if (*current>=start && *current<=end) {
            estrConcatChar(&text[actual], '*');
            colors[actual]=EDIT_LIST_VIEW_SELECTED;
        }
        lineNums[numLinesSelected++]=*current;
    }
    if (!editIsWaitingForServer() && trackerListView->lineClicked!=-1 && ((actual==trackerListView->lineClicked) ||
                                                            (shiftTrackerStatus==1))) {
        tracker->frozen=0;
        if (actual==trackerListView->lineClicked && inpLevel(INP_SHIFT))
            shiftTrackerStatus++;
        selAdd(tracker,layer,inpLevel(INP_SHIFT)?0:1, EDITOR_LIST);
        if (colors[trackerListView->lineClicked]!=EDIT_LIST_VIEW_SELECTED)    //!= because if it just got clicked then
            edit_state.last_selected_tracker=tracker;                        //it isn't selected yet
    }

    *current=*current+1;
    if (tracker->edit)
    for (i=0;i<tracker->count;i++)
        trackerPrint(&tracker->entries[i],start,end,current,text,colors,depth+1,layer);
}

// text is now an array of EStrings!  We've already created them...
void trackerTextCallback(int start,int end,char ** text,int * colors,int * max,void * theInfo) {
    int current=0;
    int i;
    if (lineNums!=NULL)
        free(lineNums);
    lineNums=(int *)calloc(sel_count, sizeof(int));
    numLinesSelected=0;

    //set up the hash table

    stashTableClear(trackersSelectedHashTable);
    for (i=0;i<sel_count;i++)
        if (sel_list[i].def_tracker!=NULL)
        {
            stashAddressAddInt(trackersSelectedHashTable, sel_list[i].def_tracker, 1, false);
        }
    if (start<=0) {
        estrPrintCharString(&text[0], " ");
        estrPrintCharString(&text[1], "-Trackers");
        colors[1]=EDIT_LIST_VIEW_DIRECTORY;
        if (trackerListView->lineClicked==1)
            editCmdUnsetActiveLayer();
    } else if (start==0) {
        estrPrintCharString(&text[0], "-Trackers");
        colors[0]=EDIT_LIST_VIEW_DIRECTORY;
        if (trackerListView->lineClicked==0)
            editCmdUnsetActiveLayer();
    }
    current=1;
    lastTrackerSelected=edit_state.last_selected_tracker;
    shiftTrackerStatus=0;
    for (i=0;i<group_info.ref_count;i++) {
        if (edit_state.last_selected_tracker==NULL || group_info.refs[i]->def==NULL)
            shiftTrackerStatus=-1;
        else if (group_info.refs[i]->def!=NULL && stashFindElement(group_info.refs[i]->def->properties,"Layer",NULL))
            shiftTrackerStatus=0;
        trackerPrint(group_info.refs[i],start,end,&current,text,colors,2,i);
    }
    *max=current;
    if (edit_state.setview && numLinesSelected==1) {
        trackerListView->amountScrolled=trackerListView->lineHeight*lineNums[0]-trackerListView->height/2;
    }
}

void trackerSelectedCallback(int ** selected,int * size) {
    *selected=lineNums;
    *size=numLinesSelected;
}

void trackerListViewCreate() {
    lineNums=NULL;
    trackerListView=newELV(-295,25,275,500);
    trackerListView->theWindowCorner=EDIT_LIST_VIEW_UPPERRIGHT;
    setELVTextCallback(trackerListView,trackerTextCallback);
    setELVInfo(trackerListView,0);
    setELVSelectedCallback(trackerListView,trackerSelectedCallback);
    trackersSelectedHashTable = stashTableCreateAddress(1000);
}

int libraryMenuColorFunc(MenuEntry * me,ClickInfo * ci) {
    static int colors[] =
    {
        0x00FF00FF,
        0xFF5F00FF,
        0x5FFF00FF,
        0xFF5F5FFF,
        0x8F8F8FFF,
        0x40AF40FF,
        0xFF0FFFFF,
        0xFFFFFFFF,
        0x5F5FFFFF,
        0x5FFFFFFF,
//        0x0F0FFFFF, // ryan said this one sucks
        0x0F7FFFFF,
        0x7F0FFFFF,
    };

    if(me->parent && !me->child)
    {
        unsigned int val=0;
        int i;
        for(i = estrLength(&me->parent->name)-1; i >= 0; i--)
            val+=me->parent->name[i]*i;
        return colors[val % ARRAY_SIZE(colors)];
    }
    return 0;
}

StashTable allLibraryObjectNames;

int isWhiteSpace(char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}


FileScanAction paletteScanner(char * dir, struct _finddata32_t * dat) {    
    char * data;
    char * cur;
    char folder[256];
    char buf[256];
    char temp[256];
    char filename[256];
    folder[0]=0;
    temp[0]=0;
    if (!strEndsWith(dat->name,".txt"))
        return FSA_EXPLORE_DIRECTORY;
    sprintf(temp,"%s%s",dir,dat->name);
    sprintf(filename,dat->name);
    if (strrchr(filename,'.')!=NULL)
        *strrchr(filename,'.')=0;
    data=fileAlloc(temp,NULL);
    if (data==NULL)
        return FSA_EXPLORE_DIRECTORY;
    cur=data;
    do {
        buf[0]=0;
        sscanf(cur,"%s",buf);
        while(*cur && (isWhiteSpace(*cur))) cur++;
        while(*cur && (!isWhiteSpace(*cur))) cur++;
        if (stricmp(buf,"folder")==0) {
            sscanf(cur,"%s",folder);
            forwardSlashes(folder);
            if (*folder && *(folder+strlen(folder)-1)=='/')
                *(folder+strlen(folder)-1)='\0';
            while(*cur && (isWhiteSpace(*cur))) cur++;
            while(*cur && (!isWhiteSpace(*cur))) cur++;
        } else  if (buf[0]) {
            extern StashTable                    group_file_hashes;
            sprintf(temp,"Palettes/%s/%s/%s",filename,folder,buf);
            if (stashFindPointerReturnPointer(allLibraryObjectNames,buf))
                addEntryToMenu(favoritesMenu,temp,libraryMenuClickFunc,NULL);
        }
    } while (buf[0]);
    fileFree(data);
    return FSA_EXPLORE_DIRECTORY;
}

void createFavoritesMenu() {
    favoritesMenu=newMenu(325,25,300,100,"Favorites");
    addEntryToMenu(favoritesMenu,"Palettes",NULL,NULL);
    addEntryToMenu(favoritesMenu,"Recent",NULL,NULL);
    recentItems=createMRUList("RecentItems","RecentItemsMRU",10,1024);
    favoritesMenuUpdateRecentItems(NULL,NULL);
    favoritesMenu->colorFunc=favoriteMenuColorFunc;
    ELVLatchTopBottom(libraryMenu->lv,favoritesMenu->lv);
    fileScanAllDataDirs("C:/palettes/",paletteScanner);
}

int recreateFavoritesMenu;

void createFavoritesMenuCallback(const char *relpath, int when) {
    char file[256];
    sprintf(file,"C:/palettes/%s",relpath);
    if (strEndsWith(relpath,".txt") && fileExists(file))
        recreateFavoritesMenu=1;
}

void libUpdateList()
{
    int        i,count=0,all_count;
    char    *s,*name,*dir;
    static int last_showall,last_countall;
    static int menuInit=0;
    all_count = objectLibraryCount();

    if (!menuInit && lib_list_count>0) {
        char buffer[255];
        allLibraryObjectNames=stashTableCreateWithStringKeys(all_count, StashDefault);
        menuInit=1;
        libraryMenu = newMenu(15,25,300,450,"Library");
        libraryMenu->colorFunc=libraryMenuColorFunc;
        for (i=0;i<all_count;i++) {
            name=objectLibraryNameFromIdx(i);
            dir=objectLibraryPathFromIdx(i);
            strcpy(buffer,strchr(dir,'/')+1);
            strcpy(strrchr(buffer,'/')+1,name);
            if (!name) continue;
            addEntryToMenu(libraryMenu,buffer,libraryMenuClickFunc,0);
            stashAddInt(allLibraryObjectNames,name,1, false);
        }
        createFavoritesMenu();
        if (dirExists("c:/palettes")) {
            FolderCache * fc=FolderCacheCreate();
            FolderCacheAddFolder(fc,"C:/palettes/",0);
            FolderCacheQuery(fc,NULL);
            FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, "*.txt", createFavoritesMenuCallback);
        }
    }

    if (recreateFavoritesMenu)
    {
        recreateFavoritesMenu=0;
        destroyMenu(favoritesMenu);
        createFavoritesMenu();
    }

    if (last_showall == edit_state.showall && last_countall == all_count)
        return;
    last_showall = edit_state.showall;
    last_countall = all_count;
    lib_list_count = 0;
    dynArrayFit(&lib_list,sizeof(lib_list[0]),&lib_list_max,all_count);
    lib_scroll.list_names = lib_list[0].name;
    for(i=0;i<all_count;i++)
    {
        char    buf[1000];

        name = objectLibraryNameFromIdx(i);
        if (!name)
            continue;
        if (!edit_state.showall && name[0] == '_')
            continue;
        if (edit_state.showall != 2 && name[strlen(name)-1] == '&')
            continue;
        dir = objectLibraryPathFromIdx(i);
        if (strnicmp(dir,"maps",4)==0)
            continue;
        strcpy(buf,dir + sizeof("object_library"));
        s = strrchr(buf,'/');
        if (s)
            *s = 0;
        sprintf(lib_list[count].name,"%s/%s",buf,name);
        count++;
    }
    lib_list_count = count;
}

static int countMenuLines(MenuEntry *me, MenuEntry *selected, int *count)
{
    int *childcount = count;

    if(!me)
        return 0;
    if(me == selected)
        return 1;

    if(!edit_state.showall && me->name[0] == '_') // perpetuating the hack
        childcount = NULL; // this entry and all children won't be displayed
    if(childcount)
        ++*childcount;

    if(me->opened && countMenuLines(me->child, selected, childcount))
        return 1;
    if(countMenuLines(me->sibling, selected, count))
        return 1;

    return 0;
}

void libOpenTo(DefTracker *tracker)
{
    if(tracker && tracker->def && groupInLib(tracker->def))
    {
        MenuEntry *me, *selected = findMenuEntry(libraryMenu->root, tracker->def->name);
        int count = 0;
        for(me = selected; me; me = me->parent)
            me->opened = 1;
        countMenuLines(libraryMenu->root, selected, &count);
        libraryMenu->lv->amountScrolled = (count + .5) * libraryMenu->lv->lineHeight - libraryMenu->lv->height/2;
    }
}

int libScrollCallback(char *name,int idx)
{
    char buf[1024];
    sel.fake_ref.def = groupDefFind(name);
    if (!sel.fake_ref.def)// && !sel.lib_load)
    {
        if (!sel.lib_load)
        {
            sprintf(buf,"load %s",name);
//            editorReplayAppend(buf);
            sel.lib_load = 1;
            strcpy(sel.lib_name,name);
            //unSelect(2);
            editDefLoad(name);
        }
    }
    else
    {
        sprintf(buf,"load %s",name);
//        editorReplayAppend(buf);
        sel.lib_load = 0;
        copyMat4(unitmat,sel.fake_ref.mat);
        selCopy(NULL);
    }
    return 0;
}

// Make sure there is enough room to hold both the group name
// and the group "type" name.
#define TRACKER_STRLEN 256
typedef struct
{
    char        name[TRACKER_STRLEN];
    DefTracker    *def;
    int            ref_id;
} TrackerScrollEnt;

TrackerScrollEnt    tracker_list[1000];
int                    tracker_list_count,tracker_list_skip;

int trackerScrollCallback(char *name,int idx)
{
    edit_state.last_selected_tracker = tracker_list[idx].def;
    if (edit_state.quickPlacement) {// || !mouseDown(MS_LEFT) && isDown(MS_LEFT)) {
        int i,dir;
        if (idx<tracker_scroll.lastIdx) {
            dir=-1;
        } else {
            dir=1;
        }
        for (i=tracker_scroll.lastIdx+dir;i!=idx+dir;i+=dir) {
            bool good=true;
            char * s=strchr(tracker_list[i].name,'-');
            if (s!=NULL) {
                s--;
                good=false;
                while (s-->tracker_list[i].name)
                    if (*s!=' ')
                        good=true;
            }
            s=strrchr(tracker_list[i].name,'*');
            if (s && *(s+1)=='\0') good=false;
            if (!good) continue;
            selAdd(tracker_list[i].def,tracker_list[idx].ref_id,1,EDITOR_LIST);
        }
    } else {
        selAdd(tracker_list[idx].def,tracker_list[idx].ref_id,1, EDITOR_LIST);
    }
    tracker_scroll.lastIdx=idx;
    return 0;
}

int trackerScrollRollover(char* name, int idx)
{
    if (tracker_list[idx].def)
        add3dCursor(tracker_list[idx].def->mat[3], NULL);
    return 0;
}

static int start_draw_offset;

void trackerUpdateList(int ref_id,DefTracker *tracker,int depth)
{
    char    *so = NULL;
    int        i;
    TrackerScrollEnt    *trk_ent;

    if (!tracker || !tracker->def || !tracker->def->in_use)
        return;
    if (++tracker_list_skip > start_draw_offset)
    {
        const char*    si;

        if (tracker_list_count >= ARRAY_SIZE(tracker_list))
            return;
        trk_ent = &tracker_list[tracker_list_count++];
        trk_ent->def = tracker;
        trk_ent->ref_id = ref_id;
        so = trk_ent->name;
        si = strrchr(tracker->def->name,'/');
        if (!si++)
            si = tracker->def->name;
        memset(so,' ',depth+1);
        if (tracker->def->count)
            so[depth] = '+';

        strcpy(so+depth+1,si);

        if (tracker->def->has_ambient)
        {
            strcat(so, "-AMB");
        }

        if (tracker->def->has_tint_color)
        {
            strcat(so, "-TINT");
        }

        if (tracker->def->has_tex)
        {
            strcat(so, "-TEX");
        }

        if (tracker->def->lod_fadenode==1)
        {
            strcat(so, "-FADE");
        }

        if (tracker->def->lod_fadenode==2)
        {
            strcat(so, "-FADE2");
        }

        if (tracker->def->no_fog_clip)
        {
            strcat(so, "-NOFOGCLIP");
        }

        if (tracker->def->no_coll)
        {
            strcat(so, "-NOCOLL");
        }

        // A hash mark means the group has properties attached.
        if (tracker->def->has_properties)   
        {
            //If this is a Layer, 
            PropertyEnt* layerProp; 
            stashFindPointer(  tracker->def->properties, "Layer" , &layerProp );
            
            if( layerProp )
            {
                extern EditSelect sel;
                if( tracker == sel.activeLayer ) //maybe also check ids?
                {
                    strcat( so, " Active");
                }
                strcat( so, " Layer" );        
            }
            else
                strcat(so, "#");
        }

        // Show the "type" on the actual group.  It's quite hard to go looking for a group
        // of a particular type otherwise.
        if (tracker->def->type_name && tracker->def->type_name[0])
        {
            strcat(so, "(");
            strcat(so, tracker->def->type_name);
            strcat(so, ")");
        }

        for(i=0;i<sel_count;i++)
        {
            if (tracker == sel_list[i].def_tracker)
            {
                strcat(so,"*");
                break;
            }
        }
        if (tracker->tricks && (tracker->tricks->flags1 & TRICK_HIDE))
            strcat(so,"~");
    }

    for(i=0;i<tracker->count;i++)
    {
        if (tracker->entries && tracker->edit)
        {
            if(so)
            {
                so[depth] = '-';
            }
            trackerUpdateList(ref_id,tracker->entries + i,depth + 1);
        }
    }
}



/*
PropertyEnt    property_list[128];
int            property_list_count;

void propertyListAdd(char* name, char* value){
    strcpy(property_list[property_list_count].name_str, name);
    strcpy(property_list[property_list_count].value_str, value);
    property_list[property_list_count].value = atof(value);
    property_list_count++;
}
*/
static int ExtractAllProperties(StashElement element){
    PropertyEnt* prop = stashElementGetPointer(element);
    uiScroll2AddName(&property_name_scroll, prop->name_str, 0, 0);
    uiScroll2AddNameSimple(&property_value_scroll, prop->value_str);
    return 1;
}

void propertyUpdateList(){
    GroupDef* def;
    
    property_name_scroll.count = 1;
    property_value_scroll.count = 1;

    // Can only display properties for one item only.
    if(sel_count != 1)
        return;

    // Can only display properties for items that have properties.
    def = sel_list[0].def_tracker->def;
    if(!def->properties)
        return;

    stashForEachElement(def->properties, ExtractAllProperties);
}

/* Function promptUserForString()
 *    Prompts the user for a new string to replace the old string.
 *    This function assumes that "newString" points to a location
 *    with enough buffer space to hold the incoming data.
 *
 *    Returns:
 *        1 - new string differs from old string
 *        0 - new string is the same as the old string.
 */
// Returns whether the new and the old values are different or not.
int promptUserForString(const char* oldString, char* newString){
    strcpy(newString, oldString);
    if( !winGetString("Enter Text", newString) )
        return 0; // user cancelled

    // JS: Do not alter user strings
    /*
    cursor = strrchr(newString, '\\');
    if(cursor){
        cursor++;
        strcpy(newString, cursor);
    }
    */

    // If the user did not change the name, do nothing.
    if(0 == strcmp(oldString, newString) || (*newString)==0)
        return 0;
    else 
        return 1;
}

int editRemoveProperty(StashTable properties, char* propName){
    PropertyEnt* prop;
    char buffer[512];
    int defChanged = 0;

    // Prompt the user to confirm the remove operation, just in case
    // the user clicked on the wrong property or if the user entered
    // remove property mode without knowing.
    sprintf(buffer, "Remove the property named \"%s\"?", propName);
    if(winMsgYesNo(buffer)){
        stashRemovePointer(properties, propName, &prop);
        destroyPropertyEnt(prop);
        defChanged = 1;
    }
    
    edit_state.removeProperty = 0;
    return defChanged;
}

int editChangePropertyName(StashTable properties, char* oldPropName, int * deleteProperty ){
    PropertyEnt* prop;
    char newPropName[1024];
    int defChanged = 0;
    int goodName = 0;

    // Prompt the user for the new property name.
    if(!promptUserForString(oldPropName, newPropName))
        return defChanged;

    *deleteProperty = 0; 

    //If the string is null, it means remove property
    if( newPropName[0] == 0 )
    {
        *deleteProperty = 1;
        return 1; //defChanged
    }

    if(strlen(newPropName))
    {
        unsigned char* curChar;
        
        for(curChar = newPropName; *curChar; curChar++)
        {
            if(isalnum((unsigned char)*curChar))
            {
                goodName = 1;
                break;
            }
        }
    }
    
    if(!goodName)
    {
        winMsgAlert("That name is unnacceptable, try again.  This time, use some letters and numbers.");
        return 0;
    }

    // Verify that the new property does not already exist.
    {
        StashElement element;

        stashFindElement(properties, newPropName, &element);
        if(element){
            winMsgAlert("The property already exists!");
            return 0;
        }
    }

    // Remove the old property entry from properties table.
    {
        stashRemovePointer(properties, oldPropName, &prop);
    }

    // Add the property
    {
        strcpy(prop->name_str, newPropName);
        stashAddPointer(properties, prop->name_str, prop, false);
        defChanged = 1;
    }

    return defChanged;
}

int editChangePropertyValueString(StashTable properties, char* propName){
    PropertyEnt* prop;
    char newValue[1024];
    int defChanged = 0;

    // Extract the property entry from the property name
    stashFindPointer( properties, propName, &prop );
    assert(prop);
    
    if(!promptUserForString(prop->value_str, newValue))
        return defChanged;    

    strcpy(prop->value_str, newValue);
    defChanged = 1;
    return defChanged;
}

// User clicked on a property name.  Display a window for the user to change the
// property name.
int propNameScrollCallback(char* oldPropName,int idx){
    GroupDef* def;
    int updateGroupDef = 0;
    int deleteProperty = 0;

    if(!isNonLibSingleSelect(0,0))
        return 0;

    // Property name can only be changed on one item at a time.
    if(sel_count != 1){
        winMsgAlert("Only works with 1 group selected");
        return 0;
    }

    def = sel_list[0].def_tracker->def;

    //Being in remove property mode -OR- an empty string will remove the property

    if( !edit_state.removeProperty){
    // Otherwise, the user has is requesting to change the property name.
        updateGroupDef = editChangePropertyName(def->properties, oldPropName, &deleteProperty );
    }

    // Has the user requested to have a property removed?
    if( deleteProperty || edit_state.removeProperty){
        updateGroupDef = editRemoveProperty(def->properties, oldPropName);

        // Update some group def variables to keep variables consistent.
        //        Don't destroy the hash table here.  It is possible that the
        //        property is "temporarily" being removed due to the way the editor
        //        deals with group updates.

        if(0 == stashGetValidElementCount(def->properties)){
            //stashTableDestroy(def->properties);
            //def->properties = 0;
            def->has_properties = 0;
        }

        if (updateGroupDef) {
            if (!def->tex_names[0] || !def->tex_names[0][0]) {
                def->has_tex &= ~1; // reset this it will be auto-refreshed on update if there is still a TexWordLayout property
            }
        }
    }

    if(updateGroupDef)
    {
        editUpdateTracker(sel_list[0].def_tracker);
        //unSelect(2);
    }

    return 0;
}

float propRadius;
static PropertyEnt* editPropRadiusTarget;

/* Function propValueScrollCallback()
 *    This callback function is called when a Property Value scroll item is
 *    clicked.  The user might be trying to perform one of the follow tasks:
 *    1)    Change property value type
 *        The property value can be either a string or a radius.
 *    2)    Change property value
 *        case 1:
 *            When the value is just a string, request the new string value
 *            from the user.
 *        case 2:
 *            When the value is a radius, go through a two step radius edit
 *            similar to other radius edits done in the editor.
 *
 *
 *
 */
int propValueScrollCallback(char* oldValue,int idx){
    GroupDef* def;
    PropertyEnt* prop;
    int updateGroupDef = 0;

    if(!isNonLibSingleSelect(0,0))
        return 0;

    // Is the user trying to complete a radius edit?
    if(editPropRadiusTarget && edit_state.editPropRadius == 1){
        estrPrintf(&editPropRadiusTarget->value_str, "%f", propRadius);
        edit_state.editPropRadius = 0;
        editPropRadiusTarget = NULL;
        editUpdateTracker(sel_list[0].def_tracker);
        return 0;
    }

    // The user is attempting to do something else...
    // Index values are always off-by-one due to the scroll header.
    idx += 1;

    // Property value can only be change on one item at a time.
    if(sel_count != 1){
        winMsgAlert("Only works with 1 group selected");
        return 0;
    }

    def = sel_list[0].def_tracker->def;

    // Extract the property associated with the value that was clicked.
    stashFindPointer( def->properties, property_name_scroll.names[idx].name, &prop );

    // Is the user clicking on the values to change its type?
    if(edit_state.showPropertyAsRadius){
        prop->type = Radius;
        edit_state.showPropertyAsRadius = 0;
        edit_state.showPropertyAsString = 0;
        updateGroupDef = 1;
    }else if(edit_state.showPropertyAsString){
        prop->type = String;
        edit_state.showPropertyAsRadius = 0;
        edit_state.showPropertyAsString = 0;
        updateGroupDef = 1;
    }else{
        // The user is clicking on the values to change its value.
        switch(prop->type){
        case Radius:
            editPropRadiusTarget = prop;
            propRadius = atof(prop->value_str);
            edit_state.editPropRadius = 1;

            // The radius edit isn't complete yet, do not send the server an update.
            updateGroupDef = 0;
            break;
        case String:
        default:
            // Only have the server update its group definition if it has changed.
            updateGroupDef = editChangePropertyValueString(def->properties, property_name_scroll.names[idx].name);
            break;
        }
    }

    if(updateGroupDef){
        editUpdateTracker(sel_list[0].def_tracker);
        unSelect(2);
    }
        
    
    return 0;
}

static int trackerFindIndexSub(DefTracker *tracker,DefTracker *search,int *count)
{
    int        i;

    if (!tracker || !tracker->def || !tracker->def->in_use)
        return 0;
    if (tracker == search)
        return 1;
    (*count)++;
    for(i=0;i<tracker->count;i++)
    {
        if (tracker->entries && tracker->edit)
        {
            if (trackerFindIndexSub(tracker->entries + i,search,count))
                return 1;
        }
    }
    return 0;
}

static int trackerFindIndex(DefTracker *search,int *count)
{
    int        i;

    for(i=0;i<group_info.ref_count;i++)
    {
        if (trackerFindIndexSub(group_info.refs[i],search,count))
            return 1;
    }
    return 0;
}


void editSetTrackerOffset(DefTracker *tracker)
{
    int        offset=0;

    if (!trackerFindIndex(tracker,&offset))
        return;
    start_draw_offset = offset - (ARRAY_SIZE(tracker_list))/2;
    start_draw_offset = MAX(0,start_draw_offset);
    uiScrollSetOffset(&tracker_scroll,offset - start_draw_offset);
}

void thrashEditor() {
    static int thrashState=0;
    static int libraryCount=0;
    int foo=38015;
    if (libraryCount==0) {
        printf("%d\n",foo);
        srand(foo);
    }
    libraryCount=objectLibraryCount();
    if (thrashState==0) {
        int dex=rand()%libraryCount;
        if (editIsWaitingForServer())
            return;
        libScrollCallback(objectLibraryNameFromIdx(dex),dex);
        thrashState=1;
    } else
    if (thrashState==1) {
        extern editCmdPaste();
        if (editIsWaitingForServer())
            thrashState=0;
        else {
            editCmdPaste();
            thrashState=rand()%5;
            if (thrashState!=0)
                thrashState=2;
            printf("%d\n",thrashState);
        }
    } else
    if (thrashState==2) {
        if (group_info.ref_count==0) {
            thrashState=0;
            return;
        }
        //pick a random group and select it, repeat with some probability
        while (rand()%45<35) {
            DefTracker * tracker;
            DefTracker * lastTracker;
            int ref=rand()%group_info.ref_count;
            tracker=group_info.refs[ref];
            while (rand()%50<45) {
                if (tracker->def==NULL) {
                    thrashState=0;
                    unSelect(1);
                    return;
                }
                if (groupInLib(tracker->def))
                    break;
                if (tracker->def->count && !tracker->count)
                    trackerOpenEdit(tracker);
                if (tracker->count==0)
                    break;
                lastTracker=tracker;
                tracker=&tracker->entries[rand()%tracker->count];
            }
            if (tracker->def==NULL) {
                thrashState=0;
                unSelect(1);
                return;
            }
            assert(tracker->def);
            selAdd(tracker,ref,1,EDITOR_LIST);
        }
        editCmdGroup();
        printf("grouped!\n");
        thrashState=0;
    }
}

int reloadPropertiesDef;

static void noteChangedProperties(const char *relpath, int when) {
    reloadPropertiesDef=1;
}

static int compareStringForQSort(const PropertyDef ** a,const PropertyDef ** b) {
    return stricmp((*a)->name,(*b)->name);
}

int editShowLibrary(int lost_focus)
{
static int init;
int        i,focus;
    if (edit_state.editorTrashing) {
        thrashEditor();
    }
    if (!init)          
    {
        init = 1;

        uiScrollInit(&lib_scroll); 
        lib_scroll.list_step = sizeof(lib_list[0]);
        lib_scroll.list_names = lib_list[0].name;
        lib_scroll.list_count = 0;
        lib_scroll.xpos = 8;
        lib_scroll.ypos = 34;
        lib_scroll.maxlines = 32;
        lib_scroll.maxcols = 32;
        lib_scroll.callback = libScrollCallback;
        lib_scroll.mouseDrag = 0;
        strcpy(lib_scroll.header,"groups");

        uiScrollInit(&cmd_scroll);
        cmd_scroll.list_step = sizeof(CmdScrollEnt);
        cmd_scroll.list_names = cmd_scroll_ents[0].name;
        cmd_scroll.list_count = ARRAY_SIZE(cmd_scroll_ents);
        cmd_scroll.xpos = 56*8 + TEXT_JUSTIFY;
        cmd_scroll.ypos = 51*8 + TEXT_JUSTIFY;
        cmd_scroll.maxlines = 10;
        cmd_scroll.maxcols = 24;
        cmd_scroll.grow_upwards = 1;
        cmd_scroll.callback = cmdScrollCallback;
        cmd_scroll.mouseDrag = 0;
        strcpy(cmd_scroll.header,"commands");

        trackerUpdateList(0,0,0);
        uiScrollInit(&tracker_scroll);
        tracker_scroll.list_step = sizeof(TrackerScrollEnt);
        tracker_scroll.list_names = tracker_list[0].name;
        tracker_scroll.list_count = tracker_list_count;
        tracker_scroll.xpos = 50*8 + TEXT_JUSTIFY;
        tracker_scroll.ypos = 34;
        tracker_scroll.maxlines = 30;
        tracker_scroll.maxcols = 30;
        tracker_scroll.grow_upwards = 0;
        tracker_scroll.callback = trackerScrollCallback;
        tracker_scroll.rollover = trackerScrollRollover;
        tracker_scroll.mouseDrag = 1;
        strcpy(tracker_scroll.header,"open groups");

        uiScroll2Init(&property_name_scroll);
        property_name_scroll.list_step = sizeof(PropertyEnt);
        property_name_scroll.list_names = NULL; //property_list[0].name_str;
        property_name_scroll.list_count = 0; //property_list_count;
        property_name_scroll.xpos = 8;
        property_name_scroll.ypos = 59 * 8 + TEXT_JUSTIFY;
        property_name_scroll.maxlines = 9;
        property_name_scroll.maxcols = 20;
        property_name_scroll.grow_upwards = 1;
        property_name_scroll.callback = propNameScrollCallback;
        property_name_scroll.callbackOnly = 0;
        property_name_scroll.linkedScroll = &property_value_scroll;
        strcpy(property_name_scroll.header,"Properties");

        uiScroll2Init(&property_value_scroll);
        property_value_scroll.list_step = sizeof(PropertyEnt);
        property_value_scroll.list_names = NULL; //property_list[0].value_str;
        property_value_scroll.list_count = 0; //property_list_count;
        property_value_scroll.xpos = 22 * 8;
        property_value_scroll.ypos = 59 * 8 + TEXT_JUSTIFY;
        property_value_scroll.maxlines = 9;
        property_value_scroll.maxcols = 8;
        property_value_scroll.grow_upwards = 1;
        property_value_scroll.callback = propValueScrollCallback;
        property_value_scroll.callbackOnly = 1;
        property_value_scroll.linkedScroll = &property_name_scroll;
        strcpy(property_value_scroll.header,"Value");

        //the new Menus 
        trackerListViewCreate();
        commandMenuCreate();

        ParserLoadFiles(NULL, "defs/properties.def", NULL,
            0, ParsePropertyDefList, &g_propertyDefList, NULL,NULL,NULL);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, "defs/properties.def", noteChangedProperties);
        {
            PropertyDef * pdef=ParserAllocStruct(sizeof(PropertyDef));
            memset(pdef,0,sizeof(PropertyDef));
            pdef->name=ParserAllocString(" ");
            eaPush(&g_propertyDefList.list,pdef);
        }
        {
            PropertyDef * pdef=ParserAllocStruct(sizeof(PropertyDef));
            memset(pdef,0,sizeof(PropertyDef));
            pdef->name=ParserAllocString("Other...");
            eaPush(&g_propertyDefList.list,pdef);
        }
        qsort(g_propertyDefList.list,eaSize(&g_propertyDefList.list)-1,sizeof(PropertyDef *),compareStringForQSort);
    }

    if (reloadPropertiesDef) {
        ParserDestroyStruct(ParsePropertyDefList,&g_propertyDefList);
        errorLogFileIsBeingReloaded("defs/properties.def");
        ParserLoadFiles(NULL, "defs/properties.def", NULL,
            0, ParsePropertyDefList, &g_propertyDefList, NULL,NULL,NULL);
        qsort(g_propertyDefList.list,eaSize(&g_propertyDefList.list)-1,sizeof(PropertyDef *),compareStringForQSort);
    }

    libUpdateList();
    lib_scroll.list_count = lib_list_count;

    tracker_list_skip = tracker_list_count = 0;
    {
        int        t = group_info.ref_count - ARRAY_SIZE(tracker_list);
        t = MAX(t,0);
        start_draw_offset = MIN(t,start_draw_offset);
    }
    for(i=0;i<group_info.ref_count;i++)
    {
        if (!group_info.refs[i]->def)
            continue;
        trackerUpdateList(i,group_info.refs[i],0);
    }
    {
        extern void propertiesMenuCreate();
        propertiesMenuCreate();
    }
    tracker_scroll.list_count = tracker_list_count;
    uiScrollSet(&tracker_scroll);

    focus=0;

    if (edit_state.useOldMenu)
        focus =  uiScrollUpdate(&tracker_scroll,lost_focus)    | uiScrollUpdate(&cmd_scroll,lost_focus) | uiScrollUpdate(&lib_scroll,lost_focus);

    edit_state.focusStolen=0;
    if (!edit_state.useOldMenu)
    {
        displayAllELV();
        handleAllELV();

        focus|=anyHaveFocusELV();
    }
    control_state.ignore = focus;
    return focus;
}
