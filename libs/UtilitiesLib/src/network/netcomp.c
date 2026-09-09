#define MEASURE_NETWORK_TRAFFIC_OPT_OUT
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/network/netcomp.h"
#include "utilitieslib/network/netio.h"
#include <zlib.h>
#include <stdio.h>


U32 packQuatElem(F32 qelem,int numbits)
{
    assert( fabsf(qelem) <= 1.0f );
    // quat elements are non-linear in their representation, so make them linear for compression
    qelem = asinf(qelem); // range is now -pi/2 to pi/2
    qelem = (F32)((1 << numbits) * (qelem + (HALFPI)) / PI);
    if (qelem < 0)
        qelem = 0;
    if (qelem > (1 << numbits)-1)
        qelem = (F32)((1 << numbits)-1);
    return qtrunc(qelem);
}

F32 unpackQuatElem(U32 val,int numbits)
{
    F32        qelem;

    qelem = (F32)(((val * PI) / (1 << numbits)) - HALFPI);
    if (qelem < -HALFPI )
        qelem = -HALFPI;
    if (qelem > (HALFPI-0.00001f))
        qelem = HALFPI;

    if ( fabsf(qelem) < 0.00001f )
        qelem = 0.0f;

    return sinf(qelem);
}

U32 packQuatElemQuarterPi(F32 qelem,int numbits)
{
    assert( fabsf(qelem) <= 0.7072f );
    // quat elements are non-linear in their representation, so make them linear for compression
    qelem = asinf(qelem); // range is now -pi/4 to pi/4
    qelem = (F32)((1 << numbits) * (qelem + (QUARTERPI)) / HALFPI);
    if (qelem < 0)
        qelem = 0;
    if (qelem > (1 << numbits)-1)
        qelem = (F32)((1 << numbits)-1);
    return round32(qelem);
}

F32 unpackQuatElemQuarterPi(U32 val,int numbits)
{
    F32        qelem;


    qelem = (F32)val;
    qelem = (F32)(((qelem * HALFPI) / (1 << numbits)) - QUARTERPI);
    if (qelem < -QUARTERPI )
        qelem = -QUARTERPI;
    if (qelem > (QUARTERPI-0.00001f))
        qelem = QUARTERPI;

    if ( fabsf(qelem) < 0.00001f )
        qelem = 0.0f;

    return sinf(qelem);
}

U32 packEuler(F32 ang,int numbits)
{
    ang = fixAngle(ang);
    ang = (F32)((1 << numbits) * (ang + PI) / (2*PI));
    if (ang < 0)
        ang = 0;
    if (ang > (1 << numbits)-1)
        ang = (F32)((1 << numbits)-1);
    return qtrunc(ang);
}

F32 unpackEuler(U32 val,int numbits)
{
F32        ang;

    ang = (F32)(((val * 2*PI) / (1 << numbits)) - PI);
    return ang;
}

U32 packPos(F32 pos)
{
int        t;

    t = (int)(pos * POS_SCALE);
    t += (1 << (POS_BITS-1));
    return qtrunc((F32)t);
}

F32 unpackPos(U32 ut)
{
int        t;

    t = ut;
    t -= (1 << (POS_BITS-1));
    return t / POS_SCALE;

}

F32 quantizePos(F32 pos)
{
    return unpackPos(packPos(pos));
}

#ifdef NEARZERO
#undef NEARZERO
#endif
#define NEARZERO 1e-16

void unitZeroMat(Mat4 mat,int *unit,int *zero)
{
    if (nearSameVec3Tol(mat[0],unitmat[0],NEARZERO)
        && nearSameVec3Tol(mat[1],unitmat[1],NEARZERO)
        && nearSameVec3Tol(mat[2],unitmat[2],NEARZERO))
        *unit = 1;
    else
        *unit = 0;

    if (nearSameVec3Tol(mat[3],zerovec3,NEARZERO))
        *zero = 1;
    else
        *zero = 0;
}

void unpackMat(PackMat *pm,Mat4 mat)
{
    copyMat4(unitmat,mat);
    if (pm->has_pyr)
        createMat3YPR(mat,pm->pyr);
    if (pm->has_scale)
        scaleMat3Vec3(mat,pm->scale);
    if (pm->has_pos)
        copyVec3(pm->pos,mat[3]);
}

// returns 1 if it was able to pack with no float error
int packMat(Mat4 mat_in,PackMat *pm)
{
    int        unit,zero;
    Mat4    mat,test_mat;

    pm->has_pos = pm->has_pyr = pm->has_scale = 0;
    copyMat4(mat_in,mat);
    unitZeroMat(mat,&unit,&zero);
    if (!unit)
    {
        extractScale(mat,pm->scale);
        if (!nearSameVec3(pm->scale,onevec3))
            pm->has_scale = 1;
        getMat3YPR(mat,pm->pyr);
        pm->has_pyr = 1;
    }
    if (!zero)
    {
        copyVec3(mat[3],pm->pos);
        pm->has_pos = 1;
    }
    unpackMat(pm,test_mat);
    return memcmp(test_mat,mat,sizeof(mat))==0;
        
}


#define INT_SCALE 2
void pktSendF32Comp(Packet *pak,F32 val)
{
int        ival;

    ival = (int)(val*INT_SCALE);
    if (val*INT_SCALE == ival)
    {
        pktSendBits(pak,1,1);
        pktSendBits(pak,1,(ival >> 31) & 1);
        pktSendBitsPack(pak,5,ABS(ival));
    }
    else
    {
        pktSendBits(pak,1,0);
        pktSendF32(pak,val);
    }
}

F32 pktGetF32Comp(Packet *pak)
{
int        is_int,negate;
F32        val;

    is_int = pktGetBits(pak,1);
    if (is_int)
    {
        negate = pktGetBits(pak,1);
        val = (1.f/INT_SCALE) * pktGetBitsPack(pak,5);
        if (negate)
            val = -val;
    }
    else
        val = pktGetF32(pak);
    return val;
}

void pktSendF32Deg(Packet *pak,F32 val)
{
    pktSendF32Comp(pak,val);
}

F32 pktGetF32Deg(Packet *pak)
{
F32        rad;

    rad = pktGetF32Comp(pak);
    return rad;
}

void sendMat4(Packet *pak,Mat4 mat)
{
    int        i;
    PackMat    pm;

    if (packMat(mat,&pm))
    {
        pktSendBits(pak,1,1); // packed
        pktSendBits(pak,1,pm.has_pos);
        pktSendBits(pak,1,pm.has_pyr);
        pktSendBits(pak,1,pm.has_scale);
        if (pm.has_pos)
        {
            for(i=0;i<3;i++)
                pktSendF32Comp(pak,pm.pos[i]);
        }
        if (pm.has_pyr)
        {
            for(i=0;i<3;i++)
                pktSendF32Deg(pak,pm.pyr[i]);
        }
        if (pm.has_scale)
        {
            for(i=0;i<3;i++)
                pktSendF32(pak,pm.scale[i]);
        }
    }
    else
    {
        pktSendBits(pak,1,0); // unpacked
        pktSendMat4(pak,mat);
    }
}

void getMat4(Packet *pak,Mat4 mat)
{
    PackMat    pm;
    int        i,packed;

    packed        = pktGetBits(pak,1);
    if (packed)
    {
        pm.has_pos    = pktGetBits(pak,1);
        pm.has_pyr    = pktGetBits(pak,1);
        pm.has_scale= pktGetBits(pak,1);
        if (pm.has_pos)
        {
            for(i=0;i<3;i++)
                pm.pos[i] = pktGetF32Comp(pak);
        }
        if (pm.has_pyr)
        {
            for(i=0;i<3;i++)
                pm.pyr[i] = pktGetF32Deg(pak);
        }
        if (pm.has_scale)
        {
            for(i=0;i<3;i++)
                pm.scale[i] = pktGetF32(pak);
        }
        unpackMat(&pm,mat);
    }
    else
    {
        pktGetMat4(pak,mat);
    }
}

void pktSendZippedAlready(Packet *pak,int numbytes,int zipbytes,void *zipdata)
{
    pktSendBitsPack(pak,1,zipbytes);
    pktSendBitsPack(pak,1,numbytes);
    pktSendBitsArray(pak,zipbytes*8,zipdata);
}

void pktSendZipped(Packet *pak,int numbytes,void *data)
{
    U8        *zip_data;
	uLongf zip_size;
    int        ret;

    zip_size = (U32)(numbytes*1.0125+12); // 1% + 12 bigger, so says the zlib docs
    zip_data = malloc(zip_size);
    ret = compress2(zip_data,&zip_size,data,numbytes,5);
    assert(ret == Z_OK);
    pktSendZippedAlready(pak,numbytes,zip_size,zip_data);
    free(zip_data);
}

U8 *pktGetZippedInfo(Packet *pak,U32 *zipbytes_p,U32 *rawbytes_p)
{
    U32        zip_size,numbytes;
    U8        *zip_data;

    zip_size = pktGetBitsPack(pak,1);
    numbytes = pktGetBitsPack(pak,1);

    zip_data = malloc(zip_size);
    pktGetBitsArray(pak,zip_size*8,zip_data);
    if (zipbytes_p)
        *zipbytes_p = zip_size;
    if (rawbytes_p)
        *rawbytes_p = numbytes;
    return zip_data;
}

U8 *pktGetZipped(Packet *pak,U32 *numbytes_p)
{
    U32        zip_size,numbytes;
    U8        *zip_data,*data;

    zip_data = pktGetZippedInfo(pak,&zip_size,&numbytes);
    data = malloc(numbytes+1);
    data[numbytes] = 0;
	uLongf output_size = numbytes;
	uncompress(data, &output_size, zip_data, zip_size);
	numbytes = output_size;
    free(zip_data);
    if (numbytes_p)
        *numbytes_p = numbytes;
    return data;
}

void pktSendGetZipped(Packet *pak_out, Packet *pak_in)
{
    U32 zsize, rsize;
    U8 *data = pktGetZippedInfo(pak_in, &zsize, &rsize);
    pktSendZippedAlready(pak_out, rsize, zsize, data);
    free(data);
}
