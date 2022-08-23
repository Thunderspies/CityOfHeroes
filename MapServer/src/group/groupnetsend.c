#include "group/group.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/utils/timing.h>
#include "comm_game.h"
#include "groupjournal.h"
#include <utilitieslib/network/netcomp.h>
#include "group/groupProperties.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdserver.h"
#include "seq/anim.h"
#include "bases/basesend.h"
#include "storyarc/mission.h"
#include "beacon/beaconFile.h"
#include "bases/bases.h"
#include "gameSys/sgraid_V2.h"
#include "dbcomm/staticMapInfo.h"

int        world_id;
char    world_name[MAX_PATH];
static GroupDef **pendingDynamicDefChanges;

static void sendAllBounds(Packet *pak)
{
    int        k,i;

    pktSendBitsPack(pak,1,group_info.file_count);
    for(k=0;k<group_info.file_count;k++)
    {
        GroupFile    *file = group_info.files[k];

        pktSendString(pak,file->fullname);
        pktSendBitsPack(pak,1,file->count);
        for(i=0;i<file->count;i++)
        {
            pktSendString(pak,file->defs[i]->name);
            pktSendF32(pak,file->defs[i]->vis_dist);
        }
    }
}

static void sendDef(Packet *pak,GroupDef *def)
{
    int            j;
    GroupEnt    *ent;

    pktSendBitsPack(pak,1,def->id+1);
    pktSendBits(pak,1,def->in_use);
    if (!def->in_use)
        return;

    pktSendString(pak,def->name);

    pktSendBits(pak,32,def->mod_time);

    pktSendBitsPack(pak,1,def->count);
    for(j=0;j<def->count;j++)
    {
        ent = &def->entries[j];
        pktSendString(pak,ent->def->name);
        sendMat4(pak,ent->mat);
    }


    // LOD stuff
    if (def->lod_scale || def->lod_near || def->lod_far || def->lod_nearfade || def->lod_farfade
            || def->lod_fadenode || def->no_fog_clip || def->no_coll || def->child_parent_fade || def->parent_fade)
    {
        pktSendBits(pak,1,1);
        pktSendIfSetF32(pak,def->lod_scale);
        pktSendIfSetF32(pak,def->lod_near);
        pktSendIfSetF32(pak,def->lod_far);
        pktSendIfSetF32(pak,def->lod_nearfade);
        pktSendIfSetF32(pak,def->lod_farfade);
        pktSendBits(pak,2,def->lod_fadenode);
        pktSendBits(pak,1,def->parent_fade);
        pktSendBits(pak,1,def->child_parent_fade);
        pktSendBits(pak,1,def->no_fog_clip);
        pktSendBits(pak,1,def->no_coll);
    }
    else
        pktSendBits(pak,1,0);

    // Child / open Tracker stuff
    // Maybe just have open_tracker?
    if (def->child_light || def->child_ambient || def->child_volume || def->child_beacon || def->open_tracker || def->child_no_fog_clip)
    {
        pktSendBits(pak,1,1);
        pktSendBits(pak,1,def->open_tracker);
        pktSendBits(pak,1,def->child_light);
        pktSendBits(pak,1,def->child_ambient);
        pktSendBits(pak,1,def->child_volume);
        pktSendBits(pak,1,def->child_beacon);
        pktSendBits(pak,1,def->child_no_fog_clip);
        //Note: def->child_properties is not send down to the client currently (is it needed?)
    }
    else
        pktSendBits(pak,1,0);

    pktSendIfSetF32(pak,def->sound_radius);
    pktSendIfSetF32(pak,def->shadow_dist);

    // Various tricks
    if (def->has_light || def->has_cubemap || def->has_ambient || def->lib_ungroupable
        || def->has_sound || def->has_volume || def->has_fog || def->has_beacon 
        || def->has_tint_color || def->has_tex || (def->type_name && def->type_name[0])
        || def->has_properties || def->has_alpha)
    {
        pktSendBits(pak,1,1);

        pktSendString(pak,def->type_name);

        pktSendBits(pak,1,def->lib_ungroupable);

        pktSendBits(pak,1,def->has_light);
        pktSendBits(pak,1,def->has_ambient);
        if (def->has_ambient || def->has_light)
        {
            if (def->has_light)
                pktSendIfSetF32(pak,def->light_radius);
            for(j=0;j<8;j++)
                pktSendBits(pak,8,def->color[0][j]);
        }

        pktSendBits(pak,1,def->has_cubemap);
        if (def->has_cubemap)
        {
            pktSendBits(pak,16,def->cubemapGenSize);
            pktSendBits(pak,16,def->cubemapCaptureSize);
            pktSendF32(pak,def->cubemapBlurAngle);
            pktSendF32(pak,def->cubemapCaptureTime);
        }

        pktSendBits(pak,1,def->has_sound);
        if (def->has_sound)
        {
            pktSendString(pak,def->sound_name);
            pktSendString(pak,def->sound_script_filename);
            pktSendBits(pak,8,def->sound_vol);
            pktSendF32(pak,def->sound_ramp);
            pktSendBits(pak,1,def->sound_exclude);
        }

        pktSendBits(pak,1,def->has_fog);
        if (def->has_fog)
        {
            pktSendF32(pak,def->fog_near);
            pktSendF32(pak,def->fog_far);
            pktSendF32(pak,def->fog_radius);
            pktSendF32(pak,def->fog_speed);
            for(j=0;j<8;j++)
                pktSendBits(pak,8,def->color[0][j]);
        }

        pktSendBits(pak,1,def->has_volume);
        if (def->has_volume)
        {
            pktSendF32(pak,def->box_scale[0]);
            pktSendF32(pak,def->box_scale[1]);
            pktSendF32(pak,def->box_scale[2]);
        }

        pktSendBits(pak, 1, def->has_beacon);
        if (def->has_beacon)
        {
            pktSendString(pak, def->beacon_name);
            pktSendF32(pak, def->beacon_radius);
        }
        pktSendBits(pak,1,def->has_tint_color);
        if (def->has_tint_color)
        {
            for(j=0;j<3;j++)
                pktSendBits(pak,8,def->color[0][j]);
            for(j=0;j<3;j++)
                pktSendBits(pak,8,def->color[1][j]);
        }

        pktSendBitsPack(pak,1,def->has_tex);
        if (def->has_tex & 1)
            pktSendString(pak,def->tex_names[0]);
        if (def->has_tex & 2)
            pktSendString(pak,def->tex_names[1]);
        
        pktSendBits(pak,1,def->has_alpha);
        if (def->has_alpha)
            pktSendF32(pak, def->groupdef_alpha);

        pktSendBitsPack(pak,1,def->has_properties);
        if(def->has_properties)
        {
            WriteGroupPropertiesToPacket(pak, def->properties);
        }
    }
    else
        pktSendBits(pak,1,0);

    pktSendBitsPack(pak,1,eaSize(&def->def_tex_swaps));
    {
        int k;
        for (k=0;k<eaSize(&def->def_tex_swaps);k++) {
            pktSendString(pak,def->def_tex_swaps[k]->tex_name);
            pktSendString(pak,def->def_tex_swaps[k]->replace_name);
            pktSendBits(pak,1,def->def_tex_swaps[k]->composite);
        }
    }

    pktSendBits(pak,1,def->model ? 1 : 0);
    if (def->model)
    {
        pktSendString(pak,def->model->name);
    }
    pktSendBits(pak,1,def->hideForGameplay);
}

static void sendGroupFile(GroupFile *file,NetLink *link,int full_update,Packet *pak,U32 *mod_time_p)
{
    extern Base g_base;
    int            i;
    GroupDef    *def;

static int use_compressed_base = 1;

    if ((!full_update && file->mod_time <= link->last_update_time) && !file->force_reload)
    {
        pktSendBits(pak,1,0);
        return;
    }
    pktSendBits(pak, 1, 1);
    pktSendString(pak, file->fullname);
    pktSendBits(pak, 32, file->bounds_checksum);
    if (file->base && file->base == &g_base)
    {
        pktSendBits(pak, 1, 1);
        pktSendBits(pak, 1, 0);
        baseSendAll(pak, file->base, file->mod_time, full_update);
        return;
    }
    else if (full_update && use_compressed_base && g_isBaseRaid)
    {
        int numbase = eaSize(&g_raidbases);
        pktSendBits(pak, 1, 1);
        pktSendBits(pak, 1 ,1);

        pktSendBits(pak, 32, g_baseRaidType);
        pktSendBits(pak, 32, numbase);
        for (i = 0; i < numbase; i++)
        {
            if (i < numbase - 1)
            {
                pktSendBits(pak, 32, sgRaidGetBaseRaidSGID(i));
                pktSendBits(pak, 32, sgRaidGetBaseRaidRaidID(i));
            }
            pktSendF32(pak, g_baseraidOffsets[i][0]);
            pktSendF32(pak, g_baseraidOffsets[i][1]);
            pktSendF32(pak, g_baseraidOffsets[i][2]);
            baseSendAll(pak, g_raidbases[i], g_raidbases[i]->mod_time, full_update);
        }

        for(i = 0; i < file->count; i++)
        {
            def = file->defs[i];
            def->mod_time = link->last_update_time;
        }
        if (link->last_update_time > *mod_time_p)
        {
            *mod_time_p = link->last_update_time;
        }
        file->force_reload = 0;
        return;
    }
    pktSendBits(pak,1,0);
    for(i=0;i<file->count;i++)
    {
        def = file->defs[i];
        if (!file->force_reload) {
            if (!full_update && def->mod_time <= link->last_update_time)
                continue;
            if (!file->modified_on_disk && def->file_idx && def->saved)
                continue;
        }
        if (def->mod_time > *mod_time_p)
            *mod_time_p = def->mod_time;
        sendDef(pak,def);
    }
    file->force_reload=0;
    pktSendBitsPack(pak,1,0);
}

static void sendLoadScreenPages(Packet *pak)
{
    const MapSpec *mapSpec = MapSpecFromMapId(db_state.base_map_id);
    const MissionDef *missiondef;
    int index, count;

    if (g_activemission
        && (missiondef = TaskMissionDefinition(&g_activemission->sahandle))
        && (count = eaSize(&missiondef->loadScreenPages)))
    {
        pktSendBits(pak, 1, 1);
        pktSendBitsAuto(pak, count);
        for (index = 0; index < count; index++)
        {
            pktSendString(pak, missiondef->loadScreenPages[index]);
        }
    }
    else if (mapSpec)
    {
        count = eaSize(&mapSpec->loadScreenPages);

        pktSendBits(pak, 1, 1);
        pktSendBitsAuto(pak, count);
        for (index = 0; index < count; index++)
        {
            pktSendString(pak, mapSpec->loadScreenPages[index]);
        }
    }
    else
    {
        // not sending load screen pages
        pktSendBits(pak, 1, 0);
    }
}

void worldSendUpdate(NetLink *link,int full_update)
{
    int            i,k;
    U32            mod_time=0;
    DefTracker    *ref;
    Packet        *pak;
    
//assert(heapValidateAll());
#if 0
    int timer = timerAlloc();
    timerStart(timer);
#endif
    pak = pktCreate();
    if (full_update)
        pktSetCompression(pak,1);
    pktSendBitsPack(pak,1,SERVER_GROUPS);

    pktSendBitsPack(pak,1,journalUndosRemaining());
    pktSendBitsPack(pak,1,journalRedosRemaining());

    if (link->last_world_id != world_id)
    {
        const MapSpec *spec;
        StaticMapInfo *info;

        link->last_world_id = world_id;
        pktSendBits(pak,1,1);
        pktSendBits(pak,2, db_state.is_endgame_raid ? 2 : (!db_state.is_static_map && !(db_state.local_server && !g_activemission)) );
        pktSendBitsPack(pak,1,db_state.instance_id);
        pktSendBitsPack( pak,1, server_state.doNotAutoGroup );//MW added
        pktSendBitsPack(pak, 1, db_state.base_map_id);

        STATIC_INFUNC_ASSERT(MAP_TEAM_COUNT <= 8);

        spec = MapSpecFromMapId(db_state.base_map_id);
        info = staticMapInfoFind(db_state.base_map_id);

        if (db_state.is_static_map && spec && info)
        {
            pktSendBitsAuto(pak, info->introZone);
            pktSendBitsPack(pak, 3, spec->teamArea);
        }
        else
        {
 			if (g_MapIsTutorial)
			{
				// Mark mission maps within tutorials as being IntroZone 4
				pktSendBitsAuto(pak, 4);
            }
            else
            {
                pktSendBitsAuto(pak, 0);
            }
            pktSendBitsPack(pak, 3, MAP_TEAM_EVERYBODY);
        }

        sendLoadScreenPages(pak);

        if(!group_info.loadscreen || !group_info.loadscreen[0])
            pktSendString(pak,0);
        else
            pktSendString(pak,group_info.loadscreen);

        if (!group_info.files)
            pktSendString(pak,0);
        else
            pktSendString(pak,group_info.files[0] ? group_info.files[0]->fullname : 0);
    }
    else
        pktSendBits(pak,1,0);

    pktSendBits(pak, 1, (beaconGetReadFileVersion()||server_state.levelEditor)? 1: 0); // Either beacons are loaded, or we explicitly told them not to load

    if (g_isBaseRaid)
    {
        pktSendBitsPack(pak,1,1);
        sendGroupFile(group_info.files[0],link,full_update,pak,&mod_time);
    }
    else
    {
        pktSendBitsPack(pak,1,group_info.file_count);
        for(k=1;k<group_info.file_count;k++)
            sendGroupFile(group_info.files[k],link,full_update,pak,&mod_time);
        if (group_info.file_count)
            sendGroupFile(group_info.files[0],link,full_update,pak,&mod_time);
    }

    for(i=0;i<group_info.ref_count;i++)
    {
        ref = group_info.refs[i];
        if (!full_update && ref->mod_time <= link->last_update_time)
            continue;
        if (ref->mod_time > mod_time)
            mod_time = ref->mod_time;
        pktSendBitsPack(pak,1,i);
        if (ref->def)
        {
            pktSendString(pak,ref->def->name);
            sendMat4(pak,ref->mat);
        }
        else
        {
            pktSendString(pak,0);
        }
    }
    if (mod_time)
    {
        link->last_update_time = mod_time;
    }
    pktSendBitsPack(pak,1,-1);

    pktSendBitsPack(pak, 1, group_info.file_count);
    pktSendBitsPack(pak, 1, groupInfoRefCount());
    pktSendBits(pak, 32, groupInfoCrcDefs());
    pktSendBits(pak, 32, groupInfoCrcRefs());

    
    errorSetVerboseLevel(10);
    verbose_printf("world update: %d bytes\n",pktGetSize(pak));
    errorSetVerboseLevel(0);

#if 0
    {
    U8        *dst;
    int        dstlen;

        printf("%d tricks %d soundshadow %d child %d total\n",trick_count,soundshadow_count,child_count,def_count);
        dstlen = pktGetSize(pak) * 2 + 16;
        dst = malloc(dstlen);
        printf("timer elapsed: %f\n",timerElapsed(timer));
        compress2(dst,&dstlen,pak->stream.data,pktGetSize(pak)+1,5);
        printf("timer elapsed: %f\n",timerElapsed(timer));
        timerFree(timer);
        printf("compressed: %d\n",dstlen);
    }
#endif

    //sendAllBounds(pak);
    pak->reliable=1;
    pktSend(&pak,link);
//assert(heapValidateAll());
}

// cleaned up world_name, can be used with ErrorFilenamef
char* currentMapFile(void)
{
    char* pos;
    static char buffer[MAX_PATH];
    strcpy(buffer, world_name);
    forwardSlashes(buffer);

    // make relative
    pos = strstr(buffer, "data/");
    if (pos)
    {
        pos += 5;
        memmove(buffer, pos, strlen(pos)+1);
    }
    return buffer;
}

void markDynamicDefChange(GroupDef *def) {
    if (eaFind(&pendingDynamicDefChanges, def) == -1)
        eaPush(&pendingDynamicDefChanges, def);
}

void sendDynamicDefChanges(NetLink *link) {
    int i;
    int numDefs = eaSize(&pendingDynamicDefChanges);

    if (numDefs > 0) {
        Packet *pak = pktCreateEx(link, SERVER_DYNAMIC_DEFS);
        pktSendBitsPack(pak, 1, numDefs);

        for (i = 0; i < numDefs; ++i) {
            pktSendString(pak, pendingDynamicDefChanges[i]->name);
            pktSendBitsPack(pak, 1, pendingDynamicDefChanges[i]->hideForGameplay);
        }

        pktSend(&pak, link);
    }
}

void clearDynamicDefChanges() {
    eaDestroy(&pendingDynamicDefChanges);
}

/*
#if 0
        for(i=0;i<file->count;i++)
        {
            if (file->defs[i])
            {
                pktSendString(pak,file->defs[i]->name);
                pktSendF32(pak,file->defs[i]->vis_dist);
                pktSendF32(pak,file->defs[i]->radius);
                pktSendF32(pak,file->defs[i]->lod_scale);
                pktSendF32(pak,file->defs[i]->shadow_dist);
                pktSendVec3(pak,file->defs[i]->min);
                pktSendVec3(pak,file->defs[i]->max);
                pktSendVec3(pak,file->defs[i]->mid);
            }
        }
        pktSendString(pak,0);
    }

    for(k=0;k<group_info.file_count;k++)
    {
        GroupFile    *file = group_info.files[k];

        for(i=0;i<file->count;i++)
        {
            def = group_info.files[k]->defs[i];
            if (!full_update && def->mod_time <= link->last_update_time)
                continue;
            if (def->file_idx && def->saved)
                continue;
            if (def->mod_time > mod_time)
                mod_time = def->mod_time;
            pktSendBitsPack(pak,1,k);
            sendDef(pak,def);
        }
    }
    pktSendBitsPack(pak,1,-1);
#endif
*/
