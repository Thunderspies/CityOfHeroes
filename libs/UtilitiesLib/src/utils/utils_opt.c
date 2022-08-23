// Some functions that used to be in utils.c but were places here so that some of utils.c can have the optimizer
// turned off
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utilitieslib/utils/utils.h"
//#include "Common.h"
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <errno.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/file.h"
#include <fcntl.h>
#include "utilitieslib/utils/wininclude.h"
#include <process.h>
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/components/earray.h"

char *forwardSlashes(char *path)
{
    char    *s;

    if(!path)
        return NULL;

    for(s=path;*s;s++)
    {
        if (*s == '\\')
            *s = '/';
        if (s!=path && (s-1)!=path && *s == '/' && *(s-1)=='/') {
            strcpy_unsafe(s, s+1);
        }
    }
    return path;
}

void forwardSlashesEString(char **path)
{
    char *s;

    if (!path || !*path)
        return;

    for (s = *path; *s; s++)
    {
        if (*s == '\\')
            *s = '/';
        if (s != *path && (s - 1) != *path && *s == '/' && *(s - 1) == '/') {
            estrRemove(path, s - *path, 1);
        }
    }
}

char *backSlashes(char *path)
{
    char    *s;

    if(!path)
        return NULL;

    for(s=path;*s;s++)
    {
        if (*s == '/')
            *s = '\\';
    }
    return path;
}

char *fixDoubleSlashes(char *fn)
{
    char *c=fn;
    while (c=strstr(c, "//"))
        strcpy_unsafe(c, c+1);
    return fn;
}

char *unquote(char *in)
{
    int l = (int)strlen(in);
    if (in[0] == '"' && in[l-1] == '"') {
        in[l-1] = '\0';
        return in + 1;
    }
    return in;
}

void concatpath_s(const char *s1,const char *s2,char *full,size_t full_size)
{
    char    *s;

    if (s2[1] == ':' || s2[0] == '/' || s2[0] == '\\')
    {
        strcpy_s(full,full_size,s2);
        return;
    }
    strcpy_s(full,full_size,s1);
    s = &full[strlen(full)-1];
    if (*s != '/' && *s != '\\')
        strcat_s(full,full_size,"/");
    if (s2[0] == '/' || s2[0] == '\\')
        s2++;
    strcat_s(full,full_size,s2);
}

//r = reentrant. just like strtok, but you keep track of the pointer yourself (last)
char *strtok_r(char *target,const char *delim,char **last)
{
    int start;

    if ( target != 0 )
        *last = target;
    else
        target = *last;

    if ( !target || *target == '\0' )
        return 0;

    start = (int)strspn(target,delim);
    target = &target[start];
    if ( *target == '\0' )
    {
        /* failure to find 'start', remember and return */
        *last = target;
        return 0;
    }

    /* found beginning of token, now look for end */
    if ( *(*last = target + strcspn(target,delim)) != '\0')
    {
        *(*last)++ = '\0';
    }
    return target;
}

//r = reentrant. just like strtok, but you keep track of the pointer yourself (last)
char *strtok_delims_r(char *target,const char *startdelim, const char *enddelim,char **last)
{
    int start;

    if ( target != 0 )
        *last = target;
    else
        target = *last;

    if ( !target || *target == '\0' )
        return 0;

    start = (int)strspn(target,startdelim);
    target = &target[start];
    if ( *target == '\0' )
    {
        /* failure to find 'start', remember and return */
        *last = target;
        return 0;
    }

    /* found beginning of token, now look for end */
    if ( *(*last = target + strcspn(target,enddelim)) != '\0')
    {
        *(*last)++ = '\0';
    }
    return target;
}


/* Function strsep2()
 *    Similar to strsep(), except this function returns the deliminator
 *    through the given retrievedDelim buffer if possible.
 *
 */
char* strsep2(char** str, const char* delim, char* retrievedDelim){
    char* token;
    int spanLength;

    // Try to grab the token from the beginning of the string
    // being processed.
    token = *str;

    // If no token can be found from the string being processed,
    // return nothing.
    if('\0' == *token)
        return NULL;

    // Find out where the token ends.
    spanLength = (int)strcspn(*str, delim);

    // Advance the given string pointer to the end of the current token.
    *str = token + spanLength;

    // Extract the retrieved deliminator if requested.
    if(retrievedDelim)
        *retrievedDelim = **str;

    // If the end of the string has been reached, the string pointer is
    // pointing at the NULL terminating character.  Return the extracted
    // token.  The string pointer will be left pointing to the NULL
    // terminating character.  If the same string pointer is passed
    // back in a later call to this function, the function would return
    // NULL immediately.
    if('\0' == **str)
        return token;

    // Otherwise, the string pointer is pointing at a deliminator.  Turn
    // it into a NULL terminating character to mark the end of the token.
    **str = '\0';

    // Advance the string pointer to the next character in the string.
    // The string can continue to be processed if the same string pointer
    // is passed back later.
    (*str)++;

    return token;
}

/* Function strsep()
 *    A re-entrant replacement for strtok, similar to strtok_r.  Given a cursor into a string,
 *    and a list of deliminators, this function returns the next token in the string pointed
 *    to by the cursor.  The given cursor will be forwarded pass the found deliminator.
 *
 *    Note that the owner string pointer should not be passed to this function.  Doing so may
 *    result in memory leaks.  This function will alter the given string pointer (cursor).
 *
 *    Moved from entScript.c
 *
 *    Parameters:
 *        str - the cursor into the string to be used.
 *        delim - a list of deliminators to be used.
 *
 *    Returns:
 *        Valid char* - points to the next token in the string.
 *        NULL - no more tokens can be retrieved from the string.
 */
char* strsep(char** str, const char* delim){
    return strsep2(str, delim, NULL);
}

#if !defined(_XBOX) && !defined(_WIN64)
// stristr /////////////////////////////////////////////////////////
//
// performs a case-insensitive lookup of a string within another
// (see C run-time strstr)
//
// str1 : buffer
// str2 : string to search for in the buffer
//
// example char* s = stristr("Make my day","DAY");
//
// S.Rodriguez, Jan 11, 2004
//
char* strstri(char* str1, const char* str2)
{
    __asm
    {
        mov ah, 'A'
        mov dh, 'Z'

        mov esi, str1
        mov ecx, str2
        mov dl, [ecx]
        test dl,dl ; NULL?
        jz short str2empty_label

outerloop_label:
        mov ebx, esi ; save esi
        inc ebx
innerloop_label:
        mov al, [esi]
        inc esi
        test al,al
        je short str2notfound_label ; not found!

        cmp     dl,ah           ; 'A'
        jb      short skip1
        cmp     dl,dh           ; 'Z'
        ja      short skip1
        add     dl,'a' - 'A'    ; make lowercase the current character in str2
skip1:        

        cmp     al,ah           ; 'A'
        jb      short skip2
        cmp     al,dh           ; 'Z'
        ja      short skip2
        add     al,'a' - 'A'    ; make lowercase the current character in str1
skip2:        

        cmp al,dl
        je short onecharfound_label
        mov esi, ebx ; restore esi value, +1
        mov ecx, str2 ; restore ecx value as well
        mov dl, [ecx]
        jmp short outerloop_label ; search from start of str2 again
onecharfound_label:
        inc ecx
        mov dl,[ecx]
        test dl,dl
        jnz short innerloop_label
        jmp short str2found_label ; found!
str2empty_label:
        mov eax, esi // empty str2 ==> return str1
        jmp short ret_label
str2found_label:
        dec ebx
        mov eax, ebx // str2 found ==> return occurence within str1
        jmp short ret_label
str2notfound_label:
        xor eax, eax // str2 nt found ==> return NULL
        jmp short ret_label
ret_label:
    }

}
#else
char *strstri(char *s,const char *srch)
{
    int        len = (int)strlen(srch);

    for(;*s;s++)
    {
        if (strnicmp(s,srch,len)==0)
            return s;
    }
    return 0;
}
#endif

const char* strstriConst(const char* str1, const char* str2)
{
    return strstri((char*)str1, str2);
}

int tokenize_line_safe(char *buf,char *args[],int max_args,char **next_line_ptr)
{
    char    *s,*next_line;
    int        i,idx;

    if (!buf)
        return 0;
    s = buf;
    for(i=0;;)
    {
        if (i < max_args)
            args[i] = 0;
        if (*s == ' ' || *s == '\t')
            s++;
        else if (*s == 0)
        {
            next_line = 0;
            break;
        }
        else if (*s == '"')
        {
            if (i < max_args)
                args[i] = s+1;
            i++;
            s = strchr(s+1,'"');
            if (!s) // bad input string
            {
                if (next_line_ptr)
                    *next_line_ptr = 0;
                return 0;
            }
            *s++ = 0;
        }
        else
        {
            if (*s != '\r' && *s != '\n')
            {
                if (i < max_args)
                    args[i] = s;
                i++;
            }
            idx = (int)strcspn(s,"\n\r \t");
            s += idx;
            if (*s == ' ' || *s == '\t')
                *s++ = 0;
            else
            {
                if (*s == 0)
                    next_line = 0;
                else
                {
                    if (s[-1] == '\r')
                        s[-1] = 0;
                    if (s[0]=='\r' && s[1] == '\n') {
                        *s=0;
                        s++;
                    }
                    *s = 0;
                    next_line = s+1;
                }
                if (i < max_args)
                    args[i] = 0;
                break;
            }
        }
    }
    if (next_line_ptr)
        *next_line_ptr = next_line;
    if(i>max_args)
        return max_args;
    return i;
}

int tokenize_line_quoted_safe(char *buf,char *args[],int maxargs,char **next_line_ptr)
{
    int        i,count;
    size_t  len;
    char    *s;

    count = tokenize_line_safe(buf,args,maxargs,0);
    for(i=0;i<count;i++)
    {
        s = args[i];
        len = (int)strlen(s);
        if (!s[0] || strcspn(s," \t") != len)
        {
            s[len+1] = 0;
            *(--args[i]) = s[len] = '"';
        }
    }
    return count;
}

char *strtok_quoted_r(char *target,char *startdelim,char *enddelim, char **last)
{
    int        start;
    char    *base;

    base = target;
    if ( target != 0 )
    {
        *last = target;
    }
    else target = *last;

    if ( !target || *target == '\0' ) 
        return 0;

    start = (int)strspn(target,startdelim);
    target = &target[start];
    if (target[0] == '"')
    {
        enddelim = "\"";
        target++;
    }

    if ( *target == '\0' )
    {
        /* failure to find 'start', remember and return */
        *last = target;
        return 0;
    }

    /* found beginning of token, now look for end */
    if ( *(*last = target + strcspn(target,enddelim)) != '\0')
    {
        *(*last)++ = '\0';
    }
    return target;
}

char *strtok_quoted(char *target,char *startdelim,char *enddelim)
{
    static char *last;
    return strtok_quoted_r(target,startdelim,enddelim,&last);
}

void strarray_quoted(char **target, char *startdelim,char *enddelim, char ***eaResults, int leaveQuotes)
{
    char *position = *target;
    char *token = 0;
    int quote = 0;

    eaSetSize(eaResults, 0);
    while(position[0])
    {
        int strlength = 0;
        if(!quote)
        {
            while(position[0] &&!!strchr(startdelim, position[0]))
                position++;

            if(position[0] == '"')
            {
                if(!leaveQuotes)
                    position++;
                quote = 1;
            }
        }

        token = position;
        if(leaveQuotes)
            position++;
        while(position[0] && (quote || !strchr(enddelim, position[0])) && position[0] != '"')
        {
            position++;
            strlength++;
        }
        if(position[0] == '"')
        {
            quote = !quote;
            if(leaveQuotes && !quote)
                position++;
            else if(leaveQuotes)//otherwise, we have to prevent this quote from being deleted
            {
                estrInsert(target,position-*target, "\"", 1);
            }
        }
        if(position[0])
        {
            (position++)[0] = '\0';
        }
        if(strlength)
        {
            eaPush(eaResults, token);
        }
    }
}

int tokenize_line_quoted_delim(char *buf,char *args[],int max_args,char **next_line_ptr, char *startdelim, char *enddelim)
{
    char *next_line, *s;
    int i;

    // first, get the line separated:
    s = buf + strcspn(buf,"\n\r");
    if (*s == 0)
        next_line = 0;
    else
    {
        if (s[-1] == '\r')
            s[-1] = 0;
        if (s[0]=='\r' && s[1] == '\n')
        {
            *s=0;
            s++;
        }
        *s = 0;
        next_line = s + 1;
    }

    // now tokenize the line:
    s = strtok_quoted(buf, startdelim, enddelim);
    for (i = 0;i < max_args && s;i++)
    {
        args[i] = s;

        s = strtok_quoted(0, startdelim, enddelim);
    }

    if (next_line_ptr)
        *next_line_ptr = next_line;

    return i;
}

char *getContainerValueStatic(char *container, char* fieldname)
{
    static char *fieldBuf = NULL;
    U32 nField;
    int i;
    
    if(!fieldBuf)
    {
        estrCreateEx(&fieldBuf, 512);
    }

    // ----------
    // try once, if the buffer is too small then resize and try again

    estrSetLength(&fieldBuf, 0);
    for( i = 0; i < 2; ++i ) 
    {
        if((nField = getContainerValue(container, fieldname, fieldBuf, estrGetCapacity(&fieldBuf))) < estrGetCapacity(&fieldBuf))
        {
            break;
        }
        else
        {
            // grow the buffer by a little more than is needed
            estrReserveCapacity(&fieldBuf, (nField*3)/2);
        }
    }

    return fieldBuf;
}

int getContainerValue(char *container, char* fieldname, char* result, int size)
{
    // scan lines looking for fieldname at start of line
    int lenRes = 0;
    int len = (int)strlen(fieldname);

    if(result)
        result[0] = 0;

    while (1)
    {
        if (!container || !container[0]) break;
        while (strchr(" \t", container[0])) container++;
        if (strnicmp(container, fieldname, len)==0)
        {
            char* valuestart = strchr(container, ' ');
            if (valuestart - container == len)
            {
                valuestart++;
                if (valuestart[0] == '"') valuestart++;
                while (1)
                {
                    if (valuestart[0] == '"' || valuestart[0] == 0 || valuestart[0] == '\n')
                    {
                        if(result)
                            result[0] = 0;
                        return lenRes;                // BREAK IN FLOW
                    }
                    lenRes++;

                    if(lenRes < size && result)
                        *result++ = *valuestart;

                    valuestart++;
                }
            }
        }
        container = strchr(container, '\n');
        if (!container || !container[0]) break;
        container++;
    }
    return lenRes;
}

void *dynArrayAdd_dbg(void **basep,int struct_size,int *count,int *max_count,int num_structs MEM_DBG_PARMS)
{
    char    *base = *basep;

    PERFINFO_AUTO_START("dynArrayAdd_dbg", 1);
    
    if (*count >= *max_count - num_structs)
    {
        if (!*max_count)
            *max_count = num_structs;
        (*max_count) <<= 1;
        if (num_structs > 1)
            (*max_count) += num_structs;
        base = srealloc(base,struct_size * *max_count);
        memset(base + struct_size * *count,0,(*max_count - *count) * struct_size); //CD: see comment below
        *basep = base;
    }

    // CD: Putting this here guarantees that memory will be zeroed.  Otherwise, if the count is reset the memory will not get zeroed.
    memset(base + struct_size * (*count), 0, struct_size * num_structs);

    (*count)+=num_structs;
    
    PERFINFO_AUTO_STOP();
    
    return base + struct_size * (*count - num_structs);
}

// same as dynArrayAdd, but for arrays of struct pointers instead of arrays of structs
void *dynArrayAddp_dbg(void ***basep,int *count,int *max_count,void *ptr MEM_DBG_PARMS)
{
    void    **mem;

    mem = dynArrayAdd_dbg((void *)basep,sizeof(void *),count,max_count,1 MEM_DBG_PARMS_CALL);
    *mem = ptr;
    return mem;
}

void *dynArrayFit_dbg(void **basep,int struct_size,int *max_count,int idx_to_fit MEM_DBG_PARMS)
{
    int        last_max;
    char    *base = *basep;

    if (idx_to_fit >= *max_count)
    {
        PERFINFO_AUTO_START("dynArrayFit_dbg", 1);

        last_max = *max_count;

        // MAK - fix to memory bug (was trying to realloc to zero size when asked to fit in element 0)
        *max_count = idx_to_fit ? idx_to_fit * 2 : 1;

        base = srealloc(base,struct_size * (*max_count));
        memset(base + struct_size * last_max, 0, ((*max_count) - last_max) * struct_size);
        *basep = base;

        PERFINFO_AUTO_STOP();
    }
    
    return base + struct_size * idx_to_fit;
}

const char *getFileName(const char *fname)
{
    const char *s;
    s = strrchr(fname,'/');
    if (!s)
        s = strrchr(fname,'\\');
    if (!s++)
        s = fname;
    return s;
}

const char *getFileNameConst(const char *fname)
{
    const char    *s;

    s = strrchr(fname,'/');
    if (!s)
        s = strrchr(fname,'\\');
    if (!s++)
        s = fname;
    return s;
}

char *getFileNameNoExt_s(char *dest, size_t dest_size, const char *filename)
{
    char *s;
    strcpy_s(dest, dest_size, getFileNameConst(filename));
    s = strrchr(dest, '.');
    if (s)
        *s = '\0';
    return dest;
}

/* getDirectoryName()
 *    Given a path to a file, this function returns the directory name
 *    where the file exists.
 *
 *    Note that this function will alter the given string to produce the
 *    directory name.
 */
char *getDirectoryName(char *fullPath)
{
    char    *cursor;

    if (!fullPath)
        return 0;
    cursor = strrchr(fullPath,'/');
    if (!cursor)
        cursor = strrchr(fullPath,'\\');
    if (cursor)
        *cursor = '\0';
    if(fullPath[0] && fullPath[1] == ':' && !fullPath[2]){
        fullPath[2] = '\\';
        fullPath[3] = '\0';
    }
    return fullPath;
}

// Takes a path such as C:\game\data and turns it into game\data (no trailing or leading slashes)
char *getDirString(const char *path)
{
    static char ret[MAX_PATH];
    strcpy(ret, path);
    backSlashes(ret);
    while (strEndsWith(ret, "\\")) {
        ret[strlen(ret)-1]=0;
    }
    if (ret[1]==':') {
        strcpy(ret, ret+2);
    }
    while (ret[0]=='\\') {
        strcpy(ret, ret+1);
    }
    return ret;
}

// adds this prefix to the filename part of the path
char *addFilePrefix(const char* path, const char* prefix)
{
    static char ret[MAX_PATH+20];

    strcpy(ret, path);
    getDirectoryName(ret);
    strcat(ret, "/");
    strcat(ret, prefix);
    strcat(ret, getFileName(path));
    return ret;
}

//-----------------------------------------------------------------------
//
//
//
void initStuffBuff(    StuffBuff *sb, int size )
{
    memset( sb, 0, sizeof( StuffBuff ) );
    sb->buff = memAlloc( size );

    if( !sb->buff )
        size = 0;

    memset( sb->buff, 0, size );
    sb->size = size;
}

void resizeStuffBuff( StuffBuff *sb, int size )
{
    if (size < sb->idx - 1)
    {
        //don't let it truncate
        return;
    }

    sb->buff = realloc(sb->buff,size);

    if (size > sb->size)
    {
        memset(sb->buff + sb->size,0,size - sb->size);
    }

    sb->size = size;
}



//
//
void clearStuffBuff(StuffBuff *sb)
{
    sb->idx = 0;

    if(sb->buff)
        sb->buff[0] = '\0';
}

//
//
void addStringToStuffBuff( StuffBuff *sb, const char *s, ... )
{
    va_list        va;
    int            size,bytes_left;

    if(!devassertmsg(sb->buff, "You're probably using a StuffBuff that hasn't been initialized.\n"))
    {
        initStuffBuff(sb,2);
    }
    for(;;)
    {
        va_start( va, s );
        bytes_left = sb->size - sb->idx - 1;
        size = quick_vsnprintf(sb->buff + sb->idx,bytes_left+1,bytes_left,s,va);
        va_end( va );

        if (size >= 0) {
            // Wrote successfully
            sb->idx += size;
            if (size == bytes_left) {
                // It wasn't null terminated, but we saved a byte in advance
                sb->buff[sb->idx] = '\0';
            }
            break;
        }

        sb->size = sb->size ? sb->size*2 : 2;
        sb->buff = realloc(sb->buff,sb->size);
    }
}

void addIntToStuffBuff( StuffBuff *sb, int i)
{
    char buf[64];
    itoa(i, buf, 10);
    addSingleStringToStuffBuff(sb, buf);
}

//
//
void addSingleStringToStuffBuff( StuffBuff *sb, const char *s)
{
    int            bytes_left;
    char        *cursor;
    int            loop=1;

    assert(s);
    
    bytes_left = sb->size - sb->idx - 1;
    cursor = sb->buff + sb->idx;
    while (loop) {
        if (bytes_left == 0) {
            sb->size *= 2;
            sb->buff = realloc(sb->buff,sb->size);
            cursor = sb->buff + sb->idx;
            bytes_left = sb->size - sb->idx - 1;
        }
        if (*s) { // not the end of the string
            sb->idx++;
            bytes_left--;
        } else {
            loop=0;
        }
        *cursor++ = *s++;
    }
}

void addEscapedDataToStuffBuff(StuffBuff *sb, void *s, int len)
{
    int bytes;
    
    bytes = sb->idx + escapeDataMaxChars(len) + 1; // existing bytes, new bytes, terminator
    if(sb->size < bytes)
    {
        while(sb->size < bytes)
            sb->size *= 2;
        sb->buff = realloc(sb->buff, sb->size);
    }

    bytes = sb->size - sb->idx - 1;
    escapeDataToBuffer(sb->buff + sb->idx, s, len, &bytes);
    sb->idx += bytes;
    sb->buff[sb->idx] = '\0';
}

void addBinaryDataToStuffBuff( StuffBuff *sb, const char *data, int len)
{
    int            bytes_left;
    char        *cursor;

    bytes_left = sb->size - sb->idx - 1;
    cursor = sb->buff + sb->idx;
    while (len) {
        if (bytes_left <= 0) {
            sb->size *= 2;
            if (sb->size < sb->idx + len) {
                sb->size = sb->idx + len + 1;
            }
            sb->buff = realloc(sb->buff,sb->size);
            cursor = sb->buff + sb->idx;
            bytes_left = sb->size - sb->idx - 1;
        }
        sb->idx++;
        bytes_left--;
        *cursor++ = *data++;
        len--;
    }
}


//
//
void freeStuffBuff( StuffBuff * sb )
{
    memFree( sb->buff );
    ZeroStruct(sb);
}

int firstBits( int val )
{
    int    i = 0;

    while( val && !( val & 1 ) )
    {
        val = val >> 1;
        i++;
    }

    return i;
}

double simpleatof(const char *s) {
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
                    } else if (*c=='#' || *c=='/') { // assume it's a comment, be nice, let it slide
                        return ret*sign;
                    }
                    printf("Bad string passed to simpleatof(): %s\n", s);
                    return atof(s);
                }
            }
            return ret*sign;
        }
        ret*=10;
        if (*c>='0' && *c<='9') {
            ret+=*c-'0';
            c++;
        } else if (*c=='#' || *c=='/') { // assume it's a comment, be nice, let it slide
            return ret*sign;
        } else {
            // bad character
            printf("Bad string passed to simpleatof(): %s\n", s);
            return atof(s);
        }
    }
    return ret*sign;
}

/* Function encodeString()
*    Encodes a string, removing all carriage returns and replacing them with
*   "\n", and replacing all backslashes with "\\"
*  Warning: not thread safe at all.
*  Returns a pointer to some static memory where the temporary result is stored
*
*  Rules for using escape/unescapeString: (applies to accountname, playername, and description)
*   Anytime a user-enterable string is sent to the db server, it should be sent as:
*     pktSendString(pkt, escapeString(s));
*   Anytime a string is read from the db server, it should be read as:
*     strcpy(dest, unescapeString(pktGetString(pkt))
*/

void escapeDataToBuffer(char *dst, const void *src, int srclen, int *dstlen)
{
    int i;
    char *out;

    assertmsg(dstlen, "must check dstlen for termination etc.");
    assertmsg(*dstlen >= escapeDataMaxChars(srclen), "buffer not big enough");

    for(i = 0, out = dst; i < srclen; i++)
    {
        switch(((char*)src)[i])
        {
            xcase '\n':    *out++='\\'; *out++='n';
            xcase '\r':    *out++='\\'; *out++='r';
            xcase '\t':    *out++='\\'; *out++='t';
            xcase '\"':    *out++='\\'; *out++='q';
            xcase '\'':    *out++='\\'; *out++='s';
            xcase '\\':    *out++='\\'; *out++='\\';
            xcase '%':    *out++='\\'; *out++='p';
            xcase '\0':    *out++='\\'; *out++='0';
            xcase '$':    *out++='\\'; *out++='d';
            xdefault:    *out++=((char*)src)[i];
        }
    }
    // *out = '\0'; // let the caller terminate, realloc, or whatever
    *dstlen = out - dst;
}

const char *escapeData(const char *instring,int len)
{
    static char *ret=NULL;
    static int retlen=0;

    int deslen;
    
    if(!instring)
        return NULL;

    deslen = escapeDataMaxChars(len)+1;
    if (retlen < deslen) // We need a bigger buffer to return the data
    {
        if (ret)
            free(ret);
        if (deslen<256)
            deslen = 256; // Allocate at least 256 bytes the first time
        ret = malloc(deslen);
        retlen = deslen;
    }

    deslen = retlen;
    escapeDataToBuffer(ret, instring, len, &deslen);
    ret[deslen] = '\0';
    return ret;
}

const char *escapeString(const char *instring)
{
    if(!instring)
        return NULL;
    return escapeData(instring,(int)strlen(instring));
}






// Same as escapeData, but only escapes CR and LF
const char *escapeLogData(const char *instring,int len)
{
    static char *ret=NULL;
    static int retlen=0;
    const char *c;
    char *out;
    int i,deslen;
    
    if(!instring)
        return NULL;
    deslen = len*2+1;
    if (retlen < deslen) // We need a bigger buffer to return the data
    {
        if (ret)
            free(ret);
        if (deslen<256)
            deslen = 256; // Allocate at least 256 bytes the first time
        ret = malloc(deslen);
        retlen = deslen;
    }
    for (i=0, c=instring, out=ret; i<len ; c++, i++)
    {
        switch (*c)
        {
            case '\n':
                *out++='\\';
                *out++='n';
                break;
            case '\r':
                *out++='\\';
                *out++='r';
                break;
            default:
                *out++=*c;
        }
    }
    *out=0;
    return ret;
}

const char *escapeLogString(const char *instring)
{
    if(!instring)
        return NULL;
    return escapeLogData(instring,(int)strlen(instring));
}

int atoi_ignore_nonnumeric(char *pch)
{
    char scratch[32];
    char *cur, *end;
    char *pchStart = pch;
    cur = scratch;
    end = scratch + ARRAY_SIZE(scratch)-1;

    // Get all the digits from the string, get rid of
    //   everything else.
    // Try to keep the cursor stable.
    while(*pch && cur < end)
    {
        if(*pch>='0' && *pch<='9')
        {
            // grab leading '-', if first number
            if(cur == scratch && pch > pchStart && pch[-1] == '-')
                *cur++ = '-';    
            *cur++ = *pch;
        }
        pch++;
    }
    *cur = '\0';

    return atoi(scratch);
}

char *itoa_with_grouping(int i, char *buf, int radix, int groupsize, int decimalplaces, char sep, char dp)
{
    char ach[255];
    int iLen;
    int iStart = 0;
    bool whole = false;
    char *pch = buf;

    itoa(i, ach, radix);

    iLen = (int)strlen(ach);

    // Deal with a leading sign character.
    if (ach[iStart]<'0' || ach[iStart]>'9')
    {
        *pch++ = ach[iStart];
        iStart++;
    }

    // Always take the first non-fractional digit to prevent accidental
    // prepending of a separator (eg. no ",100,000,000")
    if((iLen-iStart)>decimalplaces)
    {
        *pch++ = ach[iStart];
        iStart++;
        whole = true;
    }

    for(i=iLen-iStart; i>decimalplaces; i--)
    {
        if((i-decimalplaces)%groupsize==0)
        {
            *pch++ = sep;
        }
        *pch++ = ach[iLen-i];
        whole = true;
    }

    if(decimalplaces)
    {
        // There was no whole part (the number is just a fraction) so write
        // a zero in lieu of one.
        if(!whole)
        {
            *pch++ = '0';
        }

        *pch++ = dp;
        for(i=decimalplaces; i>0; i--)
        {
            // We might have been asked for more decimal places than
            // are available. Pad the fractional part with leading
            // zeroes if so.
            if((iLen-iStart)<i)
            {
                *pch++ = '0';
            }
            else
            {
                *pch++ = ach[iLen-i];
            }
        }
    }
    *pch++ = '\0';

    return buf;
}

char *itoa_with_commas(int i, char *buf)
{
    return itoa_with_grouping(i, buf, 10, 3, 0, ',', '\0');
}

char *itoa_with_commas_static(int i)
{
    static char buf[128];
    return itoa_with_commas(i,buf);
}

int pprintfv(int predicate, const char *format, va_list argptr)
{
    if(!predicate)
        return 0;
    return vprintf(format,argptr);
}

int pprintf(int predicate, const char *format, ...)
{
    int result;
    va_list argptr;

    if(!predicate)
        return 0;

    va_start(argptr, format);
    result = vprintf(format, argptr);
    va_end(argptr);
    
    return result;
}

void reverse_bytes(U8 *s,int len)
{
    char *e;
#define S(A,B) { char t = A;                          \
    A = B;                                      \
    B = t; }
    
    
    e = s + len;
    while(--e > s)
    {
        S(*s,*e);
        s++;
    }
}
