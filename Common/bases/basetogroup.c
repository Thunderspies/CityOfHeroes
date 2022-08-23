#include <utilitieslib/utils/mathutil.h>
#include "group/group.h"
#include <utilitieslib/assert/assert.h>
#include "bases/bases.h"
#include "bases/basetogroup.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/utils/utils.h>
#include "group/groupgrid.h"
#include "basefromgroup.h"
#include "seq/anim.h"
#include "bases/basedata.h"
#include "bases/basesystems.h"
#include <utilitieslib/utils/textparser.h>
#include "basetrim.h"
#include "gridcoll/gridcache.h"
#include "group/grouptrack.h"
#include <utilitieslib/utils/error.h>
#include "player/pmotion.h"

#if CLIENT
#include "render/tex.h"
#endif

#if SERVER
#include "entity/entity.h"
#include "entity/entserver.h"
#include "group/grouputil.h"
#include "group/groupProperties.h"
#include "gameSys/sgraid_V2.h"
#endif

typedef struct
{
    GroupEnt    *ents;
    int            count,max;
} EntList;

static EntList    room_list;
static EntList    decor_list[ROOMDECOR_MAXTYPES*2];
static g_editor_only;

static GroupEnt *groupentAlloc(EntList *list)
{
    GroupEnt    *ent;
    ent = dynArrayAdd(&list->ents,sizeof(list->ents[0]),&list->count,&list->max,1);
    if (!ent->mat)
        ent->mat = mpAlloc(group_info.mat_mempool);
    copyMat4(unitmat,ent->mat);
    return ent;
}


static GroupDef *findPart(BaseRoom *room,char *partname,char *subname,int height,int *idxp)
{
    char        basename[256],heightname[256],full_partname[256],noswap[256],noswapheight[256];
    GroupDef    *def;
    char        *geoswap =  "tech01";
    RoomDecor    idx=-1;
    DecorSwap    *swap;

    strcpy(full_partname,partname);
    if (subname)
        strcatf(full_partname,"_%s",subname);
    if ((stricmp(partname,"Ceiling")==0 || stricmp(full_partname,"Wall_Upper")==0) && height < 32)
        height += 16;
    if (!baseGetDecorIdx(full_partname,height,&idx) && !baseGetDecorIdx(partname,height,&idx))
        return 0;

    swap = decorSwapFind(room,idx);
     if (swap && swap->geo[0])
        geoswap = swap->geo;
    sprintf(noswap,"_base_%s_%s","tech01",full_partname);
    sprintf(basename,"_base_%s_%s",geoswap,full_partname);
    sprintf(heightname,"%s_%d",basename,height);
    sprintf(noswapheight,"%s_%d",noswap,height);
    def = groupDefFindWithLoad(heightname);
    if (!def)
        def = groupDefFindWithLoad(basename);

    if(!def)
        def = groupDefFindWithLoad(geoswap);

    if(!def)
        def = groupDefFindWithLoad(noswapheight);

    if(!def)
        def = groupDefFindWithLoad(noswap);

    if(!def)
        Errorf( "Could not find any of these defs: %s, %s, %s, %s or %s", heightname, basename, geoswap,noswapheight,noswap );
    if (idxp)
        *idxp = idx;
    return def;
}

static void setPartYawAndPos(GroupEnt *ent,F32 gridsize,F32 yaw_degrees,F32 x,F32 y, F32 z,F32 sideways,F32 forwards)
{
    Vec3    pyr = {0,0,0};

    pyr[1] = RAD(yaw_degrees);
    createMat3RYP(ent->mat,pyr);
    ent->mat[3][0] = x * gridsize + ROOM_SUBBLOCK_SIZE/2;
    ent->mat[3][1] = y;
    ent->mat[3][2] = z * gridsize + ROOM_SUBBLOCK_SIZE/2;
    moveinZ(ent->mat,forwards * gridsize);
    moveinX(ent->mat,sideways * gridsize);
}

GroupEnt *roomAddPartYaw(BaseRoom *room,char *partname,char *subname,F32 x,F32 y,F32 z,F32 yaw,F32 sideways,F32 forward)
{
    GroupEnt    *ent;
    int            idx;
    GroupDef    *def;


    def = findPart(room,partname,subname,y,&idx);
    assert(idx >=0 && idx < ROOMDECOR_MAXTYPES);
    if (g_editor_only)
        ent = groupentAlloc(&decor_list[ROOMDECOR_MAXTYPES + idx]);
    else
        ent = groupentAlloc(&decor_list[idx]);
    ent->def = def;
    setPartYawAndPos(ent,ROOM_SUBBLOCK_SIZE,yaw,x,y,z,sideways,forward);
    return ent;
}

static GroupEnt *addPartSimple(BaseRoom *room,char *partname,F32 x,F32 y,F32 z)
{
    return roomAddPartYaw(room,partname,0,x,y,z,0,0,0);
}

static void *ParserAllocStructConstructor(int size)
{
    return ParserAllocStruct(size);
}

static BaseRoom *scaleRoomBlocks(BaseRoom *room,BaseRoom *new_room)
{
    int        x,z,i,j,scale;

    scale = room->blockSize / ROOM_SUBBLOCK_SIZE;

    *new_room = *room;
    new_room->size[0] = room->size[0] * scale;
    new_room->size[1] = room->size[1] * scale;
    new_room->blocks = 0;
    new_room->doors = 0;
    roomAllocBlocks(new_room);
    eaCopyEx(&room->doors,&new_room->doors, sizeof(RoomDoor), ParserAllocStructConstructor);

    for(z=0;z<new_room->size[1];z++)
    {
        for(x=0;x<new_room->size[0];x++)
        {
            *new_room->blocks[z*new_room->size[0] + x] = *room->blocks[z/scale *room->size[0] + x/scale];
        }
    }
    for(i=0;i<eaSize(&room->doors);i++)
    {
        copyVec2(room->doors[i]->height,new_room->doors[i]->height);
        for(j=0;j<2;j++)
        {
            new_room->doors[i]->pos[j] = room->doors[i]->pos[j] * scale;
            if (new_room->doors[i]->pos[j] < -1)
                new_room->doors[i]->pos[j] = -1;
            if (new_room->doors[i]->pos[j] >= new_room->size[j])
                new_room->doors[i]->pos[j] = new_room->size[j];
        }
    }
    new_room->blockSize = ROOM_SUBBLOCK_SIZE;
    return new_room;
}

static void blockToDefs(BaseRoom *room,int x,int z,int doorway)
{
    int        block[2];
    F32        lower,upper,heights[2];

    block[0] = x;
    block[1] = z;
    roomGetBlockHeight(room,block,heights);
    lower = heights[0];
    upper = heights[1];

    if (upper <= lower)
    {
        GroupEnt    *ent;

        ent = addPartSimple(room,"Floor",x,0,z); 
        ent->mat[3][1] = ROOM_MAX_HEIGHT;
        ent = addPartSimple(room,"Ceiling",x,ROOM_MAX_HEIGHT,z);
        assert(ent->def);
        ent->mat[3][1] = 0;
    }
    else
    {
        BaseBlock    *b = roomGetBlock(room,block);

        if (!b || !b->replaced[0])
            addPartSimple(room,"Floor",x,lower,z);
        if (!b || !b->replaced[1])
            addPartSimple(room,"Ceiling",x,upper,z);
        roomAddBlockTrim(room,x,z,lower,upper,-1);
    }
    if (1)
    {
        g_editor_only = 1;
        if (doorway || z == 0)
            roomAddBlockTrim(room,x,z-1,0,ROOM_MAX_HEIGHT,2);
        if (doorway || x == 0)
            roomAddBlockTrim(room,x-1,z,0,ROOM_MAX_HEIGHT,3);
        if (doorway || z == room->size[1]-1)
            roomAddBlockTrim(room,x,z+1,0,ROOM_MAX_HEIGHT,0);
        if (doorway || x == room->size[0]-1)
            roomAddBlockTrim(room,x+1,z,0,ROOM_MAX_HEIGHT,1);
        g_editor_only = 0;
    }
}

static char *findTexForPart(BaseRoom *room,RoomDecor idx)
{
    GroupDef    *def;
    Model        *model;
    char        **texnames,*texname;

    def = findPart(room,roomdecor_names[idx],0,0,0);
    if (!def)
        return 0;

    model = def->model;
    if (!model || !model->tex_count)
        return 0;
    texnames = model->gld->texnames.strings;
    texname = texnames[model->tex_idx[0].id];
    return texname;
}

static void setupRoomTexSwaps(BaseRoom *room)
{
    char        *tex_name;
    int            i;
    GroupDef    *def = room->def;

    eaClearEx(&def->def_tex_swaps, deleteDefTexSwap);
    for(i=0;i<eaSize(&room->swaps);i++)
    {
        DefTexSwap *dts;
        DecorSwap    *swap = room->swaps[i];

        if (!swap->tex[0])
            continue;
        tex_name = findTexForPart(room,swap->type);
        if (!tex_name)
            continue;
        dts = createDefTexSwap(tex_name, swap->tex, 1);
#if CLIENT
        if (!dts->comp_tex_bind || !dts->comp_replace_bind)
        {
            deleteDefTexSwap(dts);
            continue;
        }
#endif
        eaPush(&def->def_tex_swaps, dts);
    }
}

static GroupDef *createLight()
{
    GroupDef    *def,*omnilight;
    omnilight = groupDefFindWithLoad("omnilight");
    assert(omnilight);
    def = groupDefDup(omnilight,0);
    def->light_radius = 35;
    return def;
}

static void addLights(BaseRoom *room,int x,int z)
{
    GroupEnt    *ent;
    int            i;
      F32            heights[] = { 12, 22, 30 };

       for(i=0;i<eaSize(&room->lights);i++)
    {
        ent = groupentAlloc(&room_list);
        ent->def = room->light_defs[i];
        setPartYawAndPos(ent,ROOM_SUBBLOCK_SIZE,0,x,heights[i],z,0.75,0.75);
    }
}

static void addDetail(BaseRoom *room,RoomDetail *detail)
{
    GroupDef    *def;
    GroupEnt    *ent;
    int i;

#if SERVER
    // Set these values up automatically
    if (!detail->bPowered && !detail->info->iEnergyConsume) 
        detail->bPowered = true;
    if (!detail->bControlled && !detail->info->iControlConsume) 
        detail->bControlled = true;

    detailFlagBaseUpdate(detail);
#endif

    if(detail->info->pchGroupNameBaseEdit)
    {
        GroupDef    *be_def;
        GroupEnt    *be_ent;
        be_def = groupDefFindWithLoad(detail->info->pchGroupNameBaseEdit);
        if (be_def)
        {
            be_ent = groupentAlloc(&room_list);
            be_ent->def = be_def;
            copyMat4(detail->mat,be_ent->mat);
        }
    }
    
    // Mildly hacky: use level field to determine if we should add this as geometry, or let
    //  server spawn as ent
    if(!detail->info->iLevel)
    {
        def = groupDefFindWithLoad(detail->info->pchGroupName);
        if (!def)
            return;
        ent = groupentAlloc(&room_list);
        ent->def = def;
        copyMat4(detail->mat,ent->mat);
        if (baseDetailIsPillar(detail->info))
        {
            Vec2    heights;
            int        i,block[2];
            char    name[128],*cap_names[2] = { "Base", "Capital" };

            for(i=0;i<2;i++)
            {
                sprintf(name,"%s_%s",detail->info->pchGroupName,cap_names[i]);
                def = groupDefFindWithLoad(name);
                if (!def)
                    continue;
                if (baseRoomIdxFromPos(room,detail->mat[3],&block[0],&block[1]) < 0)
                    continue;
                ent = groupentAlloc(&room_list);
                ent->def = def;
                copyMat4(detail->mat,ent->mat);
                roomGetBlockHeight(room,block,heights);
                ent->mat[3][1] = heights[i];
            }
        }

        // Set up texture swap and tinting

        if (detail->tints[0].integer && detail->tints[1].integer)
        {
            memcpy(def->color,detail->tints,sizeof(def->color));
            def->has_tint_color = 1;
        }
        eaClearEx(&def->def_tex_swaps, deleteDefTexSwap);
        for(i=0;i<eaSize(&detail->swaps);i++)
        {
            DefTexSwap *dts;
            DetailSwap    *swap = detail->swaps[i];

            if (!swap->original_tex[0] || !swap->replace_tex[0])
                continue;
            dts = createDefTexSwap(swap->original_tex, swap->replace_tex, 1);
#if CLIENT
            if (!dts->comp_tex_bind || !dts->comp_replace_bind)
            {
                deleteDefTexSwap(dts);
                continue;
            }
#endif
            eaPush(&def->def_tex_swaps, dts);
        }

    }
#if SERVER
    else if(!detail->e)
    {
        // This will construct the entity, give it the proper costumes, behaviors, etc
        roomDetailRebuildEntity(room,detail);
    }
#endif
}

static void copyEntsToDef(EntList *list,GroupDef *def)
{
    int        i;

    groupDefModify(def);
    for(i=0;i<def->count;i++) {
        mpFree(group_info.mat_mempool,def->entries[i].mat);
        def->entries[i].mat = NULL;
    }
    free(def->entries);
    def->count = list->count;
    def->entries = malloc(sizeof(GroupEnt) * list->count );
    memcpy(def->entries,list->ents,sizeof(list->ents[0]) * list->count);
    memset(list->ents,0,sizeof(list->ents[0]) * list->count);
    groupSetBounds(def);
    groupSetVisBounds(def);

    if (def)
        def->try_to_weld = 1;
}

void roomToDefs(GroupDef *def, BaseRoom *room_orig, int suffix)
{
    int            i,x,z;
    BaseRoom    room_double = {0},*room;
    RoomDecor    type;

    room = scaleRoomBlocks(room_orig,&room_double);
 
    roomSetDetailBlocks(room);
    room_list.count = 0;
    for(i=0;i<ROOMDECOR_MAXTYPES*2;i++)
    {
        GroupDef    *def;
        GroupEnt    *ent;
        DecorSwap    *swap;

        type = i % ROOMDECOR_MAXTYPES;
        def = room->decor_defs[i];
        decor_list[i].count = 0;
        if (!def)
        {
            char    name[256];

            def = room->decor_defs[i] = groupDefNew(0);
            sprintf(name,"grproom_%d_%d_%s%s",suffix, room_orig->id,roomdecor_names[type],i==type ? "" : "_editoronly");
            groupDefName(def,0,name);
        }
        groupDefModify(def);
        ent = groupentAlloc(&room_list);
        ent->def = def;

        memset(def->color,0,sizeof(def->color));
        swap = decorSwapFind(room_orig,type);
         if(swap && ( swap->tints[0].integer || swap->tints[1].integer ) )
        {
            memcpy(def->color,swap->tints,sizeof(def->color));
            def->has_tint_color = 1;
        }
        else
        {
            setVec3(def->color[0], 128, 128, 128);
            def->color[0][3] = 0xff;
            memcpy(def->color[1], def->color[0],sizeof(def->color[1]));

            def->has_tint_color = 1;
        }
    }
    for(z=0;z<room->size[1];z++)
    {
        for(x=0;x<room->size[0];x++)
            blockToDefs(room,x,z,0);
    }
    for(z=0;z<room->size[1];z++)
    {
        for(x=0;x<room->size[0];x++)
            blockConnectTrim(room,x,z);
    }
    for(z=0;z<room->size[1];z++)
    {
        for(x=0;x<room->size[0];x++)
            blockConnectTrimCaps(room,x,z);
    }

    for(i=0;i<eaSize(&room->lights);i++)
    {
        if (!room->light_defs[i])
            room->light_defs[i] = createLight();
        groupDefModify(room->light_defs[i]);
        memcpy(room->light_defs[i]->color[0],room->lights[i]->rgba,sizeof(Color));
    }
    for(z=0;z<room->size[1];z+=2)
    {
        for(x=0;x<room->size[0];x+=2)
            addLights(room,x,z);
    }
    for(i=0;i<eaSize(&room->details);i++)
    {
        addDetail(room,room->details[i]);
    }
    for(i=0;i<eaSize(&room->doors);i++)
    {
        RoomDoor    *door = room->doors[i];

        if (door->height[1])
        {
            int        offset[2] = {0,0},edge,center,primarydoor;

            primarydoor = door->pos[0] >= room->size[0] || door->pos[1] >= room->size[1];
            edge = door->pos[1] < 0 || door->pos[1] >= room->size[1];
            center = !edge;
            offset[center]    = 1;
            blockToDefs(room,door->pos[0],door->pos[1],1);
            blockToDefs(room,door->pos[0] + offset[0],door->pos[1] + offset[1],1);
            if (0 && primarydoor)
            {
                GroupEnt    *ent;
                if (door->pos[1] < 0 || door->pos[1] >= room->size[1])
                    ent = roomAddPartYaw(room,"Doorway",0,door->pos[0],door->height[1],door->pos[1],0,0.5,0.5);
                else
                    ent = roomAddPartYaw(room,"Doorway",0,door->pos[0],door->height[1],door->pos[1],90,-0.5,0.5);
                ent->mat[3][1] = 0;
            }
        }
    }
    memcpy(room_orig->decor_defs,room->decor_defs,sizeof(room->decor_defs));
    memcpy(room_orig->light_defs,room->light_defs,sizeof(room->light_defs));
    room_orig->def = def;
    for(i=0;i<ROOMDECOR_MAXTYPES*2;i++)
        copyEntsToDef(&decor_list[i],room_orig->decor_defs[i]);
    copyEntsToDef(&room_list,def);
    setupRoomTexSwaps(room_orig);

    def->has_ambient = 1;
    setVec3(def->color[0],10,10,10);
    setVec3(def->color[1],100,100,100);
    eaDestroyEx(&room->blocks,ParserFreeStruct);
}

static void fixupRoomDefPtrs(BaseRoom *room,GroupDef *def)
{
    int        j;

    room->def = def;
    for(j=0;j<ROOMDECOR_MAXTYPES*2;j++)
    {
        room->decor_defs[j] = def->entries[j].def;
    }
}

static F32 *s_offset = NULL;

void baseToDefsWithOffset(Base *base, int suffix, Vec3 offset)
{
    s_offset = offset;
    baseToDefs(base, suffix);
    s_offset = NULL;
}

void baseToDefs(Base *base, int suffix)
{
    GroupDef    *def;
    GroupEnt    *ent;
    DefTracker    *ref;
    int            i, j, w, h, new_count, room_part_count;
    char        name[256];

    def = base->def;
    if (!def)
    {
        if (!group_info.file_count)
            groupFileNew();
        def = base->def  = groupDefNew(0);
        sprintf(name, "grpbase_%d", suffix);
        groupDefName(def, 0, name);
        groupDefModify(def);
    }
    group_info.files[0]->base = base;
    baseLoadUiDefs();
//    groupDefModify(def);
 
     new_count = room_part_count = eaSize(&base->rooms);
     if( base->plot ) // because there is no happy place for plot def
    {
        w = base->plot_width/4;
        h = base->plot_height/4;
        new_count = room_part_count+(w*h)+1;
    }

    if (!def->entries || def->count != new_count )
    {
        def->entries = realloc(def->entries,sizeof(def->entries[0]) * new_count);
        base->room_mod_times = realloc(base->room_mod_times,sizeof(base->room_mod_times[0]) * new_count);
        if (new_count > def->count)
        {
            ZeroStructs(def->entries+def->count,new_count-def->count);
            ZeroStructs(base->room_mod_times+def->count,new_count-def->count);
        }
        else
        {
            ZeroStructs(base->room_mod_times,new_count);
        }
        groupDefModify(def);
    }
    for(i=0;i<room_part_count;i++)
    {
        int        rebuild=1;

        ent = &def->entries[i];

        if (!ent->def || ent->def == base->plot_def || ent->def == base->sound_def )
        {
            ent->def  = groupDefNew(0);
            sprintf(name, "grproom_%d_%d", suffix, base->rooms[i]->id);
            groupDefName(ent->def,0,name);
            if (!ent->mat)
                ent->mat = mpAlloc(group_info.mat_mempool);
            copyMat4(unitmat,ent->mat);
        }
        else if (base->room_mod_times[i] == base->rooms[i]->mod_time)
            rebuild = 0;
        copyVec3(base->rooms[i]->pos,ent->mat[3]);
        if (rebuild)
            roomToDefs(ent->def, base->rooms[i], suffix);
        else
            fixupRoomDefPtrs(base->rooms[i],ent->def);
        base->room_mod_times[i] = base->rooms[i]->mod_time;
    }

    // add plot def (crammed into last entry)
     if( base->plot )
    {
        Vec3 pos;
        for( i = 0; i < w; i++ )
        {
            for( j = 0; j < h; j++ )
            {                
                ent = &def->entries[room_part_count+(i*h)+j];

                if(!base->plot_def && base->plot && base->plot->pchGroupName )
                    base->plot_def = groupDefFindWithLoad( base->plot->pchGroupName  );
                ent->def = base->plot_def;

                if (!ent->mat)
                {    
                    ent->mat = mpAlloc(group_info.mat_mempool);
                    copyMat4(unitmat,ent->mat);
                }
            
                 copyVec3(base->plot_pos, pos);
                   pos[0] += (i+1)*base->roomBlockSize*4;
                 pos[1] -= .25f;
                pos[2] += j*base->roomBlockSize*4;
                copyVec3(pos, ent->mat[3]);
            }
        }
        ent = &def->entries[new_count - 1];

        if(!base->sound_def && base->plot)
            base->sound_def = groupDefFindWithLoad("Omni/soundsphere");
        ent->def = base->sound_def;
        groupAllocString(&base->sound_def->sound_name);
        strncpyt(base->sound_def->sound_name,"WarehouseAmb2_loop",127);
        base->sound_def->sound_vol = 0.55*128.f;
        base->sound_def->sound_radius = 5000.f;
        base->sound_def->sound_ramp = 5000.f;

        if (!ent->mat)
        {    
            ent->mat = mpAlloc(group_info.mat_mempool);
            copyMat4(unitmat,ent->mat);
        }

        copyVec3(base->plot_pos, pos);
        pos[0] += base->plot_width*base->roomBlockSize / 2;
        pos[2] += base->plot_height*base->roomBlockSize / 2;
        copyVec3(pos, ent->mat[3]);
    }
    
#if SERVER
    baseSystemsTick_PhaseOne(base, 0.0);
    baseSystemsTick_PhaseOne(base, 0.0); // Total hack to pick up changes made by adding aux items
#endif

    def->count = new_count;
    groupSetBounds(def);
    groupSetVisBounds(def);
    ref=base->ref;
    if (!ref)
    {
        ref = base->ref = groupRefNew();
        groupRefActivate(ref);
        groupRefModify(ref);
        ref->def = base->def;
        copyMat4(unitmat,ref->mat);
    }
#if SERVER
    trackerClose(ref);
    groupRefGridDel(ref);
#endif
    if (s_offset)
    {
        addVec3(ref->mat[3], s_offset, ref->mat[3]);
    }
#if SERVER
    groupRefActivate(ref);
    groupRefModify(ref);
#endif
    gridCacheInvalidate();
    pmotionVolumeTrackersInvalidate(); // make entities forget what volumes they've been in
    #if CLIENT
    {
        extern int light_recalc_count;

        printf("light recalcs: %d\n",light_recalc_count);
        light_recalc_count = 0;
    }
    #endif
}

