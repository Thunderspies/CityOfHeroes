#include "LogSearch.h"
#include <string>
#include <list>
#include "LogPattern.h"
#include "LogTime.h"
#include "LogPattern.h"
extern "C"
{
#include <utilitieslib/utils/wininclude.h>
#include <utilitieslib/components/EString.h>
}
/**********        Search            **********/
class SearchImpl
{
protected:
    struct LogLine
    {
        std::string line;
        log_time time;
    };
    static bool compare_logline (LogLine * first, LogLine * second);
    CRITICAL_SECTION foundLinesCS;
    bool ignoreDates;
    log_time start;
    log_time end;
    log_time abortOffset;
    Pattern p;
    std::list<LogLine *> foundLines;
    void addLog(std::string s, log_time t)
    {
        LogLine *line = new LogLine();
        if(line)
        {
            line->line = s;
            line->time = t;
            EnterCriticalSection(&foundLinesCS);
            foundLines.push_back(line);
            LeaveCriticalSection(&foundLinesCS);
        }
        else
            throw "Ran out of memory while creating a LogLine";
    };
    void makeLine(const char *position, const char *front, const char * &start, unsigned int &length);
    bool shouldAbort(log_time curTime);

    static bool myStrStr(const char *searchMe, 
        const unsigned long long patternMask[MAX_CHARS], 
        const unsigned int patternLen,
        const unsigned long long lineStartMask[MAX_CHARS],
        const unsigned int lineStartLen,
        const char * &start,
        unsigned int &length,
        bool canEndWithNull);

    SearchImpl(){InitializeCriticalSection(&foundLinesCS);};
    friend class Search;
};

Search::Search() : pimpl_(new SearchImpl()){}
Search::~Search() { delete pimpl_;}

bool SearchImpl::compare_logline (LogLine * first, LogLine * second)
{
    unsigned int i=0;
    if(first && second)
    {
        if (first->time<second->time) return true;
        else if (first->time>second->time) return false;
        else return false;    //equal
    }
    else return false;
}



void Search::sortResults()
{
    if(pimpl_)
        pimpl_->foundLines.sort(SearchImpl::compare_logline);
    else
        printf("ERROR: attempted to sortResults on a Search without a pimpl_.\n");
}

unsigned int Search::nResultsFound(){return (unsigned int)pimpl_->foundLines.size();};

bool SearchImpl::shouldAbort(log_time curTime)
{
    return !ignoreDates && abortOffset != -1 &&
        ((curTime < start - abortOffset) ||
        (curTime > end + abortOffset));
}

bool Search::init(const char *searchPattern, bool caseInsensitive, bool wildcards, log_time s, log_time e, log_time abort)
{
    if(pimpl_ && pimpl_->p.makePattern(searchPattern, caseInsensitive, wildcards))
    {
        pimpl_->start = ((s!=0)?s:0);
        pimpl_->end = ((e!=0)?e:~0);
        pimpl_->abortOffset = abort;
        pimpl_->ignoreDates = pimpl_->start == 0 && pimpl_->end == ~0 && pimpl_->abortOffset == -1LL;
        return true;
    }
    return false;
}

unsigned int Search::findLines(const char *text, unsigned int maxLine, bool singleLine, bool endOfFile)
{
    if(!text || !pimpl_)
        return 0;

    char *results = 0;
    log_time date, firstDate;
    const char *position = text;
    DatePattern *d = NULL;
    const unsigned long long *patternStart = NULL;
    unsigned int lengthStart;

    d = DatePattern::getDatePattern(text, position, firstDate, maxLine);
    date = firstDate;
    if(d)
    {
        patternStart = d->getPatternMask();
        lengthStart = d->getLength();
    }
    else if(!pimpl_->ignoreDates)
    {
        return 0;
    }

    if(pimpl_->ignoreDates)
    {
        static Pattern p("\n", false, false);
        patternStart = p.getPatternMask();
        lengthStart = p.getLength();
    }
    const unsigned long long *patternMask = pimpl_->p.getPatternMask();
    const unsigned int patternLen = pimpl_->p.getLength();
    const char *foundPosition = NULL;
    unsigned int lineLen;
    position = text;    //restart
    while(!pimpl_->shouldAbort(date) && 
        pimpl_->myStrStr(position, patternMask, patternLen, patternStart, lengthStart, foundPosition, lineLen, endOfFile))
    { 
        bool validLine = true;

        if(singleLine)    //just return the whole line
        {
            lineLen += foundPosition-text;
            foundPosition = text;
        }
        else if (!pimpl_->ignoreDates)
        {
            validLine = d->dateToTime(foundPosition, date, lineLen);
        }

        if(validLine && pimpl_->ignoreDates || (date >= pimpl_->start && date <= pimpl_->end && lineLen))
        {
            if(!results)
                estrCreate(&results);
            estrConcatFixedWidth(&results, foundPosition, lineLen);
            if(!singleLine)
                estrConcatChar(&results, '\n');
        }
        position = foundPosition + lineLen;
    }
    delete d;
    if(results)
    {
        pimpl_->addLog(results,firstDate);
        estrDestroy(&results);
    }
    return foundPosition-text;
}

bool SearchImpl::myStrStr(const char *searchMe, 
                          const unsigned long long patternMask[MAX_CHARS], 
                          const unsigned int patternLen,
                          const unsigned long long lineStartMask[MAX_CHARS],
                          const unsigned int lineStartLen,
                          const char * &start,
                          unsigned int &length,
                          bool canEndWithNull)
{
    unsigned long long historyPattern = ~1ULL;
    unsigned long long historyStart = ~1ULL;
    unsigned long long donePattern = 1ULL << patternLen;
    unsigned long long doneStart = 1ULL << lineStartLen;
    unsigned int startIdx = 0;
    bool foundEnd = 0;
    bool foundPattern = 0;
    unsigned char c;
    unsigned int i;
    length = 0;
    start = searchMe;
    c = searchMe[0];
    for(i = 0; searchMe[i]; ++i)
    {
        c = searchMe[i];

        historyStart |= lineStartMask[c];
        historyStart <<= 1;

        if(!(historyStart&doneStart))
        {
            foundEnd = foundPattern;
            if(!foundEnd)
                startIdx = i - lineStartLen + 1;
            else
                break;
        }

        historyPattern |= patternMask[c];
        historyPattern <<= 1;
        foundPattern |= !(historyPattern&donePattern);    //either 0 or 1
    }

    start = &searchMe[startIdx];
    if((canEndWithNull &&!!foundPattern) || foundEnd)
    {
        length = i-startIdx;//i is either startIdx[1] or we didn't find another start, so startIdx[1] is 0
        length -= (foundEnd)?lineStartLen:0;
    }
    return !!foundPattern;
}

std::string Search::popResult()
{

    std::string ret;
    if(pimpl_)
    {
        EnterCriticalSection(&pimpl_->foundLinesCS);
        if(nResultsFound())
        {
            SearchImpl::LogLine *log = pimpl_->foundLines.front();
            if(log)
            {
                ret = log->line;
                delete log;
            }
            pimpl_->foundLines.pop_front();
        }
        else
            ret = "";
        LeaveCriticalSection(&pimpl_->foundLinesCS);
        return ret;
    }
    throw "popResult on Search without a pimpl.";
    return NULL;
}
