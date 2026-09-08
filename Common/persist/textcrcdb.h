/***************************************************************************
 *     Copyright (c) 2006-2007, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 *
 *    To use these functions, the struct you are persisting should include
 *        two U32 members, one to store the CRC of the ParseTable (ver), the
 *        other to store the CRC of the data (crc)
 *    All of these functions return true on success and false on failure
 *    TODO: make this process more automated
 *
 ***************************************************************************/

#ifndef TEXTCRCDB_H
#define TEXTCRCDB_H

#include <utilitieslib/stdtypes.h>

typedef struct ParseTable ParseTable;

// writes the struct to fn.new
bool TextCRCDb_WriteNewFile(char *fn, ParseTable *pti, U32 *ver, U32 *crc, void *structptr);

// this pair does the same thing as WriteNewFile, but split into thread-safe/unsafe parts
bool TextCRCDb_WriteEString(char **estr_dst, ParseTable *pti, U32 *ver, U32 *crc, void *structptr); // thread-unsafe
bool TextCRCDb_WriteNewFileFromEString(char *fn, char *estr_src); // thread-safe (simple file write)

// loads fn.new, verifies CRCs, then clears structptr.  returns the same value as LoadFile below.
bool TextCRCDb_ValidateNewFile(char *fn, ParseTable *pti, U32 *ver, U32 *crc, void *structptr);

// moves fn.new to fn, optionally archiving fn
bool TextCRCDb_MoveNewFileToCur(char *fn, bool exp_archive, bool force);

// returns false on invalid text or data CRC mismatch
// returns true if fn does not exist, ParseTable CRC mismatch, or successful load
// on a ParseTable CRC mismatch, ver and crc will be set to 0
bool TextCRCDb_LoadFile(char *fn, ParseTable *pti, U32 *ver, U32 *crc, void *structptr);

#endif //TEXTCRCDB_H
