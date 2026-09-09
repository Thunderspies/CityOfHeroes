
#pragma once

#include <stddef.h> // for size_t
#include <ctype.h>

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

#define _tchartodigit(c)    ((c) >= '0' && (c) <= '9' ? (c) - '0' : -1)
#define atoi(s) opt_atol(s)
#define atoi64(s) opt_atol64(s)
#define SAFE_ATOI(S) ((S)?atoi(S):0)
#define SAFE_ATOF(S) ((S)?atof(S):0)

double opt_atof(const char *s);
long __cdecl opt_atol(const char *nptr);
S64 opt_atol64(const char *nptr);

int opt_strnicmp(const char *first, const char *last, size_t count);
int opt_stricmp(const char * first, const char *second);

int strncat_count(char *strDest, const char *strSource, int max);
int striucmp(const char * src, const char * dst);
char *stripUnderscores(const char *src); // Returns a non-threadsafe static buffer

// Efficient string concatination routines
#define STR_COMBINE_BEGIN_S(dest, bufferSize) \
{ \
    char *c=dest; \
    const char *s=0; \
    char* bufStart= dest; \
    size_t bufSize = bufferSize;
#define STR_COMBINE_BEGIN(dest) STR_COMBINE_BEGIN_S(dest, ARRAY_SIZE_CHECKED(dest))
#define STR_COMBINE_CAT(source) \
    for (s=source;*s; *c++=*s++);
#define STR_COMBINE_CAT_C(source_char) \
    *c++=source_char;
#define STR_COMBINE_CAT_D2(source_int) \
    *c++=((source_int)/10)+'0'; \
    *c++=((source_int)%10)+'0';
#define STR_COMBINE_CAT_D(source_int) \
    _itoa_s(source_int, c, bufSize - (c-bufStart), 10); \
    c += strlen(c);
#define STR_COMBINE_END() \
    *c=0; \
    UNUSED(s); \
    UNUSED(bufStart); \
    UNUSED(bufSize); \
}

// Preset combos.  Ex: STR_COMBINE_DSS(x, d0, s0, s1) is same as sprintf(x, "%d%s%s", d0, s0, s1)

#define STR_COMBINE_BEGIN_END(dest, x)        STR_COMBINE_BEGIN(dest);x;STR_COMBINE_END();
#define STR_COMBINE_SS(dest, s0, s1)        STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);)
#define STR_COMBINE_SD(dest, s0, d1)        STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT_D(d1);)
#define STR_COMBINE_SSS(dest, s0, s1, s2)    STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT(s2);)
#define STR_COMBINE_DSS(dest, d0, s1, s2)    STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT_D(d0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT(s2);)
#define STR_COMBINE_SDS(dest, s0, d1, s2)    STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT_D(d1);STR_COMBINE_CAT(s2);)
#define STR_COMBINE_SSD(dest, s0, s1, d2)    STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT_D(d2);)
#define STR_COMBINE_SSSS(dest, s0, s1, s2, s3)        STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT(s2);STR_COMBINE_CAT(s3);)
#define STR_COMBINE_SSDS(dest, s0, s1, d2, s3)        STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT_D(d2);STR_COMBINE_CAT(s3);)
#define STR_COMBINE_SSSD(dest, s0, s1, s2, d3)        STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT(s2);STR_COMBINE_CAT_D(d3);)
#define STR_COMBINE_SSSSS(dest, s0, s1, s2, s3, s4)    STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT(s2);STR_COMBINE_CAT(s3);STR_COMBINE_CAT(s4);)
#define STR_COMBINE_SSSSD(dest, s0, s1, s2, s3, d4)    STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT(s2);STR_COMBINE_CAT(s3);STR_COMBINE_CAT_D(d4);)
#define STR_COMBINE_SDSDS(dest, s0, d0, s1, d1, s2)    STR_COMBINE_BEGIN_END(dest, STR_COMBINE_CAT(s0);STR_COMBINE_CAT_D(d0);STR_COMBINE_CAT(s1);STR_COMBINE_CAT_D(d1);STR_COMBINE_CAT(s2);)

/* example:
STR_COMBINE_BEGIN(dest);
STR_COMBINE_CAT(s0);
STR_COMBINE_CAT(s1);
STR_COMBINE_END();
*/

C_DECLARATIONS_END
