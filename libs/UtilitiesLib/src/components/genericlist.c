#define GENERICLIST_IMPL
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/memcheck.h"

#define DEBUG_GENERIC_LIST
#ifdef DEBUG_GENERIC_LIST
int total_list_nodes_alloced = 0;
#endif


//############################################################
//Generic Linked List Manager ############################################
//Assumes that the first two elements of an object are the next and prev ptrs
//TO DO: maintain a ptr to the end of the list ( the first item added )

typedef struct ListCast
{
    void * next;
    void * prev;
} ListCast;

/*Zeros out a list member while preserving the linked list pointers
*/
void listScrubMember(void * member, int size)
{
    ListCast * listcastmember;
    ListCast * tempnext;
    ListCast * tempprev;

    if(!member)
        return;

    listcastmember = (ListCast*)member;

    tempnext  = listcastmember->next;
    tempprev  = listcastmember->prev;

    memset(listcastmember, 0, size);

    listcastmember->next  = tempnext;
    listcastmember->prev  = tempprev;
}

/*Copies one list member to another list member while preserving the receiving
item's linked list pointers
*/
void listCopyItem(void * target, void * source, int size)
{
    ListCast * listtarget;
    ListCast * tempnext;
    ListCast * tempprev;

    if( !target || !source || !size )
        return;

    listtarget = (ListCast*)target;

    tempnext  = listtarget->next;
    tempprev  = listtarget->prev;

    memcpy(target, source, size);

    listtarget->next  = tempnext;
    listtarget->prev  = tempprev;
}

/*given a ptr to a ptr to first item in the list and the item to be attached, adds an already created member
making it the list head. Returns ptr to newly added item (headptr also points there)
*/
void * listAddForeignMember(void ** headptr, void * foreignlink)
{
    ListCast * listhead;
    ListCast * newlink;

    if(!foreignlink)
        return 0;
    listhead = (ListCast*)*headptr;

    newlink = (ListCast*)foreignlink;

    newlink->next = (ListCast *)*headptr;
    newlink->prev = 0;

    if(listhead)
        listhead->prev = newlink;

    //hook front up to fx list
    *headptr = newlink;

    return newlink;
}

/*given a ptr to a ptr to first item in the list and the size of an item, adds a new member
making it the list head. Returns ptr to newly added item (headptr also points there) Size
is size of the node structure
*/
void * listAddNewMember_dbg(void ** headptr, int size MEM_DBG_PARMS)
{
    ListCast * newlink;
    ListCast * listhead;

    listhead = (ListCast*)*headptr;

    newlink = (ListCast*)smalloc(size);
    if(!newlink)
        return 0;

    memset(newlink, 0, size);

    newlink->next = (ListCast *)*headptr;
    newlink->prev = 0;

    if(listhead)
        listhead->prev = newlink;

    //hook front up to fx list
    *headptr = newlink;

#ifdef DEBUG_GENERIC_LIST
total_list_nodes_alloced++;
#endif

    return newlink;
}

/*Remove memeber from the list without freeing it*/
void listRemoveMember(void * member, void ** headptr)
{
    ListCast * oldlink;
    ListCast * listhead;
    ListCast * next;
    ListCast * prev;

    listhead = (ListCast*)*headptr;
    oldlink  = (ListCast*)member;

    if(!oldlink)
        return;

    if(oldlink == listhead)
    {
        *headptr = oldlink->next;
    }

    if(oldlink->next)
    {
        next = (ListCast *)oldlink->next;
        next->prev = oldlink->prev;
    }
    if(oldlink->prev)
    {
        prev = (ListCast *)oldlink->prev;
        prev->next = oldlink->next;
    }
    oldlink->next = 0;
    oldlink->prev = 0;
}

/*Remove and free member*/
void listFreeMember(void * member, void ** headptr)
{
    listRemoveMember(member,headptr);
#ifdef DEBUG_GENERIC_LIST
total_list_nodes_alloced--;
#endif
    free(member);
}

/*free all members of the list*/
void listClearList(void ** headptr)
{
    ListCast * listhead;
    ListCast * node;
    ListCast * next;

    listhead = (ListCast*)*headptr;

    for(node = listhead ; node ; )
    {
        next = node->next;
        free(node);
        node = next;
    }
    *headptr = 0;
}

/*free all members of the list with a given destructor*/
void listDestroyList(void ** headptr, void (*pfnDestroy)(void *pv))
{
    ListCast * listhead;
    ListCast * node;
    ListCast * next;

    listhead = (ListCast*)*headptr;

    for(node = listhead ; node ; )
    {
        next = node->next;
        if(pfnDestroy)
        {
            pfnDestroy(node);
        }
        else
        {
            free(node);
        }

        node = next;
    }
    *headptr = 0;
}

/*Check validity of list*/
int listCheckList(void ** headptr, int count)
{
    ListCast * listhead;
    ListCast * node;
    ListCast * next;
    ListCast * prev;
    int realcount = 0;

    listhead = (ListCast*)*headptr;

    if(!listhead)
    {
        assert(!count);
        return 1;
    }

    for(node = listhead ; node ; node = node->next)
    {
        realcount++;
        if(node->next)
        {
            next = (ListCast *)node->next;
            assert(next->prev = node);
        }
        if(node->prev)
        {
            prev = (ListCast *)node->prev;
            assert(prev->next = node);
        }
    }
    assert(count == realcount);
    return 1;
}

int listLength(void *headptr) {
    int ret=0;
    ListCast *walk=headptr;
    for (; walk; ret++, walk=(ListCast*)walk->next);
    return ret;
}

void listQsort(void ** headptr, int (__cdecl *compare )(const void *, const void *)) {
    int len = listLength(*headptr);

    if (len <=1) {
        return;
    }

    {
        ListCast **ptrarray = calloc(len, sizeof(void*));
        int i=0;
        ListCast *walk=*headptr;


        while(walk) {
            ptrarray[i]=walk;
            i++;
            walk=(ListCast*)walk->next;
        }

        qsort(ptrarray, len, sizeof(void*), compare);

        ptrarray[0]->prev = NULL;
        ptrarray[0]->next = ptrarray[1];
        for (i=1; i<len-1; i++) {
            ptrarray[i]->prev = ptrarray[i-1];
            ptrarray[i]->next = ptrarray[i+1];
        }
        ptrarray[len-1]->prev = ptrarray[len-2];
        ptrarray[len-1]->next = NULL;
        *headptr = ptrarray[0];
        free(ptrarray);
    }

}

int listInList(void *headptr, void *member)
{
    ListCast *walk=headptr;
    while (walk) {
        if (walk==member)
            return 1;
        walk = walk->next;
    }
    return 0;
}

// due to the nature of insertion sort, it is very quick on a list that is mostly sorted, but 
// for its worst case (a list thats completely backwards) it is O(n^2)
void *listInsertionSort( void *list, int (*compare)(void *, void *) )
{
    ListCast *cur = (ListCast*)list;
    ListCast *head = cur;

    while ( cur )
    {
        ListCast *insert = cur->prev;
        // iterate from the current node backwards until you find where this node should be inserted
        while ( insert && compare(insert, cur) == 1)
        {
            insert = insert->prev;
        }
        // if the place to insert is different than the node's current location
        if ( insert != cur->prev )
        {
            ListCast * temp = cur->next;
            
            // if we are at the beginning of the list and the cur node is not the head pointer
            if ( !insert && cur->prev )
            {
                if ( cur->next )
                    ((ListCast*)cur->next)->prev = cur->prev;
                ((ListCast*)cur->prev)->next = cur->next;
                cur->next = head;
                cur->prev = 0;
                head->prev = cur;
                head = cur;
                cur = temp;
            }
            // if there is a place to insert the cur node
            else if ( insert )
            {
                // save the insert's next pointer
                ListCast *in_next = insert->next;
                // this will probably never be false, since the iteration begins at
                // cur->prev and goes to the beginning of the list
                if ( insert->next )
                {
                    ((ListCast*)insert->next)->prev = cur;
                    insert->next = cur;
                }

                if ( cur->next )
                    ((ListCast*)cur->next)->prev = cur->prev;
                ((ListCast*)cur->prev)->next = cur->next;

                cur->next = in_next;
                cur->prev = insert;
                cur = temp;
            }
        }
        else
            cur = cur->next;
    }
    return head;
}


