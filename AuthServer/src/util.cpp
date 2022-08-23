#include "arda2/core/corFirst.h"

#include "util.h"
#include "Config.h"
#include "Log.h"
#include "LogSocket.h"
#include "Blowfish.h"
#include "cryptlib/sha.h"
#include "cryptlib/sha512.h"

using namespace std;

int Assemble(char* buf, int bufLen, const char* format, va_list ap)
{
    const char* f = format;
    char* start = buf;
    char* end = buf + bufLen;
    int t;
    char* p;
    int len;
    int i;
    while ((t = *format++)) {
        switch (t) {
        case 'S': 
            {
                wchar_t* wp = va_arg(ap, wchar_t*);
                if (wp)
                {
                    len = (static_cast<int>(wcslen(wp)) + 1)* sizeof(wchar_t);
                    if (buf + len > end)
                    {
                        goto overflow;
                    }
                    memcpy(buf, wp, len);
                    buf += len;
                }
                else
                {
                    // p is null. Is it correct?
                    if (buf + 2 > end)
                    {
                        goto overflow;
                    }
                    *buf++ = 0;
                    *buf++ = 0;
                }
            }
            break;
        case 's':
            p = va_arg(ap, char*);
            if (p) {
                len = static_cast<int>(strlen(p));
                if (buf + len + 1 > end) {
                    goto overflow;
                }
                strcpy(buf, p);
                buf += len + 1;
            }
            else {
                // p is null. Is it correct?
                if (buf + 1 > end) {
                    goto overflow;
                }
                *buf++ = 0;
            }
            break;
        case 'b':        // blind copy
            len = va_arg(ap, int);
            p = va_arg(ap, char*);
            if (buf + len > end) {
                goto overflow;
            }
            memcpy(buf, p, len);
            buf += len;
            break;
        case 'c':
            i = va_arg(ap, int);
            if (buf + 1 > end) {
                goto overflow;
            }
            *buf++ = i;
            break;
        case 'h':
            i = va_arg(ap, int);
            if (buf + 2 > end) {
                goto overflow;
            }
            *buf++ = i;
            *buf++ = i >> 8;
            break;
        case 'd':
            i = va_arg(ap, int);
            if (buf + 4 > end) {
                goto overflow;
            }
            *buf++ = i;
            *buf++ = i >> 8;
            *buf++ = i >> 16;
            *buf++ = i >> 24;
            break;
        }
    }
    return static_cast<int>(buf - start);
overflow:
    return 0;
}

int Assemble(char *buf, int bufLen, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int len = Assemble(buf, bufLen, format, ap);
    va_end(ap);
    return len;
}

int LNStrNCpy(char *dst, const char *src, int len)
{
    int copyLen = 1;
    while (*src) {
        *dst = *src++;
        copyLen++;
        if (copyLen > len) {
            break;
        }
        dst++;
    }
    *dst = 0;
    return copyLen;
}


#define MAX_PWD_LEN 16
#define ENC_PWD_LEN 128

void EncPwdShalo( char *password )
{
   char passKey[16];

    *((int *)passKey) = *((int *)password)*214013 + 2529077;
    *(((int *)passKey)+1) = *(((int *)password) + 1)*213287 + 2529089;
    *(((int *)passKey)+2) = *(((int *)password) + 2)*212987 + 2529589;
    *(((int *)passKey)+3) = *(((int *)password) + 3)*211741 + 2529997;

    password[0] = password[0] ^ passKey[0];
    for(int i = 1; i < MAX_PWD_LEN ; i++) {
        password[i] = password[i] ^ password[i - 1] ^ passKey[i & 15];
    }
    for(int i = 0; i < MAX_PWD_LEN; i++) {
        if(password[i] == 0) {
            password[i] = 0x66;
        }
    }    
}

void EncPwdL2( char *password )
{
   char passKey[16];

    *((int *)passKey) = *((int *)password)*213119 + 2529077;
    *(((int *)passKey)+1) = *(((int *)password) + 1)*213247 + 2529089;
    *(((int *)passKey)+2) = *(((int *)password) + 2)*213203 + 2529589;
    *(((int *)passKey)+3) = *(((int *)password) + 3)*213821 + 2529997;

    password[0] = password[0] ^ passKey[0];
    for(int i = 1; i < MAX_PWD_LEN ; i++) {
        password[i] = password[i] ^ password[i - 1] ^ passKey[i & 15];
    }
    for(int i = 0; i < MAX_PWD_LEN; i++) {
        if(password[i] == 0) {
            password[i] = 0x66;
        }
    }    
}

void EncPwdDev( char *password )
{
}

void EncPwdSha512( char *password, unsigned int salt )
{
    cryptLib::digest512 hash;
    cryptLib::sha512 SHA;

    // Store password for processing
    string sPassword(password);

    // Create string'ed salt
    char buf[9];
    snprintf(buf,9,"%08x",salt);
    
    // Concat salt to original password
    sPassword += string(buf);

    // Get new hash
    SHA.GetMessageDigest(hash,sPassword);

    string sHash(hash.ToString());

    // Copy out new hash
    for (unsigned int i = 0; i < ENC_PWD_LEN; ++i)
    {
        password[i] = sHash.at(i);
    }
}

bool Decrypt(unsigned char *buf, __int64 &key, int length)
{
    if ( config.encrypt == false )
        return true;

    int i;

    char *strKey = (char *)(&key);
    char source;
    char next_source;

    source = buf[0];
    buf[0] = buf[0] ^ strKey[0];
    for (i = 1; i < length; i++) {
        next_source = buf[i];
        buf[i] = buf[i] ^ source ^ strKey[i & 7];
        source = next_source;
    }

    key+=length;
    return true;
}

void Encrypt(unsigned char *buf, __int64 &key, int &length)
{
    if ( config.encrypt == false )
        return;

    char *strKey = (char *)(&key);

    buf[0] = buf[0] ^ strKey[0];
    for (int i = 1; i < length; i++) {
        buf[i] = buf[i] ^ buf[i - 1] ^ strKey[i & 7];
    }

    key+=length;
}

void WriteAction( char *action, char *account, in_addr ip, int ssn, char gender, int worldid, int stat)
{
    if ( config.UseLogD )
        return;

    time_t ActionTime;
    struct tm ActionTm;

    ActionTime = time(0);
    ActionTm = *localtime(&ActionTime);
    // occurTime, Action, account, server, ip, stat, ssn, gender
    actionlog.AddLog(LOG_NORMAL, "%d-%d-%d %d:%d:%d,%s,%s,%d,%d.%d.%d.%d,%d,%06d,%d\r\n", 
        ActionTm.tm_year + 1900, ActionTm.tm_mon + 1, ActionTm.tm_mday,
        ActionTm.tm_hour, ActionTm.tm_min, ActionTm.tm_sec,        
        action, account, worldid,
        ip.S_un.S_un_b.s_b1,
        ip.S_un.S_un_b.s_b2,
        ip.S_un.S_un_b.s_b3,
        ip.S_un.S_un_b.s_b4, stat,ssn, gender);
}

void StdAccount( char *account )
{
_BEFORE
    int len=0;
    
    account[MAX_ACCOUNT_LEN]=0;
    len = static_cast<int>(strlen(account));
    if (len <= 0 )
        return;

    account[0] = toupper( account[0] );
    
    for( int i=1;i < len; i++ ){
        if ( account[i] == ' ' ){
            account[i]=0;
            return;
        } else
            account[i] = tolower( account[i] );
    }
_AFTER_FIN
}

void DumpPacket( const unsigned char *packet, int packetlen )
{
#ifdef _DEBUG
    if ( config.DumpPacket ){
        char buffer[8192];
        char tmpbuf[4];
        memset( buffer, 0, 8192 );
        for( int i=0; (i < packetlen)&&( i < 1024) ; i++ )
        {
            memset( tmpbuf, 0, 4 );
            sprintf( tmpbuf, "%02x", packet[i] );
            strcat( buffer, tmpbuf );
        }
        buffer[1024]=0;
        logger.AddLog(LOG_WARN, "%s", buffer );
    }
#endif
}
HFONT GetAFont(HDC hDC) 
{ 
    static LOGFONT lf;  // structure for font information  

#ifdef UNICODE
#define GetObject  GetObjectW
#else
#define GetObject  GetObjectA
#endif

    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf); 
 
#undef GetObject

    // Set the font attributes, as appropriate.  
 
    return CreateFont(lf.lfHeight, lf.lfWidth, 
        lf.lfEscapement, lf.lfOrientation, lf.lfWeight, 
        lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet, 
        lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality, 
        lf.lfPitchAndFamily, lf.lfFaceName); 
} 

// param1 depends on LOG_ID
void WriteLogD ( int LOG_ID, char *account, in_addr ip, int stat, int age, int gender, int zipcode, int param1, int uid  )
{
    if ( !( config.gameId & 8) )
        return;

_BEFORE

    SYSTEMTIME tm;
    GetLocalTime( &tm );
    if(strlen(account) >14 )
        account[14]=0;

    wchar_t w_account[16];
    int len = AnsiToUnicode( account, 15, w_account, 15 );
    w_account[14] = 0;
    

    if ((config.UseLogD) && ( !LogDReconnect) && ( g_opFlag != 0)){
        wchar_t msg_packet[1024];
        memset ( msg_packet, 0, 1024 * sizeof( wchar_t ));

        if ( LOG_ID == LOG_ACCOUNT_AUTHED )
            swprintf( msg_packet, LOG_F_ACCOUNT_AUTHED,  tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,LOG_ACCOUNT_AUTHED,uid, 
            ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4 , stat, age, gender, param1, zipcode,w_account );
        else if ( LOG_ID == LOG_ACCOUNT_LOGIN )
            swprintf( msg_packet, LOG_F_ACCOUNT_LOGIN,  tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,LOG_ACCOUNT_LOGIN,uid, ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4, stat, age, gender, param1, zipcode , w_account);
    //  quitgame
        else if ( LOG_ID == LOG_ACCOUNT_LOGOUT ){
            swprintf( msg_packet, LOG_F_ACCOUNT_LOGOUT, tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,LOG_ACCOUNT_LOGOUT, uid, ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4, stat, age, gender, param1, zipcode , w_account);
        }
    //  logout
        else if ( LOG_ID == LOG_ACCOUNT_LOGOUT2 )
        swprintf( msg_packet, LOG_F_ACCOUNT_LOGOUT2, tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,LOG_ACCOUNT_LOGOUT2, uid, 
                ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4, 
                stat, age , gender, w_account);
        gLogLock.ReadLock();
        pLogSocket->Send("cddS", RQ_LOG_SEND_MSG, SERVER_TYPE, AUTH_LOG_TYPE, msg_packet);
        gLogLock.ReadUnlock();
    }

    else{

    char msgpacket[2048];
    memset( msgpacket, 0, 2048);

        if ( LOG_ID == LOG_ACCOUNT_AUTHED )
            sprintf( msgpacket, LOG_F_ACCOUNT_AUTHED_1,  tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,LOG_ACCOUNT_AUTHED,uid, 
            ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4 , stat, age, gender, param1, zipcode, account );
        else if ( LOG_ID == LOG_ACCOUNT_LOGIN )
            sprintf( msgpacket, LOG_F_ACCOUNT_LOGIN_1,  tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,LOG_ACCOUNT_LOGIN,uid, ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4, stat, age, gender, param1, zipcode, account );
    //  quitgame
        else if ( LOG_ID == LOG_ACCOUNT_LOGOUT )
            sprintf( msgpacket, LOG_F_ACCOUNT_LOGOUT_1, tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,LOG_ACCOUNT_LOGOUT, uid, ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4, stat, age, gender, param1, zipcode, account );
    //  logout
        else if ( LOG_ID == LOG_ACCOUNT_LOGOUT2 )
            sprintf( msgpacket, LOG_F_ACCOUNT_LOGOUT2_1, 
                        tm.wMonth, tm.wDay, tm.wYear, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds,
                        LOG_ACCOUNT_LOGOUT2, 
                        uid, 
                        ip.S_un.S_un_b.s_b1,ip.S_un.S_un_b.s_b2,ip.S_un.S_un_b.s_b3,ip.S_un.S_un_b.s_b4, 
                        stat, 
                        age, gender,
                        account );

        msgpacket[1023]=0;
        filelog.AddLog(LOG_NORMAL, "%s", msgpacket );
    }
_AFTER_FIN
}

int AnsiToUnicode(char* ansistr, int maxansilen, WCHAR* unistr, int maxunilen)
{
    int nLenOfWideCharStr= 0;

    nLenOfWideCharStr = MultiByteToWideChar(CP_ACP, 0, ansistr, -1, NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, ansistr, -1, unistr, nLenOfWideCharStr);
    unistr[nLenOfWideCharStr-1] = 0;

    return nLenOfWideCharStr;

}


void BlowFishEncryptPacket(unsigned char *buf, __int64 &key, int &len)
{
    len = (len + 7) & 0xfffffff8;   // Makes it multiple of 8.
// As for the checksum, simple xor is proabably enough...
    int blockLen = len >> 2;
    unsigned int checkSum = 0;
    unsigned int *intPtr = (unsigned int *)buf;
    int i;
    for (i = 0; i < blockLen; ++i) {
        checkSum ^= intPtr[i];
    }
    intPtr[i] = checkSum;
    len += 8;
    BlowfishEncrypt((unsigned char *)buf, len);
}

bool BlowFishDecryptPacket(unsigned char *buf, __int64 &key, int len)
{
    if ((len & 0x7) == 0) {    // It's not a multiple of 8
        BlowfishDecrypt((unsigned char *)buf, len);
        int blockLen = (len - 8) >> 2;
        unsigned int checkSum = 0;
        unsigned int *intPtr = (unsigned int *)buf;
        int i;
        for (i = 0; i < blockLen; ++i) {
            checkSum ^= intPtr[i];
        }
        if (intPtr[i] == checkSum) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

time_t ConvertSQLToTome( SQL_TIMESTAMP_STRUCT &sqlDate, struct tm *tm)
{
    tm->tm_sec = sqlDate.second;
    tm->tm_min = sqlDate.minute;
    tm->tm_hour = sqlDate.hour;
    tm->tm_mday = sqlDate.day;
    tm->tm_mon = sqlDate.month - 1;
    tm->tm_year = sqlDate.year - 1900;
    tm->tm_isdst = 0;

    return mktime(tm);
}

bool CheckAccount( char *account )
{
    int inx=0;
_BEFORE    

    if (account == NULL)
        return false;

    if (!isalpha(account[0]))
        return false;

    for( inx=1; inx <= MAX_ACCOUNT_LEN; inx++ ) {
        if ( account[inx] == NULL ){
            return true;            
        }
        if ( (!isalpha(account[inx])) && (!isdigit(account[inx])) ) {
            return false;
        }
    }

_AFTER_FIN
    return false;
}
