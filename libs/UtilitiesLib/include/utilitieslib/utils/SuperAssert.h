#ifndef    SUPERASSERT_H
#define SUPERASSERT_H

#include <stdlib.h>
#include <excpt.h>
#include <process.h>
#include "../stdtypes.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifdef FINAL
#define DISABLE_ASSERTIONS
#endif

#undef IDC_STOP
#undef IDC_DEBUG
#undef IDC_IGNORE
#define IDC_STOP 0
#define IDC_DEBUG 1
#define IDC_IGNORE 3

#define BREAK_INSTEAD_OF_ASSERT 0 // make sure you use do-not-check-in (no dashes) if you set this

typedef struct _EXCEPTION_POINTERS *PEXCEPTION_POINTERS;

#define ASSERTMODE_DEBUGBUTTONS        (1 << 0)
#define ASSERTMODE_ERRORREPORT        (1 << 1)
#define ASSERTMODE_MINIDUMP            (1 << 2)
#define ASSERTMODE_THROWEXCEPTION    (1 << 3)
#define ASSERTMODE_DATEDMINIDUMPS    (1 << 4) // Name .mdmp and .dmp files based on date/time
#define ASSERTMODE_FULLDUMP            (1 << 5) // Perform a full dump (needs userdump.exe)
#define ASSERTMODE_EXIT                (1 << 6) // Exit after doing any dumps
#define ASSERTMODE_ZIPPED            (1 << 7) // Zip .mdmp and .dmp files
#define ASSERTMODE_CALLBACK            (1 << 8) // Just call the callback (in the main thread)
#define ASSERTMODE_NOIGNOREBUTTONS    (1 << 9) // Do not allow the IGNORE button on asserts

#define ASSERTMODE_STDERR            (1 << 13) // Print assert info to stderr

#define LINESTR2(line) #line
#define LINESTR(line) LINESTR2(line)
#define MESSAGEPOS __FILE__ "(" LINESTR(__LINE__) ") : "

void assertInitThreadFreezing();
void assertDoNotFreezeThisThread(void* thread, unsigned long threadID);
void assertYouMayFreezeThisThread(void* thread, unsigned long threadID);
void setAssertProgramVersion(const char* versionName);
void setAssertExtraInfo(const char* info);            // used for map id's - added to assert info
void setAssertExtraInfo2(const char* info);            // used for driver versions, etc - added to assert info
void setAssertGameProgressString(const char* info);            // used for game startup progress
void setAssertUnitTesting(bool enable);                // disable some SuperAssert functionality to allow unit tests to work
void setAssertOpenGLReport(const char *fileName);
void setAssertLauncherLogs(const char *fileName);
void setAssertAuthName(const char* authName);
void setAssertEntityName(const char* entityName);
void setAssertShardName(const char* shardName);
void setAssertShardTime(int shardTime);

#ifndef DISABLE_ASSERTIONS
    //#pragma message ("DEBUG ASSERT ###################################")
    int __cdecl superassert(const char* expr, const char *errormsg, const char* filename, unsigned lineno);
    int __cdecl superassertf(const char* expr, FORMAT errormsg_fmt, const char* filename, unsigned lineno, ...);
    extern int *g_NULLPTR;
    #define FORCE_CRASH     ((*g_NULLPTR)++)

    // Assumes max length of CRYPTIC_MAX_PATH
    char * assertGetMiniDumpFilename();
    char * assertGetFullDumpFilename();

    void assertWriteMiniDump(char* filename, PEXCEPTION_POINTERS info);
    void assertWriteMiniDumpSimple(PEXCEPTION_POINTERS info);
    void assertWriteFullDump(char* filename, PEXCEPTION_POINTERS info);
    void assertWriteFullDumpSimple(PEXCEPTION_POINTERS info);

    typedef void (*ErrorCallback)(char* errMsg);

    void setAssertMode(int assertmode);
    int getAssertMode();
    void setAssertCallback(ErrorCallback func); // This callback will occur in a second thread, so be careful what you do!

    int programIsShuttingDown(void);
    void setProgramIsShuttingDown(int val);

    #define exit(retcode) { setProgramIsShuttingDown(1); exit(retcode); }

    #ifndef FINAL
    int assertIsDevelopmentMode(void);
    #else
    #define assertIsDevelopmentMode() 0
    #endif

    // if submitting error reports, force an error to be caught instead of asserting 
    int assertSubmitErrorReports();
    extern int g_ignoredivzero;
    void AssertErrorf(const char * file, int line, FORMAT fmt, ...);
    void AssertErrorFilenamef(const char *file, int line, const char *filename, FORMAT fmt, ...);

    int isCrashed(void);
    
    #if BREAK_INSTEAD_OF_ASSERT
#define failmsgf(exp, msg, ...) \
	(printf(msg ? msg : "", ##__VA_ARGS__), __debugbreak(), \
	 COH_ASSUME(exp), 0)
    #else
#define failmsgf(exp, msg, ...) \
	(superassertf(#exp, msg, __FILE__, __LINE__, ##__VA_ARGS__) ? \
	 FORCE_CRASH : 0, COH_ASSUME(exp), 0)
    #endif

#define assertmsgf(exp, msg, ...) do { \
	if ((exp)) { \
		COH_NOP(); \
	} else { \
		failmsgf(exp, msg, ##__VA_ARGS__); \
	} \
} while (0)
    #define assertmsg(exp, msg)    assertmsgf(exp, msg) 
    #if defined(assert)
    #undef assert
    #endif // undef assert
    #define assert(exp)    assertmsg(exp, 0)

#define devassertmsg(exp, msg, ...) \
	((exp) || (assertIsDevelopmentMode() ? \
	 failmsgf(exp, msg, ##__VA_ARGS__) : \
	 (AssertErrorf(__FILE__, __LINE__, "devassert (%s:%d): %s; " msg, \
	 __FILE__, __LINE__, #exp, ##__VA_ARGS__), 0)))
    #define devassert(exp) devassertmsg(exp, "")

    #define onlydevassert(exp) (assertIsDevelopmentMode() && devassert(exp))

    // this assertion is needed because we cannot rely on builds with asserts disabled to go live and the code inside these test are extremely slow
    #ifdef _FULLDEBUG
        #define onlyfulldebugdevassert(exp) devassert(exp)
    #else
        #define onlyfulldebugdevassert(exp) (0)
    #endif 

    #define verify(exp) ((exp) || (AssertErrorf(__FILE__, __LINE__, "VERIFY FAILED " __FILE__ "(%d): expression '" #exp "' was false", __LINE__),0))

    // wrap around all threads to catch all exceptions
    int assertExcept(unsigned int code, PEXCEPTION_POINTERS info);
    #define EXCEPTION_HANDLER_BEGIN __try {
    #define EXCEPTION_HANDLER_END } \
    __except (assertExcept(GetExceptionCode(), GetExceptionInformation())) \
    { exit(1); return 1; }

    void enableEmailOnAssert(int yesno);
    void addEmailAddressToAssert(char *address);
    void setMachineName(char *name); // this goes into the email that is sent if enableEmailOnAssert is on
#else
    int reportException(unsigned int code, PEXCEPTION_POINTERS info);

    //#pragma message ("NON DEBUG ASSERT ###################################")
    #define setAssertMode(mode) do {} while(0)
    #define getAssertMode() (0)
    #define setAssertResponse(res)    do {} while(0)
    #define setAssertCallback(func)    do {} while(0)

    #define AssertErrorf(file, line, fmt, ...) do {} while(0)
    #define AssertErrorFilenamef(file, line, filename, fmt, ...) do {} while(0)

    #define assertmsgf(exp, msg, ...) do { (void)(exp); } while(0)
    #define assertmsg(exp, msg) do { (void)(exp); } while(0)

    #if defined(assert)
    #undef assert
    #endif // undef assert
    #define assert(exp) do { (void)(exp); } while(0)

    #define devassertmsg(exp, msg, ...) (exp)
    #define devassert(exp) (exp)

    #define onlydevassert(exp) (0)
    #define onlyfulldebugdevassert(exp) (0)

    #define verify(exp) (exp)

    #define assertExcept(code, info) (EXCEPTION_CONTINUE_SEARCH)
    #define enableEmailOnAssert(yesno) do {} while(0)
    #define addEmailAddressToAssert(address) do {} while(0)
    #define setMachineName(name) do {} while(0)
    #define EXCEPTION_HANDLER_BEGIN __try {
    #define EXCEPTION_HANDLER_END } \
    __except (reportException(GetExceptionCode(), GetExceptionInformation())) \
    { exit(1); return 1; }
#endif

#ifdef __MINGW32__
/* Install this process's crash-reporting filter once, safely across threads.
 * Initialize reporting with memCheckInit during startup before calling this.
* Unhandled faults use assertExcept/reportException; attached debuggers retain
* Windows' normal exception dispatch. Later SetUnhandledExceptionFilter calls
* can replace this handler. This does not emulate local SEH recovery/unwinding.
*/
void assertInstallExceptionHandler(void);
#undef EXCEPTION_HANDLER_BEGIN
#undef EXCEPTION_HANDLER_END
#define EXCEPTION_HANDLER_BEGIN { assertInstallExceptionHandler();
#define EXCEPTION_HANDLER_END }
#endif

#ifdef __cplusplus
}
#endif

#endif
