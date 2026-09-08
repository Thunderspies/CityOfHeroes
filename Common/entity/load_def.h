/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef LOAD_DEF_H__
#define LOAD_DEF_H__

typedef struct PowerDictionary PowerDictionary;
typedef struct CharacterOrigins CharacterOrigins;
typedef struct CharacterClasses CharacterClasses;
typedef struct BadgeDefs BadgeDefs;
typedef struct StashTableImp *StashTable;
typedef const struct StashTableImp *cStashTable;

void load_AllDefs(void);
void load_AllDefsReload(void);

#endif /* #ifndef LOAD_DEF_H__ */

/* End of File */

