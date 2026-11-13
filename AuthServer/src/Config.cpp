/**
 * Config.cpp
 *
 * Config class definition
 *
 * author: woojeong
 *
 * created: 2002-03-11
 *
**/

#include "GlobalAuth.h"
#include "Config.h"
#include "Parser.h"

Config config;

void* LoadBinary(const char* name, int* size, bool appendNull)
{
    int fd = _open(name, _O_RDONLY|_O_BINARY, 0);
    if (fd < 0) {
        return 0;
    }

    struct _stat sb;
    _stat(name, &sb);
    int bufSize = sb.st_size;
    if (appendNull)
        bufSize++;
    void* pBuf = new char[bufSize];
    if (!pBuf) {
        _close(fd);
        return 0;
    }

    _read(fd, pBuf, sb.st_size);
    _close(fd);

    if (size) {
        *size = sb.st_size;
    }
    if (appendNull)
        ((char *)pBuf)[sb.st_size] = 0;

    return pBuf;
}

Config::Config() : AllowAccountCreation(false) {}
Config::~Config() {}

bool Config::Load(const char *filename)
{
    bool configFileLoaded = true;
    int size;
    char *pBuf = (char*)LoadBinary(filename, &size, false);
    if(pBuf) {
        std::istrstream inp(pBuf, size);
        std::noskipws(inp);
        int token;
        char key[MAX_TOKEN_LEN+1];
        char value[MAX_TOKEN_LEN+1];
        int line = 1;
        enum { eWaitKey, eWaitEqual, eWaitValue, eInComment } state = eWaitKey;
        std::pair<StringMap::iterator, bool> result;
        while ((token = GetToken(inp, value, true)) != eEOF) {
            if (state == eInComment) {
                if (token == eNewLine) {
                    state = eWaitKey;
                }
            } else {
                switch (token) {
                case eWord:
                    switch (state) {
                    case eWaitKey:
                        strcpy(key, value);
                        break;
                    case eWaitValue:
                        result = map.insert(StringMap::value_type(key, value));
                        if (result.second) {
                        } else {
                            logger.AddLog(LOG_ERROR, "Insert twice %s at line %d in %s", key, line, filename);
                        }
                        break;
                    case eInComment:
                        break;
                    default:
                        logger.AddLog(LOG_ERROR, "Invalid state at line %d in %s", line, 
                          filename);
                    }
                    break;
                case eEqual:
                    state = eWaitValue;
                    break;
                case eSemiColon:
                    state = eInComment;
                    break;
                case eNewLine:
                    state = eWaitKey;
                    line++;
                    break;
                default:
                    if (state != eInComment) {
                        logger.AddLog(LOG_ERROR, "Invalid token %s at line %d in %s", value, 
                          line, filename);
                    }
                    break;
                }
            }
        }
        delete pBuf;
    } else {
        logger.AddLog(LOG_ERROR, "Can't load %s", filename);
        configFileLoaded = false;
    }
    
    if ( configFileLoaded )
    {
        serverPort            = GetInt( "serverPort" );
        serverExPort        = GetInt( "serverExPort" );
        serverIntPort        = GetInt( "serverIntPort" );
        worldPort            = GetInt("WorldPort");
        
        numDBConn            = GetInt("DBConnectionNum");
        numServerThread        = GetInt("numServerThread");
        numServerIntThread    = GetInt("numServerIntThread");
                
        gameId                = GetInt("GameID");
        
        SocketTimeOut        = GetInt("SocketTimeOut") * 1000;
        SocketLimit            = GetInt("SocketLimit" );
        AcceptCallNum        = GetInt("AcceptCallNum");
        WaitingUserLimit    = GetInt("WaitingUserLimit");
        

        // Hard-coding this value to allow us to send variable-sized handshake packets
        PacketSizeType        = 3; //GetInt( "PacketSizeType" );        
        
        encrypt   = GetBool("Encrypt");
        AllowAccountCreation = GetBool("AllowAccountCreation", false);


        logDirectory = Get("logDirectory");
        ProtocolVer = GetInt("ProtocolVersion");
        if (ProtocolVer == OUROBOROS_PROTOCOL_VERSION_2) {
            useClientSpecifiedRegions = true;
        }
        else {
            useClientSpecifiedRegions = false;
        }

        DesApply    = GetBool("DesApply", false );
        ReadLocalServerList = GetBool("ReadLocalServerList", false );
        OneTimeLogOut = GetBool("OneTimeLogOut", false );
        GMCheckMode      = GetBool("GMCheckMode", false );
        Country      = GetInt("CountryCode");
        UserData  = GetBool("UserData", false );
        DevIP = Get("DevServerIP");
        DumpPacket = GetBool("DumpPacket", false );
        PCCafeFirst = GetBool( "PCCafeFirst", false );
        
        
        // IPServer
        UseIPServer = GetBool( "UseIPServer", false );
        IPServer = GetInetAddr( "IPServer" );
        IPPort   = GetInt( "IPPort" );
        IPConnectInterval = GetInt( "IPInterval" ) * 1000;


        // LOGDServer
        UseLogD = GetBool("uselogd", false);
        LogDIP  = GetInetAddr( "logdip");
        LogDPort= GetInt( "logdport", 0 );
        LogDReconnectInterval = GetInt( "logdconnectinterval", 60 ) * 1000;

        // GMIP
        RestrictGMIP = GetBool( "RestrictGMIP", false );
        GMIP         = GetInetAddr( "GMIP" );

        // Wanted System
        UseWantedSystem = GetBool( "UseWantedSystem", false );
        WantedIP        = GetInetAddr("WantedIP");
        WantedPort      = GetInt("WantedPort", 2122 );
        WantedReconnectInterval = GetInt("WantedReconnectInterval", 1800 ) * 1000;

        Reactivation = GetInt("Reactivation", 0);
        ReactivationValue = GetInt("ReactivationValue", 1012);
        if (DateTimestringToSystemTime(&ReactivationStart, Get("ReactivationStart")) &&
            DateTimestringToSystemTime(&ReactivationEnd, Get("ReactivationEnd")))
        {
            if (CompareDateTime(&ReactivationStart, &ReactivationEnd) != -1)
            {
                DateTimestringToSystemTime(&ReactivationEnd, Get("ReactivationStart"));
                DateTimestringToSystemTime(&ReactivationStart, Get("ReactivationEnd"));
            }
        }
        else
        {
            ClearDateTime(&ReactivationStart);
            ClearDateTime(&ReactivationEnd);
        }

        IPAccessLimit   = GetInt("IPAccessLimit", 0 );
        FreeServer        = GetBool("FreeServer", false );
        HybridServer    = GetBool("HybridServer", false );

        useForbiddenIPList = GetBool("useForbiddenIPList", false );
        supportReconnect   = GetBool("supportReconnect", false );

        gameServerSpecifiesId = GetBool("gameServerSpecifiesId", false);
        allowUnknownServers = GetBool("allowUnknownServers", false);
        useQueue = GetBool("useQueue", true);
        sendQueueLevel = GetBool("sendQueueLevel", true);
        payStatOverride = GetInt("PayStatOverride", -1);
        
        #ifdef _DEBUG
            #define DEFAULT_VERBOSE_LOGGING true
            #define DEFAULT_DEBUG_LOGGING true
        #else
            #define DEFAULT_VERBOSE_LOGGING false
            #define DEFAULT_DEBUG_LOGGING false
        #endif
        
        enableVerboseLogging = GetBool("enableVerboseLogging", DEFAULT_VERBOSE_LOGGING );
        enableDebugLogging = GetBool("enableDebugLogging", DEFAULT_DEBUG_LOGGING );

        connectionString = Get("connectionString");

        dbHasClientPorts = GetBool("DBHasClientPorts", false);
    }
    
    return configFileLoaded;
}

const char *Config::Get(const char *key) const
{
    StringMap::const_iterator i = map.find(key);
    if (i != map.end()) {
        return i->second.c_str();
    } else {
        return 0;
    }
}

bool Config::GetBool(const char *key, bool def) const
{
    const char *pValue = Get(key);
    if (pValue) {
        return _stricmp(pValue, "yes") == 0 || _stricmp(pValue, "true") == 0 || 
          _stricmp(pValue, "1") == 0;
    } else {
        return def;
    }
}

int Config::GetInt(const char *key, int def) const
{
    const char *pValue = Get(key);
    if (pValue) {
        return atoi(pValue);
    } else {
        return def;
    }
}

in_addr Config::GetInetAddr(const char *key) const
{
    in_addr addr;
    const char *pValue = Get(key);
    if (pValue) {
        addr.s_addr = inet_addr(pValue);
    } else {
        addr.s_addr = 0;
    }
    return addr;
}

int Config::DateTimestringToSystemTime(SYSTEMTIME *time, const char *str)
{
    int year;
    int month;
    int day;
    int hour;
    int minute;

    if (time == NULL)
    {
        return 0;
    }
    ClearDateTime(time);
    if (str == NULL || sscanf(str, "%d/%d/%d %d:%d", &month, &day, &year, &hour, &minute) != 5)
    {
        return 0;
    }

    time->wYear = year;
    time->wMonth = month;
    time->wDay = day;
    time->wHour = hour;
    time->wMinute = minute;

    return 1;
}

void Config::ClearDateTime(SYSTEMTIME *time)
{
    time->wYear = 0;
    time->wMonth = 0;
    time->wDay = 0;
    time->wHour = 0;
    time->wMinute = 0;
}

int Config::CompareDateTime(const SYSTEMTIME *time1, const SYSTEMTIME *time2)
{
    if (time1->wYear != time2->wYear)
    {
        return time1->wYear < time2->wYear ? -1 : 1;
    }
    if (time1->wMonth != time2->wMonth)
    {
        return time1->wMonth < time2->wMonth ? -1 : 1;
    }
    if (time1->wDay != time2->wDay)
    {
        return time1->wDay < time2->wDay ? -1 : 1;
    }
    if (time1->wHour != time2->wHour)
    {
        return time1->wHour < time2->wHour ? -1 : 1;
    }
    if (time1->wMinute != time2->wMinute)
    {
        return time1->wMinute < time2->wMinute ? -1 : 1;
    }
    return 0;
}

int Config::IsValidDateTime(const SYSTEMTIME *time)
{
    // Don't test hout and muinute for zero, since 00:00 is a valid time.
    // I could do further checking, i.e. that all values are in range.
    return time->wYear != 0 && time->wMonth != 0 && time->wDay != 0;
}


int Config::IsReactivationActive()
{
    SYSTEMTIME now;

    if (Reactivation)
    {
        return 1;
    }

    if (!IsValidDateTime(&ReactivationStart) || !IsValidDateTime(&ReactivationEnd))
    {
        return 0;
    }

    GetSystemTime(&now);

    if (CompareDateTime(&ReactivationStart, &now) <= 0 && CompareDateTime(&now, &ReactivationEnd) <= 0)
    {
        return 1;
    }

    return 0;
}
