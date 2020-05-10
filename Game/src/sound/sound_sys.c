#include "win/win_init.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dsound.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/file.h>
#include "ogg_decode.h"
#include "sound/sound_sys.h"
#include "assert.h"
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/MemoryMonitor.h>
#include <utilitieslib/stdtypes.h>
#include "player/player.h"
#include "entity/entity.h"
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/strings_opt.h>
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/osdependent.h>

void testCompressor();

#define BUFFER_FILL_SIZE 32768
static const char DSOUND_MEMMONITOR_NAME[] = "dsoundbuffer";

typedef struct DSoundState
{
    IDirectSoundBuffer        *buffer;
    IDirectSound3DBuffer    *buffer3d;
    int                        length;
    int                        frequency;
    int                        num_channels;
    int                        in_use;

    int                        last_write_pos;
    int                        bytes_left; // count when finishing non-looped sound
    int                        saved_play_cursor;
    F32                        pan;
    Vec3                    pos;
    Vec3                    lastPos;
    F32                        volRatio;
    F32                        volDb;
    DSParams                params;
} DSoundState;

typedef struct PlayStream
{
    SoundFileInfo    infoCopy;
    DecodeState        decode;
    DSoundState        dsound;
    int                streaming;
    int                loop;
} PlayStream;

#define MIN_VOLUME_DB (-10000)
#define MAX_VOLUME_DB (0)

static PlayStream                channels[MAX_EVER_SOUND_CHANNELS];
static CRITICAL_SECTION            sound_decoding_cs;
static LPDIRECTSOUND            gpds;
static LPDIRECTSOUNDBUFFER        lpDSb;
AudioState                        g_audio_state;

static struct {
    SoundFileInfo**                files;
    S32                            count;
    S32                            maxCount;
} sound_files;
    
static IDirectSound3DListener*    theListener;    //the 3d listener that corresponds to the player's position and speed
static S32                        timer;
static F32                        volumeScale;

#define DS_ASSERT(x) //assert(x)


void sndSysPositionListener(Mat4 mat,Vec3 vel)
{
    if (!theListener) {
        g_audio_state.surroundFailed=1;
        g_audio_state.uisurround=0;
        return;
    }
    if (g_audio_state.surround)
    {
        PERFINFO_AUTO_START("IDS3DL_SetPosition",1);
            IDirectSound3DListener_SetPosition(theListener,mat[3][0],mat[3][1],mat[3][2],DS3D_IMMEDIATE);
        PERFINFO_AUTO_STOP_START("IDS3DL_SetOrientation",1);
            IDirectSound3DListener_SetOrientation(theListener,mat[2][0],-mat[2][1],mat[2][2],mat[1][0],-mat[1][1],mat[1][2],DS3D_IMMEDIATE);
        PERFINFO_AUTO_STOP_START("IDS3DL_SetVelocity",1);
            IDirectSound3DListener_SetVelocity(theListener,vel[0],vel[1],vel[2],DS3D_IMMEDIATE);
        PERFINFO_AUTO_STOP();
    }
}

//apparently doing things this way has the potential to make sounds suck
//so we don't use it anymore - JW
void sndSysCommitSettings()
{
    if (!theListener) {
        g_audio_state.surroundFailed=1;
        g_audio_state.uisurround=0;
        return;
    }
    PERFINFO_AUTO_START("IDS3DL_CommitDeferredSettings",1);
        IDirectSound3DListener_CommitDeferredSettings(theListener);
    PERFINFO_AUTO_STOP();
}

// Try to avoid allocing / freeing directsound buffers, instead shuffle them around if possible
static int matchBufferType(int play_idx)
{
    int            i,idx;
    PlayStream *play,*match=0,*curr=&channels[play_idx],*empty=0;
    DSoundState    *dsound,dsound_temp;
    DecodeState    *decode = &curr->decode;

    for(i=0;i<g_audio_state.maxSoundChannels;i++) // 
    {
        idx = (i + play_idx) % g_audio_state.maxSoundChannels;
        play = &channels[idx];
        dsound = &play->dsound;

        if (!dsound->in_use || play->streaming)
        {
            if (!dsound->in_use && !empty)
                empty = play;
            continue;
        }
        if (dsound->num_channels == decode->num_channels && dsound->frequency == decode->frequency)
        {
            match = play;
            break;
        }
    }
    if (!match)
        return 0;
    if (match != curr)
    {
        dsound_temp = curr->dsound;
        curr->dsound = match->dsound;
        match->dsound = dsound_temp;
    }
    return 1;
}

static IDirectSoundBuffer *sndSysCreateSoundBuffer(int dwBufSize, int dwFreq, int num_channels)
{
    PCMWAVEFORMAT        pcmwf;
    DSBUFFERDESC        dsbdesc;
    IDirectSoundBuffer    *dwBuf=0;

    // Set up wave format structure.
    
    ZeroStruct(&pcmwf);
    pcmwf.wf.wFormatTag         = WAVE_FORMAT_PCM;
    pcmwf.wf.nChannels          = num_channels;
    pcmwf.wf.nSamplesPerSec     = dwFreq;
    pcmwf.wf.nBlockAlign        = (WORD)2 * num_channels;
    pcmwf.wf.nAvgBytesPerSec    = pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
    pcmwf.wBitsPerSample        = (WORD)16;

    // Set up DSBUFFERDESC structure.
    
    ZeroStruct(&dsbdesc);
    dsbdesc.dwSize              = sizeof(dsbdesc);
    dsbdesc.dwFlags             = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLVOLUME | DSBCAPS_GETCURRENTPOSITION2;
    if (g_audio_state.surround)
    {
        dsbdesc.dwFlags            |= DSBCAPS_CTRL3D;
        dsbdesc.guid3DAlgorithm    = g_audio_state.software ? DS3DALG_NO_VIRTUALIZATION/*does a software 3d pan*/ : DS3DALG_HRTF_FULL;
    }
    else
    {
        dsbdesc.dwFlags            |= DSBCAPS_CTRLPAN;
    }
    
    if (g_audio_state.software)
    {
        dsbdesc.dwFlags |= DSBCAPS_LOCDEFER;
    }
    
    dsbdesc.dwBufferBytes       = dwBufSize; 
    dsbdesc.lpwfxFormat         = (LPWAVEFORMATEX)&pcmwf;

    PERFINFO_AUTO_START("IDS_CreateSoundBuffer", 1);
        IDirectSound_CreateSoundBuffer(gpds,&dsbdesc,&dwBuf,NULL);
    PERFINFO_AUTO_STOP();
    return dwBuf;
}

static void soundInfoRefCount(SoundFileInfo *info,int amt)
{
    static int use_count;

    info->last_used = ++use_count;
    info->ref_count += amt;
    assert(info->ref_count >= 0);
    assert(info->ref_count <= g_audio_state.maxSoundChannels);
}

static void checkPcmCache(SoundFileInfo *info)
{
    SoundFileInfo    *candidate,*best;
    int                oldest = 0x7fffffff,total_bytes,i,cache_size = game_state.maxOggToPcm;
    int                maxPcmCache = (int)(game_state.maxPcmCacheMB * 1024 * 1024);

    cache_size = (cache_size * info->recent_play_count)/5 + cache_size;
    if (info->pcm_data || info->length > cache_size)
        return;
    if( !oggToPcm(info) )
        return; // failed to allocate pcm data

    soundInfoRefCount(info,0);
    for(;;)
    {
        total_bytes = 0;
        best = 0;
        oldest = 0x7fffffff;
        for(i=0;i<sound_files.count;i++)
        {
            candidate = sound_files.files[i];
            if (!candidate->pcm_data)
                continue;
            total_bytes += candidate->pcm_len;
            if (!candidate->ref_count && candidate->last_used < oldest)
            {
                oldest = candidate->last_used;
                best = candidate;
            }
        }
        if (total_bytes < maxPcmCache)
            break;
        if (best)
        {
            free(best->pcm_data);
            best->pcm_data = 0;
            best->pcm_len = best->frequency = best->num_channels = 0;
        }
        else
            break;
    }
}

typedef struct VolumeTimer {
    int                    minDB;
    const char*         name;
    #if USE_NEW_TIMING_CODE
        PerfInfoStaticData* piStatic;
    #else
        PerformanceInfo* piStatic;
    #endif
} VolumeTimer;

static VolumeTimer volumeTimers[] = {
    #define SET(x) { x, #x }
    SET(0),
    SET(-100),
    SET(-200),
    SET(-300),
    SET(-400),
    SET(-500),
    SET(-600),
    SET(-700),
    SET(-800),
    SET(-900),
    SET(-1000),
    SET(-2000),
    SET(-3000),
    SET(-4000),
    SET(-5000),
    SET(-6000),
    SET(-7000),
    SET(-8000),
    SET(-9000),
    SET(-10000),
};

static int getVolumeTimer(int volume){
    int i;
    for(i = 0; i < ARRAY_SIZE(volumeTimers) - 1; i++){
        if(volume >= volumeTimers[i].minDB){
            break;
        }
    }
    return i;
}

static void playStart(PlayStream *play)
{
    if (play->streaming)
        return;
    soundInfoRefCount(play->decode.info,1);
    PERFINFO_AUTO_START("IDSB_Play", 1);
    {
        int i;
        int volume;
        int ret;

        PERFINFO_AUTO_START("IDSB_GetVolume", 1);
            ret = IDirectSoundBuffer_GetVolume(play->dsound.buffer, &volume);
            DS_ASSERT(ret == DS_OK);
        PERFINFO_AUTO_STOP();
        
        i = getVolumeTimer(volume);

        PERFINFO_AUTO_START_STATIC(volumeTimers[i].name, &volumeTimers[i].piStatic, 1);
            ret = IDirectSoundBuffer_Play(play->dsound.buffer,0,0,DSBPLAY_LOOPING /*| DSBPLAY_LOCSOFTWARE*/);
            DS_ASSERT(ret == DS_OK);
        PERFINFO_AUTO_STOP();
    }
    PERFINFO_AUTO_STOP();
    play->streaming = 1;
    //printf("playing: %d\n", sndSysPlaying(play - channels));

    #if 0
        int status;
        IDirectSoundBuffer_GetStatus(play->dsound.buffer, &status);
        printf("status: %d\n", status);
    #endif
}

static void clearBuffer(PlayStream *play)
{
    int        rval;
    DSoundState    *dsound = &play->dsound;
    DecodeState    *decode = &play->decode;
    void        *pData1,*pData2;
    DWORD        dwData1Size,dwData2Size;

    PERFINFO_AUTO_START("IDSB_Lock", 1);
        rval = IDirectSoundBuffer_Lock(    dsound->buffer,
                                        0,
                                        decode->decode_len * 4,
                                        &pData1,
                                        &dwData1Size,
                                        &pData2,
                                        &dwData2Size,
                                        0);//DSBLOCK_FROMWRITECURSOR
    PERFINFO_AUTO_STOP();
    
    if (rval != DS_OK)
    {
        PERFINFO_AUTO_START("IDSB_Stop", 1);
            rval = IDirectSoundBuffer_Stop(play->dsound.buffer);
        PERFINFO_AUTO_STOP();
        return;
    }
    
    if (dwData1Size > 0)
        memset(pData1,0,dwData1Size);
    if (dwData2Size > 0)
        memset(pData2,0,dwData2Size);
        
    PERFINFO_AUTO_START("IDSB_Unlock", 1);
        rval = IDirectSoundBuffer_Unlock(dsound->buffer,pData1,dwData1Size,pData2,dwData2Size);
    PERFINFO_AUTO_STOP();
    
    DS_ASSERT(rval == DS_OK);
}

static void playStop(PlayStream *play,int soft_stop)
{
    if (!play->streaming)
        return;
    soundInfoRefCount(play->decode.info,-1);
    if (1 || soft_stop)
    {
        PERFINFO_AUTO_START("clearBuffer", 1);
            clearBuffer(play);
        PERFINFO_AUTO_STOP();
    }
    else
    {
        int ret;
        PERFINFO_AUTO_START("IDSB_Stop", 1);
            ret = IDirectSoundBuffer_Stop(play->dsound.buffer);
            DS_ASSERT(ret == DS_OK);
        PERFINFO_AUTO_STOP();
    }
    play->streaming = 0;
    play->dsound.volRatio = 0;
}

void sndSysCleanChannel(int channel)
{
    PlayStream *play = &channels[channel];
    DecodeState    *decode = &play->decode;
    DSoundState    *dsound = &play->dsound;
    int ret;
    
    if (dsound->buffer)
    {
        PERFINFO_AUTO_START("IDSB_Release", 1);
            ret = IDirectSoundBuffer_Release(dsound->buffer);
            DS_ASSERT(ret == DS_OK);
        PERFINFO_AUTO_STOP();
        dsound->buffer = 0;
        memMonitorTrackUserMemory(DSOUND_MEMMONITOR_NAME, 1, MOT_FREE, decode->decode_len * 4);
    }
    if (dsound->buffer3d)
    {
        PERFINFO_AUTO_START("IDS3DB_Release", 1);
            IDirectSound3DBuffer_Release(dsound->buffer3d);
        PERFINFO_AUTO_STOP();
        dsound->buffer3d = 0;
    }
}

static void EnterDecodingCS()
{
    EnterCriticalSection(&sound_decoding_cs);
}

static void LeaveDecodingCS()
{
    LeaveCriticalSection(&sound_decoding_cs);
}

int sndSysChannelGets3D(int channel)
{
    return channels[channel].dsound.params.gets3D;
}

static F32 volumeDbToRatio(F32 db)
{
    db = MINMAX(db, MIN_VOLUME_DB, MAX_VOLUME_DB);
    
    return powf(10, db / 2000.0);
}

static int volumeRatioToDb(F32 volRatio)
{
    static F32 minVolRatio;

    int db;

    if(minVolRatio == 0.0)
    {
        minVolRatio = volumeDbToRatio(MIN_VOLUME_DB);
    }
    
    if(volRatio <= minVolRatio)
    {
        db = MIN_VOLUME_DB;
    }
    else
    {
        volRatio= MINMAX(volRatio, 0.0, 1.0);

        db = 2000.0 * log10(volRatio);

        db = MINMAX(db, MIN_VOLUME_DB, MAX_VOLUME_DB);
    }

    return db;    
}

static void sndSysSetVolume(DSoundState* dsound, int db)
{
    if(!dsound->buffer)
    {
        return;
    }
    
    db = MINMAX(db, MIN_VOLUME_DB, MAX_VOLUME_DB);
    
    PERFINFO_AUTO_START("IDSB_SetVolume", 1);
    {
        int i = getVolumeTimer(db);
        PERFINFO_AUTO_START_STATIC(volumeTimers[i].name, &volumeTimers[i].piStatic, 1);
        {
            int ret;

            ret = IDirectSoundBuffer_SetVolume(dsound->buffer, db);
            DS_ASSERT(ret == DS_OK);
        }
        PERFINFO_AUTO_STOP();
    }
    PERFINFO_AUTO_STOP();
}

static int sndSetupDirectSound(int channel, DSParams* params)
{
    PlayStream *play = &channels[channel];
    DecodeState    *decode = &play->decode;
    DSoundState    *dsound = &play->dsound;
    int ret;
    
    // Setup DSound.
    
    sndSysCleanChannel(channel);
    dsound->buffer = sndSysCreateSoundBuffer(decode->decode_len * 4,decode->frequency,decode->num_channels);
    if (!dsound->buffer)
    {
        dsound->in_use = 0;
        return 0;
    }
    
    memMonitorTrackUserMemory(DSOUND_MEMMONITOR_NAME, 1, MOT_ALLOC, decode->decode_len * 4);
    
    if (g_audio_state.surround)
    {
        assert(!dsound->buffer3d);
        
        PERFINFO_AUTO_START("IDS_QueryInterface", 1);
            IDirectSound_QueryInterface(dsound->buffer,
                                        (const IID * const)&IID_IDirectSound3DBuffer8,
                                        (LPVOID *)&dsound->buffer3d);
        PERFINFO_AUTO_STOP();
    }
    else
    {
        dsound->buffer3d = 0;
    }

    dsound->length                = decode->decode_len * 4;
    dsound->num_channels        = decode->num_channels;
    dsound->frequency            = decode->frequency;
    dsound->in_use                = 1;
    dsound->pan                    = -1;
    zeroVec3(dsound->pos);
    zeroVec3(dsound->lastPos);
    dsound->volRatio            = -1;
    dsound->volDb                = MIN_VOLUME_DB;
    dsound->last_write_pos        = 0;
    dsound->bytes_left            = 0;
    dsound->saved_play_cursor    = 0;
    
    sndSysSetVolume(dsound, dsound->volDb);
    
    PERFINFO_AUTO_START("IDSB_SetCurrentPosition", 1);
        ret = IDirectSoundBuffer_SetCurrentPosition(dsound->buffer, 0);
        DS_ASSERT(ret == DS_OK);
    PERFINFO_AUTO_STOP();

    if (g_audio_state.surround)
    {
        dsound->params = *params;
        
        PERFINFO_AUTO_START("IDS3DB_SetMinDistance", 1);
            IDirectSound3DBuffer_SetMinDistance(dsound->buffer3d, params->innerRadius, DS3D_IMMEDIATE);
        PERFINFO_AUTO_STOP();

        PERFINFO_AUTO_START("IDS3DB_SetMaxDistance", 1);
            IDirectSound3DBuffer_SetMaxDistance(dsound->buffer3d, params->outerRadius, DS3D_IMMEDIATE);
        PERFINFO_AUTO_STOP();

        if (params->gets3D) {
            PERFINFO_AUTO_START("IDS3DB_SetMode - 3D", 1);
                IDirectSound3DBuffer_SetMode(dsound->buffer3d, DS3DMODE_NORMAL, DS3D_IMMEDIATE);
            PERFINFO_AUTO_STOP();
        } else {
            PERFINFO_AUTO_START("IDS3DB_SetMode", 1);
                IDirectSound3DBuffer_SetMode(dsound->buffer3d, DS3DMODE_DISABLE, DS3D_IMMEDIATE);
            PERFINFO_AUTO_STOP();
        }
    }

    dsound->last_write_pos = 0;
    
    return 1;
}

static struct {
    #if USE_NEW_TIMING_CODE
        PerfInfoStaticData*    timers[ARRAY_SIZE(channels)];
    #else
        PerformanceInfo*    timers[ARRAY_SIZE(channels)];
    #endif
    char*                names[ARRAY_SIZE(channels)];
}* channelTimers;

static void initChannelTimers()
{
    int i;
    
    if(channelTimers){
        return;
    }
    
    channelTimers = calloc(1, sizeof(*channelTimers));
    
    for(i = 0; i < ARRAY_SIZE(channelTimers->names); i++){
        char buffer[100];
        STR_COMBINE_BEGIN(buffer);
        STR_COMBINE_CAT("Channel ");
        STR_COMBINE_CAT_D(i);
        STR_COMBINE_END();
        channelTimers->names[i] = strdup(buffer);
    }
}

static PlayStream *playStreamCreate(SoundFileInfo *info,int channel,DSParams * params)
{
    PlayStream*        play = &channels[channel];
    DecodeState*    decode = &play->decode;
    DSoundState*    dsound = &play->dsound;
    CodecFuncs*        codec;
    
    play->infoCopy = *info;
    
    PERFINFO_AUTO_START("playStreamCreate", 1);
    
    PERFINFO_RUN(
        initChannelTimers();
        
        PERFINFO_AUTO_START_STATIC(channelTimers->names[channel], &channelTimers->timers[channel], 1);
    )

    EnterDecodingCS();
    
    PERFINFO_AUTO_START("checkPcmCache", 1);
        checkPcmCache(info);
    PERFINFO_AUTO_STOP();
    
    if (info->pcm_len)
    {
        codec = &pcm_funcs;
    }
    else
    {
        codec = &ogg_funcs;
    }

    PERFINFO_AUTO_START("playStop", 1);
        playStop(play,1);
    PERFINFO_AUTO_STOP();

    // Setup decoder.
    
    if (!decode->decode_buffer)
    {
        decode->decode_len = BUFFER_FILL_SIZE;
        decode->decode_buffer = calloc(1, BUFFER_FILL_SIZE);
    }
    if (dsound->in_use && decode->codec)
    {
        // Use old codec to free old data.
        
        PERFINFO_AUTO_START("reset", 1);
            decode->codec->reset(decode); 
        PERFINFO_AUTO_STOP();
    }
    decode->info = info;

    PERFINFO_AUTO_START("init", 1);
        codec->init(decode);
    PERFINFO_AUTO_STOP();

    decode->codec = codec;
    
    if(!sndSetupDirectSound(channel, params))
    {
        assert(!dsound->in_use);
        
        PERFINFO_AUTO_START("reset", 1);
            codec->reset(decode);
        PERFINFO_AUTO_STOP();

        play = NULL;
    }

    LeaveDecodingCS();
    PERFINFO_AUTO_STOP(); // Matches "Channel #".
    PERFINFO_AUTO_STOP(); // Matches "playStreamCreate".
    
    return play;
}

static void copyDataToDSound(PlayStream *play)
{
    void        *pData1,*pData2;
    DWORD        dwData1Size,dwData2Size;
    HRESULT        rval;
    DSoundState    *dsound = &play->dsound;
    DecodeState    *decode = &play->decode;

    if (decode->decode_count <= 0)
        return;
    
    PERFINFO_AUTO_START("IDSB_Lock", 1);
        rval = IDirectSoundBuffer_Lock(dsound->buffer,dsound->last_write_pos,decode->decode_count,&pData1,&dwData1Size,&pData2,&dwData2Size,0);//DSBLOCK_FROMWRITECURSOR
    PERFINFO_AUTO_STOP();

    if(rval == DS_OK)
    {
        if (dwData1Size > 0) 
            memcpy(pData1,decode->decode_buffer,dwData1Size);
        if (dwData2Size > 0) 
            memcpy(pData2,decode->decode_buffer+dwData1Size,dwData2Size);
        
        PERFINFO_AUTO_START("IDSB_Unlock", 1);
            rval = IDirectSoundBuffer_Unlock(dsound->buffer,pData1,dwData1Size,pData2,dwData2Size);
        PERFINFO_AUTO_STOP();

        DS_ASSERT(rval == DS_OK);
    }
    
    dsound->last_write_pos = (dsound->last_write_pos + decode->decode_count) % dsound->length;
}

static int getCircularCount(DSoundState *dsound,int play_cursor,int write_cursor)
{
    int        t;

    t = play_cursor - write_cursor;
    if (t < 0)
        t += dsound->length;
    return t;
}

static int getFreeBytes(PlayStream *play)
{
    int            dwPlayCursor;
    DSoundState    *dsound = &play->dsound;
    int            ret;

    PERFINFO_AUTO_START("IDSB_GetCurrentPosition", 1);
        ret = IDirectSoundBuffer_GetCurrentPosition(dsound->buffer,&dwPlayCursor,0);
        DS_ASSERT(ret == DS_OK);
    PERFINFO_AUTO_STOP();

    return getCircularCount(dsound,dwPlayCursor,dsound->last_write_pos);
}

static void fillBuffer(PlayStream *play)
{
    DecodeState*    decode = &play->decode;
    CodecFuncs*        codec = decode->codec;
    S32                decodeLen;

    PERFINFO_AUTO_START("fillBuffer", 1);
    
        PERFINFO_AUTO_START("Decode Sound", 1);
            decodeLen = codec->decode(decode);
        PERFINFO_AUTO_STOP();
        
        if (decodeLen >= 0 && decodeLen < decode->decode_len)
        {
            PERFINFO_AUTO_START("copyDataToDSound1", 1);
                copyDataToDSound(play);
            PERFINFO_AUTO_STOP();
            
            if (play->loop)
            {
                PERFINFO_AUTO_START("rewind", 1);
                    codec->rewind(decode);
                PERFINFO_AUTO_STOP_START("decode", 1);
                    decodeLen = codec->decode(decode);
                PERFINFO_AUTO_STOP();
            }
            else
            {
                S32                dwPlayCursor;
                S32                t;
                DSoundState*    dsound = &play->dsound;
                S32                ret;

                //oggResetDecoder(decode);
                PERFINFO_AUTO_START("IDSB_GetCurrentPosition", 1);
                    ret = IDirectSoundBuffer_GetCurrentPosition(dsound->buffer,&dwPlayCursor, 0);
                    DS_ASSERT(ret == DS_OK);
                PERFINFO_AUTO_STOP();

                if (!dsound->bytes_left)
                {
                    dsound->bytes_left = getCircularCount(dsound,dsound->last_write_pos,dwPlayCursor);
                    dsound->saved_play_cursor = dwPlayCursor;
                }
                t = getCircularCount(dsound,dwPlayCursor,dsound->saved_play_cursor);
                if (t > dsound->bytes_left)
                {
                    playStop(play,1);
                    PERFINFO_AUTO_STOP();
                    return;
                }
                memset(decode->decode_buffer,0,decode->decode_len);
                decode->decode_count = decode->decode_len;
            }
        }
        PERFINFO_AUTO_START("copyDataToDSound2", 1);
            copyDataToDSound(play);
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();

    if(decodeLen < 0)
    {
        // The decoder crashed, so kill the sound.
        
        playStop(play, 0);
    }
}

static void sndSysUpdateChannelVolume(DSoundState* dsound)
{
    S32 db = volumeRatioToDb(volumeScale * volumeDbToRatio(dsound->volDb));
    
    sndSysSetVolume(dsound, db);
}

static void sndSysUpdateVolumeScale()
{
    static U32 lastTime;
    U32 curTime = timeGetTime();
    U32 timeDelta = curTime - lastTime;
    int curActive = GetForegroundWindow() == hwnd;
    
    lastTime = curTime;

    if(curActive)
    {
        // Cap the maximum increase in volume.
        
        F32 delta;
        
        timeDelta = min(timeDelta, 100);
        
        delta = (F32)timeDelta / 1000.0f;
        
        if(volumeScale < 0.1f)
        {
            if(0.1f - volumeScale > delta * 0.1f)
            {
                volumeScale += delta * 0.1f;
                delta = 0;
            }
            else
            {
                delta -= 10.0f * (0.1f - volumeScale);
                volumeScale = 0.1f;
            }
        }

        if(volumeScale >= 0.1f)
        {
            volumeScale += delta;
        }
    }
    else
    {
        F32 delta = (F32)timeDelta / 1000.0f;
        
        if(volumeScale > 0.1f)
        {
            if(delta >= volumeScale - 0.1f)
            {
                delta -= volumeScale - 0.1f;
                volumeScale = 0.1f;
            }
            else
            {
                volumeScale -= delta;
                delta = 0;
            }
        }

        if(volumeScale <= 0.1f)
        {
            volumeScale -= delta * 0.1f;
        }
    }

    volumeScale = MINMAX(volumeScale, 0.0f, 1.0f);

#if AUDIO_DEBUG
    // fpe for test, let sounds play in background, eg for playAllSounds testing
    if( !curActive ) {
        extern int gPAS_CurSoundIndex;
        if(gPAS_CurSoundIndex != -1)
            volumeScale = 0.04f;
    }
#endif
}

static void sndSysResetRecentPlayCount()
{
    if (timerElapsed(timer) > 60)
    {
        int i;
        for(i=0;i<sound_files.count;i++)
            sound_files.files[i]->recent_play_count = 0;
        timerStart(timer);
    }
}

void sndSysTick(void)
{
    PlayStream* play;
    int            i;
    
    if (g_audio_state.noaudio)
        return;

    PERFINFO_AUTO_START("soundSystemTick", 1);

    EnterDecodingCS();
    
    sndSysUpdateVolumeScale();

    for(i=0;i<g_audio_state.maxSoundChannels;i++)
    {
        play = &channels[i];
        
        if (!play->streaming)
        {
            if(play->dsound.buffer && play->dsound.in_use)
            {
                sndSysUpdateChannelVolume(&play->dsound);
            }
                            
            continue;
        }
        
        if (getFreeBytes(play) > play->decode.decode_len * 2)
        {
            fillBuffer(play);
        }

        sndSysUpdateChannelVolume(&play->dsound);
    }
    
    sndSysResetRecentPlayCount();
    
    LeaveDecodingCS();
    
    PERFINFO_AUTO_STOP();
} 

static void soundSystemStop()
{
    timerFree(timer);
    timer=-1;
}

static void soundSystemStart()
{
    timer = timerAlloc();
    InitializeCriticalSection(&sound_decoding_cs);
    testCompressor();
}

void sndSysUpdateChannel(int channel,F32 volRatio,Vec3 pos,F32 pan,int doppler)
{
    PlayStream* play = &channels[channel];    
    DSoundState    *dsound = &play->dsound;
    static int time,lastTime;
    F64 timeLapse;
    F32 db;
    lastTime=time;
    time=clock();
    timeLapse=(F64)(time-lastTime)/CLK_TCK;
    if (timeLapse<=0)
        timeLapse=1;
        
    if(    !dsound->in_use ||
        (    dsound->volRatio == volRatio && 
            (    g_audio_state.surround &&
                sameVec3(dsound->pos,pos) &&
                sameVec3(dsound->pos,dsound->lastPos)
                ||
                !g_audio_state.surround &&
                dsound->pan==pan
            )
        )
    )
    {
        return;
    }

    PERFINFO_AUTO_START("sndSysUpdateChannel", 1);

        dsound->volRatio = volRatio;
        dsound->pan = pan;
        copyVec3(dsound->pos,dsound->lastPos);
        copyVec3(pos,dsound->pos);

        //printf("set volume & pan to %f, %f\n", volume, pan);

        // input is 0..255
        /* MAX_VOLUME_DB = full volume, MIN_VOLUME_DB = silent */

        dsound->volDb = volumeRatioToDb(volRatio);
        db = volumeRatioToDb(volRatio * volumeScale);

        sndSysSetVolume(dsound, db);

        if (g_audio_state.surround)
        {
            if(dsound->buffer3d)
            {
                int ret;
                PERFINFO_AUTO_START("IDSB_SetPosition", 1);
                    ret = IDirectSound3DBuffer_SetPosition(dsound->buffer3d, pos[0], pos[1], pos[2], DS3D_IMMEDIATE);
                    DS_ASSERT(ret == DS_OK);
                PERFINFO_AUTO_STOP();
                        
                if (doppler)
                {
                    Vec3 diff;
                    
                    subVec3(dsound->pos, dsound->lastPos, diff);
                    scaleVec3(diff, 1.0 / timeLapse, diff);
                    
                    PERFINFO_AUTO_START("IDSB_SetVelocity", 1);
                        ret = IDirectSound3DBuffer_SetVelocity(dsound->buffer3d, diff[0], diff[1], diff[2], DS3D_IMMEDIATE);
                        DS_ASSERT(ret == DS_OK);
                    PERFINFO_AUTO_STOP();
                }
            }
        }
        else
        {
            int ret;
            PERFINFO_AUTO_START("IDSB_SetPan", 1);
                // pan input value is [-1,1].
                // DirectSound range is [-10k,10k], but [-2k,2k] sounds more realistic. 
                // Nothing is ever really ONLY in one ear or the other.
                
                pan = pan * 2000;

                pan = MINMAX(pan, -10000, 10000);
                
                ret = IDirectSoundBuffer_SetPan(dsound->buffer, pan);
                DS_ASSERT(ret == DS_OK);
            PERFINFO_AUTO_STOP();
        }
        //printf("set volume & pan to %f, %f\n", volume, pan);
    PERFINFO_AUTO_STOP();
}

void sndSysPlay(int channel,void *sound_data,F32 volRatio,Vec3 pos,F32 pan,int loop,DSParams * params)
{
    SoundFileInfo*    info = sound_data;
    PlayStream*        play;

    if (g_audio_state.noaudio || !info || !info->data)
    {
        return;
    }
        
    if(loop)
    {
        PERFINFO_AUTO_START("sndSysPlay - loop", 1);
    }
    else
    {
        PERFINFO_AUTO_START("sndSysPlay - no loop", 1);
    }

        info->recent_play_count++;
        
        play = playStreamCreate(sound_data,channel,params);

        if (play)
        {
            copyVec3(pos,play->dsound.pos);
            play->loop = loop;
            EnterDecodingCS();
            fillBuffer(play);
            playStart(play);
            sndSysUpdateChannel(channel,volRatio,pos,pan,0);
            LeaveDecodingCS();
        }
        
    PERFINFO_AUTO_STOP();
}

void sndSysRewind(int channel,void *sound_data,DSParams * params)
{
    DSoundState    *dsound = &channels[channel].dsound;

    sndSysPlay(channel,sound_data,dsound->volRatio,dsound->pos,dsound->pan,channels[channel].loop,params);
}

void sndSysStop(int channel)
{
    PlayStream    *play = &channels[channel];

    if (!play->dsound.in_use)
        return;
    EnterDecodingCS();
    playStop(play,0);
    LeaveDecodingCS();
}

int sndSysPlaying(int channel)
{
    int            status;
    DSoundState    *dsound = &channels[channel].dsound;
    int            ret;

    if (!dsound->in_use || !channels[channel].streaming)
        return 0;
    
    PERFINFO_AUTO_START("IDSB_GetStatus", 1);
        ret = IDirectSoundBuffer_GetStatus(dsound->buffer,&status);
        DS_ASSERT(ret == DS_OK);
    PERFINFO_AUTO_STOP();

    if (status & DSBSTATUS_PLAYING)
        return 1;
    else
        return 0;
}

//creates the listener object that will be used for all 3d sound processing
static void createTheListener(LPDIRECTSOUNDBUFFER lpdsbPrimary) {
    HRESULT hr;

    // DirectSound is deprecated, and apparently no longer correctly positions audio as of Vista.
    //  Disabled 3D sound for these OSes until we can port to a supported API.
    if (IsUsingVistaOrLater())
        return;

    hr = IDirectSound_QueryInterface(
        lpdsbPrimary,
        (const IID * const)&IID_IDirectSound3DListener8,
        (LPVOID *)(&theListener));
    assert(SUCCEEDED(hr));
    IDirectSoundBuffer_Release(lpdsbPrimary);
}

void sndSysInit()
{
    DSBUFFERDESC            dsbdesc;
    WAVEFORMATEX            wfm;

    if (g_audio_state.noaudio)
        return;
    if(FAILED(DirectSoundCreate(NULL,&gpds,NULL)))
    {
        g_audio_state.noaudio = 1;
        Errorf("Couldn't start DirectSound, continuing without audio");
        return;
    }
    else
    {
        PERFINFO_AUTO_START("IDS_SetCooperativeLevel", 1);
            if(FAILED(IDirectSound_SetCooperativeLevel(gpds, hwnd, DSSCL_PRIORITY)))
            {
                FatalErrorf("Couldn't set DirectSound cooperative level!");
            }
        PERFINFO_AUTO_STOP();

        ZeroStruct(&dsbdesc);
        dsbdesc.dwSize = sizeof(dsbdesc);
        dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
        if (g_audio_state.surround)
            dsbdesc.dwFlags |= DSBCAPS_CTRL3D;
        dsbdesc.dwBufferBytes = 0;
        dsbdesc.lpwfxFormat = NULL;

        ZeroStruct(&wfm);
        wfm.wFormatTag = WAVE_FORMAT_PCM;
        wfm.nChannels = 2;
        wfm.nSamplesPerSec = 44100;
        wfm.wBitsPerSample = 16;
        wfm.nBlockAlign = wfm.wBitsPerSample / 8 * wfm.nChannels;
        wfm.nAvgBytesPerSec = wfm.nSamplesPerSec * wfm.nBlockAlign;

        PERFINFO_AUTO_START("IDS_CreateSoundBuffer", 1);
            if(FAILED(IDirectSound_CreateSoundBuffer(gpds,&dsbdesc,&lpDSb,NULL)))
            {
                FatalErrorf("Couldn't create primary soundbuffer");
            }
        PERFINFO_AUTO_STOP_START("IDSB_SetFormat", 1);
            if(FAILED(IDirectSoundBuffer_SetFormat(lpDSb,&wfm)))
            {
                FatalErrorf("Couldn't set soundbuffer format");
            }
        PERFINFO_AUTO_STOP_START("IDSB_Play", 1);
            if(FAILED(IDirectSoundBuffer_Play(lpDSb,0,0,DSBPLAY_LOOPING )))
            {
                FatalErrorf("Couldn't set play on primary buffer");
            }
        PERFINFO_AUTO_STOP();
    }
    if (g_audio_state.surround)
    {
        createTheListener(lpDSb);
        if (!theListener)
        {
            g_audio_state.surroundFailed=1;    // this will cause us to restart the system without surround
            g_audio_state.uisurround=0;
        }
        else
        {
            PERFINFO_AUTO_START("IDSB_SetDopplerFactor", 1);
                IDirectSound3DListener_SetDopplerFactor(theListener,DS3D_DEFAULTDOPPLERFACTOR * 3,DS3D_IMMEDIATE);
            PERFINFO_AUTO_STOP_START("IDSB_SetRolloffFactor", 1);
                IDirectSound3DListener_SetRolloffFactor(theListener,0.001,DS3D_IMMEDIATE); // disable rolloff, essentially we ignore inner/outer radius and expect distance attenuation vol to be calculated outside of this module
            PERFINFO_AUTO_STOP_START("IDSB_SetDistanceFactor", 1);
                IDirectSound3DListener_SetDistanceFactor(theListener,0.3048,DS3D_IMMEDIATE); // convert from DS default unit of meter to our default unit of foot (only really matters for doppler)
            PERFINFO_AUTO_STOP();
        }
    }
    soundSystemStart();
}

void sndSysUninit()
{
    if(lpDSb)
    {
        PERFINFO_AUTO_START("IDSB_Release", 1);
            IDirectSoundBuffer_Release(lpDSb);
            lpDSb = NULL;
        PERFINFO_AUTO_STOP();
    }
    
    if (theListener)
    {
        PERFINFO_AUTO_START("IDS3DL_Release", 1);
            IDirectSound3DListener_Release(theListener);
            theListener = NULL;
        PERFINFO_AUTO_STOP();
    }

    if (gpds)
    {
        PERFINFO_AUTO_START("IDS_Release", 1);
            IDirectSound_Release(gpds);
            gpds = NULL;
        PERFINFO_AUTO_STOP();
    }
}

void sndSysExit()
{
    int i;
    
    soundSystemStop();

    for(i = 0; i < ARRAY_SIZE(channels); i++)
    {
        sndSysCleanChannel(i);
    }

    sndSysUninit();
}

void sndSysFree(void *sound_data)
{
    SoundFileInfo    *info = sound_data;
    int                i;

    if (!info)
        return;

    EnterDecodingCS();

    for(i=0;i<g_audio_state.maxSoundChannels;i++)
    {
        if (channels[i].decode.info == info)
        {
            playStop(&channels[i],0);
        }
    }
    for(i=0;i<sound_files.count;i++)
    {
        if (sound_files.files[i] == info)
        {
            CopyStructsFromOffset(sound_files.files + i, 1, sound_files.count - i - 1);
            sound_files.count--;
            break;
        }
    }
    SAFE_FREE(info->data);
    SAFE_FREE(info->pcm_data);
    SAFE_FREE(info);

    LeaveDecodingCS();
}

int sndSysBytes(void *sound_data)
{
    SoundFileInfo    *info = sound_data;

    if (!info)
        return 0;
    return info->length;
}

char *sndSysFormat(void *sound_data)
{
    SoundFileInfo    *info = sound_data;

    if (!info)
        return "";
    if (info->pcm_data)
        return "Pcm";
    return "Ogg";
}

void *sndSysLoad(char *fname)
{
    static U32        loadCount;
    
    SoundFileInfo*    info;
    
    PERFINFO_AUTO_START("sndSysLoad", 1);
        info = calloc(sizeof(*info),1);
        
        info->loadIndex = ++loadCount;

        if (g_audio_state.noaudio)
        {
            PERFINFO_AUTO_STOP();
            return info;
        }
        
        Strncpyt(info->name,fname);
        info->data = fileAlloc(fname,&info->length);
        dynArrayAddp(&sound_files.files,&sound_files.count,&sound_files.maxCount,info);
    PERFINFO_AUTO_STOP();
    
    return info;
}

void testCompressor()
{
#if 0
    void    *info1,*info2;
    int        i;

    info2 = sndSysLoad("sound/Ogg/EmoteTest/short 2button.ogg");
    info1 = sndSysLoad("sound/Ogg/EmoteTest/button15.ogg");
    for(;;)
    {
        for(i=0;i<8;i++)
        {
            sndSysPlay(i*2,info1,1,0,0);
            sndSysPlay(i*2+1,info2,1,0,0);
            Sleep(200);
        }
    }
    for(;;)
        Sleep(1000);
#endif
}
