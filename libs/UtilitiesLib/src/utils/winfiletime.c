/*------------------------------------------------------------------------------
 * Copyright (c) 2003, Steve Hay. All rights reserved.
 * Portions Copyright (c) 2001, Jonathan M Gilligan. Used with permission.
 * Portions Copyright (c) 2001, Tony M Hoyle. Used with permission.
 *
 * Module Name: Win32::UTCFileTime
 * Source File: UTCFileTime.xs
 * Description: C and XS code for xsubpp
 *------------------------------------------------------------------------------
 */

/*------------------------------------------------------------------------------
 *
 * C code to be copied verbatim by xsubpp.
 */
#include "utilitieslib/utils/wininclude.h"  /* For the Win32 API stuff.         */

#include <ctype.h>                      /* For isalpha() and tolower().     */
#include <direct.h>                     /* For _getdrive().                 */
#include <errno.h>                      /* For EACCES.                      */
#include <fcntl.h>                      /* For the O_* flags.               */
#include <io.h>                         /* For lowio and _get_osfhandle().  */
#include <memory.h>                     /* For memset().                    */
#include <stdlib.h>                     /* For errno and tolower().         */
#include <string.h>                     /* For the str*() functions.        */
#include <sys/types.h>                  /* For struct stat.                 */
#include <sys/stat.h>                   /* For struct stat.                 */

#define WIN32_LEAN_AND_MEAN             /* Don't pull in too much crap when */
                                        /* including <windows.h> next.      */
#include "utilitieslib/utils/timing.h"
#include "utilitieslib/stdtypes.h"


#define _Debug() (0)
#define _MAX_FS      32
#define _isslash(c)  ((c) == '\\' || (c) == '/')
#define _isutcfs(fs) (!strstr(fs, "FAT"))

void warn(char *s) {}
void croak(char *s) {}

#ifndef _XBOX
/*
 * Function to determine whether or not the operating system platform is Windows
 * NT (as opposed to Win32s, Windows [95/98/ME] or Windows CE).
 */
static BOOL _IsWinNT(void) {
    static BOOL     initialised = FALSE;
    static BOOL     is_winnt;
    OSVERSIONINFOA   osver;

    if (!initialised) {
        ZeroStruct(&osver);
        osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (GetVersionExA(&osver)) {
            is_winnt = (osver.dwPlatformId == VER_PLATFORM_WIN32_NT);
        }
        else {
            warn("Could not determine operating system platform. Assuming "
                "the platform is Windows NT");
            is_winnt = TRUE;
        }
        initialised = TRUE;
    }

    return is_winnt;
}
#endif

/*
 * Function to determine whether or not file times are stored as UTC in the
 * filesystem that a given file is stored in.
 *
 * This function is based on code written by Tony M Hoyle.
 */

static BOOL _IsUTCVolume(
    const char  *name)
{
    int         len;
    char        szFs[_MAX_FS];


    len = (int)strlen(name);

    if (len >= 2 && isalpha((unsigned char)name[0]) && name[1] == ':') {
        /* An absolute path with a drive letter is specified. */
        char root[4] = "?:\\";
        root[0] = name[0];

        if (GetVolumeInformationA(root, NULL, 0, NULL, NULL, NULL, szFs,
                sizeof(szFs)))
        {
            return _isutcfs(szFs);
        }
        else {
            warn("Could not determine name of filesystem. Assuming file times "
                "are stored as UTC-based values");
            return TRUE;
        }
    }
    else if (len >= 5 && _isslash(name[0]) && _isslash(name[1])) {
        /* An absolute path with a UNC share (the minimum length of which is 5,
         * as in \\x\y) is specified. We assume that the filesystem is NTFS, and
         * hence stores UTC file times, in this case. */
        return TRUE;
    }
    else {
        /* A relative path, or something invalid, is specified. We examine the
         * filesystem of the current directory in this case. */
        if (GetVolumeInformationA(NULL, NULL, 0, NULL, NULL, NULL, szFs,
                sizeof(szFs)))
        {
            return _isutcfs(szFs);
        }
        else {
            warn("Could not determine name of filesystem. Assuming file times "
                "are stored as UTC-based values");
            return TRUE;
        }
    }
}

/*
 * Function to determine whether or not a given year is a leap year, according
 * to the standard Gregorian rule (namely, every year divisible by 4 except
 * centuries indivisble by 400).
 *
 * This function is based on code written by Jonathan M Gilligan.
 */

static BOOL _IsLeapYear(
    WORD year)
{
    return ( ((year & 3u) == 0) &&
             ((year % 100u == 0) || (year % 400u == 0)) );
}

static const WORD days_in_month[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/*
 * Function to compare a test date against a target date. The target date must
 * be specified in the day-in-month format, rather than the absolute format,
 * used by the StandardDate and DaylightDate members of a TIME_ZONE_INFORMATION
 * structure.
 * If the test date is earlier than the target date, it returns a negative
 * number. If the test date is later than the target date, it returns a positive
 * number. If the test date equals the target date, it returns zero.
 * Specifically, it returns:
 * -4/+4 if the test month is less than/greater than the target month;
 * -2/+2 if the test day   is less than/greater than the target day;
 * -1/+1 if the test time  is less than/greater than the target time;
 *   0   if the test date            equals          the target date.
 *
 * This function is based on code written by Jonathan M Gilligan.
 */

static int _CompareTargetDate(
    const SYSTEMTIME *test_st,
    const SYSTEMTIME *target_st)
{
    /* Check that the given dates are in the correct format. */
    if (test_st->wYear == 0)
        croak("The test date used in a date comparison is not in the "
              "required \"absolute\" format");
    if (target_st->wYear != 0)
        croak("The target date used in a date comparison is not in the "
              "required \"day-in-month\" format");

    if (test_st->wMonth != target_st->wMonth) {
        /* The months are different. */

        return (test_st->wMonth > target_st->wMonth) ? 4 : -4;
    }
    else {
        /* The months are the same. */

        WORD    first_dow;
        WORD    temp_dom;
        WORD    last_dom;
        int     test_ms;
        int     target_ms;

        /* If w is the day-of-the-week of some arbitrary day-of-the-month x then
         * the day-of-the-week of the first day-of-the-month is given by
         * ((1 + w - x) mod 7). */
        first_dow = (WORD)((1u + test_st->wDayOfWeek - test_st->wDay) % 7u);

        /* If y is the day-of-the-week of the first day-of-the-month then
         * the day-of-the-month of the first day-of-the-week z is given by
         * ((1 + z - y) mod 7). */
        temp_dom = (WORD)((1u + target_st->wDayOfWeek - first_dow) % 7u);

        /* If t is the day-of-the-month of the first day-of-the-week z then
         * the day-of-the-month of the (n)th day-of-the-week z is given by
         * (t + (n - 1) * 7). */
        temp_dom = (WORD)(temp_dom + target_st->wDay * 7u);

        /* We need to handle the special case of the day-of-the-month of the
         * last day-of-the-week z. For example, if we tried to calculate the
         * day-of-the-month of the fifth Tuesday of the month then we may have
         * overshot, and need to correct for that case.
         * Get the last day-of-the-month (with a suitable correction if it is
         * February of a leap year) and move the temp_dom that we have
         * calculated back one week at a time until it doesn't exceed that. */
        last_dom = days_in_month[target_st->wMonth - 1];
        if (test_st->wMonth == 2 && _IsLeapYear(test_st->wYear))
            ++last_dom;
        while (temp_dom > last_dom)
            temp_dom -= 7;

        if (test_st->wDay != temp_dom) {
            /* The days are different. */

            return (test_st->wDay > temp_dom) ? 2 : -2;
        }
        else {
            /* The days are the same. */

            test_ms = ((test_st->wHour     * 60   +
                        test_st->wMinute)  * 60   +
                       test_st->wSecond  ) * 1000 +
                      test_st->wMilliseconds;
            target_ms = ((target_st->wHour     * 60   +
                          target_st->wMinute)  * 60   +
                         target_st->wSecond  ) * 1000 +
                        target_st->wMilliseconds;
            test_ms -= target_ms;
            return (test_ms > 0) ? 1 : (test_ms < 0) ? -1 : 0;
        }
    }
}

/*
 * Function to return the time zone bias for a given local time. The bias is the
 * difference, in minutes, between UTC and local time: UTC = local time + bias.
 *
 * This function is based on code written by Jonathan M Gilligan.
 */

int _GetTimeZoneBias(
    const SYSTEMTIME *st)
{
    TIME_ZONE_INFORMATION   tz;
    int                     bias;

    if (GetTimeZoneInformation(&tz) == TIME_ZONE_ID_INVALID)
        croak("Could not get time zone information");

    /* We only deal with cases where the transition dates between standard time
     * and daylight time are given in "day-in-month" format rather than
     * "absolute" format. */
    if (tz.DaylightDate.wYear != 0 || tz.StandardDate.wYear != 0)
        croak("Cannot handle year-specific DST clues in time zone information");

    /* Get the difference between UTC and local time. */
    bias = tz.Bias;

    /* Add on the standard bias (usually 0) or the daylight bias (usually -60)
     * as appropriate for the given time. */
    if (_CompareTargetDate(st, &tz.DaylightDate) < 0) {
        bias += tz.StandardBias;
    }
    else if (_CompareTargetDate(st, &tz.StandardDate) < 0) {
        bias += tz.DaylightBias;
    }
    else {
        bias += tz.StandardBias;
    }

    return bias;
}

/* Number of "clunks" (100-nanosecond intervals) in one second. */
static const ULONGLONG  clunks_per_second = 10000000L;

/* The epoch of __time32_t values (00:00:00 Jan 01 1970 UTC) as a SYSTEMTIME. */
static const SYSTEMTIME base_st = {
    1970,   /* wYear            */
    1,      /* wMonth           */
    0,      /* wDayOfWeek       */
    1,      /* wDay             */
    0,      /* wHour            */
    0,      /* wMinute          */
    0,      /* wSecond          */
    0       /* wMilliseconds    */
};

/*
 * Function to convert a FILETIME to a __time32_t.
 * The __time32_t will be UTC-based, so if the FILETIME is local time-based then
 * set the ft_is_local flag so that a local time adjustment can be made.
 *
 * This function is based on code written by Jonathan M Gilligan.
 */

BOOL _FileTimeToUnixTime(
    const FILETIME  *ft,
    __time32_t          *ut,
    const BOOL      ft_is_local)
{
    int             bias = 0;
    static BOOL     initialised = FALSE;
    static FILETIME base_ft;
    ULARGE_INTEGER  it;

    if (ft_is_local) {
        SYSTEMTIME  st;

        /* Convert the FILETIME to a SYSTEMTIME, and get the bias from that. */
        if (FileTimeToSystemTime(ft, &st)) {
            bias = _GetTimeZoneBias(&st);
        }
        else {
            if (_Debug())
                warn("FileTimeToSystemTime() failed\n");

            /* Do the same as mktime() in the event of failure. */
            *ut = -1;
            return FALSE;
        }
    }

    /* Get the epoch of __time32_t values as a FILETIME. This calculation only needs
     * to be done once. */
    if (!initialised) {
        if (!SystemTimeToFileTime(&base_st, &base_ft))
            croak("Could not convert base SYSTEMTIME to FILETIME");
        initialised = TRUE;
    }

    /* Convert the FILETIME (which is expressed as the number of clunks
     * since 00:00:00 Jan 01 1601 UTC) to a __time32_t value by subtracting the
     * FILETIME representation of the epoch of __time32_t values and then
     * converting clunks to seconds. */
    it.QuadPart  = ((ULARGE_INTEGER *)ft)->QuadPart;
    it.QuadPart -= ((ULARGE_INTEGER *)&base_ft)->QuadPart;
    it.QuadPart /= clunks_per_second;

    /* Add the bias (which is in minutes) to get UTC. */
    it.QuadPart += bias * 60;

    *ut = it.LowPart;
    return TRUE;
}

/*
 * Function to convert a __time32_t to a FILETIME.
 * The __time32_t is UTC-based, so if a local time-based FILETIME is required then
 * set the make_ft_local flag so that a local time adjustment can be made.
 *
 * This function is based on code written by Tony M Hoyle.
 */

BOOL _UnixTimeToFileTime(
    const __time32_t    ut,
    FILETIME        *ft,
    const BOOL      make_ft_local)
{
    static BOOL     initialised = FALSE;
    static FILETIME base_ft;
    ULARGE_INTEGER  it;
    int             bias = 0;

    /* Get the epoch of __time32_t values as a FILETIME. This calculation only needs
     * to be done once. */
    if (!initialised) {
        if (!SystemTimeToFileTime(&base_st, &base_ft))
            croak("Could not convert base SYSTEMTIME to FILETIME");
        initialised = TRUE;
    }

    /* Convert the __time32_t value to a FILETIME (which is expressed as the
     * number of clunks since 00:00:00 Jan 01 1601 UTC) by converting
     * seconds to clunks and then adding the FILETIME representation of the
     * epoch of __time32_t values. */
    it.LowPart   = ut;
    it.HighPart  = 0;
    it.QuadPart *= clunks_per_second;
    it.QuadPart += ((ULARGE_INTEGER *)&base_ft)->QuadPart;

    if (make_ft_local) {
        SYSTEMTIME  st;

        /* Convert the FILETIME to a SYSTEMTIME, and get the bias from that. */
        if (FileTimeToSystemTime((FILETIME *)&it, &st)) {
            bias = _GetTimeZoneBias(&st);
        }
        else {
            if (_Debug())
                warn("FileTimeToSystemTime() failed\n");

            /* Set a zero FILETIME in the event of failure. */
            (*ft).dwLowDateTime  = 0;
            (*ft).dwHighDateTime = 0;
            return FALSE;
        }
    }

    /* Add the bias (which is in minutes) to get UTC. */
    it.QuadPart += bias * 60;

    *(ULARGE_INTEGER *)ft = it;
    return TRUE;
}

/*
 * Function to convert three FILETIME values (the last access time, last
 * modification time and creation time of a given file from calls to either
 * FindFirstFile() or GetFileInformationByHandle()) to __time32_t values.
 * The __time32_t values will be UTC-based, whatever filesystem the file is stored
 * in.
 *
 * This function is based on code written by Jonathan M Gilligan.
 */

static BOOL _FileTimesToUnixTimes(
    const char      *name,
    const FILETIME  *atime_ft,
    const FILETIME  *mtime_ft,
    const FILETIME  *ctime_ft,
    __time32_t          *u_atime_t,
    __time32_t          *u_mtime_t,
    __time32_t          *u_ctime_t)
{
    BOOL            ret;

    if (1 || _IsUTCVolume(name)) {
        /* The filesystem stores UTC file times. FindFirstFile() and
         * GetFileInformationByHandle() return them to us as unadulterated UTC
         * FILETIMEs, so just convert them to __time32_t values to be returned. */
        ret =   (!u_atime_t || _FileTimeToUnixTime(atime_ft, u_atime_t, FALSE)) &&
                (!u_mtime_t || _FileTimeToUnixTime(mtime_ft, u_mtime_t, FALSE)) &&
                (!u_ctime_t || _FileTimeToUnixTime(ctime_ft, u_ctime_t, FALSE));
    }
    else {
        FILETIME    l_atime_ft;
        FILETIME    l_mtime_ft;
        FILETIME    l_ctime_ft;

        /* The filesystem stores local file times. FindFirstFile() and
         * GetFileInformationByHandle() return them to us as incorrectly
         * converted UTC FILETIMEs, so undo the faulty time zone conversion and
         * then redo it properly, converting to __time32_t values to be returned in
         * the process. */
        ret =   FileTimeToLocalFileTime(atime_ft, &l_atime_ft)          &&
                FileTimeToLocalFileTime(mtime_ft, &l_mtime_ft)          &&
                FileTimeToLocalFileTime(ctime_ft, &l_ctime_ft)          &&
                (!u_atime_t || _FileTimeToUnixTime(&l_atime_ft, u_atime_t, TRUE)) &&
                (!u_mtime_t || _FileTimeToUnixTime(&l_mtime_ft, u_mtime_t, TRUE)) && 
                (!u_ctime_t || _FileTimeToUnixTime(&l_ctime_ft, u_ctime_t, TRUE));
    }

    return ret;
}

/*
 * Function to convert file attributes as returned by the Win32 API function
 * GetFileAttributes() into a Unix mode as stored in the st_mode field of a
 * struct stat.
 */

static unsigned short _FileAttributesToUnixMode(
    const DWORD    fa,
    const char     *name)
{
    unsigned short st_mode = 0;
    int            len;
    const char     *p;

    if (fa & FILE_ATTRIBUTE_DIRECTORY)
        st_mode |= _S_IFDIR;
    else
        st_mode |= _S_IFREG;

    if (fa & FILE_ATTRIBUTE_READONLY)
        st_mode |= (  _S_IREAD       +
                     (_S_IREAD >> 3) +
                     (_S_IREAD >> 6));
    else
        st_mode |= ( (_S_IREAD | _S_IWRITE)       +
                    ((_S_IREAD | _S_IWRITE) >> 3) +
                    ((_S_IREAD | _S_IWRITE) >> 6));

    if (fa & FILE_ATTRIBUTE_DIRECTORY)
        st_mode |= (  _S_IEXEC       +
                     (_S_IEXEC >> 3) +
                     (_S_IEXEC >> 6));

    len = (int)strlen(name);
    if (len >= 4 && (*(p = name + len - 4) == '.') &&
            (!stricmp(p, ".exe") ||  !stricmp(p, ".bat") ||
             !stricmp(p, ".com") || (!stricmp(p, ".cmd")
#ifndef _XBOX
             && _IsWinNT()
#endif
             )))
        st_mode |= (  _S_IEXEC       +
                     (_S_IEXEC >> 3) +
                     (_S_IEXEC >> 6));

    return st_mode;
}

/*
 * Function to emulate the standard C library function stat(), setting the
 * last access time, last modification time and creation time members of the
 * given "stat" structure to UTC-based __time32_t values, whatever filesystem the
 * file is stored in.
 *
 * This function is based on code in CVSNT's wnt_stat() function, and on code in
 * Perl's win32_stat() function.
 */

int _AltStat(
    const char                  *name,
    struct _stat32             *st_buf)
{
    int                         drive = 1;
    HANDLE                      hndl;
    BY_HANDLE_FILE_INFORMATION  bhfi;

    PERFINFO_AUTO_START("_AltStat", 1);
    
    ZeroStruct(&bhfi);

    /* Use CreateFile(), rather than FindFirstFile() like Microsoft's stat()
     * does, for three reasons:
     * (1) It doesn't require "List Folder Contents" permission on the parent
     *     directory like FindFirstFile() does;
     * (2) It works for directories specified with a trailing slash or backslash
     *     and it works for root (drive or UNC) directories like C: and
     *     \\SERVER\SHARE, with or without a trailing slash or backslash
     *     (provided that this is a Windows NT platform and the
     *     FILE_FLAG_BACKUP_SEMANTICS flag is passed to allow directory handles
     *     to be obtained), whereas FindFirstFile() requires non-root
     *     directories to not have a trailing slash or backslash and requires
     *     root directories to have a trailing \*; and
     * (3) The BY_HANDLE_FILE_INFORMATION stucture returned by a subsequent call
     *     to GetFileInformationByHandle() contains the number of links to the
     *     file, which the WIN32_FIND_DATA structure returned by FindFirstFile()
     *     does not. */
    if ((hndl = CreateFileA(name,
                    GENERIC_READ,
                    FILE_SHARE_READ|FILE_SHARE_WRITE,
                    NULL,
                    OPEN_EXISTING,
                    FILE_FLAG_BACKUP_SEMANTICS,
                    NULL)) == INVALID_HANDLE_VALUE)
    {
        /* If this is a valid directory (presumably under a Windows 95 platform
         * on which the FILE_FLAG_BACKUP_SEMANTICS flag doesn't do the trick)
         * then set all the fields except st_mode to zero and return TRUE, like
         * Perl's built-in functions do in this case. */
        DWORD fa = GetFileAttributesA(name);
        if (fa != 0xFFFFFFFF && (fa & FILE_ATTRIBUTE_DIRECTORY)) {
            ZeroStruct(st_buf);
            st_buf->st_mode = _FileAttributesToUnixMode(fa, name);
            PERFINFO_AUTO_STOP();
            return 0;
        }
        else {
            PERFINFO_AUTO_STOP();
            return -1;
        }
    }
    else {
        GetFileInformationByHandle(hndl, &bhfi);
        if (!CloseHandle(hndl))
            warn("Could not close file object handle");
    }

    {
        __time32_t mtime;
        if (!_FileTimesToUnixTimes(name,
                &bhfi.ftLastAccessTime, &bhfi.ftLastWriteTime, &bhfi.ftCreationTime,
                NULL, &mtime, NULL))
        {
            PERFINFO_AUTO_STOP();
            return -1;
        }
        st_buf->st_mtime = mtime;
    }

    st_buf->st_mode = _FileAttributesToUnixMode(bhfi.dwFileAttributes, name);

    st_buf->st_nlink = (short)bhfi.nNumberOfLinks;

    st_buf->st_size = bhfi.nFileSizeLow;

    /* Get the drive from the name, or use the current drive. */
    if (strlen(name) >= 2 && isalpha((unsigned char)name[0]) && name[1] == ':')
        drive = tolower(name[0]) - 'a' + 1;
#ifndef _XBOX
    else
        drive = _getdrive();
#endif

    st_buf->st_dev = st_buf->st_rdev = (_dev_t)(drive - 1);

    st_buf->st_ino = st_buf->st_uid = st_buf->st_gid = 0;

    PERFINFO_AUTO_STOP();

    return 0;
}

/*
 * Function to get the last access time, last modification time and creation
 * time of a given file.
 * The values are returned expressed as UTC-based __time32_t values, whatever
 * filesystem the file is stored in.
 *
 * This function is based on code written by Jonathan M Gilligan.
 */

int _GetUTCFileTimes(
    const char                  *name,
    __time32_t                      *u_atime_t,
    __time32_t                      *u_mtime_t,
    __time32_t                      *u_ctime_t)
{
    HANDLE                      hndl;
    WIN32_FIND_DATAA             wfd;
    BY_HANDLE_FILE_INFORMATION  bhfi;

    /* Use FindFirstFile() like Microsoft's stat() does, rather than the more
     * obvious GetFileTime(), to avoid a problem with the latter caching UTC
     * time values on FAT volumes. */
    if ((hndl = FindFirstFileA(name, &wfd)) == INVALID_HANDLE_VALUE) {
        /* FindFirstFile() will fail if the given name specifies a directory
         * with a trailing slash or backslash, or if it is a root (drive or UNC)
         * directory like C: or \\SERVER\SHARE. CreateFile() does not have these
         * restrictions (provided that this is a Windows NT platform and the
         * FILE_FLAG_BACKUP_SEMANTICS flag is passed to allow directory handles
         * to be obtained), so try that instead if FindFirstFile() failed. */
        if ((hndl = CreateFileA(name,
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_BACKUP_SEMANTICS,
                        NULL)) == INVALID_HANDLE_VALUE)
        {
            /* This function is only ever called after a call to Perl's built-in
             * stat() or lstat() function has already succeeded on the same
             * name, so this must just be a directory under a Windows 95
             * platform on which the FILE_FLAG_BACKUP_SEMANTICS flag doesn't do
             * the trick. Set all the file times to zero and return TRUE, like
             * Perl's built-in functions do in this case. */
            if(u_atime_t){
                *u_atime_t = 0;
            }
            if(u_mtime_t){
                *u_mtime_t = 0;
            }
            if(u_ctime_t){
                *u_ctime_t = 0;
            }
            return TRUE;
        }
        else {
            GetFileInformationByHandle(hndl, &bhfi);
            if (!CloseHandle(hndl))
                warn("Could not close file object handle");
            wfd.ftLastAccessTime = bhfi.ftLastAccessTime;
            wfd.ftLastWriteTime  = bhfi.ftLastWriteTime;
            wfd.ftCreationTime   = bhfi.ftCreationTime;
        }
    }
    else {
        if (!FindClose(hndl))
            warn("Could not close file search handle");
    }

    return _FileTimesToUnixTimes(name,
            &wfd.ftLastAccessTime, &wfd.ftLastWriteTime, &wfd.ftCreationTime,
            u_atime_t, u_mtime_t, u_ctime_t);
}
// #endif

/*
 * Function to set the last access time and last modification time of a given
 * file.
 * The values should be supplied expressed as UTC-based __time32_t values, whatever
 * filesystem the file is stored in.
 */

int _SetUTCFileTimes(
    const char      *name,
    const __time32_t    u_atime_t,
    const __time32_t    u_mtime_t)
{
    int             fd;
    BOOL            ret = FALSE;
    HANDLE          hndl;

    /* Try opening the file normally first, like Microsoft's utime(), and hence
     * Perl's win32_utime(), does. Note that this will fail with errno EACCES if
     * name specifies a directory or a read-only file. */
    if ((fd = open(name, O_RDWR | O_BINARY)) < 0) {
        if (errno == EACCES) {
            /* CreateFile() can open directory handles (provided that this is a
             * Windows NT platform and the FILE_FLAG_BACKUP_SEMANTICS flag is
             * passed to allow directory handles to be obtained), so try that
             * instead like Perl's win32_utime() does. This will (and should)
             * still fail on read-only files. */
            if ((hndl = CreateFileA(name,
                            GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_DELETE,
                            NULL,
                            OPEN_EXISTING,
                            FILE_FLAG_BACKUP_SEMANTICS,
                            NULL)) == INVALID_HANDLE_VALUE)
            {
                return FALSE;
            }
        }
        else {
            return FALSE;
        }
    }
    else {
        hndl = (HANDLE)_get_osfhandle(fd);
    }

    /* Use NULL for the creation time passed to SetFileTime() like Microsoft's
     * utime() does. This simply means that the information is not changed.
     * There is no need to retrieve the existing value first in order to reset
     * it like Perl's win32_utime() does. */
    if (1 || _IsUTCVolume(name)) {
        FILETIME    u_atime_ft;
        FILETIME    u_mtime_ft;

        /* The filesystem stores UTC file times. SetFileTime() will set its UTC
         * FILETIME arguments without change, so just convert the __time32_t values
         * to UTC FILETIMEs to be set. */
        if (_UnixTimeToFileTime(u_atime_t, &u_atime_ft, FALSE) &&
            _UnixTimeToFileTime(u_mtime_t, &u_mtime_ft, FALSE))
        {
            if (!SetFileTime(hndl, NULL, &u_atime_ft, &u_mtime_ft)) {
                ret = FALSE;
            }
            else {
                ret = TRUE;
            }
        }
        else {
            ret = FALSE;
        }
    }
    else {
        FILETIME    l_atime_ft;
        FILETIME    l_mtime_ft;
        FILETIME    u_atime_ft;
        FILETIME    u_mtime_ft;

        /* The filesystem stores local file times. SetFileTime() will set its
         * UTC FILETIME arguments after an incorrect local time conversion, so
         * do the conversion properly first, converting the __time32_t values to
         * local FILETIMEs in the process, and then do an extra incorrect UTC
         * conversion ready to be undone by SetFileTime() when it sets them. */
        if (_UnixTimeToFileTime(u_atime_t, &l_atime_ft, TRUE) &&
            _UnixTimeToFileTime(u_mtime_t, &l_mtime_ft, TRUE) &&
            LocalFileTimeToFileTime(&l_atime_ft, &u_atime_ft)       &&
            LocalFileTimeToFileTime(&l_mtime_ft, &u_mtime_ft))
        {
            if (!SetFileTime(hndl, NULL, &u_atime_ft, &u_mtime_ft)) {
                ret = FALSE;
            }
            else {
                ret = TRUE;
            }
        }
        else {
            ret = FALSE;
        }
    }

    if (fd >= 0)
        close(fd);
    return ret;
}
