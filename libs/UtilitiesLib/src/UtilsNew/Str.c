/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 *
 * Module Description:
 *
 *
 ***************************************************************************/
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/UtilsNew/ncstd.h"
#include "utilitieslib/UtilsNew/Array.h"
#include "utilitieslib/UtilsNew/Str.h"
#include "utilitieslib/UtilsNew/ncHash.h"

#ifdef _DEBUG
#  define CHECK_STR_SIZE(HSTR) Str_len(HSTR) // size checks automatically
#else
#  define CHECK_STR_SIZE(HSTR)
#endif

// works in conjunction with the macro invoking it.
// if mem is NULL allocation has exceeded alloca max size
Str Str_temp_dbg(void *mem, size_t mem_size DBG_PARMS)
{
    char *str = achr_init_dbg(mem, mem_size, ARRAYFLAG_ALLOCA DBG_PARMS_CALL);
    if(str)
        achr_push(&str, '\0');
    return str;
}

int Str_len_dbg(Str *hstr DBG_PARMS)
{
	int n = achr_size_dbg((char const *const *)hstr DBG_PARMS_CALL) - 1;
#ifdef CHECK_STR_SIZE
    if(hstr && *hstr)
    {
        int sl = (int)strlen(*hstr);
        assertmsgf(n == sl,"string length %i doesn't match Str len %i", sl, n);
    }
#endif
    return MAX(n,0); // -1 if not initted
}

// ab: this doesn't necessarily make sense. how do you expand a string? if size goes from 1 to 5 is it nulls
//     in the string? if so what does concat do?
// #define Str_setsize(hstr,size) Str_setsize_dbg(hstr, size DBG_PARMS_INIT)
// void Str_setsize_dbg(Str *hstr, int size DBG_PARMS)
// {
//     achr_setsize_dbg(hstr,size+1 DBG_PARMS_CALL);   // make room for terminator
//     (*hstr)[Str_len_dbg(hstr DBG_PARMS_CALL)] = 0; // terminate
// }

int Str_vcatf_dbg(char** hstr DBG_PARMS, FORMAT format, va_list args)
{
    int s,n,r;
    if(!hstr)
        return 0;
    CHECK_STR_SIZE(hstr);
    n = _vscprintf(format,args);
    if(!*hstr)
        s = 1;
    else
        s = achr_size(hstr);                                // size includes NULL
    achr_setsize_dbg(hstr,s+n DBG_PARMS_CALL);
    r = vsprintf_s((*hstr)+s-1,n+1,format,args);     // print from NULL term on
    return r;
}

int Str_catf_dbg(char** hstr DBG_PARMS, FORMAT format, ...)
{
    int r;
    if(!hstr)
        return 0;
    VA_START(args, format);
    r = Str_vcatf_dbg(hstr DBG_PARMS_CALL, format, args);
    VA_END();
    return r;
}


int Str_vprintf_dbg(Str *hstr DBG_PARMS, FORMAT format, va_list args)
{
    achr_setsize_dbg(hstr,1 DBG_PARMS_CALL); // 1 for NULL
    **hstr = 0;
    return Str_vcatf_dbg(hstr DBG_PARMS_CALL, format, args);
}

int Str_printf_dbg(Str *hstr, const char *caller_fname, int line, FORMAT format, ...)
{
    int n;
    VA_START(args, format);
    n = Str_vprintf_dbg(hstr, caller_fname, line, format, args);
    VA_END();
    return n;
}

void Str_catprintable_dbg(Str *hstr, const char *src, char def DBG_PARMS)
{
    achr_pop_dbg(hstr DBG_PARMS_CALL);
    for(; *src; src++)
    {
        if(isprint((unsigned char)*src))
            achr_push_by_cp_dbg(hstr, *src DBG_PARMS_CALL);
        else if(def)
            achr_push_by_cp_dbg(hstr, def DBG_PARMS_CALL);
    }
    achr_push_by_cp_dbg(hstr, '\0' DBG_PARMS_CALL);
}

void Str_caturi_dbg(Str *hstr, char *src DBG_PARMS)
{
    char hex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
    achr_pop_dbg(hstr DBG_PARMS_CALL);
    for(; *src; src++)
    {
        if(*src == ' ')
        {
            achr_push_by_cp_dbg(hstr, '+' DBG_PARMS_CALL);
        }
        else if(isalnum((unsigned char)*src) || strchr("$-_.!*'()", *src))
        {
            achr_push_by_cp_dbg(hstr, *src DBG_PARMS_CALL);
        }
        else
        {
            achr_push_by_cp_dbg(hstr, '%' DBG_PARMS_CALL);
            achr_push_by_cp_dbg(hstr, hex[((unsigned char)*src)/16] DBG_PARMS_CALL);
            achr_push_by_cp_dbg(hstr, hex[((unsigned char)*src)%16] DBG_PARMS_CALL);
        }
    }
    achr_push_by_cp_dbg(hstr, '\0' DBG_PARMS_CALL);
}

void Str_cathtml_dbg(Str *hstr, char *src DBG_PARMS)
{
    for(; *src; src++)
    {
        switch(*src)
        {
            xcase '&':    Str_catf_dbg(hstr DBG_PARMS_CALL, "&amp;");
            xcase '\"':    Str_catf_dbg(hstr DBG_PARMS_CALL, "&quot;");
            xcase '\'':    Str_catf_dbg(hstr DBG_PARMS_CALL, "&apos;");
            xcase '<':    Str_catf_dbg(hstr DBG_PARMS_CALL, "&lt;");
            xcase '>':    Str_catf_dbg(hstr DBG_PARMS_CALL, "&gt;");
            xdefault:    Str_catf_dbg(hstr DBG_PARMS_CALL, "%c", *src);
        }
    }
}

void Str_destroy_dbg(Str *hstr DBG_PARMS)
{
    achr_destroy_dbg(hstr,0 DBG_PARMS_CALL);
}

void Str_clear_dbg(Str *hstr DBG_PARMS)
{
    if(hstr)
    {
        achr_setsize_dbg(hstr,1 DBG_PARMS_CALL);
        (*hstr)[0] = '\0';
    }        
}

// strtok into a Str, advance passed pointer
char *Str_tok_dbg(Str *hres,char const **ppch, char const *delims DBG_PARMS)
{
    S32 lookup[256/32] = {0};
    if(!ppch || !*ppch || !hres)
        return NULL;
    for(;*delims;delims++)
        bit_set(lookup,*delims);
    bit_set(lookup,0); // add NULL
    
    // skip leading
    while(*(*ppch) && bit_tst(lookup,*(*ppch)))
        (*ppch)++;
    if(!*delims)
        return NULL;
    for(;!bit_tst(lookup,*(*ppch));(*ppch)++)
        *achr_push_dbg(hres DBG_PARMS_CALL) = *(*ppch);
    return *hres;
}

void Str_ncpy_dbg(Str *hres, const char *src, int n DBG_PARMS)
{
    n = MIN(n, (int)(src ? strlen(src) : 0)); // assert on null src instead?
    achr_setsize_dbg(hres, n+1 DBG_PARMS_CALL);
    memcpy(*hres, src, n);
    (*hres)[n] = 0;
}


void Str_setsize_dbg(Str *hstr, int n DBG_PARMS)
{
    if(!hstr)
        return;
    achr_setsize_dbg(hstr,n+1 DBG_PARMS_CALL);
    (*hstr)[n] = 0;
}

void Str_cp_raw_dbg(Str *hstr, const char *src DBG_PARMS)
{
    int n;
    if(!hstr || !src)
        return;
    n = strlen(src);
    achr_cp_raw_dbg(hstr,src,n+1 DBG_PARMS_CALL);
}

Str Str_dup_dbg(const char *src DBG_PARMS)
{
    Str ret = NULL;
    Str_cp_raw_dbg(&ret, src DBG_PARMS_CALL);
    return ret;
}
