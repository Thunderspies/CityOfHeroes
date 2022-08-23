#include "utilitieslib/utils/rand.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"

static U32 uiGlobalLCGSeed = 0x8f8f8f8f;
static U32 uiGlobalBLORNSeed = 0x8f8f8f8f;

static U32  mersenneTable[624];
static U32  mersenneCount = 0;
static bool mersenneInitialized = false;
static U32  mersenneSeed = 0x8f8f8f8f;


void testRandomNumberGenerators()
{
    /*
    U32 uiTestNum = randomU32();
    U32 uiNum = randomU32();
    U32 uiCount = 0;
    while (uiNum != uiTestNum)
    {
        uiCount++;
        uiNum = randomU32();
    }

    printf("Period is %u numbers\n", uiCount);
    */
}

#define BLORN_BITS 15
U32 uiBLORNMask;
U32* puiBLORN;

void generateBLORN()
{
    U32 uiBLORNSize = 0x1 << (BLORN_BITS-1);
    U32 uiIndex;
    uiBLORNMask = uiBLORNSize - 1;
    puiBLORN = malloc(sizeof(U32) * uiBLORNSize);
    for (uiIndex=0; uiIndex<uiBLORNSize; ++uiIndex)
    {
        puiBLORN[uiIndex] = randomU32Seeded(NULL, RandType_Mersenne);
    }
    uiGlobalBLORNSeed = randomU32Seeded(NULL, RandType_Mersenne);
}


// Noise calculation
#define NOISE_PATCH_SIZE 64
static int noise_permutation[NOISE_PATCH_SIZE];
static F32 noise_field[NOISE_PATCH_SIZE];
static bool noise_initialized = false;
static U32 noise_seed = 0;

static void initializeNoise( void )
{
    int i;
    for (i = 0; i < NOISE_PATCH_SIZE; i++)
    {
        noise_permutation[i] = (int)(randomPositiveF32Seeded( &noise_seed, RandType_LCG) * NOISE_PATCH_SIZE);
        noise_field[i] = randomF32Seeded( &noise_seed, RandType_LCG);
    }
    noise_initialized = true;
}

static INLINEDBG int getNoisePermutation2D( int x, int y )
{
    return noise_permutation[(noise_permutation[y & (NOISE_PATCH_SIZE-1)] + x) & (NOISE_PATCH_SIZE-1)];
}

static INLINEDBG int getNoisePermutation3D( int x, int y, int z )
{
    int perm;
    perm = noise_permutation[z          & (NOISE_PATCH_SIZE-1)];
    perm = noise_permutation[(perm + y) & (NOISE_PATCH_SIZE-1)];
    perm = noise_permutation[(perm + x) & (NOISE_PATCH_SIZE-1)];
    return perm;
}

static INLINEDBG F32 getNoiseInterp( F32 t )
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

static INLINEDBG F32 bilinearInterp( F32 values[4], F32 alpha, F32 beta )
{
    F32 lerpa = values[0]*(1.0f - alpha) + values[1]*alpha;
    F32 lerpb = values[2]*(1.0f - alpha) + values[3]*alpha;
    return lerpa*(1.0f - beta) + lerpb*beta;
}

static INLINEDBG F32 trilinearInterp( F32 values[8], F32 alpha, F32 beta, F32 gamma )
{
    return bilinearInterp(values, alpha, beta)*(1.0f - gamma) + 
           bilinearInterp(&(values[4]), alpha, beta)*gamma;
}

F32 noiseGetBand2D( F32 x, F32 y, F32 wavelength )
{
    F32 noise_values[4];
    F32 frequency = 1.0f / wavelength;
    int floorx = floorf(fabsf(x) * frequency );
    int floory = floorf(fabsf(y) * frequency );
    F32 alpha = getNoiseInterp(fabsf(x) * frequency - floorx);
    F32 beta = getNoiseInterp(fabsf(y) * frequency - floory);
    int i;
    for (i = 0; i < 4; i++)
    {
        noise_values[i] = noise_field[ getNoisePermutation2D( floorx + i%2, floory + (i>>1)%2) ];
    }
    return bilinearInterp( noise_values, alpha, beta );
}

F32 noiseGetBand3D( F32 x, F32 y, F32 z, F32 wavelength )
{
    F32 noise_values[8];
    F32 frequency = 1.0f / wavelength;
    int floorx = floorf(fabsf(x) * frequency );
    int floory = floorf(fabsf(y) * frequency );
    int floorz = floorf(fabsf(z) * frequency );
    F32 alpha = getNoiseInterp(fabsf(x) * frequency - floorx);
    F32 beta = getNoiseInterp(fabsf(y) * frequency - floory);
    F32 gamma = getNoiseInterp(fabsf(z) * frequency - floorz);
    int i;
    for (i = 0; i < 8; i++)
    {
        noise_values[i] = noise_field[ getNoisePermutation3D( floorx + i%2, floory + (i>>1)%2, floorz + (i>>2)%2 ) ];
    }
    return trilinearInterp( noise_values, alpha, beta, gamma );
}

F32 noiseGetFullSpectrum2D( F32 x, F32 y, F32 min_wavelength, F32 max_wavelength, F32 falloff_rate )
{
    F32 wavelength = max_wavelength;
    F32 sum = 0;
    while (wavelength > min_wavelength)
    {
        F32 band = noiseGetBand2D( x, y, wavelength );
        band *= 1.0f / powf(max_wavelength / wavelength, falloff_rate);
        sum += band;
        wavelength /= 2.0f;
    }    
    return sum;
}

F32 noiseGetFullSpectrum3D( F32 x, F32 y, F32 z, F32 min_wavelength, F32 max_wavelength, F32 falloff_rate )
{
    F32 wavelength = max_wavelength;
    F32 sum = 0;
    while (wavelength > min_wavelength)
    {
        F32 band = noiseGetBand3D( x, y, z, wavelength );
        band *= 1.0f / powf(max_wavelength / wavelength, falloff_rate);
        sum += band;
        wavelength /= 2.0f;
    }    
    return sum;
}

// Mersenne twister initialization


void initializeMersenneGenerator( void )
{
    int i;
    mersenneTable[0] = mersenneSeed;
    for (i = 1; i < 624; i++)
    {
        mersenneTable[i] = 69069 * mersenneTable[i-1] + 1;
    }
    mersenneInitialized = true;
}

void generateUntemperedMersenneTable( void )
{
    int i;
    U32 x;
    
    if (!mersenneInitialized)
        initializeMersenneGenerator();

    for (i = 0; i < 623; i++)
    {
        x = (0x80000000 & mersenneTable[i]) | (0x7fffffff & mersenneTable[i+1]);
        if (x & 0x1) // x odd?
            mersenneTable[i] = mersenneTable[(i + 397) % 624] ^ (x >> 1) ^ (U32)(2567483615);
        else
            mersenneTable[i] = mersenneTable[(i + 397) % 624] ^ (x >> 1);
    }
    x = (0x80000000 & mersenneTable[623]) | (0x7fffffff & mersenneTable[0]);
    if (x & 0x1) // x odd?
        mersenneTable[623] = mersenneTable[396] ^ (x >> 1) ^ (U32)(2567483615);
    else
        mersenneTable[623] = mersenneTable[396] ^ (x >> 1);
}

void seedMersenneGenerator( U32 seed )
{
    mersenneSeed = seed;
    initializeMersenneGenerator();
    generateUntemperedMersenneTable();
    mersenneCount = 1;
}

// This is the core of the random library... we might want to rearchitect this to be faster at some point
U32 randomStepSeed(U32* uiSeed, RandType eRandType)
{
    switch( eRandType )
    {
        case RandType_LCG:
        {
            // A (possibly incorrect) attempt to get around the LSB's being poorly distributed in a linear congruent PRNG, by using the MSB's from one cycle
            // as the LSB of the next, and running two cycles per step, such that one cycle is hidden.
            // For example, without this, seeds would always alternate between even and odd
            U32* uiLCGSeed = uiSeed?uiSeed:&uiGlobalLCGSeed;
            U32 uiTemp = (*uiLCGSeed * 214013L + 2531011L) & 0xFFFFFFFF;
            *uiLCGSeed = ((uiTemp * 214013L + 2531011L) & 0xFFFF0000) >> 16 | (uiTemp & 0xFFFF0000);
            return *uiLCGSeed;
        }
        xcase RandType_Mersenne:
        {
            // Mersenne twister implementation
            // every 624th call the function will be somewhat slower, as it generates 624 untempered 
            // values in one shot, then performs some swizzling to get the tempered value
            U32 out;
            if ( uiSeed )
            {
                Errorf("Do not pass a seed to the mersenne random generator, instead use seedMersenneGenerator(). Note that it is slow!");
                seedMersenneGenerator(*uiSeed);
            }
            else if (mersenneCount == 0)
                generateUntemperedMersenneTable();

            out = mersenneTable[mersenneCount];
            out ^= out >> 11;
            out ^= (out << 7) & (U32)(2636928640);
            out ^= (out << 15) & (U32)(4022730752);
            out ^= out >> 18;
            mersenneCount = (mersenneCount + 1) % 624;
            return out;
        }
        xcase RandType_BLORN:
        {
            U32* uiBLORNSeed = uiSeed?uiSeed:&uiGlobalBLORNSeed;
            return puiBLORN[(++(*uiBLORNSeed)) & uiBLORNMask];
        }
    }
    Errorf("Unsupported RandType: %d\n", eRandType);
    return 0;
}



void initRand()
{
    generateUntemperedMersenneTable();
    generateBLORN();
    initializeNoise();
} 