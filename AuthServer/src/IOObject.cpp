// IOObject.cpp: implementation of the CIOObject class.
//
//////////////////////////////////////////////////////////////////////
#include "GlobalAuth.h"
#include "IOObject.h"
#include "Job.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIOObject::CIOObject()
{
    refcount = 1;
}

CIOObject::~CIOObject()
{
    _ASSERTE(refcount == 0);
}

int CIOObject::AddRef( void ) 
{
    InterlockedIncrement( &refcount );
    return refcount;
}

void CIOObject::ReleaseRef( void )
{
    if ( InterlockedDecrement( &refcount ) == 0 )
        delete this;
}

BOOL CIOObject::PostObject( int dwDataSize, HANDLE completionPort )
{
    return PostQueuedCompletionStatus( completionPort, dwDataSize, (ULONG_PTR)this, NULL );
}

BOOL CIOObject::RegisterTimer( UINT waitTime, BOOL setEvent )
{
    if(job.PushTimer(this, waitTime, setEvent)) {
        AddRef();
        return TRUE;
    } else {
        return FALSE;
    }

}

bool CIOObject::RegisterEvent( HANDLE hEvent )
{
    if ( job.PushEvent( hEvent, this ))
        return true;
    else
        return false;
}

void CIOObject::OnIOCallback( BOOL bSuccess, DWORD dwTransferred, LPOVERLAPPED lpOverlapped )
{
    return;
}

void CIOObject::OnTimerCallback(void)
{
    ReleaseRef();
    return;
}

void CIOObject::OnEventCallback( void )
{
}
