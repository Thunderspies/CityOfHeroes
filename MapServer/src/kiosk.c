/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/earray.h>

#include "entity/entPlayer.h"
#include "group/grouputil.h"
#include "group/group.h"
#include <utilitieslib/utils/utils.h>
#include "language/langServerUtil.h"
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/textparser.h>
#include "svr/svr_base.h"
#include "comm_game.h"
#include "gameComm/VillainDef.h"
#include "cmdparse/cmdserver.h"
#include "player/stats_base.h"

#include "kiosk.h"
#include <utilitieslib/utils/mathutil.h>
#include "dbcomm/dbnamecache.h"
#include "entity/entity.h"

#include "gameSys/arenamapserver.h"

typedef struct Kiosk
{
    char *pchName;
    Vec3 pos;
} Kiosk;

static Kiosk **g_Kiosks = NULL;

/**********************************************************************func*
 * kiosk_Create
 *
 */
static Kiosk *kiosk_Create(void)
{
    return calloc(1, sizeof(Kiosk));
}

/**********************************************************************func*
 * kiosk_Destroy
 *
 */
static void kiosk_Destroy(Kiosk *p)
{
    if(p->pchName)
    {
        free(p->pchName);
    }

    free(p);
}

/**********************************************************************func*
 * kiosk_LocationRecorder
 *
 */
static int kiosk_LocationRecorder(GroupDefTraverser* traverser)
{
    char *pchKiosk;
    Kiosk *pKiosk;

    // We are only interested in nodes with properties.
    if(!traverser->def->properties)
    {
        // We're only interested in subtrees with properties.
        if(!traverser->def->child_properties)
        {
            return 0;
        }

        return 1;
    }

    // Does this thing have a Kiosk?
    pchKiosk = groupDefFindPropertyValue(traverser->def, "Kiosk");
    if(pchKiosk==NULL)
    {
        return 1;
    }

    pKiosk = kiosk_Create();
    copyVec3(traverser->mat[3], pKiosk->pos);
    pKiosk->pchName = strdup(pchKiosk);

    eaPush(&g_Kiosks, pKiosk);

    return 1;
}


/**********************************************************************func*
 * kiosk_Load
 *
 */
static void kiosk_DestroyAdapter(void* arg0)
{
    kiosk_Destroy((Kiosk *)arg0);
}

void kiosk_Load(void)
{
    GroupDefTraverser traverser = {0};

    loadstart_printf("Loading kiosk locations.. ");

    // If we're doing a reload, clear out old data first.
    if(g_Kiosks)
    {
        eaClearEx(&g_Kiosks, kiosk_DestroyAdapter);
        eaSetSize(&g_Kiosks, 0);
    }
    else
    {
        eaCreate(&g_Kiosks);
    }

    groupProcessDefExBegin(&traverser, &group_info, kiosk_LocationRecorder);

    loadend_printf("%i found", eaSize(&g_Kiosks));
}


/**********************************************************************func*
 * DumpResults
 *
 */
static void DumpRaceResults(Entity *e, StuffBuff *psb, DBStatResult *pres, int iNumPlaces)
{
    int i;
    int iPlace;
    int iLast = -1;
    int iCnt = eaiSize(&pres->pIDs);

    if(iCnt<1)
    {
        addStringToStuffBuff(psb, "<span align=center><scale .75>%s</scale></span><br>\n", localizedPrintf(e,"KioskNoInfo"));
        return;
    }

    addStringToStuffBuff(psb, "<scale 1.0><table><tr><scale .80><td width=30></td><td align=right width=1>%s", localizedPrintf(e,"RankString") );
    addStringToStuffBuff(psb, "</td><td align=right width=1>%s", localizedPrintf(e,"TimeString") );
    addStringToStuffBuff(psb, "</td><td width=55><bsp>%s</td></scale></tr>\n", localizedPrintf(e,"HeroString") );

    iNumPlaces--;
    for(i=0,iPlace=0; i<iCnt && iPlace<iNumPlaces; i++)
    {
        addStringToStuffBuff(psb, "<tr><td></td>");

        if(iLast != pres->piValues[i])
        {
            iPlace = i;
            addStringToStuffBuff(psb, "<td align=right>%8d</td>", iPlace+1);
        }
        else
        {
            addStringToStuffBuff(psb, "<td></td>");
        }
        iLast = pres->piValues[i];

        {
            //Format time
            int time, minutes, seconds;
            time = 0xffffffff - pres->piValues[i];  //reverse (Shannon's stuff wants higher to be better)
            minutes = time / 60;
            seconds = time % 60;

            addStringToStuffBuff(psb, "<td align=right>%d:%d</td><td><bsp>%s</td>",
                minutes, seconds,
                dbPlayerNameFromId(pres->pIDs[i]));
        }

        addStringToStuffBuff(psb, "</tr>\n");
    }

    addStringToStuffBuff(psb, "</table></scale><br>\n");
}



/**********************************************************************func*
 * DumpResults
 *
 */
static void DumpResults(Entity *e, StuffBuff *psb, DBStatResult *pres, int iNumPlaces)
{
    int i;
    int iPlace;
    int iLast = -1;
    int iCnt = 0;
    
    if(pres)
        iCnt = eaiSize(&pres->pIDs);

    if(iCnt<1)
    {
        addStringToStuffBuff(psb, "<span align=center><scale .75>%s</scale></span><br>\n", localizedPrintf(e,"KioskNoInfo"));
        return;
    }

    addStringToStuffBuff(psb, "<scale 1.0><table><tr><scale .80><td width=30></td>");
    addStringToStuffBuff(psb, "<td align=right width=1>%s</td>",
                                localizedPrintf(e,"RankString"));
    addStringToStuffBuff(psb, "<td align=right width=1>%s</td>",
                                localizedPrintf(e,"CountString"));
    addStringToStuffBuff(psb, "<td width=55><bsp>%s</td></scale></tr>\n",
                                localizedPrintf(e,"HeroString"));

    iNumPlaces--;
     for(i=0,iPlace=0; i<iCnt && iPlace<iNumPlaces; i++)
    {
        addStringToStuffBuff(psb, "<tr><td></td>");

        if( dbPlayerTypeFromId(pres->pIDs[i]) == kPlayerType_Hero )
            addStringToStuffBuff(psb, "<font face=computer outline=0 color=paragon>" );
        else
            addStringToStuffBuff(psb, "<font face=computer outline=0 color=villagon>" );

        if(iLast != pres->piValues[i])
        {
            iPlace = i;
            addStringToStuffBuff(psb, "<td align=right>%8d&nbsp;</td>", iPlace+1);
        }
        else
        {
            addStringToStuffBuff(psb, "<td></td>");
        }
        iLast = pres->piValues[i];

        addStringToStuffBuff(psb, "<td align=right>&nbsp;&nbsp;%d</td><td><bsp>%s</td>",
            pres->piValues[i],
            dbPlayerNameFromId(pres->pIDs[i]));

        addStringToStuffBuff(psb, "</font>" );

        addStringToStuffBuff(psb, "</tr>\n");
    }

    addStringToStuffBuff(psb, "</table></scale><br>\n");
}

/**********************************************************************func*
 * kiosk_VillainGroupIsShown
 *
 */
bool kiosk_VillainGroupIsShown(int iGroup)
{
    if(iGroup>=0 && iGroup<VG_MAX &&
        villainGroupShowInKiosk(iGroup))
    {
        return true;
    }

    return false;
}

/**********************************************************************func*
 * DumpLeaders
 *
 */
static void DumpLeaders(Entity * e, StuffBuff *psb, char *which, int iPer)
{
//    StaticDefineInt *sdi;
    int i;


    if(iPer>=kStatPeriod_Count)
    {
        iPer=0;
    }

    if(server_state.nostats)
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span><br>\n", localizedPrintf(e,"KioskWelcome"));
        addStringToStuffBuff(psb, "<span><scale 1.2>\n");
        addStringToStuffBuff(psb, "%s", localizedPrintf(e,"KioskHomePage"));
        addStringToStuffBuff(psb, "<br><br>%s", localizedPrintf(e,"StatsOffline"));
        addStringToStuffBuff(psb, "</scale></span>\n");
    }
    else if(stricmp(which, "policers")==0)
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskTopPolicersInMap", db_state.map_name));
        DumpResults(e,psb, stat_GetTable(db_state.map_name, kStat_Kills, iPer), 10);

        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskTopPolicers"));
        DumpResults(e,psb, stat_GetTable("overall", kStat_Kills, iPer), 10);
    }
    else if(stricmp(which, "damagers")==0)
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskTopDamagers"));
        DumpResults(e,psb, stat_GetTable("damage_given", kStat_General, iPer), 10);
    }
    else if(stricmp(which, "healers")==0)
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskTopHealers"));
        DumpResults(e,psb, stat_GetTable("healing_given", kStat_General, iPer), 10);
    }
    else if(stricmp(which, "celebrities")==0)
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskTopCelebritiesInMap", db_state.map_name));
        DumpResults(e,psb, stat_GetTable(db_state.map_name, kStat_Influence, iPer), 10);

        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskTopCelebrities"));
        DumpResults(e,psb, stat_GetTable("overall", kStat_Influence, iPer), 10);
    }
    else if(stricmp(which, "racers")==0)
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskTopRacers"));
        DumpRaceResults(e, psb, stat_GetTable("race_founders_falls", kStat_General, iPer), 10);
    }
    else if(stricmp(which, "nemeses")==0)
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskVillainGroupNemeses"));
        // All the villain groups

        for (i = 0; i < VG_MAX; i++)
        {
            if (kiosk_VillainGroupIsShown(i))
            {
                const char *name = villainGroupGetName(i);
                const char *displayName = villainGroupGetPrintName(i);
                addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span>\n", localizedPrintf(e,"KioskBlankNemeses", displayName));
                DumpResults(e,psb, stat_GetTable(name, kStat_Kills, iPer), 3);
            }
        }
    }
    else if(stricmp(which, "arena")==0)
    {
        DumpArenaLeaders(e, psb, 10);
    }
    else
    {
        addStringToStuffBuff(psb, "<span align=center><scale 1.5>%s</scale></span><br>\n", localizedPrintf(e,"KioskWelcome"));
        addStringToStuffBuff(psb, "<span><scale 1.2>\n");
        addStringToStuffBuff(psb, "%s", localizedPrintf(e,"KioskHomePage"));
        addStringToStuffBuff(psb, "</scale></span>\n");
    }
}


/**********************************************************************func*
 * kiosk_Headline
 *
 */
kiosk_Headline(Entity *e, StuffBuff *psb)
{
    DBStatResult *pres;
    int n;
    int i;

    addStringToStuffBuff(psb, "%s -- ", localizedPrintf(e,"KioskTopPolicers"));

    pres = stat_GetTable("damage_given", kStat_General, kStatPeriod_Today);
    n = eaiSize(&pres->pIDs);
    n = min(5, n);
    for(i=0; i<n; i++)
    {
        if(i>0)
        {
            addStringToStuffBuff(psb, ", ");
        }
        addStringToStuffBuff(psb, "%s", dbPlayerNameFromId(pres->pIDs[i]));
    }
}


/**********************************************************************func*
 * kiosk_FindClosest
 *
 */
Kiosk *kiosk_FindClosest(const Vec3 vec)
{
    float fDist = SQR(20);
    Kiosk *pKiosk = NULL;
    int i;

    for(i=eaSize(&g_Kiosks)-1; i>=0; i--)
    {
        float fNewDist = distance3Squared(vec, g_Kiosks[i]->pos);
        if(fNewDist < fDist)
        {
            pKiosk = g_Kiosks[i];
            fDist = fNewDist;
        }
    }

    return pKiosk;
}

/**********************************************************************func*
 * kiosk_Tick
 *
 */
void kiosk_Tick(Entity *e)
{
    if(e->pl->at_kiosk)
    {
        kiosk_Check(e);
    }
}

/**********************************************************************func*
 * kiosk_Check
 *
 */
bool kiosk_Check(Entity *e)
{
    if(kiosk_FindClosest(ENTPOS(e))==NULL)
    {
        START_PACKET(pak, e, SERVER_BROWSER_CLOSE);
        END_PACKET
        e->pl->at_kiosk = 0;
        return false;
    }

    return true;
}

/**********************************************************************func*
 * kiosk_Navigate
 *
 */
void kiosk_Navigate(Entity *e, char *cmd, char *cmd2)
{
    StuffBuff sb;
    int iPer;

    if(!kiosk_Check(e))
    {
        return;
    }

    e->pl->at_kiosk = 1;

    if(stricmp(cmd2, "home")==0)
        iPer = kStatPeriod_Today;
    else
        iPer = atoi(cmd2);

    initStuffBuff(&sb, 128);

    addStringToStuffBuff(&sb, "<font face=computer outline=0 color=#32ff5d><b>\n");
    addStringToStuffBuff(&sb, "<span align=center>\n");
    addStringToStuffBuff(&sb, "<scale 1.8>%s</scale><br><br>\n", localizedPrintf(e,"KioskCityInfo"));
    addStringToStuffBuff(&sb, "</span>\n");

    addStringToStuffBuff(&sb, "<table><tr>\n");

    addStringToStuffBuff(&sb, "<td>\n");
        addStringToStuffBuff(&sb, "<font face=computer outline=0 color=#22ff4d><b>\n"); // #a9e3f7
        addStringToStuffBuff(&sb, "<linkbg #88888844><linkhoverbg #578836><linkhover #a7ff6c><link #52ff7d><b>\n");
        addStringToStuffBuff(&sb, "<table>\n");
//            addStringToStuffBuff(&sb, "<tr><td align=center><br><br><br>%s</td></tr>\n", localizedPrintf("KioskChooseTopic"));

             addStringToStuffBuff(&sb, "<a href='cmd:kiosk home home'><tr%s><td align=center valign=center>%s</td></tr></a>\n", stricmp(cmd,"home")==0?" selected=1":"", localizedPrintf(e,"KioskHome"));
            addStringToStuffBuff(&sb, "<tr><td><br></td></tr>\n");

            addStringToStuffBuff(&sb, "<tr><td align=left valign=center>%s</td></tr>", localizedPrintf(e, "TopicString") );
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk policers %s'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd2, stricmp(cmd,"policers")==0?" selected=1":"", localizedPrintf(e,"KioskPolicers"));
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk damagers %s'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd2, stricmp(cmd,"damagers")==0?" selected=1":"", localizedPrintf(e,"KioskDamagers"));
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk healers %s'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd2, stricmp(cmd,"healers")==0?" selected=1":"", localizedPrintf(e,"KioskHealers"));
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk nemeses %s'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd2, stricmp(cmd,"nemeses")==0?" selected=1":"", localizedPrintf(e,"KioskNemeses"));
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk arena %s'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd2, stricmp(cmd,"arena")==0?" selected=1":"", localizedPrintf(e,"KioskArena"));
//            addStringToStuffBuff(&sb, "<a href='cmd:kiosk celebrities %s'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd2, stricmp(cmd,"celebrities")==0?" selected=1":"", localizedPrintf("KioskCelebrities"));
            //RACERS if( static_map_info  == founder's falls )
            //addStringToStuffBuff(&sb, "<a href='cmd:kiosk racers %s'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd2, stricmp(cmd,"healers")==0?" selected=1":"", localizedPrintf("KioskRacers"));

            addStringToStuffBuff(&sb, "<tr><td align=left valign=center>%s</td></tr>", localizedPrintf(e, "TimePeriodString") );
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk %s %d'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd, kStatPeriod_Today    , iPer==kStatPeriod_Today    ?" selected=1":"", localizedPrintf(e,"KioskToday"));
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk %s %d'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd, kStatPeriod_ThisMonth, iPer==kStatPeriod_ThisMonth?" selected=1":"", localizedPrintf(e,"KioskThisMonth"));
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk %s %d'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd, kStatPeriod_Yesterday, iPer==kStatPeriod_Yesterday?" selected=1":"", localizedPrintf(e,"KioskYesterday"));
            addStringToStuffBuff(&sb, "<a href='cmd:kiosk %s %d'><tr%s><td align=right valign=center>%s</td></tr></a>\n", cmd, kStatPeriod_LastMonth, iPer==kStatPeriod_LastMonth?" selected=1":"", localizedPrintf(e,"KioskLastMonth"));

        addStringToStuffBuff(&sb, "</table>\n");
        addStringToStuffBuff(&sb, "</linkhover>\n");
        addStringToStuffBuff(&sb, "</linkhoverbg>\n");
        addStringToStuffBuff(&sb, "</linkbg>\n");
        addStringToStuffBuff(&sb, "</font>\n");
    addStringToStuffBuff(&sb, "</td>\n");

    addStringToStuffBuff(&sb, "<td>\n");
    addStringToStuffBuff(&sb, "<img src=white.tga height=275 width=1 color=#32ff5d><br>\n");
    addStringToStuffBuff(&sb, "</td>\n");

    addStringToStuffBuff(&sb, "<td width=100>\n");
    DumpLeaders(e, &sb, cmd, iPer);
    addStringToStuffBuff(&sb, "</td>\n");

    addStringToStuffBuff(&sb, "</tr></table>\n");
    addStringToStuffBuff(&sb, "</b></font>\n");

    START_PACKET(pak, e, SERVER_BROWSER_TEXT);
    pktSendString(pak, sb.buff);
    END_PACKET

    freeStuffBuff(&sb);
}

/* End of File */
