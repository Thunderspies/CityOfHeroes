/* sha512ImplImpl.h: Defines the heart and soul of the SHA-512 algorithm.
* 
* @author: Chris Cowden, ccowden@ncsoft.com
*
* created: 8/23/2006
*
* @file
*
* Functions:
*
* Disclaimer: The implementation was derived (and all magic numbers were taken)
* from the document describing the SHA algorithms, which can be found here,
* http://csrc.nist.gov/publications/fips/fips180-2/fips180-2.pdf
*
* More detailed implementation notes can be found at the end of this file (below).
*/

#ifndef _SHA512IMPL_H_
#define _SHA512IMPL_H_

#include "../include/cryptlib/sha.h"

namespace cryptLib
{
    class sha512Impl
    {
    public:

        sha512Impl();
        ~sha512Impl();

        void GetMessageDigest(digest512& hash, std::string& message);

    private:

        // Private Data
        uint64_t K[80];        // constants
        uint8_t w_bits;        // == 64-bits for 512-bit algorithm

        // Types
        struct MessageBlock // a message block of 1024 bits
        {
            uint64_t _[16];
        };
        struct MessageSchedule    // used during computation
        {
            uint64_t _[80];
        };

        // Private Functions
        void _LoadK();
        // Bit-manipulation functions
        inline uint64_t _ROTR(const uint8_t n, const uint64_t x);
        inline uint64_t _SHR(const uint8_t n, const uint64_t x);
        inline uint64_t _Ch(const uint64_t x, const uint64_t y, const uint64_t z);
        inline uint64_t _Maj(const uint64_t x, const uint64_t y, const uint64_t z);
        // Algorithm-specific functions
        // For all magic numbers, please see disclaimer at beginning of document
        uint64_t _UppercaseSigma_0(const uint64_t x);
        uint64_t _UppercaseSigma_1(const uint64_t x);
        uint64_t _LowercaseSigma_0(const uint64_t x);
        uint64_t _LowercaseSigma_1(const uint64_t x);

        inline void _PackMBlock(MessageBlock& m, const std::string& s, uint32_t index);
    };

    // Bit Ops Functions
    inline uint64_t sha512Impl::_ROTR(const uint8_t n, const uint64_t x)
    {
        return ((x >> n) | (x << (w_bits-n)));
    }
    inline uint64_t sha512Impl::_SHR(const uint8_t n, const uint64_t x)
    {
        return (x >> n);
    }
    inline uint64_t sha512Impl::_Ch(const uint64_t x, const uint64_t y, const uint64_t z)
    {
        return ((x & y) ^ ((~x) & z));
    }
    inline uint64_t sha512Impl::_Maj(const uint64_t x, const uint64_t y, const uint64_t z)
    {
        return ((x & y) ^ (x & z) ^ (y & z));
    }
    // Algorithm-specific functions
    // For all magic numbers, please see disclaimer at beginning of document
    inline uint64_t sha512Impl::_UppercaseSigma_0(const uint64_t x)
    {
        return (_ROTR(28,x) ^ _ROTR(34,x) ^ _ROTR(39,x));
    }
    // For all magic numbers, please see disclaimer at beginning of document
    inline uint64_t sha512Impl::_UppercaseSigma_1(const uint64_t x)
    {
        return (_ROTR(14,x) ^ _ROTR(18,x) ^ _ROTR(41,x));
    }
    // For all magic numbers, please see disclaimer at beginning of document
    inline uint64_t sha512Impl::_LowercaseSigma_0(const uint64_t x)
    {
        return (_ROTR(1,x) ^ _ROTR(8,x) ^ _SHR(7,x));
    }
    // For all magic numbers, please see disclaimer at beginning of document
    inline uint64_t sha512Impl::_LowercaseSigma_1(const uint64_t x)
    {
        return (_ROTR(19,x) ^ _ROTR(61,x) ^ _SHR(6,x));
    }

/*** Implementation Notes (Caveats):
    * This was a quick implementation that took about a day and a half of coding. It is incorrect in
    that it is limited to strings of maximum length 2^32 instead of 2^128 as the algorithm states. This
    is mainly due to using std::string as the source string which is limited to 2^32 characters in length.
    * I began implementing this with a mind to make it flexible towards implementing the 256-bit and
    384 bit versions of the SHA-2 family of algorithms, but this would need a thorough code refactoring
    overhaul to ensure that goal. The flexibility begins and ends with the line "#define w_bits 64"
    * Efficiency and speed can now be (re)worked into this code now that correctness has been reasonably
    established (save the first caveat above). One suggestion is to convert the common operations 
    (functions) to macros (i.e., ROTR(),Ch(),Maj(),etc.). Another is to remove the SHR() function since
    I added this simply for functional equivalence to the SHA specification. I'll leave it to the 
    efficiency experts to tweak this.
    * I chose not to do anything at the step "Step 2: Parse the message", since this was equivalent to
    preprocessing the entire message, which could become unwieldy for larger messages. Instead I created
    the function "PackMessageBlock()" to "preprocess" in-place. Other refactoring options should be
    considered, especially when addressing the first caveat above.
    * I judged the correctness of my implementation by performing the tests "Single Block Message" and
    "Multi-Block Message" as listed in PDF referenced above.
    * Ideally, the core functionality of the algorithm should be converted into a library (or
    incorporated into an existing library, such as ncFoundation or arda2).
    * The LoadK() function exists (and other similar anamolies) due to the fact that I don't know how
    (if there exists a way) to create 64-bit constants (integers) in C/C++. I resolved this throughout
    the code by static_cast'ing a 32-bit constant, bit-shifting the 64-bit temporary left 32 bits, then
    adding the 64-bit temporary to the lower 32-bit constant, also 64-bit static_cast'ed. Lame, but it
    gets the job done.
    * This implementation assumes that a "message" is composed of whole bytes. Thus, in the padding step
    we assume that we can pad the whole byte "0xA0", instead of just the bit "1", as the SHA document
    states. This should be changed for maximum compliance, and to allow this to be used in a wider
    range of applications.
    * For correctness testing I also downloaded the SHA library "QuickHash" from 
    http://www.slavasoft.com/quickhash/index.htm and wrote the following win32 console app:

    /////////////
    #include <cstdlib>
    #include <cstdio>
    #include <string>
    #include "QuickHash.h"

    using namespace QuickHash;
    using namespace std;

    int main (int argc, char* argv[])
    {
    if (argc > 1)
    {
    string msg = argv[1];
    for (int i = 2; i < argc; ++i)
    {
    msg += " ";
    msg += argv[i];
    }

    char dest[CSHA512::HEXDIGESTSIZE];
    CSHA512::CalculateHex(dest,msg.c_str());

    printf("digest: %s\n",dest);
    }

    system("pause");
    return 0;
    }
    /////////////

*** 64-bit notes
    printf format specifier: use %I64d for signed integer output and %I64X for hex output.
    data type: use __int64 (WIN32 only) or unsigned long long for ANSI portability (compiler dependent).
    operations:
    AND: &
    OR: |
    XOR: ^
    Bitwise complement: ~
    addition: +
    left-shift: <<
    right-shift: >>   
***/

} // namespace cryptLib

#endif // _SHA512IMPL_H_
