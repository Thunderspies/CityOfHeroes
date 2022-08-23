#include "logParser3.h"
#include <utilitieslib/utils/wininclude.h> //for threads
#include "LogSearch.h"
#include "LogTime.h"
#include "LogPattern.h"
#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/utils.h>
#include "LogConfig.h"
extern "C"
{
#include <utilitieslib/utils/file.h>
}

bool LogFileSearch::getFilesByPattern(char *dirName, 
                                      const char *filePattern, 
                                      Search &fileSearch,
                                      log_time start, 
                                      log_time end, 
                                      log_time threshold)
{
    if(dirName && filePattern)
    {
        char **allFiles = 0;
        int nFiles;
        fileSearch.init(filePattern, true, true, start, end, threshold);

        allFiles = fileScanDir(dirName,&nFiles);
        for(int i = 0; i < nFiles; i++)
        {
            fileSearch.findLines(allFiles[i], -1, true, true);
        }
        return true;
    }
    else
        throw "getFilesByPattern received a NULL";
    return false;
}

bool LogFileSearch::searchFile(std::string fileStr, 
                               const LpConfig &config, 
                               Search &s, 
                               char *buffer)
{
    const char *file = fileStr.c_str();
    if(!file || !file[0])
        return false;
    FILE *in = static_cast<FILE*>(fopen(file, "rbz"));
    if(!in)
    {
        printf("\nunable to open gzipped file %s\n", file);
        perror("Error");
        return false;
    }
    try
    {
        bool fileDone = false;
        char *startRead = buffer;
        unsigned int fileSegment = 0;
        while(!fileDone)
        {
            unsigned int startOffset = startRead-buffer;
            unsigned int readTo = config.bufferSize+config.maxLineSize-1 - startOffset;
            unsigned int read = fread(startRead, 1, readTo, in);
            bool isLastSegment = readTo != read;
            fileSegment++;

            startRead[read] = 0; //we have to end the file.
            unsigned int searchedTo = s.findLines(buffer, config.maxLineSize, false, isLastSegment);
            if(!isLastSegment) //we have more file to read.
            {
                unsigned int unread = startOffset+read-searchedTo;
                if(!searchedTo)
                {
                    printf("Warning: no date found in segment %d of %s\n", fileSegment, file);
                }
                if(unread > searchedTo)
                {
                    printf("Last line in %s, segment %d is too long.  Aborting log parse on this file.\n", file, fileSegment);
                    fileDone = true;
                }
                else
                {
                    char *oldStart = buffer + searchedTo;
                    strncpy(buffer, oldStart, unread);
                    startRead = buffer + unread;
                }
            }
            else
            {
                if(!read)
                    printf("Unknown error reading %s.  Aborting log parse on this file.\n", file);
                fileDone = true;
            }
        }
    }
    catch (const char * e)
    {
        printf("Failed to search %s: %s\n", file, e);
    }
    fclose(in);
    return true;
}



int LogFileSearch::searchWork()
{
    
    char *myText = new char[config.bufferSize+config.maxLineSize];
    if(!myText)
    {
        printf("You don't have enough memory.  Paragon Tech will need to make the app take less memory.\n");
        return -1;
    }
    InterlockedIncrement(&concurrentSearchers);
    while(filesToSearch.nResultsFound())
    {
        searchFile(filesToSearch.popResult(), config, s, myText);
    }
    InterlockedDecrement(&concurrentSearchers);
    delete[] myText;
    
    return 0;
}


/**
* This is the entry point for new search threads.
*
* All new search threads start here.
*
* @param $lpParam
*   The LogFileSearch structure must be passed here.
*
* @return
*   "@return" true if the search was successful.  False if not.

*/
static unsigned int  WINAPI s_thread_doSearch(void * lpParam)
{
    LogFileSearch * lfSearch = (LogFileSearch *)lpParam;
    if(lfSearch)
        return lfSearch->searchWork();
    else
        return false;
}


bool LogFileSearch::init(int argc, char **args)
{
    if(!config.readArguments(argc, args))
    {
        return false;
    }
    return getFilesByPattern(config.directoryName, config.filePattern, filesToSearch, config.startTime, config.endTime, config.abortThreshold);
}

void LogFileSearch::updateSearchConsole(unsigned int nFiles,
                                        unsigned int &nFilesRemaining,
                                        unsigned int &nFilesRemainingOld)
{
        if(nFilesRemainingOld != nFilesRemaining)
        {
            nFilesRemainingOld = nFilesRemaining;
            float newPercentDone = (1 - nFilesRemaining/(float)nFiles)*100;
            printf("reading %.2f%%         ", (newPercentDone>0)?newPercentDone:0);
            CONSOLE_SCREEN_BUFFER_INFO info;
            PCONSOLE_SCREEN_BUFFER_INFO  cInfo = &info;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), cInfo);
            if(cInfo)
            {
                COORD pt = cInfo->dwCursorPosition;
                pt.X= 0;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pt);
            }
        }
}


bool LogFileSearch::performSearch()
{
    unsigned int nResults = 0;
    FILE * out = static_cast<FILE*>(fopen(config.outputName, "w"));
    if(!out)    //I open the file early so we don't waste time searching if we can't open the output file.
    {
        printf("Couldn't open the specified output file %s.  Aborting search.\n", config.outputName);
        return -1;
    }

    try
    {
        unsigned int nFiles = filesToSearch.nResultsFound();
        unsigned int nFilesRemaining = 0;
        unsigned int nFilesRemainingOld = 0;
        float percentDone = 0;
        if(!s.init(config.search, !config.caseSensitive, config.useWildcards, config.startTime, config.endTime, config.abortThreshold))
            throw "Unable to initialize search";

        for(unsigned int i = 0; i < config.nThreads; i++)
        {
            unsigned int threadId;
            HANDLE thread = CreateThread( NULL, 0, s_thread_doSearch, this, 0, &threadId);
            if(!threadId)
                throw "Unable to create thread";
            else
                CloseHandle(thread);
        }

        while((nFilesRemaining = filesToSearch.nResultsFound() + concurrentSearchers ))
        {
            updateSearchConsole(nFiles, nFilesRemaining, nFilesRemainingOld);
            Sleep(1000);
        }
        
        while(concurrentSearchers){Sleep(500);}
        nFilesRemaining = 0;
        updateSearchConsole(nFiles, nFilesRemaining, nFilesRemainingOld);

        if(config.startTime != -1)//we care about time.  This is a hack.
            s.sortResults();
        nResults += s.nResultsFound();
        while(s.nResultsFound())
        {
            std::string output = s.popResult();
            fprintf(out, "%s\n", output.c_str());
        }
    }
    catch (const char *errorMsg)
    {
        printf("%s\n", errorMsg);
        fclose(out);
        return false;
    }

    fclose(out);
    printf("\nResults found: %d; %f seconds\n", nResults, (((float)clock())/CLOCKS_PER_SEC));
    return true;
}

int main(int argc, char **args)
{
    LogFileSearch fileSearch;
    setbuf(stdout, NULL);
    
    fileSearch.init(argc, args);
    fileSearch.performSearch();    
}


