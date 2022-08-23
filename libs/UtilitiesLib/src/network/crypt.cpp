#undef sprintf
#undef toupper
#undef fopen

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "cryptlib/cryptlib.h"
#include "cryptlib/rng.h"
#include "cryptlib/dh.h"
#include "cryptlib/hmac.h"
#include "cryptlib/md5.h"
#include "cryptlib/adler32.h"
#include "cryptlib/sha.h"
#include "cryptlib/rsa.h"
#include "cryptlib/filters.h"
#include "cryptlib/files.h"
#include "cryptlib/modes.h"
#include "cryptlib/hex.h"
#include "cryptlib/aes.h"
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/network/crypt.h"
#include "utilitieslib/utils/mathutil.h"

using namespace CryptoPP;
using namespace CryptoPP::Weak1;

C_DECLARATIONS_BEGIN

#include "utilitieslib/utils/endian.h"

THREADSAFE_STATIC LC_RNG *rnd_gen = NULL;
THREADSAFE_STATIC DH *dh = NULL;
THREADSAFE_STATIC MD5 *md5 = NULL;
THREADSAFE_STATIC Adler32 *adler32 = NULL;
THREADSAFE_STATIC RijndaelEncryption *crypt_store = NULL;
THREADSAFE_STATIC RijndaelDecryption *crypt_retrieve = NULL;
THREADSAFE_STATIC SHA512 *sha512 = NULL;

static byte crypt_iv[Rijndael::BLOCKSIZE];

// P=143957385117416095198798092915178010143, G=2
static const byte dh_hex_params[] = "3046024100889C7197E0B88D04A2CF46FD125FF29CB80F1FC427107B88F5731A960944B55949DFF8ACF6B835DA411129A4922E3F5C6C4D2F12310C5AFF3EF8CA401FA8F61F020102";
static const byte dh_hex_precomputation[] =
    "30820429020101020201000240662AAB385DD658F217922B08C8E02829D7D2A0B38ACE8D651FA6B03DE431DFF4226015F91BD75E713CCC8312497541EABB1872C96CDAEF024315A13FA1051DA3024024"
    "C4E4065E3CD40F564363DE3C64FC421438100A371CDB846878272E5B9A12653E41B79A3B3097A0C81F2A93D9EB7C6624824EE2A528F42338B2302786B34B5802403D5713694413755B4F82C7E7E54240"
    "A8E057C184560A81E524C30B985949FF6BAEEF70EAC9DB4647EB1264E27DE79635EF053EF82FB9C9FB1AC1173F3183D381024019B94BB70B1FA18D418E004AADFD1E1F9F38F50DA55F46FF54184AD693"
    "1DBE8924E8F6BFD53EC3EABF1A373D827C6912711A85A25CDC1B9A17E8A7FD167E977F024029864CDC5A9AE82D6297069F4736447481DA4B4F1284665EBBBA203C642454999DD7967DBB5AC09C8470B0"
    "0D31D23C212674E009314D5501B41011A58CB5A35D0240633E2A5E02F48C834C8238A1A34B2AE6765108F663C15D4CE1487E28113CA2620C3A3146BF71CE986BF9D6E39F83BE989974ECD428672B54A0"
    "BBDF8D71F87763024037FE969553D38D4CC670729C2EFBE753CD869492EFE818238ED042475E7A13736C08477E14E36A9B13578E55AEA6FF8C41145FB1BB37BCB5637ED65753DEFB7502404634545BE9"
    "0D92982D75651685694E5D2BFA493632C9C13FD791843E377150F2C8F6F4915E2348449A923BFF7258666BE268497AF4760E50D26960D0FE60DDD50240501D79C0B8148388A43739C5849E658BB1E110"
    "DF3B41377513D9FCF07C8FD909AB5B06DEACA39114BCBF7E1A1568B641FF20F74B2648835BED2181B43F89F2B802404EB220B3FC735882C3F595359F6CF25F0514DAA605959FB5592861CA496B535DB1"
    "3AB2703D03AE8073F29C96F5155EC6634BEA7F867FD363D034D1213C40BFAB02406D28FC89890C1B759F8587C22CCA4B1BD48FCC07D6E675EF8CF384123F5E474C91FEA753A98727708E0FF69C020DB1"
    "9D4F7C343C6DD95CE4F99747FD5FD1B321024027FAEF5C4118E06229C0D0990674FDA940FD635D0F1A720E5E9DFC33EFF94B37EFDC0493C26EB5E8E68F7BD50B6A3252F55EC2AA0C7CAE273FCC1B7F5E"
    "47C99A02400BEFA03E8946A44BDF132085DC73A211F4216FA69E28C6BA1204CB0F18FA03997614614FBF1683736A5CE676950350DB6F67158A45B6A17B6BE421F1E8BFF3F102410085DFAAAD6FDF9E4D"
    "9F5A499EE0432998E1426A259EB263F4DF4837DE11BC33554AF5EA9CDAE4544750C3379BF959DE708250BBABCCB400E730C7A7964FE5C83902403A8F973428A8F9974494EA33AADF50993B7A77E1057A"
    "E673039F111806DCD3884D10AA68F98028188774216DD9C6C513E00420BA9D5760AAC6A9A8CB35F782E20241008012B503338A7D80585E74FFF9937FC4D84AF2918DF72927F6C79CECC04443D80CF0CF"
    "F4DF0C8E2E42E0A26506841DA4E2B0B9AA33D9F33A06F1899157538295";

void cryptInit()
{
    S64        seed64;
    U32        seed;

    if (rnd_gen) return;  // let cryptInit be called multiple times

    GET_CPU_TICKS_64(seed64);
    seed = (U32)seed64;
    rnd_gen = new LC_RNG(seed);

    cryptMD5Init();
    cryptAdler32Init();
    cryptSHA512Init();

#if 0
    LC_RNG fixed_rnd_gen(0x2f8306af);
    dh = new DH(fixed_rnd_gen, 512);

    std::cout << std::endl << "HEX_PARAMS:" << std::endl;
    HexEncoder hex_params(new FileSink(std::cout));
    dh->DEREncode(hex_params);
    std::cout << std::endl;

    dh->AccessMaterial().Precompute();

    std::cout << std::endl << "HEX_PRECOMP:" << std::endl;
    HexEncoder hex_precomp(new FileSink(std::cout));
    dh->AccessMaterial().SavePrecomputation(hex_precomp);
    std::cout << std::endl;
#else
    HexDecoder hex_params;
    hex_params.PutMessageEnd(dh_hex_params, sizeof(dh_hex_params)-1);
    dh = new DH(hex_params);

    HexDecoder hex_precomp;
    hex_precomp.PutMessageEnd(dh_hex_precomputation, sizeof(dh_hex_precomputation)-1);
    dh->AccessMaterial().LoadPrecomputation(hex_precomp);
#endif

    // Setup the client-side slightly encrypted storage
    SecByteBlock key(Rijndael::DEFAULT_KEYLENGTH);

    rnd_gen->GenerateBlock(key, key.size());
    rnd_gen->GenerateBlock(crypt_iv, sizeof(crypt_iv));

    crypt_store =  new RijndaelEncryption(key, key.size());
    crypt_retrieve = new RijndaelDecryption(key, key.size());

}

void cryptMD5Init()
{
    if (!md5)
        md5 = new MD5;
}


void cryptMakeKeyPair(U32 *private_key,U32 *public_key)
{
    dh->GenerateKeyPair(*rnd_gen,(U8 *)private_key,(U8 *)public_key);
}

int cryptMakeSharedSecret(U32 *shared_secret,U32 *my_private_key,U32 *their_public_key)
{
    return dh->Agree((U8 *)shared_secret, (U8 *)my_private_key, (U8 *)their_public_key, true);
}

void cryptMD5Update(U8 *data,int len)
{
    md5->Update((byte *)data,len);
}

void cryptMD5Final(U32 hash[4])
{
    // cryptopp now does also transform so no need for endian swap
    md5->Final((byte*)hash);
}

void cryptAdler32Init()
{
    if (!adler32) {
        adler32 = new Adler32;
    }
    adler32->Reset();
}

void cryptAdler32Update(const U8 *data, int len)
{
    adler32->Update((byte *)data,len);
}

U32 cryptAdler32Final()
{
    U32    hash;
    adler32->Final((byte *)&hash);
    hash = endianSwapIfBig(U32, hash);
    return hash;
}

U32 cryptAdler32(U8 *data,int len)
{
    U32    hash;

    adler32->Update((byte *)data,len);
    adler32->Final((byte *)&hash);
    //printf("In cryptAdler32\n");
    hash = endianSwapIfBig(U32, hash);
    return hash;
}

U32 cryptAdler32Str(char *str, int len)
{
    if(!len)
        len = (int)strlen(str);
    
    cryptAdler32Init();
    cryptAdler32Update((U8*)str,len);
    return  cryptAdler32Final();
    STATIC_INFUNC_ASSERT(sizeof(char) == sizeof(U8));
}

void cryptStore(U8 * dst, const U8 * src, size_t len)
{
    CFB_Mode_ExternalCipher::Encryption enc(*crypt_store, crypt_iv);
    enc.ProcessString((byte*)dst, (const byte*)src, len); 
}

void cryptRetrieve(U8 * dst, const U8 * src, size_t len)
{
    // CFB uses the Encryptor for decryption
    CFB_Mode_ExternalCipher::Decryption dec(*crypt_store, crypt_iv);
    dec.ProcessString((byte*)dst, (const byte*)src, len);
}

void cryptSHA512Init()
{
    if (!sha512)
        sha512 = new SHA512;
}

void cryptSHA512Update(const U8 *data, int len)
{
    sha512->Update((byte *)data,len);
}

void cryptSHA512Final(U64 hash[8])
{
    // cryptopp now does also transform so no need for endian swap
    sha512->Final((byte*)hash);
    if (!isBigEndian())
    {
        for (int i = 0; i < 8; i++)
            hash[i] = endianSwapU64(hash[i]);
    }
}

HMAC_SHA1_Handle cryptHMAC_SHA1Create( const U8* key, int len )
{
    HMAC<SHA1>* hmac = new HMAC<SHA1>( key, len );
    return hmac;
}

void cryptHMAC_SHA1Update(HMAC_SHA1_Handle hmac, const U8 *data, int len)
{
    HMAC<SHA1>* hmac_sha1 = reinterpret_cast<HMAC<SHA1>*>(hmac);
    hmac_sha1->Update((byte *)data,len);
}

void cryptHMAC_SHA1Final(HMAC_SHA1_Handle hmac, U32 hash[5])
{
    HMAC<SHA1>* hmac_sha1 = reinterpret_cast<HMAC<SHA1>*>(hmac);
    hmac_sha1->Final((byte *)hash);
}

void cryptHMAC_SHA1Destroy(HMAC_SHA1_Handle hmac)
{
    HMAC<SHA1>* hmac_sha1 = reinterpret_cast<HMAC<SHA1>*>(hmac);
    delete hmac_sha1;
}

void cryptRSAEncrypt(const U8 *modBuf, U32 modlen, const U8 *expBuf, U32 explen, U8 *plain, U32 plainSize, U8 *cipher, U32 *cipherSize)
{
    RSAFunction publicKey;
    Integer mod(modBuf, modlen);
    Integer exp(expBuf, explen);
    
    publicKey.Initialize(mod, exp);

    RSAES_OAEP_SHA_Encryptor encryptor( publicKey );

    U32 plainBlockSize = encryptor.FixedMaxPlaintextLength();
    U32 cipherBlockSize = encryptor.FixedCiphertextLength();

    assert( (plainSize + plainBlockSize-1) / plainBlockSize <= (*cipherSize/cipherBlockSize));

    U32 plainNext = 0;
    U32 cipherNext = 0;
    for (; plainNext<plainSize; plainNext += plainBlockSize, cipherNext += cipherBlockSize)
        encryptor.Encrypt(*rnd_gen, plain+plainNext, MIN(plainSize-plainNext, plainBlockSize), cipher+cipherNext);        

    *cipherSize = cipherNext;
}

C_DECLARATIONS_END
