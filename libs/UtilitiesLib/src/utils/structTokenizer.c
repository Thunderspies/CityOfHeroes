/*\
 *
 *    structTokenizer.c - Copyright 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Provides a single-pass, destructive tokenizer for textparser.  The tokenizer
 *  is independent and can be used independently, although it is heavily
 *  optimized for speed over convenience.
 *
 */

#include "utilitieslib/utils/structTokenizer.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/structInternals.h"
#include "utilitieslib/utils/structDefines.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/strings_opt.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/foldercache.h"

// NOTE: in future, if more flexibility is required, we can
// separate INCLUDE handling to a layer above tokenizer.
// That would simplify the Tokenizer code.

typedef struct TokenizerContext
{
    FILE* handle;
    long curoffset;
    struct TokenizerContext* previous;
    int curline;
    int nextline;
    char filename[MAX_PATH];
    const char* string;
    int stringlen;

    unsigned int contextisstring:1;        // this context is a placeholder because we're processing a string
} TokenizerContext;

typedef struct Tokenizer
{
    TokenizerContext* context;
    DefineContext* globaldefines;
    DefineContext* localdefines;
    StaticDefine* staticdefines;        // simple defines with no scoping
    long offsetinbuffer;
    long lengthofbuffer;
    long pastescape;                    // offset past the escaped string
    long exoffset;                        // offset for Ex processing

    unsigned int commanext:1;
    unsigned int eolnext:1;
    unsigned int quotenext:1;
    unsigned int commaaftertoken:1;
    unsigned int eolaftertoken:1;
    unsigned int quoteaftertoken:1;
    unsigned int instring:1;            // we just returned a string from Peek (don't tokenize on further calls)
    unsigned int inescape:1;

    // these two entries should be last
    char buffer[TOKEN_BUFFER_LENGTH];
    char escape[TOKEN_BUFFER_LENGTH];    // temp buffer if we need to read an escaped string
} Tokenizer;

// static strings as placeholders for newlines and commas
char g_comma[] = ",";
char g_eol[] = "\n";
char g_delims[] = " ,\n\t\r";


static void TokenizerInit(Tokenizer* tok)
{
    tok->context = NULL;
    tok->globaldefines = NULL;
    tok->localdefines = NULL;
    tok->staticdefines = NULL;
    tok->offsetinbuffer = 0;
    tok->lengthofbuffer = 0;
    tok->pastescape = 0;
    tok->exoffset = 0;

    tok->commanext = 0;
    tok->eolnext = 0;
    tok->quotenext = 0;
    tok->commaaftertoken = 0;
    tok->eolaftertoken = 0;
    tok->quoteaftertoken = 0;
    tok->instring = 0;
    tok->inescape = 0;
}

// open a file and sets up a context, returns NULL on failure
static TokenizerContext* OpenTokenFile(const char* filename)
{
    const char UnicodeSig[2] = { 0xff, 0xfe };
    const char UnicodeBESig[2] = { 0xfe, 0xff };
    const char UTF8Sig[3] = { 0xef, 0xbb, 0xbf };
    char opensig[3];

    // try to open file
    TokenizerContext* ret = (TokenizerContext*)memAlloc(sizeof(TokenizerContext));
    ret->handle = fileOpen(filename, "rb");
    ret->curoffset = 0;
    ret->previous = NULL;
    ret->curline = 1;
    ret->nextline = 1;
    strncpy_s(SAFESTR(ret->filename), filename, _TRUNCATE);
    ret->filename[MAX_PATH-1] = 0;
    ret->string = NULL;
    ret->stringlen = -1;
    ret->contextisstring = 0;
    if (!ret->handle)
    {
        writeConsole(OUTPUT_VERBOSE, "Tokenizer: couldn't open %s", filename);
        memFree(ret);
        return NULL;
    }

    // test for a UTF-16 or UTF-32 file
    fread(opensig, sizeof(char), 3, ret->handle);
    if (!strncmp(opensig, UnicodeSig, 2) ||
        !strncmp(opensig, UnicodeBESig, 2))
    {
        ErrorFilenamef(filename, "Tokenizer: %s is a unicode file and cannot be opened\n", filename);
        fileClose(ret->handle);
        memFree(ret);
        return NULL;
    }
    // we're fine with UTF-8
    if (!strncmp(opensig, UTF8Sig, 3))
    {
        verbose_printf("Tokenizer: skipping UTF-8 header on %s\n", filename);
        ret->curoffset = 3;
    }

    // record every file tokenized
    if (g_parselist) FileListInsert(&g_parselist, filename, 0);

    return ret;
}

char* TokenizerGetFileName(TokenizerHandle tokenizer)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok || !tok->context) return NULL;
    return tok->context->filename;
}

int TokenizerGetCurLine(TokenizerHandle tokenizer)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok || !tok->context) return 0;
    return tok->context->curline;
}

char* TokenizerGetFileAndLine(TokenizerHandle tokenizer)
{
    static char buf[MAX_PATH+12];

    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok || !tok->context) return NULL;
    sprintf_s(SAFESTR(buf), "%s, line %i", tok->context->filename, tok->context->curline);
    return buf;
}

// all eol tokens will be returned this way
int IsEol(const char* token)
{
    // MAK - need to use actual pointer to distinguish the g_eol marker from a string containing just eol
    return token == g_eol;
    // return token[0] == '\n' && token[1] == 0;
}

// closes file and frees context, doesn't do anything to list
static void CloseTokenFile(TokenizerContext* context)
{
    fileClose(context->handle);
    memFree(context);
}

// reload buffer with offset passed, and set buffer vars correctly
// lengthofbuffer set to zero on error or eof
static void LoadBuffer(Tokenizer* tok, long offset)
{
    tok->context->curoffset = offset;    // record where our buffer is

    if(tok->context->contextisstring)
    {
        if (tok->context->stringlen == -1)
            tok->context->stringlen = (int)strlen(tok->context->string);
        if(offset < tok->context->stringlen)
        {
            tok->lengthofbuffer = min(tok->context->stringlen-offset, TOKEN_BUFFER_LENGTH-1);
            strncpy_s(tok->buffer, TOKEN_BUFFER_LENGTH, tok->context->string + offset, tok->lengthofbuffer);
        }
        else
        {
            tok->lengthofbuffer = 0;
            return;
        }
    }
    else
    {
        fseek(tok->context->handle, offset, SEEK_SET);
        tok->lengthofbuffer = (long)fread(tok->buffer, sizeof(char), TOKEN_BUFFER_LENGTH, tok->context->handle);
    }

    if (tok->lengthofbuffer < TOKEN_BUFFER_LENGTH) tok->buffer[tok->lengthofbuffer] = 0;
    tok->offsetinbuffer = 0;
}

TokenizerHandle TokenizerCreateString(const char* string, int length)
{
    Tokenizer* tok;

    TokenizerContext* ret;

    if(!string || !string[0])
    {
        Errorf("Empty string passed into TokenizerCreateString()");
        return NULL;
    }

    tok = (Tokenizer*)calloc(1, sizeof(Tokenizer));
    TokenizerInit(tok);

    ret = (TokenizerContext*)memAlloc(sizeof(TokenizerContext));
    ret->handle = NULL;
    ret->curoffset = 0;
    ret->previous = NULL;
    ret->curline = 1;
    ret->nextline = 1;
    ret->filename[0] = 0;
    ret->string = string;
    ret->stringlen = length;
    ret->contextisstring = 1;

    tok->context = ret;

    LoadBuffer(tok, tok->context->curoffset);
/*
    tok->lengthofbuffer = min(TOKEN_BUFFER_LENGTH, strlen(string));
    strncpy(tok->buffer, string, tok->lengthofbuffer);
    if (tok->lengthofbuffer < TOKEN_BUFFER_LENGTH) tok->buffer[tok->lengthofbuffer] = 0;
*/

    return (TokenizerHandle)tok;
}

// external TokenizerCreate function, returns a handle or NULL on failure
TokenizerHandle TokenizerCreateEx(const char* filename, int ignore_empty)
{
    Tokenizer* tok = (Tokenizer*)calloc(1, sizeof(Tokenizer));
    TokenizerInit(tok);

    tok->context = OpenTokenFile(filename);
    if (!tok->context)
    {
        free(tok);
        tok = NULL;
    }
    else
    {
        LoadBuffer(tok, tok->context->curoffset);
        if (tok->lengthofbuffer == 0)
        {
            CloseTokenFile(tok->context);
            free(tok);
            tok = NULL;
            if (!ignore_empty)
                writeConsole(OUTPUT_VERBOSE, "Tokenizer: %s is empty", filename);
        }
    }
    return (TokenizerHandle)tok;
}

// external TokenizerDestroy function, closes any open files and frees mem
void TokenizerDestroy(TokenizerHandle tokenizer)
{
    TokenizerContext* topcontext;
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (tok == NULL) return;

    // destroy each context and close files
    for (topcontext = tok->context; topcontext != NULL; topcontext = tok->context)
    {
        tok->context = topcontext->previous;
        if(!topcontext->contextisstring)
            CloseTokenFile(topcontext);
        else
            memFree(topcontext);
    }
    free(tok);
}

// Escapes the given string and put it in target
int TokenizerEscape(char * target, const char* source)
{
    char *start = target;

    *target++ = '<';
    *target++ = '&';
    while (*source)
    {
        if (*source == '\\')
        {
            *target++ = '\\';
            *target++ = '\\';
        }
        else if (*source == '>')
        {
            *target++ = '\\';
            *target++ = '>';
        }
        else if (*source == '&')
        {
            *target++ = '\\';
            *target++ = '&';
        }
        else if (*source == '\n')
        {
            *target++ = '\\';
            *target++ = 'n';
        }
        else if (*source == '\r')
        {
            *target++ = '\\';
            *target++ = 'r';
        }
        else
            *target++ = *source;
        source++;
    }
    *target++ = '&';
    *target++ = '>';
    *target = 0;

    return target - start; //returns new size
}


// Unescapes the string, ignoring initial and ending tags if necessary
int TokenizerUnescape(char* target, const char* source)
{
    char *start = target;
    while (*source)
    {
        if (*source == '<' && *(source+1) == '&')
        {
            source++; source++;
            continue;
        }
        else if (*source == '&' && *(source+1) == '>')
        {
            source++; source++;
            break;
        }        
        else if (*source == '\\') 
        {
            source++;
            if (*source == 'n')
            {
                *target++ = '\n';
                source++;
            }
            else if (*source == 'r')
            {
                *target++ = '\r';
                source++;
            }
            else
                *target++ = *source++;
        }
        else
            *target++ = *source++;
    }
    *target = 0;
    return target - start; //returns new size
}

// external TokenizerPeek function, advances to next token and returns it,
// returns NULL at end of file
const char* TokenizerPeek(TokenizerHandle tokenizer, int ignorelinebreak, int ignorecomma)
{
    long i;
    Tokenizer* tok = (Tokenizer*)tokenizer;
    char filename[MAX_PATH];
    TokenizerContext* savecontext;
    char* ret;
    const char* define;

    if (tok == NULL) return NULL;

    // short circuit if we're currently in a string
    if (tok->instring)
    {
        if (!ignorecomma && tok->commanext) return g_comma;
        if (!ignorelinebreak && tok->eolnext) return g_eol;
        return tok->buffer + tok->offsetinbuffer;
    }
    if (tok->inescape)
    {
        if (!ignorecomma && tok->commanext) return g_comma;
        if (!ignorelinebreak && tok->eolnext) return g_eol;
        return tok->escape;
    }

    // main loop to look for tokens, we go back to here after comments, etc.
    while (1)
    {
        // advance buffer if needed
        if (tok->offsetinbuffer >= tok->lengthofbuffer ||
            tok->offsetinbuffer > TOKEN_BUFFER_LENGTH - MAX_TOKEN_LENGTH - 2)
        {
            LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
        }

        // check and see if we've run out of space
        while (tok->lengthofbuffer == 0)
        {
            // try and pop context
            if (tok->context->previous)
            {
                savecontext = tok->context->previous;
                CloseTokenFile(tok->context);
                tok->context = savecontext;
                LoadBuffer(tok, tok->context->curoffset);
                tok->eolnext = 1;    // make sure to set eol between contexts
            }
            else break;
        }
        if (tok->lengthofbuffer == 0) return NULL; // if no more contexts, return null

        // advance past white space
        while (strchr(g_delims, tok->buffer[tok->offsetinbuffer]))
        {
            if (tok->buffer[tok->offsetinbuffer] == ',') tok->commanext = 1;
            if (tok->buffer[tok->offsetinbuffer] == '\n') { tok->eolnext = 1; tok->context->nextline++; }
            tok->offsetinbuffer++;

            // advance buffer if we get close to end
            if (tok->offsetinbuffer >= tok->lengthofbuffer ||
                tok->offsetinbuffer > TOKEN_BUFFER_LENGTH - MAX_TOKEN_LENGTH - 2)
            {
                LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
                if (tok->lengthofbuffer == 0)
                {
                    break; // ran out of text, need to continue and load new context
                }
            }
        }
        if (tok->lengthofbuffer == 0) continue; // try and load new context

        // take care of string
        if (tok->buffer[tok->offsetinbuffer] == '"' || tok->quotenext)
        {
            // advance buffer if we are too close to end for a string
            if (tok->offsetinbuffer > TOKEN_BUFFER_LENGTH - MAX_STRING_LENGTH - 2)
            {
                LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
            }

            // advance real quick looking for a terminator or eol
            i = tok->offsetinbuffer;
            if (!tok->quotenext) i++; // past open string marker
            while (i < tok->lengthofbuffer && tok->buffer[i] != '"' && tok->buffer[i] != '\n') i++;
            if (i >= tok->lengthofbuffer || tok->buffer[i] == '\n')
            {
                ErrorFilenamef(TokenizerGetFileName(tok), "Tokenizer: no end of string found in %s\n", TokenizerGetFileAndLine(tok));
                g_parseerr = 1;
                tok->offsetinbuffer = i;
                LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
                continue; // start over again and try to recover
            }

            // now we found the end string marker
            tok->buffer[i] = '\0';
            if (!tok->quotenext) tok->offsetinbuffer++; // get past open string marker
            tok->instring = 1;
            tok->quotenext = 0;
            if (!ignorecomma && tok->commanext) return g_comma;
            if (!ignorelinebreak && tok->eolnext) return g_eol;
            return tok->buffer + tok->offsetinbuffer;    // return complete string, no defines processing
        }

        // take care of multi-line strings & escaped strings
        if ((tok->buffer[tok->offsetinbuffer] == '<' && tok->buffer[tok->offsetinbuffer+1] == '<') ||
            (tok->buffer[tok->offsetinbuffer] == '<' && tok->buffer[tok->offsetinbuffer+1] == '&'))
        {
            int escapedstring = tok->buffer[tok->offsetinbuffer+1] == '&';

            // advance buffer if we are too close to end for a string
            if (tok->offsetinbuffer > TOKEN_BUFFER_LENGTH - MAX_STRING_LENGTH - 4)
            {
                LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
            }

            // advance real quick looking for the string terminator
            i = tok->offsetinbuffer+2;
            while (i < tok->lengthofbuffer-1)
            {
                if (!escapedstring && tok->buffer[i] == '>' && tok->buffer[i+1] == '>') break;
                if (escapedstring && tok->buffer[i] == '&' && tok->buffer[i+1] == '>') break;
                if (tok->buffer[i] == '\n') { tok->context->nextline++; }
                i++;
            }
            if (i >= tok->lengthofbuffer-1)
            {
                ErrorFilenamef(TokenizerGetFileName(tok), "Tokenizer: no end of multi-line string started on %s, line %i\n", TokenizerGetFileName(tok), tok->context->curline);
                g_parseerr = 1;
                tok->offsetinbuffer = i+1;
                LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
                continue; // start over again and try to recover (after eating to EOF)
            }

            // now we found the end string marker
            tok->buffer[i] = '\0';
            tok->buffer[i+1] = '\0';
            tok->offsetinbuffer += 2; // get past open string marker

            if (escapedstring)
            {
                tok->pastescape = i+2;
                tok->inescape = 1;
                TokenizerUnescape(tok->escape, tok->buffer + tok->offsetinbuffer);
                if (!ignorecomma && tok->commanext) return g_comma;
                if (!ignorelinebreak && tok->eolnext) return g_eol;
                return tok->escape;
            }
            else
            {
                // skip past initial EOL if found
                if (tok->buffer[tok->offsetinbuffer] == '\n') tok->offsetinbuffer++;
                else if (tok->buffer[tok->offsetinbuffer] == '\r' && tok->buffer[tok->offsetinbuffer + 1] == '\n') tok->offsetinbuffer += 2;
                tok->instring = 1;
                if (!ignorecomma && tok->commanext) return g_comma;
                if (!ignorelinebreak && tok->eolnext) return g_eol;
                return tok->buffer + tok->offsetinbuffer;    // return complete string, no defines processing
            }
        }

        // take care of comment
        else if (tok->buffer[tok->offsetinbuffer] == '#' ||
            (tok->buffer[tok->offsetinbuffer] == '/' && tok->buffer[tok->offsetinbuffer+1] == '/'))
        {
            // advance buffer if we are too close to end to get complete comment in buffer
            if (tok->offsetinbuffer > TOKEN_BUFFER_LENGTH - MAX_STRING_LENGTH - 2)
            {
                LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
            }

            // advance real quick looking for an eol
            i = tok->offsetinbuffer+2;
            while (i < tok->lengthofbuffer && tok->buffer[i] != '\n') i++;
            i++; // past eol
            if (i >= tok->lengthofbuffer) LoadBuffer(tok, tok->context->curoffset + i);
            else tok->offsetinbuffer = i;

            // now we're past eol, just start over
            tok->eolnext = 1;
            tok->context->nextline++;
        }

        // take care of INCLUDE
        else if ((tok->eolnext || (tok->offsetinbuffer == 0 && tok->context->curline == 1))  // at beginning of line
            && (_strnicmp(tok->buffer+tok->offsetinbuffer, "include ", 8) == 0)) // found "include "
        {
            // advance to the filename
            tok->offsetinbuffer += 8;
            while (tok->offsetinbuffer < tok->lengthofbuffer && strchr(" \t", tok->buffer[tok->offsetinbuffer]))
                tok->offsetinbuffer++;
            // ok to fall through end of buffer for a minute

            // look for end of filename
            i = tok->offsetinbuffer + 1;
            while (i < tok->lengthofbuffer && !strchr(g_delims, tok->buffer[i])) i++;
            if (i >= TOKEN_BUFFER_LENGTH)
            {
                // actually hit end of buffer.  this is an error, as we should have
                // had plenty of room for filename (MAX_TOKEN_LENGTH > MAX_PATH)
                ErrorFilenamef(TokenizerGetFileName(tok), "Tokenizer: error reading include file name in %s, line %i\n",
                    TokenizerGetFileName(tok), tok->context->nextline);
                g_parseerr = 1;
                LoadBuffer(tok, tok->context->curoffset + tok->offsetinbuffer);
                continue; // start over

            }
            // otherwise, we hit the end of the file
            if (i >= tok->lengthofbuffer)
            {
                tok->buffer[i] = 0;    // chop the end of the buffer
            }
            if (tok->buffer[i] == '\n') tok->context->nextline++;
            tok->buffer[i] = 0; // chop filename
            strcpy(filename, tok->buffer + tok->offsetinbuffer); // copy it out
            tok->offsetinbuffer = i+1; // reset stream

            // open new file
            tok->context->curoffset += tok->offsetinbuffer;
            forwardSlashes(filename); // required for prefix match
            if (FolderCacheMatchesIgnorePrefixAnywhere(filename))  // ignore this include file
            {
                // still need to note this file in my parse list, weird but otherwise I
                // won't notice its absence and require a reparse later
                if (g_parselist) FileListInsert(&g_parselist, filename, 0);
                continue; // fall through and continue
            }
            savecontext = tok->context;
            tok->context = OpenTokenFile(filename);
            if (!tok->context)
            {
                g_parseerr = 1;
                tok->context = savecontext;
                ErrorFilenamef(TokenizerGetFileName(tok),
                    "Tokenizer: error opening include file %s referenced in %s, line %i\n",
                    filename, TokenizerGetFileName(tok), tok->context->nextline);
                continue; // fall through and continue
            }
            else
            {
                // push context
                tok->context->previous = savecontext;
                LoadBuffer(tok, tok->context->curoffset);
                tok->eolnext = 1;
                continue; // fall through and continue
            }
        }

        // otherwise, chop off a regular token
        else
        {
            // look for end of token
            i = tok->offsetinbuffer;
            while (i < tok->lengthofbuffer && !strchr(g_delims, tok->buffer[i]) && tok->buffer[i] != '"') i++;
            if (i >= tok->lengthofbuffer)
            {
                if (i == tok->lengthofbuffer && tok->lengthofbuffer < TOKEN_BUFFER_LENGTH)
                {
                    // just hit end of file, it's ok..
                }
                else
                {
                    ErrorFilenamef(TokenizerGetFileName(tok),
                        "Tokenizer: token too long in %s\n", TokenizerGetFileAndLine(tok));
                    g_parseerr = 1;
                    LoadBuffer(tok, tok->context->curoffset+i);
                    continue; // start again
                }
            }

            // check for important whitespace after token
            if (tok->buffer[i] == ',') tok->commaaftertoken = 1;
            else if (tok->buffer[i] == '\n') { tok->eolaftertoken = 1; tok->context->nextline++; }
            else if (tok->buffer[i] == '"') tok->quoteaftertoken = 1;

            // chop off token and return
            tok->buffer[i] = 0;
            if (!ignorecomma && tok->commanext) return g_comma;
            if (!ignorelinebreak && tok->eolnext) return g_eol;
            ret = tok->buffer + tok->offsetinbuffer;
            break; // do define processing
        }
    } // forever

    // have token in <ret>, process through defines
    if (tok->localdefines)
    {
        define = DefineLookup(tok->localdefines, ret);
        if (define) return define;
    }
    if (tok->globaldefines)
    {
        define = DefineLookup(tok->globaldefines, ret);
        if (define) return define;
    }
    if (tok->staticdefines)
    {
        define = StaticDefineLookup(tok->staticdefines, ret);
        if (define) return define;
    }
    return ret;
}

// external TokenizerGet function, as PeekToken but eats token
const char* TokenizerGet(TokenizerHandle tokenizer, int ignorelinebreak, int ignorecomma)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    const char* ret = TokenizerPeek(tokenizer, ignorelinebreak, ignorecomma);
    if (ret == NULL) return NULL; // should grab case where tok == NULL

    if (tok->eolnext) tok->context->curline = tok->context->nextline;
    if (!ignorecomma && tok->commanext) { tok->commanext = 0; return ret; }
    if (!ignorelinebreak && tok->eolnext) { tok->eolnext = 0; return ret; }
    tok->commanext = 0;
    tok->eolnext = 0;

    // eat token
    if (tok->inescape)
        tok->offsetinbuffer = tok->pastescape;
    else
    {
        while (*(tok->buffer + tok->offsetinbuffer)) tok->offsetinbuffer++; // find NULL
        tok->offsetinbuffer++;
    }
    tok->inescape = 0;
    tok->instring = 0;
    tok->exoffset = 0;

    // promote flags
    if (tok->commaaftertoken) { tok->commaaftertoken = 0; tok->commanext = 1; }
    if (tok->eolaftertoken) { tok->eolaftertoken = 0; tok->eolnext = 1; }
    if (tok->quoteaftertoken) { tok->quoteaftertoken = 0; tok->quotenext = 1; }

    return ret;
}

const char* TokenizerPeekEx(TokenizerHandle tokenizer, int ignorelinebreak, int ignorecomma, char* seps, int* length)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    const char *end, *peek = TokenizerPeek(tokenizer, ignorelinebreak, ignorecomma);
    int len;
    if (peek == NULL) return NULL;

    // sanity check ourselves first..
    len = (int)strlen(peek);
    if (len <= tok->exoffset)
    {
        Errorf("Internal tokenizer error");
        return peek;
    }

    // inside strings we don't look for separators
    if (tok->instring || tok->inescape)
    {
        *length = len;
        return peek;
    }

    // exoffset should point to our current subtoken, figure out correct length of it
    peek += tok->exoffset;
    if (strchr(seps, *peek)) // pointing to seperator
    {
        *length = 1; // all seperators are considered separate subtokens
    }
    else // pointing to non-seperator, get whole subtoken
    {
        end = peek+1;
        while (*end && !strchr(seps, *end)) end++;
        *length = end - peek;
    }
    return peek;
}

const char* TokenizerGetEx(TokenizerHandle tokenizer, int ignorelinebreak, int ignorecomma, char* seps, int* length)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    const char *peek = TokenizerPeekEx(tokenizer, ignorelinebreak, ignorecomma, seps, length);
    int len;

    // eat subtoken if possible, otherwise degenerate to eating the rest of the token
    len = (int)strlen(peek);
    if (*length < len)
    {
        tok->exoffset += *length;
    }
    else
    {
        TokenizerGet(tokenizer, ignorelinebreak, ignorecomma);
    }
    return peek;
}

void TokenizerSetGlobalDefines(TokenizerHandle tokenizer, DefineContext* context)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok) return;
    tok->globaldefines = context;
}

DefineContext* TokenizerGetGlobalDefines(TokenizerHandle tokenizer)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok) return NULL;
    return tok->globaldefines;
}

void TokenizerSetLocalDefines(TokenizerHandle tokenizer, DefineContext* context)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok) return;
    tok->localdefines = context;
}

DefineContext* TokenizerGetLocalDefines(TokenizerHandle tokenizer)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok) return NULL;
    return tok->localdefines;
}

void TokenizerSetStaticDefines(TokenizerHandle tokenizer, StaticDefine* list)
{
    Tokenizer* tok = (Tokenizer*)tokenizer;
    if (!tok) return;
    tok->staticdefines = list;
}

// callback for when we really shouldn't be called back..
int ParserErrorCallback(TokenizerHandle tok, const char* nexttoken, void* structptr)
{
    const char* token = TokenizerGet(tok, 1, 1);
    ErrorFilenamef(((Tokenizer *)tok)->context->filename,"Parser: unrecognized token %s in %s\n", token, TokenizerGetFileAndLine(tok));
    g_parseerr = 1;

    return 1; // don't fail out
}

