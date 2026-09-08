/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/components/StashTable.h>

#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/language/AppLocale.h>
#include "filter/profanity.h"
#include <utilitieslib/utils/SimpleParser.h>

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/powers.h"
#include "entity/character_base.h"
#include <utilitieslib/network/net_structdefs.h>
#include "svr/svr_player.h"
#include "language/langServerUtil.h"
#include "cmdparse/cmdcommon.h"
#include <utilitieslib/utils/timing.h>
#include "entity/entGameActions.h"
#include "storyarc/storyarcinterface.h"
#include "storyarc/storyarcutil.h"
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/scriptvars.h>
#include <utilitieslib/language/MessageStore.h>
#include "generator/encounter.h"

static char **s_NPCChatter;
static char **s_NPCChatterHero;
static char **s_NPCChatterVillain;

typedef struct MapChat 
{
    char **ppchChatter[3];
} MapChat;

StashTable g_hashMapsToChat;

MP_DEFINE(MapChat);


/**********************************************************************func*
 * ReadChatterFile
 *
 */
static void ReadChatterFile(char *pch)
{
    int len;
    char *mem = fileAlloc(pch, &len);
    char *s, *walk=mem;
    int pos=0;
    int line=0;
    int iChatType = 2;
    bool bLastWasMap = false;
    char **ppchMaps;
    StashElement elem;
    MapChat *pmc;
    const char UTF8Sig[3] = { 0xef, 0xbb, 0xbf };

    eaCreate(&ppchMaps);

    stashFindElement(g_hashMapsToChat, "*", &elem);
    if(!elem)
    {
        MP_CREATE(MapChat, 50);
        pmc = MP_ALLOC(MapChat);
        stashAddPointer(g_hashMapsToChat, "*", pmc, false);
    }
    eaPush(&ppchMaps, "*");

    while (walk && walk < mem + len)
    {
        s = walk;
        walk = strchr(s, '\r');
        if (!walk) break;
        while (*walk=='\r' || *walk=='\n') walk++;
        s = strtok(s, "\r\n");
        line++;
        if (s)
        {
            // check for UTF-8 code and remove if found
            if (!strncmp(s, UTF8Sig, 3))
            {
                s += 3;
            }

            if(strncmp(s, "//", 2)==0)
            {
                continue;
            }
            else if(*s == '[')
            {
                if(stricmp(s, "[hero]")==0)
                {
                    bLastWasMap = false;
                    iChatType = 0;
                }
                else if(stricmp(s, "[villain]")==0)
                {
                    bLastWasMap = false;
                    iChatType = 1;
                }
                else if(stricmp(s, "[any]")==0
                    || stricmp(s, "[all]")==0)
                {
                    bLastWasMap = false;
                    iChatType = 2;
                }
                else if(strnicmp(s, "[map", 4)==0)
                {
                    if(!bLastWasMap)
                    {
                        eaSetSize(&ppchMaps, 0);
                    }

                    if(strnicmp("any", s+5, 3)==0
                        || strnicmp("all", s+5, 3)==0)
                    {
                        eaPush(&ppchMaps, "*");
                    }
                    else
                    {
                        char *pos = strchr(s, ']');
                        if(pos) *pos='\0';
                        eaPush(&ppchMaps, s+5);
                    }

                    iChatType = 2;
                    bLastWasMap = true;
                }
                else if(stricmp(s, "[end]")==0)
                {
                    bLastWasMap = false;
                    eaSetSize(&ppchMaps, 0);
                    eaPush(&ppchMaps, "*");
                    iChatType = 2;
                }
            }
            else
            {
                bLastWasMap = false;

                if(getCurrentLocale()==0)
                {
                    verifyPrintable(s, pch, line);
                }

                if(!IsProfanity(s, (int)strlen(s)) && !isEmptyStr(s))
                {
                    int n = eaSize(&ppchMaps);
                    int i;
                    for(i=0; i<n; i++)
                    {
                        stashFindElement(g_hashMapsToChat, ppchMaps[i], &elem);
                        if(!elem)
                        {
                            MP_CREATE(MapChat, 50);
                            pmc = MP_ALLOC(MapChat);
                            stashAddPointer(g_hashMapsToChat, ppchMaps[i], pmc, false);
                        }
                        else
                        {
                            pmc = (MapChat *)stashElementGetPointer(elem);
                        }

                        eaPush(&(pmc->ppchChatter[iChatType]), s);
                    }
                }
            }
        }
    }

    eaDestroy(&ppchMaps);

    // Don't free this, the memory is referenced in the EArray now.
    //  to free it later, probably have to either store the pointer or
    //  free(pearray[0])
    //fileFree(mem);
}


/**********************************************************************func*
 * npcLoadChatter
 *
 */
void npcLoadChatter(void)
{
    char pchPath[MAX_PATH];
    char pch[MAX_PATH];
    const char **ppchPrefix;

    if(!g_hashMapsToChat)
        g_hashMapsToChat = stashTableCreateWithStringKeys(20, StashDeepCopyKeys);

    strcpy(pchPath, "/texts/");
    strcat(pchPath, locGetName(locGetIDInRegistry()));
    strcat(pchPath, "/server/");

    strcpy(pch, pchPath);
    strcat(pch, "npc_chatter.txt");
    ReadChatterFile(pch);

    // load each of the optional versions
    ppchPrefix = g_StdAdditionalFilePrefixes;
    while(**ppchPrefix)
    {
        char *pchPrefixName = addFilePrefix(pch, *ppchPrefix);
        if (fileExists(pchPrefixName))
        {
            ReadChatterFile(pchPrefixName);
        }
        ppchPrefix++;
    }
}

/**********************************************************************func*
 * RandomPlayer
 *
 */
static Entity *RandomPlayer(void)
{
    int i;

    if(player_ents[PLAYER_ENTS_ACTIVE].count <= 0)
    {
        return NULL;
    }

    i = rand() % player_ents[PLAYER_ENTS_ACTIVE].count;
    return player_ents[PLAYER_ENTS_ACTIVE].ents[i];
}

/**********************************************************************func*
 * RandomPower
 *
 */
static Power *RandomPower(Character *p)
{
    int iNumSets, iNumPows;
    int iset;
    int ipow;
    iNumSets = eaSize(&p->ppPowerSets);
    if (!iNumSets)
        return NULL;
    iset = rand()%iNumSets;
    iNumPows = eaSize(&p->ppPowerSets[iset]->ppPowers);
    if (!iNumPows)
        return NULL;
    ipow = rand()%iNumPows;
    return p->ppPowerSets[iset]->ppPowers[ipow];
}

/**********************************************************************func*
 * RandomChatter
 *
 */
static char *RandomChatter(Entity *e, Entity *ePlayer)
{
    int i;
    int iBanks;
    StashElement elem;
    MapChat *pmcGlobal = NULL;
    MapChat *pmcMap = NULL;
    int iCntType = 0;
    int iCntGeneral = 0;
    int iCntMapGeneral = 0;
    int iCntMapType = 0;

    stashFindElement(g_hashMapsToChat, "*", &elem);
    if(elem)
    {
        pmcGlobal = (MapChat *)stashElementGetPointer(elem);
        iCntGeneral = eaSize(&pmcGlobal->ppchChatter[2]);
        iCntType = eaSize(&pmcGlobal->ppchChatter[ePlayer->pl->playerType]);
    }

    stashFindElement(g_hashMapsToChat, db_state.map_name, &elem);
    if(elem)
    {
        pmcMap = (MapChat *)stashElementGetPointer(elem);
        iCntMapGeneral = eaSize(&pmcMap->ppchChatter[2]);
        iCntMapType = eaSize(&pmcMap->ppchChatter[ePlayer->pl->playerType]);
    }

    if(iCntType+iCntGeneral+iCntMapType+iCntMapGeneral==0) return NULL;

    // I trigger off of the NPC's first letter to determine which thing
    // to say. Since there are more sayings than letters of the alphabet,
    // I rotate through them every 2 minutes.

    iBanks = ((iCntType+iCntGeneral+iCntMapType+iCntMapGeneral)/15)+1;
    i = (timerSecondsSince2000()/120)%iBanks;

    // i is now which "bank" of 15 lines I want to choose from

    // I'm going to assume that owner%15 is equal probability for each
    // I want the (owner%15)th line from the ith bank. Each bank is 15 long
    i = (e->owner%15)*iBanks+i;
    i = i % (iCntType+iCntGeneral+iCntMapType+iCntMapGeneral);  // in case the last bank is not full

    if(i<iCntGeneral+iCntType)
    {
        if(i<iCntType)
        {
            return pmcGlobal->ppchChatter[ePlayer->pl->playerType][i%iCntType];
        }
        else
        {
            i-=iCntType;
            return pmcGlobal->ppchChatter[2][i%iCntGeneral];
        }
    }
    else
    {
        i -= iCntGeneral+iCntType;
        if(i<iCntMapType)
        {
            return pmcMap->ppchChatter[ePlayer->pl->playerType][i%iCntMapType];
        }
        else
        {
            i-=iCntMapType;
            return pmcMap->ppchChatter[2][i%iCntMapGeneral];
        }
    }
}

/**********************************************************************func*
 * npcInteract
 *
 */
void npcInteract(Entity *npc, Entity *e)
{
    char ach[1024];
    char ach2[1024];
    time_t t;
    Entity *e2;

    time(&t);
    strcpy(ach, ctime(&t));
    strncpy(ach2, ach+11, 5);

    ach[0]='\0';

    // let the RandomFame system replace chatter if it wants to
    //RandomFameSay(e, ach);

    // otherwise, do normal thing
    if (ach[0] == 0)
        switch(npc->name[0] % 23)
    {
        case 0:
            strcpy( ach, localizedPrintf(e, "NPCRealTime", ach2));
            break;

        case 1:
            strcpy( ach, localizedPrintf(e, "NPCCityTime", (int)server_visible_state.time, (int)(fmod(server_visible_state.time,1) * 60)));
            break;

        case 2:
            if(player_ents[PLAYER_ENTS_ACTIVE].count<10)
            {
                strcpy( ach, localizedPrintf(e, "NPCNeverAround"));
            }
            else
            {
                strcpy( ach, localizedPrintf(e, "NPCCountHeroes", player_ents[PLAYER_ENTS_ACTIVE].count));
            }
            break;

        case 3:
            if((e2=RandomPlayer())!=NULL && e2->name[0])
            {
                strcpy( ach, localizedPrintf(e, "NPCComplimentRandom", e2->name));
            }
            break;

//        case 4:
//            if((e2=RandomPlayer())!=NULL)
//            {
//                Character *p = e2->pchar;
//                Power *pow = RandomPower(p);
//                if (pow==NULL)
//                    break;
//
//                msxPrintf(svrMenuMessages, ach, e->playerLocale, HVMsg(e, "NPCSawPower"), e2->name, pow->ppowBase->pchDisplayName);
//            }
//            break;

//        case 5:
//            {
//                Character *p = e->pchar;
//                Power *pow = RandomPower(p);
//                if (pow==NULL)
//                    break;
//
//                if(pow->stats.iCntUsed>10)
//                    msxPrintf(svrMenuMessages, ach, e->playerLocale, HVMsg(e, "NPCUsedPower"), pow->ppowBase->pchDisplayName, pow->stats.iCntUsed);
//            }
//            break;

//        case 6:
//            {
//                Character *p = e->pchar;
//                Power *pow = RandomPower(p);
//                if (pow==NULL)
//                    break;
//
//                if(pow->stats.fDamageGiven > 100)
//                {
//                    msxPrintf(svrMenuMessages, ach, e->playerLocale, HVMsg(e, "NPCDamagePower"), (int)pow->stats.fDamageGiven, pow->ppowBase->pchDisplayName);
//                }
//            }
//            break;

//        case 7:
//            {
//                Character *p = e->pchar;
//                Power *pow = RandomPower(p);
//                if (pow==NULL)
//                    break;
//
//                if(pow->stats.iCntUsed==0)
//                {
//                    // Just get the default message
//                }
//                else if(pow->stats.iCntHits<pow->stats.iCntMisses)
//                {
//                    msxPrintf(svrMenuMessages, ach, e->playerLocale, HVMsg(e, "NPCMissedALot"), pow->ppowBase->pchDisplayName);
//                }
//                else
//                {
//                    msxPrintf(svrMenuMessages, ach, e->playerLocale, HVMsg(e, "NPCHitALot"), pow->ppowBase->pchDisplayName);
//                }
//            }
//            break;

        case 8:
            if(((int)e->total_time/60) < 60)
            {
                strcpy( ach, localizedPrintf(e, "NPCTotalMinutes", e->total_time/60));
            }
            else
            {
                strcpy( ach, localizedPrintf(e, "NPCTotalHours", e->total_time/60/60));
            }
            break;

        case 9:
            strcpy( ach, localizedPrintf(e, "NPCTotalLogins", e->login_count));
            break;

        case 10:
            if(((timerSecondsSince2000()-e->on_since)/60) < 60)
            {
                strcpy( ach, localizedPrintf(e, "NPCSessionMinutes", (timerSecondsSince2000()-e->on_since)/60));
            }
            else
            {
                strcpy( ach, localizedPrintf(e, "NPCSessionHours", (timerSecondsSince2000()-e->on_since)/60/60));
            }
            break;
    }

    if(ach[0]=='\0') 
    {
        char *pch = RandomChatter(npc, e);
        if(!pch)
        {
            strncpy(ach, localizedPrintf(e, "NiceDay"), 1024);
        }
        else
        {
            strncpy(ach, pch, 1024);
        }
    }

    
    {
        ScriptVarsTable vars = {0};

        npc->audience = erGetRef(e);
        if(EncounterVarsFromEntity(npc))
            ScriptVarsTableScopeCopy(&vars, EncounterVarsFromEntity(npc));
        saBuildScriptVarsTable(&vars, e, SAFE_MEMBER(e, db_id), 0, NULL, NULL, NULL, NULL, NULL, 0, 0, false);
        saUtilEntSays(npc, kEntSay_NPCDirect, saUtilTableAndEntityLocalize(ach, &vars, e));
    }
    

    //    serveFloater(e, npc, ach, 0.0f, kFloaterStyle_Chat, 0);
}

/* End of File */
