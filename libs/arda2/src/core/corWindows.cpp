/*****************************************************************************
    created:    2001/04/23
    copyright:    2001, NCSoft. All Rights Reserved
    author(s):    Peter M. Freese
    
    purpose:    
*****************************************************************************/

#include "../../include/arda2/core/corFirst.h" // includes corWindows.h
#include <stdio.h>
#include <stdarg.h>

void corOutputDebugString(const char* format, ...)
{
  if (!format)
    return;

  char tmp[2048];
  // varargs magic
  va_list args;
  va_start(args, format);
  // print to the temporary buffer
  vsnprintf(tmp, sizeof(tmp), format, args);
  va_end(args);
  // finally output it
#if CORE_SYSTEM_WINAPI
  OutputDebugStringA(tmp);
#else
  // best approximation
  fputs(tmp, stderr);
  fputs("\n", stderr);
  fflush(stderr);
#endif
}

#if CORE_SYSTEM_WINAPI

void corOutputDebugString(const WCHAR* format, ...)
{
  if (!format)
    return;

  WCHAR tmp[512];
  // varargs magic
  va_list args;
  va_start(args, format);
  // print to the temporary buffer
  _vsnwprintf(tmp, sizeof(tmp), format, args);
  va_end(args);
  tmp[512 - 1] = 0;
  // finally output it
#if CORE_SYSTEM_WINAPI
  OutputDebugStringW(tmp);
#else
  // best approximation
  fputws(tmp, stderr);
  fputws(L"\n", stderr);
  fflush(stderr);
#endif
}

#endif
