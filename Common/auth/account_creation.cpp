#include "account_creation.h"
#include <secblock.h>
#include <sha.h>
#include <string.h>

void accountCreationClear(void *data, size_t length)
{
    volatile unsigned char *p = (volatile unsigned char *)data;
    while (length--)
        *p++ = 0;
}

AccountCreationResult accountCreationValidate(const char *name, const char *password, char lowercaseName[ACCOUNT_CREATE_NAME_BYTES])
{
    size_t nameLength = 0;
    size_t passwordLength = 0;
    if (!name || !password || !lowercaseName)
        return ACCOUNT_CREATE_INVALID_REQUEST;

    while (nameLength < ACCOUNT_CREATE_NAME_BYTES && name[nameLength])
    {
        const unsigned char c = (unsigned char)name[nameLength++];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')))
            return ACCOUNT_CREATE_INVALID_USERNAME;
    }
    if (!nameLength || nameLength == ACCOUNT_CREATE_NAME_BYTES)
        return ACCOUNT_CREATE_INVALID_USERNAME;

    while (passwordLength < ACCOUNT_CREATE_PASSWORD_BYTES && password[passwordLength])
    {
        const unsigned char c = (unsigned char)password[passwordLength++];
        if (c < 32 || c > 126)
            return ACCOUNT_CREATE_INVALID_PASSWORD;
    }
    if (passwordLength < 8 || passwordLength == ACCOUNT_CREATE_PASSWORD_BYTES)
        return ACCOUNT_CREATE_INVALID_PASSWORD;

    for (size_t i = 0; i < nameLength; ++i)
    {
        const unsigned char c = (unsigned char)name[i];
        lowercaseName[i] = (char)(c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c);
    }
    memset(lowercaseName + nameLength, 0, ACCOUNT_CREATE_NAME_BYTES - nameLength);
    return ACCOUNT_CREATE_SUCCESS;
}

AccountCreationResult accountCreationValidateBlock(unsigned char block[ACCOUNT_CREATE_PLAIN_BYTES])
{
    if (!block)
        return ACCOUNT_CREATE_INVALID_REQUEST;

    int ended = 0;
    for (size_t i = 0; i < ACCOUNT_CREATE_PLAIN_BYTES; ++i)
    {
        if (i == ACCOUNT_CREATE_NAME_BYTES)
        {
            if (!ended)
                return ACCOUNT_CREATE_INVALID_USERNAME;
            ended = 0;
        }
        if (!block[i])
            ended = 1;
        else if (ended)
            return ACCOUNT_CREATE_INVALID_REQUEST;
    }
    if (!ended)
        return ACCOUNT_CREATE_INVALID_PASSWORD;
    return accountCreationValidate((char *)block, (char *)block + ACCOUNT_CREATE_NAME_BYTES, (char *)block);
}

int accountCreationHash(const char *lowercaseName, const char *password, unsigned char hash[128])
{
    static const char hex[] = "0123456789abcdef";
    memset(hash, 0, 128);
    try
    {
        unsigned int a = 1;
        unsigned int b = 0;
        const size_t passwordLength = strlen(password);
        // Secure buffers also clear plaintext and digest when crypto throws.
        CryptoPP::SecByteBlock input(ACCOUNT_CREATE_PASSWORD_BYTES - 1 + 8);
        CryptoPP::SecByteBlock digest(CryptoPP::SHA512::DIGESTSIZE);
        for (size_t i = 0; lowercaseName[i]; ++i)
        {
            a = (a + (unsigned char)lowercaseName[i]) % 65521;
            b = (b + a) % 65521;
        }
        const unsigned int adler = (b << 16) | a;
        memcpy(input.data(), password, passwordLength);
        for (size_t i = 0; i < 4; ++i)
        {
            const unsigned int value = (adler >> (8 * i)) & 255;
            input[passwordLength + 2 * i] = hex[value >> 4];
            input[passwordLength + 2 * i + 1] = hex[value & 15];
        }
        CryptoPP::SHA512 sha512;
        sha512.CalculateDigest(digest.data(), input.data(), passwordLength + 8);
        for (size_t i = 0; i < digest.size(); ++i)
        {
            hash[2 * i] = hex[digest[i] >> 4];
            hash[2 * i + 1] = hex[digest[i] & 15];
        }
        return 1;
    }
    catch (...)
    {
        accountCreationClear(hash, 128);
        return 0;
    }
}

int accountCreationFrame(const unsigned char *payload, size_t length, size_t rsaBytes, int blowfish)
{
    if (!payload || length < 4 || rsaBytes == 0 || rsaBytes > 512)
        return 0;
    const size_t plainLength = 4 + rsaBytes;
    const size_t paddedLength = ((plainLength + 1 + 7) & ~(size_t)7) - 1;
    const unsigned int size = payload[0] | ((unsigned int)payload[1] << 8) | ((unsigned int)payload[2] << 16) | ((unsigned int)payload[3] << 24);
    if (size != rsaBytes || length != (blowfish ? paddedLength + 8 : plainLength))
        return 0;
    if (blowfish)
    {
        for (size_t i = plainLength; i < paddedLength; ++i)
            if (payload[i])
                return 0;
        /* Four checksum bytes, followed by four reserved zero bytes. */
        for (size_t i = paddedLength + 4; i < length; ++i)
            if (payload[i])
                return 0;
    }
    return 1;
}
