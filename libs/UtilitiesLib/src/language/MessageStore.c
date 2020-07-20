#include "utilitieslib/utils/superassert.h"
#include "string.h"
#include "utilitieslib/language/MessageStore.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/fileutil.h"
#include "utilitieslib/utils/utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/stringutil.h"
#include "utilitieslib/utils/scriptvars.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/language/AppLocale.h"
#include "utilitieslib/components/queue.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/StringTable.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/structdefines.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/Serialize.h"
#include "utilitieslib/utils/log.h"
// needed for localization attribute processing

typedef struct{
    const char*    variableName;
    const char*    variableType;
    U8            variableTypeChar;

    // temp value - filled in during msPrintf and only valid then
    void*        variablePointer;    // Where is this thing stored on the stack?
} NamedVariableDef;

typedef struct TextMessage
{
    const char*        messageID;                // The message ID associated with the message.  For debugging convenience.
    int                messageIndex;            // The message index into a string table,  to be printed as double-byte strings.
    int                helpIndex;                // The help index into a string table,  to be printed as double-byte strings.

    Array*            variableDefNameIndices; // An earray of indices into the namedVariableDefPool string table
                                            // note that the type index is always this +1
} TextMessage;

typedef struct ExtendedTextMessage {
    U32                newMessage:1;
    U32                modified:1;
    char*            commentLine;
} ExtendedTextMessage;


int                        msPrintfError;    // True when the messageID is unknown.
static MessageStore*    cmdMessages;
static U8                UTF8BOM[] = {0xEF, 0xBB, 0xBF};
static int                hideTranslationErrors;

typedef struct MessageStore {
    int                                localeID;                    // locale ID corresponding to those in AppLocale.h
    bool                            cmdstore;
    StashTable                         messageIDStash;                // msg ID -> msg map.
    StashTable                         messageStash;                // only used by cmdstore's because it needs to go both ways

    MemoryPool                        textMessagePool;            // yep, might be overkill.
    StringTable                        messages;                    // actual message storage. Stored as single-byte strings.
    StringTable                        variableStringTable;        // Stores variable name strings and varable type strings.  Stored as single-byte strings.
    char*                            idPrepend;                    // the string to try prepending to message ids, (fallback to un-prepended if fail, or not set)

    char*                            fileLoadedFrom;                // The file this store was loaded from (for saving changes)
    bool                            bLocked;
    
    SharedHeapHandle*                pHandle;                    // if it's in shared heap, this will be the handle to that

    MessageStoreFormatHandler        formatHandler[26][2];        // One each for upper and lower case letters.
    
    StaticDefineInt*                attributes;
    StaticDefineInt**                attribToValue;
    S32*                            valueOffsetInUserData;
    
    U32                                useExtendedTextMessage : 1;    // Enables extra info that's used by texWords.
} MessageStore;

// these used to be external
static MessageStore* createMessageStore(MessageStoreLoadFlags flags);
static void initMessageStore(MessageStore* store, int locid, char const *idPrepend);
static int msAddMessages(MessageStore* store, MessageStoreLoadFlags flags, const char* messageFilename, const char* messageTypeFilename);
static int msAddMessagesFromMem(MessageStore* store, MessageStoreLoadFlags flags, char *messageData, char *messageTypes);
static bool msMoveStoreIntoSharedHeap(MessageStore* store);
static void setStorePointersToSharedHeap(MessageStore *store);
static int msWriteBin(const char *fname, MessageStore *store);
static int msReadBin(const char *fname, MessageStore *store);
static void setCmdMessageStore( MessageStore *store );

// also used in MultiMessageStore.c
void destroyMessageStore(MessageStore* store);

typedef struct MSFileInfo
{
    __time32_t time_write;
    char filename[1]; // str hack
} MSFileInfo;

static MSFileInfo ***s_dirfiles = NULL;
static FileScanAction msGetDirFilesListHelper(char* dir, struct _finddata32_t* data)
{
    FileScanAction retval = FSA_EXPLORE_DIRECTORY;
    MSFileInfo *info;
    size_t len;

    // Ignore all directories.
    if(data->attrib & _A_SUBDIR)
        return retval;

    // Ignore all .bak files.
    if(strEndsWith(data->name, ".bak"))
        return retval;

    // Store virtual filenames.
    len = strlen(dir) + 1 + strlen(data->name); // "dir/name"
    info = malloc(len + sizeof(MSFileInfo)); // sizeof(MSFileInfo) includes room for filename's terminator
    info->time_write = data->time_write;
    sprintf_s(info->filename,len+1,"%s/%s",dir,data->name);
    eaPush(s_dirfiles,info);

    return retval;
}

static void msGetDirFilesList(MSFileInfo ***dirfiles, char **dirs)
{
    int i;

    assert(s_dirfiles==NULL); // not thread-safe

    s_dirfiles = dirfiles;
    for(i = 0; dirs && dirs[i]; ++i)
    {
        // Get a list of filenames in the directory.
        fileScanAllDataDirs(dirs[i], msGetDirFilesListHelper);
    }
    s_dirfiles = NULL;
}

void LoadMessageStore( MessageStore **pStore, char **files, char **dirs, int locid, const char *idPrepend,
                        char* persistfile, char *pcSharedMemoryName, MessageStoreLoadFlags flags )
{
    int i;
    char persistfilepath[MAX_PATH], buf[MAX_PATH];
    MessageStore *store = createMessageStore(flags);
    SharedHeapAcquireResult share_result = SHAR_Error;
    bool reloading = *pStore != NULL || (flags & MSLOAD_RELOADING);
    bool good_bin = false;
    bool handle_files = (!persistfile || isDevelopmentMode()) && !(flags & MSLOAD_NOFILES); // whether or not we should look at the original files
    MSFileInfo **dirfiles = NULL;

    assert(pStore);

    // Clear out the old store, if we have one
    if(reloading)
        destroyMessageStore(*pStore);

    if(persistfile)
    {
        // build the path to the bin file
        sprintf(persistfilepath,"%sbin/%s-%s.bin",
            (flags & MSLOAD_SERVERONLY ? "server/" : ""),
            persistfile,locGetAlpha2(locid));

        // Don't let them get a shared memory handle they aren't able to fill
        if(!handle_files && !fileLocateRead(persistfilepath,buf))
        {
            while(!fileLocateRead(persistfilepath,buf))
            {
                if(isProductionMode()) // this should only happen in dev mode when one app requires another to make bins
                    FatalErrorFilenamef(persistfilepath,"Unable to locate %s",persistfilepath);
                printf("Waiting on %s...\n",persistfilepath);
                Sleep(5000);
            }
            Sleep(5000); // give the other app a chance to write the file
        }
    }

    // Load from shared memory? (this is an early out)
    if(pcSharedMemoryName && !reloading && !(flags & MSLOAD_FORCEBINS))
    {
        // First, get the name right
        char cSharedName[128];
        STR_COMBINE_SSD(cSharedName, pcSharedMemoryName, "-", locid);

        share_result = sharedHeapAcquire(&store->pHandle, cSharedName);
        if(share_result == SHAR_DataAcquired)
        {
            setStorePointersToSharedHeap(store);
            store->localeID = locid;
            msSetPrepend(store, idPrepend);
            // not creating a memory pool for new texts
            *pStore = store;
            return;
        }
    }

    if(handle_files)
    {
        // get a list of files and timestamps for checking the bin's timeliness and/or loading files
        msGetDirFilesList(&dirfiles,dirs);
    }

    // Load from bins?
    if(persistfile && !reloading && !(flags & MSLOAD_FORCEBINS))
    {
        writeConsole(OUTPUT_DEBUG, "Reading from %s", persistfilepath);
        if(fileLocateRead(persistfilepath, buf))
        {
            bool timely_bin = true;

            if(handle_files)
            {
                __time32_t bin_time_write = fileLastChanged(persistfilepath);

                for(i = 0; files && files[i]; i+=2)
                {
                    if( fileLastChanged(files[i]) > bin_time_write ||
                        (files[i+1] && fileLastChanged(files[i+1]) > bin_time_write) )
                    {
                        timely_bin = false;
                        break;
                    }
                }

                for(i = eaSize(&dirfiles)-1; timely_bin && i >= 0; --i)
                    if(dirfiles[i]->time_write > bin_time_write)
                        timely_bin = false;
            }

            if(timely_bin) // bin is up-to-date
            {
                // try loading the store
                if(msReadBin(persistfilepath, store))
                {
                    good_bin = true;
                }
                else
                {
                    // wasteful and hacky, but I'd like to ensure a fresh copy
                    SharedHeapHandle *pHandle = store->pHandle;
                    store->pHandle = NULL;
                    destroyMessageStore(store);
                    store = createMessageStore(flags);
                    store->pHandle = pHandle;
                }
            }
            // else just load from the files
        }
    }

    // Load from the actual files?
    if(!good_bin)
    {
        if(handle_files)
        {
            // it looks like we're doing this the slow way
            initMessageStore(store, locid, idPrepend);

            for(i = 0; files && files[i]; i+=2)
                msAddMessages(store, flags, files[i], files[i+1]);
            for(i = 0; i < eaSize(&dirfiles); ++i)
                msAddMessages(store, flags, dirfiles[i]->filename,NULL);

            if(persistfile)
            {
                mkdirtree(fileLocateWrite(persistfilepath, buf));
                if(!msWriteBin(buf,store)) // make it fast next time
                {
                    if(flags & MSLOAD_FORCEBINS)
                        FatalErrorFilenamef(persistfilepath,"Could not write %s",persistfilepath);
                    else
                        ErrorFilenamef(persistfilepath,"Could not write %s",persistfilepath);
                }
            }
        }
        else
        {
            // Something's wrong, we're in production, required a bin, and couldn't load it
            FatalErrorFilenamef(persistfilepath,"Could not load %s",persistfilepath);
        }
    }

    // If we got a shared memory handle earlier, lets fill it up to help out the other loading schmoes
    if(share_result == SHAR_FirstCaller)
        msMoveStoreIntoSharedHeap(store);

    eaDestroyEx(&dirfiles,NULL); // free the temporary directory listing

    store->localeID = locid;
    msSetPrepend(store, idPrepend);

    *pStore = store;
}

void LoadMessageStoreFromMem(MessageStore **pStore, char *messages, char *types)
{
    const MessageStoreLoadFlags flags = MSLOAD_DEFAULT;

    assert(pStore);

    // Clear out the old store, if we have one
    if(*pStore != NULL)
        destroyMessageStore(*pStore);

    *pStore = createMessageStore(flags);
    initMessageStore(*pStore,0,NULL);
    msAddMessagesFromMem(*pStore, flags, messages, types);
}

MessageStore* createMessageStore(MessageStoreLoadFlags flags)
{
    MessageStore* store = calloc(1, sizeof(*store));
    store->useExtendedTextMessage = !!(flags & MSLOAD_EXTENDED);
    return store;
}

static void setCmdMessageStore( MessageStore *store )
{
    store->cmdstore = true;
}

void msSetHideTranslationErrors(int hideErrors)
{
    hideTranslationErrors = !!hideErrors;
}

int msGetHideTranslationErrors(void)
{
    return hideTranslationErrors;
}

void destroyTextMessage(TextMessage* textMessage){
    destroyArray(textMessage->variableDefNameIndices);
    textMessage->variableDefNameIndices = NULL;
}

void destroyMessageStore(MessageStore* store)
{
    if(!store){
        return;
    }
    
    if ( store->pHandle ) // otherwise, its in shared memory
    {
        sharedHeapMemoryManagerLock();
        sharedHeapRelease(store->pHandle);
        sharedHeapMemoryManagerUnlock();
        store->pHandle = NULL;
    }
    else
    {
        if(store->messageIDStash)
            stashTableDestroyEx(store->messageIDStash, NULL, destroyTextMessage);
        if(store->messageStash)
            stashTableDestroy(store->messageStash);
        if(store->textMessagePool)
            destroyMemoryPool(store->textMessagePool);
        if(store->messages)
            destroyStringTable(store->messages);
        if(store->variableStringTable)
            destroyStringTable(store->variableStringTable);
    }

    SAFE_FREE(store->fileLoadedFrom);

    SAFE_FREE(store);
}


/* Function readEnclosedString()
 *    Extract a string that is enclosed by the specified characters.
 *    This function destroys the given string.
 */
char* readEnclosedString(char** input, char openDelim, char closeDelim){
    char openDelimStr[2];
    char closeDelimStr[2];
    char* beginEnclosed;

    openDelimStr[0] = openDelim;
    openDelimStr[1] = '\0';

    closeDelimStr[0] = closeDelim;
    closeDelimStr[1] = '\0';

    if(beginEnclosed = strsep(input, openDelimStr)){
        char* token;
        token = strsep(input, closeDelimStr);
        return token;
    }

    return NULL;
}

char* readQuotedString(char** input){
    return readEnclosedString(input, '\"', '\"');
}

void eatWhitespace(char** str){
    while(**str == ' ')
        (*str)++;
}

static U32 getNumNamedVariableDefs(StashTable pTable, U32 uiNumTextMessages )
{
    StashTableIterator iter;
    StashElement elem;

    U32 uiNumNamedVariableDefIndices = 0;
    U32 uiNumCountedTextMessages = 0;

    stashGetIterator(pTable, &iter);
    while (stashGetNextElement(&iter, &elem))
    {
        TextMessage* pOldTM = stashElementGetPointer(elem);
        uiNumNamedVariableDefIndices += SAFE_MEMBER(pOldTM->variableDefNameIndices, size);
        uiNumCountedTextMessages++;
    }

    assert( uiNumCountedTextMessages == uiNumTextMessages ); // assert that the count is correct

    return uiNumNamedVariableDefIndices;
}

static void copyTextMessageStructsToShared(    MessageStore* store,
                                            StashTable pTable,
                                            void* pTextMessageStorageAddress,
                                            size_t uiTextMessageSize)
{
    StashTableIterator iter;
    StashElement elem;
    char* pCurrentAddress = pTextMessageStorageAddress;
    
    assert(!store->useExtendedTextMessage);

    stashGetIterator(pTable, &iter);
    while (stashGetNextElement(&iter, &elem))
    {
        TextMessage* pOldTM = stashElementGetPointer(elem);
        U32 uiNumNamedVariableDefIndices = SAFE_MEMBER(pOldTM->variableDefNameIndices, size);
        TextMessage* pNewTm = (TextMessage*)pCurrentAddress;

        // Copy the text message
        *pNewTm = *pOldTM;

        // Fix the pointer to the named variable defs, and copy if they exist
        pCurrentAddress += sizeof(TextMessage);
        
        if ( uiNumNamedVariableDefIndices > 0 )
        {
            Array* indices = pNewTm->variableDefNameIndices = (Array*)pCurrentAddress;
            pCurrentAddress += sizeof(*indices);
            indices->storage = (void*)pCurrentAddress;
            indices->size = indices->maxSize = uiNumNamedVariableDefIndices;
            memcpy(indices->storage, pOldTM->variableDefNameIndices->storage, sizeof(int) * uiNumNamedVariableDefIndices);
            pCurrentAddress += sizeof(int) * uiNumNamedVariableDefIndices;
        }

        // Fix the messageId pointer
        pNewTm->messageID = stashElementGetStringKey(elem);

        // Fix the stash table's value pointer
        stashElementSetPointer(elem, pNewTm);

        // Free the old variabledefs
        destroyArray(pOldTM->variableDefNameIndices);
        pOldTM->variableDefNameIndices = NULL;
    }

    assert( (char*)pTextMessageStorageAddress + uiTextMessageSize == pCurrentAddress ); // we should exactly fill up the space
}


static void setStorePointersToSharedHeap(MessageStore* store)
{
    void** pTargetAddress = store->pHandle->data;

    void* pVariableStrTableAddress = pTargetAddress[0];
    void* pIdStashTableAddress = pTargetAddress[1];
    void* pMessageStrTableAddress = &pTargetAddress[2]; // just past the header (size 2*sizeof(void*))

    // set the old elements
    store->messageIDStash = pIdStashTableAddress;
    store->messages = pMessageStrTableAddress;
    store->variableStringTable = pVariableStrTableAddress;
}

static size_t getUITextMessageSize(MessageStore* store){
    StashTableIterator iter;
    StashElement elem;
    size_t size = 0;
    
    stashGetIterator(store->messageIDStash, &iter);
    while (stashGetNextElement(&iter, &elem))
    {
        TextMessage* tm = stashElementGetPointer(elem);
    
        size += sizeof(*tm);
        
        if(tm->variableDefNameIndices){
            size += sizeof(*tm->variableDefNameIndices);
        }
    }
    
    return size;
}

static bool msMoveStoreIntoSharedHeap(MessageStore* store)
{
    size_t uiMessageStringTableSize = strTableGetCopyTargetAllocSize(store->messages);
    size_t uiVariableStringTableSize = strTableGetCopyTargetAllocSize(store->variableStringTable);
    size_t uiIdStashSize = stashGetCopyTargetAllocSize(store->messageIDStash);
    size_t uiHeaderSize = sizeof(void*) * 2;

    U32 uiNumTextMessages = stashGetValidElementCount(store->messageIDStash);
    U32 uiNamedVariableDefs = getNumNamedVariableDefs(store->messageIDStash, uiNumTextMessages);
    size_t uiTextMessageSize = getUITextMessageSize(store);
    size_t uiNamedVariableDefIndexArraySize = sizeof(int) * uiNamedVariableDefs;

    size_t uiTotalAllocSize =
        uiMessageStringTableSize
        + uiVariableStringTableSize
        + uiIdStashSize
        + uiHeaderSize
        + uiTextMessageSize
        + uiNamedVariableDefIndexArraySize;

    void* pMessageStrTableAddress;
    void* pVariableStrTableAddress;
    void* pIdStashTableAddress;
    void* pTextMessageStorageAddress;
    void** pTargetAddress;

    // do the alloc
    if ( !sharedHeapAlloc(store->pHandle, uiTotalAllocSize) )
    {
        sharedHeapMemoryManagerUnlock();
        return false;
    }

    pTargetAddress = store->pHandle->data;

    assert( pTargetAddress );

    pMessageStrTableAddress = &pTargetAddress[2]; // past the header
    pVariableStrTableAddress = (char*)pMessageStrTableAddress + uiMessageStringTableSize;
    pIdStashTableAddress = (char*)pVariableStrTableAddress + uiVariableStringTableSize;
    pTextMessageStorageAddress = (char*)pIdStashTableAddress + uiIdStashSize;



    // future processes need to know the offsets
    pTargetAddress[0] = pVariableStrTableAddress;
    pTargetAddress[1] = pIdStashTableAddress;

    // Copy the tables
    strTableCopyToAllocatedSpace(store->messages, pMessageStrTableAddress, uiMessageStringTableSize);
    strTableCopyToAllocatedSpace(store->variableStringTable, pVariableStrTableAddress, uiVariableStringTableSize);
    stashCopyToAllocatedSpace(store->messageIDStash, pIdStashTableAddress, uiIdStashSize);

    // Copy the text message objects and update the hash table values
    copyTextMessageStructsToShared(store, pIdStashTableAddress, pTextMessageStorageAddress, uiTextMessageSize + uiNamedVariableDefIndexArraySize);


    // Free the old objects (the variable def name arrays are already free)
    /*
    if(store->messageStash)
        stashTableDestroy(store->messageStash);
        */
    assert( !store->messageStash ); // for now
    if(store->messageIDStash)
        stashTableDestroy(store->messageIDStash);
    if(store->textMessagePool)
        destroyMemoryPool(store->textMessagePool);
    store->textMessagePool = NULL;
    if(store->messages)
        destroyStringTable(store->messages);
    if(store->variableStringTable)
        destroyStringTable(store->variableStringTable);

    // set the old elements
    store->messageIDStash = pIdStashTableAddress;
    store->messages = pMessageStrTableAddress;
    store->variableStringTable = pVariableStrTableAddress;

    
    // set the read only flags
    stashTableLock(store->messageIDStash);
    strTableLock(store->messages);
    strTableLock(store->variableStringTable);

    store->bLocked = true;

    // Release the shared heap lock
    sharedHeapMemoryManagerUnlock();

    return true;

}

#define MS_BIN_VER 20090521

static int msWriteBin(const char *fname, MessageStore* store)
{
    int written = 0;
    SimpleBufHandle file;
    StashTableIterator si;
    StashElement se;

    assert(!store->useExtendedTextMessage && !store->cmdstore); // I don't want to deal with these

    file = SimpleBufOpenWrite(fname, 1);
    if(!file)
    {
        unlink(fname);
        return 0;
    }

    written += SimpleBufWriteU32(MS_BIN_VER, file);
    written += WriteStringTable(file, store->messages);
    written += WriteStringTable(file, store->variableStringTable);
    written += SimpleBufWriteU32(stashGetValidElementCount(store->messageIDStash), file);
    for(stashGetIterator(store->messageIDStash,&si); stashGetNextElement(&si,&se);)
    {
        U32 i;
        TextMessage *textMessage = stashElementGetPointer(se);
        U32 len = (U32)strlen(textMessage->messageID);
        U32 var_count = SAFE_MEMBER(textMessage->variableDefNameIndices,size);

        written += SimpleBufWriteU32(len, file);
        written += SimpleBufWrite(textMessage->messageID, len, file);
        written += SimpleBufWriteU32(textMessage->messageIndex, file);
        written += SimpleBufWriteU32(textMessage->helpIndex, file);
        written += SimpleBufWriteU32(var_count, file);
        for(i = 0; i < var_count; ++i)
            written += SimpleBufWriteU32((U32)(intptr_t)textMessage->variableDefNameIndices->storage[i], file);
    }

    SimpleBufClose(file);
    return written;
}

static int msReadBin(const char *fname, MessageStore *store)
{
    U32 ver, message_count;
    U32 i;
    char *str = NULL;

    SimpleBufHandle file = SimpleBufOpenRead(fname);
    if(!file)
        return 0;
    
    if(    SimpleBufReadU32(&ver, file) != 4 ||
        ver != MS_BIN_VER ||
        !(store->messages = ReadStringTable(file)) ||
        !(store->variableStringTable = ReadStringTable(file)) ||
        SimpleBufReadU32(&message_count, file) != 4 ||
        message_count == 0 )
    {
        SimpleBufClose(file);
        return 0;
    }

    store->textMessagePool = createMemoryPool();
    initMemoryPool(store->textMessagePool, sizeof(TextMessage), message_count); // not sizing for ExtendedTextMessages
    mpSetMode(store->textMessagePool, ZERO_MEMORY_BIT);
    store->messageIDStash = stashTableCreateWithStringKeys(message_count+(message_count>>1), StashDeepCopyKeys); // 50% more so no resizing
    // not creating store->messageStash, since we don't bin cmdstores
    // not checking locale, since it's encoded in the file name

    for(i = 0; i < message_count; ++i)
    {
        TextMessage *textMessage = mpAlloc(store->textMessagePool);
        U32 len = 0;
        U32 var_count = 0;

        if( SimpleBufReadU32(&len, file) != 4 ||
            estrSetLengthNoMemset(&str, len) != len ||
            SimpleBufRead(str, len, file) != (int)len ||
            SimpleBufReadU32(&textMessage->messageIndex, file) != 4 ||
            SimpleBufReadU32(&textMessage->helpIndex, file) != 4 ||
            SimpleBufReadU32(&var_count, file) != 4 )
        {
            SimpleBufClose(file);
            estrDestroy(&str);
            return 0;
        }

        if(var_count)
        {
            U32 j;
            textMessage->variableDefNameIndices = createArray();
            for(j = 0; j < var_count; ++j)
            {
                U32 var;
                if(SimpleBufReadU32(&var, file) != 4)
                {
                    SimpleBufClose(file);
                    estrDestroy(&str);
                    return 0;
                }
                arrayPushBack(textMessage->variableDefNameIndices, U32_TO_PTR(var));
            }
        }

        if( !stashAddPointer(store->messageIDStash, str, textMessage, false) ||
            !verify(stashGetKey(store->messageIDStash, str, &textMessage->messageID)) )
        {
            SimpleBufClose(file);
            estrDestroy(&str);
            return 0;
        }
    }

    SimpleBufClose(file);
    estrDestroy(&str);
    return 1;
}

static void initMessageStore(MessageStore* store, int locid, char const *idPrepend)
{
    S32 textMessageSize =    sizeof(TextMessage) +
                            (store->useExtendedTextMessage ? sizeof(ExtendedTextMessage) : 0);

    // Initialize message store internal variables.
    #define EXPECTED_MESSAGE_COUNT 100

    if( store->cmdstore )
        store->messageStash = stashTableCreateWithStringKeys(EXPECTED_MESSAGE_COUNT, StashDeepCopyKeys);

    // Create the TextMessage pool.
    store->textMessagePool = createMemoryPool();
    initMemoryPool(    store->textMessagePool, textMessageSize, EXPECTED_MESSAGE_COUNT);
    mpSetMode(store->textMessagePool, ZERO_MEMORY_BIT);

    store->messageIDStash = stashTableCreateWithStringKeys(EXPECTED_MESSAGE_COUNT, StashDeepCopyKeys);
    store->messages = strTableCreate(Indexable);
    store->variableStringTable = strTableCreate(Indexable);

    #undef EXPECTED_MESSAGE_COUNT

    store->localeID = locid;
    msSetPrepend(store, idPrepend);
}


// returns 1 if message doesn't contain any weird upper ASCII characters
int verifyPrintable(char* message, const char* messageFilename, int lineCount)
{
    U8* str = message;
    if (isProductionMode()) return 1; // don't do in depth searching for production servers
    while (*str)
    {
        U8 c = *str;
        
        if (c <= 127)
        {
            if ((c < 'a' || c > 'z')
                &&
                (c < 'A' || c > 'Z')
                &&
                (c < '0' || c > '9')
                &&
                !strchr("!@#$%^&*()-_=+[]{|};:',<.>/?\" ~\r\n\t", c))
            {
                ErrorFilenamef(messageFilename, "Bad character '%c' (%i) in \"%s\", line %i in file %s\n", c, str-message+1, message, lineCount, messageFilename);
                return 0;
            }
        }
        str = UTF8GetNextCharacter(str);
    }
    return 1;
}


static TextMessage *msAddMessage(MessageStore *store, const char *messageID, const char *messageText, const char *helpText)
{
    TextMessage* textMessage;
    bool bSuccess;

    assert(!store->bLocked); // this message store is locked, perhaps because it was put in shared memory. you are adding to this too late

    // Create a text message to hold text message related stuff.
    textMessage = mpAlloc(store->textMessagePool);

    // Add this message to the messageID hash table so we can look this message up by ID later.
    if(store->cmdstore)
    {
        stashAddPointer( store->messageStash, messageText, textMessage, false);
    }
    if (!stashAddPointer(store->messageIDStash, messageID, textMessage, false))
        return NULL;

    // Store a pointer back to the messageID in the text message structure so we know what the
    // ID is when debugging.
    bSuccess = stashGetKey(store->messageIDStash, messageID, &textMessage->messageID);
    assert(bSuccess); // we just added it, it better be there

    // Convert the message into a wide character format, place it in the message store's message
    // string table, then store the message itself into the text message structure.
    //textMessage->message = strTableAddString(store->messages, (void*)UTF8ToWC(message));
    textMessage->messageIndex = strTableAddStringGetIndex(store->messages, messageText);
    if (helpText)
        textMessage->helpIndex = strTableAddStringGetIndex(store->messages, helpText);

    return textMessage;
}

static MessageStoreFormatHandler getFormatHandler(MessageStore* store, char character)
{
    U32 isUpper = character >= 'A' && character <= 'Z';
    U32 index = character - (isUpper ? 'A' : 'a');
    
    if(store && index <= 'Z' - 'A'){
        return store->formatHandler[index][isUpper];
    }
    
    return NULL;
}

// Parses ContactDef/Entity/DB_ID based info and everything that has attribs or conditionals
// returns 1 if parsing succeeded, 0 if it failed
// Note: srcstr is not conserved
// if outputBuffer is NULL, just parses the srcstr for correctness
static int ParseAttrCond(    MessageStore*        store,
                            char*                outputBuffer,
                            int                    bufferLength,
                            char*                srcstr,
                            void*                param,
                            char                paramType,
                            char*                 wholemsg,
                            ScriptVarsTable*    vars, 
                            const char*         filename,
                            int                    linecount)
{
    MessageStoreFormatHandler        formatHandler;
    S32                                foundAttr=0;
    U8                                userDataBuffer[1000];
    MessageStoreFormatHandlerParams    params = {0};

    char *attribstr = 0, *valueToCompareTo, *outputIfTrue, *outputIfFalse;
    char *parse=srcstr, *temp;
    const char* tempCharStore;        // temporary result storage for error checking - Voodoo                                    
    
    if(outputIfFalse = strchr(srcstr, '|'))    // be at least somewhat flexible with spaces around the | else marker
    {
        temp = outputIfFalse;
        do {
            *temp = '\0';
        } while(*(--temp) == ' ');
        do {
            outputIfFalse++;
        } while(*outputIfFalse == ' ');
    }
    if(valueToCompareTo = strchr(srcstr, '='))
    {
        temp = valueToCompareTo;
        do{ *temp = '\0'; }while(*(--temp) == ' ');
        do{ valueToCompareTo++; }while(*valueToCompareTo == ' ');
        parse = valueToCompareTo;
    }
    if(outputIfTrue = strchr(parse, ' '))
    {
        *outputIfTrue++ = '\0';
        parse = outputIfTrue;
    }
    if((paramType != 's') && (attribstr = strchr(srcstr, '.')))    //check for attributes
    {
        *attribstr++ = '\0';
        parse = attribstr;
    }

    if(!outputBuffer && filename)
    {
        if(valueToCompareTo && !outputIfTrue)
        {
            ErrorFilenamef(filename, "Found conditional but no clause to print based on the outcome, file %s, message %s, line %i", filename, wholemsg, linecount);
            if(outputBuffer)
                outputBuffer[0] = '\0';
            return 0;
        }

        if(outputIfFalse && !(outputIfTrue && valueToCompareTo))
        {
            ErrorFilenamef(filename, "Found | but no text if conditional is true, or no conditional test, file %s, message %s, line %i", filename, wholemsg, linecount);
            if(outputBuffer)
                outputBuffer[0] = '\0';
            return 0;
        }

        if(attribstr && strchr(attribstr, ' '))
        {
            ErrorFilenamef(filename, "Found an illegal space, file %s, message %s, line %i", filename, wholemsg, linecount);
            if(outputBuffer)
                outputBuffer[0] = '\0';
            return 0;
        }
    }
    
    formatHandler = getFormatHandler(store, paramType);
    
    if(formatHandler){        
        params.fileName = filename;
        params.lineCount = linecount;
        
        params.userData = userDataBuffer;
        params.param = param;
        
        params.srcstr = srcstr;
        params.wholemsg = wholemsg;
        params.attribstr = attribstr;
        params.outputBuffer = outputBuffer;
        params.bufferLength = bufferLength;
        params.vars = vars;
        
        formatHandler(&params);

        if (params.done) // has this function take care of everything?
            return 1;

    } else {
        switch(paramType)
        {
            xcase 's':
                if(outputBuffer)
                    strcpy_s(outputBuffer, bufferLength, (char*)param);
            xdefault:
                if(filename)
                    ErrorFilenamef(filename, "Unknown paramType in ParseAttrCond while parsing %s, file %s, message %s, line %i", srcstr, filename, wholemsg, linecount);
                else
                    Errorf("Unknown paramType in ParseAttrCond while parsing %s", srcstr);
                if(outputBuffer)
                    outputBuffer[0] = '\0';
                return 0;
        }
    }

    if(attribstr)
    {
        foundAttr = StaticDefineIntGetInt(store->attributes, attribstr);
        
        if(foundAttr >= 0){
            if(outputBuffer){
                S32 value = *(S32*)(userDataBuffer + store->valueOffsetInUserData[foundAttr]);
                tempCharStore = StaticDefineIntRevLookup(store->attribToValue[foundAttr], value);
                
                if (tempCharStore !=NULL) 
                    strcpy_s(outputBuffer, bufferLength, tempCharStore);
            }
        }else{
            if(filename)
                ErrorFilenamef(filename, "Unsupported attribute value %s, file %s, message %s, line %i", attribstr, filename, wholemsg, linecount);
            else
                Errorf("Unsupported attribute value %s", attribstr);
            if(outputBuffer)
                outputBuffer[0] = '\0';
            return 0;
        }
    }

    // TODO: conditionals for %s etc
    if(valueToCompareTo)
    {
        if(attribstr && !StaticDefineIntLookup(store->attribToValue[foundAttr], valueToCompareTo))
        {
            if(filename)
                ErrorFilenamef(filename, "Unsupported conditional value %s, file %s, message %s, line %i", valueToCompareTo, filename, wholemsg, linecount);
            else
                Errorf("Unsupported conditional value %s", valueToCompareTo);
            if(outputBuffer)
                outputBuffer[0] = '\0';
            return 0;
        }
        if(!outputIfTrue)
        {
            if(filename)
                ErrorFilenamef(filename, "Found conditional but no rest; String: %s, Conditional: %s, file %s, message %s, line %i", srcstr, valueToCompareTo, filename, wholemsg, linecount);
            else
                Errorf("Found conditional but no rest; String: %s");
            if(outputBuffer)
                outputBuffer[0] = '\0';
            return 0;
        }

        if(outputBuffer){
            const char* output = "";
            
            if(!stricmp(outputBuffer, valueToCompareTo)){
                output = outputIfTrue;
            }
            else if(outputIfFalse){
                output = outputIfFalse;
            }
            
            strcpy_s(outputBuffer, bufferLength, output);
        }
    }

    if(outputBuffer){
        for(parse = outputBuffer + strlen(outputBuffer) - 1;
            parse >= outputBuffer && *parse == ' ';
            parse--);
        
        if(parse[1] == ' '){
            parse[1] = 0;
        }
    }

    return 1;
}


//TODO: clean up this mess
static int loadMessageData(    MessageStore* store, MessageStoreLoadFlags flags,
                            char *messageFileContents, const char *messageFilename )
{
    int lineCount = 1;
    const char *pchCur;
    char *cur, *varstart, *varend;
    bool verifyCharacters=!store->localeID && (!messageFilename || !strstriConst(messageFilename, "texture")); // Some textures have spanish text, etc

    char *remain;
    int len;
    static char *buf = 0;

    pchCur = messageFileContents;
    if(!messageFileContents)
        return 0;

    if (!buf)
        estrCreate(&buf);    // buffer for validity checking

    store->fileLoadedFrom = strdup(messageFilename);
    
    // Are we really reading an UTF8 file?
    // If not, do not initialize the message store.
    if(0 != memcmp(pchCur, UTF8BOM, 3))
    {
        // it's ok not to have UTF8 files...
//        ErrorFilenamef(messageFilename, "Unable to read file \"%s\" because it is not a UTF8 file!\n", messageFilename);
//        return 0;
    } else {
        // Read and remove the UTF8 byte order marker.
        pchCur += 3;
    }

    //
    // Message files are made up of MessageID-Message pairs.
    // The MessageID appears first on the line and must start on the
    //   first column of the line. It must have double-quotes (") around
    //   it and cannot contain double-quotes.
    // The Message follows the MessageID and can either be double-quoted
    //   or quoted with << >>.
    // If double-quoted, the entire message must be on the same line; it
    //   cannot contain \r or \n. It may contain "s. The last double-quote
    //   found on the line is used to delimit  the string.
    // Messages quoted with << and >> may span multiple lines and may
    //   also contain "s.
    //
    // Blank lines and lines starting with // and # are ignored.
    //
    // Example (ignore the comment prefix :-)
    //
    // # Message file
    // "str1" "regular"
    // "str2" "has "embedded" double-quotes(")"
    // "str3" <<Uses special "s on one line>>
    // # I am a comment!
    // "str4" <<Uses
    //   multiple
    //   lines just because it's fun>>
    // "str5" "<<has the left and right chevrons in the messages>>"
    // # End of message file
    //

    while(*pchCur != 0)
    {
        static StuffBuff message;
        static StuffBuff messageID;
        static StuffBuff helpMessage;
        char *pchNextLine = NULL;
        char *pchEOL;

        // Init the message and messageID scratchpads.
        if(!message.buff)
        {
            initStuffBuff(&message, 512);
            initStuffBuff(&messageID, 512);
            initStuffBuff(&helpMessage, 512);
        }
        else
        {
            clearStuffBuff(&message);
            clearStuffBuff(&messageID);
            clearStuffBuff(&helpMessage);
        }

        // Look for the end of this line. EOLs must contain an \r. \n
        // is optional.
        pchEOL = strchr(pchCur, '\r');
        if(pchEOL)
        {
            pchNextLine = pchEOL+1;
            while(*pchNextLine=='\r' || *pchNextLine=='\n')
            {
                pchNextLine++;
                if(*pchNextLine=='\r')
                    lineCount++;
            }
        }
        else
        {
            pchEOL = pchNextLine = (char*)pchCur+strlen((char*)pchCur);
        }

        if(pchCur[0]=='/' && pchCur[1]=='/')
        {
            // Skip comment lines
            goto nextline;
        }
        else if(pchCur[0]=='#')
        {
            // Skip comment lines
            goto nextline;
        }
        else if(pchCur[0]=='\r')
        {
            // Blank line
            goto nextline;
        }
        else if(pchCur[0]!='"')
        {
            if (!hideTranslationErrors)
                ErrorFilenamef(messageFilename, "Unable to find MessageID on line %d in file %s\n", lineCount, messageFilename);
            goto nextline;
        }
        else
        {
            // OK, We got a ", get the MessageID
            char *pchEnd;

            // Skip to the next "
            pchCur++;
            pchEnd = strchr(pchCur, '"');
            
            if(!pchEnd || pchEnd>pchEOL)
            {
                if (!hideTranslationErrors)
                    ErrorFilenamef(messageFilename, "Unable to find MessageID terminator on line %d in file %s\n", lineCount, messageFilename);
                goto nextline;
            }

            // Got the messageID.
            addBinaryDataToStuffBuff(&messageID, pchCur, pchEnd-pchCur);
            addBinaryDataToStuffBuff(&messageID, "\0", 1);

            // Skip forward to the next " or < : start of message
            pchCur = pchEnd+1;
            while(pchCur[0]!='\"' && (pchCur[0]!='<' || pchCur[1]!='<') && pchCur<pchEOL)
                pchCur++;

            if( store->cmdstore ) // we are loading commands, so now grab the key for that
            {
                if(pchCur[0]=='\"')
                {
                    pchCur++;
                    pchEnd = strchr(pchCur, '"');
                    if(!pchEnd || pchEnd>pchEOL)
                    {
                        if (!hideTranslationErrors)
                            ErrorFilenamef(messageFilename, "Unable to find Message terminator on line %d in file %s\n", lineCount, messageFilename);
                        goto nextline;
                    }

                    // Got the command.
                    addBinaryDataToStuffBuff(&message, pchCur, pchEnd-pchCur);
                    addBinaryDataToStuffBuff(&message, "\0", 1);

                    // Skip forward to the next " or <
                    pchCur = pchEnd+1;
                    while(pchCur[-1]!='\\' && pchCur[0]!='\"' && (pchCur[0]!='<' || pchCur[1]!='<') && pchCur<pchEOL)
                        pchCur++;
                }
            }

            if(pchCur[0]=='\"')
            {
                // Scan to the end of the line and truncate the string
                // after the last "
                char *pchLastQuote = NULL;
                int iCntExtra=0;
                
                pchCur++;
                pchEnd = (char*)pchCur;
                while(pchEnd<pchEOL)
                {
                    if(pchEnd[-1]!='\\' && pchEnd[0]=='\"')
                    {
                        pchLastQuote = pchEnd;
                        iCntExtra = 0;
                    }
                    else if(*pchEnd!=' ' && *pchEnd!='\t' && *pchEnd!='\r' && *pchEnd!='\n')
                    {
                        iCntExtra++;
                    }
                    pchEnd++;
                }
                
                if(pchLastQuote==NULL)
                {
                    if (!hideTranslationErrors)
                        ErrorFilenamef(messageFilename, "No terminator for MessageID: \"%s\" found on line %i in file %s\n", messageID.buff, lineCount, messageFilename);
                    goto nextline;
                }
                else if(iCntExtra>0)
                {
                    if (!hideTranslationErrors)
                        ErrorFilenamef(messageFilename, "Warning: Extra characters after message for MessageID: \"%s\" found on line %i in file %s\n", messageID.buff, lineCount, messageFilename);
                }
                
                // clear out any escaped chars (allowed to modify this buffer?)
                {
                    char *tmp = pchEnd;
                    for(tmp = strchr((char*)pchCur,'\\'); tmp && tmp < pchEnd; tmp = strchr(tmp+1,'\\'))
                    {
                        switch ( tmp[1] )
                        {
                        case '"':
                        case '\r':
                        case '\n':
                            // get rid of preceding slash
                            memmove(tmp,tmp+1,pchEnd - tmp);
                            pchEnd--;
                            break;
                        default:
                            break;
                        };
                    }    
                }
                // Got the message
                if(store->cmdstore)
                {
                    addBinaryDataToStuffBuff(&helpMessage, pchCur, pchLastQuote-pchCur);
                    addBinaryDataToStuffBuff(&helpMessage, "\0", 1);
                }
                else
                {
                    addBinaryDataToStuffBuff(&message, pchCur, pchLastQuote-pchCur);
                    addBinaryDataToStuffBuff(&message, "\0", 1);
                }
            }
            else if(pchCur[0]=='<' && pchCur[1]=='<')
            {
                pchCur+=2;
                
                // Scan forward for the >>
                // Strings within << >> delimiters can cross line boundaries.
                pchEnd = strchr(pchCur, '>');
                while(pchEnd && pchEnd[1]!='>')
                {
                    pchEnd = strchr(pchEnd+1, '>');
                }
                if(!pchEnd)
                {
                    ErrorFilenamef(messageFilename, "Unable to find >> terminator for MessageID \"%s\" on line %d in file %s\n", messageID.buff, lineCount, messageFilename);
                    goto nextline;
                }
                
                // Got the message
                if( store->cmdstore )
                {
                    addBinaryDataToStuffBuff(&helpMessage, pchCur, pchEnd-pchCur);
                    addBinaryDataToStuffBuff(&helpMessage, "\0", 1);
                }
                else
                {
                    addBinaryDataToStuffBuff(&message, pchCur, pchEnd-pchCur);
                    addBinaryDataToStuffBuff(&message, "\0", 1);
                }
                
                // Fix up the line count
                while(pchCur<pchEnd)
                {
                    if(*pchCur=='\r')
                        lineCount++;
                    pchCur++;
                }
                
                // Skip to the end of the line
                pchNextLine = NULL;
                pchEOL = strchr(pchCur, '\r');
                if(pchEOL)
                {
                    pchNextLine = pchEOL;
                    do {
                        pchNextLine++;
                    } while(*pchNextLine=='\r' || *pchNextLine=='\n');
                }
            }
            else
            {
                if (!hideTranslationErrors)
                    ErrorFilenamef(messageFilename, "Unable to find message for MessageID \"%s\" on line %d in file %s\n", messageID.buff, lineCount, messageFilename);
                goto nextline;
            }
            
            // If we get here, we have a messageID and a message.
            
            // Check if braces match
            for(cur = message.buff; 1; cur = varend + 1)
            {
                varstart = strchr(cur, '{');
                varend = strchr(cur, '}');
                
                if(!varstart && !varend)
                    break;
                
                if((varstart && !varend) || (!varstart && varend) || (varstart > varend) || ((varstart = strchr(varstart+1, '{')) && varstart < varend))
                {
                    if (!hideTranslationErrors)
                        ErrorFilenamef(messageFilename, "Found a {} mismatch in messageID %s in file %s, line %i", messageID.buff, messageFilename, lineCount);
                    goto nextline;
                }
            }
            
            // Check for valid attributes/conditionals
            for(remain = message.buff; varstart = strchr(remain, '{'); remain = varend + 1)
            {
                varend = strchr(varstart, '}');
                len = varend - varstart - 1;
                estrClear(&buf);
                estrConcatFixedWidth(&buf, varstart + 1, len);
                if(!ParseAttrCond(store, NULL, 0, buf, buf, 's', messageID.buff, NULL, messageFilename, lineCount))
                    ErrorFilenamef(messageFilename, "The previous error occurred in messageID %s in file %s, line %i", messageID.buff, messageFilename, lineCount);
            }
            
            // Add the message to the list.
            {
                const TextMessage* textMessage;
                
                // Make sure the message ID does not already exist.
                if(stashFindPointerConst(store->messageIDStash, messageID.buff, &textMessage))
                {
                    if(0 != stricmp(strTableGetConstString(store->messages, textMessage->messageIndex), message.buff))
                    {

                        // If the message ID already exists, print out an error message and ignore the new message.
                        if(!hideTranslationErrors && !(flags & MSLOAD_FORCEBINS))
                        {
                            ErrorFilenamef(messageFilename, "Duplicate MessageID: \"%s\" found on line %i in file %s\n", messageID.buff, lineCount, messageFilename);
                            LOG(LOG_DUPLICATE_MESSAGEIDS, LOG_LEVEL_IMPORTANT, 0, "Duplicate MessageID: \"%s\" found on line %i in file %s\n", messageID.buff, lineCount, messageFilename);
                        }
                    }
                    goto nextline;
                }
                
                // make sure string doesn't include unprintable ASCII characters
//                if (!verifyPrintable(message.buff, messageFilename, lineCount))
//                    goto nextline;
                if(verifyCharacters)    // using the default (english) locale so check for weird chars
                    verifyPrintable(message.buff, messageFilename, lineCount);
                
                msAddMessage(store, messageID.buff, message.buff, helpMessage.buff);
            }
        }
        
    nextline:
        pchCur = pchNextLine;
        lineCount++;
    }
    return 1;
}

static void addTextMessageDefNameIndex(TextMessage* textMessage, int index)
{
    if(textMessage)
    {
        if(!textMessage->variableDefNameIndices)
        {
            textMessage->variableDefNameIndices = createArray();
        }
        
        arrayPushBack(textMessage->variableDefNameIndices, (void*)(intptr_t)index);
    }
}

static int loadMessageTypeData(    MessageStore* store, MessageStoreLoadFlags flags,
                                char* messageTypeData, const char *messageTypeFilename )
{
    char lineBuffer[1024];
    char* parseCursor;
    int lineCount = 0;
    intptr_t len;
    char* messageID;
    char* variableName;
    char* variableType;
    char* curr;
    TextMessage* textMessage, *v_textMessage, *p_textMessage, *l_textMessage;
    char * end;
    
    if(!messageTypeData)
        return 0;

    end=messageTypeData+strlen(messageTypeData);
    for(curr = messageTypeData;(curr<end) && (len = strcspn(curr,"\n"));curr += len+1)
    {
        char v_messageID[1024];
        char p_messageID[1024];
        char l_messageID[1024];

        assert(len < ARRAY_SIZE(lineBuffer));
        memcpy(lineBuffer, curr, len);
        lineBuffer[len] = 0;
        if (lineBuffer[len-1] == '\r')
            lineBuffer[len-1] = 0;
        lineCount++;
        parseCursor = lineBuffer;
        
        if(lineBuffer[0] == '#') // at least TRY to get rid of commented out strings
            continue;
        
        // Assume that every line always starts with a quoted string.
        // Extract the quoted string.  This is the message ID.
        messageID = readQuotedString(&parseCursor);
        if(!messageID)
            continue;

        STR_COMBINE_SS(v_messageID, "v_", messageID);
        STR_COMBINE_SS(p_messageID, "p_", messageID);
        STR_COMBINE_SS(l_messageID, "l_", messageID);

        if (!stashFindPointer(store->messageIDStash, messageID, &textMessage))
            textMessage = NULL;
        if (!stashFindPointer(store->messageIDStash, v_messageID, &v_textMessage))
            v_textMessage = NULL;
        if (!stashFindPointer(store->messageIDStash, p_messageID, &p_textMessage))
            p_textMessage = NULL;
        if (!stashFindPointer(store->messageIDStash, l_messageID, &l_textMessage))
            l_textMessage = NULL;

        if( textMessage || v_textMessage || l_textMessage || p_textMessage )// Make sure the message ID exists.
        {
            // For each pair of strings enclosed in { and }...
            while(variableType = readEnclosedString(&parseCursor, '{', '}'))
            {
                int iVariableNameIndex, iVariableTypeIndex;
                
                // Get the variable name and the expected type.
                variableName = strsep(&variableType, ",");
                
                eatWhitespace(&variableName);
                eatWhitespace(&variableType);
                
                iVariableNameIndex = strTableAddStringGetIndex(store->variableStringTable, variableName);
                iVariableTypeIndex = strTableAddStringGetIndex(store->variableStringTable, variableType);

                // TODO: it may be better to set NoRedundance and keep both idxs
                assert( iVariableTypeIndex == iVariableNameIndex + 1 );

                if(v_textMessage && !v_textMessage->variableDefNameIndices){
                    v_textMessage->variableDefNameIndices = createArray();
                }
                
                if(p_textMessage && !p_textMessage->variableDefNameIndices){
                    p_textMessage->variableDefNameIndices = createArray();
                }

                if(l_textMessage && !l_textMessage->variableDefNameIndices){
                    l_textMessage->variableDefNameIndices = createArray();
                }

                addTextMessageDefNameIndex(textMessage, iVariableNameIndex);
                addTextMessageDefNameIndex(v_textMessage, iVariableNameIndex);
                addTextMessageDefNameIndex(p_textMessage, iVariableNameIndex);
                addTextMessageDefNameIndex(l_textMessage, iVariableNameIndex);
            }
        }
        else if( !textMessage )
        {
            // If the message ID does not exist, print out an error message and proceed to the next message.
            if(!hideTranslationErrors && !(flags & MSLOAD_FORCEBINS))
                ErrorFilenamef(messageTypeFilename, "Unknown MessageID: \"%s\" found on line %i in file %s\n", messageID, lineCount, messageTypeFilename);
        }
    }
    return 1;
}

static int msAddMessages(    MessageStore* store, MessageStoreLoadFlags flags,
                            const char* messageFilename, const char* messageTypeFilename )
{
    char    *messageData,*messageTypes;
    int        ret;

    messageData = fileAlloc(messageFilename, 0);
    ret = loadMessageData(store, flags, messageData, messageFilename);
    free(messageData);
    if (!ret)
        return 0;
    
    messageTypes = fileAlloc(messageTypeFilename, 0);
    ret = loadMessageTypeData(store, flags, messageTypes, messageTypeFilename);
    free(messageTypes);
    return ret;
}

static int msAddMessagesFromMem(MessageStore* store, MessageStoreLoadFlags flags,
                                char *messageData, char *messageTypes )
{
    return    loadMessageData(store, flags, messageData, "read from mem") &&
            loadMessageTypeData(store, flags, messageTypes, "read from mem");
}

/* Function msPrintf()
 *    Parameters:
 *        store - an initialized message store where the specified message can be found.
 *        outputBuffer - a valid string buffer where the produced string will be printed.
 *        messageID - a UTF-8 string that will identify a single string in the given store.
 *        ... - parameters required by printf call.
 *
 *    Returns:
 *        -1 - The message cannot be printed because the messageID is unknown to the given store.
 *        valid number - the number of characters printed into the output buffer.
 *        or
 *        valid number -    the number of bytes required to store the result, excluding the NULL terminator
 *                        if the outputBuffer is NULL.
 */
int msPrintf(MessageStore* store, char* outputBuffer, int bufferLength, const char* messageID, ...){
    int result;
    
    VA_START(arg, messageID);
    result = msvaPrintfInternal(store, outputBuffer, bufferLength, messageID, NULL, NULL, arg);
    VA_END();
    
    return result;
}

int msPrintfVars(MessageStore* store, char* outputBuffer, int bufferLength, const char* messageID, ScriptVarsTable* vars, int recur_depth, int flags, ...){
    int result;
    
    VA_START(arg, flags);
    result = msvaPrintfInternalEx(store, outputBuffer, bufferLength, messageID, NULL, vars, recur_depth, flags, arg);
    VA_END();
    
    return result;
}

// va versions
int msvaPrintfEx(int flags, MessageStore* store, char* outputBuffer, int bufferLength, const char* messageID, va_list arg){
    return msvaPrintfInternalEx(store, outputBuffer, bufferLength, messageID, NULL, NULL, 0, flags, arg);
}

int msvaPrintf(MessageStore* store, char* outputBuffer, int bufferLength, const char* messageID, va_list arg){
    return msvaPrintfEx(0, store, outputBuffer, bufferLength, messageID, arg);
}

static int vsprintfInternal(char* outputBuffer, int bufferLength, const char* format, va_list arg)
{
    static char buf[16384] = {0};
    
    if(!outputBuffer)
    {
        return _vsnprintf_s(buf, sizeof(buf), sizeof(buf), format, arg);
    }
    else
    {
        return vsprintf_s(outputBuffer, bufferLength, format, arg);
    }
}

// get stored string from message store
const char* msGetUnformattedMessageConst(MessageStore* store, const char* messageID)
{
    const TextMessage* textMessage;

    if(!store)
    {
        msPrintfError = 1;
        return messageID;
    }

    // Look up the message to be printed.
    if (!stashFindPointerConst(store->messageIDStash, messageID, &textMessage))
        textMessage = NULL;

    // If the message cannot be found, print the messageID itself.
    if(!textMessage){
        msPrintfError = 1;
        return messageID;
    }

    // found it
    return strTableGetConstString(store->messages, textMessage->messageIndex);
}

// get string from message store without any formatting
char* msGetUnformattedMessage(    MessageStore* store,
                                char* outputBuffer,
                                int bufferLength,
                                char* messageID)
{
    const char *str = msGetUnformattedMessageConst(store,messageID);
    strcpy_s(outputBuffer,bufferLength,str);
    return outputBuffer;
}

// put a temp pointer in with each type definition
static void msGetTypeDefPointers(Array* messageTypeDef, va_list arg)
{
    #define va_advance(ap, byteCount) (ap += byteCount)
    int i;

    if (!messageTypeDef) return; // ok

    for(i = 0; i < messageTypeDef->size; i++)
    {
        NamedVariableDef* def;
        int variableTypeSize = sizeof(int);
        def = messageTypeDef->storage[i];

        // Different variable types occupy different amount of stack space.
        // How much space does this kind of variable take up?
        
        switch(def->variableTypeChar){
            // Doubles are 8 bytes long.
            case 'f':
            case 'g':
            case 'G':
                variableTypeSize = sizeof(double);
                break;
        }
        def->variablePointer = arg;
        va_advance(arg, variableTypeSize);
    }

    #undef va_advance
}

static char getFormatChar(const char* format){
    for(; *format; format++){
        if(    *format >= 'a' && *format <= 'z' ||
            *format >= 'A' && *format <= 'Z')
        {
            return *format;
        }
    }
}

static void getAttribSeparatorAndCompareValue(    char* param,
                                                char** attribSeparatorPtr,
                                                char** equalSignPtr)
{
    char* attribSeparator = 0;
    char* equalSign = 0;

    if(attribSeparator = strchr(param, '.'))
    {
        param = attribSeparator + 1;
    }
    if(equalSign = strchr(param, '='))
    {
        param = equalSign;
        do{ *param = '\0'; }while(*--param == ' ');
    }
    
    *attribSeparatorPtr = attribSeparator;
    *equalSignPtr = equalSign;
}


#define MAX_PARAM_RECURSION 5
// replace a particular {Param} found in string
static void msPrintParam(    const char* messageID,
                            char* outputBuffer,
                            int bufferLength,
                            char* param,
                            MessageStore* store,
                            Array* messageTypeDef,
                            ScriptVarsTable* vars,
                            int recur_depth,
                            int flags,
                            va_list arg)
{
    int i;
    char* attribSeparator;
    char* equalSign;
    
    getAttribSeparatorAndCompareValue(param, &attribSeparator, &equalSign);
    
    // look for the param in the message types first
    if (messageTypeDef)
    {
        for(i = 0; i < messageTypeDef->size; i++)
        {
            NamedVariableDef*            def = messageTypeDef->storage[i];
            MessageStoreFormatHandler    formatHandler = getFormatHandler(store, def->variableTypeChar);
            const char*                    type;
            char*                        varPointerStr;
            char**                        varPointer = (char**)def->variablePointer;

            if(attribSeparator)
            {
                if(formatHandler)
                {
                    *attribSeparator = '\0';
                }
                else
                {
                    *attribSeparator = '.';
                }
            }
            
            // Not a match
            if (stricmp(def->variableName, param))
            {
                continue;
            }

            // For these cases the pointer does not point directly to a string
            // but is a double pointer. In these cases we want to preserve the
            // pointer from being modified so we make a local copy of it.
            if (formatHandler || def->variableTypeChar == 'r')
            {
                varPointerStr = *varPointer;
                varPointer = &varPointerStr;
            }

            type = def->variableType;

            // reference parameters need another lookup through the message store
            if(def->variableTypeChar == 'r')
            {
                char*    keyString = *varPointer;
                int        validString = 0;
                
                assert(!formatHandler);
                
                // MS: Exception wrapper to prevent a bad string parameter from causing an access violation.
                
                if(keyString)
                {
                    __try{
                        validString = !IsBadStringPtrA(keyString, 1*1024*1024); // scan up to 1MB of memory
                    }__except(validString = 0, EXCEPTION_EXECUTE_HANDLER){}
                }
                
                if(validString)
                {
                    const TextMessage* referencedMessage=0;

                    // try the villains string first
                    if( store->idPrepend )
                    {
                        char villainsMessageID[16 * 1024];

                        Strncpyt( villainsMessageID, store->idPrepend );
                        Strncatt( villainsMessageID, keyString );
                        if (!stashFindPointerConst(store->messageIDStash, villainsMessageID, &referencedMessage))
                            referencedMessage = NULL;
                    }

                    if(!referencedMessage)
                    {
                        if( !stashFindPointerConst(store->messageIDStash, keyString, &referencedMessage))
                            referencedMessage = NULL;
                    }

                    if(referencedMessage)
                    {
                        *(const char**)varPointer = strTableGetConstString(store->messages, referencedMessage->messageIndex);
                    }
                }
                else
                {
                    *(char**)varPointer = "Invalid String Parameter";
                    
                    Errorf("Bad parameter for: %s:%s", messageID, def->variableName);
                }
                
                type = "%s";
            }

            if(attribSeparator)
                *attribSeparator = '.';    // put the period back to let ParseAttrCond() parse the string itself
            if(equalSign)
                *equalSign = '=';

            if(formatHandler)
            {
                ParseAttrCond(store, outputBuffer, bufferLength, param, *(void**)varPointer, def->variableTypeChar, NULL, vars, NULL, 0);
            }
            else
            {
                vsprintf_s(outputBuffer, bufferLength, type, (void*)varPointer);
                if(equalSign)
                    ParseAttrCond(store, outputBuffer, bufferLength, outputBuffer, outputBuffer, 's', NULL, vars, NULL, 0);
            }

            return;
            // complete
        }
    }

    // otherwise, try a variable lookup through the vars table
    if (vars)
    {
        if(recur_depth < MAX_PARAM_RECURSION)
        {
            const char* replace;
            char vartype;

            replace = ScriptVarsTableLookupTyped(vars, param, &vartype);

            if(attribSeparator && replace == param)
            {
                *attribSeparator = '\0';
                replace = ScriptVarsTableLookupTyped(vars, param, &vartype);
            }

            if (replace != param) // script vars need a recursive call through msPrintf for further replacement
            {
                if(equalSign)
                    *equalSign = '=';
                if(attribSeparator)
                    *attribSeparator = '.';

                switch(vartype)
                {
                    xcase 's':
                        msPrintfVars(store, outputBuffer, bufferLength, replace, vars, recur_depth + 1, flags);
                        msPrintfError = 0;
                        if(equalSign)
                            ParseAttrCond(store, outputBuffer, bufferLength, outputBuffer, outputBuffer, 's', NULL, vars, NULL, 0);

                    xdefault:
                        ParseAttrCond(store, outputBuffer, bufferLength, param, (void*)replace, vartype, NULL, vars, NULL, 0);
                }
                return;
            }
        }
        else
        {
            LOG(LOG_ERROR, LOG_LEVEL_IMPORTANT, LOG_CONSOLE_ALWAYS, "msPrintParam exceeded recursion depth");
        }
    }

    // just print bad param if we failed
    strcpy_s(outputBuffer, bufferLength, param);
}

int msvaPrintfInternal(MessageStore* store, char* outputBuffer, int bufferLength, const char* messageID, Array* messageTypeDef, ScriptVarsTable* vars, va_list arg)
{
    return msvaPrintfInternalEx(store, outputBuffer, bufferLength, messageID, messageTypeDef, vars, 0, 0, arg);
}

// ------------------------------------------------------------
// Special Korean handling

// http://code.cside.com/3rdpage/us/unicode/converter.html - used to convert

// Hangul syllables
#define HANGUL_PRECOMP_BASE      0xAC00
#define HANGUL_PRECOMP_MAX       0xD7A3
#define NUM_JONGSEONG            28  // (T) Trailing Consonants
 
// return true if the given char is a precomposed Hangul syllable
bool CharIsHangulSyllable(wchar_t wch) 
{
    return (wch >= HANGUL_PRECOMP_BASE && wch <= HANGUL_PRECOMP_MAX) ? true : false;
}

// prev char is previous char of this fixup
// if (CharIsHangulSyllable(prevChar)) {
//     koreanVowelSound = !((prevChar - HANGUL_PRECOMP_BASE) % NUM_JONGSEONG);
//     buffer->Add(fixup[koreanVowelSound ? 1 : 0]);
// }

// Special Korean handling
// ------------------------------------------------------------

static bool s_CharIsPostPositionable(wchar_t prevChar)
{
    return CharIsHangulSyllable( prevChar ) || 
        (prevChar <= '9' && prevChar >= '0') ||
        (prevChar <= 'z' && prevChar >= 'A'); 
}

static bool s_ChoosePostposition(wchar_t prevChar,int *pIsKoreanVowelSound, char *postposVars)
{    
    bool res = false;
    
    if( s_CharIsPostPositionable( prevChar ) )
    {
        int koreanVowelSound = !((prevChar - HANGUL_PRECOMP_BASE) % NUM_JONGSEONG);
//         char *copyPos = variableName;
//         int varlength = UTF8GetLength(variableName);
        
        if (prevChar <= '9' && prevChar >= '0') {
            koreanVowelSound = (prevChar == '2' || prevChar == '4' || prevChar == '5' || prevChar == '9');
        } else if (prevChar <= 'z' && prevChar >= 'A') {
            koreanVowelSound = (prevChar == 'a' || prevChar == 'e' || prevChar == 'i' || prevChar == 'o' ||
                                prevChar == 'u' || prevChar == 'y' || prevChar == 'A' || prevChar == 'E' ||
                                prevChar == 'I' || prevChar == 'O' || prevChar == 'U' || prevChar == 'Y');
        }
        
        // --------------------
        // special case: see if previous character has 'f' in it
        if( !koreanVowelSound )
        {
            wchar_t special[] =  
                {
                    0xc73c, 0xb85c, 0xb85c, 0
                };
            wchar_t var[ARRAY_SIZE(special)];

            UTF8ToWideStrConvert(postposVars, var, ARRAY_SIZE( var ));

            
            if( 0 == wcsncmp(special, var, ARRAY_SIZE( special )) )
            {
                // ( ( ( alpha * 0x15 ) + beta ) * 0x1C ) + (gamma + 0x1) + 0xAC00
                // To extract the consonants or vowels, the following formulas can be used as fundamental,
                // Final Consonant: 0x11A8 + ( ( The value of a hangul syllable - 0xAC00 ) % 0x1C - 1)
                // Vowel: 0x1161 + int ( ( ( ( The value of hangul syllable - 0xAC00 ) / 0x1C ) ) % 0x15 )
                // First Consonant:  0x1100 + int ( ( ( ( The value of hangul syllable - 0xAC00 ) / 0x1C ) ) / 0x15 )
                wchar_t finalConsonantOffset = (( prevChar - 0xAC00 ) % 0x1C - 1);
//                wchar_t finalConsonant = 0x11A8 + finalConsonantOffset;
//         wchar_t vowel = 0x1161 + (((( prevChar - 0xAC00 ) / 0x1C ) ) % 0x15 );
//         wchar_t firstConsonant = 0x1100 + int (((( prevChar - 0xAC00 ) / 0x1C ) ) / 0x15 );
                
                if( finalConsonantOffset == 7 )
                {
                    koreanVowelSound = true;
                }
            }
        }
        

        // ----------
        // if we get here, use postpos 

        if( pIsKoreanVowelSound )
        {
            *pIsKoreanVowelSound = koreanVowelSound;
        }
        res = true;
    }

    // ----------
    // finally

    return res;
}

static void initVariableNameAndType(NamedVariableDef* def,
                                    const char* variableName,
                                    const char* variableType)
{
    int i;
    
    def->variableName = variableName;
    def->variableType = variableType;

    for(i = 0, def->variableTypeChar = 0; def->variableType[i]; i++){
        char c = tolower(def->variableType[i]);

        if(c >= 'a' && c <= 'z'){
            devassert(!def->variableTypeChar);
            def->variableTypeChar = def->variableType[i];
        }
    }

    devassert(def->variableTypeChar);
}

static char *resizeTempBuffer(char *buffer, int *size, char **cursor, char **bufEnd)
{
    char *newMessageResize;
    int offset = *cursor - buffer;
    int origSize = *size;

    *size = *size * 2; 
    newMessageResize = (char *) malloc(*size);
    strncpy_s(newMessageResize, *size, buffer, origSize);
    free(buffer);
    *bufEnd = newMessageResize + *size;
    *cursor = newMessageResize + offset;
    return newMessageResize;
}


// MAK - <rant>I'd really like to take the sprintf compatibility out.  I don't know why
// callers use both msPrintf and sprintf interchangeably.  You can't call
// msPrintf with a string containing {Param} and get the results you expect.</rant>
int msvaPrintfInternalEx(    MessageStore* store,
                            char* outputBuffer,
                            int bufferLength,
                            const char* messageID,
                            Array* messageTypeDef,
                            ScriptVarsTable* vars,
                            int recur_depth,
                            int flags,
                            va_list arg)
{
    #define BUFFER_SIZE 16 * 1024
    const TextMessage* textMessage = NULL;
    int     newMessageSize = 0;
    char *newMessage = NULL;
    char *newMessageEnd = NULL;
//    char newMessage[BUFFER_SIZE];
    char paramBuf[BUFFER_SIZE];
    const char* oldMessageCursor;
    char* newMessageCursor;
    const char* varEnd = NULL; // where the start of a var from '{}' is

    //ZeroArray(newMessage);
    msPrintfError = 0;
    if(!store)
    {
        msPrintfError = 1;
        return vsprintfInternal(outputBuffer, bufferLength, messageID, arg);
    }

    // Look up the message to be printed.
    if( flags & TRANSLATE_REVERSE )
    {
        if (!stashFindPointerConst(store->messageStash, messageID, &textMessage))
            textMessage = NULL;
    }
    else
    {
        // server message stores could have v_, l_ and p_ alternates so
        // the single prefix in the message store structure isn't enough
        // for our needs and we have to hardcode the extra prefixes

        // try loyalist-specific text first
        if( store->idPrepend && !(flags&TRANSLATE_NOPREPEND) && flags&TRANSLATE_PREPEND_L_P )
        {
            char loyalistMessageID[BUFFER_SIZE];
            Strncpyt( loyalistMessageID, "L_" );
            Strncatt( loyalistMessageID, messageID );
            if( !stashFindPointerConst( store->messageIDStash, loyalistMessageID, &textMessage ) )
                textMessage = NULL;
        }

        // try the villains/praetorians string first
        if( !textMessage && store->idPrepend && !(flags&TRANSLATE_NOPREPEND) )
        {
            char villainsMessageID[BUFFER_SIZE];

            if( ( flags&TRANSLATE_PREPEND_P ) || ( flags&TRANSLATE_PREPEND_L_P ) )
                Strncpyt( villainsMessageID, "P_" );
            else
                Strncpyt( villainsMessageID, store->idPrepend );

            Strncatt( villainsMessageID, messageID );
            if (!stashFindPointerConst(store->messageIDStash, villainsMessageID, &textMessage))
                textMessage = NULL;
        }
        
        // fallback to the coh string
        if( !textMessage )
        {
            if (!stashFindPointerConst(store->messageIDStash, messageID, &textMessage))
                textMessage = NULL;
        }
    }

    // If the message cannot be found, print the messageID itself.
    if(!textMessage && !(flags & HANDLE_PARAMS_ANYWAY)){
        msPrintfError = 1;
        return vsprintfInternal(outputBuffer, bufferLength, messageID, arg);
    }

    // put a temp pointer in with each type definition
    if(!messageTypeDef && textMessage) // caller can override the typedef
    {
        // construct a local stack typedef array from the information in the textMessage->variableDefNameIndices and the string table
        U32 uiNumVariableDefs = SAFE_MEMBER(textMessage->variableDefNameIndices, size);
        U32 uiIndex;
        messageTypeDef = _alloca(sizeof(Array));
        messageTypeDef->size = messageTypeDef->maxSize = uiNumVariableDefs;
        if ( uiNumVariableDefs > 0 )
            messageTypeDef->storage = _alloca(sizeof(*messageTypeDef->storage) * uiNumVariableDefs);
        for (uiIndex=0; uiIndex < uiNumVariableDefs; ++uiIndex)
        {
            NamedVariableDef*    pDef = _alloca(sizeof(*pDef));
            int*                indices = (int*)textMessage->variableDefNameIndices->storage;
            
            initVariableNameAndType(pDef,
                                    strTableGetConstString(store->variableStringTable, indices[uiIndex]),
                                    strTableGetConstString(store->variableStringTable, indices[uiIndex] + 1));

            pDef->variablePointer = NULL;
            messageTypeDef->storage[uiIndex] = pDef;
        }
    }
    msGetTypeDefPointers(messageTypeDef, arg);

    // MAK - modified so string is generated in one pass
    if (!textMessage && (flags & HANDLE_PARAMS_ANYWAY))
        oldMessageCursor = messageID;
    else if( flags & TRANSLATE_HELP )
        oldMessageCursor = strTableGetConstString(store->messages, textMessage->helpIndex);
    else if( flags & TRANSLATE_REVERSE )
        oldMessageCursor = textMessage->messageID;
    else
        oldMessageCursor = strTableGetConstString(store->messages, textMessage->messageIndex);

    newMessageSize = ((int) strlen(oldMessageCursor)) + 1; 
//    newMessageSize = ((int) strlen(oldMessageCursor)) * 2; 
    newMessage = (char *) calloc(newMessageSize, 1);
    newMessageCursor = newMessage;
    newMessageEnd = newMessage + newMessageSize;

    while(*oldMessageCursor)
    {
        switch(*oldMessageCursor)
        {
            // If the \{ or \} characters are encountered, turn them into { and } characters, respectively.
            xcase '\\':
            {
                while (newMessageCursor + 1 >= newMessageEnd) 
                {
                    newMessage = resizeTempBuffer(newMessage, &newMessageSize, &newMessageCursor, &newMessageEnd);
                }

                switch(oldMessageCursor[1])
                {
                    xcase '{':
                    case '}':
                        *(newMessageCursor++) = oldMessageCursor[1];
                        oldMessageCursor += 2;
                    xcase 'n':
                        *(newMessageCursor++) = '\n';
                        oldMessageCursor += 2;
                    xdefault:
                        // If a "\{" or "\}" was not found, just copy the backslash character.
                        *(newMessageCursor++) = *(oldMessageCursor++);
                }
            }
            
            xcase '{':
            {
                // If we find the open brace by itself, we've found the beginning of a variable name.
                char *endvar;
                char variableName[1024];

                // MAK - need to avoid mangling oldMessage because it may be in shared memory
                oldMessageCursor++;
                endvar = strchr(oldMessageCursor, '}');

                if (!endvar || endvar > oldMessageCursor + 1023)
                {
                    Errorf("Error processing clause in Message %s, clause is too long", messageID);
                    break; // no good way to recover here, log it?
                }

                strncpyt(variableName, oldMessageCursor, endvar - oldMessageCursor + 1);

                msPrintParam(messageID, paramBuf, ARRAY_SIZE(paramBuf), variableName, store, messageTypeDef, vars, recur_depth, flags, arg);

                while (strlen(paramBuf) + newMessageCursor >= newMessageEnd) 
                {
                    newMessage = resizeTempBuffer(newMessage, &newMessageSize, &newMessageCursor, &newMessageEnd);
                }

                strcpy_s(newMessageCursor, newMessage + newMessageSize - newMessageCursor, paramBuf);

                oldMessageCursor = endvar + 1;
                newMessageCursor += strlen(newMessageCursor);
                varEnd = newMessageCursor; // track the end of the variable explicitly
            }

            xcase '[':
            {
            // Korean postposition thing
                if( getCurrentLocale() == 3 // its korean
                    && varEnd ) // we have a variable that was substituted
                {
                    char *endvar;
                    char variableName[1024];
                    wchar_t prevChar = 0;
                    int koreanVowelSound = 0;
                    
                    // MAK - need to avoid mangling oldMessage because it may be in shared memory
                    oldMessageCursor++;
                    endvar = strchr(oldMessageCursor, ']');
                    
                    if (!endvar || endvar > oldMessageCursor + 1023)
                    {
                        Errorf("Error processing clause in Message %s, clause is too long", messageID);
                        break; // no good way to recover here, log it?
                    }
                    
                    strncpy_s(SAFESTR(variableName), oldMessageCursor, endvar - oldMessageCursor);
                    variableName[endvar - oldMessageCursor] = '\0';
                    
                    // get the last korean character in the converted message
                    {
                        wchar_t var[1024];
                        int lenVar = 0;
                        char *tmp = newMessage;
                        int i;
                        
                        while (*tmp && tmp < varEnd)
                        {
                            tmp = UTF8GetNextCharacter( tmp );
                            lenVar++;
                        }
                        
                        i = UTF8ToWideStrConvert(newMessage, var, ARRAY_SIZE( var ));
                        for(;i >= 0; --i)
                        {
                            if(s_CharIsPostPositionable(var[i]))
                                break;
                        }
                        
                        if(i >= 0)
                            prevChar = var[i];
                    }
                    
                    // perform the conversion (if applicable)
                    if ( prevChar && s_ChoosePostposition(prevChar, &koreanVowelSound, variableName) )
                    {
                        char *copyPos = variableName;
                        int varlength = UTF8GetLength(variableName);
                        
                        if (koreanVowelSound) 
                        {
                            // skip first or second
                            copyPos += UTF8GetCharacterLength(copyPos);
                            if (varlength > 2)
                                copyPos += UTF8GetCharacterLength(copyPos);
                            
                            // use rest
                            while (*copyPos) 
                            {
                                while (newMessageCursor + 1 >= newMessageEnd) 
                                {
                                    newMessage = resizeTempBuffer(newMessage, &newMessageSize, &newMessageCursor, &newMessageEnd);
                                }

                                *newMessageCursor++ = *copyPos++;
                            }
                            
                        } else {
                            int copySize = UTF8GetCharacterLength(variableName);
                            if (varlength > 2)
                            {
                                char *nextChar = UTF8GetNextCharacter(variableName);
                                copySize += UTF8GetCharacterLength(nextChar);
                            }

                            while (newMessageCursor + copySize + 1 >= newMessageEnd) 
                            {
                                newMessage = resizeTempBuffer(newMessage, &newMessageSize, &newMessageCursor, &newMessageEnd);
                            }

                            // use first or first and second (adverb case)
                            strncpy_s(newMessageCursor, newMessageEnd - newMessageCursor - 1, 
                                variableName, copySize);
                            newMessageCursor += copySize;
                        }
                        oldMessageCursor = endvar + 1; 
                    } 
                    else 
                    {
                        // didn't find anything, just append the variable data and be done
                        while (newMessageCursor + (int) (endvar - oldMessageCursor + 2) >= newMessageEnd) 
                        {
                            newMessage = resizeTempBuffer(newMessage, &newMessageSize, &newMessageCursor, &newMessageEnd);
                        }

                        strncpy_s(newMessageCursor, endvar - oldMessageCursor + 1,
                            oldMessageCursor - 1, endvar - oldMessageCursor + 2);
                        
                        newMessageCursor += endvar - oldMessageCursor + 2;
                        oldMessageCursor = endvar + 1;
                    }                     
                } else {
                    while (newMessageCursor + 1 >= newMessageEnd) 
                    {
                        newMessage = resizeTempBuffer(newMessage, &newMessageSize, &newMessageCursor, &newMessageEnd);
                    }
                    *(newMessageCursor++) = *(oldMessageCursor++);
                }
            }
            
            xdefault:
            {
                while (newMessageCursor + 1 >= newMessageEnd) 
                {
                    newMessage = resizeTempBuffer(newMessage, &newMessageSize, &newMessageCursor, &newMessageEnd);
                }
                *(newMessageCursor++) = *(oldMessageCursor++);
            }
        }
    }

    *newMessageCursor = '\0';
    if(outputBuffer) 
    {
        strcpy_s(outputBuffer, bufferLength, newMessage);
    }

    newMessageSize = newMessageCursor - newMessage;
    if (newMessage) {
        free(newMessage);
    }
    return newMessageSize;
    
    #undef BUFFER_SIZE
}

/* Function msCompileMessageType()
 *    Generate a message type definition in the format expected by the message store
 *    functions, given the string to define the message type.
 */
Array* msCompileMessageType(const char* typeDefString){
    char* parseCursor;
    char* variableName;
    char* variableType;
    char messageTypeDef[1024];
    Array* result;

    result = createArray();
    strcpy(messageTypeDef, typeDefString);

    // Hook up the message types definition.
    // For each pair of strings enclosed in { and }...
    parseCursor = messageTypeDef;
    while(variableType = readEnclosedString(&parseCursor, '{', '}')){
        NamedVariableDef* def;

        // Get the variable name and the expected type.
        variableName = strsep(&variableType, ",");

        eatWhitespace(&variableName);
        eatWhitespace(&variableType);

        def = calloc(1, sizeof(NamedVariableDef));
        
        initVariableNameAndType(def, strdup(variableName), strdup(variableType));
        
        arrayPushBack(result, def);
    }

    return result;
}

void msSetFormatHandler(MessageStore* store, char character, MessageStoreFormatHandler handler)
{
    U32 isUpper = character >= 'A' && character <= 'Z';
    U32 index = character - (isUpper ? 'A' : 'a');
    
    if(    verify(index <= 'Z' - 'A') &&
        verify(!strchr("srfgG", character)))
    {
        store->formatHandler[index][isUpper] = handler;
    }
}

void msSetAttributeTables(    MessageStore* store,
                            StaticDefineInt* attributes, 
                            StaticDefineInt** attribToValue,
                            S32* valueOffsetInUserData)
{
    store->attributes = attributes;
    store->attribToValue = attribToValue;
    store->valueOffsetInUserData = valueOffsetInUserData;
}

void msSetPrepend(MessageStore* store, const char* prepend){
    SAFE_FREE(store->idPrepend);
    
    if(prepend){
        store->idPrepend = strdup(prepend);
    }
}

int msIsLocked(MessageStore* store){
    return store->bLocked ? 1 : 0;
}

const char* vaCmdTranslate( const char *str, va_list args, int help, int reverse )
{
    static char  *buffer = NULL;
    static int buffersize = 0;
    int requiredBufferSize;
    int flag = 0;

    if (!str)
        return NULL;

    if( help )
        flag |= TRANSLATE_HELP;
    if(reverse)
        flag |= TRANSLATE_REVERSE;

    requiredBufferSize = msvaPrintfInternalEx( cmdMessages, NULL, 0, str, NULL, NULL, 0, flag, args );
    requiredBufferSize = MAX(requiredBufferSize, (int)strlen(str));
    if( buffersize < requiredBufferSize+1 )
    {
        buffersize = requiredBufferSize+1;
        buffer = realloc( buffer, buffersize );
    }

    strcpy_s(buffer, buffersize, str);

    msvaPrintfInternalEx( cmdMessages, buffer, buffersize, str, NULL, NULL, 0, flag, args );

    return buffer;
}

const char *cmdTranslated(const char * str, ...)
{
    va_list va;
    const char  *buffer;

    va_start(va, str);
    buffer = vaCmdTranslate( str, va, 0, 0 );
    va_end(va);

    return buffer;
}

const char *cmdRevTranslated(const char * str, ...)
{
    va_list va;
    const char  *buffer;

    va_start(va, str);
    buffer = vaCmdTranslate( str, va, 0, 1 );
    va_end(va);

    return buffer;
}

const char *cmdHelpTranslated(const char * str, ...)
{
    va_list va;
    const char  *buffer;

    va_start(va, str);
    buffer = vaCmdTranslate( str, va, 1, 0 );
    va_end(va);

    return buffer;
}

int msContainsKey(MessageStore* store, const char *messageID)
{
    return stashFindPointerConst(store->messageIDStash, messageID, NULL);
}

ExtendedTextMessage* getExtendedMessage(MessageStore* store, TextMessage* msg){
    return store->useExtendedTextMessage ? (ExtendedTextMessage* )(msg + 1) : NULL;
}

static void msSetMessageIsModified(MessageStore* store, TextMessage* msg, int modified){
    ExtendedTextMessage* extMsg = getExtendedMessage(store, msg);
    
    if(extMsg){
        extMsg->modified = !!modified;
    }
}

static int msGetMessageIsModified(MessageStore* store, TextMessage* msg){
    ExtendedTextMessage* extMsg = getExtendedMessage(store, msg);
    
    return SAFE_MEMBER(extMsg, modified);
}

static void msSetMessageIsNew(MessageStore* store, TextMessage* msg, int newMessage){
    ExtendedTextMessage* extMsg = getExtendedMessage(store, msg);
    
    if(extMsg){
        extMsg->newMessage = !!newMessage;
    }
}

static int msGetMessageIsNew(MessageStore* store, TextMessage* msg){
    ExtendedTextMessage* extMsg = getExtendedMessage(store, msg);
    
    return SAFE_MEMBER(extMsg, newMessage);
}

void msClearExtendedDataFlags(MessageStore* store){
    StashTableIterator iter;
    StashElement elem;
    
    stashGetIterator(store->messageIDStash, &iter);
    
    while (stashGetNextElement(&iter, &elem))
    {
        TextMessage* textMessage = stashElementGetPointer(elem);
        
        msSetMessageIsModified(store, textMessage, 0);
        msSetMessageIsNew(store, textMessage, 0);
    }
}

void msSetMessageComment(MessageStore* store, TextMessage* msg, const char* comment){
    ExtendedTextMessage* extMsg = getExtendedMessage(store, msg);
    
    if(extMsg){
        extMsg->commentLine = strTableAddString(store->messages, comment);
    }
}

const char* msGetMessageComment(MessageStore* store, TextMessage* msg){
    ExtendedTextMessage* extMsg = getExtendedMessage(store, msg);
    
    return SAFE_MEMBER(extMsg, commentLine);
}

void msUpdateMessage(MessageStore *store, const char *messageID, const char *currentLocaleText, const char *comment)
{
    TextMessage *oldTextMessage = NULL;
    
    stashFindPointer(store->messageIDStash, messageID, &oldTextMessage);

    if (!oldTextMessage) {
        // New message!
        TextMessage *newTextMessage = msAddMessage(store, messageID, currentLocaleText, NULL);
        assert(newTextMessage);
        msSetMessageIsNew(store, newTextMessage, 1);
        msSetMessageComment(store, newTextMessage, comment);
    } else {
        // Modifying an old message
        oldTextMessage->messageIndex = strTableAddStringGetIndex(store->messages, currentLocaleText);
    
        msSetMessageIsModified(store, oldTextMessage, 1);
        msSetMessageComment(store, oldTextMessage, comment);
    }
}

void msSaveMessageStore(MessageStore *store)
{
    StashTableIterator iter;
    StashElement elem;
    char *messageData, *cursor;
    int argc;
    char *args[10];
    FILE *fout;
    bool changed=false;
    bool checkedOut=true;
    int trysLeft=5;
    char filename[512];
    
    assert(store->useExtendedTextMessage);

    // Check for anything changed
    stashGetIterator(store->messageIDStash, &iter);
    while (stashGetNextElement(&iter, &elem))
    {
        TextMessage *textMessage = stashElementGetPointer(elem);
        if(    msGetMessageIsModified(store, textMessage) ||
            msGetMessageIsNew(store, textMessage))
        {
            changed = true;
            break;
        }
    }
    if (!changed)
        return;

    // Read message store, keeping comments, changing modified versions
    fileLocateWrite(store->fileLoadedFrom, filename);
    mkdirtree(filename);
    messageData = fileAlloc(store->fileLoadedFrom, 0);
    if (!messageData) {
        Errorf("Error reading in old %s", store->fileLoadedFrom);
        return;
    }
    if(0 != memcmp(messageData, UTF8BOM, 3))
    {
        printf("Unable to read file \"%s\" because it is not a UTF8 file!\n", store->fileLoadedFrom);
        free(messageData);
        return;
    }

    fout = fileOpen(store->fileLoadedFrom, "wb");
    if (!fout) {
        Errorf("Error opening %s for writing", store->fileLoadedFrom);
        free(messageData);
        return;
    }
    fwrite(UTF8BOM, 1, ARRAY_SIZE(UTF8BOM), fout);
    cursor = messageData;
    cursor+=ARRAY_SIZE(UTF8BOM);
    while (cursor && *cursor)
    {
        int len;
        char *end = strchr(cursor, '\r');
        if (!end) {
            end = cursor + strlen(cursor)-1;
        } else {
            if (end[1]=='\n') {
                end++;
            }
        }
        len = end - cursor + 1;
//        while (strchr("\r \t\n", *cursor)) // Skip these characters
//            cursor++;
        if (cursor[0]=='\r' || cursor[0]=='#') {
            // Comment or empty line, pass-through
            fwrite(cursor, 1, len, fout);
            if (cursor[len-1]!='\n') {
                char c='\n';
                fwrite(&c, 1, 1, fout);
            }
            cursor+=len;
        } else {
            // Data line
            char *line = calloc(len+1, 1);
            strncpy_s(line, len+1, cursor, len);
            argc = tokenize_line_safe(cursor, args, ARRAY_SIZE(args), &cursor);
            if (argc<2) {                
                ErrorFilenamef(store->fileLoadedFrom, "Invalid format: found %d tokens, expected 2 or more", argc);
                fwrite(line, 1, len, fout);
            } else {
                TextMessage *textMessage;
                if (!stashFindPointer(store->messageIDStash, args[0], &textMessage))
                    textMessage = NULL;

                if(    textMessage &&
                    (    msGetMessageIsModified(store, textMessage) ||
                        msGetMessageIsNew(store, textMessage)))
                {
                    if (strchr(strTableGetConstString(store->messages,textMessage->messageIndex), '\"')) {
                        fprintf(fout, "\"%s\", <<%s>>\r\n", textMessage->messageID, strTableGetConstString(store->messages,textMessage->messageIndex));
                    } else {
                        fprintf(fout, "\"%s\", \"%s\"\r\n", textMessage->messageID, strTableGetConstString(store->messages,textMessage->messageIndex));
                    }
                    msSetMessageIsModified(store, textMessage, 0);
                    msSetMessageIsNew(store, textMessage, 0);
                } else {
                    fwrite(line, 1, len, fout);
                }
            }
            free(line);
        }
    }
    // Write all new messages
    stashGetIterator(store->messageIDStash, &iter);
    while (stashGetNextElement(&iter, &elem)) {
        TextMessage *textMessage = stashElementGetPointer(elem);
        if (msGetMessageIsNew(store, textMessage)) {
            // Append
            fprintf(fout, "%s\r\n", msGetMessageComment(store, textMessage));

            if (strchr(strTableGetConstString(store->messages, textMessage->messageIndex), '\"')) {
                fprintf(fout, "\"%s\", <<%s>>\r\n", textMessage->messageID, strTableGetConstString(store->messages, textMessage->messageIndex));
            } else {
                fprintf(fout, "\"%s\", \"%s\"\r\n", textMessage->messageID, strTableGetConstString(store->messages, textMessage->messageIndex));
            }
            msSetMessageIsNew(store, textMessage, 0);
        }
    }
    fclose(fout);
    free(messageData);
}

void msCreateCmdMessageStore(const char* fileName, int localeID)
{
    const MessageStoreLoadFlags flags = MSLOAD_DEFAULT;

    if(cmdMessages){
        destroyMessageStore(cmdMessages);
        cmdMessages = NULL;
    }

    cmdMessages = createMessageStore(flags);
    setCmdMessageStore( cmdMessages ); // flag it for reading triples of cmds
    initMessageStore( cmdMessages, localeID, NULL );
    
    msAddMessages(cmdMessages, flags, fileName, NULL);
}
