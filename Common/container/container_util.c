/*\
 *
 *    container_util.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Common functions for dealing with containers:
 *        - traces
 *        - reflect infos
 *        - hexString stuff moved here
 *
 */

#include "container_util.h"
#include "comm_backend.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/components/earray.h>
#include <stdio.h>

// *********************************************************************************
//  traces
// *********************************************************************************

void containerSendTrace(Packet* pak, U32 cmd, U32 listid, U32 cid, U32 user_cmd, U32 user_data)
{
    pktSendBitsPack(pak, 1, cmd);
    pktSendBitsPack(pak, 1, listid);
    pktSendBitsPack(pak, 1, cid);
    pktSendBitsPack(pak, 1, user_cmd);
    pktSendBitsPack(pak, 1, user_data);
}

void containerGetTrace(Packet* pak, U32* cmd, U32* listid, U32* cid, U32* user_cmd, U32* user_data)
{
    *cmd        = pktGetBitsPack(pak, 1);
    *listid        = pktGetBitsPack(pak, 1);
    *cid        = pktGetBitsPack(pak, 1);
    *user_cmd    = pktGetBitsPack(pak, 1);
    *user_data    = pktGetBitsPack(pak, 1);
}

// *********************************************************************************
//  reflect infos
// *********************************************************************************

MP_DEFINE(ContainerReflectInfo);
void ContainerReflectInfoDestroy(void* infoData)
{
    ContainerReflectInfo* info = (ContainerReflectInfo*)infoData;
    MP_FREE(ContainerReflectInfo, info);
}

void containerReflectAdd(ContainerReflectInfo*** reflectlist, U32 target_list, U32 target_cid, U32 target_dbid, int del)
{
    ContainerReflectInfo* info;

    if (!*reflectlist)
        eaCreate(reflectlist);
    MP_CREATE(ContainerReflectInfo, 10);
    info = MP_ALLOC(ContainerReflectInfo);
    info->target_list = target_list;
    info->target_cid = target_cid;
    info->target_dbid = target_dbid;
    info->del = del? 1: 0;
    eaPush(reflectlist, info);
}

void containerReflectDestroy(ContainerReflectInfo*** reflectlist)
{
    eaDestroyEx(reflectlist, ContainerReflectInfoDestroy);
}

// send all records with info->send on
void containerReflectSend(Packet* pak, ContainerReflectInfo*** reflectlist)
{
    int i, count = 0, size = eaSize(reflectlist);

    for (i = 0; i < size; i++)
    {
        ContainerReflectInfo* info = (*reflectlist)[i];
        if (!info->filter) count++;
    }
    pktSendBitsPack(pak, 1, count);
    for (i = 0; i < size; i++)
    {
        ContainerReflectInfo* info = (*reflectlist)[i];
        if (!info->filter)
        {
            pktSendBitsPack(pak, 1, info->target_list);
            pktSendBitsPack(pak, 1, info->target_cid);
            pktSendBitsPack(pak, 1, info->target_dbid);
            pktSendBits(pak, 1, info->del);
        }
    }
}

void containerReflectGet(Packet* pak, ContainerReflectInfo*** reflectlist)
{
    int i, size;

    MP_CREATE(ContainerReflectInfo, 10);
    if (!*reflectlist)
        eaCreate(reflectlist);
    eaClearEx(reflectlist, ContainerReflectInfoDestroy);
    size = pktGetBitsPack(pak, 1);
    for (i = 0; i < size; i++)
    {
        ContainerReflectInfo* info = MP_ALLOC(ContainerReflectInfo);
        info->target_list = pktGetBitsPack(pak, 1);
        info->target_cid = pktGetBitsPack(pak, 1);
        info->target_dbid = pktGetBitsPack(pak, 1);
        info->del = pktGetBits(pak, 1);
        eaPush(reflectlist, info);
    }
}

// @testing -AB: see if we can detect corruption  :07/19/06
bool checkContainerBadges_Internal(const char *container)
{
    if( container )
    {
        int l = 0;
        char *lvl = strstr(container,"\nLevel ");
        if( lvl && 1 == sscanf(lvl,"\nLevel %i", &l) && l > 10 )
        {
            char *badges = strstr(lvl,"Badges[0].Owned ");
            bool bad = badges!=NULL; // no badges, don't worry
            for(;badges&&*badges!='"'&&*badges!='\n'&&*badges;badges++); // find open quote
            if( badges && *badges == '"' ) // if found open, look for nonzero
            {
                for(badges++;*badges!='"'&&*badges!='\n'&&*badges;badges++)
                {
                    if(*badges!='0')
                    {
                        bad = false; // got at least one non-zero
                        break;
                    }
                }
            }
            return !bad;
        }
    }
    return true;
}
