#ifndef _REFERENCESYSTEM_H_
#define _REFERENCESYSTEM_H_

#include "../stdtypes.h"
#include "EString.h"
#include "earray.h"


//for more information about the reference system, see the WIKI page here:
//http://code:8081/display/cs/Reference+System


//a pointer to a bad referent, to indicate an error 
#define REFERENT_INVALID ((void*)0xffffffffffffffffll)


//set to 1 to make all reference checks much more careful, but slower
#define REFSYSTEM_DEBUG_MODE 0


//set to 1 or 0 to turn on or off testharness code
#define REFSYSTEM_TEST_HARNESS 0

//set to 1 for the test harness to use strings as references, 0 for opaque data
#define TEST_HARNESS_STRINGVERSION 1

//reference dictionaries can be referred to two ways:
//(1) By string (name) (slower)
//(2) By DictionaryHandle (faster)
//
//All the Reference System functions are set up to take either type of argument, and automatically
//figure out how to treat it
typedef void *DictionaryHandle;

//a define to make it super-obvious from looking at the function prototype that it takes a handle or string
#define DictionaryHandleOrName DictionaryHandle 



//a reference handle is what the outside world uses to get pointers to the data is has a reference to. It's basically
//a managed pointer, but might sometimes (during debug modes) be a little struct full of cookies or what
//have you
typedef void *ReferenceHandle;


/*this macro is used inside a struct definition to define a reference to a particular type.
so instead of

Power *pMyPower;

you would do

REF_TO(Power) hMyPower;
*/

#define REF_TO(myType) union { myType *pData; ReferenceHandle handle; }


/*this macro takes the reference and gives you back the pointer that the reference
refers to. This is VERY FAST (ie, just a pointer lookup), but it's very important that
you use this macro rather than try to dereference the pointer yourself*/
#define GET_REF(handleName) RefSystem_ReferentFromHandle(&handleName.handle)


//I'm declaring Referent and ReferenceData as undefined structs so that it's clear when a pointer is a pointer, rather
//than declaring some type as a void* or something.

//a pointer to "reference data" is a pointer to the actual reference itself, that is, the string that describes 
//the Referent, or the hierarchical structs that define the Referent
struct ReferenceData;

//a "Referent" is the thing that is pointed to by a reference. It's just a pointer, but I've given it a type to make
//things as generally unambiguous as possible
struct Referent;

//The Reference system internally uses CRC/Hash values to do searches and compares of references
typedef U32 ReferenceHashValue;




//In debug mode, RefSystem_ReferentFromHandle() takes some time to verify the integrity of the
//handle and all the internal stuff. Plus, there's a CheckIntegrity function that scans through every
//last reference and checks all the handles.
#if REFSYSTEM_DEBUG_MODE

struct Referent *RefSystem_ReferentFromHandle(ReferenceHandle *pHandle);
void RefSystem_CheckIntegrity(bool bDumpReport);

#else


//This is what gets from the handle to the referent. The whole point of this whole system is to 
//allow this to be just a pointer dereference. Debug modes might contain versions of this with
//error checking built in
static INLINEDBG struct Referent* RefSystem_ReferentFromHandle(ReferenceHandle *pHandle)
{
    return (struct Referent*)(*pHandle);
}

#define RefSystem_CheckIntegrity(bDumpReport) {}

#endif



//You should always use this macro to add a reference. dictNameOrHandle is the name of the reference dictionary,
//pRefData is a pointer to the ReferenceData, and handleName is your handle.
#define ADD_REF(dictNameOrHandle, pRefData, handleName) RefSystem_AddHandle(dictNameOrHandle, pRefData, &handleName.handle, false, 0)

//adds a volatile ref. (For more on volatile refs, see the WIKI page.)
#define ADD_REF_VOLATILE(dictNameOrHandle, pRefData, handleName) RefSystem_AddHandle(dictNameOrHandle, pRefData, &handleName.handle, true, 0)

//Takes a ReferenceHandle, registers it with the system, and points it towards whatever data is requested
//DO NOT CALL THIS FUNCTION (usually). Instead, use ADD_REF
bool RefSystem_AddHandle(DictionaryHandleOrName dictHandle, struct ReferenceData *pRefData, ReferenceHandle *pHandle, 
    bool bHandleIsVolatile, U32 iRequiredVersion);


//use this macro (and function) to add a reference directly from a string. Internally, the string will be converted
//to a ReferenceData*, then ADD_REF/RefSystem_AddHandle will be called. 
#define ADD_REF_FROM_STRING(dictNameOrHandle, string, handleName) RefSystem_AddHandleFromString(dictNameOrHandle, string, &handleName.handle, false, false)
bool RefSystem_AddHandleFromString(DictionaryHandleOrName dictHandle, const char *pString, ReferenceHandle *pHandle, 
    bool bHandleIsVolatile, bool bUseVersioningIfApplicable);


//use this macro to set a reference handle to point to NULL. You should only do this for handles that are not active. For instance,
//when you first allocate a structure, and it has handles in it.
#define CLEAR_UNATTACHED_REF(handleName) { handleName.handle = NULL; }



//use this macro to copy a reference.
#define COPY_REF(dstHandleName, srcHandleName) RefSystem_CopyHandle(&dstHandleName.handle, &srcHandleName.handle);

//sets DstHandle to point to the same thing SrcHandle is pointing to
void RefSystem_CopyHandle(ReferenceHandle *pDstHandle, ReferenceHandle *pSrcHandle);


//moves a handle from one place to another. Note that this does NOT actually write into either pOldHandle or pNewHandle. It assumes
//that moving a handle is part of moving a block of data, which was all presumably copied by memcpy or realloc or something. It just
//tells the reference system where the handle now is.
void RefSystem_MoveHandle(ReferenceHandle *pNewHandle, ReferenceHandle *pOldHandle);



//use this macro to remove a reference. You MUST remove all handles you create.
#define REMOVE_REF(handleName) RefSystem_RemoveHandle(&handleName.handle, false)

//This macro removes the specified handle, but, unlike REMOVE_REF, does not care if the handle
//doesn't actually exist. 
#define REMOVE_REF_IF_ACTIVE(handleName) RefSystem_RemoveHandle(&handleName.handle, true)

void RefSystem_RemoveHandle(ReferenceHandle *pHandle, bool bOKIfNotAHandle);



//This tells the reference system that a handle has moved from one place to another. Note that this does
//not read or write data to either newHandle or oldHandle. The assumption is that if you are relocating a struct
//that contains handles, you will allocate memory for the new struct, and use memcpy to copy all the struct data in 
//one fell swoop, which clones the actual data inside the reference handle, so that the reference system doesn't
//need to actually set the handle. However, the reference system DOES need to know where the handle now is.
#define MOVE_REF(newHandle, oldHandle) RefSystem_MoveHandle(&newHandle.handle, &oldHandle.handle)


//given a reference handle, returns the Reference Data
#define REF_DATA_FROM_HANDLE(handleName) RefSystem_RefDataFromHandle(&handleName.handle)
struct ReferenceData *RefSystem_RefDataFromHandle(ReferenceHandle *pHandle);



//this function checks whether the given ReferenceHandle is "active" with the system. Which is basically equivalent to checking
//whether the given piece of memory is a Reference Handle
bool RefSystem_IsHandleActive(ReferenceHandle *pHandle);
#define IS_REF_HANDLE_ACTIVE(handleName) RefSystem_IsHandleActive(&handleName.handle)



//given a reference handle, get its reference data, convert it to a string, and append it to
//an already-existing EString
void RefSystem_StringFromHandle(char **ppEString, ReferenceHandle *pHandle, bool bAttachVersionIfApplicable);
#define REF_STRING_FROM_HANDLE(ppEString, handleName) RefSystem_StringFromHandle(ppEString, &handleName.handle, false)


//given a reference handle, returns the dictionary name this reference belongs to
char *RefSystem_DictionaryNameFromReferenceHandle(ReferenceHandle *pHandle);
#define REF_DICTNAME_FROM_REF_HANDLE(handleName) RefSystem_DictionaryNameFromReferenceHandle(&handleName.handle)


//Tells the system that something new exists that might be referenced, in case there are already
//things loaded that are trying to point to it. This also actually "fills" the dictionary, if it's a
//self-defining dictionary
void RefSystem_AddReferent(DictionaryHandleOrName dictHandle, struct ReferenceData *pData, struct Referent *pReferent);

//a referent has been removed from RAM. All handles pointing to it should be made NULL.
//
//In certain instances, you not only want handles pointing to this referent to become NULL, you actually
//want them to completely cease to be handles, that is, just return to being simple plain old NULL pointers. In
//that case, pass in true for bCompletelyRemoveHandlesToMe.
void RefSystem_RemoveReferent(struct Referent *pReferent, bool bCompletelyRemoveHandlesToMe);

//a referent has moved from one place in RAM to another. Update all handles pointing to it.
void RefSystem_MoveReferent(struct Referent *pNewReferent, struct Referent *pOldReferent);

//Given a reference (the ReferenceData pointer), does all the calculations necessary to convert it to a 
//referent pointer
struct Referent *RefSystem_DirectlyDecodeReference(DictionaryHandleOrName dictHandle, struct ReferenceData *pRefData);

//initializes the reference system
void RefSystem_Init(void);


//This macro adds a "simple pointer reference", that is, an essentially nameless reference, from a handle to a pointer. This is useful
//when you want to have pointers which automatically get NULLed when the things the point to go away, without needing to be able to move things or
//have handles that are NULL-but-still-active. It saves you having to construct an entire reference dictionary to deal with super-simple references
#define ADD_SIMPLE_POINTER_REFERENCE(handleName, pReferent) ADD_REF("nullDictionary", (struct ReferenceData*)(pReferent), handleName)



//given a dictionary name, returns true if that dictionary exists, false otherwise
bool RefSystem_DoesDictionaryExist(char *pDictionaryName);

//given a dictionary name, returns a handle to that dictionary, which makes execution of most
//reference system functions faster.
DictionaryHandle RefSystem_GetDictionaryHandleFromName(char *pDictionaryName);


//The collection of callbacks that a reference dictionary needs to provide:

//should return REFERENT_INVALID if the reference is poorly formed, NULL if it is not pointing
//to anything
typedef struct Referent *RefCallBack_DirectlyDecodeReference(struct ReferenceData *pRefData);

typedef ReferenceHashValue RefCallBack_GetHashFromReferenceData(struct ReferenceData *pRefData);




//functions to convert reference data to strings and back. 

//This function converts a reference data to a string, and APPENDS that string to the EString
typedef bool RefCallBack_ReferenceDataToString(char **ppEString, struct ReferenceData *pRefData);

//This function converts a string to a reference and returns the reference data, which can 
//be freed by calling RefCallBack_FreeReferenceData. Returns NULL if the string is badly formatted
typedef struct ReferenceData *RefCallBack_StringToReferenceData(const char *pString);



//given a ReferenceData pointer is owned by someone else, makes a copy of it that can be owned
//by the reference system.
typedef struct ReferenceData *RefCallBack_CopyReferenceData(struct ReferenceData *pRefData);

//Frees the ReferenceData that was created by RefCallBack_CopyReferenceData or RefCallBack_StringToReferenceData
typedef void RefCallBack_FreeReferenceData(struct ReferenceData *pRefData);

//---------

//Registers a reference dictionary, given a name and all the necessary callback functions
DictionaryHandle RefSystem_RegisterDictionary(char *pName,
    RefCallBack_DirectlyDecodeReference *pDecodeCB,
    RefCallBack_GetHashFromReferenceData *pGetHashCB,
    RefCallBack_ReferenceDataToString *pRefDataToStringCB,
    RefCallBack_StringToReferenceData *pStringToRefDataCB,
    RefCallBack_CopyReferenceData *pCopyReferenceCB,
    RefCallBack_FreeReferenceData *pFreeReferenceCB);

//Registers a reference dictionary which uses strings as reference data. This is how most reference dictionaries are,
//and is faster and more efficient than using opaque data as reference data.
DictionaryHandle RefSystem_RegisterDictionaryWithStringRefData(char *pName,
     RefCallBack_DirectlyDecodeReference *pDecodeCB,
     bool bCaseSensitive);


//Registers a "self-defining" dictionary. See the wiki page for more info
DictionaryHandle RefSystem_RegisterSelfDefiningDictionary(char *pName, bool bCaseSensitive);

//given a name, returns the referent with that name. Only valid (and useful) for self-defining dictionaries.
struct Referent *RefSystem_GetReferentFromName(DictionaryHandleOrName dictHandle, char *pReferentName);


//This callback is only specified for specific dictionaries that do versioning for volatile references (see the Wiki page)
typedef U32 RefCallBack_GetVersionFromReferent(struct Referent *pReferent);

//Sets the version callback for a dictionary
void RefSystem_DictionarySetVersionCB(DictionaryHandleOrName dictHandle, RefCallBack_GetVersionFromReferent *pGetVersionCB);



//--------------------------------Stuff for REFARRAYS

//a REFARRAY is a special kind of earray that knows how to work with the reference system. Basically, as you'd imagine,
//it's an earray of References.

//internal functions used by the ref system, and by text parser. Use the MACROed versions instead.
int RefSystem_PushIntoRefArray(void ***pppRefArray, DictionaryHandleOrName dictHandle, struct ReferenceData *pRefData);
void RefSystem_InsertIntoRefArray(void ***pppRefArray, DictionaryHandleOrName dictHandle, struct ReferenceData *pRefData, int index);

/*to define a REFARRAY, just do
typedef struct
{
    int iFoo;
    float fBar;
    REFARRAY hRefArray;
} myStruct;

myStruct *pStruct;

then

CREATE_REFARRAY(pStruct->hRefArray)
etc
*/

typedef struct
{
    mEArrayHandle ppInternal_DO_NOT_USE;
} REFARRAY;

//this actually gets you a reference from a ref array. 
#define GET_REF_FROM_REFARRAY(handleName, index) RefSystem_ReferentFromHandle(&handleName.ppInternal_DO_NOT_USE[index])

//creates a ref array
#define CREATE_REFARRAY(handleName) eaCreateReferenceArray(&handleName.ppInternal_DO_NOT_USE)

//destroys the ref array
#define DESTROY_REFARRAY(handleName) eaDestroy(&handleName.ppInternal_DO_NOT_USE)

//pushes something onto the end of the ref array
#define PUSH_REFARRAY(dictNameOrHandle, pRefData, handleName) RefSystem_PushIntoRefArray(&handleName.ppInternal_DO_NOT_USE, dictNameOrHandle, pRefData)

//inserts something into a ref array (this is potentially a tad slow)
#define INSERT_REFARRAY(dictNameOrHandle, pRefData, handleName, index) RefSystem_InsertIntoRefArray(&handleName.ppInternal_DO_NOT_USE, dictNameOrHandle, pRefData, index)

//removes something from a ref array (potentially a tad slow)
#define REMOVE_REFARRAY(handleName, index) eaRemove(&handleName.ppInternal_DO_NOT_USE, index)

//does a fast remove from a ref array (does not maintain order)
#define REMOVE_REFARRAY_FAST(handleName, index) eaRemoveFast(&handleName.ppInternal_DO_NOT_USE, index)

//gets the size of a ref array
#define REFARRAY_SIZE(handleName) eaSizeUnsafe(&(handleName.ppInternal_DO_NOT_USE))


// some testing hooks
typedef struct RTHObject RTHObject;
void RTH_Test(void);



#endif

