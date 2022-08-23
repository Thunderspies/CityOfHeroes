#include <string.h>

#include "dbcontainerpack.h"
#include <utilitieslib/utils/cmdoldparse.h>
#include <utilitieslib/utils/timing.h>
#include <utilitieslib/utils/utils.h>
#include "float.h"
#include <utilitieslib/components/StringCache.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/earray.h>
#include "entity/gametypes.h"
#include "comm_backend.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/StringUtil.h>
#include <math.h>


#define DESC_STRUCTADDR(desc) ((LineDesc *)(desc->indirection[0].offset))
#define MAX_BUF 16384
#define DESC_DBSIZE(desc) ((desc->dbsize)?(desc->dbsize):(desc->size*2)) // Makes room for unescaped strings

/* Function siGetAddress()
 *    Given the beginning of a parent structure and an indirection definition,
 *    return the address of the field or structure specified by the indirection.
 *
 *    If the field has been marked as a pointer, indirection will be performed
 *    after the field is retrieved.
 *
 *    Returns:
 *        NULL - indirection cannot be performed.
 *        Valid address - the address of the specified structure or field.
 */
void* siApplyIndirection(unsigned char* parent, StructIndirection* indirection)
{
    unsigned char* child;

    // Make sure we have an indirection definition to work with.
    assert(indirection);

    // Indirection sanity check.
    if(!parent)
        return NULL;

    // Get to the address of the child field.
    child = parent + indirection->offset;

    // If the specified field is a pointer, perform the indirection now
    // to get to the beginning of the child structure.
    if(indirection->isPointer)
        child = *(unsigned char**)child;

    return child;
}

void* siApplyMultipleIndirections(unsigned char* parent, StructIndirection* indirections, int indirectionCount)
{
    int i;
    unsigned char* child = parent;

    for(i = 0; i < indirectionCount; i++)
    {
        child = siApplyIndirection(child, &indirections[i]);
    }

    return child;
}

int decodeLine(char **buffptr,char *table,char *field,char **val_ptr,int *idx_ptr,char *idxstr) // ought to be static
{
    char    *buff = *buffptr,*val,*s,*s2,*end;
    int        idx = 0,xx=0;
    static char *local_buff=0;


    if(!local_buff)
        estrCreate(&local_buff);
    estrClear(&local_buff);

    // Don't do anything if we don't have a valid buffer to work with.
    if (!*buff)
        return 0;

    // Skip all newline and return characters
    while(*buff == '\n' || *buff == '\r')
        buff++;

    // Copy a single line into a seperate buffer.
    end = strpbrk(buff, "\n\r" );
    estrConcatFixedWidth(&local_buff, buff, end-buff);
    if (*end == '\r')
        end++;

    // Store the newline character and null terminate the string.
    estrConcatChar(&local_buff, '\n');
    buff = end+1;

    val = strchr(local_buff,' ');
    if (!val)
        return 0;
    *val++ = 0;

    // Deal with quoted strings.
    if (*val == '"')
    {
        val++;
        s = strrchr(val,'"');
        *s = 0;
    }
    s = strchr(val,'\n');
    if (s)
        *s = 0;

    strcpy(table,local_buff);
    s = strchr(table,'[');
    if (s)
    {
        *s++ = 0;
        s2 = strchr(s,']');
        *s2++ = 0;
        if (idxstr)
            strcpy(idxstr,s);
        idx = atoi(s);
        s = strchr(s2,'.')+1;
        strcpy(field,s);
    }
    else
    {
        idx = 0;
        strcpy(field,table);
    }
    *buffptr = buff;
    *idx_ptr = idx;
    *val_ptr = val;
    return 1;
}


/* Function getAddr()
 *    Given a LineDesc, return the address of the specified substructure.    
 *
 *    Currently, this function is being used in one of 3 ways:
 *        1. Given a parent structure, retrieve the address of a specified field.
 *
 *        2. Given a parent structure, retrieve the base of a substructure or substructure array.
 *                Search for "getAddr() usage #2" to see an example of where this is used.
 *
 *        3. Given a parent structure array, retrieve a specific field of a particular element in the given array.
 *                Search for "getAddr() usage #3" to see an example of where this is used.
 */
static void* getAddr(char* mem, StructDesc* structDesc, LineDesc* desc, int idx)
{
    char* child = mem;

    // If there is no parent, give up.
    if (!mem)
        return 0;

    // Do we have a structure descriptions?
    // If so, apply any array indirection now.
    if(structDesc)
    {
        // If we're looking at an array of structures, perform structure indexing now.
        if(AT_STRUCT_ARRAY == structDesc->arrayDesc.type)
        {
            child += structDesc->structureSize * idx;
        }

        if(AT_POINTER_ARRAY == structDesc->arrayDesc.type)
        {
            child += sizeof(void*) * idx;
        }

        // If we're looking at a pointer array, we need to perform an indirection to get
        // to the base of the structure.
        if(AT_POINTER_ARRAY == structDesc->arrayDesc.type)
        {
            child = *(char**)child;
        }

        if (AT_EARRAY == structDesc->arrayDesc.type) {
            child = *(char**)child;
            child += sizeof(void*) * idx;
            child = *(char**)child;
        }
    }

    // Assuming "child" now points at the beginning of the parent structure,
    // get the address of the specified field by performing all indirections specified
    // by the line description.
    child = siApplyMultipleIndirections(child, desc->indirection, MAX_INDIRECTIONS);

    return child;
}


void lineToStruct(char* mem, StructDesc* structDesc, char* table, char* field, char* val, int idx)
{
    int i;
    LineDesc    *desc;
    char        *parent_mem = mem;

    // Get to the base of the array to be serialized.
    mem = siApplyMultipleIndirections(mem, structDesc->arrayDesc.indirection, MAX_INDIRECTIONS);

    for(i = 0; structDesc->lineDescs[i].name; i++)
    {
        desc = &structDesc->lineDescs[i];
        if (stricmp(desc->name,table)==0)
        {
            if(desc->indirection[0].offset == IGNORE_LINE_DESC)
            {
                // do nothing
            }
            else
            {
                switch (desc->type)
                {
                    xcase PACKTYPE_INT:
                    {
                        // getAddr() usage #1 when call from elsewhere.
                        // getAddr() usage #3 when called recursively.
                        int *iptr = getAddr(mem,structDesc,desc,idx);
                        *iptr = atoi(val);
                    }
                    xcase PACKTYPE_CONREF:
                    {
                        // getAddr() usage #1 when call from elsewhere.
                        // getAddr() usage #3 when called recursively.
                        int *iptr = getAddr(mem,structDesc,desc,idx);
                        *iptr = atoi(val);
                    }
                    xcase PACKTYPE_ATTR:
                    {
                        intptr_t* iptr = getAddr(mem,structDesc,desc,idx);
                        if (desc->int_from_str_func)
                            *iptr = desc->int_from_str_func(val);
                        else if (desc->int_from_ptr_and_str_func)
                        {
                            int    sub;
                            U8 *cptr = (U8 *)iptr;

                            sub = desc->indirection[1].offset;
                            if (!sub)
                                sub = desc->indirection[0].offset;
                            *iptr = desc->int_from_ptr_and_str_func(parent_mem,cptr-sub,val);
                        }
                        else
                            *iptr = (intptr_t)allocAddString(val);
                    }
                    xcase PACKTYPE_STR_UTF8:
                    case PACKTYPE_STR_ASCII:
                    {
                        char *sptr = getAddr(mem,structDesc,desc,idx);
                        strncpyt(sptr,unescapeString(val),desc->size);
                    }
                    xcase PACKTYPE_STR_UTF8_CACHED:
                    case PACKTYPE_STR_ASCII_CACHED:
                    {
                        const char **sptr = getAddr(mem,structDesc,desc,idx);
                        *sptr = allocAddString(unescapeString(val));
                    }
                    xcase PACKTYPE_ESTRING_UTF8:
                    case PACKTYPE_ESTRING_ASCII:
                    case PACKTYPE_LARGE_ESTRING_UTF8:
                    case PACKTYPE_LARGE_ESTRING_ASCII:
                    {
                        char **sptr = getAddr(mem,structDesc,desc,idx);
                        estrPrintCharString(sptr,unescapeString(val));
                    }
                    xcase PACKTYPE_LARGE_ESTRING_BINARY:
                    {
                        char **sptr = getAddr(mem,structDesc,desc,idx);
                        int len = (int)strlen(val);
                        estrSetLengthNoMemset(sptr, len);
                        unescapeDataToBuffer(*sptr, val, &len);
                        estrSetLengthNoMemset(sptr, len);
                    }
                    xcase PACKTYPE_TEXTBLOB:
                    {
                        char **sptr = getAddr(mem,structDesc,desc,idx);
                        *sptr = strdup(val);
                    }
                    xcase PACKTYPE_FLOAT:
                    {
                        F32 *fptr = getAddr(mem,structDesc,desc,idx);
                        *fptr = atof(val);
                    }
                    xcase PACKTYPE_DATE:
                    {
                        U32 *iptr = getAddr(mem,structDesc,desc,idx);
                        *iptr = timerGetSecondsSince2000FromString(val);
                    }
                    xcase PACKTYPE_SUB:
                    {
                        lineToStruct(mem, (StructDesc*)DESC_STRUCTADDR(desc), field, 0, val, idx);
                    }
                    xcase PACKTYPE_EARRAY:
                    {
                        StructDesc*    sub_descs;
                        void***        child;

                        sub_descs = (StructDesc*)DESC_STRUCTADDR(desc);
                        // get EArray root
                        child = siApplyMultipleIndirections(mem, sub_descs->arrayDesc.indirection, MAX_INDIRECTIONS);

                        if (!(*child)) {
                            eaCreate(child);
                        }
                        while (idx >= eaSizeUnsafe(child)) {
                            typedef void * (*Constructor)(void);
                            void *newElement=((Constructor)desc->size)();
                            eaPush(child, newElement);
                        }
                        if ((*child)[idx]==NULL) {
                            typedef void * (*Constructor)(void);
                            void *newElement=((Constructor)desc->size)();
                            eaSet(child, newElement, idx);
                        }
                        lineToStruct(mem, (StructDesc*)DESC_STRUCTADDR(desc), field, 0, val, idx);
                    }
                    xcase PACKTYPE_BIN_STR:
                    {
                        U32  *ptr = getAddr(mem, structDesc, desc, idx);
                        char *binString = hexStrToBinStr(val, desc->size * 2);
                        memcpy(ptr, binString, desc->size);
                    }                    
                    xdefault:
                    {
                        devassert(!"unknown packtype");
                    }    
                }                                
            }
            break;
        }
    }
}

// This function should parallel readRow() in container_sql.c
void structToLine(StuffBuff* sb, char* mem, char* table, StructDesc* structDesc, LineDesc *desc, int idx)
{
    char    buf[MAX_BUF],*parent_mem = mem;

    if (desc->flags & LINEDESCFLAG_READONLY)
        return;
    mem = siApplyMultipleIndirections(parent_mem, structDesc->arrayDesc.indirection, MAX_INDIRECTIONS);

    if (table) {
        // WAS: sprintf(buf,"%s[%d].%s",table,idx,desc->name);
        STR_COMBINE_BEGIN(buf);
        STR_COMBINE_CAT(table);
        STR_COMBINE_CAT("[");
        itoa(idx, c, 10);
        c += strlen(c);
        STR_COMBINE_CAT("].");
        STR_COMBINE_CAT(desc->name);
        STR_COMBINE_END();
    } else
        strcpy(buf, desc->name);

    if(desc->indirection[0].offset == IGNORE_LINE_DESC)
    {
        // do nothing
    }
    else
    {
        switch (desc->type)
        {
            xcase PACKTYPE_INT:
            {
                int *iptr = getAddr(mem,structDesc,desc,idx);
                if (iptr && *iptr)
                {
                    //WAS: addStringToStuffBuff( sb, "%s %d\n",buf,*iptr);
                    addSingleStringToStuffBuff( sb, buf);
                    addSingleStringToStuffBuff( sb, " ");
                    addIntToStuffBuff( sb, *iptr);
                    addSingleStringToStuffBuff( sb, "\n");
                }
            }
            xcase PACKTYPE_CONREF:
            {
                int *iptr = getAddr(mem,structDesc,desc,idx);
                if (iptr && *iptr)
                {
                    //WAS: addStringToStuffBuff( sb, "%s %d\n",buf,*iptr);
                    addSingleStringToStuffBuff( sb, buf);
                    addSingleStringToStuffBuff( sb, " ");
                    addIntToStuffBuff( sb, *iptr);
                    addSingleStringToStuffBuff( sb, "\n");
                }
            }
            xcase PACKTYPE_ATTR:
            {
                intptr_t* iptr = getAddr(mem,structDesc,desc,idx);
                if (iptr && *iptr)
                {
                    const char *attr;

                    if (desc->str_from_int_func)
                    {
                        attr = desc->str_from_int_func(*iptr);
                    }
                    else if (desc->str_from_ptr_and_int_func)
                    {
                        int sub;
                        U8 *cptr = (U8 *)iptr;

                        sub = desc->indirection[1].offset;
                        if (!sub)
                            sub = desc->indirection[0].offset;

                        attr = desc->str_from_ptr_and_int_func(parent_mem,cptr-sub,*iptr);
                    }
                    else
                    {
                        attr = (char*)*iptr;
                    }

                    if(attr)
                    {
                        char *dup = _strlwr(strcpy(_alloca(strlen(attr)+1), attr));

                        addSingleStringToStuffBuff( sb, buf);
                        addSingleStringToStuffBuff( sb, " \"");
                        addSingleStringToStuffBuff( sb, dup);
                        addSingleStringToStuffBuff( sb, "\"\n");
                    }
                }
            }
            xcase PACKTYPE_FLOAT:
            {
                F32 *fptr = getAddr(mem,structDesc,desc,idx);
                if (fptr && *fptr)
                {
                    if (! _finite(*fptr)) // check for valid float
                        *fptr = 0;
                    addStringToStuffBuff( sb, "%s %F\n",buf,*fptr);
                }
            }
            xcase PACKTYPE_STR_UTF8:
            case PACKTYPE_STR_ASCII:
            {
                char *sptr = getAddr(mem,structDesc,desc,idx);
                if (sptr && *sptr) {
                    //WAS: addStringToStuffBuff( sb, "%s \"%s\"\n",buf,escapeString(sptr));
                    addSingleStringToStuffBuff( sb, buf);
                    addSingleStringToStuffBuff( sb, " \"");
                    addSingleStringToStuffBuff( sb, escapeStringn(sptr, DESC_DBSIZE(desc)));
                    addSingleStringToStuffBuff( sb, "\"\n");
                }
            }
            xcase PACKTYPE_ESTRING_UTF8:
            case PACKTYPE_ESTRING_ASCII:
            case PACKTYPE_STR_UTF8_CACHED:
            case PACKTYPE_STR_ASCII_CACHED:
            {
                char **sptr = getAddr(mem,structDesc,desc,idx);
                if (sptr && *sptr && **sptr)
                {
                    addSingleStringToStuffBuff( sb, buf);
                    addSingleStringToStuffBuff( sb, " \"");
                    addSingleStringToStuffBuff( sb, escapeStringn(*sptr, DESC_DBSIZE(desc)));
                    addSingleStringToStuffBuff( sb, "\"\n");
                }
            }
            xcase PACKTYPE_TEXTBLOB:
            {
                char **sptr = getAddr(mem, structDesc, desc, idx);
                if(sptr && *sptr && **sptr)
                {
                    addSingleStringToStuffBuff( sb, buf);
                    addSingleStringToStuffBuff( sb, " \"");
                    addEscapedDataToStuffBuff(sb, *sptr, (int)strlen(*sptr));
                    addSingleStringToStuffBuff( sb, "\"\n");
                }
            }
            xcase PACKTYPE_LARGE_ESTRING_BINARY:
            case PACKTYPE_LARGE_ESTRING_UTF8:
            case PACKTYPE_LARGE_ESTRING_ASCII:
            {
                char **sptr = getAddr(mem, structDesc, desc, idx);
                if(estrLength(sptr))
                {
                    addSingleStringToStuffBuff( sb, buf);
                    addSingleStringToStuffBuff( sb, " \"");
                    addEscapedDataToStuffBuff(sb, *sptr, estrLength(sptr));
                    addSingleStringToStuffBuff( sb, "\"\n");
                }
            }
            xcase PACKTYPE_DATE:
            {
                U32        *iptr = getAddr(mem,structDesc,desc,idx);
                char    datestr[100];

                if (iptr)
                {
                    int year = timerDayFromSecondsSince2000(*iptr)/10000;
                    if(year >= 2001 && year <= 2100) // the dbserver ignores dates outside this range
                    {
                        //WAS: addStringToStuffBuff( sb, "%s \"%s\"\n",buf,timerMakeDateStringFromSecondsSince2000(datestr,*iptr));
                        addSingleStringToStuffBuff( sb, buf);
                        addSingleStringToStuffBuff( sb, " \"");
                        addSingleStringToStuffBuff( sb, timerMakeDateStringFromSecondsSince2000(datestr,*iptr));
                        addSingleStringToStuffBuff( sb, "\"\n");
                    }
                }
            }
            xcase PACKTYPE_SUB:
            {
                StructDesc*    sub_descs;
                int            i,j;

                sub_descs = (StructDesc*)DESC_STRUCTADDR(desc);

                // Save out up to the specified number of structures.
                for(j=0;j<desc->size;j++)
                {
                    // For each structure, save out all relavent fields.
                    for(i=0;sub_descs->lineDescs[i].type;i++)
                        structToLine(sb,mem,desc->name,sub_descs,&sub_descs->lineDescs[i],j);
                }
            }
            xcase PACKTYPE_EARRAY:
            {
                StructDesc*    sub_descs;
                void***        child;
                int            i,j;

                sub_descs = (StructDesc*)DESC_STRUCTADDR(desc);
                // get EArray root
                child = siApplyMultipleIndirections(mem, sub_descs->arrayDesc.indirection, MAX_INDIRECTIONS);

                // Save out up to the specified number of structures.
                for(j=0;j<eaSizeUnsafe(child);j++)
                {
                    // For each structure, save out all relavent fields.
                    for(i=0;sub_descs->lineDescs[i].type;i++)
                        structToLine(sb,mem,desc->name,sub_descs,&sub_descs->lineDescs[i],j);
                }
            }
            xcase PACKTYPE_BIN_STR:
            {
                unsigned char *ptr = getAddr(mem, structDesc, desc, idx);
                if(ptr)
                {
                    char *hexString = binStrToHexStr(ptr, desc->size);
                    if(hexString[0]) {
                        //WAS: addStringToStuffBuff( sb, "%s \"%s\"\n", buf, hexString);
                        addSingleStringToStuffBuff( sb, buf);
                        addSingleStringToStuffBuff( sb, " \"");
                        addSingleStringToStuffBuff( sb, hexString);
                        addSingleStringToStuffBuff( sb, "\"\n");
                    }
                }
            }
            xdefault:
            {
                devassert(!"unknown packtype");
            }    
        }
    }        
}

static void structLineType(char *buf, size_t bufsz, char *table, LineDesc *desc)
{
    assert((desc->type == PACKTYPE_ATTR) || (!desc->str_from_int_func && !desc->str_from_ptr_and_int_func));

    switch (desc->type)
    {
        xcase PACKTYPE_ATTR:
        {
            assert(desc->size == MAX_ATTRIBNAME_LEN);
            snprintf_s(buf, bufsz, bufsz, "attribute");
        }
        xcase PACKTYPE_STR_UTF8:
        case PACKTYPE_ESTRING_UTF8:
        case PACKTYPE_STR_UTF8_CACHED:
        {
            assert(DESC_DBSIZE(desc) > 0 && DESC_DBSIZE(desc) < 0x1000);
            snprintf_s(buf, bufsz, bufsz, "unicodestring[%d]", DESC_DBSIZE(desc));
        }
        xcase PACKTYPE_STR_ASCII:
        case PACKTYPE_ESTRING_ASCII:
        case PACKTYPE_STR_ASCII_CACHED:
        {
            assert(DESC_DBSIZE(desc) > 0 && DESC_DBSIZE(desc) < 0x1000);
            snprintf_s(buf, bufsz, bufsz, "ansistring[%d]", DESC_DBSIZE(desc));
        }
        xcase PACKTYPE_LARGE_ESTRING_BINARY:
        {
            assert(desc->size == 0);
            snprintf_s(buf, bufsz, bufsz, "binary(max)");
        }
        xcase PACKTYPE_LARGE_ESTRING_UTF8:
        {
            assert(desc->size == 0);
            snprintf_s(buf, bufsz, bufsz, "unicodestring(max)");
        }
        xcase PACKTYPE_LARGE_ESTRING_ASCII:
        {
            assert(desc->size == 0);
            snprintf_s(buf, bufsz, bufsz, "ansistring(max)");
        }
        xcase PACKTYPE_BIN_STR:
        {
            // Binary strings are stored as hexadecimal characters.  Since each byte is
            // represented by two ascii characters, it requires a storage space twice as
            // long as the actual data.
            assert(desc->size > 0 && desc->size*2 < 0x1000);
            snprintf_s(buf, bufsz, bufsz, "ansistring[%d]", desc->size * 2);
        }
        xcase PACKTYPE_INT:
        {
            assert(desc->size == SIZE_INT32 || desc->size == SIZE_INT16 || desc->size == SIZE_INT8);
            snprintf_s(buf, bufsz, bufsz, "int%d", desc->size);
        }
        xcase PACKTYPE_CONREF:
        {
            assert(desc->size > 0 && desc->size < MAX_CONTAINER_TYPES);
            snprintf_s(buf, bufsz, bufsz, "int4");
        }
        xcase PACKTYPE_FLOAT:
        {
            assert(desc->size == SIZE_FLOAT32);
            snprintf_s(buf, bufsz, bufsz, "float%d", desc->size);
        }
        xcase PACKTYPE_DATE:
        {
            assert(desc->size == 0);
            snprintf_s(buf, bufsz, bufsz, "datetime");
        }
        xcase PACKTYPE_TEXTBLOB:
        {
            assert(desc->size == 0);
            snprintf_s(buf, bufsz, bufsz, "textblob");
        }
        xdefault:
        {
            devassert(!"unknown packtype");
        }    
    }
}

void structToLineTemplate(StuffBuff *sb,char *table,LineDesc *desc,int idx) // ought to be static
{
    if (desc->type == PACKTYPE_SUB || desc->type == PACKTYPE_EARRAY)
    {
        int eff_size = desc->size;
        StructDesc *sub_descs;
        int i;

        if (desc->type == PACKTYPE_EARRAY)
            eff_size = MAX_CONTAINER_EARRAY_SIZE; // EArray structs use ->size for something else

        sub_descs = (StructDesc*)DESC_STRUCTADDR(desc);
        for(i=0; sub_descs->lineDescs[i].type; i++)
            structToLineTemplate(sb, desc->name, &sub_descs->lineDescs[i], eff_size);
    }
    else
    {
        char type[1024];

        if (table)
            addStringToStuffBuff(sb, "%s[%d].%s", table, idx, desc->name);
        else
            addSingleStringToStuffBuff(sb, desc->name);

        structLineType(type, ARRAY_SIZE(type), table, desc);
        addStringToStuffBuff(sb, " \"%s\"%s\n", type, (desc->flags &  LINEDESCFLAG_INDEXEDCOLUMN) ? " indexed" : "");
    }
}

void structToLineSchema(StuffBuff *sb, char *table, LineDesc *desc, int idx)
{
    if (desc->type == PACKTYPE_SUB || desc->type == PACKTYPE_EARRAY)
    {
        int i;
        int eff_size = desc->type == PACKTYPE_EARRAY ? 9999 : desc->size;
        StructDesc* sub_descs = (StructDesc*)DESC_STRUCTADDR(desc);

        addStringToStuffBuff(sb, "<tr bgcolor='#eeeeee'><td valign=top>%s<td valign=top>table<td>%s\n", desc->name, sub_descs->desc ? sub_descs->desc : "");
        addSingleStringToStuffBuff(sb, "<tr><td><td colspan=2><table cellspacing=0 cellpadding=3 width='100%%'>\n");
        for(i = 0; sub_descs->lineDescs[i].type; i++)
            structToLineSchema(sb, desc->name, &sub_descs->lineDescs[i], eff_size);
        addSingleStringToStuffBuff(sb, "</table>\n");
    }
    else
    {
        char type[1024];
        structLineType(type, ARRAY_SIZE(type), table, desc);
        addStringToStuffBuff(sb, "<tr><td>%s<td>%s%s<td>%s\n", desc->name, type, (desc->flags &  LINEDESCFLAG_INDEXEDCOLUMN) ? " indexed" : "", desc->desc ? desc->desc : "");
    }
}

void dbContainerUnpack(StructDesc *descs,char *buff,void *mem)
{
    int            idx;
    char        table[100],field[200],*val;

    while(decodeLine(&buff,table,field,&val,&idx,0))
    {
        lineToStruct(mem,descs,table,field,val,idx);
    }
}

char *dbContainerPackage(StructDesc *descs,void *mem)
{
    StuffBuff    sb;
    int            i;

    initStuffBuff( &sb, MAX_BUF );
    for(i=0;descs->lineDescs[i].type;i++)
        structToLine(&sb,mem,0,descs,&descs->lineDescs[i],0);
    return sb.buff;
}

char *dbContainerTemplate(StructDesc *descs)
{
    StuffBuff    sb;
    int            i;

    initStuffBuff( &sb, MAX_BUF );
    for(i=0;descs->lineDescs[i].type;i++)
        structToLineTemplate(&sb,0,&descs->lineDescs[i],1);
    return sb.buff;
}

char *dbContainerSchema(StructDesc *descs, char *name)
{
    StuffBuff    sb;
    int            i;

    initStuffBuff(&sb, MAX_BUF);

    addSingleStringToStuffBuff(&sb, "<html><body>");
    addSingleStringToStuffBuff(&sb, "<table cellspacing=0 cellpadding=3>\n");
    addStringToStuffBuff(&sb, "<tr bgcolor='#eeeeee'><td valign=top>%s<td valign=top>table<td>%s\n", name, descs->desc ? descs->desc : "");
    addSingleStringToStuffBuff(&sb, "<tr><td><td colspan=2><table cellspacing=0 cellpadding=3 width='100%%'>\n");
    for(i = 0; descs->lineDescs[i].type; i++)
        structToLineSchema(&sb, 0, &descs->lineDescs[i], 1);
    addSingleStringToStuffBuff(&sb, "</table>\n");
    addSingleStringToStuffBuff(&sb, "</table>\n");
    addSingleStringToStuffBuff(&sb, "</body></html>\n");

    return sb.buff;
}
