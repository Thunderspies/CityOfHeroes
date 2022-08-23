#define MEASURE_NETWORK_TRAFFIC_OPT_OUT
#include "utilitieslib/network/net_packetutil.h"
#include <stdio.h>
#include "utilitieslib/network/net_packet.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/utils.h"

extern int g_bAssertOnBitStreamError;

void pktSendF(Packet *pak, int precision, F32 min, F32 max, F32 f)
{
    int val = (int)((f - min)/(max - min) * ((1 << precision) - 1));
    if (g_bAssertOnBitStreamError) {
        assert(val >=0 && val < (1<< precision));
    }
    pktSendBits(pak, precision, val);
}

F32 pktGetF(Packet *pak, int precision, F32 min, F32 max)
{
    int val = pktGetBits(pak, precision);
    return val * (max - min) / ((1<<precision)-1) + min;
}

bool pktIsFDiff(int precision, F32 min, F32 max, F32 f1, F32 f2)
{
    int val1 = (int)((f1 - min)/(max - min) * ((1 << precision) - 1));
    int val2 = (int)((f2 - min)/(max - min) * ((1 << precision) - 1));
    return val1!=val2;
}

void pktSendIfSetF32(Packet *pak,float f)
{
    pktSendBits(pak, 1, f!=0.0?1:0);
    if (f!=0.0) {
        pktSendF32(pak, f);
    }
}

F32 pktGetIfSetF32(Packet *pak)
{
    if (pktGetBits(pak, 1)) {
        return pktGetF32(pak);
    } else {
        return 0;
    }
}

void pktSendIfSetBitsPack(Packet *pak,int minbits,U32 val)
{
    assert(minbits>1); // Otherwise you gain nothing
    pktSendBits(pak, 1, val?1:0);

    if (val) {
        pktSendBitsPack(pak, minbits, val);
    }
}

U32 pktGetIfSetBitsPack(Packet *pak,int minbits)
{
    if (pktGetBits(pak, 1)) {
        return pktGetBitsPack(pak, minbits);
    } else {
        return 0;
    }
}

void pktSendIfSetBits(Packet *pak,int numbits,U32 val)
{
    assert(numbits>1); // Otherwise you gain nothing!
    if (numbits!=32) {
        val = val & ((1<<numbits)-1);
    }
    pktSendBits(pak, 1, val?1:0);
    if (val) {
        pktSendBits(pak, numbits, val);
    }
}

U32 pktGetIfSetBits(Packet *pak,int numbits)
{
    if (pktGetBits(pak, 1)) {
        return pktGetBits(pak, numbits);
    } else {
        return 0;
    }
}

void pktSendIfSetString(Packet *pak, const char *str)
{
    int sendit = (str && *str);
    pktSendBits(pak, 1, sendit);
    if (sendit) {
        pktSendString(pak, str);
    }
}

const char *pktGetIfSetString(Packet *pak)
{
    if (pktGetBits(pak, 1)) {
        return pktGetString(pak);
    } else {
        return "";
    }
}

void pktAppendToEstr(Packet *pak, char **estr)
{
    int iLen;
    char *temp;
    temp = pktGetStringAndLength(pak,&iLen);
    estrConcatMinimumWidth(estr,temp,iLen);
}

// MAK - 2, 10 & 24 come from looking at the curve of packets sent with
// pktSendBitsPack(1).  I was getting 2 bit numbers 1/3 of the time I saw a 1
// bit number, so I ended up with an initial bin size of 2.
// Feel free to change them if you come up with better numbers
int auto_bins[] = {2, 10, 24, 32};

void pktSendBitsAuto(Packet *pak, U32 val)
{
    int bin, bits = count_bits(val);
    for (bin = 0; bin < 4; bin++)
        if (auto_bins[bin] >= bits) break;
    pktSendBits(pak, 2, bin);
    pktSendBits(pak, auto_bins[bin], val);

    // test this for a bit?
    if (auto_bins[bin] < 32)
    {
        unsigned int bitmask = (unsigned int)(1 << auto_bins[bin]) - (unsigned int)1;
        assert((val & ~bitmask) == 0);
    }
}

U32 pktGetBitsAuto(Packet *pak)
{
    int bin = pktGetBits(pak, 2);
    if(AINRANGE(bin, auto_bins))
        return pktGetBits(pak, auto_bins[bin]);

    if(!pak->stream.errorFlags)
    {
        assert(!g_bAssertOnBitStreamError);
        pak->stream.errorFlags = BSE_TYPEMISMATCH;
    }
    return -1;
}

void pktSendBitsAuto2(Packet *pak, U64 val)
{
    pktSendBitsAuto(pak,((U32*)(&val))[0]);
    pktSendBitsAuto(pak,((U32*)(&val))[1]);
}

U64 pktGetBitsAuto2(Packet *pak)
{
    U64 val;
    ((U32*)(&val))[0] = pktGetBitsAuto(pak);
    ((U32*)(&val))[1] = pktGetBitsAuto(pak);
    return val;
}

void pktSendStringf(Packet *pak, const char *fmt, ...)
{
    VA_START(va, fmt);
    pktSendStringfv(pak, fmt, va);
    VA_END();
}

void pktSendStringfv(Packet *pak, const char *fmt, va_list va)
{
    char *str = estrTemp();
    estrConcatfv(&str, fmt, va);
    pktSendString(pak, str);
    estrDestroy(&str);
}

static int defaultBitsPack(cStashTable ht)
{
    return log2i(stashGetMaxSize(ht));
}
int do_not_assert_on_bad_indexed_string_read=0; // TestClient hack

void pktSendIndexedString(Packet *pak, const char *str, cStashTable htStrings)
{
    if (str)
    {
        int index = 0;
        int inTable = stashFindIndexByKey(htStrings, str, &index);
        
        pktSendBits(pak, 1, 1 );
        pktSendBits(pak, 1, inTable);
        if (pak->hasDebugInfo) {
            if (str == NULL) {
                pktSendString(pak, "(null)");
            } else {
                pktSendString(pak, str);
            }
        }
        if (inTable) {
            pktSendBitsPack(pak, defaultBitsPack(htStrings), index);
        } else {
            pktSendString(pak, str);
        }
    }
    else
    {
        pktSendBits(pak, 1, 0);
    }
}

const char *pktGetIndexedString(Packet *pak, cStashTable htStrings)
{
    if (pktGetBits(pak, 1))
    {
        int index;
        const char *debug_string="(no string)";
        const char *ret;
        int inTable = pktGetBits(pak, 1);
        if (pak->hasDebugInfo) {
            int debug_intable;
            debug_string = allocAddString(pktGetString(pak));
            debug_intable = stashFindIndexByKey(htStrings, debug_string, &index) || (strcmp(debug_string, "(null)")==0);
            assert(!g_bAssertOnBitStreamError || do_not_assert_on_bad_indexed_string_read || inTable == debug_intable);
        }
        if (inTable) {
            START_BIT_COUNT(pak, "stringIndex");
            index = pktGetBitsPack(pak, defaultBitsPack(htStrings));
            if (!stashFindKeyByIndex(htStrings, index, &ret))
                ret = NULL;
            STOP_BIT_COUNT(pak);

            if (pak->hasDebugInfo) {
                if (ret==NULL) {
                    assert(!g_bAssertOnBitStreamError || do_not_assert_on_bad_indexed_string_read || strcmp(debug_string, "(null)")==0);
                } else {                
                    assert(!g_bAssertOnBitStreamError || do_not_assert_on_bad_indexed_string_read || stricmp(ret, debug_string)==0);
                }
            }
            return ret;
        } else {
            START_BIT_COUNT(pak, "stringText");
            ret = (const char*)allocAddString(pktGetString(pak));
            STOP_BIT_COUNT(pak);
            return ret;
        }
    }
    else
    {
        return NULL;
    }
}

static int defaultBitsPackGeneric(cStashTable ht)
{
    return log2i(stashGetMaxSize(ht));
}

void pktSendIndexedGeneric(Packet *pak, int val, cStashTable ht)
{
    U32 index=0;
    int inTable = (val && ht && stashIntFindIndexByKey(ht, val, &index));

    pktSendBits(pak, 1, inTable);
    if (pak->hasDebugInfo) {
        pktSendBitsPack(pak, 1, val);
    }
    if (inTable) {
        pktSendBitsPack(pak, defaultBitsPackGeneric(ht), index);
    } else {
        pktSendBits(pak, 32, val);
    }
}

int pktGetIndexedGeneric(Packet *pak, cStashTable ht)
{
    U32 index;
    int inTable;
    int val;
    int debug_val=0;

    inTable = pktGetBits(pak, 1);
    if (pak->hasDebugInfo) {
        int debug_intable;
        debug_val = pktGetBitsPack(pak, 1);
        debug_intable = (debug_val && stashIntFindPointer(ht, debug_val, NULL) );
        assert(!g_bAssertOnBitStreamError || do_not_assert_on_bad_indexed_string_read || inTable == debug_intable);
    }
    if (inTable) {
        index = pktGetBitsPack(pak, defaultBitsPackGeneric(ht));
        if (!stashIntFindKeyByIndex(ht, index, &val) )
            val = 0;
        if (pak->hasDebugInfo) {
            assert(!g_bAssertOnBitStreamError || do_not_assert_on_bad_indexed_string_read || debug_val==val);
        }
        return val;
    } else {
        return pktGetBits(pak, 32);
    }
}
