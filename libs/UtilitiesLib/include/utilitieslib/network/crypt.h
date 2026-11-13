#ifndef _CRYPT_H
#define _CRYPT_H

#include "../stdtypes.h"

C_DECLARATIONS_BEGIN

void cryptInit();
void cryptMD5Init();
void cryptMakeKeyPair(U32 *private_key,U32 *public_key);
int cryptMakeSharedSecret(U32 *shared_secret,U32 *my_private_key,U32 *their_public_key);
void cryptMD5Update(U8 *data,int len);
void cryptMD5Final(U32 hash[4]);
void cryptAdler32Init();
void cryptAdler32Update(const U8 *data, int len);
U32 cryptAdler32Final();
U32 cryptAdler32(U8 *data,int len);
U32 cryptAdler32Str(char *str, int len);
void cryptStore(U8 * dst, const U8 * src, size_t len);
void cryptRetrieve(U8 * dst, const U8 * src, size_t len);
typedef void* HMAC_SHA1_Handle;
HMAC_SHA1_Handle cryptHMAC_SHA1Create(const U8* key, int len);
void cryptHMAC_SHA1Update(HMAC_SHA1_Handle hmac, const U8 *data, int len);
void cryptHMAC_SHA1Final(HMAC_SHA1_Handle handle, U32 hash[5]);
void cryptHMAC_SHA1Destroy(HMAC_SHA1_Handle handle);
void cryptSHA512Init();
void cryptSHA512Update(const U8 *data, int len);
void cryptSHA512Final(U64 hash[8]);
// RSA-OAEP-SHA1 using OS-seeded randomness. Buffers must not alias. Input cipherSize is the output capacity.
// Returns nonzero on success; failure clears that capacity and sets cipherSize to zero. Does not throw.
int cryptRSAEncrypt(const U8 *modBuf, U32 modlen, const U8 *expBuf, U32 explen, const void *plain, U32 plainSize, void *cipher, U32 *cipherSize);

C_DECLARATIONS_END

#endif
