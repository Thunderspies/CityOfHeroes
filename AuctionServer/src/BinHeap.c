/***************************************************************************
 *     Copyright (c) 2006-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 * $Workfile: $
 * $Revision: 1.36 $
 * $Date: 2006/02/07 06:13:59 $
 *
 * Module Description:
 *
 * Revision History:
 *
 ***************************************************************************/
#include "BinHeap.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>

// *********************************************************************************
//  Binary Heap: binary structure where 
// - the parent of a node is <= current node.
// - always balanced
// - insert/remove log(N)
// - determine least value: constant time
// - contiguous storage
//
// stored in an earray where for elt N at index i I(N->left) = 2*i. I(N->right) = 2*i+1
// *********************************************************************************

MP_DEFINE(BinHeap);
BinHeap* binheap_Create( BinHeapCmpFp cmp)
{
    BinHeap *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(BinHeap, 64);
    res = MP_ALLOC( BinHeap );
    if( verify( res ))
    {
        res->cmp = cmp;
    }
    return res;
}

void binheap_Destroy( BinHeap *hItem )
{
    if(hItem)
    {
        eaDestroy(&hItem->elts);
        MP_FREE(BinHeap, hItem);
    }
}

BinHeap *binheap_Copy( BinHeap *hItem )
{
    BinHeap *res = NULL;

    if(hItem)
    {
        res = binheap_Create(hItem->cmp);
        eaCopy(&res->elts, &hItem->elts);
    }
    return res;
}

// utils to abstract out the 1-based math
#define BH_LEFT(I) (((I+1)*2)-1)
#define BH_RIGHT(I) (BH_LEFT(I)+1)
#define BH_PARENT(I) (((I+1)/2)-1)
#define BH_SWAP(BH,I,J) { void *tmp = (BH)->elts[(I)]; \
 (BH)->elts[(I)] = (BH)->elts[(J)]; \
 (BH)->elts[(J)] = tmp; }

void *binheap_Top(BinHeap *bh)
{
    return eaSizeUnsafe(&bh->elts) ? bh->elts[0] : NULL;
}

static void s_binheap_PushFix(BinHeap *bh, int i)
{
    while(i)
    {
        int iParent = BH_PARENT(i);
        if(bh->cmp(bh->elts[iParent],bh->elts[i]) > 0)
        {
            BH_SWAP(bh,i,iParent);
        }
        else
        {
            break;
        }
        i = iParent;
    }
}

void binheap_Push(BinHeap *bh, void *elt)
{
    int i;

    if(!bh||!elt)
        return;

    i = eaPush(&bh->elts,elt);
    s_binheap_PushFix(bh,i);
}

void *binheap_Peek(BinHeap *bh, int i)
{
    if(!INRANGE0(i,eaSizeUnsafe(&bh->elts)))
        return NULL;

    return bh->elts[i];
}

void *binheap_Remove(BinHeap *bh, int i)
{
    void *res;

    if(!INRANGE0(i,eaSizeUnsafe(&bh->elts)))
        return NULL;

    res = bh->elts[i];

    for(;;)
    {
        int ileft = BH_LEFT(i);
        int iright = BH_RIGHT(i);
        int iswap = 0;
        void *cl = eaGet(&bh->elts,ileft);
        void *cr = eaGet(&bh->elts,iright);

        if(!cl&&!cr)
            break;
        else if(!cl)
            assertmsg(0,"cr but no cl");
        else if(!cr || bh->cmp(cl,cr) < 0 )
            iswap = ileft;
        else
            iswap = iright;
        bh->elts[i] = bh->elts[iswap];
        i = iswap;
    }

    if(eaSizeUnsafe(&bh->elts) > i+1)
    {
        // we made a gap, close it with the last element
        bh->elts[i] = eaPop(&bh->elts);
        s_binheap_PushFix(bh,i);
    }
    else
    {
        eaPop(&bh->elts);
    }

    return res;
}

void *binheap_RemoveElt(BinHeap *bh, void *elt)
{
    return binheap_Remove(bh,eaFind(&bh->elts,elt));
}


void *binheap_Pop(BinHeap *bh)
{
    return binheap_Remove(bh,0);
}

int binheap_Find(BinHeap *bh,void *elt)
{
    return eaFind(&bh->elts,elt);
}

int binheap_Size(BinHeap *bh)
{
    return eaSizeUnsafe(&bh->elts);
}

// testing only
int binheap_IntCmp(void *lhs, void *rhs)
{
    intptr_t a = (intptr_t)lhs;
    intptr_t b = (intptr_t)rhs;
    return (int)(b - a);
}

bool binheap_Test()
{
    int t1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, };
    int t2[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, };
    int t3[] = {10, 9, 8, 3, 2, 7, 6, 5, 4, 1, };
    intptr_t answer[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, };
    int i;
    struct
    {
        BinHeapCmpFp fp;
        int *elts;
    } tmp = 
        {
            binheap_IntCmp,
            NULL,
        };
    
    BinHeap *bh = (BinHeap*)&tmp; //binheap_Create(binheap_IntCmp);

    for( i = 0; i < ARRAY_SIZE(t1); ++i ) 
        binheap_Push(bh, S32_TO_PTR(t1[i]));
    if(((intptr_t)binheap_Top(bh)) != answer[0])
    {
        printf("heap top failed\n");
        return false;
    }
    
    for( i = 0; i < ARRAY_SIZE( answer ); ++i ) 
    {
        if(((intptr_t)binheap_Pop(bh)) != answer[i])
        {
            printf("t1 pop heap failed %i\n",i);
            return false;
        }
    }
    
    if(binheap_Size(bh)!=0)
    {
        printf("size not zero\n");
        return false;
    }

    for( i = 0; i < ARRAY_SIZE( t2 ); ++i )
        binheap_Push(bh,S32_TO_PTR(t2[i]));
    for( i = 0; i < ARRAY_SIZE( answer ); ++i ) 
    {
        if(((intptr_t)binheap_Pop(bh)) != answer[i])
        {
            printf("t2 pop heap failed\n");
            return false;
        }
    }

    if(binheap_Size(bh)!=0)
    {
        printf("size not zero\n");
        return false;
    }

    for( i = 0; i < ARRAY_SIZE( t3 ); ++i )
        binheap_Push(bh,S32_TO_PTR(t3[i]));
    for( i = 0; i < ARRAY_SIZE( answer ); ++i ) 
    {
        if(((intptr_t)binheap_Pop(bh)) != answer[i])
        {
            printf("t3 pop heap failed\n");
            return false;
        }
    }

    printf("binheap test: success\n");
    return true;
}
