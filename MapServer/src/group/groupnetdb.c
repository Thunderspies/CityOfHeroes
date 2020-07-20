#include "group/groupfileload.h"
#include "group/groupfileloadutil.h"
#include "group/groupfilesave.h"
#include "group/groupfilelib.h"
#include "groupjournal.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>
#include "groupnetsend.h"
#include <utilitieslib/network/netcomp.h>
#include "groupitems.h"
#include "group/grouptrack.h"
#include "beacon/beacon.h"
#include "beacon/beaconPrivate.h"
#include "beacon/beaconFile.h"
#include "generator/entgen.h"
#include "missionMapInit.h"
#include "cmdparse/cmdserver.h"
#include "generator/entgenLoader.h"    // for spawnAreaReload()
#include "comm_game.h"
#include "groupdb_util.h"
#include "groupdbmodify.h"
#include <utilitieslib/utils/utils.h>
#include "group/groupdyn.h"
#include "dbcomm/dbdoor.h"
#include "storyarc/locationTask.h"    // for VisitLocationLoad()
#include "plaque.h"
#include "zowie.h"
#include "dbcomm/dbcomm.h"
#include "beacon/beaconConnection.h"
#include "kiosk.h"
#include "svr/svr_base.h"
#include "group/groupscene.h"
#include "comm_game.h"
#include "gridcoll/gridcache.h"
#include "gameSys/arenakiosk.h"
#include "groupdynsend.h"
#include "entity/entity.h"
#include "group/group.h"
#include "group/groupProperties.h"
#include <utilitieslib/utils/file.h>
#include "bases/bases.h"
#include "bases/basedata.h"
#include "bases/baseparse.h"
#include "bases/basetogroup.h"
#include "bases/baseserverrecv.h"
#include "container/baseloadsave.h"
#include "seq/anim.h"
#include "group/groupnovodex.h"
#include "NovodeX/NwWrapper.h"
#include "script/scriptengine.h"
#include "entity/entserver.h"
#include "beacon/beaconClientServerPrivate.h"
#include "gameSys/raidmapserver.h"
#include "MissionControl.h"
#include "dbcomm/dbmapxfer.h"
#include "storyarc/playerCreatedStoryarcServer.h"
//#include "storyarcprivate.h"
#include "gameSys/sgraid_V2.h"
#include "storyarc/missiongeoCommon.h"


int        no_update;
int        world_modified;
int        editOrigNoCheckout;            //if edit orig is on we want to update everything,
//but not check anything out (excpet for the case below)
DefTracker * editOrigCheckouts[50];    //layers that are supposed to be checked out because they
                                    //contain the defenition of the thing modified
void insertUniqueEntry(DefTracker * tracker) {
    int i=0;
    while (editOrigCheckouts[i]!=0 && editOrigCheckouts[i]!=tracker) i++;
    editOrigCheckouts[i]=tracker;
}

DefTracker * getGreatestParent(DefTracker * tracker) {
    if (tracker==NULL) return NULL;
    if (tracker->parent!=NULL) return getGreatestParent(tracker->parent);
    return tracker;
}




//Info about the edit state of each layer
typedef struct LayerCheckoutStatus
{
    char name[256];
    int perforceStatus;
} LayerCheckoutStatus;

LayerCheckoutStatus** layerCheckoutStatuses; 


void clearLayerCheckoutStatuses()
{
    if ( layerCheckoutStatuses )
    {
        eaDestroyEx(&layerCheckoutStatuses, NULL);
        layerCheckoutStatuses = NULL;
    }
}

void pktGetTrackerHandle(Packet *pak, TrackerHandle *handle)
{
    int i;
    assert(handle);
    ZeroStruct(handle);
    handle->ref_id = pktGetBitsPack(pak, 1);
    handle->depth = pktGetBitsPack(pak,1);
    for(i = 0; i < handle->depth; i++)
    {
        char *name;
        handle->idxs[i] = pktGetBitsPack(pak,1);
        name = pktGetString(pak);
        handle->names[i] = name && name[0] ? strdup(name) : NULL;
    }
}

TrackerHandle* pktGetTrackerHandleAlloc(Packet *pak)
{
    TrackerHandle *handle = calloc(1, sizeof(*handle));
    pktGetTrackerHandle(pak, handle);
    return handle;
}

DefTracker *pktGetTrackerHandleUnpacked(Packet *pak)
{
    DefTracker *tracker;
    TrackerHandle handle;
    pktGetTrackerHandle(pak, &handle);
    tracker = trackerFromHandle(&handle);
    trackerHandleClear(&handle);
    return tracker;
}

void pktGetTrackerHandleNoAllocPreChecked(Packet *pak, TrackerHandle *handle)
{
    trackerHandleFillNoAllocNoCheck(pktGetTrackerHandleUnpacked(pak), handle);
}


// returns the id of the new item
int map_full_load;

int worldModified()
{
    int        t;

    t = world_modified;
    world_modified = 0;
    return t;
}

void groupLoadMap(char *mapname,int import,int do_beacon_reload)
{
    void entKillAll();
    char * pchMap;

    map_full_load = !import;
    forwardSlashes(mapname);
    pchMap = mapname;

#if !defined(BEACONIZER)
    if (mapname && isDevelopmentMode())
    {
        if (fileIsAbsolutePath(mapname))
        {
            char *dataDir = fileDataDir();
            if (strnicmp(dataDir, mapname, strlen(dataDir)) != 0)
            {
                sendEditMessage(NULL, EDITOR_MESSAGE_ERROR,
                    "Error: Loading map (%s) from a different branch. The file should come from (%s).",
                    mapname, dataDir);
                return;
            }
        }
    }
#endif

    loadstart_printf("loading map: %s..",mapname);
    entKillAll();
    if (!import)
    {
        entFreeGridAll();
        groupReset();
    }
    if (!import)
    {
        //Reset the state of the map
        world_id++;
        world_name[0] = 0;
        strcpy(db_state.map_name, mapname);
        // If the map was in a mission, clear the mission, and end the scripts
        if (db_state.local_server)
        {
            MissionForceUninit();
        }

        clearLayerCheckoutStatuses(); //Relookup which files you have checked out.
        group_info.scenefile[0] = 0;
        {
            char    *s,*db,*args[10],*map_data;
            int        swapped=0;
            int sg1, sg2;

            db = strdup(mapname);
            for(s=db;*s;s++)
            {
                if (*s == ',' || *s == '=')
                {
                    *s = ' ';
                    swapped++;
                }
            }
            if (swapped == 3 && tokenize_line_safe(db,args,ARRAY_SIZE(args),0) == 4)
            {
                map_data = baseLoadMapFromDb(atoi(args[1]),atoi(args[3]));
                if (map_data[0]) // do we have an existing base?
                {
                    int i,j;
                    baseFromStr(map_data,&g_base);
                    baseFixBaseIfBroken(&g_base);
                    g_base.supergroup_id = atoi(args[1]);
                    g_base.user_id = atoi(args[3]);
                    // General cleanup
                    for(i=eaSize(&g_base.rooms)-1; i>=0; i--)
                    {
                        BaseRoom *room = g_base.rooms[i];
                        for(j=eaSize(&room->details)-1; j>=0; j--)
                        {
                            room->details[j]->iAuxAttachedID = 0; // Aux will be reattached during baseToDefs
                        }
                    }
                }
                else
                {
                    g_base.supergroup_id = atoi(args[1]);
                    if(g_base.user_id = atoi(args[3])) // userid
                        g_base.roomBlockSize = APARTMENT_BLOCK_SIZE;
                    baseCreateDefault(&g_base);
                }
                baseResetIds(&g_base);
                baseToDefs(&g_base, 0);
                beaconRequestBeaconizing(db_state.map_name);
            }
            else if(2 == sscanf(db_state.map_name, "%i vs %i", &sg1, &sg2))
            {
                int raidOK;
                int i;
                int numBasesInRaid = 2;

                g_isBaseRaid = 1;

                raidOK = sgRaidParseInfoStringAndInit();
                assert(raidOK);

                // This will need a bit of a rework if we put more than two bases on a map
                for (i = 0; i < numBasesInRaid; i++)
                {
                    raidOK = sgRaidLoadPhase1(i);
                    assert(raidOK);
                }
                for (i = 0; i < numBasesInRaid; i++)
                {
                    raidOK = sgRaidLoadPhase2(i);
                    assert(raidOK);
                }
                raidOK = sgRaidSetupNoMansLand();
                assert(raidOK);

                //sgRaidDumpMapToConsole(); // for debugging
                beaconRequestBeaconizing(db_state.map_name);
            }
            else
            {
                if (strEndsWith(mapname,".base"))
                {
                    baseFromFile(mapname,&g_base);
                    baseFixBaseIfBroken(&g_base);
                    baseToDefs(&g_base, 0);
                }
                else
                    groupLoad(mapname);
            }
            free(db);
        }
    }
    else
        groupLoadTo(mapname,group_info.files ? group_info.files[0] : 0);

    groupRecalcAllBounds();
    {
        int        i;

        for(i=1;i<group_info.file_count;i++)
            objectLibraryWriteBoundsFileIfDifferent(group_info.files[i]);
    }
    group_info.completed_map_load_time = ++group_info.ref_mod_time;

    if (!group_info.scenefile[0])
        sceneLoad("scenes/default_scene.txt");
    else
        sceneLoad(group_info.scenefile);

    if (!import)
    {
        char buffer[1000];

        strcpy(world_name,pchMap);

        sprintf(buffer, "Map: %s\n", world_name);

        setAssertExtraInfo(buffer);

        group_info.lastSavedTime = ++group_info.ref_mod_time; //After the load, because loads register as modifies
    }

    world_modified = 1;
    verbose_printf("entgenReload\n");

    if(!server_state.beaconProcessOnLoad)
    {
        spawnAreaReload(pchMap);
    }

    loadend_printf("done"); // map load

    if ((server_state.dummy_raid_base || !OnSGBase()) &&
        do_beacon_reload &&
        server_state.levelEditor!=1)
    {
        loadstart_printf("loading beacons.. ");
        beaconReload();
        loadend_printf("done");
    }

    if(!server_state.beaconProcessOnLoad)
    {
        initMap(0);

        if (g_isBaseRaid)
        {
            sgRaidCommonSetup();
            sgRaidInitBaseRaidScript();
        }

        VisitLocationLoad();
        plaque_Load();
        kiosk_Load();
        ArenaKioskLoad();
        zowie_Load();
        if (isDevelopmentMode())
        {
            overlappingVolume_Load();
        }
    }

    verbose_printf("loadmap done\n");
    groupDynBuildBreakableList();

    if (server_state.prune)
        groupSavePrunedLibs();

    loadstart_printf("unpacking all collision triangles...");
    groupCreateAllCtris();
    loadend_printf("done");

#if NOVODEX

    loadstart_printf("Starting novodex...");

    if ( nwInitializeNovodex() )
    {
        loadend_printf("success");
    }
    else
    {
        loadend_printf("failed");
    }

    loadstart_printf("freeing all .geo files...");
    modelFreeAllGeoData();
    loadend_printf("done");

#endif
}

static int diffColor(GroupDef *old,GroupDef *curr)
{
    int        i;

    if (old->has_ambient != curr->has_ambient)
        return 1;
    if (old->has_light != curr->has_light)
        return 1;
    if (old->light_radius != curr->light_radius)
        return 1;
    for(i=0;i<3;i++)
    {
        if (old->color[0][i] != curr->color[0][i])
            return 1;
    }
    return 0;
}

#define GETDEFSTR(strname) \
    strcpy(buf,pktGetString(pak)); \
    strname = buf; \
    buf += strlen(strname) + 1;

static void getGroupDef(Packet *pak,GroupDef *def,char *buf)
{
    int        def_size;
    char    *dir;

    def_size = pktGetBitsPack(pak,1);
    if (def_size != sizeof(GroupDef))
    {
        Errorf("trackerupdate size mismatch %d/%d",sizeof(GroupDef),def_size);
        return;
    }

    pktGetBitsArray(pak,def_size * 8,def);

    GETDEFSTR(dir);
    GETDEFSTR(def->name);
    GETDEFSTR(def->type_name);
    GETDEFSTR(def->beacon_name);
    GETDEFSTR(def->sound_name);
    GETDEFSTR(def->sound_script_filename);
    GETDEFSTR(def->tex_names[0]);
    GETDEFSTR(def->tex_names[1]);
    def->file = groupFileFromName(dir);
    if(def->has_properties)
    {
        def->properties = ReadGroupPropertiesFromPacket(pak);
        if (def->properties)
        {
            char    *s;
            stashFindPointer( def->properties,"Health", &s );
            if (s!=NULL)
            {
                def->breakable=1;
            }
        }
    } else
        def->properties = 0;
    {
        int k = pktGetBitsPack(pak,1);
        def->def_tex_swaps = NULL; // may have been set to something random by pktGetBitsArray above
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
}

// If is_error is <0, then we are just going to store the link* in case of a NULL later
void sendEditMessage(NetLink *link, EditorMessageType msgType, const char *fmt, ...) {
    static NetLink *savedlink;
    char *buf = estrTemp();
    va_list arglist;
    Packet    *pak = pktCreate();
    int printToServerConsole = 0;

    //More dumbness
    if( msgType == EDITOR_MESSAGE_ERROR_SERVER_WRITE )
    {
        printToServerConsole = 1;
    }

    //Somebody's dumb idea: if the msgType (previously isErr) is -1, then just save the link ) 
    if ( msgType == EDITOR_MESSAGE_JUST_SAVE_LINK ) {
        savedlink = link;
        return;
    } else if (link==NULL) {
        //assert(savedlink);
        link = savedlink;
    }

    va_start(arglist, fmt);
    estrConcatfv(&buf, fmt, arglist);
    va_end(arglist);

    pktSendBitsPack(pak,1,SERVER_EDITMSG);
    pktSendBits(pak,2, msgType);
    pktSendString(pak, buf);
    pktSend(&pak,link);

    if( printToServerConsole ) 
        printf("%s\n", buf); // Display message on mapserver window as well as on client

    estrDestroy(&buf);
}

static void sendSaveInfo(NetLink *link,char *name,int num_bytes)
{
    //if (num_bytes <=0) {
    //    sendEditMessage(link, 1, "Could not save %s: wrote %d bytes\nPerhaps it is not checked out or no changes have been made.",name,num_bytes);
    //} else {
    sendEditMessage( link, 0, "Saved %s",name );
    //}
    /*    compacted into sendEditMessage call above:
    char    buf[1000];
    Packet    *pak = pktCreate();

    pktSendBitsPack(pak,1,SERVER_EDITMSG);
    pktSendBits(pak,1,num_bytes <= 0);
    sprintf(buf,"%s: wrote %d bytes",name,num_bytes);
    pktSendString(pak,buf);
    pktSend(&pak,link);*/
}

static void groupdbUngroup(Packet *pak)
{
    int i, count;
    UpdateMode mode;
    TrackerHandle *handles;

    mode = pktGetBitsPack(pak,1);
    count = pktGetBitsPack(pak,1); // I haven't tested count>1, but it should work fine (with the sorting I added below) -GG
    handles = calloc(count, sizeof(*handles));
    for(i = 0; i < count; i++)
        pktGetTrackerHandleNoAllocPreChecked(pak, &handles[i]);
    if(count)
        qsort(handles, count, sizeof(*handles), trackerHandleComp); // sorts reverse from what we want

    for(i = count-1; i >= 0; --i) // ungroup deepest handles first
    {
        groupdbInstanceTracker(trackerFromHandle(&handles[i]), mode);
        groupdbUngroupObject(&handles[i], mode);
    }
    free(handles);
}

static void groupdbGroupOrAttach(Packet *pak,char *cmd_name)
{
    TrackerHandle *handles, refHandle;
    int count;
    int pivot_idx, use_origin;
    int attach;
    int i;
    DefTracker *parent, *ref;
    UpdateMode mode;
    Mat4 pivot;

    mode        = pktGetBitsPack(pak,1);
    attach        = strcmp(cmd_name,"attach")==0;
    count        = pktGetBitsPack(pak,1);
    pivot_idx    = pktGetBitsPack(pak,1);
    use_origin    = pivot_idx < 0;
    parent        = pktGetTrackerHandleUnpacked(pak);


    // do all of the checking now.  if there are multiple deletions, the groups' names could change.
    // set things up to do the pivot stuff later
    handles = calloc(count, sizeof(handles[0]));

    if(use_origin)
        copyMat4(unitmat, pivot);
    else
        assert(INRANGE0(pivot_idx, count));

    for(i = 0; i < count; i++)
    {
        DefTracker *tracker = pktGetTrackerHandleUnpacked(pak);
        trackerHandleFillNoAllocNoCheck(tracker, &handles[i]);
        if(use_origin)
            addVec3(tracker->mat[3], pivot[3], pivot[3]);
        else if(i == pivot_idx)
            copyMat4(tracker->mat, pivot);
    }
    if(use_origin)
        scaleVec3(pivot[3], 1.0/count, pivot[3]);

    parent = groupdbInstanceTracker(parent, mode);
    ref = attach ? parent : groupdbPasteObject(parent, NULL, pivot, mode, false);
    trackerHandleFillNoAllocNoCheck(ref, &refHandle);

    for(i = 0; i < count; i++)
    {
        groupdbInstanceTracker(trackerFromHandle(&handles[i])->parent, mode);
        ref = trackerFromHandle(&refHandle);
        groupdbAttachObject(&handles[i], ref, mode);
    }

    free(handles);
}

static void groupdbPaste(Packet *pak)
{
    int            count;
    int            cut;
    UpdateMode    mode;
    int i;
    struct
    {
        TrackerHandle parent;
        TrackerHandle child;
        U32 file_id;
        U32 group_id;
        Mat4 mat;
    } *pasties;

    PERFINFO_AUTO_START("groupdbPaste", 1);

    mode        = pktGetBitsPack(pak,1);
    count        = pktGetBitsPack(pak,1);
    cut            = pktGetBitsPack(pak,1);

    if (!count)
        return;
    pasties = calloc(count, sizeof(pasties[0]));

    // do all of the checking now.  if there are multiple pastes, the groups' names could change.
    for(i = 0; i < count; i++)
    {
        pktGetTrackerHandleNoAllocPreChecked(pak, &pasties[i].parent);
        pktGetTrackerHandleNoAllocPreChecked(pak, &pasties[i].child);
        pasties[i].file_id = pktGetBitsPack(pak,1);
        pasties[i].group_id = pktGetBitsPack(pak,1);
        getMat4(pak, pasties[i].mat);
    }

    for(i=0;i<count;i++)
    {
        DefTracker *parent = groupdbInstanceTracker(trackerFromHandle(&pasties[i].parent), mode);
        DefTracker *child = trackerFromHandle(&pasties[i].child);
        GroupDef *object = group_info.files[pasties[i].file_id]->defs[pasties[i].group_id];
        if(cut && child)
            groupdbMoveObject(child, object->name, pasties[i].mat);
        else
            groupdbPasteObject(parent, object->name, pasties[i].mat, mode, true);
    }

    free(pasties);
    PERFINFO_AUTO_STOP();
}

static void groupdbDelete(Packet *pak)
{
    int i;
    UpdateMode mode = pktGetBitsPack(pak,1);
    int count = pktGetBitsPack(pak,1);
    TrackerHandle *handles;

    if(count <= 0)
        return;
    handles = calloc(count, sizeof(handles[0]));

    // do all of the checking now.  if there are multiple deletions, the groups' names could change.
    for(i = 0; i < count; i++)
        pktGetTrackerHandleNoAllocPreChecked(pak, &handles[i]);

    for(i = 0; i < count; i++)
    {
        groupdbInstanceTracker(trackerFromHandle(&handles[i])->parent, mode);
        groupdbQueueDeleteObject(trackerFromHandle(&handles[i]));
    }

    free(handles);
}

static void groupdbDetach(Packet *pak) {
    groupdbPaste(pak);
    groupdbDelete(pak);
}

static void groupdbNew()
{
//    journalReset();
    groupReset();
    strcpy(world_name,"noname.txt");
    world_id++;
    clearLayerCheckoutStatuses();
}

static void groupdbSaveSelection(Packet *pak,NetLink *link)
{
    char    selname[256],defname[256];
    int        count,i;
    DefTracker **trackers = NULL;

    strcpy(selname,pktGetString(pak));

    count = pktGetBitsPack(pak,1);
    if (!count)
        return;
    eaSetSize(&trackers, count);
    for(i=0;i<count;i++)
    {
        trackers[i] = pktGetTrackerHandleUnpacked(pak);
        printf("%s\n",trackers[i] ? groupDefGetFullName(trackers[i]->def,SAFESTR(defname)) : "empty");
    }
    sendSaveInfo(link,selname,groupSaveSelection(selname,trackers,count));
    //groupSave(selname);
    eaDestroy(&trackers);
}

static void groupdbTraySwap(Packet *pak)
{
    char    selname[256],defname[256];
    int        count,i;
    DefTracker **trackers = NULL;

    strcpy(selname,pktGetString(pak));

    count = pktGetBitsPack(pak,1);
    if (!count)
        return;
    eaSetSize(&trackers, count);
    for(i=0;i<count;i++)
    {
        trackers[i] = pktGetTrackerHandleUnpacked(pak);
        printf("%s\n",trackers[i] ? groupDefGetFullName(trackers[i]->def,SAFESTR(defname)) : "empty");
    }
    groupTraySwap(selname,trackers,count);
    eaDestroy(&trackers);
}

static DefTracker * receiveAndDoNotUpdateTracker(Packet * pak)
{
    DefTracker *ret;
    GroupDef def_data;
    char str_buf[2048];
    ret = pktGetTrackerHandleUnpacked(pak);
    pktGetBitsPack(pak,1);
    pktGetBitsPack(pak,1);
    getGroupDef(pak,&def_data,str_buf);
    return ret;
}

static LayerCheckoutStatus * getLayerCheckoutStatus( const char * name )
{
    int i = 0;
    LayerCheckoutStatus * les = 0;

    if ( layerCheckoutStatuses )
    {
        for (i=0; i<eaSize(&layerCheckoutStatuses); ++i)
        {
            les = layerCheckoutStatuses[i];
            if ( stricmp(les->name, name) == 0 )
                return les;
        }
    }
    else
    {
        // No earray, we must create it
        eaCreate(&layerCheckoutStatuses);
    }


    // If we got this far, we don't yet have a LayerCheckoutStatus, so add one
    les = calloc(1, sizeof(LayerCheckoutStatus));

    strcpy(les->name, name);

    eaPush(&layerCheckoutStatuses, les);

    return les;
}

static DefTracker * receiveAndUpdateTracker(Packet * pak, UpdateMode update_orig)
{
    int            file_idx,def_idx,dup=0,j/*,ref_count*/;
    PartInfo    part_info;
    GroupDef    *def,def_data = {0};
    char        str_buf[2048];
    TrackerHandle handle;
    DefTracker *ret, *ref;
    PropertyEnt *shareProp;
    bool bHadSharedProp;

    pktGetTrackerHandle(pak, &handle);
    for(j = 0; j < handle.depth; j++)    // GGFIXME: this turns off checking in case we're updating multiple trackers in a single command
        SAFE_FREE(handle.names[j]);        //            figure out a clean way to turn name checking back on for this command.
    file_idx = pktGetBitsPack(pak,1);
    def_idx    = pktGetBitsPack(pak,1);

    ref = groupRefPtr(handle.ref_id);
    ret = groupdbInstanceTracker(trackerFromHandle(&handle), update_orig);

    getGroupDef(pak,&def_data,str_buf);
    def = ret->def;//group_info.files[file_idx]->defs[def_idx];

    journalDef(def);

    part_info.def = def;
    ref = trackerRoot(ret);
    groupRefModify(ref);
    groupDefModify(def);

    def->has_sound        = def_data.has_sound;
    def->sound_radius    = def_data.sound_radius;
    def->sound_vol        = def_data.sound_vol;
    def->sound_ramp        = def_data.sound_ramp;
    def->sound_exclude    = def_data.sound_exclude;
    if (def_data.sound_name[0])
    {
        groupAllocString(&def->sound_name);
        strcpy(def->sound_name,def_data.sound_name);
    }
    if (def_data.sound_script_filename[0])
    {
        groupAllocString(&def->sound_script_filename);
        strcpy(def->sound_script_filename,def_data.sound_script_filename);
    }

    def->has_fog        = def_data.has_fog;
    def->fog_radius        = def_data.fog_radius;
    def->fog_near        = def_data.fog_near;
    def->fog_far        = def_data.fog_far;
    def->fog_speed        = def_data.fog_speed;
    def->has_volume        = def_data.has_volume;
    def->lod_fadenode    = def_data.lod_fadenode;
    def->no_fog_clip    = def_data.no_fog_clip;
    def->no_coll        = def_data.no_coll;
    def->has_tint_color    = def_data.has_tint_color;
    memcpy(def->color,def_data.color,sizeof(def->color));
    copyVec3(def_data.box_scale,def->box_scale);

    def->has_ambient    = def_data.has_ambient;
    def->has_light        = def_data.has_light;
    def->has_cubemap    = def_data.has_cubemap;
    def->cubemapGenSize    = def_data.cubemapGenSize;
    def->cubemapCaptureSize = def_data.cubemapCaptureSize;
    def->cubemapBlurAngle = def_data.cubemapBlurAngle;
    def->cubemapCaptureTime = def_data.cubemapCaptureTime;

    def->light_radius    = def_data.light_radius;
    if (def_data.type_name[0])
    {
        groupAllocString(&def->type_name);
        strcpy(def->type_name, def_data.type_name);
    }
    def->saved = 0;

    def->has_beacon        = def_data.has_beacon;
    def->beacon_radius    = def_data.beacon_radius;

    if (def_data.beacon_name[0])
    {
        groupAllocString(&def->beacon_name);
        strcpy(def->beacon_name, def_data.beacon_name);
    }

    def->has_tex        = def_data.has_tex;
    for(j=0;j<2;j++)
    {
        if (def_data.tex_names[j][0])
        {
            groupAllocString(&def->tex_names[j]);
            strcpy(def->tex_names[j],def_data.tex_names[j]);
        }
    }

    //new texture stuff
    eaDestroyEx(&def->def_tex_swaps, deleteDefTexSwap);
    def->def_tex_swaps = def_data.def_tex_swaps;

    // special case: watch for changes to def's with "SharedFrom" property set.  Since these defs are not
    //    editable, the only time this should occur is when the SharedFrom property is removed.  In this case 
    //    we need to mark the parent as dirty so that it gets checked out and saved.
    bHadSharedProp = def->properties != NULL && stashFindPointer(def->properties, "SharedFrom", &shareProp);

    groupDefDeleteProperties(def);
    def->has_properties = def_data.has_properties;
    def->properties = def_data.properties;

    groupRefGridDel(ref);
    groupRefActivate(ref);

    if(bHadSharedProp && def->properties != NULL && !stashFindPointer(def->properties, "SharedFrom", &shareProp)) {
        if(ref->parent)
            groupRefModify(ref->parent);
    }

    // make a handle to ret and then reopen it
    return trackerFromHandle(&handle);
}

// Updates multiple trackers at once.  This function expects that the trackers will
// be updated in the same fashion, so if groups need to be renamed they will all be
// renamed to the same name if possible.  (Example, if there are three instances of
// grp1, and this function is called on two of them, those two will be changed to 
// grp2, rather than one grp2 and one grp3)
// we also expect the trackers to come in sorted alphabetically by tracker->def->name
static void groupdbUpdateTrackers(Packet *pak)
{
    int edit_orig = pktGetBitsPack(pak,1);
    int num_trackers = pktGetBitsPack(pak,1);
    int i;
    char ** names=0;
    int * occ=0;     //number of times each names shows up (if more than 1)
    int index=0;
    const char ** order=0; // list of names in the order they are received

    // we are going to read in num_trackers names, and fill up the array
    // occ with a count of each time a name appears (corresponding to the
    // entries in names)
    const char * name=strdup(pktGetString(pak));
    eaPushConst(&order,name);
    for (i=1;i<num_trackers;i++)
    {
        char * tempname=strdup(pktGetString(pak));
        if (strcmp(tempname,name)==0)
        {
            if (eaSize(&names)>0 && strcmp(names[eaSize(&names)-1],name)==0)
            {
                (int)occ[eaSize(&names)-1]++;
            }
            else
            {
                eaPushConst(&names,name);        // push on a 2 because we didn't hit this
                eaiPush(&occ, 2);    // the first time we saw this value in name
            }
        }
        name=tempname;
        eaPushConst(&order,name);
    }

    // all we really care about is whether or not we're modifying all instances
    // of a given group, so now just set the elements in occ to 1 if we have all
    // instances, 0 otherwise
    for (i=0;i<eaSize(&names);i++)
    {
        occ[i]=(groupDefRefCount(groupDefFind(names[i])))==(int)occ[i];
    }


    for (i=0;i<num_trackers;i++)
    {
        int override=0;
        int noupdate=0;
        if (eaSize(&names)>0)
        {
            if (index<eaSize(&names)-1 && strcmp(names[index+1],order[i])==0)
                index++;
            if (strcmp(names[index],order[i])==0)
            {
                if (occ[index]==-1)
                    noupdate=1;
                else
                {
                    override=occ[index];
                    occ[index]=-1;
                }
            }
        }
        // if we've already updated a tracker with this name, just read in the packet
        // but don't do the normal updating, since it's already been done
        if (noupdate)
        {
            DefTracker * tracker=receiveAndDoNotUpdateTracker(pak);
            int depth=0;
            DefTracker * ref;
            int idxs[100];
            int j;
            ref=tracker;

            //we need to know the ultimate parent, the depth, and how
            //to get to this object to properly update it in the hierarchy
            while (ref->parent) {
                idxs[depth]=ref-ref->parent->entries;
                ref=ref->parent;
                depth++;
            }
            if (depth > 0) {
                PartInfo part_info;
                tracker->def=groupDefFind(name);
                part_info.def=tracker->def;
                for (j=0;j<depth/2;j++) {
                    int temp=idxs[j];
                    idxs[j]=idxs[depth-j-1];
                    idxs[depth-j-1]=temp;
                }
            }
            groupRefModify(ref);
            tracker->def=groupDefFind(name);
            groupRefGridDel(ref);
            groupRefActivate(ref);
        }
        else
        {
            DefTracker *tracker = receiveAndUpdateTracker(pak, override || edit_orig ? UM_ORIGINAL : UM_NORMAL);
            name=tracker->def->name;
        }
    }

    eaDestroyExConst(&order,NULL);
    eaDestroyConst(&names);
    eaiDestroy(&occ);
    groupRecalcAllBounds();
}

static void groupdbUpdateTracker(Packet *pak)
{
    UpdateMode mode = pktGetBitsPack(pak,1);
    receiveAndUpdateTracker(pak, mode);
}

static void groupdbUpdateDef(Packet *pak)
{
    int            file_idx,def_idx,update_bounds = 0;
    GroupDef    def_data,*def;
    char        str_buf[2048];

    file_idx = pktGetBitsPack(pak,1);
    def_idx    = pktGetBitsPack(pak,1);
    getGroupDef(pak,&def_data,str_buf);
    def = group_info.files[file_idx]->defs[def_idx];
    journalDef(def);
    groupDefModify(def);
    if (!def_data.lod_autogen)
    {
        def->lod_autogen = def_data.lod_autogen;
        if (def->lod_farfade != def_data.lod_farfade || def->lod_far != def_data.lod_far)
            update_bounds = 1;
    }
    def->lod_scale = def_data.lod_scale;
    def->lod_near = def_data.lod_near;
    def->lod_far = def_data.lod_far;
    def->lod_nearfade = def_data.lod_nearfade;
    def->lod_farfade = def_data.lod_farfade;
    def->has_ambient = def_data.has_ambient;
    def->has_light = def_data.has_light;
    def->has_cubemap = def_data.has_cubemap;
    def->cubemapGenSize    = def_data.cubemapGenSize;
    def->cubemapCaptureSize = def_data.cubemapCaptureSize;
    def->cubemapBlurAngle = def_data.cubemapBlurAngle;
    def->cubemapCaptureTime = def_data.cubemapCaptureTime;
    def->has_volume = def_data.has_volume;

    if (def_data.type_name[0])
    {
        groupAllocString(&def->type_name);
        strcpy(def->type_name, def_data.type_name);
    }
    memcpy(def->color,def_data.color,sizeof(def->color));
    if (def->in_use && !def_data.in_use)
    {
        //delLibSubs(def,def_data.name);
        groupDefFree(def,1);
        groupPruneDefs(1);
    }
    else
    {
        {
            int j;
            //Only if you are a ref that refers to this def
            //In fact, I think this whole loop is unnecessary
            for(j=0;j<group_info.ref_count;j++)
            {
                DefTracker * ref = group_info.refs[j];
                if( ref->def && 0 == stricmp( ref->def->name,def->name ) ) 
                    groupRefModify( ref );
            }
            groupDefName(def,def_data.file->fullname,def_data.name);
            objectLibraryAddName(def);
            checkoutDefFile(def);
        }
        groupSetVisBounds(def);
        moveDefsToLib(def);
    }
    if (update_bounds)
        groupRecalcAllBounds();
    world_modified = 1;
}

static void groupdbDefLoad(Packet *pak)
{
    char    name[1000];

    strcpy(name,pktGetString(pak));
    if (!groupFileLoadFromName(name))
    {
        Errorf("Can't find group %s\n",name);
        log_printf("getvrml.log","groupdbDefLoad(\"%s\") FAILED",name);
    }
    else
    {
        log_printf("getvrml.log","groupdbDefLoad(\"%s\")",name);
    }
    world_modified = 1;
}

static int isEditCommand(const char *s)
{
    static char editCommands[] =
        " undo "
        " ungroup "
        " group "
        " attach "
        " paste "
        " delete "
        //        " new " // not an editting command
        " savelibs "
        //        " save "
        " savesel "
        " trayswap "
        //        " load "
        " updatetracker "
        " updatedef "
        " defload "
        " scenefile "
        " ungroupall "
        " groupall ";
    char    paddedname[128];

    // assert(strlen(s)<125);
    // Pad the identifier with spaces (handles case of an identifier being part of another identifier
    sprintf(paddedname, " %s ", s);

    return strstr(editCommands, paddedname)!=0;
}

void consoleSave(NetLink * link,int numBytes) {
    sendSaveInfo(link,world_name,numBytes);
    if (groupPruneDefs(0))
        world_modified = 1;
}

void worldHandleMsg(NetLink *link,Packet *pak)
{
    char    *s;
    extern Grid obj_grid;
    extern int consoleSaveCommand;
    char oldSceneFile[256];
    char oldLoadScreen[256];
    U32 modTimeBeforeUpdate;
    int journaled = 1;
    int doesntRequireCheckOut = 0;
    int import = 0;
    int rootMapHasBeenModified = 0;

    memset(editOrigCheckouts,0,50*sizeof(int));

    PERFINFO_AUTO_START("worldHandleMsg", 1);

    strcpy( oldSceneFile, group_info.scenefile ); 
    strcpy( oldLoadScreen, group_info.loadscreen ); 
    modTimeBeforeUpdate = group_info.ref_mod_time;
    map_full_load = 0;
    editOrigNoCheckout = 0;
    sendEditMessage(link, -1, NULL); // Save the link so that the deeply embed call to sendEditMessage withing groupdbmodify does not fail

    gridCacheInvalidate();

    s = pktGetString(pak);

    //Sanity check that nobody is editing the maps on the live servers
    if (isProductionMode()) {
        dbLog("Editor:worldCmd", ((ClientLink*)link->userData)->entity, "%s", s);
    }

    #if NOVODEX
        // Since these are editor commands, simply turn off the physics on the server, to keep things simple for now
        nwDeinitializeNovodex();
    #endif

    //Hm, why is this even being checked below? It is always set...
    world_modified = 1; 
    if(strcmp(s,"load")==0 || strcmp(s,"new")==0 || strcmp(s,"redo")==0 || strcmp(s,"undo")==0)
        journaled = 0;
    else
        groupdbEditTransactionBegin(s);
    no_update=0;
    if (!strstri(s,"save"))
        baseReset(&g_base);
    if (strcmp(s,"undo")==0)
    {
        journalUndo();
        groupTrackersHaveBeenReset();
    }
    if (strcmp(s,"redo")==0)
    {
        journalRedo();
        groupTrackersHaveBeenReset();
    }
//    journalNext();
    if (strcmp(s,"ungroup") == 0)
        groupdbUngroup(pak);
    else if (strcmp(s,"group")==0 || strcmp(s,"attach")==0)
        groupdbGroupOrAttach(pak,s);
    else if (strcmp(s,"detach")==0)
        groupdbDetach(pak);
    else if (strcmp(s,"paste")==0)
        groupdbPaste(pak);
    else if (strcmp(s,"delete")==0)
        groupdbDelete(pak);
    else if (strcmp(s,"new")==0)
        groupdbNew();
    else if (strcmp(s,"savelibs")==0)
    {
        if (groupPruneDefs(1))
            world_modified = 1;
        sendSaveInfo(link,"libs",groupSaveLibs());
    }
    else if (strcmp(s,"save")==0)
    {
        char newname[128];
        strcpy(newname, pktGetString(pak));

        //Save As : If we are doing a save as, treat as if making a new map (checked out initially, and read/writeable)
        if (!strcmp(world_name, newname)==0) 
        {
            strcpy(world_name,newname);
            //TO DO, save as doesn't try to do checkouts.  It never has, but something to think about...
            //should I have this? Now new names will be added to the layerCheckOutStatus, it's OK to leave the old ones.
            clearLayerCheckoutStatuses();
        } 

        if (db_state.map_supergroupid || db_state.map_userid)
        {
            baseSaveMapToDb();
            sendSaveInfo(link,world_name,1);
        }
        else
        {
            sendSaveInfo(link,world_name,groupSave(world_name));
        }
        if (groupPruneDefs(0))
            world_modified = 1;

// This is dangerous. It calls (down the rabbit hole) EncounterUnload(),
// which frees all EncounterGroups, without fixing up anything pointing to
// those groups.
// ditto below
//         MissionDebugSaveMapStatsToFile(((ClientLink*)link->userData)->entity);

    }
    else if (strcmp(s,"autosave")==0)
    {
        char newname[128];
        char oldname[128];


        strcpy(newname, pktGetString(pak));

        //    printf("received autosave packet,%s,%s\n",newname,world_name);

        strcpy(oldname,world_name);
        strcpy(world_name,newname);            

        sendSaveInfo(link,newname,groupSave(world_name));
        if (groupPruneDefs(0))
            world_modified = 1;
        strcpy(world_name,oldname);

        // Not sure if this should be here or not...
        //if (OnMissionMap())
        //{
//             MissionDebugSaveMapStatsToFile(((ClientLink*)link->userData)->entity);
        //}
    }
    else if (strcmp(s,"savesel")==0)
        groupdbSaveSelection(pak,link);
    else if (strcmp(s,"trayswap")==0)
        groupdbTraySwap(pak);
    else if (strcmp(s,"load")==0)
    {
        char    *mapname;

        strdup_alloca(mapname, pktGetString(pak));
        import = pktGetBits(pak,1);

        groupLoadMap(mapname,import,1);
        if (!import)
            doesntRequireCheckOut = 1; //because world_modified is so hinky
        else
            rootMapHasBeenModified = 1; // need to check out root map 

        //if this sets the world_name to an autosaved file, get rid of the .autosave part of it
        if (strstr(world_name,".autosave.txt")!=NULL)
        {
            strcpy(strstr(world_name,".autosave.txt"),".txt");
        }
        printf("%s world_name \"%s\"\n", (import ? "IMPORTING" : "LOADING"), world_name);
    }
    else if (strcmp(s,"updatetracker")==0)
        groupdbUpdateTracker(pak);
    else if (strcmp(s,"updatetrackers")==0)
        groupdbUpdateTrackers(pak);
    else if (strcmp(s,"updatedef")==0)
        groupdbUpdateDef(pak);
    else if (strcmp(s,"defload")==0)
        groupdbDefLoad(pak);
    else if (strcmp(s,"scenefile")==0)
        strcpy(group_info.scenefile,pktGetString(pak));
    else if (strcmp(s,"loadscreen")==0)
        strcpy(group_info.loadscreen,pktGetString(pak));
    else if (strcmp(s,"ungroupall")==0)
        ungroupAll();
    else if (strcmp(s,"groupall")==0)
        groupAll();
    if(journaled)
        groupdbEditTransactionEnd();

    if (no_update)
        world_modified=0;

    /////////////////// Handle Checkouts ///////////////////////////////////////////////////////////////////
    //Figure out what got modified (in theory, the change should propagate up to the refs
    //Loop through the refs, looking for modified refs (I don't think I need to check defs)
    // check map out if possible
    //Don't bother with any of this if the world doesn't have a name
    //"load" is weird because world_modified is done for everything, which is just wrong, which is why I've got doesntRequireCheckOut
    if( world_modified && world_name[0] && doesntRequireCheckOut == 0 )//load tags all as new, but don't check that stuf out yet && 0 != strcmp(s,"save") )
    {
        int i;
        DefTracker * ref;
        int ignoreLayerWriting;

        //ignore layers if the person loaded this layer directly as the main map
        //TO DO : make sure theres' only one layer and it has the same name as the map 
        if( strstri( world_name, "_layer_" ) )  
            ignoreLayerWriting = 1;
        else
            ignoreLayerWriting = 0;

        //find layer that have been modified
        for(i=0;i<group_info.ref_count;i++)
        {
            ref = group_info.refs[i];

            //TO DO : Are two checks redundant? //TO DO ref->def->saved = 0;
            if( ref->mod_time > modTimeBeforeUpdate || (ref->def && ref->def->mod_time > modTimeBeforeUpdate) )
            {
                PropertyEnt* layerProp = 0;
                int j;

                if( ref->def )
                    stashFindPointer(  ref->def->properties, "Layer", &layerProp );

                if (editOrigNoCheckout) {
                    j=0;
                    while (editOrigCheckouts[j]!=0 && editOrigCheckouts[j]!=ref) j++;
                    if (editOrigCheckouts[j]!=ref) continue;
                }

                if( layerProp && !ignoreLayerWriting)
                {
                    char givenName[256]; 
                    char layerFileName[MAX_PATH]; 
                    char *    layerName = 0;
                    char * suffix;
                    int    checkoutSuccess = 0;

                    //layerName = layerProp->value_str;

                    //MW changed layerNmae from the value of the property to the name of the def.
                    {
                        extractGivenName( givenName,ref->def->name );
                        if( !givenName[0] )
                        {
                            Errorf( "The Group %s is tagged as a layer, but you didn't name it. Do that, and save it again", ref->def->name );
                            layerName = "RenameMe";
                        }
                        else
                        {
                            layerName = givenName;
                        }
                    }

                    if( !groupRefShouldAutoGroup(ref) || server_state.doNotAutoGroup)
                    {
                        PropertyEnt *shareProp;
                        if(!stashFindPointer(ref->def->properties, "SharedFrom", &shareProp))
                        {
                            /////Get the file name the layer will be saved as
                            //for Layer file names ".../City_01_01.txt" becomes ".../City_01_01_layer_layerName.txt"
                            strcpy( layerFileName, world_name);
                            suffix = strstri( layerFileName, ".txt" );
                            sprintf( suffix, "_layer_%s.txt", layerName );
                        }
                        else
                        {
                            strcpy(layerFileName, world_name);
                            forwardSlashes(layerFileName);
                            if(!(suffix = strstri(layerFileName, "maps/")))
                                suffix = layerFileName;
                            strcpy(suffix, shareProp->value_str);
                        }
                    }
                    else
                    {
                        sendEditMessage(NULL, 1, "Error: Layer %s is AutoGrouped \nIt can't be modified unless you are running in artist mode (with -ImAnArtist in your mapserver command line)\n Your changes to this layer won't be saved!", layerName  );
                    }
                }
                else
                    rootMapHasBeenModified = 1; //This is not a layer
            }
        }
    }
    ////////////////End Handle Checkouts ///////////////////////////////////////////////////////////////////

    if (world_modified && !no_update)
    {
        dbGatherMapLinkInfo();
        groupTrackersHaveBeenReset();
        groupRecalcAllBounds();
        groupDynBuildBreakableList();
        groupDynForceRescan();
    }
    PERFINFO_AUTO_STOP();
}

void groupdbRunBatch(char *batch_str)
{
    char    *args[100],*s;
    int        count;

    s = strdup(batch_str);
    while(s)
    {
        count=tokenize_line(s,args,&s);
        if (!count || args[0][0] == '#')
            continue;
        if (stricmp(args[0],"load")==0)
            groupLoadMap(args[1],0,0);
        if (stricmp(args[0],"save")==0)
        {
            groupSave(args[1]);
        }
        if (stricmp(args[0],"import")==0)
        {
            groupLoadMap(args[1],1,0);
        }
        if (stricmp(args[0],"groupall")==0)
        {
            //groupAll();
        }
        if (stricmp(args[0],"quit")==0)
            exit(0);
    }
}

