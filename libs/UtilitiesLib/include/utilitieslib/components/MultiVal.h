/***************************************************************************
*     Copyright (c) 2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/


/********************************** MultiVal ***********************************
    MultiVal is our version of a Variant.  This data type supports standard 64
bit data types such as S64, F64, and pointers.  Pointers can either be deep 
copies of data, or shallow copied depending on a flag bit.   This flag determines
if the data is freed when the type is changed or the MultiVal is destroyed.

    The type field of the MultiVal consists of four sub fields.  These subfields
are:  Flags(8):Game(8):Opers(8):Unit(8).  Basic types such as ints or floats
will only have the UNIT subfield set, and a basic operator will only have the
OPER field set.  However the system does allow for complex types where several
fields are set, such an "MULTI_FUNCTIONCALL" where it is operator which contains
STRING data as well.   This system allows 255 generic data types, operators,
and game specific types since all 8 bits zero always denotes NONE of that subtype.

    Because of the MALLOC/FREE issues inherent in this system, it is strongly
recommended that you use the "Standard MultiVal Manipulators" when acting upon
a MultiVal, especially when creating, copying, changing or destroying.

*** NOTES ***
Interesting combinations can be used between the OP and TYPE field to provide
a mechanism for more specialized operator switching.  For instance, the
"MULTI_FUNCTIONCALL" is defined to use a "MMT_STRING" type.  However you
could also define a "MULTI_FUNCTIONCALLPTR" which used a "MMT_POINTER" type
or a "MULTI_FUNCTIONCALLINDEX" which used a "MMT_INT" type, so that during
expression evaluation, the MultiVal could be converted from hashing on
string for function lookup to a direct pointer or function table lookup.
In this case, the MULTI_GET_OPER would return MMT_FUNCTIONCALL, and the
function could switch on MULTI_GET_TYPE to determine lookup method.

The GAME subfield is currently undefined.  This is provided so that a
game can use it's own group of 256 game specific types.  However these types
will not be supported by TextParser without additional changes.

Also, the TextParser will always allocate data in a "permanent manner", meaning
that it is assumed these loaded objects will never go out of scope, so they
are never tagged as "MULTI_FREE".   This allows the Expression system to copy
MultiVal data without need of Deep Copies.   This can be changed, but
consequences to the Expression, TextParser, and Command systems must be examined.
*******************************************************************************/

#ifndef MULTIVAL_H
#define MULTIVAL_H

#include "../stdtypes.h"

#define MULTI_TYPE_MASK                0x000000FF
#define MULTI_OPER_MASK                0x0000FF00
#define MULTI_GAME_MASK                0x00FF0000
#define MULTI_FLAG_MASK                0xFF000000

#define MULTI_GET_TYPE(x)            ((x) &  MULTI_TYPE_MASK)            //Return type stored in union (if any)
#define MULTI_GET_OPER(x)            (((x) & MULTI_OPER_MASK) >> 8)        //Return type of operator (if any)
#define MULTI_GET_GAME(x)            (((x) & MULTI_GAME_MASK) >> 16)        //Return game specific information (if any)
#define MULTI_FLAGLESS_TYPE(x)        ((x) & ~MULTI_FLAG_MASK)            //Return type without flags

#define MULTI_IS_FREE(x)            ((x) &  MULTI_FREE)            //Test free bit


//External definitions
typedef struct Entity Entity;
typedef struct BaseEntity BaseEntity;
typedef void* (*CustomMemoryAllocator)(void* data, size_t size);


typedef enum
{
    MMA_NONE= 0,                // There is no array, if there is data, it is contain in the union
    MMA_FIXED,                    // There is a fixed size of data, it is pointed to by the union
    MMA_EARRAY                    // There is an eaArray, it is pointed to by the union
} MMArray;

typedef enum 
{
    MMT_NONE = 0,                  // This is valid type meaning there is no associated data
    MMT_INT32,                    // A 32 bit signed int
    MMT_INT64,                    // A 64 bit signed int
    MMT_FLOAT32,                // A 32 bit float
    MMT_FLOAT64,                // A 64 bit float
    MMT_INTARRAY,                // EArray of ints
    MMT_FLOATARRAY,                // EArray of floats
    MMT_VEC3,                    // Pointer to a Vec3
    MMT_VEC4,                    // Pointer to a Vec4
    MMT_MAT4,                    // Pointer to a Mat4
    MMT_QUAT,                    // Pointer to a Quat
    MMT_STRING,                    // const char* 
    MMT_MULTIVALARRAY,            // not persisted or sent down, only function prototype

// Non persisted types
    MMT_NP_ENTITYARRAY = 128,    // not persisted or sent down
    MMT_NP_POINTER,                // Pointer which can not be persisted or sent

    MMT_TYPE_END
} MMType;

typedef enum 
{
    MMO_NONE = 0,
    MMO_ADD,                
    MMO_SUBTRACT,
    MMO_NEGATE,
    MMO_MULTIPLY,
    MMO_DIVIDE,
    MMO_EXPONENT,
    MMO_BIT_AND,
    MMO_BIT_OR,
    MMO_BIT_NOT,
    MMO_BIT_XOR,
    MMO_PAREN_OPEN,
    MMO_PAREN_CLOSE,
    MMO_BRACE_OPEN,
    MMO_BRACE_CLOSE,
    MMO_EQUALITY,
    MMO_LESSTHAN,
    MMO_LESSTHANEQUALS,
    MMO_GREATERTHAN,
    MMO_GREATERTHANEQUALS,
    MMO_FUNCTIONCALL,
    MMO_STACKPTR,
    MMO_IDENTIFIER,
    MMO_COMMA,
    MMO_AND, 
    MMO_OR,
    MMO_NOT,
    MMO_CONTINUATION,
    MMO_STATEMENT_BREAK,
    MMO_OBJECT_PATH,
    MMO_LOCATION,
    MMO_OP_END            
} MMOper;


/********* MultiValType Field ********
*  Bits 31-24 (8)    Flags             *
*  Bits 23-16 (8)   Undef             *
*  Bits 15- 8 (8)   Ops                 *
*  Bits 7 - 0 (8)   Types             *
*  This supports mixed op & type     *
*  Game bits are undefined ATM         *
*************************************/
#define MTE(flags, op, type)    ((flags) | ((op) << 8) | (type))

typedef enum MultiValType
{
    // Flag Bits
    MULTI_FREE                = 0x80000000,    // Data is to be free'd when done

    // Special Types
    MULTI_INVALID            = 0x0000FFFF,
    MULTI_NONE                 = MTE(0, MMO_NONE, MMT_NONE),                

    // Basic Types (shallow copied data - not freed)
    MULTI_INT                 = MTE(0, MMO_NONE, MMT_INT64),                            
    MULTI_FLOAT             = MTE(0, MMO_NONE, MMT_FLOAT64),                        
    MULTI_INTARRAY             = MTE(0, MMO_NONE, MMT_INTARRAY),                        
    MULTI_FLOATARRAY         = MTE(0, MMO_NONE, MMT_FLOATARRAY),                    
    MULTI_MULTIVALARRAY        = MTE(0, MMO_NONE, MMT_MULTIVALARRAY),                        
    MULTI_VEC3                 = MTE(0, MMO_NONE, MMT_VEC3),                            
    MULTI_VEC4                 = MTE(0, MMO_NONE, MMT_VEC4),                            
    MULTI_MAT4                 = MTE(0, MMO_NONE, MMT_MAT4),
    MULTI_QUAT                = MTE(0, MMO_NONE, MMT_QUAT),
    MULTI_STRING             = MTE(0, MMO_NONE, MMT_STRING),                            

    // Non Persisted Shallow Types
    MULTI_NP_ENTITYARRAY     = MTE(0, MMO_NONE, MMT_NP_ENTITYARRAY),                    
    MULTI_NP_POINTER         = MTE(0, MMO_NONE, MMT_NP_POINTER),                        
                            
    // Basic Types (deep copied data - freed on change)
    MULTI_INTARRAY_F        = MTE(MULTI_FREE, MMO_NONE, MMT_INTARRAY),                        
    MULTI_FLOATARRAY_F         = MTE(MULTI_FREE, MMO_NONE, MMT_FLOATARRAY),                    
    MULTI_MULTIVALARRAY_F    = MTE(MULTI_FREE, MMO_NONE, MMT_MULTIVALARRAY),                        
    MULTI_STRING_F            = MTE(MULTI_FREE, MMO_NONE, MMT_STRING),
    MULTI_VEC3_F            = MTE(MULTI_FREE, MMO_NONE, MMT_VEC3),
    MULTI_VEC4_F            = MTE(MULTI_FREE, MMO_NONE, MMT_VEC4),
    MULTI_MAT4_F            = MTE(MULTI_FREE, MMO_NONE, MMT_MAT4),
    MULTI_QUAT_F            = MTE(MULTI_FREE, MMO_NONE, MMT_QUAT),
        
    // Non Persisted Deep Types
    MULTI_NP_ENTITYARRAY_F     = MTE(MULTI_FREE, MMO_NONE, MMT_NP_ENTITYARRAY),                    
    MULTI_NP_POINTER_F         = MTE(MULTI_FREE, MMO_NONE, MMT_NP_POINTER),                        

    // Operator Types (produced by tokenizer)
    MULTIOP_NOOP            = MTE(0, MMO_NONE, MMT_NONE),
    MULTIOP_ADD                = MTE(0, MMO_ADD, MMT_NONE),
    MULTIOP_SUBTRACT        = MTE(0, MMO_SUBTRACT, MMT_NONE),
    MULTIOP_NEGATE            = MTE(0, MMO_NEGATE, MMT_NONE),
    MULTIOP_MULTIPLY        = MTE(0, MMO_MULTIPLY, MMT_NONE),
    MULTIOP_DIVIDE            = MTE(0, MMO_DIVIDE, MMT_NONE),
    MULTIOP_BIT_AND            = MTE(0, MMO_BIT_AND, MMT_NONE),
    MULTIOP_BIT_OR            = MTE(0, MMO_BIT_OR, MMT_NONE),
    MULTIOP_BIT_NOT            = MTE(0, MMO_BIT_NOT, MMT_NONE),
    MULTIOP_BIT_XOR            = MTE(0, MMO_BIT_XOR, MMT_NONE),
    MULTIOP_EXPONENT        = MTE(0, MMO_EXPONENT, MMT_NONE),
    MULTIOP_PAREN_OPEN        = MTE(0, MMO_PAREN_OPEN, MMT_NONE),
    MULTIOP_PAREN_CLOSE        = MTE(0, MMO_PAREN_CLOSE, MMT_NONE),
    MULTIOP_BRACE_OPEN        = MTE(0, MMO_BRACE_OPEN, MMT_NONE),
    MULTIOP_BRACE_CLOSE        = MTE(0, MMO_BRACE_CLOSE, MMT_NONE),
    MULTIOP_EQUALITY        = MTE(0, MMO_EQUALITY, MMT_NONE),
    MULTIOP_LESSTHAN        = MTE(0, MMO_LESSTHAN, MMT_NONE),
    MULTIOP_LESSTHANEQUALS  = MTE(0, MMO_LESSTHANEQUALS, MMT_NONE),
    MULTIOP_GREATERTHAN        = MTE(0, MMO_GREATERTHAN, MMT_NONE),
    MULTIOP_GREATERTHANEQUALS=MTE(0, MMO_GREATERTHANEQUALS, MMT_NONE),                    
    MULTIOP_FUNCTIONCALL    = MTE(0, MMO_FUNCTIONCALL, MMT_STRING),
    MULTIOP_IDENTIFIER         = MTE(0, MMO_IDENTIFIER, MMT_STRING),
    MULTIOP_COMMA            = MTE(0, MMO_COMMA, MMT_NONE),                                
    MULTIOP_AND                = MTE(0, MMO_AND, MMT_NONE), // put this in front of start to make the debugger happy
    MULTIOP_OR                = MTE(0, MMO_OR, MMT_NONE),
    MULTIOP_NOT                = MTE(0, MMO_NOT, MMT_NONE),
    MULTIOP_CONTINUATION    = MTE(0, MMO_CONTINUATION, MMT_NONE),
    MULTIOP_STATEMENT_BREAK    = MTE(0, MMO_STATEMENT_BREAK, MMT_NONE),
    MULTIOP_OBJECT_PATH        = MTE(0, MMO_OBJECT_PATH, MMT_STRING),

    MULTIOP_LOC_STRING        = MTE(0, MMO_LOCATION, MMT_STRING),
    MULTIOP_LOC_MAT4        = MTE(0, MMO_LOCATION, MMT_MAT4),
    MULTIOP_LOC_MAT4_F        = MTE(MULTI_FREE, MMO_LOCATION, MMT_MAT4),

    // Operator types with non-persisted data
    MULTIOP_NP_STACKPTR        = MTE(0, MMO_STACKPTR, MMT_NP_POINTER),
} MultiValType;


typedef struct MultiInfo
{
    void*        ptr;            //Pointer to data
    MMArray        atype;            //Array type    
    MMType        dtype;            //Data type
    U32            size;            //Size of data;
    U32            count;            //Number of items;
    U32            width;            //Width of an individual item
} MultiInfo;

typedef struct MultiVal
{
    union {
        const void* ptr;
        const char* str;
        void* ptr_noconst;
        S32    int32;
        S64 intval;
        S32* intptr;
        F32 float32;
        F64 floatval;
        F32* floatptr;
        Vec3* vecptr;
        BaseEntity*** entarray;
    };
    MultiValType type;
} MultiVal;

typedef const MultiVal  CMultiVal;
typedef MultiVal**        MultiValArray;



/******************  Beginning of Standard MultiVal Manipulators ******************/

// MultiVal Construction/Manipulation Routines
//        NOTE:  These can be turned INLINE or into DEFINES as needed for speed

// Allocate and construct a blank MultiVal
MultiVal*    MultiValCreate(void);

// Reconstruct a MultiVal (for stack/non "created" multivals)
void        MultiValConstruct(MultiVal* val, int type, int count);

// Fills the multival with the availible data
void        MultiValFill(MultiVal* dst, void *data);

//Dup a MultiVal, creating a new instance.  Copy internal data using allocator if specified

// Create a duplicate of a MultiVal
MultiVal*    MultiValDup(CMultiVal* val);

CMultiVal*    MulitValDupWith(CMultiVal* val, CustomMemoryAllocator fp, void* cookie);

//Copy a MultiVal to an existing location.  Copy internal data using allocator if specified

// Copies to Destination, from source
void        MultiValCopy(MultiVal* dst, CMultiVal* src);

// Copies to Destination, from source and forces the dst to be MULTI_FREE (if remotely applicable)
void        MultiValDeepCopy(MultiVal* dst, CMultiVal* src);

void        MultiValCopyWith(MultiVal* dst, CMultiVal* src, CustomMemoryAllocator fp, void* cookie, int deepCopy);

//Clean out a MultiVal, and Release it if it's destroyed

// Frees a MultVal
void        MultiValDestroy(MultiVal* val);

// Sets a MultiVal to the default state
void        MultiValClear(MultiVal* val);

// MultiVal Query Routines

//    Returns information about this multival
void        MultiValInfo(CMultiVal* src, MultiInfo* info);

//    Returns information about this type, ptrs & size are invalid
void        MultiValTypeInfo(MultiValType t, MultiInfo* info);

//  MultiVal Get and Set Routines
//        NOTE:  These functions assume the DST is a valid MultiVal of ANY type, and all copies are DEEP
void        MultiValSetInt(MultiVal* dst, S64 val);
void        MultiValSetFloat(MultiVal* dst, F64 val);
void        MultiValSetString(MultiVal* dst, const char* str);
void        MultiValSetStringLen(MultiVal* dst, const char* str, int len);
void        MultiValSetPointer(MultiVal* dst, const void* ptr, int len);
void        MultiValSetVec3(MultiVal* dst, const Vec3* val);
void        MultiValSetVec4(MultiVal* dst, const Vec4* val);
void        MultiValSetMat4(MultiVal* dst, const Mat4 val);
void        MultiValSetQuat(MultiVal* dst, const Quat* val);
void        MultiValSetIntArray(MultiVal* dst, int** eiArray);
void        MultiValSetFloatArray(MultiVal* dst, float** efArray);
void        MultiValSetEntityArray(MultiVal* dst, Entity** eeArray);

//        NOTE:  These functions assume the DST is a valid MultiVal of ANY type, and all copies are SHALLOW

// Shallow copy
void        MultiValReferenceString(MultiVal* dst, const char *str);
// Shallow copy
void        MultiValReferencePointer(MultiVal* dst, void* ptr);
// Shallow copy
void        MultiValReferenceVec3(MultiVal* dst, const Vec3* val);
// Shallow copy
void        MultiValReferenceVec4(MultiVal* dst, const Vec4* val);
// Shallow copy
void        MultiValReferenceMat4(MultiVal* dst, const Mat4* val);
// Shallow copy
void        MultiValReferenceQuat(MultiVal* dst, const Quat* val);
// Shallow copy
void        MultiValReferenceIntArray(MultiVal* dst, int** eIntArray);
// Shallow copy
void        MultiValReferenceFloatArray(MultiVal* dst, float** eFloatArray);
// Shallow Copy
void        MultiValReferenceEntityArray(MultiVal* dst, Entity*** eeArray);


//        MultiVal Access Routines
//    These routines will attempt to convert when possible.  If conversion fails
//    it will set *TST to false if valid, or assert if no tst pointer was passed
#define        MultiValGetAscii    MultiValGetString

// Convert FLT, INT, STR, and ID to INT
S64            MultiValGetInt(CMultiVal* src, bool* tst);

// Convert FLT, INT, STR, and ID to FLT
F64            MultiValGetFloat(CMultiVal* src, bool* tst);

// Return char* for ID or STR
const char* MultiValGetString(CMultiVal* src, bool* tst);

Vec3*        MultiValGetVec3(CMultiVal* src, bool* tst);
Vec4*        MultiValGetVec4(CMultiVal* src, bool* tst);
Mat4*        MultiValGetMat4(CMultiVal* src, bool* tst);
Quat*        MultiValGetQuat(CMultiVal* src, bool* tst);
void*        MultiValGetPointer(CMultiVal* src, bool* tst);
int**        MultiValGetIntArray(CMultiVal* src, bool* tst);
float**        MultiValGetFloatArray(CMultiVal* src, bool *tst);
Entity***    MultiValGetEntityArray(CMultiVal* dst, bool *tst);

//    MultiVal Test Routines

// Returns true if the data type matches for this MultiVal
bool        MultiValIsDataType(CMultiVal* src, MMType type);

// Returns true if type is FLOAT or INT
bool        MultiValIsNumber(CMultiVal* src);

// Returns true if type is STRING or IDENTIFIER
bool        MultiValIsAscii(CMultiVal* src);

// Returns true if type is some operator
bool        MultiValIsOp(CMultiVal* src);

// Returns true if type is NONE
bool        MultiValIsEmpty(CMultiVal* src);

// EArray Manipulation Routines

// Create and empty EArray
MultiValArray*    MultiValArrayCreate(void);

// Clear and delete all the multivals in the EArray
void            MultiValArrayClear(MultiValArray* array);

// Destroy an EArray
void            MultiValArrayDestroy(MultiValArray* array);

// Useful functions for debugging

// Returns a string representation of multivaltypes that people can actually read
const char* MultiValTypeToReadableString(MultiValType t);

// Appends a "reasonable" text representation to the estring
char*        MultiValPrint(CMultiVal* val);

/******************  End of Standard MultiVal Manipulators ******************/



#endif
