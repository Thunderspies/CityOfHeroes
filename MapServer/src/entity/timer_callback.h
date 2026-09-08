/***************************************************************************
 *     Copyright (c) 2000-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef TIMER_CALLBACK_H__
#define TIMER_CALLBACK_H__

#define MAX_ENTLIST_LEN ( 50 )

typedef struct AlterWork
{
    Entity      *owner_ent;
    int         owner_db_id;
    Entity     *relative_to_me;
    Vec3        posn;
    int         entlist[ MAX_ENTLIST_LEN ];
    int         duration;
    int         radius;
    int         dmg;
    int         arm_dmg;
    int         leg_dmg;
    int         success_rating;
} AlterWork;


typedef struct MemPool
{
    void    *free_list;
    void    *storage;
} MemPool;

typedef struct TimerCallback
{
    void       *data;
    int         (* code )( void *go );  //gets passed pointer to data.  Returns 0 if ready to be killed.
    MemPool    *mp;                     //data for universal memory allocate and free.
    int         next;                   //singly linked list.
    float       timer;
} TimerCallback;


extern MemPool  alter_work_mem_pool;

extern TimerCallback  *timerCreateCallback(int (* code )( void *go ), MemPool *mem_pool);
extern void timerServeCallbacks(void);


#endif /* #ifndef TIMER_CALLBACK_H__ */

/* End of File */

