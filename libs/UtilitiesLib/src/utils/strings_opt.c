#include <string.h>
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/memcheck.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/utils/timing.h"

// Note: currently, opt_strnicmp and opt_strupr are not used

int opt_strnicmp(const char * src, const char * dst, size_t count)
{
    if ( count )
    {
        //        if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE ) {
        int f, l;
        do {

            if ( ((f = (unsigned char)(*(src++))) >= 'A') &&
                (f <= 'Z') )
                f -= 'A'-'a';

            if ( ((l = (unsigned char)(*(dst++))) >= 'A') &&
                (l <= 'Z') )
                l -= 'A'-'a';

        } while ( --count && f && (f == l) );
        //        }
        //        else { // localized version?
        //            int f,l;
        //            do {
        //                f = tolower( (unsigned char)(*(dst++)) );
        //                l = tolower( (unsigned char)(*(src++)) );
        //            } while (--count && f && (f == l) );
        //        }

        return( f - l ); // note: that's not a "one" it's an "L"
    }
    return 0;
}

char * opt_strupr (char * string)
{
    //    if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE )
    //    {
    char *cp;       /* traverses string for C locale conversion */

    for ( cp = string ; *cp ; ++cp )
        if ( ('a' <= *cp) && (*cp <= 'z') )
            *cp -= 'a'-'A';

    return(string);
    //    }   /* C locale */
    //
    //    return _strupr(string); // call localized version which handles all sorts of stuff.
}

int __cdecl opt_stricmp (const char * dst, const char * src)
{
    //    if ( __lc_handle[LC_CTYPE] == _CLOCALEHANDLE ) {
    int f, l;

    do {
        if ( ((f = (unsigned char)(*(dst++))) >= 'A') &&
                (f <= 'Z') )
            f -= 'A' - 'a';
        if ( ((l = (unsigned char)(*(src++))) >= 'A') &&
                (l <= 'Z') )
                l -= 'A' - 'a';
    } while ( f && (f == l) );

    //    } else { // localized version
    //        int f,l;
    //        do {
    //            f = tolower( (unsigned char)(*(dst++)) );
    //            l = tolower( (unsigned char)(*(src++)) );
    //        } while ( f && (f == l) );
    //    }

    return(f - l);
}

// Concatenates two strings, up to a max number, and returns the number of characters copied
int strncat_count(char *strDest, const char *strSource, int max)
{
    register char *c=strDest;
    register const char *s;
    int left = max;
    while (*c && left) {
        c++;
        left--;
    }
    for (s=strSource;*s && left; )
    {
        *c++=*s++;
        left--;
    }
    if (!left) {
        *--c=0;
        left++;
    }
    else { 
        *c = 0;  //added to make sure string is always terminated after leaving function 
    }
    
    return max - left;

}


/***  copied from CRT, removed threaded code
*long atol(char *nptr) - Convert string to long
*
*Purpose:
*       Converts ASCII string pointed to by nptr to binary.
*       Overflow is not detected.
*
*Entry:
*       nptr = ptr to string to convert
*
*Exit:
*       return long int value of the string
*
*Exceptions:
*       None - overflow is not detected.
*
*******************************************************************************/

long __cdecl opt_atol(const char *nptr)
{
        int c;              /* current char */
        long total;         /* current total */
        int sign;           /* if '-', then negative, otherwise positive */

        while ( *nptr == ' ' ) // was isspace
            ++nptr;

        c = (int)*nptr++;
        sign = c;           /* save sign indication */
        if (c == '-' || c == '+')
            c = (int)*nptr++;    /* skip sign */

        total = 0;

        while ( (c = _tchartodigit(c)) != -1 ) {
            total = 10 * total + c;     /* accumulate digit */
            c = *nptr++;    /* get next char */
        }

        if (sign == '-')
            return -total;
        else
            return total;   /* return result, negated if necessary */
}

S64 opt_atol64(const char *nptr)
{
    int c;              /* current char */
    S64 total;         /* current total */
    int sign;           /* if '-', then negative, otherwise positive */

    while ( *nptr == ' ' ) // was isspace
        ++nptr;

    c = (int)*nptr++;
    sign = c;           /* save sign indication */
    if (c == '-' || c == '+')
        c = (int)*nptr++;    /* skip sign */

    total = 0;

    while ( (c = _tchartodigit(c)) != -1 ) {
        total = 10 * total + c;     /* accumulate digit */
        c = *nptr++;    /* get next char */
    }

    if (sign == '-')
        return -total;
    else
        return total;   /* return result, negated if necessary */
}

double opt_atof(const char *s) {
    register const char *c=s;
    double ret=0;
    int sign=1;
    if (*c=='-') { sign=-1;  c++; }
    while (*c) {
        if (*c=='.') {
            double mult=1.0;
            c++;
            while (*c) {
                mult*=0.1;
                if (*c>='0' && *c<='9') {
                    ret+=mult*(*c-'0');
                    c++;
                } else { // bad character
                    if (*c=='e') {
                        int exponent = atoi(++c);
                        if (exponent) {
                            return atof(s);
                        }
                        return ret*sign;
                    }
                    return atof(s);
                }
            }
            return ret*sign;
        }
        ret*=10;
        if (*c>='0' && *c<='9') {
            ret+=*c-'0';
            c++;
        } else {
            return atof(s);
        }
    }
    return ret*sign;
}

// Underscore insensitive stricmp
int striucmp(const char * src, const char * dst)
{
    int f, l;
    do {
        while (*src=='_') src++;
        while (*dst=='_') dst++;

        if ( ((f = (unsigned char)(*(src++))) >= 'A') &&
            (f <= 'Z') )
            f -= 'A'-'a';

        if ( ((l = (unsigned char)(*(dst++))) >= 'A') &&
            (l <= 'Z') )
            l -= 'A'-'a';

    } while ( f && (f == l) );

    return( f - l ); // note: that's not a "one" it's an "L"
}

char *stripUnderscores(const char *src) // Returns a non-threadsafe static buffer
{
    static char buffer[1024];
    char *c;
    char *out;
    Strcpy(buffer, src);
    for (c=out=buffer; *c; c++) {
        if (*c!='_') {
            *out++ = *c;
        }
    }
    *out = 0;
    return buffer;
}
