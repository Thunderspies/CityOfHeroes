#ifndef ESTRING_H
#define ESTRING_H

#include <malloc.h>
#include <string.h>
#include <stdarg.h>

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

// ARM NOTE: Is there a way to validate that we are calling the estr*EString functions
//   with valid eString parameters without incurring so much overhead that we might as well
//   just call the estr*CharString functions instead?

//---------------------------------------------------------------------------------
// EString: narrow/UTF-8 character interface
//---------------------------------------------------------------------------------

void estrCreate(char** str);
void estrCreateEx(char** str, int initLength);

// Please note that these two break eString convention
//   and return the new eStrings instead of using the first parameter!
char* estrCloneEString(const char* src);
char* estrCloneCharString(const char* src);

void estrDestroy(char** str);
static INLINEDBG void estrDestroyUnsafe(char* str) { estrDestroy(&str); } // for use in e.g. eaDestroyEx
void estrClear(char** str);

unsigned int estrGetCapacity(const char** str);
unsigned int estrReserveCapacity(char** str, unsigned int newCapacity);

unsigned int estrLength(const char* const * str);
unsigned int estrSetLength(char** str, unsigned int newLength);
unsigned int estrSetLengthNoMemset(char** str, unsigned int newLength); // Doesn't clear the new string

unsigned int estrUTF8CharacterCount(const char** str);    // Returns UTF-8 character count, NOT THE SAME AS estrLength()!

//---------------------------------------------------------------------------------
// estrPrint* family:
// Clears the eString before copying.  Call estrConcat* if you don't want to clear.
//---------------------------------------------------------------------------------
void estrPrintEString(char** str, const char* src);
void estrPrintCharString(char** str, const char* src);
int estrPrintf(char** str, const char* format, ...); // NEVER call this with no parameters or a format string of just "%s", use a different estrPrint*()!

//---------------------------------------------------------------------------------
// estrConcat* family:
// If you need to concat a single character, use estrConcatChar().
// else if you need to concat an EString, use estrConcatEString().
// else if you need to concat a static character array (a char[] or a quoted literal "string"),
//   use estrConcatStaticCharArray().
// else if you already know the length of the source (without calling strlen()),
//   use estrConcatFixedWidth().
// else use estrConcatCharString().
//
// estrConcatMinimumWidth() has different behavior!  This will post-fill spaces to guarantee
//   you add at least minWidth characters to the eString.  It can add more than the minimum!
//
// Only call estrConcatf if you really need it!  Use one of the above functions if you don't
//   have multiple parameters or have one parameter with further static
//---------------------------------------------------------------------------------
void estrConcatEString(char** str, const char* src);
void estrConcatCharString(char** str, const char* src);
void estrConcatChar(char** str, char src);
void estrConcatFixedWidth(char** str, const char* src, int width);
void estrConcatMinimumWidth(char** str, const char* src, int minWidth); // Left aligned, fixed width
#define estrConcatStaticCharArray(STR, APPENDME) estrConcatMinimumWidth(STR, APPENDME, ARRAY_SIZE( APPENDME )-1) // doesn't use ARRAY_SIZE_CHECKED because of the false negative!
// ARM NOTE: Should estrConcatStaticCharArray() call estrConcatFixedWidth() instead of estrConcatMinimumWidth()?
int estrConcatf(char** str, const char* format, ...); // NEVER call this with no parameters or a format string of just "%s", use a different estrConcat*()!
int estrConcatfv(char** str, const char* format, va_list args);

void estrInsert(char** str, int index, const char* src, int count);
void estrRemove(char** str, int index, int count);


//---------------------------------------------------------------------------------
// EString: some utilities for zipping/unzipping estrings
//          these don't track the zipped-state of the strings
//---------------------------------------------------------------------------------

void estrPackData(char **str, const void *src, int srclen);
void estrUnpackStr(char **str, const char * const *src);
static INLINEDBG void estrPackStr(char **str, const char * const *src) { estrPackData(str, *src, estrLength(src)); }
static INLINEDBG void estrPackStr2(char **str, const char *src) { estrPackData(str, src, (int)strlen(src)); }
static INLINEDBG void estrPack(char **str) { char *tmp = NULL; const char *src = *str; estrPackStr(&tmp, &src); estrDestroy(str); *str = tmp; }
static INLINEDBG void estrUnpack(char **str) { char *tmp = NULL; const char *src = *str; estrUnpackStr(&tmp, &src); estrDestroy(str); *str = tmp; }


//---------------------------------------------------------------------------------
// EString: wide character interface
//---------------------------------------------------------------------------------

static INLINEDBG void estrWideCreate(char** str) { estrCreate((char**)str); }
static INLINEDBG void estrWideCreateEx(char** str, int initLength) { estrCreateEx((char**)str, initLength); }

static INLINEDBG void estrWideDestroy(unsigned short** str) { estrDestroy((char**)str); }
static INLINEDBG void estrWideClear(unsigned short** str) { estrClear((char**)str); }

static INLINEDBG unsigned int estrWideReserveCapacity(unsigned short** str, unsigned int reserveCapacity) { return estrReserveCapacity((char**) str, reserveCapacity * 2); }

static INLINEDBG unsigned int estrWideLength(const unsigned short** str) { return estrLength((const char**)str) / 2; }
static INLINEDBG unsigned int estrWideSetLength(unsigned short** str, unsigned int newLength) { return estrSetLength((char**)str, newLength * 2); }
static INLINEDBG unsigned int estrWideSetLengthNoMemset(unsigned short** str, unsigned int newLength) { return estrSetLengthNoMemset((char**)str, newLength * 2); }

static INLINEDBG void estrWideConcatEString(unsigned short** str, const unsigned short* src) { estrConcatEString((char**)str, (const char*)src); }
void estrWideConcatCharString(unsigned short** str, unsigned short* src);

static INLINEDBG void estrWideInsert(unsigned short** str, int index, const unsigned short* src, int count) { estrInsert((char**)str, index * 2, (const char*)src, count * 2); }
static INLINEDBG void estrWideRemove(unsigned short** str, int index, int count) { estrRemove((char**)str, index * 2, count * 2); }


//---------------------------------------------------------------------------------
// EString: wide<->narrow conversion
//---------------------------------------------------------------------------------

void estrConvertToNarrow(char **output, unsigned short *input);
void estrConvertToWide(unsigned short **output, char *input);


//---------------------------------------------------------------------------------
// EString: utility functions
//---------------------------------------------------------------------------------

void estrRemoveDoubleQuotes(char **str);


//---------------------------------------------------------------------------------
// EString: Private bits needed by stack functions below
//---------------------------------------------------------------------------------

#define ESTR_DEFAULT_SIZE 64
#define ESTR_HEADER "ESTR"

typedef struct 
{
    char header[4];                // should contain "ESTR"
    unsigned int bufferCapacity;
    unsigned int stringLength;
    bool isStack;
    union
    {
        char str[1];
        unsigned short wideStr[1];
    };
} EString;

#define EStrHeaderSize offsetof(EString, str)
#define EStrTerminatorSize 2

static INLINEDBG void estrTerminateString(EString* estr)
{
    estr->str[estr->stringLength] = 0;
    estr->str[estr->stringLength+1] = 0;
}

//---------------------------------------------------------------------------------
// EString: Stack Strings. Try to allocate the EString on the stack, going to heap on realloc
//---------------------------------------------------------------------------------

#define MAX_STACK_ESTR 10000 // maximum size of EString allowed to be allocated on stack

#define estrStackCreate(strptr, initLength) \
    if((strptr) && (initLength) > 0 && (initLength) <= MAX_STACK_ESTR) {\
        EString* estr_ZZ = (EString*)_alloca(EStrHeaderSize + EStrTerminatorSize + (initLength));\
        memcpy(estr_ZZ->header, "ESTR", 4);\
        estr_ZZ->bufferCapacity = (initLength);\
        estr_ZZ->stringLength = 0;\
        estr_ZZ->isStack = true;\
        estrTerminateString(estr_ZZ);\
        *(strptr) = estr_ZZ->str;\
    } else {\
        estrCreateEx((char**)(strptr),(initLength));\
    }

// estrTemp(), estrTempEx(bufsize)
// allocated on the stack, reallocates from the heap if necessary
// the initial call never allocates from the heap, so it's safe to use
//    estrTemp() in a declaration and return before using that variable
// estrDestroy() must be called if the variable is used!

#define estrTemp()            estrTempEx(1024-EStrHeaderSize-EStrTerminatorSize)
#define estrTempEx(bufsize)    estrInternalTempFill((EString*)_alloca((bufsize)+EStrHeaderSize+EStrTerminatorSize),(bufsize)) // change to cpp_reinterpret_cast

static INLINEDBG char* estrInternalTempFill(EString *estr, unsigned int bufsize)
{
    if(!estr)            // no more space on the stack
        return NULL;    // string will be created when it's used
    memcpy(estr->header, "ESTR", 4);
    estr->bufferCapacity = bufsize;
    estr->stringLength = 0;
    estr->isStack = 1;
    estrTerminateString(estr);
    return estr->str;
}


#ifndef ESTRING_IMPL
/* Read-only views of typed character-buffer handles. The handle may point to
* mutable or const characters; these queries never replace or edit them.
*/
#define estrLength(str) (estrLength)((const char *const *)(str))
#define estrGetCapacity(str) (estrGetCapacity)((const char **)(str))
#define estrUTF8CharacterCount(str) \
	(estrUTF8CharacterCount)((const char **)(str))
#endif

C_DECLARATIONS_END

#endif
