#include <crashrpt/Stackwalk.h>
#include "utilitieslib/utils/Stackdump.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/SuperAssert.h"

struct 
{
    int initialized;
    HMODULE dll;
    pfnSWReady Ready;
    pfnSWStartup Startup;
    pfnSWShutdown Shutdown;
    pfnSWDumpStack DumpStack;
    pfnSWDumpStackToBuffer DumpStackToBuffer;

}sw;

int sdReady()
{
    if(sw.initialized)
        return sw.Ready();
    return 0;
}

int sdStartup()
{
    if(!sw.dll)
    {
        // Load the CrashRpt dll and bind all the stackwalk functions.
        sw.dll = loadCrashRptDll();
        if(!sw.dll)
            return 0;

        sw.Ready                = (pfnSWReady)GetProcAddress(sw.dll, "swReady");
        sw.Startup                = (pfnSWStartup)GetProcAddress(sw.dll, "swStartup");
        sw.Shutdown                = (pfnSWShutdown)GetProcAddress(sw.dll, "swShutdown");
        sw.DumpStack            = (pfnSWDumpStack)GetProcAddress(sw.dll, "swDumpStack");
        sw.DumpStackToBuffer    = (pfnSWDumpStackToBuffer)GetProcAddress(sw.dll, "swDumpStackToBuffer");

        if(sw.Ready && sw.Startup && sw.DumpStack && sw.DumpStackToBuffer)
            sw.initialized = 1;
    }

    if(sw.initialized) {
        int ret = sw.Startup();
        if (ret) {
            assertDoNotFreezeThisThread(NULL,ret);
        }
    }
    return 0;
}

void sdShutdown()
{
    if(sw.initialized)
        sw.Shutdown();

    if(sw.dll)
    {
        FreeLibrary(sw.dll);
        ZeroStruct(&sw);
    }
}

void sdDumpStack()
{
    if(!sw.initialized)
        sdStartup();
    if(sw.initialized)
        sw.DumpStack();
}

void sdDumpStackToBuffer(char* buffer, int bufferSize, PCONTEXT stack)
{
    if(!sw.initialized)
        sdStartup();
    if(sw.initialized)
        sw.DumpStackToBuffer(buffer, bufferSize, stack);
}
