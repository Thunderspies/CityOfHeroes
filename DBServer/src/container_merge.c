#include <string.h>
#include <stdlib.h>
#include "container_tplt.h"
#include "container_merge.h"
#include <utilitieslib/utils/utils.h>
#include "dbserver/servercfg.h"
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/mathutil.h>
#include "dbinit.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/components/estring.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/log.h>

static int cmpLines(const LineTracker *a,const LineTracker *b)
{
    return a->idx - b->idx;
}

static int getMemSize(int size)
{
    return (size + 512) & ~(512-1);
}

static void growBuffer(char **buf, int *len, int size, int min)
{
    int newlen = MAX(MAX(min, *len), getMemSize(size));
    if (!*buf) {
        *buf = calloc(newlen, 1);
        *len = newlen;
    } else {
        if (newlen > *len) {
            free(*buf);
            *buf = calloc(newlen, 1);
            *len = newlen;
        }
    }
}

static int lineNotNull(ContainerTemplate *tplt,LineTracker *test_line,char *test_text)
{
    SlotInfo        *test_slot = &tplt->slots[test_line->idx];
    AttributeList    *attr = test_slot->table->columns[test_slot->idx].attr;

    if (test_line->is_str && test_line->str_idx == FAKE_STR_IDX)
    {
        return 0;
    }
    else if (attr)
    {
        static int none=-1,tex_none=-1;

        if (none < 0 && !stashFindInt(attr->hash_table,"none",&none))
            none = 0;
        if (tex_none < 0 && !stashFindInt(attr->hash_table,"!none",&tex_none))
            tex_none = 0;
        if (test_line->ival != none && test_line->ival != tex_none)
            return 1;
    }
    else if (test_line->is_str)
        return valueNotNull(test_text);
    else if (test_line->ival != 0)
        return 1;
    return 0;
}

void reinitLineList(LineList *list)
{
    list->cmd_count = 0;
    list->count = 0;
    list->text_count = 0;
}

void freeLineList(LineList *list)
{
    free(list->lines);
    free(list->row_cmds);
    free(list->text);
}

void clearLineList(LineList *list)
{
    freeLineList(list);
    memset(list,0,sizeof(*list));
}

int textToLineList(ContainerTemplate *tplt,char *text,LineList *list,char *errmsg)
{
    char    cmd_name[1000];
    char    *str=text,*name;
    int        idx,highest=0,re_sort=0,cmd_len;
    LineTracker    *line;

    reinitLineList(list);
    if (!str)
        return 0;
    for(;;)
    {
        name = strchr(str,' ');
        if (!name)
            break;
        cmd_len = name-str;
        if (cmd_len >= ARRAY_SIZE(cmd_name))
        {
            if (errmsg)
                sprintf(errmsg,"element too long");
            return 0;
        }
        memcpy(cmd_name,str,cmd_len);
        cmd_name[cmd_len] = 0;
        if (!stashFindInt(tplt->all_hashes,cmd_name,&idx))
        {
            LOG_OLD_ERR( "textLineToList had a bad_element: %s",cmd_name);
            if (errmsg)
            {
                sprintf(errmsg,"bad_element: %s",cmd_name);
                return 0;
            }
            goto failed;
        }
        tplt->lines[idx] = str;
        line = dynArrayAdd(&list->lines,sizeof(list->lines[0]),&list->count,&list->max_lines,1);
        line->idx = idx;
        line->is_str = 0;
        line->size = 0;

        if (idx > highest)
            highest = idx;
        else
            re_sort = 1;

        {
            TableInfo    *table = tplt->slots[idx].table;
            ColumnInfo    *col    = &table->columns[tplt->slots[idx].idx];
            char        *s,*value;
            int            len,curr_count;

            s = strchr(str,' ') + 1;
            len = (int)strcspn(s,"\n\r");
            if (*s == '"')
            {
                s++;
                len -= 2;
            }
            curr_count = list->text_count;

            value = dynArrayAdd(&list->text,1,&curr_count,&list->text_max,len+1);
            strncpy(value,s,len);
            value[len] = 0;

            if (col->attr)
            {
                if (!value[0])
                    stashFindInt(col->attr->hash_table,"none",&line->ival);
                else if (!stashFindInt(col->attr->hash_table,value,&line->ival))
                {
                    LOG_OLD_ERR( "textLineToList had bad_attribute: %s",value);
                    list->count--;
                    goto failed;
                }
            }
            else switch (col->data_type)
            {
                xcase CFTYPE_INT:
                case CFTYPE_SHORT:
                case CFTYPE_BYTE:
                    line->ival = atoi(value);
                xcase CFTYPE_FLOAT:
                    line->fval = atof(value);
                xdefault:
                    line->size = curr_count - list->text_count - 1; // subtract the NULL
                    line->is_str = 1;
                    line->str_idx = list->text_count;
                    list->text_count = curr_count;
            }
            //printf("%s = [%s]\n",cmd_name,value);
        }
failed:
        str = strchr(str,'\n');
        if (!str++)
            break;
    }
    if (re_sort)
        qsort(list->lines,list->count,sizeof(list->lines[0]),cmpLines);

    //printf("orig: %d, binary size: %d, text: %d\n",strlen(text),8*list->count,list->text_count);
    return 1;
}

const char *lineValueToText(ContainerTemplate *tplt,LineList *list,LineTracker *line)
{
    THREADSAFE_STATIC char buf[200];
    const char            *value = buf;
    TableInfo            *table = tplt->slots[line->idx].table;
    ColumnInfo            *col    = &table->columns[tplt->slots[line->idx].idx];

    if (line->is_str && line->str_idx == FAKE_STR_IDX)
    {
#ifdef FULLDEBUG
        assert(col->reserved_word); // This should only happen for for the ContainerId column
        assert(line->size == 0);
#endif
        // Indicate that this row has no data, but it does exist
        return FAKE_ROW_PLACEHOLDER;
    }
    else if (col->attr)
    {
#ifdef FULLDEBUG
        assert(!line->is_str);
        assert(line->size == 0);
#endif
        if (line->ival < 1 || line->ival >= eaSize(&col->attr->names))
            return "none";
        value = col->attr->names[line->ival];
    }
    else switch(col->data_type)
    {
        xcase CFTYPE_INT:
        case CFTYPE_SHORT:
        case CFTYPE_BYTE:
#ifdef FULLDEBUG
            assert(!line->is_str);
            assert(line->size == 0);
#endif
            sprintf(buf, "%d", line->ival);
        xcase CFTYPE_FLOAT:
#ifdef FULLDEBUG
            assert(!line->is_str);
            assert(line->size == 0);
#endif
            safe_ftoa(line->fval,buf);
        xdefault:
            value = list->text + line->str_idx;
#ifdef FULLDEBUG
            assert(line->is_str);            
            assert(line->size == strlen(value));
#endif
    }
    return value;
}

/**
* @note This is only allowed to be called from the main thread.
*/
static char* doOfflineFixup(const char *value)
{
    size_t index = 0;
    size_t valueLength = strlen(value);

    static char *fixedValue = NULL;

    if (!fixedValue)
        estrCreate(&fixedValue);

    estrClear(&fixedValue);

    do
    {
        size_t bytesUntilMatch = strcspn(&value[index], "'\"");
        estrConcatFixedWidth(&fixedValue, &value[index], (int)bytesUntilMatch);
        switch (value[index + bytesUntilMatch])
        {
        case '"':
            estrConcatStaticCharArray(&fixedValue, "\\q");
            break;
        case '\'':
            estrConcatStaticCharArray(&fixedValue, "\\s");
            break;
        case '\0':
            break;
        default:
            devassertmsg(0, "Programmer erred here.");
            break;
        }

        index += bytesUntilMatch + 1;
    }
    while (index < valueLength);

    return fixedValue;
}

char *lineListToText(ContainerTemplate *tplt,LineList *list,int offlineFixup)
{
    THREADSAFE_STATIC char *text;
    THREADSAFE_STATIC int text_max;

    int i;
    int text_count=0;

    THREADSAFE_STATIC_MARK(text);

    if (!text_max)
        dynArrayFit(&text, 1, &text_max, CONTAINER_TEXT_INITIAL_SIZE);

    for(i=0;i<list->count;i++)
    {
        LineTracker *line;
        TableInfo *table;
        ColumnInfo *col;
        char key[SHORT_SQL_STRING];
        int key_len;
        int quote_space = 0;
        char *s;
        const char *value;
        int value_len;

        line = &list->lines[i];
        table = tplt->slots[line->idx].table;
        col    = &table->columns[tplt->slots[line->idx].idx];

        if (table != tplt->tables)
            key_len = sprintf(key, "%s[%d].%s", table->name, tplt->slots[line->idx].sub_id, col->name);
        else
            key_len = sprintf(key, "%s", col->name);

        // Append the table name
        s = dynArrayAdd(&text, 1, &text_count, &text_max, key_len+1);
        sprintf(s, "%s", key);

        value = lineValueToText(tplt, list, line);

        // Repair broken data
        if (offlineFixup && col->data_type >= FIRST_QUOTED_TYPE)
            value = doOfflineFixup(value);

        value_len = (int)strlen(value);

        if (col->attr || col->data_type >= FIRST_QUOTED_TYPE)
            quote_space = 2;

        // Make room for the value, quotes and newline
        s = dynArrayAdd(&text, 1, &text_count, &text_max, value_len + quote_space + 1);

        // Overwrite the previous null with a space
        s[-1] = ' ';

        if (quote_space)
            *s++ = '"';

        strcpy(s, value);
        s += value_len;

        if (quote_space)
            *s++ = '"';

        *s = '\n';
    }

    // Null terminate the string
    *((char*)dynArrayAdd(&text, 1, &text_count, &text_max, 1)) = 0;

    return text;
}

void mergeLineLists(ContainerTemplate *tplt,LineList *orig,LineList *diff,LineList *curr)
{
    int            orig_idx=0,diff_idx=0,last_subid=0,row_idx=0,row_text_idx=0,data_set=0;
    LineTracker    *orig_line,*diff_line,*curr_line=0,*src_line;
    char        *src_text;
    TableInfo    *last_table=0;
    SlotInfo    *src_slot;
    RowAddDel    *cmd;

    reinitLineList(curr);
    for(;;)
    {
        src_text = 0;
        orig_line = diff_line = 0;
        if (orig_idx < orig->count)
            orig_line = &orig->lines[orig_idx];
        if (diff_idx < diff->count)
            diff_line = &diff->lines[diff_idx];
        if (!orig_line && !diff_line)
            break;
        if (!orig_line || (diff_line && diff_line->idx <= orig_line->idx))
        {
            if (orig_line && diff_line->idx == orig_line->idx)
                orig_idx++;
            else
            {
                // see if we need to add a row
                int            i,row_found=0;
                SlotInfo    *orig_slot,*diff_slot = &tplt->slots[diff_line->idx];

                for(i=0;i<2;i++)
                {
                    if (orig_idx+i >= orig->count)
                        continue;
                    orig_slot    = &tplt->slots[orig_line->idx];
                    if (orig_slot->table == diff_slot->table && orig_slot->sub_id == diff_slot->sub_id)
                    {
                        row_found = 1;
                        break;
                    }
                }
                if (!row_found && (diff_slot->table != last_table || last_subid !=  diff_slot->sub_id))
                {
                    cmd = dynArrayAdd(&diff->row_cmds,sizeof(diff->row_cmds[0]),&diff->cmd_count,&diff->cmd_max,1);
                    cmd->idx = diff_line->idx;
                    cmd->add = 1;
                }
            }
            src_line = diff_line;
            diff_idx++;
            if (diff_line->is_str)
                src_text = diff->text + diff_line->str_idx;
        }
        else
        {
            src_line = orig_line;
            orig_idx++;
            if (orig_line->is_str && orig_line->str_idx != FAKE_STR_IDX)
                src_text = orig->text + orig_line->str_idx;
        }
        src_slot = &tplt->slots[src_line->idx];
        if (curr->count  && (last_table != src_slot->table || last_subid != src_slot->sub_id))
        {
            if (!data_set)
            {
                //printf("del row %s[%d]\n",last_table->name,last_subid);
                //sqlDeleteRow(last_table->name,container_id,last_subid);
                cmd = dynArrayAdd(&diff->row_cmds,sizeof(diff->row_cmds[0]),&diff->cmd_count,&diff->cmd_max,1);
                cmd->idx = curr->lines[row_idx].idx;
                cmd->add = 0;

                curr->count = row_idx;
                curr->text_count = row_text_idx;
            }
            else
            {
                row_idx = curr->count;
                row_text_idx = curr->text_count;
            }
            data_set = 0;
        }
        if (!curr_line || &tplt->slots[curr_line->idx] != src_slot)
            curr_line = dynArrayAdd(&curr->lines,sizeof(curr->lines[0]),&curr->count,&curr->max_lines,1);
        *curr_line = *src_line;
        if (src_text)
        {
            char *s;

            curr_line->str_idx = curr->text_count;
            s = dynArrayAdd(&curr->text,1,&curr->text_count,&curr->text_max, (int)strlen(src_text)+1);
            strcpy(s,src_text);
        }
        data_set |= lineNotNull(tplt,src_line,src_text);
        last_table = src_slot->table;
        last_subid = src_slot->sub_id;
    }
    if (!data_set)
    {
        cmd = dynArrayAdd(&diff->row_cmds,sizeof(diff->row_cmds[0]),&diff->cmd_count,&diff->cmd_max,1);
        cmd->idx = curr->lines[row_idx].idx;
        cmd->add = 0;

        curr->count = row_idx;
        curr->text_count = row_text_idx;
    }
}

#define HEADER_SIZE (sizeof(int)*3)

void *lineListToMem(LineList *src,int *size_p,void **mem_p,int *max_p)
{
    THREADSAFE_STATIC int    *mem_static;
    THREADSAFE_STATIC int    mem_max;
    int        *mem,size,cmd_size,line_size,text_size;
    U8        *curr;

    THREADSAFE_STATIC_MARK(mem_static);

    if (!mem_p)
    {
        mem_p = &mem_static;
        max_p = &mem_max;
    }
    dynArrayFit(mem_p,1,max_p,HEADER_SIZE);
    mem = (int*)(*mem_p);
    line_size    = (mem[0]=src->count) * sizeof(src->lines[0]);
    cmd_size    = (mem[1]=src->cmd_count) * sizeof(src->row_cmds[0]);
    text_size    = (mem[2]=src->text_count) * sizeof(src->text[0]);
    size = line_size + cmd_size + text_size + HEADER_SIZE;
    dynArrayFit(mem_p,1,max_p,size);
    mem = (int*)(*mem_p);

    curr=((U8*)mem)+HEADER_SIZE;
    memcpy(curr,src->lines,line_size);
    curr+= line_size;

    memcpy(curr,src->row_cmds,cmd_size);
    curr+= cmd_size;

    memcpy(curr,src->text,text_size);
    if (size_p)
        *size_p = size;
    return *mem_p;
}

void arrayFit(void **basep,int struct_size,int *max_count,int idx_to_fit,void **datap)
{
    int        last_max,copy_bytes;
    char    *base = *basep,*data = *datap;
    #define    EXTRA_SPACE (32-1)

    if (idx_to_fit >= *max_count)
    {
        last_max = *max_count;
        *max_count = (idx_to_fit + EXTRA_SPACE) & ~EXTRA_SPACE;
        base = realloc(base,struct_size * *max_count);
        memset(base + struct_size * last_max,0,(*max_count - last_max) * struct_size);
        *basep = base;
    }
    copy_bytes = struct_size * idx_to_fit;
    memcpy(base,data,copy_bytes);
    *datap = data + copy_bytes;
}

void memToLineList(void *mem_v,LineList *dst)
{
    int        *mem = mem_v;
    U8        *curr=((U8*)mem)+HEADER_SIZE;

    dst->count    = mem[0];
    dst->cmd_count    = mem[1];
    dst->text_count    = mem[2];

    arrayFit(&dst->lines,sizeof(dst->lines[0]),&dst->max_lines,dst->count,&curr);
    arrayFit(&dst->row_cmds,sizeof(dst->row_cmds[0]),&dst->cmd_max,dst->cmd_count,&curr);
    arrayFit(&dst->text,sizeof(dst->text[0]),&dst->text_max,dst->text_count,&curr);
}

void copyLineList(LineList *src,LineList *dst)
{
    int        size;
    void    *mem;

    mem = lineListToMem(src,&size,0,0);
    memToLineList(mem,dst);
}

void tpltUpdateDataContainer(LineList *orig_list,LineList *diff_list,ContainerTemplate *tplt)
{
    THREADSAFE_STATIC LineList new_list;
    THREADSAFE_STATIC_MARK(new_list);

    mergeLineLists(tplt,orig_list,diff_list,&new_list);
    copyLineList(&new_list,orig_list);
}

void tpltFixOfflineData(char **buffer, int *bufferSize, ContainerTemplate *tplt)
{
    THREADSAFE_STATIC LineList list;
    char    *text;

    THREADSAFE_STATIC_MARK(list);

    if (server_cfg.container_size_debug)
        assert(!*buffer || *bufferSize);
    if (!tplt) {
        return;
    }

    textToLineList(tplt,*buffer,&list,0);
    text = lineListToText(tplt,&list,1);
    growBuffer(buffer, bufferSize, (int)strlen(text)+1, 256);
    strcpy(*buffer, text);
}

void tpltUpdateData(char **oldBuffer, int *bufferSize, char *diff,ContainerTemplate *tplt)
{
    THREADSAFE_STATIC LineList orig_list;
    THREADSAFE_STATIC LineList diff_list;
    THREADSAFE_STATIC LineList new_list;
    char    *text;

    THREADSAFE_STATIC_MARK(orig_list);
    THREADSAFE_STATIC_MARK(diff_list);
    THREADSAFE_STATIC_MARK(new_list);

    if (server_cfg.container_size_debug)
        assert(!*oldBuffer || *bufferSize);
    if (!tplt) {
        growBuffer(oldBuffer, bufferSize, (int)strlen(diff) + 1, 256);
        strcpy(*oldBuffer, diff);
        return;
    }

    textToLineList(tplt,diff,&diff_list,0);
    textToLineList(tplt,*oldBuffer,&orig_list,0);
    mergeLineLists(tplt,&orig_list,&diff_list,&new_list);
    text = lineListToText(tplt,&new_list,0);
    growBuffer(oldBuffer, bufferSize, (int)strlen(text) + 1, 256);
    strcpy(*oldBuffer, text);
}
