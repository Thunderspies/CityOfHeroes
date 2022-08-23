#include "LogPattern.h"

//purpose of this program is to measure the size of a stucture when allocated.
#include <string.h>
#include <queue>
#include "LogTime.h"
#include "LogSearch.h"

#include <iostream>

class PatternImpl
{
protected:
    unsigned long long patternMask[MAX_CHARS];
    char patternString[BITS_LONG];
    void initialize();
    unsigned int length;
    void setPatternMask(const char c, const int i)
    {
        if(i >= BITS_LONG)
            throw "Pattern element is too long";
        patternMask[(unsigned char)c] &= ~(1ULL << i);
    }
    friend class Pattern;
};



void PatternImpl::initialize()
{
    for(int i = 0; i < MAX_CHARS; i++)
    {
        patternMask[i] = ~0;
    }
}

const unsigned long long *Pattern::getPatternMask() { return pimpl_->length? pimpl_->patternMask: NULL;};
const char *Pattern::getPatternString() { return pimpl_->patternString;};
unsigned int Pattern::getLength(){ return pimpl_->length;};
Pattern::Pattern(): pimpl_(new PatternImpl()){pimpl_->length = 0;};

bool Pattern::makePattern(const char *in, bool caseInsensitive, bool wildCards)
{
    if(!in)
        return false;
    pimpl_->length = (unsigned int)strlen(in);
    if(pimpl_->length >= BITS_LONG)
    {
        throw "The Pattern is too long";
    }
    else
    {
        strcpy(pimpl_->patternString, in);
        pimpl_->initialize();
        for(unsigned int i = 0; i < pimpl_->length; i++)
        {
            char c = pimpl_->patternString[i];
            switch(c)
            {
            case '#':
                if(wildCards)
                {
                    for(int j = 0; j < 10; j++)
                        pimpl_->setPatternMask('0' + j, i);
                }
                else
                    pimpl_->setPatternMask(c, i);
                break;
            case '@':
                if(wildCards)
                {
                    for(int j = 'A'; j <= 'Z'; j++)
                    {
                        pimpl_->setPatternMask(j, i);
                        pimpl_->setPatternMask(j+ UPPER_TO_LOWER, i);
                    }
                }
                break;
            default:
                pimpl_->setPatternMask(c,i);
                if(caseInsensitive)
                {
                    if(c >= 'A' && c <= 'Z')
                        pimpl_->setPatternMask(c+UPPER_TO_LOWER, i);
                    else if(c >= 'a' && c <= 'z')
                        pimpl_->setPatternMask(c-UPPER_TO_LOWER, i);
                }
            }
        }
    }
    return true;
}

Pattern::Pattern(const char *in, bool caseInsensitive, bool wildCards): pimpl_(new PatternImpl())
{
    if(!in)
        throw "NULL input to Pattern";
    else
    {
        if(!makePattern(in, caseInsensitive, wildCards))
            throw "Unable to make pattern for Pattern()";
    }
}

Pattern::~Pattern(){delete pimpl_;};



///////date here

class DatePatternImpl
{
protected:
    static const char formats[][20];
    static const unsigned int nFormats;
    static char patterns[][20];        //this is where the Pattern class pattern char is
    static unsigned int pLengths[];    //the lengths of each pattern
    static struct tm locations[];    //where in the date the time element is stored
    static struct tm lengths[];    //how large each element is
    static bool isInitialized;
    unsigned int formatId;

    static void initFormat(const char in[], 
        char *pattern, 
    struct tm &locations, 
    struct tm &lengths, 
        unsigned int &lineLength);
    void init();
    static unsigned int nStrStr(const char *searchMe, 
        const unsigned long long *masks[MAX_CHARS],
        const unsigned int patternLens[], 
        const char * &found,
        unsigned int nCharsToSearch);
    friend class DatePattern;
};


const char DatePatternImpl::formats[][20] = {    "YYMMDD HH:mm:SS",
"MM:DD:YYYY:HH:mm:SS",
"YYYYMMDD HHmmSS",
"YYYY-MM-DD-HH-mm-SS"};

const unsigned int DatePatternImpl::nFormats = sizeof(DatePatternImpl::formats)/sizeof(DatePatternImpl::formats[0]);
char DatePatternImpl::patterns[nFormats][20] = {0};        //this is where the Pattern class pattern char is
unsigned int DatePatternImpl::pLengths[nFormats] = {0};    //the lengths of each pattern
struct tm DatePatternImpl::locations[nFormats] = {0};    //where in the date the time element is stored
struct tm DatePatternImpl::lengths[nFormats] = {0};    //how large each element is
bool DatePatternImpl::isInitialized = false;


void DatePatternImpl::initFormat(const char in[], 
                                 char *pattern, 
struct tm &location, 
struct tm &length,
    unsigned int &lineLength)
{
    char c=0, lastC = 0;
    unsigned int charCount = 1;
    lineLength = (unsigned int)strlen(in);
    for(unsigned int i = 0; i<lineLength; i++)
    {
        c = in[i];
        if(c == lastC)
            charCount++;
        else
        {
            lastC = c;
            charCount = 1;
        }
        switch(c)
        {
        case ':':
        case '.':
        case ';':
        case ',':
        case ' ':
        case '-':
            pattern[i] = c;
            break;
        case 'Y':
            pattern[i] = '#';
            location.tm_year = i-charCount+1;
            length.tm_year = charCount;
            break;
        case 'M':
            pattern[i] = '#';
            location.tm_mon = i-charCount+1;
            length.tm_mon = charCount;
            break;
        case 'D':
            pattern[i] = '#';
            location.tm_mday = i-charCount+1;
            length.tm_mday = charCount;
            break;
        case 'H':
            pattern[i] = '#';
            location.tm_hour = i-charCount+1;
            length.tm_hour = charCount;
            break;
        case 'm':
            pattern[i] = '#';
            location.tm_min = i-charCount+1;
            length.tm_min = charCount;
            break;
        case 'S':
            pattern[i] = '#';
            location.tm_sec = i-charCount+1;
            length.tm_sec = charCount;
            break;
        default:
            throw "unknown character found in DatePattern";
        }
    }
}

DatePattern::DatePattern(): pimpl_(new DatePatternImpl())
{
    pimpl_->formatId = -1;
    if(!pimpl_->isInitialized)
        pimpl_->init();
}

bool DatePattern::initToId(unsigned int fId)
{
    if(fId < pimpl_->nFormats)
    {
        pimpl_->formatId = fId;
        return makePattern(pimpl_->patterns[pimpl_->formatId], false, true);
    }
    return false;
}

DatePattern::DatePattern(unsigned int fId): pimpl_(new DatePatternImpl())
{
    pimpl_->formatId = -1;
    if(!pimpl_->isInitialized)
        pimpl_->init();
    if(!initToId(fId))
        throw "Unable to create DatePattern with provided fId";
}
DatePattern::~DatePattern(){delete pimpl_;};

bool DatePattern::dateToTime(const char date[], log_time &ret, unsigned long long lineLength)
{
    unsigned int year, month, day, hour, minute, second;
    unsigned int formatId = pimpl_->formatId;
    if(formatId < pimpl_->nFormats)
    {
        ret = -1;
        if(lineLength < pimpl_->pLengths[formatId])
        {
            return false;
        }
        month = myAtoi(date+pimpl_->locations[formatId].tm_mon, pimpl_->lengths[formatId].tm_mon);
        day = myAtoi(date+pimpl_->locations[formatId].tm_mday, pimpl_->lengths[formatId].tm_mday);
        year = myAtoi(date+pimpl_->locations[formatId].tm_year, pimpl_->lengths[formatId].tm_year);
        hour = myAtoi(date+pimpl_->locations[formatId].tm_hour, pimpl_->lengths[formatId].tm_hour);
        minute = myAtoi(date+pimpl_->locations[formatId].tm_min, pimpl_->lengths[formatId].tm_min);
        second = myAtoi(date+pimpl_->locations[formatId].tm_sec, pimpl_->lengths[formatId].tm_sec);
        LogTime::makeTime(ret, year, month, day, hour, minute, second);
        return ret != -1;
    }
    else
        throw "dateToTime called on DatePattern with bad format id";
    return false;
}

void DatePatternImpl::init()
{
    if(isInitialized)
        return;
    isInitialized = true;
    for(int i = 0; i < nFormats; i++)
    {
        DatePatternImpl::initFormat(formats[i], patterns[i], locations[i], lengths[i], pLengths[i]);
    }
}

unsigned int DatePattern::myAtoi(const char *in, unsigned int nChars)
{
    unsigned int ret = 0;
    for(unsigned int i = 0; i < nChars; i++)
    {
        ret *= 10;
        ret += in[i] - '0';
    }
    return ret;
}

unsigned int DatePatternImpl::nStrStr(const char *searchMe, 
                                      const unsigned long long *masks[MAX_CHARS],
                                      const unsigned int patternLens[], 
                                      const char * &found,
                                      unsigned int nCharsToSearch)
{
    if(searchMe)
    {
        unsigned long long histories[DatePatternImpl::nFormats];
        unsigned long long completes[DatePatternImpl::nFormats];
        for(unsigned int i = 0; i < DatePatternImpl::nFormats; i++)
        {
            histories[i] = ~1;
            completes[i] = 1ULL << patternLens[i];
        }

        for(unsigned int i = 0; searchMe[i] && i<nCharsToSearch; ++i)
        {
            for(unsigned int j = 0; j < DatePatternImpl::nFormats; j++)
            {
                histories[j] |= masks[j][searchMe[i]];
                histories[j] <<= 1;
                if(!(histories[j]&completes[j]))
                {
                    found = searchMe + i - patternLens[j] + 1;
                    return j;
                }                
            }
        }
    }
    else throw "nStrStr received a NULL parameter";
    return -1;
}

DatePattern *DatePattern::getDatePattern(const char *text, 
                                         const char * &start, 
                                         log_time &firstDate, 
                                         unsigned int stopAfter)
{
    DatePattern *ret;
    unsigned int nFormats = DatePatternImpl::nFormats;
    DatePattern dPats[DatePatternImpl::nFormats];
    const unsigned long long *masks[DatePatternImpl::nFormats];
    unsigned int mLengths[DatePatternImpl::nFormats];
    bool initSuccess = true;
    for(unsigned int i = 0; i < DatePatternImpl::nFormats; i++)
    {
        if(!dPats[i].initToId(i))
            throw "getDatePattern was unable to initialize its date patterns";
        masks[i] = dPats[i].getPatternMask();
        initSuccess &= !!masks[i];
        mLengths[i] = dPats[i].getLength();
    }

    if(initSuccess)
    {
        unsigned int foundId = DatePatternImpl::nStrStr(text,
            masks, mLengths,
            start,
            stopAfter);
        if(foundId < nFormats)
            ret = new DatePattern(foundId);
        else
        {
#ifdef VERBOSE
            printf( "No date found by getDatePattern\n" );
#endif
            return NULL;
        }
        ret->dateToTime(start, firstDate, ret->getLength());
        if(firstDate == -1)
            throw "bad date found in getDatePattern";
        return ret;
    }

    throw "getDatePattern unable to get DatePattern masks";
    return NULL;
}