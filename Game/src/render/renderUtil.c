/***************************************************************************
 *     Copyright (c) 2001-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 */
#include <utilitieslib/utils/mathutil.h>
#include "win/win_init.h"
#include <utilitieslib/utils/error.h>
#include <stdio.h>
#include "graphics/font.h"
#include "graphics/gfxwindow.h"
#include "cmdparse/cmdgame.h"
#include "cmdparse/cmdcommon.h"
#include "render/render.h"
#include "assert.h"
#include <utilitieslib/utils/sysutil.h> //For specs
#include <utilitieslib/utils/timing.h> //for specs
#include "graphics/font.h" //dumb to be here
#include "UI/uiWindows.h"
#include "gameComm/wdwbase.h"
#include "clientError.h"
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/utils/utils.h>
#include "UI/sprite/sprite_text.h"
#include "render/thread/rt_queue.h"
#include "render/renderprim.h"
#include <utilitieslib/utils/file.h>
#include <utilitieslib/utils/FolderCache.h>
#include "render/tex.h"
#include <utilitieslib/version/AppRegCache.h>
#include "group/groupscene.h"
#include "graphics/gfx.h"
#include "render/renderEffects.h"
#include "group/groupfileload.h"
#include "graphics/gfxSettings.h"
#include "seq/anim.h"
#include <utilitieslib/components/SharedMemory.h>
#include <utilitieslib/components/SharedHeap.h>
#include "seq/modelReload.h"
#include "group/grouptrack.h"
#include <utilitieslib/utils/cpu_count.h>
#include "NvPanelApi.h"
#include "graphics/groupdraw.h"
#include "entity/entclient.h"
#include "group/groupgrid.h"
#include "videoMemory.h"
#include <utilitieslib/components/StashTable.h>
#include "graphics/sun.h"
#include <utilitieslib/language/MessageStoreUtil.h>
#include <utilitieslib/utils/osdependent.h>
#include <utilitieslib/utils/RegistryReader.h>
#include "render/thread/rt_pbuffer.h"
#include "UI/uiConsole.h"
#include "graphics/gfxLoadScreens.h"
#include "graphics/gfxDebug.h"
#include <utilitieslib/language/AppLocale.h>
#include <utilitieslib/version/AppVersion.h>

#pragma warning(disable:4996)

extern int g_win_ignore_popups;
extern char g_GameArguments[2048];    // main.c


void rdrInit(void)
{
    rdrQueueCmd(DRAWCMD_INITDEFAULTS);
}

static int FindPrimaryDisplayDevice(DISPLAY_DEVICEA* dd)
{
    int i = 0;
    dd->DeviceID[0] = 0;
    dd->cb = sizeof(DISPLAY_DEVICEA);

    while (EnumDisplayDevicesA(NULL, i, dd, 0) && !(dd->StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE))
        i++;

    return (dd->StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0;
}

// N.B. This gets information on the primary adapter of the system.
// That may not be the adapter used by the gl context in the renderer
// so take this information with a grain of salt.
static int GetVideoCardIdentification( char * deviceString, int * vendorID, int * deviceID, DISPLAY_DEVICEA* dd)
{
    char * id = 0;

    id = dd->DeviceID;

    if (0 == id || '\0' == id[0]) 
    {
        *vendorID = 0;
        *deviceID = 0;
        deviceString[0] = 0;
        return 0;
    }

    // get vendor ID
    strcpy( deviceString, dd->DeviceString ); 
    {
        char vendorIDStr[5];
        char deviceIDStr[5];
        char end[1];
        char * endptr;
        
        end[0] = 0;
        endptr = end;

        strncpy( vendorIDStr, &id[8], 4 );
        strncpy( deviceIDStr, &id[17], 4 );
        vendorIDStr[4] = 0;
        deviceIDStr[4] = 0;

        *vendorID = (int)strtol( vendorIDStr, &endptr, 16 );
        *deviceID = (int)strtol( deviceIDStr, &endptr, 16 );
    }

    return 1;
}

SystemSpecs systemSpecs={0};

void getNVidiaSystemSpecs(SystemSpecs * systemSpecs)
{
    HINSTANCE hLib = LoadLibraryA("NVCPL.dll");
    fNvCplGetDataInt NvCplGetDataInt;
    fNvGetDisplayInfo NvGetDisplayInfo;
    if (hLib == 0) {
        return;
    }
    NvCplGetDataInt = (fNvCplGetDataInt)GetProcAddress(hLib, "NvCplGetDataInt");
    if (NvCplGetDataInt) {
        DWORD value;
        if (NvCplGetDataInt(NVCPL_API_VIDEO_RAM_SIZE, &value)) {
            systemSpecs->videoMemory = value;
        }
    }
    NvGetDisplayInfo = (fNvGetDisplayInfo)GetProcAddress(hLib, "NvGetDisplayInfo");
    if (NvGetDisplayInfo) {
        NVDISPLAYINFO display_info={0};
        display_info.cbSize = sizeof(display_info);
        display_info.dwInputFields1 = 0xffffffff;
        display_info.dwInputFields2 = 0xffffffff;
        if (NvGetDisplayInfo("0", &display_info)) {
            if (display_info.nDisplayMode == NVDISPLAYMODE_STANDARD ||
                display_info.nDisplayMode == NVDISPLAYMODE_NONE)
            {
                // Single monitor
            } else {
                // Assume problems on old drivers!
                systemSpecs->numMonitors = 2;
            }
        }
    }

    FreeLibrary(hLib);
}

OSVERSIONINFOEXA osinfo;

static time_t getVideoDriverDate(int vendorID) 
{
    DISPLAY_DEVICEA dd;
    char driverDateString[128];
    time_t result = 0;

    if (FindPrimaryDisplayDevice(&dd)) {
        static const char stripPrefix[] = "\\Registry\\Machine\\";
        
        if (strnicmp(dd.DeviceKey, stripPrefix, sizeof(stripPrefix) - 1) == 0) {
            RegReader deviceReader = createRegReader();
            char deviceKey[128];
            sprintf_s(deviceKey, sizeof(deviceKey), "HKEY_LOCAL_MACHINE\\%s",
                dd.DeviceKey + sizeof(stripPrefix) - 1);
            initRegReader(deviceReader, deviceKey);

            if (rrReadString(deviceReader, "DriverDate", driverDateString, sizeof(driverDateString))) {
                struct tm driverDate = {0};

                // Don't know if other locales use a different date format.
                if (3 == sscanf(driverDateString, "%d-%d-%d", &driverDate.tm_mon, 
                    &driverDate.tm_mday, &driverDate.tm_year))
                {
                    driverDate.tm_mon -= 1;
                    driverDate.tm_year -= 1900;
                    result = mktime(&driverDate);

                    if (result == -1)
                        result = 0;
                }
            }

            destroyRegReader(deviceReader);
        }
    }

    // If we could not get the driver date from the registry, use the old logic to look at the driver file
    if (result == 0)
    {
        static char *NV_DLLs[] = {
            "nvogl32.dll",
            "nvoglv32.dll",
            "nvoglnt.dll",
            "nvopengl.dll",
            "nv4_disp.dll",
            "nvapi.dll",
            "nvdisp.drv",
        };
        static char *AMD_DLLs[] = {
            "atioglxx.dll",
            "atioglx1.dll",
            "atioglx2.dll",
            "atioglgl.dll",
            "aticfx32.dll",
            "ati2drag.drv",
        };
        static char *INTEL_DLLs[] = {
            "igldev32.dll",
            "ig4icd32.dll",
            "ig4dev32.dll",
            "ig4icd64.dll",
            "ig4dev64.dll",
            "ialmgdev.dll",
            "iglicd32.dll",
            "igldev64.dll",
            "iglicd64.dll",
            "ialmgicd.dll",
        };
        static char *S3_VIA_DLLs[] = {
            "vticd.dll",
            "vtogl32.dll"
            "s3g700.dll",
            "s3g700.sys",
            "s3gIGPgl.dll",
            "s3gIGPglA.dll",
        };
        static char *SIS_DLLs[] = {
            "sisgl.dll",
            "SiSGlv.dll",
            "sisgl770.dll",
        };
        char winDirName[1024];
        char dllPathName[1024];

        char **names = NULL;
        int i, array_size = 0;

        if(vendorID == VENDOR_NV )
        {
            names = NV_DLLs;
            array_size = ARRAY_SIZE(NV_DLLs);
        }
        else if(vendorID == VENDOR_AMD)
        {
            names = AMD_DLLs;
            array_size = ARRAY_SIZE(AMD_DLLs);
        }
        else if(vendorID == VENDOR_INTEL)
        {
            names = INTEL_DLLs;
            array_size = ARRAY_SIZE(INTEL_DLLs);
        }
        else if(vendorID == VENDOR_S3G || vendorID == VENDOR_VIA)
        {
            names = S3_VIA_DLLs;
            array_size = ARRAY_SIZE(S3_VIA_DLLs);
        }
        else if(vendorID == VENDOR_SIS)
        {
            names = SIS_DLLs;
            array_size = ARRAY_SIZE(SIS_DLLs);
        }
        
        GetSystemDirectoryA(winDirName, 1024);

        for (i=0; i<array_size; i++)
        {
            sprintf( dllPathName, "%s\\%s", winDirName, names[i] );

            result = fileLastChanged(dllPathName);

            if (result)
                break;
        }
    }

    // Reject all dates before 1990 (does not account for leap years, but close enough)
    if (result < 20 * 365*24*60*60)
        result = 0;

    return result;
}

// Taken from getExecutableVersionEx() in sysutil.c
bool rdrGetFileVersion(char *fileName, int *v1, int *v2, int *v3, int *v4)
{
    int result;
    void* fileVersionInfo;
    char* moduleFilename = fileName;
    int fileVersionInfoSize;
    VS_FIXEDFILEINFO* fileInfo;
    int fileInfoSize;

    devassert(v1 && v2 && v3 && v4);

    fileVersionInfoSize = GetFileVersionInfoSizeA(moduleFilename, 0);

    // If the file doesn't have any version information...
    if(!fileVersionInfoSize)
        return false;

    // Allocate some buffer space and retrieve version information.
    fileVersionInfo = calloc(1, fileVersionInfoSize);
    result = GetFileVersionInfoA(moduleFilename, 0, fileVersionInfoSize, fileVersionInfo);
    if (!result)
        return false;

    result = VerQueryValueA(fileVersionInfo, "\\", &fileInfo, &fileInfoSize);
    if (!result)
        return false;

    #define HIBITS(x) x >> 16
    #define LOBITS(x) x & ((1 << 16) - 1)

    *v1 = HIBITS(fileInfo->dwFileVersionMS);
    *v2 = LOBITS(fileInfo->dwFileVersionMS);
    *v3 = HIBITS(fileInfo->dwFileVersionLS);
    *v4 = LOBITS(fileInfo->dwFileVersionLS);

    #undef HIBITS
    #undef LOBITS

    free(fileVersionInfo);

    return true;
}


static int isOldDriver();
static void setupOldDriverWarning();
static const char *getVideoCardManufacturerWebsite(SystemSpecs * systemSpecs)
{
    switch (systemSpecs->videoCardVendorID) {
        case VENDOR_AMD:
            return "www.amd.com";
        case VENDOR_NV:
            return "www.nvidia.com";
        case VENDOR_INTEL:
            return "www.intel.com";
        case VENDOR_S3G:
            return "www.s3graphics.com";
        case VENDOR_VIA:
            return "www.viaarena.com";
        case VENDOR_SIS:
            return "www.sis.com";
    }

    return "UNKNOWN";
}

static void showBadDriverWarning(SystemSpecs * systemSpecs)
{
    char dateStr[128];
    char text[4096];
    int locale = getCurrentLocale();

    // Format the driver date
    if (systemSpecs->videoDriverDate)
    {
        if (locIsEuropean(locale))
        {
            // Day-Month-Year
            strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", localtime(&systemSpecs->videoDriverDate));
        }
        else
        {
            // Month-Day-year
            strftime(dateStr, sizeof(dateStr), "%m-%d-%Y", localtime(&systemSpecs->videoDriverDate));
        }
    }
    else
    {
        sprintf_s(dateStr, sizeof(dateStr), "Unknown");
    }

    // Make sure this pop-up is not ignored
    if (!isGuiDisabled())
        g_win_ignore_popups = 0;

    strcpy( text, textStd("BadDriver", systemSpecs->videoCardName, dateStr, getVideoCardManufacturerWebsite(systemSpecs)));

    winMsgError(text);
}

// This function looks for known drivers which will crash before getting to the
// old driver version check in rdrSetChipOptions() and therefore would not show
// the old driver warning
void rdrExamineDriversEarly(SystemSpecs * systemSpecs)
{
    bool badDriver = false;

    // Disabled ATI Driver check.  The new game progress logic, which will put up a dialog if a crash
    // occurs on a previous attempt, should be sufficient.  Besides, it looks like we may still be
    // excluding some good drivers with the below check and some newer drivers are also crashing in setupPFD().
#ifdef ENABLE_BAD_ATI_DRIVER_CHECK
    // Look for bad ATI drivers
    if (systemSpecs->videoCardVendorID == VENDOR_AMD)
    {
        int v1, v2, v3, v4;

        // Get the atiogl.dll version 
        // NOTE: we could get the "DriverVersion" from the registry, using code similar
        // to the getVideoDriverDate() function.  But, that registry entry may only be
        // valid for AMD/ATI drivers.
        if (rdrGetFileVersion("atioglxx.dll", &v1, &v2, &v3, &v4))
        {
            // See http://developer.amd.com/download/ccc/pages/default.aspx for atioglxx.dll version numbers

            // ATI Catalyst 9.12 crashes in setupPFD()
            // see if the version is between 6.14.10.9232 (9.12 drivers) and 6.14.10.9252 (10.1 drivers)
            // to identify modified (but still problematic) 9.12 drivers
            // UPDATED: some 9.12 drivers are fine.  From the crash reports, these are the problem versions:
            //  6.14.10.9210
            //  6.14.10.9232
            //  6.14.10.9236
            if (v1 == 6 && v2 == 14 && v3 == 10 && (v4 >= 9210 && v4 <= 9236))
                badDriver = true;
        }
        else
        {
            // Could not determing the ATI OpenGL DLL version.
            // Do we care?  Should we set badDriver = true and bail?
        }
    }
#endif

    // Add more problem driver checks as we find problem drivers that won't initialize OpenGL

    // Not 100% sure the way the driver date is extracted from the registry works for non-ATI/AMD drivers
    // If we are seeing 0-0-0 dates, that may be the problem.
    if (badDriver)
    {
        showBadDriverWarning(systemSpecs);

        // It should be safe to just exit here since we have not connected to the auth server or db server yet.
        // We haven't initializes OpenGL.
        if (!game_state.ignoreBadDrivers)
            exit(-1);
    }

    if (isOldDriver())
        setupOldDriverWarning();
}

// NOTE: All these system specs are basic system
// specifications that can be queried without an
// OpenGL context. As such the video adapter information
// (which is obtained on the primary adapter in the system)
// may not represent the device ultimately used by the GL context
// (for example on an system with nVidia Optimus)
// Always prefer information logged from rdr_caps which comes from
// the GL context with regard to video device information and capabilities
void rdrGetSystemSpecs( SystemSpecs * systemSpecs )
{
    DISPLAY_DEVICEA dd;
    RegReader rr;
    char *s;

    if (systemSpecs->isFilledIn==1)
        return;

    rr = createRegReader();
    initRegReader(rr, regGetAppKey());

    // OS version:
    ZeroMemory(&osinfo, sizeof(osinfo));
    osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
    if (GetVersionExA((LPOSVERSIONINFOA) &osinfo))
    {
        if ((osinfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
            (osinfo.dwMajorVersion >= 5))
        {
            // get extended version info for 2000 and later
            ZeroMemory(&osinfo, sizeof(osinfo));
            osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
            GetVersionExA((LPOSVERSIONINFOA) &osinfo);
        }
    }
    systemSpecs->highVersion = osinfo.dwMajorVersion;
    systemSpecs->lowVersion = osinfo.dwMinorVersion;
    systemSpecs->build = osinfo.dwBuildNumber;
    systemSpecs->servicePackMajor = osinfo.wServicePackMajor;
    systemSpecs->servicePackMinor = osinfo.wServicePackMinor;

    // Bandwidth (from updater)
    Strncpyt(systemSpecs->bandwidth, regGetPatchBandwidth());

    systemSpecs->lastPiggCorruption[0] = 0;
    rrReadString(rr, "LastPiggCorruption", systemSpecs->lastPiggCorruption, sizeof(systemSpecs->lastPiggCorruption));

    // N.B. This gets information on the primary adapter of the system.
    // That may not be the adapter used by the gl context in the renderer
    // so take this information with a grain of salt.
    FindPrimaryDisplayDevice(&dd);
    GetVideoCardIdentification( systemSpecs->videoCardName, &systemSpecs->videoCardVendorID, &systemSpecs->videoCardDeviceID, &dd );
    
    getPhysicalMemory( &systemSpecs->maxPhysicalMemory , &systemSpecs->availablePhysicalMemory );
    
    systemSpecs->CPUSpeed = (F32)getRegistryMhz();
    systemSpecs->numVirtualCPUs = getNumVirtualCpus();
    systemSpecs->numRealCPUs = getNumRealCpus();

    s = getenv("PROCESSOR_IDENTIFIER");
    if (s)
        Strncpyt(systemSpecs->cpuIdentifier, s);
    else
        systemSpecs->cpuIdentifier[0] = 0;

    __cpuid(systemSpecs->cpuInfo, 1);

    systemSpecs->videoDriverDate = getVideoDriverDate(systemSpecs->videoCardVendorID);

    if (systemSpecs->videoCardVendorID == VENDOR_NV) {
        getNVidiaSystemSpecs(systemSpecs);
    } else if (systemSpecs->videoCardVendorID == VENDOR_AMD || systemSpecs->videoCardVendorID == VENDOR_INTEL) {
        systemSpecs->videoMemory = getVideoMemoryMBs();
    }

    destroyRegReader(rr);

    systemSpecs->isFilledIn = 1;
}

static const char *dateToString(time_t date) {
    static char result[16];

    if (date) {
        strftime(result, sizeof(result), "%m-%d-%Y", localtime(&date));
    } else {
        sprintf_s(result, sizeof(result), "Unknown");
    }

    return result;
}

void rdrPrintSystemSpecs(SystemSpecs *systemSpecs){
    rdrGetSystemSpecs(systemSpecs);

    if (systemSpecs->CPUSpeed) {
        writeConsole(OUTPUT_VERBOSE, "CPU Frequency: %0.0f MHz", systemSpecs->CPUSpeed / 1000000.0);
    } else {
        writeConsole(OUTPUT_VERBOSE, "CPU Frequency: Unknown");
    }

    if (systemSpecs->maxPhysicalMemory) {
        writeConsole(OUTPUT_VERBOSE, "RAM: %u MB", systemSpecs->maxPhysicalMemory / (1024 * 1024));
    } else {
        writeConsole(OUTPUT_VERBOSE, "RAM: Unknown");
    }

    if (systemSpecs->availablePhysicalMemory) {
        writeConsole(OUTPUT_VERBOSE, "Free RAM: %u MB", (systemSpecs->availablePhysicalMemory / (1024 * 1024)));
    } else {
        writeConsole(OUTPUT_VERBOSE, "Free RAM: Unknown");
    }

    writeConsole(OUTPUT_VERBOSE, "OS Version: Windows %d.%d.%d", systemSpecs->highVersion, systemSpecs->lowVersion, systemSpecs->build);
    writeConsole(OUTPUT_VERBOSE, "Command Line Arguments: %s", g_GameArguments);

    {
        int i = 0;
        DISPLAY_DEVICE dd;
        dd.cb = sizeof(DISPLAY_DEVICE);
        while (EnumDisplayDevices(NULL, i, &dd, 0) && dd.StateFlags & DISPLAY_DEVICE_ACTIVE) {
            char buf[128] = { 0 };
            sprintf(buf, "Video Device %d: %s, ID: %s, Flags: 0x%08x", i, dd.DeviceString, dd.DeviceID, dd.StateFlags);
            if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                strcat(buf, " (PRIMARY)");
            }
            writeConsole(OUTPUT_VERBOSE, "%s", buf);
            ++i;
        }
    }
    writeConsole(OUTPUT_VERBOSE, "Driver Date: %s", dateToString(systemSpecs->videoDriverDate));
    if (systemSpecs->videoMemory) {
        writeConsole(OUTPUT_VERBOSE, "VRAM: %u MB", systemSpecs->videoMemory);
    } else {
        writeConsole(OUTPUT_VERBOSE, "VRAM: Unknown");
    }
    writeConsole(OUTPUT_VERBOSE, "Video Device Vendor ID: 0x%X", systemSpecs->videoCardVendorID);
    writeConsole(OUTPUT_VERBOSE, "Video Device Device ID: 0x%X", systemSpecs->videoCardDeviceID);

    if (!rdr_caps.filled) {
        writeConsole(OUTPUT_VERBOSE, "Missing OpenGL Context -- OUTDATED VIDEO CARD DRIVER");
    } else {
        writeConsole(OUTPUT_VERBOSE, "Render settings: VBOs %d", rdr_caps.use_vbos);
        {
            char path[SPEC_BUF_SIZE] = { 0 };
            int i = 0;
            for (; i < 32; i++) {
                if ((1 << i) & rdr_caps.chip) {
                    char *chipname = rdrChipToString(1 << i);
                    strcatf(path, "%s ", chipname);
                }
            }
            writeConsole(OUTPUT_VERBOSE, "Render path: %s", path);
        }
        writeConsole(OUTPUT_VERBOSE, "Render features: %s", rdrFeaturesString(false));

        if (game_state.oldDriver) {
            writeConsole(OUTPUT_VERBOSE, "OUTDATED VIDEO CARD DRIVER");
        }
    }
}

char *rdrFeaturesString(bool onlyUsed)
{
    static char buf[1000];
    int i;
    buf[0]=0;
    for (i=0; i<GFXF_NUM_FEATURES; i++) {
        int bit = 1 << i;
        if (rdr_caps.allowed_features & bit) {
            if (!onlyUsed || (rdr_caps.features & bit)) {
                strcat(buf, rdrFeatureToString(bit));
                if (!onlyUsed && rdr_caps.features & bit)
                    strcat(buf, "*");
                strcat(buf, " ");
            }
        }
    }

    return buf;
}

void rdrGetSystemSpecString( SystemSpecs * systemSpecs, char * buf )
{
    //printf( "######################################################\n");
    buf[0] = 0;

    //**
    // system specifications
    // (obtained w/o the GL context and don't necessarily reflect it)
    if( systemSpecs->CPUSpeed )
        strcatf( buf, "CPU: %0.0f Mhz / ", (systemSpecs->CPUSpeed / 1000000.0) );
    else
        strcatf( buf, "CPU: Unknown / ");

    if( systemSpecs->maxPhysicalMemory )
        strcatf( buf, "Memory: %u MBs / ",  ( systemSpecs->maxPhysicalMemory / (1024*1024) ) );
    else
        strcatf( buf, "Memory: Unknown / " );

    if( systemSpecs->availablePhysicalMemory )
        strcatf( buf, "Available Memory: %u MBs / ",  ( systemSpecs->availablePhysicalMemory / (1024*1024) ) );
    else
        strcatf( buf, "Available Memory: Unknown / " );

    strcatf( buf, "OS Version: %d.%d.%d / ", systemSpecs->highVersion, systemSpecs->lowVersion, systemSpecs->build);
    strcatf( buf, "\n");

    strcatf( buf, "Command Line Arguments: %s\n", g_GameArguments);

    // primary video adapter information (not necessarily what gl context uses)
    // Enumerate all video apaters
    {
        int i = 0;
        DISPLAY_DEVICE dd;
        dd.cb = sizeof(DISPLAY_DEVICE);
        while (EnumDisplayDevices(NULL, i, &dd, 0))
        {
            strcatf( buf, " / Video Device %d: %s, ID: %s, Flags: 0x%08x", i, dd.DeviceString, dd.DeviceID, dd.StateFlags);
            if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
                strcatf( buf, " (PRIMARY)" );

            i++;
        }
        
    }
    strcatf(buf, "Driver Date: %s / ", dateToString(systemSpecs->videoDriverDate));
    if( systemSpecs->videoMemory)
        strcatf( buf, "Video Memory: %u MBs",  systemSpecs->videoMemory );
    else
        strcatf( buf, "Video Memory: Unknown" );
    strcatf( buf, " / VideoCardVendorID: 0x%X",  systemSpecs->videoCardVendorID );
    strcatf( buf, " / VideoCardDeviceID: 0x%X",  systemSpecs->videoCardDeviceID );
    strcatf( buf, "\n");

    //**
    // Information obtained from the GL context
    if (!rdr_caps.filled)
    {
        // Put the "OUTDATED VIDEO CARD DRIVER" text in the string so the crash report system will advice the 
        // player to update their video card driver if the game crashes during OpenGL initialization.
        strcatf( buf, "OpenGL has not been initialized yet  OUTDATED VIDEO CARD DRIVER");
    }
    else
    {
        strcatf( buf, "OpenGL vendor: %s\n",  rdr_caps.gl_vendor[0] ? rdr_caps.gl_vendor : "Unknown" );
        strcatf( buf, "OpenGL renderer: %s\n", rdr_caps.gl_renderer[0] ? rdr_caps.gl_renderer : "Unknown" );
        strcatf( buf, "OpenGL version: %s\n", rdr_caps.gl_version[0] ? rdr_caps.gl_version : "Unknown" );
        strcatf( buf, "Render settings: VBOS %d\n", rdr_caps.use_vbos );
        strcat( buf, "Render path: ");
        {
            int i=0;
            for (i=0;i<32; i++) {
                if ((1<<i) & rdr_caps.chip) {
                    char *chipname = rdrChipToString(1 << i);
                    strcatf(buf, "%s ", chipname);
                }
            }
        }
        strcat(buf, "\n");
        strcat(buf, "Render features: ");
        strcat(buf, rdrFeaturesString(false));
        strcat(buf, "\n");

        if (game_state.oldDriver)
            strcat(buf, "OUTDATED VIDEO CARD DRIVER\n"); // Do not change this string, it is hardcoded in CrashRpt.dll
        else
            strcat(buf, "Acceptable video card driver\n");
    }
}

char *rdrGetSystemSpecCSVString( void )
{
    static char buf[2048];    // @todo this should be allocated by caller since it is temporary
    buf[0] = 0;

    sprintf( buf+strlen(buf), "CPU,%0.0f, ", systemSpecs.CPUSpeed / 1000000.0 );
    sprintf( buf+strlen(buf), "NumCPUs,%d, ", systemSpecs.numVirtualCPUs );
    sprintf( buf+strlen(buf), "Memory,%u, ",  systemSpecs.maxPhysicalMemory / (1024*1024) );
    sprintf( buf+strlen(buf), "VideoCard,\"%s\", ", systemSpecs.videoCardName );
    sprintf( buf+strlen(buf), "DriverDate,\"%s\", ", dateToString(systemSpecs.videoDriverDate) );
    sprintf( buf+strlen(buf), "AvailableMemory,%u, ",  systemSpecs.availablePhysicalMemory / (1024*1024) );
    strcatf( buf, "RenderSettings,VBOS,%d,Bumpmaps,%d,VertShaders,%d, ", rdr_caps.use_vbos, (rdr_caps.features & GFXF_BUMPMAPS),rdr_caps.use_vertshaders );
    strcat( buf, "RenderPath,\"");
    {
        int i=0;
        for (i=0;i<32; i++) {
            if ((1<<i) & rdr_caps.chip) {
                char *chipname = rdrChipToString(1 << i);
                strcatf(buf, "%s ", chipname);
            }
        }
    }
    sprintf( buf+strlen(buf)-1, "\", ");

    if (game_state.oldDriver)
        strcat(buf, "OutdatedVideoCardDriver, ");
    else
        strcat(buf, "AcceptableVideoCardDriver, ");
    sprintf( buf+strlen(buf), "CPUIdentifier,\"%s\", ", systemSpecs.cpuIdentifier );
    sprintf( buf+strlen(buf), "CPUInfo,%u %u %u %u,",systemSpecs.cpuInfo[0],systemSpecs.cpuInfo[1],systemSpecs.cpuInfo[2],systemSpecs.cpuInfo[3]);
    sprintf( buf+strlen(buf), "OSVersion,%d,%d,%d, ", systemSpecs.highVersion, systemSpecs.lowVersion, systemSpecs.build );
    sprintf( buf+strlen(buf), "Bandwidth,\"%s\", ", systemSpecs.bandwidth );
    sprintf( buf+strlen(buf), "ScreenX,%d, ", game_state.screen_x );
    sprintf( buf+strlen(buf), "ScreenY,%d, ", game_state.screen_y );
    sprintf( buf+strlen(buf), "RefreshRate,%d, ", game_state.refresh_rate );
    sprintf( buf+strlen(buf), "Fullscreen,%d, ", game_state.fullscreen );
    sprintf( buf+strlen(buf), "ServicePack,%d,%d, ", systemSpecs.servicePackMajor, systemSpecs.servicePackMinor );
    sprintf( buf+strlen(buf), "Features,\"%s\", ", rdrFeaturesString(false));
    sprintf( buf+strlen(buf), "Skin,%d, ", game_state.skin);
    sprintf( buf+strlen(buf), "NumRealCPUs,%d, ", systemSpecs.numRealCPUs );
    sprintf( buf+strlen(buf), "GL_VENDOR,\"%s\", ", rdr_caps.gl_vendor );
    sprintf( buf+strlen(buf), "GL_RENDERER,\"%s\", ", rdr_caps.gl_renderer );
    sprintf( buf+strlen(buf), "GL_VERSION,\"%s\", ", rdr_caps.gl_version );
    sprintf( buf+strlen(buf), "VideoMemory,%u, ",  systemSpecs.videoMemory );
    sprintf( buf+strlen(buf), "VideoCardVendorID,0x%X, ",  systemSpecs.videoCardVendorID );
    sprintf( buf+strlen(buf), "VideoCardDeviceID,0x%X, ",  systemSpecs.videoCardDeviceID );
    sprintf( buf+strlen(buf), "LastPiggCorruption,\"%s\", ",  systemSpecs.lastPiggCorruption );

    // TEMPORARY NCsoft Launcher status bit
    sprintf( buf+strlen(buf), "Launcher,%d, ",  game_state.usedLauncher );
    return buf;
}

void rdrPrintScreenSystemSpecs( )
{
    //printf( "######################################################\n");

    if( systemSpecs.CPUSpeed )
    {
        //TO DO format this
        xyprintf( 10, 10, "CPU Speed  : %0.0f Mhz\n", (systemSpecs.CPUSpeed / 1000000.0) );
    }
    else
        xyprintf( 10, 10, "CPU Speed  : Unable to determine\n");

    if( systemSpecs.videoCardName[0] )
        xyprintf( 10, 11, "Video Card : %s \n", systemSpecs.videoCardName );
    else
        xyprintf( 10, 11, "Video Card : Unable to identify your video card.\n" );

    if( systemSpecs.maxPhysicalMemory )
        xyprintf( 10, 12, "Memory     : %u MBs\n",  ( systemSpecs.maxPhysicalMemory / (1024*1024) ) );
    else
        xyprintf( 10, 12, "Memory     : Unable to determine\n" );

    //printf( "######################################################\n");
}

// This getShaderDebugSpecialDefines functionality is used to pass additional #define's to shaders
//    prior to compiling the shader.  It would be better to pull this out and replace with an array of
//  strings that can be entered on command line and    used as is as defines, so no new code needs to be
//  written here to enable toggling new shader defines (prepend string with '-' to remove a define that
//  has previously been set here (HYPNOS_TBD).
int getShaderDebugSpecialDefines(const char** defineArray /*OUT*/, int defineArraySz /*IN*/)
{
    int nIndex = 0;
    #define AddToList(_Str_)    if ( nIndex < defineArraySz ) { defineArray[nIndex++] = (_Str_); }

    // set of mutually exclusive defines, only one can be used at a time
    {
        char *specials[] = {
            NULL, 
            "NO_SPECULAR",            // shadertest 1
            "SPECULAR_ONLY",        // shadertest 2
            "SPECULAR_GLOSS",
            "HIGH_QUALITY",
            "TEXCOORDS0",
            "TEXCOORDS1",
            "WHICH_SHADER",            // shadertest 7
            "SHOWLIGHTING",
            "SHOW_DOF",
            "TEXCOORD_TEST",
            "SHOW_LIGHTMAP",        
            "DIFFUSE_ONLY",            // shadertest 12
            "BASE_TEXTURE_ONLY",
            "BLEND_TEXTURE_ONLY",
        };
        char *shader_test_val=NULL;
        if (game_state.shaderTest > 0 && game_state.shaderTest < ARRAY_SIZE(specials))
        {
            AddToList( specials[game_state.shaderTest] );
        }
    }

    if ( game_state.useMRTs )            AddToList( "MRT_DEPTH" );
    if ( game_state.noBump )            AddToList( "NO_BUMP" );

    switch(game_state.shaderTestMode1)
    {
        case SHADERTEST_SPEC_ONLY:        AddToList( "SPECULAR_ONLY" );    break;
        case SHADERTEST_DIFFUSE_ONLY:    AddToList( "DIFFUSE_ONLY" );    break;
        case SHADERTEST_NORMALS_ONLY:    AddToList( "NORMALS_ONLY" );    break;
        case SHADERTEST_TANGENTS_ONLY:    AddToList( "TANGENTS_ONLY" );    break;
    }
    switch(game_state.shaderTestMode2)
    {
        case SHADERTEST_BASEMAT_ONLY:    AddToList( "BASE_TEXTURE_ONLY" );    break;
        case SHADERTEST_BLENDMAT_ONLY:    AddToList( "BLEND_TEXTURE_ONLY" );    break;
    }

    return nIndex; // i.e., the number of items added
}

// equivalent to glFrustum( l, r, b, t, znear, zfar ):
void rdrSetupFrustum(F32 l, F32 r, F32 b, F32 t, F32 znear, F32 zfar)
{
    memset(gfx_state.projection_matrix,0,sizeof(float)*16);
    gfx_state.projection_matrix[0][0] = 2*znear/(r-l);
    gfx_state.projection_matrix[1][1] = 2*znear/(t-b);
    gfx_state.projection_matrix[2][0] = (r+l)/(r-l);
    gfx_state.projection_matrix[2][1] = (t+b)/(t-b);
    gfx_state.projection_matrix[2][2] = -(zfar+znear)/(zfar-znear);
    gfx_state.projection_matrix[2][3] = -1;
    gfx_state.projection_matrix[3][2] = -2*zfar*znear/(zfar-znear);
}

// equivalent to glOrtho( l, r, b, t, znear, zfar ):
void rdrSetupOrtho(F32 l, F32 r, F32 b, F32 t, F32 znear, F32 zfar)
{
    memset(gfx_state.projection_matrix,0,sizeof(float)*16);
    gfx_state.projection_matrix[0][0] = 2/(r-l);
    gfx_state.projection_matrix[1][1] = 2/(t-b);
    gfx_state.projection_matrix[2][2] = -2/(zfar-znear);
    gfx_state.projection_matrix[3][0] = -(r+l)/(r-l);
    gfx_state.projection_matrix[3][1] = -(t+b)/(t-b);
    gfx_state.projection_matrix[3][2] = -(zfar+znear)/(zfar-znear);
    gfx_state.projection_matrix[3][3] = 1;
}

// equivalent to gluPerspective( fovy, aspect, znear, zfar ):
void rdrSetupPerspectiveProjection(F32 fovy, F32 aspect, F32 znear, F32 zfar, bool bFlipX, bool bFlipY)
{
    F32 l, r, b, t;

    t = znear * tan(fovy * PI / 360.0);
    b = -t;
    l = b * aspect;
    r = t * aspect;
    
    // caller can request that rendering is inverted along y axis (dynamic cubemaps need this)
    if(bFlipY) 
    {
        t = -t;
        b = -b;
    }

    if(bFlipX) 
    {
        l = -l;
        r = -r;
    }

    rdrSetupFrustum(l, r, b, t, znear, zfar);
}

// equivalent to glOrtho( ((float)width/height)*-ortho_zoom, ((float)width/height)*ortho_zoom, -ortho_zoom, ortho_zoom, 10, 50000):
void rdrSetupOrthographicProjection(int width, int height, int ortho_zoom)
{
    F32 l = ((float)width/height)*-ortho_zoom;
    F32 r = ((float)width/height)*ortho_zoom;
    F32 b = -ortho_zoom;
    F32 t = ortho_zoom;
    F32 n = 10;
    F32 f = 50000;

    rdrSetupOrtho(l, r, b, t, n, f);
}

void rdrSendProjectionMatrix(void)
{
    rdrQueue(DRAWCMD_PERSPECTIVE,gfx_state.projection_matrix,sizeof(float)*16);
}

void rdrViewport(int x, int y, int width,int height, bool bNeedScissoring)
{
    Viewport    v;

    v.x = x;
    v.y = y;
    v.width = width;
    v.height = height;
    v.bNeedScissoring = bNeedScissoring;
    rdrQueue(DRAWCMD_VIEWPORT,&v,sizeof(v));
}

void rdrPerspective(F32 fovy, F32 aspect, F32 znear, F32 zfar, bool bFlipX, bool bFlipY)
{
    if (game_state.ortho)
    {
        int width, height;
        windowClientSize( &width, &height );
        rdrSetupOrthographicProjection(width, height, game_state.ortho_zoom);
    }
    else
    {
        rdrSetupPerspectiveProjection(fovy, aspect, znear, zfar, bFlipX, bFlipY);
    }

    rdrSendProjectionMatrix();
}

void rdrFixMat(Mat4 mat)
{
Mat4    matx,unitfix = 
    {
        -1, +0, +0,
        +0, +1, +0,
        +0, +0, +1,
        +0, +0, +0,
    };
    mulMat3(unitfix,mat,matx);
    copyMat3(matx,mat);
}

static U8 *pixbuf;
static volatile int rd_idx,wr_idx;
static int pix_x,pix_y,numframes;

U8 *rdrPixBufGet(int *width,int *height)
{
    int        pix_x,pix_y;
    U8        *pix;

    CHECKNOTGLTHREAD;
    windowSize(&pix_x,&pix_y);
    pix = malloc(pix_x * pix_y * 4);
    rdrGetFrameBuffer(0,0,pix_x,pix_y,GETFRAMEBUF_RGBA,pix);
    *width = pix_x;
    *height = pix_y;
    return pix;
}

void rdrPixBufDumpBmp(const char *filename)
{
    int width, height;
    U8 *pixbuf;
    U8 *pixrow;
    FILE    *file;
    int int_4;
    short int_2;
    char int_1;

    //pixbuf = rdrPixBufGet(&width, &height);
    windowSize(&width,&height);
    pixbuf = malloc(width * height * 4);
    rdrGetFrameBuffer(0,0,width,height,GETFRAMEBUF_RGB,pixbuf);

    file = fopen(filename,"wb");
    if (file)
    {
        int width4 = ((width * 3) + 3) & ~3;
        int pad = width4 - (width * 3);
        int row, column;
        const int header2size = 40;

        // bmp header
        int_1 = 'B';
        fwrite(&int_1,1,1,file);
        int_1 = 'M';
        fwrite(&int_1,1,1,file);

        int_4 = width4 * height + 14 + header2size;    // filesize
        fwrite(&int_4,4,1,file);

        int_2 = 0;                    // reserved
        fwrite(&int_2,2,1,file);

        int_2 = 0;                    // reserved
        fwrite(&int_2,2,1,file);

        int_4 = 14 + header2size;            // offset to bitmap data
        fwrite(&int_4,4,1,file);

        // DIB header (Windows V3)
        int_4 = 40;                    // header size
        fwrite(&int_4,4,1,file);

        int_4 = width;                // width
        fwrite(&int_4,4,1,file);

        int_4 = height;                // height
        fwrite(&int_4,4,1,file);

        int_2 = 1;                    // num color planes
        fwrite(&int_2,2,1,file);

        int_2 = 24;                    // bpp
        fwrite(&int_2,2,1,file);

        int_4 = 0;                    // compression
        fwrite(&int_4,4,1,file);

        int_4 = width4 * height;    // raw image size
        fwrite(&int_4,4,1,file);

        int_4 = 2835;                // horizontal pixels per meter
        fwrite(&int_4,4,1,file);

        int_4 = 2835;                // vertical pixels per meter
        fwrite(&int_4,4,1,file);

        int_4 = 0;                    // num colors in palette
        fwrite(&int_4,4,1,file);

        int_4 = 0;                    // num important colors used
        fwrite(&int_4,4,1,file);

        // raw image data
        // each row needs to be padded out to 4 bytes
        int_4 = 0;                    // pad
        pixrow = malloc(width * 3);

        for (row = 0; row < height; row++)
        {
            U8 *pCurrentSrc = pixbuf + row * width * 3;
            U8 *pCurrentDest = pixrow;

            // Need to reverse from RGB to BGR
            for (column = 0; column < width; column++)
            {
                pCurrentDest[0] = pCurrentSrc[2];
                pCurrentDest[1] = pCurrentSrc[1];
                pCurrentDest[2] = pCurrentSrc[0];
                pCurrentSrc += 3;
                pCurrentDest += 3;
            }

            fwrite(pixrow, width * 3, 1, file);

            if (pad)
                fwrite(&int_4,pad,1,file);
        }

        free(pixrow);

        fclose(file);
    }
    free(pixbuf);
}

void rdrGetPixelColor(int x, int y, U8 * pixbuf)
{
    rdrGetFrameBuffer(x,y,1,1,GETFRAMEBUF_RGB,pixbuf);
}

void rdrGetPixelDepth(F32 x, F32 y, F32 * depthbuf)
{
    PERFINFO_AUTO_START("rdrGetPixelDepth",1)
    rdrGetFrameBufferAsync(x,y,1,1,GETFRAMEBUF_DEPTH_FLOAT,depthbuf);
    PERFINFO_AUTO_STOP();
}

void setCgShaderPathCallback( const char* shaderDirectory )
{
    rdrQueue(DRAWCMD_SETCGPATH,(char*)shaderDirectory,shaderDirectory?(strlen(shaderDirectory)+1):0);
}

void setCgShaderMode( unsigned int shaderMode )
{
    rdrQueue(DRAWCMD_SETCGSHADERMODE, &shaderMode, sizeof(shaderMode));
}

void rdrSetNumberOfTexUnits( int num_units )        // use this to tell the engine how many texture units are available
{
    rdr_caps.num_texunits        = MIN(num_units, TEXLAYER_ALL_LAYERS);
    rdr_caps.max_layer_texunits    = MIN(num_units, TEXLAYER_MAX_LAYERS);    // Max texture units that can be used by material specs on this HW platform
}

void setNormalMapTestMode( const char* testMode )
{
    struct {
        unsigned int testModeNumber;
        const char* szCmd;
    } optTbl[] = {
        { 0, "off" },
        { 0, "0" },
        { 1, "use bumpmap1" },
        { 1, "1" },
        { 2, "use dxt5nm-multiply1" },
        { 2, "2" },
        { 3, "use none" },
        { 3, "3" },
        { 4, "view bumpmap1" },
        { 4, "4" },
        { 5, "view dxt5nm-multiply1" },
        { 5, "5" },
        { 6, "show test_materials" },
        { 6, "6" }
    };
    int i;
    for ( i=0; i < ARRAY_SIZE(optTbl); i++ )
    {
        if ( stricmp(testMode, optTbl[i].szCmd ) == 0 )
        {
            rdrQueue(DRAWCMD_SETNORMALMAPTESTMODE, &optTbl[i].testModeNumber, sizeof(optTbl[i].testModeNumber));
            return;
        }
    }
    conPrintf( "Invalid normal map test mode: '%s'", testMode );
}

void setUseDXT5nmNormals( unsigned int useDXT5nmNormals )
{
    if ( useDXT5nmNormals != game_state.use_dxt5nm_normal_maps )
    {
        game_state.use_dxt5nm_normal_maps = useDXT5nmNormals;
        rdrQueue(DRAWCMD_SETDXT5NMNORMALS, &useDXT5nmNormals, sizeof(useDXT5nmNormals));
    }    
}

void reloadShaderCallback(const char *relpath, int when)
{
    // Show loading screen
    gfxShowLoadingScreen();

    // reload shaders
    rdrQueue(DRAWCMD_RELOADSHADER,(char*)relpath,relpath?(strlen(relpath)+1):0);

    // stop loading screen
    gfxFinishLoadingScreen();
}

void rebuildCachedShadersCallback(const char *relpath, int when)
{
    rdrQueue(DRAWCMD_REBUILDCACHEDSHADERS,(char*)relpath,relpath?(strlen(relpath)+1):0);
}

static int isOldDriver() {
    static const double OLD_DRIVER_TIME_SECS = 60.0 * 60 * 24 * 30.437 * 18; // 18 months
    time_t now = time(NULL);

    return systemSpecs.videoDriverDate != 0 && now != -1 && !IsUsingCider() &&
        difftime(now, systemSpecs.videoDriverDate) > OLD_DRIVER_TIME_SECS;
}

static void setupOldDriverWarning()
{
    bool showDialog = false;
    char dateStr[128];
    int locale = getCurrentLocale();

    game_state.oldDriver = 1;

    // Format the driver date
    if (systemSpecs.videoDriverDate)
    {
        if (locIsEuropean(locale))
        {
            // Day-Month-Year
            strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", localtime(&systemSpecs.videoDriverDate));
        }
        else
        {
            // Month-Day-year
            strftime(dateStr, sizeof(dateStr), "%m-%d-%Y", localtime(&systemSpecs.videoDriverDate));
        }
    }
    else
    {
        sprintf_s(dateStr, sizeof(dateStr), "Unknown");
    }

    strcpy( game_state.oldDriverMessage[0], textStd("OldDriver1", systemSpecs.videoCardName, dateStr));
    strcpy( game_state.oldDriverMessage[1], textStd("OldDriver2", getVideoCardManufacturerWebsite(&systemSpecs)));
    strcpy( game_state.oldDriverMessage[2], textStd("OldDriver3"));    

    // Always show the dialog in development mode
    if (isDevelopmentMode() && game_state.quick_login)
        showDialog = true;
    else
    {
        // Only show the dialog if the game version changes or the driver date changes
        const char *currentVersion = getExecutablePatchVersion(NULL);
        char registryVersion[100];
        int registryLastDate = 0;
        RegReader    rr;

        rr = createRegReader();
        initRegReader(rr, regGetAppKey());

        // If the last version does not exist or is different
        if (!rrReadString(rr, "DriverCheckLastVersion", registryVersion, 100))
        {
            showDialog = true;
        }
        else if (strcmp(currentVersion, registryVersion) != 0)
        {
            showDialog = true;
        }


        // If the last driver date version does not exist or is different
        if (!rrReadInt(rr, "DriverCheckLastDate", &registryLastDate))
        {
            showDialog = true;
        }
        else if (registryLastDate != systemSpecs.videoDriverDate)
        {
            showDialog = true;
        }

        destroyRegReader(rr);
    }

    if (showDialog)
    {
        char buf[2000];
        sprintf_s(buf, sizeof(buf), "%s\n\n%s\n\n%s", game_state.oldDriverMessage[0],
            game_state.oldDriverMessage[1], game_state.oldDriverMessage[2]);
        MessageBoxA(0, buf, "Warning - Old Video Card Driver", MB_ICONWARNING );
    }
}

void rdrClearOldDriverCheck()
{
    const char *currentVersion = getExecutablePatchVersion(NULL);
    RegReader    rr;

    rr = createRegReader();
    initRegReader(rr, regGetAppKey());

    // Update the registry so the old driver dialog will not be shown next time
    // if the game version is the same and the driver date is the same
    rrWriteString(rr, "DriverCheckLastVersion", currentVersion);
    rrWriteInt(rr, "DriverCheckLastDate", systemSpecs.videoDriverDate);

    destroyRegReader(rr);
}

void rdrSetChipOptions()
{
    int    failure = rdr_caps.bad_card_or_driver;
    int no_hdr_dof = 0;

    CHECKNOTGLTHREAD;

    rdr_caps.filled = 1;

    //TO DO : clean up this hack
    if (game_state.create_bins) {
        game_state.novertshaders = 1;
        game_state.noVBOs = 1;
        rdr_caps.use_vbos = 0;
        rdr_caps.use_vertshaders = 0;
        return;
    }
    //End hack
    // command line overrides

    rdrQueueFlush();

    if (game_state.forceNv1x) {
        rdr_caps.chip = NV1X|ARBVP;
        rdr_caps.supports_arb_np2 = false;
        rdr_caps.supports_pixel_format_float = false;
    }
    if (game_state.forceNv2x) {
        rdr_caps.chip = NV1X|NV2X|ARBVP;
        rdr_caps.supports_arb_np2 = false;
        rdr_caps.supports_pixel_format_float = false;
    }
    if (game_state.forceR200) {
        rdr_caps.chip = R200|ARBVP;
        rdr_caps.supports_arb_np2 = false;
        rdr_caps.supports_pixel_format_float = false;
    }
    if (game_state.forceNoChip) {
        rdr_caps.chip = 0;
        rdr_caps.supports_arb_np2 = false;
        rdr_caps.supports_pixel_format_float = false;
    }

    if (game_state.useARBassembly) {
        rdr_caps.chip |= ARBVP | ARBFP;
    }
    if (game_state.noNV) { // Override for disabling it
        rdr_caps.chip &= ~(NV1X | NV2X);
    }
    if (game_state.noATI) { // Override for disabling it
        rdr_caps.chip &= ~(R200);
    }
    if (game_state.noARBfp) { // Use vendor-specific fragment 'shaders' (i.e. texture env/combiner programs)
        rdr_caps.chip &= ~(ARBFP|GLSL);
    }
    if (game_state.useTexEnvCombine) {
        rdr_caps.chip = TEX_ENV_COMBINE;
    }

    // Check implementation limits for restricted ARB environment and disable accordingly.
    // e.g. Intel 915G family only has 16 temporaries and 24 bound params for shader programs
    // and limitations with Cg compiled instruction mix that we switch it over to TEX_ENV_COMBINE
    // until such time as they can be resolved (if ever).
    if (rdr_caps.chip&ARBFP && (rdr_caps.max_program_native_temporaries < 17 && rdr_caps.max_program_local_parameters < 32) )
    {
        printf( "ARB program capability is being disabled because of hardware/driver implementation limits: [%d,%d]\n", rdr_caps.max_program_native_temporaries, rdr_caps.max_program_local_parameters);
        rdr_caps.chip = TEX_ENV_COMBINE;
    }

    if (rdr_caps.chip == (ARBVP|TEX_ENV_COMBINE))
    {
        // ARBvp without some advanced fragment shader is useless, just use tex_env_combine
        rdr_caps.chip = TEX_ENV_COMBINE;
    }

    if (rdr_caps.chip == ARBVP) {
        // Supports ARBVP, but nothing else!  We need either at least NV1X, R200, or ARBFP support
        failure = 1;
    }

    // TODO: Is this necessary anymore?
    if (rdr_caps.chip == TEX_ENV_COMBINE && systemSpecs.videoCardVendorID == VENDOR_S3G)
        rdr_caps.chip = 0;

    //Warn if the chip is just bad  ////////////////////////////////////////////
    if ( 0 == rdr_caps.chip || failure )
    {
        writeConsole(OUTPUT_ERROR, "Your video driver is missing support for required extensions. Bailing");
        
        // Make sure this pop-up is not ignored
        if (!isGuiDisabled())
            g_win_ignore_popups = 0;

        if (strcmp(rdr_caps.gl_renderer, "GDI Generic") == 0)
            winMsgError( textStd("NoICD", systemSpecs.videoCardName) );
        else
            showBadDriverWarning(&systemSpecs);

        // Always quit
        rdr_caps.chip = 0;
        cmdParse( "quit" );
    }

    //////// Figure out which features not to use
    rdr_caps.allowed_features = GFXF_BUMPMAPS|GFXF_BUMPMAPS_WORLD;
    rdr_caps.use_vertshaders = 1;
    rdr_caps.use_vbos         = 1;

    /////////////////////////////////////////////////////////////////////////////////////
    //If you don't know, dont use anything (you probably 
    if( !rdr_caps.chip )
    {
        rdr_caps.use_vbos            = 0;
        rdr_caps.allowed_features    &=~GFXF_BUMPMAPS;
        rdr_caps.use_vertshaders    = 0;
        rdr_caps.use_pbuffers        = 0;
    }

    if (game_state.noRTT)
        rdr_caps.supports_render_texture = 0;

    if (game_state.noVBOs && systemSpecs.videoCardVendorID==VENDOR_AMD)
    {
        // VBOs not supported and ATI card, disable bumpmaps, skinning, etc
        rdr_caps.use_vbos         = 0;
        rdr_caps.allowed_features    &=~GFXF_BUMPMAPS;
        rdr_caps.use_vertshaders = 0; //We are choosing to do our own skinning on these cards
        if (rdr_caps.chip & R200) {
            // Xpress 200 cards support ARBfp, but we must use the R200 path to avoid vertex shaders
            rdr_caps.chip &= ~ARBFP;
        }
    }

    //if this is a gf2 or less, dont use VBOs or Bumpmaps or VertShaders
    if( !( rdr_caps.chip & ( R200 | NV2X | ARBFP ) ) )
    {
        rdr_caps.use_vbos         = 0;  //don't use VBOs since I'm software skinning
        rdr_caps.allowed_features    &=~GFXF_BUMPMAPS;
        rdr_caps.use_vertshaders = 0; //We are choosing to do our own skinning on these cards
    }

    // set rdr_caps.num_texunits here only if we didn't get a value when it was originally 
    //    read in rdrInitExtensionsDirect
    if( !rdr_caps.num_texunits )
    {
        if( !( rdr_caps.chip & ( R200 | NV2X | ARBFP ) ) )
        {
            if (rdr_caps.chip == TEX_ENV_COMBINE)
                rdrSetNumberOfTexUnits( 3 ); // We use all 3 units here
            else
                rdrSetNumberOfTexUnits( 2 );
        }
        else if (rdr_caps.chip & ARBFP)
            rdrSetNumberOfTexUnits( TEXLAYER_ALL_LAYERS );
        else
            rdrSetNumberOfTexUnits( 4 ); // NV2X/GF4/R200 path
    }

    if (rdr_caps.chip == TEX_ENV_COMBINE && systemSpecs.videoCardVendorID!=VENDOR_AMD)
    {
        // Intel Extreme, etc, don't even try!
        rdr_caps.supports_pbuffers = 0;
        rdr_caps.use_pbuffers = 0;
    }

    if (rdr_caps.chip == TEX_ENV_COMBINE) {
        // Turn these off, they're slow on old cards
        rdr_caps.disable_stencil_shadows = 1;
    }

    if ((rdr_caps.allowed_features & GFXF_BUMPMAPS) && systemSpecs.videoCardVendorID!=VENDOR_INTEL) {
        // Newer (ARBfp) Intel cards
        rdr_caps.use_pbuffers = 1;
        game_state.cursor_cache = 1; // They have problems with the pbuffer and the cursor, use the cache to hide it
    }
    if (!IsUsingCider() && (rdr_caps.chip & NV1X))
    {
        rdr_caps.use_pbuffers = 1; // GF2s are now fast with PBuffers, don't know what driver version fixed this
    }

    if (game_state.usePBuffers) {
        // User-level override
        rdr_caps.supports_pbuffers = 1;
        rdr_caps.use_pbuffers = 1;
    }

    if (!game_state.useFBOs) {
        rdr_caps.supports_fbos = 0;
        rdr_caps.supports_ext_framebuffer_blit = 0;
        rdr_caps.supports_ext_framebuffer_multisample = 0;
    }

    if (rdr_caps.chip & ARBFP)
    {
        bool bMinUltraRequirements;

        rdr_caps.allowed_features |= GFXF_WATER|GFXF_WATER_DEPTH;

        if (rdr_caps.chip & NV2X ||        // NVidia
            rdr_caps.chip & R200 ||        // ATI
            game_state.useARBassembly)    // other
        {
            if ((rdr_caps.supports_render_texture || rdr_caps.supports_fbos) && !no_hdr_dof)
            {
                // All features available
                rdr_caps.allowed_features |= GFXF_BLOOM|GFXF_TONEMAP|GFXF_DOF|GFXF_DESATURATE;
                if (rdr_caps.supports_pixel_format_float)
                    rdr_caps.allowed_features |= GFXF_FPRENDER;
            }
        }

        // All of these marked as supported until shaders are loaded
        rdr_caps.allowed_features |= GFXF_MULTITEX|GFXF_MULTITEX_DUAL;

        // Some special graphics features all ARBfp cards get
        if (game_state.useHQ)
        {
            rdr_caps.allowed_features |= GFXF_HQBUMP|GFXF_MULTITEX_HQBUMP;
        }

        // Check implementation limits for restricted ARB environment and disable
        // features accordingly.
        // e.g. Intel 915G family only has 16 temporaries and 24 bound params for shader programs
        // and the MULTI and WATER shaders will fail to load on this chipset.
        // Note that the Intel 915G has other limitations with Cg compiled instruction mix that
        // we switch it over to TEX_ENV_COMBINE earlier.
        if (rdr_caps.max_program_native_temporaries < 17 || rdr_caps.max_program_local_parameters < 32 )
        {
            printf( "Some standard features are being disabled because your video card is not capable: [%d,%d]\n", rdr_caps.max_program_native_temporaries, rdr_caps.max_program_local_parameters);
            rdr_caps.allowed_features &= ~(GFXF_MULTITEX|GFXF_MULTITEX_DUAL|GFXF_MULTITEX_HQBUMP);
            rdr_caps.allowed_features &= ~(GFXF_BLOOM|GFXF_TONEMAP|GFXF_FPRENDER|GFXF_DOF|GFXF_DESATURATE);
            rdr_caps.allowed_features &= ~(GFXF_WATER|GFXF_WATER_DEPTH);
        }

        // Enable SSAO if the extension requirements are met, it will disable itself later if the shaders cannot be loaded
        if(rdr_caps.supports_fbos && (rdr_caps.partially_supports_arb_np2 || rdr_caps.supports_arb_rect))
            rdr_caps.allowed_features |= GFXF_AMBIENT;

        // Ultra mode feature requirements.  Require a reasonable number of shader implementation specific limits
        #define NUM_REQUIRED_INSTRUCTIONS        128
        #define NUM_ENV_PARAMS_FOR_ULTRA        64    // ULTRA features need more than the 32 minimum fragment program ENV constants specified by ARBFP1
        bMinUltraRequirements = (rdr_caps.supports_fbos &&
                                 rdr_caps.max_program_native_instructions >= NUM_REQUIRED_INSTRUCTIONS &&
                                 rdr_caps.max_program_env_parameters >= NUM_ENV_PARAMS_FOR_ULTRA 
                                 );
        if(bMinUltraRequirements)
        {
            rdr_caps.allowed_features |= GFXF_CUBEMAP;
            
            if ( rdr_caps.supports_arb_shadow )
                rdr_caps.allowed_features |= GFXF_SHADOWMAP;    

            rdr_caps.allowed_features |= GFXF_WATER_REFLECTIONS;
        }
        else
        {
            printf( "Ultra mode features disabled because your video card is not capable:\n" );
            if(!rdr_caps.supports_fbos )
                printf( "  supports fbos = %d\n", rdr_caps.supports_fbos );
            if(rdr_caps.max_program_native_instructions < NUM_REQUIRED_INSTRUCTIONS)
                printf( "  max_program_native_instructions = %d (%d required)\n", rdr_caps.max_program_native_instructions, NUM_REQUIRED_INSTRUCTIONS );
            if(rdr_caps.max_program_env_parameters < NUM_ENV_PARAMS_FOR_ULTRA)
                printf( "  max_program_env_parameters = %d (%d required)\n", rdr_caps.max_program_env_parameters, NUM_ENV_PARAMS_FOR_ULTRA );
            if ( !rdr_caps.supports_arb_shadow )
                printf( "  does not support shadow extensions\n" );
        }

        // if this hardware fails the instruction limit on fragment programs then
        // it is also going to fail certain other features like full MULTI and SSAO
        // e.g., ATI R300 (9600,9800), Intel 915
        if (rdr_caps.max_program_native_instructions < NUM_REQUIRED_INSTRUCTIONS)
        {
            printf( "Some graphics features are disabled because your video card is not capable:\n" );
            printf( " max_program_native_instructions = %d (%d required)\n", rdr_caps.max_program_native_instructions, NUM_REQUIRED_INSTRUCTIONS );

            // @todo we must not be correctly detecting failed loading of SSAO shaders
            // as SSAO can be enabled on Radeon 9800 XT causing black materials.
            printf( " Ultra mode Ambient Occlusion is disabled.\n" );
            rdr_caps.allowed_features &= ~(GFXF_AMBIENT);

            
            printf( " MULTI material degraded.\n" );
            rdr_caps.limited_fragment_instructions = 1; // flag used to get multi9 shader to compile with reduced capabilities.
            rdr_caps.allowed_features &= ~GFXF_MULTITEX_HQBUMP;
        }
    } // if( rdr_caps.chip & ARBFP)

    ///////////// Set any limits based on the video card chipset.

    // Note that we are not checking driver versions here, just chip type.
    // Ideally features should already be disabled based on rdr_caps settings, but we catch
    // any additional blacklist cards here to be safe.

    // NONE AT THIS TIME (strive to keep it that way)

    ///////////// Set any limits specified in the command line

    if( game_state.enableVBOs == 0 ) //from gfx settings now, actually
        rdr_caps.use_vbos = 0;

    if( game_state.disableVBOs )
        rdr_caps.use_vbos = 0;

    if( game_state.noVBOs || game_state.gfxdebug ) //legacy
        rdr_caps.use_vbos = 0;

    if( game_state.nopixshaders ) {
        rdr_caps.allowed_features    &=~GFXF_BUMPMAPS;
    }

    if (game_state.nofancywater)
        rdr_caps.allowed_features &= ~(GFXF_WATER|GFXF_WATER_DEPTH);

    if (game_state.noNP2)
        rdr_caps.supports_arb_np2 = 0;

    if (game_state.noPBuffers) {
        rdr_caps.use_pbuffers = 0;
        rdr_caps.supports_pbuffers = 0;

        if (!rdr_caps.supports_fbos)
        {
            rdr_caps.allowed_features &= ~(GFXF_BLOOM|GFXF_TONEMAP|GFXF_FPRENDER|GFXF_DOF|GFXF_DESATURATE);
        }
    }

    if (game_state.maxTexUnits)
        rdrSetNumberOfTexUnits( game_state.maxTexUnits );

    if (game_state.noStencilShadows)
        rdr_caps.disable_stencil_shadows = 1;

    // Test if there is hardware profile support for user clip planes when not using position invariance
    // (e.g. for skinned objects) and that we don't have a command line option to disable it ("no_nv_clip")
    rdr_caps.supports_vp_clip_planes_nv = rdr_caps.chip & NV4X && !game_state.no_nv_clip && !game_state.safemode && !IsUsingCider();

    rdr_caps.features = rdr_caps.allowed_features; // Default everything on until we get a UI option

    // Off by default, because it's slower
    rdr_caps.features &= ~(GFXF_FPRENDER);

    // Check for features specified by settings in registry/generic command lines
    rdr_caps.features |= game_state.gfx_features_enable & rdr_caps.allowed_features;
    rdr_caps.features &= ~game_state.gfx_features_disable;

    // TODO: Does this get set auto-magically now?
    if (game_state.nohdr)
        rdr_caps.features &= ~(GFXF_BLOOM|GFXF_TONEMAP|GFXF_FPRENDER|GFXF_DESATURATE);

    //Final check since no vert shaders implies these things
    if( game_state.novertshaders )
    {
        rdr_caps.use_vertshaders = 0;
        rdr_caps.use_vbos         = 0;
        rdr_caps.features         &=~GFXF_BUMPMAPS;
        rdr_caps.allowed_features&=~GFXF_BUMPMAPS;
    }
    // Disable features that need bumpmaps
    if (!(rdr_caps.features & GFXF_BUMPMAPS))
    {
        rdr_caps.features &= ~(GFXF_WATER|GFXF_WATER_DEPTH|GFXF_MULTITEX|GFXF_BUMPMAPS_WORLD|GFXF_CUBEMAP|GFXF_SHADOWMAP);
    }
    if (!(rdr_caps.allowed_features & GFXF_BUMPMAPS))
    {
        rdr_caps.allowed_features &= ~(GFXF_WATER|GFXF_WATER_DEPTH|GFXF_MULTITEX|GFXF_BUMPMAPS_WORLD|GFXF_CUBEMAP|GFXF_SHADOWMAP);
    }

    rdr_caps.use_fixedfunctionfp = 0;

    if (!rdr_caps.use_vertshaders )
        rdr_caps.use_fixedfunctionvp = 0;
    else
        rdr_caps.use_fixedfunctionvp = 1;


    // Special hardware emulation and debugging
    if (game_state.useChipDebug)
    {
        // Intel 915G
        rdr_caps.features = GFXF_BUMPMAPS | GFXF_BUMPMAPS_WORLD | GFXF_HQBUMP;
        rdr_caps.allowed_features = GFXF_BUMPMAPS | GFXF_BUMPMAPS_WORLD | GFXF_HQBUMP;
        rdr_caps.chip = ARBFP | ARBVP;
        rdr_caps.supports_fbos = 0;
        rdr_caps.use_vbos         = 0;
    }

    {
        char specBuf[SPEC_BUF_SIZE];
        rdrGetSystemSpecString( &systemSpecs, specBuf );
        setAssertExtraInfo2(specBuf);
    }
    // Add callback for re-loading shaders
    FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE_AND_DELETE, "shaders/*", rebuildCachedShadersCallback);
}

U32 gFrameNum = 0;

void rdrInitTopOfFrame(void)
{
    RdrFrameState rfs={0};

    rfs.curFrame = game_state.client_frame;
    rfs.sliClear = game_state.sliClear;
    rfs.numFBOs = game_state.sliFBOs = CLAMP(game_state.sliFBOs, 1, MAX_SLI);
    rfs.sliLimit = game_state.sliLimit = CLAMP(game_state.sliLimit, 0, MAX_FRAMES);

    rdrQueue(DRAWCMD_INITTOPOFFRAME,&rfs,sizeof(rfs));
}

void rdrInitTopOfView(const Mat4 viewmat, const Mat4 inv_viewmat)
{
    extern int override_window_size;
    RdrViewState rvs={0};

    copyVec3(g_sun.shadow_direction,rvs.shadow_direction);
    copyVec3(g_sun.position,rvs.sun_position);
    copyVec4(g_sun.ambient,rvs.sun_ambient);
    copyVec4(g_sun.diffuse,rvs.sun_diffuse);
    copyVec4(g_sun.ambient_for_players,rvs.sun_ambient_for_players);
    copyVec4(g_sun.diffuse_for_players,rvs.sun_diffuse_for_players);
    copyVec4(g_sun.direction,rvs.sun_direction);
    copyVec4(g_sun.direction_in_viewspace,rvs.sun_direction_in_viewspace);
    copyVec3(gfx_state.shadowmap_light_direction_ws,rvs.shadowmap_light_direction);
    copyVec3(gfx_state.shadowmap_light_direction_vs,rvs.shadowmap_light_direction_in_viewspace);
    copyVec4(g_sun.no_angle_light,rvs.sun_no_angle_light);
    copyVec4(g_sun.shadowcolor,rvs.shadowcolor);
    copyMat4(inv_viewmat,rvs.inv_viewmat);
    copyMat4(viewmat,rvs.viewmat);
    windowSize(&rvs.width,&rvs.height);
    windowPhysicalSize(&rvs.origWidth, &rvs.origHeight);
    rvs.override_window_size = override_window_size;
    rvs.lamp_alpha            = g_sun.lamp_alpha;
    rvs.lamp_alpha_byte        = g_sun.lamp_alpha;
    rvs.fixed_sun_pos        = g_sun.fixed_position;
    rvs.nearz                = game_state.nearz;
    rvs.farz                = game_state.farz;
    rvs.gfxdebug            = gfx_state.mainViewport ? game_state.gfxdebug : 0;
    rvs.wireframe_seams        = (gfx_state.mainViewport && (game_state.gfxdebug & GFXDEBUG_SEAMS)) ? 1 : 0;
    rvs.wireframe            = (gfx_state.mainViewport && game_state.wireframe) ? 1:0;
    rvs.gloss_scale            = g_sun.gloss_scale;
    rvs.client_loop_timer    = game_state.client_loop_timer;
    rvs.time                = server_visible_state.time;
    rvs.shadowvol            = game_state.shadowvol;
    rvs.white_tex_id        = texDemandLoad(white_tex);
    rvs.black_tex_id        = texDemandLoad(black_tex);
    rvs.grey_tex_id            = texDemandLoad(grey_tex);
    rvs.dummy_bump_tex_id    = texDemandLoad(dummy_bump_tex);
    rvs.building_lights_tex_id    = texDemandLoad(building_lights_tex);
    rvs.texAnisotropic        = CLAMP(game_state.texAnisotropic, 1, rdr_caps.maxTexAnisotropic);
    rvs.antialiasing        = game_state.antialiasing;
    rvs.renderScaleFilter    = game_state.renderScaleFilter;
    rvs.toneMapWeight        = (rdr_caps.features&GFXF_TONEMAP)?g_sun.toneMapWeight:0;
    rvs.toneMapWeight        *= game_state.bloomWeight;
    rvs.bloomWeight            = (rdr_caps.features&GFXF_BLOOM)?g_sun.bloomWeight:0;
    rvs.bloomWeight            *= game_state.bloomWeight;
    rvs.bloomScale            = game_state.bloomScale;
    rvs.desaturateWeight    = 1.0f - (1.0f - game_state.desaturateWeight) * (1.0f - g_sun.desaturateWeight);
    rvs.luminance            = g_sun.luminance;
#define GSOVERRIDE(v) (game_state.dof_values.v?game_state.dof_values.v:g_sun.dof_values.v)
    rvs.dofFocusDistance    = GSOVERRIDE(focusDistance);
    rvs.dofFocusValue        = MAX(GSOVERRIDE(focusValue), game_state.camera_blur);
    rvs.dofNearDist            = GSOVERRIDE(nearDist);
    rvs.dofNearValue        = MAX(GSOVERRIDE(nearValue)*game_state.dofWeight, game_state.camera_blur);
    rvs.dofFarDist            = GSOVERRIDE(farDist);
    rvs.dofFarValue            = MAX(GSOVERRIDE(farValue)*game_state.dofWeight, game_state.camera_blur);
#undef GSOVERRIDE
    copyVec3(g_sun.fogcolor, rvs.fogcolor);
    copyVec2(g_sun.fogdist, rvs.fogdist);

    rvs.renderPass = gfx_state.renderPass;
    rvs.write_depth_only = gfx_state.writeDepthOnly;
    rvs.cubemapMode = gfx_state.doCubemaps ? game_state.cubemapMode : CUBEMAP_OFF;
    rvs.shadowMode = gfx_state.mainViewport ? game_state.shadowMode : SHADOW_OFF;

    if (rvs.shadowMode >= SHADOW_SHADOWMAP_LOW && ( !game_state.shadowMapsAllowed || scene_info.shadowMaps_Off ))
        rvs.shadowMode = SHADOW_OFF;

    rvs.waterRefractionSkewNormalScale = game_state.waterRefractionSkewNormalScale;
    rvs.waterRefractionSkewDepthScale = game_state.waterRefractionSkewDepthScale;
    rvs.waterRefractionSkewMinimum = game_state.waterRefractionSkewMinimum;
    rvs.waterReflectionOverride = game_state.waterReflectionOverride;
    rvs.waterReflectionSkewScale = game_state.waterReflectionSkewScale;
    rvs.waterReflectionStrength = game_state.waterReflectionStrength;
    rvs.waterFresnelBias = game_state.waterFresnelBias;
    rvs.waterFresnelScale = game_state.waterFresnelScale;
    rvs.waterFresnelPower = game_state.waterFresnelPower;

    rvs.shadowmap_extrusion_scale        = gfx_state.shadowmap_extrusion_scale;
    rvs.lowest_visible_point_last_frame    = gfx_state.lowest_visible_point_last_frame;

    // Water Time Of Day scale (1.0 = normal, 0.0 = no reflection)
    if (isIndoors())
        rvs.waterTODscale = 1.0f;
    else
        rvs.waterTODscale = 1.0f - (rdr_view_state.lamp_alpha * game_state.waterReflectionNightReduction / 255.0f);
    
    rdrQueue(DRAWCMD_INITTOPOFVIEW,&rvs,sizeof(rvs));
    //rdrSetFog(g_sun.fogdist[0], g_sun.fogdist[1], g_sun.fogcolor1); // TODO: moves these into inittopofframe and fix pbuffer?
    rdrSetBgColor(g_sun.bg_color);
}

void reloadAllAnims()
{
    StashElement element;
    StashTableIterator it;

    forceGeoLoaderToComplete();

    // Turn off shared memory, reloading breaks it
    sharedMemorySetMode(SMM_DISABLED);
    sharedHeapTurnOff("Anims reloading, thus turning off shared heap\n");

    geoLoadResetNonExistent();

    loadstart_printf("Reloading all geometry...");
#if CLIENT
    modelFreeAllLODs(0);
#endif
    stashGetIterator(glds_ht, &it);
    while(stashGetNextElement(&it, &element))
    {
        GeoLoadData*    gld;
        gld = stashElementGetPointer(element);
        reloadGeoLoadData(gld);
    }
    loadend_printf("done.");
#if CLIENT
    loadstart_printf("Resetting geometry on entities...");
    entResetModels();
    loadend_printf("done.");
#endif
    loadstart_printf("Resetting group bounds...");
    groupResetAll(RESET_BOUNDS);
    loadend_printf("done.");
}

int rdrToggleFeatures(int gfx_features_enable, int gfx_features_disable, bool onlyDynamic)
{
    int old_features;
    int diff;
    bool reloadTricks=false;
    int requires_reload_flags = (GFXF_WATER|GFXF_MULTITEX|GFXF_MULTITEX_DUAL|GFXF_WATER_DEPTH|GFXF_BUMPMAPS|GFXF_BUMPMAPS_WORLD); // WATER_DEPTH not technically needed, but it makes the options screen confusing

    if (!rdr_caps.filled_in) {
        // Save these for the call to rdrSetChipOptions
        game_state.gfx_features_enable |= gfx_features_enable;
        game_state.gfx_features_disable &= ~gfx_features_enable;
        game_state.gfx_features_enable &= ~gfx_features_disable;
        game_state.gfx_features_disable |= gfx_features_disable;
        return 0;
    }

    // Enable/disable features based on what is selected + what is supported
    rdrQueueFlush();
    old_features = rdr_caps.features;
    if (onlyDynamic) {
        gfx_features_enable &= ~requires_reload_flags;
        gfx_features_disable &= ~requires_reload_flags;
    }
    rdr_caps.features |= gfx_features_enable&rdr_caps.allowed_features;
    rdr_caps.features &= ~gfx_features_disable;
    if (!(rdr_caps.features & GFXF_BUMPMAPS))
        rdr_caps.features &= ~(GFXF_WATER|GFXF_WATER_DEPTH|GFXF_MULTITEX|GFXF_CUBEMAP|GFXF_SHADOWMAP);

    if (!(rdr_caps.features & GFXF_WATER))
        game_state.waterMode = WATER_OFF;

    if (!(rdr_caps.features & GFXF_CUBEMAP))
        game_state.cubemapMode = CUBEMAP_OFF;

    if (!(rdr_caps.features & GFXF_SHADOWMAP) && game_state.shadowMode >= SHADOW_SHADOWMAP_LOW)
        game_state.shadowMode = SHADOW_STENCIL;

    diff = old_features^rdr_caps.features;

    if (diff & requires_reload_flags) {
        reloadTricks=true;
    }

    if (reloadTricks) {
        if (!onlyDynamic)
            gfxShowLoadingScreen();

        trickReloadPostProcess();
    }

    if (game_state.novertshaders || !rdr_caps.use_vertshaders) {
        game_state.enableVBOs = false;
    }
    if( rdr_caps.use_vbos != game_state.enableVBOs && !game_state.noVBOs ) {
        // We can enable/disable VBOs on the fly now.
        if (reloadTricks) {
            // Some things already freed above
            modelResetVBOs(false);
        } else {
            // Free stuff
            modelResetVBOs(true);
            entResetTrickFlags(); // Free seq->rgbs_array
        }
        rdrQueueFlush();
        rdr_caps.use_vbos = game_state.enableVBOs && !game_state.noVBOs;
    }

    gfxFinishLoadingScreen();

    return diff;
}
