#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/structHist.h"
#include "utilitieslib/utils/structNet.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/components/StringCache.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/timing.h"
#include <string.h>
#include "utilitieslib/utils/tokenstore.h"
#include "utilitieslib/utils/textparserUtils.h"

//NOTE: THIS IS A UNION!
typedef union StructHistElem
{
    union StructHistElem *branch;
    F32 val;
    char * text;
} StructHistElem;


typedef struct StructHistImp {
    StructHistElem *vals;    //this is a tree of StructHistElems
    U32 lastTimestamp;
    F32 timeAccum;
    ParseTable *tpi;
    U32 remove:1; // flagged for removal from the SHC
    const char *folder; // Folder to dump logs into
} StructHistImp;

typedef struct StructHistCollectionImp {
    StashTable ghtStructHists;
    StashTable ghtRemovedStructHists;
    U32 timestamp; // Used in dumpProcessor callback
    char logDir[MAX_PATH];
} StructHistCollectionImp;

static int dateSetGlobally = false;
static char *currentDate=NULL;

StructHist createStructHist(ParseTable *tpi, char *folder)
{
    StructHist sh = calloc(sizeof(StructHistImp), 1);
    sh->tpi = tpi;
    sh->folder = allocAddString(folder);
    return sh;
}

void shFreeSampleData(ParseTable* fieldDefs, void *** vals);

void destroyStructHist(StructHist sh)
{
    shFreeSampleData(sh->tpi, (void***)&sh->vals);
    free(sh);
}

void shFreeSampleData(ParseTable* fieldDefs, void *** vals) 
{
    int fieldIndex;

    if (!vals || !*vals)
        return;
    // Iterate through all fields defined in ParseTable, and if they're a struct, recurse and free
    FORALL_PARSEINFO(fieldDefs, fieldIndex)
    {
        ParseTable* fd = &fieldDefs[fieldIndex];
        int storage = TokenStoreGetStorageType(fd->type);
        void** fieldAddr;

        fieldAddr = (*vals) + fieldIndex;
        if(fd->type & TOK_REDUNDANTNAME)
            continue;

        if (storage != TOK_STORAGE_DIRECT_SINGLE && storage != TOK_STORAGE_INDIRECT_SINGLE)
        {
            Errorf("shFreeSampleData is not compatible with array fields");
            continue;
        }
        
        switch (TOK_GET_TYPE(fd->type))
        {
        case TOK_STRUCT_X:
            shFreeSampleData((ParseTable*)fd->subtable, (void***)fieldAddr);
            break;
        case TOK_STRING_X:
        case TOK_FILENAME_X:
            {
                char** s=(char**)fieldAddr;
                SAFE_FREE(*s);
            }
            break;
        }
    }
    SAFE_FREE(*vals);
}



void shSampleInternal(StructHistElem *vals, void *elem, F32 dt, ParseTable *fieldDefs)
{
    int i;
    

    // Iterate through all fields defined in StructDef
    FORALL_PARSEINFO(fieldDefs, i)
    {
        StructHistElem *valptr;
        F32 valadd = 0;
        ParseTable* fd = &fieldDefs[i];
        int format = TOK_GET_FORMAT_OPTIONS(fd->format);
        int storage = TokenStoreGetStorageType(fd->type);

        if(fd->type & TOK_REDUNDANTNAME)
            continue;
        
        if (format == TOK_FORMAT_FRIENDLYDATE ||
            format == TOK_FORMAT_FRIENDLYSS2000 ||
            format == TOK_FORMAT_DATESS2000 ||
            format == TOK_FORMAT_IP)
            continue;

        if (storage != TOK_STORAGE_DIRECT_SINGLE && storage != TOK_STORAGE_INDIRECT_SINGLE)
        {
            Errorf("shSampleInternal is not compatible with array fields");
            continue;
        }

        // Pointer to where the running total is stored
        valptr = &(vals[i]);

        switch (TOK_GET_TYPE(fd->type))
        {
        case TOK_INT_X:
            valadd = (F32)(elem?TokenStoreGetInt(fieldDefs, i, elem, 0):0);
            break;
        case TOK_INT64_X:
            valadd = (F32)(elem?TokenStoreGetInt64(fieldDefs, i, elem, 0):0);
            break;
        case TOK_INT16_X:
            valadd = (F32)(elem?TokenStoreGetInt16(fieldDefs, i, elem, 0):0);
            break;
        case TOK_BOOL_X:
        case TOK_U8_X:
            valadd = (F32)(elem?TokenStoreGetU8(fieldDefs, i, elem, 0):0);
            break;
        case TOK_F32_X:
            valadd = elem?TokenStoreGetF32(fieldDefs, i, elem, 0):0;
            if (format == TOK_FORMAT_PERCENT) {
                valadd*=100;
            }
            break;

        case TOK_STRUCT_X:
            if (!valptr->branch) {
                valptr->branch = calloc(ParseTableCountFields((ParseTable*)fd->subtable), sizeof(StructHistElem));
            }
            shSampleInternal(valptr->branch, elem?TokenStoreGetPointer(fieldDefs, i, elem, 0): 0, dt, (ParseTable*)fd->subtable);
            break;
        case TOK_STRING_X:
            {
                char* newstr = elem? TokenStoreGetPointer(fieldDefs, i, elem, 0): 0;
                if (valptr->text && (!(newstr) || strcmp(valptr->text, newstr)!=0)) {
                    // It's changed
                    free(valptr->text);
                    valptr->text=NULL;
                }
                if (!valptr->text) {
                    // String not allocated or it's new
                    valptr->text = strdup(newstr);
                }
            }
            break;
        case TOK_IGNORE:
        case TOK_START:
        case TOK_END:
            // Ignored
            break;

        default:
            assert(0);
        }
        if (valadd) {
            valptr->val+= valadd*dt;
        }
    }

}

void shSample(StructHist sh, void *elem, U32 timestamp)
{
    F32 dt;
    if (sh->lastTimestamp == 0) {
        // Initial values, just zero out the structure/allocate it
        dt = 0.;
    } else {
        dt = timerSeconds(timestamp - sh->lastTimestamp);
    }
    if (!sh->vals) {
        sh->vals = calloc(ParseTableCountFields(sh->tpi), sizeof(StructHistElem));
    }
    shSampleInternal(sh->vals, elem, dt, sh->tpi);
    sh->lastTimestamp = timestamp;
    sh->timeAccum += dt;
}

void shResetInternal(StructHistElem *vals, ParseTable *fieldDefs)
{
    int i;
    

    // Iterate through all fields defined in StructDef
    FORALL_PARSEINFO(fieldDefs, i)
    {
        StructHistElem * valptr;
        ParseTable* fd = &fieldDefs[i];
        int format = TOK_GET_FORMAT_OPTIONS(fd->format);
        int storage = TokenStoreGetStorageType(fd->type);

        if(fd->type & TOK_REDUNDANTNAME)
            continue;
        
        if (format == TOK_FORMAT_FRIENDLYDATE ||
            format == TOK_FORMAT_DATESS2000 ||
            format == TOK_FORMAT_FRIENDLYSS2000 ||
            format == TOK_FORMAT_IP)
            continue;

        if (storage != TOK_STORAGE_DIRECT_SINGLE && storage != TOK_STORAGE_INDIRECT_SINGLE)
        {
            Errorf("shSampleInternal is not compatible with array fields");
            continue;
        }

        // Pointer to where the running total is stored
        valptr = &(vals[i]);

        switch (TOK_GET_TYPE(fd->type))
        {
        case TOK_INT_X:
        case TOK_INT64_X:
        case TOK_INT16_X:
        case TOK_U8_X:
        case TOK_BOOL_X:
        case TOK_F32_X:
            valptr->val = 0.0;
            break;

        case TOK_STRUCT_X:
            if (!valptr->branch) {
                valptr->branch = calloc(ParseTableCountFields((ParseTable*)fd->subtable), sizeof(StructHistElem));
            }
            shResetInternal(valptr->branch, (ParseTable*)fd->subtable);
            break;
        case TOK_STRING_X:
            // No point in resetting strings, we'll just re-alloc them!
            break;
        case TOK_IGNORE:
        case TOK_START:
        case TOK_END:
            // Ignored
            break;

        default:
            assert(0);
        }
    }
}

void shReset(StructHist sh)
{
    sh->lastTimestamp = 0;
    sh->timeAccum = 0;
    // Need to loop through the structure and clear the F32*s and **s.
    if (!sh->vals) {
        sh->vals = calloc(ParseTableCountFields(sh->tpi), sizeof(StructHistElem));
    }
    shResetInternal(sh->vals, sh->tpi);
}

void shDumpInternal(FILE* fout, StructHistElem *vals, F32 timeAccum, ParseTable *fieldDefs, int headerOnly)
{
    int i;
    assert(vals);

    // Iterate through all fields defined in StructDef
    FORALL_PARSEINFO(fieldDefs, i)
    {
        StructHistElem* valptr;
        ParseTable* fd = &fieldDefs[i];
        int format = TOK_GET_FORMAT_OPTIONS(fd->format);
        int storage = TokenStoreGetStorageType(fd->type);

        if(fd->type & TOK_REDUNDANTNAME)
            continue;
        
        if (format == TOK_FORMAT_FRIENDLYDATE ||
            format == TOK_FORMAT_DATESS2000 ||
            format == TOK_FORMAT_FRIENDLYSS2000 ||
            format == TOK_FORMAT_IP)
            continue;

        if (storage != TOK_STORAGE_DIRECT_SINGLE && storage != TOK_STORAGE_INDIRECT_SINGLE)
        {
            Errorf("shSampleInternal is not compatible with array fields");
            continue;
        }

        // Pointer to where the running total is stored
        valptr = &(vals[i]);

        switch(TOK_GET_TYPE(fd->type))
        {
        case TOK_INT_X:
        case TOK_INT64_X:
        case TOK_INT16_X:
        case TOK_U8_X:
        case TOK_BOOL_X:
        case TOK_F32_X:
            if (headerOnly) {
                fprintf(fout, "\"%s\",", fd->name);
            } else {
                F32 val = valptr->val / timeAccum;
                int ipart = (int)val;
                F32 fpart = val - ipart;
                if (ABS(fpart) < 0.001 || ABS(fpart)>0.999) {
                    fprintf(fout, "%1.f,", val);
                } else {
                    fprintf(fout, "%f,", val);
                }
            }
            break;

        case TOK_STRUCT_X:
            shDumpInternal(fout, valptr->branch, timeAccum, (ParseTable*)fd->subtable, headerOnly);
            break;
        case TOK_STRING_X:
            if (headerOnly) {
                fprintf(fout, "\"%s\",", fd->name);
            } else {
                fprintf(fout, "\"%s\",", valptr->text?valptr->text:"");
            }
            break;
        case TOK_IGNORE:
        case TOK_START:
        case TOK_END:
            // Ignored
            break;

        default:
            assert(0);
        }
    }
}

void shDump(StructHist sh, const char *logdir)
{
    char fname[MAX_PATH];
    FILE *fout;
    
    sprintf_s(SAFESTR(fname), "./%s/%s.csv", logdir, sh->folder);

    if (!dateSetGlobally) {
        currentDate = timerGetTimeString();
    }

    mkdirtree(fname);
    if (!fileExists(fname)) {
        fout = fopen(fname, "at");
        assert(fout);
        fprintf(fout, "\"Date\",");
        shDumpInternal(fout, sh->vals, sh->timeAccum, sh->tpi, 1);
        fprintf(fout, "\n");
    } else {
        fout = fopen(fname, "at");
        if (!fout) {
            printf("Not logging, failed to open %s\n", fname);
            return;
        }
        assert(fout);
    }

    fprintf(fout, "\"%s\",", currentDate);
    shDumpInternal(fout, sh->vals, sh->timeAccum, sh->tpi, 0);
    fprintf(fout, "\n");
    fclose(fout);
}


StructHistCollection createStructHistCollection(const char *logDir)
{
    StructHistCollection shc = calloc(sizeof(StructHistCollectionImp),1);
    shc->ghtStructHists = stashTableCreateAddress(128);
    shc->ghtRemovedStructHists = stashTableCreateInt(16);
    Strncpyt(shc->logDir, logDir);
    return shc;
}

static void destroyStructHistAdapter(void* arg0)
{
    destroyStructHist((StructHist)(intptr_t)arg0);
}

void destroyStructHistCollection(StructHistCollection shc)
{
    if (shc) {
        stashTableDestroyEx(shc->ghtStructHists, NULL, destroyStructHistAdapter);
        stashTableDestroyEx(shc->ghtRemovedStructHists, NULL, destroyStructHistAdapter);
        free(shc);
    }
}

void shcRemove(StructHistCollection shc, void *elem, U32 timestamp)
{
    StructHist sh=0;
    static int removed_count=0;

    if (!shc)
        return;

    if (stashAddressFindPointer(shc->ghtStructHists, elem, &sh))
    {
        shSample(sh, elem, timestamp);
        sh->remove = 1;
        // Move to removed hashTable (really just used as a list)
        removed_count++;
        stashIntAddPointer(shc->ghtRemovedStructHists, removed_count, sh, false);
        stashAddressRemovePointer(shc->ghtStructHists, elem, NULL);
    }
}

void shcUpdate(StructHistCollection *pshc, void *elem, ParseTable *tpi, U32 timestamp, char *folder)
{
    StructHistCollection shc;
    StructHist sh=0;
    if (*pshc == NULL)
        *pshc = createStructHistCollection("perflogs");

    shc = *pshc;

    if (!stashAddressFindPointer(shc->ghtStructHists, elem, &sh)) {
        sh = createStructHist(tpi, folder);
        stashAddressAddPointer(shc->ghtStructHists, elem, sh, false);
    }
    assert(!sh->remove);
    shSample(sh, elem, timestamp);
}


static int pruneProcessor(void *ht, StashElement element)
{
    StashTable ght = ht;
    StructHist sh = (StructHist)stashElementGetPointer(element);
    assert(sh->remove);
    if (sh->remove) {
        stashIntRemovePointer(ght, stashElementGetIntKey(element), NULL);
        destroyStructHist(sh);
    }
    return 1;
}

static void shcPrune(StructHistCollection shc)
{
    if (!shc)
        return;
    stashForEachElementEx(shc->ghtRemovedStructHists, pruneProcessor, shc->ghtRemovedStructHists);
}

static int dumpProcessor(void *vpshc, StashElement element)
{
    StructHistCollection shc = (StructHistCollection)vpshc;
    U32 timestamp = shc->timestamp;
    StructHist sh = (StructHist)stashElementGetPointer(element);
    if (sh->lastTimestamp) {
        if (!sh->remove) {
            shSample(sh, (void*)stashElementGetKey(element), timestamp);
        }
        shDump(sh, shc->logDir);
        shReset(sh);
        if (!sh->remove) {
            shSample(sh, (void*)stashElementGetKey(element), timestamp);
        }
    } else {
        // Hasn't ever been sampled (should only happen on a clear?)
        //assert(0);
    }
    return 1;
}

void shcDump(StructHistCollection shc, U32 timestamp)
{
    if (!shc)
        return;

    currentDate = timerGetTimeString();
    dateSetGlobally = true;
    shc->timestamp = timestamp;
    stashForEachElementEx(shc->ghtStructHists, dumpProcessor, (void*)shc);
    stashForEachElementEx(shc->ghtRemovedStructHists, dumpProcessor, (void*)shc);

    shcPrune(shc);
}


static int intvalue=0;
static F32 floatvalue=0;
static bool g_sh_ignore_earrays=false;
void shDoOperationSetInt(int i)
{
    intvalue = i;
}
void shDoOperationSetFloat(F32 f)
{
    floatvalue = f;
}
void shSetOperationSilentlyIgnoreEArrays(bool ignore)
{
    g_sh_ignore_earrays = true;
}

void shDoOperation(StructOp op, ParseTable *fieldDefs, void *dest, const void *operand)
{
    int i;
    // Iterate through all fields defined in ParseTable, and only those fields
    FORALL_PARSEINFO(fieldDefs, i)
    {
        ParseTable* fd = &fieldDefs[i];
        int format = TOK_GET_FORMAT_OPTIONS(fd->format);
        int storage = TokenStoreGetStorageType(fd->type);
        int n, numelems = 1;
        void* src = (void*)operand;

        if(fd->type & TOK_REDUNDANTNAME)
            continue;
        
        if (format == TOK_FORMAT_FRIENDLYDATE ||
            format == TOK_FORMAT_DATESS2000 ||
            format == TOK_FORMAT_FRIENDLYSS2000 ||
            format == TOK_FORMAT_IP)
            continue;

        if (storage == TOK_STORAGE_INDIRECT_EARRAY)
        {
            if (!g_sh_ignore_earrays)
                Errorf("shDoOperation is not compatible with earray fields");
            continue;
        }
        if (storage == TOK_STORAGE_DIRECT_EARRAY) // F32 *, int *
        {
            if (src == OPERAND_INT || src == OPERAND_FLOAT)
            {
                // Doesn't matter what the size is, we just do all of them
            } else {
                // Two arrays, they must be the same size!
                int numsrcelems = TokenStoreGetNumElems(fieldDefs, i, src);
                numelems = TokenStoreGetNumElems(fieldDefs, i, dest);
                if (numelems != numsrcelems) {
                    Errorf("shDoOperation called with earrays with mismatched sizes");
                    MIN1(numelems, numsrcelems);
                }
            }
        }
        if (storage == TOK_STORAGE_DIRECT_FIXEDARRAY || storage == TOK_STORAGE_INDIRECT_FIXEDARRAY)
        {
            numelems = fd->param;
        }

#define VAL(get) ((src == OPERAND_INT)?(intvalue):((src == OPERAND_FLOAT)?(floatvalue):get(fieldDefs, i, src, n)))
#define DOIT(set, get)    \
    switch (op) {        \
    xcase STRUCTOP_ADD:    \
        set(fieldDefs, i, dest, n, get(fieldDefs, i, dest, n) + VAL(get)); \
    xcase STRUCTOP_MIN:    \
        set(fieldDefs, i, dest, n, MIN(get(fieldDefs, i, dest, n), VAL(get))); \
    xcase STRUCTOP_MAX:    \
        set(fieldDefs, i, dest, n, MAX(get(fieldDefs, i, dest, n), VAL(get))); \
    xcase STRUCTOP_DIV:    \
        set(fieldDefs, i, dest, n, get(fieldDefs, i, dest, n) / VAL(get)); \
    xcase STRUCTOP_MUL:    \
        set(fieldDefs, i, dest, n, get(fieldDefs, i, dest, n) * VAL(get)); \
    xcase STRUCTOP_LERP:\
        set(fieldDefs, i, dest, n, (1-floatvalue)*get(fieldDefs, i, dest, n) + (floatvalue)*VAL(get) ); \
    }

        // iterate over fixed array
        for (n = 0; n < numelems; n++)
        {
            switch (TOK_GET_TYPE(fd->type))
            {
            xcase TOK_INT_X:
                DOIT(TokenStoreSetInt, TokenStoreGetInt);
            xcase TOK_INT64_X:
                DOIT(TokenStoreSetInt64, TokenStoreGetInt64);
            xcase TOK_INT16_X:
                DOIT(TokenStoreSetInt16, TokenStoreGetInt16);
            xcase TOK_U8_X:
            case TOK_BOOL_X:
                DOIT(TokenStoreSetU8, TokenStoreGetU8);
            xcase TOK_F32_X:
                if (n == 0 && TOK_GET_FORMAT_OPTIONS(fd->format)==TOK_FORMAT_HSV) {
                    if (op == STRUCTOP_LERP) {
                        F32 v0 = TokenStoreGetF32(fieldDefs, i, dest, n);
                        F32 v1 = VAL(TokenStoreGetF32);
                        F32 diff = v1 - v0;
                        if (ABS(diff) < 180) {
                            TokenStoreSetF32(fieldDefs, i, dest, n, v0 + (floatvalue)*diff);
                        } else {
                            F32 newval;
                            if (diff > 180)
                                diff-=360;
                            else
                                diff+=360;
                            newval = v0 + (floatvalue)*diff;
                            if (newval >= 360)
                                newval-=360;
                            if (newval < 0)
                                newval+=360;
                            TokenStoreSetF32(fieldDefs, i, dest, n, newval);
                        }
                    } else {
                        assertmsg(0, "HSV value passed to operation other than LERP, don't know what to do!");
                    }
                } else {
                    DOIT(TokenStoreSetF32, TokenStoreGetF32);
                }
            xcase TOK_STRUCT_X:
                shDoOperation(op, fd->subtable, TokenStoreGetPointer(fieldDefs, i, dest, n), 
                    (src == OPERAND_INT || src == OPERAND_FLOAT)? src:
                    TokenStoreGetPointer(fieldDefs, i, src, n));
            case TOK_STRING_X:
            case TOK_IGNORE:
            case TOK_START:
            case TOK_END:
                // Ignored
                break;
            default:
                break;
            }
        }
    }
}
