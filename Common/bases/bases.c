#include <utilitieslib/utils/utils.h>
#include "bases/bases.h"
#include "group/group.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include "bases/basedata.h"
#include "bases/basesystems.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/EString.h>
#include "seq/anim.h"
#include "gridcoll/gridcoll.h"
#include "group/groupfileload.h"
#include <float.h>
#include "bases/baselegal.h"
#include "entity/entity.h"
#include "entity/Supergroup.h"
#include "entity/entPlayer.h"
#include <utilitieslib/utils/error.h>
#include "bases/baseparse.h"
#include "bases/baseRaid.h"
#include "entity/boost.h"

#if SERVER
#include "ailib/aiBehaviorPublic.h"
#include "entity/entity.h"
#include "entity/entserver.h"
#include "ai/entai.h"
#include "generator/entgen.h"
#include "gameComm/VillainDef.h"
#include "seq/seq.h"
#include "gameComm/Npc.h"
#include "entity/character_animfx.h"
#include "bases/baseserverrecv.h"
#include "bases/baseparse.h"
#include "bases/baseserver.h"
#include "dbcomm/dbdoor.h"
#include "entity/motion.h"
#include "gameSys/sgraid.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/log.h>

extern int world_modified;

#include "gameSys/raidmapserver.h"
#include "storyarc/storyarcprivate.h"

#elif CLIENT
#include "entity/entclient.h"
#include "cmdparse/cmdgame.h"
#include "UI/sprite/sprite_text.h"
#endif
#include "seq/tricks.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#define addsome(x) ((x) > 0?((x)+0.01):(x)-0.01)
#define subsome(x) ((x) > 0?((x)-0.01):(x)+0.01)
#define trunc(x) ((x) > 0?floor(x):ceil(x))

void baseFree(Base * base)
{

}



char *roomdecor_names[] = 
{
    "Floor_0",
    "Floor_4",
    "Floor_8",
    "Ceiling_32",
    "Ceiling_40",
    "Ceiling_48",
    "Wall_Lower_4",
    "Wall_Lower_8",
    "Wall_16",
    "Wall_32",
    "Wall_48",
    "Wall_Upper_32",
    "Wall_Upper_40",
    "TrimLow",
    "TrimHi",
    "Doorway",
};  // there is another array clientside 'roomdecor_desc' that contians UI info



/**********************************************************************func*
* FindDecorIdx
*
*/
int baseFindDecorIdx(const char *pchGeo)
{
    int i;
    for(i=0; i<ARRAY_SIZE(roomdecor_names); i++)
    {
        if(strstriConst(pchGeo, roomdecor_names[i]))
        {
            return i;
        }
    }

    return -1;
}

baseGoupNameList baseReferenceLists[ROOMDECOR_MAXTYPES+1];
Base    g_base;
int g_isBaseRaid = 0;
Base **g_raidbases;
eRaidType g_baseRaidType;

void baseAddReferenceName( char * name )
{
    int i, decor_types = ROOMDECOR_MAXTYPES;

    if( !baseReferenceLists[0].names )
    {
         for( i = 0; i < decor_types; i++ )
             eaCreate(&baseReferenceLists[i].names);
    }

    for( i = ARRAY_SIZE(roomdecor_names)-1; i >= 0; i-- )
    {
        if( strstriConst( name, roomdecor_names[i]) )
        {
            if( (i == ROOMDECOR_LOWERTRIM || i == ROOMDECOR_UPPERTRIM) )
            {    
                if( !strstriConst( name, "str_con" ) )
                    return;
            }

            eaPush( &baseReferenceLists[i].names, name );
            return;
        }
    }
}



static int baseReferenceSort(const char** name1, const char** name2 )
{
#if CLIENT
    return strcmp( textStd((char*)*name1), textStd((char*)*name2) );
#elif SERVER
    return strcmp( *name1, *name2 );
#endif
}


void baseMakeAllReferenceArray()
{
    int i, j, count;

    // create erray of unique geometry types from first room decor
    count = eaSize(&baseReferenceLists[0].names);
    for( i = 0; i < count; i++ )
    {
        char str[128];
        char * endPtr;
        bool earray_push = true;
        strncpyt( str, baseReferenceLists[0].names[i]+6, 128 ); // +6 skips "_base_"
        endPtr = strstri( str, roomdecor_names[0] ) - 1;  // -1 skips past trailing "_"
        *endPtr = '\0';

        // make sure its not there already
        for( j = 0; j < eaSize(&baseReferenceLists[ROOMDECOR_ALL].names); j++ )
        {
            if( stricmp( str, baseReferenceLists[ROOMDECOR_ALL].names[j] ) == 0 )
                earray_push = false;
        }

        if( earray_push )
            eaPush( &baseReferenceLists[ROOMDECOR_ALL].names, strdup(str) );
    }

    // now check each item in the earray to make sure it has a match for each room decor type
    count = 0;
    while( count < eaSize(&baseReferenceLists[ROOMDECOR_ALL].names) )
    {
        int all_decors_had_match = true;

        for( i = ROOMDECOR_MAXTYPES-1; i >= 0; i-- )
        {
            int match_found = false;

            for( j = eaSize(&baseReferenceLists[i].names)-1; j >= 0; j-- )
            {
                if( strstri( baseReferenceLists[i].names[j], baseReferenceLists[ROOMDECOR_ALL].names[count] ) )
                    match_found = true;
            }

            if( !match_found )
                all_decors_had_match = false;
        }

        if( !all_decors_had_match )
        {
            free(eaRemove( &baseReferenceLists[ROOMDECOR_ALL].names, count ));
            count = 0;
        }
        else
            count++;
    }

    for( i = 0; i < ROOMDECOR_ALL; i++ )
    {
        eaQSort(baseReferenceLists[i].names, baseReferenceSort );
    }
}


void roomAllocBlocks(BaseRoom *room)
{
    int        i;

    if (!room->lights)
    {
        eaCreate(&room->lights);
        eaSetSize(&room->lights,3);
        for(i=0;i<eaSize(&room->lights);i++)
            room->lights[i] = ParserAllocStruct(sizeof(Color));
    }
    if (!room->doors)
    {
        eaCreate(&room->doors);
#if 0
        eaSetSize(&room->doors,4);
        for(i=0;i<eaSize(&room->doors);i++)
            room->doors[i] = ParserAllocStruct(sizeof(RoomDoor));
#endif
    }
    eaDestroyEx(&room->blocks,ParserFreeStruct);
    eaCreate(&room->blocks);
    eaSetSize(&room->blocks,room->size[0] * room->size[1]);
    for(i=0;i<eaSize(&room->blocks);i++)
        room->blocks[i] = ParserAllocStruct(sizeof(BaseBlock));
}




int baseGetDecorIdx(char *partname,int height,RoomDecor *idx)
{
    char    heightname[200];
    int        i;

    if (height < 0)
        strcpy(heightname,partname);
    else
        sprintf(heightname,"%s_%d",partname,height);
    for(i=0;i<ROOMDECOR_MAXTYPES;i++)
    {
        if (stricmp(roomdecor_names[i],heightname)==0 || stricmp(roomdecor_names[i],partname)==0)
        {
            *idx = i;
            return 1;
        }
    }
    return 0;
}

DecorSwap *decorSwapFind(BaseRoom *room,RoomDecor idx)
{
    int        i;

    for(i=0;i<eaSize(&room->swaps);i++)
    {
        if (room->swaps[i]->type == idx)
            return room->swaps[i];
    }
    return 0;
}

DecorSwap *decorSwapFindOrAdd(BaseRoom *room,RoomDecor idx)
{
    DecorSwap    *swap;

    swap = decorSwapFind(room,idx);
    if (swap)
        return swap;
    swap = ParserAllocStruct(sizeof(DecorSwap));
    swap->type = idx;
    eaPush(&room->swaps, swap);
    return swap;
}

DetailSwap *detailSwapFind(RoomDetail *detail,char original[128])
{
    int        i;

    for(i=0;i<eaSize(&detail->swaps);i++)
    {
        if (strcmp(detail->swaps[i]->original_tex,original) == 0)
            return detail->swaps[i];
    }
    return 0;
}


DetailSwap *detailSwapSet(RoomDetail *detail, char original[128], char replaced[128])
{
    DetailSwap *swap;
    swap = detailSwapFind(detail,original);
    if (swap)
    {
        memcpy(swap->replace_tex,replaced,128);
        return swap;
    }
    swap = ParserAllocStruct(sizeof(DetailSwap));
    memcpy(swap->replace_tex,replaced,128);
    memcpy(swap->original_tex,original,128);

    eaPush(&detail->swaps,swap);
    return swap;
}


void baseClearRoom(BaseRoom *room, F32 height[2])
{
    int            i;

    for(i=0;i<room->size[0]*room->size[1];i++)
    {
        BaseBlock    *block = room->blocks[i];

        if(height)
        {
            memcpy(block->height, height, sizeof(F32)*2);
        }
        else
        {
            block->height[0] = 0;
            block->height[1] = ROOM_MAX_HEIGHT;
        }
    }
    for(i=0;i<eaSize(&room->lights);i++)
        setVec3(room->lights[i]->rgba,60-10*i,60-10*i,60-10*i);
    eaDestroyEx(&room->swaps,ParserFreeStruct);
    eaDestroyEx(&room->details, detailCleanup);
    eaClearEx(&room->doors,ParserFreeStruct);

}

BaseRoom *baseGetRoom(Base *base,int id)
{
    int        i;

    for(i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        if (base->rooms[i]->id == id)
            return base->rooms[i];
    }
    return 0;
}

static void rotateEArray( void ***earray, int new_size[2], int orig_size[2], int rotation )
{
    void **temp;
    int i=0,x,z;

    if( !earray )
        return;

    //first make a copy
    eaCreate(&temp);

    // copy from original in correct order
    for( z = 0; z < new_size[1]; z++ )
    {
        for( x = 0; x < new_size[0]; x++ )
        {
            int coord[2];
            void * thing;
            coord[0] = x;
            coord[1] = z;
            rotateCoordsIn2dArray( coord, orig_size, rotation, 1 );

            thing = eaGet( earray, coord[0] + coord[1]*orig_size[0] );
            if( thing )
                eaPush(&temp, thing );
        }
    }

    // clear the original
    //eaClear(earray);
    eaSetSize(earray,0);

    // re-set all the pointers
    for(i = 0;i<eaSizeUnsafe(&temp); i++ )
        eaPush(earray,temp[i]);

    // delete copy
    eaDestroy(&temp);
}

BaseRoom *roomUpdateOrAdd(int room_id,int size[2],int rotation,Vec3 pos,F32 height[2], const RoomTemplate *info)
{
    BaseRoom    *room = baseGetRoom(&g_base,room_id);
    int            i,numblocks;

    for(i=0;i<3;i++)
        pos[i] = ((int)(pos[i]+16384) & ~31) - 16384;
    pos[1] = 0;

     if(room && rotation)
    {
        Vec3 center;
        float fGrid = g_base.roomBlockSize;

        rotateEArray( &room->blocks, size, room->size, rotation );
        //rotateEArray( &room->swaps, size, room->size, rotation );        // these are not on a per block basis
        //rotateEArray( &room->lights, size, room->size, rotation );    // *I think*

        // rotate all items
           for( i = 0; i < eaSize(&room->details); i++ )
        {
               RoomDetail * detail = room->details[i]; 
            Vec3 rotvec;
             Mat4 item_mat,rotmat,mat2;

             rotvec[0] = rotvec[2] = 0; 
            rotvec[1] = rotation*(-PI/2);

            copyMat4( unitmat, rotmat );
            createMat3PYR(rotmat, rotvec); 

            setVec3( center, room->pos[0] + room->size[0]*fGrid/2, 0, room->pos[2] + room->size[1]*fGrid/2);
              copyMat4( detail->mat, item_mat );
            addVec3( item_mat[3], room->pos, item_mat[3] );
            subVec3( item_mat[3], center, item_mat[3] );

            //mulVecMat4(item_mat[3], rotmat, mat2);
            mulMat4( rotmat, item_mat, mat2 );
            copyMat4( mat2, item_mat );

            setVec3( center, pos[0] + size[0]*fGrid/2, 0, pos[2] + size[1]*fGrid/2);
            addVec3( item_mat[3], center, item_mat[3] );
            subVec3( item_mat[3], pos, item_mat[3] );
            copyMat4( item_mat, detail->mat );

            //copyVec3( item_mat[3], pos1 );
            //mulVecMat3(item_mat[3], mat,  pos1);
            //copyVec3( pos1, item_mat[3] );
            //addVec3( item_mat[3], center, item_mat[3] );

            //subVec3( item_mat[3], room->pos, item_mat[3] );
            //copyMat4( item_mat, detail->mat );
            

            //setVec3(pos1, room->pos[0]+detail->mat[3][0] - center[0], room->pos[2]+detail->mat[3][2] - center[2], 0);
         //    
  
            //detail->mat[3][0] = center[0]+pos2[0]-room->pos[0];
            //detail->mat[3][2] = center[2]+pos2[1]-room->pos[2];

            //rotate done, now move
            
        }
    }

    if (!room)
    {
        room = ParserAllocStruct(sizeof(*room));
        room->blockSize = g_base.roomBlockSize;
        eaPush(&g_base.rooms,room);
        room->id = room_id;

        copyVec2(size,room->size);
        roomAllocBlocks(room);
        numblocks = eaSize(&g_base.rooms[0]->blocks);
        baseClearRoom(room,height);
    }

    copyVec2(size,room->size);
    copyVec3(pos,room->pos);

    if(info)
    {
        strcpy(room->name, info->pchName);
        room->info = info;
    }

    return room;
}

RoomDetail *roomGetDetail(BaseRoom *room,int id)
{
    int        i;

    for(i=eaSize(&room->details)-1;i>=0;i--)
    {
        if (room->details[i]->id == id)
            return room->details[i];
    }
    return 0;
}

RoomDetail *baseGetDetail(Base *base, int id, BaseRoom **out_room)
{
    // Also returns the room if requested
    int i;
    RoomDetail *detail = NULL;

    for(i=eaSize(&base->rooms)-1;i>=0;i--)
    {
        detail = roomGetDetail(base->rooms[i],id);
        if (detail)
        {
            if (out_room) *out_room = base->rooms[i];
            return detail;
        }
    }
    return 0;
}


RoomDetail *roomGetDetailFromPos(BaseRoom *room,Vec3 pos)
{
    int i;

    for(i=eaSize(&room->details)-1;i>=0;i--)
    {
        if (distance3Squared(pos, room->details[i]->mat[3]) < 0.1)
            return room->details[i];
    }
    return 0;
}

RoomDetail *roomGetDetailFromWorldPos(BaseRoom *room,const Vec3 worldpos)
{
    Vec3 pos;
    
    subVec3(worldpos, room->pos, pos);

    return roomGetDetailFromPos(room, pos);
}

RoomDetail *roomGetDetailFromWorldMat(BaseRoom *room, const Mat4 src)
{
    Mat4 mat;
    Vec3 pos;
    int i;
    copyMat4(src,mat);
    subVec3(mat[3],room->pos,pos);    

    for(i=eaSize(&room->details)-1;i>=0;i--)
    {
        if (distance3Squared(pos, room->details[i]->mat[3]) < 0.1)
        {   //distance must be close, but rotation must be exact
            int j;
            copyVec3(room->details[i]->mat[3],mat[3]);
            for (j = 0; j < 4; j++) {
                if (!nearSameVec3(mat[j],room->details[i]->mat[j]))
                    break;
            }
            if ( j == 4)
            {
                return room->details[i];
            }
        }
    }
    return NULL;
}


int baseRoomIdxFromPos(BaseRoom *room,Vec3 pos,int *xp,int *zp)
{
    int x,z;

    *xp = *zp = -100000;
    if (!room)
        return -1;
    x = (pos[0] + room->blockSize * 256) / room->blockSize;
    z = (pos[2] + room->blockSize * 256) / room->blockSize;
    *xp = x = x - 256;
    *zp = z = z - 256;
    if (x<0 || x>=room->size[0])
        return -1;
    if (z<0 || z>=room->size[1])
        return -1;
    return x + room->size[0] * z;
}

int roomPosFromIdx(BaseRoom *room,int idx,Vec3 pos)
{
    if (!room || idx >= room->size[0] * room->size[1])
        return 0;
    pos[1] = 0;
    pos[0] = (idx % room->size[0]) * room->blockSize;
    pos[2] = (idx / room->size[0]) * room->blockSize;
    return 1;
}

BaseRoom *baseRoomLocFromPos(Base *base, const Vec3 pos, int block[2])
{
    int i;

    for(i=0; i<eaSize(&base->rooms); i++)
    {
        Vec3 loc;
        BaseRoom* room = base->rooms[i];

#define CALC_BLOCK_POS(which, blockwhich) \
    loc[which] = (pos[which]-room->pos[which]); \
    if(loc[which] == 0)  \
        loc[which] += 1;  \
    else if(loc[which] == room->size[blockwhich]*room->blockSize) \
        loc[which] -= 1; \
    loc[which] = floor(addsome(loc[which])/room->blockSize);

        CALC_BLOCK_POS(0, 0);
        CALC_BLOCK_POS(2, 1);

        // This checks to see if the block is in the room or in any of the 
        // blocks surrounding the room.
        if(loc[0]>=-1 && loc[0]<=room->size[0]
            && loc[2]>=-1 && loc[2]<=room->size[1])
        {
            // But we don't want the exterior corners.
            if((loc[0]<0 && (loc[2]<0 || loc[2]>=room->size[1]))
                || (loc[0]>=room->size[0] && (loc[2]<0 || loc[2]>=room->size[1])))
            {
                return NULL;
            }

            if(block)
            {
                block[0] = loc[0];
                block[1] = loc[2];
            }

            return room;
        }
    }

    return NULL;
}

int getRoomBlockFromPos(BaseRoom *room, Vec3 pos, int block[2])
{
    int i,j;
    float lx, hx, ly, hy;

      if(!room)
        return 0;

    for( i = 0; i < room->size[0]; i++ )
    {
        lx = room->pos[0] + room->blockSize*i;
        hx = lx + room->blockSize;

        for( j = 0; j < room->size[1]; j++ )
        {
            ly = room->pos[2] + room->blockSize*j;
            hy = ly + room->blockSize;
            
            if( pos[0] >= lx && pos[0] <= hx && pos[2] >= ly && pos[2] <= hy )
            {
                block[0] = i;
                block[1] = j;
                return 1;
            }
        }
    }

    return 0;
}

void baseSetIllegalHeight(BaseRoom *room,int idx)
{
    room->blocks[idx]->height[0] = ROOM_MAX_HEIGHT+1;
    room->blocks[idx]->height[1] = -1;
}

int baseIsLegalHeight(F32 lower,F32 upper)
{
    int        floors[] = {0,4,8}, ceilings[] = {ROOM_MAX_HEIGHT-16,ROOM_MAX_HEIGHT-8,ROOM_MAX_HEIGHT};
    int        i,floor_ok=0,ceiling_ok=0;

    for(i=0;i<ARRAY_SIZE(floors);i++)
    {
        if (lower == floors[i])
            floor_ok = 1;
    }
    for(i=0;i<ARRAY_SIZE(ceilings);i++)
    {
        if (upper == ceilings[i])
            ceiling_ok = 1;
    }
    if (floor_ok && ceiling_ok)
        return 1;
    return 0;
}

static BaseBlock *getBlock(BaseRoom *room,int block[2])
{
    int        idx,x = block[0],z = block[1];

    if (x < 0 || z < 0 || x >= room->size[0] || z >= room->size[1])
        return 0;
    idx = x + z * room->size[0];
    return room->blocks[idx];
}

int roomDoorwayHeight(BaseRoom *room,int block[2],F32 heights[2])
{
    int        i,x,z;

    x = block[0];
    z = block[1];
    if (room->blockSize < g_base.roomBlockSize)
    {
        if (x < 0)
            x = -1;
        else if (x >= room->size[0])
            x = room->size[0];
        else
            x &= ~1;

        if (z < 0)
            z = -1;
        else if (z >= room->size[1])
            z = room->size[1];
        else
            z &= ~1;
    }
    for(i=0;i<eaSize(&room->doors);i++)
    {
        if (room->doors[i]->pos[0] == x && room->doors[i]->pos[1] == z && room->doors[i]->height[1])
        {
            if (heights)
                copyVec2(room->doors[i]->height,heights);
            return 1;
        }
    }
    return 0;
}

void roomGetBlockHeight(BaseRoom *room,int block_idx[2],F32 heights[2])
{
    BaseBlock    *block;

    if (roomDoorwayHeight(room,block_idx,heights))
        return;
    block = getBlock(room,block_idx);
    if (!block)
    {
        heights[1] = -1;
        heights[0] = ROOM_MAX_HEIGHT + 1;
    }
    else
        copyVec2(block->height,heights);
}

void baseLoadUiDefs()
{
    GroupDef    *def;
    def = groupDefFindWithLoad("_Bases_ground_mount_16");
    assert(def);
    def = groupDefFindWithLoad("_Bases_ground_mount_32");
    assert(def);
}

BaseBlock *roomGetBlock(BaseRoom *room, int block[2])
{
    if( !room->blocks )
        return 0;
    if (block[0] < 0 || block[0] >= room->size[0])
        return 0;
    if (block[1] < 0 || block[1] >= room->size[1])
        return 0;
    return room->blocks[block[1] * room->size[0] + block[0]];
}

int baseDetailIsPillar(const Detail *detail)
{
    if (detail && strEndsWith(detail->pchGroupName,"pillar"))
        return 1;
    return 0;
}

int baseDetailIsPit(const Detail *detail)
{
    if (detail && strstriConst(detail->pchGroupName,"_pit_"))
        return 1;
    return 0;
}

int getDetailSurface(const Detail *detail)
{
    return detail->eSurface;
}

static void snapWallSide(Vec3 min,Vec3 max)
{
    int        i;
    F32        *valp = &min[0];
    int index = -1;

    for(i=0;i<3;i+=2)
    {
        if (fabs(min[i]) <= fabs(*valp))
        {
            valp = &min[i];
            index = i;
        }
        if (fabs(max[i]) <= fabs(*valp))
        {
            valp = &max[i];
            index = i;
        }
    }
    if (index != -1)
    {
        *valp = 0.0f;
    }
}

typedef struct
{
    Vec3    min,max;
} DetailSupport;

typedef struct
{
    Vec3            min,max;
    Vec3            vol_min,vol_max;
    DetailSupport    **supports;
} DetailInfo;

static DetailInfo    **detail_infos;
static int            detail_info_max;

#define SUPPORT_FLOOR(x) (floor(x/DETAIL_BLOCK_FEET + .05) * DETAIL_BLOCK_FEET)
#define SUPPORT_CEIL(x)    (ceil(x/DETAIL_BLOCK_FEET - .05) * DETAIL_BLOCK_FEET)

int getBoundsRecur(const Mat4 mat,GroupDef *def,Vec3 min,Vec3 max,DetailInfo *info)
{
    int            i,set=0;
    GroupEnt    *ent;
    Model        *model;

    if (!def)
        return 0;
    groupLoadIfPreload(def);
    model = def->model;
    if (model && !strEndsWith(model->name,"__PARTICLE"))
    {
        TrickInfo *trick = model->trick?model->trick->info:NULL;

        // If this is a volume trigger, stick bounds elsewhere (currently only supports one volume per detail)
        if (def->volume_trigger)
        {
            groupGetBoundsMinMax(def,mat,info->vol_min,info->vol_max);
        }
        else if (!trick || (!(trick->tnode.flags1 & TRICK_NOCOLL)))
        {
            groupGetBoundsMinMax(def,mat,min,max);
            set = 1;
            if (def->stackable_floor || def->stackable_wall || def->stackable_ceiling)
            {
                DetailSupport    *support = ParserAllocStruct(sizeof(*support));
                Vec3            sup_min = {8e8,8e8,8e8}, sup_max = {-8e8,-8e8,-8e8 };

                groupGetBoundsMinMax(def,mat,sup_min,sup_max);
                for(i=0;i<3;i++)
                {
                    support->min[i] = SUPPORT_FLOOR(sup_min[i]);
                    support->max[i] = SUPPORT_CEIL(sup_max[i]);
                }
                eaPush(&info->supports,support);
            }
        }
    }
    for(i=0;i<def->count;i++)
    {
        Mat4    child_mat;

        ent = &def->entries[i];
        mulMat4(mat,ent->mat,child_mat);
        set |= getBoundsRecur(child_mat,ent->def,min,max,info);
    }
    return set;
}

int getBounds(GroupDef *def,DetailInfo *info)
{
    Vec3    min = {8e8,8e8,8e8},max = {-8e8,-8e8,-8e8};
    int        ret;

    if (!(ret=getBoundsRecur(unitmat,def,min,max,info)))
    {
        zeroVec3(info->min);
        zeroVec3(info->max);
    }
    else
    {
        copyVec3(min,info->min);
        copyVec3(max,info->max);
    }
    return ret;
}

DetailInfo *detailGetInfo(const Detail *detail)
{
    DetailInfo    *info;
    GroupDef    *def, *def_mount;

    dynArrayFit((void*)&detail_infos,sizeof(DetailInfo*),&detail_info_max,detail->uniqueId);
    if (!(info = detail_infos[detail->uniqueId]))
    {
        info = detail_infos[detail->uniqueId] = calloc(sizeof(DetailInfo),1);
        def = groupDefFindWithLoad(detail->pchGroupName);
        if (!def)
            return 0;
        getBounds(def,info);
        if (detail->bMounted && (def_mount = groupDefFindWithLoad(detail->pchGroupNameMount)))
        {
            DetailInfo info_mount;
            getBounds(def_mount,&info_mount);
            MINVEC3(info_mount.min,info->min,info->min);
            MAXVEC3(info_mount.max,info->max,info->max);
            // Supports?
        }
    }
    return info;
}

#define ROUNDIFCLOSE(x) (fabs(x - round(x)) < 0.1 ? (x) = (float)round(x) : (x))

int baseDetailExtents(const Detail *detail,Mat4 mat,Vec3 min,Vec3 max)
{
    Vec3        tmp_min, tmp_max, tmp_min2, tmp_max2, size;
    int            i;
    Surface        surf = getDetailSurface(detail);
    DetailInfo    *info;

    info = detailGetInfo(detail);
    if (!info)
        return 0;
    ROUNDIFCLOSE(mat[3][0]);
    ROUNDIFCLOSE(mat[3][1]);
    ROUNDIFCLOSE(mat[3][2]);
    mulVecMat3(info->min,mat,tmp_min);
    mulVecMat3(info->max,mat,tmp_max);
    MINVEC3(tmp_min,tmp_max,tmp_min2);
    MAXVEC3(tmp_min,tmp_max,tmp_max2);
    if (surf == kSurface_Floor)
    {
        tmp_min2[1] = 0.001; //make sure to include pivot point
    }
    else if (surf == kSurface_Ceiling)
    {
        tmp_max2[1] = -0.001; //make sure to include pivot point
    }
    else if (surf == kSurface_Wall)
    {
        snapWallSide(tmp_min2,tmp_max2);
    }
    for(i=0;i<3;i++)
    {
        if (i == 1 && surf == kSurface_Ceiling)
            mat[3][i] = ceil(mat[3][i]/DETAIL_BLOCK_FEET) * DETAIL_BLOCK_FEET;
        else
            mat[3][i] = floor(mat[3][i]/DETAIL_BLOCK_FEET) * DETAIL_BLOCK_FEET;
        if (baseDetailIsPillar(detail))
        {
            if (i != 1)
                mat[3][i] = floor(mat[3][i]/16) * 16 + 8;
            else
                mat[3][i] = 0;
        }
        if (detail->iReplacer)
        {
             if (i != 1)
                mat[3][i] = floor(mat[3][i]/g_base.roomBlockSize) * g_base.roomBlockSize + g_base.roomBlockSize/2;
        }
        tmp_min[i] = floor(tmp_min2[i]/DETAIL_BLOCK_FEET + .01) * DETAIL_BLOCK_FEET;
        tmp_max[i] = ceil(tmp_max2[i]/DETAIL_BLOCK_FEET - .01) * DETAIL_BLOCK_FEET;
        size[i] = (tmp_max[i] - tmp_min[i]) / DETAIL_BLOCK_FEET;
        if (size[i] == 0.0f)
        {
            tmp_max[i]+= 1.0f;
        }
    }
    addVec3(mat[3],tmp_min,min);
    addVec3(mat[3],tmp_max,max);
    return 1;
}

int baseDetailVolumeTriggerExtents(const Detail *detail,Mat4 mat,Vec3 min,Vec3 max)
{
    Vec3        tmp_min, tmp_max, tmp_min2, tmp_max2;
    int            i;
    DetailInfo    *info;

    info = detailGetInfo(detail);
    if (!info)
        return 0;
    mulVecMat3(info->vol_min,mat,tmp_min);
    mulVecMat3(info->vol_max,mat,tmp_max);
    MINVEC3(tmp_min,tmp_max,tmp_min2);
    MAXVEC3(tmp_min,tmp_max,tmp_max2);
    for(i=0;i<3;i++)
    {
        // Increase precision for volumes because some are offset non-integer distances.  Awesome!
        mat[3][i] = 0.1 * floor(10.0*mat[3][i]/DETAIL_BLOCK_FEET) * DETAIL_BLOCK_FEET;
        tmp_min[i] = 0.1 * floor(10.0*(tmp_min2[i]/DETAIL_BLOCK_FEET + .01)) * DETAIL_BLOCK_FEET;
        tmp_max[i] = 0.1 * ceil(10.0*(tmp_max2[i]/DETAIL_BLOCK_FEET - .01)) * DETAIL_BLOCK_FEET;
    }
    addVec3(mat[3],tmp_min,min);
    addVec3(mat[3],tmp_max,max);
    return 1;
}


int basePosToGrid(int size[2],F32 feet,Vec3 pos,int xz[2],int big_end,int clamp)
{
    int        i;

    for(i=0;i<2;i++)
    {
        if (big_end)
            xz[i] = floor((pos[i*2] - 0.01) / feet);
        else
            xz[i] = floor(pos[i*2] / feet);
        if (xz[i] < 0)
        {
            if (!clamp)
                return 0;
            xz[i] = 0;
        }
        if (xz[i] >= size[i])
        {
            if (!clamp)
                return 0;
            xz[i] = size[i]-1;
        }    
    }
    return 1;
}

int blockGridUnused(BaseRoom *room,Vec3 min,Vec3 max)
{
    int        x,z,min_grid[2],max_grid[2];

    if (!basePosToGrid(room->size,room->blockSize,min,min_grid,0,0))
        return 0;
    if (!basePosToGrid(room->size,room->blockSize,max,max_grid,1,0))
        return 0;
    for(z=min_grid[1];z<=max_grid[1];z++)
    {
        for(x=min_grid[0];x<=max_grid[0];x++)
        {
            if (room->blocks[z * room->size[0] + x]->height[0] > min[1])
                return 0;
            if (room->blocks[z * room->size[0] + x]->height[1] < max[1])
                return 0;
        }
    }
    return 1;
}

int blockGridContact(BaseRoom *room,Vec3 min,Vec3 max,Surface surf)
{
    int        i,x,z,min_grid[2],max_grid[2],block[2];
    Vec2    heights;

    for(i=0;i<2;i++)
    {
        min_grid[i] = floor((min[i*2]+0.01) / room->blockSize);
        max_grid[i] = floor((max[i*2]-0.01) / room->blockSize);
    }
    for(z=min_grid[1];z<=max_grid[1];z++)
    {
        for(x=min_grid[0];x<=max_grid[0];x++)
        {
            block[0] = x;
            block[1] = z;
            roomGetBlockHeight(room,block,heights);
            if (surf == kSurface_Floor)
            {
                if (heights[0] != min[1])
                    return 0;
            }
            else if (surf == kSurface_Ceiling)
            {
                if (heights[1] != max[1])
                    return 0;
            }
            else if (surf == kSurface_Wall)
            {
                if (heights[0] < max[1] && heights[1] > min[1])
                    return 0;
            }
        }
    }
    return 1;
}

int detailFindIntersecting(BaseRoom *room,Vec3 box_min,Vec3 box_max,RoomDetail *isect_details[],int max_count)
{
    int            i,j,count=0;
    RoomDetail    *detail;
    GroupDef    *def;
    Vec3        det_min,det_max;

    for(i=0;i<eaSize(&room->details);i++)
    {
        detail = room->details[i];
        def = groupDefFindWithLoad(detail->info->pchGroupName);
        baseDetailExtents(detail->info,detail->mat,det_min,det_max);
        for(j=0;j<3;j++)
        {
            if (det_max[j] <= box_min[j])
                break;
            if (box_max[j] <= det_min[j])
                break;
        }
        if (j < 3)
            continue;
        if (count < max_count)
            isect_details[count] = detail;
        count++;
    }
    return count;
}

int detailFindContact(BaseRoom *room,Vec3 box_min,Vec3 box_max,RoomDetail *isect_details[],int max_count)
{
    int            i,j,count=0;
    RoomDetail    *detail;
    Vec3        det_min,det_max;

    for(i=0;i<eaSize(&room->details);i++)
    {
        detail = room->details[i];
        baseDetailExtents(detail->info,detail->mat,det_min,det_max);
        for(j=0;j<3;j++)
        {
            if (box_min[j] == box_max[j])
            {
                if (detail->mat[3][j] != box_min[j])
                    break;
            }
            else
            {
                if (det_max[j] <= box_min[j])
                    break;
                if (box_max[j] <= det_min[j])
                    break;
            }
        }
        if (j < 3)
            continue;
        if (count < max_count)
            isect_details[count] = detail;
        count++;
    }
    return count;
}

int detailFindSupport(BaseRoom *room,int skip_id,Vec3 box_min,Vec3 box_max,int *id_ptr)
{
    int                i,j,k,count=0,num_supports;
    Vec3            sup_min,sup_max;
    DetailInfo        *info;
    DetailSupport    *support;
    RoomDetail        *detail;

    *id_ptr = -1;
    for(i=0;i<eaSize(&room->details);i++)
    {
        detail = room->details[i];
        if (detail->id == skip_id)
            continue;
        info = detailGetInfo(detail->info);
        if (!(num_supports=eaSize(&info->supports)))
            continue;

        for(k=0;k<num_supports;k++)
        {
            F32        t;

            support = info->supports[k];
            mulVecMat4(support->min,detail->mat,sup_min);
            mulVecMat4(support->max,detail->mat,sup_max);
            for(j=0;j<3;j++)
            {
                if (sup_max[j] < sup_min[j])
                {
                    t = sup_min[j];
                    sup_min[j] = sup_max[j];
                    sup_max[j] = t;
                }
                sup_min[j] = SUPPORT_FLOOR(sup_min[j]);
                sup_min[j] = SUPPORT_CEIL(sup_min[j]);
            }
            for(j=0;j<3;j++)
            {
                if (sup_min[j] == sup_max[j])
                {
                    if (sup_max[j] != box_min[j])
                        break;
                }
                else
                {
                    if (sup_max[j] <= box_min[j])
                        break;
                    if (box_max[j] <= sup_min[j])
                        break;
                }
            }
            if (j < 3)
                continue;
            *id_ptr = detail->id;
            if (count)
                *id_ptr = 0;
            count++;
        }
    }
    return count;
}

int detailSupportFindContacts(BaseRoom *room,RoomDetail *detail,RoomDetail *isect_details[],int max_count)
{
    DetailInfo    *info = detailGetInfo(detail->info);
    int            i,j,k,num_supports,count=0;
    RoomDetail    *temp_details[100];

    if (!info || !(num_supports = eaSize(&info->supports)))
          return 0;

     for(i=0;i<num_supports;i++)
    {
        Vec3        tmp_min,tmp_max,sup_min,sup_max;

        mulVecMat3( info->supports[i]->min, detail->mat, tmp_min );
        mulVecMat3( info->supports[i]->max, detail->mat, tmp_max );

        addVec3(tmp_min,detail->mat[3],tmp_min);
        addVec3(tmp_max,detail->mat[3],tmp_max);

        MINVEC3( tmp_min, tmp_max, sup_min );
        MAXVEC3( tmp_min, tmp_max, sup_max );

        count += detailFindContact(room,sup_min,sup_max,temp_details+count,ARRAY_SIZE(temp_details)-count);

        count = MIN(count,ARRAY_SIZE(temp_details));
        for(j=count-1;j>=0;j--)
        {
            for(k=j-1;k>=0;k--)
            {
                if (temp_details[j] == temp_details[k])
                    break;
            }
            if (k >= 0 || detail->id == temp_details[j]->id)
            {
                count--;
                break;
            }
        }
    }
    CopyStructs(isect_details,temp_details,MIN(count,max_count));
    return count;
}

int detailCountInRoom( const char *pchCat, BaseRoom * room )
{
    int i, count = 0;
    for( i = eaSize(&room->details)-1; i >= 0; i-- )
    {
        if(stricmp(room->details[i]->info->pchCategory, pchCat)==0)
            count++;
    }

    return count;
}

int detailCountInBase( const char *pchCat, Base * base )
{
    int i, count = 0;
    for( i = eaSize(&base->rooms)-1; i >= 0; i-- )
        count += detailCountInRoom( pchCat, base->rooms[i] );

    return count;
}

bool detailAllowedInRoom( BaseRoom *room, RoomDetail * detail, int new_room )
{
    int i, limit = 0, bfound = 0, count;

    for( i = eaSize(&room->info->ppDetailCatLimits)-1; i>=0; i-- )
    {
        if( stricmp(room->info->ppDetailCatLimits[i]->pCat->pchName, detail->info->pchCategory)==0 )
        {
            if( !room->info->ppDetailCatLimits[i]->iLimit ) 
                return true; // unlimited 

            limit = room->info->ppDetailCatLimits[i]->iLimit;
            bfound = true;
            break;
        }
    }

    if( !bfound ) // No items from that category are allowed
        return 0;

     count = detailCountInRoom( detail->info->pchCategory, room );

    if( detail->id && !new_room )    // moving existing item
        return (count<=limit);
    else                //adding new item
        return (count<limit);
}

bool detailAllowedInBase( Base * base, RoomDetail * detail )
{
    int i, limit = 0, bfound = 0, count;

    for( i = eaSize(&base->plot->ppDetailCatLimits)-1; i>=0; i-- )
    {
        if( stricmp(base->plot->ppDetailCatLimits[i]->pCat->pchName, detail->info->pchCategory)==0 )
        {
            if( !base->plot->ppDetailCatLimits[i]->iLimit ) 
                return true; // unlimited 

            limit = base->plot->ppDetailCatLimits[i]->iLimit;
            bfound = true;
            break;
        }
    }

    if( !bfound ) // No items from that category are allowed
        return 0;

    count = detailCountInBase( detail->info->pchCategory, base );

    if( detail->id )    // moving existing item
        return (count<=limit);
    else                //adding new item
        return (count<limit);
}

int baseDetailLimit( Base * base, char * pchCat )
{
    int i;
    for( i = eaSize(&base->plot->ppDetailCatLimits)-1; i>=0; i-- )
    {
        if( stricmp(base->plot->ppDetailCatLimits[i]->pCat->pchName, pchCat)==0 )
        {
            if( !base->plot->ppDetailCatLimits[i]->iLimit ) 
                return 0x7fffffff; // unlimited 

            return base->plot->ppDetailCatLimits[i]->iLimit;
        }
    }

    return 0;
}

int detailCanFit(BaseRoom *room,RoomDetail *detail, int *error)
{
    Vec3        min,max;
    int            support_id=-1;
    int            i,count;
    RoomDetail    *details[20];
    int    block_idx[2];
    BaseBlock    *block;

    if (!baseDetailExtents(detail->info,detail->mat,min,max))
    {
        return 1;
        if(error)
            *error=kDetErr_NoExtents;
        return 0;
    }

    // fits within room
    if( min[0] < 0 || min[2] < 0 || 
        max[0] > room->blockSize * room->size[0] ||
        max[2] > room->blockSize * room->size[1] )
        return 0;

    if( min[1] < 0 || max[1] > ROOM_MAX_HEIGHT )
        return 0;

    // HACK HACK HACK.
    // For details that are hard against the south or west wall, detail->mat[0] or
    // detail->mat[2] will be an exact multiple of room->blockSize, and so roomGetBlock(...)
    // claims they're outside the room.  So we check items, and if we find
    // them in this condition, drop the block index by one prior to calling roomGetBlock(...)

     block_idx[0] = floor(detail->mat[3][0] / room->blockSize);
    block_idx[1] = floor(detail->mat[3][2] / room->blockSize);

    if (block_idx[0] == room->size[0] && detail->mat[3][0] < block_idx[0] * room->blockSize + 0.0001f)
    {
        block_idx[0]--;
    }
    if (block_idx[1] == room->size[1] && detail->mat[3][2] < block_idx[1] * room->blockSize + 0.0001f)
    {
        block_idx[1]--;
    }

    block = roomGetBlock(room,block_idx);
    if (!block)
        return 0;

    // This will loosen base restrictions across the board.
    return 1;

    if (baseDetailIsPillar(detail->info))
    {
        block_idx[0] = floor(detail->mat[3][0] / room->blockSize);
        block_idx[1] = floor(detail->mat[3][2] / room->blockSize);
        block = roomGetBlock(room,block_idx);
        if (!block || !baseIsLegalHeight(block->height[0],block->height[1]))
        {
            if(error)
                *error=kDetErr_BadPillarFit;
            return 0;
        }
    }
    else
    {
        Surface    surf;
        Vec3    edge_min,edge_max;

        if (!blockGridUnused(room,min,max))
        {
            if(error)
                *error=kDetErr_UsedBlock;
            return 0;
        }
        copyVec3(min,edge_min);
        copyVec3(max,edge_max);
        surf = detail->info->eSurface;
        if (surf == kSurface_Floor)
            edge_max[1] = edge_min[1];
        else if (surf == kSurface_Ceiling)
            edge_min[1] = edge_max[1];
        else if (surf == kSurface_Wall)
        {
            int        j;

            for(j=0;j<3;j+=2)
            {
                if (detail->mat[3][j] == edge_min[j])
                {
                    edge_min[j] = edge_max[j] = edge_min[j] - 0.5;
                }
                else if (detail->mat[3][j] == edge_max[j])
                {
                    edge_min[j] = edge_max[j] = edge_max[j] + 0.5;
                }
            }
        }
        if (!blockGridContact(room,edge_min,edge_max,surf))
        {
            int        i,j,axis[3],steps[2];
            Vec3    cube_min,cube_max;

            if (surf == kSurface_Floor || surf == kSurface_Ceiling)
            {
                axis[0] = 0;
                axis[1] = 2;
                axis[2] = 1;
            }
            else
            {
                axis[1] = 1;
                if (edge_min[0] == edge_max[0])
                    axis[0] = 2;
                else
                    axis[0] = 0;
                axis[2] = 2 - axis[0];
            }
            steps[0] = edge_max[axis[0]] - edge_min[axis[0]];
            steps[1] = edge_max[axis[1]] - edge_min[axis[1]];
            if (!steps[0] || !steps[1])
            {            
                if(error)
                    *error=kDetErr_NoSteps;
                return 0;
            }
            for(i=0;i<steps[0];i++)
            {
                for(j=0;j<steps[1];j++)
                {
                    cube_min[axis[0]] = edge_min[axis[0]] + i;
                    cube_max[axis[0]] = edge_min[axis[0]] + i + 1;
                    cube_min[axis[1]] = edge_min[axis[1]] + j;
                    cube_max[axis[1]] = edge_min[axis[1]] + j + 1;
                    cube_min[axis[2]] = cube_max[axis[2]] = edge_min[axis[2]];

                    if (!blockGridContact(room,cube_min,cube_max,surf))
                    {
                        if (!detailFindSupport(room,detail->id,cube_min,cube_max,&support_id))
                        {
                            if(error)
                                *error=kDetErr_NoStack;
                            return 0;
                        }
                    }
                }
            }
        }
    }

    count = detailFindIntersecting(room,min,max,details,ARRAY_SIZE(details));
    for(i=0;i<count;i++)
    {
        if (details[i]->id != detail->id && details[i]->id != support_id)
        {
            if(error)
                *error=kDetErr_Intersection;
            return 0;
        }
    }

    // flood fill
    return roomDetailLegal(room,detail,error);
}

int basePlotAllowed( Base *base, const BasePlot * plot )
{
    int i ,base_count, cat_count = eaSize(&plot->ppDetailCatLimits);

    for( i = 0; i < cat_count; i++ )
    {
         base_count = detailCountInBase( plot->ppDetailCatLimits[i]->pchCatName, base );

        if( plot->ppDetailCatLimits[i]->iLimit && base_count > plot->ppDetailCatLimits[i]->iLimit  )
            return false;
    }

    return true;
}


int roomHeightIntersecting(BaseRoom *room,int block[2],F32 height[2],RoomDetail *isect_details[],int max_count)
{
    int            i,j,count=0,isect_count=0;
    Vec2        old_height;
    Vec3        min,max,edge_min,edge_max;
    RoomDetail    *temp_details[100];
    

    min[1] = 0;
    min[0] = block[0] * room->blockSize;
    min[2] = block[1] * room->blockSize;
    copyVec3(min,max);
    max[0] += room->blockSize;
    max[2] += room->blockSize;

    roomGetBlockHeight(room,block,old_height);

    for(i=0;i<2;i++)
    {
        if (old_height[i] == height[i])
            continue;
        min[1] = MIN(old_height[i],height[i]);
        max[1] = MAX(old_height[i],height[i]);

        count += detailFindIntersecting(room,min,max,&temp_details[count],ARRAY_SIZE(temp_details)-count);

        copyVec3(min,edge_min);
        copyVec3(max,edge_max);
        edge_min[1] = edge_max[1] = old_height[i];
        count += detailFindContact(room,edge_min,edge_max,&temp_details[count],ARRAY_SIZE(temp_details)-count);
        for(j=0;j<3;j+=2)
        {
            copyVec3(min,edge_min);
            copyVec3(max,edge_max);
            edge_min[j] = edge_max[j] = min[j];
            count += detailFindContact(room,edge_min,edge_max,&temp_details[count],ARRAY_SIZE(temp_details)-count);
            edge_min[j] = edge_max[j] = max[j];
            count += detailFindContact(room,edge_min,edge_max,&temp_details[count],ARRAY_SIZE(temp_details)-count);
        }
    }
    for(i=0;i<count;i++)
    {
        if (baseDetailIsPillar(temp_details[i]->info))
            continue;
        for(j=0;j<isect_count;j++)
        {
            if (isect_details[j] == temp_details[i])
                break;
        }
        if (isect_count >= max_count)
            break;
        if (j >= isect_count)
            isect_details[isect_count++] = temp_details[i];
    }
    return max_count ? isect_count : count;
}

void entFreeBase(Entity *e)
{
#ifdef SERVER
    Base *base = &g_base;
    BaseRoom *room = NULL;
    RoomDetail *detail = NULL;
    
    // The ent doesn't think it's attached to a detail or a room
    //  This means either it isn't, or the detail already detached itself
    if(!e->idDetail)
        return;

    room = baseGetRoom(base, e->idRoom);

    if(room)
        detail = roomGetDetail(room, e->idDetail);
    
    if(!detail || (detail->e != e && detail->eMount != e))
    {
        // Haven't found right detail, try really hard to find it
        int i,j;
        detail = NULL;
        for (i=eaSize(&base->rooms)-1;i>=0;i--)
        {
            room = base->rooms[i];
            for (j=eaSize(&room->details)-1;j>=0;j--)
            {
                RoomDetail *d = room->details[j];
                if(d->e == e || d->eMount == e)
                {
                    detail = room->details[j];
                    break;
                }
            }
            if(detail) break;
        }
    }

    if(detail)
    {
        // We found the detail.  That means for some reason one of
        //  this detail's ents got entFree'd, maybe a normal process, 
        //  or potentially through unexpected means.
        // In response we force the detail to explodify itself,
        //  and trigger a rebuild as a destroyed version.
        bool isdestroyed = detail->bDestroyed;
        LOG_DEBUG("basedbg Entfree called on %s without clearing detail id\n",e->name);
        if(isdestroyed) detail->bDestroyed = false;
        detailSetDestroyed(detail,true,true,!isdestroyed);

        if(detail->e == e) 
            detail->e = NULL;
        else if(detail->eMount == e) 
            detail->eMount = NULL;
    }
#endif
    return;
}

void detailCleanup(RoomDetail *detail)
{
    detachAuxiliary(NULL,detail);
    detailFlagBaseUpdate(detail);

    if(detail->e)
    {
#ifdef SERVER
        LOG_OLD("basedbg Cleaning up %s\n",detail->e->name);
        detail->e->idDetail = 0;
        detail->e->idRoom = 0;
#endif
        entFree(detail->e);
        detail->e = NULL;
    }
    if(detail->eMount)
    {
#ifdef SERVER
        detail->eMount->idDetail = 0;
        detail->eMount->idRoom = 0;
#endif
        entFree(detail->eMount);
        detail->eMount = NULL;
    }
#if SERVER
    {
        Entity * interacting = baseDetailInteracting(detail);
        if (interacting) 
        { //If someone is using this detail, kick them off
            baseDeactivateDetail(interacting,detail->info->eFunction);
        }
    }
#endif
}

void roomDetailRemove(BaseRoom *room, RoomDetail *detail)
{
    detailCleanup(detail);
    eaFindAndRemove(&room->details, detail);
    destroyRoomDetail( detail );
}

static Entity *roomDetailCreateEntity(BaseRoom *room, RoomDetail *detail, const char *group, bool interact)
{
#if CLIENT
    return NULL;
#elif SERVER
    Entity *ent = NULL;
    const VillainDef* def;
    Mat4 newmat;

    if (!group || !group[0])
    {
        return NULL;
    }
    else
    {
        // Exit out early if the library piece we're looking for doesn't happen to exist
        GroupDef *groupdef = groupDefFindWithLoad( group );
        if( !groupdef )
        {
            Errorf( "The library piece %s is not loaded or doesn't exist, but the detail %s wants to make it.", group, detail->info->pchName );
            return NULL;
        }
    }

    ent = entCreateEx(NULL, ENTTYPE_CRITTER);
    if(!ent)
        return NULL;
    def = villainFindByName(detail->info->pchEntityDef);
    if(!def)
    {
        entFree(ent);
        return NULL;
    }

    if(!villainCreateInPlace(ent, def, detail->info->iLevel, NULL, false, NULL, NULL, 0, NULL))
    {
        entFree(ent);
        return NULL;
    }

    copyMat4(detail->mat, newmat);
    if(detail->info->eSurface == kSurface_Ceiling)
    {
        newmat[3][1] -= 0.05f;
    }
    addVec3(room->pos,newmat[3],newmat[3]);
    ent->motion->input.flying = 1;
    entUpdateMat4Instantaneous(ent, newmat);

    aiInit(ent, NULL);
    aiBehaviorMarkAllFinished(ent,ENTAI(ent)->base, 0);
    if (interact)
    {
        aiBehaviorAddString(ent,ENTAI(ent)->base,detail->info->pchBehavior);
    }
    else
    {
        // Switch to boring mode...
        int index;
        const cCostume *costume = NULL;
        
        npcFindByName("v_base_object_unselectable", &index);
        if (index && (costume = npcDefsGetCostume(index,0)))
        {
            ent->costume = costume;
            svrChangeBody(ent, "v_base_object_unselectable");
            ent->npcIndex = index;
        }

        aiBehaviorAddString(ent,ENTAI(ent)->base,"Invincible,Untargetable,DoNothing");
    }
//    aiBehaviorProcess(ent,ENTAI(ent));

    LOG_DEBUG("basedbg Created %s, changing world group to %s\n",ent->name,group);
    seqChangeDynamicWorldGroup(group, ent); 

    ent->fade = 0;
    ent->idRoom = room->id;
    ent->idDetail = detail->id;

    return ent;
#endif
}

void roomDetailRebuildEntity(BaseRoom *room, RoomDetail *detail)
{
#if SERVER
    static char *pchBadGroupName = "_bases_ground_generic_08";
    static char *pchDarkenFX = "Hardcoded/Pulse/unpowered.fx";
    Entity *e = detail->e;
    Entity *eMount = detail->eMount;
    bool darken = false;
    bool darkenMount = false;

    if (e)
    {
        LOG_DEBUG("basedbg Rebuilding %s\n",e->name);
        e->idDetail = 0;
        e->idRoom = 0;
        entFree(e);
        detail->e = NULL;
    }
    if (eMount)
    {
        eMount->idDetail = 0;
        eMount->idRoom = 0;
        entFree(eMount);
        detail->eMount = NULL;
    }

    if (detail->bDestroyed)
    {
        // Build destroyed version of the detail
        detail->e = roomDetailCreateEntity(room,detail,detail->info->pchGroupNameDestroyed,0);
        
        if (detail->e == NULL)
        {
            darken = true;

            if (!detail->e)
                detail->e = roomDetailCreateEntity(room,detail,detail->info->pchGroupNameUnpoweredMount,0);
            
            if (!detail->e)
                detail->e = roomDetailCreateEntity(room,detail,detail->info->pchGroupNameUnpowered,0);
            
            if (!detail->e)
                detail->e = roomDetailCreateEntity(room,detail,pchBadGroupName,0);
        }
    }
    else if (roomDetailLookUnpowered(detail))
    {
        // Build unpowered version of the detail
        if (detail->info->pchGroupNameUnpowered && 0!=stricmp(detail->info->pchGroupNameUnpowered,"dim"))
        {
            detail->e = roomDetailCreateEntity(room,detail,detail->info->pchGroupNameUnpowered, 1);
        }
        else
        {
            detail->e = roomDetailCreateEntity(room,detail,detail->info->pchGroupName, 1);
            darken = true;
        }
        
        if (!detail->e)
            detail->e = roomDetailCreateEntity(room,detail,pchBadGroupName,1);

        if (detail->info->bMounted) 
        {
            if (detail->info->pchGroupNameUnpoweredMount && 0!=stricmp(detail->info->pchGroupNameUnpoweredMount,"dim"))
            {
                detail->eMount = roomDetailCreateEntity(room,detail,detail->info->pchGroupNameUnpoweredMount, 0);
            }
            else
            {
                detail->eMount = roomDetailCreateEntity(room,detail,detail->info->pchGroupNameMount, 0);
                darkenMount = true;
            }

            if (!detail->eMount)
                detail->eMount = roomDetailCreateEntity(room,detail,pchBadGroupName,0);
        }
    }
    else
    {
        // Build powered version of the detail
        detail->e = roomDetailCreateEntity(room,detail,detail->info->pchGroupName, 1);
        if (!detail->e)
            detail->e = roomDetailCreateEntity(room,detail,pchBadGroupName,1);

        if (detail->info->bMounted)
        {
            detail->eMount = roomDetailCreateEntity(room,detail,detail->info->pchGroupNameMount, 0);
            if (!detail->eMount)
                detail->eMount = roomDetailCreateEntity(room,detail,pchBadGroupName,0);
        }
    }

    // Darken it if needed
    if (darken && detail->e && detail->e->pchar)
    {
        character_PlayMaintainedFX(detail->e->pchar, detail->e->pchar, pchDarkenFX, 
            colorPairNone, 0.0f, PLAYFX_NOT_ATTACHED, PLAYFX_NO_TIMEOUT, PLAYFX_CONTINUING);
    }

    if (darkenMount && detail->eMount && detail->eMount->pchar)
    {
        character_PlayMaintainedFX(detail->eMount->pchar, detail->eMount->pchar, pchDarkenFX, 
            colorPairNone, 0.0f, PLAYFX_NOT_ATTACHED, PLAYFX_NO_TIMEOUT, PLAYFX_CONTINUING);
    }

    detailBehaviorSetToDefault(detail);
    detailBehaviorUpdateActive(detail);
#endif
}


void detailUpdateEntityCostume(RoomDetail *detail, bool rebuild)
{
#if SERVER
    static char *pchDarkenFX = "Hardcoded/Pulse/unpowered.fx";
    Entity *e = detail->e;
    bool bRecostume = !rebuild;

    if (!detail->info|| !detail->info->pchEntityDef) // Not actually a detail that creates an entity
        return;

    if (rebuild)
    {
        // Rebuild the entity from scratch
        BaseRoom *room = e?baseGetRoom(&g_base, e->idRoom):NULL;
        if (!room)
            baseGetDetail(&g_base,detail->id,&room); // Locates the room for us

        if (room)
            roomDetailRebuildEntity(room, detail);
        else
            bRecostume = true;
    }

    if (e && bRecostume)
    {
        // Try for a costume change
        Entity *eMount = detail->eMount;
        bool darken = false;
        bool darkenMount = false;

        if (detail->bDestroyed) 
        {
            LOG_DEBUG("basedbg Recostuming %s to %s\n",e->name,detail->info->pchGroupNameDestroyed);
            seqChangeDynamicWorldGroup(detail->info->pchGroupNameDestroyed, e);
        }
        else if (roomDetailLookUnpowered(detail))
        {
            if(detail->info->pchGroupNameUnpowered && 0!=stricmp(detail->info->pchGroupNameUnpowered,"dim"))
            {
                LOG_DEBUG("basedbg Recostuming %s to %s\n",e->name,detail->info->pchGroupNameUnpowered);
                seqChangeDynamicWorldGroup(detail->info->pchGroupNameUnpowered, e);
            }
            else
            {
                LOG_DEBUG("basedbg Recostuming %s to %s\n",e->name,detail->info->pchGroupName);
                seqChangeDynamicWorldGroup(detail->info->pchGroupName, e);
                darken = true;
            }
            if (eMount)
            {
                if(detail->info->pchGroupNameUnpoweredMount && 0!=stricmp(detail->info->pchGroupNameUnpoweredMount,"dim"))
                {
                    seqChangeDynamicWorldGroup(detail->info->pchGroupNameUnpoweredMount, eMount);
                }
                else
                {
                    seqChangeDynamicWorldGroup(detail->info->pchGroupNameMount, eMount);
                    darkenMount = true;
                }
            }
        }
        else
        {
            LOG_DEBUG("basedbg Recostuming %s to %s\n",e->name,detail->info->pchGroupName);
            seqChangeDynamicWorldGroup(detail->info->pchGroupName, e);
            if (eMount) 
            {
                seqChangeDynamicWorldGroup(detail->info->pchGroupNameMount, eMount);
            }
        }

        // (un)Darken it if needed
        if (e->pchar)
        {
            entity_KillMaintainedFX(e,e,pchDarkenFX);
            if(darken)
            {
                character_PlayMaintainedFX(e->pchar, e->pchar, pchDarkenFX, 
                    colorPairNone, 0.0f, PLAYFX_NOT_ATTACHED, PLAYFX_NO_TIMEOUT, PLAYFX_CONTINUING);
            }
        }

        if(eMount && eMount->pchar)
        {
            entity_KillMaintainedFX(eMount,eMount,pchDarkenFX);
            if(darkenMount)
            {
                character_PlayMaintainedFX(eMount->pchar, eMount->pchar, pchDarkenFX, 
                    colorPairNone, 0.0f, PLAYFX_NOT_ATTACHED, PLAYFX_NO_TIMEOUT, PLAYFX_CONTINUING);
            }
        }
    }
#endif
}

#if SERVER
//For cathedral of pain...it may be OK for other supergroup missions
bool SupergroupIsOnSupergroupMission( Supergroup * sg )
{
    if( sg && sg->activetask && sg->activetask->missionMapId)
        return true;

    return false;
}
#endif //SERVER

bool playerCanModifyBase( Entity *e, Base *base )
{ 
    if (!e)
        return false;
    if( e->access_level )
        return true;

    if(base->user_id == e->db_id)
        return true;

    if    (!e->supergroup_id                                                // in supergroup
             || !base->rooms                                            // base not loaded
              || !sgroup_hasPermission(e, SG_PERM_BASE_EDIT)     // has the appropriate permission 
             || e->supergroup_id != base->supergroup_id                    // This map belongs to the supergroup in question
#if CLIENT
             || game_state.base_raid
             || game_state.no_base_edit
#elif SERVER 
             || RaidIsRunning()
             || server_state.no_base_edit
             || SupergroupIsOnSupergroupMission( e->supergroup )
             || RaidSGIsAttacking( e->supergroup_id )
#endif
        )
    {
        return false;
    }

    return true;
}


bool playerCanPlacePersonal( Entity * e , Base * base )
{

    if (!e)
        return false;
    if( e->access_level )
        return true;
    if(base->user_id == e->db_id)
        return true;
    if (!e->supergroup_id
            || !base->rooms                                            // base not loaded
            || e->supergroup_id != base->supergroup_id  // This map belongs to the supergroup in question
#if CLIENT
            || game_state.base_raid
            || game_state.no_base_edit
#elif SERVER 
            || RaidIsRunning()
            || server_state.no_base_edit
            || SupergroupIsOnSupergroupMission( e->supergroup )
            || RaidSGIsAttacking( e->supergroup_id )
#endif
        )  
        return false;

    // BASE_TODO: Make sure this base belongs to supergroup
    return true;
}


#if SERVER
bool baseFixBaseIfBroken(Base *base)
{
    int i;
    bool bHasEntrance;

    if (!group_info.file_count) // file-less defs go to group_info.files[0]
        groupFileNew();

    // If there's no plot, then give up.
    if(!base->plot)
    {
        world_modified=1;
        baseReset(base);
        baseCreateDefault(base);
        return true;
    }

    for(i=0; i<eaSize(&base->rooms); i++)
    {
        if(!base->rooms[i] || !base->rooms[i]->info)
        {
            eaRemove(&base->rooms, i);
            i--;
            continue;
        }
        else
        {
            int j;
            for(j=0; j<eaSize(&base->rooms[i]->details); j++)
            {
                RoomDetail * pDetail = base->rooms[i]->details[j];
                if(!pDetail || !pDetail->info)
                {
                    eaRemove(&base->rooms[i]->details, j);
                    j--;
                    continue;
                }
                // Fix up permissions the first time we load a base.
                if (pDetail->info->bIsStorage && pDetail->permissions == 0)
                {
                    pDetail->permissions = STORAGE_BIN_DEFAULT_PERMISSIONS;
                }

                // Don't allow floating stairs
                //if(pDetail->info->bDoNotBlock && !detailCanFit(base->rooms[i],pDetail,0))
                //    roomDetailRemove(base->rooms[i], pDetail);
            }
        }

        if(stricmp(base->rooms[i]->info->pchRoomCat, "Entrance")==0)
            bHasEntrance = true;
    }

    if(!bHasEntrance)
    {
        world_modified=1;
        baseReset(base);
        baseCreateDefault(base);
        return true;
    }

    return false;
}
extern bool g_bBaseUpdateAux;
extern bool g_bBaseUpdateEnergy;
extern bool g_bBaseUpdateControl;
void baseForceUpdate(Base *base)
{
    int i;
    g_bBaseUpdateAux = true;
    g_bBaseUpdateControl = true;
    g_bBaseUpdateEnergy = true;

    for(i=0; i<eaSize(&base->rooms); i++)
    {
        if(!base->rooms[i] || !base->rooms[i]->info)
        {
            continue;
        }
        else
        {
            int j;
            for(j=0; j<eaSize(&base->rooms[i]->details); j++)
            {
                RoomDetail *det = base->rooms[i]->details[j];                
                Mat4 door_pos;                
                if(!det || !det->info)
                {
                    continue;
                }
                det->parentRoom = base->rooms[i]->id;
                copyMat4( unitmat, door_pos ); //Construct a real matrix from room + detail
                addVec3( base->rooms[i]->pos, det->mat[3], door_pos[3] );
                updateAllBaseDoorTypes( det, door_pos );
            }
        }

    }

}
#endif

// *********************************************************************************
// storage items
// *********************************************************************************

StoredSalvage* storedsalvage_Create()
{
    return ParserAllocStruct(sizeof(StoredSalvage));
}

void storedsalvage_Destroy(StoredSalvage *item)
{
    if( item )
    {
        ParserFreeStruct(item);
    }
}

StoredInspiration* storedinspiration_Create()
{
    return ParserAllocStruct(sizeof(StoredInspiration));
}

void storedinspiration_Destroy(StoredInspiration *item)
{
    if( item )
    {
        ParserFreeStruct(item);
    }
}

StoredEnhancement* storedenhancement_Create(const BasePower *ppowBase, int iLevel, int iNumCombines, int amount)
{
    StoredEnhancement *s = ParserAllocStruct(sizeof(StoredEnhancement));
    if(s)
    {
        eaPush(&s->enhancement,ParserAllocStruct(sizeof(*s->enhancement[0])));
        s->enhancement[0]->ppowBase = ppowBase;
        s->enhancement[0]->iLevel = iLevel;
        s->enhancement[0]->iNumCombines = iNumCombines;
        s->amount = amount;
    }
    return s;
}

void storedenhancement_Destroy(StoredEnhancement *item)
{
    if( item )
    {
        ParserFreeStruct(item);
    }
}


// *********************************************************************************
// Ring-buf of log msgs 
// *********************************************************************************

#define BASELOG_N_MSGS (1024)

void BaseAddLogMsgv(char *msg, va_list ap)
{
    static char *lm = NULL;
    if(lm)
        estrClear(&lm);
    else
        estrCreate( &lm );
    estrConcatfv(&lm, msg, ap);

    while( eaSize( &g_base.baselogMsgs ) >= BASELOG_N_MSGS )
    {
        ParserFreeString(g_base.baselogMsgs[0]);
        eaRemove(&g_base.baselogMsgs, 0);
    }
    eaPush(&g_base.baselogMsgs, ParserAllocString(lm)); // need parser mem for this
}

void BaseAddLogMsg(char *msg, ...)
{
     va_list ap; 
    va_start(ap, msg);
    BaseAddLogMsgv(msg, ap);
    va_end(ap);
}