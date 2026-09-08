#include <utilitieslib/stdtypes.h>
#include "sound_ambient.h"
#include "sound/sound.h"
#include "group/group.h"
#include "group/grouptrack.h"
#include "cmdparse/cmdgame.h"
#include "graphics/font.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>
#include "graphics/groupdraw.h"
#include "group/groupgrid.h"
#include "win/win_init.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/components/StashTable.h>
#include "graphics/vistray.h"

#define        MAX_SOUND_SPHERES_PER_AREA    200
#define        WHITESPACE_CHARS " \n\r\t"

char ** changedSoundScripts;

typedef struct
{
    DefTracker    *tracker;
    F32            volume;
    char        *name;
    F32            inner,outer;
    Vec3        pos;
    F32            exclude_ramp;
    SoundScript *script;
    int            script_index;
    F32            variance;
} AmbInfo;

int new_mode = 1;
F32 server_time;    //this way nothing funky can happen when we're looking for scripted sounds
extern AudioState g_audio_state;

#define RANDF_IN_RANGE(low, hi)        (low + (hi - low) * ((float)rand()/(float)RAND_MAX))


static int cmpAmbVolume(const void* vaData, const void* vbData)
{
    const AmbInfo * va = (const AmbInfo *)vaData;
    const AmbInfo * vb = (const AmbInfo *)vbData;
    return (int) ((vb->volume - va->volume) * 1024);
}

static int cmpAmbName(const void* vaData, const void* vbData)
{
    const AmbInfo * va = (const AmbInfo *)vaData;
    const AmbInfo * vb = (const AmbInfo *)vbData;
    return stricmp(vb->name,va->name);
}

static int sndSoundScriptCanPlayNow(SoundScript * ss)
{
    bool bInTimeWindow = (ss->start<ss->stop && server_time>ss->start && server_time<ss->stop) ||
            (ss->stop<ss->start && (server_time>ss->start || server_time<ss->stop));

    // we also now always check time window for all scripts, not just "window" sounds
    if( !bInTimeWindow )
        return 0;

    if (ss->type==SoundScriptPeriodic)
    {
        F32 deltaTime;

        if( ss->nextTime == -1.f ) {
            // first time thru after initializing sound script, need to set nextTime
            ss->nextTime = server_time + RANDF_IN_RANGE(0.f, ss->curPeriod); // first playback can happen any time in period
            if( ss->nextTime >= 24.f )
                ss->nextTime -= 24.f;
        }

        // protect against case where clock rollover occurs between server_time and nextTime
        deltaTime = server_time - ss->nextTime;
        if( !(deltaTime > 0 && deltaTime < 23.5) )
            return 0; // not due yet

        return 1;
    }
    if (ss->type==SoundScriptWindowed)
    {
        // already checked time window, so play it
        return 1;
    }
    return 0;
}

static void clearSoundScripts(GroupDef * def)
{
    int i, j, k;
    if (def->sound_script_filename==NULL)
        return;
    for (i=0;i<eaSize(&changedSoundScripts);i++)
    {
        const char* psubstr = strstri(changedSoundScripts[i],def->sound_script_filename);
        if (psubstr!=NULL &&
            strcmp(psubstr,def->sound_script_filename)==0)
        {
            for (j=0;j<eaSize(&def->sound_scripts);j++)
            {
                SoundScript * ss=def->sound_scripts[j];
                for(k=0;k<ss->name_count;k++)
                    free(ss->names[k]);
                free(ss);
            }
            eaDestroy(&def->sound_scripts);
            def->sound_scripts=0;
            free(changedSoundScripts[i]);
            eaRemove(&changedSoundScripts,i);
        }
    }
}

#define MAX_NAMES_PER_SCRIPT  32 // allow more than one sound name in a script, will choose one for each play
static SoundScript* parseSoundScriptParameters(char * errors)
{
    char * token, *names[MAX_NAMES_PER_SCRIPT];
    int parseOk=1, name_count = 0, index, tempInt;
    SoundScript ss, *pss = NULL;

    memset(&ss,0,sizeof(SoundScript));
    ss.ramp=30;
    ss.stop = 24.f; // by default window is all day long, no exclusions
    ss.curPeriod = ss.minPeriod = ss.maxPeriod = 1;
    token=strtok(NULL, WHITESPACE_CHARS);
    while (token && stricmp(token,"end")!=0)
    {
        if (stricmp(token,"name")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            if(name_count < MAX_NAMES_PER_SCRIPT)
            {
                extern StashTable sound_name_hashes;
                names[name_count++] = strdup(token);
                if (!stashFindInt(sound_name_hashes,token,&index)) {
                    sprintf(errors+strlen(errors),"Could not find sound %s\n",token);
                }
            } else {
                sprintf(errors+strlen(errors),"Too many names in script, max count is %d.\n", MAX_NAMES_PER_SCRIPT);
            }
        } else if (stricmp(token,"period")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.minPeriod);
            ss.maxPeriod = ss.minPeriod; // old way, no randomization
            parseOk=1;
        } else if (stricmp(token,"minPeriod")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.minPeriod);
            parseOk=1;
        } else if (stricmp(token,"maxPeriod")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.maxPeriod);
            parseOk=1;
        } else if (stricmp(token,"start")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.start);
            parseOk=1;
        } else if (stricmp(token,"stop")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.stop);
            parseOk=1;
        } else if (stricmp(token,"volume")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.minVolume);
            ss.maxVolume = ss.minVolume; // old way, no randomization
            parseOk=1;
        } else if (stricmp(token,"minVolume")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.minVolume);
            parseOk=1;
        } else if (stricmp(token,"maxVolume")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.maxVolume);
            parseOk=1;
        } else if (stricmp(token,"radius")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.radius);
            parseOk=1;
        } else if (stricmp(token,"randomize")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%d",&tempInt);
            ss.randomize = (tempInt != 0 ? 1:0);
            parseOk=1;
        } else if (stricmp(token,"interrupt")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%d",&tempInt);
            ss.canInterrupt = (tempInt != 0 ? 1:0);
            parseOk=1;
        } else if (stricmp(token,"ramp")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%f",&ss.ramp);
            parseOk=1;
        } else if (stricmp(token,"dimension")==0) {
            token=strtok(NULL,WHITESPACE_CHARS);
            sscanf(token,"%d",&tempInt);
            if( tempInt == 2 || tempInt == 3 ) // force 2d or force 3d, otherwise 0 is default behavior (radius > 150ft is 2D)
                ss.forceDimension = tempInt;
            parseOk=1;
        } else if (parseOk) {
            sprintf(errors+strlen(errors),"Unrecognized token \"%s\"\n",token);
            parseOk=0;
        }
        token=strtok(NULL,WHITESPACE_CHARS);
    }

    if(parseOk)
    {
        static int id_counter = 1;
        int names_array_size = name_count * sizeof(char*);
        pss = malloc(sizeof(SoundScript) + names_array_size);
        memcpy( pss, &ss, sizeof(SoundScript) );
        pss->name_count = name_count;
        pss->names = (char**)(pss + 1); // names array at end of struct
        memcpy( pss->names, names, names_array_size );
        pss->last_played = pss->name_count-1; // for ordered playback, start with first entry in list next time gets played
        pss->curVolume = RANDF_IN_RANGE(pss->minVolume, pss->maxVolume);
        pss->curPeriod = RANDF_IN_RANGE(pss->minPeriod, pss->maxPeriod);
        pss->nextTime = -1.f; // indicates that nextTime needs to be calculated
        pss->script_id = id_counter++; // give script a unique id so can identify it via owner field
        if(id_counter > 0xFFFF) id_counter = 1; // don't let get too high
    }

    return pss;
}

// we allow comments using '#' character to end of line
static void killComments(char * data)
{
    char* pCursor = data;
    char* pComment = strchr(pCursor, '#');
    while(pComment)
    {
        pCursor = pComment;
        while( *pCursor != '\n' && *pCursor != '\r' )
            *pCursor++ = ' '; // replace comment body with spaces
        pComment = strchr(pCursor, '#');
    }
}

static void loadSoundScripts(GroupDef * def)
{
    char filename[256];
    char * data;
    char * token;
    char * readAt;
    char errors[1024];
    int parseOk=1;
    float maxVol = 0.f;
    errors[0]=0;
    
    if (g_audio_state.noaudio)
        return;

    sprintf(filename,"sound\\scripts\\%s",def->sound_script_filename);
    data=fileAlloc(filename,0);
    if( !data )
        return;
    killComments(data);
    readAt=data;
    token=strtok(readAt, WHITESPACE_CHARS);
    eaCreate(&def->sound_scripts);
    while (token && stricmp(token,"sound")==0) {
        SoundScript * ss=NULL;
        SoundScriptType sstype = SoundScriptNone;
        token=strtok(NULL,WHITESPACE_CHARS);
        if (token && stricmp(token,"periodic")==0) {
            sstype = SoundScriptPeriodic;
            ss = parseSoundScriptParameters(errors);
            parseOk=1;
        } else if (token && stricmp(token,"window")==0) {
            sstype = SoundScriptWindowed;
            ss = parseSoundScriptParameters(errors);
            ss->canInterrupt = 1; // window scripts are looping, need to keep pumping calls to sndFindBestSphereGroup
            parseOk=1;
        } else if (parseOk) {
            sprintf(errors+strlen(errors),"Unrecognized token \"%s\"\n",token);
            parseOk=0;
        }
        if (ss!=NULL) {
            ss->type = sstype;
            maxVol = MAX(ss->maxVolume, maxVol);
            eaPush(&def->sound_scripts,ss);
        }
        token=strtok(NULL,WHITESPACE_CHARS);
    };
    if (*errors) {
        char errorMsg[2048];
        sprintf(errorMsg,"File: %s\n%s",filename,errors);
        winErrorDialog(errorMsg,"Sound Script Errors",NULL,0);
    } else {
        // fpe 12/8/2010 -- udpate the def volume with the max volume any script will ever use,
        //    since this volume is used for sorting sound spheres, and if no script is active the
        //    def's sound_vol is used.  At least I think that's how it works...
        def->sound_vol = (U8)(maxVol * 128.f);
    }

    fileFree(data);
}

static void findSoundsRecur(DefTracker *tracker,const Vec3 pos,int *count,AmbInfo *sound_ids,int max)
{
    Vec3    dv;
    int        i;
    F32        d;
    GroupDef    *def = tracker->def;
    GroupEnt    *ent;

    if (!def->sound_radius)
        return;
    mulVecMat4(def->mid,tracker->mat,dv);
    subVec3(pos,dv,dv);
    d = lengthVec3(dv);
    if (d > def->sound_radius)
        return;

    if (!tracker->entries)
        trackerOpen(tracker);
    for(ent=def->entries,i=0;i<def->count;i++,ent++)
    {
        if( !(ent->flags_cache & CACHEING_IS_DONE) || (ent->flags_cache & HAS_SOUND_RADIUS))
        {
            if (ent->def->sound_radius)
                findSoundsRecur(&tracker->entries[i],pos,count,sound_ids,max);
        }
    }

    if (def->has_sound && *count < max && !groupTrackerHidden(tracker))
    {
        memset(&sound_ids[*count],0,sizeof(AmbInfo));
        sound_ids[*count].tracker = tracker;
        copyVec3(tracker->mat[3],sound_ids[*count].pos);
        (*count)++;

#ifndef FINAL
        if(*count >= max )
        {
            // we need to bump up the size of the array or else won't hear some sounds
            static bool bOnce = false;
            if( !bOnce ) {
                // only show this assert once
                bOnce = true;
                assertmsgf(0, "findSoundsRecur ran out of slots to fill, some sounds will never play!  Let a programmer know that MAX_SOUND_SPHERES_PER_AREA needs to be > %d.", MAX_SOUND_SPHERES_PER_AREA);
            }
        }
#endif

        if (eaSize(&changedSoundScripts))
        {
            clearSoundScripts(def);
        }
        if (def->sound_script_filename && !def->sound_scripts)
        {
            loadSoundScripts(def);
        }
        if (def->sound_scripts)
        {
            for (i=0;i<eaSize(&def->sound_scripts);i++)
            {
                SoundScript* ss = def->sound_scripts[i];
                if (sndSoundScriptCanPlayNow(ss))
                {
                    memset(&sound_ids[*count],0,sizeof(AmbInfo));
                    sound_ids[*count].tracker = tracker;
                    copyVec3(tracker->mat[3],sound_ids[*count].pos);
                    sound_ids[*count].script = ss;
                    sound_ids[*count].script_index = i;
                    (*count)++;
                    ss->shouldPlay=1;

                    // choose next sound to play if more than one sound
                    if(ss->name_count > 1)
                    {
                        if( ss->randomize ) {
                            ss->last_played = rand() % ss->name_count;
                        } else {
                            ss->last_played++;
                            if( ss->last_played >= ss->name_count)
                                ss->last_played = 0; // wrap
                        }
                    }

                    // for periodic sounds, set the volume and period for next playback
                    if(ss->type == SoundScriptPeriodic)
                    {
                        ss->curVolume = RANDF_IN_RANGE(ss->minVolume, ss->maxVolume);
                        ss->curPeriod = RANDF_IN_RANGE(ss->minPeriod, ss->maxPeriod);
                        ss->nextTime = server_time + ss->curPeriod;
                        if( ss->nextTime >= 24.f )
                            ss->nextTime -= 24.f;

                        #ifndef FINAL
                            if( game_state.sound_info > 1) {
                                printf("-----------------------\n");
                                printf("Time %.3f: playing periodic sound \"%s\" from script \"%s\"\n\tvol %.2f, nextTime %.2f\n\tscript rad/ramp = %.1f/%.1f, ssphere rad/ramp = %.1f/%.1f\n",
                                    server_time, ss->names[ss->last_played], def->sound_script_filename, ss->curVolume, ss->nextTime,
                                    ss->radius, ss->ramp, def->sound_radius, def->sound_ramp);
                            }
                        #endif
                    }
                }
                else
                    def->sound_scripts[i]->shouldPlay=0;
            }
        }
    }
}

static void findSounds(const Vec3 pos,int *count,AmbInfo *sound_ids,int max)
{
    DefTracker    *ref;
    int            i;

    for(i=0;i<group_info.ref_count;i++)
    {
        ref = group_info.refs[i];
        if (ref->def)
            findSoundsRecur(ref,pos,count,sound_ids,max);
    }
}


#define MAXFADE 45.f
#define MINFADE 25.f
#define FADESCALE 0.5f

void showSounds(int x,int y,int count,AmbInfo *sounds, bool bPostMerge)
{
    int            i, yoffset;
    GroupDef    *def;

    if(!count)
        return;

    xyprintf(x,y,"%s-Merge Count: %d",bPostMerge ? "Post":"Pre", count);
    yoffset = y+1;
    for(i=0;i<count;i++)
    {        
        if(bPostMerge && i == g_audio_state.maxSoundSpheres)
            xyprintf(x,yoffset++,"--------------- MAX SOUND SPHERES = %d -----------------", g_audio_state.maxSoundSpheres);
        
        def = sounds[i].tracker->def;
        xyprintf(x,yoffset++,"%s%s (def %s) %.2f",def->sound_script_filename ? def->sound_script_filename : sounds[i].name,
            def->sound_script_filename ? "(SCR)" : "", def->name,sounds[i].volume);
    }
}

void setAmbientInfo(int count,AmbInfo *sounds, bool bMerged)
{
    int            i;
    GroupDef    *def;
    AmbInfo        *amb = sounds;

    for(i=0;i<count;i++, amb++)
    {
        F32 inner;

        def = amb->tracker->def;

        if (def->sound_ramp)
            inner = def->sound_radius - def->sound_ramp;
        else {
            inner = def->sound_radius * FADESCALE;
            if (def->sound_radius - inner > MAXFADE)
                inner = def->sound_radius - MAXFADE;
            if (def->sound_radius - inner < MINFADE)
                inner = def->sound_radius - MINFADE;
        }

        if (inner < 0)
            inner = 0;

        if(!bMerged) { // merged sound already have volumes calculated and added together, don't change
            float soundVol;

            // calculate attenuation volume
            amb->volume = sndSphereVolume(inner,def->sound_radius,amb->pos);

            // multiply that by the sound volume (different for script vs regular sound)
            if (amb->script) {
                assert(amb->script == def->sound_scripts[amb->script_index]);
                soundVol = amb->script->curVolume;
            }
            else
                soundVol = def->sound_vol * (1.f/128.f);

            if(soundVol != 0.f)
                amb->volume *= soundVol;

#ifndef FINAL
            // for testing a single debug sound (eg "sounddebugname CityNovaBed1_Loop")
            if(game_state.soundDebugName[0]) {
                if(def->sound_name && stricmp(def->sound_name, game_state.soundDebugName)) {
                    // kill this sound since it's not the one we care about
                    //amb->volume = 0.f;
                } else {
                    // for debug break
                    amb->volume = amb->volume;
                }
            }
#endif
        }
        if (amb->script!=NULL)
            amb->name = amb->script->names[amb->script->last_played];
        else
            amb->name = def->sound_script_filename ? "" : def->sound_name; // if has a script, we ignore sound_name, set to empty here for smarter merging downstream
        if (new_mode)
        {
            amb->inner = MAX(inner,amb->inner);
            amb->outer = MAX(def->sound_radius,amb->outer);
        }
        else
        {
            amb->inner = inner;
            amb->outer = def->sound_radius;
        }
    }
}

void mergeSameSounds(int *count_ptr,AmbInfo *sounds,const Mat4 head_mat)
{
    int i;
    int merge_count;
    int count;
    static AmbInfo* merge_sounds;
    static int merge_max;
    AmbInfo* curr = NULL;
    Vec3 rel_pos;
    Vec3 dv;
    Vec3 secondMoment;
    F32 volume;

    count = *count_ptr;
    if (!count)
        return;
    merge_count = 0;
    dynArrayFit(&merge_sounds,sizeof(merge_sounds[0]),&merge_max,count);
    qsort(sounds,count,sizeof(AmbInfo),cmpAmbName);
    for(i=0;i<=count;i++)
    {
        // fpe 8/09 -- don't merge periodic one shots now that each of these can have a list
        //    of sounds within a single script that play randomly.  Merging them would be arbitrary
        //    since most of the time the two scripts would be playing different sounds, but sometimes 
        //    they would be the same and only then would they merge, which seems rather arbitrary so
        //    we don't allow any merging in this case.
        bool bFirstOrLast = (i==0 || i==count);
        bool bDoMerge = !bFirstOrLast && strlen(sounds[i].name) && !stricmp(sounds[i].name,sounds[i-1].name) &&
                !(sounds[i].script && sounds[i].script->type == SoundScriptPeriodic); 
        if (bFirstOrLast || !bDoMerge)
        {
            if (i)
            {
                F32        inv_vol = 1000;
                Vec3    rel_pos_squared;

                if (volume)
                    inv_vol = 1.f / volume;
                scaleVec3(rel_pos,inv_vol,rel_pos);
                addVec3(rel_pos,head_mat[3],curr->pos);
                scaleVec3(secondMoment, inv_vol, secondMoment);
                mulVecVec3(rel_pos, rel_pos, rel_pos_squared);
                curr->variance = secondMoment[0] - rel_pos_squared[0] + 
                    secondMoment[1] - rel_pos_squared[1] +
                    secondMoment[2] - rel_pos_squared[2];
                curr->volume = volume;
                if (new_mode)
                {
                    curr->inner = MAX(curr->inner,sounds[i-1].inner);
                    curr->outer = MAX(curr->outer,sounds[i-1].outer);
                }
                if (i==count)
                    goto done;
            }
            curr = &merge_sounds[merge_count++];
            *curr = sounds[i];
            zeroVec3(rel_pos);
            zeroVec3(secondMoment);
            volume=0;
        }
        subVec3(sounds[i].tracker->mat[3],head_mat[3],dv);
        scaleVec3(dv,sounds[i].volume,dv);
        addVec3(rel_pos,dv,rel_pos);
        mulVecVec3(dv, dv, dv);
        scaleVec3(dv,1/sounds[i].volume,dv);
        addVec3(dv, secondMoment, secondMoment);
        volume += sounds[i].volume;
        volume = MIN(volume, 1.f);
    }
done:
    memcpy(sounds,merge_sounds,sizeof(sounds[0]) * merge_count);
    *count_ptr = merge_count;
}

void keepSameOwner(int count,AmbInfo *sounds)
{
    static AmbInfo    last_sounds[MAX_EVER_SOUND_SPHERES];
    static U32 last_ref_mod_time;
    int                i,j;
    AmbInfo            t;

    if (last_ref_mod_time != group_info.ref_mod_time)
    {
        memset(last_sounds,0,sizeof(last_sounds));
        last_ref_mod_time = group_info.ref_mod_time;
    }

    memset(&sounds[count],0,(g_audio_state.maxSoundSpheres-count)*sizeof(AmbInfo));
    for(i=0;i<g_audio_state.maxSoundSpheres;i++)
    {
        for(j=0;j<count;j++)
        {
            if (last_sounds[i].name && sounds[j].name && strcmp(last_sounds[i].name,sounds[j].name)==0)
            {
                if (i != j)
                {
                    t = sounds[j];
                    sounds[j] = sounds[i];
                    sounds[i] = t;
                }
                break;
            }
        }
    }
    memcpy(last_sounds,sounds,sizeof(AmbInfo) * g_audio_state.maxSoundSpheres);
}

static void excludeSounds(int count,AmbInfo *sounds,const Mat4 head_mat)
{
    int            i;
    F32            mindist = 16e16,ramp,d,maxvol;
    AmbInfo        *excluder=0;
    GroupDef    *def;

    for (i=0;i<count;i++)
    {
        if (!vistrayIsSoundReachable(head_mat[3], sounds[i].pos))
            sounds[i].volume = 0;
    }

    for(i=0;i<count;i++)
    {
        sounds[i].exclude_ramp = 1;
        if (sounds[i].tracker->def->sound_exclude)
        {
            d = distance3Squared(sounds[i].pos,head_mat[3]);
            if (d < mindist)
            {
                excluder = &sounds[i];
                mindist = d;
            }
        }
    }
    if (!excluder)
        return;

    def = excluder->tracker->def;
    maxvol = sndSphereVolume(excluder->inner,def->sound_radius,head_mat[3]);
    if (def->sound_vol)
        maxvol *= def->sound_vol * (1.f/128.f);
    ramp = 1.f - (excluder->volume / maxvol);
    // any soundsphere not enclosed by the excluder gets ramped down by the inverse of the excluder's volume
    for(i=0;i<count;i++)
    {
        d = distance3(sounds[i].pos,excluder->pos);
        if (d > excluder->outer)
            sounds[i].exclude_ramp = ramp;
        sounds[i].volume *= sounds[i].exclude_ramp;
    }
}

static void removeZeroVolumeSounds(int *count, AmbInfo *sounds)
{
    int iIn = 0, iOut = 0;

    while (iIn < *count && sounds[iIn].volume > 0.0f)
    {
        ++iIn;
        ++iOut;
    }

    while (iIn < *count)
    {
        if (sounds[iIn].volume > 0.0f)
            sounds[iOut++] = sounds[iIn];

        ++iIn;
    }

    *count = iOut;
}

void sndAmbientHeadPos(const Mat4 head_mat)
{
    int                i,count=0;
    AmbInfo            sounds[MAX_SOUND_SPHERES_PER_AREA];
    GroupDef        *def;

    server_time=server_visible_state.time;
    findSounds(head_mat[3],&count,sounds,MAX_SOUND_SPHERES_PER_AREA);
    setAmbientInfo(count,sounds,false);
    excludeSounds(count,sounds,head_mat);
    removeZeroVolumeSounds(&count, sounds);
    if(game_state.sound_info > 1) showSounds(5,23,count,sounds, false);
    mergeSameSounds(&count,sounds,head_mat);
    setAmbientInfo(count,sounds,true);
    qsort(sounds,count,sizeof(AmbInfo),cmpAmbVolume);
    if(game_state.sound_info > 1) showSounds(60,23,count,sounds, true);
    count = MIN(g_audio_state.maxSoundSpheres,count);
    keepSameOwner(count,sounds);
    for(i=0;i<g_audio_state.maxSoundSpheres;i++)
    {
        AmbInfo    *amb;
        F32        volume;

        amb = &sounds[i];
        if (!amb->tracker)
            continue;
        def = amb->tracker->def;
        if (amb->script && !def->sound_scripts[amb->script_index]->shouldPlay)
            continue;
        if (!amb->script && def->sound_script_filename)
            continue;
#ifndef FINAL
        if (game_state.ambient_info)
        {
            xyprintf(2,18+i,"%-20.20s  %f {%-3.1f, %-3.1f, %-3.1f}",def->sound_name,amb->volume,
                amb->pos[0],amb->pos[1],amb->pos[2]);
        }
#endif
        // fpe -- why go to the trouble to merge sound sphere volumes if not going to use the merged data? 
        //    Using amb->volume fixes volume jumps when have overlapping spheres.
        //volume=def->sound_vol?(def->sound_vol*(1.f/128.f)):1.f;
        volume = amb->volume;

        if (amb->script)
        {
            SoundScript * ss=def->sound_scripts[amb->script_index];
            F32 inner,outer;
            int curOwner;

            assert(ss == amb->script);

            if (ss->curVolume!=0)
            {
                volume=ss->curVolume;
            }
            if (ss->radius!=0)
            {
                outer=ss->radius;
                inner=ss->radius-ss->ramp;
                if (inner<0) inner=0;
            }
            else
            {
                inner=amb->inner;
                outer=amb->outer;
            }
            
#ifndef FINAL
            // warn if sound radius is less than script radius as this will cause sound to cut in sharply since the 
            //    script isn't processed until we are inside the sound radius
            if(game_state.sound_info && ss->radius > amb->outer)
            {
                xyprintfcolor(2,35,255,0,0,"WARNING: Sound def '%s' has radius %.2f which is less than the soundScript '%s' radius %.2f",
                    def->name, amb->outer, def->sound_script_filename, ss->radius);
            }
#endif
            curOwner = SOUND_AMBIENT_SCRIPT_BASE + ss->script_id;
            sndPlaySpatial(ss->names[ss->last_played], curOwner, ss->forceDimension,amb->pos, inner, outer,
                amb->exclude_ramp*volume, volume, (ss->canInterrupt==1 ? SOUND_PLAY_FLAGS_INTERRUPT:0), amb->tracker, amb->variance);
        }
        else
        {
            sndPlaySpatial(def->sound_name,SOUND_AMBIENT + i,0,amb->pos,amb->inner,amb->outer,
                                        amb->exclude_ramp * volume,volume, SOUND_PLAY_FLAGS_INTERRUPT, amb->tracker, amb->variance);
        }
    }
}

