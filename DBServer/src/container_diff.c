#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utilitieslib/stdtypes.h>
#include "container_tplt_utils.h"
#include "container_diff.h"
#include "container.h"
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/strings_opt.h>
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/components/HashFunctions.h>
#include <utilitieslib/utils/log.h>

#define RESERVED(res) (strncmp(name,res,sizeof(res)-1)==0 && (name[sizeof(res)-1] == 0 || name[sizeof(res)-1] == ' '))
static int containerReservedField(char *name)
{
    if (RESERVED("TeamupsId")
        || RESERVED("SupergroupsId")
        || RESERVED("TaskforcesId")
        || RESERVED("AuthId")
        || RESERVED("AuthName")
        || RESERVED("Name")
        || RESERVED("StaticMapId")
        || RESERVED("MapId")
        || RESERVED("Active")
        || RESERVED("Ents2[0].LevelingPactsId")
        || RESERVED("Ents2[0].LeaguesId"))
        return 1;
    else
        return 0;
}

static int printNull(ContainerLine *old_line, char **buf)
{
    if (containerReservedField(old_line->field))
        return 0;
    
    return estrConcatf(buf, "%s %s\n", old_line->field, isNumber(old_line->value) ? "0" : "\"\"");
}

static int skipLine(char *line)
{
    char    *s;

    if (containerReservedField(line))
        return 1;
    s = strchr(line,' ');
    if (!s++ || valueNotNull(s))
        return 0;
    return 1;
}

void checkContainerDiff(const char *ref_orig,const char *diff_orig,const char *actual_diff,const char *orig_container_data)
{
    int        i,j,total_len,len_ref,len_diff;
    char    *r,*ref,*d,*diff;

    strdup_alloca(ref,ref_orig);
    strdup_alloca(diff,diff_orig);
    total_len = (int)(1 + MIN(strlen(ref),strlen(diff)));
    for(j=i=0;i<total_len;)
    {
        do
        {
            r = ref+i;
            len_ref = (int)strcspn(ref+i,"\n");
            r[len_ref] = 0;
            i += len_ref+1;
        }
        while(skipLine(r));

        do
        {
            d = diff+j;
            len_diff = (int)strcspn(diff+j,"\n");
            d[len_diff] = 0;
            j += len_diff+1;
        }
        while(skipLine(d));

        if (stricmp(r,d)!=0)
        {
            assertmsgf(0,"Diff Error REFERENCE:%s ACTUAL:%s DIFF:%s ORIG_CONTAINER:%s",ref_orig,diff_orig,actual_diff,orig_container_data);
        }
    }
}

char *containerDiff(const char *old,const char *curr)
{
    static IndexedContainer old_cont,curr_cont;
    int i, idx, j = 0, create = 0, spam, first_sub = 0;
    ContainerLine* old_line = NULL;
    ContainerLine* curr_line;
    static char * buf = NULL;
    static bool in_recursion = false;
    bool                    found;
    assert(!in_recursion); // not recursive or reentrant
    in_recursion = true;    

    if (!buf)
        estrCreateEx(&buf, CONTAINER_TEXT_INITIAL_SIZE);
    estrClear(&buf);

    if (!old)
    {
        estrConcatCharString(&buf, curr);
        in_recursion = false;
        return buf;
    }

    if (!old_cont.hash_table)
        create = 1;

    containerInitIndex(&curr_cont,curr,create);
    containerInitIndex(&old_cont,old,create);

    for(i=0;i<curr_cont.count;i++)
    {
        curr_line = &curr_cont.lines[i];

        found = stashFindInt(old_cont.hash_table,curr_line->field,&idx);
        if (found)
        {
            for(;j<idx;j++)
            {
                old_line = &old_cont.lines[j];

                if (!stashFindInt(curr_cont.hash_table,old_line->field,&spam))
                    printNull(&old_cont.lines[j], &buf);
            }
            old_line = &old_cont.lines[idx];
        }
        else if (!first_sub && strchr(curr_line->field,'['))
        {
            first_sub = 1;
            for(;j<old_cont.count;j++)
            {
                old_line = &old_cont.lines[j];

                if (strchr(old_line->field,'['))
                    break;

                if (!stashFindInt(curr_cont.hash_table,old_line->field,&spam))
                    printNull(&old_cont.lines[j], &buf);
            }
        }

        if (!found && !valueNotNull(curr_line->value)) // containerloadsave wrote a null, when value was already null
            continue;

        if (!found || strcmp(curr_line->value, old_line->value) != 0)
        {
#ifdef FULLDEBUG
            // This string indicates that the row exists, but has no data and should be skipped by valueNotNull
            assert(strcmp(curr_line->value, FAKE_ROW_PLACEHOLDER) != 0);
#endif

            estrConcatf(&buf, "%s %s\n", curr_line->field, curr_line->value);
        }
    }

    for(;j<old_cont.count;j++)
    {
        old_line = &old_cont.lines[j];

        if (!stashFindInt(curr_cont.hash_table,old_line->field,&spam))
            printNull(&old_cont.lines[j], &buf);
    }

    in_recursion = false;
    return buf;
}

typedef struct
{
    int        list_id;
    int        container_id;
    char    *data;
} ContainerCache;

#define CONTAINER_CACHE_BITS 11
#define CONTAINER_CACHE_SIZE (1 << CONTAINER_CACHE_BITS)
#define CONTAINER_CACHE_MASK (CONTAINER_CACHE_SIZE -1)

ContainerCache container_cache[CONTAINER_CACHE_SIZE];

static ContainerCache *findEntry(int list_id,int container_id)
{
    U32        hash,hash_data[2];

    hash_data[0] = container_id;
    hash_data[1] = list_id;
    hash = hashCalc((void*)hash_data, sizeof(hash_data), 0xa74d94e3);

    return &container_cache[hash & CONTAINER_CACHE_MASK];
}

static ContainerCache *findMatch(int list_id,int container_id)
{
    ContainerCache    *cache;

    cache = findEntry(list_id,container_id);
    if (!(cache->list_id == list_id && cache->container_id == container_id))
        return 0;
    return cache;
}

void containerCacheAdd(int list_id,int container_id,const char *data)
{
    ContainerCache    *cache;

    if (container_id < 0)
        return;
    cache = findEntry(list_id,container_id);
    if (cache->data)
        free(cache->data);
    cache->container_id = container_id;
    cache->list_id = list_id;
    cache->data = _strdup(data);
}

char *containerCacheDiff(int list_id,int container_id,const char *curr)
{
    ContainerCache    *cache;

    if (container_id < 0)
        return 0;
    if (!(cache = findMatch(list_id,container_id)))
        return 0;
    return containerDiff(cache->data,curr);
}

void containerCacheRemoveEntry(int list_id,int container_id)
{
    ContainerCache    *cache;

    if (!(cache = findMatch(list_id,container_id)))
        return;
    free(cache->data);
    memset(cache,0,sizeof(*cache));
}

void containerCacheSendDiff(Packet *pak,int list_id,int container_id,const char *data,int diff_debug)
{
    char *diff;
    
    diff = containerCacheDiff(list_id,container_id,data);
    

    if (!"cheesy checking") 
    {
        char *args[10],*s,*buf;
        int in_sub=0;
        s = buf = strdup(diff?diff:data);
        for(;;)
        {
            if (!tokenize_line(s,args,&s))
                break;
            if (strchr(args[0],'['))
                in_sub = 1;
            else
                assert(!in_sub);
        }
        free(buf);
    }

    if (diff)
    {
        pktSendBits(pak,1,0);
        pktSendBits(pak,1,diff_debug);
        pktSendString(pak,diff);
        if (diff_debug)
        {
            ContainerCache    *cache = findEntry(list_id,container_id);
            pktSendString(pak,data);
            LOG_DEBUG( "REFERENCE:%s DIFF:%s ORIG_CONTAINER:%s ",data,diff,cache->data);
        }
    }
    else
    {
        pktSendBits(pak,1,1);
        pktSendBits(pak,1,0);
        pktSendString(pak,data);
    }
    containerCacheAdd(list_id,container_id,data);
}

