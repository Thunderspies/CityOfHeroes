#pragma once
#include "LogTime.h"
class PatternImpl;

/**********        Pattern            **********/
class Pattern
{
public:
    /**
    * Prepares a pattern object to efficiently search for the specified pattern
    *
    * This function sets up the search structures for the bitapp algorithm.
    *
    * @param $in
    *   a null terminated C string representing the pattern we will match.
    * @param $caseInsensitive
    *   Should the search ignore case?
    * @param $wildCards
    *   Should the search use # and @ wildcards?
    *
    * @return
    *   "@return" were we able to successfully create the pattern?
    */
    bool makePattern(const char *in, 
        bool caseInsensitive, 
        bool wildCards);
    /**
    * Returns the search data structure for the specified pattern.
    *
    * @return
    *   "@return" a bitmask of positions for every possible character.
    */
    const unsigned long long *getPatternMask();
    /**
    * Returns C string representation of the specified pattern.
    *
    * @return
    *   "@return" a null terminated c string representing the pattern.
    */
    const char *getPatternString();
    /**
    * Returns the number of characters in the pattern.
    *
    * @return
    *   "@return" the number of characters in the pattern.
    */
    unsigned int getLength();
    Pattern(const char *in, 
        bool caseInsensitive, 
        bool wildCards);
    Pattern();
    virtual ~Pattern();

private:
    /**
    * Pattern Class' private implementation.
    */
    PatternImpl *pimpl_;
};

/**********        Dates            **********/
class DatePatternImpl;
/** 
 * Provides a Bitapp employable pattern for recognizing and translating dates
 *
 * The following formats are currently provided:
 * - "YYMMDD HH:mm:SS",
 * - "MM:DD:YYYY:HH:mm:SS",
 * - "YYYYMMDD HHmmSS",
 * - "YYYY-MM-DD-HH-mm-SS"
 *
 * Adding new formats is really as easy as adding a new string to the end of the
 * DatePatternImpl::formats array, though there is currently a size constraint of
 * 20 characters for the whole pattern.
 */

class DatePattern: public Pattern
{
public:
    DatePattern();
    /** 
    * Creates a Bitapp usable Pattern with the specified date pattern index
    *
    * @param $formatId
    *   specifies which of the formats from the DatePatternImpl::formats array
    *   you want to use.
    */
    DatePattern(unsigned int formatId);
    ~DatePattern();
    /** 
    * Initializes the Pattern with the specified date pattern index
    *
    * @param $formatId
    *   specifies which of the formats from the DatePatternImpl::formats array
    *   you want to use.
    */
    bool initToId(unsigned int formatId);
    /** 
    * Converts a given string into a date_time using the specified date format
    *
    * @param $date
    *   C Style character string that you want to translate.
    * @param $time
    *   the interpreted time is returned here.
    * @param $lineLength
    *   the length of the dateline to read.
    *
    * @return
    *   "@return" was initialization successful?
    */
    virtual bool dateToTime(const char date[], 
        log_time &time, 
        unsigned long long lineLength);

    /** 
    * Factory function for creating a date pattern based off of the date provided
    *
    * @param $text
    *   C style string that you believe has a date in it.  
    * @param $start
    *   return pointer to the start of any date found.
    * @param $firstDate
    *   the log_time representation of any date found.
    * @param $stopAfter
    *   How many character to read before we assume there is no date to be found
    *   and give up.
    *
    * @return
    *   "@return" factory created date pattern matching input text.
    */
    static DatePattern *getDatePattern(const char *text, 
        const char * &start, 
        log_time &firstDate, 
        unsigned int stopAfter);
    /** 
    * atoi was too safe and took a huge portion of the used CPU.
    *
    * @param $in
    *   C style string that you believe has an int in it.  
    * @param $nChars
    *   number of characters to read for this integer
    *
    * @return
    *   "@return" the integer found or 0 on error...yeah, so you don't know if
    *   there was an error or it's just zero.  It's not safe, and that's what's
    *   cool about it.
    */
    static unsigned int myAtoi(const char *in, 
        unsigned int nChars);

private:
    DatePatternImpl *pimpl_;
};
