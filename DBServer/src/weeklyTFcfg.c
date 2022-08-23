#include "weeklyTFcfg.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>
#include "dbrelay.h"
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/utils.h>
#include "dbinit.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/utils/tokenstore.h>
#include <utilitieslib/utils/file.h>

static int weekly_TF_reload = 0;

void WeeklyTF_GenerateTokenString(const WeeklyTFCfg* weeklyTF_cfg, char **buff)
{
    if (weeklyTF_cfg)
    {
        int numTokens = eaSize(&weeklyTF_cfg->weeklyTFTokens);
        estrConcatf(buff, "%i", (int)weeklyTF_cfg->epochStartTime);    //    convert to int for send, the atoi set will convert back to uint
        if (numTokens)
        {
            int i;
            for (i = 0; i < numTokens; ++i)
            {
                estrConcatf(buff, ":%s", weeklyTF_cfg->weeklyTFTokens[i]);
            }
        }
        else
        {
            estrConcatStaticCharArray(buff, ":none");
        }
    }
}
static void WeeklyTF_UpdateServers(WeeklyTFCfg *weeklyTF_cfg)
{
    if (weeklyTF_cfg)
    {
        char *buff = estrTemp();
        estrConcatStaticCharArray(&buff, "cmdrelay\nweeklyTF_db_updateTokenList ");
        WeeklyTF_GenerateTokenString(weeklyTF_cfg, &buff);
        sendToServers(-1, buff, 0);
        estrDestroy(&buff);
    }
}

static void WeeklyTFReloadCallback(const char *relPath, int when)
{
    WeeklyTFCfgLoad();
    if (map_list)
        WeeklyTF_UpdateServers(WeeklyTFCfg_getCurrentWeek(1)); // this shouldn't trigger WeeklyTF_UpdateServers inside itself
}

TokenizerParseInfo ParseDateTime[] = {
    { ".",                    TOK_STRUCTPARAM | TOK_STRING(DateTime,dateStr, 0), },
    { ".",                    TOK_STRUCTPARAM | TOK_STRING(DateTime,timeStr, 0), },
    { "\n",                    TOK_END },
    { "", 0, 0 }
};

TokenizerParseInfo ParseWeeklyTFConfig[] = {
    { "{",                    TOK_START    },
    { "EpochStartTime",        TOK_OPTIONALSTRUCT(WeeklyTFCfg,dateTime, ParseDateTime), },
    { "WeeklyTFTokens",        TOK_STRINGARRAY(WeeklyTFCfg,weeklyTFTokens), },
    { "}",                    TOK_END    },
    { "", 0, 0 }
};

TokenizerParseInfo ParseWeeklyTFConfigList[] = {
    { "WeeklyTF",        TOK_STRUCT(WeeklyTFCfgList,weeklyTF_cfg, ParseWeeklyTFConfig), },
    { "", 0, 0 }
};

WeeklyTFCfgList wtf_list;

static int wtf_comparator(const WeeklyTFCfg** a, const WeeklyTFCfg** b)
{
    if ((*a)->epochStartTime == (*b)->epochStartTime)
        return 0;
    return ((*a)->epochStartTime > (*b)->epochStartTime) ? 1 : -1;
}
WeeklyTFCfg* WeeklyTFCfg_getCurrentWeek(int reload)
{
    static int lastCheck = 0;
    static WeeklyTFCfg *currentWeek = NULL;
    int i;
    unsigned int now = timerSecondsSince2000();

    if(reload)
        currentWeek = NULL;

    if (!currentWeek || now - lastCheck > (SECONDS_PER_MINUTE * 5))
    {
        WeeklyTFCfg *retWeek = NULL;
        for (i = 0; i < eaSize(&wtf_list.weeklyTF_cfg); ++i)
        {
            WeeklyTFCfg *wtf_cfg = wtf_list.weeklyTF_cfg[i];
            if (now < wtf_cfg->epochStartTime)
            {
                if (!retWeek)
                {
                    retWeek = wtf_cfg;
                }
                break;
            }
            retWeek = wtf_cfg;
        }
        lastCheck = now;
        if (currentWeek)
        {
            if (retWeek != currentWeek)
            {
                WeeklyTF_UpdateServers(retWeek);
            }
        }
        currentWeek = retWeek;
    }
    devassert(currentWeek);
    return currentWeek;
}
void WeeklyTFCfgLoad()
{
    FILE    *file = NULL;
    char*    relFilename = "server/db/weeklyTF.cfg";
    static bool loaded_once = false;
    printf("Loading %s\n", relFilename);
    
    StructClear(ParseWeeklyTFConfigList, &wtf_list);
    ParserLoadFiles(NULL, relFilename, NULL, 0, ParseWeeklyTFConfigList, &wtf_list, NULL, NULL, NULL);
    {
        int i;
        char *dateTimeStr = NULL;
        for (i = 0; i < eaSize(&wtf_list.weeklyTF_cfg); ++i)
        {
            WeeklyTFCfg *wtf_cfg = wtf_list.weeklyTF_cfg[i];
            if (devassert(wtf_cfg->dateTime && wtf_cfg->dateTime->dateStr && wtf_cfg->dateTime->timeStr))
            {
                estrClear(&dateTimeStr);
                estrConcatf(&dateTimeStr, "%s %s", wtf_cfg->dateTime->dateStr, wtf_cfg->dateTime->timeStr);
                wtf_cfg->epochStartTime = timerGetSecondsSince2000FromString(dateTimeStr);
            }
        }
        estrDestroy(&dateTimeStr);
        eaQSort(wtf_list.weeklyTF_cfg, wtf_comparator);
    }
    
    
    if (!loaded_once) {
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE|FOLDER_CACHE_CALLBACK_CAN_USE_SHARED_MEM, "server/db/WeeklyTF.cfg", WeeklyTFReloadCallback);
    }

    loaded_once = true;
}

void WeeklyTF_AddTFToken(WeeklyTFCfg* weeklyTF_cfg, char *s)
{
    if (weeklyTF_cfg && StringArrayFind(weeklyTF_cfg->weeklyTFTokens, s) == -1)
    {
        eaPush(&weeklyTF_cfg->weeklyTFTokens, strdup(s));
        WeeklyTF_UpdateServers(weeklyTF_cfg);
    }
}
void WeeklyTF_RemoveTFToken(WeeklyTFCfg* weeklyTF_cfg, char *s)
{
    int index;
    if (weeklyTF_cfg && (index = StringArrayFind(weeklyTF_cfg->weeklyTFTokens, s)) != -1)
    {
        free(eaRemove(&weeklyTF_cfg->weeklyTFTokens, index));
        WeeklyTF_UpdateServers(weeklyTF_cfg);
    }
}
void WeeklyTF_SetEpochTime(WeeklyTFCfg* weeklyTF_cfg, char *date, char *time)
{
    if (weeklyTF_cfg && date && time)
    {
        char dateTime[25];
        unsigned int newTime;
        sprintf_s(SAFESTR(dateTime), "%s %s", date, time);
        newTime = timerGetSecondsSince2000FromString(dateTime);
        if (newTime != weeklyTF_cfg->epochStartTime)
        {
            weeklyTF_cfg->epochStartTime = newTime;
            eaQSort(wtf_list.weeklyTF_cfg, wtf_comparator);
            WeeklyTF_UpdateServers(weeklyTF_cfg);
        }
    }
}