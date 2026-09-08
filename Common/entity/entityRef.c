/***************************************************************************
 *     Copyright (c) 2000-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <utilitieslib/assert/assert.h>
#include <stdio.h>

#include "entity/entity.h"
#include "entity/entityRef.h"

/****************************************************
 * Entity Reference
 */

typedef union
{
    struct
    {
        U32 owner;
        U32 UID;
    };
    EntityRef ref;
} EntityRefImp;

#define DB_ID_BIT  (0x80000000)

/**********************************************************************func*
 * Function erGetRef()
 *  Given an entity pointer, this function returns an entity reference
 *  that can be used to retrieve the entity later.
 *
 *  Returns:
 *      Valid EntityRef - if the entity is valid.
 */
EntityRef erGetRef(Entity* ent)
{
    EntityRefImp ref;

    if(!ent)
    {
        return 0;
    }

    assert(ent && ent->owner > 0 && ent->owner < entities_max);

    if(ent->db_id>0)
    {
        ref.UID = ent->db_id | DB_ID_BIT;
    }
    else
    {
        ref.UID = ent->id & ~DB_ID_BIT;
    }

    ref.owner = ent->owner;

    return ref.ref;
}

/**********************************************************************func*
 * Function erGetEnt()
 *  Given an entity reference, this function returns the corresponding
 *  entity if the entity still exists.
 *
 *  Returns:
 *      Valid Entity* - if the entity still exists.
 *      NULL - the entity no longer exists.
 */
Entity* erGetEnt(EntityRef value)
{
    EntityRefImp ref;
    ref.ref = value;
    if(ref.owner != 0
        && ref.owner < (U32)entities_max
        && entity_state[ref.owner] & ENTITY_IN_USE
        && entities[ref.owner])
    {
        if(ref.UID & DB_ID_BIT)
        {
            int db_id = (ref.UID & ~DB_ID_BIT);

            if(entities[ref.owner]->db_id==db_id)
            {
                return entities[ref.owner];
            }
            else
            {
                return entFromDbId(db_id);
            }
        }
        else if ((entities[ref.owner]->id & ~DB_ID_BIT)==ref.UID)
        {
            return entities[ref.owner];
        }
    }

    return NULL;
}

/***************************************************************************
 * erGetEntID
 *
 */
S32 erGetEntID(EntityRef value)
{
    EntityRefImp ref;
    ref.ref = value;
    if(ref.owner != 0
        && ref.owner < (U32)entities_max
        && entity_state[ref.owner] & ENTITY_IN_USE
        && entities[ref.owner])
    {
        if(ref.UID & DB_ID_BIT)
        {
            int db_id = (ref.UID & ~DB_ID_BIT);

            if(entities[ref.owner]->db_id==db_id)
            {
                return ref.owner;
            }
            else
            {
                Entity* ent = entFromDbId(db_id);
                return ent ? ent->owner : 0;
            }
        }
        else if ((entities[ref.owner]->id & ~DB_ID_BIT)==ref.UID)
        {
            return ref.owner;
        }
    }

    return 0;
}

/**********************************************************************func*
 * Function erGetDbId()
 *  Given an entity reference, this function returns the corresponding
 *  db_id if the entity was a player.
 *
 *  Returns:
 *      db_id of entref, or
 *      0 if not a player
 *
 */
S32 erGetDbId(EntityRef value)
{
    EntityRefImp ref;
    ref.ref = value;
    if(ref.UID & DB_ID_BIT)
    {
        return ref.UID & ~DB_ID_BIT;
    }

    return 0;
}

/***************************************************************************
 * erGetRefString
 *
 * Returns a string representing the entref for an entity.
 *
 * The string returned is from a small static pool of strings. You can use
 *   the returned string and call this function again immediately and you'll
 *   get a different buffer, but only few times before the buffers are reused.
 *
 * If your going to keep the string around, copy it.
 *
 */
char* erGetRefString(Entity* ent)
{
#define MAX_STRREFS        8
#define MAX_STRREF_LEN 30

    static char buf[MAX_STRREFS][MAX_STRREF_LEN];
    static int idx = 0;

    EntityRef er = erGetRef(ent);

    idx++;
    if(idx>=MAX_STRREFS)
        idx=0;

    sprintf(buf[idx], "%I64x", er);

    return buf[idx];
}

/***************************************************************************
 * erGetEntFromString
 *
 */
Entity* erGetEntFromString(const char* refstring)
{
    EntityRef er = 0;
    sscanf(refstring, "%I64x", &er);
    return erGetEnt(er);
}


/* End of File */
