#include "bases/bases.h"
#include "bases/baseparse.h"
#include "bases/basesend.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "bases/basedata.h"
#include <utilitieslib/network/bindiff.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/network/crypt.h>

static void sendRoomHeader(Packet *pak,BaseRoom *room,BaseNetCmd cmd,int idx)
{
    pktSendBitsPack(pak,1,cmd);
    pktSendBitsPack(pak,1,room->id);
    pktSendBitsPack(pak,1,idx);
}

void baseSendHeight(Packet *pak,BaseRoom *room, int type, int idx,F32 floor,F32 ceiling)
{
    pktSendBitsPack(pak,1,BASENET_ROOMHEIGHT);
    pktSendBitsPack(pak,1,type);

    if( type != kBaseSend_Base )
    {
        pktSendBitsPack(pak,1,room->id);
        if( type == kBaseSend_Single )
            pktSendBitsPack(pak,1,idx);
    }
    pktSendF32Comp(pak,floor);
    pktSendF32Comp(pak,ceiling);
}

void baseSendTexSwap(Packet *pak,BaseRoom *room,int idx,char *texname)
{
    sendRoomHeader(pak,room,BASENET_TEXSWAP,idx);
    pktSendString(pak,texname);
}

void baseSendGeoSwap(Packet *pak,BaseRoom *room,int idx,char *style)
{
    sendRoomHeader(pak,room,BASENET_GEOSWAP,idx);
    pktSendString(pak,style);
}

void baseSendTint(Packet *pak,BaseRoom *room,int idx,Color *colors)
{
    sendRoomHeader(pak,room,BASENET_TINT,idx);
    pktSendBits(pak,32,colors[0].integer);
    pktSendBits(pak,32,colors[1].integer);
}

void baseSendLight(Packet *pak,BaseRoom *room,int idx,Color light_color)
{
    sendRoomHeader(pak,room,BASENET_LIGHT,idx);
    pktSendBits(pak,32,light_color.integer);
}

void baseSendDoorwayDelete(Packet *pak,BaseRoom *room,int pos[2])
{
    sendRoomHeader(pak,room,BASENET_DOORWAY_DELETE,0);
    pktSendBitsPack(pak,1,pos[0]);
    pktSendBitsPack(pak,1,pos[1]);
}

void baseSendDoorway(Packet *pak,BaseRoom *room,int pos[2],F32 height[2])
{
    sendRoomHeader(pak,room,BASENET_DOORWAY,0);
    pktSendBitsPack(pak,1,pos[0]);
    pktSendBitsPack(pak,1,pos[1]);
    pktSendF32Comp(pak,height[0]);
    pktSendF32Comp(pak,height[1]);
}

void baseSendDetailDelete(Packet *pak, BaseRoom *room,RoomDetail *detail)
{
    sendRoomHeader(pak, room, BASENET_DETAIL_DELETE, detail->id);
}

void baseSendDetailMove(Packet *pak, BaseRoom *room,RoomDetail *detail, BaseRoom *newroom)
{
    if (newroom == NULL)
        newroom = room;
    pktSendBitsPack(pak,1,BASENET_DETAIL_MOVE);
    pktSendBitsPack(pak,1,room->id);
    pktSendBitsPack(pak,1,newroom->id);
    pktSendBitsPack(pak,1,detail->id);
    sendMat4(pak,detail->mat);
}

void baseSendDetails(Packet *pak,BaseRoom *room,RoomDetail **details,int count,int reset)
{
    int            i;
    RoomDetail    *detail;

    pktSendBitsPack(pak,1,BASENET_DETAILS);
    pktSendBitsPack(pak,1,room->id);
    pktSendBitsPack(pak,1,count);
    pktSendBits(pak,1,reset);
    for(i=0;i<count;i++)
    {
        detail = details[i];
        pktSendBitsPack(pak,1,detail->id);
        pktSendBitsPack(pak,1,detail->invID);
        pktSendBitsPack(pak,1,detail->permissions);
        sendMat4(pak,detail->mat);

        if(detail->info)
        {
            pktSendString(pak,detail->info->pchName);
        }
        else
        {
            pktSendString(pak,"");
        }
    }
}

void baseSendPermissions(Packet *pak,BaseRoom *room,RoomDetail *detail)
{
    pktSendBitsPack(pak,1,BASENET_PERMISSIONS);
    pktSendBitsPack(pak,1,room->id);
    pktSendBitsPack(pak,1,detail->id);
    pktSendBitsPack(pak,1,detail->permissions);
    if(detail->info)
    {
        pktSendString(pak,detail->info->pchName);
    }
    else
    {
        pktSendString(pak,"");
    }
}
#if SERVER

static void baseCalcNetDelta(Base *base)
{
    char    *estr = 0,*delta;

    free(base->delta.zip_data);
    estrCreate(&estr);
    baseToBin(&estr,&g_base);

    cryptAdler32Init();
    
    base->lastCRC = cryptAdler32(base->data_str, estrLength(&base->data_str));
    base->delta.unpack_size = bindiffCreatePatch(base->data_str,estrLength(&base->data_str),estr,estrLength(&estr),&delta,32);
    base->delta.zip_data = zipData(delta,base->delta.unpack_size,&base->delta.pack_size);
    free(delta);

    if (base->data_str)
        estrDestroy(&base->data_str);
    base->data_str = estr;
    base->data_len = estrLength(&estr);
}

void baseSendAll(Packet *pak,Base *base,U32 mod_time,int full_update)
{
    if (mod_time != base->mod_time)
    {
        baseCalcNetDelta(base);
        base->mod_time = mod_time;
        free(base->full.zip_data);
        ZeroStruct(&base->full);
    }
    if (full_update && !base->full.zip_data)
    {
        base->full.unpack_size = estrLength(&base->data_str);
        base->full.zip_data = zipData(base->data_str,base->full.unpack_size,&base->full.pack_size);
    }

    if (full_update)
    {
        pktSendBits(pak,1,1);
        pktSendBitsPack(pak,1,base->curr_id);
        pktSendZippedAlready(pak,base->full.unpack_size,base->full.pack_size,base->full.zip_data);
    }
    else
    {
        pktSendBits(pak,1,0);
        pktSendBits(pak,32,base->lastCRC);
        pktSendBitsPack(pak,1,base->curr_id);
        pktSendZippedAlready(pak,base->delta.unpack_size,base->delta.pack_size,base->delta.zip_data);
    }
}

#endif
