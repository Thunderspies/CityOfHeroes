#include <stdio.h>
#include <stdarg.h>
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/assert/assert.h"

#undef sprintf_s
#undef snprintf_s
#undef vsprintf_s
#undef vsnprintf_s
#undef _vsnprintf_s

#define SPRINT_EVERYTHING INT_MAX

static INLINEDBG void safecopy(char *src,char **dst,int *space)
{
    int        len = (int)strlen(src);

    *space -= len;
    if (*space < 0)
    {
        len += *space;
        *space = 0;
    }
    memcpy(*dst,src,len);
    *dst += len;
}

static INLINEDBG int core_vsnprintf(char *buf,size_t buf_size,size_t max_bytes,const char* fmt, va_list ap_orig)
{
    va_list        ap = ap_orig;
    const        char *sc;
    char        *tail = buf,temp[200];
    int            space = max_bytes == _TRUNCATE ? (int)(buf_size-1) : (int)max_bytes;

    for(sc = fmt; *sc && space > 0; sc++)
    {
        if (sc[0] == '%')
        {
            switch(sc[1])
            {
                xcase 's':
                {
                    char *s = va_arg(ap,char *);
                    if (!s)
                        s = "(null)";
                    safecopy(s,&tail,&space);
                }
                xcase 'd':
                case  'i':
                {
                    int val = va_arg(ap,int);
                    _ltoa(val,temp,10);
                    safecopy(temp,&tail,&space);
                }
                xcase 'f':
                {
                    F32 val = (F32)va_arg(ap,double);
                    sprintf_s(temp,ARRAY_SIZE_CHECKED(temp),"%f",val);
                    safecopy(temp,&tail,&space);
                }
                xcase 'F':
                {
                    F32 val = (F32)va_arg(ap,double);
                    safe_ftoa(val,temp);
                    safecopy(temp,&tail,&space);
                }
                xcase '%':
                {
                    safecopy("%",&tail,&space);
                }
                xcase ' ':   // deal with the unescaped percentage signs without calling vsnprintf
                {
                    safecopy("% ",&tail,&space);    
                }
                xdefault:
                {
                    // temporarily using the non-secure _vsnprintf to prevent CoX crashing
                    // due to incorrectly escaped %s. _CRT_SECURE_NO_DEPRECATE has been
                    // defined in the preprocessor properties to accommodate this, which
                    // should also be removed when switching back to _vsnprintf_s.
                    #undef _vsnprintf
                    return _vsnprintf(buf,max_bytes,fmt,ap_orig);
                }
            }
            sc++;
        }
        else
        {
            *tail++ = *sc;
            space--;
        }
    }
    if(max_bytes == _TRUNCATE)
        space++; // we saved a spot in the buffer for a \0, add it back

    if(space > 0)
    {
        *tail = 0;
        assert((size_t)(tail-buf+1) <= buf_size);
        return tail - buf;
    }
    else
    {
        assert((size_t)(tail-buf) <= buf_size);
        return -1;
    }
}

int quick_sprintf(char *buf,size_t buf_size,const char* fmt, ...)
{
    va_list        ap;
    int            ret;

    va_start(ap, fmt);
    ret = core_vsnprintf(buf, buf_size, SPRINT_EVERYTHING, fmt, ap);
    va_end(ap);
    return ret;
}

void quick_strcatf(char *buf, size_t buf_size, const char *fmt, ...)
{
    va_list ap;
    size_t buf_len = strlen(buf);

    va_start(ap, fmt);
    core_vsnprintf(buf+buf_len, buf_size-buf_len, SPRINT_EVERYTHING, fmt, ap);
    va_end(ap);
}

int quick_snprintf(char *buf,size_t buf_size,size_t maxlen,const char* fmt, ...)
{
    va_list        ap;
    int            ret;

    va_start(ap, fmt);
    ret = core_vsnprintf(buf,buf_size,maxlen,fmt,ap);
    va_end(ap);
    return ret;
}

int quick_vsnprintf(char *buf,size_t buf_size,size_t maxlen,const char* fmt, va_list ap)
{
    return core_vsnprintf(buf,buf_size,maxlen,fmt,ap);
}

int quick_vsprintf(char *buf,size_t buf_size,const char* fmt, va_list ap)
{
    return core_vsnprintf(buf, buf_size, SPRINT_EVERYTHING, fmt, ap);
}

int quick_vscprintf(char *format,const char *args)
{
    int        token = 0;
    char    *s,*str;

    strdup_alloca(str,format);
    for(s=str;*s;s++)
    {
        if (token && *s == 'F')
            *s = 'f'; // %f is always equal length or longer
        if (*s == '%')
            token = !token;
        else
            token = 0;
    }
    return _vscprintf(str, (char *)args);
}

#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/timing.h"
char    zzz_dir2[50] = "zombiedir";
char    *zzz_filep    = "zombiefile";

void strCombineTest(char *buffer,size_t buffer_size,char *dir2,char *fileinfo_name)
{
    //sprintf(buffer, "%s/%s", dir2, fileinfo.name);
    STR_COMBINE_BEGIN_S(buffer,buffer_size);
    STR_COMBINE_CAT(dir2);
    STR_COMBINE_CAT("/");
    STR_COMBINE_CAT(fileinfo_name);
    STR_COMBINE_END();
}

void quickSprintfPerfTest()
{
    int        i,timer,reps=1000000;
    char    buffer[1000];

    timer = timerAlloc();
    timerStart(timer);
    buffer[0] = 0;
    for(i=0;i<reps;i++)
    {
        quick_sprintf(SAFESTR(buffer), "%s/%s", zzz_dir2, zzz_filep);
    }
    printf("quick_sprintf: %f (%s)\n",timerElapsed(timer),buffer);

    timerStart(timer);
    buffer[0] = 0;
    for(i=0;i<reps;i++)
    {
        strCombineTest(SAFESTR(buffer), zzz_dir2, zzz_filep);
    }
    printf("strCombine: %f (%s)\n",timerElapsed(timer),buffer);

    timerStart(timer);
    buffer[0] = 0;
    for(i=0;i<reps;i++)
    {
        sprintf_s(SAFESTR(buffer), "%s/%s", zzz_dir2, zzz_filep);
    }
    printf("sprintf: %f (%s)\n",timerElapsed(timer),buffer);
    
    timerFree(timer);
}
