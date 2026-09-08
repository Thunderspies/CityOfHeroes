#ifndef ZOWIECLIENT_H
#define ZOWIECLIENT_H

typedef struct StashTableImp *StashTable;
struct TaskStatus;

extern StashTable zowieDestList;

#ifdef TEST_CLIENT
#define updateAutomapZowies(t,u)
#define zowieAddGlowFlags(t)
#define zowieSoundTick()
#define zowieRemoveGlowFlags()
#define clientZowie_Load()
#define zowieReset()
#else
// Values for setsaved
#define USE_CACHED            0        // Use the currently cached value
#define SET_CACHED            1        // Set the cache to the passed parameter
#define DESTROY_CACHED        2        // If and only if the passed parameter is the same as the cached, set the cached to NULL.  Used whenever we destroy a TaskStatus
void updateAutomapZowies(struct TaskStatus *task, int cacheControl);
void zowieAddGlowFlags(struct TaskStatus *task);
void zowieSoundTick();
void zowieRemoveGlowFlags();
void clientZowie_Load();
void zowieReset();
#endif

#endif // ZOWIECLIENT_H
