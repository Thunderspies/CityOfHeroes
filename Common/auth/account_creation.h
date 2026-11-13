#ifndef ACCOUNT_CREATION_H
#define ACCOUNT_CREATION_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* These values are encoded as one byte by AuthServer and Game. */
typedef enum AccountCreationResult
{
    ACCOUNT_CREATE_SUCCESS = 0,
    ACCOUNT_CREATE_DISABLED = 1,
    ACCOUNT_CREATE_INVALID_USERNAME = 2,
    ACCOUNT_CREATE_INVALID_PASSWORD = 3,
    ACCOUNT_CREATE_NAME_TAKEN = 4,
    ACCOUNT_CREATE_BLOCKED_IP = 5,
    ACCOUNT_CREATE_INVALID_REQUEST = 6,
    ACCOUNT_CREATE_INVALID_STATE = 7,
    ACCOUNT_CREATE_DATABASE_FAILURE = 8,
    ACCOUNT_CREATE_RESULT_COUNT
} AccountCreationResult;

/* RSA-OAEP-SHA1 plaintext consists of two NUL-terminated, zero-padded fields. */
#define ACCOUNT_CREATE_NAME_BYTES 15
#define ACCOUNT_CREATE_PASSWORD_BYTES 16
#define ACCOUNT_CREATE_PLAIN_BYTES 31

/* Helpers are reentrant and perform no network/database operations.
 * Validate complete C strings without trimming or truncation: names contain
 * 1-14 ASCII letters/digits; passwords contain 8-15 printable ASCII bytes,
 * including spaces. On success, write the lowercase, zero-padded name.
 * Output may alias name and is unchanged on failure. Null pointers return
 * INVALID_REQUEST. Caller owns all buffers. */
AccountCreationResult accountCreationValidate(const char *name, const char *password, char lowercaseName[ACCOUNT_CREATE_NAME_BYTES]);

/* Validate both fixed fields and their zero padding; normalize the name in
 * place only on success. A null block returns INVALID_REQUEST. */
AccountCreationResult accountCreationValidateBlock(unsigned char block[ACCOUNT_CREATE_PLAIN_BYTES]);

/* PHP-compatible SHA512(password + little-endian Adler32(lowercaseName) hex).
 * Inputs must have passed validation and must not alias the 128-byte output.
 * Write exactly 128 lowercase hex bytes (no terminator). Return 1 on success;
 * on crypto/allocation failure, clear the output and return 0. */
int accountCreationHash(const char *lowercaseName, const char *password, unsigned char hash[128]);

/* Clear caller-owned storage with writes that cannot be optimized away.
 * data may be null only when length is zero. */
void accountCreationClear(void *data, size_t length);

/* Validate a length-prefixed RSA block and optional Blowfish padding/checksum
 * trailer after transport decryption/checksum verification. payload excludes
 * the opcode and is read-only. rsaBytes must be 1-512. Return 1 for a valid
 * frame, otherwise 0 (including null payload). */
int accountCreationFrame(const unsigned char *payload, size_t length, size_t rsaBytes, int blowfish);

#ifdef __cplusplus
}
#endif
#endif
