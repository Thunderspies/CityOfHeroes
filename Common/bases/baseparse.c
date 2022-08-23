#include "bases/bases.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include "bases/baseparse.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/assert/assert.h>
#include "bases/basedata.h"
#include "entity/salvage.h"
#include "entity/boost.h"

ParseLink g_base_roomInfoLink = {
    (void***)&g_RoomTemplateDict.ppRooms,
    {
        { offsetof(RoomTemplate,pchName), 0 },
    }
};

ParseLink g_base_detailInfoLink = {
    (void***)&g_DetailDict.ppDetails,
    {
        { offsetof(Detail,pchName), 0 },
    }
};

ParseLink g_base_BasePlotLink = {
    (void***)&g_BasePlotDict.ppPlots,
    {
        { offsetof(BasePlot,pchName), 0 },
    }
};

static TokenizerParseInfo parse_detailswap[] = {
    { "Geo",            TOK_FIXEDSTR(DetailSwap,original_tex)},
    { "Tex",            TOK_FIXEDSTR(DetailSwap,replace_tex)},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_storedsalvage[] = {
    { "Salvage",        TOK_LINK(StoredSalvage,salvage,0,&g_salvageInfoLink) },
    { "Amount",            TOK_INT(StoredSalvage,amount,0)},
    { "NameEntFrom",    TOK_FIXEDSTR(StoredSalvage,nameEntFrom)},    
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};
STATIC_ASSERT(SIZEOF2(StoredSalvage,nameEntFrom) <= 128)

extern ParseLink g_basepower_InfoLink;
static TokenizerParseInfo parse_storedinspiration[] = {
    { "Power",            TOK_LINK(StoredInspiration,ppow,0,&g_basepower_InfoLink) },
    { "Amount",            TOK_INT(StoredInspiration,amount,0)},
    { "NameEntFrom",    TOK_FIXEDSTR(StoredInspiration,nameEntFrom)},    
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};
STATIC_ASSERT(SIZEOF2(StoredInspiration,nameEntFrom) <= 128);

static TokenizerParseInfo parse_storedBoost[] = 
{
    { "BasePower",        TOK_LINK(Boost,ppowBase,0,&g_basepower_InfoLink) },
    { "Level",            TOK_INT(Boost,iLevel,0)},    
    { "NumCombines",    TOK_INT(Boost,iNumCombines,0)},    
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_storedEnhancement[] = {
    { "Enhancement",    TOK_STRUCT(StoredEnhancement,enhancement,parse_storedBoost) },
    { "Amount",            TOK_INT(StoredEnhancement,amount,0)},
    { "NameEntFrom",    TOK_FIXEDSTR(StoredEnhancement,nameEntFrom)},    
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};
STATIC_ASSERT(SIZEOF2(StoredEnhancement,nameEntFrom) <= 128)

static TokenizerParseInfo parse_roomdetail[] = {
    { "Pos",            TOK_VEC3(RoomDetail,mat[3])},
    { "Pyr",            TOK_MATPYR(RoomDetail,mat)},
    { "Info",            TOK_LINK(RoomDetail,info,0,&g_base_detailInfoLink) },
    { "Id",                TOK_INT(RoomDetail,id,0)},
    { "Creator",        TOK_FIXEDSTR(RoomDetail,creator_name)},
    { "CreationTime",    TOK_INT(RoomDetail,creation_time,0)},
    { "Tint1",            TOK_INT(RoomDetail,tints[0].integer,0)},
    { "Tint2",            TOK_INT(RoomDetail,tints[1].integer,0)},
    { "DetailSwap",        TOK_STRUCT(RoomDetail,swaps,    parse_detailswap)},
    { "AuxID",            TOK_INT(RoomDetail,iAuxAttachedID,0)},
    { "Powered",        TOK_U8(RoomDetail,bPowered,0)},
    { "UsingBattery",    TOK_U8(RoomDetail,bUsingBattery,0)},
    { "Controlled",        TOK_U8(RoomDetail,bControlled,0)},
    { "ParentID",        TOK_INT(RoomDetail,parentRoom,0)},
    { "StoredSalvage",    TOK_STRUCT(RoomDetail,storedSalvage, parse_storedsalvage)},
    { "StoredInspiration",    TOK_STRUCT(RoomDetail,storedInspiration, parse_storedinspiration) },
    { "StoredEnhancement",    TOK_STRUCT(RoomDetail,storedEnhancement, parse_storedEnhancement) },
    { "storagelog",        TOK_DEPRECATED    },
    { "Permissions",    TOK_INT(RoomDetail,permissions,0)},

    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_decorswap[] = {
    { "Type",            TOK_INT(DecorSwap,type,0)},
    { "Geo",            TOK_FIXEDSTR(DecorSwap,geo)},
    { "Tex",            TOK_FIXEDSTR(DecorSwap,tex)},
    { "Tint1",            TOK_INT(DecorSwap,tints[0].integer,0)},
    { "Tint2",            TOK_INT(DecorSwap,tints[1].integer,0)},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_block[] = {
    { "Floor",            TOK_F32(BaseBlock,height[0],0)},
    { "Ceiling",        TOK_F32(BaseBlock,height[1],0)},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_roomdoor[] = {
    { "BlockX",            TOK_INT(RoomDoor,pos[0],0)},
    { "BlockZ",            TOK_INT(RoomDoor,pos[1],0)},
    { "Floor",            TOK_F32(RoomDoor,height[0],0)},
    { "Ceiling",        TOK_F32(RoomDoor,height[1],0)},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_light[] = {
    { "Color",            TOK_INT(Color,integer,0)},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

static TokenizerParseInfo parse_room[] = {
    { "Id",                TOK_INT(BaseRoom,id,0)},
    { "Pos",            TOK_VEC3(BaseRoom,pos)},
    { "Width",            TOK_INT(BaseRoom,size[0],0)},
    { "Height",            TOK_INT(BaseRoom,size[1],0)},
    { "BlockSize",        TOK_INT(BaseRoom,blockSize,0)},
    { "ModTime",        TOK_INT(BaseRoom,mod_time,0)},
    { "Info",            TOK_LINK(BaseRoom,info,0,&g_base_roomInfoLink)},
    { "Detail",            TOK_STRUCT(BaseRoom,details,parse_roomdetail)},
    { "DecorSwap",        TOK_STRUCT(BaseRoom,swaps,parse_decorswap)},
    { "Name",            TOK_FIXEDSTR(BaseRoom,name)},
    { "Light",            TOK_STRUCT(BaseRoom,lights,parse_light)},
    { "Door",            TOK_STRUCT(BaseRoom,doors,parse_roomdoor)},
    { "Block",            TOK_STRUCT(BaseRoom,blocks,parse_block)},
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

TokenizerParseInfo parse_base[] = {
    { "Base",            TOK_START,        0},
    { "Version",        TOK_INT(Base,version,0) },
    { "RoomBlockSize",    TOK_INT(Base,roomBlockSize,0) },
    { "SupergroupId",    TOK_INT(Base,supergroup_id,0) },
    { "UserId",            TOK_INT(Base,user_id,0) },
    { "EnergyProduced",    TOK_INT(Base,iEnergyProUI,0) },
    { "EnergyConsumed",    TOK_INT(Base,iEnergyConUI,0) },
    { "ControlProduced",TOK_INT(Base,iControlProUI,0) },
    { "ControlConsumed",TOK_INT(Base,iControlConUI,0) },
    { "Room",            TOK_STRUCT(Base,rooms,parse_room) },
    { "PlotPos",        TOK_VEC3(Base,plot_pos)},
    { "Plot",            TOK_LINK(Base,plot,0,&g_base_BasePlotLink)},
    { "PlotWidth",        TOK_INT(Base,plot_width,0) },
    { "PlotHeight",        TOK_INT(Base,plot_height,0) },
    { "baselog",        TOK_STRINGARRAY(Base,baselogMsgs)    },
    { "End",            TOK_END,            0},
    { "", 0, 0 }
};

void baseResetIds(Base *base)
{
    int        i,j;

    base->curr_id = 0;
    for(i=0;i<eaSize(&base->rooms);i++)
        base->rooms[i]->id = ++base->curr_id;
    for(i=0;i<eaSize(&base->rooms);i++)
    {
        BaseRoom    *room = base->rooms[i];

        for(j=0;j<eaSize(&room->details);j++)
            room->details[j]->id = ++base->curr_id;
    }
}

void baseResetIdsForBaseRaid(Base *base, int initial_id)
{
    int        i,j;

    base->curr_id = initial_id;
    for(i=0;i<eaSize(&base->rooms);i++)
        base->rooms[i]->id = ++base->curr_id;
    for(i=0;i<eaSize(&base->rooms);i++)
    {
        BaseRoom    *room = base->rooms[i];

        for(j=0;j<eaSize(&room->details);j++)
            room->details[j]->id = ++base->curr_id;
    }
}

int baseToStr(char **estr,Base *base)
{
    base->version = 1;
    return ParserWriteText(estr,parse_base,base, 0, 0);
}

int baseFromStr(char *str,Base *base)
{
    return ParserReadText(str,-1,parse_base,base);
}

int baseToBin(char **estr,Base *base)
{
    base->version = 1;
    return ParserWriteBin(estr,parse_base,base, 0, 0);
}

int baseFromBin(char *bin,U32 num_bytes,Base *base)
{
    return ParserReadBin(bin,num_bytes,parse_base,base);
}

static const Detail *basedata_FindDetail(char *fullname)
{
    return (const Detail *)ParserLinkFromString(&g_base_detailInfoLink,fullname);
}

int baseToFile(char *fnamep,Base *base)
{
    int        len;
    char    *estr = 0;
    FILE    *file;
    char    fname[MAX_PATH];

    changeFileExt(fnamep,".base",fname);
    baseToStr(&estr,base);
    len = estrLength(&estr);
    file = fileOpen(fname,"wb");
    if (!file)
        return 0;
    fwrite(estr,len,1,file);
    fclose(file);
    return len;
}

int baseFromFile(char *fname,Base *base)
{
    char    *mem;
    int        ok;

    mem = fileAlloc(fname,0);
    if (!mem)
        return 0;
    ParserDestroyStruct(parse_base,base);
    ok = baseFromStr(mem,base);
    baseResetIds(base);
    base->curr_id++;
    free(mem);
    return ok;
}

void rebuildBaseFromStr( Base * base )
{
    ParserDestroyStruct(parse_base,base);
    baseFromBin(base->data_str,base->data_len,base);
}


#include "group/group.h"
void baseReset(Base *base)
{
    int        i;

    ParserDestroyStruct(parse_base,base);
    memset(base,0,sizeof(*base));
    for(i=0;i<group_info.file_count;i++)
        group_info.files[i]->base = 0;
}

void destroyRoomDetail( RoomDetail * detail )
{
    ParserDestroyStruct( parse_roomdetail, detail );
}

void destroyRoom( BaseRoom * room )
{
    ParserDestroyStruct( parse_room, room );
}
