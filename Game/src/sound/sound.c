#include "sound/sound.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <utilitieslib/utils/file.h>
#include <ctype.h>
#include "sound/sound_sys.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/memcheck.h>
#include "assert.h"
#include "sound_ambient.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/error.h>
#include "graphics/font.h"
#include "UI/uiGame.h"
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/FolderCache.h>
#include <dsound.h>
#include "player/player.h"
#include "entity/entity.h"
#include "entity/EntPlayer.h"
#include "entity/motion.h"
#include "auth/authUserData.h"
#include <utilitieslib/utils/strings_opt.h>
#include "graphics/vistray.h"
#include "graphics/gfxLoadScreens.h"
#include "LWC.h"

enum 
{
    eSoundFlags_Loop        = 1 << 0,
    eSoundFlags_Music        = 1 << 1,
};


typedef struct
{
    char* name;
    int max_instances;    // max instances that can play at once.  Default = 1 
    int cur_instances;    // set/maintained at runtime
    char** sounds;            // list of sounds in this group
} SndInstanceGroup;

typedef struct
{
    SndInstanceGroup**        instGroups;
} SndInstanceGroupList;

static TokenizerParseInfo ParseSndInstanceGroup[] =
{
    { "",                    TOK_STRUCTPARAM | TOK_STRING(SndInstanceGroup, name, 0)},
    { "maxInstances",        TOK_INT(SndInstanceGroup, max_instances, 1)                },
    { "Sounds",                TOK_STRINGARRAY(SndInstanceGroup, sounds)                },
    { "{",                    TOK_START,        0                                        },
    { "}",                    TOK_END,        0                                        },
    { "", 0, 0 }
};

static TokenizerParseInfo ParseSoundInstGroupList[] =
{
    { "Group",        TOK_STRUCT(SndInstanceGroupList, instGroups, ParseSndInstanceGroup) },
    { "", 0, 0 }
};
static SndInstanceGroupList sound_instance_groups = {0};

typedef struct
{
    char            path[80];        // eg "sound/ogg/praetoria/blah.ogg"
    void*            sound_data;
    F32                volRatio;        // set once at load time, default 1.0f
    U32                last_active;    // only set on sound playback thread
    U8                flags;            // set once at load time
    U8                cur_instances;    // only set on sound playback thread
    U8                nameIndex;        // for easy conversion of path to name (eg, path[nameIndex] = "blah.ogg"
    U8                pad;
    SndInstanceGroup* instGroup;    // the instance group this sound belongs to (if any)
} SoundNameInfo;

typedef struct
{
    Vec3            pos;
    F32                inner_radius;
    F32                outer_radius;
    F32                volRatio;
    F32                secTimeoutTotal;
    F32                secTimeoutCurrent;
    F32                secTimeoutBuffer; // time that can pass without servicing before we start fading
    S32                owner;
    U16                onplayer;
    U16                forceDimension;
    void*            def_tracker; // only used for editor debug
    F32                max_volume;
    F32                spatialVariance;
} SoundSphere;

typedef struct
{
    S32                index;
    F32                volRatio;
    SoundNameInfo*    name_info;
    SoundSphere*    spheres;
    S32                count;
    S32                sphere_max;
} SphereGroup;

struct {
    U32                    file_cache_bytes;
} sound_state;



typedef struct SoundPlaySpatialInfo SoundPlaySpatialInfo;

typedef struct SoundPlaySpatialInfo {
    SoundPlaySpatialInfo*    next;
    int                        desc_idx;
    int                        owner;
    U16                        flags;
    U16                        forceDimension;
    Vec3                    pos;
    F32                        inner;
    F32                        outer;
    F32                        volRatio;
    F32                        max_volume;
    void*                    def_tracker;
    F32                        spatialVariance;
} SoundPlaySpatialInfo;

typedef struct SoundPlaySpatialQueue {
    SoundPlaySpatialInfo*    head;
    SoundPlaySpatialInfo*    tail;
    _declspec(align(4)) U32 count; // ensure alignment for use across thread
} SoundPlaySpatialQueue;

CRITICAL_SECTION csSoundQueue;
CRITICAL_SECTION csCreateSoundPlaySpatialInfo;
CRITICAL_SECTION csSoundUpdateInfo;

static SoundPlaySpatialQueue queueMain;

static struct {
    struct {
        Mat4    mat;
        Vec3    vel;
        F32        decrementTimeout;
    } main, thread;
    
    S32        hasMat;

    struct {
        S32        stopAllSounds;
        S32        freeSoundCache;
    } flagsMain, flagsThread;
} soundUpdateInfo;

MP_DEFINE(SoundPlaySpatialInfo);

StashTable                sound_name_hashes;
static SoundNameInfo    *sound_name_infos;
static int                sound_name_count,sound_name_max;

static SphereGroup        sphereGroups[MAX_EVER_SOUND_CHANNELS];
static Mat4                head_mat;
static F32                volume_control[SOUND_VOLUME_COUNT] = {DEFAULT_VOLUME_FX, DEFAULT_VOLUME_MUSIC, DEFAULT_VOLUME_VO}; 

typedef struct VolumeDucking {
    F32 current;
    F32 target;
    F32 deltaPerSec;
    F32 defaultIntervals[2]; // duck down, up time intervals
    F32    defaultDuckVol;
} VolumeDucking;
static VolumeDucking volume_ducking[SOUND_VOLUME_COUNT];

#if AUDIO_DEBUG
const char* volumeTypeNames[SOUND_VOLUME_COUNT] = {"FX", "MUSIC","VO"};
#endif

Mat4                    glob_head_mat;

SoundEditorInfo    *sound_editor_infos;
int                sound_editor_count,sound_editor_max;

//for dynamic reloading
static bool bReloadDefs = false;
static bool bReloadSounds = false;

extern F32 server_time;

#if AUDIO_DEBUG
    static void sndUpdateDebug();
    static void LogSoundInstance(SoundNameInfo* sni, const char* event, int channel, int deltaCount);
    #define LOG_SOUND_INSTANCE(a,b,c,d) LogSoundInstance(a,b,c,d)
#else
    #define LOG_SOUND_INSTANCE(a,b,c,d)
#endif // AUDIO_DEBUG

static int sndGetIndex(const char *name)
{
    int        idx;

    if (g_audio_state.noaudio)
        return -1;

    if (!stashFindInt(sound_name_hashes,name,&idx))
        return -1;
    return idx;
}

const char *sndGetFullPath(const char *name)
{
    int        idx;

    if (g_audio_state.noaudio)
        return name;

    if (!stashFindInt(sound_name_hashes,name,&idx))
        return name;
    return sound_name_infos[idx].path;
}

F32 sndSphereVolume(F32 inner,F32 outer,const Vec3 pos)
{
    F32        dist, scale_dist;
    Vec3    dv;

    // outer = total sound radius.
    // inner = radius on the outside of the sphere where sound volume ramps from 1 to 0, going outwards.

    // fpe 1/9/10 -- this code used to behave such that it ignored inner and just used outer. We
    //    now have the option to behave the old way or the new way.  The new way is the "correct" way, but
    //    requires updating alot of sound nodes to set inner radius to 0 in order to get the same behavior as
    //    the old code produced.  
    if( game_state.ignoreSoundRamp ) 
        inner = 0.0f;

    subVec3(pos,head_mat[3],dv);
    dist = normalVec3(dv);
    if(dist >= outer )
        return 0.f;

    if( dist <= inner )
        return 1.f;

    scale_dist = outer-inner;
    return (1.f - ((dist-inner) / scale_dist));
}

static int gets3D(SoundSphere* sphere)
{
    return sphere->forceDimension == 3 ||
                !( sphere->onplayer ||
                sphere->forceDimension == 2 ||
                sphere->owner==SOUND_PLAYER || 
                sphere->owner==SOUND_MUSIC ||
                sphere->owner==SOUND_GAME ||
                (sphere->owner>=SOUND_AMBIENT && (sphere->inner_radius>=150 ||
                    (sphere->inner_radius<1 && sphere->outer_radius>=150))) ); // added this line to deal with old data which has ramp=radius (ie inner radius = 0)
}

static F32 sphereGroupGetVolume(SoundSphere *spheres,int count,F32 *mixOut)
{
    F32                scale;
    F32                volRatio;
    F32                total_vol = 0;
    F32                total_scale = 0;
    Vec3            dv,rel_pos = {0,0,0};
    S32                i;
    SoundSphere*    sphere = spheres;
    F32                sound_max=0;
    F32                mix = 0;

    for(i=0;i<count;i++,sphere++)
    {
        F32 length;
        F32 fadingRatio = 1.0f;
        F32 dMix;
        
        if(sphere->secTimeoutTotal > 0.f)
        {
            // allow secTimeoutBuffer to pass before we begin fading
            if(sphere->secTimeoutCurrent < sphere->secTimeoutTotal)
                fadingRatio = sphere->secTimeoutCurrent / sphere->secTimeoutTotal; // linear fade over total fadeout time
        }            

        // MS: Forcing inner radius to full radius.
        //scale = sndSphereVolume(sphere->outer_radius,sphere->outer_radius,sphere->pos);
        // fpe 08/03/09, reverted the comment above, see sndSphereVolume for old vs new volume calc
        scale = sndSphereVolume(sphere->inner_radius,sphere->outer_radius,sphere->pos);

        volRatio = scale * sphere->volRatio * fadingRatio;
        if(gets3D(sphere)){
            subVec3(sphere->pos,head_mat[3],dv);
            length = lengthVec3(dv);
        }else{
            zeroVec3(dv);
            length = 0;
        }

        normalVec3(dv);
        scaleVec3(dv,volRatio,dv);
        dMix = dotVec3(dv, head_mat[0]);

        if (sphere->spatialVariance > 0.0f)
            dMix *= 1.0 - exp(-length * length / (2 * sphere->spatialVariance));

        mix += dMix;
        total_vol += volRatio;
        total_scale += scale;
        sound_max = MAX(sphere->max_volume,sound_max);
    }
    total_vol = MIN(1,total_vol);
    total_scale = MIN(1,total_scale);
    if (!total_vol)
        return 0;
    if (mixOut)
        *mixOut = total_vol ? mix / total_vol : 0;

    return total_vol>sound_max?sound_max:total_vol;
}

static void removeSphereFromSphereGroup(SphereGroup* sphereGroup,int idx)
{
    CopyStructsFromOffset(sphereGroup->spheres + idx, 1, --sphereGroup->count - idx);
}

static int sndFindBestSphereGroup(SoundSphere *sphere,int index,F32 volume, bool bInterrupt)
{
    int                i,j;
    SphereGroup*    sphereGroup;
    SoundNameInfo * name_info;

    // tbd, find better place for this
    // check to see if we have exceeded max instance count
    name_info = &sound_name_infos[index];
    if(/*TBD? !sphere->onplayer &&*/ name_info->instGroup && name_info->instGroup->cur_instances >= name_info->instGroup->max_instances) {
#if AUDIO_DEBUG
        if(game_state.sound_info) {
            printf("Not allowing playback of sound \"%s\" because at max instance limit (%d) for instance group \"%s\"\n",
                name_info->path+name_info->nameIndex, name_info->instGroup->max_instances, name_info->instGroup->name);
        }
#endif
        return -1;
    }

    assert(sphere->owner >=0); // sanity check, then remove the conditional below
    if( sphere->owner >=0 )
    {
        // one SphereGroup per owner
        for(i=0;i<g_audio_state.maxSoundChannels;i++)
        {
            sphereGroup = sphereGroups + i;
            for(j=0;j<sphereGroup->count;j++)
            {
                //with surround sound on we want to make sure we're not knocking out one SOUND_PLAYER sound
                //with another, since in surround mode those are handled slightly differently
                if ((sphereGroup->spheres[j].owner == sphere->owner))// && (!g_audio_state.surround || sphereGroup->spheres[j].owner>=100000))
                {
                    // if new sound is same as existing, or only one sphere in SphereGroup, ok to re-use
                    if (sphereGroup->count == 1 || sphereGroup->index == index)
                    {
                        if( !bInterrupt )
                            return -1; // dont interrupt already playing sound on this owner
                        sphereGroup->spheres[j] = *sphere;
                        return i;
                    }
                    else
                    {
                        removeSphereFromSphereGroup(sphereGroup, j--);
                    }
                }
            }
        }

        // if sound is mergable, find same sound and add it in
        for(i=0;i<g_audio_state.maxSoundChannels;i++)
        {
            sphereGroup = sphereGroups + i;
            
            if (sphereGroup->index == index && (sphereGroup->name_info->flags & eSoundFlags_Loop) )
            {
                dynArrayFit(&sphereGroup->spheres,sizeof(sphereGroup->spheres[0]),&sphereGroup->sphere_max,sphereGroup->count);
                sphereGroup->spheres[sphereGroup->count++] = *sphere;
                //printf("%10d: found same %d!\n", time(NULL), i);
                return i;
            }
        }
    }

    // any empty sphereGroups?
    for(i=0;i<g_audio_state.maxSoundChannels;i++)
    {
        sphereGroup = sphereGroups + i;
        if (sphereGroup->index < 0 || !sphereGroup->count)
        {
            dynArrayFit(&sphereGroup->spheres,sizeof(sphereGroup->spheres[0]),&sphereGroup->sphere_max,1);
            sphereGroup->spheres[0] = *sphere;
            sphereGroup->count = 1;
            //printf("%10d: found empty %d!\n", time(NULL), i);
            return i;
        }
    }

    // Kick out a quieter sound
    {
        F32        softest=volume;
        int        idx=-1;

        for(i=0;i<g_audio_state.maxSoundChannels;i++)
        {
            sphereGroup = sphereGroups + i;
            if (sphereGroup->volRatio < softest)
            {
                softest = sphereGroup->volRatio;
                idx = i;
            }
        }
        if (idx >= 0)
        {
            sphereGroup = sphereGroups + idx;
            sphereGroup->spheres[0] = *sphere;
            sphereGroup->count = 1;
#if AUDIO_DEBUG
            if( game_state.sound_info > 2 ) {
                printf("Time %.3f: sndFindBestSphereGroup kicking out quieter sound \"%s\" to make room for new sound \"%s\"\n",
                    server_time, sound_name_infos[sphereGroup->index].path, name_info->path);
            }
#endif
            //printf("found quieter!\n");
            return idx;
        }
    }
    return -1;
}

typedef struct MuteFlagNames {
    char        soundType[50];
    int            muteFlag;
} MuteFlagNames;

static MuteFlagNames sMuteFlagNames[] =
{
    { "none",        0 },
    { "fx",            SOUND_MUTE_FLAGS_FX },
    { "music",        SOUND_MUTE_FLAGS_MUSIC },
    { "other",        SOUND_MUTE_FLAGS_OTHERFX },
    { "sphere",        SOUND_MUTE_FLAGS_AMBSPHERES },
    { "script",        SOUND_MUTE_FLAGS_AMBSCRIPTS },
    { "vo",            SOUND_MUTE_FLAGS_VOICEOVER },
    { "all",        SOUND_MUTE_FLAGS_ALL },
};

bool sndMute(const char* soundType, bool bMute)
{
    // convert name to mute flag
    int muteFlag = -1, i;
    int numTypes = sizeof(sMuteFlagNames)/sizeof(sMuteFlagNames[0]);
    for(i=0; i<numTypes; i++)
    {
        if( !stricmp(soundType, sMuteFlagNames[i].soundType) ) {
            muteFlag = sMuteFlagNames[i].muteFlag;
            break;
        }
    }

    if(muteFlag != -1) {
        if( (muteFlag == 0 && bMute) || (muteFlag == SOUND_MUTE_FLAGS_ALL && !bMute) )
            game_state.soundMuteFlags = 0; // clear all mute flags
        else if(bMute)
            game_state.soundMuteFlags |= muteFlag;
        else if(muteFlag)
            game_state.soundMuteFlags &= ~muteFlag;
    }

    return (muteFlag != -1);
}

static F32 applyUserVolumeSetting(int owner, SoundNameInfo* name_info, F32 volume )
{
    SoundVolumeType soundVolumeType;

    if (owner == SOUND_MUSIC || (name_info && name_info->flags & eSoundFlags_Music))
        soundVolumeType = SOUND_VOLUME_MUSIC;
    else if(owner == SOUND_VOICEOVER)
        soundVolumeType = SOUND_VOLUME_VOICEOVER;
    else
        soundVolumeType = SOUND_VOLUME_FX;

#if AUDIO_DEBUG
    if(game_state.soundMuteFlags) {
        // convert owner to sound type to determine if this sound type is muted
        if(soundVolumeType == SOUND_VOLUME_MUSIC) {
            if(game_state.soundMuteFlags & SOUND_MUTE_FLAGS_MUSIC)
                return 0;
        } else if(owner >= SOUND_FX_BASE && owner < SOUND_FX_LAST ) {
            if(game_state.soundMuteFlags & SOUND_MUTE_FLAGS_FX)
                return 0;
        } else if(owner == SOUND_GAME || owner == SOUND_PLAYER ) {
            if(game_state.soundMuteFlags & SOUND_MUTE_FLAGS_OTHERFX)
                return 0;
        } else if(owner == SOUND_VOICEOVER ) {
            if(game_state.soundMuteFlags & SOUND_MUTE_FLAGS_VOICEOVER)
                return 0;
        } else if(owner >= SOUND_AMBIENT_SPHERE_BASE && owner < SOUND_AMBIENT_SCRIPT_BASE ) {
            if(game_state.soundMuteFlags & SOUND_MUTE_FLAGS_AMBSPHERES)
                return 0;
        } else if(owner >= SOUND_AMBIENT_SCRIPT_BASE ) {
            if(game_state.soundMuteFlags & SOUND_MUTE_FLAGS_AMBSCRIPTS)
                return 0;
        }
    }
#endif

    return volume * getLoadScreenVolumeAdjustment(soundVolumeType == SOUND_VOLUME_MUSIC) *
        volume_control[soundVolumeType] * volume_ducking[soundVolumeType].current;
}

static void sndSpatialize(int sphereGroupIdx,SphereGroup *sphereGroup,int doppler)
{
    F32                mix = 0;
    F32                volRatio;
    SoundSphere*    sphere;
    int                use_mix;

    sphere = &sphereGroup->spheres[0];

    // fpe changed BASE_AMBIENT_TO_PAN from (SOUND_AMBIENT+8) to SOUND_AMBIENT so that would
    //    include non-scripted soundspheres.  Not sure why they didn't have these panning, but 
    //    Adam has requested that these pan as well.  They already do if 3d sound is enabled (in
    //    which case the pan value calculated here (as mix) doesn't even get used.
    #define BASE_AMBIENT_TO_PAN    SOUND_AMBIENT
    use_mix = sphere->owner < SOUND_MUSIC || sphere->owner >= BASE_AMBIENT_TO_PAN;

    switch(sphereGroup->count){
        #define CASE(x) xcase x:PERFINFO_AUTO_START("sphereGroupGetVolume:"#x, 1);
        CASE(0);CASE(1);CASE(2);CASE(3);CASE(4);CASE(5);CASE(6);CASE(7);CASE(8);CASE(9);
        CASE(10);CASE(11);CASE(12);CASE(13);CASE(14);CASE(15);CASE(16);CASE(17);CASE(18);CASE(19);
        xdefault:PERFINFO_AUTO_START("sphereGroupGetVolume:>=20", 1);
        #undef CASE
    }
        volRatio = sphereGroup->volRatio = sphereGroupGetVolume(sphereGroup->spheres,sphereGroup->count,use_mix ? &mix : NULL);
    PERFINFO_AUTO_STOP();

    if (volRatio > 1.f) {
        volRatio = 1.f;
    }

    volRatio = applyUserVolumeSetting(sphere->owner, sphereGroup->name_info, volRatio);

    sndSysUpdateChannel(sphereGroupIdx,volRatio,sphere->pos,mix,doppler);
}

// Ducking functionality is currently only set up to be called from the sound thread, do not call from main thread.
//    If needed on main thread, will need to create commands that call into sound thread to access.
static void resetDucking()
{
    int i;
    for(i=0; i<SOUND_VOLUME_COUNT; i++)
    {
        volume_ducking[i].current = 1.f;
        volume_ducking[i].target = 1.f;
        volume_ducking[i].deltaPerSec = 0.f;
    }
}

static void updateDucking(F32 deltaTime)
{
    int i;
    for(i=0; i<SOUND_VOLUME_COUNT; i++)
    {
        if(volume_ducking[i].deltaPerSec != 0.f)
        {
            F32 newVol = volume_ducking[i].current + volume_ducking[i].deltaPerSec * deltaTime;
            bool incrementing = volume_ducking[i].deltaPerSec > 0.f;
            if((incrementing && newVol > volume_ducking[i].target) || (!incrementing && newVol < volume_ducking[i].target))
            {
                newVol = volume_ducking[i].target;
                volume_ducking[i].deltaPerSec = 0.f; // stop ducking
#if AUDIO_DEBUG
                if(game_state.sound_info != 0) {
                    printf("Ducking reached target %.2f on %5s channel\n", newVol, volumeTypeNames[i]);
                }
#endif
            }
            volume_ducking[i].current = newVol;
        }
    }
}

static void setDucking(int volumeTypeFlags, F32 targetVolume, F32 interval)
{
    int i;
    for(i=0; i<SOUND_VOLUME_COUNT; i++)
    {
        if( !(volumeTypeFlags&(1<<i)))
            continue; // dont apply to this bus

        volume_ducking[i].target = targetVolume;
        volume_ducking[i].deltaPerSec = (targetVolume - volume_ducking[i].current)/interval;
#if AUDIO_DEBUG
        if(game_state.sound_info != 0) {
            printf("Ducking updated on %5s channel: target %.2f, interval %.2f\n", volumeTypeNames[i], targetVolume, interval);
        }
#endif
    }
}

// Watch for changes to channels that play only a single volume type so can do any necessary ducking.
static void OnUpdateInstanceCount(int deltaCount, int owner)
{
    if(owner == SOUND_VOICEOVER)
    {
        assert(volume_control[SOUND_VOLUME_VOICEOVER] != 0.f); // shouldnt play if vol is zero
        if(deltaCount == 1)
        {
            // duck music and fx
            setDucking(SOUND_VOLUME_FLAG_MUSIC, volume_ducking[SOUND_VOLUME_MUSIC].defaultDuckVol, volume_ducking[SOUND_VOLUME_MUSIC].defaultIntervals[0]);
            setDucking(SOUND_VOLUME_FLAG_FX, volume_ducking[SOUND_VOLUME_FX].defaultDuckVol, volume_ducking[SOUND_VOLUME_FX].defaultIntervals[0]);
        }
        else
        {
            // return music and fx to unducked levels
            setDucking(SOUND_VOLUME_FLAG_MUSIC, 1.f, volume_ducking[SOUND_VOLUME_MUSIC].defaultIntervals[1]);
            setDucking(SOUND_VOLUME_FLAG_FX, 1.f, volume_ducking[SOUND_VOLUME_FX].defaultIntervals[1]);
        }
    }
}

// This is the ONLY place the instance count gets changed (for both sounds and instancegroups).
static void UpdateInstanceCount(SoundNameInfo* sni, int deltaCount, const char* event, int channel, int owner)
{
    // tbd, remove check for game_state.sound_info once all asserts have gone away
    assert(deltaCount == -1 || deltaCount == 1);
    assertmsgf(!game_state.sound_info || deltaCount > 0 || sni->cur_instances || soundUpdateInfo.flagsThread.freeSoundCache, \
        "UpdateInstanceCount: trying to decrement sound instance count already at zero for sound \"%s\"", sni->path+sni->nameIndex );

    LOG_SOUND_INSTANCE(sni, event, channel, deltaCount);
    if(deltaCount > 0 || sni->cur_instances)
        sni->cur_instances += deltaCount;

    OnUpdateInstanceCount(deltaCount, owner);

    // also update the instance group count if there is one
    if(sni->instGroup)
    {
        assert(!game_state.sound_info || deltaCount > 0 || sni->instGroup->cur_instances || soundUpdateInfo.flagsThread.freeSoundCache);
        if(deltaCount > 0 || sni->instGroup->cur_instances)
            sni->instGroup->cur_instances += deltaCount;
            
#if AUDIO_DEBUG
        if(game_state.sound_info) {
            // In full debug build, go thru all sounds in instGroup and verify that total count adds up
            //    to the instGroup cached value
            int total = 0, j;
            int numSounds = eaSize(&sni->instGroup->sounds);
            for (j = 0; j < numSounds; j++)
            {
                int idx;
                char* sndName = sni->instGroup->sounds[j];
                if (!stashFindInt(sound_name_hashes, sndName, &idx))
                    continue; // already asserted about this error at load time

                total += sound_name_infos[idx].cur_instances;
            }
            assertmsgf(total == sni->instGroup->cur_instances, "ERROR: UpdateInstanceCount found mismatch in cached instance count for sound group \"%s\"", sni->instGroup->name);
        }
#endif
    }
}

static void sndMakeRoom(U32 max_bytes, SoundNameInfo *ignored_info)
{
    int        i,oldest;
    U32        oldest_time;

    for(;;)
    {
        oldest=-1;
        oldest_time=0xffffffff;
        sound_state.file_cache_bytes = 0;
        for(i=0;i<sound_name_count;i++)
        {
            if(!sound_name_infos[i].sound_data)
                continue;

            if(    sound_name_infos[i].last_active < oldest_time &&
                ignored_info != &sound_name_infos[i])
            {
                oldest_time = sound_name_infos[i].last_active;
                oldest = i;
            }
            
            sound_state.file_cache_bytes += sndSysBytes(sound_name_infos[i].sound_data);
        }
        if (sound_state.file_cache_bytes < max_bytes || oldest < 0)
            break;

        // deal with instance count. 
        // dont need this, plus screws up count.  Revisit if necessary, but will need to call sndSysPlaying with appropriate channel.
        //UpdateInstanceCount(&sound_name_infos[oldest], -1, "sndMakeRoom", 0);

        sndSysFree(sound_name_infos[oldest].sound_data);
        sound_name_infos[oldest].sound_data = 0;

        for(i=0;i<g_audio_state.maxSoundChannels;i++)
        {
            SphereGroup* sphereGroup = sphereGroups + i;
            if (sphereGroup->index == oldest)
            {
                //printf("%3d, %3d: setting to -1\n", __LINE__, i);
                sphereGroup->index = -1;
                sphereGroup->count = 0;
            }
        }
    }
}

SoundPlaySpatialInfo* createSoundPlaySpatialInfo()
{
    SoundPlaySpatialInfo* info;
    
    EnterCriticalSection(&csCreateSoundPlaySpatialInfo);
        MP_CREATE(SoundPlaySpatialInfo, 100);
        info = MP_ALLOC(SoundPlaySpatialInfo);
    LeaveCriticalSection(&csCreateSoundPlaySpatialInfo);
    
    return info;
}

void destroySoundPlaySpatialInfo(SoundPlaySpatialInfo* info)
{
    if(info)
    {
        EnterCriticalSection(&csCreateSoundPlaySpatialInfo);
            MP_FREE(SoundPlaySpatialInfo, info);
        LeaveCriticalSection(&csCreateSoundPlaySpatialInfo);
    }
}

static void sndAddToQueue(SoundPlaySpatialInfo* info)
{
    info->next = NULL;

    EnterCriticalSection(&csSoundQueue);
        if(queueMain.tail)
        {
            queueMain.tail->next = info;
        }
        else
        {
            queueMain.head = info;
        }
        
        queueMain.tail = info;
        
        queueMain.count++;
    LeaveCriticalSection(&csSoundQueue);
}

static void sndBeginFadeoutThreaded(int owner, F32 fadeOutSecs)
{
    int                i,j;
    SphereGroup*    sphereGroup;

    // find the spheregroup(s) that has this owner and force the timeout value
    for(i=0;i<g_audio_state.maxSoundChannels;i++)
    {
        sphereGroup = sphereGroups + i;
        for(j=0;j<sphereGroup->count;j++)
        {
            SoundSphere * sphere = &sphereGroup->spheres[j];
            if (sphere->owner == owner)
            {
                sphere->secTimeoutTotal = fadeOutSecs;
                sphere->secTimeoutCurrent = fadeOutSecs;
                sphere->secTimeoutBuffer = 0; // begin fading right away
            }
        }
    }
}

// called on sound playback thread
static void sndPlaySpatialThreaded(SoundPlaySpatialInfo* info)
{
    SphereGroup*    sphereGroup;
    SoundNameInfo*    name_info;
    SoundSphere        sphere = {0};
    S32                sphereGroupIdx;
    F32                volRatio;
    bool            bLoopingSound, bSameSound;

    // check for sndStop command
    if(info->desc_idx == -1)
    {
        sndBeginFadeoutThreaded(info->owner, info->inner); // fadeOutSecs stored in info->inner
        return;
    }

    name_info = &sound_name_infos[info->desc_idx];
    bLoopingSound = (name_info->flags & eSoundFlags_Loop) != 0;
    if (!name_info->sound_data)
    {
        name_info->sound_data = sndSysLoad(name_info->path);
        name_info->last_active = timerSecondsSince2000();
        sndMakeRoom((U32)(game_state.maxSoundCacheMB*1024*1024),name_info);
    }

    sphere.inner_radius        = info->inner;
    sphere.outer_radius        = info->outer;
    sphere.owner            = info->owner;
    sphere.onplayer            = (info->flags&SOUND_PLAY_FLAGS_ONPLAYER)?1:0;
    sphere.forceDimension    = info->forceDimension;
    sphere.volRatio            = info->volRatio;
    sphere.max_volume        = info->max_volume;
    sphere.def_tracker        = info->def_tracker;
    sphere.spatialVariance    = info->spatialVariance;
    copyVec3(info->pos,sphere.pos);

    // sustain flag means to keep playing looping sounds until sndStop explicitly called
    if((info->flags&SOUND_PLAY_FLAGS_SUSTAIN || !bLoopingSound)) {
        sphere.secTimeoutCurrent = sphere.secTimeoutTotal = -1.f;
        sphere.secTimeoutBuffer = 0.f; // not used
    } else {
        sphere.secTimeoutTotal = 1.0f; // used to calculate fadingRatio
        sphere.secTimeoutBuffer = 0.5f; // allow this time to elapse before enforcing the fadeout on unattended sounds
        sphere.secTimeoutCurrent = sphere.secTimeoutTotal + sphere.secTimeoutBuffer;
    }

    volRatio = sphereGroupGetVolume(&sphere,1,0);
    sphereGroupIdx = sndFindBestSphereGroup(&sphere,info->desc_idx,volRatio,(info->flags&SOUND_PLAY_FLAGS_INTERRUPT)!= 0);
    if (sphereGroupIdx < 0)
        return;
    
    sphereGroup = sphereGroups + sphereGroupIdx;
    bSameSound = (sphereGroup->index == info->desc_idx);

    if(    !bLoopingSound ||
        !bSameSound ||
        !sndSysPlaying(sphereGroupIdx))
    {
        DSParams params = {0};
        params.innerRadius    = info->inner;
        params.outerRadius    = info->outer;
        params.gets3D        = gets3D(&sphere);
        if (sphereGroup->index >= 0)
        {
            // already a sound playing on this sphere, either stop it or rewind it (if same sound)
            if (bSameSound)
                sndSysRewind(sphereGroupIdx,name_info->sound_data,&params);
            else {
                sndSysStop(sphereGroupIdx);
                UpdateInstanceCount(&sound_name_infos[sphereGroup->index], -1, "stopped", sphereGroupIdx, info->owner);
            }
        }
        if (!bSameSound)
        {
            F32 tempVolRatio = applyUserVolumeSetting(info->owner,name_info,volRatio);
            sndSysPlay(sphereGroupIdx,name_info->sound_data,tempVolRatio,sphere.pos,0,bLoopingSound?1:0,&params);
            UpdateInstanceCount(name_info, 1, "played", sphereGroupIdx, info->owner);
        }
    }
    
    //printf("%d: setting to: %d\n", sphereGroupIdx, info->desc_idx);
    sphereGroup->name_info    = name_info;
    sphereGroup->volRatio    = volRatio;
    sphereGroup->index        = info->desc_idx;

    PERFINFO_AUTO_START("sndSpatialize", 1);
        sndSpatialize(sphereGroupIdx,sphereGroup,0);
    PERFINFO_AUTO_STOP();
}

static void setEditorInfo(Vec3 pos,F32 radius,void *def_tracker)
{
    SoundEditorInfo    *sei;

    sei = dynArrayAdd(&sound_editor_infos,sizeof(sound_editor_infos[0]),&sound_editor_count,&sound_editor_max,1);
    copyVec3(pos,sei->pos);
    sei->radius = radius;
    sei->def_tracker = def_tracker;
}

static bool applyFadeOut(SphereGroup* sphereGroup)
{
    int        i;

    assert(sphereGroup->count);
    for(i=0;i<sphereGroup->count;i++)
    {
        SoundSphere* sphere = &sphereGroup->spheres[i];
        if(sphere->secTimeoutTotal < 0.f)
            continue; // indicates that sound was played with SOUND_PLAY_FLAGS_SUSTAIN flag, so don't auto-fade it.  Requires explicit call to sndStop to end it.

        sphere->secTimeoutCurrent -= min(sphere->secTimeoutCurrent, soundUpdateInfo.thread.decrementTimeout);
        if (sphere->secTimeoutCurrent <= 0)
            removeSphereFromSphereGroup(sphereGroup, i--);
    }

    // return true if fadeout results in 
    return (sphereGroup->count == 0);
}

static enum {
    SOUNDPLAY_NOTSTARTED = 0,
    SOUNDPLAY_STARTED,
    SOUNDPLAY_TOLDTODIE,
    SOUNDPLAY_DIED,
} soundPlayingThreadState;

static void sndPlayingThreadExit()
{
    if(soundPlayingThreadState == SOUNDPLAY_STARTED)
    {
        sndStopAll();
        sndFreeAll();

        soundPlayingThreadState = SOUNDPLAY_TOLDTODIE;
        
        while(soundPlayingThreadState != SOUNDPLAY_DIED)
        {
            Sleep(1);
        }

        soundPlayingThreadState = SOUNDPLAY_NOTSTARTED;
    }
}

static void sndUpdateGetInputInfo()
{
    EnterCriticalSection(&csSoundUpdateInfo);
    
    if(soundUpdateInfo.hasMat)
    {
        soundUpdateInfo.hasMat = 0;
        soundUpdateInfo.thread = soundUpdateInfo.main;
        
        soundUpdateInfo.main.decrementTimeout = 0;
    }
    
    soundUpdateInfo.flagsThread = soundUpdateInfo.flagsMain;
    
    ZeroStruct(&soundUpdateInfo.flagsMain);
    
    LeaveCriticalSection(&csSoundUpdateInfo);
}

static void sndUpdateCheckStopAllSounds()
{
    if(soundUpdateInfo.flagsThread.stopAllSounds)
    {
        int i;
        for(i=0;i<g_audio_state.maxSoundChannels;i++)
        {
            if (sphereGroups[i].index >= 0)
            {
                sndSysStop(i);
                sphereGroups[i].index = -1;
                sphereGroups[i].count = 0;
                UpdateInstanceCount(sphereGroups[i].name_info, -1, "StopAll", i, -1);
            }
        }
        resetDucking();
    }
}

static void sndUpdateCheckFreeSoundCache()
{
    if(soundUpdateInfo.flagsThread.freeSoundCache)
    {
        sndMakeRoom(0, 0);
    }
}

static int sSndThreadTimer = 0; // use this timer only on the sound thread

static void sndUpdateThreaded(F32 deltaTime)
{
    int                i,count=0;
    SphereGroup*    sphereGroup;

    // Read the input data from the main thread.
    sndUpdateGetInputInfo();
    sndUpdateCheckStopAllSounds();
    sndUpdateCheckFreeSoundCache();
    updateDucking(deltaTime);

    sound_editor_count = 0;
    copyMat4(soundUpdateInfo.thread.mat, head_mat);
    copyMat4(soundUpdateInfo.thread.mat, glob_head_mat);

    if (g_audio_state.surround)
    {
        sndSysPositionListener(soundUpdateInfo.thread.mat, soundUpdateInfo.thread.vel);
//        sndSysCommitSettings();
    }

    for(i=0;i<g_audio_state.maxSoundChannels;i++)
    {
        bool bClearSphereGroup = false, bFaded = false;
        int owner;

        sphereGroup = &sphereGroups[i];
        if (sphereGroup->index < 0)
            continue;
        owner = sphereGroup->spheres[0].owner;

        if (!sndSysPlaying(i) )
        {
            bClearSphereGroup = true;
        }
        else if (applyFadeOut(sphereGroup) )
        {
            sndSysStop(i);
            bClearSphereGroup = true;
            bFaded = true;
        }

        if(bClearSphereGroup)
        {
            sphereGroup->index = -1;
            sphereGroup->count = 0;
            UpdateInstanceCount(sphereGroup->name_info, -1, bFaded?"faded out":"finished", i, owner);
            continue;
        }

        count++;
        sphereGroup->name_info->last_active = timerSecondsSince2000();
        sndSpatialize(i,sphereGroup,1);

#if AUDIO_DEBUG
        if (game_state.sound_info)
        {
            F32 userVol = applyUserVolumeSetting(owner, sphereGroup->name_info, sphereGroup->volRatio);
            if (g_audio_state.surround)
                xyprintf(15,5+i,"%d.%d\t%s\t(%d)\t%s %0.2f %0.2f %s",i,sphereGroup->count,(sndSysChannelGets3D(i)?"3D":"2D"),sphereGroup->spheres[0].owner,sndSysFormat(sphereGroup->name_info->sound_data),sphereGroup->volRatio,userVol, sphereGroup->name_info->path);
            else
                xyprintf(15,5+i,"%d.%d\t%s\t%0.2f %0.2f %s",i,sphereGroup->count,sndSysFormat(sphereGroup->name_info->sound_data),sphereGroup->volRatio,userVol, sphereGroup->name_info->path);
        }
#endif

#ifndef FINAL // only need to set editor info if editor can be activated
        {
            int j;
            for(j=0;j<sphereGroup->count;j++)
            {
                SoundSphere *sphere = &sphereGroup->spheres[j];
                setEditorInfo(sphere->pos,sphere->outer_radius,sphere->def_tracker);
            }
        }
#endif
    }

    soundUpdateInfo.thread.decrementTimeout = 0;

#if AUDIO_DEBUG
    if (game_state.sound_info)
    {
        xyprintf(4,4,"%d sphereGroups    time %.2f",count, server_time);
        xyprintf(4,5,"%s",(g_audio_state.surround?"Surround":"Stereo"));
    }
#endif
}

static SoundPlaySpatialInfo *soundPlayingThreadFetchCommands()
{
    SoundPlaySpatialInfo *result;

    EnterCriticalSection(&csSoundQueue);
        result = queueMain.head;
        queueMain.count = 0; // atomic zeroing of count since we access from main thread without CS
        ZeroStruct(&queueMain);
    LeaveCriticalSection(&csSoundQueue);
    
    return result;
}

static void soundPlayingThreadDispatchCommands(SoundPlaySpatialInfo *head)
{
    while (head != NULL)
    {
        SoundPlaySpatialInfo *next = head->next;

        if(soundPlayingThreadState != SOUNDPLAY_TOLDTODIE)
        {
            sndPlaySpatialThreaded(head);
        }
        
        destroySoundPlaySpatialInfo(head);

        head = next;
    }
}            

void soundTick(F32 deltaTime) 
{
    SoundPlaySpatialInfo *commands;
    PERFINFO_AUTO_START("soundPlayingThread", 1);

    commands = soundPlayingThreadFetchCommands();

    soundPlayingThreadDispatchCommands(commands);

    PERFINFO_AUTO_START("sndUpdateThreaded", 1);
    sndUpdateThreaded(deltaTime);
    PERFINFO_AUTO_STOP();

    PERFINFO_AUTO_STOP();
}

static DWORD WINAPI soundPlayingThread( LPVOID lpParam )
{
    EXCEPTION_HANDLER_BEGIN
    
    int i;
    int sndSysTickCount = 0;
    F32 curTime, lastTime;

    // reinit sphere groups
    memset(&sphereGroups[0], 0, sizeof(SphereGroup) * MAX_EVER_SOUND_CHANNELS);
    for(i=0;i<MAX_EVER_SOUND_CHANNELS;i++) {
        sphereGroups[i].index = -1;
    }

    if ( !sSndThreadTimer )
    {
        // one time creation
        sSndThreadTimer = timerAlloc();
        timerStart(sSndThreadTimer);
    }

    sndSysInit();

    // Quick and dirty fixup for 1800 that should be fixed up in the trunk. This loop would formerly
    // iterate every 5 ms, while the code in sndSysTick would iterate every 20 ms.
    lastTime = timerElapsed(sSndThreadTimer);
    while(soundPlayingThreadState != SOUNDPLAY_TOLDTODIE)
    {
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

        if (sndSysTickCount++ == 4) {
            sndSysTick();
            sndSysTickCount = 0;
        }

        curTime = timerElapsed(sSndThreadTimer);
        soundTick(curTime - lastTime);
        lastTime = curTime;

        Sleep(5);
    }
    
    // Stop everything.
    
    soundUpdateInfo.flagsThread.stopAllSounds = 1;
    soundUpdateInfo.flagsThread.freeSoundCache = 1;
    
    sndUpdateThreaded(0.f);
    
    // Shut down the sound system.

    sndSysExit();

    soundPlayingThreadState = SOUNDPLAY_DIED;

    return 0;

    EXCEPTION_HANDLER_END
}

static void sndPlayingThreadStart()
{
    DWORD dwThreadId, dwThrdParam = 0;

    if(g_audio_state.noaudio || soundPlayingThreadState != SOUNDPLAY_NOTSTARTED)
        return;
    
    resetDucking(); // ducking is controlled only on sound thread

    _beginthreadex(NULL, 0, soundPlayingThread, &dwThrdParam, 0, &dwThreadId);
    
    soundPlayingThreadState = SOUNDPLAY_STARTED;
}

// called from the main thread by function sndAmbientHeadPos in sound_ambient.c
bool sndPlaySpatial(const char *name,int owner,U8 forceDimension,const Vec3 pos,F32 inner,F32 outer,F32 volRatio,F32 max_volume,int flags, void *def_tracker, F32 spatialVariance)
{
    int                desc_idx;
    SoundPlaySpatialInfo* info;
    SoundNameInfo    *name_info;

    if(g_audio_state.noaudio)
        return false;

    // dont allow queue to get too big (happens when lots of oggs playing)
    if(queueMain.count > 256) {
        printf("sndPlaySpatial ignoring request to play sound \"%s\" because queue size = %d\n", name, queueMain.count);
        return false;
    }

    desc_idx = sndGetIndex(name);
    if (desc_idx < 0)
        return false;

    name_info = &sound_name_infos[desc_idx];
    if (applyUserVolumeSetting(owner,name_info,1.f) <= 0.0f) // that kind of sound (fx/music) is muted, so don't try to play it
        return false;

    if (distance3Squared(pos,head_mat[3]) > SQR(outer))
        return false;
        
    if (!(flags&SOUND_PLAY_FLAGS_ONPLAYER) && !vistrayIsSoundReachable(pos, head_mat[3]))
        return false;

    info = createSoundPlaySpatialInfo();
    
    info->desc_idx            = desc_idx;
    info->inner                = inner;
    info->max_volume        = max_volume;
    info->flags                = flags;
    info->forceDimension    = forceDimension;
    info->outer                = outer;
    info->owner                = owner;
    info->volRatio            = volRatio;
    info->def_tracker        = def_tracker;
    info->spatialVariance    = spatialVariance;
    copyVec3(pos, info->pos);

#if AUDIO_DEBUG
    if( game_state.sound_info > 2 && !(name_info->flags & eSoundFlags_Loop) ) {
        printf("-----------------------\n");
        printf("Time %.3f: sndPlaySpatial \"%s\" volume %.2f, owner %d, flags 0x%02x\n",
            server_time, name, volRatio, owner, flags);
    }
#endif

    sndAddToQueue(info);
        
    return true;
}

int sndIsLoopingSample(const char *name)
{
    int idx = sndGetIndex(name);

    if( idx >= 0)
        return (sound_name_infos[idx].flags & eSoundFlags_Loop) ? 1:0;
    else 
        return 0;
}

void sndUpdate(const Mat4 cam_mat)
{
    static int firstTime=1;
    static int gameStateSurround;
    bool bChangedNumChannels = false;
    extern int oggStatesInUse;
    Entity* e = playerPtr();

#if AUDIO_DEBUG
    if(game_state.sound_info)
    {
        xyprintf(    3, 3,
            "sound cache: %.2f MB      oggStatesInUse: %2d     queue: %3d\n",
                    sound_state.file_cache_bytes / (1024.f*1024.f),
                    oggStatesInUse,
                    queueMain.count);
    }
#endif

    EnterCriticalSection(&csSoundUpdateInfo);
        soundUpdateInfo.hasMat = 1;
        copyMat4(cam_mat, soundUpdateInfo.main.mat);
        if(e && e->motion)
        {
            copyVec3(e->motion->vel, soundUpdateInfo.main.vel);
        }
        else
        {
            zeroVec3(soundUpdateInfo.main.vel);
        }
        soundUpdateInfo.main.decrementTimeout += TIMESTEP / 30.0f;
    LeaveCriticalSection(&csSoundUpdateInfo);

    PERFINFO_AUTO_START("sndAmbientHeadPos", 1);
        sndAmbientHeadPos(cam_mat);
    PERFINFO_AUTO_STOP();

    if (firstTime)
    {
        game_state.surround_sound=g_audio_state.uisurround;
        firstTime=0;
        gameStateSurround=game_state.surround_sound;
    }

    if (gameStateSurround!=game_state.surround_sound)
    {
        g_audio_state.uisurround=game_state.surround_sound;
    }
    
#if AUDIO_DEBUG
    bChangedNumChannels = (g_audio_state.maxSoundChannels != game_state.maxSoundChannels ||
        g_audio_state.maxSoundSpheres != game_state.maxSoundSpheres);
    sndUpdateDebug(); // do any debug processing
#endif

    if(    g_audio_state.uisurround != g_audio_state.surround || bChangedNumChannels )
    {
        // Change surround state if requested (and allowed), or if turning off.
        sndPlayingThreadExit();
        g_audio_state.surround = g_audio_state.uisurround;
        g_audio_state.maxSoundChannels = game_state.maxSoundChannels = MIN(game_state.maxSoundChannels, MAX_EVER_SOUND_CHANNELS);
        g_audio_state.maxSoundSpheres = game_state.maxSoundSpheres = MIN(game_state.maxSoundSpheres, MAX_EVER_SOUND_SPHERES);
        sndPlayingThreadStart();
    }

    gameStateSurround=game_state.surround_sound=g_audio_state.uisurround=g_audio_state.surround;
}

// Queue up command to stop sounds playing on the given channel
void sndStop(int owner, F32 fadeOutSecs)
{
    SoundPlaySpatialInfo* info;

    if(g_audio_state.noaudio)
        return;
        
    info = createSoundPlaySpatialInfo();
    memset(info, 0, sizeof(SoundPlaySpatialInfo));
    info->owner                = owner;
    info->desc_idx            = -1; // indicates stop request
    info->inner                = fadeOutSecs;

#if AUDIO_DEBUG
    if(game_state.sound_info > 2) {
        printf("-----------------------\n");
        printf("Time %.3f: sndStop called, owner %d, fadeTime %.2f secs\n",
            server_time, owner, fadeOutSecs);
    }
#endif

    sndAddToQueue(info);
}

void sndPlay(const char *name,int owner)
{
    sndPlayEx(name,owner,1.0f,SOUND_PLAY_FLAGS_INTERRUPT);
}

void sndPlayNoInterrupt(const char *name,int owner)
{
    sndPlayEx(name,owner,1.0f,SOUND_PLAY_FLAGS_NONE);
}

void sndPlayEx(const char *name,int owner, F32 volRatio, int flags)
{
    PERFINFO_AUTO_START("sndPlaySpatial", 1);
    if(owner >= SOUND_MUSIC)
        flags |= SOUND_PLAY_FLAGS_ONPLAYER; // need this here?
    sndPlaySpatial(name,owner,0,zerovec3,100.f,100000.f,volRatio,1.f,flags,0, 0.0f);
    PERFINFO_AUTO_STOP();
}

// sound_type: 0 = fx, 1 = music, 2 = voiceover
// volume 0..1
void sndVolumeControl( SoundVolumeType sound_type, F32 volume )
{
    if(sound_type >= 0 && sound_type < ARRAY_SIZE(volume_control))
        volume_control[sound_type] = volume;
}

static FileScanAction soundLoadCallback(char* dir, struct _finddata32_t* data)
{
    char            *s, buf[256];
    SoundNameInfo    *name_info;

    if (!(data->attrib & _A_SUBDIR))
    {
        if(!strEndsWith(data->name, ".ogg"))
            return FSA_EXPLORE_DIRECTORY;
        dir = strstri(dir, "sound/ogg");
        assert(dir); // sanity check that all sounds are where we expect them to be
        
        name_info = dynArrayAdd(&sound_name_infos,sizeof(sound_name_infos[0]),&sound_name_count,&sound_name_max,1); // already zeroed
        name_info->volRatio = 1.0f; // not used currently
        name_info->flags = strstri(dir, "music") ? eSoundFlags_Music: 0;

        assert((strlen(dir) + strlen(data->name) + 2) < ARRAY_SIZE(name_info->path));
        STR_COMBINE_BEGIN(name_info->path);
        STR_COMBINE_CAT(dir);
        STR_COMBINE_CAT("/");
        STR_COMBINE_CAT(data->name);
        STR_COMBINE_END();

        strcpy(buf,data->name);
        s = strrchr(buf,'.');
        if (s-buf >= 5 && strnicmp(s-5,"_loop",5)==0)
            name_info->flags |= eSoundFlags_Loop;
        *s = 0;

        // save short name string (just point to it in path)
        s = strrchr(name_info->path, '/');
        assert(s);
        name_info->nameIndex = s+1 - name_info->path;

        if (!stashAddInt(sound_name_hashes, buf, sound_name_count-1, false))
        {
            int        idx;
            char    *name1,*name2;

            stashFindInt(sound_name_hashes, buf, &idx);
            name1 = sound_name_infos[idx].path;
            name2 = name_info->path;
            Errorf("duplicate sound %s\n 1st %s\n 2nd %s\n",buf,name1,name2);
            errorLogFileHasError(name1);
            errorLogFileHasError(name2);
        }
    }
    return FSA_EXPLORE_DIRECTORY;
}

// Called from checkOddball in game.c
void reloadSounds()
{
    if (bReloadSounds || bReloadDefs)
    {
        sndExit();
        sndInit();
        bReloadSounds = false;
        bReloadDefs = false;
    }
}

static void noteChangedSounds(const char *relpath, int when)
{
    char buf[256];
    strcpy(buf,relpath);
    if (strstr(relpath, "/_")) return;
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);

    if (strstri(buf,"script")!=NULL)
    {
        extern char ** changedSoundScripts;
        eaPush(&changedSoundScripts,strdup(relpath));
    }
    else if(strstri(buf,".def")!=NULL)
    {
        bReloadDefs = true;
    }
    else
    {
        bReloadSounds = true;
    }
}

static bool LoadSoundDefs()
{
    int count, i;

    StructClear(ParseSoundInstGroupList, &sound_instance_groups); // in case we are reloading
    if (!ParserLoadFiles("sound/defs", ".def","soundinfo.bin", 0, ParseSoundInstGroupList, &sound_instance_groups, NULL, NULL, NULL))
    {
        printf("Failed to load sound/defs!\n");
        return false;
    }

    // now go thru each SoundInfo and copy the metadata to the existing SoundNameInfo for that sound.
    count = eaSize(&sound_instance_groups.instGroups);
    for (i = 0; i < count; i++)
    {
        SoundNameInfo * sni;
        int numSounds, idx, j;
        SndInstanceGroup* instGroup = sound_instance_groups.instGroups[i];
        instGroup->max_instances = CLAMP(instGroup->max_instances, 0, 255);
        instGroup->cur_instances = 0;
        numSounds = eaSize(&instGroup->sounds);
        for (j = 0; j < numSounds; j++)
        {
            char* sndName = instGroup->sounds[j];
            if (!stashFindInt(sound_name_hashes, sndName, &idx))
            {
                // If in LWC mode, this is ok since some data will be loaded later.
                if( !LWC_IsActive() && game_state.sound_info > 0 ) {
                    printf("ERROR: Sound instance group \"%s\" references unknown sound name \"%s\".  Ignoring.\n", instGroup->name, sndName);
                }
                continue;
            }

            // add this sound info to its instance group (only one IG allowed per sound)
            sni = &sound_name_infos[idx];
            assertmsgf(!sni->instGroup, "Sound \"%s\" is assigned to 2 instance groups! (\"%s\" and \"%s\").  Ignoring the first.",
                            sni->path, sni->instGroup->name, instGroup->name);
            sni->instGroup = instGroup;
        }
    }

    return true;
}

// tbd, callback 
void sndOnLWCAdvanceStage(LWC_STAGE stage)
{
    // LWC loaded a new stage, so we need to load new sounds that might be in the  newly loaded pigg files.
    // TBD, just load the new sounds.  For now we re-init sound system (same behavior as dev build when a sound is dropped into data folder so I know this path works)
    bReloadSounds = true;
    reloadSounds();
}

void sndInit()
{
    static int init = 0;
    Entity * e=playerPtr();
    
    if(!init){
        int i;
        init = 1;
        InitializeCriticalSection(&csSoundQueue);
        InitializeCriticalSection(&csCreateSoundPlaySpatialInfo);
        InitializeCriticalSection(&csSoundUpdateInfo);

        // set default ducking intervals/volume
        for(i=0; i<SOUND_VOLUME_COUNT; i++)
        {
            volume_ducking[i].defaultDuckVol = 0.5f;
            volume_ducking[i].defaultIntervals[0] = 0.5f;
            volume_ducking[i].defaultIntervals[1] = 4.0f;
        }
    }

    g_audio_state.surround = g_audio_state.uisurround;
    g_audio_state.maxSoundChannels = game_state.maxSoundChannels;
    g_audio_state.maxSoundSpheres = game_state.maxSoundSpheres;
    sound_name_hashes = stashTableCreateWithStringKeys(1000,StashDeepCopyKeys);
    
    writeConsole(OUTPUT_DEBUG, "Loading sounds");
    fileScanAllDataDirs("sound", soundLoadCallback);
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, "sound/*", noteChangedSounds);
    LoadSoundDefs();
    sndPlayingThreadStart();
    writeConsole(OUTPUT_DEBUG, "Loaded sounds");
}

void sndExit()
{
    sndPlayingThreadExit();

    // free all resources that are allocated in sndInit
    stashTableDestroy(sound_name_hashes);
    sound_name_hashes = NULL;
    free(sound_name_infos);
    sound_name_infos = NULL;
    sound_name_count = sound_name_max = 0;
    StructClear(ParseSoundInstGroupList, &sound_instance_groups);
}

void sndStopAll()
{
    EnterCriticalSection(&csSoundUpdateInfo);
        soundUpdateInfo.flagsMain.stopAllSounds = 1;
    LeaveCriticalSection(&csSoundUpdateInfo);
}

void sndFreeAll()
{
    EnterCriticalSection(&csSoundUpdateInfo);
        soundUpdateInfo.flagsMain.freeSoundCache = 1;
    LeaveCriticalSection(&csSoundUpdateInfo);
}

#if AUDIO_DEBUG
// DEBUG STUFF
typedef struct RepeatSoundData
{
    char name[64];
    int countdown;
    F32 interval;
    int nextOwner;
    F32 nextTriggerTime;
} RepeatSoundData;
static RepeatSoundData sRepeatSound;
static int sDebugTimer = 0;

// playAllSounds functionality
int gPAS_CurSoundIndex = -1;
static F32 gPAS_LastStartTime = 0.f;
#define PAS_NUM_TEST_CHANNELS 32
#define PAS_INTERVAL 0.05f

static void sndUpdateDebug()
{
    F32 curTime;
    if ( !sDebugTimer )
    {
        // one time creation
        sDebugTimer = timerAlloc();
        timerStart(sDebugTimer);
    }

    if(g_audio_state.noaudio)
        return;

    curTime = timerElapsed(sDebugTimer);

    if(sRepeatSound.countdown)
    {
        // see if time to trigger next repeat sound (for testing max instance functionality)
        if(curTime >= sRepeatSound.nextTriggerTime)
        {
            sndPlay(sRepeatSound.name, sRepeatSound.nextOwner++);  
            if(sRepeatSound.nextOwner >= SOUND_FX_LAST)
                sRepeatSound.nextOwner = SOUND_FX_BASE;
            sRepeatSound.nextTriggerTime = curTime + sRepeatSound.interval;
            sRepeatSound.countdown--;
        }
    }

    if(gPAS_CurSoundIndex != -1)
    {
        char buffer[1000];

        // time to play next sound?
        if((curTime - gPAS_LastStartTime) >= PAS_INTERVAL)
        {
            gPAS_CurSoundIndex++;
            if(gPAS_CurSoundIndex >= sound_name_count)
                gPAS_CurSoundIndex = 0;

            printf("Playing sound %d/%d: %s\n", gPAS_CurSoundIndex, sound_name_count, sound_name_infos[gPAS_CurSoundIndex].path);
            //sound_name_infos[gPAS_CurSoundIndex].flags &= ~eSoundFlags_Loop;// so it just plays once
            strcpy(buffer, getFileNameConst(sound_name_infos[gPAS_CurSoundIndex].path));
            if(strchr(buffer, '.')){
                *strchr(buffer, '.') = 0;
            }

            gPAS_LastStartTime = curTime;
            sndPlaySpatial(buffer, SOUND_FX_BASE + gPAS_CurSoundIndex % PAS_NUM_TEST_CHANNELS, 0, head_mat[3], 20, 100, 1, 1, SOUND_PLAY_FLAGS_INTERRUPT, 0, 0.0f);
        }
    }
}

void sndTestRepeat(const char *name, F32 interval, int count)
{
    strncpy(sRepeatSound.name, name, sizeof(sRepeatSound.name));
    sRepeatSound.countdown = count;
    sRepeatSound.interval = interval;
    sRepeatSound.nextOwner = SOUND_FX_BASE;
    sRepeatSound.nextTriggerTime = 0.f;
}

void LogSoundInstance(SoundNameInfo* sni, const char* eventType, int channel, int deltaCount)
{
    // hacky use of sound_info, using negative numbers to see only instance printf output
    if(game_state.sound_info <= -2 || game_state.sound_info > 3 || (game_state.sound_info == -1 && (sni->cur_instances>1 || sni->instGroup->cur_instances>1)) ) {
        char buf[64];
        buf[0] = 0;
        //if(deltaCount < 0) deltaCount = 0; // we show the total pre-decrement but post-increment
        if(sni->instGroup) {
            sprintf(buf, "  [Group \"%s\" count = %2d]", sni->instGroup->name, sni->instGroup->cur_instances + deltaCount);
        }
        printf("[ch %2d]: %-42s : [%-9s] count = %2d%s\n", channel, sni->path+sni->nameIndex, eventType, sni->cur_instances + deltaCount, buf);
    }
}

void sndSetDuckingParams(SoundVolumeType volType, F32 duckedVolume, F32 downInterval, F32 upInterval)
{
    volume_ducking[volType].defaultDuckVol = MINMAX(duckedVolume, 0.f, 1.f);
    volume_ducking[volType].defaultIntervals[0] = downInterval;
    volume_ducking[volType].defaultIntervals[1] = upInterval;
}

#endif // AUDIO_DEBUG
