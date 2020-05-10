/*****************************************************************************
    created:    2001/04/23
    copyright:  2001, NCSoft. All Rights Reserved
    author(s):  Peter M. Freese
    
    purpose:    
*****************************************************************************/

#include "../../include/arda2/core/corFirst.h"
#include "../../include/arda2/core/corErrorHandler.h"
#include <stdio.h>
#include <ctime>

#if CORE_SYSTEM_XENON
#include <Xbdm.h>
#endif

// instantiate static members
errSeverity corErrorHandler::m_defaultFilter = ES_Debug;
errSeverity corErrorHandler::m_dialogBoxMinSeverity = ES_Error;

// this sort of this should be done through a designated initializer list, but MSVC
// doesn't support them...
const char *sErrorStringTable[] =
{
    "DEBUG",            // [ES_Debug]
    "INFO",             // [ES_Info]
    "WARNING",          // [ES_Warning]
    "ERROR",            // [ES_Error]
    "ASSERTION FAILED", // [ES_Assertion]
    "FATAL",            // [ES_Fatal]
};


////////////
// Constructor

corErrorHandler::corErrorHandler() :
    m_filter(ES_Debug),
    m_installed(false)
{
    // empty
}


////////////
// Destructor

corErrorHandler::~corErrorHandler()
{
    // make sure the handler was removed from the handler chain
    if ( m_installed )
        Remove();
}


corErrorHandler::HandlerList &corErrorHandler::GetHandlerList()
{
  static HandlerList theList;
  return theList;
}

void corErrorHandler::SetFilter( errSeverity minSeverity )
{
    m_filter = minSeverity; 
    OnSetFilter();
}


void corErrorHandler::Install()
{
    GetHandlerList().push_back(this);
    m_installed = true;
    OnInstall();
}


void corErrorHandler::Remove()
{
    if ( m_installed)
    {
        // alert the subclass of removal
        OnRemove();

    GetHandlerList().remove(this);

        m_installed = false;
    }
    else
    {
        ERR_REPORT(ES_Error, "Attempt to remove non-installed handler");
    }
}

errHandlerResult corErrorHandler::HandleError( const char* szFileName, const int iLineNumber, 
    errSeverity nSeverity, const char* szDescription)
{
    const char *szSeverity = sErrorStringTable[nSeverity];

    // ES_Assertion or worse (e.g. ES_Fatal) abort
    errHandlerResult result = EH_Ignored;

    // if there are no installed handlers, then call the default one
    if ( GetHandlerList().empty() )
    {
        result = DefaultReport(szFileName, iLineNumber, nSeverity, szSeverity, szDescription);
    }
    else
    {
        // walk the list of error handlers, giving each one a chance to process the error. The list
        // is walked in reverse order to give top level handlers first chance to handle the error
        HandlerList::reverse_iterator iEnd = GetHandlerList().rend();
        for ( HandlerList::reverse_iterator i = GetHandlerList().rbegin(); i != iEnd; i++ )
        {
            corErrorHandler *handler = *i;

            if ( nSeverity >= handler->GetFilter() )
            {
                errHandlerResult r = handler->Report(szFileName, iLineNumber, nSeverity, szSeverity, szDescription);

                if ( r > result )
                    result = r;
            }
        }

    }

    // promote high severities to breaks or aborts
    if ( result <  EH_Break )
    {
        if ( nSeverity >= ES_Fatal )
        {
            result = EH_Abort;
        }
        else if (nSeverity == ES_Assertion )
        {
            result = EH_Break;
        }
    }

    return result;
}


//
// Default error reporting function (static)
//
errHandlerResult corErrorHandler::DefaultReport(const char* szFileName, int iLineNumber, 
        errSeverity nSeverity, const char *szSeverity, const char* szDescription)
{
    if (nSeverity < m_defaultFilter)
    {
        return EH_Ignored;
    }

    errHandlerResult retval = EH_Logged;

    char message[1024];

    static char timeStamp[128] = {0};
    memset(timeStamp,0,128);

#if CORE_SYSTEM_LINUX
    static struct tm t;
    static time_t _t;
    time(&_t);
    localtime_r(&_t,&t);
    strftime(timeStamp,128,"%T",&t);
#elif CORE_SYSTEM_WINAPI
    _strtime(timeStamp);
#endif

    // format a message
    _snprintf(message, sizeof(message), "[%s] %s - %s(%d) : %s", 
        timeStamp, szSeverity, szFileName, iLineNumber, szDescription);
    message[1024 - 1] = 0;

    // don't run this message on the XBox360, we'll just see two
    // printouts in the debugger's output window (which can kill
    // the framerate)
    #if !CORE_SYSTEM_XENON
        // put it on stderr stream, and flush the stream
        fputs(message, stderr);
        fputs("\n", stderr);
        fflush(stderr);
    #endif

    // format output to MSDev error message format, so we can dbl-click it to go to error
    corOutputDebugString("%s(%d) : [%s] %s: %s\n", 
        szFileName, iLineNumber, timeStamp, szSeverity, szDescription);
    
    if ( nSeverity >= m_dialogBoxMinSeverity )
    {
#if CORE_SYSTEM_XENON
        retval = EH_Break;
#elif CORE_SYSTEM_WINAPI
        const int result = MessageBoxA(NULL, message, "Program error", MB_OKCANCEL | MB_APPLMODAL);
        if (result == IDCANCEL)
            retval = EH_Break;
#endif // !Xenon
    }

    return retval;
}
