#include <stdio.h>
#include "container.h"
#include <utilitieslib/network/netio.h>
#include "comm_backend.h"
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include "dbdispatch.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/strings_opt.h>
#include <zlib.h>
#include <utilitieslib/network/netcomp.h>
#include <utilitieslib/utils/mathutil.h>
#include "dbserver/servercfg.h"

U32 door_curr_tag;


void doorStatusCb(DoorCon *container,char *buf)
{
    char* s=container->raw_data;
    int count=0, idx = (int)strlen(container->raw_data)-1;

    for(;idx>=0;idx--)
    {
        if (s[idx] == '\n' && ++count==3)
        {
            s = s+idx;
            break;
        }
    }
    sprintf(buf,"%3d locked %d MapId %d  Pos %f %f %f %s",
        container->id,container->locked,
        container->map_id,container->pos[0],container->pos[1],container->pos[2],s);
    for(s=buf;*s;s++)
    {
        if (*s == '\n')
            *s = ' ';
    }
}

void doorSetSpecialInfo(char *data,int *map_id,Vec3 pos)
{
    char    *str,*s,*args[10];
    int        j;

    str = s = _strdup(data);
    tokenize_line(s,args,&s);
    *map_id = atoi(args[1]);
    for(j=0;j<3;j++)
    {
        tokenize_line(s,args,&s);
        pos[j] = atof(args[1]);
    }
    free(str);
}

void doorListSetSpecialInfo(DbList *list)
{
    int        i;
    DoorCon    *container;

    for(i=0;i<list->num_alloced;i++)
    {
        container = (DoorCon *)list->containers[i];
        doorSetSpecialInfo(container->raw_data,&container->map_id,container->pos);
    }
}

// returns 1 if data changed
static int addDoorEntry(char *str,int min_idx,int max_idx)
{
    int i, map_id;
    DbList* list;
    DoorCon* container = NULL;
    Vec3    pos;

    doorSetSpecialInfo(str,&map_id,pos);
    list = dbListPtr(CONTAINER_DOORS);
    for(i=min_idx;i<max_idx;i++)
    {
        container = (DoorCon *)list->containers[i];
        if (map_id == container->map_id && nearSameVec3(pos,container->pos))
        {
            if (strcmp(str,container->raw_data)==0)
            {
                container->tag = door_curr_tag;
                return 0;
            }
            SAFE_FREE(container->raw_data);
            container->container_data_size = 0;
            break;
        }
    }
    if (i >= max_idx)
    {
        //printf("adding at %f %f %f\n",pos[0],pos[1],pos[2]);
        container = containerAlloc(list,-1);
        copyVec3(pos,container->pos);
        container->map_id = map_id;
    }
    container->tag = door_curr_tag;
    container->raw_data = _strdup(str);
    container->container_data_size = (U32)strlen(str)+1;
    return 1;
}

void getMapMinMax(int map_id,int *min_idx,int *max_idx)
{
    int        i,min=-1,max=0;
    DoorCon    *container;

    for(i=0;i<doors_list->num_alloced;i++)
    {
        container = (DoorCon *)doors_list->containers[i];
        if (container->map_id == map_id)
        {
            if (min<0)
                min = i;
            max = i+1;
        }
    }
    if (min < 0)
        min = 0;
    *min_idx = min;
    *max_idx = max;
}

void sendDoorList(NetLink *link,int rebuild)
{
    int                    i;
    Packet                *pak;
    static StuffBuff    sb;
    static int            zipsize,rawsize;
    static char            *zipdata;

    if (rebuild || !sb.idx)
    {
        if (!sb.size)
            initStuffBuff(&sb,100000);
        free(zipdata);
        sb.idx = 0;
        for(i=0;i<doors_list->num_alloced;i++)
        {
            DoorCon                *container;

            container = (DoorCon *)doors_list->containers[i];
            addStringToStuffBuff(&sb,"%d \"%s\"\n",container->id,escapeString(container->raw_data));
        }
        zipdata = zipData(sb.buff,sb.idx,&zipsize);
    }
    pak = pktCreateEx(link,DBSERVER_SEND_DOORS);
    pktSendBitsPack(pak,1,doors_list->num_alloced);
    pktSendZippedAlready(pak,sb.idx,zipsize,zipdata);
    pktSend(&pak,link);
}

static bool shouldSendDoorListToMap(MapCon *container)
{
    // This is a fallback/safety switch in case we mess something up.
    // Do the old behavior if send_doors_to_all_maps is set.
    if (server_cfg.send_doors_to_all_maps)
        return true;

    // Send the door list to static maps
    if (container->is_static)
        return true;

    // Send the door list to the supergroup base
    // See teamCheckLeftMission()
    if (container->map_name && strstri(container->map_name, "Supergroupid=" ))
        return true;
    
    // Do not send the door list to end-game raids
    // See teamCheckLeftMission()
    //if (container->mission_info && container->mission_info[0] == 'E')
    //    return true;

    // Do not send the door list to the arena maps
    // See teamCheckLeftMission()
    //if (container->map_name && strstri(container->map_name, "Arena_" ))
    //    return true;

    // Hopefully, only mission maps fall through to here
    return false;
}

void sendDoorListToMapServers()
{
    DbList    *maps;
    int        i,first=1;
    MapCon    *container;

    maps = dbListPtr(CONTAINER_MAPS);
    for(i=0;i<maps->num_alloced;i++)
    {
        container = (MapCon *)maps->containers[i];
        if (!container->link)
            continue;
        if (!shouldSendDoorListToMap(container))
            continue;
        sendDoorList(container->link,first);
        first = 0;                           
    }
}

void handleNewClientDoors(Packet *pak,NetLink *link)
{
    int        i,count,map_id,modified=0;
    MapCon    *container;
    DoorCon    *door_con;
    DbList    *list;

    ++door_curr_tag;
    map_id = pktGetBitsPack(pak,1);
    count = pktGetBitsPack(pak,1);
    container = containerPtr(dbListPtr(CONTAINER_MAPS),map_id);
    if (!container)
        return;
    container->map_running = 1;
    if (!container->is_static)
        count = 0; // shouldn't be getting doors from non-static maps
    if(container->base_map_id)
        container = containerPtr(dbListPtr(CONTAINER_MAPS),container->base_map_id);
    if (!container)
        return;
    map_id = container->id;
    if (count)
    {
        int        min_idx,max_idx;
        getMapMinMax(map_id,&min_idx,&max_idx);
        for(i=0;i<count;i++)
            modified |= addDoorEntry(pktGetString(pak),min_idx,max_idx);

        list = dbListPtr(CONTAINER_DOORS);
        for(i=list->num_alloced-1;i>=0;i--)
        {
            door_con = (DoorCon *) list->containers[i];
            if (door_con->map_id == map_id && door_con->tag != door_curr_tag)
            {
                containerDelete(door_con);
                modified = 1;
            }
        }
        if (modified)
            sendDoorListToMapServers();
        else if (shouldSendDoorListToMap(container))

            sendDoorList(link,0);
    }
    else if (shouldSendDoorListToMap(container))
        sendDoorList(link,0);
}


