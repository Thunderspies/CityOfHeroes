#include "cmdparse/cmdgame.h"
#include "language/langClientUtil.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/language/MessageStoreUtil.h>
#include "language/commonLangUtil.h"
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/Earray.h>
#include "auth/authUserData.h"
#include <utilitieslib/utils/osdependent.h>
#include <utilitieslib/utils/utils.h>

MessageStore* cmdMessages;
MessageStore* texWordsMessages;
MessageStore* loadingTipMessages;

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
    reloadClientMessageStores(getCurrentLocale());
}

#define estr_print(dst,src) estrPrintf(&dst,"texts\\%s\\%s", loc_name, src)
#define push_estr(ea,str) { char *temp_str = NULL; estr_print(temp_str, str); eaPush(&ea, temp_str); }

void reloadClientMessageStores(int localeID) {
    static bool inited = false;

    char** ppchMessageFiles = { NULL };
    char** ppchMessageDirs = { NULL };
    char** ppchTexWordsFiles = { NULL };
    char** ppchLoadingTipFiles = { NULL };
    char *pchCmdMessageFile = NULL;
    char *idPrepend = NULL;
    MessageStoreLoadFlags flags = game_state.create_bins ? MSLOAD_FORCEBINS : MSLOAD_DEFAULT;
    char *loc_name = locGetName(localeID);

    if (game_state.skin == UISKIN_VILLAINS) {
        idPrepend = "v_";
    } else if (game_state.skin == UISKIN_PRAETORIANS) {
        idPrepend = "p_";
    }

    // generate the list of files/folders
    push_estr(ppchMessageFiles, "menuMessages.ms"); eaPush(&ppchMessageFiles, estrCloneCharString("texts\\menuMessages.types"));
    push_estr(ppchMessageFiles, "v_menuMessages.ms"); eaPush(&ppchMessageFiles, estrCloneCharString("texts\\v_menuMessages.types"));
    push_estr(ppchMessageFiles, "Inventions\\Salvage.xls.ms"); eaPush(&ppchMessageFiles, NULL);
    push_estr(ppchMessageFiles, "Inventions\\Salvage_rewards.xls.ms"); eaPush(&ppchMessageFiles, NULL); 
    push_estr(ppchMessageFiles, "powerThemes.ms"); eaPush(&ppchMessageFiles, NULL);

    if (!IsUsingCider()) {
        // PC versions
        // For creating ENGLIGH bins, load both UK and NA EULAs and NDAs
        if (game_state.create_bins && localeID == LOCALE_ID_ENGLISH) {
            push_estr(ppchMessageFiles, "eula-uk.ms"); eaPush(&ppchMessageFiles, NULL);
            push_estr(ppchMessageFiles, "nda-uk.ms"); eaPush(&ppchMessageFiles, NULL);
            push_estr(ppchMessageFiles, "eula.ms"); eaPush(&ppchMessageFiles, NULL);
            push_estr(ppchMessageFiles, "nda.ms"); eaPush(&ppchMessageFiles, NULL);
        } else if (locIsBritish(localeID)) {
            // Handle UK specific EULA/NDA
            push_estr(ppchMessageFiles, "eula-uk.ms"); eaPush(&ppchMessageFiles, NULL);
            push_estr(ppchMessageFiles, "nda-uk.ms"); eaPush(&ppchMessageFiles, NULL);
        } else {
            push_estr(ppchMessageFiles, "eula.ms"); eaPush(&ppchMessageFiles, NULL);
            push_estr(ppchMessageFiles, "nda.ms"); eaPush(&ppchMessageFiles, NULL);
        }
    } else {
        // Mac versions
        // We should not be creating bins on the Mac so we don't need the extra logic from above
        devassert(!game_state.create_bins);
        if (locIsBritish(localeID)) {
            // Handle UK specific EULA/NDA
            push_estr(ppchMessageFiles, "eula-uk-mac.ms"); eaPush(&ppchMessageFiles, NULL);
            push_estr(ppchMessageFiles, "nda-uk-mac.ms"); eaPush(&ppchMessageFiles, NULL);
        } else {
            push_estr(ppchMessageFiles, "eula-mac.ms"); eaPush(&ppchMessageFiles, NULL);
            push_estr(ppchMessageFiles, "nda-mac.ms"); eaPush(&ppchMessageFiles, NULL);
        }
    }
    eaPush(&ppchMessageFiles, NULL);

    push_estr(ppchMessageDirs, "powers");
    push_estr(ppchMessageDirs, "badges");
    push_estr(ppchMessageDirs, "PopHelp");
    push_estr(ppchMessageDirs, "classes");
    push_estr(ppchMessageDirs, "origins");
    push_estr(ppchMessageDirs, "boostset");
    push_estr(ppchMessageDirs, "attribs");
    push_estr(ppchMessageDirs, "costume");
    push_estr(ppchMessageDirs, "bases");
    push_estr(ppchMessageDirs, "defs");
    push_estr(ppchMessageDirs, "cards");
    push_estr(ppchMessageDirs, "villains");
    push_estr(ppchMessageDirs, "Player_Created");
    eaPush(&ppchMessageDirs, NULL);

    push_estr(ppchTexWordsFiles, "textureWords.ms"); //eaPush(&ppchTexWordsFiles, NULL);
    push_estr(ppchTexWordsFiles, "v_textureWords.ms"); eaPush(&ppchTexWordsFiles, NULL);
    eaPush(&ppchTexWordsFiles, NULL);

    push_estr(ppchLoadingTipFiles, "loadingTips.ms"); eaPush(&ppchLoadingTipFiles, NULL);
    eaPush(&ppchLoadingTipFiles, NULL);

    estr_print(pchCmdMessageFile, "cmdMessagesClient.ms");

    // do the actual loading
    LoadMessageStore(&menuMessages, ppchMessageFiles, ppchMessageDirs, localeID, idPrepend, "clientmessages", NULL, flags);
    unsigned int i;
    for (i = 0; ppchMessageFiles && ppchMessageFiles[i]; i += 2) {
        writeConsole(OUTPUT_DEBUG, "\t%s", ppchMessageFiles[i]);
    }
    for (i = 0; ppchMessageDirs && ppchMessageDirs[i]; i++) {
        writeConsole(OUTPUT_DEBUG, "\t%s\\", ppchMessageDirs[i]);
    }
    installCustomMessageStoreHandlers(menuMessages);

    LoadMessageStore(&texWordsMessages, ppchTexWordsFiles, NULL, localeID, idPrepend, NULL, NULL, MSLOAD_EXTENDED);
    for (i = 0; ppchTexWordsFiles && ppchTexWordsFiles[i]; i += 2) {
        writeConsole(OUTPUT_DEBUG, "\t%s", ppchTexWordsFiles[i]);
    }

    LoadMessageStore(&loadingTipMessages, ppchLoadingTipFiles, NULL, localeID, idPrepend, "loadingtipmessages", NULL, flags);
    for (i = 0; ppchLoadingTipFiles && ppchLoadingTipFiles[i]; i += 2) {
        writeConsole(OUTPUT_DEBUG, "\t%s", ppchLoadingTipFiles[i]);
    }

    msCreateCmdMessageStore(pchCmdMessageFile, localeID);
    writeConsole(OUTPUT_DEBUG, "\t%s", pchCmdMessageFile);

    // cleanup
    eaDestroyEx(&ppchMessageFiles, estrDestroyUnsafe);
    eaDestroyEx(&ppchMessageDirs, estrDestroyUnsafe);
    eaDestroyEx(&ppchTexWordsFiles, estrDestroyUnsafe);
    eaDestroyEx(&ppchLoadingTipFiles, estrDestroyUnsafe);
    estrDestroy(&pchCmdMessageFile);

    // setup reloading
    if (!inited && !game_state.texWordEdit) {
        char localePath[2000];
        inited = true;
        sprintf(localePath, "texts/%s/*.ms", loc_name);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, localePath, reloadTextCallback);
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "texts/*.types", reloadTextCallback);
    }
}

