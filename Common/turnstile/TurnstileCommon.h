/*
*
*    TurnstileCommon.h - Copyright 2010 NC Soft
*        All Rights Reserved
*        Confidential property of NC Soft
*
*    Turnstile system for matching players for large raid content
*
*/

#ifndef TURNSTILE_COMMON_H
#define TURNSTILE_COMMON_H

typedef struct StaticDefineInt StaticDefineInt;

#define    MAX_EVENT    1000            // Maximum event count in certain places.  If we exceed this limit, asserts will start tripping, in which case
                                    // you just increase this to fix the problem

typedef enum
{
    kTurnstileCategory_Trial,                // standard 1-50 content
    kTurnstileCategory_IncarnateTrial,        // post-50 content
    kTurnstileCategory_HolidayTrial,        // special seasonal content
    kTurnstileCategory_TaskforceContact,    // task force contact
    kTurnstileCategory_Count,
} TurnstileCategory;

extern StaticDefineInt ParseTurnstileCategory[];

#endif // TURNSTILE_SERVER_COMMON_H