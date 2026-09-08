#include "UI/uiRaidResult.h"
#include <utilitieslib/network/net_packet.h>
#include <utilitieslib/network/net_packetutil.h>
#include "gameComm/wdwbase.h"
#include "UI/uiWindows.h"
#include "UI/uiUtilGame.h"
#include "UI/uiUtil.h"
#include <utilitieslib/utils/qsortG.h>
#include "entity/gametypes.h"
#include <utilitieslib/components/earray.h>
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiScrollBar.h"

// strings used in this window (excluding the why string sent from the server)
static char * attackersString        = "RaidAttackersString";
static char * defendersString        = "RaidDefenderssString";
static char * defeatsString            = "RaidDefeatsString";
static char * timesDefeatedString    = "RaidTimesDefeatedString";

typedef struct RaidPlayer
{
    char name[MAX_NAME_LEN];    // player's name
    int defeats;                // number of times they defeated someone else
    int defeated;                // number of times someone else defeated them
} RaidPlayer;

int compareRaidPlayers(const void* aData, const void* bData)
{
    const RaidPlayer ** a = (const RaidPlayer **)aData;
    const RaidPlayer ** b = (const RaidPlayer **)bData;
    if ((*a)->defeats!=(*b)->defeats)
        return (*b)->defeats-(*a)->defeats;        // more defeats puts you higher in the list
    if ((*a)->defeated!=(*b)->defeated)
        return (*a)->defeated-(*b)->defeated;    // fewer times defeated puts you higher in the list
    return stricmp((*a)->name,(*b)->name);
}

typedef struct RaidGroup
{
    char * sgname;            // name of the supergroup
    RaidPlayer ** group;    // all the participants in the raid
} RaidGroup;

RaidGroup allPlayers[2];    // allPlayers[0]==attackers, allPlayers[1]==defenders
char * reason;
int attackerswin;

void uiRaidResultReceive(Packet * pak)
{
    int i;
    char * temp;
    int len;
    if (window_getMode(WDW_RAIDRESULT) != WINDOW_DISPLAYING)
    {
        window_setMode(WDW_RAIDRESULT,WINDOW_DISPLAYING);
    }

    if (reason!=NULL)
    {
        free(reason);
        for (i=0;i<2;i++)
        {
            free(allPlayers[i].sgname);
            eaDestroyEx(&allPlayers[i].group,NULL);
        }
    }

    temp=strdup(pktGetString(pak));
    reason=malloc(strlen("Raid")+strlen(temp)+1);
    sprintf(reason,"Raid%s",temp);
    len=strlen(reason);
    for (i=0;i<len;i++)
        if (reason[i]==' ')
            reason[i]='_';

    attackerswin=pktGetBits(pak,1);

    for (i=0;i<2;i++)
    {
        int j=pktGetBitsAuto(pak);
        while (j--)
        {
            RaidPlayer * rp=malloc(sizeof(RaidPlayer));
            sprintf(rp->name,"%s",pktGetString(pak));
            if (rp->name[0]==0)
            {
                free(rp);
                continue;
            }
            rp->defeats =pktGetBitsAuto(pak);
            rp->defeated=pktGetBitsAuto(pak);
            eaPush(&allPlayers[i].group,rp);
        }
        qsortG(allPlayers[i].group,eaSize(&allPlayers[i].group),sizeof(*(allPlayers[i].group)),compareRaidPlayers);
    }


}

static int colors[4]={0x3f00007f,0x7f00007f,0x00003f7f,0x00007f7f};

int uiRaidResultDisplay()
{
    float x, y, z, wd, ht, sc;
    int i;
    int color, bcolor;
    int maxht=400;
    int totht;
    int offset=0;
    static ScrollBar scrollbar;
    int lh=20;
    CBox box;

    if (window_getMode(WDW_RAIDRESULT) != WINDOW_DISPLAYING)
        return 0;

    if(!window_getDims(WDW_RAIDRESULT, &x, &y, &z, &wd, &ht, &sc, &color, &bcolor))
        return 0;

    font(&game_14);
    font_color(CLR_WHITE, CLR_WHITE);

    ht=25*sc+lh*sc*(eaSize(&allPlayers[0].group)+eaSize(&allPlayers[1].group)+3);

    totht=ht;
    if (ht>maxht)
    {
        ht=maxht;
        window_setDims(WDW_RAIDRESULT, x, y, wd, ht);
    }
    drawFrame(PIX3, R10, x, y, z-1000, wd, ht, sc, color, 0x0000009f );
 
    if (totht>maxht)
    {
        scrollbar.wdw=WDW_RAIDRESULT;
        doScrollBar(&scrollbar,maxht-35*sc,totht-45*sc,x,20*sc,z,0,0);
        offset=scrollbar.offset;
    }
    set_scissor(1);
    scissor_dims(x+3*sc,y+5*sc,wd-5*sc,ht-10*sc);

    x+=10*sc;
    y+=25*sc;
    y-=offset;

    cprnt(x+wd/2-15*sc,y,z,sc,sc,reason);

    y+=lh*sc;
    for (i=attackerswin?0:1;(attackerswin)?(i<2):(i>=0);(attackerswin)?(i++):(i--))
    {
        int j;
        int col1=0*sc;
        int col2=250*sc;
        int col3=370*sc;
        prnt(x+col1,y,z,sc,sc,i==0?attackersString:defendersString);
        prnt(x+col2-str_wd(&game_14,sc,sc,defeatsString),y,z,sc,sc,defeatsString);
        prnt(x+col3-str_wd(&game_14,sc,sc,timesDefeatedString),y,z,sc,sc,timesDefeatedString);

        BuildCBox(&box,x-10*sc,y-(lh-2)*sc,wd-1*sc,lh*sc);
        drawBox(&box,z,0,i==0?0xff00007f:0x0000ff7f);

        y+=lh*sc;
        for (j=0;j<eaSize(&allPlayers[i].group);j++)
        {

            prnt(x+col1,y,z,sc,sc,"%s",allPlayers[i].group[j]->name);
            prnt(x+col2-str_wd(&game_14,sc,sc,"%d",allPlayers[i].group[j]->defeats),y,z,sc,sc,"%d",allPlayers[i].group[j]->defeats);
            prnt(x+col3-str_wd(&game_14,sc,sc,"%d",allPlayers[i].group[j]->defeated),y,z,sc,sc,"%d",allPlayers[i].group[j]->defeated);
            BuildCBox(&box,x-10*sc,y-(lh-2)*sc,wd-1*sc,lh*sc);
            drawBox(&box,z,0,colors[((i%2)<<1)+(j%2)]);
            y+=lh*sc;
        }
    }
    set_scissor(0);

    return 0;
}

