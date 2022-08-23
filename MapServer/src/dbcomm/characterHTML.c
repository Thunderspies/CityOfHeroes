/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/components/bitfield.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/components/StashTable.h>

#include "dbcomm.h"
#include "dbquery.h"
#include "varutils.h"
#include "container/containerloadsave.h"
#include "language/langServerUtil.h"
#include "svr/svr_player.h"

#include "entity/entity.h"
#include "entity/entPlayer.h"
#include "entity/character_base.h"
#include "entity/classes.h"
#include "entity/origins.h"
#include "entity/powers.h"
#include "entity/boostset.h"
#include "entity/costume.h"
#include "kiosk.h"
#include "gameComm/VillainDef.h"
#include "entity/attrib_names.h"
#include "player/badges.h"
#include "language/commonLangUtil.h"
#include <utilitieslib/language/MessageStoreUtil.h>

#include "characterInfo.h"

#include "imageServer.h"
#include "comm_backend.h"
#include "player/pl_stats_internal.h"
#include "entity/friendCommon.h"
#include "entity/Supergroup.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/components/HashFunctions.h>

#define BADGES
#define UPDATE2
#define STATS

// If this is enabled the XML will contain syntax for multiple builds,
// otherwise it will stay in the old single-build syntax.
// #define MULTIPLE_BUILDS_XML        1

#define strdupa(str) strcpy(_alloca(strlen(str)+1), str);

int GenCharacterByName(char *pchName, FILE *fp, bool bUseCaption, bool showBanned, bool forceCostumeRefresh);
int GenCharacter(int dbid, FILE *fp, bool bUseCaption, bool showBanned);
bool CheckCostumeChange(char *pchSafeName, Costume *pCostume, int costumeNum);

static StashTable s_hashSupergroups;

static char *EscapeApostrophes(char *pch);
static char *PutInCommas(int iVal);
static char *TranslateStat(Entity *e, const char *pch, bool *pbShow);
static char *ImgSrc(const char *pch);
static char *SecondsToString(U32 i);
static char *GetSupergroupName(Entity *e);
static char *ReplaceVars(char **pestr, char *pchSrc, char *pchHeroName, char *pchShardName, char *pchHeroHTML);

static int GenCostumeCSV(Entity *e, char *pchBaseName, bool useCaption, bool isBanned);

static int GenXML(Entity *e, char *pchDest);
static void PowersXML(char **pestr, Entity *e, PowerSet *pset);
static void EnhancementXML(Entity *e, char **pestr, Boost *boost, int level, int iNumCombines, int iCharLevel, char *tabs);


#ifdef BADGES
static void BadgeSetXML(char **pestr, Entity *e, BadgeType eCat);
#endif

#ifndef UPDATE2
static int dbQueryGetContainerIDForName(int type, char *db_id);
#endif

int CountBadgesForEnt(Entity *e)
{
    int iCnt = 0;
    if(e && e->pl)
    {
        int i;
        int iSize = eaSize(&g_BadgeDefs.ppBadges);
        for(i = 0; i < iSize; i++)
        {
            if(g_BadgeDefs.ppBadges[i]->eCategory!=kBadgeType_Internal
                && !g_BadgeDefs.ppBadges[i]->bDoNotCount
                && g_BadgeDefs.ppBadges[i]->iIdx<=g_BadgeDefs.idxMax)
            {
                if(BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, g_BadgeDefs.ppBadges[i]->iIdx))
                {
                    iCnt++;
                }
            }
        }
    }

    return iCnt;
}

// defined in friends.c
int updateFriendList( Entity *e );

char *GenFriendsListForEnt(Entity *e)
{
    char *friendsList = NULL;
    int i;

    if ( e->friendCount == 0 )
        return NULL;
    
    estrCreate(&friendsList);
    updateFriendList(e);
    estrConcatf(&friendsList, "%d", e->friends[0].dbid);

    for ( i = 1; i < e->friendCount; ++i )
    {
        estrConcatf(&friendsList, "|%d", e->friends[i].dbid);
    }

    return friendsList;
}

void GenFriendXML( char **pestrHeroXML, Entity *e )
{
    int i;
    char achSafeName[MAX_PATH];
    char costumext[MAX_PATH];
    
    if ( e->friendCount == 0 )
        return;
    
    updateFriendList(e);

    for ( i = 0; i < e->friendCount; ++i )
    {
        strcpy(achSafeName, e->friends[i].name);
        imageserver_MakeSafeFilename(achSafeName);

        estrConcatf(pestrHeroXML, "\t\t<FRIEND ID=\"%d\">\n",e->friends[i].dbid);
        estrConcatf(pestrHeroXML, "\t\t\t<FRIENDNAME><![CDATA[%s]]></FRIENDNAME>\n",e->friends[i].name);
        estrConcatf(pestrHeroXML, "\t\t\t<FRIENDSAFENAME><![CDATA[%s]]></FRIENDSAFENAME>\n",achSafeName);
        sprintf_s(SAFESTR(costumext), "_Thumbnail.jpg");
        estrConcatf(pestrHeroXML, "\t\t\t<THUMBNAIL><![CDATA[%s]]></THUMBNAIL>\n", MakePictureURL(achSafeName, costumext));
        estrConcatCharString(pestrHeroXML, "\t\t</FRIEND>\n");
    }
}

/**********************************************************************func*
 * GenCharacterByName
 *
 */
int GenCharacterByName(char *pchName, FILE *fp, bool bUseCaption, bool showBanned, bool forceCostumeRefresh)
{
    int dbid;

    dbid = dbQueryGetContainerIDForName(3, pchName);

    if (forceCostumeRefresh)
    {
        ResetCostumeCRCs(dbid);
    }

    return GenCharacter(dbid, fp, bUseCaption, showBanned);
}

/**********************************************************************func*
 * GenCharacter
 *
 */
int GenCharacter(int dbid, FILE *fp, bool bUseCaption, bool showBanned)
{
    char *pch;
    char pchCmdBuf[128] = "";
    int iRet = 0;

    sprintf_s(SAFESTR(pchCmdBuf), "-dbquery -timeout 120 -get 3 %d", dbid);
    pch = dbQuery(pchCmdBuf);
    if(pch)
    {
        if (stricmp(pch, "(dbQuery failed)")==0)
        {
            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                "DBServer query error for DB ID %d - closing connection\n",
                dbid);

            iRet = -1;
        }
        else if(simpleMatch("container * doesnt exist\n", pch))
        {
            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                "DBServer returned no data for DB ID %d\n", dbid);

            iRet = 1;
        }
        else
        {
            char achDestFile[MAX_PATH];

            Entity *e = entAlloc();
            SET_ENTTYPE(e) = ENTTYPE_PLAYER;
            e->db_id = dbid;
            entTrackDbId(e);
            playerVarAlloc(e, ENTTYPE(e));

            unpackEnt(e, pch);

            // Added a check to make sure we don't try to generate pages for
            // characters with messed up powers, because they will be missing
            // the appropriate data structures.
            if (!iRet && !e->pchar || !e->pchar->entParent)
            {
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "Invalid powers detected for DB ID %d\n", dbid);

                iRet = 1;
            }

            if (iRet)
            {
                entUntrackDbId(e);
                playerVarFree(e);
                entFreeCommon(e);

                return iRet;
            }

            // Guard against corrupt gender in the database. It will
            // crash the page generator when processing badges with
            // gender-specific titles otherwise.
            if (e->gender < GENDER_NEUTER || e->gender > GENDER_FEMALE)
            {
                if (e->name_gender >= GENDER_NEUTER &&
                    e->name_gender <= GENDER_FEMALE)
                {
                    e->gender = e->name_gender;
                }
                else
                {
                    e->name_gender = GENDER_MALE;
                    e->gender = GENDER_MALE;
                }
            }

            // Guard against bad locale in the database as well. Locales
            // which we don't have message files for will crash the page
            // generator.
            if (!locIsUserSelectable(e->playerLocale))
            {
                e->playerLocale = LOCALE_ID_ENGLISH;
            }

            if(!e->name || *e->name=='\0')
            {
                printf("*** Error: Unable to fetch character properly!\n");
                filelog_printf(g_CharacterInfoSettings.achLogFileName,
                    "DBServer returned invalid data for DB ID %d\n", dbid);

                iRet = 1;
            }
            else
            {
                static PerformanceInfo* perfInfo;
                char achSafeName[MAX_PATH];
                bool bBanned = false;
                ExistInfo *pei = NULL;
                char *old_name = NULL;
                strcpy(achSafeName, e->name);
                imageserver_MakeSafeFilename(achSafeName);

                old_name = UpdateExistsInfo(achSafeName, e->db_id, e->last_time);
                SetExistsInfoGenerated(achSafeName, e->db_id);

                printf("  Generating for %s (%s)\n", e->name, achSafeName);

                strcpy(achDestFile, MakeCharacterFilename(achSafeName, ".xml"));

                mkdirtree(achDestFile);

                if (e->pl->banned || IsAuthnameOnBannedList(e->auth_name))
                {
                    bBanned = true;
                }

                if(bBanned && !showBanned)
                {
                    // Delete character webpage
                    unlink(achDestFile);

                    // Delete character picture
                    strcpy(achDestFile, MakePictureFilename(achSafeName, ".jpg"));
                    unlink(achDestFile);

                    // Add to banned list for later logfiling
                     LogBannedCharacter(e->name);
                }
                else
                {
                    if(!g_CharacterInfoSettings.bNoHTML)
                        iRet |= GenXML(e, achDestFile);

                    iRet |= GenCostumeCSV(e, achSafeName, bUseCaption, bBanned);
                }

                if(fp)
                {
                    char achDate[64];
                    timerMakeDateStringFromSecondsSince2000(achDate, e->last_time);

                    // for rename: remove existing
                    if(old_name)
                    {
                        fprintf(fp,"%s,%s,,,,,,,,,,,%s,,,1\n",
                                old_name,
                                g_CharacterInfoSettings.achShardName,
                                MakeCharacterURL(old_name, ".html"));
                    }

                    //0-4: SafeName,Shard,Name,CharacterID,PlayerType
                    fprintf(fp,"%s,%s,%s,%d,%s",
                        achSafeName,
                        g_CharacterInfoSettings.achShardName,
                        e->name,
                        e->db_id,
                        ENT_IS_HERO(e) ? "Hero" : "Villain");
                    //5: Origin
                    fprintf(fp, ",%s", localizedPrintf(e,e->pchar->porigin->pchDisplayName));
                    //6: Archetype
                    fprintf(fp, ",%s", localizedPrintf(e,e->pchar->pclass->pchDisplayName));
                    //7-12: Level,SupergroupID,SuperGroupName,LastActiveTimeSecs,LastActive
                    fprintf(fp,",%d,%d,%s,%d,%s",
                        e->pchar->iLevel+1,
                        e->supergroup_id,
                        GetSupergroupName(e),
                        e->last_time,
                        achDate);
                    //URL,PageUpdateTimeSecs,PageUpdateTime,Deleted
                    fprintf(fp,",%s,%d,%s,%d",
                        MakeCharacterURL(achSafeName, ".html"),
                        g_CharacterInfoSettings.uLastUpdate,
                        g_CharacterInfoSettings.achLastUpdate,
                        bBanned?1:0);                    

                    //XPEarned,Influence,Healing
                    fprintf(fp,",%d,%d,%d",
                        e->pchar->iExperiencePoints,
                        e->pchar->iInfluencePoints,
                        stat_Get(e->pl, 0, kStat_General, "healing_given"));

                        {
                            U32 pvpKills = stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Kills, "pvp"),
                                pvpDeaths = stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Deaths, "pvp");
                            //PvEVictories,PvEDefeats,PvPVictories,PvPDefeats,PvPReputation
                            fprintf(fp, ",%d,%d,%d,%d,%.2f",
                                stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Kills, "overall") - pvpKills,
                                stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Deaths, "overall") - pvpDeaths,
                                pvpKills,
                                pvpDeaths,
                                e->pl->reputationPoints);
                        }

                        {
                            char *friends = GenFriendsListForEnt(e);

                            //BadgesEarned,HoursPlayed,Friends
                            fprintf(fp, ",%d,%d,%s\n",
                                CountBadgesForEnt(e),
                                (e->total_time/60)/60,
                                friends ? friends : "" );

                            estrDestroy(&friends);
                        }
                }

            }

            entUntrackDbId(e);
            playerVarFree(e);
            entFreeCommon(e);
        }
    }

    return iRet;
}


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

/**********************************************************************func*
 * GenCostumeCSV
 *
 */
#include "imageServer.h"
ImageServerParams costume_params = 
{
    200,
    400,
    0xffffff,
    "Portrait_bkgd_contact.tga",
    "",
    "",
    FALSE,
    TRUE
};

static int GenCostumeCSV(Entity *e, char *pchBaseName, bool useCaption, bool isBanned)
{
    char achSafeName[MAX_PATH];
    char achCSVPath[MAX_PATH];
    char ext[MAX_PATH];
    int ret = 0, j;
    int numSlots = costume_get_num_slots(e);
    bool redo_thumbnail = false;

    // 0 costume slots actually means 1 costume slot (numSlots are actually numExtraSlots)
    //if (!numSlots)
    //    numSlots = 1;

    if ( e->pl )
    {
        if (ENT_IS_HERO(e))
            strcpy_s(SAFESTR(costume_params.bgtexture), "V_Portrait_bkgd_blue.tga");
        else
            strcpy_s(SAFESTR(costume_params.bgtexture), "V_Portrait_bkgd_red.tga");
    }

    costume_params.x_res = 200;
    costume_params.y_res = 400;

    for (j = 0; j < numSlots; j++)
    {
        // We need the output filename so we can check if the file exists.
        strcpy(achSafeName, e->name);

        imageserver_MakeSafeFilename(achSafeName);

        sprintf(achCSVPath, "%s/%s_costume%d.csv", g_CharacterInfoSettings.achCostumeRequestPath, achSafeName, j+1);
        ConvertFilenameToLowercase(achCSVPath);

        sprintf(ext, "_costume%d.jpg", j+1);

        strcpy(achSafeName, MakePictureFilename(achSafeName, ext));
        mkdirtree(achSafeName);
        strcpy(costume_params.outputname, achSafeName);

        // This call will update the costume CRC as well as checking it.
        if (!CheckCostumeChange(pchBaseName, e->pl->costume[j], j))
            continue;

        // Thumbnail is made from the first costume slot, so if that changes
        // we must also rebuild the thumbnail graphic as well.
        if (j == 0)
            redo_thumbnail = true;

        // Rebuild the safe name for our output message.
        strcpy(achSafeName, e->name);

        imageserver_MakeSafeFilename(achSafeName);

        // Generate costume body shot request for the image server.
        printf("    Generating costume %s_costume%d for %s\n", achSafeName, j+1, e->name);
        
        if (useCaption)
        {
            strncpy(costume_params.caption, e->name, MAX_CAPTION-1);
            if (isBanned)
                strncat(costume_params.caption, " (BANNED)", MAX_CAPTION-1);
        }
        else
            costume_params.caption[0] = 0;

        if (!imageserver_WriteToCSV(costume_as_const(e->pl->costume[j]), &costume_params, achCSVPath))
            ret=1;
    }

    // Build the thumbnail filename so we can check if it exists.
    strcpy(achSafeName, e->name);
    imageserver_MakeSafeFilename(achSafeName);
    sprintf(ext, "_thumbnail.jpg");
    strcpy(achSafeName, MakePictureFilename(achSafeName, ext));
    mkdirtree(achSafeName);
    strcpy(costume_params.outputname, achSafeName);

    // make the headshot thumbnail
    if (redo_thumbnail)
    {
        costume_params.x_res = 80;
        costume_params.y_res = 80;

        strcpy(achSafeName, e->name);
        imageserver_MakeSafeFilename(achSafeName);
        sprintf(achCSVPath, "%s/%s_thumbnail.csv", g_CharacterInfoSettings.achCostumeRequestPath, achSafeName);
        ConvertFilenameToLowercase(achCSVPath);
        printf("    Generating costume %s_thumbnail for %s\n", achSafeName, e->name);
        sprintf(ext, "_Thumbnail.jpg");
        strcpy(achSafeName, MakePictureFilename(achSafeName, ext));
        mkdirtree(achSafeName);
        strcpy(costume_params.outputname, achSafeName);
        costume_params.headshot = TRUE;
        if (useCaption)
        {
            strncpy(costume_params.caption, e->name, MAX_CAPTION-1);
            if (isBanned)
                strncat(costume_params.caption, " (BANNED)", MAX_CAPTION-1);
        }
        else
            costume_params.caption[0] = 0;

        if (!imageserver_WriteToCSV(costume_as_const(e->pl->costume[0]), &costume_params, achCSVPath))
            ret=1;
        costume_params.headshot = FALSE;
    }

    return ret;
}


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

int GetShardIdFromName(char *shardName)
{
    static StashTable shardnames = NULL;
    int ret = -1;

    if ( !shardnames )
    {
        shardnames = stashTableCreateWithStringKeys(15, StashDefault);
        stashAddInt(shardnames, "freedom", 0, 1);
        stashAddInt(shardnames, "justice", 1, 1);
        stashAddInt(shardnames, "pinnacle", 2, 1);
        stashAddInt(shardnames, "virtue", 3, 1);
        stashAddInt(shardnames, "liberty", 4, 1);
        stashAddInt(shardnames, "guardian", 5, 1);
        stashAddInt(shardnames, "infinity", 6, 1);
        stashAddInt(shardnames, "protector", 7, 1);
        stashAddInt(shardnames, "victory", 8, 1);
        stashAddInt(shardnames, "champion", 9, 1);
        stashAddInt(shardnames, "triumph", 10, 1);
        stashAddInt(shardnames, "ustrainingroom", 11, 1);
        stashAddInt(shardnames, "usqa_freedom", 12, 1);
        stashAddInt(shardnames, "defiant", 13, 1);
        stashAddInt(shardnames, "vigilance", 14, 1);
        stashAddInt(shardnames, "zukunft", 15, 1);
        stashAddInt(shardnames, "union", 16, 1);
        stashAddInt(shardnames, "eutrainingroom", 17, 1);
        stashAddInt(shardnames, "euqa_defiant", 18, 1);
        stashAddInt(shardnames, "euqa_vigilance", 19, 1);
        stashAddInt(shardnames, "euqa_zukunft", 20, 1);
    }

    if ( stashFindInt(shardnames, shardName, &ret) )
        return ret;
    else
    {
        // Compress the hashed string range so we can tell it apart from
        // ordinary values.
        ret = 100 + abs(burtlehash3(shardName, (ub4)strlen(shardName), DEFAULT_HASH_SEED) % 200000000);
    }

    return ret;
}

static void convertFilenameTGAtoGIF(const char *pchSrc, char *pchDest)
{
    char *extPos;

    strcpy_s(pchDest, MAX_PATH, pchSrc);

    extPos = strrchr(pchDest, '.');

    if (extPos)
    {
        strcpy_s(extPos + 1, 4, "gif");
    }
    else
    {
        strcat(pchDest, ".gif");
    }
}

static int GenXML(Entity *e, char *pchDest)
{
    char achSafeName[MAX_PATH];
    char achGifFileName[MAX_PATH];
    char costumext[MAX_PATH];
    char *estrHeroXML;
    int i;

    strcpy(achSafeName, e->name);
    imageserver_MakeSafeFilename(achSafeName);

    printf("    Generating %s for %s\n", pchDest, e->name);

    filelog_printf(g_CharacterInfoSettings.achLogFileName, "Generating XML for %s\n", e->name);

    estrCreate(&estrHeroXML);
    estrConcatStaticCharArray(&estrHeroXML, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

    estrConcatf(&estrHeroXML, "<CHARACTER ID=\"%d\" VERSION=\"%s\">\n", e->db_id, XML_VERSION_STRING);
// BASIC
    estrConcatStaticCharArray(&estrHeroXML, "\t<BASIC>\n");
        estrConcatf(&estrHeroXML, "\t\t<NAME><![CDATA[%s]]></NAME>\n", e->name);
        estrConcatf(&estrHeroXML, "\t\t<SAFENAME><![CDATA[%s]]></SAFENAME>\n", achSafeName);
        estrConcatStaticCharArray(&estrHeroXML, "\t\t<TITLE>");
        if(*e->pl->titleCommon || *e->pl->titleOrigin || *e->pl->titleTheText)
        {
            estrConcatf(&estrHeroXML, "%s %s %s",
                e->pl->titleTheText?e->pl->titleTheText:"",
                e->pl->titleCommon?e->pl->titleCommon:"",
                e->pl->titleOrigin?e->pl->titleOrigin:"");
        } 
        estrConcatStaticCharArray(&estrHeroXML, "</TITLE>\n");
    #ifdef BADGES
        // Badge Title
        estrConcatStaticCharArray(&estrHeroXML, "\t\t<BADGETITLE>");
        if(e->pl->titleBadge)
        {
            const BadgeDef *badge = badge_GetBadgeByIdx(e->pl->titleBadge);
            if(badge)
            {
                estrConcatf(&estrHeroXML, "<![CDATA[%s]]>", printLocalizedEntFromEntLocale(badge->pchDisplayTitle[ENT_IS_VILLAIN(e)?1:0], e));
            }
        }
        estrConcatStaticCharArray(&estrHeroXML, "</BADGETITLE>\n");
    #endif
        estrConcatf(&estrHeroXML, "\t\t<TYPE>%s</TYPE>\n", ENT_IS_HERO(e) ? "Hero" : "Villain");
        sprintf_s(SAFESTR(costumext), "_Thumbnail.jpg");
        estrConcatf(&estrHeroXML, "\t\t<THUMBNAIL><![CDATA[%s]]></THUMBNAIL>\n", MakePictureURL(achSafeName, costumext));
        convertFilenameTGAtoGIF(e->pchar->porigin->pchIcon, achGifFileName);
        ConvertFilenameToLowercase(achGifFileName);
        estrConcatf(&estrHeroXML, "\t\t<ORIGIN>\n\t\t\t<NAME>%s</NAME>\n\t\t\t<ICONFILE>%s</ICONFILE>\n\t\t</ORIGIN>\n", localizedPrintf(e,e->pchar->porigin->pchDisplayName), achGifFileName);
        convertFilenameTGAtoGIF(e->pchar->pclass->pchIcon, achGifFileName);
        ConvertFilenameToLowercase(achGifFileName);
        estrConcatf(&estrHeroXML, "\t\t<ARCHETYPE>\n\t\t\t<NAME>%s</NAME>\n\t\t\t<ICONFILE>%s</ICONFILE>\n\t\t</ARCHETYPE>\n", localizedPrintf(e,e->pchar->pclass->pchDisplayName), achGifFileName);
        estrConcatf(&estrHeroXML, "\t\t<LEVEL>%d</LEVEL>\n", e->pchar->iLevel+1);
        estrConcatStaticCharArray(&estrHeroXML, "\t\t<SUPERGROUPNAME>");
        if ( e->supergroup_id )
            estrConcatf(&estrHeroXML, "<![CDATA[%s]]>", GetSupergroupName(e));
        estrConcatStaticCharArray(&estrHeroXML, "</SUPERGROUPNAME>\n");
        estrConcatf(&estrHeroXML, "\t\t<SUPERGROUPID>%d</SUPERGROUPID>\n", e->supergroup_id);
        estrConcatf(&estrHeroXML, "\t\t<SERVER ID=\"%d\">\n\t\t\t<NAME>%s</NAME>\n\t\t</SERVER>\n", GetShardIdFromName(g_CharacterInfoSettings.achShardName), g_CharacterInfoSettings.achShardName);
        estrConcatf(&estrHeroXML, "\t\t<BACKGROUND><![CDATA[%s]]></BACKGROUND>\n", e->strings->playerDescription && *e->strings->playerDescription ? e->strings->playerDescription : "" );
        estrConcatf(&estrHeroXML, "\t\t<BATTLECRY><![CDATA[%s]]></BATTLECRY>\n", e->strings->motto && *e->strings->motto ? e->strings->motto : "" );
        estrConcatStaticCharArray(&estrHeroXML, "\t\t<COSTUMES>\n");
        for ( i = 0; i < costume_get_num_slots(e) && i < MAX_COSTUMES; ++i )
        {
            char costumext[50];
            sprintf_s(SAFESTR(costumext), "_Costume%d.jpg", i+1);
            estrConcatf(&estrHeroXML, "\t\t\t<COSTUME ID=\"%d\">\n\t\t\t\t<COSTUMEFILE><![CDATA[%s]]></COSTUMEFILE>\n\t\t\t</COSTUME>\n",
                i+1, MakePictureURL(achSafeName, costumext));
        }
        estrConcatStaticCharArray(&estrHeroXML, "\t\t</COSTUMES>\n");
    estrConcatStaticCharArray(&estrHeroXML, "\t</BASIC>\n");

// POWERS
    if(!e->pl->webHidePowers)
    {
        int build = 0;
        int max_builds = 1;

        // TODO: Calc/find max builds on character here.

#if defined(MULTIPLE_BUILDS_XML)
        estrConcatf(&estrHeroXML, "\t<BUILDS MAX=%d>\n", max_builds);
#endif

        for (build = 0; build < max_builds; build++)
        {
#if defined(MULTIPLE_BUILDS_XML)
            estrConcatf(&estrHeroXML, "\t\t<POWERS BUILD=%d>\n", build + 1);
#else
            estrConcatStaticCharArray(&estrHeroXML, "\t\t<POWERS>\n");
#endif
            // TODO: Change character build here.

            for ( i = 0; i < eaSize(&e->pchar->ppPowerSets); ++i )
            {
                if ( !e->pchar->ppPowerSets[i] )
                    continue;

                estrConcatf(&estrHeroXML, "\t\t\t<POWERSET NAME=\"%s\">\n", localizedPrintf(e, e->pchar->ppPowerSets[i]->psetBase->pchDisplayName));

                estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t<CAT>");
                if(stricmp(e->pchar->ppPowerSets[i]->psetBase->pchName, "Inherent")==0)
                {
                    estrConcatStaticCharArray(&estrHeroXML, "inherent");
                }
                else if(stricmp(e->pchar->ppPowerSets[i]->psetBase->pchName, "Temporary_Powers")==0)
                {
                    estrConcatStaticCharArray(&estrHeroXML, "temp");
                }
                else if(e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Primary])
                {
                    estrConcatStaticCharArray(&estrHeroXML, "primary");
                }
                else if(e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Secondary])
                {
                    estrConcatStaticCharArray(&estrHeroXML, "secondary");
                }
                else if(e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Pool])
                {
                    estrConcatStaticCharArray(&estrHeroXML, "pool");
                }
                else if(e->pchar->ppPowerSets[i]->psetBase->pcatParent == e->pchar->pclass->pcat[kCategory_Epic])
                {
                    estrConcatStaticCharArray(&estrHeroXML, "epic");
                }
                estrConcatStaticCharArray(&estrHeroXML, "</CAT>\n");

                PowersXML(&estrHeroXML, e, e->pchar->ppPowerSets[i]);
                estrConcatStaticCharArray(&estrHeroXML, "\t\t\t</POWERSET>\n");
            }

            estrConcatStaticCharArray(&estrHeroXML, "\t\t</POWERS>\n");
        }
#if defined(MULTIPLE_BUILDS_XML)
        estrConcatStaticCharArray(&estrHeroXML, "\t</BUILDS>\n");
#endif
    }

// BADGES
#ifdef BADGES
    if (!e->pl->webHideBadges)
    {
        estrConcatStaticCharArray(&estrHeroXML, "\t<BADGES>\n");
            for(i=kBadgeType_LastBadgeCategory-1; i>0; i--)
            {
                BadgeSetXML(&estrHeroXML, e, i);
            }
        estrConcatStaticCharArray(&estrHeroXML, "\t</BADGES>\n");
    }
#endif

// STATS
    if(!e->pl->webHideStats)
    {
        estrConcatStaticCharArray(&estrHeroXML, "\t<STATS>\n");
            estrConcatStaticCharArray(&estrHeroXML, "\t\t<SINGLE>\n");
                estrConcatf(&estrHeroXML, "\t\t\t<SECURITYLEVEL>%d</SECURITYLEVEL>\n", e->pchar->iLevel+1);
                estrConcatf(&estrHeroXML, "\t\t\t<INFLUENCE>%d</INFLUENCE>\n", e->pchar->iInfluencePoints);
                estrConcatf(&estrHeroXML, "\t\t\t<HEALING>%d</HEALING>\n", stat_Get(e->pl, 0, kStat_General, "healing_given"));
                estrConcatf(&estrHeroXML, "\t\t\t<BADGESEARNED>%d</BADGESEARNED>\n", CountBadgesForEnt(e));
                estrConcatf(&estrHeroXML, "\t\t\t<TOTALTIME>%d</TOTALTIME>\n", e->total_time);
                estrConcatf(&estrHeroXML, "\t\t\t<EXPERIENCE>\n\t\t\t\t<POINTS>%d</POINTS>\n\t\t\t\t<DEBT>%d</DEBT>\n\t\t\t</EXPERIENCE>\n", e->pchar->iExperiencePoints, e->pchar->iExperienceDebt);
                // PVP/E stats
                {
                    U32 pvpKillsThisMonth = stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Kills, "pvp") + stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Kills, "arena"),
                        pvpDeathsThisMonth = stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Deaths, "pvp") + stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Deaths, "arena"),
                        pvpKillsLastMonth = stat_Get(e->pl, kStatPeriod_LastMonth, kStat_Kills, "pvp") + stat_Get(e->pl, kStatPeriod_LastMonth, kStat_Kills, "arena"),
                        pvpDeathsLastMonth = stat_Get(e->pl, kStatPeriod_LastMonth, kStat_Deaths, "pvp") + stat_Get(e->pl, kStatPeriod_LastMonth, kStat_Deaths, "pvp");

                    estrConcatStaticCharArray(&estrHeroXML, "\t\t\t<PVP>\n");
                        estrConcatf(&estrHeroXML, "\t\t\t\t<REPUTATION>%.2f</REPUTATION>\n",e->pl->reputationPoints);
                        estrConcatf(&estrHeroXML, "\t\t\t\t<THISMONTH>\n\t\t\t\t\t<KILLS>%d</KILLS>\n\t\t\t\t\t<DEATHS>%d</DEATHS>\n\n\t\t\t\t</THISMONTH>\n", pvpKillsThisMonth, pvpDeathsThisMonth);
                        estrConcatf(&estrHeroXML, "\t\t\t\t<LASTMONTH>\n\t\t\t\t\t<KILLS>%d</KILLS>\n\t\t\t\t\t<DEATHS>%d</DEATHS>\n\n\t\t\t\t</LASTMONTH>\n", pvpKillsLastMonth, pvpDeathsLastMonth);
                    estrConcatStaticCharArray(&estrHeroXML, "\t\t\t</PVP>\n");

                    estrConcatStaticCharArray(&estrHeroXML, "\t\t\t<PVE>\n");
                        estrConcatf(&estrHeroXML, "\t\t\t\t<THISMONTH>\n\t\t\t\t\t<KILLS>%d</KILLS>\n\t\t\t\t\t<DEATHS>%d</DEATHS>\n\n\t\t\t\t</THISMONTH>\n", 
                            stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Kills, "overall") - pvpKillsThisMonth, 
                            stat_Get(e->pl, kStatPeriod_ThisMonth, kStat_Deaths, "overall") - pvpDeathsThisMonth);
                        estrConcatf(&estrHeroXML, "\t\t\t\t<LASTMONTH>\n\t\t\t\t\t<KILLS>%d</KILLS>\n\t\t\t\t\t<DEATHS>%d</DEATHS>\n\n\t\t\t\t</LASTMONTH>\n", 
                            stat_Get(e->pl, kStatPeriod_LastMonth, kStat_Kills, "overall") - pvpKillsLastMonth, 
                            stat_Get(e->pl, kStatPeriod_LastMonth, kStat_Deaths, "overall") - pvpDeathsLastMonth);
                    estrConcatStaticCharArray(&estrHeroXML, "\t\t\t</PVE>\n");
                }
            estrConcatStaticCharArray(&estrHeroXML, "\t\t</SINGLE>\n");


            estrConcatStaticCharArray(&estrHeroXML, "\t\t<PERIODIC>\n");
            for(i=stat_GetStatCount()-1; i>=0; i--)
            {
                const char *pch = stat_GetStatName(i);

                if(pch!=NULL)
                {
                    bool bShow;
                    char *pchLocalized = TranslateStat(e, pch, &bShow);

                    if(bShow)
                    {
                        estrConcatf(&estrHeroXML, "\t\t\t<STATCAT SORTID=\"%d\">\n", stat_GetStatCount()-i);
                        estrConcatf(&estrHeroXML, "\t\t\t\t<NAME>%s</NAME>\n", pchLocalized);
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t<LASTACTIVEDAY>\n");
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<VICTORIES>%d</VICTORIES>\n", e->pl->stats[kStatPeriod_Today].cats[kStat_Kills][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<DEFEATS>%d</DEFEATS>\n", e->pl->stats[kStatPeriod_Today].cats[kStat_Deaths][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<TIMESPENT>%d</TIMESPENT>\n", e->pl->stats[kStatPeriod_Today].cats[kStat_Time][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<INF>%d</INF>\n", e->pl->stats[kStatPeriod_Today].cats[kStat_Influence][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<XP>%d</XP>\n", e->pl->stats[kStatPeriod_Today].cats[kStat_XP][i]);
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t</LASTACTIVEDAY>\n");
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t<PREVIOUSDAY>\n");
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<VICTORIES>%d</VICTORIES>\n", e->pl->stats[kStatPeriod_Yesterday].cats[kStat_Kills][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<DEFEATS>%d</DEFEATS>\n", e->pl->stats[kStatPeriod_Yesterday].cats[kStat_Deaths][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<TIMESPENT>%d</TIMESPENT>\n", e->pl->stats[kStatPeriod_Yesterday].cats[kStat_Time][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<INF>%d</INF>\n", e->pl->stats[kStatPeriod_Yesterday].cats[kStat_Influence][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<XP>%d</XP>\n", e->pl->stats[kStatPeriod_Yesterday].cats[kStat_XP][i]);
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t</PREVIOUSDAY>\n");
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t<LASTACTIVEMONTH>\n");
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<VICTORIES>%d</VICTORIES>\n", e->pl->stats[kStatPeriod_ThisMonth].cats[kStat_Kills][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<DEFEATS>%d</DEFEATS>\n", e->pl->stats[kStatPeriod_ThisMonth].cats[kStat_Deaths][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<TIMESPENT>%d</TIMESPENT>\n", e->pl->stats[kStatPeriod_ThisMonth].cats[kStat_Time][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<INF>%d</INF>\n", e->pl->stats[kStatPeriod_ThisMonth].cats[kStat_Influence][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<XP>%d</XP>\n", e->pl->stats[kStatPeriod_ThisMonth].cats[kStat_XP][i]);
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t</LASTACTIVEMONTH>\n");
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t<PREVIOUSMONTH>\n");
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<VICTORIES>%d</VICTORIES>\n", e->pl->stats[kStatPeriod_LastMonth].cats[kStat_Kills][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<DEFEATS>%d</DEFEATS>\n", e->pl->stats[kStatPeriod_LastMonth].cats[kStat_Deaths][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<TIMESPENT>%d</TIMESPENT>\n", e->pl->stats[kStatPeriod_LastMonth].cats[kStat_Time][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<INF>%d</INF>\n", e->pl->stats[kStatPeriod_LastMonth].cats[kStat_Influence][i]);
                            estrConcatf(&estrHeroXML, "\t\t\t\t\t<XP>%d</XP>\n", e->pl->stats[kStatPeriod_LastMonth].cats[kStat_XP][i]);
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t\t</PREVIOUSMONTH>\n");
                        estrConcatStaticCharArray(&estrHeroXML, "\t\t\t</STATCAT>\n");
                    }
                }
            }
            estrConcatStaticCharArray(&estrHeroXML, "\t\t</PERIODIC>\n");

        estrConcatStaticCharArray(&estrHeroXML, "\t</STATS>\n");
    }

// FRIENDS
    if (!e->pl->webHideFriends)
    {
        estrConcatStaticCharArray(&estrHeroXML, "\t<FRIENDS>\n");
            GenFriendXML(&estrHeroXML, e);
        estrConcatStaticCharArray(&estrHeroXML, "\t</FRIENDS>\n");
    }

    estrConcatStaticCharArray(&estrHeroXML, "</CHARACTER>");

    {
        FILE *fp;
        fp = fopen(pchDest, "w");
        if(fp==NULL)
        {
            printf("*** Error: Unable to open file '%s'\n", pchDest);
            filelog_printf(g_CharacterInfoSettings.achLogFileName,
                "Could not write generated XML to '%s'\n", pchDest);

            return 1;
        }
        fprintf(fp, "%s", estrHeroXML);
        fclose(fp);
    }

    estrDestroy(&estrHeroXML);

    return 0;
}



#ifdef BADGES
static void BadgeSetXML(char **pestrHeroXML, Entity *e, BadgeType eCat)
{
    int i;
    int idxType = ENT_IS_VILLAIN(e)?1:0;
    int iSize = eaSize(&g_BadgeDefs.ppBadges);
    
    estrConcatf(pestrHeroXML, "\t\t<BADGECAT NAME=\"%s\">\n", localizedPrintf(e,badge_CategoryGetName(e,kCollectionType_Badge,eCat)));
    for(i=0; i<iSize; i++)
    {
        if(!g_BadgeDefs.ppBadges[i]->bDoNotCount &&
            g_BadgeDefs.ppBadges[i]->iIdx<=g_BadgeDefs.idxMax
            && g_BadgeDefs.ppBadges[i]->eCategory==eCat)
        {
            if(BitFieldGet(e->pl->aiBadgesOwned, BADGE_ENT_BITFIELD_SIZE, g_BadgeDefs.ppBadges[i]->iIdx))
            {
                estrConcatf(pestrHeroXML, "\t\t\t<BADGE NAME=\"%s\">\n", printLocalizedEntFromEntLocale(g_BadgeDefs.ppBadges[i]->pchDisplayTitle[idxType], e));
                    estrConcatf(pestrHeroXML, "\t\t\t\t<ICONFILE>%s</ICONFILE>\n", ImgSrc(g_BadgeDefs.ppBadges[i]->pchIcon[idxType]));
                    //estrConcatf(pestrHeroXML, "\t\t\t\t<DESCRIPTION><![CDATA[%s]]></DESCRIPTION>\n", 
                    //    localizedPrintf(e, g_BadgeDefs.ppBadges[i]->pchDisplayText[ENT_IS_VILLAIN(e)?1:0]));
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t</BADGE>\n");
            }
        }
    }
    estrConcatStaticCharArray(pestrHeroXML, "\t\t</BADGECAT>\n");
}
#endif

static void PowersXML(char **pestrHeroXML, Entity *e, PowerSet *pset)
{
    int ipow, n;
    char *pch;

    n = eaSize(&pset->ppPowers);
    for(ipow=0; ipow<n; ipow++)
    {
        int m;
        int iboost;
        char *pchHelp;
        char *pchSet;
        Power *ppow = pset->ppPowers[ipow];

        // if it is an auto power and doesnt have a buff icon, dont make any xml for it
        if ( !ppow || !ppow->ppowBase || (ppow->ppowBase->eType == kPowerType_Auto && !ppow->ppowBase->bShowBuffIcon) )
            continue;

        pch = strdupa(localizedPrintf(e,ppow->ppowBase->pchDisplayName));
        pchHelp = strdupa(localizedPrintf(e,ppow->ppowBase->pchDisplayHelp));
        pchSet = strdupa(localizedPrintf(e,ppow->ppowBase->psetParent->pchDisplayName));

        estrConcatf(pestrHeroXML, "\t\t\t\t<POWER NAME=\"%s\">\n", pch);
        estrConcatf(pestrHeroXML, "\t\t\t\t\t<ICONFILE>%s</ICONFILE>\n", ImgSrc(ppow->ppowBase->pchIconName));
        estrConcatf(pestrHeroXML, "\t\t\t\t\t<DESCRIPTION><![CDATA[%s]]></DESCRIPTION>\n", pchHelp);

        m = eaSize(&ppow->ppBoosts);
        for(iboost=0; iboost<m; iboost++)
        {
            if (ppow->ppBoosts[iboost] )
            {
                pch = localizedPrintf(e,ppow->ppBoosts[iboost]->ppowBase->pchDisplayName);
                EnhancementXML(e, pestrHeroXML, ppow->ppBoosts[iboost], ppow->ppBoosts[iboost]->iLevel, ppow->ppBoosts[iboost]->iNumCombines, pset->pcharParent->iLevel, "\t\t\t\t\t" );
            }
            else
            {
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t<ENHANCEMENT>\n");
                if (ENT_IS_HERO(e))
                {
                    estrConcatf(pestrHeroXML, "\t\t\t\t\t\t<BORDER>%s</BORDER>\n", ImgSrc("EnhncTray_Ring.tga"));
                    estrConcatf(pestrHeroXML, "\t\t\t\t\t\t<POG>%s</POG>\n", ImgSrc("EnhncTray_RingHole.tga"));
                }
                else
                {
                    estrConcatf(pestrHeroXML, "\t\t\t\t\t\t<BORDER>%s</BORDER>\n", ImgSrc("V_EnhncTray_Ring.tga"));
                    estrConcatf(pestrHeroXML, "\t\t\t\t\t\t<POG>%s</POG>\n", ImgSrc("V_EnhncTray_RingHole.tga"));
                }
                // all of this stuff is blank since its undefined (and unnecessary) for an empty enhancement
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<BORDER_L></BORDER_L>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<BORDER_R></BORDER_R>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<NUMBERFRAME></NUMBERFRAME>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<ICONFILE></ICONFILE>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<LEVEL></LEVEL>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<NUMCOMBINES></NUMCOMBINES>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<EFFECTS></EFFECTS>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<USABLEBY></USABLEBY>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t\t<DESCRIPTION></DESCRIPTION>\n");
                estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t\t</ENHANCEMENT>\n");
            }
        }

        estrConcatStaticCharArray(pestrHeroXML, "\t\t\t\t</POWER>\n");
    }
}

static int stripDoublePercents(char *str)
{
    if ( str )
    {
        int len = 0;
        char *c = str;
        while ( *str )
        {
            if ( *str == '%' && *(str+1) == '%' )
            {
                ++str;
                continue;
            }
            *c++ = *str++;
            ++len;
        }
        *c = 0;
        return len;
    }

    return -1;
}

static void EnhancementInfoXML(Entity *e, char **pestrHeroXML, Boost *boost, int level, char *tabs)
{
    int i;
    char *buf = NULL;
    const BasePower *ppowBase = NULL;
    BoostSet *pBoostSet = NULL;

    if ( !e || !pestrHeroXML || !boost )
        return;

    if ( !tabs ) // no tabs is what you want, no tabs is what you'll get...
        tabs = "";

    ppowBase = boost->ppowBase;
    if ( !ppowBase )
        return;

    pBoostSet = (BoostSet *) ppowBase->pBoostSetMember;

    estrConcatf(pestrHeroXML, "%s<SHORTHELP><![CDATA[%s]]></SHORTHELP>\n", tabs, localizedPrintf(e, ppowBase->pchDisplayShortHelp));
    if ( !ppowBase->pBoostSetMember )
    {
        estrConcatf(pestrHeroXML, "%s<POWERSETSHORTHELP><![CDATA[%s]]></POWERSETSHORTHELP>\n", tabs,
            localizedPrintf(e, g_PowerDictionary.ppPowerCategories[ppowBase->id.icat]->ppPowerSets[ppowBase->id.iset]->pchDisplayShortHelp));
    }

    buf = character_BoostGetHelpTextInEntLocale(e, boost, ppowBase, level);
    if (buf)
    {
        int len = stripDoublePercents(buf);
        estrSetLength(&buf, len);
        estrConcatf(pestrHeroXML, "%s<HELPTEXT><![CDATA[%s]]></HELPTEXT>\n", tabs, buf);
        estrDestroy(&buf);
    }
    if ( pBoostSet )
    {
        int numSlotted = 0;
        char tabsPlus[128] = {0};
        sprintf_s(SAFESTR(tabsPlus), "%s\t", tabs);

        estrConcatf(pestrHeroXML, "%s<SETINFO>\n", tabs);

        // is this slotted in a power and how many of that set are in this power
        if (boost && boost->ppowParent)
        {
            Power *pPower = boost->ppowParent;
            int count = eaSize(&pPower->ppBoosts);

            // find amount of this boost set in the current power
            for (i = 0; i < eaSize(&pPower->ppBoosts); i++) 
            {
                Boost *pSlottedBoost = pPower->ppBoosts[i];

                if (pSlottedBoost && pSlottedBoost->ppowBase->pBoostSetMember &&
                    stricmp(boost->ppowBase->pBoostSetMember->pchName, 
                    pSlottedBoost->ppowBase->pBoostSetMember->pchName) == 0)
                    numSlotted++;
            }

            estrConcatf(pestrHeroXML, "%s<SETNAME>%s</SETNAME>\n", tabsPlus, localizedPrintf(e, pBoostSet->pchDisplayName));
            estrConcatf(pestrHeroXML, "%s<SETCATEGORY>%s</SETCATEGORY>\n", tabsPlus, localizedPrintf(e, pBoostSet->pchGroupName));
            estrConcatf(pestrHeroXML, "%s<TOTALINSET>%d</TOTALINSET>\n", tabsPlus, eaSize(&pBoostSet->ppBoostLists));
            estrConcatf(pestrHeroXML, "%s<NUMSLOTTED>%d</NUMSLOTTED>\n", tabsPlus, numSlotted);
        }
        else 
        {
            estrConcatf(pestrHeroXML, "%s<SETNAME>%s</SETNAME>\n", tabsPlus, localizedPrintf(e, pBoostSet->pchDisplayName));
            estrConcatf(pestrHeroXML, "%s<SETCATEGORY>%s</SETCATEGORY>\n", tabsPlus, localizedPrintf(e, pBoostSet->pchGroupName));
            estrConcatf(pestrHeroXML, "%s<TOTALINSET>%d</TOTALINSET>\n", tabsPlus, eaSize(&pBoostSet->ppBoostLists));
        }

        if (pBoostSet && pBoostSet->ppBoostLists)
        {
            estrConcatf(pestrHeroXML, "%s<SETLIST>\n", tabsPlus);

            // set items
            for (i = 0; i < eaSize(&pBoostSet->ppBoostLists); i++)
            {
                int j;
                int slotted = 0;
                const BoostList *pBoostList = pBoostSet->ppBoostLists[i];

                for (j = 0; j < eaSize(&pBoostList->ppBoosts); j++)
                {
                    // check to see if this is slotted in this power
                    if (boost && boost->ppowParent)
                    {
                        Power *pPower = boost->ppowParent;
                        int k, count = eaSize(&pPower->ppBoosts);

                        for (k = 0; k < eaSize(&pPower->ppBoosts); k++) 
                        {
                            Boost *pBoost = pPower->ppBoosts[k];
                            if (pBoost && stricmp(pBoost->ppowBase->pchName, pBoostList->ppBoosts[j]->pchName) == 0)
                                slotted = 1;
                        }
                    } 
                    estrConcatf(pestrHeroXML, "\t%s<SETENHANCEMENT SLOTTED=\"%d\"><![CDATA[%s]]></SETENHANCEMENT>\n",
                        tabsPlus, slotted, localizedPrintf(e, pBoostList->ppBoosts[j]->pchDisplayName));
                }
            }

            estrConcatf(pestrHeroXML, "%s</SETLIST>\n", tabsPlus);
        }


        // set bonuses
        if ( eaSize(&pBoostSet->ppBonuses) )
        {
            estrConcatf(pestrHeroXML, "%s<SETBONUSES>\n", tabsPlus);
            for (i = 0; i < eaSize(&pBoostSet->ppBonuses); i++)
            {
                int active = 0;
                const BoostSetBonus *pBonus = pBoostSet->ppBonuses[i];

                if (pBonus->iMinBoosts == 1)
                    continue;

                if (numSlotted >= pBonus->iMinBoosts)
                    active = 1;
                else 
                    active = 0;

                if (pBonus->pBonusPower != NULL)
                {
                    estrConcatf(pestrHeroXML, "\t%s<BONUSPOWER ACTIVE=\"%d\" MINENHANCMENTS=\"%d\"><![CDATA[%s]]></BONUSPOWER>\n",
                        tabsPlus, active, pBonus->iMinBoosts, localizedPrintf(e, pBonus->pBonusPower->pchDisplayHelp));
                }

                if (eaSize(&pBonus->ppAutoPowers) > 0) {
                    int j;
                    for (j = 0; j < eaSize(&pBonus->ppAutoPowers); j++)
                    {
                        const BasePower *pAuto = pBonus->ppAutoPowers[j];
                        char *str = localizedPrintf(e, pAuto->pchDisplayHelp);
                        stripDoublePercents(str);

                        estrConcatf(pestrHeroXML, "\t%s<BONUSAUTOPOWER ACTIVE=\"%d\" MINENHANCMENTS=\"%d\"><![CDATA[%s]]></BONUSAUTOPOWER>\n",
                            tabsPlus, active, pBoostSet->ppBonuses[i]->iMinBoosts, str);
                    }
                }            
            }
            estrConcatf(pestrHeroXML, "%s</SETBONUSES>\n", tabsPlus);
        }

        estrConcatf(pestrHeroXML, "%s</SETINFO>\n", tabs);
    }
}

static void EnhancementXML(Entity *e, char **pestrHeroXML, Boost *boost, int level, int iNumCombines, int iCharLevel, char *tabs)
{
    char ach[256];
    char *frameL = NULL;
    char *frameR = NULL;
    char *pog = NULL;
    char *pchName;
    char *pchDesc;
    static char *achOrigins = NULL;
    static char *achTypes = NULL;
    int i = 0;
    const BasePower *ppowBase = NULL;
    int isInvention = 0;

    if ( !boost )
        return;

    ppowBase = boost->ppowBase;

    estrClear(&achOrigins);
    estrClear(&achTypes);

    if(strstriConst(ppowBase->pchName, "generic"))
    {
        int size = eaiSize(&(int *)ppowBase->pBoostsAllowed);
        frameL = "E_orgin_generic";
        frameR = "E_orgin_generic";

        //strcpy(achOrigins, "Usable by all origins.");
        estrConcatf(&achOrigins, "%s\t<USABLEBY>All</USABLEBY>\n", tabs);
        estrConcatf(&achTypes, "%s\t<EFFECTS></EFFECTS>\n", tabs); // it feels like this shouldnt be empty
        while(!pog && i<size)
        {
            if(ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins))
            {
                pog = g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName;
            }
            else
            {
                i++;
            }
        }
    }
    else if(baseset_IsCrafted(ppowBase->psetParent))
    {
        int size = eaiSize(&(int *)ppowBase->pBoostsAllowed);
        frameL = "E_orgin_recipe";
        frameR = "E_orgin_recipe";
        isInvention = 1;

        estrConcatf(&achTypes, "%s\t<EFFECTS></EFFECTS>\n", tabs); // it feels like this shouldnt be empty
        estrConcatf(&achOrigins, "%s\t<USABLEBY>All</USABLEBY>\n", tabs);
        while(!pog && i<size)
        {
            if(ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins))
            {
                pog = g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName;
            }
            else
            {
                i++;
            }
        }
    }
    else if(strstriConst(ppowBase->pchName, "hamidon")
        || strstriConst(ppowBase->pchName, "titan")
        || strstriConst(ppowBase->pchName, "hydra"))
    {
        int size = eaiSize(&(int *)ppowBase->pBoostsAllowed);
        frameL = "E_orgin_uber";
        frameR = "E_orgin_uber";

        //strcpy(achOrigins, "Usable by all origins.");
        estrConcatf(&achOrigins, "%s\t<USABLEBY>All</USABLEBY>\n", tabs);
        estrConcatf(&achTypes, "%s\t<EFFECTS></EFFECTS>\n", tabs); // it feels like this shouldnt be empty
        while(!pog && i < size)
        {
            if(ppowBase->pBoostsAllowed[i] >= eaSize(&g_CharacterOrigins.ppOrigins))
            {
                pog = g_AttribNames.ppBoost[ppowBase->pBoostsAllowed[i]-eaSize(&g_CharacterOrigins.ppOrigins)]->pchIconName;
            }
            else
            {
                i++;
            }
        }
    }
    else
    {
        int iSizeOrigins = eaSize(&g_CharacterOrigins.ppOrigins);
        int iSize = eaiSize(&(int *)ppowBase->pBoostsAllowed);

        //strcpy(achOrigins, "Usable by: ");

        for(i = 0; i < iSize; i++)
        {
            int num = ppowBase->pBoostsAllowed[i];
            
            // an origin, get frame sprite
            if(num < iSizeOrigins)
            {
                char *frameName = NULL;
                const char *origin = g_CharacterOrigins.ppOrigins[num]->pchName;

                if(stricmp(origin, "Magic") == 0)
                {
                    frameName = "E_orgin_magic";
                }
                else if( stricmp( origin, "Science") == 0)
                {
                    frameName = "E_orgin_scientific";
                }
                else if( stricmp( origin, "Mutation") == 0)
                {
                    frameName = "E_orgin_mutant";
                }
                else if( stricmp( origin, "Technology") == 0)
                {
                    frameName = "E_orgin_tech";
                }
                else if( stricmp( origin, "Natural") == 0)
                {
                    frameName = "E_orgin_natural";
                }
                else if( stricmp( origin, "Generic") == 0)
                {
                    frameName = "E_orgin_generic";
                }

                if(!frameL)
                {
                    frameL = frameName;
                }
                else if (!frameR)
                {
                    frameR = frameName;
                    strcat(achOrigins, ", ");
                }
                //strcat(achOrigins, localizedPrintf(0,g_CharacterOrigins.ppOrigins[num]->pchDisplayName));
                estrConcatf(&achOrigins, "%s\t<USABLEBY>", tabs);
                estrConcatCharString(&achOrigins,localizedPrintf(0,g_CharacterOrigins.ppOrigins[num]->pchDisplayName));
                estrConcatStaticCharArray(&achOrigins, "</USABLEBY>\n");
            }
            else if(!pog)
            {
                num -= iSizeOrigins;
                pog = g_AttribNames.ppBoost[num]->pchIconName;
                //if(!achTypes[0])
                //{
                //    strcpy(achTypes, "Effects: ");
                //}
                //else
                //{
                //    strcat(achTypes, ", ");
                //}
                estrConcatf(&achTypes, "%s\t<EFFECTS>", tabs);
                estrConcatCharString(&achTypes,localizedPrintf(0,g_AttribNames.ppBoost[num]->pchDisplayName));
                estrConcatStaticCharArray(&achTypes, "</EFFECTS>\n");
            }
        }
    }

    if(!frameR)
    {
        frameR = frameL;
    }

    pchName = strdupa(localizedPrintf(e,ppowBase->pchDisplayName));
    //pchDesc = strdupa(EscapeApostrophes(localizedPrintf(0,ppowBase->pchDisplayHelp)));
    pchDesc = character_BoostGetHelpTextInEntLocale(e, NULL, ppowBase, level);


    {
        char tabsPlus[128] = {0};
        sprintf_s(SAFESTR(tabsPlus), "%s\t", tabs);
        estrConcatf(pestrHeroXML, "%s<ENHANCEMENT NAME=\"%s\">\n", tabs, pchName);
            estrConcatf(pestrHeroXML, "%s<BORDER></BORDER>\n", tabsPlus); // only used for empty enhancements
            sprintf(ach, "%s_L.tga", frameL);
            estrConcatf(pestrHeroXML, "%s<BORDER_L>%s</BORDER_L>\n", tabsPlus, ImgSrc(ach));
            sprintf(ach, "%s_R.tga", frameR);
            estrConcatf(pestrHeroXML, "%s<BORDER_R>%s</BORDER_R>\n", tabsPlus, ImgSrc(ach));
            estrConcatf(pestrHeroXML, "%s<POG>%s</POG>\n", tabsPlus, ImgSrc(pog));
            estrConcatf(pestrHeroXML, "%s<NUMBERFRAME>%s</NUMBERFRAME>\n", tabsPlus, ImgSrc("E_Number_Frame_2digit.tga"));
            estrConcatf(pestrHeroXML, "%s<ICONFILE>%s</ICONFILE>\n", tabsPlus, ImgSrc(ppowBase->pchIconName));
            estrConcatf(pestrHeroXML, "%s<LEVEL>%d</LEVEL>\n", tabsPlus, level+1);
            estrConcatf(pestrHeroXML, "%s<ISINVENTION>%d</ISINVENTION>\n", tabsPlus, isInvention);
            estrConcatf(pestrHeroXML, "%s<NUMCOMBINES>%d</NUMCOMBINES>\n", tabsPlus, iNumCombines);
            //estrConcatf(pestrHeroXML, "%s<EFFECTS>%s</EFFECTS>\n", tabsPlus, achTypes);
            //estrConcatf(pestrHeroXML, "%s<USABLEBY>%s</USABLEBY>\n", tabsPlus, achOrigins);
            estrConcatEString(pestrHeroXML, achTypes);
            estrConcatEString(pestrHeroXML, achOrigins);
            estrConcatf(pestrHeroXML, "%s<DESCRIPTION>\n", tabsPlus);
                EnhancementInfoXML(e, pestrHeroXML, boost, level, tabsPlus);
            estrConcatf(pestrHeroXML, "%s</DESCRIPTION>\n", tabsPlus);
        estrConcatf(pestrHeroXML, "%s</ENHANCEMENT>\n", tabs, pchName);
    }

    estrDestroy(&pchDesc);
}

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/


#ifdef STATS
/**********************************************************************func*
 * TranslateStat
 *
 */
static char *TranslateStat(Entity *e, const char *pch, bool *pbShow)
{
    int iGroup = StaticDefineIntGetInt(ParseVillainGroupEnum, pch);
    if(iGroup>=0)
    {
        *pbShow = kiosk_VillainGroupIsShown(iGroup);
        pch = villainGroupGetPrintName(iGroup);
    }
    else
    {
        *pbShow = true;
    }

    return localizedPrintf(e,pch);
}
#endif

/**********************************************************************func*
 * PutInCommas
 *
 */
static char *PutInCommas(int iVal)
{
    static char achOut[256];
    char achTmp[256];
    size_t i;
    int iDest;
    size_t iLen;

    itoa(iVal, achTmp, 10);
    iLen = strlen(achTmp);

    iDest = 0;
    achOut[iDest++] = achTmp[0];
    for(i=iLen-1; i>0; i--)
    {
        if(i%3==0)
        {
            achOut[iDest++] = ',';
        }
        achOut[iDest++] = achTmp[iLen-i];
    }
    achOut[iDest++] = 0;

    return achOut;
}

/**********************************************************************func*
 * ImgSrc
 *
 */
static char *ImgSrc(const char *pch)
{
    static char achOut[256];
    char *pos;
#define IMGEXT ".gif"
    if(!pch)
    {
        //return "\"\"";
        return "";
    }

    //strcpy(achOut, "\"");
    //strcat(achOut, g_CharacterInfoSettings.achImageWebRoot);
    //strcat(achOut, "/");
    //strcat(achOut, pch);

    strcpy_s(SAFESTR(achOut), pch);
    pos = strstr(achOut, ".tga");
    if(pos)
    {
        strcpy(pos, IMGEXT);
    }
    else
    {
        pos = strstr(achOut, ".psd");
        if(pos)
        {
            strcpy(pos, IMGEXT);
        }
        else if(!strstri(achOut, IMGEXT))
        {
            strcat(achOut, IMGEXT);
        }
    }
    //strcat(achOut, "\"");

    ConvertFilenameToLowercase(achOut);

    return achOut;
}


/**********************************************************************func*
 * SecondsToString
 *
 */
static char *SecondsToString(U32 i)
{
    static char achOut[256];
    int d, h, m, s;
    bool bGot = false;

    achOut[0] = 0;

    d = i/(60*60*24);
    if(d>0)
    {
        strcatf(achOut, "%d days", d);
        i -= d*60*60*24;
        bGot = true;
    }

    h = i/(60*60);
    if(h>0)
    {
        strcatf(achOut, "%s%d hours", bGot?", ":"", h);
        i -= h*60*60;
        bGot = true;
    }

    m = i/60;
    if(m>0)
    {
        strcatf(achOut, "%s%d mins", bGot?", ":"", m);
        i -= m*60;
        bGot = true;
    }

    s = i;
    if(s>0)
    {
        strcatf(achOut, "%s%d secs", bGot?", ":"", s);
        bGot = true;
    }

    return achOut;
}


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

#ifndef UPDATE2
static int dbQueryGetContainerIDForName(int type, char *db_id)
{
    char buf[2000] = "";
    char *s;
    int iRet=atoi(db_id);

    if(iRet==0)
    {
        // Let's guess that db_id is a name instead
        sprintf(buf, "-dbquery -find %d name \"%s\"", type, db_id);
        s = dbQuery(buf);
        if(s && strnicmp("\ncontainer_id = ", s, 16)==0)
        {
            char *pch=strstr(s, " = ");
            if(pch)
            {
                iRet = atoi(pch+3);
            }
        }
        else
        {
            printf("Unable to get container_id for \"%s\"\n", db_id);
        }
    }

    return iRet;
}
#endif

/**************************************************************************/
/**************************************************************************/
/**************************************************************************/

/**********************************************************************func*
 * SupergroupCallback
 *
 */
static void SupergroupCallback(Packet *pak, int iId, int count)
{
    if(count>0)
    {
        char *pch = strdup(unescapeString(pktGetString(pak)));
        stashIntAddPointer(s_hashSupergroups, iId, pch, false);
    }
}

/**********************************************************************func*
 * GetSupergroupName
 *
 */
static char *GetSupergroupName(Entity *e)
{
    char* pcResult = NULL;

    if(!s_hashSupergroups)
    {
        s_hashSupergroups = stashTableCreateInt(4);
    }

    if(e && e->supergroup_id)
    {
        if(!stashIntFindPointer(s_hashSupergroups, e->supergroup_id, &pcResult))
        {
            char achWhere[1024];

            sprintf(achWhere, "WHERE ContainerId=%d", e->supergroup_id);
            dbReqCustomData(CONTAINER_SUPERGROUPS, "Supergroups", "", achWhere, "Name", SupergroupCallback, e->supergroup_id);
            dbMessageScanUntil("DBCLIENT_REQ_CUSTOM_DATA", NULL);

            if (!stashIntFindPointer(s_hashSupergroups, e->supergroup_id, &pcResult))
                pcResult = NULL;
        }
    }

    if(pcResult)
        return pcResult;


    return "";
}

/**********************************************************************func*
 * EscapeApostrophes
 *
 */
static char *EscapeApostrophes(char *pch)
{
    static char *estr = NULL;
    char *p;

    if(!estr)
        estrCreate(&estr);
    else
        estrClear(&estr);

    while((p = strchr(pch, '\''))!=NULL)
    {
        estrConcatFixedWidth(&estr, pch, p-pch);
        estrConcatStaticCharArray(&estr, "\\'");
        pch = p+1;
    }
    estrConcatf(&estr, "%s", pch);

    return estr;
}

/**********************************************************************func*
 * ReplaceVars
 *
 */
static char *ReplaceVars(char **pestr, char *pchSrc, char *pchHeroName, char *pchShardName, char *pchHeroHTML)
{
    while(1)
    {
        char *pchHero = strstri(pchSrc, "{HeroName}");
        char *pchShard = strstri(pchSrc, "{ShardName}");
        char *pchHTML = strstri(pchSrc, "{HeroHTML}");

        pchHero = pchHero ? pchHero : (char *)-1;
        pchShard = pchShard ? pchShard : (char *)-1;
        pchHTML = pchHTML ? pchHTML : (char *)-1;

        if(pchHero<pchShard && pchHero<pchHTML)
        {
            estrConcatFixedWidth(pestr, pchSrc, pchHero-pchSrc);
            estrConcatCharString(pestr, pchHeroName);
            pchSrc=pchHero+strlen("{HeroName}");
        }
        else if(pchShard<pchHero && pchShard<pchHTML)
        {
            estrConcatFixedWidth(pestr, pchSrc, pchShard-pchSrc);
            estrConcatCharString(pestr, pchShardName);
            pchSrc=pchShard+strlen("{ShardName}");
        }
        else if(pchHTML<pchHero && pchHTML<pchShard)
        {
            estrConcatFixedWidth(pestr, pchSrc, pchHTML-pchSrc);
            estrConcatCharString(pestr, pchHeroHTML);
            pchSrc=pchHTML+strlen("{HeroHTML}");
        }
        else
        {
            estrConcatCharString(pestr, pchSrc);
            break;
        }
    }

    return *pestr;
}



/* End of File */
