/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 *
 * Module Description:
 *
 *
 ***************************************************************************/
#pragma once

#include "ncstd.h"
NCHEADER_BEGIN

#if _DEBUG
#define DEBUG_NCARRAY_HANDLES 1
#endif

#define VERBOSE_HANDLE_TRACKING 0
#if VERBOSE_HANDLE_TRACKING
#define ARRAY_LOG(FMT,...) LOG(FMT,__VA_ARGS__)
#else
#define ARRAY_LOG(FMT,...)
#endif

typedef enum ArrayFlags
{
    ARRAYFLAG_ALLOCA    = (1<<0),
} ArrayFlags;

typedef struct ArrayHdr
{
    int size;
    int cap;
    U32 code;
    U32 flags;
} ArrayHdr;
STATIC_ASSERT(!(sizeof(ArrayHdr)%sizeof(void*))); // alignment

#define NCARRAY_MEMSIZE(CAP) ((CAP)*(sizeof(TYPE_T))+sizeof(ArrayHdr))
#define NCARRAY_FROM_T(P) (P?((ArrayHdr*)(((U8*)(P))- sizeof(ArrayHdr))):0)
#define T_FROM_NCARRAY(P) (P?((TYPE_T*)(((U8*)(P)) + sizeof(ArrayHdr))):0)
#define ARRAY_MIN_CAPACITY(TYPE_SIZE) ((64 - sizeof(ArrayHdr) + (TYPE_SIZE) - 1)/(TYPE_SIZE))

// don't ask, but:
// 1. TYPE_FUNC_PREFIX => TFP (an argument)
// 2. pass TFP into 3 evals the argument, turning it into e.g. foo
// 3. foo concatted with func
#define ADECL3(FUNC,PREFIX) PREFIX ## _ ## FUNC
#define ADECL2(FUNC,PREFIX) ADECL3(FUNC,PREFIX)
#define ADECL(FUNC) ADECL2(FUNC,TYPE_FUNC_PREFIX)
#define S_ADECL(FUNC) ADECL(FUNC ## _s)


// *************************************************************************
//  void* Array
// *************************************************************************

#define ap_temp()                                ap_init_dbg(malloc_stack(1024), 1024, ARRAYFLAG_ALLOCA DBG_PARMS_INIT)
#define ap_create(CAP)                            ap_create_dbg(CAP DBG_PARMS_INIT)
/* These adapters accept addresses of typed pointer arrays. The storage API
 * updates the caller's handle; element destruction keeps its existing contract.
 */
#define ap_destroy(handle, ap_destroyelt_fp) \
	ap_destroy_dbg((void***)(handle), ap_destroyelt_fp DBG_PARMS_INIT)
#define ap_size(handle) \
	ap_size_dbg((void* const *const *)(handle) DBG_PARMS_INIT)
#define ap_push(handle,ptr) \
	ap_push_by_cp_dbg((void***)(handle),ptr DBG_PARMS_INIT)
#define ap_pop(handle) ap_pop_dbg((void***)(handle) DBG_PARMS_INIT)
#define ap_setsize(h,size)                        ap_setsize_dbg(h,size DBG_PARMS_INIT)
#define ap_cp(hdest,hsrc)                        ap_cp_dbg(hdest,hsrc,0 DBG_PARMS_INIT)
#define ap_rm(handle, offset, num)                ap_rm_dbg(handle, offset, num  DBG_PARMS_INIT)
#define ap_append(handle, src, num)                ap_append_dbg(handle, src, num  DBG_PARMS_INIT)
#define ap_top(handle)                            (ap_size(handle) ? (*handle)[ap_size(handle)-1] : NULL)
#define ap_get(handle, i)                        ((i) >= 0 && (i) < ap_size(handle) ? (*handle)[i] : NULL)

#define TYPE_T void*
#define TYPE_FUNC_PREFIX ap
#include "Array_def.h"
#undef TYPE_T
#undef TYPE_FUNC_PREFIX

// *************************************************************************
// char array
// *************************************************************************

#define achr_temp() achr_init_dbg(malloc_stack(1024), 1024, ARRAYFLAG_ALLOCA DBG_PARMS_INIT)
#define achr_create(CAP) achr_create_dbg(CAP DBG_PARMS_INIT)
#define achr_destroy(handle) achr_destroy_dbg(handle,0 DBG_PARMS_INIT)
#define achr_size(handle) \
	achr_size_dbg((char const *const *)(handle) DBG_PARMS_INIT)
#define achr_inrange(handle,I) \
	(I >= 0 && I < achr_size_dbg((char const *const *)(handle) \
	DBG_PARMS_INIT))
#define achr_push(handle,c) achr_push_by_cp_dbg(handle,c DBG_PARMS_INIT)
#define achr_pushn(handle,n) achr_pushn_dbg(handle, n DBG_PARMS_INIT)
#define achr_pop(handle) achr_pop_dbg(handle DBG_PARMS_INIT)
#define achr_setsize(handle, size) achr_setsize_dbg(handle,size DBG_PARMS_INIT)
#define achr_cp(hdest, hsrc, n) achr_cp_dbg(hdest, hsrc, n DBG_PARMS_INIT)
#define achr_ncp(hdest, src, n) achr_cp_raw_dbg(hdest, src, n DBG_PARMS_INIT)
#define achr_rm(handle, offset, n) achr_rm_dbg(handle,offset,n DBG_PARMS_INIT)
#define achr_insert(hdest, src, i, n) achr_insert_dbg(hdest, src, i, n  DBG_PARMS_INIT)
#define achr_append(handle, src, n ) achr_append_dbg(handle, src,n  DBG_PARMS_INIT)
#define achr_mv(pdst, handle, n) do { memmove(pdst,*handle,n); achr_rm(handle,0,n); } while(0)
#define achr_pushfront(ha) achr_pushfront_dbg(ha  DBG_PARMS_INIT) // expensive!!!


// helpers
#define achr_clear(handle) (void)(*handle ? (achr_setsize(handle,0),0) : 0)
#define achr_get(handle, i ) ((handle && i >= 0 && i < achr_size(handle))? ((*handle)[i]) : 0)

#define TYPE_T char
#define TYPE_FUNC_PREFIX achr
#include "Array_def.h"
#undef TYPE_T
#undef TYPE_FUNC_PREFIX

// *************************************************************************
// int array
// *************************************************************************

#define aint_temp() aint_init_dbg(malloc_stack(1024), 1024, ARRAYFLAG_ALLOCA DBG_PARMS_INIT)
#define aint_create(CAP) aint_create_dbg(CAP DBG_PARMS_INIT)
/* Signed and unsigned 32-bit arrays share these integer storage operations. */
#define aint_destroy(handle) \
	aint_destroy_dbg((int**)(handle), NULL DBG_PARMS_INIT)
#define aint_size(handle) \
	aint_size_dbg((int const *const *)(handle) DBG_PARMS_INIT)
#define aint_push(handle,val) \
	aint_push_by_cp_dbg((int**)(handle),val DBG_PARMS_INIT)
#define aint_pop(handle) aint_pop_dbg((int**)(handle) DBG_PARMS_INIT)
#define aint_setsize(handle, size) \
	aint_setsize_dbg((int**)(handle),size DBG_PARMS_INIT)
#define aint_cp(hdest, hsrc, n) \
	aint_cp_dbg((int**)(hdest), (int const *const *)(hsrc), n \
	DBG_PARMS_INIT)

#define TYPE_T int
#define TYPE_FUNC_PREFIX aint
#include "Array_def.h"
#undef TYPE_T
#undef TYPE_FUNC_PREFIX

// Flat struct hack ////////////////////////////////////////////////////////////
#ifndef __cplusplus
typedef struct ArrayHack
{
    int count;
    int capacity;
} ArrayHack;
STATIC_ASSERT(!(sizeof(ArrayHack)%sizeof(void*))); // alignment
#define as_head(as)    ((*(ArrayHack**)as)-1)
#define as_size(as)    ((*as) ? as_head(as)->count : 0)
#define as_top(as)    (as_size(as) ? ((*as)+as_head(as)->count-1) : NULL)
#define as_push(as)    (as_dopush((void**)(as), sizeof(**as)), as_top(as))
#define as_pop(as)    (as_head(as)->count--)
#define as_popall(as) ((*as) ? as_head(as)->count = 0 : 0)
static COH_FORCEINLINE void as_dopush(void **pas, size_t size)
{
    ArrayHack *ah = *pas ? as_head(pas) : calloc(1, sizeof(ArrayHack));
    if(ah->count == ah->capacity)
        ah = realloc(ah, sizeof(ArrayHack) + (++ah->capacity)*size);
    memset((U8*)ah + sizeof(ArrayHack) + (ah->count++)*size, 0, size);
    *pas = ah+1;
}
#endif /////////////////////////////////////////////////////////////////////////

NCHEADER_END
