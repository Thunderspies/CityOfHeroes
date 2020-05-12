/***************************************************************************
*     Copyright (c) 2003-2006, Cryptic Studios
*     All Rights Reserved
*     Confidential Property of Cryptic Studios
***************************************************************************/

#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/ListView.h" // for LV #defines
#include "utilitieslib/utils/structNet.h"
#include <stddef.h>   // for offsetof
#include "utilitieslib/assert/assert.h"
#include <math.h> // fabs

#include "utilitieslib/network/netio.h"
#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/structinternals.h"



static char *invalidName="IncompatibleDataType"; // Used when creating dynamic TPIs
static char *outOfSyncName="outOfSyncName"; // Used when the struct parser enum table does not exactly match

static bool s_inSync = true;

int intFromTokenType(TokenizerTokenType v);


/* sdDifferenceDetector functions
*  Returns:
*      1 - values are different.
*      2 - values are not different.
*/
typedef int (*sdDifferenceDetector)(void*, void*);

static int diff(ParseTable *tpi, void *a, void *b);

static int sdBadDifferenceDetector(void* useless1, void* useless2)
{
    assert(0);
    return 0;
}

typedef struct FloatAccuracyTable {
    FloatAccuracy fa;
    float unpackmult;
    float packmult;
    int    minbits;
} FloatAccuracyTable;

// These must be in the same order as the enum for quick lookup
FloatAccuracyTable floattable[] = {
    {0, 1, 1, 32}, // NO PACKING
    {FLOAT_HUNDRETHS, 0.01, 100, 8},
    {FLOAT_TENTHS, 0.1, 10, 6},
    {FLOAT_ONES, 1, 1, 7},
    {FLOAT_FIVES, 5, 0.2, 5},
    {FLOAT_TENS, 10, 0.1, 4},
};

float ParserUnpackFloat(int i, FloatAccuracy fa)
{
    devassert(fa);
    return (float)i*floattable[fa].unpackmult;
}

int ParserPackFloat(float f, FloatAccuracy fa)
{
    int ret = (int)(f*floattable[fa].packmult + 0.5);
    devassert(fa);
    if (ret == 0 && f>0.0) // Because this is used to send client HP, we don't *ever* want to send a 0 unless it's actually a 0 floating point value
        ret = 1;
    return ret;
}

int ParserFloatMinBits(FloatAccuracy fa) {
    devassert(fa);
    return floattable[fa].minbits;
}

void packDelta(Packet *pak, int delta)
{
    pktSendBits(pak, 1, delta!=0);
    if (delta==0)
        return;
    pktSendBits(pak, 1, delta > 0);
    if (delta < 0) {
        pktSendBitsPack(pak, 1, -delta);
    } else {
        pktSendBitsPack(pak, 1, delta);
    }
}

int unpackDelta(Packet *pak)
{
    if (!pktGetBits(pak, 1))
        return 0;
    if (pktGetBits(pak, 1)) {
        // positive
        return pktGetBitsPack(pak, 1);
    } else {
        return -(int)pktGetBitsPack(pak, 1);
    }
}

void sdPackStruct(ParseTable* tpi, Packet* pak, void* oldVersion, void* newVersion, bool forcePackAllFields, bool allowDiffs, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    bool sendAbsolute = forcePackAllFields || !allowDiffs || oldVersion==NULL;
    int i;

    if (oldVersion==NULL) {
        forcePackAllFields = 1;
    }

    assert(oldVersion || !allowDiffs);

    if (pak->hasDebugInfo) {
        pktSendBits(pak, 1, sendAbsolute ? 1 : 0);
    }

    // Iterate through all fields defined in StructDef
    if (newVersion)
    FORALL_PARSEINFO(tpi, i)
    {
        int type = TOK_GET_TYPE(tpi[i].type);
        if (type == TOK_IGNORE ||
            type == TOK_START ||
            type == TOK_END)
            continue;
        if (tpi[i].type & TOK_REDUNDANTNAME) continue;

        if (iOptionFlagsToMatch && !(iOptionFlagsToMatch & tpi[i].type))
        {
            continue;
        }

        if (iOptionFlagsToExclude && (iOptionFlagsToExclude & tpi[i].type))
        {
            continue;
        }

        if (forcePackAllFields || (TOKARRAY_INFO(tpi[i].type).compare(tpi, i, oldVersion, newVersion, 0)))
        {
            // Send a bit saying there is a field diff to follow.
            pktSendBits(pak, 1, 1);

            // Send the index of this field.
            // We're assuming that that we'll have the exact same definition
            // during unpack.
            pktSendBitsPack(pak, 1, i);

            TOKARRAY_INFO(tpi[i].type).senddiff(pak, tpi, i, oldVersion, newVersion, 0, sendAbsolute, forcePackAllFields, allowDiffs, iOptionFlagsToMatch, iOptionFlagsToExclude);
        }
    }

    // Say there there are no more diffs.
    pktSendBits(pak, 1, 0);
}

void ParserSendEmptyDiff(ParseTable* sd, Packet* pak, void* oldVersion, void* newVersion, bool forcePackAllFields, bool allowDiffs, bool sendAbsoluteOverride)
{
    ParserSend(sd, pak, newVersion, newVersion, forcePackAllFields, allowDiffs, sendAbsoluteOverride, 0, 0);
}

/* void ParserSend()
*  Given the structure definition and two versions of the structure, this function
*  packs up values in the new version that are different from the old version into
*  the given packet.
*
*  Parameters:
*      forcePackAllFields:
*          Send full value of all fields.  Bypass diff detection and bypass delta
*          delta generation.  This should be used when a structure is being sent
*          for the first time.
*
*        allowDiffs:
*            Whether or not to allow diffs, if set to false, only absolute values
*            will be sent.
*/
void ParserSend(ParseTable* sd, Packet* pak, void* oldVersion, void* newVersion, bool forcePackAllFields, bool allowDiffs, bool sendAbsoluteOverride, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    bool sendAbsolute = forcePackAllFields || !allowDiffs || oldVersion==NULL;
    // Is this a full update or a diff update?
    assert(!sendAbsoluteOverride || !allowDiffs);
    if (pak->hasDebugInfo) {
        pktSendBits(pak, 1, sendAbsoluteOverride);
    }
    if (!sendAbsoluteOverride) {
        pktSendBits(pak, 1, sendAbsolute ? 0 : 1);
    }
    sdPackStruct(sd, pak, oldVersion, newVersion, forcePackAllFields, !sendAbsolute, iOptionFlagsToMatch, iOptionFlagsToExclude);
}

void sdUnpackStruct(ParseTable *fieldDefs, Packet *pak, void *data, void*** pktIds, bool allowDiffs)
{
    int hasMoreData;
    int absValues = !allowDiffs;
    int numFields = ParseTableCountFields(fieldDefs);

    if (pak->hasDebugInfo) {
        // full update update or diff update?
        int sentabsValues = pktGetBits(pak, 1);
        assert(absValues == sentabsValues);
    }

    if (pktIds && !*pktIds) {
        *pktIds = calloc(sizeof(void*)*numFields, 1);
    }

    while(hasMoreData = pktGetBits(pak, 1))
    {
        int fieldIndex;
        void *dataToFill = data;
        assert(hasMoreData!=-1);

        fieldIndex = pktGetBitsPack(pak, 1);
        if (fieldIndex < 0 || fieldIndex >=numFields) {
            assert(!"Received bad data in packed struct from server (code probably out of sync)");
            return;
        }
        if (fieldDefs[fieldIndex].name == invalidName) // need to ignore this field
            dataToFill = NULL;
        else if (fieldDefs[fieldIndex].name == outOfSyncName) // we try to receive data even tho we are partially out of sync
            s_inSync = false;
        TOKARRAY_INFO(fieldDefs[fieldIndex].type).recvdiff(pak, fieldDefs, fieldIndex, dataToFill, 0, absValues, (pktIds)? (*pktIds + fieldIndex): 0);
    }
}

bool ParserRecv(ParseTable* sd, Packet* pak, void* data, void*** pktIds, bool sendAbsoluteOverride)
{
    bool allowDiffs;
    if (pak->hasDebugInfo) {
        assert(sendAbsoluteOverride == (bool)pktGetBits(pak, 1));
    }
    if (sendAbsoluteOverride) {
        allowDiffs = false;
    } else {
        allowDiffs = pktGetBits(pak, 1);
    }
    s_inSync = true;
    sdUnpackStruct(sd, pak, data, pktIds, allowDiffs);
    return s_inSync;
}


void ParserFreePktIds(ParseTable* fieldDefs, void *** pktIds) 
{
    int fieldIndex;

    if (!pktIds || !*pktIds)
        return;

    FORALL_PARSEINFO(fieldDefs, fieldIndex)
    {
        if (fieldDefs[fieldIndex].type & TOK_REDUNDANTNAME) continue;
        TOKARRAY_INFO(fieldDefs[fieldIndex].type).freepktids(fieldDefs, fieldIndex, (*pktIds) + fieldIndex);
    }
    SAFE_FREE(*pktIds);
}

void sdFreeParseInfo(ParseTable* fieldDefs)
{
    int i;
    if (!fieldDefs)
        return;

    FORALL_PARSEINFO(fieldDefs, i)
    {
        ParseTable* fd = &fieldDefs[i];
        if (fd->name && fd->name != invalidName && fd->name != outOfSyncName) {
            free((void*)fd->name);
            fd->name = NULL;
        }
        if (TOK_GET_TYPE(fd->type) == TOK_STRUCT_X) {
            sdFreeParseInfo((ParseTable*)fd->subtable);
            fd->subtable = NULL;
        }
    }
    free(fieldDefs);
}


bool sdFieldsCompatible(ParseTable *field1, ParseTable *field2)
{
    if (stricmp(field1->name, field2->name)==0) 
    {
        TokenizerTokenType ttt1 = TOK_GET_TYPE(field1->type);
        TokenizerTokenType ttt2 = TOK_GET_TYPE(field2->type);

        if (TOK_GET_PRECISION(field1->type) != TOK_GET_PRECISION(field2->type)) // same network packing
            return false;
        if ((TOK_FIXED_ARRAY & field1->type) != (TOK_FIXED_ARRAY & field2->type)) // same array type, but don't care about indirection
            return false;
        if ((TOK_EARRAY & field1->type) != (TOK_EARRAY & field2->type))    // ''
            return false;
        if ((TOK_FIXED_ARRAY & field1->type) && field1->param != field2->param) // size of fixed array the same
            return false;
        if (ttt1==ttt2)
            return true;
        if ((ttt1==TOK_INT_X || ttt1==TOK_U8_X || ttt1==TOK_BOOL_X  || ttt1==TOK_INT16_X) &&
            (ttt2==TOK_INT_X || ttt2==TOK_U8_X || ttt2==TOK_BOOL_X  || ttt2==TOK_INT16_X))
        {
            // All integer types are compatible, but we must use the *local* type when unpacking
            return true;
        }
    }
    return false;
}

void sdPackParseInfo(ParseTable* fieldDefs, Packet *pak)
{
    int i, totalFields;

    totalFields = ParseTableCountFields(fieldDefs);
    pktSendBitsPack(pak, 1, totalFields);

    // Iterate through all fields defined in StructDef
    for(i = 0; i < totalFields; i++)
    {
        ParseTable* fd = &fieldDefs[i];

        pktSendString(pak, fd->name);
        assert(sizeof(fd->type) == 4); // have to change to pktSendBits64 if this gets larger
        pktSendBits(pak, 32, fd->type);
        pktSendBitsAuto(pak, (U32)fd->param); // may be cutting from 64 to 32, but pointers are ignored on other side
        if (TOK_GET_TYPE(fd->type) == TOK_STRUCT_X)
            sdPackParseInfo((ParseTable*)fd->subtable, pak);
    }
    assert(i==totalFields);
}

ParseTable *sdUnpackParseInfo(ParseTable *supported, Packet *pak, bool needExactMatch)
{
    int i, j, totalFields;
    ParseTable *ret;

    totalFields = pktGetBitsPack(pak, 1);
    ret = calloc(sizeof(ParseTable), totalFields+1);
    ParseTableClearCachedInfo(ret);

    // Iterate through all fields defined in StructDef
    for(i = 0; i<totalFields; i++)
    {
        ParseTable* fd = &ret[i];
        ParseTable* match = NULL;

        fd->name = strdup(pktGetString(pak));
        fd->type = pktGetBits(pak, 32);
        fd->param = pktGetBitsAuto(pak);

        // Try to find local match
        if (supported) {
            for (j=0; supported[j].name; j++) {
                if (sdFieldsCompatible(&supported[j], fd)) {
                    match = &supported[j];
                    break;
                }
            }

            // We did not find an exact match, just hope for the best
            if (!match && !needExactMatch) {
                for (j=0; supported[j].name; j++) {
                    if (stricmp(supported[j].name, fd->name)==0) {
                        free((void*)fd->name);
                        fd->name = outOfSyncName;
                        fd->type = supported[j].type;
                        fd->param = supported[j].param;
                        match = &supported[j];
                        break;
                    }
                }
            }
        }

        if (TOK_GET_TYPE(fd->type) == TOK_STRUCT_X)
            fd->subtable = sdUnpackParseInfo(match? match->subtable: NULL, pak, needExactMatch);

        // copy compatibility info if we matched
        if (match) {
            fd->type = match->type; // Use local type because it could be different, but still compatible
            fd->param = match->param;
            fd->storeoffset = match->storeoffset;
            fd->format = match->format;
        } else {
            free((void*)fd->name);
            fd->name = invalidName;
        }
    }
    assert(i==totalFields);

    return ret;
}
