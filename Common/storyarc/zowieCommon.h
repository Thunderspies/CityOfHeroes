#include "group/grouputil.h"
#include "group/groupProperties.h"
#include "group/group.h"
#include "entity/entity.h"
#include "entity/entityRef.h"

// Define both the client and server versions.  The common routines only reference two members: name and mat, and both of these will hold the same values
// on the client as they do on the server.

#ifdef SERVER
typedef struct Zowie
{
    char *name;
    Vec3 pos;
    Entity *e;
    void *def;
    U32 last_zowie_bit;
    int last_task;
    EntityRef interact_entref;    //the entity interacting with this zowie
} Zowie;
#else
typedef struct Zowie
{
    char *name;
    Vec3 pos;
    GroupDef *def;
    F32 soundFade;
} Zowie;
#endif

extern Zowie **g_Zowies;

// This is the maximum number of Zowies we can have glowing for a single mission
#define MAX_GLOWING_ZOWIE_COUNT        32

int zowie_type_matches(const char *names1, const char *names2);
int count_matching_zowies_up_to(char *task_zowie_name, int n);
int find_zowie_from_pos_and_name(char *name, Vec3 zPos);
int find_nth_zowie_of_this_type(char *type, int n);
unsigned int ZowieBuildPool(int count, int avail, unsigned int seed, int *result);
int zowieSortCompare(const void *a, const void *b);
