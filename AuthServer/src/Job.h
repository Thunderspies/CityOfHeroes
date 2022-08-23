// Job.h: interface for the CJob class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOB_H__7E288E30_F7C2_4FA0_8C77_3A7C0AEB0FAD__INCLUDED_)
#define AFX_JOB_H__7E288E30_F7C2_4FA0_8C77_3A7C0AEB0FAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalAuth.h"
#include "Lock.h"
#include "IOObject.h"

#define TOP_TICK_EXPIRE 24*3600*1000
//#define TOP_TICK_EXPIRE 1000
class CIOTimer 
{
public:
    DWORD   m_tick;
    CIOObject *m_pObject;
    CIOTimer(CIOObject *pObject, DWORD tick) : m_tick(tick), m_pObject(pObject) {}
    bool operator<(const CIOTimer& right) const 
    {
        return (LONG)+(m_tick - right.m_tick) > 0;
    }
};

class CIOTopTimer : public CIOObject
{
public:
    CIOTopTimer(){}
    ~CIOTopTimer(){}
    virtual void OnTimerCallback(void);
    virtual void OnEventCallback() {}
    virtual void OnIOCallback(BOOL vector, LPOVERLAPPED pOverlapped,DWORD transferred ) {}
};

typedef std::vector<CIOTimer> Timervector;
typedef std::priority_queue<CIOTimer, Timervector> PriorityTimerQueue;


class CJob  
{
public:
    CJob();
    ~CJob();
    BOOL PushTimer(CIOObject *pObject, DWORD tick, BOOL setEvent);
    BOOL PushEvent(HANDLE handle, CIOObject *pObject);
    void SetTerminate();
    void RunTimer();
    void RunEvent();

private:
    CIOTopTimer *pIOTopTimer;
    CLock m_lock;
    DWORD m_topTick;
    HANDLE m_timerEvent;
    HANDLE m_terminateEvent;
    std::vector<HANDLE> m_handles;
    std::vector<CIOObject *> m_objects;
    PriorityTimerQueue m_timerQueue;
    BOOL m_terminating;
};

extern CJob job;
#endif // !defined(AFX_JOB_H__7E288E30_F7C2_4FA0_8C77_3A7C0AEB0FAD__INCLUDED_)
