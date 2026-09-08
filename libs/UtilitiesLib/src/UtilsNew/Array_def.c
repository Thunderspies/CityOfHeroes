/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 *
 * Module Description: an X macro
 * TYPE_T : defined to the type
 * TYPE_T_CODE : the four byte code identifying this array. 
 *               see MAKE_TYPE_CODE()
 * TYPE_FUNC_PREFIX : e.g. achr, ap, etc.
 * e.g.
   #define TYPE_T Foo
   #define TYPE_FUNC_PREFIX foo
 * when done:
   #undef TYPE_T
   #undef TYPE_FUNC_PREFIX
 ***************************************************************************/
#include "utilitieslib/UtilsNew/ncstd.h"
#include "windows.h"
#include "utilitieslib/UtilsNew/Array_def.h"
#include "utilitieslib/UtilsNew/ncHash.h"
#include "utilitieslib/UtilsNew/Array.h"

#ifndef TYPE_T
#error "TYPE_T not defined"
#endif

#ifndef TYPE_FUNC_PREFIX
#error "TYPE_FUNC_PREFIX not defined"
#endif

#if DEBUG_NCARRAY_HANDLES
static HashPtr *S_ADECL(ncarray_tracker); // every array in existence 
static void S_ADECL(check_handle)(void *P)
{
//     ARRAY_LOG("checking %p",P);
    if(P && !HashPtr_Find(&S_ADECL(ncarray_tracker),P))
        assertmsgf(0,"unknown handle %p passed to Array",P);
}

static void S_ADECL(track_handle)(void *P)
{
    static int ntracks = 0;
    ntracks++;
    if(!P || !HashPtr_Insert(&S_ADECL(ncarray_tracker),P,P,FALSE))
        assertmsgf(0,"already tracking handle %p",P);
    ARRAY_LOG("track(%i) %p\n",ntracks,P);
}

static void S_ADECL(untrack_handle)(void *P)
{
    static int untracks;
    untracks++;
    if(P && !HashPtr_Remove(&S_ADECL(ncarray_tracker),P))
        assertmsgf(0,"not tracking handle %p",P);
    ARRAY_LOG("untrack(%i) %p",untracks,P);
}

    
#       define NCARRAY_TRACK_HANDLE(P) S_ADECL(track_handle)(P);
#       define NCARRAY_UNTRACK_HANDLE(P) S_ADECL(untrack_handle)(P)
#       define NCARRAY_CHECK_HANDLE(P) S_ADECL(check_handle)(P)
#else
#       define NCARRAY_TRACK_HANDLE(P)
#       define NCARRAY_UNTRACK_HANDLE(P)
#       define NCARRAY_CHECK_HANDLE(P)
#endif

static ArrayHdr* S_ADECL(fill)(void *mem, int memsize, U32 flags DBG_PARMS)
{
	ArrayHdr *buf = (ArrayHdr*)mem;
	if(!buf)
		return NULL;
    line;
    caller_fname;
    
	NCARRAY_TRACK_HANDLE(buf);
	buf->size = 0;
	buf->cap = (memsize - sizeof(ArrayHdr))/sizeof(TYPE_T);
#define TOSTR2(X) #X
#define TOSTR(X) TOSTR2(X)
	buf->code = make_type_code_from_str(TOSTR(TYPE_FUNC_PREFIX));
#undef TOSTR
#undef TOSTR2
	buf->flags = flags;
	return buf;
}

// for resizing the ArrayHdr. never shrinks the array
static ArrayHdr* S_ADECL(re_capacity)(ArrayHdr *a, int cap DBG_PARMS) 
{
    if(cap < ARRAY_MIN_CAPACITY(sizeof(TYPE_T)))
       cap = ARRAY_MIN_CAPACITY(sizeof(TYPE_T));

    if(!a)
        return S_ADECL(fill)(NCMALLOC_DBGCALL(NCARRAY_MEMSIZE(cap)), NCARRAY_MEMSIZE(cap), 0 DBG_PARMS_CALL);

    NCARRAY_CHECK_HANDLE(a);
    if(a->cap < cap)
    {
		ArrayHdr *res;
		if(a->flags & ARRAYFLAG_ALLOCA)
		{
			res = (ArrayHdr*)NCMALLOC_DBGCALL(NCARRAY_MEMSIZE(cap));
			memcpy(res, a, NCARRAY_MEMSIZE(a->cap));
		}
		else
		{
			res = (ArrayHdr*)NCREALLOC_DBGCALL(a,NCARRAY_MEMSIZE(cap));
		}
		if(a != res)
		{
			NCARRAY_UNTRACK_HANDLE(a);
			NCARRAY_TRACK_HANDLE(res);
		}
        res->cap = cap;
        return res;
    }
    return a;
}
    
    
ArrayHdr *S_ADECL(setsize_dbg)(TYPE_T** ha, int size DBG_PARMS)
{
    ArrayHdr *res = NULL;
    assert(ha); 
    if(*ha)
        res = NCARRAY_FROM_T(*ha);
    else
        res = S_ADECL(re_capacity)(NULL,size*2 DBG_PARMS_CALL);

    if(res->cap < size)
        res = S_ADECL(re_capacity)(res,size*2 DBG_PARMS_CALL);

    if(res->size < size)
    {
        TYPE_T *elts = T_FROM_NCARRAY(res);
        elts += res->size;
        ClearStructs(elts,size-res->size);
    }
    res->size = size;
    *ha = T_FROM_NCARRAY(res);
    // todo: some fill values here
    return res;
}

// public one. doesn't return ArrayHdr
void ADECL(setsize_dbg)(TYPE_T** ha, int size DBG_PARMS)
{
    S_ADECL(setsize_dbg)(ha,size DBG_PARMS_CALL);
}


TYPE_T* ADECL(create_dbg)(int capacity DBG_PARMS) 
{
    return T_FROM_NCARRAY(S_ADECL(re_capacity)(NULL,capacity DBG_PARMS_CALL));
}

TYPE_T* ADECL(init_dbg)(void *mem, size_t mem_size, U32 flags DBG_PARMS)
{
	if(mem)
	{
		int cap = (mem_size - sizeof(ArrayHdr))/sizeof(TYPE_T);
		ArrayHdr *buf = S_ADECL(fill)(mem, cap, flags DBG_PARMS_CALL);
		assert(mem_size >= sizeof(ArrayHdr)); // we just overflowed mem
		if(buf)
			return T_FROM_NCARRAY(buf);
	}
	return NULL;
}

void ADECL(destroy_dbg)(TYPE_T** ha, ADECL(destroyelt_fp) *fp DBG_PARMS) 
{
    ArrayHdr *a;
    assert(ha);
    a = NCARRAY_FROM_T(*ha);
    NCARRAY_CHECK_HANDLE(a);
    NCARRAY_UNTRACK_HANDLE(a);
    caller_fname;
    line;
	if(!a)
		return;
    if(fp)
    {
        int i;
        for( i = 0; i < a->size; ++i ) 
            fp((*ha)+i);
    }
	if(!(a->flags & ARRAYFLAG_ALLOCA))
		free(a);
    *ha = NULL;
}

int ADECL(size_dbg)(TYPE_T const * const * ha DBG_PARMS) 
{
    ArrayHdr *a;
    caller_fname;
    line;
    assert(ha);
    a = NCARRAY_FROM_T(*ha);        
    NCARRAY_CHECK_HANDLE(a);
    return SAFE_MEMBER(a,size);
}

TYPE_T* ADECL(push_dbg)(TYPE_T **ha DBG_PARMS) 
{
	return ADECL(pushn_dbg)(ha, 1 DBG_PARMS_CALL);
}

TYPE_T* ADECL(pushn_dbg)(TYPE_T **ha, int n DBG_PARMS) 
{
	ArrayHdr *a;
    TYPE_T *res;
	assert(ha);
	a = NCARRAY_FROM_T(*ha);
	NCARRAY_CHECK_HANDLE(a);
    if(n <= 0)
        return NULL;
	a = S_ADECL(setsize_dbg)(ha, SAFE_MEMBER(a,size)+n DBG_PARMS_CALL);
    res = &(*ha)[a->size - n];
    ClearStructs(res,n);
	return res;
}

TYPE_T* ADECL(pushfront_dbg)(TYPE_T **ha DBG_PARMS)
{
	ArrayHdr *a;
    ADECL(push_dbg)(ha DBG_PARMS_CALL);
	a = NCARRAY_FROM_T(*ha);
    memmove(*ha + 1, *ha,(a->size-1)*sizeof(TYPE_T));
    return *ha;
}


int ADECL(push_by_cp_dbg)(TYPE_T **ha, TYPE_T b DBG_PARMS) 
{
    TYPE_T *a;
    a = ADECL(push_dbg)(ha DBG_PARMS_CALL);
    if(!a)
        return -1;
    *a = b;
    return (int)(a - *ha);
}
    
TYPE_T* ADECL(pop_dbg)(TYPE_T **ha DBG_PARMS) 
{
    ArrayHdr *a;
    assert(ha);
    a = NCARRAY_FROM_T(*ha);
    NCARRAY_CHECK_HANDLE(a);
    // if we ever want to track this
    caller_fname;
    line;
        
    if(SAFE_MEMBER(a,size) <= 0)
        return NULL;
    return &(*ha)[--a->size];
}

TYPE_T* ADECL(top_dbg)(TYPE_T **ha DBG_PARMS) 
{
    ArrayHdr *a;
    assert(ha);
    a = NCARRAY_FROM_T(*ha);
    NCARRAY_CHECK_HANDLE(a);
    // if we ever want to track this
    caller_fname;
    line;
        
    if(SAFE_MEMBER(a,size) <= 0)
        return NULL;
    return &(*ha)[a->size-1];
}

void ADECL(cp_raw_dbg)(TYPE_T **hdest,TYPE_T const *s,int n DBG_PARMS) 
{
    ArrayHdr *d;
    assert(hdest && s);
    d = NCARRAY_FROM_T(*hdest);
    NCARRAY_CHECK_HANDLE(d);
    if(n < 0)
        return;
    d = S_ADECL(setsize_dbg)(hdest ,n DBG_PARMS_CALL);
    memmove(*hdest,s,n*sizeof(TYPE_T));
}

void ADECL(cp_dbg)(TYPE_T **hdest,TYPE_T const * const *hsrc,int n DBG_PARMS) 
{
    ArrayHdr *s;
    assert(hdest && hsrc);
    s = NCARRAY_FROM_T(*hsrc);
    if(n == 0)
        n = s->size;
    ADECL(cp_raw_dbg)(hdest,*hsrc,n DBG_PARMS_CALL);
}

void ADECL(insert_dbg)(TYPE_T **hdest, TYPE_T *src, int i, int n DBG_PARMS)
{
    ArrayHdr *d = NCARRAY_FROM_T(*hdest);
    TYPE_T *offset;
    int new_size;
    int remain;
    NCARRAY_CHECK_HANDLE(d);
    if(n <= 0 || i < 0 || i > SAFE_MEMBER(d,size))
        return;
    new_size = n + SAFE_MEMBER(d,size);
    remain = SAFE_MEMBER(d,size) - i;
    d = S_ADECL(setsize_dbg)(hdest,new_size DBG_PARMS_CALL);
    offset = (*hdest)+i;
    memmove(offset+n,offset,remain*sizeof(TYPE_T));
    memmove(offset,src,n*sizeof(TYPE_T));
}

void ADECL(append_dbg)(TYPE_T **hdest, TYPE_T *src, int n DBG_PARMS) 
{
    ArrayHdr *d = NCARRAY_FROM_T(*hdest);
    ADECL(insert_dbg)(hdest,src,SAFE_MEMBER(d,size),n DBG_PARMS_CALL);
}
 
void ADECL(rm_dbg)(TYPE_T **ha, int offset, int n DBG_PARMS)
{
    ArrayHdr *a;
    assert(ha);
    a = NCARRAY_FROM_T(*ha);        
    NCARRAY_CHECK_HANDLE(a);

    // if we ever want to track this
    caller_fname;
    line;
    if(offset < 0 || SAFE_MEMBER(a,size) <= offset || n < 0)
        return;
    n = MIN(n,a->size-offset);
    if(n)
    {
        a->size -= n;
        memmove((*ha)+offset,(*ha)+offset+n,(a->size-offset)*sizeof(TYPE_T));
        ClearStructs((*ha)+a->size,n);
    }        
}

// find using comparator, or memcmp
int ADECL(find_dbg)(TYPE_T **ha, TYPE_T *b, ADECL(cmp_fp) *cmp, void *ctxt DBG_PARMS)
{
    int i;
	for (i = 0;
		i < ADECL(size_dbg)((TYPE_T const *const *)ha DBG_PARMS_CALL);
		++i)
    {
        TYPE_T *a = (*ha)+i;
        if(cmp)
        {
            if(0 == cmp(a,b,ctxt))
            return i;
        }
        else if(0 == memcmp(a,b,sizeof(*a)))
            return i;
    }
    return -1;
}

void ADECL(foreach_ctxt_dbg)(TYPE_T **ha, ADECL(foreach_ctxt_fp) *fp, void *ctxt DBG_PARMS)
{
    int i;
    if(!fp)
        return;
	for (i = 0;
		i < ADECL(size_dbg)((TYPE_T const *const *)ha DBG_PARMS_CALL);
		++i)
        fp(&(*ha)[i],ctxt);
}

void ADECL(foreach_dbg)(TYPE_T **ha, ADECL(foreach_fp) *fp DBG_PARMS)
{
    int i;
    if(!fp)
        return;
	for (i = 0;
		i < ADECL(size_dbg)((TYPE_T const *const *)ha DBG_PARMS_CALL);
		++i)
        fp(&(*ha)[i]);
}


// private:
//     ArrayHdr(int size, int cap) : size(size), cap(cap)
//     {
//         code = type_code;
//     }
//     ~ArrayHdr() {
//     }
//     void* operator new(size_t,void *buf) {
//         return buf;
//     }

//     void operator delete(void * /*buf*/, void * /*p*/) {
//         assertm(0,"don't ever use this code");
//     }
// };

#undef NCARRAY_TRACK_HANDLE
#undef NCARRAY_UNTRACK_HANDLE
#undef NCARRAY_CHECK_HANDLE

#undef TYPE_T 
#undef TYPE_FUNC_PREFIX 
