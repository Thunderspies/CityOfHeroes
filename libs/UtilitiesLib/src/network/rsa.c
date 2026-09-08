/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 *
 * Module Description:
 *
 *
 ***************************************************************************/
#include "utilitieslib/network/rsa.h"
#include "utilitieslib/utils/file.h"
#include "utilitieslib/network/bignum.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/assert/assert.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/mathutil.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/structNet.h"
#include "utilitieslib/utils/log.h"

// padding scheme for encryption
typedef enum RsaPad
{
    RsaPad_None,
    RsaPad_PKCS1,
//     RsaPad_OAEP,
    RsaPad_Count,
} RsaPad;

static RsaPad g_rsa_padding = RsaPad_PKCS1;

char const *RsaPad_ToString(RsaPad s)
{
    static const char *strs[] = {
        "None",
        "PKCS",
        "Count",
    };
    return AINRANGE( s, strs ) ? strs[s] : "invalid enum";
}

RsaPad rsaSetPad(RsaPad pad)
{
    RsaPad tmp = g_rsa_padding;
    g_rsa_padding = pad;
    return tmp;
}

//----------------------------------------
// a raw encrypt. 'RSAEP' in docs
//  PKCSl-v1_5 RSA encryption:
// IN:
//  pubexp : byte stream of public exponent in big endian format
//  pubmod : byte stream of public modulus in big endian format
//  msg    : what to encrypt
// OUT:
//  res : malloc'd byte stream
// algo:
// 1. do an EME-PKCSl-v1_5 padding 
//   - gen PS : a string 8 or more digits long of random numbers to fill up
//              to the number of bytes in the modulus.
//   - EM := 0x00 || 0x02 || PS || 0x00 || M
// 2. do the RSA Encryption :
//   - m = OS2IP(EM)            # Octet Stream To Integer Primitive (bignum)
//   - c = m^e mod n            # RSA encrypt m, c : ciphertext
//   - C = I2OSP(c)             # output string
//----------------------------------------
U8 *rsaEncrypt(U8 **res, int *res_len, char *msg, U8 *pubexp, int pubexp_len, U8 *mod, int mod_len)
{
    static U8 default_pubexp[] = { 0x1,0x00,0x01 };
    int pad_len = 0;
    int msg_len;
    int ps_len;
    char *em;
    int em_len;
    U8 *tmp = NULL;
    BigNum *m = NULL;
    BigNum *e = NULL;
    BigNum *n = NULL;
    BigNum *c = NULL;
    U8 *r = NULL;

    if(!pubexp)
    {
        pubexp = default_pubexp;
        pubexp_len = ARRAY_SIZE(default_pubexp);
    }

    if(!msg || !res || !res_len || !msg || !mod 
       || mod_len<0)
        return NULL;    

    msg_len = strlen(msg);

    // pad

    switch(g_rsa_padding)
    {
    case RsaPad_None:
        em = malloc(msg_len);
        pad_len = 0;
        break;
    case RsaPad_PKCS1:
    {
        int i;
        ps_len = mod_len - msg_len - 4; // pad len
        
        if(ps_len < 8)      // rsa specified pad minimum
            return NULL; 
        
        em = malloc(ps_len+3+msg_len);
        em[0] = 0;
        em[1] = 0x02;
        for( i = 0; i < ps_len; ++i ) 
            em[i+2] = rand() % 255 + 1;
        i+= 2;
        em[i++] = 0x00;
        pad_len = i;
    }
    break;
//     case RsaPad_OAEP:
//     {
//         U8 *p;
//         pad_len = mod_len - msg_len - 1;
//         if(pad_len < 40) // min digest length
//         {
//             printf("rsaEncrypt: msg too long to OAEP pad");
//             return NULL;
//         }
        
//         p = em = malloc(pad_len + msg_len + 1);
//         *p++ = 0;
//     }
    break;
    default:
        Errorf("unknown padding scheme %i, exiting", g_rsa_padding);
        return NULL;
    };

    // encrypt, big endian
    memcpy(em+pad_len,msg,msg_len);
    em_len = pad_len + msg_len;
    BigNum_FromBigEndBytes(&m,em,em_len);
    BigNum_FromBigEndBytes(&e,pubexp,pubexp_len);       // big endian
    BigNum_FromBigEndBytes(&n,mod,mod_len);             // big endian

    // encrypt op
    if(!BigNum_RsaEncrypt(&c,m,e,n))
        goto done;

    // no error, get result.
    BigNum_ToBigEndBytes(res,res_len,c);
    r = *res;
done:
    SAFE_FREE(em);
    BigNum_Destroy(&m);
    BigNum_Destroy(&e);
    BigNum_Destroy(&n);
    BigNum_Destroy(&c);
    
    return r;
}

//----------------------------------------
//  convert OpenSSL dump to bytes helper
// e.g. will take the modulus string from this command and 
// make a usable rsa modulus out of it.
// C:\abs\trial_to_account>openssl rsa -text -pubin -in u:\pubkey_short.pem
// Modulus (128 bit):
//     00:d9:2e:8b:c4:1a:b1:b0:25:81:2d:5c:64:9c:c5:
//     a5:dd
// Exponent: 65537 (0x10001)
//----------------------------------------
U8 *rsaOpenSSLToBytes(U8 **res, int *res_len, char *str)
{
    int len;
    char *s;
	U8 *r;
    if(!str || !res)
        return NULL;
    s = str;
    len = strlen(str);
    len = (len+2)/3; // two chars per hex, and remove the ':'
    *res = realloc(*res,len);
    r = *res;
    for(;;)
    {
        int tmp;
        while(*s && !isHexChar(*s))
            s++;
        if(!s[0] || !s[1])
            break;
        if(1 != sscanf(s,"%2x",&tmp))
        {
            LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS, "RSA couldn't scan hex number from %s",s);
            *res = 0;
            return NULL;
        }
        *r++ = tmp;
        s += 2;
    }
    *res_len = r - *res;
    return *res;
}

//----------------------------------------
//  'OpenSSL' means:
// - pubmod : modulus string readable by rsaOpenSSLToBytes
// - pubexp : 65537 (0x10001)
//----------------------------------------
U8 *rsaEncryptOpenSSL(U8 **pres, int *res_len, char *msg, char *modulus)
{
    U8 *mod = NULL;
    int mod_len;
    U8 *res = NULL;
    if(!rsaOpenSSLToBytes(&mod,&mod_len,modulus))
        goto done;
    res = rsaEncrypt(pres,res_len,msg,NULL,0,mod,mod_len);
    if(!res)
        goto done;
done:
    SAFE_FREE(mod);
    return res;
}


char *rsaEncryptToBase64(char *msg, U8 *mod, int mod_len)
{
    static char *b64 = NULL;
    U8 *enc = NULL;
    int enc_len = 0;
    char *res = NULL;
    
    if(!rsaEncrypt(&enc,&enc_len,msg,NULL,0,mod,mod_len))
        goto done;

    res = base64_encode(&b64,enc,enc_len);
done:
    free(enc);
    return res;
}


//----------------------------------------
// Decrypt an encrypted message.
// currently support PKCS1-v1.5 padding
//----------------------------------------
U8 *rsaDecrypt(U8 **res, int *res_len, U8 *ciphertext,int ciphertext_len, U8 *privexp, int privexp_len, U8 *mod, int mod_len )
{
    int i;
    BigNum *r = NULL;
    BigNum *n = NULL;
    BigNum *d = NULL;
    BigNum *c = NULL;
	U8 *end;
    char *fail_msg = NULL;
	U8 *p;
	U8 *tmp = NULL;
    
    if(!res || !res_len || !privexp || privexp_len <= 0 || !mod || mod_len<0 || !ciphertext || ciphertext_len <= 0)
        return NULL;

    BigNum_FromBigEndBytes(&n,mod,mod_len);             // big endian
    BigNum_FromBigEndBytes(&d,privexp,privexp_len);     // big endian
    BigNum_FromBigEndBytes(&c,ciphertext,ciphertext_len);

    BigNum_RsaDecrypt(&r,c,d,n);

    BigNum_ToBigEndBytes(&tmp,&i,r);
    p = tmp;
    end = tmp+i;
//     i = BigNum_NumBytes(r);
//     p = (char*)r->data;
//     end = (char*)(r->data) + i;
    
    // handle padding
    switch(g_rsa_padding)
    {
    case RsaPad_PKCS1:
    {
        if(i < 3) // not even room for the padding note: rsa says 8 chars min, but screw em
        {
            fail_msg = "RsaDecrypt: resulting message not big enough";
            goto done;
        }
        if(*p == 0x00)
            p++;
        if(*p++ != 0x02)
        {
            fail_msg = "RsaDecrypt: second byte not 0x02";
            goto done;
        }
        
        p = strchr(p,0);
        if(!p || p >= end)
        {
            fail_msg = "RsaDecrypt: couldn't find NULL terminator (???)";
            goto done;
        }
        p++;
    }
    break;
    case RsaPad_None:
        break;
    default:
        Errorf("Unknown padding scheme %i",g_rsa_padding);
        break; // may as well keep going
    };
    
    // now we have the message, remove the leading pad
    *res_len = end - p;
    *res = realloc(*res,*res_len+1); // give room to add NULL terminator for caller
    memcpy(*res,p,*res_len);
    (*res)[*res_len] = 0; // NULL for debugging, not in size
done:
    BigNum_Destroy(&r);
    BigNum_Destroy(&n);
    BigNum_Destroy(&d);
    BigNum_Destroy(&c);
    
    if(fail_msg)
    {
        LOG(LOG_ERROR, LOG_LEVEL_VERBOSE, LOG_CONSOLE_ALWAYS,  "RSA failed: %s", fail_msg);
        return NULL;
    }
    else
    {
        return *res;
    }
}

//----------------------------------------
//  Helper for openssl-style hex strings
// 
//----------------------------------------
U8 *rsaDecryptOpenSSL(U8 **pres, int *res_len, U8 *ciphertext,int ciphertext_len, char *private_exponent, char *modulus )
{
    char *res = NULL;
    U8 *mod = NULL;
    int mod_len;
    U8 *privexp = NULL;
    int privexp_len;
    if(!ciphertext)
        return NULL;
    if(!rsaOpenSSLToBytes(&mod,&mod_len,modulus))
        goto done;
    if(!rsaOpenSSLToBytes(&privexp,&privexp_len,private_exponent))
        goto done;
    res = rsaDecrypt(pres,res_len,ciphertext,ciphertext_len,privexp,privexp_len,mod,mod_len);
done:
    SAFE_FREE(mod);
    SAFE_FREE(privexp);
    return res;
}

int rsaTest(void)
{
    RsaPad pad_in = g_rsa_padding;
    RsaPad i;
    int num_errors = 0;
    int num_passed = 0;
    U8 *enc = NULL;
    int enc_len;

    U8 *bytes = NULL;
    int bytes_len;
    U8 *dec = NULL;
    int dec_len;
    U8 eb[] = { 0x1,0x00,0x01 };
    int verbose_tests = 0;
#define TOSTR2(A) #A
#define TOSTR(A) TOSTR2(A)
#define TEST(X) if(!(X)) {                              \
        printf(TOSTR(__LINE__) ": " #X " failed.\n");   \
        ++num_errors;                                   \
    } else {                                            \
       num_passed++;                                    \
       if(verbose_tests)                                \
          printf("passed: " #X "\n");                   \
    }
    
#define BP(A) A,ARRAY_SIZE(A)    
    // complicated, from openssl created keys 
    for(i = RsaPad_None; i < RsaPad_Count; ++i)
    {
// from pubkey:
//             
        char nb[] = {0x00, 0xd9, 0x2e, 0x8b, 0xc4, 0x1a, 0xb1, 0xb0, 
                     0x25, 0x81, 0x2d, 0x5c, 0x64, 0x9c, 0xc5, 0xa5, 
                     0xdd};
        char db[] = { // private exponent
            0x03, 0x71, 0x70, 0x77, 0xd0, 0x70, 0x86, 0xb9, 
            0x5a, 0x03, 0x92, 0xbc, 0x4a, 0x10, 0xc3, 0x25
        };
        char msg[] = "ab";
        printf("rsa test: %s\n",RsaPad_ToString(i));
        rsaSetPad(i);
        TEST(rsaEncrypt(&enc,&enc_len,msg,BP(eb),BP(nb)));
        TEST(rsaDecrypt(&dec,&dec_len,enc,enc_len,BP(db),BP(nb)));        

        TEST(dec && 0 == strcmp(msg,dec));
    }

    // openssl reader test
    {
        // output of openssl rsa -text -pubin -in pubkey.pem
        char key[] = "    00:d0:ee:ce:5e:39:6e:4a:d0:3e:f9:70:47:94:0c: \
    b4:5e:15:ab:bf:38:10:91:4f:ca:8a:76:19:48:bf: \
    96:07:a0:33:a3:b6:b6:99:a3:af:a7:ee:70:52:3c: \
    f1:07:5c:4c:57:49:73:f9:8c:1b:98:11:fe:b2:0f: \
    78:00:39:5a:6c:29:87:88:be:26:ca:c7:3f:10:d6: \
    98:ee:2a:0f:d3:47:35:c2:7a:b9:ed:86:0c:da:92: \
    ac:08:18:9d:74:9b:2e:4c:f9:da:f6:18:28:12:63: \
    14:45:19:18:22:e7:77:e6:f9:c1:66:80:fa:d6:e8: \
    58:47:e2:89:1a:d9:d4:8c:79";

        // test tobytes function 
        {
            U8 res_bytes[] = {
0x00,0xd0,0xee,0xce,0x5e,0x39,0x6e,0x4a,0xd0,0x3e,0xf9,0x70,0x47,0x94,0x0c,
0xb4,0x5e,0x15,0xab,0xbf,0x38,0x10,0x91,0x4f,0xca,0x8a,0x76,0x19,0x48,0xbf,
0x96,0x07,0xa0,0x33,0xa3,0xb6,0xb6,0x99,0xa3,0xaf,0xa7,0xee,0x70,0x52,0x3c,
0xf1,0x07,0x5c,0x4c,0x57,0x49,0x73,0xf9,0x8c,0x1b,0x98,0x11,0xfe,0xb2,0x0f,
0x78,0x00,0x39,0x5a,0x6c,0x29,0x87,0x88,0xbe,0x26,0xca,0xc7,0x3f,0x10,0xd6,
0x98,0xee,0x2a,0x0f,0xd3,0x47,0x35,0xc2,0x7a,0xb9,0xed,0x86,0x0c,0xda,0x92,
0xac,0x08,0x18,0x9d,0x74,0x9b,0x2e,0x4c,0xf9,0xda,0xf6,0x18,0x28,0x12,0x63,
0x14,0x45,0x19,0x18,0x22,0xe7,0x77,0xe6,0xf9,0xc1,0x66,0x80,0xfa,0xd6,0xe8,
0x58,0x47,0xe2,0x89,0x1a,0xd9,0xd4,0x8c,0x79
            };
            
            TEST(rsaOpenSSLToBytes(&bytes,&bytes_len,key));
            TEST(bytes_len == ARRAY_SIZE(res_bytes));
            TEST(0==memcmp(bytes,res_bytes,bytes_len));
        }
    }

    // some OpenSSL real life examples 
    if(0==system("openssl version"))
    {
        char *privkey = "-----BEGIN RSA PRIVATE KEY----- \r\n\
MIICXQIBAAKBgQDbfz6OtUuW8K5dU0Irr/D/jj9Q+sCKLJ8ZtvoUu5ekd0LvZkme \r\n\
f0pDi107qr81FOhPzcIG+HyBF9+EUK8d/nJnxUpexYFDHtG/TvK2if4oDiflzf7S \r\n\
XzJto+aYtlIHK6Icf4OvSUsdCi71Bqcu6A6hvtveX1wCG0CvvLhceAPf+wIDAQAB \r\n\
AoGBAMD88e3brSh7WXOovqdWvJiVY0o6Dovui7y6Sstr3Pq3+VwwHU6EMLGOmVza \r\n\
1d9AELoJ+SzT0fRXHylhH3dJvx+pVbbFYjmxSGIfs8rjaGV6VKa9piS/na/Kjtk6 \r\n\
gsIp42RTPh7CAy6AAZECJAltO4qeW3jJ0+PCyrYL1CXUVRvRAkEA7yFWq68fOD4i \r\n\
kC90KWmTHsYkVE98wDsrvqjlihdNXRMnD5FaAHaxzhsytFp+atcytP7XjTJedAp9 \r\n\
yT2zxP6c3wJBAOr7VUQ6Fjyq90v/gBTqKbXo71dGvdTD/aCLXnPR5EkZq1RWX/tm \r\n\
3gn0LO5TAAI4HlvSomi5b2B0U+Hk1VHzhGUCQFeeQpCDiQ/ljGqCSLDH0zUqarNN \r\n\
sKsKwzuHzRss8JbS5rQIkQ6sbvfS9WAp7DofgZ/Z5IcC1qL0GSS8a/sZQ8cCQQCh \r\n\
hbw51tfcQgUVd36aYc/kHEcRLi5k54ga6FI3uOp8GSn9IhZ+IFq2auLLu8AAxoSP \r\n\
x70d0YGuwqe6WmsMFyFFAkB3UwWY61cnmMbrm4BUhT8xrXcPtPIUdtP0iXedupQO \r\n\
0a5kAdk4CG50KDAmh9sB0csBBbg7bE9zsQ/iY7otTBl8 \r\n\
-----END RSA PRIVATE KEY-----  \r\n";

        char *modulus = "00:db:7f:3e:8e:b5:4b:96:f0:ae:5d:53:42:2b:af:\
f0:ff:8e:3f:50:fa:c0:8a:2c:9f:19:b6:fa:14:bb:                         \
97:a4:77:42:ef:66:49:9e:7f:4a:43:8b:5d:3b:aa:                         \
bf:35:14:e8:4f:cd:c2:06:f8:7c:81:17:df:84:50:                         \
af:1d:fe:72:67:c5:4a:5e:c5:81:43:1e:d1:bf:4e:                         \
f2:b6:89:fe:28:0e:27:e5:cd:fe:d2:5f:32:6d:a3:                         \
e6:98:b6:52:07:2b:a2:1c:7f:83:af:49:4b:1d:0a:                         \
2e:f5:06:a7:2e:e8:0e:a1:be:db:de:5f:5c:02:1b:                         \
40:af:bc:b8:5c:78:03:df:fb";
        char *privexp = "    00:c0:fc:f1:ed:db:ad:28:7b:59:73:a8:be:a7:56:\
    bc:98:95:63:4a:3a:0e:8b:ee:8b:bc:ba:4a:cb:6b:                       \
    dc:fa:b7:f9:5c:30:1d:4e:84:30:b1:8e:99:5c:da:                       \
    d5:df:40:10:ba:09:f9:2c:d3:d1:f4:57:1f:29:61:                       \
    1f:77:49:bf:1f:a9:55:b6:c5:62:39:b1:48:62:1f:                       \
    b3:ca:e3:68:65:7a:54:a6:bd:a6:24:bf:9d:af:ca:                       \
    8e:d9:3a:82:c2:29:e3:64:53:3e:1e:c2:03:2e:80:                       \
    01:91:02:24:09:6d:3b:8a:9e:5b:78:c9:d3:e3:c2:                       \
    ca:b6:0b:d4:25:d4:55:1b:d1";
        char msg_raw[] = "hello_________________________________________________________________________________________________________________________\r\n";
        // write private key
        {
            FILE *fp = fopen("c:/temp/privtest.pem","wb");
            fprintf(fp,privkey);
            fclose(fp);
        }
        
        // hello, no pad - needs 128 chars to match modulus len
        rsaSetPad(RsaPad_None);
        {
            U8 res[] = {
0x3f, 0xbf, 0x03, 0xb4, 0x5d, 0xc0, 0x37, 0xf2, 0x4e, 0xf8, 0x2c, 0xd8, 0x2f, 0x73, 0xad, 0x75, 0xd5, 0x6a, 0x05, 0x28, 0xf0, 0x28, 0x6a, 0x07, 0xf8, 0xe0, 0x6e, 0x6c, 0x12, 0x09, 0xb6, 0x73, 
0x8b, 0x92, 0x69, 0xc7, 0xfa, 0x7c, 0x46, 0x5a, 0x12, 0xb9, 0xe6, 0x9a, 0x74, 0xfc, 0x5f, 0xbf
, 0xed, 0xed, 0x79, 0x71, 0xa2, 0xfb, 0x74, 0x1c, 0xa3, 0xbe, 0x79, 0x16, 0x51, 0x15, 0xd9, 0xd7
, 0x33, 0xb5, 0x4f, 0x17, 0xa8, 0xd4, 0x0a, 0x1a, 0x05, 0x67, 0x53, 0x7c, 0x1f, 0xcd, 0x1d, 0x06
, 0x27, 0xc7, 0x4b, 0x2f, 0xcc, 0xc7, 0xb1, 0xc1, 0x3f, 0xb1, 0xa5, 0xf0, 0xb2, 0x91, 0x00, 0xe2
, 0x30, 0x06, 0x3c, 0xdc, 0x07, 0x6e, 0xc8, 0x70, 0x86, 0xd0, 0x1d, 0xd8, 0x29, 0x4b, 0xd4, 0x0c
, 0x90, 0x40, 0x16, 0x7b, 0x66, 0x81, 0x07, 0x89, 0x4d, 0x1a, 0xaa, 0x5e, 0x69, 0x6c, 0x66, 0x60
            };


            TEST(rsaEncryptOpenSSL(&enc,&enc_len,msg_raw,modulus));
            TEST(enc_len == ARRAY_SIZE(res));
            TEST(0==memcmp(enc,res,enc_len));
            TEST(rsaDecryptOpenSSL(&dec,&dec_len,enc,enc_len,privexp,modulus));
            TEST(0 == strncmp(dec,msg_raw,ARRAY_SIZE(msg_raw)-1));
        }

        // encrypt something and have openssl decrypt it?
        rsaSetPad(RsaPad_None);
        {
            char *filename_in = "c:/temp/rsa_encrypted_raw.txt";
            char *filename_out = "c:/temp/rsa.txt";
            FILE *fp = fopen(filename_in,"wb"); 
            char *tmp;
            TEST(rsaEncryptOpenSSL(&enc,&enc_len,msg_raw,modulus));
            fwrite(enc,1,enc_len,fp);
            fclose(fp);
            TEST(0==systemf("openssl.exe rsautl -in %s -out %s -inkey c:/temp/privtest.pem -decrypt -raw", filename_in, filename_out));
            tmp = fileAlloc(filename_out,NULL);
            TEST(tmp && 0==strcmp(tmp,msg_raw)); 
            SAFE_FREE(tmp);
        }

        rsaSetPad(RsaPad_PKCS1);
        {
            char msg_pkcs1[] = "hello world!\r\n";
            char *filename_in = "c:/temp/rsa_encrypted.txt";
            char *filename_out = "c:/temp/rsa.txt";
            FILE *fp = fopen(filename_in,"wb"); 
            char *tmp;
            TEST(rsaEncryptOpenSSL(&enc,&enc_len,msg_pkcs1,modulus));
            fwrite(enc,1,enc_len,fp);
            fclose(fp);
            TEST(0 == systemf("openssl.exe rsautl -in %s -out %s -inkey c:/temp/privtest.pem -decrypt", filename_in, filename_out));
            tmp = fileAlloc(filename_out,NULL);
            TEST(tmp && 0==strcmp(tmp,msg_pkcs1)); 
            SAFE_FREE(tmp);
        }   
    }
    else
    {
        printf("openssl not installed. skipping openssl compatability.\n");
    }
    
    // for trial to account
    {
        char *modulus = "    00:d0:ee:ce:5e:39:6e:4a:d0:3e:f9:70:47:94:0c: \
    b4:5e:15:ab:bf:38:10:91:4f:ca:8a:76:19:48:bf: \
    96:07:a0:33:a3:b6:b6:99:a3:af:a7:ee:70:52:3c: \
    f1:07:5c:4c:57:49:73:f9:8c:1b:98:11:fe:b2:0f: \
    78:00:39:5a:6c:29:87:88:be:26:ca:c7:3f:10:d6: \
    98:ee:2a:0f:d3:47:35:c2:7a:b9:ed:86:0c:da:92: \
    ac:08:18:9d:74:9b:2e:4c:f9:da:f6:18:28:12:63: \
    14:45:19:18:22:e7:77:e6:f9:c1:66:80:fa:d6:e8: \
    58:47:e2:89:1a:d9:d4:8c:79";
        char *msg = "hello world!\r\n";
        char *tmp64 = NULL;
        FILE *fp;
        TEST(rsaEncryptOpenSSL(&enc,&enc_len,msg,modulus));
        fp = fopen("c:/abs/trial_to_account/msg_encrypted.txt","wb");
        base64_encode(&tmp64,enc,enc_len);
        //fwrite(enc,1,enc_len,fp);
        fprintf(fp,tmp64);
        fclose(fp);
    }
    

    g_rsa_padding = pad_in;
    SAFE_FREE(enc);
    SAFE_FREE(dec);
    return num_errors;
}

