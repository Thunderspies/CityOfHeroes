#include "comm_backend.h"
#include <stdio.h>

int main(void)
{
    int type;
    /* Slot 24 is permanently retired. Neighboring IDs must remain stable. */
    if (CONTAINER_RAIDS != 25 || CONTAINER_LEAGUES != 26)
        return 1;
    for (type = -1; type <= MAX_CONTAINER_TYPES; ++type)
    {
        int expected = type >= 1 && type < MAX_CONTAINER_TYPES && type != 24;
        if (!!CONTAINER_IS_VALID(type) != expected)
        {
            fprintf(stderr, "Unexpected validity for container type %d\n", type);
            return 1;
        }
    }
    if (!CONTAINER_IS_GROUP(CONTAINER_TEAMUPS) ||
        !CONTAINER_IS_GROUP(CONTAINER_SUPERGROUPS) ||
        !CONTAINER_IS_GROUP(CONTAINER_LEAGUES) ||
        !CONTAINER_IS_STATSERVER_OWNED(CONTAINER_LEAGUES))
        return 1;
    return 0;
}
