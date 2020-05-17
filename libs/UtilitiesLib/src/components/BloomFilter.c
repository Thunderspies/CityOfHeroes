#include "utilitieslib/components/BloomFilter.h"

#include "utilitieslib/utils/mathutil.h"
#include <stdlib.h>
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/HashFunctions.h"
#include "utilitieslib/components/BitArray.h"

typedef struct BloomFilter
{
    BitArray bitTable;
    U32     uiBitTableSize;
    U32        uiHashFunctions;
    U32        uiMaskSize;
    U32        uiNumUniqueElements;
    U32        uiNumUniqueElementsExpected;
    U8        uiMaskSizeLog2;
} BloomFilter;

// random 32 bit ints
const U32 uiBFSeedArray[16] = 
{
    0x61eec548,
    0xd2f4181b,
    0x40c9db89,
    0xd1d1e988, // 4
    0x789339c8,
    0xfe41d83b,
    0x5bc0da56,
    0x7307aa9e, // 8
    0xaa2e8ec0,
    0x98388a03,
    0x9098f03e,
    0x24b08e0d, // 12
    0x5d13dc7a,
    0xba30ba78,
    0xeec51bee,
    0x9129995d, // 16
};


BloomFilter* bloomFilterCreate(U32 uiExpectedUniqueElements, F32 fFalsePositiveRate)
{
    BloomFilter* bf = malloc(sizeof(BloomFilter));

    // Calculate optimal bit table size
    F32 fBitTableSize;

    assert(fFalsePositiveRate > 0.0001f && fFalsePositiveRate < 0.6f); // you've picked an unreasonable false positive rate...
    
    fBitTableSize = (F32) (-2.081 * uiExpectedUniqueElements * log(fFalsePositiveRate));

    bf->uiBitTableSize = pow2((int)ceil(fBitTableSize)) >> 1;
    bf->uiHashFunctions = (U32)ceil(0.7f * ((F32)bf->uiBitTableSize / (F32)uiExpectedUniqueElements));

    bf->uiMaskSize = bf->uiBitTableSize;
    bf->uiMaskSizeLog2 = log2i(bf->uiBitTableSize);

    bf->uiNumUniqueElements = 0;

    bf->bitTable = createBitArray(bf->uiBitTableSize);
    bf->uiNumUniqueElementsExpected = uiExpectedUniqueElements;


    return bf;
}

typedef enum eBFProcess
{
    eBFProcess_Set,
    eBFProcess_Check,
    eBFProcess_CheckAndSet,
} eBFProcess;

static U32 bloomHashMask(BloomFilter* bf)
{
    return bf->uiMaskSize - 1;
}

static bool bloomFindKeys(BloomFilter* bf, U32 uiKey, eBFProcess eInstructions)
{
    U32 uiHashFunctionsLeft = bf->uiHashFunctions;
    U32 uiSeedIndex = 0;
    bool bFoundIt = true;
    while (1)
    {
        U32 uiHashResult = burtlehash2(&uiKey, 1, uiBFSeedArray[uiSeedIndex++]);
        U8 uiShifts = 0;
        U8 uiBitsLeftInHash = 32;
        while ( uiBitsLeftInHash > bf->uiMaskSizeLog2 )
        {
            U32 uiIndex = ((bloomHashMask(bf) << uiShifts) & uiHashResult ) >> uiShifts;
            uiShifts += bf->uiMaskSizeLog2;
            uiBitsLeftInHash -= bf->uiMaskSizeLog2;

            assert( uiIndex < bf->uiBitTableSize );

            // process index
            switch ( eInstructions )
            {
            case eBFProcess_Check:
                if (!baIsSet(bf->bitTable, uiIndex))
                    return false;
                break;
            case eBFProcess_Set: // it doesn't hurt to return the check part anyway
            case eBFProcess_CheckAndSet:
                if ( baSetBit(bf->bitTable, uiIndex) == 2) // 2 means that the bit wasn't set, but now is
                {
                    bFoundIt = false;
                    bf->uiNumUniqueElements++;
                    if ( bf->uiNumUniqueElements > ( bf->uiNumUniqueElementsExpected << 1) )
                    {
                        Errorf("This table has twice as many unique elements as you expected, false positive rate is probably much higher now");
                    }
                }
                break;
            };
            
            if ( --uiHashFunctionsLeft == 0 )
            {
                // we're done, finish up
                return bFoundIt;
            }
        }
    }
}

void bloomFilterDestroy(BloomFilter* bf)
{
    destroyBitArray(bf->bitTable);
    free(bf);
}
void bloomAddIntKey(BloomFilter* bf, U32 uiKey)
{
    bloomFindKeys(bf, uiKey, eBFProcess_Set);
}
bool bloomFindIntKey(BloomFilter* bf, U32 uiKey)
{
    return bloomFindKeys(bf, uiKey, eBFProcess_Check);
}
bool bloomFindAndAddIntKey(BloomFilter* bf, U32 uiKey)
{
    return bloomFindKeys(bf, uiKey, eBFProcess_CheckAndSet);
}
void bloomClearTable(BloomFilter* bf, U32 uiKey)
{
    baClearAllBits(bf->bitTable);
}

void testBloomFilter()
{
    const U32 uiNumToTest = 65536;
    const U32 uiNumUnique = 8192;
    const F32 fErrorRate = 0.14f;
    BloomFilter* bf = bloomFilterCreate(uiNumUnique, fErrorRate);
    U32 uiIndex = 0;
    U32 uiFalsePositive=0;

    // Generate data
    U32* puiTestData = _alloca(sizeof(U32) * uiNumToTest);
    for (uiIndex=0; uiIndex < uiNumToTest; uiIndex++)
    {
        puiTestData[uiIndex] = qrand() % uiNumUnique;
    }

    // Now, use bloomfilter to find unique numbers
    for (uiIndex=0; uiIndex < uiNumToTest; uiIndex++)
    {
        U32 uiTestIndex;
        bool bAlreadySet = bloomFindIntKey(bf, puiTestData[uiIndex]);
        bool bAlreadySet2 = bloomFindAndAddIntKey(bf, puiTestData[uiIndex]);
        bool bAlreadySetForReal = false;
        assert( bAlreadySet == bAlreadySet2);

        // should be set true now
        assert( bloomFindIntKey(bf, puiTestData[uiIndex]));

        // make sure that what the bloomfilter thought was unique or not is true
        for (uiTestIndex=0; uiTestIndex<uiIndex && !bAlreadySetForReal; ++uiTestIndex )
        {
            if ( puiTestData[uiIndex] == puiTestData[uiTestIndex] )
            {
                bAlreadySetForReal = true;
            }
        }
        if ( bAlreadySet )
        {
            //printf("Positive\n");
        }

        if ( bAlreadySetForReal != bAlreadySet )
        {
            if ( bAlreadySet )
            {
                // false positive
                uiFalsePositive++;
            }
            else
            {
                assert(0); // false negative
            }
        }
    }

    printf("Found %d false positives out of %d tests of %d unique keys with a planned error rate of %.3f and actual error rate of %.3f\n", uiFalsePositive, uiNumToTest, uiNumUnique, fErrorRate, (F32)uiFalsePositive / (F32)uiNumUnique );
}