#include "cmdparse/cmdserver.h"
#include "language/langServerUtil.h"
#include <utilitieslib/language/MultiMessageStore.h>
#include <utilitieslib/utils/error.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/textparser.h>
#include "entity/entPlayer.h"
#include "entity/entity.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/EString.h>
#include "entity/entPlayer.h"
#include "entity/entity.h"
#include "svr/svr_base.h"
#include <utilitieslib/language/MessageStore.h>
#include "language/commonLangUtil.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include "auth/authUserData.h"

MultiMessageStore* svrMenuMessages;
MessageStore*    cmdMessages;

int translateFlag(const Entity *e)
{
    if( ENT_IS_IN_PRAETORIA(e) )
    {
        if( ENT_IS_VILLAIN(e) )
            return TRANSLATE_PREPEND_L_P;
        else
            return TRANSLATE_PREPEND_P;
    }
    else
    {
        if( ENT_IS_VILLAIN(e) )
            return 0;
        else
            return TRANSLATE_NOPREPEND;
    }
}

/****************************************************************************************/

static char* menuMessagesTranslate(int prepend_flags, int localeID, const char* str, va_list arg)
{
    static char* bufs[5] = {0};
    static U32 bufRotor = 0;    
    U32 bufCur = bufRotor++%ARRAY_SIZE( bufs );
    int requiredBufferSize;
    int strLength;

    if (!str)
        return NULL;

    strLength = (int)strlen(str);
    requiredBufferSize = msxPrintfVA(svrMenuMessages, NULL, 0, localeID, str, NULL, NULL, prepend_flags, arg);
    requiredBufferSize = MAX(requiredBufferSize, strLength);
    estrSetLength( &bufs[bufCur], requiredBufferSize );

    if(msxPrintfVA(svrMenuMessages, bufs[bufCur], requiredBufferSize + 1, localeID, str, NULL, NULL, prepend_flags, arg) == -1)
        return NULL;

    return bufs[bufCur];
}

/*
 *
 ****************************************************************************************/

// This function should be used whenever a display string is being constructed by the server.
char* localizedPrintf(const Entity * e, const char* messageID,  ...)
{
    char* msg;
    va_list    arg;
    int prepend = 0;
    int locale = getCurrentLocale();

    if(!messageID)
        return NULL;

    if( e )
    {
        prepend = translateFlag( e );
        locale = e->playerLocale;
    }

    va_start(arg, messageID);
    PERFINFO_AUTO_START("menuMessagesTranslate", 1);

    msg = menuMessagesTranslate( prepend, locale, messageID, arg);
    PERFINFO_AUTO_STOP();
    va_end(arg);
    return msg;
}

char* localizedPrintfVA(const Entity * e, const char* messageID, va_list arg)
{
    char* msg;
    int prepend = 0;
    int locale = getCurrentLocale();

    if(!messageID)
        return NULL;

    if( e )
    {
        prepend = translateFlag( e );
        locale = e->playerLocale;
    }

    PERFINFO_AUTO_START("menuMessagesTranslate", 1);
    msg = menuMessagesTranslate( prepend, locale, messageID, arg);
    PERFINFO_AUTO_STOP();

    return msg;
}

const char* clientPrintf( ClientLink * client, const char* str, ...)
{
    if( client && client->entity )
    {
        char * msg;
        va_list    arg;
        va_start(arg, str);
        PERFINFO_AUTO_START("menuMessagesTranslate", 1);
        msg = localizedPrintfVA( client->entity, str, arg);
        PERFINFO_AUTO_STOP();
        va_end(arg);

        return msg;
    }
    else
        return str;
}

int isLocalizeable(const Entity *e, const char *messageID)
{
    return messageID != msxGetUnformattedMessageConst(svrMenuMessages, e->playerLocale, messageID);
}

/**********************************************************************func*
*
*
*/
static void reloadTextCallback(const char *relpath, int when)
{
    bool loop;
    int count=0;
    do {
        loop=false;
        fileWaitForExclusiveAccess(relpath);
        errorLogFileIsBeingReloaded(relpath);
        if (!fileExists(relpath)) {
            loop=true;
            count++;
            Sleep(10);
        }
    } while (loop && count<25);
    loadServerMessageStores();
}

void loadServerMessageStores(void)
{
    static int inited = 0;

    char                    cmdpath[2000];
    int                        locale = getCurrentLocale();
    MessageStoreLoadFlags    flags = MSLOAD_SERVERONLY;

    char* apchMessageFiles[] = {
        "texts\\menuMessages.ms",    "texts\\menuMessages.types",
        "texts\\powers.txt",         NULL,
        NULL
    };

    char* apchMessageDirs[] = { 
        "/texts/powers",
        "/texts/badges",
        "/texts/server/badges_svr",
        "/texts/server/lockedDoors",
        "/texts/reward",
        "/texts/classes",
        "/texts/origins",
        "/texts/boostset",
        "/texts/attribs",
        "/texts/bases",
        "/texts/inventions",
        "/texts/Villains",
        NULL
    };

    if(server_state.create_bins)
    {
        locale = -1; // load all locales
        flags |= MSLOAD_FORCEBINS;
    }

    // All the ordinary text message stores can co-exist in shared memory
    if(server_state.tsr >= 2)
        locale = -1;

    LoadMultiMessageStore(&svrMenuMessages,"messages","svrMenuMessages",locale,
        "v_",apchMessageFiles,apchMessageDirs,installCustomMessageStoreHandlers,flags);

    sprintf(cmdpath, "texts\\%s\\cmdMessagesServer.ms", locGetName(locale));
    msCreateCmdMessageStore(cmdpath, 0);
    setCurrentMenuMessages();

    if(!inited)
    {
        inited = 1;
        sprintf(cmdpath, "texts/%s/*.ms", locGetName(locale));
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, cmdpath, reloadTextCallback);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "texts/*.types", reloadTextCallback);
    }

}

void setCurrentMenuMessages()
{
    menuMessages = svrMenuMessages->localizedMessageStore[getCurrentLocale()];
}