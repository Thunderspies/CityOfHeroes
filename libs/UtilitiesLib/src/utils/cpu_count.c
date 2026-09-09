#ifdef _WIN32

#include "utilitieslib/utils/cpu_count.h"
#include "excpt.h"
#include "utilitieslib/utils/wininclude.h"
#include "utilitieslib/utils/SuperAssert.h"
#ifdef __GNUC__
#include <cpuid.h>
#undef __cpuid
#endif

#if defined(_MSC_VER) && _MSC_VER < 1400 // don't have intrinsic
void __cpuid(int CPUInfo[4], int param)
{
    int CPUInfo2[4];
    __asm {
    mov   eax, param      // call cpuid with eax = param
    cpuid
    mov   CPUInfo2, eax // eax contains cpu family type info
    mov   CPUInfo2+4, ebx  // edx has info whether Hyper-Threading
    mov   CPUInfo2+8, ecx  // edx has info whether Hyper-Threading
    mov   CPUInfo2+12, edx  // edx has info whether Hyper-Threading
    }
    memcpy(CPUInfo, CPUInfo2, sizeof(CPUInfo2));
}
#endif

#define HT_BIT      0x10000000  // EDX[28] - Bit 28 set indicates 
                        // Hyper-Threading Technology is supported 
                        // in hardware.
#define FAMILY_ID   0x0f00      // EAX[11:8] - Bit 11 thru 8 contains family 
                        // processor id
#define EXT_FAMILY_ID 0x0f00000 // EAX[23:20] - Bit 23 thru 20 contains 
                        // extended family  processor id
#define PENTIUM4_ID   0x0f00      // Pentium 4 family processor id
// Returns non-zero if Hyper-Threading Technology is supported on 
// the processors and zero if not.  This does not mean that 
// Hyper-Threading Technology is necessarily enabled.
unsigned int HTSupported(void)
{
    int CPUInfo[4];
    int vendor_id[4] = {0}; 
#ifdef __GNUC__
	if (__get_cpuid_max(0, NULL) < 1)
		return 0;
	__cpuid(vendor_id, 0);
	__cpuid(CPUInfo, 1);
#else
    __try {            // verify cpuid instruction is supported
        __cpuid(vendor_id,0); // vendor id string (only check HT on Intel)
        __cpuid(CPUInfo, 1); // capabilities        
    }
    __except (EXCEPTION_EXECUTE_HANDLER ) {
        return 0;   // CPUID is not supported and so Hyper-Threading Technology
        // is not supported
    }
    
#endif

    //  Check to see if this is a Pentium 4 or later processor
    if (((CPUInfo[0] & FAMILY_ID) ==  PENTIUM4_ID) || (CPUInfo[0] & EXT_FAMILY_ID))
        if (vendor_id[1] == 'uneG') 
            if (vendor_id[3] == 'Ieni')
                if (vendor_id[2] == 'letn')
                    return (CPUInfo[3] & HT_BIT);  // Hyper-Threading Technology
    return 0;  // This is not a genuine Intel processor.
}

#define NUM_LOGICAL_BITS    0x00FF0000 // EBX[23:16] indicate number of
                                        // logical processors per package
// Returns the number of logical processors per physical processors.
unsigned char LogicalProcessorsPerPackage(void)
{
    int CPUInfo[4];
    if (!HTSupported()) return (unsigned char) 1;
    __cpuid(CPUInfo, 1);
    return (unsigned char) ((CPUInfo[1] & NUM_LOGICAL_BITS) >> 16);
}

#define INITIAL_APIC_ID_BITS    0xFF000000 // EBX[31:24] unique APIC ID
// Returns the 8-bit unique Initial APIC ID for the processor this
// code is actually running on. The default value returned is 0xFF if
// Hyper-Threading Technology is not supported.
unsigned char GetAPIC_ID (void)
{
    int CPUInfo[4];
    if (!HTSupported()) return (unsigned char) -1;
    __cpuid(CPUInfo, 1);
    return (unsigned char) ((CPUInfo[1] & INITIAL_APIC_ID_BITS) >> 24);
}

int HyperThreadingEnabled(void)
{
// Check to see if Hyper-Threading Technology is available 
if (HTSupported()) {  // Bit 28 set indicated Hyper-Threading Technology
    unsigned char HT_Enabled = 0;
    unsigned char Logical_Per_Package; 
//   printf ("Hyper-Threading Technology is available.\n");
    Logical_Per_Package = LogicalProcessorsPerPackage();
//   printf ("Logical Processors Per Package: %d\n", Logical_Per_Package);

    if (Logical_Per_Package > getNumVirtualCpus()) {
        // Dual-core's with HT off were erroneously triggering this,
        //  this should catch it.
        return 0;
    }

    // Just because logical processors is > 1 
    // does not mean that Hyper-Threading Technology is enabled.
    if (Logical_Per_Package > 1) {
        HANDLE hCurrentProcessHandle;
        DWORD_PTR dwProcessAffinity;
        DWORD_PTR dwSystemAffinity;
        DWORD dwAffinityMask;
        
        // Physical processor ID and Logical processor IDs are derived
        // from the APIC ID.  We'll calculate the appropriate shift and
        // mask values knowing the number of logical processors per
        // physical processor package.
        unsigned char i = 1;
        unsigned char PHY_ID_MASK = 0xFF;
        unsigned char PHY_ID_SHIFT = 0;
        
        while (i < Logical_Per_Package){
            i *= 2;
            PHY_ID_MASK <<= 1;
            PHY_ID_SHIFT++;
        }
        
        // The OS may limit the processors that this process may run on.
        hCurrentProcessHandle = GetCurrentProcess();
        GetProcessAffinityMask(hCurrentProcessHandle, &dwProcessAffinity,
                               &dwSystemAffinity);
        
        // If our available process affinity mask does not equal the 
        // available system affinity mask, then we may not be able to 
        // determine if Hyper-Threading Technology is enabled.
        if (dwProcessAffinity != dwSystemAffinity)
            ;//printf ("This process can not utilize all processors.\n"); 
        
        dwAffinityMask = 1;
        while (dwAffinityMask != 0 && dwAffinityMask <= dwProcessAffinity) {
            // Check to make sure we can utilize this processor first.
            if (dwAffinityMask & dwProcessAffinity) {
                if (SetProcessAffinityMask(hCurrentProcessHandle, 
                                           dwAffinityMask)) {
                    unsigned char APIC_ID;
                    unsigned char LOG_ID;
                    unsigned char PHY_ID;
                    
                    Sleep(0);  // We may not be running on the cpu that we
                    // just set the affinity to.  Sleep gives the OS
                    // a chance to switch us to the desired cpu.
                    
                    APIC_ID = GetAPIC_ID();
                    LOG_ID = APIC_ID & ~PHY_ID_MASK;
                    PHY_ID = APIC_ID >> PHY_ID_SHIFT;
                    
                    // Print out table of processor IDs
//            printf ("OS Affinity ID: 0x%.8x, APIC ID: %d PHY ID: %d, LOG ID: %d\n",
//              dwAffinityMask, APIC_ID, PHY_ID, LOG_ID);
                    if (LOG_ID != 0) HT_Enabled = 1;
                }
                else {
                    // This shouldn't happen since we check to make sure we
                    // can utilize this processor before trying to set 
                    // affinity mask.
//            printf ("Set Affinity Mask Error Code: %d\n", 
//                                        GetLastError());
                }
            }
            dwAffinityMask = dwAffinityMask << 1;
        }
        // Don't forget to reset the processor affinity if you use this code
        // in an application.
        SetProcessAffinityMask(hCurrentProcessHandle, dwProcessAffinity);
        if (HT_Enabled) 
            return 1;
        else
            return 0;
    }
    else
    return 0;
}
else {
    return 0;
}
}

int getNumVirtualCpus(void)
{
    static int        cpu_count;
    DWORD_PTR        process = 0, system = 0;
    int                i;

    if (cpu_count)
        return cpu_count;
    GetProcessAffinityMask(GetCurrentProcess(), &process, &system);
    for(i=0;i<32;i++)
    {
        if (process & (DWORD_PTR)(1<<i))
            ++cpu_count;
    }
    return cpu_count;
}

#endif

#ifdef _XBOX

int HyperThreadingEnabled(void)
{
    return 1;
}

int getNumVirtualCpus(void)
{
    return 6;
}

#endif

int getNumRealCpus(void)
{
    static int cpu_count;

    if (cpu_count)
        return cpu_count;
    cpu_count = getNumVirtualCpus();
    if (HyperThreadingEnabled() && cpu_count > 1)
        cpu_count/=2;
    return cpu_count;
}

int getCacheLineSize(void) {
    static int cache_line_bytes = 0;
    DWORD size = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION *proc_info;
    HANDLE hModule;
    typedef BOOL (WINAPI *PGetLogicalProcessorInformation)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);
    PGetLogicalProcessorInformation GetLogicalProcessorInformation;

    if (cache_line_bytes)
        return cache_line_bytes;

    //GetLogicalProcessorInformation(NULL, &size);
    hModule = GetModuleHandleA("kernel32.dll");
    assert(hModule);
    GetLogicalProcessorInformation = (PGetLogicalProcessorInformation)GetProcAddress(hModule,"GetLogicalProcessorInformation");
    assert(GetLogicalProcessorInformation);
    GetLogicalProcessorInformation(NULL, &size);
    proc_info = _alloca(size);

    if (!GetLogicalProcessorInformation(proc_info, &size)) {
        cache_line_bytes = EXPECTED_WORST_CACHE_LINE_SIZE;
        return cache_line_bytes;
    }

    while(size >= sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION)) {
        if (proc_info->Relationship == RelationCache) {
            if (proc_info->Cache.Type == CacheData) {
                if (proc_info->Cache.LineSize > cache_line_bytes) {
                    cache_line_bytes = proc_info->Cache.LineSize;
                }
            }
        }

        proc_info++;
        size -= sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
    }

    return cache_line_bytes;
}
