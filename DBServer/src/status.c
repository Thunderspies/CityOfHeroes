#include <stdio.h>
#include "status.h"
#include <utilitieslib/utils/timing.h>
#include "comm_backend.h"
#include <utilitieslib/utils/utils.h>
#include "container_tplt.h"
#include "container_tplt_utils.h"
#include <utilitieslib/components/EString.h>

void entStatusCb(EntCon *container,char *buf)
{
    sprintf(buf,"%5d Name %-16s Auth %-16s Ip %-16s MapId %d SmapId %d  %s%s",
        container->id,container->ent_name,container->account,makeIpStr(container->client_ip),
        container->map_id,container->static_map_id,
        container->connected ? "" : "NoConnect ",
        container->callback_link ? "InMapXfer " : "");
}

void mapStatusCb(MapCon *container,char *buf)
{
    int        idx,seconds1,seconds2,cpu_seconds,uptime;

    if (!container->link)
        sprintf(buf,"%d %s NotReady 1 (Not started) Info %s",container->id,container->map_name, container->mission_info);
    else
    {
        RemoteProcessInfo    *rpi = &container->remote_process_info;
        char                ipbuf[1000];

        uptime = timerSecondsSince2000() - container->on_since;
        seconds1 = timerCpuSeconds() - container->link->lastRecvTime;
        seconds2 = timerCpuSeconds() - container->lastRecvTime;
        cpu_seconds = rpi->total_cpu_time / 1000;
        sprintf(ipbuf,"%s:%d",makeIpStr(container->ip_list[0]),container->udp_port);
        idx = sprintf(buf,"%2d %-22s S:%2d/%-2d Ip:%-16s Mem:%4dM Cpu:%d:%02d.%02d = %0.2f %0.2f  Up: %d:%02d Info %s",
            container->id,getFileName(container->map_name),
            seconds1, // Seconds since network activity
            seconds2, // Seconds since last mapserver WHO update (indication that main tick is running)
            ipbuf,
            rpi->mem_used_phys / 1024,
            cpu_seconds / 60,cpu_seconds % 60,(rpi->total_cpu_time/10)%100,
            rpi->cpu_usage,rpi->cpu_usage60,
            uptime / 3600,(uptime/60)%60,
            container->mission_info);
        if (container->starting)
            sprintf(buf + idx," NotReady 1");
    }
}

void groupStatusCb(GroupCon *container,char *buf)
{
    int        j,idx;

    idx = sprintf(buf,"%3d Active %d locked %d",
        container->id,
        container->active,container->locked);
    if (container->member_id_count)
    {
        idx += sprintf(buf+idx,"\nMembers:");
        for(j=0;j<container->member_id_count;j++)
            idx += sprintf(buf+idx," %d",container->member_ids[j]);
    }
}

static void serverWho(Packet *pak_in,char **buf)
{
    int        map_idx,i,idx=0,total_players=0,total_players_connecting=0,total_monsters=0,total_ents=0,static_count=0,mission_count=0,edit_count=0;
    DbList    *map_list = dbListPtr(CONTAINER_MAPS);
    MapCon    *map;

    map_idx = pktGetBitsPack(pak_in,1);
    if (map_idx < 0)
    {
        for(i=0;i<map_list->num_alloced;i++)
        {
            map = (MapCon*) map_list->containers[i];
            if (!map->active)
                continue;
            total_players += map->num_players;
            total_players_connecting += map->num_players_connecting;
            total_monsters += map->num_monsters;
            total_ents += map->num_ents;
            if (map->is_static)
                static_count++;
            else if (map->edit_mode)
                edit_count++;
            else
                mission_count++;
        }
        estrConcatStaticCharArray(buf, "\nServer-wide stats\n");
        estrConcatf(buf,"Zone Maps:          %d\n",static_count);
        estrConcatf(buf,"Mission Maps:       %d\n",mission_count);
        estrConcatf(buf,"Local Maps:         %d\n",edit_count);
    }
    else
    {
        char    *name;

        map = containerPtr(map_list,map_idx);
        if (!map)
        {
            estrPrintf(buf,"\nMap %d doesn't exist.\n",map_idx);
            return;
        }
        name = map->map_name;
        if (!name[0])
            name = "localmap";
        if (!map->active)
        {
            estrPrintf(buf,"\nMap %s (%d) isn't running.\n",name,map_idx);
            return;
        }
        total_players = map->num_players;
        total_players_connecting += map->num_players_connecting;
        total_monsters = map->num_monsters;
        total_ents = map->num_ents;
        estrPrintf(buf,"\nStats for map %s (%d)\n",name,map_idx);
    }
    estrPrintf(buf,"Entitities:         %d\n",total_ents);
    estrPrintf(buf,"Players Playing:    %d\n",total_players);
    estrPrintf(buf,"Players Connecting: %d\n",total_players_connecting);
    estrPrintf(buf,"Monsters:           %d\n",total_monsters);
}

static void playerWho(Packet *pak_in,char **buf)
{
    DbList* ent_list = dbListPtr(CONTAINER_ENTS);
    char* name;
    EntCon* e = NULL;
    int i, idx = 0;

    name = pktGetString(pak_in);
    for(i=0;i<ent_list->num_alloced;i++)
    {
        e = (EntCon*) ent_list->containers[i];
        if (stricmp(e->ent_name,name)==0)
            break;
    }
    if (i >= ent_list->num_alloced)
    {
        estrConcatf(buf,"\nplayer %s is not online or doesn't exist.\n",name);
        return;
    }
    estrConcatf(buf, "\nPlayer:     %s\n",e->ent_name);
    estrConcatf(buf, "Account:    %s\n",e->account);
    estrConcatf(buf, "Authid:     %d\n",e->auth_id);
    if (e->teamup_id)
        estrConcatf(buf,"TeamUp:     %d\n",e->teamup_id);
    if (e->supergroup_id)
    {
        GroupCon    *group = containerPtr(dbListPtr(CONTAINER_SUPERGROUPS),e->supergroup_id);
        char        groupname[128] = "??Bad Id??";

        if (group)
            findFieldTplt(dbListPtr(CONTAINER_SUPERGROUPS)->tplt,&group->line_list,"Name",groupname);
        estrConcatf(buf,"SuperGroup: %s (%d)\n",groupname,e->supergroup_id);
    }
    if (e->taskforce_id)
    {
        GroupCon    *group = containerPtr(dbListPtr(CONTAINER_TASKFORCES), e->taskforce_id);
        char        taskforcename[128] = "?? Bad Id ??";
        if (group)
            findFieldTplt(dbListPtr(CONTAINER_TASKFORCES)->tplt,&group->line_list, "Name", taskforcename);
        estrConcatf(buf,"TaskForce: %s (%d)\n",taskforcename,e->taskforce_id);
    }
    if (e->raid_id)
        estrConcatf(buf,"Raid: %d\n", e->raid_id);
    if (e->levelingpact_id)
        estrConcatf(buf,"LevelingPact: %d\n", e->levelingpact_id);
    {
        MapCon    *map = containerPtr(dbListPtr(CONTAINER_MAPS),e->map_id);
        char    mapname[128] = "??Bad Id??";

        if (map)
            strcpy(mapname,map->map_name);
        if (!mapname[0])
            strcpy(mapname,"localedit");
        estrConcatf(buf,"Map:        %s (%d)\n",escapeString(mapname),e->map_id);
    }
    if (e->league_id)
        estrConcatf(buf, "League: %d\n", e->league_id);
    if (e->map_id != e->static_map_id)
    {
        MapCon    *map = containerPtr(dbListPtr(CONTAINER_MAPS),e->static_map_id);
        char    mapname[128] = "??Bad Id??";

        if (map)
            strcpy(mapname,map->map_name);
        estrConcatf(buf,"Static Map: %s (%d)\n",escapeString(mapname),e->static_map_id);
    }
    {
        char    posx[64]="0",posy[64]="0",posz[64]="0";

        findFieldTplt(ent_list->tplt,&e->line_list,"PosX",posx);
        findFieldTplt(ent_list->tplt,&e->line_list,"PosY",posy);
        findFieldTplt(ent_list->tplt,&e->line_list,"PosZ",posz);
        estrConcatf(buf,"Static Pos: %s %s %s\n",posx,posy,posz);
    }
}

static void supergroupWho(Packet *pak_in,char **buf)
{
    DbList        *supergroup_list = dbListPtr(CONTAINER_SUPERGROUPS);
    char        *name,namebuf[128],fieldName[128];
    int            i,idx = 0, allyId;
    GroupCon* sg = NULL;
    GroupCon* sgAlly = NULL;

    name = pktGetString(pak_in);
    for(i=0;i<supergroup_list->num_alloced;i++)
    {
        sg = (GroupCon *) supergroup_list->containers[i];
        findFieldTplt(supergroup_list->tplt,&sg->line_list,"Name",namebuf);
        if (stricmp(namebuf,name)==0)
            break;
    }
    if (i >= supergroup_list->num_alloced)
    {
        estrConcatf(buf,"Supergroup %s doesn't exist, or no members online\n",name);
        return;
    }
    estrConcatf(buf,"\nSupergroup %s (%d)\n",name, sg->id);

    for (i = 0;; i++)
    {
        sprintf(fieldName, "SuperGroupAllies[%d].AllyId", i);
        if (!findFieldTplt(supergroup_list->tplt,&sg->line_list, fieldName, namebuf))
            break;
        if (allyId = atoi(namebuf))
        {
            sprintf(fieldName, "SuperGroupAllies[%d].AllyName", i);
            if (!findFieldTplt(supergroup_list->tplt,&sg->line_list, fieldName, namebuf))
            {
                sgAlly = containerPtr(supergroup_list, allyId);
                if (sgAlly)
                    findFieldTplt(supergroup_list->tplt,&sgAlly->line_list,"Name",namebuf);
                else
                    strcpy(namebuf, "<offline>");
            }
            estrConcatf(buf, "Ally: %s (%d)\n", namebuf, allyId);
        }
    }

    for(i=0;i<sg->member_id_count;i++)
    {
        EntCon    *e;
        char    *member;

        e = containerPtr(dbListPtr(CONTAINER_ENTS),sg->member_ids[i]);
        if (!e)
            member = "<offline>";
        else
            member = e->ent_name;
        estrConcatf(buf,"Member: %s (%d)\n",member,sg->member_ids[i]);
    }
}

static void groupWho(int type, Packet *pak_in, char **buf)
{
    DbList        *list = dbListPtr(type);
    int            group_idx = pktGetBitsPack(pak_in, 1);
    GroupCon    *group = containerPtr(list, group_idx);
    char        name[sizeof(list->name)];

    strcpy(name, list->name);
    if(devassert(name[0]))
        name[strlen(name)-1] = '\0';

    if(!group || !devassert(list->tplt->member_id == CONTAINER_ENTS))
    {
        estrConcatf(buf, "%s %d doesn't exist\n", name, group_idx);
    }
    else
    {
        int i;
        DbList *member_list = dbListPtr(list->tplt->member_id);
        estrConcatf(buf,"\n%s %d\n", name, group_idx);
        for(i = 0; i < group->member_id_count; i++)
        {
            int member_idx = group->member_ids[i];
            EntCon *member = containerPtr(member_list, member_idx);
            estrConcatf(buf, "%s (%d)\n", member ? member->ent_name : "offline", member_idx);
        }
    }
}

void handleWho(Packet *pak_in, NetLink *link)
{
    char *s_in = pktGetString(pak_in);
    char *s_out = estrTemp();

    if(stricmp(s_in, "serverwho")==0)
        serverWho(pak_in, &s_out);
    else if(stricmp(s_in, "who")==0)
        playerWho(pak_in, &s_out);
    else if(stricmp(s_in, "supergroupwho")==0)
        supergroupWho(pak_in, &s_out);
    else if(stricmp(s_in, "teamupwho")==0)
        groupWho(CONTAINER_TEAMUPS, pak_in, &s_out);
    else if(stricmp(s_in, "raidwho")==0)
        groupWho(CONTAINER_RAIDS, pak_in, &s_out);
    else if(stricmp(s_in, "levelingpactwho")==0)
        groupWho(CONTAINER_LEVELINGPACTS, pak_in, &s_out);
    else if(stricmp(s_in, "leaguewho")==0)
        groupWho(CONTAINER_LEAGUES, pak_in, &s_out);

    {
        Packet    *pak_out = pktCreateEx(link, DBSERVER_WHO);
        pktSendString(pak_out, s_out);
        pktSend(&pak_out, link);
    }

    estrDestroy(&s_out);
}

void handleServerStatsUpdate(Packet *pak,int map_id)
{
    MapCon    *map_con;

    map_con = containerPtr(dbListPtr(CONTAINER_MAPS),map_id);
    if (!map_con)
        return;
    map_con->num_ents = pktGetBitsPack(pak,1);
    map_con->num_players = pktGetBitsPack(pak,1);
    map_con->num_monsters = pktGetBitsPack(pak,1);
    map_con->num_players_connecting = pktGetBitsPack(pak,1);
    map_con->ticks_sec = pktGetF32(pak);
    map_con->long_tick = pktGetF32(pak);
    map_con->num_hero_players = pktGetBitsPack(pak,1);
    map_con->num_villain_players = pktGetBitsPack(pak,1);
    map_con->num_relay_cmds = pktGetBitsAuto(pak);
    if (!pktEnd(pak)) {
        map_con->average_ping = pktGetBitsPack(pak, 9);
    }
    if (!pktEnd(pak)) {
        map_con->min_ping = pktGetBitsPack(pak, 9);
        map_con->max_ping = pktGetBitsPack(pak, 9);
    }
    map_con->num_players_sent_since_who_update = 0;
    map_con->lastRecvTime = timerCpuSeconds();
}

void handleRequestSGEldestOn(Packet *pak_in, NetLink *link)
{
    char rankBuf[128];
    char nameBuf[128] = "";
    Packet *pak_out;
    int sgid, memberid = 0, minrank, i;
    GroupCon *sg;
    U32 timeon = 0;
    U32 timenow = timerSecondsSince2000();

    sgid = pktGetBits(pak_in, 32);
    minrank = pktGetBits(pak_in, 2);

    sg = containerPtr(dbListPtr(CONTAINER_SUPERGROUPS), sgid);

    for(i=0;i<sg->member_id_count;i++)
    {
        EntCon    *e;

        if ((e = containerPtr(dbListPtr(CONTAINER_ENTS),sg->member_ids[i])) && e->active)
        {
            if (!findFieldTplt(dbListPtr(CONTAINER_ENTS)->tplt,&e->line_list, "Rank", rankBuf)) {
                sprintf(rankBuf,"%d",0);
            }
            if ((atoi(rankBuf) >= minrank) && (timenow - e->on_since > timeon))
            {
                timeon = timenow - e->on_since;
                memberid = sg->member_ids[i];
                findFieldTplt(dbListPtr(CONTAINER_ENTS)->tplt,&e->line_list, "Name", nameBuf);
            }
        }
    }

    pak_out = pktCreateEx(link,DBSERVER_SG_ELDEST_ON);
    pktSendBits(pak_out,32,memberid);
    pktSendString(pak_out, nameBuf); // don't escape this!  it is already escaped in the database.
    pktSend(&pak_out, link);
}




