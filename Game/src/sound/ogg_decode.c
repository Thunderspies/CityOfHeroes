#include <vorbis/vorbisfile.h>
#include "sound/sound_sys.h"
#include "stdlib.h"
#include "memory.h"
#include "assert.h"
#include <limits.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/SuperAssert.h>
#include <utilitieslib/stdtypes.h>
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

// Upstream codecs own their allocations; only decoder lifecycle is tracked.
typedef struct OggState OggState;

typedef struct OggState
{
    OggVorbis_File    vf;
    MemFile            mf;
    S32                current_section;
    S32                in_use;
    
    struct {
        OggState*    next;
        OggState*    prev;
    } sibling;
    
    char            name[128];
} OggState;

OggState*    oggStateList;

S32            oggStatesInUse;

static size_t readbytes(void *dst, size_t struct_size, size_t num_structs, void* mfData)
{
    MemFile * mf = (MemFile *)mfData;
    size_t available, amt;

    if (!mf || !dst || !mf->mem || !struct_size ||
        mf->curr < 0 || mf->length < mf->curr)
        return 0;

    // Bound the item count before multiplying, as required by fread semantics.
    available = (size_t)(mf->length - mf->curr) / struct_size;
    if (num_structs > available)
        num_structs = available;
    amt = num_structs * struct_size;
    memcpy(dst,mf->mem + mf->curr,amt);

    mf->curr += (int)amt;
    return num_structs;
}

 
static int seekbytes(void *v_mf,ogg_int64_t amt,int whence)
{
    MemFile *mf = v_mf;
    ogg_int64_t base;

    if (!mf || mf->curr < 0 || mf->length < mf->curr)
        return -1;
    switch (whence) {
    case SEEK_SET: base = 0; break;
    case SEEK_CUR: base = mf->curr; break;
    case SEEK_END: base = mf->length; break;
    default: return -1;
    }
    // Check the offset before adding so even signed 64-bit extremes are safe.
    if (amt < -base || amt > (ogg_int64_t)mf->length - base)
        return -1;
    mf->curr = (int)(base + amt);
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
}


// returns true if succeeded, 0 otherwise
static int oggInitDecoder(DecodeState *decode)
{
    OggState    *ogg;
    vorbis_info *vi;
    ogg_int64_t samples;

    if (!decode)
        return 0;
    decode->frequency = decode->num_channels = decode->pcm_len = 0;
    decode->decode_count = 0;

    PERFINFO_AUTO_START("oggInitDecoder", 1);
        // Callers reset before init; this allocation may hold old PCM state.
        ogg = realloc(decode->codec_state,sizeof(*ogg));
        if (!ogg) {
            PERFINFO_AUTO_STOP();
            return 0;
        }
        decode->codec_state = ogg;
        ZeroStruct(ogg);
        if (!decode->info || !decode->info->data || decode->info->length <= 0) {
            PERFINFO_AUTO_STOP();
            return 0;
        }
        ogg->mf.mem = decode->info->data;
        ogg->mf.length = decode->info->length;
        
        Strncpyt(ogg->name, decode->info->name);
        
        PERFINFO_AUTO_START("ov_open_callbacks", 1);
            if (ov_open_callbacks(&ogg->mf, &ogg->vf, NULL, 0, callbacks) < 0)
            {
                PERFINFO_AUTO_STOP();
                PERFINFO_AUTO_STOP();
                return 0;
            }
        PERFINFO_AUTO_STOP();

        vi = ov_info(&ogg->vf,-1);
        samples = ov_pcm_total(&ogg->vf,-1);
        if (!vi || vi->rate <= 0 || vi->rate > INT_MAX ||
            vi->channels <= 0 || vi->channels > INT_MAX / 2 || samples < 0 ||
            samples > INT_MAX / (vi->channels * 2)) {
            ov_clear(&ogg->vf);
            PERFINFO_AUTO_STOP();
            return 0;
        }

        decode->frequency = (int)vi->rate;
        decode->num_channels = vi->channels;
        decode->pcm_len = (int)(decode->num_channels * 2 * samples);

        oggStatesInUse++;
        ogg->in_use = 1;
        oggListAdd(ogg);
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
    OggState*        ogg;
    int                accumulator = 0;

    if (!decode)
        return -1;
    ogg = decode->codec_state;
    decode->decode_count = 0;
    if (!ogg || !decode->frequency || !ogg->in_use || !decode->info ||
        decode->decode_len < 0 ||
        (decode->decode_len > 0 && !decode->decode_buffer)) {
        decode->decode_count = -1;
        return -1;
    }
    if (!decode->decode_len)
        return 0;
    
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
            
                __try{
                    bytes=ov_read(&ogg->vf,decode->decode_buffer + decode->decode_count,decode->decode_len - decode->decode_count,0,2,1,&ogg->current_section);
                }__except(wouldCrash = 1, EXCEPTION_EXECUTE_HANDLER){}
                
                if(wouldCrash){
                    handleOggCrash(decode);
                    decode->decode_count = -1;
                    bytes = -1;
                }

            PERFINFO_AUTO_STOP();

            if (bytes == OV_HOLE)
                continue;
            if (bytes < 0)
                decode->decode_count = -1;
            
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
    OggState *ogg;

    if (!decode)
        return;
    decode->frequency = decode->num_channels = decode->pcm_len = 0;
    decode->decode_count = 0;
    // An unsuccessful realloc can leave a smaller PCM allocation here. Only
    // live list members are owned by Vorbis and safe to inspect as OggState.
    for (ogg = oggStateList; ogg && ogg != decode->codec_state;
        ogg = ogg->sibling.next) {}
    if (!ogg)
        return;

    PERFINFO_AUTO_START("oggResetDecoder", 1);
        ov_clear(&ogg->vf);
    PERFINFO_AUTO_STOP();
        
    
    devassert(oggStatesInUse > 0);
    oggStatesInUse--;
    
    ogg->in_use = 0;

    oggListRemove(ogg);
}

void oggRewindDecoder(DecodeState *decode)
{
    OggState    *ogg = decode ? decode->codec_state : NULL;
    int result;

    if (!ogg || !decode->frequency || !ogg->in_use)
        return;

    PERFINFO_AUTO_START("oggRewindDecoder", 1);
        result = ov_raw_seek(&ogg->vf,0);
    PERFINFO_AUTO_STOP();
    if (result)
        oggResetDecoder(decode);
    decode->decode_count = result ? -1 : 0;
}

bool oggToPcm(SoundFileInfo *info)
{
    DecodeState    decode;
    bool success = false;

    if (!info || info->pcm_data)
        return false;

    PERFINFO_AUTO_START("oggToPcm", 1);
        ZeroStruct(&decode);
        decode.info = info;
        if (!oggInitDecoder(&decode) || decode.pcm_len <= 0)
            goto cleanup;
        decode.decode_buffer = malloc(decode.pcm_len);

        if(decode.decode_buffer) {
            decode.decode_len = decode.pcm_len;
            // Streamed prefixes are playable, but a PCM cache must be complete.
            if (oggDecode(&decode) == decode.pcm_len) {
                info->pcm_len = decode.pcm_len;
                info->pcm_data = decode.decode_buffer;
                info->frequency = decode.frequency;
                info->num_channels = decode.num_channels;
                decode.decode_buffer = NULL;
                success = true;
            }
        }

cleanup:
        oggResetDecoder(&decode);
        SAFE_FREE(decode.decode_buffer);
        SAFE_FREE(decode.codec_state);

    PERFINFO_AUTO_STOP();

    return success;
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
