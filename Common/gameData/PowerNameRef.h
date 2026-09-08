/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#ifndef POWERNAMEREF_H__
#define POWERNAMEREF_H__

#include <utilitieslib/utils/textparser_types.h>


// We use this structure to load references to Powers from data files.
typedef struct PowerNameRef
{
    const char* powerCategory;
    const char* powerSet;
    const char* power;
    int level;
    int deflevel;   // used for rewarding only: sets the level delta or the actual level depending on fixedlevel (below)
    int remove;     // used for rewarding only: if true, removes the named power.
    int fixedlevel; // used for rewarding only: if true, deflevel is a fixed reward level to give
    int dontSetStance;
} PowerNameRef;

extern TokenizerParseInfo ParsePowerNameRef[];


#endif /* #ifndef POWERNAMEREF_H__ */

/* End of File */

