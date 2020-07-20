#include "edit/edit.h"
#include "win/win_init.h"
#include "sound/sound.h"
#include "edit/edit_net.h"
#include "graphics/sun.h"
#include "graphics/gfx.h"
#include "graphics/gfxLoadScreens.h"
#include "gridcoll/gridcoll.h"
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/utils.h>
#include "edit/edit_select.h"
#include "edit/edit_cmd_file.h"
#include "group/group.h"
#include "group/groupfilelib.h"
#include "group/groupfileload.h"
#include "group/groupfileloadutil.h"
#include "group/groupproperties.h"
#include "group/groupdynrecv.h"
#include "graphics/groupMiniTrackers.h"
#include "cmdparse/cmdgame.h"
#include <process.h>
#include "assert.h"
#include "edit/edit_cmd.h"
#include "clientcomm/clientcomm.h"
#include "graphics/groupdraw.h"
#include "render/texWords.h"
#include "graphics/fog.h"
#include "render/tex.h"
#include "group/groupdyn.h"
#include "UI/uiQuit.h"
#include <utilitieslib/components/Earray.h>
#include "group/groupgrid.h"
#include "graphics/vistray.h"
#include "NovodeX/NwWrapper.h" 
#include "bases/basefromgroup.h" 
#include "graphics/zOcclusion.h"
#include "group/groupnovodex.h"
#include "bases/baseclientrecv.h"
#include "seq/anim.h"
#include <utilitieslib/utils/timing.h>
#include "group/grouputil.h"
#include "bases/baseparse.h"
#include "bases/baseraid.h"
#include "bases/bases.h"
#include "render/thread/rt_init.h"
#include "graphics/groupdrawutil.h"
#include "MissionControl.h"
#include "UI/uiBaseInput.h"
#include <utilitieslib/components/StashTable.h>
#include "UI/uiAutomap.h"
#include "seq/gfxtree.h"
#include "storyarc/zowieClient.h"
#include "UI/uiOptions.h"
#include "entity/entVarUpdate.h"
#include "entity/entDebug.h"
#include "graphics/gfxLoadScreens.h"
#include "game.h"
#include <utilitieslib/components/StringCache.h>

extern int globMapLoadedLastTick;
char gMapName[256] = {0};

static void groupSetDrawInfo(GroupDef *def)
{
    Vec3        size;
    F32            r1,r2;
    const F32    *mid;

    if (!def)
        return;
    mid = zerovec3;
    subVec3(def->max,mid,size);
    r1 = lengthVec3(size);
    subVec3(def->min,mid,size);
    r2 = lengthVec3(size);
    if (!def->lod_far)
        def->lod_far = def->radius * 10;
}

static int isMatch(char *s,char *cmd,int len)
{
    if (len != (int)strlen(cmd))
        return 0;
    return (strnicmp(s,cmd,len)==0);
}

static int bitSet(U32 flag,U32 bit)
{
    if (flag & bit)
        return 1;
    return 0;
}

static void getGeo(char *geo_name,GroupDef *def)
{
    def->model = groupModelFind(geo_name);
    groupSetTrickFlags(def);
}

int map_full_load;

void setClientFlags(GroupDef *def, const char *blameFileName)
{
    PERFINFO_AUTO_START("setClientFlags",1);

    groupSetDrawInfo(def);
    if (isFxTypeName(def->type_name))
        def->has_fx = 1;

    if (def->properties) {
        if ( stashFindElement(def->properties, "TexWordLayout", NULL))
            def->has_tex |= 1;
    }
    if (def->has_tex & 1)
    {
        TexBind *bind;
        PropertyEnt    *prop;

        if (def->properties && (prop = stashFindPointerReturnPointer(def->properties, "TexWordLayout"))) {
            // Has a TexWord layout file!
            TexWordParams *params = createTexWordParams();
            int i;
            params->layoutName = allocAddString(prop->value_str);
            eaCreate(&params->parameters);
            for (i=0; i<10; i++) { // Check for up to 10 parameters
                char key[16];
                sprintf(key, "TexWordParam%d", i+1);
                stashFindPointer( def->properties, key, &prop );
                if (prop) {
                    eaSetSize(&params->parameters, i+1);
                    eaSet(&params->parameters, prop->value_str, i);
                }
            }
            bind = texLoadDynamic(params,TEX_LOAD_DONT_ACTUALLY_LOAD, TEX_FOR_WORLD, blameFileName);
        } else {
            // Simple texture replacement
            bind = texLoad(def->tex_names[0],TEX_LOAD_DONT_ACTUALLY_LOAD, TEX_FOR_WORLD);
        }
        if(def->tex_binds.base)
            texRemoveRef(def->tex_binds.base);
        def->tex_binds.base = bind;
        if (bind->tex_layers[TEXLAYER_GENERIC])
            def->tex_binds.generic = bind->tex_layers[TEXLAYER_GENERIC];
    }
    if (def->has_tex & 2)
    {
        BasicTexture *bind = texLoadBasic(def->tex_names[1],TEX_LOAD_DONT_ACTUALLY_LOAD, TEX_FOR_WORLD);
        def->tex_binds.generic = bind;
    }
    
    PERFINFO_AUTO_STOP();
}

static void setAllClientFlags()
{
    int    i,k;

    for(k=0;k<group_info.file_count;k++)
    {
        GroupFile    *file = group_info.files[k];

        for(i=0;i<file->count;i++)
        {
            GroupDef    *def;

            def = file->defs[i];
            if (!def || !def->in_use)
                continue;
            setClientFlags(def, file->fullname);
        }
    }

}

static void clientDemoFixup()
{
    int            i;
    DefTracker    *ref;

    setAllClientFlags();
    for(i=0;i<group_info.ref_count;i++)
    {
        ref = group_info.refs[i];
        if (!ref->def)
            continue;

        //trackerUpdate(ref,ref->def,0);
        groupRefActivate(ref);
    }
}

#if 0
static int checkColorSwap(U8 rgb[4],StashTable previous)
{
    int            i,j,count,idx=0;
    ColorSwap    **swaps;
    U8            rgb_find[4];
    Vec3        vec1,vec2;
    F32            d,max_dot=0,scale;

    if (!rgb[0] && !rgb[1] && !rgb[2])
        return -1;
    memcpy(rgb_find,rgb,4);
    rgb_find[3] = 0;
    swaps = sceneGetColorSwaps(&count);
    if (!count)
        return -1;
    if (!stashFindInt(previous,rgb_find,&idx))
    {
        for(i=0;i<count;i++)
        {
            copyVec3(rgb,vec1);
            copyVec3(swaps[i]->src,vec2);
            normalVec3(vec1);
            normalVec3(vec2);
            d = dotVec3(vec1,vec2);
            if (d > max_dot)
            {
                idx = i;
                max_dot = d;
            }
        }
        if (max_dot > .98)
        {
            stashAddInt(previous,rgb_find,idx, false);
        }
        else
        {
            idx = -1;
            stashAddInt(previous,rgb_find,-1, false);
            return 0;
        }
    }
    if (idx < 0)
        return -1;
    copyVec3(rgb,vec1);
    copyVec3(swaps[idx]->src,vec2);
    scale = lengthVec3(vec1) / lengthVec3(vec2);
    for(j=0;j<3;j++)
        rgb[j] = swaps[idx]->dst[j] * scale;
    return 1;
}

void groupCheckColorSwap(GroupDef *def,StashTable lookup_cache)
{
    int        i;

    for(i=0;i<2;i++)
    {
        if (!checkColorSwap(def->color[i],lookup_cache))
            printf("ColorSwapFail (%3d %3d %3d) %s\n",4*def->color[i][0],4*def->color[i][1],4*def->color[i][2],def->name);
    }
}
#endif

void clientLoadMapStart(char *mapname)
{
    // Textures are not asked to be loaded while loading a map anymore (done in gfxLoadRelevantTextures)
    //texDynamicUnload(0);
    //texLoadQueueStart();
    char* fixedMapName;

    strcpy(gMapName,mapname);
    fixedMapName = strstri(gMapName, "maps");
    if (fixedMapName)
        strcpy(gMapName,fixedMapName);
    else
        strcpy(gMapName,mapname);
    forwardSlashes(gMapName);
    gfxSetBGMapname(gMapName);    
    showBgAdd(2);
  
    strcpy(game_state.world_name,mapname); 

    game_setAssertSystemExtraInfo();

    map_full_load = 1;
    unSelect(1);
    editShowTitle();

}

void finishLoadMap(bool tray_link_all, bool do_auto_weld, bool do_demo_fixup)
{
    int        i;

    automapClearRemainderIcons(-1);
    sunSetSkyFadeClient(0, 1, 0.0);

    if (!group_info.scenefile[0])
        sceneLoad("scenes/default_scene.txt");
    else
        sceneLoad(group_info.scenefile);

    PERFINFO_AUTO_START("automap_init", 1);
    
        automap_init();
    
    PERFINFO_AUTO_STOP_START("texCheckSwapList", 1);
    
        //texLoadQueueFinish();
        //texDynamicUnload(1);

        texCheckSwapList();
        globMapLoadedLastTick = 1; //entities need to konw to reset their lighting
        showBgAdd(-2);

    PERFINFO_AUTO_STOP_START("trackerUpdate", 1);

        for(i=0;i<group_info.ref_count;i++)
        {
            trackerUpdate(group_info.refs[i],group_info.refs[i]->def,0);
            groupRefActivate(group_info.refs[i]);
        }
        editNetUpdate();

    PERFINFO_AUTO_STOP_START("fogGather", 1);

        fogGather();

        if (tray_link_all)
        {
            extern Grid obj_grid;

            PERFINFO_AUTO_STOP_START("vistrayDetailTrays", 1);

            obj_grid.valid_id++;
            group_info.tray_opened = 0;
            vistrayDetailTrays();
        }

    if (do_auto_weld) {
        PERFINFO_AUTO_STOP_START("groupWeldAll", 1);
        groupWeldAll();
    }

    if (do_demo_fixup) {
        PERFINFO_AUTO_STOP_START("clientDemoFixup", 1);
        clientDemoFixup();
    }

    PERFINFO_AUTO_STOP_START("groupDrawBuildMiniTrackers", 1);
    groupDrawBuildMiniTrackers(); 

    PERFINFO_AUTO_STOP_START("ZowieLoad", 1);
    clientZowie_Load();

    PERFINFO_AUTO_STOP();


}

void groupInitNovodeX()
{
#ifdef NOVODEX
    if (nx_state.initialized)
    {
        nwDeinitializeNovodex();
    }
    nx_state.gameInPhysicsPossibleState = 1;
    nwInitializeNovodex();
#endif
}

void demoLoadMap(char *mapname)
{
    clientLoadMapStart(mapname);
    groupLoadFull(game_state.world_name);
    finishLoadMap(true, true, true);
    groupCreateAllCtris();
    groupInitNovodeX();
}

typedef enum OOS_Action {
    ACTION_IGNORE,
    ACTION_RELOAD,
    ACTION_EXIT,
} OOS_Action;
static OOS_Action clientServerOutOfSync(char *message)
{
    char buf[2048];
    int ret;
    if (isProductionMode())
        return ACTION_IGNORE;
    sprintf(buf, "\
ERROR: Client and server are out of sync, this can be caused by creating new \
library pieces or adding new geometry on the fly.  Solution is to save the map \
and library pieces and reload.\n\n\
Would you like this to be done automatically for you?  If you say yes, your \
changes will be saved, if you say No, the client will exit (you can manually \
restart the mapserver if you do not wish to save).\n\n\
%s", message);
    ret = winMsgYesNo(buf);
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000000)
        return ACTION_IGNORE;
    if (ret)
        return ACTION_RELOAD;
    else
        return ACTION_EXIT;
}
void fixClientServerOutOfSync(void)
{
    edit_state.savelibs = 1;
    edit_state.save = 1;
    edit_state.reload = 1;
    editProcess();
}

static void receiveAndCheckAllBounds(Packet *pak)
{
    char        fname[MAX_PATH],*objname;
    GroupFile    *file;
    int            i,k,svr_file_count,count;

    svr_file_count = pktGetBitsPack(pak,1);
    for(k=0;k<svr_file_count;k++)
    {
        strcpy(fname,pktGetString(pak));
        count = pktGetBitsPack(pak,1);
        file = groupFileFind(fname);

        for(i=0;i<count;i++)
        {
            F32        vis_dist;

            objname = pktGetString(pak);
            vis_dist = pktGetF32(pak);
            //assert(!file || vis_dist == file->defs[i]->vis_dist);
        }
    }
}

static void freeEntries(GroupDef *def)
{
    if (def && def->entries)
    {    //MW GROUPENT
        int j;
        for( j = 0 ; j < def->count ; j++ )
        {
            mpFree(group_info.mat_mempool,(Mat4Ptr)def->entries[j].mat);
        }
        free(def->entries);
    }
    def->entries = 0;
}

int receiveDef(Packet *pak,int id,GroupFile *file)
{
    int            j,in_use,has_geo,defs_not_in_bottom_up_order=0;
    char        *s;
    GroupEnt    *ent;
    GroupDef    *def,*future_def;
    extern GroupDef *groupDefPtrFromFuture(GroupFile *file,int id,GroupDef *future_def);

    in_use = pktGetBits(pak,1);
    if (!in_use)
    {
        def = groupDefPtr(file,id);
        groupDefFree(def,0);
        return 0;
    }
    s = pktGetString(pak);
    future_def = groupDefFind(s);
    def = groupDefPtrFromFuture(file,id,future_def);
    freeEntries(def);
    memset(def,0,sizeof(GroupDef));
    def->id = id;
    def->in_use = in_use;
    def->file = file;
    groupDefName(def,def->file->fullname,s);

    def->mod_time = pktGetBits(pak,32);

    def->count = pktGetBitsPack(pak,1);
    def->entries = calloc(def->count,sizeof(GroupEnt));
    for(j=0;j<def->count;j++)
    {
        char    *name;

        ent = &def->entries[j];
        name = pktGetString(pak);
        if (!ent->def || strcmp(ent->def->name,name)!=0)
        {
            ent->def = groupDefFind(name);
            if (!ent->def)
                ent->def = groupDefFuture(name);
        }
        ent->mat = mpAlloc(group_info.mat_mempool);
        getMat4(pak,(Mat4Ptr)ent->mat);
    }

    if (pktGetBits(pak,1))
    {
        F32    scale = pktGetIfSetF32(pak);
        if (scale != def->lod_scale)
            groupDrawCacheInvalidate();
        def->lod_scale = scale;
        def->lod_near = pktGetIfSetF32(pak);
        def->lod_far = pktGetIfSetF32(pak);
        def->lod_nearfade = pktGetIfSetF32(pak);
        def->lod_farfade = pktGetIfSetF32(pak);
        def->lod_fadenode = pktGetBits(pak,2);
        def->parent_fade = pktGetBits(pak,1);
        def->child_parent_fade = pktGetBits(pak,1);
        def->no_fog_clip = pktGetBits(pak,1);
        def->no_coll = pktGetBits(pak,1);
    }

    if (pktGetBits(pak,1))
    {
        def->open_tracker = pktGetBits(pak,1);
        def->child_light = pktGetBits(pak,1);
        def->child_ambient = pktGetBits(pak,1);
        def->child_volume = pktGetBits(pak,1);
        def->child_beacon = pktGetBits(pak, 1);
        def->child_no_fog_clip = pktGetBits(pak,1);
    }

    def->sound_radius = pktGetIfSetF32(pak);
    def->shadow_dist = pktGetIfSetF32(pak);

    if (pktGetBits(pak,1))
    {
        groupAllocString(&def->type_name);
        strcpy(def->type_name,pktGetString(pak));

        def->lib_ungroupable = pktGetBits(pak,1);

        def->has_light = pktGetBits(pak,1);
        def->has_ambient = pktGetBits(pak,1);
        if (def->has_ambient || def->has_light)
        {
            if (def->has_light)
                def->light_radius = pktGetIfSetF32(pak);
            for(j=0;j<8;j++)
                def->color[0][j] = pktGetBits(pak,8);
        }
        
        def->has_cubemap = pktGetBits(pak,1);
        if (def->has_cubemap)
        {
            def->cubemapGenSize = pktGetBits(pak,16);
            def->cubemapCaptureSize = pktGetBits(pak,16);
            def->cubemapBlurAngle = pktGetF32(pak);
            def->cubemapCaptureTime = pktGetF32(pak);
        }

        def->has_sound = pktGetBits(pak,1);
        if (def->has_sound)
        {
            char buf[256];
            groupAllocString(&def->sound_name);
            strcpy(def->sound_name,pktGetString(pak));
            strcpy(buf,pktGetString(pak));
            if (buf[0])
            {
                groupAllocString(&def->sound_script_filename);
                strcpy(def->sound_script_filename,buf);
            }
            else
                def->sound_script_filename=NULL;
            def->sound_vol = pktGetBits(pak,8);
            def->sound_ramp = pktGetF32(pak);
            def->sound_exclude = pktGetBits(pak,1);
        }

        def->has_fog = pktGetBits(pak,1);
        if (def->has_fog)
        {
            def->fog_near = pktGetF32(pak);
            def->fog_far = pktGetF32(pak);
            def->fog_radius = pktGetF32(pak);
            def->fog_speed = pktGetF32(pak);
            for(j=0;j<8;j++)
                def->color[0][j] = pktGetBits(pak,8);
        }

        def->has_volume = pktGetBits(pak,1);
        if (def->has_volume)
        {
            def->box_scale[0] = pktGetF32(pak);
            def->box_scale[1] = pktGetF32(pak);
            def->box_scale[2] = pktGetF32(pak);
        }

        def->has_beacon = pktGetBits(pak, 1);
        if(def->has_beacon)
        {
            groupAllocString(&def->beacon_name);
            strcpy(def->beacon_name, pktGetString(pak));
            def->beacon_radius = pktGetF32(pak);
        }

        def->has_tint_color = pktGetBits(pak, 1);
        if (def->has_tint_color)
        {
            for(j=0;j<3;j++)
                def->color[0][j] = pktGetBits(pak,8);
            for(j=0;j<3;j++)
                def->color[1][j] = pktGetBits(pak,8);
        }

        def->has_tex = pktGetBitsPack(pak,1);
        for(j=0;j<2;j++)
        {
            if (def->has_tex & (1 << j))
            {
                groupAllocString(&def->tex_names[j]);
                strcpy(def->tex_names[j],pktGetString(pak));
            }
        }

        def->has_alpha = pktGetBits(pak,1);
        if (def->has_alpha) {
            def->groupdef_alpha = pktGetF32(pak);
        } else {
            def->groupdef_alpha = 0;
        }

        def->has_properties = pktGetBitsPack(pak, 1);
        if(def->has_properties)
            def->properties = ReadGroupPropertiesFromPacket(pak);
    }

    {
        int k=pktGetBitsPack(pak,1);
        assert(!def->def_tex_swaps); // zeroed above
        while (k--)
        {
            int composite;
            char tex[MAX_PATH], rep[MAX_PATH];
            strcpy(tex, pktGetString(pak));
            strcpy(rep, pktGetString(pak));
            composite = pktGetBits(pak,1);
            eaPush(&def->def_tex_swaps, createDefTexSwap(tex, rep, composite));
        }
    }

    setClientFlags(def, file->fullname);

    groupLoadRequiredLibsForDef(def);
    has_geo = pktGetBits(pak,1);
    if (has_geo)
    {
        getGeo(pktGetString(pak),def);
    }
    def->hideForGameplay = pktGetBits(pak,1);
    groupSetDrawInfo(def);
    defs_not_in_bottom_up_order += groupSetBounds(def);
    groupSetVisBounds(def);
    return defs_not_in_bottom_up_order;
}

static bool newWorldIsBase()
{
    return ( eaSize(&g_base.rooms) > 0 );
}

static void receiveGroupFile(Packet *pak,int svr_file_idx,GroupFile *loadto)
{
    int            def_id,get_file,base_file,base_raid;
    char        *fname;
    U32            checksum;
    GroupFile    *file;

    get_file = pktGetBits(pak,1);
    if (!get_file)
        return;
    strcpy((fname=group_info.svr_file_names[svr_file_idx]),pktGetString(pak));
    checksum = pktGetBits(pak,32);
    base_file = pktGetBits(pak,1);
    g_isBaseRaid = 0;
    sgRaidCommonSetup();
    if (base_file)
    {
        base_raid = pktGetBits(pak,1);
        if (base_raid)
        {
            unsigned int numbase;
            unsigned int i;

            while (eaSize(&g_raidbases))
            {
                Base *base = eaRemove(&g_raidbases, 0);
                baseReset(base);
                free(base);
            }

            g_isBaseRaid = 1;
            g_baseRaidType = pktGetBits(pak, 32);
            numbase = pktGetBits(pak, 32);
            g_sgRaidNumSgInRaid = numbase - 1;

            for (i = 0; i < numbase; i++)
            {
                Vec3 offset;

                if (i < g_sgRaidNumSgInRaid)
                {
                    g_sgRaidSGIDs[i] = pktGetBits(pak, 32);
                    g_sgRaidRaidIDs[i] = pktGetBits(pak, 32);
                }
                offset[0] = pktGetF32(pak);
                offset[1] = pktGetF32(pak);
                offset[2] = pktGetF32(pak);
                baseReceive(pak, i + 1, offset, i + 1 == numbase);
            }
            sgRaidCommonSetup();
            return;
        }
        else
        {
            baseReceive(pak, 0, NULL, 0);
            return;
        }
    }
    if (!loadto)
    {
        if (!groupFileFind(fname) && !objectLibraryLoadBoundsFile(fname,checksum))
            groupLoadFull(fname);
    }
    else
        groupLoadTo(fname,loadto);

    file = 0;
    for(;;)
    {
        def_id = pktGetBitsPack(pak,1)-1;
        if (def_id < 0)
            break;
        if (!file)
            file = groupFileFromName(fname);
        receiveDef(pak,def_id,file);
    }
}

bool onIndoorMission(void)
{
    return (game_state.mission_map || game_state.alwaysMission) && (strstri( gMapName, "outdoor" ) == NULL);
}

static void receiveLoadScreenPages(Packet *pak)
{
    int index;

    for (index = eaSize(&g_comicLoadScreen_pageNames) - 1; index >= 0; index--)
    {
        free(g_comicLoadScreen_pageNames[index]);
    }
    eaDestroy(&g_comicLoadScreen_pageNames);
    g_comicLoadScreen_pageIndex = 0;

    if (pktGetBits(pak, 1))
    {
        int count = pktGetBitsAuto(pak);

        for (index = 0; index < count; index++)
        {
            eaPush(&g_comicLoadScreen_pageNames, strdup(pktGetString(pak)));
        }

        if (count)
        {
            // if count > 0, string is unused, but must be non-NULL
            gfxSetBGMapname("FAKE_STRING_LOAD_THE_PAGE_OMG");
        }
    }
}

// *************************************************************************
//  !!!!! please keep in sync with TestClient/externs.c/(483):void worldReceiveGroups(Packet *pak)
//  only the reads up to new_world and into that mattter
// *************************************************************************
void worldReceiveGroups(Packet *pak)
{
    static TrackerHandle *handles;
    static int handles_allocated;

    int            i,j,defs_not_in_bottom_up_order=0;
    DefTracker    *ref;
    int            new_world;
    extern int editorUndosRemaining;
    extern int editorRedosRemaining;
    extern Grid obj_grid;
    GroupFile    *grp_file=0;
    DefTracker *lightTracker;    //used to create the non-checking handle above.
    int handles_count;
    Mat4 selParentMat;
    Mat4 selOffsetMat;
    Mat4 selScaleMat;
    int copied_mats = 0;

    PERFINFO_AUTO_START("top", 1);

        #if NOVODEX
            nx_state.gameInPhysicsPossibleState = 0;
            nwDeinitializeNovodex();
        #endif
    
        zoClearOccluders();


        //Do not check that names of lights match.
        lightTracker = trackerFromHandle(edit_state.lightHandle);
        if(lightTracker)
        {
            //get rid of names...
            trackerHandleClear(edit_state.lightHandle);
            trackerHandleFillNoAllocNoCheck(lightTracker, edit_state.lightHandle);
        }

        // this is so we can keep things selected during a server update (which could come
        // any time because of an autosave)
        handles_count = sel_count;
        if(handles_allocated < handles_count)
        {
            SAFE_FREE(handles);
            handles = calloc(handles_count, sizeof(handles[0]));
            handles_allocated = handles_count;
        }
        for (j=0;j<sel_count;j++)
            trackerHandleFillNoAllocNoCheck(sel_list[j].def_tracker, &handles[j]);

        if (sel.offsetnode && sel.parent && sel.scalenode)
        {
            copyMat4(sel.offsetnode->mat,selOffsetMat);
            copyMat4(sel.parent->mat,selParentMat);
            copyMat4(sel.scalenode->mat,selScaleMat);
            copied_mats = 1;
        }
        unSelect(1);

        if (game_state.sound_info)
            sndStopAll();
        obj_grid.valid_id++;
        editorUndosRemaining = pktGetBitsPack(pak,1);
        editorRedosRemaining = pktGetBitsPack(pak,1);
        for(i=0;i<group_info.ref_count;i++)
            trackerUpdate(group_info.refs[i],group_info.refs[i]->def,0);
        edit_state.last_selected_tracker = NULL;

        new_world = pktGetBits(pak,1);
        map_full_load = 0;
        if (new_world)
        {
            char    *fname;
            
            game_state.mission_map = pktGetBits(pak,2);
            game_state.map_instance_id = pktGetBitsPack(pak,1);
            game_state.doNotAutoGroup = pktGetBitsPack(pak,1); //MW added
            game_state.iAmAnArtist = game_state.doNotAutoGroup && isDevelopmentMode();
            game_state.base_map_id = pktGetBitsPack(pak, 1);
            game_state.intro_zone = pktGetBitsAuto(pak);
            game_state.team_area = pktGetBitsPack(pak, 3);

            receiveLoadScreenPages(pak);

            PERFINFO_AUTO_START("groupReset", 1);
                groupReset();
            PERFINFO_AUTO_STOP_CHECKED("groupReset");
            baseReset(&g_base);
            zowieReset();
            if (!game_state.local_map_server) {
                if (game_state.edit_base != kBaseEdit_None)
                    baseSetLocalMode(kBaseEdit_None);
            }
            
            strncpy(group_info.loadscreen, pktGetString(pak), MAX_PATH);

            fname = pktGetString(pak);
            if (!group_info.file_count)
                grp_file = groupFileNew();
            else
                grp_file = group_info.files[0];
            groupFileSetName(grp_file,fname);
            clientLoadMapStart(fname);
        }
        game_state.beacons_loaded = pktGetBits(pak, 1);

    PERFINFO_AUTO_STOP_START("middle", 1);

        {
            int            k,file_count;

            file_count = pktGetBitsPack(pak,1);
            if (group_info.svr_file_count != file_count)
            {
                group_info.svr_file_names = realloc(group_info.svr_file_names,sizeof(group_info.svr_file_names[0]) * file_count);
                group_info.svr_file_count = file_count;
            }
            for(k=1;k<file_count;k++)
                receiveGroupFile(pak,k,0);
            if (file_count)
                receiveGroupFile(pak,0,grp_file);
        }

    PERFINFO_AUTO_STOP_START("middle2", 1);

        // Read in group references
        for(;;)
        {
            char        *defname;
            GroupDef    *def;

            i = pktGetBitsPack(pak,1);
            if (i < 0)
                break;
            ref = groupRefPtr(i);
            vistrayClosePortals(ref);
            vistrayFreeDetail(ref);

            if (!edit_state.isDirty && !new_world) {
                edit_state.isDirty=1;
                editShowTitle();
            }
            defname = pktGetString(pak);
            if (defname[0])
            {
                def = groupDefFind(defname);
                if (!def)
                {
                    char    buf[1000];
                    OOS_Action action;

                    sprintf(buf,"Def \"%s\" not found",defname);
                    action = clientServerOutOfSync(buf);
                    if (action == ACTION_RELOAD) {
                        // auto fix it and leave this function
                        fixClientServerOutOfSync();
                        return;
                    } 
                    else if (action == ACTION_EXIT) 
                    {
                        quitToLogin(0);
                        return;
                    }
                }
                ref->def = def;
                getMat4(pak,ref->mat);
                ref->dirty=1;
                if (i >= group_info.ref_count)
                    group_info.ref_count = i+1;
                if(ref && ref->def){
                    ref->def_mod_time = ref->def->mod_time;
                }
                obj_grid.valid_id++;
                if (!new_world && edit_state.liveTrayLink)
                    vistrayAddDetail(ref);
                // calling trackerUpdate() with force of 1 is expensive.  Could we only set force to 1 if !new_world?
                trackerUpdate(ref,ref->def,1);
                // Need to also force a full relight.  Or, we could get flickering for a few frames
                game_state.forceFullRelight = 1;
            }
            else
            {
                trackerClose(ref);
                groupRefDel(ref);
                ref->def = 0;
            }
        }

    PERFINFO_AUTO_STOP_START("middle3", 1);

        {
            U32 defCount = pktGetBitsPack(pak, 1);
            U32 refCount = pktGetBitsPack(pak, 1);
            U32 defsCRC = pktGetBits(pak, 32);
            U32 refsCRC = pktGetBits(pak, 32);
            OOS_Action action = ACTION_IGNORE;
            if (new_world && isDevelopmentMode() && game_state.local_map_server) 
            {
                #if 0
                    if (defsCRC != groupInfoCrcDefs()) {
                        action = clientServerOutOfSync("Defs CRCs don't match");
                    }
                #endif
                
                if (action == ACTION_IGNORE && refsCRC != groupInfoCrcRefs()) {
                    action = clientServerOutOfSync("Refs CRCs don't match");
                }
                if (action == ACTION_IGNORE && refCount != groupInfoRefCount()) {
                    action = clientServerOutOfSync("Ref count doesn't match");
                }
                if (action == ACTION_RELOAD) 
                {
                    // auto fix it and leave this function
                    fixClientServerOutOfSync();
                    return;
                } 
                else if (action == ACTION_EXIT) 
                {
                    quitToLogin(0);
                    return;
                }
            }
        }

    PERFINFO_AUTO_STOP_START("bottom", 1);

        if (1 || new_world || defs_not_in_bottom_up_order)
        {
            PERFINFO_AUTO_STOP_START("groupRecalcAllBounds", 1);
            groupRecalcAllBounds();
        }

        // this reselects anything that was unselected so that the server update appears seamless
        for (j=0;j<group_info.ref_max;j++)
        {
            if (group_info.refs[j]) //&& group_info.refs[j]->def && group_info.refs[j]->dirty)
            {
                trackerUpdate(group_info.refs[j],group_info.refs[j]->def,g_base.curr_id ? 0 : 1);//1);
    //                groupRefGridDel(group_info.refs[j]);
                groupRefActivate(group_info.refs[j]);
                group_info.refs[j]->dirty=0;
            }
        }
        {
            GfxNode *temp = sel.parent;
            sel.parent=NULL;
            unSelect(1);
            for(j = 0; j < handles_count; j++)
            {
                TrackerHandle *handle = &handles[j];
                DefTracker *tracker = trackerFromHandle(handle);
                if(tracker && tracker->def)
                    selAdd(tracker, handle->ref_id, 1, 0);
            }
            sel.parent=temp;
            selCopy(NULL);
            
            //fill the lightHandle back in with the updated Name information.
            lightTracker = trackerFromHandle(edit_state.lightHandle);
            if(lightTracker)
                trackerHandleFill(lightTracker, edit_state.lightHandle);

            if(sel.offsetnode && sel.parent && sel.scalenode && copied_mats)
            {
                copyMat4(selOffsetMat,sel.offsetnode->mat);
                copyMat4(selScaleMat,sel.scalenode->mat);
                copyMat4(selParentMat,sel.parent->mat);
            }
        }

        if (new_world)
        {
            //setAllClientFlags(); // JE: Moved to happen when defs are created, since not all defs are created at this point!
            PERFINFO_AUTO_STOP_START("finishLoadMap", 1);
            finishLoadMap(true, true, false);
        }
        PERFINFO_AUTO_STOP_START("groupRecalcAllBounds", 1);
        groupRecalcAllBounds();
        PERFINFO_AUTO_STOP_START("groupTrackersHaveBeenReset", 1);
        groupTrackersHaveBeenReset();
//        PERFINFO_AUTO_STOP_START("groupDynBuildBreakableList", 1);
        if(new_world && !newWorldIsBase())
        {
            PERFINFO_AUTO_STOP_START("groupCreateAllCtris", 1);
            groupCreateAllCtris();
            PERFINFO_AUTO_STOP_START("groupInitNovodeX", 1);
            groupInitNovodeX();
        }
        PERFINFO_AUTO_STOP_START("MissionControlReprocess", 1);
        MissionControlReprocess();
        if (!new_world) {
            PERFINFO_AUTO_STOP_START("fogGather", 1);
            fogGather();
            PERFINFO_AUTO_STOP_START("groupDrawBuildMiniTrackers", 1);
            groupDrawBuildMiniTrackers();
        }

        updateCutSceneCameras(); 

    PERFINFO_AUTO_STOP();
}

void receiveDynamicDefChanges(Packet *pak) {
    int i;
    int numDefs = pktGetBitsPack(pak, 1);

    for (i = 0; i < numDefs; ++i) {
        char *defName = pktGetString(pak);

        if (defName) {
            GroupDef *def = groupDefFind(defName);

            if (def)
                def->hideForGameplay = !!pktGetBitsPack(pak, 1);
        }
    }

    fogGather();
}

/*
#include <utilitieslib/utils/error.h>
#include "group/grouputil.h"
static FILE *dump_fout=NULL;
static int uid=0;
static int dumpDebugGroupTree_cb(GroupDefTraverser *def_trav)
{
    GroupDef *def = def_trav->def;
    fprintf(dump_fout, "%d: Group: %s\n", uid++, def->name?def->name:"UNNAMED");
    fprintf(dump_fout, "  LODs: %1.3f %1.3f %1.3f %1.3f %1.3f\n", def->lod_far, def->lod_farfade, def->lod_near, def->lod_nearfade, def->lod_scale);
    fprintf(dump_fout, "  LOD bits: %d %d %d\n", def->lod_autogen, def->lod_fadenode, def->lod_fromtrick);
    fprintf(dump_fout, "  %s %1.3f %d %d %d %d %d %d %d %d\n", def->beacon_name, def->beacon_radius, def->breakable, def->child_ambient, def->child_beacon, def->child_breakable, def->child_has_fog, def->child_light, def->child_parent_fade, def->child_properties);
    fprintf(dump_fout, "  %d %d %s\n", def->color[0][0], def->count, def->dir);
    fprintf(dump_fout, "  %d %d %d %d %d %d %d %d %d\n", def->door_volume, def->file_idx, def->has_ambient, def->has_beacon, def->has_fog, def->has_fx, def->has_light, def->has_properties, def->has_sound, def->has_tex, def->has_tint_color);
    fprintf(dump_fout, "  %d %d %d %d %d %f\n", def->id, def->init, def->key_light, def->lava_volume, def->lib_ungroupable, def->light_radius);
    fprintf(dump_fout, "  %d %1.3f %1.3f %1.3f %1.3f %1.3f %1.3f\n", def->materialVolume, def->max[0], def->max[1], def->max[2], def->mid[0], def->mid[1], def->mid[2]);
    fprintf(dump_fout, "  %1.3f %1.3f %1.3f %d %d %d %d %d %d\n", def->min[0], def->min[1], def->min[2], def->no_beacon_ground_connections, def->obj_lib, def->open, def->open_tracker, def->outside, def->parent_fade);
    if (def->model) {
        fprintf(dump_fout, "  Model %s T:%d V:%d\n", def->model->name?def->model->name:"UNNAMED", def->model->tri_count, def->model->vert_count);
        if (def->model->tri_count) {
            if (def->model->ctris) {
                fprintf(dump_fout, "  ctri flags: %d\n", def->model->ctris[0].flags);
            } else {
                fprintf(dump_fout, "  NO CTRIS\n");
            }
        } else {
            fprintf(dump_fout, "  NO TRIANGLES\n");
        }
    } else {
        fprintf(dump_fout, "  NO MODEL\n");
    }

    return 1;
}

void dumpDebugGroupTree(void)
{

    loadstart_printf("dumping debug record of group values!\n");
    {
        GroupDefTraverser traverser={0};
        extern int ctri_total;
        dump_fout = fopen("C:/groupInfoNew.txt", "w");
        groupProcessDefExBegin(&traverser, &group_info, dumpDebugGroupTree_cb);
        fprintf(dump_fout, "ctri total: %d\n", ctri_total);
        fclose(dump_fout);
    }
    loadend_printf("done");
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000000)
        exit(1);
}












#if 0
            for(i=0;;i++)
            {
                char    *objname;
                F32        radius,vis_dist,lod_scale,shadow_dist;
                Vec3    min,max,mid;
                GroupDef    *def;

                objname = pktGetString(pak);
                if (!objname[0])
                    break;
                vis_dist = pktGetF32(pak);
                radius = pktGetF32(pak);
                lod_scale = pktGetF32(pak);
                shadow_dist = pktGetF32(pak);
                pktGetVec3(pak,min);
                pktGetVec3(pak,max);
                pktGetVec3(pak,mid);
                def = groupDefFind(objname);
                assert(def->vis_dist == vis_dist);
            }
#endif
#if 0
            file = groupFileFromName(fname);
            if (file->idx != k)
            {
                GroupFile    *other = group_info.files[k];
                group_info.files[k] = file;
                group_info.files[file->idx] = other;
                other->idx = file->idx;
                file->idx = k;
            }

    if (new_world)
        groupFileFromName(mapname);



    // Read in group definitions
    for(;;)
    {
        int        file_idx;

        file_idx = pktGetBitsPack(pak,1);
        if (file_idx < 0)
            break;
        i = pktGetBitsPack(pak,1);
        assert(file_idx < group_info.file_count && group_info.files[file_idx]);
        file = group_info.files[file_idx];
            
        //i += last_def_idx + 1;
        def = groupDefPtr(file,i);
        receiveDef(pak,def,i,file);
    }
#endif



*/
