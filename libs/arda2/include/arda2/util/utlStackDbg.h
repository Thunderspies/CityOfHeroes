/*****************************************************************************
    created:    2002/04/03
    copyright:    2002, NCSoft. All Rights Reserved
    author(s):    Tom Gambill
    
    purpose:    Provides the ability to walk the stack with symbols during the
    execution of the application

    This code was borrowed from BugSlayer MSJ
*****************************************************************************/

#ifndef INCLUDED_utlStackDbg
#define INCLUDED_utlStackDbg

#if CORE_SYSTEM_WINAPI && !CORE_COMPILER_GNU

class utlStackDbg
{
public:
    utlStackDbg() : m_bInstalled(false), m_hProcess(0), m_hLibrary(0) {};
    ~utlStackDbg() { Release(); };

static utlStackDbg& GetSingleton();

    void    Release();
    bool    IsInstalled() const { return m_bInstalled; }

    DWORD    GetCallerReturnAddress(int iStackFramesBack=2);
    int        StackProbe( int iStartDepth, DWORD *pAddresses, int iProbeDepth );
    void    GetModuleNameFromAddress(DWORD dwAddress, char *szModuleName, int iMaxModuleName);
    bool    GetSymbolNameFromAddress(DWORD dwAddress, char *szSymbolName, int iMaxSymbolNameLength);
    bool    GetFileNameAndLineFromAddress(DWORD dwAddress, char *&szFileName, int &riLineNumber);
    bool    BuildStackTraceStringFromProbe(int iNumAddresses, DWORD *pAddresses, char *szStackTrace, int iMaxLen);
    bool    BuildStackTraceStringFromAddress(int iStartDepth, int iMaxDepth, char *szStackTrace, int iMaxLen);

private:
    bool  Install();

    bool                       m_bInstalled;
    HANDLE                     m_hProcess;
    HMODULE                    m_hLibrary;
};

#endif // CORE_SYSTEM_WINAPI && !CORE_COMPILER_GNU

#endif //INCLUDED_utlStackDbg
