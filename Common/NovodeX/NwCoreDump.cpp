#if !BEACONIZER
// Parse PhysX allocator methods before CRT debug macros redefine malloc/free.
#include <NxPhysics.h>
#endif
#include "NovodeX/NwWrapper.h"
#if NOVODEX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

//#include "NxVersionNumber.h"
#include <NxPMap.h>
#include <NxSceneStats.h>
#include "NxCooking.h"
#include <NxStream.h>
#include <NxCCDSkeleton.h>

#undef fopen

namespace NX_CORE_DUMP
{

#if defined(_WIN32) || defined(_XBOX) || defined(__CELLOS_LV2__)
#define    CORE_DUMP_ENABLE 1
#else
#define    CORE_DUMP_ENABLE 0
#endif

NX_INLINE NxU16 coredump_flip(const NxU16* v)
{
    const NxU8* b = (const NxU8*)v;
    NxU16 f;
    NxU8* bf = (NxU8*)&f;
    bf[0] = b[1];
    bf[1] = b[0];
    return f;
}

NX_INLINE NxU32 coredump_flip(const NxU32* v)
{
    const NxU8* b = (const NxU8*)v;
    NxU32 f;
    NxU8* bf = (NxU8*)&f;
    bf[0] = b[3];
    bf[1] = b[2];
    bf[2] = b[1];
    bf[3] = b[0];
    return f;
}


class CoreDump
{
public:
    CoreDump(void);

    // save    the    contents of    the    state of the physics SDK in    text or    binary.
    bool coreDump( NxPhysicsSDK *sdk, const char *fname, bool binary, bool saveoutCookedData );

    // return the munged core dump version relative to this SDK
    static unsigned int getCoreVersion( NxPhysicsSDK *sdk );

private:

#if    CORE_DUMP_ENABLE
    void Print(const char *fmt,...);
    void WriteBool(    const char *desc, NX_BOOL yesNo );
    void WriteChar( const char *desc, char c, bool descAsInt = false );
    void WriteInt( const char *desc, int n );
    void WriteLong(    const char *desc, long l );
    void WriteShort( const char    *desc, short s );
    void WriteUnsigned(    const char *desc, unsigned u );
    void WriteUnsignedLong(    const char *desc, unsigned long    ul );
    void WriteUnsignedShort( const char    *desc, unsigned    short us );
    void WriteFloat( const char    *desc, float f );
    void WriteFlag(    const char *desc, unsigned f );
    void WriteString( const    char *info );
    void WriteParam( NxParameter p,    NxReal v );
    void Write(    const char *desc, NxU32    v );
    void Write(    const char *desc, NxScene *scene );
    void WriteSceneDesc( NxScene *scene    );
    void Write(    const char *desc, NxActor *a );
    void Write(    const char *desc, NxPairFlag *a    );
    void Write(    const char *desc, NxMaterial *a,bool hardwareScene    );
    void Write(    const char *desc, NxJoint *j );
    void Write(    const char *desc, NxSpringDesc *spring );
    void Write(    const char *desc, const    NxMat34    &p );
    void Write(    const char *desc, const    NxBodyDesc *b );
    void PrintFlag(    unsigned int state,    const char *desc, unsigned int flag    );
    void Write(    const char *desc, const    NxVec3 &v );
    void Write(    const char *desc, const    NxQuat &q );
    void Write(    const char *desc, const    NxShape    *shape );
    void Write(    const char *desc, const    NxShapeDesc    &d,const NxShape *s    );
    void Write(    const char *desc, const    NxTriangleMesh *mesh );
    void Write(    const char *desc, const    NxConvexMesh *mesh );
#if NX_SDK_VERSION_NUMBER >= 230
    void Write(    const char *desc, NxCCDSkeleton *skeleton    );
    void Write(    const char *desc, NxTireFunctionDesc *func );
#endif
#if NX_SDK_VERSION_NUMBER >= 240
    void Write(    const char *desc, const    NxHeightField *mesh );
#endif
    void Write(    const char *desc, const    NxJointLimitSoftDesc &l    );
    void Write(    const char *desc, const    NxJointLimitSoftPairDesc &l    );
    void Write(    const char *desc, const    NxJointDriveDesc &d    );
    void Write(    const char *desc, NxScene &scene, NxEffector &e );
    void Write(    NxJointDesc    *d,    NxJoint    *j );
    void Write(    NxPrismaticJointDesc *desc,    NxJoint    *j );
    void Write(    NxRevoluteJointDesc    *desc, NxJoint *j );
    void Write(    NxCylindricalJointDesc *desc, NxJoint *j );
    void Write(    NxSphericalJointDesc *desc,    NxJoint    *j );
    void Write(    NxPointOnLineJointDesc *desc, NxJoint *j );
    void Write(    NxPointInPlaneJointDesc    *desc, NxJoint *j );
    void Write(    NxDistanceJointDesc    *desc, NxJoint *j );
    void Write(    NxPulleyJointDesc *desc, NxJoint *j    );
    void Write(    NxFixedJointDesc *desc,    NxJoint    *j );
    void Write(    NxD6JointDesc *desc, NxJoint *j    );
#if NX_SDK_VERSION_NUMBER >= 230
    bool AddSkeleton(NxCCDSkeleton *skeleton);
#endif

    bool AddConvex(NxConvexMesh    *mesh);
    bool AddTriangleMesh(NxTriangleMesh    *mesh);

#if NX_SDK_VERSION_NUMBER >= 230
    int    GetSkeletonIndex(const NxCCDSkeleton *skeleton); //    look it    up
#endif

    int    GetConvexIndex(const NxConvexMesh *mesh);
    int    GetTriangleMeshIndex(const NxTriangleMesh *mesh);
    int    GetActorIndex(const    NxActor    *a)    const;
    int    GetShapeIndex(const NxActor *a,const NxShape *s) const;

#if NX_SDK_VERSION_NUMBER >= 240
    bool AddHeightField( NxHeightField *hf );
    int GetHeightFieldIndex( const NxHeightField *hf );
#endif

  // binary    writing
    void WriteChar_b( char val );
    void WriteInt_b( int val );
    void WriteLong_b( long val );
    void WriteShort_b( short val );
    void WriteUnsigned_b( unsigned val );
    void WriteUnsignedLong_b( unsigned long    val    );
    void WriteUnsignedShort_b( unsigned    short val );
    void WriteFloat_b( float val );
    void WriteDouble_b(    double val );
    void WriteString_b(    const char *val    );
    void WriteFlag_b( unsigned flag    );
    void WriteMatrix_b(    const NxMat34 &matrix );
    void WriteVec3_b( const    NxVec3 &v );
    void WriteBodyDesc_b( const    NxBodyDesc *b );

    void SaveUserData(NxScene *scene);
    void RestoreUserData(NxScene *scene);

    FILE *                        mFph;
    bool                        mBinary;
    bool                        mSaveCookedData;
    int                            mIndent;
    NxArray< NxTriangleMesh    * >    mTriangleMeshes;
    NxArray< NxConvexMesh    * >    mConvexMeshes;
#if NX_SDK_VERSION_NUMBER >= 230
    NxArray< NxCCDSkeleton    * >    mSkeletons;
#endif
#if NX_SDK_VERSION_NUMBER >= 240
    NxArray< NxHeightField * >    mHeightFields;
#endif
    NxU32                        mNbActors;
    NxActor    **                    mActors;

    void **                        mActorUserData; // preserves the original actor user data field.

#endif
};



class UserStream : public NxStream
    {
    public:
                                UserStream(const char* filename, bool load);
    virtual                        ~UserStream();

    virtual        NxU8            readByte()                                const;
    virtual        NxU16            readWord()                                const;
    virtual        NxU32            readDword()                                const;
    virtual        float            readFloat()                                const;
    virtual        double            readDouble()                            const;
    virtual        void            readBuffer(void* buffer, NxU32 size)    const;

    virtual        NxStream&        storeByte(NxU8 b);
    virtual        NxStream&        storeWord(NxU16 w);
    virtual        NxStream&        storeDword(NxU32 d);
    virtual        NxStream&        storeFloat(NxReal f);
    virtual        NxStream&        storeDouble(NxF64 f);
    virtual        NxStream&        storeBuffer(const void* buffer, NxU32 size);

                FILE*            fp;
    };

class MemoryWriteBuffer : public NxStream
    {
    public:
                                MemoryWriteBuffer();
    virtual                        ~MemoryWriteBuffer();

    virtual        NxU8            readByte()                                const    { NX_ASSERT(0);    return 0;    }
    virtual        NxU16            readWord()                                const    { NX_ASSERT(0);    return 0;    }
    virtual        NxU32            readDword()                                const    { NX_ASSERT(0);    return 0;    }
    virtual        float            readFloat()                                const    { NX_ASSERT(0);    return 0.0f;}
    virtual        double            readDouble()                            const    { NX_ASSERT(0);    return 0.0;    }
    virtual        void            readBuffer(void* buffer, NxU32 size)    const    { NX_ASSERT(0);                }

    virtual        NxStream&        storeByte(NxU8 b);
    virtual        NxStream&        storeWord(NxU16 w);
    virtual        NxStream&        storeDword(NxU32 d);
    virtual        NxStream&        storeFloat(NxReal f);
    virtual        NxStream&        storeDouble(NxF64 f);
    virtual        NxStream&        storeBuffer(const void* buffer, NxU32 size);

                NxU32            currentSize;
                NxU32            maxSize;
                NxU8*            data;
    };

class MemoryReadBuffer : public NxStream
    {
    public:
                                MemoryReadBuffer(const NxU8* data);
    virtual                        ~MemoryReadBuffer();

    virtual        NxU8            readByte()                                const;
    virtual        NxU16            readWord()                                const;
    virtual        NxU32            readDword()                                const;
    virtual        float            readFloat()                                const;
    virtual        double            readDouble()                            const;
    virtual        void            readBuffer(void* buffer, NxU32 size)    const;

    virtual        NxStream&        storeByte(NxU8 b)                            { NX_ASSERT(0);    return *this;    }
    virtual        NxStream&        storeWord(NxU16 w)                            { NX_ASSERT(0);    return *this;    }
    virtual        NxStream&        storeDword(NxU32 d)                            { NX_ASSERT(0);    return *this;    }
    virtual        NxStream&        storeFloat(NxReal f)                        { NX_ASSERT(0);    return *this;    }
    virtual        NxStream&        storeDouble(NxF64 f)                        { NX_ASSERT(0);    return *this;    }
    virtual        NxStream&        storeBuffer(const void* buffer, NxU32 size)    { NX_ASSERT(0);    return *this;    }

    mutable        const NxU8*        buffer;
    };


UserStream::UserStream(const char* filename, bool load) : fp(NULL)
    {
    fp = fopen(filename, load ? "rb" : "wb");
    }

UserStream::~UserStream()
    {
    if(fp)    fclose(fp);
    }

// Loading API
NxU8 UserStream::readByte() const
    {
    NxU8 b;
    size_t r = fread(&b, sizeof(NxU8), 1, fp);
    NX_ASSERT(r);
    return b;
    }

NxU16 UserStream::readWord() const
    {
    NxU16 w;
    size_t r = fread(&w, sizeof(NxU16), 1, fp);
    NX_ASSERT(r);
    return w;
    }

NxU32 UserStream::readDword() const
    {
    NxU32 d;
    size_t r = fread(&d, sizeof(NxU32), 1, fp);
    NX_ASSERT(r);
    return d;
    }

float UserStream::readFloat() const
    {
    NxReal f;
    size_t r = fread(&f, sizeof(NxReal), 1, fp);
    NX_ASSERT(r);
    return f;
    }

double UserStream::readDouble() const
    {
    NxF64 f;
    size_t r = fread(&f, sizeof(NxF64), 1, fp);
    NX_ASSERT(r);
    return f;
    }

void UserStream::readBuffer(void* buffer, NxU32 size)    const
    {
    size_t w = fread(buffer, size, 1, fp);
    NX_ASSERT(w);
    }

// Saving API
NxStream& UserStream::storeByte(NxU8 b)
    {
    size_t w = fwrite(&b, sizeof(NxU8), 1, fp);
    NX_ASSERT(w);
    return *this;
    }

NxStream& UserStream::storeWord(NxU16 w)
    {
    size_t ww = fwrite(&w, sizeof(NxU16), 1, fp);
    NX_ASSERT(ww);
    return *this;
    }

NxStream& UserStream::storeDword(NxU32 d)
    {
    size_t w = fwrite(&d, sizeof(NxU32), 1, fp);
    NX_ASSERT(w);
    return *this;
    }

NxStream& UserStream::storeFloat(NxReal f)
    {
    size_t w = fwrite(&f, sizeof(NxReal), 1, fp);
    NX_ASSERT(w);
    return *this;
    }

NxStream& UserStream::storeDouble(NxF64 f)
    {
    size_t w = fwrite(&f, sizeof(NxF64), 1, fp);
    NX_ASSERT(w);
    return *this;
    }

NxStream& UserStream::storeBuffer(const void* buffer, NxU32 size)
    {
    size_t w = fwrite(buffer, size, 1, fp);
    NX_ASSERT(w);
    return *this;
    }




MemoryWriteBuffer::MemoryWriteBuffer() : currentSize(0), maxSize(0), data(NULL)
    {
    }

MemoryWriteBuffer::~MemoryWriteBuffer()
    {
    free(data);
    }

NxStream& MemoryWriteBuffer::storeByte(NxU8 b)
    {
    storeBuffer(&b, sizeof(NxU8));
    return *this;
    }
NxStream& MemoryWriteBuffer::storeWord(NxU16 w)
    {
    storeBuffer(&w, sizeof(NxU16));
    return *this;
    }
NxStream& MemoryWriteBuffer::storeDword(NxU32 d)
    {
    storeBuffer(&d, sizeof(NxU32));
    return *this;
    }
NxStream& MemoryWriteBuffer::storeFloat(NxReal f)
    {
    storeBuffer(&f, sizeof(NxReal));
    return *this;
    }
NxStream& MemoryWriteBuffer::storeDouble(NxF64 f)
    {
    storeBuffer(&f, sizeof(NxF64));
    return *this;
    }
NxStream& MemoryWriteBuffer::storeBuffer(const void* buffer, NxU32 size)
    {
    NxU32 expectedSize = currentSize + size;
    if(expectedSize > maxSize)
        {
        maxSize = expectedSize + 4096;

        NxU8* newData = (NxU8*)malloc(maxSize);
        if(data)
            {
            memcpy(newData, data, currentSize);
            free(data);
            }
        data = newData;
        }
    memcpy(data+currentSize, buffer, size);
    currentSize += size;
    return *this;
    }


MemoryReadBuffer::MemoryReadBuffer(const NxU8* data) : buffer(data)
    {
    }

MemoryReadBuffer::~MemoryReadBuffer()
    {
    // We don't own the data => no delete
    }

NxU8 MemoryReadBuffer::readByte() const
    {
    NxU8 b;
    memcpy(&b, buffer, sizeof(NxU8));
    buffer += sizeof(NxU8);
    return b;
    }

NxU16 MemoryReadBuffer::readWord() const
    {
    NxU16 w;
    memcpy(&w, buffer, sizeof(NxU16));
    buffer += sizeof(NxU16);
    return w;
    }

NxU32 MemoryReadBuffer::readDword() const
    {
    NxU32 d;
    memcpy(&d, buffer, sizeof(NxU32));
    buffer += sizeof(NxU32);
    return d;
    }

float MemoryReadBuffer::readFloat() const
    {
    float f;
    memcpy(&f, buffer, sizeof(float));
    buffer += sizeof(float);
    return f;
    }

double MemoryReadBuffer::readDouble() const
    {
    double f;
    memcpy(&f, buffer, sizeof(double));
    buffer += sizeof(double);
    return f;
    }

void MemoryReadBuffer::readBuffer(void* dest, NxU32 size) const
    {
    memcpy(dest, buffer, size);
    buffer += size;
    }





//-- BZ_NOTE: This should really use NxStream and the Serialize code in
//-- Core/Common/Src for dealing with endian issues. Actually.. What really should
//-- happen is the Serialize code is moved into Foundation so it is accessible by
//-- code outside of the SDK itself. BTW. Why don't we have an ENDIAN define? I'm
//-- going to just use _XBOX, but that really is silly in the general.

//-- BZ_TODO: refactor the write* functions to call the write*_b functions    to get
//-- rid of code duplication.

//==================================================================================
static const char *    getFloatString( float v, bool binary = false )
{
    static char    data[64*16];
    static int    index=0;

    char *ret =    &data[index*64];
    ++index;
    if (index == 16    ) index    = 0;

    if ( v == FLT_MAX )
    {
        strcpy(ret,"fltmax");
    }
    else if    ( v    == FLT_MIN )
    {
        strcpy(ret,"fltmin");
    }
    else if    ( v    == 1 )
    {
        strcpy(ret,"1");
    }
    else if    ( v    == 0 )
    {
        strcpy(ret,"0");
    }
    else if    ( v    == -1 )
    {
        strcpy(ret,"-1");
    }
    else
    {
        if ( binary    )
        {
            unsigned int *iv = (unsigned int *)    &v;
            sprintf(ret,"%.4f$%x", v, *iv );
        }
        else
        {
            sprintf(ret,"%.9f",    v );
            const char *dot    = strstr(ret,".");
            if ( dot )
            {
                int    len    = (int)strlen(ret);
                char *foo =    &ret[len-1];
                while (    *foo ==    '0'    ) foo--;
                if ( *foo == '.' )
                    *foo = 0;
                else
                    foo[1] = 0;
            }
        }
    }

    return ret;
}

//==================================================================================
static const char *    getDoubleString( double v, bool binary = false )
{
    static char    data[64*16];
    static int    index=0;

    char *ret =    &data[index*64];
    ++index;
    if (index == 16    ) index    = 0;

    if ( v == DBL_MAX )
    {
        strcpy(ret,"dblmax");
    }
    else if    ( v    == DBL_MIN )
    {
        strcpy(ret,"dblmin");
    }
    else if    ( v    == 1 )
    {
        strcpy(ret,"1");
    }
    else if    ( v    == 0 )
    {
        strcpy(ret,"0");
    }
    else if    ( v    == -1 )
    {
        strcpy(ret,"-1");
    }
    else
    {
        if ( binary    )
        {
            unsigned long int *iv = (unsigned long int *) &v;
            sprintf(ret, "%.4lf$%x", v, *iv );
        }
        else
        {
            sprintf(ret,"%.9lf", v );
            const char *dot    = strstr(ret,".");
            if ( dot )
            {
                int    len    = (int)strlen(ret);
                char *foo =    &ret[len-1];
                while (    *foo ==    '0'    ) foo--;
                if ( *foo == '.' )
                    *foo = 0;
                else
                    foo[1] = 0;
            }
        }
    }

    return ret;
}

//==================================================================================
static const char *    getString(NxJointProjectionMode    &m)
{
    const char *ret    = "unknown?";
    switch ( m )
    {
        case NX_JPM_NONE: ret =    "NX_JPM_NONE"; break;
        case NX_JPM_POINT_MINDIST: ret = "NX_JPM_POINT_MINDIST"; break;
    }
    return ret;
}

//==================================================================================
static const char *    getString(NxD6JointMotion m)
{
    const char * ret = "unknown?";

  switch ( m )
  {
    case NX_D6JOINT_MOTION_LOCKED: ret = "NX_D6JOINT_MOTION_LOCKED"; break;
    case NX_D6JOINT_MOTION_LIMITED:    ret    = "NX_D6JOINT_MOTION_LIMITED"; break;
    case NX_D6JOINT_MOTION_FREE: ret = "NX_D6JOINT_MOTION_FREE"; break;
  }

  return ret;
}

//==================================================================================
static const char *    getString(NxJointType type)
{
    const char *ret    = "unknown?";

  switch ( type    )
  {
    case NX_JOINT_PRISMATIC: ret = "NX_JOINT_PRISMATIC"; break;
    case NX_JOINT_REVOLUTE:    ret    = "NX_JOINT_REVOLUTE"; break;
    case NX_JOINT_CYLINDRICAL: ret = "NX_JOINT_CYLINDRICAL"; break;
    case NX_JOINT_SPHERICAL: ret = "NX_JOINT_SPHERICAL"; break;
    case NX_JOINT_POINT_ON_LINE: ret = "NX_JOINT_POINT_ON_LINE"; break;
    case NX_JOINT_POINT_IN_PLANE: ret =    "NX_JOINT_POINT_IN_PLANE"; break;
    case NX_JOINT_DISTANCE:    ret    = "NX_JOINT_DISTANCE"; break;
    case NX_JOINT_PULLEY: ret =    "NX_JOINT_PULLY"; break;
    case NX_JOINT_FIXED: ret = "NX_JOINT_FIXED"; break;
    case NX_JOINT_D6: ret =    "NX_JOINT_D6"; break;
    }

  return ret;
}

//==================================================================================
static const char *    getString(NxHeightFieldAxis    a)
{
    const char *ret    = "unknown?";

    switch ( a )
    {
        case NX_X:
            ret    = "NX_X";
            break;
        case NX_Y:
            ret    = "NX_Y";
            break;
        case NX_Z:
            ret    = "NX_Z";
            break;
        case NX_NOT_HEIGHTFIELD:
            ret    = "NX_NOT_HEIGHTFIELD";
            break;
    }
  return ret;
}

//==================================================================================
static const char *    getString(NxShapeType type)
{
    const char * ret = "unknown?";

    switch ( type )
    {
        case NX_SHAPE_PLANE:
            ret    = "NX_SHAPE_PLANE";
        break;

        case NX_SHAPE_SPHERE:
            ret    = "NX_SHAPE_SPHERE";
        break;

        case NX_SHAPE_BOX:
            ret    = "NX_SHAPE_BOX";
        break;

        case NX_SHAPE_CAPSULE:
            ret    = "NX_SHAPE_CAPSULE";
        break;

        case NX_SHAPE_CONVEX:
            ret    = "NX_SHAPE_CONVEX";
        break;

#if NX_SDK_VERSION_NUMBER >= 230
        case NX_SHAPE_WHEEL:
            ret = "NX_SHAPE_WHEEL";
        break;
#endif

        case NX_SHAPE_MESH:
            ret    = "NX_SHAPE_MESH";
        break;

#if NX_SDK_VERSION_NUMBER >= 240
        case NX_SHAPE_HEIGHTFIELD:
            ret = "NX_SHAPE_HEIGHTFIELD";
        break;
    
        case NX_SHAPE_RAW_MESH:
            ret = "NX_SHAPE_RAW_MESH";
        break;
#endif

        case NX_SHAPE_COMPOUND:
            ret    = "NX_SHAPE_COMPOUND";
        break;
    }

    return ret;
}

//==================================================================================
static const char *    getString(NxCombineMode    mode)
{
    const char * ret = "unknown?";
    switch ( mode )
    {
      case NX_CM_AVERAGE: ret =    "NX_CM_AVERAGE"; break;
      case NX_CM_MIN:      ret =    "NX_CM_MIN"; break;
      case NX_CM_MULTIPLY:ret =    "NX_CM_MULTIPLY"; break;
      case NX_CM_MAX:      ret =    "NX_CM_MAX"; break;
    }
    return ret;
}

//==================================================================================
CoreDump::CoreDump(void) :
    mActorUserData( 0 ),
    mFph( 0 ),
    mBinary( true ),
    mSaveCookedData( false ),
    mIndent( 0 ),
    mNbActors( 0 ),
    mActors( 0 )
{
}

//==================================================================================
unsigned int CoreDump::getCoreVersion(NxPhysicsSDK *sdk)
{
    unsigned int ret = 0;
      NxU32 apiRev, descRev, branchId, sdkRev;

#if NX_SDK_VERSION_NUMBER >= 230
    if ( sdk )
    {
      sdkRev = sdk->getInternalVersion( apiRev, descRev, branchId );
    }
    else
#endif
    {
        sdkRev        = NX_SDK_VERSION_NUMBER;
#if NX_SDK_VERSION_NUMBER >= 230
        apiRev        = NX_SDK_API_REV;
        descRev        = NX_SDK_DESC_REV;
        branchId    = NX_SDK_BRANCH;
#else
            apiRev = 0;
            descRev = 0;
            branchId = 0;
#endif
    }

    // The core dump version is determined first by what SDK version we are on 2.3.0 etc...
    // next by the version of the serialized data stream and, finally, by what
    // branch we are building against.
    ret = ( sdkRev << 16 ) | ( descRev << 8 ) | ( branchId << 8 );

    return ret;
}

//==================================================================================
//    save the contents of the state of the physics SDK in text or binary.
//==================================================================================
bool CoreDump::coreDump( NxPhysicsSDK *sdk, const char *fname, bool binary, bool saveoutCookedData )
{
    bool ret = false;

#if    CORE_DUMP_ENABLE

    mFph = fopen(fname,"wb");
    mBinary    = binary;
    mIndent    = 0;
    if ( mBinary )
    {
        mSaveCookedData = saveoutCookedData;
    }

    if ( 1 ) //    gather all of the triangle meshes and convex hulls in the scene!
    {
        NxU32 scount = sdk->getNbScenes();
        for    (NxU32 i=0;    i<scount; i++)
        {
            NxScene    *scene = sdk->getScene(i);

            NxU32 acount = scene->getNbActors();

            if ( acount    )
            {
                NxActor    **actors = scene->getActors();
                for    (NxU32 j=0;    j<acount; j++)
                {
                    NxActor    *a = actors[j];
                    NxU32 nbActorShapes      =    a->getNbShapes();
                    NxShape    *const*    actorShapes    = a->getShapes();

                    for    (NxU32 k=0;    k<nbActorShapes; k++)
                    {
                        NxShape    *shape = actorShapes[k];
                        switch ( shape->getType() )
                        {
                            case NX_SHAPE_PLANE:
                            {
                                const NxPlaneShape *current = shape->isPlane();
                                assert( current );
#if NX_SDK_VERSION_NUMBER >= 230
                                if ( current->getCCDSkeleton() )
                                {
                                    AddSkeleton( current->getCCDSkeleton() );
                                }
#endif
                            }
                            break;
                            
                            case NX_SHAPE_SPHERE:
                            {
                                const NxSphereShape *current = shape->isSphere();
                                assert(    current    );
#if NX_SDK_VERSION_NUMBER >= 230
                                if ( current->getCCDSkeleton() )
                                {
                                    AddSkeleton( current->getCCDSkeleton() );
                                }
#endif
                            }
                            break;

                            case NX_SHAPE_BOX:
                            {
                                const NxBoxShape *current = shape->isBox();
                                assert(    current    );
#if NX_SDK_VERSION_NUMBER >= 230
                                if ( current->getCCDSkeleton() )
                                {
                                    AddSkeleton( current->getCCDSkeleton() );
                                }
#endif
                            }
                            break;

                            case NX_SHAPE_CAPSULE:
                            {
                                const NxCapsuleShape *current = shape->isCapsule();
                                assert( current );
#if NX_SDK_VERSION_NUMBER >= 230
                                if ( current->getCCDSkeleton() )
                                {
                                    AddSkeleton( current->getCCDSkeleton() );
                                }
#endif
                            }
                                break;
                            case NX_SHAPE_CONVEX:
                            {
                                const NxConvexShape *current = shape->isConvexMesh();
                                assert( current );
                                NxConvexShapeDesc Desc;
                                current->saveToDesc(Desc);
#if NX_SDK_VERSION_NUMBER >= 230
                                if ( current->getCCDSkeleton() )
                                {
                                    AddSkeleton( current->getCCDSkeleton() );
                                }
#endif
                                if ( Desc.meshData )    
                                    AddConvex(Desc.meshData);
                            }
                            break;

#if NX_SDK_VERSION_NUMBER >= 230
                            // note that in the earlier versions of the SDK, NX_SHAPE_WHEEL did not exist
                            case NX_SHAPE_WHEEL:
                            {
                                // k, nxwheelshapes do not have a ccd skeletons, nor
                                // do they have meshes (thus far), so don't sweat this
                            }
                            break;
#endif
                            case NX_SHAPE_MESH:
                            {
                                const NxTriangleMeshShape *current     = shape->isTriangleMesh();
                                assert( current );
                                NxTriangleMeshShapeDesc    Desc;
                                assert(    current    );
                                current->saveToDesc(Desc);
#if NX_SDK_VERSION_NUMBER >= 230
                                if ( current->getCCDSkeleton() )
                                {
                                    AddSkeleton( current->getCCDSkeleton() );
                                }
#endif
                                if ( Desc.meshData )    
                                    AddTriangleMesh(Desc.meshData);
                            }
                            break;

#if NX_SDK_VERSION_NUMBER >= 240
                            case NX_SHAPE_HEIGHTFIELD:
                            {
                                const NxHeightFieldShape *current = shape->isHeightField();
                                NxHeightFieldShapeDesc    Desc;
                                assert(    current    );
                                current->saveToDesc(Desc);
                                // k, NxHeightFieldShapes do not have a ccd skeletons, nor
                                // do they have meshes (thus far), but they DO have other data, so...
                                if ( Desc.heightField )
                                {
                                    AddHeightField( Desc.heightField );
                                }
                            }
                            break;
                        
                            case NX_SHAPE_RAW_MESH:
                                assert(0); // this should not happen
                            break;
#endif
                            case NX_SHAPE_COMPOUND:
                                assert(0); // this is impossible!
                            break;

                            default:
                                assert(0); //!!???
                            break;
                        }
                    }
                }
            }

        }
    }

    if ( mFph )
    {
        WriteInt("[Physics SDK Version]", getCoreVersion(sdk) );
        ++mIndent;

        // write out the number    of values we are writing
        WriteInt("[NUM_PARAMS]", NX_PARAMS_NUM_VALUES );
        for    (int i=NX_PENALTY_FORCE; i<    NX_PARAMS_NUM_VALUES; i++)
        {
            NxParameter    p =    (NxParameter) i;
            NxReal v = sdk->getParameter( p    );
            WriteParam(    p, v );
        }

        // list    of all of
        if ( mBinary )
        {
            unsigned int i;

            // k, output triangle mesh info
            unsigned numTriangleMeshes = mTriangleMeshes.size();
            WriteInt_b(    numTriangleMeshes );
            for    ( i    = 0; i < numTriangleMeshes;    ++i    )
            {
                char scratch[512];
                sprintf( scratch, "[TriangleMesh%d]", i+1 );
                //WriteTriangleMesh_b( mTriangleMeshes[i] );
                Write( scratch,    mTriangleMeshes[i] );
            }

            // k, output the convex    mesh info
            unsigned numConvexMeshes = mConvexMeshes.size();
            WriteInt_b(    numConvexMeshes    );
            for    ( i    = 0; i < numConvexMeshes; ++i )
            {
                char scratch[512];
                sprintf( scratch, "[ConvexMesh%d]",    i+1    );
                //WriteConvexMesh_b( scratch, mConvexMeshes[i] );
                Write( scratch,    mConvexMeshes[i] );
            }
#if NX_SDK_VERSION_NUMBER >= 230
            // k, output skeletons
            unsigned numSkeletons =    mSkeletons.size();
            WriteInt_b(    numSkeletons );
            for    ( i    = 0; i < numSkeletons; ++i )
            {
                char scratch[512];
                sprintf( scratch, "[CCDSkeleton%d]", i+1 );
                Write( scratch,    mSkeletons[i] );
            }
#endif

#if NX_SDK_VERSION_NUMBER >= 240
            // write out the height fields
            unsigned numHeightFields = mHeightFields.size();
            WriteInt_b( numHeightFields );
            for ( i = 0; i < numHeightFields; ++i )
            {
                char scratch[512];
                sprintf( scratch, "HeightField %d", i+1 );
                Write( scratch, mHeightFields[i] );
            }
#endif
        }
        else
        {
            unsigned int i;
            Print("[nbTriangleMeshes] %d", mTriangleMeshes.size() );
            ++mIndent;

            for    (i=0; i<mTriangleMeshes.size(); ++i )
            {
                char scratch[512];
                sprintf(scratch,"[TriangleMesh%d]",    i );
                Write(scratch, mTriangleMeshes[i] );
            }
            --mIndent;

            Print("[nbConvexMeshes]    %d",   mConvexMeshes.size()    );
            ++mIndent;
            if ( 1 )
            {
                for    ( i=0; i<mConvexMeshes.size(); ++i )
                {
                        char scratch[512];
                        sprintf(scratch,"[ConvexMesh%d]", i+1 );
                        Write(scratch, mConvexMeshes[i]    );
                }
            }
            --mIndent;
#if NX_SDK_VERSION_NUMBER >= 230
            unsigned numSkeletons = mSkeletons.size();
            Print("[nbCCDSkeletons]    %d", numSkeletons );
            ++mIndent;
            if ( 1 )
            {
                for    ( i    = 0; i < numSkeletons; ++i )
                {
                    char scratch[512];
                    sprintf( scratch, "[CCDSkeleton%d]", i+1 );
                    Write( scratch,    mSkeletons[i] );
                }
            }
            --mIndent;
#endif

#if NX_SDK_VERSION_NUMBER >= 240
            unsigned numHeightFields = mHeightFields.size();
            Print("[nbHeightFields] %d", numHeightFields );

            ++mIndent;
            for ( i = 0; i < numHeightFields; ++i )
            {
                char scratch[512];
                sprintf( scratch, "[HeightField%d]", i+1 );
                Write( scratch, mHeightFields[i] );
            }
            --mIndent;
#endif
        }

        NxU32 scount = sdk->getNbScenes();
        Write("[NbScenes]",scount);
        ++mIndent;
        if (  1    )
        {
            for    (NxU32 i=0;    i<scount; i++)
            {
                    NxScene    *scene = sdk->getScene(i);
                    assert(    scene );
                    char scratch[512];
                    sprintf(scratch,"[Scene%d]", i+1 );
                    SaveUserData(scene);
                    Write(scratch,scene);
                    RestoreUserData(scene);
            }
        }

        --mIndent;
        --mIndent;

        fclose(mFph);
        mFph = 0;
        ret = true;
    }
#endif

    return ret;
}


#if    CORE_DUMP_ENABLE

//==================================================================================
void CoreDump::WriteBool( const    char *desc,    NX_BOOL yesNo )
{
    if ( mFph )
    {
        if ( mBinary )
        {
            char value = yesNo ? 1 : 0;
            fwrite(    &value,    sizeof(char), 1, mFph );
        }
        else
        {
            Print( "%s = %s", desc,    yesNo ?    "TRUE" : "FALSE" );
        }
    }
}

//==================================================================================
void CoreDump::WriteChar( const char *desc, char c, bool descAsInt )
{
    if ( mFph )
    {
        if ( mBinary )
        {
            fwrite(    &c,    sizeof(char), 1, mFph );
        }
        else
        {
            if ( descAsInt )
            {
                Print( "%s = %d", desc,    (int)c );
            }
            else
            {
                Print( "%s = %c", desc,    c );
            }
        }
    }
}

//==================================================================================
void CoreDump::WriteInt(const char *desc,int n)
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&n);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &n,    sizeof(int), 1,    mFph );
        #endif
        }
        else
            Print("%s =    %d", desc, n );
    }
}

//==================================================================================
void CoreDump::WriteLong( const    char *desc,    long l )
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&l);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &l,    sizeof(long), 1, mFph );
        #endif
        }
        else
            Print("%s =    %ld", desc,    l );
    }
}

//==================================================================================
void CoreDump::WriteShort( const char *desc, short s )
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU16 value=coredump_flip((const NxU16*)&s);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &s,    sizeof(short), 1, mFph );
        #endif
        }
        else
            Print("%s =    %d", desc, s );
    }
}

//==================================================================================
void CoreDump::WriteUnsigned( const    char *desc,    unsigned u )
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&u);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &u,    sizeof(unsigned), 1, mFph );
        #endif
        }
        else
            Print("%s =    %d", desc, u );
    }
}

//==================================================================================
void CoreDump::WriteUnsignedLong( const    char *desc,    unsigned long ul )
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&ul);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &ul, sizeof(unsigned long),    1, mFph    );
        #endif
        }
        else
            Print("%s =    %ld", desc,    ul );
    }
}

//==================================================================================
void CoreDump::WriteUnsignedShort( const char *desc, unsigned short    us )
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU16 value=coredump_flip((const NxU16*)&us);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else        
            fwrite(    &us, sizeof(unsigned short), 1,    mFph );
        #endif
        }
        else
            Print("%s =    %d", desc, us );
    }
}

//==================================================================================
void CoreDump::WriteFloat( const char *desc, float f )
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxF32 value=coredump_flip((const NxF32*)&f);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &f,    sizeof(float), 1, mFph );
        #endif
        }
        else
            Print( "%s = %s", desc,    getFloatString(    f, false ) );
            //Print( "%s %f", desc,    f );
    }
}

//==================================================================================
void CoreDump::WriteFlag( const    char *desc,    unsigned f )
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&f);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &f,    sizeof(unsigned), 1, mFph );
        #endif
        }
        else
            Print( "%s = 0x%x", desc,    f );
    }
}

//==================================================================================
void CoreDump::WriteString(    const char *info )
{
    if ( mFph )
    {
        if ( mBinary )
        {
            int    len    = 0;
            assert(    mBinary    );

            if ( info )
            {
                len    = strlen( info );

            #ifdef _XBOX
                NxU32 value=coredump_flip((const NxU32*)&len);
                fwrite(    &value,    sizeof(value), 1,    mFph );
            #else
                fwrite(    &len, sizeof(int), 1, mFph );
            #endif
                fwrite(    info, sizeof(char),    sizeof(char)*len, mFph );
            }
            else
            {
            #ifdef _XBOX
                NxU32 value=coredump_flip((const NxU32*)&len);
                fwrite(    &value,    sizeof(value), 1,    mFph );
            #else
                fwrite(    &len, sizeof(int), 1, mFph );
            #endif
            }         
        }
        else
        {
            Print( "%s", info );
        }
    }
}

//==================================================================================
void CoreDump::WriteChar_b(    char val )
{
    if ( mFph )
    {
        assert(    mBinary    );
        fwrite(    &val, sizeof(char),    1, mFph    );
    }
}

//==================================================================================
void CoreDump::WriteInt_b( int val )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxU32 value=coredump_flip((const NxU32*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(int), 1, mFph );
    #endif
    }
}

//==================================================================================
void CoreDump::WriteLong_b(    long val )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxU32 value=coredump_flip((const NxU32*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(long),    1, mFph    );
    #endif
    }
}

//==================================================================================
void CoreDump::WriteShort_b( short val )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxU16 value=coredump_flip((const NxU16*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(short), 1,    mFph );           
    #endif
    }
}

//==================================================================================
void CoreDump::WriteUnsigned_b(    unsigned val )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxU32 value=coredump_flip((const NxU32*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(unsigned),    1, mFph    );          
    #endif
    }
}

//==================================================================================
void CoreDump::WriteUnsignedLong_b(    unsigned long val )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxU32 value=coredump_flip((const NxU32*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(unsigned long), 1,    mFph );           
    #endif
    }
}

//==================================================================================
void CoreDump::WriteUnsignedShort_b( unsigned short    val    )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxU16 value=coredump_flip((const NxU16*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(unsigned short), 1, mFph );        
    #endif
    }
}

//==================================================================================
void CoreDump::WriteFloat_b( float val )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxF32 value=coredump_flip((const NxF32*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(float), 1,    mFph );           
    #endif
    }
}

//==================================================================================
void CoreDump::WriteDouble_b( double val )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxF64 value=coredump_flip((const NxF64*)&val);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &val, sizeof(double), 1, mFph );        
    #endif
    }
}

//==================================================================================
void CoreDump::WriteString_b( const    char *val )
{
    if ( mFph )
    {
        int    len    = 0;
        assert(    mBinary    );

        if ( val )
        {
            len    = strlen( val );

        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&len);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &len, sizeof(int), 1, mFph );
        #endif
            fwrite(    &val, sizeof(char),    sizeof(char)*len, mFph );
        }
        else
        {
        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&len);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(    &len, sizeof(int), 1, mFph );
        #endif
        }
    }
}

//==================================================================================
void CoreDump::WriteFlag_b(    unsigned flag )
{
    if ( mFph )
    {
        assert(    mBinary    );
    #ifdef _XBOX
        NxU32 value=coredump_flip((const NxU32*)&flag);
        fwrite(    &value,    sizeof(value), 1,    mFph );
    #else
        fwrite(    &flag, sizeof(unsigned), 1,    mFph );
    #endif
    }
}

//==================================================================================
void CoreDump::WriteMatrix_b( const    NxMat34    &matrix    )
{
    if ( mFph )
    {
        assert(    mBinary    );
        NxReal m[9];
        matrix.M.getRowMajor( m    );

        // k, write    out    the    matrix
        for    ( int i    = 0; i < 9;    ++i    )
        {
            WriteFloat_b( m[i] );
        }
        WriteFloat_b( matrix.t.x );
        WriteFloat_b( matrix.t.y );
        WriteFloat_b( matrix.t.z );
    }
}

//==================================================================================
void CoreDump::WriteVec3_b(    const NxVec3 &v    )
{
    if ( mFph )
    {
        assert(    mBinary    );
        WriteFloat_b( v.x );
        WriteFloat_b( v.y );
        WriteFloat_b( v.z );
    }
}

//==================================================================================
void CoreDump::WriteParam(NxParameter p,NxReal v)
{
    if ( mFph )
    {
        if ( mBinary )
            WriteFloat_b( v    );
        else
        {
            const char *param =    "Unknown?";

            switch ( p )
            {
#if NX_SDK_VERSION_NUMBER > 230
                case NX_BBOX_NOISE_LEVEL:
                    param = "NX_BBOX_NOISE_LEVEL";
                    break;
                case NX_SOLVER_CONVERGENCE_THRESHOLD:
                    param = "NX_SOLVER_CONVERGENCE_THRESHOLD";
                    break;
#endif

#if NX_SDK_VERSION_NUMBER >= 231 && NX_USE_FLUID_API
                case NX_VISUALIZE_FLUID_DRAINS:
                    param = "NX_VISUALIZE_FLUID_DRAINS";
                    break;
                case NX_VISUALIZE_FLUID_PACKETS:
                    param = "NX_VISUALIZE_FLUID_PACKETS";
                    break;
                case NX_VISUALIZE_FLUID_MOTION_LIMIT:
                    param = "NX_VISUALIZE_FLUID_MOTION_LIMIT";
                    break;
                case NX_VISUALIZE_FLUID_DYN_COLLISION:
                    param = "NX_VISUALIZE_FLUID_DYN_COLLISION";
                    break;
#endif
#if NX_SDK_VERSION_NUMBER >= 230
                case NX_CCD_EPSILON:
                    param = "NX_CCD_EPSILON";
                    break;
#endif
                case NX_PENALTY_FORCE:
                    param =    "NX_PENALTY_FORCE";    break;
                case NX_SKIN_WIDTH:
                    param =    "NX_SKIN_WIDTH"; break;
                case NX_DEFAULT_SLEEP_LIN_VEL_SQUARED:
                    param =    "NX_DEFAULT_SLEEP_LIN_VEL_SQUARED";    break;
                case NX_DEFAULT_SLEEP_ANG_VEL_SQUARED:
                    param =    "NX_DEFAULT_SLEEP_ANG_VEL_SQUARED";    break;
#if NX_SDK_VERSION_NUMBER >= 230
                case NX_BOUNCE_THRESHOLD:
#else
                case NX_BOUNCE_TRESHOLD:
#endif
                    param =    "NX_BOUNCE_THRESHOLD"; break;
                case NX_DYN_FRICT_SCALING:
                    param =    "NX_DYN_FRICT_SCALING";    break;
                case NX_STA_FRICT_SCALING:
                    param =    "NX_STA_FRICT_SCALING";    break;
                case NX_MAX_ANGULAR_VELOCITY:
                    param =    "NX_MAX_ANGULAR_VELOCITY"; break;
                case NX_CONTINUOUS_CD:
                    param =    "NX_CONTINUOUS_CD";    break;
                case NX_VISUALIZATION_SCALE:
                    param =    "NX_VISUALIZATION_SCALE"; break;
                case NX_VISUALIZE_WORLD_AXES:
                    param =    "NX_VISUALIZE_WORLD_AXES"; break;
                case NX_VISUALIZE_BODY_AXES:
                    param =    "NX_VISUALIZE_BODY_AXES"; break;
                case NX_VISUALIZE_BODY_MASS_AXES:
                    param =    "NX_VISUALIZE_BODY_MASS_AXES"; break;
                case NX_VISUALIZE_BODY_LIN_VELOCITY:
                    param =    "NX_VISUALIZE_BODY_LIN_VELOCITY"; break;
                case NX_VISUALIZE_BODY_ANG_VELOCITY:
                    param =    "NX_VISUALIZE_BODY_ANG_VELOCITY"; break;
                case NX_VISUALIZE_BODY_JOINT_GROUPS:
                    param =    "NX_VISUALIZE_BODY_JOINT_GROUPS"; break;
                case NX_VISUALIZE_JOINT_LOCAL_AXES:
                    param =    "NX_VISUALIZE_JOINT_LOCAL_AXES"; break;
                case NX_VISUALIZE_JOINT_WORLD_AXES:
                    param =    "NX_VISUALIZE_JOINT_WORLD_AXES"; break;
                case NX_VISUALIZE_JOINT_LIMITS:
                    param =    "NX_VISUALIZE_JOINT_LIMITS"; break;
                case NX_VISUALIZE_CONTACT_POINT:
                    param =    "NX_VISUALIZE_CONTACT_POINT"; break;
                case NX_VISUALIZE_CONTACT_NORMAL:
                    param =    "NX_VISUALIZE_CONTACT_NORMAL"; break;
                case NX_VISUALIZE_CONTACT_ERROR:
                    param =    "NX_VISUALIZE_CONTACT_ERROR"; break;
                case NX_VISUALIZE_CONTACT_FORCE:
                    param =    "NX_VISUALIZE_CONTACT_FORCE"; break;
                case NX_VISUALIZE_ACTOR_AXES:
                    param =    "NX_VISUALIZE_ACTOR_AXES"; break;
                case NX_VISUALIZE_COLLISION_AABBS:
                    param =    "NX_VISUALIZE_COLLISION_AABBS";    break;
                case NX_VISUALIZE_COLLISION_SHAPES:
                    param =    "NX_VISUALIZE_COLLISION_SHAPES"; break;
                case NX_VISUALIZE_COLLISION_AXES:
                    param =    "NX_VISUALIZE_COLLISION_AXES"; break;
                case NX_VISUALIZE_COLLISION_COMPOUNDS:
                    param =    "NX_VISUALIZE_COLLISION_COMPOUNDS";    break;
                case NX_VISUALIZE_COLLISION_VNORMALS:
                    param =    "NX_VISUALIZE_COLLISION_VNORMALS"; break;
                case NX_VISUALIZE_COLLISION_FNORMALS:
                    param =    "NX_VISUALIZE_COLLISION_FNORMALS"; break;
#if NX_SDK_VERSION_NUMBER >= 230
                case NX_VISUALIZE_COLLISION_EDGES:
                    param =    "NX_VISUALIZE_COLLISION_EDGES";    break;
#endif
                case NX_VISUALIZE_COLLISION_SPHERES:
                    param =    "NX_VISUALIZE_COLLISION_SPHERES"; break;
                case NX_VISUALIZE_COLLISION_STATIC:
                    param =    "NX_VISUALIZE_COLLISION_STATIC"; break;
                case NX_VISUALIZE_COLLISION_DYNAMIC:
                    param =    "NX_VISUALIZE_COLLISION_DYNAMIC"; break;
                case NX_VISUALIZE_COLLISION_FREE:
                    param =    "NX_VISUALIZE_COLLISION_FREE"; break;
#if NX_SDK_VERSION_NUMBER >= 230
                case NX_VISUALIZE_COLLISION_CCD:
                    param =    "NX_VISUALIZE_COLLISION_CCD"; break;
                case NX_VISUALIZE_COLLISION_SKELETONS:
                    param =    "NX_VISUALIZE_COLLISION_SKELETONS";    break;
#endif
        #if    NX_USE_FLUID_API
                case NX_VISUALIZE_FLUID_EMITTERS:
                    param =    "NX_VISUALIZE_FLUID_EMITTERS"; break;
                case NX_VISUALIZE_FLUID_POSITION:
                    param =    "NX_VISUALIZE_FLUID_POSITION"; break;
                case NX_VISUALIZE_FLUID_VELOCITY:
                    param =    "NX_VISUALIZE_FLUID_VELOCITY"; break;
                case NX_VISUALIZE_FLUID_KERNEL_RADIUS:
                    param =    "NX_VISUALIZE_FLUID_KERNEL_RADIUS";    break;
                case NX_VISUALIZE_FLUID_BOUNDS:
                    param =    "NX_VISUALIZE_FLUID_BOUNDS"; break;
        #endif

        #if    NX_USE_CLOTH_API
                case NX_VISUALIZE_CLOTH_COLLISIONS:
                    param =    "NX_VISUALIZE_CLOTH_COLLISIONS"; break;
                case NX_VISUALIZE_CLOTH_SELFCOLLISIONS:
                    param =    "NX_VISUALIZE_CLOTH_SELFCOLLISIONS"; break;
                case NX_VISUALIZE_CLOTH_WORKPACKETS:
                    param =    "NX_VISUALIZE_CLOTH_WORKPACKETS"; break;
        #endif

                case NX_ADAPTIVE_FORCE:
                    param =    "NX_ADAPTIVE_FORCE"; break;
#if NX_SDK_VERSION_NUMBER >= 230
                case NX_COLL_VETO_JOINTED:
                    param =    "NX_COLL_VETO_JOINTED";    break;
#endif
#if NX_SDK_VERSION_NUMBER >= 230
                case NX_TRIGGER_TRIGGER_CALLBACK:
                    param =    "NX_TRIGGER_TRIGGER_CALLBACK"; break;
                case NX_SELECT_HW_ALGO:
                    param =    "NX_SELECT_HW_ALGO"; break;
                case NX_VISUALIZE_ACTIVE_VERTICES:
                    param =    "NX_VISUALIZE_ACTIVE_VERTICES";    break;
#endif
                case NX_PARAMS_NUM_VALUES:
                    param =    "NX_PARAMS_NUM_VALUES";    break;
                default:
                  assert(0);
                  break;
            }
        
            ++mIndent;
            Print("[%s]    %s", param,    getFloatString(v) );
            --mIndent;
        }
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,NxU32    v)
{
    if ( mFph )
    {
        if ( mBinary )
        {
        #ifdef _XBOX
            NxU32 value=coredump_flip((const NxU32*)&v);
            fwrite(    &value,    sizeof(value), 1,    mFph );
        #else
            fwrite(&v, sizeof(NxU32), 1, mFph );
        #endif
        }
        else
            Print("%s %d", desc, v );
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc, NxScene *scene)
{
    if ( mFph )
    {
        ++mIndent;
        Print("%s",    desc);

        NxU32 i;

        // write the scene description first
        WriteSceneDesc(    scene );

        // write material stuff
        const NxU32 count = scene->getNbMaterials();
        WriteUnsigned( "[NbMaterials]",    count );

        if ( count )
        {

            NxSceneDesc sd;
#if NX_SDK_VERSION_NUMBER >= 230
            scene->saveToDesc(sd);
            bool hardwareScene = true;
            if ( sd.simType == NX_SIMULATION_SW ) hardwareScene = false;
#else
            bool hardwareScene = false;
#endif

            NxMaterial **materials = new NxMaterial *[count];
            memset(materials, 0, sizeof(NxMaterial *)*count );
            NxU32 iterator = 0;
            NxU32 gcount = scene->getMaterialArray(    &materials[0], count, iterator );


            for    ( i    = 0; i < count;    ++i    )
            {
                NxMaterial *mat    = materials[i];

                char scratch[128];
                sprintf( scratch, "[NxMaterial%d]", i+1 );
                ++mIndent;
                Write( scratch,    mat, hardwareScene    );
                --mIndent;
            }

            if(materials)
            {
                delete[] materials;
                materials=NULL;
            }
        }

        // write actor stuff
        NxU32 actorCount = scene->getNbActors();
        mNbActors         = actorCount;
        WriteUnsigned( "[NbActors]", actorCount    );

        if ( actorCount    )
        {
            NxActor    **actors = scene->getActors();
            mActors    = actors;
            for    ( i    = 0; i < actorCount; ++i )
            {
                NxActor    *a = actors[i];
                char scratch[512];
               
                sprintf(scratch,"[NxActor%d]", i+1);
                ++mIndent;
                Write( scratch,    a );
                --mIndent;
            }
        }

        // write pairs
        NxU32 pairCount    = scene->getNbPairs();
        WriteInt( "[NbPairs]", pairCount );

        if ( pairCount )
        {
            NxPairFlag *array = new NxPairFlag[pairCount];
            memset(array, 0, pairCount*sizeof(NxPairFlag));
            scene->getPairFlagArray( array,    pairCount );
            for    ( i    = 0; i < pairCount;    ++i    )
            {
                char scratch[512];
                sprintf(scratch,"[NxPairFlag%d]", i+1 );

                ++mIndent;
                Write( scratch,    &array[i] );
                --mIndent;
            }


            if(array!=NULL)
            {
                delete[] array;
                array=NULL;
            }
        }

        // write joints
        NxU32 jointCount = scene->getNbJoints();
        WriteInt( "[NbJoints]",    jointCount );
        if ( jointCount    )
        {
            scene->resetJointIterator();
            for    ( i    = 0; i < jointCount; ++i )
            {
                NxJoint    *j = scene->getNextJoint();
                assert(j);
                char scratch[512];
                sprintf(scratch,"[NxJoint%d]", i );

                ++mIndent;
                Write(scratch,j);
                --mIndent;
            }
        }

        // now write effectors
        NxU32 effectorCount = scene->getNbEffectors();
        WriteInt( "[NbEffectors]", effectorCount );
        if ( effectorCount )
        {
            scene->resetEffectorIterator();
            for ( i = 0; i < effectorCount; ++i )
            {
                NxEffector *e = scene->getNextEffector();
                assert( e );
                char scratch[512];
                sprintf( scratch,"[NxEffector%d]", i );

                ++mIndent;
                Write( scratch, *scene, *e );
                --mIndent;
            }
        }

        if ( 0 )
        {
//#pragma message("TODO: Implement binary core dump    for    'Fluids'")
            //count    = scene->getNbFluids();
        }
        mActors    = 0;
        --mIndent;
    }
}

//==================================================================================
void CoreDump::WriteSceneDesc( NxScene *scene )
{
    // here    we write out the scene description - note that currently we    cannot
    // obtain the scene    description    from a scene, so we    will "fake"    it for now
    if ( mFph && scene )
    {
        Print( "sceneDesc" );
        ++mIndent;

        // k, simply get the scene desc with our new call
        NxSceneDesc    desc;
#if NX_SDK_VERSION_NUMBER >= 230
        scene->saveToDesc(desc);
#else

      scene->getGravity( desc.gravity );
        scene->getTiming( desc.maxTimestep, desc.maxIter, desc.timeStepMethod, 0 );



#endif

        // now write everything
        WriteBool( "groundPlane",         desc.groundPlane );
        WriteBool( "boundsPlanes",        desc.boundsPlanes );
        Write( "gravity",                 desc.gravity );
        WriteInt( "timeStepMethod",          desc.timeStepMethod );
        WriteFloat(    "maxTimestep",        desc.maxTimestep );
        WriteUnsigned( "maxIter",         desc.maxIter );
#if NX_SDK_VERSION_NUMBER >= 230
        WriteUnsigned( "simType",         desc.simType );
#endif

        WriteBool( "limits", desc.limits ? true : false );
        if ( desc.limits )
        {
            //
            WriteUnsigned( "limits.maxActors",          desc.limits->maxNbActors );
            WriteUnsigned( "limits.maxBodies",          desc.limits->maxNbBodies );
            WriteUnsigned( "limits.maxStaticShapes",  desc.limits->maxNbStaticShapes );
            WriteUnsigned( "limits.maxDynamicShapes", desc.limits->maxNbDynamicShapes    );
            WriteUnsigned( "limits.maxJoints" ,          desc.limits->maxNbJoints );    
        }

        WriteBool( "maxBounds", desc.maxBounds ? true : false );
        if ( desc.maxBounds )
        {
            if ( mBinary )
            {
                WriteVec3_b( desc.maxBounds->min );
                WriteVec3_b( desc.maxBounds->max );
            }
            else
            {
                Print("%maxBounds->min=(%s,%s,%s)", getFloatString(desc.maxBounds->min.x),
                    getFloatString(desc.maxBounds->min.y), getFloatString(desc.maxBounds->min.z) );
                Print("%maxBounds->max=(%s,%s,%s)", getFloatString(desc.maxBounds->max.x),
                    getFloatString(desc.maxBounds->max.y), getFloatString(desc.maxBounds->max.z) );
            }
        }

        // write out pntr members, but only if ASCII, as we won't know what the heck they
        // are (they are user-defined)
        if ( !mBinary )
        {
            Print( "NxUserNotify Present = %s", desc.userNotify ? "true" : "false" );
            Print( "NxUserTriggerReport Present = %s", desc.userTriggerReport ? "true" : "false" );
            Print( "NxUserContactReport Present = %s", desc.userContactReport ? "true" : "false" );
            Print( "userData Present = %s", desc.userData ? "true" : "false" );
        }

        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,const    NxVec3 &v)
{
    if ( mFph )
    {
        if ( mBinary )
        {
            //WriteVec3_b( v );
            WriteFloat_b( v.x );
            WriteFloat_b( v.y );
            WriteFloat_b( v.z );
        }
        else
        {
            Print("%s=%s,%s,%s", desc, getFloatString(v.x),    getFloatString(v.y), getFloatString(v.z) );
        }
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,const    NxBodyDesc *b)
{
    if ( mFph )
    {
        // indicate whether it has a body or not
        char buff[128];
        sprintf( buff, "BodyDesc Present" );
        WriteBool( buff, b ? true : false );

        if ( b )
        {
            Print("body=DYNAMIC");
            ++mIndent;

            Write( "massLocalPose",                b->massLocalPose );
            Write( "massSpaceInertia",             b->massSpaceInertia );
            WriteFloat(    "mass",                       b->mass );
            Write( "linearVelocity",               b->linearVelocity );
            Write( "angularVelocity",              b->angularVelocity );
            WriteFloat(    "wakeUpCounter",           b->wakeUpCounter );
            WriteFloat(    "linearDamping",           b->linearDamping );
            WriteFloat(    "angularDamping",          b->angularDamping );
            WriteFloat(    "maxAngularVelocity",      b->maxAngularVelocity );
#if NX_SDK_VERSION_NUMBER >= 230
            WriteFloat(    "CCDMotionThreshold",      b->CCDMotionThreshold );
#endif
            WriteFloat(    "sleepLinearVelocity",     b->sleepLinearVelocity );
            WriteFloat(    "sleepAngularVelocity",       b->sleepAngularVelocity );
            WriteUnsigned( "solverIterationCount", b->solverIterationCount );

            if ( mBinary )
            {
                WriteFlag_b( b->flags );
            }
            else
            {
                PrintFlag( b->flags, "NX_BF_DISABLE_GRAVITY",NX_BF_DISABLE_GRAVITY );
                PrintFlag( b->flags, "NX_BF_FROZEN_POS_X",    NX_BF_FROZEN_POS_X );
                PrintFlag( b->flags, "NX_BF_FROZEN_POS_Y",    NX_BF_FROZEN_POS_Y );
                PrintFlag( b->flags, "NX_BF_FROZEN_POS_Z",    NX_BF_FROZEN_POS_Z );
                PrintFlag( b->flags, "NX_BF_FROZEN_ROT_X",    NX_BF_FROZEN_ROT_X );
                PrintFlag( b->flags, "NX_BF_FROZEN_ROT_Y",    NX_BF_FROZEN_ROT_Y );
                PrintFlag( b->flags, "NX_BF_FROZEN_ROT_Z",    NX_BF_FROZEN_ROT_Z );
                PrintFlag( b->flags, "NX_BF_KINEMATIC",        NX_BF_KINEMATIC );
                PrintFlag( b->flags, "NX_BF_VISUALIZATION",    NX_BF_VISUALIZATION );
            }

            --mIndent;
        }
        else
        {
            Print("body=NULL");
        }
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,const    NxMat34    &p)
{
    if ( mFph )
    {
        NxReal m[9];
        p.M.getRowMajor( m );

        if ( mBinary )
        {
            // k, write    out    the    matrix
            for    ( int i    = 0; i < 9;    ++i    )
            {
                WriteFloat_b( m[i] );
            }
            WriteFloat_b( p.t.x    );
            WriteFloat_b( p.t.y    );
            WriteFloat_b( p.t.z    );
        }
        else
        {
            NxReal m[9];
            p.M.getRowMajor(m);
            Print("%s=%s,%s,%s    %s,%s,%s  %s,%s,%s    %s,%s,%s", desc, getFloatString(m[0]), getFloatString(m[1]), getFloatString(m[2]),     getFloatString(m[3]), getFloatString(m[4]), getFloatString(m[5]),    getFloatString(m[6]), getFloatString(m[7]),    getFloatString(m[8]),  getFloatString(p.t.x), getFloatString(p.t.y), getFloatString(p.t.z) );
        }
    }
}

//==================================================================================
void CoreDump::PrintFlag(unsigned int state,const char *desc,unsigned int flag)
{
    if ( !mBinary )
    {
        if ( state & flag )
            Print("%s = true", desc );
        else
            Print("%s = false", desc );
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc, NxActor *a)
{
    if ( mFph )
    {
        NxActorDesc    actorDesc;
        NxBodyDesc descbody;
        NxBodyDesc *bodyDescPntr = 0;

        // print out description
        Print("%s",    desc);

        // indent
        ++mIndent;

        // k, does this    have a dynamic body?
        if ( a->saveBodyToDesc(descbody) )
        {
            // yes,    save off the body description
            bodyDescPntr = &descbody;
        }

        // get actor description
        a->saveToDesc( actorDesc );

#if NX_SDK_VERSION_NUMBER >= 230
        if ( bodyDescPntr && ( bodyDescPntr->CCDMotionThreshold < 0 ) )
        {
            bodyDescPntr->CCDMotionThreshold    = 0;
        }
#endif
        // k, time to write    all    information    - first    write matrix
        Write( "globalPose", actorDesc.globalPose );

        // write body description
        Write( "[body]", bodyDescPntr );

        // k, this is a    HACK - if you take a look in NxBodyDesc::isValidInternal(),    there is a check to    make sure
        // that    a body description does    NOT    have both a    mass and density (actually is a    little more    involved),
        // so we HACK it here to zero out the density if we    have a body    and    it has a mass
        float density =    actorDesc.density;
        if ( bodyDescPntr && ( bodyDescPntr->mass >    0 )    )
        {
            // gotta make sure the density is 0
            density    = 0;
        }

        WriteFloat(    "density", density );


        // write the actor's group
        WriteInt( "group", actorDesc.group );

        char buff[128];
        if ( mBinary )
        {
            // write flags
            WriteFlag_b( actorDesc.flags );

            // also place name in the buffer
            sprintf( buff, a->getName()    ? a->getName() : "null"    );
        }
        else
        {
            // write flags
            PrintFlag( actorDesc.flags,    "NX_AF_DISABLE_COLLISION", NX_AF_DISABLE_COLLISION );
            PrintFlag( actorDesc.flags,    "NX_AF_DISABLE_RESPONSE",  NX_AF_DISABLE_RESPONSE );

#if    NX_USE_FLUID_API
            PrintFlag( actorDesc.flags,    "NX_AF_FLUID_DISABLE_COLLISION", NX_AF_FLUID_DISABLE_COLLISION );
#endif              
            // also place name in the buffer
            sprintf( buff, "name=\"%s\"", a->getName() ? a->getName() :    "null" );
        }
        WriteString( buff );

        // now write the # of shapes
        NxU32 numActorShapes        = a->getNbShapes();
        NxShape* const*    actorShapes    = a->getShapes();
        WriteInt( "[NbShapes]",    numActorShapes );
        ++mIndent;

        // write out each shape
        for    ( NxU32    i=0; i<numActorShapes; ++i )
        {
            NxShape    *shape = actorShapes[i];
            Write("NxShape",shape);
        }
        --mIndent;
        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,const    NxShapeDesc    &d,const NxShape *shape)
{
    if ( mFph )
    {
        // write out local pose
        Write( "localPose",    d.localPose    );

        // write group, material index, mass, density and skin width
        WriteInt( "group", d.group );
        WriteInt( "materialIndex", d.materialIndex );
#if NX_SDK_VERSION_NUMBER >= 230
        WriteFloat(    "mass",    d.mass );
        WriteFloat(    "density", d.density );
#endif
        WriteFloat(    "skinWidth", d.skinWidth );

        // k, write out rest of data
        char scratch[218];
        if ( mBinary )
        {
            WriteFlag_b( d.shapeFlags );

#if NX_SDK_VERSION_NUMBER >= 230
            if ( shape->getCCDSkeleton() )
            {
                // get which it is!
                int index = GetSkeletonIndex( shape->getCCDSkeleton() );
                WriteInt_b( index );
            }
            else
            {
                WriteInt_b( -1 );
            }
#endif

#ifdef NX_SUPPORT_NEW_FILTERING
            //!< Groups    bitmask    for    collision filtering
            WriteUnsigned_b( d.groupsMask.bits0    );
            WriteUnsigned_b( d.groupsMask.bits1    );
            WriteUnsigned_b( d.groupsMask.bits2    );
            WriteUnsigned_b( d.groupsMask.bits3    );
#endif
            sprintf( scratch, shape->getName() ? shape->getName() :    "null" );
        }
        else
        {
            PrintFlag(d.shapeFlags,"NX_TRIGGER_ON_ENTER",    NX_TRIGGER_ON_ENTER    );
            PrintFlag(d.shapeFlags,"NX_TRIGGER_ON_LEAVE",    NX_TRIGGER_ON_LEAVE    );
            PrintFlag(d.shapeFlags,"NX_TRIGGER_ON_STAY",    NX_TRIGGER_ON_STAY );
            PrintFlag(d.shapeFlags,"NX_TRIGGER_ENABLE",    NX_TRIGGER_ENABLE );

            PrintFlag(d.shapeFlags,"NX_SF_VISUALIZATION",    NX_SF_VISUALIZATION    );
            PrintFlag(d.shapeFlags,"NX_SF_DISABLE_COLLISION",    NX_SF_DISABLE_COLLISION    );

            PrintFlag(d.shapeFlags,"NX_SF_FEATURE_INDICES",    NX_SF_FEATURE_INDICES );
            PrintFlag(d.shapeFlags,"NX_SF_DISABLE_RAYCASTING",    NX_SF_DISABLE_RAYCASTING );
            PrintFlag(d.shapeFlags,"NX_SF_POINT_CONTACT_FORCE",    NX_SF_POINT_CONTACT_FORCE );
            PrintFlag(d.shapeFlags,"NX_SF_DISABLE_RESPONSE",    NX_SF_DISABLE_RESPONSE );

#if    NX_USE_FLUID_API
            PrintFlag(d.shapeFlags,"NX_SF_FLUID_DRAIN",    NX_SF_FLUID_DRAIN );
            PrintFlag(d.shapeFlags,"NX_SF_FLUID_DISABLE_COLLISION",    NX_SF_FLUID_DISABLE_COLLISION );
#endif

#if NX_SDK_VERSION_NUMBER >= 230
            int index = -1;
            if ( shape->getCCDSkeleton() )
            {
                // get which it is!
                index = GetSkeletonIndex( shape->getCCDSkeleton() );
            }
            WriteInt( "ccdSkeleton", index );
#endif

#ifdef NX_SUPPORT_NEW_FILTERING
            Print("groupsMask=%d %d %d %d", d.groupsMask.bits0, d.groupsMask.bits1, d.groupsMask.bits2, d.groupsMask.bits3 );
#endif

            sprintf( scratch, "name=\"%s\"", shape->getName() ?    shape->getName() : "null" );
        }
        WriteString( scratch );
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc, const NxShape *_shape)
{

#if NX_SDK_VERSION_NUMBER >= 230
    const NxShape *shape = _shape;
#else
    NxShape *shape = (NxShape *) _shape; // in the 2.2 release the shape accessors were not const safe.
#endif

    if ( mFph )
    {
        // indicate whether it has a shape or not
        char buff[128];
        sprintf( buff, "ShapePresent" );
        WriteBool( buff, shape ? true : false );

        if ( shape )
        {
            Print("%s=%s", desc, getString(    shape->getType() ) );
            ++mIndent;

            // get the type
            NxShapeType    type = shape->getType();

            // should we write out the description?     For now am    NOT    doing so.
            //WriteString( "shapeDesc=", getString(    type ) );

            // k, got to write the type    first
            WriteInt( "type", type );
                    
            // now write out shape type    specific params
            switch(    type )
            {
                case NX_SHAPE_PLANE:
                {
                    const NxPlaneShape *current    = shape->isPlane();
                    NxPlaneShapeDesc desc;
                    assert(    current    );
                    current->saveToDesc( desc );
                    Write("PlaneShapeDesc",    desc, shape    );

                    if ( mBinary )
                    {
                        // now write plane shape desc specific params
                        WriteVec3_b( desc.normal );
                        WriteFloat_b( desc.d );
                    }
                    else
                    {
                        Print("plane=%s,%s,%s,%s", getFloatString(desc.normal.x), getFloatString(desc.normal.y), getFloatString(desc.normal.z),    getFloatString(desc.d) );
                    }
                }
                break;

                case NX_SHAPE_SPHERE:
                {
                    const NxSphereShape    *current = shape->isSphere();
                    NxSphereShapeDesc desc;
                    assert(    current    );
                    current->saveToDesc( desc );
                    Write("SphereShapeDesc", desc, shape );

                    WriteFloat(    "radius", desc.radius );
                }
                break;

                case NX_SHAPE_BOX:
                {
                    const NxBoxShape *current =    shape->isBox();
                    NxBoxShapeDesc desc;
                    assert(    current    );
                    current->saveToDesc( desc );
                    Write("BoxShapeDesc", desc,    shape );

                    // now write its dimensions
                    Write( "BoxDimensions",    desc.dimensions    );
                }
                break;

                case NX_SHAPE_CAPSULE:
                {
                    const NxCapsuleShape *current =    shape->isCapsule();
                    NxCapsuleShapeDesc desc;
                    assert(    current    );
                    current->saveToDesc( desc );
                    Write("CapsuleShapeDesc", desc,    shape );

                    WriteFloat(    "radius", desc.radius );
                    WriteFloat(    "height", desc.height );
                    if ( mBinary )
                    {
                        WriteFlag( "flags",      desc.flags );
                    }
                    else
                    {
                        PrintFlag( desc.flags, "NX_SWEPT_SHAPE", NX_SWEPT_SHAPE );
                    }
                }
                break;

                case NX_SHAPE_CONVEX:
                {
                    const NxConvexShape    *current = shape->isConvexMesh();
                    NxConvexShapeDesc desc;
                    assert(    current    );
                    current->saveToDesc( desc );
                    Write("ConvexShapeDesc", desc, shape );

                    // write its flags
                    if ( mBinary )
                    {
                        WriteInt( "meshFlags", desc.meshFlags );
                    }
                    else
                    {
                        PrintFlag( desc.meshFlags, "NX_MESH_SMOOTH_SPHERE_COLLISIONS", NX_MESH_SMOOTH_SPHERE_COLLISIONS );
                    }

#ifdef NX_SUPPORT_CONVEX_SCALE
                    // write scale
                    WriteFloat(    "scale", desc.scale    );
#endif

                    // k, get its index    into the array of convex meshes!
                    int    index =    GetConvexIndex(    desc.meshData );

                    if ( mBinary )
                    {
                        //WriteUnsigned_b( reinterpret_cast<int>( desc.meshData    ) );
                        WriteInt_b(    index );
                    }
                    else
                    {
                        if ( desc.meshData )
                            Print("meshData=ConvexMesh%d", index+1 );
                        else
                            Print("meshData=NULL");
                    }
                }
                break;

#if NX_SDK_VERSION_NUMBER >= 230
                // note that in the earlier versions of the SDK, NX_SHAPE_WHEEL did not exist
                case NX_SHAPE_WHEEL:
                {
                    const NxWheelShape *current = (const NxWheelShape *)shape->is( NX_SHAPE_WHEEL );
                    NxWheelShapeDesc desc;
                    assert( current );
                    current->saveToDesc( desc );

                    // write its "normal" description information
                    Write( "NxWheelShapeDesc", desc, shape );

                    // now write its simple individual items
                    WriteFloat( "radius",            desc.radius );
                    WriteFloat( "suspensionTravel", desc.suspensionTravel );
                    WriteFloat( "motorTorque",        desc.motorTorque );
                    WriteFloat( "brakeTorque",        desc.brakeTorque );
                    WriteFloat( "steerAngle",        desc.steerAngle );
                    WriteFloat( "inverseWheelMass", desc.inverseWheelMass );

                    if ( mBinary )
                    {
                        WriteInt( "wheelFlags",            desc.wheelFlags );
                    }
                    else
                    {
                        PrintFlag( desc.wheelFlags, "NX_WF_WHEEL_AXIS_CONTACT_NORMAL",    NX_WF_WHEEL_AXIS_CONTACT_NORMAL );
                        PrintFlag( desc.wheelFlags, "NX_WF_INPUT_LAT_SLIPVELOCITY",        NX_WF_INPUT_LAT_SLIPVELOCITY );
                        PrintFlag( desc.wheelFlags, "NX_WF_INPUT_LNG_SLIPVELOCITY",        NX_WF_INPUT_LNG_SLIPVELOCITY );
                        PrintFlag( desc.wheelFlags, "NX_WF_UNSCALED_SPRING_BEHAVIOR",    NX_WF_UNSCALED_SPRING_BEHAVIOR );
                        PrintFlag( desc.wheelFlags, "NX_WF_AXLE_SPEED_OVERRIDE",        NX_WF_AXLE_SPEED_OVERRIDE );
                    }

                    // write its spring desc (note this always has one!)
                    Write( "spring=TRUE",            &desc.suspension );
                    
                    // finally its NxTireFunctionDesc's (2 of them)
                    Write( "lateralTireForceFunc",        &desc.lateralTireForceFunction );
                    Write( "longitudinalTireForceFunc",    &desc.longitudalTireForceFunction );
                }
                break;
#endif
                case NX_SHAPE_MESH:
                {
                    const NxTriangleMeshShape *current = shape->isTriangleMesh();
                    NxTriangleMeshShapeDesc    desc;
                    assert(    current    );
                    current->saveToDesc( desc );
                    Write( "TriMeshShapeDesc", desc, shape );

                    // write its flags
                    if ( mBinary )
                    {
                        WriteInt( "meshFlags", desc.meshFlags );
                    }
                    else
                    {
                        PrintFlag( desc.meshFlags, "NX_MESH_SMOOTH_SPHERE_COLLISIONS", NX_MESH_SMOOTH_SPHERE_COLLISIONS );
                    }

#ifdef NX_SUPPORT_CONVEX_SCALE
                    // write scale
                    WriteFloat(    "scale", desc.scale    );
#endif

                    // k, get the index of this triangle mesh
                    int    index =    GetTriangleMeshIndex( desc.meshData    );

                    if ( mBinary )
                    {
                        WriteInt_b(    index );
                    }
                    else
                    {
                        if ( desc.meshData )
                            Print("meshData=TriangleMesh%d", index+1 );
                        else
                            Print("meshData=NULL");
                    }
                }
                break;

#if NX_SDK_VERSION_NUMBER >= 240
                case NX_SHAPE_HEIGHTFIELD:
                {
                    const NxHeightFieldShape *current = shape->isHeightField();
                    NxHeightFieldShapeDesc desc;
                    assert( current );
                    current->saveToDesc( desc );
                    Write( "HeightFieldShapeDesc", desc, shape );

                    // write its flags
                    if ( mBinary )
                    {
                        WriteInt( "HeightField_MeshFlags", desc.meshFlags );
                    }
                    else
                    {
                        PrintFlag( desc.meshFlags, "NX_MESH_SMOOTH_SPHERE_COLLISIONS", NX_MESH_SMOOTH_SPHERE_COLLISIONS );
                    }

#ifdef NX_SUPPORT_CONVEX_SCALE
                    // write scale
                    WriteFloat(    "HeightField_Scale", desc.scale    );
#endif

                    // now write its other info
                    WriteUnsigned( "HeightField_HoleMaterial",          desc.holeMaterial );
                    WriteUnsigned( "HeightField_MaterialIndexHighBits", desc.materialIndexHighBits );
                    WriteFloat( "HeightField_ColumnScale",              desc.columnScale );
                    WriteFloat( "HeightField_RowScale",                 desc.rowScale );
                    WriteFloat( "HeightField_HeightScale",              desc.heightScale );

                    int index = -1;
                    if ( desc.heightField )
                    {
                        index = GetHeightFieldIndex( desc.heightField );
                        assert( index >= 0 );
                    }
                    WriteInt( "HeightField_HeightFieldIndex", index );    
                }
                break;
            
                case NX_SHAPE_RAW_MESH:
                    assert(    0 );
                break;
#endif
                case NX_SHAPE_COMPOUND:
                    assert(    0 );
                break;

                default:
                    assert(0); //!!???
                break;
            }

            --mIndent;
        }
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc, NxPairFlag *p)
{
    if ( mFph && p )
    {
        Print("%s",    desc);
        ++mIndent;

        int    actorIndex1    = -1, shapeIndex1 =    -1;
        int    actorIndex2    = -1, shapeIndex2 =    -1;

        if ( p->isActorPair() )
        {
            const NxActor *a1    = (const NxActor *) p->objects[0];
            const NxActor *a2    = (const NxActor *) p->objects[1];
            actorIndex1            = GetActorIndex(a1);
            actorIndex2            = GetActorIndex(a2);
            assert(    actorIndex1 >= 0 );
            assert(    actorIndex2 >= 0 );
        }
        else
        {
            const NxShape *s1 = (const NxShape *) p->objects[0];
            const NxShape *s2 = (const NxShape *) p->objects[1];
            const NxActor *a1 = &s1->getActor();
            const NxActor *a2 = &s2->getActor();

            actorIndex1    = GetActorIndex(a1);
            actorIndex2    = GetActorIndex(a2);
            shapeIndex1 = GetShapeIndex(a1,s1);
            shapeIndex2 = GetShapeIndex(a2,s2);
    
            assert( actorIndex1 >= 0);    
            assert( actorIndex2 >= 0);
            assert( shapeIndex1 >= 0 );
            assert( shapeIndex2 >= 0 );
        }

        // write the flags
        if ( mBinary )
        {
            WriteFlag( "pairFlag", p->flags    );
        }
        else
        {
            bool isActorPair = (p->isActorPair() != 0);
            Print( "PAIR_FLAG_IS_ACTOR_PAIR = %s",        isActorPair ? "true" : "false" );
            Print( "PAIR_FLAG_NX_IGNORE_PAIR = %s",        (p->flags & NX_IGNORE_PAIR) ? "true" : "false" );
            Print( "PAIR_FLAG_NX_NOTIFY_ON_TOUCH = %s",    (p->flags & NX_NOTIFY_ON_TOUCH) ? "true" : "false" );
        }

        // now,    we need    pntrs to the actors    as well!
        WriteInt( "ActorIndex1", actorIndex1 );
        WriteInt( "ShapeIndex1", shapeIndex1 );
        WriteInt( "ActorIndex2", actorIndex2 );
        WriteInt( "ShapeIndex2", shapeIndex2 );

        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,NxSpringDesc *spring)
{
    if ( mFph )
    {
        Print("%s",    desc );
        ++mIndent;

        // indicate whether it has a spring desc or not
        char buff[128];
        sprintf( buff, "SpringDesc Present" );
        WriteBool( buff, spring ? true : false );

        if ( spring    )
        {
            WriteFloat(    "spring_spring", spring->spring    );
            WriteFloat(    "spring_damper", spring->damper    );
            WriteFloat(    "spring_targetValue", spring->targetValue );
        }

        --mIndent;          
    }
}

//==================================================================================
void CoreDump::Write(const char    *descrip, NxMaterial *m,bool hardwareScene)
{
    if ( mFph )
    {
        Print("%s",    descrip);
        ++mIndent;

        NxMaterialDesc desc;
        if ( m )
        {
          m->saveToDesc(desc);
            if ( hardwareScene )
            {
                desc.dynamicFriction = 0.5f;
                desc.staticFriction  = 0.5f;
                desc.restitution     = 0.2f;
            }
        }
        else
        {
            desc.staticFriction = 0.5f;
            desc.dynamicFriction = 0.5f;
            desc.restitution = 0.2f;
        }

        // write material props
        WriteFloat(    "dynamicFriction",    desc.dynamicFriction );
        WriteFloat(    "staticFriction",    desc.staticFriction    );
        WriteFloat(    "restitution",        desc.restitution );
        WriteFloat(    "dynamicFrictionV",    desc.dynamicFrictionV );
        WriteFloat(    "staticFrictionV",    desc.staticFrictionV );

        // write combine modes
        WriteInt( "frictionCombineMode",    desc.frictionCombineMode );
        WriteInt( "restitutionCombineMode",    desc.restitutionCombineMode    );

        if ( mBinary )
        {
            WriteFloat_b( desc.dirOfAnisotropy.x );
            WriteFloat_b( desc.dirOfAnisotropy.y );
            WriteFloat_b( desc.dirOfAnisotropy.z );
        
            // write the flags
            WriteFlag_b( desc.flags    );
        }
        else
        {
            Print("%s=%s,%s,%s", "dirOfAnisotropy", 
                getFloatString(desc.dirOfAnisotropy.x),    
                getFloatString(desc.dirOfAnisotropy.y),
                getFloatString(desc.dirOfAnisotropy.z));

            PrintFlag( desc.flags, "NX_MF_ANISOTROPIC", NX_MF_ANISOTROPIC );
        }

        // does    it have    a spring description?
#if NX_SDK_VERSION_NUMBER >= 230
        char scratch[512];
        if ( desc.spring )
        {
            sprintf( scratch, "spring=TRUE"    );
        }
        else
        {
            sprintf( scratch, "spring=NULL"    );
        }
        Write( scratch,    desc.spring    );
#endif
        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write(NxJointDesc *d, NxJoint *j)
{
    if ( mFph )
    {
        Print("[NxJointDesc]");
        ++mIndent;

        // write joint type
        int jointType = d->getType();
        if ( mBinary )
        {
            WriteInt_b(    jointType );
        }
        else
        {
            Print("type=%s", getString(    (NxJointType)jointType ) );
        }

        // write actor indexes
        int actorIndex1 = -1, actorIndex2 = -1;
        actorIndex1 = mBinary ? GetActorIndex(d->actor[0]) : GetActorIndex(d->actor[0])+1;
        actorIndex2 = mBinary ? GetActorIndex(d->actor[1]) : GetActorIndex(d->actor[1])+1;

        WriteInt( "actorIndex1", actorIndex1 );
        WriteInt( "actorIndex2", actorIndex2 );
            
        // write normal, axis, anchor (both    1 and 2)
        Write("localNormal1", d->localNormal[0]    );
        Write("localAxis1",      d->localAxis[0] );
        Write("localAnchor1", d->localAnchor[0]    );

        Write("localNormal2", d->localNormal[1]    );
        Write("localAxis2",      d->localAxis[1] );
        Write("localAnchor2", d->localAnchor[1]    );

        // write force and torque
        WriteFloat(    "maxForce",    d->maxForce    );
        WriteFloat(    "maxTorque", d->maxTorque );

        if ( mBinary )
        {
            // write its name
            WriteString( j->getName() );

            // write its flags
            WriteFlag( "joint flags", d->jointFlags    );
        }
        else
        {
#if    0
            Print("name=%s", d->name );
#else
            Print("name=\"%s\"", j->getName() );
#endif

            PrintFlag(d->jointFlags, "NX_JF_COLLISION_ENABLED",    NX_JF_COLLISION_ENABLED    );
            PrintFlag(d->jointFlags, "NX_JF_VISUALIZATION",    NX_JF_VISUALIZATION    );
        }

        // in addition, we also have to write out its limit planes!
        j->resetLimitPlaneIterator();
        if ( j->hasMoreLimitPlanes() )
        {
            NxVec3 planeNormal;
            NxVec3 planeLimitPt;
            NxVec3 worldLimitPt;
            NxReal planeD;
            int numLimitPlanes = 0;
            char buff[128];
            bool ok = true;

            while ( ok )
            {
                j->getNextLimitPlane( planeNormal, planeD );
                ++numLimitPlanes;
                ok = j->hasMoreLimitPlanes();
            }
            // k, write # of limit planes, then write them!
            WriteInt( "NumLimitPlanes", numLimitPlanes );

            // write limit point
            bool onActor2 = j->getLimitPoint( planeLimitPt );
            Write( "PlaneLimitPoint", planeLimitPt );
            WriteBool( "PlaneLimitPointOnActor2", onActor2 );

            // SRM : k, get the actor positions!
            NxVec3 actorPos1(0,0,0), actorPos2(0,0,0);
            if ( d->actor[0] )
            {
                NxActorDesc    actorDesc;
                d->actor[0]->saveToDesc( actorDesc );
                actorPos1.x = actorDesc.globalPose.t.x;
                actorPos1.y = actorDesc.globalPose.t.y;
                actorPos1.z = actorDesc.globalPose.t.z;
            }
            if ( d->actor[1] )
            {
                NxActorDesc    actorDesc;
                d->actor[1]->saveToDesc( actorDesc );
                actorPos2.x = actorDesc.globalPose.t.x;
                actorPos2.y = actorDesc.globalPose.t.y;
                actorPos2.z = actorDesc.globalPose.t.z;
            }

            // write the plane normals
            j->resetLimitPlaneIterator();
            for ( int iter = 0; iter < numLimitPlanes; ++iter )
            {
                j->getNextLimitPlane( planeNormal, planeD );
                sprintf( buff, "[LimitPlane%d]", iter+1);
                Print( buff );
                ++mIndent;
                sprintf( buff, "planeNormal%d", iter+1 );
                Write( buff, planeNormal );
                sprintf( buff, "planeD%d", iter+1 );
                WriteFloat( buff, planeD );

                // we have to determine the world limit point
                // note that planeD = -(planeNormal) DOT (worldLimitPt)
                // try choosing x,y = 0 for world limit pt, which makes it
                // worldLimitPt.z = planeD / -planeNormal.z
                if ( jointType == NX_JOINT_REVOLUTE )
                {
                    if ( ( fabs(planeNormal.z) > fabs(planeNormal.x) ) &&
                         ( fabs(planeNormal.z) > fabs(planeNormal.y) ) )
                    {
                        worldLimitPt.x = planeLimitPt.x;
                        worldLimitPt.y = planeLimitPt.y;
                        worldLimitPt.z = onActor2 ? actorPos1.z : actorPos2.z;
                    }
                    // k, that didn't work - try x,z = 0
                    else if ( ( fabs(planeNormal.y) > fabs(planeNormal.x) ) &&
                              ( fabs(planeNormal.y) > fabs(planeNormal.z) ) )
                    {
                        worldLimitPt.x = planeLimitPt.x;
                        worldLimitPt.z = planeLimitPt.z;
                        worldLimitPt.y = onActor2 ? actorPos1.y : actorPos2.y;
                    }
                    else
                    {
                        worldLimitPt.y = planeLimitPt.y;
                        worldLimitPt.z = planeLimitPt.z;
                        worldLimitPt.x = onActor2 ? actorPos1.x : actorPos2.x;
                    }
                }
                else
                {
                    if ( ( fabs(planeNormal.z) > fabs(planeNormal.x) ) &&
                        ( fabs(planeNormal.z) > fabs(planeNormal.y) ) )
                    {
                        worldLimitPt.x = planeLimitPt.x;
                        worldLimitPt.y = planeLimitPt.y;
                        worldLimitPt.z = -planeD / planeNormal.z;
                    }
                    // k, that didn't work - try x,z = 0
                    else if ( ( fabs(planeNormal.y) > fabs(planeNormal.x) ) &&
                                ( fabs(planeNormal.y) > fabs(planeNormal.z) ) )
                    {
                        worldLimitPt.x = planeLimitPt.x;
                        worldLimitPt.z = planeLimitPt.z;
                        worldLimitPt.y = -planeD / planeNormal.y;
                    }
                    else
                    {
                        worldLimitPt.y = planeLimitPt.y;
                        worldLimitPt.z = planeLimitPt.z;
                        worldLimitPt.x = -planeD / planeNormal.x;
                    }
                }
                sprintf( buff, "worldLimitPt%d", iter+1 );
                Write( buff, worldLimitPt );
                --mIndent;
            }
        }
        else
        {
            WriteInt( "NumLimitPlanes", 0 );
        }

        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write( NxPrismaticJointDesc *desc, NxJoint *j )
{
    Print("[NxPrismaticJointDesc]");
    ++mIndent;

    // always save out basic joint info    - that is ALL for this joint!
    Write( (NxJointDesc    *)desc,    j );

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxRevoluteJointDesc *desc, NxJoint *j    )
{
    Print("[NxRevoluteJointDesc]");
    ++mIndent;

    // always save out basic joint info
    Write( (NxJointDesc    *)desc,    j );

    // now save    out    specific information - start with low/high limits
    WriteFloat(    "limit_low_hardness",      desc->limit.low.hardness );
    WriteFloat(    "limit_low_restitution",  desc->limit.low.restitution );
    WriteFloat(    "limit_low_value",          desc->limit.low.value    );
    WriteFloat(    "limit_high_hardness",      desc->limit.high.hardness    );
    WriteFloat(    "limit_high_restitution", desc->limit.high.restitution );
    WriteFloat(    "limit_high_value",          desc->limit.high.value );

    // save    out    motor description
    WriteBool( "motor_freeSpin",      desc->motor.freeSpin ? true :    false );
    WriteFloat(    "motor_maxForce",      desc->motor.maxForce );
    WriteFloat(    "motor_velTarget",      desc->motor.velTarget    );

    // save    out    spring desc
    WriteFloat(    "spring_spring",      desc->spring.spring );
    WriteFloat(    "spring_damper",      desc->spring.damper );
    WriteFloat(    "spring_targetValue", desc->spring.targetValue );

    // save    out    reset
    WriteFloat(    "projectionDistance", desc->projectionDistance );
    WriteFloat(    "projectionAngle",      desc->projectionAngle    );

    if ( mBinary )
    {
        WriteFlag( "flags", desc->flags );
        WriteUnsigned( "projectionMode", desc->projectionMode );
    }
    else
    {
        // flags first
        PrintFlag( desc->flags, "NX_RJF_LIMIT_ENABLED", NX_RJF_LIMIT_ENABLED );
        PrintFlag( desc->flags, "NX_RJF_MOTOR_ENABLED", NX_RJF_MOTOR_ENABLED );
        PrintFlag( desc->flags, "NX_RJF_SPRING_ENABLED", NX_RJF_SPRING_ENABLED );

        // now projection mode stuff
        if ( desc->projectionMode == NX_JPM_NONE )
        {
            Print( "projectionMode = NX_JPM_NONE" );
        }
        else if    ( desc->projectionMode == NX_JPM_POINT_MINDIST )
        {
            Print( "projectionMode = NX_JPM_POINT_MINDIST" );
        }
        else
        {
            Print( "projectionMode = ??Unknown??" );
        }
    }

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxCylindricalJointDesc *desc,    NxJoint    *j )
{
    Print("[NxCylindricalJointDesc]");
    ++mIndent;

    // always save out basic joint info    - that is ALL for this joint!
    Write( (NxJointDesc    *)desc,    j );

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxSphericalJointDesc *desc, NxJoint *j )
{
    Print("[NxSphericalJointDesc]");
    ++mIndent;

    // always save out basic joint info
    Write( (NxJointDesc    *)desc,    j );

    // save    out    spring descs
    WriteFloat(    "twistSpring_spring",       desc->twistSpring.spring    );
    WriteFloat(    "twistSpring_damper",       desc->twistSpring.damper    );
    WriteFloat(    "twistSpring_targetValue", desc->twistSpring.targetValue );
    WriteFloat(    "swingSpring_spring",       desc->swingSpring.spring    );
    WriteFloat(    "swingSpring_damper",       desc->swingSpring.damper    );
    WriteFloat(    "swingSpring_targetValue", desc->swingSpring.targetValue );
    WriteFloat(    "jointSpring_spring",       desc->jointSpring.spring    );
    WriteFloat(    "jointSpring_damper",       desc->jointSpring.damper    );
    WriteFloat(    "jointSpring_targetValue", desc->jointSpring.targetValue );

    // projection dist
    WriteFloat(    "projectionDistance", desc->projectionDistance );

    // other limits    (twist and swing)
    WriteFloat(    "twistLimit_low_hardness",       desc->twistLimit.low.hardness );
    WriteFloat(    "twistLimit_low_restitution",  desc->twistLimit.low.restitution    );
    WriteFloat(    "twistLimit_low_value",           desc->twistLimit.low.value );
    WriteFloat(    "twistLimit_high_hardness",       desc->twistLimit.high.hardness );
    WriteFloat(    "twistLimit_high_restitution", desc->twistLimit.high.restitution );
    WriteFloat(    "twistLimit_high_value",       desc->twistLimit.high.value );

    WriteFloat(    "swingLimit_hardness",           desc->swingLimit.hardness );
    WriteFloat(    "swingLimit_restitution",       desc->swingLimit.restitution    );
    WriteFloat(    "swingLimit_value",               desc->swingLimit.value );

    if ( mBinary )
    {
        // flags
        WriteFlag( "flags", desc->flags );

        // swing axis
        WriteFloat(    "swingAxis.x", desc->swingAxis.x );
        WriteFloat(    "swingAxis.y", desc->swingAxis.y );
        WriteFloat(    "swingAxis.z", desc->swingAxis.z );

        // projection mode
        WriteUnsigned( "projectionMode", desc->projectionMode );
    }
    else
    {
        // flags first
        PrintFlag( desc->flags, "NX_SJF_TWIST_LIMIT_ENABLED", NX_SJF_TWIST_LIMIT_ENABLED );
        PrintFlag( desc->flags, "NX_SJF_SWING_LIMIT_ENABLED", NX_SJF_SWING_LIMIT_ENABLED );
        PrintFlag( desc->flags, "NX_SJF_TWIST_SPRING_ENABLED", NX_SJF_TWIST_SPRING_ENABLED );
        PrintFlag( desc->flags, "NX_SJF_SWING_SPRING_ENABLED", NX_SJF_SWING_SPRING_ENABLED );
        PrintFlag( desc->flags, "NX_SJF_JOINT_SPRING_ENABLED", NX_SJF_JOINT_SPRING_ENABLED );

        // swing axis
        Print( "swingAxis =    (%f, %f, %f)", desc->swingAxis.x, desc->swingAxis.y, desc->swingAxis.z );

        // projection mode
        if ( desc->projectionMode == NX_JPM_NONE )
        {
            Print( "projectionMode = NX_JPM_NONE" );
        }
        else if    ( desc->projectionMode == NX_JPM_POINT_MINDIST )
        {
            Print( "projectionMode = NX_JPM_POINT_MINDIST" );
        }
        else
        {
            Print( "projectionMode = ??Unknown??" );
        }
    }

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxPointOnLineJointDesc *desc,    NxJoint    *j )
{
    Print("[NxPointOnLineJointDesc]");
    ++mIndent;

    // always save out basic joint info    - that is ALL for this joint!
    Write( (NxJointDesc    *)desc,    j );

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxPointInPlaneJointDesc *desc, NxJoint *j    )
{
    Print("[NxPointInPlaneJointDesc]");
    ++mIndent;

    // always save out basic joint info    - that is ALL for this joint!
    Write( (NxJointDesc    *)desc,    j );

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxDistanceJointDesc *desc, NxJoint *j    )
{
    Print("[NxDistanceJointDesc]");
    ++mIndent;

    // always save out basic joint info
    Write( (NxJointDesc    *)desc,    j );

    // min/max dist
    WriteFloat(    "minDistance", desc->minDistance );
    WriteFloat(    "maxDistance", desc->maxDistance );

    // flags
    if ( mBinary )
    {
        WriteFlag( "flags",    desc->flags    );
    }
    else
    {
        PrintFlag( desc->flags, "NX_DJF_MAX_DISTANCE_ENABLED", NX_DJF_MAX_DISTANCE_ENABLED );
        PrintFlag( desc->flags, "NX_DJF_MIN_DISTANCE_ENABLED", NX_DJF_MIN_DISTANCE_ENABLED );
        PrintFlag( desc->flags, "NX_DJF_SPRING_ENABLED", NX_DJF_SPRING_ENABLED );
    }

    // spring desc
    WriteFloat(    "spring_spring",      desc->spring.spring );
    WriteFloat(    "spring_damper",      desc->spring.damper );
    WriteFloat(    "spring_targetValue", desc->spring.targetValue );

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxPulleyJointDesc    *desc, NxJoint *j )
{
     Print("[NxPulleyJointDesc]");
    ++mIndent;

    // always save out basic joint info
    Write( (NxJointDesc    *)desc,    j );

    // params
    WriteFloat(    "distance",     desc->distance    );
    WriteFloat(    "stiffness", desc->stiffness );
    WriteFloat(    "ratio",     desc->ratio );

    if ( mBinary )
    {
        WriteFlag( "flags",         desc->flags );
    }
    else
    {
        PrintFlag( desc->flags, "NX_PJF_IS_RIGID", NX_PJF_IS_RIGID );
        PrintFlag( desc->flags, "NX_PJF_MOTOR_ENABLED", NX_PJF_MOTOR_ENABLED );
    }

    // motor desc
    WriteBool( "motor_freeSpin",   desc->motor.freeSpin    ? true : false );
    WriteFloat(    "motor_maxForce",  desc->motor.maxForce    );
    WriteFloat(    "motor_velTarget", desc->motor.velTarget );

    // and pulley info
    if ( mBinary )
    {
        WriteFloat(    "pulley0.x", desc->pulley[0].x );
        WriteFloat(    "pulley0.y", desc->pulley[0].y );
        WriteFloat(    "pulley0.z", desc->pulley[0].z );
        WriteFloat(    "pulley1.x", desc->pulley[1].x );
        WriteFloat(    "pulley1.y", desc->pulley[1].y );
        WriteFloat(    "pulley1.z", desc->pulley[1].z );
    }
    else
    {
        Print( "desc->pulley0 =    (%f, %f, %f)", desc->pulley[0].x, desc->pulley[0].y, desc->pulley[0].z );
        Print( "desc->pulley1 =    (%f, %f, %f)", desc->pulley[1].x, desc->pulley[1].y, desc->pulley[1].z );
    }

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxFixedJointDesc *desc, NxJoint *j )
{
    Print("[NxFixedJointDesc]");
    ++mIndent;

   // always save out basic    joint info - that is ALL for this joint!
    Write( (NxJointDesc    *)desc,    j );

    --mIndent;
}

//==================================================================================
void CoreDump::Write( NxD6JointDesc    *desc, NxJoint *j )
{
    Print( "[NxD6JointDesc]" );
    ++mIndent;

    // always save out basic joint info
    Write( (NxJointDesc    *)desc,    j );

    // now save    out    joint specific info
    WriteUnsigned( "xMotion",          desc->xMotion    );
    WriteUnsigned( "yMotion",          desc->yMotion    );
    WriteUnsigned( "zMotion",          desc->zMotion    );
    WriteUnsigned( "swing1Motion",      desc->swing1Motion );
    WriteUnsigned( "swing2Motion",      desc->swing2Motion );
    WriteUnsigned( "twistMotion",      desc->twistMotion    );

    Write( "[linearLimit]",           desc->linearLimit );
    Write( "[swing1Limit]",           desc->swing1Limit );
    Write( "[swing2Limit]",           desc->swing2Limit );
    Write( "[twistLimit]",           desc->twistLimit    );
    Write( "[xDrive]",               desc->xDrive    );
    Write( "[yDrive]",               desc->yDrive    );
    Write( "[zDrive]",               desc->zDrive    );
    Write( "[swingDrive]",           desc->swingDrive    );
    Write( "[twistDrive]",           desc->twistDrive    );
    Write( "[slerpDrive]",           desc->slerpDrive    );
    Write( "drivePosition",           desc->drivePosition );
    Write( "driveOrientation",       desc->driveOrientation );
    Write( "driveLinearVelocity",  desc->driveLinearVelocity );
    Write( "driveAngularVelocity", desc->driveAngularVelocity );

    WriteUnsigned( "projectionMode",  desc->projectionMode );
    WriteFloat(    "projectionDistance", desc->projectionDistance );
    WriteFloat(    "projectionAngle",      desc->projectionAngle    );
    WriteFloat(    "gearRatio",          desc->gearRatio );

    if ( mBinary )
    {
        // change to write flag
        WriteFlag_b( desc->flags );
    }
    else
    {
        PrintFlag( desc->flags,    "NX_D6JOINT_SLERP_DRIVE", NX_D6JOINT_SLERP_DRIVE );
        PrintFlag( desc->flags,    "NX_D6JOINT_GEAR_ENABLED", NX_D6JOINT_GEAR_ENABLED );
    }
    --mIndent;
}

//==================================================================================
void CoreDump::Write( const    char *desc,    const NxJointLimitSoftDesc &l )
{
    if ( mFph )
    {
        Print("%s",    desc );
        ++mIndent;

        WriteFloat("values",      l.value );
        WriteFloat("restitution", l.restitution    );
        WriteFloat("spring",      l.spring );
        WriteFloat("damping",      l.damping    );

        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,const    NxJointDriveDesc &d)
{
    if ( mFph )
    {
        Print("%s",    desc );
        ++mIndent;

        WriteFloat(    "spring", d.spring );
        WriteFloat(    "damping", d.damping );
        WriteFloat(    "forceLimit", d.forceLimit );

        if ( mBinary )
        {
            WriteFlag_b( d.driveType.bitField );
        }
        else
        {
            if ( d.driveType.getFlagMask(NX_D6JOINT_DRIVE_POSITION)    )
                Print("NX_D6JOINT_DRIVE_POSITION=true");
            else
                Print("NX_D6JOINT_DRIVE_POSITION=false");

            if ( d.driveType.getFlagMask(NX_D6JOINT_DRIVE_VELOCITY)    )
                Print("NX_D6JOINT_DRIVE_VELOCITY=true");
            else
                Print("NX_D6JOINT_DRIVE_VELOCITY=false");
        }

        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write( const char *desc, NxScene &scene, NxEffector &e )
{
    if ( mFph )
    {
        Print("%s",    desc );
        ++mIndent;

        // k, which scene does this belong to (uh, it better belong to the one
        // that we asked it for!)?
        NxScene &nxscene = e.getScene();
        assert( &nxscene == &scene );
        
        // now, is this a spring and damper effector?
        NxSpringAndDamperEffector *sade = e.isSpringAndDamperEffector();
        WriteBool( "isSpringAndDamperEffector", (sade != 0) );
        if ( sade )
        {
            // k, get the spring and damper effector desc
            NxSpringAndDamperEffectorDesc desc;
            desc.setToDefault();

            // get the effector description
#if NX_SDK_VERSION_NUMBER >= 230
            sade->saveToDesc( desc );
#else
#pragma message("There was not saveToDesc method in 2.2, we need to do this manually")
#endif

            // get the actors from the pntrs
            int actorIndex1 = -1, actorIndex2 = -1;
            if ( desc.body1 )
            {
                actorIndex1 = GetActorIndex( desc.body1 );
            }
            if ( desc.body2 )
            {
                actorIndex2 = GetActorIndex( desc.body2 );
            }

            // save out actor indexes
            WriteInt( "actorIndex1", actorIndex1 );
            WriteInt( "actorIndex2", actorIndex2 );

            // k, we currently cannot write who these things are connected
            // to, but we will write out their world matrices
            Write( "jointConnectionPos1", desc.pos1 );
            Write( "jointConnectionPos2", desc.pos2 );

            // k, now we write information about the linear spring
            Print( "LinearSpringInfo" );
            ++mIndent;
            WriteFloat( "distCompressSaturate", desc.springDistCompressSaturate );
            WriteFloat( "distRelaxed",         desc.springDistRelaxed );
            WriteFloat( "distStretchSaturate", desc.springDistStretchSaturate );
            WriteFloat( "maxCompressForce",    desc.springMaxCompressForce );
            WriteFloat( "maxStretchForce",     desc.springMaxStretchForce );
            --mIndent;

            // k, now we write information about the linear damper
            Print( "LinearDamperInfo" );
            ++mIndent;
            WriteFloat( "velCompressSaturate", desc.damperVelCompressSaturate );
            WriteFloat( "velStretchSaturate",  desc.damperVelStretchSaturate );
            WriteFloat( "maxCompressForce",    desc.damperMaxCompressForce );
            WriteFloat( "maxStretchForce",     desc.damperMaxStretchForce );
            --mIndent;
        }

        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc,const    NxJointLimitSoftPairDesc &l)
{
    if ( mFph )
    {
        Print("%s",    desc );

        ++mIndent;
        Write("[low]", l.low );
        Write("[high]",    l.high );
        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write(const char    *desc, NxJoint *j)
{
    if ( mFph && j )
    {
        Print( "%s", desc );
        ++mIndent;

        NxJointType    type = j->getType();

        switch ( type )
        {
            case NX_JOINT_PRISMATIC:
            {
                NxPrismaticJoint *jt = j->isPrismaticJoint();
                assert(jt);
                NxPrismaticJointDesc d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_REVOLUTE:
            {
                NxRevoluteJoint    *jt    = j->isRevoluteJoint();
                assert(jt);
                NxRevoluteJointDesc    d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_CYLINDRICAL:
            {
                NxCylindricalJoint *jt = j->isCylindricalJoint();
                assert(jt);
                NxCylindricalJointDesc d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_SPHERICAL:
            {
                NxSphericalJoint *jt = j->isSphericalJoint();
                assert(jt);
                NxSphericalJointDesc d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_POINT_ON_LINE:
            {
                NxPointOnLineJoint *jt = j->isPointOnLineJoint();
                assert(jt);
                NxPointOnLineJointDesc d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_POINT_IN_PLANE:
            {
                NxPointInPlaneJoint    *jt    = j->isPointInPlaneJoint();
                assert(jt);
                NxPointInPlaneJointDesc    d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_DISTANCE:
            {
                NxDistanceJoint    *jt    = j->isDistanceJoint();
                assert(jt);
                NxDistanceJointDesc    d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_PULLEY:
            {
                NxPulleyJoint *jt =    j->isPulleyJoint();
                assert(jt);
                NxPulleyJointDesc d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_FIXED:
            {
                NxFixedJoint *jt = j->isFixedJoint();
                assert(jt);
                NxFixedJointDesc d;
                jt->saveToDesc(d);
                Write( &d, j );
            }
            break;

            case NX_JOINT_D6:
            {
                NxD6Joint *jt =    j->isD6Joint();
                assert(jt);
                NxD6JointDesc d;
                jt->saveToDesc(d);
                Write( &d, j );                   
            }
            break;

            default:
                assert(    false );
            break;
        }         

        --mIndent;
    }
}

//==================================================================================
void CoreDump::Print(const char    *fmt,...)
{
    if ( !mBinary )
    {
        char wbuff[2048];
        vsprintf(wbuff,    fmt, (char *)(&fmt+1));

        for    (int i=0; i<mIndent; i++)
            fprintf(mFph,"    ");
  
        fprintf(mFph,"%s\r\n", wbuff );
    }
}

//==================================================================================
#if NX_SDK_VERSION_NUMBER >= 230
bool CoreDump::AddSkeleton(NxCCDSkeleton *skeleton)
{
    int    count =    mSkeletons.size();
    NxCCDSkeleton **list = &mSkeletons[0];
    for    (int i=0; i<count; i++)
    {
        if ( skeleton == list[i] ) 
        {
            return false; //    not    new    to the list
        }
    }

    mSkeletons.push_back(skeleton);
    return true;
}
#endif

//==================================================================================
bool CoreDump::AddConvex(NxConvexMesh *mesh)
{
    int    count =    mConvexMeshes.size();
    NxConvexMesh **list    = &mConvexMeshes[0];
    for    (int i=0; i<count; i++)
    {
        if ( mesh == list[i] ) 
        {
            return false; //    not    new    to the list
        }
    }

    mConvexMeshes.push_back(mesh);

    return true;
}

//==================================================================================
bool CoreDump::AddTriangleMesh(NxTriangleMesh *mesh)
{
    int    count =    mTriangleMeshes.size();
    NxTriangleMesh **list =    &mTriangleMeshes[0];
    for    (int i=0; i<count; i++)
    {
        if ( mesh == list[i] ) 
        {
            return false; //    not    new    to the list
        }
    }

    mTriangleMeshes.push_back(mesh);

    return true;
}

//==================================================================================
#if NX_SDK_VERSION_NUMBER >= 240
bool CoreDump::AddHeightField( NxHeightField *hf )
{
    int    count =    mHeightFields.size();
    NxHeightField **list = &mHeightFields[0];
    for    (int i=0; i<count; i++)
    {
        if ( hf == list[i] ) 
        {
            return false; //    not    new    to the list
        }
    }

    mHeightFields.push_back(hf);

    return true;
}

//==================================================================================
int CoreDump::GetHeightFieldIndex( const NxHeightField *hf )
{
    if ( hf )
    {
        int    count =    mHeightFields.size();
        NxHeightField **list = &mHeightFields[0];
        for    (int i=0; i<count; i++)
        {
            if ( hf == list[i] ) 
            {
                return i;
            }
        }

        assert(0);
    }

    return -1;
}
#endif // #if NX_SDK_VERSION_NUMBER >= 240

//==================================================================================
#if NX_SDK_VERSION_NUMBER >= 230
int    CoreDump::GetSkeletonIndex(const NxCCDSkeleton *skeleton)
{
    if ( skeleton )
    {
        int    count =    mSkeletons.size();
        NxCCDSkeleton **list = &mSkeletons[0];
        for    (int i=0; i<count; i++)
        {
            if ( skeleton == list[i] ) 
            {
                return i;
            }
        }

        assert(0);
    }

    return -1;
}
#endif

//==================================================================================
int    CoreDump::GetConvexIndex(const NxConvexMesh    *mesh)
{
    if ( mesh )
    {
        int    count =    mConvexMeshes.size();

        NxConvexMesh **list    = &mConvexMeshes[0];

        for    (int i=0; i<count; i++)
        {
            if ( mesh == list[i] ) 
            {
                return i;
            }
        }

        assert(0);
    }

    return -1;
}

//==================================================================================
int    CoreDump::GetTriangleMeshIndex(const NxTriangleMesh    *mesh)
{
    if ( mesh )
    {
        int    count =    mTriangleMeshes.size();

        NxTriangleMesh **list =    &mTriangleMeshes[0];

        for    (int i=0; i<count; i++)
        {
            if ( mesh == list[i] ) 
            {
                return i;
            }
        }

        assert(0);
    }

    return -1;
}

//==================================================================================
void CoreDump::Write(const char    *d, const NxTriangleMesh *mesh)
{
    unsigned i;

    if ( mesh )
    {
        Print("%s",    d );
        ++mIndent;

        if ( mBinary )
        {
            // write its index
            int    index =    GetTriangleMeshIndex( mesh );
            WriteInt_b(    index );
        }

        NxTriangleMeshDesc desc;
        mesh->saveToDesc( desc );

        // output its stride index
        WriteInt( "materialIndexStride", desc.materialIndexStride );
        if ( desc.materialIndices )
        {
            // first write out the number of them!
            WriteInt( "NumMaterialIndices", desc.numTriangles );

            Print( "[Material Indices]" );
            ++mIndent;

            char buff[32];
            if ( desc.materialIndexStride == 4 )
            {
                int *matPntr = (int *)desc.materialIndices;

                // k, actually write out the indices!!!
                for ( i = 0; i < desc.numTriangles; ++i )
                {
                    // k, now we write out the indices!
                    int val = *matPntr;
                    ++matPntr;
                    sprintf( buff, "matIndex%d", i+1 );
                    WriteInt( buff, val );
                }
            }
            else if ( desc.materialIndexStride == 2 )
            {
                short *matPntr = (short *)desc.materialIndices;

                // k, actually write out the indices!!!
                for ( i = 0; i < desc.numTriangles; ++i )
                {
                    // k, now we write out the indices!
                    short val = *matPntr;
                    ++matPntr;
                    sprintf( buff, "matIndex%d", i+1 );
                    WriteShort( buff, val );
                }
            }
            else if ( desc.materialIndexStride == 1 )
            {
                char *matPntr = (char *)desc.materialIndices;

                // k, actually write out the indices!!!
                for ( i = 0; i < desc.numTriangles; ++i )
                {
                    // k, now we write out the indices!
                    char val = *matPntr;
                    ++matPntr;
                    sprintf( buff, "matIndex%d", i+1 );
                    WriteChar( buff, val, true );
                }
            }
            else
            {
                // uh, what?
                assert( 0 );
            }
            --mIndent;
        }
        else
        {
            if ( mBinary )
            {
                WriteUnsigned_b( 0 );
            }
            else
            {
                Print("materialIndices=NULL" );
            }
        }

        // write its height    field vertical axis    type
        WriteInt( "heightFieldVerticalAxis",     desc.heightFieldVerticalAxis );
        WriteFloat(    "heightFieldVerticalExtent", desc.heightFieldVerticalExtent    );
        WriteFloat(    "convexEdgeThreshold",       desc.convexEdgeThreshold    );

        // write its flags
        if ( mBinary )
        {
            WriteFlag_b( desc.flags    );
        }
        else
        {
            PrintFlag(desc.flags, "NX_MF_FLIPNORMALS",    NX_MF_FLIPNORMALS );
            PrintFlag(desc.flags, "NX_MF_16_BIT_INDICES", NX_MF_16_BIT_INDICES );
        }

        // write num verts and stride bytes
        WriteInt( "numVertices",      desc.numVertices );
        WriteInt( "pointStrideBytes", desc.pointStrideBytes    );

        // write the verts
        ++mIndent;
        const char *vbase =    (const char    *)desc.points;
        for    ( i    = 0; i < desc.numVertices; ++i )
        {
            if ( desc.pointStrideBytes == 3*sizeof(float) )
            {
                const float    *p = (const    float *)&vbase[i * desc.pointStrideBytes];

                // write each vert (remember, xyz)
                if ( mBinary )
                {
                    WriteFloat_b( p[0] );
                    WriteFloat_b( p[1] );
                    WriteFloat_b( p[2] );
                }
                else
                {
                    Print("V%d=(%s,%s,%s)",    i+1, getFloatString(p[0]), getFloatString(p[1]), getFloatString(p[2]) );
                }
            }
            else if ( desc.pointStrideBytes == 3*sizeof(double) )
            {
                const double *p = (const double *)&vbase[i * desc.pointStrideBytes];

                // write each vert (remember, xyz)
                if ( mBinary )
                {
                    WriteDouble_b( p[0] );
                    WriteDouble_b( p[1] );
                    WriteDouble_b( p[2] );
                }
                else
                {
                    Print("V%d=(%s,%s,%s)",    i+1, getDoubleString(p[0]), getDoubleString(p[1]), getDoubleString(p[2]) );
                }
            }
        }
        --mIndent;

        // write num triangles and stride bytes
        WriteInt( "numTriangles",         desc.numTriangles );
        WriteInt( "triangleStrideBytes", desc.triangleStrideBytes );

        // write the triangles
        ++mIndent;
        if ( mBinary )
        {
            if ( desc.triangleStrideBytes/3    == 1 )
            {
                const char *tris = (const char *)desc.triangles;
                for    ( i    = 0; i < desc.numTriangles;    ++i    )
                {
                    WriteChar_b( *tris );
                    ++tris;
                    WriteChar_b( *tris );
                    ++tris;
                    WriteChar_b( *tris );
                    ++tris;
                }
            }
            else if    ( desc.triangleStrideBytes/3 ==    2 )
            {
                const short    *tris =    (const short *)desc.triangles;
                for    ( i    = 0; i < desc.numTriangles;    ++i    )
                {
                    WriteShort_b( *tris    );
                    ++tris;
                    WriteShort_b( *tris    );
                    ++tris;
                    WriteShort_b( *tris    );
                    ++tris;
                }         
            }
            else if    ( desc.triangleStrideBytes/3 ==    4 )
            {
                const int *tris    = (const int *)desc.triangles;
                for    ( i    = 0; i < desc.numTriangles;    ++i    )
                {
                    WriteInt_b(    *tris );
                    ++tris;
                    WriteInt_b(    *tris );
                    ++tris;
                    WriteInt_b(    *tris );
                    ++tris;
                }         
            }
            else
            {
                assert(    false );
            }

            // SRM : new - writeout the actual mesh data if user wants it, to avoid
            // having to cook a mesh when they read it back in!
            unsigned totalAmt = 0;
            int buffSize = 0;
            char *data   = 0;
            bool status  = false;
            if ( mSaveCookedData )
            {
                NxTriangleMeshDesc tmd;
                mesh->saveToDesc( tmd );
                // now write the size of the data
                NxInitCooking();
                //UserStream us( "c:\\tri.bin", true );
                UserStream userStream( "c:\\tri.bin", false );
                status = NxCookTriangleMesh( tmd, userStream );
            }

            if ( status )
            {
                // k, now open up the file for reading binary
                FILE *fp = fopen( "c:\\tri.bin", "rb" );                    
                if ( fp )
                {
                    char tmp[512];
                    int amtRead = fread( tmp, sizeof(char), sizeof(char)*512, fp );
                    while ( amtRead )
                    {
                        // add amount read to total amount
                        int newAmt = totalAmt + amtRead;

                        // is total amount read greater than our buffer?
                        if ( newAmt > buffSize )
                        {
                            // yes, so allocate a new buffer
                            char *newData = new char[buffSize+10240];
                            if ( newData )
                            {
                                // k, do we have existing data?
                                if ( buffSize )
                                {
                                    // copy existing to the new data
                                    memcpy( newData, data, sizeof(char)*buffSize );

                                    // delete old data
                                    delete [] data;
                                }

                                // setup pntr again
                                data = newData;

                                // update buffer size
                                buffSize += 10240;
                            }
                            else
                            {
                                delete [] data;
                                totalAmt = 0;
                                data = 0;
                                amtRead = 0;
                            }
                        }

                        // copy read in data to our buffer
                        if ( data )
                        {
                            memcpy( data+totalAmt, tmp, sizeof(char)*amtRead );

                            // update total amt
                            totalAmt = newAmt;
                        }

                        // get next info
                        amtRead = fread( tmp, sizeof(char), sizeof(char)*512, fp );
                    }
                    // close file
                    fclose( fp );
                }                    
            }

            if ( totalAmt && data )
            {
                WriteUnsigned_b( totalAmt );
                fwrite( data, sizeof(char), sizeof(char)*totalAmt, mFph );
                delete [] data;
            }
            else
            {
                WriteUnsigned_b( 0 );
            }

            // write pmap info
            unsigned size =    mesh->getPMapSize();
            if ( mesh->hasPMap() )
            {
                NxPMap pmap;
                pmap.dataSize =    size;
                pmap.data     = (void *)(new char[size]);

                bool success = mesh->getPMapData( pmap );

                if ( success )
                {
                    // k, write    its    size and the data
                    WriteUnsigned_b( size );
                    fwrite(    pmap.data, sizeof(char), pmap.dataSize,    mFph );
                }
                else
                {
                    WriteUnsigned_b( 0 );
                }

                // now deallocate the memory we allocated
                delete [] pmap.data;
            }
            else
            {
                WriteUnsigned_b( 0 );
            }
        }
        else
        {
            const char *base = (const char *) desc.triangles;
            for(NxU32 i=0; i<desc.numTriangles;    i++)
            {
                const char *t =    &base[i*desc.triangleStrideBytes];
                if ( desc.flags    & NX_MF_16_BIT_INDICES )
                {
                    const NxU16    *tri = (const NxU16    *) t;
                    Print("Tri%d=(%d,%d,%d)", i+1, tri[0], tri[1], tri[2] );
                }
                else
                {
                    const NxU32    *tri = (const NxU32    *) t;
                    Print("Tri%d=(%d,%d,%d)", i+1, tri[0], tri[1], tri[2] );
                }
            }

            // write pmap info
            if ( desc.pmap )
            {
                Print("pmap    = %08X,    size = %d",    desc.pmap->data, desc.pmap->dataSize );
            }
            else
            {
                Print("pmap=NULL");
            }
        }
        --mIndent;
        --mIndent;
    }
}

//==================================================================================
void CoreDump::Write( const    char *d, const NxConvexMesh    *mesh )
{
    if ( mesh )
    {
        unsigned i;
        Print("%s",    d );
        ++mIndent;

        if ( mBinary )
        {
            // write its index
            int    index =    GetConvexIndex(    mesh );
            WriteInt_b(    index );
        }

        // output its info
        NxConvexMeshDesc desc;
        mesh->saveToDesc(desc);

        if ( mBinary )
        {
            WriteFlag_b( desc.flags    );
        }
        else
        {
            PrintFlag(desc.flags, "NX_CF_FLIPNORMALS",        NX_CF_FLIPNORMALS );
            PrintFlag(desc.flags, "NX_CF_16_BIT_INDICES",    NX_CF_16_BIT_INDICES );
            PrintFlag(desc.flags, "NX_CF_COMPUTE_CONVEX",    NX_CF_COMPUTE_CONVEX );
#if   NX_SDK_VERSION_NUMBER == 230
            PrintFlag(desc.flags, "NX_CF_INFLATE_CONVEX",    NX_CF_INFLATE_CONVEX );
#endif
#if NX_SDK_VERSION_NUMVER >= 231
            PrintFlag(desc.flags, "NX_CF_USE_LEGACY_COOKER",    NX_CF_USE_LEGACY_COOKER );
#endif
        }

        // write num verts,    etc.
        WriteInt( "NumVertices",      desc.numVertices    );
        WriteInt( "PointStrideBytes", desc.pointStrideBytes    );

        ++mIndent;
        const char *vbase =    (const char    *)desc.points;
        for    ( i    = 0; i < desc.numVertices; ++i )
        {
            // write the vertices (for now,    we will    write out the indices)
            const float    *p = (const    float *)&vbase[i * desc.pointStrideBytes];
            if ( mBinary )
            {
                WriteInt_b(    i );
                WriteFloat_b( p[0] );
                WriteFloat_b( p[1] );
                WriteFloat_b( p[2] );
            }
            else
            {
                const float    *p = (const    float *)&vbase[i*desc.pointStrideBytes];
                Print("V%d=(%s,%s,%s)",    i+1, getFloatString(p[0]), getFloatString(p[1]), getFloatString(p[2]) );
            }
        }
        --mIndent;

        WriteInt( "NumTriangles",        desc.numTriangles );
        WriteInt( "TriangleStrideBytes", desc.triangleStrideBytes );

        ++mIndent;
        // k, now write    out    the    convex info (indices)
        const char *tbase =    (const char    *) desc.triangles;
        for( i=0; i<desc.numTriangles; ++i )
        {
            const char *t =    &tbase[i*desc.triangleStrideBytes];
            if ( desc.flags    & NX_MF_16_BIT_INDICES )
            {
                const NxU16    *tri = (const NxU16    *) t;
                if ( mBinary )
                {
                    WriteInt_b(    tri[0] );
                    WriteInt_b(    tri[1] );
                    WriteInt_b(    tri[2] );
                }
                else
                {
                    Print("Tri%d=(%d,%d,%d)", i+1, tri[0], tri[1], tri[2] );
                }
            }
            else
            {
                const NxU32    *tri = (const NxU32    *) t;
                if ( mBinary )
                {
                    WriteInt_b(    tri[0] );
                    WriteInt_b(    tri[1] );
                    WriteInt_b(    tri[2] );
                }
                else
                {
                    Print("Tri%d=(%d,%d,%d)", i+1, tri[0], tri[1], tri[2] );
                }
            }
        }
        --mIndent;

        unsigned totalAmt = 0;
        int buffSize = 0;
        char *data   = 0;
        bool status  = false;
        if ( mBinary && mSaveCookedData )
        {
            NxConvexMeshDesc cmd;
            mesh->saveToDesc( cmd );
            
            NxInitCooking();
            //UserStream us( "c:\\goog.bin", true );
            UserStream userStream( "c:\\goog.bin", false );
            status = NxCookConvexMesh( cmd, userStream );
        }
        if ( status )
        {
            // k, now open up the file for reading binary
            FILE *fp = fopen( "c:\\goog.bin", "rb" );                    
            if ( fp )
            {
                char tmp[512];
                int amtRead = fread( tmp, sizeof(char), sizeof(char)*512, fp );
                while ( amtRead )
                {
                    // add amount read to total amount
                    int newAmt = totalAmt + amtRead;

                    // is total amount read greater than our buffer?
                    if ( newAmt > buffSize )
                    {
                        // yes, so allocate a new buffer
                        char *newData = new char[buffSize+10240];
                        if ( newData )
                        {
                            // k, do we have existing data?
                            if ( buffSize )
                            {
                                // copy existing to the new data
                                memcpy( newData, data, sizeof(char)*buffSize );

                                // delete old data
                                delete [] data;
                            }

                            // setup pntr again
                            data = newData;

                            // update buffer size
                            buffSize += 10240;
                        }
                        else
                        {
                            delete [] data;
                            totalAmt = 0;
                            data = 0;
                            amtRead = 0;
                        }
                    }

                    // copy read in data to our buffer
                    if ( data )
                    {
                        memcpy( data+totalAmt, tmp, sizeof(char)*amtRead );

                        // update total amt
                        totalAmt = newAmt;
                    }

                    // get next info
                    amtRead = fread( tmp, sizeof(char), sizeof(char)*512, fp );
                }
                // close file
                fclose( fp );
            }                    
        }

        if ( mBinary )
        {
            if ( totalAmt && data )
            {
                WriteUnsigned_b( totalAmt );
                fwrite( data, sizeof(char), sizeof(char)*totalAmt, mFph );
                delete [] data;
            }
            else
            {
                WriteUnsigned_b( 0 );
            }
        }

        --mIndent;
    }
}

//==================================================================================
#if NX_SDK_VERSION_NUMBER >= 230
void CoreDump::Write( const char *desc, NxCCDSkeleton *skeleton )
{
    if ( skeleton )
    {
        unsigned size        = skeleton->getDataSize();
        unsigned char *buff    = new unsigned char[size];

        if ( buff )
        {
            void *v                    = (void *)buff;
            unsigned amtReturned    = skeleton->save( v, size );
            assert( amtReturned == size );

            if ( mBinary )
            {
                // write its size
                WriteUnsigned( desc, amtReturned );

                // now write its info
                fwrite( v, sizeof(char), sizeof(char)*amtReturned, mFph );

                // now delete our temp storage
                delete [] buff;
            }
            else
            {
                char tmp[128];
                Print( "%s", desc );

                ++mIndent;

                // k, a little different - we output it byte by byte, 
                // after we output how many are forthcoming
                sprintf( tmp, "CCDSkelNumBytes" );
                WriteInt( tmp, amtReturned );

                for ( unsigned i = 0; i < amtReturned; ++i )
                {
                    sprintf( tmp, "CCDSkelByte%d", i );
                    int value = (int)buff[i];
                    WriteInt( tmp, value );
                }
                --mIndent;
            }
        }
    }
}

//==================================================================================
void CoreDump::Write( const char *desc, NxTireFunctionDesc *func )
{
    // k, write out the NxTireFunctionDesc
    if ( mFph && func )
    {
        Print("%s",    desc );
        ++mIndent;

        WriteFloat(    "extremumSlip",        func->extremumSlip );
        WriteFloat(    "extremumValue",    func->extremumValue );
        WriteFloat(    "asymptoteSlip",    func->asymptoteSlip );
        WriteFloat( "asymptoteValue",    func->asymptoteValue );
        WriteFloat( "stiffnessFactor",    func->stiffnessFactor );

        --mIndent;          
    }
}
#endif // #if NX_SDK_VERSION_NUMBER >= 230

#if NX_SDK_VERSION_NUMBER >= 240
//==================================================================================
void CoreDump::Write( const char *d, const NxHeightField *hf )
{
    if ( hf )
    {
        Print( "%s", d );
        ++mIndent;

        if ( mBinary )
        {
            // write its index
            int    index =    GetHeightFieldIndex( hf );
            WriteInt_b(    index );
        }

        NxHeightFieldDesc desc;
        hf->saveToDesc( desc );

        if ( mBinary )
        {
            WriteFlag_b( desc.flags    );
        }
        else
        {
            PrintFlag( desc.flags, "NX_HF_NO_BOUNDARY_EDGES", NX_HF_NO_BOUNDARY_EDGES );
        }

        WriteUnsigned( "HeightField_NumRows",            desc.nbRows );
        WriteUnsigned( "HeightField_NumColumns",        desc.nbColumns );
        WriteUnsigned( "HeightField_Format",            desc.format ); 
        WriteFloat( "HeightField_VerticalExtent",        desc.verticalExtent );
        WriteFloat( "HeightField_ConvexEdgeThreshold",    desc.convexEdgeThreshold );

        if ( desc.samples )
        {
            //brief The offset in bytes between consecutive samples in the samples array.
            WriteInt( "HeightField_SampleStride", desc.sampleStride );

            Print( "[HeightField_Information]" );
            ++mIndent;

            /**
            There are nbRows * nbColumn samples in the array,
            which define nbRows * nbColumn vertices and cells,
            of which (nbRows - 1) * (nbColumns - 1) cells are actually used.

            The array index of sample(row, column) = row * nbColumns + column.
            The byte offset of sample(row, column) = sampleStride * (row * nbColumns + column).
            The sample data follows at the offset and spans the number of bytes defined by the format.
            Then there are zero or more unused bytes depending on sampleStride before the next sample.

            \brief Height field height data is 16 bit signed integers, followed by triangle materials.     
            Each sample is 32 bits wide arranged as follows:
            \image html heightFieldFormat_S16_TM.png

            1) First there is a 16 bit height value.
            2) Next, two one byte material indices, with the high bit of each byte reserved for special use.
            (so the material index is only 7 bits).
            The high bit of material0 is the tess-flag.
            The high bit of material1 is reserved for future use.
            
            There are zero or more unused bytes before the next sample depending on NxHeightFieldDesc.sampleStride, 
            where the application may eventually keep its own data.

            This is the only format supported at the moment.

            **/
            unsigned totalAmt = desc.nbColumns * desc.nbRows;

            if ( desc.format == NX_HF_S16_TM )
            {
                unsigned *samplesUnsigned = (unsigned *)desc.samples;

                if ( mBinary )
                {
                    fwrite( samplesUnsigned, sizeof(unsigned), sizeof(unsigned)*totalAmt, mFph );
                }
                else
                {
                    for ( unsigned i = 0; i < totalAmt; ++i )
                    {
                        // get index into the samples array - note that the data is formatted
                        // as (16-bits height field info)(8-bit material index)(8-bit material index)
                        int heightFieldValue    = samplesUnsigned[i] & 0xffff;
                        int materialIndex1        = ( samplesUnsigned[i] & 0x7f0000 ) >> 16;
                        int tessFlag1            = ( samplesUnsigned[i] & 0x800000 ) >> 23;
                        int materialIndex2        = ( samplesUnsigned[i] & 0x7f000000 ) >> 24;
                        int tessFlag2            = ( samplesUnsigned[i] & 0x80000000 ) >> 31;

                        char buff[128];
                        sprintf( buff, "HeightField_HeightFieldValue_%d", i );
                        WriteInt( buff, heightFieldValue );

                        sprintf( buff, "HeightField_MaterialIndexOne_%d", i );
                        WriteInt( buff, materialIndex1 );

                        sprintf( buff, "HeightField_MaterialIndexTwo_%d", i );
                        WriteInt( buff, materialIndex2 );

                        sprintf( buff, "HeightField_TessFlagOne_%d", i );
                        WriteInt( buff, tessFlag1 );

                        sprintf( buff, "HeightField_TessFlagTwo_%d", i );
                        WriteInt( buff, tessFlag2 );
                    }
                }
            }
            else
            {
                // currently NX_HF_S16_TM is only type, so assert here
                assert( 0 );
            }

            --mIndent;
        }
        else
        {
            // no data, indicate stride is 0!
            WriteInt( "HeightField_SampleStride", 0 );
        }

        --mIndent;
    }
}
#endif // #if NX_SDK_VERSION_NUMBER >= 240

//==================================================================================
int    CoreDump::GetActorIndex(const NxActor *a) const
{
    int ret = -1;

    if ( a )
    {
        const char *index = (const char *) a->userData;
        ret = (int)(index - 0);
    }
  return ret;
}

//==================================================================================
int    CoreDump::GetShapeIndex(const NxActor *a,const NxShape *s) const
{
    int ret = -1;

  assert(a);
  assert(s);
    if ( a )
    {
      NxU32 numShapes    = a->getNbShapes();
        if ( numShapes )
        {
            NxShape    *const *shapes = a->getShapes();
            for    ( NxU32    k =    0; k    < numShapes; ++k )
            {
                const NxShape *shape = shapes[k];
                if ( shape == s )
                {
                  ret = k;
                  break;
                }
            }
        }
    }
  return ret;
}

//==================================================================================
void CoreDump::Write(const char    *desc,const    NxQuat &q)
{
    if ( mFph )
    {
        NxReal quat[4];
        q.getXYZW( quat    );

        if ( mBinary )
        {
            WriteFloat(    "quat0", quat[0] );
            WriteFloat(    "quat1", quat[1] );
            WriteFloat(    "quat2", quat[2] );
            WriteFloat(    "quat3", quat[3] );
        }
        else
        {
            Print("%s=%s,%s,%s,%s",    desc,
              getFloatString( quat[0] ),
              getFloatString( quat[1] ),
              getFloatString( quat[2] ),
              getFloatString( quat[3] )    );
        }
    }
}


void CoreDump::SaveUserData(NxScene *scene)
{
    assert( mActorUserData == 0 );

    NxU32 acount = scene->getNbActors();

    if ( acount )
    {
          mActorUserData = (void **) malloc( sizeof(void *)*acount );
        NxActor    **actors = scene->getActors();
        const char *foo = 0;

        for (NxU32 i=0; i<acount; i++)
          {
            NxActor *a = actors[i];
            const char *index = foo+i;
            mActorUserData[i] = a->userData;
            a->userData = (void *)index;
          }
    }
}

void CoreDump::RestoreUserData(NxScene *scene)
{
  NxU32 acount = scene->getNbActors();

  if ( acount )
  {
      assert(mActorUserData);

        NxActor    **actors = scene->getActors();
      for (NxU32 i=0; i<acount; i++)
      {
          NxActor *a = actors[i];
          a->userData = mActorUserData[i];
      }

      free(mActorUserData);
      mActorUserData = 0;

  }
}

#endif

}; // End of Namespace

using namespace    NX_CORE_DUMP;

bool NxCoreDump(NxPhysicsSDK *sdk,const char *fname,bool binary,bool saveCooked)
{
    CoreDump cd;
    return cd.coreDump(sdk,fname,binary,saveCooked);
}

unsigned int NxGetCoreVersion(NxPhysicsSDK *sdk) // return munged version number id of this SDK or current header files.
{
    return CoreDump::getCoreVersion(sdk);
}

#endif
