/***************************************************************************
 *     Copyright (c) 2008-2008, NCSoft
 *     All Rights Reserved
 *     Confidential Property of NCSoft
 *
 * Module Description:
 *
 *
 ***************************************************************************/
#include "utilitieslib/stdtypes.h"
#include "utilitieslib/UtilsNew/ncstd.h"
#include "utilitieslib/UtilsNew/Array.h"
#include "utilitieslib/UtilsNew/ncHash.h"


#define TYPE_T void*
#define TYPE_FUNC_PREFIX ap
#include "Array_def.c"
#undef TYPE_T
#undef TYPE_FUNC_PREFIX

#define TYPE_T char
#define TYPE_FUNC_PREFIX achr
#include "Array_def.c"
#undef TYPE_T
#undef TYPE_FUNC_PREFIX

#define TYPE_T int
#define TYPE_FUNC_PREFIX aint
#include "Array_def.c"
#undef TYPE_T
#undef TYPE_FUNC_PREFIX
