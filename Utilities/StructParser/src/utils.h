#pragma once

#include <cstdio>

/* Open a borrowed non-null filename/mode. Write streams are staged outputs;
 * close the returned owned stream with CloseFile. Failures throw or exit.
 */
FILE *fopen_nofail(const char *name, const char *mode);
// Test a borrowed non-null path for a regular file; filesystem errors throw.
bool FileExists(const char *path);
