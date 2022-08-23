#include "utilitieslib/network/bindiff.h"
#include "utilitieslib/assert/assert.h"
#include <stdio.h>
#include "utilitieslib/utils/file.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/components/StashTable.h"
#include <string.h>

THREADSAFE_STATIC int insert_bytes,copy_bytes,insert_ops,copy_ops;

typedef struct
{
    U8    *data;
    U32    size;
    int    max;
} DataBlock;

typedef struct
{
    int    src_pos;
    int    count;
} DiffCommand;

typedef struct
{
    DiffCommand    *data;
    U32    size;
    int    max;
} CommandBlock;

typedef struct
{
    int                MinMatchSize;
    StashTable        FingerPrintTable;
    DataBlock        src,dst;
    CommandBlock    commands;
    DataBlock        copydata;
    U16                checksum_low,checksum_high;
    U32                *rolling_crcs;
} DiffState;



// Ripped from glib.
// Visit http://www.geocities.com/primes_r_us/ and produce a new table
// if this is not suitable.
static const unsigned int primes[] =
{
  11,
  19,
  37,
  73,
  109,
  163,
  251,
  367,
  557,
  823,
  1237,
  1861,
  2777,
  4177,
  6247,
  9371,
  14057,
  21089,
  31627,
  47431,
  71143,
  106721,
  160073,
  240101,
  360163,
  540217,
  810343,
  1215497,
  1823231,
  2734867,
  4102283,
  6153409,
  9230113,
  13845163,
};

static const unsigned short randomValue[256] =
{
  0xbcd1, 0xbb65, 0x42c2, 0xdffe, 0x9666, 0x431b, 0x8504, 0xeb46,
  0x6379, 0xd460, 0xcf14, 0x53cf, 0xdb51, 0xdb08, 0x12c8, 0xf602,
  0xe766, 0x2394, 0x250d, 0xdcbb, 0xa678, 0x02af, 0xa5c6, 0x7ea6,
  0xb645, 0xcb4d, 0xc44b, 0xe5dc, 0x9fe6, 0x5b5c, 0x35f5, 0x701a,
  0x220f, 0x6c38, 0x1a56, 0x4ca3, 0xffc6, 0xb152, 0x8d61, 0x7a58,
  0x9025, 0x8b3d, 0xbf0f, 0x95a3, 0xe5f4, 0xc127, 0x3bed, 0x320b,
  0xb7f3, 0x6054, 0x333c, 0xd383, 0x8154, 0x5242, 0x4e0d, 0x0a94,
  0x7028, 0x8689, 0x3a22, 0x0980, 0x1847, 0xb0f1, 0x9b5c, 0x4176,
  0xb858, 0xd542, 0x1f6c, 0x2497, 0x6a5a, 0x9fa9, 0x8c5a, 0x7743,
  0xa8a9, 0x9a02, 0x4918, 0x438c, 0xc388, 0x9e2b, 0x4cad, 0x01b6,
  0xab19, 0xf777, 0x365f, 0x1eb2, 0x091e, 0x7bf8, 0x7a8e, 0x5227,
  0xeab1, 0x2074, 0x4523, 0xe781, 0x01a3, 0x163d, 0x3b2e, 0x287d,
  0x5e7f, 0xa063, 0xb134, 0x8fae, 0x5e8e, 0xb7b7, 0x4548, 0x1f5a,
  0xfa56, 0x7a24, 0x900f, 0x42dc, 0xcc69, 0x02a0, 0x0b22, 0xdb31,
  0x71fe, 0x0c7d, 0x1732, 0x1159, 0xcb09, 0xe1d2, 0x1351, 0x52e9,
  0xf536, 0x5a4f, 0xc316, 0x6bf9, 0x8994, 0xb774, 0x5f3e, 0xf6d6,
  0x3a61, 0xf82c, 0xcc22, 0x9d06, 0x299c, 0x09e5, 0x1eec, 0x514f,
  0x8d53, 0xa650, 0x5c6e, 0xc577, 0x7958, 0x71ac, 0x8916, 0x9b4f,
  0x2c09, 0x5211, 0xf6d8, 0xcaaa, 0xf7ef, 0x287f, 0x7a94, 0xab49,
  0xfa2c, 0x7222, 0xe457, 0xd71a, 0x00c3, 0x1a76, 0xe98c, 0xc037,
  0x8208, 0x5c2d, 0xdfda, 0xe5f5, 0x0b45, 0x15ce, 0x8a7e, 0xfcad,
  0xaa2d, 0x4b5c, 0xd42e, 0xb251, 0x907e, 0x9a47, 0xc9a6, 0xd93f,
  0x085e, 0x35ce, 0xa153, 0x7e7b, 0x9f0b, 0x25aa, 0x5d9f, 0xc04d,
  0x8a0e, 0x2875, 0x4a1c, 0x295f, 0x1393, 0xf760, 0x9178, 0x0f5b,
  0xfa7d, 0x83b4, 0x2082, 0x721d, 0x6462, 0x0368, 0x67e2, 0x8624,
  0x194d, 0x22f6, 0x78fb, 0x6791, 0xb238, 0xb332, 0x7276, 0xf272,
  0x47ec, 0x4504, 0xa961, 0x9fc8, 0x3fdc, 0xb413, 0x007a, 0x0806,
  0x7458, 0x95c6, 0xccaa, 0x18d6, 0xe2ae, 0x1b06, 0xf3f6, 0x5050,
  0xc8e8, 0xf4ac, 0xc04c, 0xf41c, 0x992f, 0xae44, 0x5f1b, 0x1113,
  0x1738, 0xd9a8, 0x19ea, 0x2d33, 0x9698, 0x2fe9, 0x323f, 0xcde2,
  0x6d71, 0xe37d, 0xb697, 0x2c4f, 0x4373, 0x9102, 0x075d, 0x8e25,
  0x1672, 0xec28, 0x6acb, 0x86cc, 0x186e, 0x9414, 0xd674, 0xd1a5
};

#define ROLLING_MASK 0xffffff

static U32 checksum(const unsigned char* buf, int matchSize)
{
  int i = matchSize;
  unsigned short low  = 0;
  unsigned short high = 0;

  for (; i != 0; i -= 1)
  {
      low  += randomValue[*buf++];
      high += low;
  }

  return (high << 16 | low) & ROLLING_MASK;
}

static int checksumInit(DiffState *state,const unsigned char* buf, int size)
{
    int        i;

    state->checksum_low = state->checksum_high = 0;
    for (i=0;i<size;i++)
    {
        state->checksum_low  += randomValue[buf[i]];
        state->checksum_high += state->checksum_low;
    }
    return (state->checksum_high << 16 | state->checksum_low) & ROLLING_MASK;
}

static int checksumRoll(DiffState *state,const unsigned char* buf,int size)
{
    state->checksum_low = state->checksum_low - randomValue[buf[-1]] + randomValue[buf[size-1]];
    state->checksum_high = state->checksum_high - randomValue[buf[-1]] * size + state->checksum_low;
    return (state->checksum_high << 16 | state->checksum_low) & ROLLING_MASK;
}


U32 *bindiffMakeFingerprints(U8 *data,U32 len,int minMatchSize,U32 *size_p)
{
    U32 j,i,*crcs;

    crcs = malloc(len * sizeof(crcs[0]) / minMatchSize);
    for(j=i=0;i+minMatchSize <= len; i+=minMatchSize,j++)
    {
        crcs[j] = checksum(&data[i],minMatchSize);
    }
    if (size_p)
        *size_p = len / minMatchSize;
    return crcs;
}

static void hashFingerPrintTable(DiffState *diff)
{
    U32 j,i;

    diff->FingerPrintTable = stashTableCreateInt(4 * diff->src.size / diff->MinMatchSize);
    for(j=i=0;i+diff->MinMatchSize <= diff->src.size; i+=diff->MinMatchSize,j++)
    {
        if (diff->rolling_crcs[j])
            stashIntAddInt(diff->FingerPrintTable,diff->rolling_crcs[j],j,1);
    }
}

static void addCommand(DiffState *diff,int insert,int count,int src_idx)
{
    DiffCommand    *cmd;

    if (insert)
    {
        insert_bytes+=count;
        insert_ops++;
        src_idx = 0x80000000;
    }
    else
    {
        copy_bytes+=count;
        copy_ops++;
    }
    cmd = dynArrayAdd(&diff->commands.data,sizeof(diff->commands.data[0]),&diff->commands.size,&diff->commands.max,1);
    cmd->src_pos    = src_idx;
    cmd->count        = count;
}

static void insertData(DiffState *diff,int pos,int count)
{
    U8    *mem;

    if (!count)
        return;
    mem = dynArrayAdd(&diff->copydata.data,sizeof(diff->copydata.data[0]),&diff->copydata.size,&diff->copydata.max,count);
    memcpy(mem,diff->dst.data+pos,count);
    addCommand(diff,1,count,0);
}

static void copyData(DiffState *diff,int pos,int src_pos,int count)
{
    addCommand(diff,0,count,src_pos);
}

static int expandMatch(const DataBlock *src,DataBlock *dst,int src_idx,int dst_idx,int *backwards_p,int max_match)
{
    int        i,count,backwards = *backwards_p;
    U8        *s,*d;

    s = &src->data[src_idx];
    d = &dst->data[dst_idx];

    backwards = MIN(src_idx,backwards);
    for(i=1;i<=backwards;i++)
    {
        if (s[-i] != d[-i])
            break;
    }
    backwards = i-1;
    count = MIN(src->size - src_idx,dst->size - dst_idx);
    count = MIN(count,max_match - backwards);
    for(i=0;i<count;i++)
    {
        if (s[i] != d[i])
            break;
    }
    *backwards_p = backwards;
    return i + backwards;
}

static void calcDiff(DiffState *diff)
{
    U32                    match_size,j,i,insert_pos=0,src_idx,idx;
    int                    hash=0,re_init=1,backwards=0;
    const DataBlock        * const src = &diff->src;
    DataBlock            * const dst = &diff->dst;
    const unsigned        max_src_idx = diff->src.size/diff->MinMatchSize;

    insert_bytes = copy_bytes = insert_ops = copy_ops = 0;

    for(i=0;i+diff->MinMatchSize <= dst->size;i++)
    {
        if (re_init)
            hash = checksumInit(diff,&dst->data[i],diff->MinMatchSize);
        else
            hash = checksumRoll(diff,&dst->data[i],diff->MinMatchSize);
        re_init = 0;

        if (!stashIntFindInt(diff->FingerPrintTable, hash, &src_idx))
            continue;

        for(idx=src_idx,j=i;j+diff->MinMatchSize<=dst->size && idx<max_src_idx;j+=diff->MinMatchSize,idx++)
        {
            if (diff->rolling_crcs[idx] != checksum(&dst->data[j],diff->MinMatchSize))
                break;
        }
        match_size = j - i;

        if (src->data)
        {
            backwards = i-insert_pos;
            match_size = expandMatch(src,dst,src_idx*diff->MinMatchSize,i,&backwards,0x7fffffff);
            if (match_size < (U32)diff->MinMatchSize)
                continue;
        }
        else
        {
            if (match_size < (U32)diff->MinMatchSize * 3)
                continue;
        }
        insertData(diff,insert_pos,i-insert_pos-backwards);
        copyData(diff,i-backwards,src_idx * diff->MinMatchSize - backwards,match_size);
        i+=match_size-1-backwards;
        insert_pos = i+1;
        re_init = 1;
    }
    insertData(diff,insert_pos,dst->size-insert_pos);
}

static int packDiff(DiffState *diff,U8 **output)
{
    U8            *mem;
    U32            cmdsize;

    // pack diff bytes into one chunk
    cmdsize = diff->commands.size * sizeof(diff->commands.data[0]);
    mem = malloc(cmdsize + diff->copydata.size + sizeof(U32));
    *((U32 *)mem) = diff->commands.size;
    memcpy(mem+4,diff->commands.data,cmdsize);
    memcpy(mem+4+cmdsize,diff->copydata.data,diff->copydata.size);
    *output = mem;
    free(diff->copydata.data);
    free(diff->commands.data);
    return cmdsize + diff->copydata.size + 4;
}

int bindiffCreatePatchFromFingerprints(U32 *crcs,U32 num_crcs,U8 *target,U32 target_len,U8 **output,int minMatchSize)
{
    DiffState    diff = {0};

    // Init diffstate
    diff.dst.data        = target;
    diff.dst.size        = target_len;
    diff.MinMatchSize    = minMatchSize;
    diff.rolling_crcs    = crcs;
    diff.src.size        = num_crcs * minMatchSize;

    hashFingerPrintTable(&diff);
    calcDiff(&diff);
    stashTableDestroy(diff.FingerPrintTable);

    return packDiff(&diff,output);
}

int bindiffCreatePatch(U8 *orig,U32 orig_len,U8 *target,U32 target_len,U8 **output,int minMatchSize)
{
    DiffState    diff = {0};

    if (0)
    {
        int        num_crcs,ret;
        U32        *crcs;

        minMatchSize = 256;
        crcs = bindiffMakeFingerprints(orig,orig_len,minMatchSize,&num_crcs);
        ret = bindiffCreatePatchFromFingerprints(crcs,num_crcs,target,target_len,output,minMatchSize);
        free(crcs);
        return ret;
    }
    // Init diffstate
    diff.src.data = orig;
    diff.src.size = orig_len;
    diff.dst.data = target;
    diff.dst.size = target_len;
    diff.MinMatchSize = minMatchSize;

    // Create a table to hold all the finger prints for the orig file.
    if (minMatchSize)
    {
        diff.rolling_crcs = bindiffMakeFingerprints(diff.src.data,diff.src.size,diff.MinMatchSize,0);
        hashFingerPrintTable(&diff);
        calcDiff(&diff);
        free(diff.rolling_crcs);
        stashTableDestroy(diff.FingerPrintTable);
    }
    else
        insertData(&diff,0,diff.dst.size);

    return packDiff(&diff,output);
}

int bindiffApplyPatch(U8 *src,U8 *patchdata,U8 **target_p)
{
    int            i,cmd_count,data_pos=0,dst_pos=0;
    DiffCommand    *cmds,*cmd;
    U8            *dst,*data;

    cmd_count = *((U32*)patchdata);
    cmds = (DiffCommand*)(patchdata+4);
    data = patchdata+cmd_count*sizeof(DiffCommand)+4;
    for(i=0;i<cmd_count;i++)
        dst_pos += cmds[i].count;
    dst = malloc(dst_pos);
    for(dst_pos=i=0;i<cmd_count;i++)
    {
        cmd = &cmds[i];
        if (cmd->src_pos == 0x80000000)
        {
            memcpy(dst+dst_pos,data+data_pos,cmd->count);
            data_pos += cmd->count;
        }
        else
        {
            if (!src)
            { //We've been told to apply a patch to data we don't have
                free(dst);
                return -1;
            }
            memcpy(dst+dst_pos,src+cmd->src_pos,cmd->count);
        }
        dst_pos += cmd->count;
    }
    *target_p = dst;
    return dst_pos;
}

