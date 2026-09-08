#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <time.h>
#include "../stdtypes.h"
#include "memcheck.h"

C_DECLARATIONS_BEGIN

// Defined here so any software that depends on UtilitiesLib will include the networking library implicitly
#pragma comment(lib, "Ws2_32.lib")

typedef struct _CHAR_INFO CHAR_INFO;
typedef struct _COORD COORD;

// Bitmasks for creating colors
typedef enum ConsoleColor
{
    COLOR_LEAVE        = -1,

    COLOR_BLACK        = 0,
    COLOR_BLUE        = 1,
    COLOR_GREEN        = 2,
    COLOR_RED        = 4,
    COLOR_CYAN        = COLOR_BLUE|COLOR_GREEN,
    COLOR_MAGENTA    = COLOR_BLUE|COLOR_RED,
    COLOR_YELLOW    = COLOR_RED|COLOR_GREEN,
    COLOR_WHITE        = COLOR_BLUE|COLOR_GREEN|COLOR_RED,

    COLOR_BRIGHT    = 8,
} ConsoleColor;

typedef struct StuffBuff
{
    int        idx;
    int        size;
    char    *buff;
}StuffBuff;

// the counter guarantees uniqueness for each time used as a param.
#define UNIQUEVAR3(VAR, CTR)  VAR ## CTR
#define UNIQUEVAR2(VAR, CTR) UNIQUEVAR3(VAR, CTR)
#define UNIQUEVAR UNIQUEVAR2(uniquevar, __COUNTER__) 
                // an example of using uniquevar. if you
                // need to use a uniquevar multipe times, use the __COUNTER__
                // in the macro itself

char *changeFileExt_s(const char *fname,const char *new_extension,char *buf, size_t buf_size);
#define changeFileExt(fname,new_extension,buf) changeFileExt_s(fname, new_extension, SAFESTR(buf))
void changeFileExtEString(const char *fname,const char *new_extension,char **buf);
char *forwardSlashes(char *path);
void forwardSlashesEString(char **path);
char *backSlashes(char *path);
char *fixDoubleSlashes(char *fn);
char *unquote(char *str);
void concatpath_s(const char *s1,const char *s2,char *full,size_t full_size);
#define concatpath(s1,s2,full) concatpath_s(s1, s2, SAFESTR(full))
void makefullpath_s(const char *dir,char *full,size_t full_size);
#define makefullpath(dir,full) makefullpath_s(dir, SAFESTR(full))
void mkdirtree(const char *path);
void rmdirtree(char *path);
void rmdirtreeEx(char* path, int forceRemove);
int isFullPath(char *dir);
int makeDirectories(const char* dirPath);
int makeDirectoriesForFile(const char* filePath);
char *strtok_r(char *target,const char *delim,char **last);
char *strtok_delims_r(char *target,const char *startdelim, const char *enddelim,char **last);
#define FOR_STRTOK(STR,DELIM,SRC) for(STR=strtok(SRC,DELIM);STR;STR=strtok(NULL,DELIM))
#define FOR_STRTOK_R(STR,DELIM,SRC,R) for(STR=strtok_r(SRC,DELIM,&R);STR;STR=strtok_r(NULL,DELIM,&R))
char* strsep2(char** str, const char* delim, char* retrievedDelim);
char* strsep(char** str, const char* delim);
char *strstri(char *s,const char *srch);
const char *strstriConst(const char *s,const char *srch);
#define STRSTRTOEND(P,SRC,MATCH) (((P)=strstr(SRC,MATCH))?((P)+=ARRAY_SIZE(MATCH)-1):NULL)
#define tokenize_line(buf, args, next_line_ptr) tokenize_line_safe(buf, args, ARRAY_SIZE(args), next_line_ptr)
int tokenize_line_safe(char *buf,char *args[],int max_args,char **next_line_ptr);
#define tokenize_line_quoted(buf, args, next_line_ptr) tokenize_line_quoted_safe(buf, args, ARRAY_SIZE(args), next_line_ptr)
int tokenize_line_quoted_safe(char *buf,char *args[],int max_args,char **next_line_ptr);
int tokenize_line_quoted_delim(char *buf,char *args[],int max_args,char **next_line_ptr, char *startdelim, char *enddelim);
void strarray_quoted(char **estr, char *startdelim,char *enddelim, char ***eaResults, int leavequotes);
char *strtok_quoted(char *target,char *startdelim,char *enddelim);
char *strtok_quoted_r(char *target,char *startdelim,char *enddelim, char **last);
char *getContainerValueStatic(char *container, char* fieldname);
int getContainerValue(char *container, char* fieldname, char* result, int size);
int system_detach(char *cmd, int minimized);
int systemf(FORMAT cmd, ...);
#define dynArrayAdd(basep,struct_size,count,max_count,num_structs) dynArrayAdd_dbg(basep,struct_size,count,max_count,num_structs MEM_DBG_PARMS_INIT)
#define dynArrayAddp(basep,count,max_count,ptr) dynArrayAddp_dbg(basep,count,max_count,ptr MEM_DBG_PARMS_INIT)
#define dynArrayAddStruct(basep,count,max_count) dynArrayAdd((void**)basep,sizeof(**(basep)),count,max_count,1)
#define dynArrayAddStructType(type,basep,count,max_count) ((type*)dynArrayAddStruct(basep,count,max_count))
#define dynArrayAddStructs(basep,count,max_count,num_structs) dynArrayAdd((void**)basep,sizeof(**(basep)),count,max_count,num_structs)
#define dynArrayFit(basep,struct_size,max_count,idx_to_fit) dynArrayFit_dbg(basep,struct_size,max_count,idx_to_fit MEM_DBG_PARMS_INIT)
#define dynArrayFitStructs(basep,max_count,idx_to_fit) dynArrayFit((void**)basep,sizeof(**(basep)),max_count,idx_to_fit)
void *dynArrayAdd_dbg(void **basep,int struct_size,int *count,int *max_count,int num_structs MEM_DBG_PARMS);
void *dynArrayAddp_dbg(void ***basep,int *count,int *max_count,void *ptr MEM_DBG_PARMS);
void *dynArrayFit_dbg(void **basep,int struct_size,int *max_count,int idx_to_fit MEM_DBG_PARMS);
bool isConsoleOpen();

void setGuiDisable(bool disable);
bool isGuiDisabled();

void newConsoleWindow(void);
void setConsoleTitle(char *msg);
#define consoleSetFGColor(fg) consoleSetColor((ConsoleColor)(fg), COLOR_LEAVE)
void consoleSetColor(ConsoleColor fg, ConsoleColor bg);
void consoleSetDefaultColor();
void consoleCapture(void);
CHAR_INFO *consoleGetCapturedData(void);
void consoleGetCapturedSize(COORD* coord);
void consoleGetDims(COORD* coord);
void consoleBringToTop(void);
void consoleInit(int minWidth, int minHeight, int bufferLines);
int consoleYesNo(void);
int printPercentageBar(int filled, int total);
void backSpace(int num, bool clear);
const char *getFileName(const char *fname);
char *getFileNameNoExt_s(char *dest, size_t dest_size, const char *filename);
#define getFileNameNoExt(dest, filename) getFileNameNoExt_s(SAFESTR(dest), filename)
const char *getFileNameConst(const char *fname);
char *getDirectoryName(char *fullPath);
char *getDirString(const char *path);
char *addFilePrefix(const char* path, const char* prefix);
char *getUserName(void);
char *getHostName(void);
void initStuffBuff(    StuffBuff *sb, int size );
void resizeStuffBuff( StuffBuff *sb, int size );
void clearStuffBuff(StuffBuff *sb);
void addStringToStuffBuff( StuffBuff *sb, FORMAT s, ... );
void addIntToStuffBuff( StuffBuff *sb, int i);
void addSingleStringToStuffBuff( StuffBuff *sb, const char *s);
void addEscapedDataToStuffBuff(StuffBuff *sb, void *s, int len);
void addBinaryDataToStuffBuff( StuffBuff *sb, const char *data, int len);
void freeStuffBuff( StuffBuff * sb );
int firstBits( int val );
double simpleatof(const char *s);
bool simpleMatchExact(const char *exp, const char *tomatch); // This version of simpleMatch doesn't do the weird prefix matching thing, so "the" does not match "then"
bool simpleMatch(const char *exp, const char *tomatch); // Does a simple wildcard match (only 1 '*' is supported).  simpleMatch assumes that exp is a prefix, so simpleMatch("the", "then") is true
char *getLastMatch(); // Returns the portion that matched in the last call to simpleMatch
bool matchExact(const char *exp, const char *tomatch);
bool match(const char *exp, const char *tomatch); // Supports multiple wildcards, but slow on any length strings
void OutputDebugStringf(FORMAT fmt, ... ); // Outputs a string to the debug console (useful for remote debugging, and outputing stuff we don't want users to see)
void fxCleanFileName_s(char buffer[], size_t buffer_size, const char* source);
#define fxCleanFileName(buffer, source) fxCleanFileName_s(SAFESTR(buffer), source)
void seqCleanSeqFileName(char buffer[], size_t buffer_size, const char* source);
const char *escapeData(const char *instring,int len);
const char *escapeString(const char *instring); // converts '\n' => '\\n', etc, for sending to the DBServer
const char *escapeStringn(const char *instring, unsigned int len);
static INLINEDBG int escapeDataMaxChars(int srclen) { return srclen*2; }
void escapeDataToBuffer(char *dst, const void *src, int srclen, int *dstlen); // threadsafe, dstlen must be at least escapeDataMaxChars(srclen), modifies dstlen to actual len, does not terminate
char* unescapeDataToBuffer(char *dst, const char *src, int *outlen); // threadsafe, safe for src and dst to be the same
char* unescapeStringToBuffer(char *dst, const char *src); // threadsafe, safe for src and dst to be the same
#define unescapeStringInplace(str) unescapeStringToBuffer(str, str)
const char *unescapeData(const char *instring,int *outlen);
const char *unescapeString(const char *instring); // inverse of above
const char *escapeLogString(const char *instring); // same as escapeString, only escapes CR and LF, used for log files
int strIsAlpha(const unsigned char* str);
int strIsNumeric(const unsigned char* str);
int strHasSQLEscapeChars(const unsigned char* str);
int strEndsWith(const char* str, const char* ending);
int strStartsWith(const char* str, const char* start);
#define printDate(date, buf) printDate_s(date, SAFESTR(buf))
char *printDate_s(__time32_t date, char *buf, size_t buf_size);
#if !defined(_LIB) && !SECURE_STRINGS
char *strncpyta(char **buf, const char *s, int n); // Does a strncpy, and allocates the buffer if null, null terminates
#endif
#define Strncpyt(dest, src) strncpyt(dest, src, ARRAY_SIZE_CHECKED(dest))
char *strncpyt(char *buf, const char *s, int n); // Does a strncpy, null terminates
char *strcpy_unsafe(char *dst, const char *src);
#define Vsprintf(dst, fmt, ap) vsprintf_s(dst, ARRAY_SIZE_CHECKED(dst), fmt, ap)
#define Vsnprintf(dst, count, fmt, ap) vsprintf_s(dst, ARRAY_SIZE_CHECKED(dst), count, fmt, ap)
#define Strncatt(dest, src) strncat_s(dest, ARRAY_SIZE_CHECKED(dest), src, _TRUNCATE)
#define strncatt(dest, src) strncat_s(dest, ARRAY_SIZE_CHECKED(dest), src, _TRUNCATE)
char *strInsert( char * dest, const char * insert_ptr, const char *insert ); // returns static ptr to string with 'insert' inserted at the insert_ptr of dest
char *strchrInsert( const char * dest, const char * insert, int character ); // inserts 'insert' at every character location
char *strstrInsert( const char * src, const char * find, const char * replace );    // find-and-replace within a string
void strchrReplace( char *dest, int find, int replace );// replace 'find' characters with 'replace' characters in string
char *strchrReplaceStatic(char const *str, char c, char r); // make static copy of string.
char *strchrRemoveStatic(char const *str, char c); // remove character
void strReplaceWhitespace( char *dest, int replace );// replace whitespace characters with 'replace' character in string
void strToFileName(char *dest); // make a valid filename out of a string
#define strstriReplace(src, find, replace) strstriReplace_s(SAFESTR(src), find, replace)
void strstriReplace_s(char *src, size_t src_size, const char *find, const char *replace); // replace "find" string with "replace" string in src (case insensitive)
void* loadCrashRptDll();
void removeLeadingAndFollowingSpaces(char * str);
void removeLeadingAndFollowingSpacesUnlessAllSpaces(char * str);
char *incrementName(unsigned char *name, unsigned int maxlen);
void *zipData(const void *src,U32 src_len,U32 *zip_size_p);
void disableWow64Redirection(void);    // IMPORTANT: this screws up winsock
void enableWow64Redirection(void);
__time32_t statTimeToUTC(__time32_t statTime);
__time32_t statTimeFromUTC(__time32_t utcTime);
#define getAutoDbName(server_name) getAutoDbName_s(SAFESTR(server_name))
void getAutoDbName_s(char *server_name, size_t server_name_size); // Determines appropriate DbServer from data directory, modifies server_name
int createShortcut(char *file, char *out, int icon, char *working_dir, char *args, char *desc);
intptr_t spawnProcess(char *cmdLine, int mode); // spawn a process. mode should be either _P_WAIT (for synchronous) or _P_NOWAIT (for asynchronous).  returns either the return value of the process (synchronous), or the process handle (asynchronous)

#define printUnit(buf, val) printUnit_s(SAFESTR(buf), val)
char *printUnit_s(char *buf, size_t buf_size, S64 val);
#define printTimeUnit(buf, val) printTimeUnit_s(SAFESTR(buf), val)
char *printTimeUnit_s(char *buf, size_t buf_size, U32 val);
void printfColor(int color, FORMAT str, ...);

char* getCommaSeparatedInt(S64 x);

void swap(void *a, void *b, size_t width);
#define SWAP32(a,b) {int tempIntForSwap=(a);(a)=(b);(b)=tempIntForSwap;} // a swap for 32-bit or smaller integer variables
#define SWAPF32(a,b) {float tempFloatForSwap=(a);(a)=(b);(b)=tempFloatForSwap;} // a swap for 32-bit floating point variables
#define SWAPP(a,b) {void *tempPointerForSwap=(a);(a)=(b);(b)=tempPointerForSwap;}
#define SwapStruct(a,b) (sizeof(*(a))==sizeof(*(b)) ? swap((a),(b),sizeof(*(a))) : (void)(*((int*)0x00)=1))

typedef struct Packet Packet;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;
StashTable receiveHashTable(Packet * pak, int mode);
void sendHashTable(Packet * pak, StashTable table);

//JE: Changed this to a function, since where it was used, there was lots of dereferencing
//    going on in the parameters... sped it up by ~20% acording to VTune
static INLINEDBG F32 CLAMPF32(F32 var, F32 min, F32 max) {
    return ((var) < (min)? (min) : ((var) > (max)? (max) : (var)));
}
static INLINEDBG void CLAMPVEC4(Vec4 v, F32 min, F32 max)
{
    v[0] = CLAMPF32(v[0], min, max);
    v[1] = CLAMPF32(v[1], min, max);
    v[2] = CLAMPF32(v[2], min, max);
    v[3] = CLAMPF32(v[3], min, max);
}
static INLINEDBG void CLAMPVEC3(Vec4 v, F32 min, F32 max)
{
    v[0] = CLAMPF32(v[0], min, max);
    v[1] = CLAMPF32(v[1], min, max);
    v[2] = CLAMPF32(v[2], min, max);
}
#define CLAMP(var, min, max) \
    ((var) < (min)? (min) : ((var) > (max)? (max) : (var)))

#define SEQ_NEXT(var,min,max) (((var) + 1) >= (max) ? (min) : ((var) + 1))
#define SEQ_PREV(var,min,max) ((var) <= (min) ? ((max) - 1) : ((var) - 1))

// ------------------------------------------------------------
// ranges

#define INRANGE( var, min, max ) ((var) >= min && (var) < max)
#define INRANGE0(var, max) INRANGE(var, 0, max)
#define AINRANGE( var, array ) INRANGE0(var, ARRAY_SIZE(array) )
#define AGET(array,i) (AINRANGE(i,array)?((array)[i]):NULL)
#define EAINRANGE( index, eavar ) (INRANGE0( index, eaSize( &(eavar))))
#define EAINTINRANGE( index, eaintvar ) (INRANGE0( index, eaiSize(&(eaintvar))))

// ------------------------------------------------------------
// bit ops

#define POW_OF_2(var) (!((var) & ((var) - 1)))
static INLINEDBG int get_num_bits_set( int v )
{
    int c; // c accumulates the total bits set in v
    for (c = 0; v; c++)
    {
        v &= v - 1; // clear the least significant bit set
    }    
    return c;
}

//------------------------------------------------------------
//  A simple merge-sort, keeps the relative order of items.
// ex: a1 b2 c2 d0 (sorted by abc)
// would become: d0 a1 b2 c2 (sorted by 012)
// as opposed to qsort which could produce d0 a1 c2 b2 -or- d0 a1 b2 c2
// worst case O(n^2), but is O(n) on an already sorted list (faster than qsort)
//----------------------------------------------------------
void stableSort(void *base, size_t num, size_t width, const void* context,
                int (__cdecl *comp)(const void* item1, const void* item2, const void* context));

// returns location where this key should be inserted into array (can be equal to n)
size_t bfind(void* key, void* base, size_t num, size_t width, int (__cdecl *compare)(const void*, const void*));

#define strdup_alloca(dst,src) {const char* srcCopy=src; size_t len = strlen(srcCopy)+1; dst = (char*)_alloca(len);strcpy_s(dst,len,srcCopy);}

const char* unescapeAndUnpack(const char *data);
const char* packAndEscape(const char *str);
char* hexStringFromIntList(const int* pIntList, U32 uiNumInts);
int* intListFromHexString(const char* pcString, U32 uiNumInts);
int isHexChar(char h);
U8 *base64_decode(U8 **pres, int *res_len, char *s);
char *base64_encode(char **pres, U8 *s, int length);

int atoi_ignore_nonnumeric(char *pch);
char *itoa_with_grouping(int i, char *buf, int radix, int groupsize, int decimalplaces, char sep, char dp);
char *itoa_with_commas(int i, char *buf);
char *itoa_with_commas_static(int i);
#undef _beginthreadex    // defined in stdtypes.h to include_utils_h_for_threads
#undef CreateThread
uintptr_t x_beginthreadex(void *security, unsigned stack_size,
                          unsigned ( __stdcall *start_address )( void * ),
                          void *arglist, unsigned initflag, unsigned *thrdaddr,
                          char* filename, int linenumber);
#define _beginthreadex(security,stack,start,args,flags,ptid) x_beginthreadex(security,stack,start,args,flags,ptid,(char*)__FILE__,__LINE__)
#define CreateThread(security,stack,start,args,flags,ptid) (HANDLE)x_beginthreadex(security,stack,start,args,flags,ptid,(char*)__FILE__,__LINE__)

void reverse_bytes(U8 *s,int len);

char *loadCmdline(char *cmdFname,char *buf,int bufsize); // for loading cmdline.txt

typedef enum OutputLevel {
    OUTPUT_INFO,
    OUTPUT_VERBOSE,
    OUTPUT_DEBUG,
    OUTPUT_WARNING,
    OUTPUT_ERROR
} OutputLevel;
void writeConsole(OutputLevel level, const char *format, ...);

C_DECLARATIONS_END

#endif // _UTILS_H
