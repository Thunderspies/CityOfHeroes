/*\
 *
 *    container_store.h/c - Copyright 2004, 2005 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *  Offers a standard container store - an EArray of containers based on a template
 *
 */

#ifndef CONTAINER_STORE_H
#define CONTAINER_STORE_H

#include <utilitieslib/stdtypes.h>
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
typedef struct StructDesc StructDesc;
typedef struct ContainerInfo ContainerInfo;
typedef struct Packet Packet;

// **************************************** container store
// hash table of container data, with functions for create and delete
// automatically obeys container server rules for uniqueness of container ids

// merge with DbList sometime?
typedef struct ContainerStore
{
    StashTable    items;
    U32                    max_session_id;        // high-water mark of id's touched while dbserver is running

    U32                    list_id;
    char*                name;                // name of store for debug purposes
    StructDesc*            struct_desc;        // pack/unpack description

    void*                (*create)(U32);        // (U32 cid)
    void                (*destroy)(void*);    // (void* container)    
    void                (*clear)(void*,U32);// (void* container, U32 cid)
} ContainerStore;

void* cstoreAdd(ContainerStore* cstore);
void* cstoreGetAdd(ContainerStore* cstore, U32 cid, int add);
#define cstoreGet(store, id) cstoreGetAdd((store), (id), 0)
void cstoreDelete(ContainerStore* cstore, U32 cid);
int cstoreCount(ContainerStore* cstore);
void cstoreForEach(ContainerStore* cstore, void (*func)(void*, U32));
void cstoreClearAll(ContainerStore *cstore);

char *cstorePackage(ContainerStore* cstore, void* container, U32 cid);
void cstoreUnpack(ContainerStore* cstore, void* container, char* mem, U32 cid);
char* cstoreTemplate(ContainerStore* cstore);
char* cstoreSchema(ContainerStore* cstore, char *name);
void cstoreHandleUpdate(ContainerStore* cstore, ContainerInfo* ci);
void cstoreHandleReflectUpdate(ContainerStore* cstore, int* ids, int deleting, Packet* pak);

// ********************************************** simple container struct interface
// ok, so obviously I'm going overboard with this, but I keep typing the same damn
// functions over and over again..
// - use if you just want your structures in a memory pool, you must have a StructDesc named Struct_desc

#define CONTAINERSTRUCT_INTERFACE(Struct) \
    Struct* Struct##Create(U32 cid); \
    void Struct##Destroy(Struct* container); \
    void Struct##Clear(Struct* container, U32 cid); \
    extern ContainerStore g_##Struct##Store[]; 

#define CONTAINERSTRUCT_IMPL(Struct, ListId) \
    static void* Struct##CreateCallback(U32 cid) { return Struct##Create(cid); } \
    static void Struct##DestroyCallback(void* container) { Struct##Destroy((Struct*)container); } \
    static void Struct##ClearCallback(void* container, U32 cid) { Struct##Clear((Struct*)container, cid); } \
    ContainerStore g_##Struct##Store[] = \
    { 0, 0, ListId, #Struct, Struct##_desc, Struct##CreateCallback, Struct##DestroyCallback, Struct##ClearCallback}; \
    \
    MP_DEFINE(Struct); \
    Struct* Struct##Create(U32 cid)    \
    {    \
        Struct* ret; \
        MP_CREATE(Struct, 100);        \
        ret = MP_ALLOC(Struct);        \
        ret->id = cid;                \
        return ret;                    \
    }    \
    void Struct##Destroy(Struct* container)    \
    {    \
        MP_FREE(Struct, container); \
    }    \
    void Struct##Clear(Struct* container, U32 cid) \
    {    \
        memset(container, 0, sizeof(*container));    \
        container->id = cid;    \
    }    


#endif // CONTAINER_STORE_H