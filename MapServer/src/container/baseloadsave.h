#ifndef _BASELOADSAVE_H
#define _BASELOADSAVE_H

#include "container/dbcontainerpack.h"

extern struct StructDesc base_desc[];

char *baseLoadText(int supergroupid,int userid);
void baseSaveText(int supergroup_id,int user_id,char *text);

#if !DBQUERY
char *baseLoadMapFromDb(int supergroupid,int userid);
void baseSaveMapToDb(void);
#endif

#endif
