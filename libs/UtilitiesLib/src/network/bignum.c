/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 *
 * Module Description:
 *
 *
 ***************************************************************************/
#include "utilitieslib/network/bignum.h"

#include "utilitieslib/utils/endian.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/structNet.h"
#include "utilitieslib/network/netio_core.h"
#include "utilitieslib/network/net_linklist.h"
#include "utilitieslib/network/net_link.h"
#include "utilitieslib/utils/log.h"

static void BigNum_DivMod(BigNum **numerator, BigNum *mod, BigNum **div /*optional*/);
static void BigNum_ToString(char **res, BigNum *a);

MP_DEFINE(BigNum);

static BigNum *BigNum_Create(int data_len)
{
    BigNum *res = NULL;

    // create the pool, arbitrary number
    MP_CREATE(BigNum, 64);
    res = MP_ALLOC( BigNum );
    if( res )
    {
        res->data_len = data_len;
        res->data = calloc(res->data_len, sizeof(*res->data));
    }
    return res;
}

static void BigNum_Realloc(BigNum **r, int data_len)
{
    if(!r)
        return;
    if(!*r)
    {
        *r = BigNum_Create(data_len);
    }
    else
    {
        (*r)->data_len = data_len;
        (*r)->data = realloc((*r)->data, (*r)->data_len*sizeof(*(*r)->data));
    }    
}

static void BigNum_FixTop(BigNum **r)
{
    int i;
    BigNum *res;
    if(!r || !*r)
        return;
    res = *r;
    for( i = res->msd-1; i >= 0 && !res->data[i] ; --i );
    res->msd = i+1;
    // for consistency: zero still counts as a number
    if(0==res->msd)
        res->msd++;
}


// little endian solution, makes more sense on x86
// "0987654321" => 0x0102 0x03040506 0x07080900
//
// openssl print format:
// l = data[n-i]
// 
void BigNum_FromBytes(BigNum **r, U8 const *bytes, int n_bytes )
{
    int data_len;

    if(!r || !bytes || n_bytes <= 0)
        return;

    data_len = (n_bytes+3)/4;
    BigNum_Realloc(r,data_len);
    (*r)->msd = data_len;

    // copy the data
    (*r)->data[(*r)->msd-1] = 0;
    memcpy((*r)->data,bytes,n_bytes);
    BigNum_FixTop(r);
}

void BigNum_ToBytes(U8 **res, int *res_len, BigNum const *a)
{
    if(!res || !res_len || !a)
        return;
    if(!a->msd)
    {
        *res_len = 0;
        return;
    }

    // figure out the bytes needed for msd
    // ab: just grab all the bytes, it is
    // up to the encoder to know sub-DWORD
    // length.
//     top = a->data[a->msd-1];
//     for(n = 4; n >= 0; n--)
//     {
//         if(top & 0xff<<((n-1)*8))
//             break;
//     }
    
    *res_len = BigNum_NumBytes(a);
    *res = realloc(*res, *res_len+1); // give room for NULL
    memcpy(*res,a->data,*res_len);
    (*res)[*res_len] = 0; // just a convenience for debugging, not included in size
}

U32 BigNum_NumBytes(BigNum const *a)
{
    int i;
    U32 tmp;
    if(!a || !a->msd)
        return 0;
    tmp = a->data[a->msd-1];
    // 00 00 00 00 => 0 
    //(okay, msd cannot be zero, unless a == 0, which we count as 1 byte)
    for(i=3; i>0;--i)
    {
        if(tmp&(0xff<<i*8))
            break;
    }
    return (a->msd-1)*4 + i + 1;
}

static void BigNum_FromU32(BigNum **r, U32 d)
{
    if(!r) 
        return;
    BigNum_Realloc(r,1);
    (*r)->data[0] = d;
    (*r)->msd = 1;
    BigNum_FixTop(r);
    STATIC_INFUNC_ASSERT(sizeof(BigNum) == (sizeof(U32*)+sizeof(int)*2));
}

static void BigNum_FromU64(BigNum **r, U64 d)
{
    if(!r)
        return;
    BigNum_Realloc(r,2);
    (*r)->data[0] = 0xFFFFFFFF & d;
    (*r)->data[1] = d >> 32;
    (*r)->msd = 2;
    BigNum_FixTop(r);
    STATIC_INFUNC_ASSERT(sizeof(BigNum) == (sizeof(U32*)+sizeof(int)*2));
}

// resize buffer, does not affect msd

static void BigNum_Copy(BigNum **r, BigNum *a)
{
    if(!r || !a)
        return;
    BigNum_Realloc(r,a->data_len);
    CopyStructs((*r)->data,a->data,a->msd);
    (*r)->msd = a->msd;
    // update this function if new members added
    STATIC_INFUNC_ASSERT(sizeof(BigNum) == (sizeof(U32*)+sizeof(int)*2));
}

void BigNum_Destroy( BigNum **hItem )
{
    if(hItem && *hItem)
    {
        SAFE_FREE((*hItem)->data);
        MP_FREE(BigNum, *hItem);
        *hItem = NULL;
    }
}

#define BIGNUM_SWAP(A,B) \
    {                    \
        BigNum *tmp;     \
        tmp = A;         \
        A = B;           \
        B = tmp;         \
    }
        
#define BIGNUM_SWAP_LARGEST(A,B) \
    if(BigNum_Eq(A,B)<0)             \
    {                                   \
        BigNum *tmp = a;                \
        a = b;                          \
        b = tmp;                        \
    }



int BigNum_Eq(BigNum *a, BigNum *b)
{
    int res = 0;
    int i;
    if(!a || !b)
        return 0;
    
    // well behaved msd has a value in its msd
    res = a->msd - b->msd;
    if(0!=res)
        return res;

    for( i = b->msd - 1; i >= 0; --i ) 
    {
        // can't subtract, 0xffffffff - 0x1 is negative, 
        // is s64 conversion cheaper?
        if(a->data[i] > b->data[i])
            return 1;
        else if(b->data[i] > a->data[i])
            return -1;
    }
    return res;
}

int BigNum_EqU32(BigNum *a, U32 b)
{
    if(!a)
        return 0;
    if(a->msd > 1)
        return 1;
    if(a->msd == 0) // this case is NaN, and is always strictly less than b
        return -1;
    return a->data[0] - b;
}

bool BigNum_IsZero(BigNum *a)
{
    if(!a)
        return FALSE;
    return a->msd == 1 && a->data[0] == 0;
}

static void BigNum_AddU32(BigNum **r, BigNum *a, U32 b, int offset)
{
    BigNum *c;
    U32 carry;
    int i;
    U32 *ad;
    if(!a || !r || offset < 0)
        return;
    BigNum_Realloc(r,a->msd+1);
    c = *r;
    carry = b;
    ad = a->data;
    for(i = offset; i < a->msd && carry; ++i ) 
    {
        U32 tmp = ad[i] + carry;
        if(tmp < ad[i] || tmp < carry)
            carry = 1;
        else 
            carry = 0;
        c->data[i] = tmp;
    }
    for(;i<a->msd;++i)
        c->data[i] = a->data[i];
    c->msd = a->msd;
    if(carry)
    {
        c->data[i] = carry;
        c->msd += 1;
    }
}


static void BigNum_Add(BigNum **r, BigNum *a, BigNum *b)
{
    int msd;
    U32 carry;
    int i;
    BigNum *c;
    U32 *ad;
    U32 *bd;

    if(!a || !b)
        return;

    BIGNUM_SWAP_LARGEST(a,b);

    msd = MAX(a->msd,b->msd);
    BigNum_Realloc(r,msd+1);
    c = *r;

    ad = a->data;
    bd = b->data;
    carry = 0;
    for( i = 0; i < b->msd; ++i ) 
    {
        U32 tmp = ad[i] + bd[i] + carry;
        if(tmp < ad[i] || tmp < bd[i])
            carry = 1;
        else
            carry = 0;
        c->data[i] = tmp;
    }
    // continue carrying
    BigNum_AddU32(&c,a,carry,i);
}

static void BigNum_Mul(BigNum **r, BigNum *a, BigNum *b)
{
    int i;
    BigNum *c;
    U32 *ad, *bd;
    BigNum *r_orig = NULL;
    
    if(!r || !a || !b)
        return;

    // no good way to do in-place multiplication
    // make a temporary if assigning to an arg
    if(*r == a || *r == b)
    {
        r_orig = *r;
        *r = NULL;
    }
    
    BigNum_Realloc(r, b->msd+a->msd);
    (*r)->msd = b->msd+a->msd;
    c = *r;

    BIGNUM_SWAP_LARGEST(a,b);
    
    ad = a->data;
    bd = b->data;

    ZeroStructs(c->data,a->msd+b->msd);
    for( i = 0; i < b->msd; ++i ) 
    {
        int j;
        U32 carry = 0;
        for( j = 0; j < a->msd; ++j ) 
        {
            // no need to worry about overflow,
            // worst case (largest mult, largest carry, largest val)
            // ad[j]*bd[i] = 0xffffffff * 0xffffffff = FFFFFFFE 00000001
            // carry = FFFFFFFE <= largest carry possible
            // data[i+j] = FFFFFFFF <= largest data value
            // c+d = 1 FFFFFFFE
            // c+d+t = FFFFFFFF FFFFFFFF # *phew* just fits (naturally)
            U64 tmp = (U64)ad[j] * bd[i] + carry + c->data[i+j];
            U32 tmp_lo = tmp & 0x00000000FFFFFFFFULL;
            U32 tmp_hi = (tmp & 0xFFFFFFFF00000000ULL)>>32;
            c->data[i+j] = tmp_lo;
            carry = tmp_hi;
        }
        c->data[i+j] = carry;
    }
    BigNum_FixTop(r);

    // if we needed a temporary, copy to passed in value
    // and destroy the temporary (don't destroy passed in)
    if(*r != r_orig)
    {
        BigNum_Copy(&r_orig,*r);
        BigNum_Destroy(r);
        *r = r_orig;
    }
}

static void BigNum_ShiftLeft(BigNum **a)
{
    U32 carry = 0;
    int i;
    if(!a || !*a)
        return;
    
    for( i = 0; i < (*a)->msd; ++i )
    {
        U32 tmp = (*a)->data[i] >> 31;
        (*a)->data[i] = ((*a)->data[i]<<1) | carry ;
        carry = tmp;
    }
    if(carry)
    {
        (*a)->msd++;
        BigNum_Realloc(a,(*a)->msd);
        (*a)->data[(*a)->msd-1] = 1;
    }
}

static void BigNum_ShiftRight(BigNum **a)
{
    U32 carry = 0;
    int i;
    if(!a || !*a)
        return;
    
    for( i = (*a)->msd - 1; i >= 0; --i )
    {
        U32 tmp = ((*a)->data[i] & 0x1) << 31;
        (*a)->data[i] =  ((*a)->data[i] >> 1) | carry;
        carry = tmp;
    }
    BigNum_FixTop(a);
}


static void BigNum_SubU32(BigNum **r, BigNum *a, U32 amt, int offset)
{
    int i;
    U32 orig;

    if(!r || !a || !amt || offset < 0)
        return;
    
    BigNum_Realloc(r,a->msd);
    i = offset;
    for(i = offset; i < a->msd; ++i)
    {
        orig = a->data[i];
        (*r)->data[i] = orig - amt;
        if((*r)->data[i] < orig) // no underflow
            break;
        amt = 1;
    } 

    // if we made it to msd, we could have reduced it
    BigNum_FixTop(r);
}

void BigNum_Sub(BigNum **r, BigNum *a, BigNum *b)
{
    int i;
    int carry;
    int n;
    if(!r || !a || !b)
        return;
    BigNum_Realloc(r,MAX(a->msd,b->msd));

    carry = 0;
    n = MIN(a->msd,b->msd);

    // subtract b from a until all words are covered
    for( i = 0; i < n; ++i ) 
    {
        U32 tmp = a->data[i] - b->data[i] - carry;
        if(tmp > a->data[i])
            carry = 1;
        else 
            carry = 0;
        (*r)->data[i] = tmp;
    }
    BigNum_FixTop(r);
    if(carry)
        BigNum_SubU32(r,a,carry,i); // subtract the final carry 
}

//----------------------------------------
//  Integer devision:
// A/B = div(a,b)
//
// while(B<A)
//  i++
//  B *= 2
//
// r = 2^i
// C = B/2
// while(i>=0)
//   if(B<A)
//      r += 2^i
//      B += C
//   B /= 2
//   i--
//----------------------------------------
// static void BigNum_Div(BigNum **res, BigNum *a, BigNum *b)
// {
// }

//----------------------------------------
//  Set a bit. 
// warning, allocates as much as is needed
//----------------------------------------
static void BigNum_SetBit(BigNum **res, int bit)
{
    int i;
    int b;
    BigNum *r;
    if(!res || bit < 0)
        return;
    i = bit/32;
    b = bit%32;
    BigNum_Realloc(res,i+1);
    r = *res;
    while(r->msd <= i)          // zero any wilderness
        r->data[r->msd++] = 0;
    r->data[i] = 1<<b | r->data[i];
}


// my impl. I'm sure there is a much faster way.)
// theory:
//  1. (a - mod) % mod = a % mod
//  2. (a - N*mod) % mod = a % mod
// 
// so, double mod repeatedly until a is exceeded, then subtract normally
// 
//  m := mod
//  while a > m do
//     m := m * 2;
//
//  m := m / 2;
//  i := i - 1;
// 
//  # m < a
//  # i has number of doubles we made left.
//  # since m <= a < m*2
//  while i > 0 do
//       if a > m
//         a := a - m;
//       m := m / 2;
//       i := i - 1;
//  end
//    
// O(logn) = O(logn + logn + logn)
static void BigNum_DivMod(BigNum **numerator, BigNum *mod, BigNum **div /*optional*/)
{
    BigNum *a;
    BigNum *m;
    BigNum *d = NULL;
    
    int i;
    if(!numerator || !*numerator || !mod)
        return;

    if(div)
    {
        BigNum_FromU32(div,0);
        d = *div;        
    }
    
    // mod bigger than value short circuit
    if(mod->msd > (*numerator)->msd)
        return;

    a = *numerator;
    m = NULL;
    BigNum_Copy(&m,mod);

    // double until we exceed
    for(i = 0; BigNum_Eq(a,m) >= 0; ++i)
        BigNum_ShiftLeft(&m);

    // get below
    i--;
    BigNum_ShiftRight(&m);

    // subtract the rest
    for(; i >= 0; --i)
    {
        if(BigNum_Eq(a,m) >= 0)
        {
            BigNum_Sub(&a,a,m);
            if(d)
                BigNum_SetBit(&d,i);
        }
        
        BigNum_ShiftRight(&m);
    }

    BigNum_Destroy(&m);
    // not needed
//     while(BigNum_Eq((*a),mod) > 0)
//         BigNum_Sub(a,(*a),mod);
}

static void BigNum_Mod(BigNum **numerator, BigNum *mod)
{
    BigNum_DivMod(numerator,mod,NULL);
}

static void BigNum_Div(BigNum **div, BigNum *numerator, BigNum *denom)
{
    BigNum *r = NULL;
    BigNum_Copy(&r,numerator);
    BigNum_DivMod(&r,denom,div);
    BigNum_Destroy(&r);
}

//----------------------------------------
//  c = (n^e) % m
//
// Exponentiation by squaring (wikipedia)
//  In summary, it costs as much to do 10^2 * 10^2 as it does to do 10*10.
//  So square, and halve exponent until you get an odd value, pull that out
//  so you can keep squaring. e.g.:
//   1. 10^3 = 10*10^2
//   2. 10^6 = pow(10^2,3) = 10^2 * 10^4 <- so pull out one 10^2, and double.
//   3. 10^24 = pow(10^2,12) = pow(10^4,6) = pow(10^8,3) = 10^8 * 10^16
//
// Pow(x,n) =
//      n == 0    -> 1
//      n is odd  -> x * Pow(x^2,(n-1)/2)
//      n is even -> Pow(x^2,n/2)
//
// the modulus operation has the property:
// (a*b)% m == (a%m * b%m)
//
// so you can take the modulus at each step:
// e.g. 4^13 % 497
//   4, e' = 1, c = (4) % 497
//  16, e' = 2, c = (4 * 4) % 497
//  64, e' = 2, c = (4 * 4 * 4) % 497
// 256, 
//  30, <= mod step.
// 120, <= 30 * 4
// ...
// 445
// PowMod(x,n) =
//      n == 0          -> 1
//      n is odd        -> x%m * Pow(x^2,n-1/2)
//      n is even       -> Pow(x^2,n/2)
//----------------------------------------
void BigNum_PowMod(BigNum **res, BigNum *num, BigNum  *exp, BigNum *m)
{
    BigNum *e = NULL;
    BigNum *n = NULL;
    BigNum *r = NULL;
    if(!res || !num || !exp || !m)
        return;    

    // modifying these
    BigNum_Copy(&e,exp);
    BigNum_Copy(&n,num);

    // reserve enough bits for the square
    BigNum_Realloc(res,MAX(m->msd,e->msd*2));
    r = *res;
    r->msd = 1;
    r->data[0] = 1;

    while(!BigNum_IsZero(e))
    {
        if(e->data[0] & 1) // odd
        {
            BigNum_Mul(&r,r,n);
            BigNum_Mod(&r,m);
        }
        BigNum_Mul(&n,n,n);
        BigNum_Mod(&n,m);
        BigNum_ShiftRight(&e);
    }
}


// relatively big fibs.
static U64 fibs[] = 
{
    0,    1,    1,    2,    3,    5,    8,    13,    21,    34,    55,    89,    144,    233,    377,    610,    987,    1597,    2584,    4181,    6765,    10946,    17711,    28657,    46368,    75025,    121393,    196418,    317811,    514229,    832040,    1346269,    2178309,    3524578,    5702887,    9227465,    14930352,    24157817,    39088169,    63245986,    102334155,    165580141,    267914296,    433494437,    701408733,    1134903170,    1836311903,    2971215073,    4807526976,    7778742049,    12586269025,    20365011074,    32951280099,    53316291173,    86267571272,    1.39584E+11,    2.25851E+11,    3.65435E+11,    5.91287E+11,    9.56722E+11,    1.54801E+12,    2.50473E+12,    4.05274E+12,    6.55747E+12,    1.06102E+13,    1.71677E+13,    2.77779E+13,    4.49456E+13,    7.27235E+13,    1.17669E+14,    1.90392E+14,    3.08062E+14,    4.98454E+14,    8.06516E+14,    1.30497E+15,    2.11149E+15,    3.41645E+15,    5.52794E+15,    8.94439E+15,    1.44723E+16,    2.34167E+16,    3.78891E+16,    6.13058E+16,    9.91949E+16,    1.60501E+17,    2.59695E+17,    4.20196E+17,};


//----------------------------------------
//  euclid's greatest common divisor algo:
// The premise is subtracting two numbers
// does not affect their common divisors
// e.g. 
// a = d*a'
// b = d*b'
// a - b = d*(a' - b')
// ab: 
//----------------------------------------
// void BigNum_GCD(BigNum **res, BigNum *a_in, BigNum *b_in)
// {
//     BigNum *a = NULL;
//     BigNum *b = NULL;
//     if(!res || !a_in || !b_in)
//         return;
    
//     BigNum_Copy(&a,a_in);
//     BigNum_Copy(&b,b_in);
//     while(BigNum_Eq(a,b) != 0)
//     {
//         BIGNUM_SWAP_LARGEST(a_in,b_in);
//         BigNum_Sub(&a,a,b);
//     }
//     BigNum_Copy(res,a);
//     BigNum_Destroy(&a);
//     BigNum_Destroy(&b);
// }

// x_res is mod inverse if gcd is 1
// ab: need negative BigNum's to do this
// void BigNum_GCD(BigNum **x_res, BigNum **y_res, BigNum **gcd, BigNum *a_in, BigNum *b_in)
// {
//     BigNum *x = NULL;
//     BigNum *y = NULL;
//     BigNum *a = NULL;
//     BigNum *b = NULL;
//     BigNum *tmp = NULL;
//     BigNum *tmp2 = NULL;
//     BigNum *quotient = NULL;
//     BigNum *lastx = NULL;
//     BigNum *lasty = NULL;
//     if(!a_in || !b_in || !x_res || !y_res || !gcd)
//         return;

//     BigNum_FromU32(&x,0);
//     BigNum_FromU32(&y,1);
//     BigNum_FromU32(&lastx,1);
//     BigNum_FromU32(&lasty,0);

//     BigNum_Copy(&a,a_in);
//     BigNum_Copy(&b,b_in);
    
//     while(b->msd)
//     {
//         BigNum_DivMod(&a,b,&quotient); // a = a % b
//         BIGNUM_SWAP(a,b);              // a = b, b = a % b

//         // new_x = lastx - quotient*x
//         // lastx = x
//         // x = new_x
//         BigNum_Mul(&tmp,quotient,x);
//         BigNum_Sub(&tmp2,lastx,tmp); // tmp2 = new_x
//         BIGNUM_SWAP(lastx,x);        // lastx = x
//         BIGNUM_SWAP(tmp2,x);         // x = new_x

//         BigNum_Mul(&tmp,quotient,y);
//         BigNum_Sub(&tmp2,lasty,tmp); // tmp2 = new_y
//         BIGNUM_SWAP(lasty,y);        // lasty = y
//         BIGNUM_SWAP(tmp2,y);         // y = new_y
//     }

//     BigNum_Copy(x_res,lastx);
//     BigNum_Copy(y_res,lasty);
//     BigNum_Copy(gcd,a);

//     BigNum_Destroy(&x);
//     BigNum_Destroy(&y);
//     BigNum_Destroy(&a);
//     BigNum_Destroy(&b);
//     BigNum_Destroy(&tmp);
//     BigNum_Destroy(&tmp2);
//     BigNum_Destroy(&quotient);
//     BigNum_Destroy(&lastx);
//     BigNum_Destroy(&lasty);
// }

//----------------------------------------
//  inputs:
// - two primes p,q
// out:
// - n : modulus p*q
// - e : exponent, (chosen as 0x10001)
// - d : private exponent, de = 1(%totient(n)) (de is 1 congruent to n)
//
// totient(p,q) = (p-1)(q-1)
//----------------------------------------
// void BigNum_GenKeyFromPrimes(BigNum **n, BigNum **e, BigNum **d, BigNum *p, BigNum *q)
// {
//     int i;
//     BigNum *gcd = NULL;
//     BigNum *totient = NULL;
//     BigNum *ptmp = NULL;
//     BigNum *qtmp = NULL;
    
//     if(!n || !e || !d || !p || !q)
//         return;

//     BigNum_Mul(n,p,q);
//     BigNum_FromU32(e,0x10001);
    
//     BigNum_SubU32(&ptmp,p,1,0);
//     BigNum_SubU32(&qtmp,q,1,0);
//     BigNum_Mul(&totient,ptmp,qtmp);
    
//     i = 0;
//     {
//         BigNum_FromU32(&ptmp,i*2+1);
//         BigNum_Mul(d,totient)
//         BigNum_GCD();
//                        ++i;
//     } while(gcd->msd != 1 && gcd->data[0] != 1);

//     // cleanup
//     BigNum_Destroy(&totient);
//     BigNum_Destroy(&ptmp);
//     BigNum_Destroy(&qtmp);
// }


//----------------------------------------
// a raw encrypt. 'RSAEP' in docs
//  PKCSl-v1_5 RSA encryption:
// IN:
//  (n,e) = public key mod and exponent
//   msg = what to encrypt
// OUT:
//  C : ciphertext
//
// algo:
//   - c = m^e mod n
//----------------------------------------
bool BigNum_RsaEncrypt(BigNum **res, BigNum *m, BigNum *e, BigNum *n)
{

    if(!res || !e || !n || !m)
        return FALSE;


#define PARAM_ERROR(T,E) if(T) {                 \
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "RsaEncrypt error: " # E);; \
        return FALSE;                                     \
    }

    PARAM_ERROR(BigNum_Eq(n,e) <= 0,"key smaller than exponent.");
    PARAM_ERROR(BigNum_Eq(n,m) <= 0,"data larger than modulus");
    PARAM_ERROR(n->msd >= 512,"modulus cannot be longer than 2048 bytes");
#undef PARAM_ERROR

//    BigNum_FromU32(&e,0x10001); // 65537, commonly used.
//     BigNum_FromBytes(&n,pubkey,pubkey_len);
    BigNum_PowMod(res,m,e,n);
    return TRUE;
}

//----------------------------------------
// A raw RSA decrypt, 'RSADP' in docs
// (n,d) : private key's mod and exponent
// c : ciphertext
// 
// m = c^d % n
//----------------------------------------
bool BigNum_RsaDecrypt(BigNum **res, BigNum *c, BigNum *d, BigNum *n)
{
    char *fail_msg = NULL;
    if(!n || !d || !c || !res) 
        return FALSE;

    if(BigNum_Eq(c,n) >= 0)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "RsaDecrypt: private mod smaller than message, impossible.");
        return FALSE;
    }

    BigNum_PowMod(res,c,d,n);
    return TRUE;
}

void BigNum_FromBigEndBytes(BigNum **res,U8 const *bytes, U8 bytes_len)
{
    U8 *tmp;
    if(!bytes || bytes_len < 0)
        return;
    tmp = malloc(bytes_len);
    memcpy(tmp,bytes,bytes_len);
    reverse_bytes(tmp,bytes_len);
    BigNum_FromBytes(res,tmp,bytes_len);
    SAFE_FREE(tmp);
}

void BigNum_ToBigEndBytes(U8 **res, int *res_len, BigNum const *a)
{
    BigNum_ToBytes(res,res_len,a);
    reverse_bytes(*res,*res_len);
}


// debug: convert to base 10 string
static void BigNum_ToString(char **res, BigNum *a)
{
    BigNum *div = NULL;
    BigNum *mod = NULL;
    BigNum *ten = NULL;
    BigNum *t = NULL;
    int n;
    char *s;
    if(!res || !a)
        return;

    // figure out max number of characters needed
    // 2^(32*a->msd) = 10^n
    // lg_10(2) * 32*a->msd = n
    // lg_10(2) ~= .302
    // 
    // eg. msd = 1 => (.302 * 32) => 9, add 1 = 10, and you have enough
    // for 4 billion, add one more for NULL.
    n = (0.302f * a->msd * 32) + 2; 
    *res = realloc(*res,n);
    s = *res;
    BigNum_Copy(&t,a);
    BigNum_FromU32(&ten,10);
    while(!BigNum_IsZero(t))
    {
        BigNum_Copy(&mod,t);
        BigNum_DivMod(&mod,ten,&t);
        *s++ = mod->data[0] + '0';
    }
    reverse_bytes(*res,s - *res);
    *s = 0;
}

int BigNum_Test()
{
    int r_bytes_len;
    U8 *r_bytes = NULL;
    int num_errors = 0, num_passed = 0;
    char *dbg_str = NULL;
    int i;
    BigNum *a = NULL;
    BigNum *b = NULL;
    BigNum *d = NULL;
    BigNum *e = NULL;
    BigNum *m = NULL;
    BigNum *n = NULL;
    BigNum *r = NULL;

#define TOSTR2(A) #A
#define TOSTR(A) TOSTR2(A)
#define TEST(X) if(!(X)) {                      \
        printf(TOSTR(__LINE__) ": " #X " failed.\n");  \
        ++num_errors;                           \
    } else num_passed++;

#define TESTU32(A,U) TEST(0 == BigNum_EqU32(A,U));

    // from bytes 
    {
        // test BigNum_NumBytes
        {
            BigNum_FromU32(&r,0);
            TEST(BigNum_NumBytes(r) == 1);
        }
        
        // tobytes
        {
            BigNum_FromU32(&r,0xaa);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);
            TEST(r_bytes_len == 1 && r_bytes[0] == 0xaa);

            BigNum_FromU32(&r,0xbbaa);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);
            TEST(r_bytes_len == 2 && r_bytes[0] == 0xaa && r_bytes[1] == 0xbb);

            BigNum_FromU32(&r,0xccbbaa);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);
            TEST(r_bytes_len == 3 && r_bytes[0] == 0xaa && r_bytes[1] == 0xbb && r_bytes[2] == 0xcc);

            BigNum_FromU32(&r,0xddccbbaa);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);
            TEST(r_bytes_len == 4 && r_bytes[0] == 0xaa && r_bytes[1] == 0xbb && r_bytes[2] == 0xcc && r_bytes[3] == 0xdd);
        }
        
        // check 1234567890
        {
            char str[] = { 0, 9, 8, 7, 6, 5, 4, 3, 2, 1 };
            BigNum_FromBytes(&r,str, ARRAY_SIZE(str));
            TEST(r->data[0] == 0x07080900);
            TEST(r->data[1] == 0x03040506);
            TEST(r->data[2] == 0x0102);
            TEST(r->msd == 3);
            TEST(BigNum_NumBytes(r) == ARRAY_SIZE(str));
        }

        // check 12345678
        {
            char str[] = { 8, 7, 6, 5, 4, 3, 2, 1 };
            BigNum_FromBytes(&r,str, ARRAY_SIZE(str));
            TEST(r->data[0] == 0x05060708);
            TEST(r->data[1] == 0x01020304);
            TEST(r->msd == 2);
            TEST(BigNum_NumBytes(r) == ARRAY_SIZE(str));
        }

        // march across bytes
        for( i = 0; i < 8; ++i ) 
        {
            U8 src[8] = {0};
            U32 res_expected = 0xAB<<(i%4)*8;
            U32 res;
            src[i] = 0xAB;
            BigNum_FromBytes(&r, src,ARRAY_SIZE(src));
            res = r->data[i/4];
            TEST( res == res_expected );
            TEST( r->msd == i/4+1 );
        }
    }

    // add
    {
        char as[] = { 1, 255, 255, 255, 
                      254, 255, 255, 255,
                      255, 255, 255, 255}; 
        BigNum_FromBytes(&a,as,ARRAY_SIZE(as));
        BigNum_AddU32(&r,a,255,0);
        TEST(r->msd == 3);
        TEST(r->data[0] == 0);
        TEST(r->data[1] == 0xffffffff);
        TEST(r->data[2] == 0xffffffff);
        
        BigNum_FromU32(&b,255);
        BigNum_Add(&r,b,a);
        TEST(r->msd == 3);
        TEST(r->data[0] == 0);
        TEST(r->data[1] == 0xffffffff);
        TEST(r->data[2] == 0xffffffff);        
    }

    // equal
    {
        BigNum_FromU64(&a,0x100000000);
        BigNum_FromU32(&b,1);
        TEST(BigNum_Eq(a,b) > 0);
        TEST(BigNum_Eq(b,a) < 0);
        TEST(BigNum_Eq(b,b) == 0);
        TEST(BigNum_Eq(a,a) == 0);
    }
    
    // shift
    {
        BigNum_FromU32(&r,1);
        BigNum_ShiftRight(&r);
        TEST( 0 == BigNum_EqU32(r,0) );

        BigNum_FromU64(&r, 0x100000000);
        BigNum_ShiftRight(&r);
        TEST( r->data[0] == 0x80000000 );
        TEST( r->msd == 1 );

        BigNum_FromU32(&r,1);
        BigNum_ShiftLeft(&r);
        TEST( r->data[0] == 2 );
        TEST( r->msd == 1 );

        BigNum_FromU64(&r, 0x80000000);
        BigNum_ShiftLeft(&r);
        TEST( r->data[1] == 1 && r->data[0] == 0);
        TEST( r->msd == 2 );
    }

    // mul
    {
        // biggest biggest
        {
            BigNum_FromU64(&a,0xFFFFFFFFFFFFFFFFULL);
            BigNum_Mul(&r,a,a);
            // FFFFFFFF,FFFFFFFE,00000000,00000001
            TEST(r->msd == 4 
                 && r->data[0] == 1 
                 && r->data[1] == 0 
                 && r->data[2] == 0xFFFFFFFE 
                 && r->data[3] == 0xFFFFFFFF);
        }

        // biggest random
        {
            BigNum_FromU64(&a,123456789987654321);
            BigNum_Mul(&r,a,a);
            // 2EF77 C5D89413 866E8B51 4EE22E61
            TEST(r->msd == 4 
                 && r->data[0] == 0x4EE22E61 
                 && r->data[1] == 0x866E8B51
                 && r->data[2] == 0xC5D89413
                 && r->data[3] == 0x2EF77 );
        }

        // biggest
        {
            // 0x1 00000000
            BigNum_FromU64(&a,1ULL<<32);
            BigNum_Mul(&r,a,a);
            // 0x1 00000000 00000000
            TEST(r->msd == 3 && r->data[2] == 1);
        }
        // biggest
        {
            BigNum_FromU64(&a,10000000000000000000);
            BigNum_Mul(&r,a,a);
            BigNum_ToString(&dbg_str,r);
            TEST(0 == strcmp(dbg_str,"100000000000000000000000000000000000000"));
            // 4B3B4CA8 5A86C47A 098A2240 00000000
        }

        // simple
        {
            BigNum_FromU32(&a,4);
            BigNum_Mul(&r,a,a);
            TESTU32(r,16);
        }
    
        // simple
        {
            BigNum_FromU32(&a,4);
            BigNum_FromU32(&b,5);
            BigNum_Mul(&r,a,b);
            TEST( r->data[0] == 20 );
            TEST( r->msd == 1 );
        }
        
        // bigger
        {
            BigNum_FromU32(&a,2); 
            BigNum_FromU64(&b,1ULL<<62);
            BigNum_Mul(&r,a,b);
            TEST( r->data[0] == 0 );
            TEST( r->data[1] == 1UL<<31 );
            TEST( r->msd == 2 );
        }

        // biggest
        {
            BigNum_FromU32(&a,2); 
            BigNum_FromU64(&b,1ULL<<63);
            BigNum_Mul(&r,a,b);
            TEST( r->data[0] == 0 );
            TEST( r->data[1] == 0 );
            TEST( r->data[2] == 1 );
            TEST( r->msd == 3 );
        }

        // self 
        {
            BigNum_FromU32(&a,4);
            BigNum_FromU32(&b,5);
            BigNum_Mul(&a,a,b);
            TEST( a->data[0] == 20 );
            TEST( a->msd == 1 );
        }
    }

    // mod
    {        
        // simple
        {
            BigNum_FromU32(&a,4);
            BigNum_FromU32(&r,5);
            BigNum_DivMod(&r,a,&d);
            TEST( BigNum_EqU32(r,1) == 0 );
            TEST( BigNum_EqU32(d,1) == 0 );
        }

        // simple
        {
            BigNum_FromU32(&a,4);
            BigNum_FromU32(&r,22);
            BigNum_DivMod(&r,a,&d);
            TEST( BigNum_EqU32(r,2) == 0 );
            TEST( BigNum_EqU32(d,5) == 0 );
        }

        // simple, but a little tricky
        {
            BigNum_FromU32(&a,22);
            BigNum_FromU32(&r,22);
            BigNum_DivMod(&r,a,&d);
            TEST( BigNum_EqU32(r,0) == 0 );
            TEST( BigNum_EqU32(d,1) == 0 );
        }

        // simple, but a little tricky
        {
            BigNum_FromU32(&a,22);
            BigNum_FromU32(&r,21);
            BigNum_DivMod(&r,a,&d);
            TEST( BigNum_EqU32(r,21) == 0 );
            TEST( BigNum_EqU32(d,0) == 0 );
        }        

        // bigger
        {
            BigNum_FromU64(&a,0x100000000); 
            BigNum_FromU64(&r,-1);
            BigNum_DivMod(&r,a,&d);
            TEST( BigNum_EqU32(r, 0xffffffff) == 0 );
            TEST( BigNum_EqU32(d, 0xffffffff) == 0 );
        }

        // bigger base 10
        {
            BigNum_FromU64(&a,1000); 
            BigNum_FromU64(&r,999999); 
            BigNum_DivMod(&r,a,&d);
            TEST( BigNum_EqU32(r,999) == 0 );
            TEST( BigNum_EqU32(d,999) == 0 );
        }

        // bigger again
        {
            BigNum_FromU64(&a,1000);
            BigNum_FromU64(&r,10999); 
            BigNum_DivMod(&r,a,&d);
            TEST( BigNum_EqU32(r,999) == 0 );
            TEST( BigNum_EqU32(d,10)  == 0 );
        }

        // bigger again
        {
            BigNum_FromU64(&a,10000000000); // 10 billion
            BigNum_FromU64(&r,10000000009999999999); // 20 digits
            BigNum_FromU64(&b,9999999999);
            BigNum_DivMod(&r,a,&d);
            TEST( 0 == BigNum_Eq(r,b) );
            TEST( 0 == BigNum_EqU32(d,1000000000));
        }
    }
    

    // modexp
    {
        // even biggerer
        {
            BigNum_FromU64(&a,123456789987654321);
            BigNum_FromU32(&e,3);
            BigNum_FromU32(&n,10);
            BigNum_PowMod(&r,a,e,n);
            TESTU32(r,1);
        }

        // super simple
        {
            BigNum_FromU32(&b,4);
            BigNum_FromU32(&e,3);
            BigNum_FromU32(&m,10); 
            BigNum_PowMod(&r,b,e,m);
            TEST(r->data[0] == 4); // 64 % 10
        }

        // simple
        {
            BigNum_FromU32(&b,4);
            BigNum_FromU32(&e,13);
            BigNum_FromU32(&m,497);
            BigNum_PowMod(&r,b,e,m);
            TEST(r->data[0] == 445);
        }

        // biggerer
        {
            BigNum_FromU32(&b,96);
            BigNum_FromU32(&e,10);
            BigNum_FromU64(&m,1ULL<<48);
            BigNum_PowMod(&r,b,e,m);
            TEST(0==BigNum_EqU32(r,0));
        }        
    }

    // base64
    {
        char *data = "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDmCYgI9OIBsb1Erth7ReZ2d76uLmtcLmWsL0P8e8M6b5inYT8Apnzf022pK5ynBashaOad+65ET0KINQa8705ZOlzTzvCImBjYXoLNL/pmgGpaKnj87JcStzz/3crvSe4EWfuQnq8R6MRWV3U+meinRYY53svISE4AzfFnPb7GTQIDAQAB";
        U8 bytes[] =
        {
0x30, 0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01,
0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89, 0x02, 0x81, 0x81, 0x00, 0xe6, 0x09, 0x88,
0x08, 0xf4, 0xe2, 0x01, 0xb1, 0xbd, 0x44, 0xae, 0xd8, 0x7b, 0x45, 0xe6, 0x76, 0x77, 0xbe, 0xae,
0x2e, 0x6b, 0x5c, 0x2e, 0x65, 0xac, 0x2f, 0x43, 0xfc, 0x7b, 0xc3, 0x3a, 0x6f, 0x98, 0xa7, 0x61,
0x3f, 0x00, 0xa6, 0x7c, 0xdf, 0xd3, 0x6d, 0xa9, 0x2b, 0x9c, 0xa7, 0x05, 0xab, 0x21, 0x68, 0xe6,
0x9d, 0xfb, 0xae, 0x44, 0x4f, 0x42, 0x88, 0x35, 0x06, 0xbc, 0xef, 0x4e, 0x59, 0x3a, 0x5c, 0xd3,
0xce, 0xf0, 0x88, 0x98, 0x18, 0xd8, 0x5e, 0x82, 0xcd, 0x2f, 0xfa, 0x66, 0x80, 0x6a, 0x5a, 0x2a,
0x78, 0xfc, 0xec, 0x97, 0x12, 0xb7, 0x3c, 0xff, 0xdd, 0xca, 0xef, 0x49, 0xee, 0x04, 0x59, 0xfb,
0x90, 0x9e, 0xaf, 0x11, 0xe8, 0xc4, 0x56, 0x57, 0x75, 0x3e, 0x99, 0xe8, 0xa7, 0x45, 0x86, 0x39,
0xde, 0xcb, 0xc8, 0x48, 0x4e, 0x00, 0xcd, 0xf1, 0x67, 0x3d, 0xbe, 0xc6, 0x4d, 0x02, 0x03, 0x01, 
0x00, 0x01
        };
        char *d = NULL; 
        char *bts = NULL;
        int n;
        base64_encode(&d,bytes,ARRAY_SIZE(bytes));
        TEST(0 == strcmp(d,data));
        base64_decode(&bts,&n,d);
        TEST(n == ARRAY_SIZE(bytes));
        TEST(memcmp(bts,bytes,n) == 0);
        SAFE_FREE(d);
        SAFE_FREE(bts);

//         reverse_string(bytes);
//         BigNum_FromBytes(&a,bytes,ARRAY_SIZE(bytes));
    }

    // BigNum_ToString
    {
        BigNum_FromU32(&r,10);
        BigNum_ToString(&dbg_str,r);
        TEST(0 == strcmp(dbg_str,"10"));
    }

    // rsa encryption from fundamental operations
    {
        // simple
        {
            // p = 32771
            // q = 32779
            // n = 1074200609 # pq
            // totient = 1074135060
            // e = 65537 
            // d = 489955193
            char msg[] = "hi!";            
            BigNum_FromBytes(&a,msg,ARRAY_SIZE(msg));
            BigNum_FromU32(&n,1074200609);
            BigNum_FromU32(&e,65537); 
            BigNum_FromU32(&d,489955193);
            
//             printf("encrypt: b = a^e%%n\n");
//             BigNum_ToString(&dbg_str,a);
//             printf("a: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,e);
//             printf("e: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,n);
//             printf("mod: %s\n",dbg_str);
            
            BigNum_PowMod(&b,a,e,n);
            
//             printf("\ndecrypt: r = b^d%%n\n");
//             BigNum_ToString(&dbg_str,b);
//             printf("b: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,d);
//             printf("d: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,n);
//             printf("mod: %s\n",dbg_str);
            
            BigNum_PowMod(&r,b,d,n);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);
            TEST(0 == strcmp(msg,r_bytes));
        }
        
        // simple, from openssl created keys
        {
            // n = 3305663677 # pq
            // e = 65537 
            // d = 129877573 
            char msg[] = "abc";
            BigNum_FromBytes(&a,msg,ARRAY_SIZE(msg));
            BigNum_FromU64(&n,3305663677);
            BigNum_FromU32(&e,65537); 
            BigNum_FromU32(&d,129877573);
            
//             printf("encrypt: b = a^e%%n\n");
//             BigNum_ToString(&dbg_str,a);
//             printf("a: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,e);
//             printf("e: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,n);
//             printf("mod: %s\n",dbg_str);
            
            BigNum_PowMod(&b,a,e,n);
            
//             printf("\ndecrypt: r = b^d%%n\n");
//             BigNum_ToString(&dbg_str,b);
//             printf("b: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,d);
//             printf("d: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,n);
//             printf("mod: %s\n",dbg_str);
            
            BigNum_PowMod(&r,b,d,n);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);

            TEST(0 == strcmp(msg,r_bytes));
        }
        
        
        // complicated, from openssl created keys 
        {
// from pubkey:
//             
            char mod_bytes[] = {0x00, 0xd9, 0x2e, 0x8b, 0xc4, 0x1a, 0xb1, 0xb0, 
                                0x25, 0x81, 0x2d, 0x5c, 0x64, 0x9c, 0xc5, 0xa5, 
                                0xdd};
            char d_bytes[] = { // private exponent
                0x03, 0x71, 0x70, 0x77, 0xd0, 0x70, 0x86, 0xb9, 
                0x5a, 0x03, 0x92, 0xbc, 0x4a, 0x10, 0xc3, 0x25
            };
            char msg[] = "aaaaaaaabbbbbb\r\n";
            
            reverse_bytes(mod_bytes,ARRAY_SIZE(mod_bytes));
            reverse_bytes(d_bytes,ARRAY_SIZE(d_bytes));
            
            BigNum_FromBytes(&n,mod_bytes,ARRAY_SIZE(mod_bytes));
            BigNum_FromU32(&e,65537);
            BigNum_FromBytes(&d,d_bytes,ARRAY_SIZE(d_bytes));
            
            BigNum_FromBytes(&a,msg,ARRAY_SIZE(msg));
            // 10,334,410,032,606,748,633,331,426,664
//             printf("encrypt:\n");
//             BigNum_ToString(&dbg_str,a);
//             printf("a: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,e);
//             printf("e: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,n);
//             printf("n: %s\n",dbg_str);
            
            BigNum_PowMod(&b,a,e,n);
            
//             printf("\ndecrypt:\n");
//             BigNum_ToString(&dbg_str,b);
//             printf("b: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,d);
//             printf("d: %s\n",dbg_str);
//             BigNum_ToString(&dbg_str,n);
//             printf("n: %s\n",dbg_str);
            
            BigNum_PowMod(&r,b,d,n);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);
            r_bytes[r_bytes_len] = 0;
            TEST(0==strcmp(r_bytes,msg));       
        }
    }

    // rsa encrypt/decrypt
    {        
        // simple, from openssl created keys
        {
            // n = 3305663677 # pq
            // e = 65537 
            // d = 129877573 
            char msg[] = "a";
            BigNum_FromBytes(&a,msg,ARRAY_SIZE(msg));
            BigNum_FromU64(&n,3305663677);
            BigNum_FromU32(&e,65537); 
            BigNum_FromU32(&d,129877573);
            BigNum_RsaEncrypt(&b,a,e,n);
            BigNum_RsaDecrypt(&r,b,d,n);
            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);

            TEST(0 == strcmp(msg,r_bytes));
        }
        
        
        // complicated, from openssl created keys 
        {
// from pubkey:
//             
            char mod_bytes[] = {0x00, 0xd9, 0x2e, 0x8b, 0xc4, 0x1a, 0xb1, 0xb0, 
                                0x25, 0x81, 0x2d, 0x5c, 0x64, 0x9c, 0xc5, 0xa5, 
                                0xdd};
            char d_bytes[] = { // private exponent
                0x03, 0x71, 0x70, 0x77, 0xd0, 0x70, 0x86, 0xb9, 
                0x5a, 0x03, 0x92, 0xbc, 0x4a, 0x10, 0xc3, 0x25
            };
            char msg[] = "ab";
            
            reverse_bytes(mod_bytes,ARRAY_SIZE(mod_bytes));
            reverse_bytes(d_bytes,ARRAY_SIZE(d_bytes));
            
            BigNum_FromBytes(&n,mod_bytes,ARRAY_SIZE(mod_bytes));
            BigNum_FromU32(&e,65537);
            BigNum_FromBytes(&d,d_bytes,ARRAY_SIZE(d_bytes));            
            BigNum_FromBytes(&a,msg,ARRAY_SIZE(msg));

            BigNum_RsaEncrypt(&b,a,e,n);
            BigNum_RsaDecrypt(&r,b,d,n);

            BigNum_ToBytes(&r_bytes,&r_bytes_len,r);
            TEST(0==strcmp(r_bytes,msg));       
        }
    }
    

    // GCD - need negative BigNum's to do this
    // {
//         BigNum *x = NULL;
//         BigNum *y = NULL;
//         BigNum *gcd = NULL;
        
//         // simple
//         {
//             BigNum_FromU32(&a,7);
//             BigNum_FromU32(&b,255);
//             BigNum_GCD(&x,&y,&gcd);
//             TEST(0==BigNum_EqU32(gcd,1));
//             TEST(0==BigNum_EqU32(a,73))
//         }
        
        
//         // bigger
//         {            
//             char prime1[] = "7514321432143214321";
//             char prime2[] = "1560693731205395696805643173480603251";
//         }
        
//         BigNum_Destroy(&x);
//         BigNum_Destroy(&y);
//         BigNum_Destroy(&gcd);

//     }

    SAFE_FREE(r_bytes);
    SAFE_FREE(dbg_str);
    BigNum_Destroy(&a);
    BigNum_Destroy(&b);
    BigNum_Destroy(&d);
    BigNum_Destroy(&e);
    BigNum_Destroy(&m);
    BigNum_Destroy(&n);
    BigNum_Destroy(&r);
    return num_errors;
}
