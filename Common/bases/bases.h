#ifndef _BASES_H
#define _BASES_H

#include <utilitieslib/stdtypes.h>
#include "entity/gametypes.h"
#include <utilitieslib/utils/Color.h>
#include <utilitieslib/utils/SuperAssert.h>
#include "bases/baseRaid.h"
#include "utilitieslib/components/StashTable.h"
#include "bases/basedata.h"

#define ROOM_SUBBLOCK_SIZE 16
#define BASE_BLOCK_SIZE 32
#define APARTMENT_BLOCK_SIZE 16
#define ROOM_MAX_HEIGHT 48
#define DETAIL_BLOCK_FEET 1
#define BASE_FORCED_LEVEL 50
#define BASE_FREE_ACCESS 3

typedef    struct GroupDef    GroupDef;
typedef    struct DefTracker    DefTracker;
typedef    struct RoomTemplate    RoomTemplate;
typedef    struct Detail        Detail;
typedef    struct DetailCategory DetailCategory;
typedef    struct Entity        Entity;
typedef struct BasePlot        BasePlot;
typedef struct BaseRoom        BaseRoom;
typedef struct SalvageItem SalvageItem;
typedef struct BasePower BasePower;
typedef struct Boost Boost;

typedef enum BaseEditType
{
    kBaseEdit_None,
    kBaseEdit_Architect,
    kBaseEdit_AddPersonal,
    kBaseEdit_Plot,
}BaseEditType;

typedef struct BaseBlock
{
    F32        height[2];
    U8        replaced[2];
} BaseBlock;

typedef struct DetailBlock
{
    S8        height[2];
    U8        floor_height;
    U8        connected : 1;
    U8        blocked : 1;
    U8        checked : 1;
    U8        open : 1;
    U8        zgrow : 1;
    U8        right : 1;
} DetailBlock;

typedef struct DetailSwap
{
    char   original_tex[128];
    char   replace_tex[128];
} DetailSwap;

// ab: decided not to use SalvageInventoryItem as 
// StoredSalvage items live on the parse heap
typedef struct StoredSalvage
{
    const SalvageItem *salvage;
    S32 amount;
    char nameEntFrom[MAX_NAME_LEN];
} StoredSalvage;
StoredSalvage* storedsalvage_Create();
void storedsalvage_Destroy(StoredSalvage *item);

typedef struct StoredInspiration
{
    const BasePower *ppow;
    S32 amount;
    char nameEntFrom[MAX_NAME_LEN];    
} StoredInspiration;
StoredInspiration* storedinspiration_Create();
void storedinspiration_Destroy(StoredInspiration *item);

typedef struct StoredEnhancement
{
    Boost **enhancement; // only 1, earray for textparser reasons
    S32 amount;
    char nameEntFrom[MAX_NAME_LEN];    
} StoredEnhancement;
StoredEnhancement* storedenhancement_Create(const BasePower *ppowBase, int iLevel, int iNumCombines, int amount);
void storedenhancement_Destroy(StoredEnhancement *item);

// This defaults to red star and gold star leaders only, to avoid theft problems
#define STORAGE_BIN_DEFAULT_PERMISSIONS 0x00700070
// Minimum permissions are red star leader and GM's can take and put - this prevents these ever being turned off 
#define STORAGE_BIN_MINIMUM_PERMISSIONS 0x00600060

typedef struct RoomDetail
{
    Mat4    mat;
    int        id;
    char    creator_name[128];
    U32        creation_time;

    // Used for texture and color swapping
    Color  tints[2];
    DetailSwap **swaps;

    // Extra stuff for game systems
    const Detail *info;

    // Unpersisted info
    Entity *e;                        // If this detail is created as an entity, this points to the entity.
    Entity *eMount;                    // Points to the secondary entity used for turret mounts.
    int iAuxAttachedID;                // The detail ID to which this detail is auxiliary

    U8 bDestroyed;
    U8 bPowered;
    U8 bUsingBattery; // Using a battery for power (or, if this is a battery, if it's in use)
    U8 bControlled;
    U8 bActiveAux;    // Is this aux item currently applying itself
    U8 bActiveBehavior; // Is this detail running its primary behavior (vs turned off)
        // The various on/off states of the detail.

    int iBonusEnergy;
    int iBonusControl;                // Bonus energy or control being supplied to this detail via aux items

    float fLifetimeLeft;            // Length of time this detail has before it dies.

    bool bAuxTick;                    // True if this detail is an aux detail that needs to be handled during the auxiliary item tick

    int parentRoom;                    // Backreference to room this detail is in

    int invID;                        // The inventory ID of this detail, if was created from personal detail

    U32 permissions;                // Permission bits for this item.

    StoredSalvage **storedSalvage;    // salvage that is stored in this detail.

    StoredInspiration **storedInspiration;    // inspiration that is stored in this detail.

    StoredEnhancement **storedEnhancement;    // enhancement that is stored in this detail.

    // deprecated. taking up too much room in db
//     char **storageLog;
        // log of changes made to this by players
} RoomDetail;

typedef enum
{
    ROOMDECOR_FLOOR_0        = 0,
    ROOMDECOR_FLOOR_4        = 1,
    ROOMDECOR_FLOOR_8        = 2,
    ROOMDECOR_CEILING_32    = 3,
    ROOMDECOR_CEILING_40    = 4,
    ROOMDECOR_CEILING_48    = 5,
    ROOMDECOR_WALL_LOWER_4    = 6,
    ROOMDECOR_WALL_LOWER_8    = 7,
    ROOMDECOR_WALL_16        = 8,
    ROOMDECOR_WALL_32        = 9,
    ROOMDECOR_WALL_48        = 10,
    ROOMDECOR_WALL_UPPER_32    = 11,
    ROOMDECOR_WALL_UPPER_40    = 12,
    ROOMDECOR_LOWERTRIM        = 13,
    ROOMDECOR_UPPERTRIM        = 14,
    ROOMDECOR_DOORWAY        = 15,
    ROOMDECOR_MAXTYPES,
} RoomDecor;

#define ROOMDECOR_ALL ROOMDECOR_MAXTYPES

typedef struct RoomDoor
{
    int        pos[2];
    F32        height[2];
} RoomDoor;

typedef struct
{
    RoomDecor    type;
    char        geo[128];
    char        tex[128];
    Color        tints[2];
} DecorSwap;

typedef struct BaseRoom
{
    Vec3        pos;
    int            id;
    BaseBlock    **blocks;
    int            size[2];
    GroupDef    *def;
    int            blockSize;
    RoomDoor    **doors;
    DecorSwap    **swaps;
    Color        **lights;
    GroupDef    *decor_defs[ROOMDECOR_MAXTYPES*2];
    GroupDef    *light_defs[3];
    RoomDetail    **details;
    U32            mod_time;
    DetailBlock    *detail_blocks;
//    int            detail_size[2];
    char        name[128];
    const RoomTemplate*info;
    U32            tagged : 1;
} BaseRoom;

typedef struct
{
    U8        *zip_data;
    U32        pack_size;
    U32        unpack_size;
} ZipData;

typedef struct Base
{
    int            version;
    BaseRoom    **rooms;
    GroupDef    *def;
    DefTracker    *ref;
    int            curr_id;
    int            roomBlockSize;
    int            supergroup_id;
    int            user_id; // for apartments
    
    // Saved and sent to client for display purposes
    int iEnergyProUI;
    int iEnergyConUI;
    int iControlProUI;
    int iControlConUI;

    char        *data_str;
    int            data_len;

#if SERVER
    U32            mod_time;
    ZipData        delta;
    ZipData        full;
    U32            timeRanDbQueryForSgrp;
        // when the base last queried the db for the
        // supergroup. needed for when no group members are online to
        // fetch the supergroup from.
    int            nUpkeepsLate; 
        // how many upkeep periods late the base currently thinks it is.
    U32            lastCRC;
    BOOL        base_prestige_balanced; // does the value of the base match the value of its items.
#elif CLIENT
    int            update_time;
#endif

    U32            *room_mod_times;
    const BasePlot    *plot;
    Vec3        plot_pos;
    int            plot_width;
    int            plot_height;

    GroupDef    *plot_def;
    GroupDef    *sound_def;
    char	music[50];		// Background music currently playing.
	unsigned int	musicTrack;	// Used to update entities without doing a string comparison on each one.

    char **baselogMsgs;
} Base;

typedef enum
{
    BASENET_ROOM,
    BASENET_ROOM_MOVE,
    BASENET_ROOM_DELETE,
    BASENET_ROOMHEIGHT,
    BASENET_TEXSWAP,
    BASENET_GEOSWAP,
    BASENET_TINT,
    BASENET_LIGHT,
    BASENET_DETAILS,
    BASENET_DETAIL_DELETE,
    BASENET_DETAIL_MOVE,
    BASENET_DOORWAY,
    BASENET_DOORWAY_DELETE,
    BASENET_PLOT,
    BASENET_MODE,
    BASENET_BASESTYLE,
    BASENET_BASELIGHTING,
    BASENET_BASETINT,
    BASENET_PERMISSIONS,
} BaseNetCmd;

typedef struct BasePart
{
    int                room_idx;
    Mat4            mat;
    F32                height;
    char            name[128];
    char            swapto[128];
}  BasePart;

extern Base    g_base;

// Used in base raid mapping
#define checkMinMax(x, z) ( \
    (minx = (x) < minx ? (x) : minx), \
    (minz = (z) < minz ? (z) : minz), \
    (maxx = (x) + 1 > maxx ? (x) + 1 : maxx), \
    (maxz = (z) + 1 > maxz ? (z) + 1 : maxz))

extern char *roomdecor_names[ROOMDECOR_MAXTYPES];

typedef struct baseGoupNameList {
    char ** names;
}baseGoupNameList;
extern baseGoupNameList baseReferenceLists[ROOMDECOR_MAXTYPES+1];

int baseFindDecorIdx(const char *pchGeo);
void baseAddReferenceName( char * name );
void baseMakeAllReferenceArray(void);
void roomAllocBlocks(BaseRoom *room);

int basePosToGrid(int size[2],F32 feet,Vec3 pos,int xz[2],int big_end,int clamp);

DecorSwap *decorSwapFind(BaseRoom *room,RoomDecor idx);
DecorSwap *decorSwapFindOrAdd(BaseRoom *room,RoomDecor idx);

DetailSwap *detailSwapFind(RoomDetail *detail,char original[128]);
DetailSwap *detailSwapSet(RoomDetail *detail, char original[128], char replaced[128]);

BaseRoom *baseGetRoom(Base *base,int room_idx);
BaseRoom *roomUpdateOrAdd(int room_id,int size[2],int rotation,Vec3 pos,F32 height[2], const RoomTemplate *info);
void baseSetIllegalHeight(BaseRoom *room,int idx);
int baseIsLegalHeight(F32 lower,F32 upper);
void baseClearRoom(BaseRoom *room,F32 height[2]);
void baseLoadUiDefs();
int baseGetDecorIdx(char *partname,int height,RoomDecor *idx);
int baseRoomIdxFromPos(BaseRoom *room,Vec3 pos,int *xp,int *zp);
int roomPosFromIdx(BaseRoom *room,int idx,Vec3 pos);
RoomDetail *roomGetDetail(BaseRoom *room,int id);
BaseBlock *roomGetBlock(BaseRoom *room, int block[2]);

RoomDetail *baseGetDetail(Base *base, int id, BaseRoom **out_room);

BaseRoom *baseRoomLocFromPos(Base *base, const Vec3 pos, int block[2]);

int getRoomBlockFromPos(BaseRoom *room, Vec3 pos, int block[2]);

RoomDetail *roomGetDetailFromPos(BaseRoom *room,Vec3 pos);
RoomDetail *roomGetDetailFromWorldPos(BaseRoom *room,const Vec3 pos);
RoomDetail *roomGetDetailFromWorldMat(BaseRoom *room, const Mat4 mat);


int roomDoorwayHeight(BaseRoom *room,int block[2],F32 heights[2]);
void roomGetBlockHeight(BaseRoom *room,int block_idx[2],F32 heights[2]);

int baseDetailIsPillar(const Detail *detail);
int baseDetailExtents(const Detail *detail,Mat4 mat,Vec3 min,Vec3 max);
int baseDetailVolumeTriggerExtents(const Detail *detail,Mat4 mat,Vec3 min,Vec3 max);

enum
{
    kDetErr_NoErr,
    kDetErr_NoExtents,
    kDetErr_BadPillarFit,
    kDetErr_UsedBlock,
    kDetErr_NoSteps,
    kDetErr_NoStack,
    kDetErr_Intersection,
    kDetErr_BlocksDoors,
    kDetErr_BlocksDetail,
};

enum
{
    kBaseErr_NoErr,
    kBaseErr_NotConnected,
    kBaseErr_PlotTooSmall,
    kBaseErr_PlotRestrictions,
    kBaseErr_BlocksDoors,
    kBaseErr_BlocksDetail,
};


int detailCanFit(BaseRoom *room,RoomDetail *detail, int *error);

int detailFindIntersecting(BaseRoom *room,Vec3 box_min,Vec3 box_max,RoomDetail *isect_details[],int max_count);
int detailSupportFindContacts(BaseRoom *room,RoomDetail *detail,RoomDetail *isect_details[],int max_count);
int detailCountInRoom( const char * pchCategory, BaseRoom * room );
int detailCountInBase( const char *pchCat, Base * base );
bool detailAllowedInRoom( BaseRoom *room, RoomDetail * detail, int new_room );
bool detailAllowedInBase( Base * base, RoomDetail * detail );
int baseDetailLimit( Base * base, char * pchCat );
int basePlotAllowed( Base * base, const BasePlot * plot );

int roomHeightIntersecting(BaseRoom *room,int block[2],F32 height[2],RoomDetail *isect_details[],int max_count);

void entFreeBase(Entity *e);
void detailCleanup(RoomDetail *detail);
void roomDetailRemove(BaseRoom *room, RoomDetail *detail);

void roomDetailRebuildEntity(BaseRoom *room, RoomDetail *detail);
void detailUpdateEntityCostume(RoomDetail *detail, bool rebuild);

bool playerCanModifyBase( Entity *e, Base * base );
    // used to see if player is allowed to change base

bool playerCanPlacePersonal( Entity * e, Base * base );

bool baseFixBaseIfBroken(Base *base);
void baseForceUpdate(Base *base);

void BaseAddLogMsgv(char *msg, va_list ap);
void BaseAddLogMsg(char *msg, ...);

void sgRaidConvertRaidBasesToMapData();
int sgRaidGetTeamNumber(Entity *e);

#define isBaseMap() (g_base.rooms != NULL)

#endif
