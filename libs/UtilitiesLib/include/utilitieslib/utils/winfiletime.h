#ifndef _WINFILETIME_H
#define _WINFILETIME_H

int _AltStat(const char *name, struct _stat32 *st_buf);

int _SetUTCFileTimes(
    const char      *name,
    const __time32_t    u_atime_t,
    const __time32_t    u_mtime_t);

int _GetUTCFileTimes(
    const char                  *name,
    __time32_t                      *u_atime_t,
    __time32_t                      *u_mtime_t,
    __time32_t                      *u_ctime_t);

BOOL _FileTimeToUnixTime(
    const FILETIME  *ft,
    __time32_t          *ut,
    const BOOL      ft_is_local);

BOOL _UnixTimeToFileTime(
    const __time32_t    ut,
    FILETIME        *ft,
    const BOOL      make_ft_local);

#endif
