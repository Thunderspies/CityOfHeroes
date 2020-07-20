#include <vorbis/vorbisfile.h>
#include "sound/sound_sys.h"
#include "stdlib.h"
#include "memory.h"
#include "assert.h"
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/memorypool.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/file.h>
#include "win/win_init.h"

typedef struct
{
    char    *mem;
    int        curr;
    int        length;
} MemFile;

typedef struct OggState OggState;

typedef struct OggState
{
    OggVorbis_File    vf;
    MemFile            mf;
    S32                current_section;
    S32                in_use;
    
    S32                pool_memory_used;
    S32                heap_memory_used;
    
    struct {
        OggState*    next;
        OggState*    prev;
    } sibling;
    
    char            name[128];
} OggState;

OggState*    curOggState;

OggState*    oggStateList;

S32            oggStatesInUse;

#define USE_OGG_MEMPOOLS 1

#define OGG_ALLOC_FILEINFO        const char* fileName, int line

#if USE_OGG_MEMPOOLS
typedef struct OggMemPool
{
    S32            chunkSize;
    S32            chunkCount;
    MemoryPool    memPool;
    char*        name;
    S32            maxAlloced;
} OggMemPool;

static OggMemPool oggMemPools[] = {
    { 1        * 128,    100    },
    { 5        * 128,    100    },
    { 1        * 1024,    50    },
    { 10    * 1024, 10    },
    { 50    * 1024, 10    },
    { 100    * 1024, 5    },
    { 200    * 1024, 2    },
    { 300    * 1024, 2    },
    { 400    * 1024, 1    },
    { 500    * 1024, 1    },
    { 1000    * 1024, 1    },
    { 0 },
};

static struct {
    U32        usedSize;
    U32        usedSizeMax;
    
    U32        usedPoolSize;
    U32        usedPoolSizeMax;

    U32        usedHeapSize;
    U32        usedHeapSizeMax;
} oggMemInfo;


static OggMemPool* getOggMemPool(S32 size)
{
    OggMemPool* pool = oggMemPools;
    
    for(; pool->chunkSize; pool++)
    {
        if(size <= pool->chunkSize || !pool->chunkSize)
        {
            if(!pool->memPool && pool->chunkSize){
                char buffer[100];
                
                STR_COMBINE_BEGIN(buffer);
                STR_COMBINE_CAT("oggPool(");
                STR_COMBINE_CAT_D(pool->chunkSize);
                STR_COMBINE_CAT("b)");
                STR_COMBINE_END();
                
                pool->name = strdup(buffer);
                pool->memPool = createMemoryPoolNamed(pool->name, __FILE__, __LINE__);
                initMemoryPool(pool->memPool, pool->chunkSize + sizeof(S32), pool->chunkCount);
                mpSetMode(pool->memPool, 0);
            }
            
            return pool;
        }
    }
    
    assert(0);
    
    return NULL;
}



static void* addOggMemoryHeader(S32* mem, S32 size)
{
    *mem = size;
    return ++mem;
}

static void checkAllocationSizes(){
    U32 oggHeapSize = 0;
    U32 oggPoolSize = 0;
    OggState* cur;
    
    if(isProductionMode()){
        return;
    }
    
    for(cur = oggStateList; cur; cur = cur->sibling.next){
        assert(cur->in_use);
        oggPoolSize += cur->pool_memory_used;
        oggHeapSize += cur->heap_memory_used;
    }
    
    assert(oggPoolSize == oggMemInfo.usedPoolSize);
    assert(oggHeapSize == oggMemInfo.usedHeapSize);
}

static void* allocSizeFromPool(OggMemPool* pool, S32 size, S32 zeroMemory)
{
    void* mem;
    
    assert(size >= 0);
    
    oggMemInfo.usedSize += size;
    
    if(oggMemInfo.usedSize > oggMemInfo.usedSizeMax)
    {
        oggMemInfo.usedSizeMax = oggMemInfo.usedSize;
    }

    devassert(curOggState);

    if(pool->chunkSize)
    {
        // Allocate from a MemoryPool.
        
        mem = mpAlloc(pool->memPool);
        
        oggMemInfo.usedPoolSize += pool->chunkSize;
        
        if(oggMemInfo.usedPoolSize > oggMemInfo.usedPoolSizeMax)
        {
            oggMemInfo.usedPoolSizeMax = oggMemInfo.usedPoolSize;
        }

        if(curOggState)
        {
            curOggState->pool_memory_used += pool->chunkSize;
        }
    }
    else
    {
        // Allocate from the heap.
        
        mem = malloc(size + sizeof(S32));
        
        oggMemInfo.usedHeapSize += size;

        if(oggMemInfo.usedHeapSize > oggMemInfo.usedHeapSizeMax)
        {
            oggMemInfo.usedHeapSizeMax = oggMemInfo.usedHeapSize;
        }
        
        if(curOggState)
        {
            curOggState->heap_memory_used += size;
        }
    }

    mem = addOggMemoryHeader(mem, size);
    
    if(zeroMemory){
        memset(mem, 0, size);
    }

    return mem;    
}

static void freeMemoryFromPool(OggMemPool* pool, S32* mem)
{
    U32 size = mem[-1];
    
    devassert(size <= oggMemInfo.usedSize);
    
    oggMemInfo.usedSize -= size;
    
    devassert(curOggState);

    if(pool->chunkSize)
    {
        // Free from a MemoryPool.
        
        mpFree(pool->memPool, mem - 1);
        
        devassert(pool->chunkSize <= (S32)oggMemInfo.usedPoolSize);

        oggMemInfo.usedPoolSize -= pool->chunkSize;

        if(curOggState)
        {
            devassert(pool->chunkSize <= curOggState->pool_memory_used);
            
            curOggState->pool_memory_used -= pool->chunkSize;
        }
    }
    else
    {
        // Free from the heap.
        
        free(mem - 1);

        devassert(size <= oggMemInfo.usedHeapSize);

        oggMemInfo.usedHeapSize -= size;

        if(curOggState)
        {
            devassert((S32)size <= curOggState->heap_memory_used);
            
            curOggState->heap_memory_used -= size;
        }
    }
}

static int oggHeapEntranceCount;

static void enterOggHeap()
{
    assert(InterlockedIncrement(&oggHeapEntranceCount) == 1);
}

static void leaveOggHeap()
{
    assert(InterlockedDecrement(&oggHeapEntranceCount) == 0);
}    

static void* allocateOggMemory(S32 size, S32 zeroMemory)
{
    OggMemPool* pool;
    void* memory;
    
    enterOggHeap();
    
    pool = getOggMemPool(size);
    
    memory = allocSizeFromPool(pool, size, zeroMemory);
    
    leaveOggHeap();
    
    return memory;
}

void* cryptic_ogg_malloc(S32 size, OGG_ALLOC_FILEINFO)
{
    return allocateOggMemory(size, 0);
}

void* cryptic_ogg_calloc(S32 num, S32 size, OGG_ALLOC_FILEINFO)
{
    return allocateOggMemory(num * size, 1);
}

void* cryptic_ogg_realloc(void* userData, S32 newSize, OGG_ALLOC_FILEINFO)
{
    OggMemPool* oldPool;
    OggMemPool* newPool;
    void*        newMemory;
    S32            oldSize;
    
    if(!userData)
    {
        return cryptic_ogg_malloc(newSize, fileName, line);
    }
    
    enterOggHeap();
    
    oldSize = *((S32*)userData - 1);
    oldPool = getOggMemPool(oldSize);
    newPool = getOggMemPool(newSize);
    
    if(oldPool == newPool)
    {
        leaveOggHeap();
        return userData;
    }
    
    newMemory = allocSizeFromPool(newPool, newSize, 0);
    
    memcpy(newMemory, userData, min(oldSize, newSize));
    
    freeMemoryFromPool(oldPool, userData);
    
    leaveOggHeap();
    
    return newMemory;
}

void cryptic_ogg_free(void* userData, OGG_ALLOC_FILEINFO)
{
    OggMemPool* pool;
    
    if(!userData)
    {
        return;
    }
    
    enterOggHeap();
    
    pool = getOggMemPool(*((S32*)userData - 1));

    freeMemoryFromPool(pool, userData);
    
    leaveOggHeap();
}
#else
void* cryptic_ogg_malloc(S32 size, OGG_ALLOC_FILEINFO)
{
    #if 0
        // MS: Debugging code to find memory leak.
        
        char* slash;
        
        if((slash = strrchr(fileName, '\\')) && !stricmp(slash + 1, "sharedbook.c"))
        {
            int x = 10;
        }
    #endif
        
    return _malloc_dbg(size, 1, fileName, line);
}

void* cryptic_ogg_calloc(S32 num, S32 size, OGG_ALLOC_FILEINFO)
{
    return _calloc_dbg(num, size, 1, fileName, line);
}

void* cryptic_ogg_realloc(void* userData, S32 newSize, OGG_ALLOC_FILEINFO)
{
    return _realloc_dbg(userData, newSize, 1, fileName, line);
}

void cryptic_ogg_free(void* userData, OGG_ALLOC_FILEINFO)
{
    _free_dbg(userData, 1);
}
#endif

static void setCurOggState(OggState* oggState)
{
    curOggState = oggState;
}

static void clearCurOggState()
{
    setCurOggState(NULL);
}


static size_t readbytes(void *dst,size_t struct_size,size_t num_structs,MemFile *mf)
{
    size_t        t,amt;

    amt = num_structs * struct_size;
    t = mf->length - mf->curr;
    if (t < amt)
        amt = t;
    memcpy(dst,mf->mem + mf->curr,amt);

    mf->curr += amt;
    if (struct_size != 1)
        return amt / struct_size;
    return amt;
}

 
static int seekbytes(void *v_mf,ogg_int64_t amt,int whence)
{
    MemFile *mf = v_mf;
    if (whence == SEEK_SET)
        mf->curr = amt;
    if (whence == SEEK_CUR)
        mf->curr += amt;
    if (mf->curr >= mf->length)
        mf->curr = mf->length;
    if (whence == SEEK_END)
    {
        mf->curr = mf->length-amt;
        if (mf->curr < 0)
            mf->curr = 0;
    }
    return 0;
}

static int closebytes(void/*MemFile*/ *mf)
{
    return 0;
}

static long tellbytes(void *v_mf)
{
    MemFile *mf = v_mf;
    return mf->curr;
}

static ov_callbacks callbacks = { readbytes,
    seekbytes,
    closebytes,
    tellbytes };

static void oggListAdd(OggState* ogg)
{
    assert(!ogg->sibling.next && !ogg->sibling.prev);
    
    if(oggStateList){
        assert(!oggStateList->sibling.prev);
        
        oggStateList->sibling.prev = ogg;
    }
    
    ogg->sibling.next = oggStateList;
    oggStateList = ogg;

    checkAllocationSizes();
}

static void oggListRemove(OggState* ogg)
{
    if(ogg->sibling.prev)
    {
        ogg->sibling.prev->sibling.next = ogg->sibling.next;
    }
    else
    {
        assert(oggStateList == ogg);
        
        oggStateList = ogg->sibling.next;
    }

    if(ogg->sibling.next)
    {
        ogg->sibling.next->sibling.prev = ogg->sibling.prev;
    }
    
    ZeroStruct(&ogg->sibling);

    checkAllocationSizes();
}


// returns true if succeeded, 0 otherwise
static int oggInitDecoder(DecodeState *decode)
{
    OggState    *ogg;
    vorbis_info *vi;

    PERFINFO_AUTO_START("oggInitDecoder", 1);
        ogg = decode->codec_state = realloc(decode->codec_state,sizeof(*ogg));
        ZeroStruct(ogg);
        ogg->mf.mem = decode->info->data;
        ogg->mf.length = decode->info->length;
        
        Strncpyt(ogg->name, decode->info->name);
        
        PERFINFO_AUTO_START("ov_open_callbacks", 1);
            setCurOggState(ogg);
            if (ov_open_callbacks(&ogg->mf, &ogg->vf, NULL, 0, callbacks) < 0)
            {
                clearCurOggState();
                PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();
                return 0;
            }
            clearCurOggState();
        PERFINFO_AUTO_STOP();

        oggStatesInUse++;
        
        ogg->in_use = 1;

        oggListAdd(ogg);
        
        setCurOggState(ogg);
        vi = ov_info(&ogg->vf,-1);
        clearCurOggState();

        decode->frequency = vi->rate;
        decode->num_channels = vi->channels;

        setCurOggState(ogg);
        decode->pcm_len = decode->num_channels * 2 * ov_pcm_total(&ogg->vf,-1);
        clearCurOggState();
    PERFINFO_AUTO_STOP();
    
    return 1;
}

void handleOggCrash(DecodeState* decode){
    static int beenHere = 0;
    
    char buffer[200*1000];
    int buffer_len;
    OggState ogg;
    int yes;
    
    if(beenHere){
        return;
    }
    
    beenHere = 1;
    
    yes = winMsgYesNo(    "CityOfHeroes.exe would have crashed just now, but it won't unless you press YES.\n\n"
                        "If you press NO, it might still crash, but there's a chance it won't.\n\n"
                        "By pressing YES, you will submit a helpful crash report to Cryptic Studios.\n\n"
                        "So to summarize:\n\n"
                        "    YES = submit crash report, and exit CoH.\n"
                        "    NO = continue playing.\n\n"
                        "Note: This message will only appear once.");
                        
    if(yes){
        yes = winMsgYesNo("Are you SURE you want to submit the crash report?");
    }
    
    if(!yes){
        return;
    }
                
    ogg = *(OggState*)decode->codec_state;
    
    memcpy(buffer, decode->info->data, min(sizeof(buffer), decode->info->length));
    buffer_len = decode->info->length;
    
    assertmsg(0, "Crashing because ogg decoder caused an exception and the user chose to submit a crash report.");
}

int oggDecode(DecodeState *decode)
{
    SoundFileInfo    infoLocal;
    OggState*        ogg = decode->codec_state;
    int                accumulator = 0;
    
    if (!devassert(ogg->in_use))
        return -1;
    
    infoLocal = *decode->info;

    PERFINFO_RUN(
        int size = decode->decode_len;
        
        PERFINFO_AUTO_START("oggDecode", 1);
        
        if(size <= 100){                                
            PERFINFO_AUTO_START("decode <= 100", 1);            
        }                                                    
        else if(size <= 1000){                                
            PERFINFO_AUTO_START("decode <= 1,000", 1);        
        }                                                    
        else if(size <= 10 * 1000){                                
            PERFINFO_AUTO_START("decode <= 10,000", 1);        
        }                                                    
        else if(size <= 100 * 1000){                            
            PERFINFO_AUTO_START("decode <= 100,000", 1);        
        }                                                    
        else if(size <= SQR(1000)){                            
            PERFINFO_AUTO_START("decode <= 1,000,000", 1);    
        }                                                    
        else if(size <= 10 * SQR(1000)){                            
            PERFINFO_AUTO_START("decode <= 10,000,000", 1);    
        }                                                    
        else{                                                
            PERFINFO_AUTO_START("decode > 10,000,000", 1);    
        }                                                    
    );
    
    #if 1
        for(decode->decode_count = 0;decode->decode_count < decode->decode_len;)
        {
            int wouldCrash = 0;
            int bytes;
            
            PERFINFO_AUTO_START("ov_read", 1);
            
                setCurOggState(ogg);
                __try{
                    bytes=ov_read(&ogg->vf,decode->decode_buffer + decode->decode_count,decode->decode_len - decode->decode_count,0,2,1,&ogg->current_section);
                }__except(wouldCrash = 1, EXCEPTION_EXECUTE_HANDLER){}
                clearCurOggState();
                
                if(wouldCrash){
                    handleOggCrash(decode);
                    decode->decode_count = -1;
                    bytes = -1;
                }

            PERFINFO_AUTO_STOP();
            
            if (bytes > 0)
            {
                // i'm pretty sure this means the thread will not sleep, since 
                // the decode_len is usually set to 32768
                #define SLEEP_SIZE (30 * 10000)
                
                decode->decode_count += bytes;
                
                accumulator += bytes;
                
                if(accumulator >= SLEEP_SIZE)
                {
                    while(accumulator >= SLEEP_SIZE)
                    {
                        accumulator -= SLEEP_SIZE;
                    }
                    
                    Sleep(1);
                }
                
                #undef SLEEP_SIZE
            }
            
            if (bytes <= 0)
            {
                break;
            }
        }
    #else
        if (!decode->init)
        {
            decode->init = 1;
            memset(decode->decode_buffer,0,decode->decode_len);
            decode->decode_count = decode->decode_len;
        }
        else
            decode->decode_count = 0;
    #endif
    
    PERFINFO_RUN(PERFINFO_AUTO_STOP();PERFINFO_AUTO_STOP(););

    return decode->decode_count;
}

void oggResetDecoder(DecodeState *decode)
{
    OggState    *ogg = decode->codec_state;

    if (!ogg || !ogg->in_use)
        return;

    PERFINFO_AUTO_START("oggResetDecoder", 1);
        setCurOggState(ogg);
        ov_clear(&ogg->vf);
        clearCurOggState();
    PERFINFO_AUTO_STOP();
        
    devassert(!ogg->heap_memory_used);
    devassert(!ogg->pool_memory_used);
    
    devassert(oggStatesInUse > 0);
    oggStatesInUse--;
    
    ogg->in_use = 0;

    oggListRemove(ogg);
}

void oggRewindDecoder(DecodeState *decode)
{
    OggState    *ogg = decode->codec_state;

    PERFINFO_AUTO_START("oggRewindDecoder", 1);
        setCurOggState(ogg);
        ov_raw_seek(&ogg->vf,0);
        clearCurOggState();
    PERFINFO_AUTO_STOP();
}

bool oggToPcm(SoundFileInfo *info)
{
    DecodeState    decode;
    assert(!info->pcm_data);

    PERFINFO_AUTO_START("oggToPcm", 1);
        ZeroStruct(&decode);
        decode.info = info;
        oggInitDecoder(&decode);
        decode.decode_buffer = calloc(decode.pcm_len, 1);

        if(decode.decode_buffer) {
            decode.decode_len = decode.pcm_len;
            oggDecode(&decode);
            info->pcm_len = decode.pcm_len;
            info->pcm_data = decode.decode_buffer;
            info->frequency = decode.frequency;
            info->num_channels = decode.num_channels;
        }

        oggResetDecoder(&decode);
        SAFE_FREE(decode.codec_state);

    PERFINFO_AUTO_STOP();

    return (info->pcm_data != NULL);
}

int pcmInitDecoder(DecodeState *decode)
{
    MemFile            *mf;
    SoundFileInfo    *info = decode->info;

    PERFINFO_AUTO_START("pcmInitDecoder", 1);
        decode->codec_state =realloc(decode->codec_state,sizeof(*mf));
        mf = decode->codec_state;
        ZeroStruct(mf);
        mf->mem = info->pcm_data;
        mf->length = info->pcm_len;
        decode->frequency = info->frequency;
        decode->num_channels = info->num_channels;
        decode->pcm_len = info->pcm_len;
    PERFINFO_AUTO_STOP();
    
    return 1;
}

int pcmDecode(DecodeState *decode)
{
    MemFile    *mf = decode->codec_state;

    PERFINFO_AUTO_START("pcmDecode", 1);
        decode->decode_count = readbytes(decode->decode_buffer,1,decode->decode_len,mf);
    PERFINFO_AUTO_STOP();
    
    return decode->decode_count;
}

void pcmResetDecoder(DecodeState *decode) {}

int pcmValidDecoder(DecodeState *decode)
{
    MemFile    *mf = decode->codec_state;
    SoundFileInfo    *info = decode->info;

    return(info->pcm_data == mf->mem);
}

void pcmRewindDecoder(DecodeState *decode)
{
    MemFile    *mf = decode->codec_state;
    assert(pcmValidDecoder(decode));
    mf->curr = 0;    
}

CodecFuncs ogg_funcs = { oggInitDecoder, oggDecode, oggResetDecoder, oggRewindDecoder };
CodecFuncs pcm_funcs = { pcmInitDecoder, pcmDecode, pcmResetDecoder, pcmRewindDecoder };
