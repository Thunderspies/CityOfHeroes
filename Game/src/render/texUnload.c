#include "render/texUnload.h"
#include "render/tex.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/utils/sysutil.h>
#include <utilitieslib/utils/memlog.h>
#include <utilitieslib/components/Earray.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/osdependent.h>
#include "videoMemory.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>

static int dynamicUnloadEnabled=0;
static U32 nextUnloadTimeSeconds = 0;

#define TEXUNLOAD_FREQUENCY_SECONDS 15 // check to unload old textures


void texDynamicUnload(int enable) {
    dynamicUnloadEnabled = enable;
}

int texDynamicUnloadEnabled() {
    return dynamicUnloadEnabled;
}

int memory_allowed=128*1000000; // Reassigned dynamically below... This is the value where when unloading textures, we will stop unloading once we get down to this point
// Currently, we can be drawing over 32 MB of textures on *one* frame, we never want to unload
// under this amount, with mipLevel of 1, this goes down to under 10 MB
#define MIN_ALLOWED_TEX_MEMORY ((game_state.actualMipLevel?16:32)*1024)

// Define the age of texstures that can be unloaded
#define AGE_MULT (game_state.actualMipLevel?2:1) // If we're taking less memory, keep them around longer
#define MAX_AGE_TO_BE_UNLOADED    (timerCpuSpeed()*seconds_before_unload*AGE_MULT)     // in the same units as last_used_timestamp (CPU ticks)
int seconds_before_unload = 60; // Number of seconds a texture must not have been drawn before it can be unloaded

#define RAW_UNLOAD_TIME (MAX_AGE_TO_BE_UNLOADED/4) // Free these fairly agressively, as they're rarely reused

static U32 ageByTexBind(const BasicTexture *bind) // higher gets unloaded after more time
{
    if (bind->use_category & TEX_FOR_UI) {
        if (bind->memory_use[0] >= 1*1024*1024) {
            return MAX_AGE_TO_BE_UNLOADED/4;
        }
        return MAX_AGE_TO_BE_UNLOADED;
    }
    if (bind->memory_use[0] >= 1*1024*1024) {
        return MAX_AGE_TO_BE_UNLOADED/2;
    }
    if (bind->use_category & TEX_FOR_UTIL) return MAX_AGE_TO_BE_UNLOADED * 10;
    if (bind->use_category & TEX_FOR_FX) return MAX_AGE_TO_BE_UNLOADED * 5;
    if (bind->use_category & TEX_FOR_NOTSURE) return MAX_AGE_TO_BE_UNLOADED * 2;
    if (bind->use_category & TEX_FOR_ENTITY) return MAX_AGE_TO_BE_UNLOADED * 2;
    if (bind->use_category & TEX_FOR_WORLD) return MAX_AGE_TO_BE_UNLOADED;
    return MAX_AGE_TO_BE_UNLOADED;
}

void texDetermineAllowedTextureMemory(void)
{
    DWORDLONG ulAvail, ulMax; // Bytes
    long avail, max, remaining; // KB

    getPhysicalMemory(&ulMax, &ulAvail);
    max = ulMax / 1024;
    avail = ulAvail / 1024;

    if (IsUsingCider())
    {
        // Mac only until we know this is stable.
        max = getVideoMemoryMBs() * 1024;
        seconds_before_unload = 5;
        remaining = max * 3 / 4;
    }
    else
    {
        /*
        //TODO: add a command line option to overrid this (maybe just a -nounload)
        //TODO: Right now these are fairly arbitrary, need we do some testing?
        if (max <=256*1024) { // 256MB system or less
            seconds_before_unload = 90;
            remaining = 32*1024;
        } else if (max <=384*1024) { // 384M
            seconds_before_unload = 120;
            remaining = 48*1024;
        } else if (max <=512*1024) {
            seconds_before_unload = 240;
            remaining = 64*1024;
        } else { // Over 512MB (768MB+) of RAM
            seconds_before_unload = 240;
            remaining = 256*1024; // This should be as much memory as we have textures in the game.
        }
        */

        // Simplified logic above and should provide better scaling for 
        // systems with memory > 512MB of RAM.
        remaining = max / 8;
        remaining = MINMAX(remaining, MIN_ALLOWED_TEX_MEMORY, 256*1024);
        seconds_before_unload = 5;
    }


    // Don't do this, since we also dynamically load other data.
    //    if (remaining < avail) {
    //        // If our heuristic tells us there's less memory than is physically available, then use the physical value.
    //        remaining = avail;
    //    }
    if (remaining < MIN_ALLOWED_TEX_MEMORY) {
        remaining = MIN_ALLOWED_TEX_MEMORY;
    }



    // TODO: Should we further simplify the above logic to this?
    /*
    max = getVideoMemoryMBs() * 1024;
    seconds_before_unload = 5;
    remaining = max * 3 / 4;
    remaining = MINMAX(MIN_ALLOWED_TEX_MEMORY, 256*1024);
    */

    memory_allowed = remaining*1024;
    if (memory_allowed < 64*1024) {
        printf("Limiting texture usage to %1.1fMB\n", memory_allowed / 1024.0);
    }
}

int compareTimestamps(U32 a, U32 b) // Assumes no timestamps are in the future
{
    if (a > game_state.client_frame_timestamp) {
        // client_frame_timestamp has wrapped since time A
        if (b > game_state.client_frame_timestamp) {
            // since b has not wrapped yet as well, just compare them
            return a>b?1:a==b?0:-1;
        } else {
            // b has wrapped with the time, b must be newer
            return -1;
        }
    } else {
        if (b > game_state.client_frame_timestamp) {
            // timestamp has wrapped since B, A must be newer
            return 1;
        } else {
            // no wrapping recently
            return a>b?1:a==b?0:-1;
        }
    }
}

static U32 getLastUsedTime(const BasicTexture *bind, int index)
{
    U32 last_used_time_stamp = bind->last_used_time_stamp[index];

    // Cubemap faces 1-5 do not have their last_used_time_stamp updated.
    // So, look at face 0
    if (bind->texopt_flags & TEXOPT_CUBEMAP && !strEndsWith(bind->name, "0"))
    {
        // temporarily change the bind name to find face 0
        char *face0name = (char*)bind->name;
        BasicTexture *face0bind;
        int faceNumPosition = strlen(face0name) - 1;
        const char zeroeth_face_char = face0name[faceNumPosition];

        face0name[faceNumPosition] = '0';
        face0bind = texFind(face0name);

        // restore the original bind name
        face0name[faceNumPosition] = zeroeth_face_char;

        if (face0bind)
            last_used_time_stamp = face0bind->last_used_time_stamp[index];
    }

    return last_used_time_stamp;
}

static bool unloadCriteria(BasicTexture *bind)
{
    U32 last_used_time_stamp = getLastUsedTime(bind, 0);
    U32 timeToUnloadAt = last_used_time_stamp + ageByTexBind(bind);
    bool ret;

    if (timeToUnloadAt < last_used_time_stamp) {
        // it wrapped
        if (game_state.client_frame_timestamp < last_used_time_stamp) {
            // timestamp has also wrapped, just compare them
            ret = timeToUnloadAt < game_state.client_frame_timestamp;
        } else {
            // timestamp has not wrapped, therefore this unload time must be in the future
            ret = false;
        }
    } else {
        // unload time didn't wrap
        if (game_state.client_frame_timestamp < last_used_time_stamp) {
            // timestamp has wrapped, therefore this unload time must be in the past
            ret = true;
        } else {
            // timestamp has not wrapped just compare them
            ret = timeToUnloadAt < game_state.client_frame_timestamp;
        }
    }
    return ret;
}

static bool unloadCriteriaRaw(BasicTexture *bind)
{
    U32 last_used_time_stamp = getLastUsedTime(bind, 1);
    U32 timeToUnloadAt = last_used_time_stamp + RAW_UNLOAD_TIME;
    bool ret;

    if (bind->rawReferenceCount)
        return false; // Don't unload anything that has an outstanding request on it

    if (timeToUnloadAt < last_used_time_stamp) {
        // it wrapped
        if (game_state.client_frame_timestamp < last_used_time_stamp) {
            // timestamp has also wrapped, just compare them
            ret = timeToUnloadAt < game_state.client_frame_timestamp;
        } else {
            // timestamp has not wrapped, therefore this unload time must be in the future
            ret = false;
        }
    } else {
        // unload time didn't wrap
        if (game_state.client_frame_timestamp < last_used_time_stamp) {
            // timestamp has wrapped, therefore this unload time must be in the past
            ret = true;
        } else {
            // timestamp has not wrapped just compare them
            ret = timeToUnloadAt < game_state.client_frame_timestamp;
        }
    }
    return ret;
}

static int unloadComparator(const BasicTexture **tex1, const BasicTexture **tex2)
{
    // First check to see if raw data is loaded
    if ((*tex1)->load_state[1] == TEX_LOADED) {
        if ((*tex2)->load_state[1] == TEX_LOADED) {
            // compare the timestamps
            return compareTimestamps(getLastUsedTime(*tex1,1), getLastUsedTime(*tex2,1));
        } else {
            // Only tex1 has raw data loaded
            return -1;
        }
    } else {
        if ((*tex2)->load_state[1] == TEX_LOADED) {
            // Only tex2 has raw data loaded
            return 1;
        }
    }
    // Neither have raw data loaded, do normal comparisons
    if ((*tex1)->load_state[0] == TEX_LOADED) {
        if ((*tex2)->load_state[0] == TEX_LOADED) {
            // compare the timestamps, and take into account the bias from the ageByTexBind function
            return compareTimestamps(getLastUsedTime(*tex1,0) - ageByTexBind(*tex2), getLastUsedTime(*tex2,0) - ageByTexBind(*tex1));
        } else {
            // Only tex1 is loaded
            return -1;
        }
    } else {
        if ((*tex2)->load_state[0] == TEX_LOADED) {
            return 1;
        }
        return 0; // Neither loaded
    }
}

void texUnloadTexturesToFitMemory(void)
{
    int removed_count, removed_raw_count, remove_cursor, num_textures;
    BasicTexture * bind;
    static bool inited=false;
    bool freeRawOnly=false;
    U32 now;
    bool forceUnload = false;

    if (!dynamicUnloadEnabled)
        return;

    if (!inited) {
        texDetermineAllowedTextureMemory();
        inited=true;
    }

    now = timerCpuSeconds();
    if (nextUnloadTimeSeconds == 0 || now >= nextUnloadTimeSeconds)
    {
        nextUnloadTimeSeconds = timerCpuSeconds() + TEXUNLOAD_FREQUENCY_SECONDS;
        forceUnload = true;
    }

    if (!forceUnload && texMemoryUsage[0] <= memory_allowed) {
        if (texMemoryUsage[1]>white_tex->memory_use[1]) {
            freeRawOnly=true;
        } else {
            return;
        }
    }

    if (!freeRawOnly) {
        // Sorting the global array of TexBinds... why does this feel dirty?
        eaQSort(g_basicTextures, unloadComparator);
    }


    // Free textures that meet the criteria of being old or relatively old and large
    removed_count = 0;
    removed_raw_count = 0;

    num_textures = eaSize(&g_basicTextures);
    for (remove_cursor = 0; remove_cursor < num_textures && (freeRawOnly || forceUnload || texMemoryUsage[0] > memory_allowed); remove_cursor++) {
        bind = g_basicTextures[remove_cursor];
        if (bind == white_tex) // Never free white
            continue;
        if (bind->load_state[1] == TEX_LOADED)
        {
            // This texture has raw data loaded
            if (unloadCriteriaRaw(bind))
            {
                memlog_printf(0, "%u: Unloading RAW texture %45s, age of %5.2f, cat %d", game_state.client_frame_timestamp, bind->name, (float)(game_state.client_frame_timestamp - getLastUsedTime(bind,1))/timerCpuSpeed(), bind->use_category);
                texFree(bind, 1);
                removed_raw_count++;
                if (freeRawOnly && texMemoryUsage[1]==white_tex->memory_use[1])
                    break;
            }
        } else if (freeRawOnly) {
            continue;
        } else if (bind->load_state[0] == TEX_LOADED) {
            if (unloadCriteria(bind))
            {
                memlog_printf(0, "%u: Unloading texture %45s, age of %5.2f, cat %d", game_state.client_frame_timestamp, bind->name, (float)(game_state.client_frame_timestamp - getLastUsedTime(bind,0))/timerCpuSpeed(), bind->use_category);
                texFree(bind, 0);
                removed_count++;
            }
        } else {
            // All textures from here on out are not loaded, because we sorted them
            break;
        }
    }

    if (removed_count)
        memlog_printf(0, "Dynamically freed %d textures, %d RAW textures\n", removed_count, removed_raw_count);
}
