#ifndef NET_PACKET_COMMON_H
#define NET_PACKET_COMMON_H

#include "../utils/Color.h"

/****************************************************************************************************
 * Basic Packet operations                                                                            *
 ****************************************************************************************************/
void pktSetDebugInfo();
void pktSetDebugInfoTo(int on);
int pktGetDebugInfo();

void pktSetOrdered(Packet* pak, bool ordered);
bool pktGetOrdered(Packet* pak);void pktSetByteAlignment(Packet* pak, int align);
int pktGetByteAlignment(Packet* pak);

void pktSetCompression(Packet* pak, int compression);
int pktGetCompression(Packet* pak);

void pktSendBits(Packet *pak,int numbits,U32 val);
U32 pktGetBits(Packet *pak,int numbits);

void pktSendBitsArray(Packet *pak,int numbits,const void *data);
void pktGetBitsArray(Packet *pak,int numbits,void *data);

void pktSendBitsPack(Packet *pak,int minbits,U32 val);
int pktGetLengthBitsPack(Packet *pak,int minbits,U32 val);
U32 pktGetBitsPack(Packet *pak,int minbits);

void pktSendString(Packet *pak,const char *str);
void pktSendStringAligned(Packet *pak, const char *str);
char *pktGetStringAndLength(Packet *pak, int *pOutLen);
char* pktGetStringTemp(Packet *pak); // byte-aligned only, good until the packet is destroyed

static INLINEDBG char *pktGetString(Packet *pak)
{
    int dummy;
    return pktGetStringAndLength(pak, &dummy);
}

void pktSendF32(Packet *pak,float f);
F32 pktGetF32(Packet *pak);

void pktAlignBitsArray(Packet *pak);

void pktAppend(Packet* pak, Packet* data);
void pktAppendRemainder(Packet* pak, Packet* data);
void pktMakeCompatible(Packet *pak, Packet const *pak_in);

U32 pktGetSize(Packet *pak);
U32 pktEnd(Packet *pak);
void pktReset(Packet *pak);

// Send/Get If Set (sends only 1 bit if the value is 0)
void pktSendIfSetBits(Packet *pak,int numbits,U32 val);
U32 pktGetIfSetBits(Packet *pak,int numbits);

void pktSendIfSetBitsPack(Packet *pak,int minbits,U32 val);
U32 pktGetIfSetBitsPack(Packet *pak,int minbits);

void pktSendIfSetF32(Packet *pak,float f);
F32 pktGetIfSetF32(Packet *pak);

// counts bits in val & sends in a couple sizes
void pktSendBitsAuto(Packet *pak, U32 val);
U32 pktGetBitsAuto(Packet *pak);

// pktSendBitsAuto twice
void pktSendBitsAuto2(Packet *pak, U64 val);
U64 pktGetBitsAuto2(Packet *pak);

// do a get/send bits 1, with proper casting
static INLINEDBG int pktSendBit(Packet *pak, int val) { pktSendBits(pak, 1, val); return val; }
#define pktSendBool(pak, val) pktSendBit(pak, (val)?1:0)
#define pktGetBool(pak) (pktGetBits(pak, 1)!=0)

void pktSendZippedAlready(Packet *pak,int numbytes,int zipbytes,void *zipdata);
void pktSendZipped(Packet *pak,int numbytes,void *data);
U8 *pktGetZipped(Packet *pak,U32 *numbytes_p);
U8 *pktGetZippedInfo(Packet *pak,U32 *zipbytes_p,U32 *rawbytes_p);
void pktSendGetZipped(Packet *pak_out, Packet *pak_in);

void pktSendF(Packet *pak, int precision, F32 min, F32 max, F32 f);
F32 pktGetF(Packet *pak, int precision, F32 min, F32 max);

void pktSendIfSetString(Packet *pak, const char *str);
const char *pktGetIfSetString(Packet *pak);

void pktAppendToEstr(Packet *pak, char **estr);

void pktSendStringf(Packet *pak, FORMAT fmt, ...);
void pktSendStringfv(Packet *pak, const char *fmt, va_list va);

void pktSendIndexedString(Packet *pak, const char *str, cStashTable htStrings);
const char *pktGetIndexedString(Packet *pak, cStashTable htStrings);

void pktSendIndexedGeneric(Packet *pak, int val, cStashTable ht);
int pktGetIndexedGeneric(Packet *pak, cStashTable ht);

void pktSendF32Comp(Packet *pak, F32 val);
F32 pktGetF32Comp(Packet *pak);
void pktSendF32Deg(Packet *pak, F32 val);
F32 pktGetF32Deg(Packet *pak);

void sendMat4(Packet *pak, Mat4 mat);
void getMat4(Packet *pak, Mat4 mat);

/****************************************************************************************************
 * Passthrough shorthand                                                                            *
 ****************************************************************************************************/
static INLINEDBG int pktSendGetBitsAuto(Packet *pak_out, Packet *pak_in) { int i = pktGetBitsAuto(pak_in); pktSendBitsAuto(pak_out,i); return i; }
static INLINEDBG long long pktSendGetBitsAuto2(Packet *pak_out, Packet *pak_in) { long long i = pktGetBitsAuto2(pak_in); pktSendBitsAuto2(pak_out,i); return i; }
static INLINEDBG const char* pktSendGetStringAligned(Packet *pak_out, Packet *pak_in) { const char *s = pktGetStringTemp(pak_in); pktSendStringAligned(pak_out,s); return s; }
static INLINEDBG int pktSendGetBitsPak(Packet *pak_out, Packet *pak_in, int minbits) { U32 i = pktGetBitsPack(pak_in,minbits); pktSendBitsPack(pak_out,minbits, i); return i; }
static INLINEDBG void pktSendGetBitsArray(Packet *pak_out,Packet *pak_in, int numbits,void *data) { pktGetBitsArray(pak_in,numbits,data);    pktSendBitsArray(pak_out,numbits,(const void *)data);}
static INLINEDBG F32 pktSendGetF32(Packet *pak_out, Packet *pak_in){float f = pktGetF32(pak_in); pktSendF32(pak_out,f);return f;}
static INLINEDBG const char *pktSendGetStringNLength(Packet *pak_out, Packet *pak_in){    const char *temp = pktGetString(pak_in); pktSendString(pak_out,temp); return temp;}
static INLINEDBG U32 pktSendGetBits(Packet *pak_out,Packet *pak_in, int numbits){U32 temp = pktGetBits(pak_in, numbits); pktSendBits(pak_out,numbits,temp); return temp;}
#define pktSendGetString(pak_out,pak_in)                pktSendString(pak_out,pktGetString(pak_in))
#define pktSendGetBool(pak_out,pak_in)                    pktSendBool(pak_out,pktGetBool(pak_in))

/****************************************************************************************************
* Basic packet operations for advanced data types                                                    *
****************************************************************************************************/
static INLINEDBG void pktSendColor(Packet *pak, Color clr)
{
    pktSendBits(pak, 8, clr.r);
    pktSendBits(pak, 8, clr.g);
    pktSendBits(pak, 8, clr.b);
    pktSendBits(pak, 8, clr.a);
}

static INLINEDBG Color pktGetColor(Packet *pak)
{
    Color clr;
    clr.r = pktGetBits(pak, 8);
    clr.g = pktGetBits(pak, 8);
    clr.b = pktGetBits(pak, 8);
    clr.a = pktGetBits(pak, 8);
    return clr;
}

static INLINEDBG void pktSendVec3(Packet *pak, const Vec3 vec)
{
    pktSendF32(pak, vec[0]);
    pktSendF32(pak, vec[1]);
    pktSendF32(pak, vec[2]);
}

static INLINEDBG void pktGetVec3(Packet *pak, Vec3 vec)
{
    vec[0] = pktGetF32(pak);
    vec[1] = pktGetF32(pak);
    vec[2] = pktGetF32(pak);
}

static INLINEDBG void pktSendMat4(Packet *pak, const Mat4 mat)
{
    pktSendVec3(pak, mat[0]);
    pktSendVec3(pak, mat[1]);
    pktSendVec3(pak, mat[2]);
    pktSendVec3(pak, mat[3]);
}

static INLINEDBG void pktGetMat4(Packet *pak, Mat4 mat)
{
    pktGetVec3(pak, mat[0]);
    pktGetVec3(pak, mat[1]);
    pktGetVec3(pak, mat[2]);
    pktGetVec3(pak, mat[3]);
}

static INLINEDBG void pktSendVec4(Packet *pak, const Vec4 vec)
{
    pktSendF32(pak, vec[0]);
    pktSendF32(pak, vec[1]);
    pktSendF32(pak, vec[2]);
    pktSendF32(pak, vec[3]);
}

static INLINEDBG void pktGetVec4(Packet *pak, Vec4 vec)
{
    vec[0] = pktGetF32(pak);
    vec[1] = pktGetF32(pak);
    vec[2] = pktGetF32(pak);
    vec[3] = pktGetF32(pak);
}

// Uncomment to enable network profiling!
#define MEASURE_NETWORK_TRAFFIC_ON

#if defined(MEASURE_NETWORK_TRAFFIC_ON) && !defined(MEASURE_NETWORK_TRAFFIC_OPT_OUT)
#define pktSendBits(pak,numbits,val) profile_pktSendBits(__FILE__, __LINE__, pak, numbits, val)
#define pktSendBitsPack(pak,minbits,val) profile_pktSendBitsPack(__FILE__, __LINE__, pak, minbits, val)
#define pktSendIfSetBits(pak,numbits,val) profile_pktSendIfSetBits(__FILE__, __LINE__, pak, numbits, val)
#define pktSendIfSetBitsPack(pak,minbits,val) profile_pktSendIfSetBitsPack(__FILE__, __LINE__, pak, minbits, val)
#define pktSendString(pak,str) profile_pktSendString(__FILE__, __LINE__, pak,str)
#define pktSendF32(pak,f) profile_pktSendF32(__FILE__, __LINE__, pak, f)
#define pktSendIfSetBits(pak, numbits, val) profile_pktSendIfSetBits(__FILE__, __LINE__, pak, numbits, val)
#define pktSetCompression(pak, compression) profile_pktSetCompression(__FILE__, __LINE__, pak, compression)
#define pktSendBitsArray(pak,numbits,data) profile_pktSendBitsArray(__FILE__, __LINE__, pak, numbits, data)
#define pktSendStringAligned(pak, str) profile_pktSendStringAligned(__FILE__, __LINE__, pak, str)
#define pktAlignBitsArray(pak) profile_pktAlignBitsArray(__FILE__, __LINE__, pak)
#define pktAppend(pak, data) profile_pktAppend(__FILE__, __LINE__, pak, data)
#define pktAppendRemainder(pak, data) profile_pktAppendRemainder(__FILE__, __LINE__, pak, data)
#define pktSendIfSetF32(pak, f) profile_pktSendIfSetF32(__FILE__, __LINE__, pak, f)
#define pktSendBitsAuto(pak, val) profile_pktSendBitsAuto(__FILE__, __LINE__, pak, val)
#define pktSendBitsAuto2(pak, val) profile_pktSendBitsAuto2(__FILE__, __LINE__, pak, val)
#define pktSendBit(pak, val) profile_pktSendBit(__FILE__, __LINE__, pak, val)
#define pktSendZippedAlready(pak,numbytes,zipbytes,zipdata) profile_pktSendZippedAlready(__FILE__, __LINE__, pak,numbytes,zipbytes,zipdata)
#define pktSendZipped(pak,numbytes,data) profile_pktSendZipped(__FILE__, __LINE__, pak,numbytes,data)
#define pktSendGetZipped(pak_out, pak_in) profile_pktSendGetZipped(__FILE__, __LINE__, pak_out, pak_in)
#define pktSendGetBitsAuto(pak_out, pak_in) profile_pktSendGetBitsAuto(__FILE__, __LINE__, pak_out, pak_in)
#define pktSendGetBitsAuto2(pak_out, pak_in) profile_pktSendGetBitsAuto2(__FILE__, __LINE__, pak_out, pak_in)
#define pktSendGetStringAligned(pak_out, pak_in) profile_pktSendGetStringAligned(__FILE__, __LINE__, pak_out, pak_in)
#define pktSendGetBitsPak(pak_out, pak_in, minbits) profile_pktSendGetBitsPak(__FILE__, __LINE__, pak_out, pak_in, minbits)
#define pktSendGetBitsArray(pak_out,pak_in, numbits,data) profile_pktSendGetBitsArray(__FILE__, __LINE__, pak_out,pak_in, numbits,data)
#define pktSendGetF32(pak_out, pak_in) profile_pktSendGetF32(__FILE__, __LINE__, pak_out, pak_in)
#define pktSendGetStringNLength(pak_out, pak_in) profile_pktSendGetStringNLength(__FILE__, __LINE__, pak_out, pak_in)
#define pktSendGetBits(pak_out, pak_in, numbits) profile_pktSendGetBits(__FILE__, __LINE__, pak_out, pak_in, numbits)
#define pktSendColor(pak, clr) profile_pktSendColor(__FILE__, __LINE__, pak, clr)
#define pktSendVec3(pak, vec) profile_pktSendVec3(__FILE__, __LINE__, pak, vec)
#define pktSendMat4(pak, mat) profile_pktSendMat4(__FILE__, __LINE__, pak, mat)
#define pktSendVec4(pak, vec) profile_pktSendVec4(__FILE__, __LINE__, pak, vec)
#define pktSendF(pak, precision, min, max, f) profile_pktSendF(__FILE__, __LINE__, pak, precision, min, max, f)
#define pktSendIfSetString(pak, str) profile_pktSendIfSetString(__FILE__, __LINE__, pak, str)
#define pktSendStringf(pak, ...) \
	profile_pktSendStringf(__FILE__, __LINE__, pak, __VA_ARGS__)
#define pktSendStringfv(pak, fmt, va) profile_pktSendStringfv(__FILE__, __LINE__, pak, fmt, va)
#define pktSendIndexedString(pak, str, htStrings) profile_pktSendIndexedString(__FILE__, __LINE__, pak, str, htStrings)
#define pktSendIndexedGeneric(pak, val, ht) profile_pktSendIndexedGeneric(__FILE__, __LINE__, pak, val, ht)
#define pktSendF32Comp(pak, val) profile_pktSendF32Comp(__FILE__, __LINE__, pak, val)
#define pktSendF32Deg(pak, val) profile_pktSendF32Deg(__FILE__, __LINE__, pak, val)
#define sendMat4(pak, mat) profile_sendMat4(__FILE__, __LINE__, pak, mat)
#endif

void netprofile_dumpStats(void);
void netprofile_resetStats(void);
void netprofile_setTimer(unsigned int timeToCollect);
void netprofile_decrementTimer(void);

void profile_pktSendIfSetBits(char const* file, int line, Packet *pak,int numbits,int val);
void profile_pktSendIfSetBitsPack(char const* file, int line, Packet *pak,int minbits,int val);
void profile_pktSendBits(char const* file, int line, Packet *pak,int numbits,U32 val);
void profile_pktSendBitsPack(char const* file, int line, Packet *pak,int minbits,U32 val);
void profile_pktSendString(char const* file, int line, Packet *pak,const char *str);
void profile_pktSendF32(char const* file, int line, Packet *pak, F32 f);
void profile_pktSetCompression(char const* file, int line, Packet* pak, int compression);
void profile_pktSendBitsArray(char const* file, int line, Packet *pak,int numbits,const void *data);
void profile_pktSendStringAligned(char const* file, int line, Packet *pak, const char *str);
void profile_pktAlignBitsArray(char const* file, int line, Packet *pak);
void profile_pktAppend(char const* file, int line, Packet* pak, Packet* data);
void profile_pktAppendRemainder(char const* file, int line, Packet* pak, Packet* data);
void profile_pktSendIfSetF32(char const* file, int line, Packet *pak,float f);
void profile_pktSendBitsAuto(char const* file, int line, Packet *pak, U32 val);
void profile_pktSendBitsAuto2(char const* file, int line, Packet *pak, U64 val);
int profile_pktSendBit(char const* file, int line, Packet *pak, int val);
void profile_pktSendZippedAlready(char const* file, int line, Packet *pak,int numbytes,int zipbytes,void *zipdata);
void profile_pktSendZipped(char const* file, int line, Packet *pak,int numbytes,void *data);
void profile_pktSendGetZipped(char const* file, int line, Packet *pak_out, Packet *pak_in);
int profile_pktSendGetBitsAuto(char const* file, int line, Packet *pak_out, Packet *pak_in);
long long profile_pktSendGetBitsAuto2(char const* file, int line, Packet *pak_out, Packet *pak_in);
const char *profile_pktSendGetStringAligned(char const* file, int line, Packet *pak_out, Packet *pak_in);
int profile_pktSendGetBitsPak(char const* file, int line, Packet *pak_out, Packet *pak_in, int minbits);
void profile_pktSendGetBitsArray(char const* file, int line, Packet *pak_out,Packet *pak_in, int numbits,void *data);
F32 profile_pktSendGetF32(char const* file, int line, Packet *pak_out, Packet *pak_in);
const char *profile_pktSendGetStringNLength(char const* file, int line, Packet *pak_out, Packet *pak_in);
U32 profile_pktSendGetBits(char const* file, int line, Packet *pak_out,Packet *pak_in, int numbits);
void profile_pktSendColor(char const* file, int line, Packet *pak, Color clr);
void profile_pktSendVec3(char const* file, int line, Packet *pak, const Vec3 vec);
void profile_pktSendMat4(char const* file, int line, Packet *pak, const Mat4 mat);
void profile_pktSendVec4(char const* file, int line, Packet *pak, const Vec4 vec);
void profile_pktSendF(char const* file, int line, Packet *pak, int precision, F32 min, F32 max, F32 f);
void profile_pktSendIfSetString(char const* file, int line, Packet *pak, const char *str);
void profile_pktSendStringf(char const* file, int line, Packet *pak, FORMAT fmt, ...);
void profile_pktSendStringfv(char const* file, int line, Packet *pak, const char *fmt, va_list va);
void profile_pktSendIndexedString(char const* file, int line, Packet *pak, const char *str, cStashTable htStrings);
void profile_pktSendIndexedGeneric(char const* file, int line, Packet *pak, int val, cStashTable ht);
void profile_pktSendF32Comp(char const* file, int line, Packet *pak, int val);
void profile_pktSendF32Deg(char const* file, int line, Packet *pak, int val);
void profile_sendMat4(char const* file, int line, Packet *pak, Mat4 mat);

#endif
