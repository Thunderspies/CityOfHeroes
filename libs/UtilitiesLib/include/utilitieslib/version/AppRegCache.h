// Shared between game, mapserver, and patch client.
#ifndef APPREGCACHE_H
#define APPREGCACHE_H

#include <stddef.h>

extern char app_registry_name[128];


void        regSetAppName(const char* appName);
const char* regGetAppName(void);
void        regRefresh(void);
const char* regGetCurrentVersion(const char* projectName);
const char* regGetLastVersion(const char* projectName);
void        regSetInstallationDir(const char* installDir);
const char* regGetInstallationDir(void);
const char* regGetPatchValue(void);
const char* regGetPatchBandwidth(void);
const char* regGetAppKey(void);

char*        regGetAppString(const char *key, const char *deflt, char *dest, size_t dest_size);
void        regPutAppString(const char *key, const char *value);
int            regGetAppInt(const char *key, int deflt);
void        regPutAppInt(const char *key, int value);

#endif