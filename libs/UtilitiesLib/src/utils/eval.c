/***************************************************************************
 *     Copyright (c) 2004-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "utilitieslib/assert/assert.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/components/earray.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/utils/mathutil.h"  // for rule30Float
#include "utilitieslib/utils/timing.h"    // for timergetSecondsSince20000FromString

#include "utilitieslib/utils/eval.h"

typedef enum CellType
{
    kCellType_Void,
    kCellType_Int,
    kCellType_String,
    kCellType_Float,
} CellType;

typedef void (*EvalFunc)(EvalContext *pcontext);

typedef struct FuncDesc
{
    const char *pchName;
    EvalFunc pfn;
    int iNumIn;
    int iNumOut;
    int reregistered;
} FuncDesc;


/***************************************************************************/
/* Helper functions */
/***************************************************************************/

/**********************************************************************func*
 * FindFuncDesc
 *
 */
static FuncDesc *FindFuncDesc(EvalContext *pcontext, const char *pchName)
{
    FuncDesc *pdesc;

    if(stashFindPointer(pcontext->hashFuncs, pchName, &pdesc))
        return pdesc;

    return NULL;
}

/**********************************************************************func*
 * FindFunc
 *
 */
static EvalFunc FindFunc(EvalContext *pcontext, const char *pchName)
{
    FuncDesc *pdesc = FindFuncDesc(pcontext, pchName);

    if(pdesc)
    {
        return pdesc->pfn;
    }

    return NULL;
}

/**********************************************************************func*
 * eval_Dump
 *
 */
void eval_Dump(EvalContext *pcontext)
{
    int i;
    for(i=eaSize(&pcontext->piVals)-1; i>=0; i--)
    {
        int itype = pcontext->piTypes[i];
        switch(itype)
        {
            case kCellType_Int:
            {
                printf("    %2d: int: %d\n", i, PTR_TO_S32(pcontext->piVals[i]));
                break;
            }

            case kCellType_String:
            {
                printf("    %2d: str: <%s>\n", i, pcontext->piVals[i]);
                break;
            }

            case kCellType_Float:
            {
                printf("    %2d: flt: %f\n", i, PTR_TO_F32(pcontext->piVals[i]));
                break;
            }

            default:
            {
                printf("    %2d: ???: <%08x>\n", i, PTR_TO_S32(pcontext->piVals[i]));
                break;
            }
        }
    }
}

/***************************************************************************/
/* Stack functions */
/***************************************************************************/

bool eval_StackIsEmpty(EvalContext *pcontext)
{
    return eaiSize(&pcontext->piTypes)<=0;
}

/**********************************************************************func*
 * ToNum
 *
 */
void ToNum(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;

    if(n>=0)
    {
        if(pcontext->piTypes[n]!=kCellType_Int
            && pcontext->piTypes[n]!=kCellType_Float)
        {
            EvalFunc pfn;

            if(pcontext->bDebug) 
            {
                printf("  (convert to number)\n");
                printf("  {\n");
            }

            if(pfn = FindFunc(pcontext, "num>"))
            {
                if(pcontext->bDebug) printf("    (num>)\n");
                pfn(pcontext);
            }
            else if(pfn = FindFunc(pcontext, "int>"))
            {
                if(pcontext->bDebug) printf("    (int>)\n");
                pfn(pcontext);
            }
            else if(pfn = FindFunc(pcontext, "float>"))
            {
                if(pcontext->bDebug) printf("    (float>)\n");
                pfn(pcontext);
            }

            if(pcontext->bDebug) 
            {
                printf("    Result:\n");
                eval_Dump(pcontext);
                printf("  }\n");
            }
        }
    }
}

/**********************************************************************func*
 * ToInt
 *
 */
void ToInt(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;

    if(n>=0)
    {
        ToNum(pcontext);

        switch(pcontext->piTypes[n])
        {
            case kCellType_Int:
                break;

            case kCellType_Float:
                {
                    const char* sval = eaPopConst(&pcontext->piVals);
                    float fval = PTR_TO_F32(sval);
                    eaiPop(&pcontext->piTypes);

                    eval_IntPush(pcontext, (int)fval);
                }
                break;
        }
    }
}

/**********************************************************************func*
 * ToFloat
 *
 */
void ToFloat(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;

    if(n>=0)
    {
        ToNum(pcontext);

        switch(pcontext->piTypes[n])
        {
            case kCellType_Int:
                {
                    int ival = PTR_TO_S32(eaPopConst(&pcontext->piVals));
                    eaiPop(&pcontext->piTypes);

                    eval_FloatPush(pcontext, (float)ival);
                }
                break;

            case kCellType_Float:
                break;
        }
    }
}

/**********************************************************************func*
 * ToDate
 *
 */
void ToDate(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;
    if(n>=0)
    {
        U32 usecs = 0;

        int itype = eaiPop(&pcontext->piTypes);
        switch(itype)
        {
            case kCellType_String:
                {
                    const char *pch = eaPopConst(&pcontext->piVals);
                    usecs = timerGetSecondsSince2000FromString(pch);

                    eval_IntPush(pcontext, usecs);
                }
                break;

            case kCellType_Float:
            case kCellType_Int:
                eaiPush(&pcontext->piTypes, itype);
                break;
        }
    }
}

/**********************************************************************func*
 * eval_IntPop
 *
 */
int eval_IntPop(EvalContext *pcontext)
{
    int itype;
    const char *pch;
    int ival;

    ToInt(pcontext);

    itype = eaiPop(&pcontext->piTypes); // returns 0 if empty array
    pch = eaPopConst(&pcontext->piVals);
    ival = PTR_TO_S32(pch);

    if (itype == kCellType_Void)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a whole number but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (itype == kCellType_String)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a whole number but found the word '%s'. It found this while analyzing '%s'.", pch, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (itype == kCellType_Float)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a whole number but found the decimal number '%f'. It found this while analyzing '%s'.", PTR_TO_F32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    return ival;
}

/**********************************************************************func*
 * eval_IntPush
 *
 */
void eval_IntPush(EvalContext *pcontext, int i)
{
    eaiPush(&pcontext->piTypes, kCellType_Int);
    eaPushConst(&pcontext->piVals, S32_TO_PTR(i));
}

/**********************************************************************func*
 * eval_IntPeek
 *
 */
int eval_IntPeek(EvalContext *pcontext)
{
    const char *pch;
    int ival = 0;
    int n;

    ToInt(pcontext);

    n = eaiSize(&pcontext->piTypes) - 1;
    if (n >= 0)
    {
        int itype = pcontext->piTypes[n];
        pch = pcontext->piVals[n];
        ival = PTR_TO_S32(pch);

        if (itype == kCellType_Void)
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a whole number but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
        else if (itype == kCellType_String)
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a whole number but found the word '%s'. It found this while analyzing '%s'.", pch, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
        else if (itype == kCellType_Float)
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a whole number but found the decimal number '%f'. It found this while analyzing '%s'.", PTR_TO_F32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a whole number but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    return ival;
}

/**********************************************************************func*
 * eval_StringPop
 *
 */
const char *eval_StringPop(EvalContext *pcontext)
{
    int itype = eaiPop(&pcontext->piTypes);
    const char *pch = eaPopConst(&pcontext->piVals);

    switch(itype)
    {
        case kCellType_String:
            break;

        case kCellType_Int:
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a word but found the whole number '%i'. It found this while analyzing '%s'.", PTR_TO_S32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
            pch = "*integer*";
            break;
        }
        case kCellType_Float:
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a word but found the decimal number '%f'. It found this while analyzing '%s'.", PTR_TO_F32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
            pch = "*float*";
            break;
        }
        case kCellType_Void:
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a word but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
            pch = "*empty stack*";
            break;
        }
    }

    if(!pch)
        pch="*null*";

    return pch;
}

/**********************************************************************func*
 * eval_StringPush
 *
 */
void eval_StringPush(EvalContext *pcontext, const char *pch)
{
    eaiPush(&pcontext->piTypes, kCellType_String);
    eaPushConst(&pcontext->piVals, pch);
}

/**********************************************************************func*
 * eval_StringPeek
 *
 */
const char *eval_StringPeek(EvalContext *pcontext)
{
    const char *pch = NULL;
    int n = eaiSize(&pcontext->piTypes) - 1;

    if (n >= 0)
    {
        int itype = pcontext->piTypes[n];
        const char *pch = pcontext->piVals[n];

        switch(itype)
        {
        case kCellType_String:
            break;

        case kCellType_Int:
            {
                estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a word but found the whole number '%i'. It found this while analyzing '%s'.", PTR_TO_S32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
                pch = "*integer*";
                break;
            }
        case kCellType_Float:
            {
                estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a word but found the decimal number '%f'. It found this while analyzing '%s'.", PTR_TO_F32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
                pch = "*float*";
                break;
            }
        case kCellType_Void:
            {
                estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a word but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
                pch = "*empty stack*";
                break;
            }
        }

        if(!pch)
            pch="*null*";
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a word but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        pch = "*empty stack*";
    }

    return pch;
}

/**********************************************************************func*
 * eval_BoolPop
 *
 */
bool eval_BoolPop(EvalContext *pcontext)
{
    const char *pch;
    bool bRet = false;
    int itype = eaiPop(&pcontext->piTypes);

    switch(itype)
    {
        case kCellType_Int:
            bRet = PTR_TO_S32(eaPopConst(&pcontext->piVals))!=0;
            break;

        case kCellType_Float:
            pch = eaPopConst(&pcontext->piVals);
            bRet = PTR_TO_F32(pch)!=0;
            break;

        case kCellType_String:
        {
            pch = eaPopConst(&pcontext->piVals);
            bRet = (pch && strlen(pch)>0);
            break;
        }

        default:
            eaPopConst(&pcontext->piVals);
            break;
    }

    return bRet;
}

/**********************************************************************func*
 * eval_BoolPush
 *
 */
void eval_BoolPush(EvalContext *pcontext, bool b)
{
    eval_IntPush(pcontext, b);
}

/**********************************************************************func*
 * eval_BoolPeek
 *
 */
bool eval_BoolPeek(EvalContext *pcontext)
{
    bool bRet = false;
    int n = eaiSize(&pcontext->piTypes)-1;

    if(n>=0)
    {
        int itype = pcontext->piTypes[n];

        switch(itype)
        {
            case kCellType_Int:
                bRet = PTR_TO_S32(pcontext->piVals[n])!=0;
                break;

            case kCellType_Float:
                bRet = PTR_TO_F32(pcontext->piVals[n]) != 0.0f;
                break;

            case kCellType_String:
            {
                const char *pch = pcontext->piVals[n];
                bRet = (pch && strlen(pch)>0);
                break;
            }
        }
    }

    return bRet;
}

/**********************************************************************func*
 * eval_FloatPop
 *
 */
float eval_FloatPop(EvalContext *pcontext)
{
    int itype;
    float fval;
    const char* pch;

    ToFloat(pcontext);

    itype = eaiPop(&pcontext->piTypes);
    pch = eaPopConst(&pcontext->piVals);
    fval = PTR_TO_F32(pch);

    if (itype == kCellType_Void)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a decimal number but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (itype == kCellType_Int)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a decimal number but found the whole number '%i'. It found this while analyzing '%s'.", PTR_TO_S32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }
    else if (itype == kCellType_String)
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a decimal number but found the word '%s'. It found this while analyzing '%s'.", pch, pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    return fval;
}

/**********************************************************************func*
 * eval_FloatPush
 *
 */
void eval_FloatPush(EvalContext *pcontext, float f)
{
    char* pch;
    eaiPush(&pcontext->piTypes, kCellType_Float);
    PTR_ASSIGNFROM_F32(pch, f);
    eaPushConst(&pcontext->piVals, pch);
}

/**********************************************************************func*
 * eval_FloatPeek
 *
 */
float eval_FloatPeek(EvalContext *pcontext)
{
    float fval = 0.0f;
    const char* pch;
    int n;

    ToFloat(pcontext);

    n = eaiSize(&pcontext->piTypes) - 1;
    if (n >= 0)
    {
        int itype = pcontext->piTypes[n];
        pch = pcontext->piVals[n];
        fval = PTR_TO_F32(pch);

        if (itype == kCellType_Void)
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a decimal number but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
        else if (itype == kCellType_Int)
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a decimal number but found the whole number '%i'. It found this while analyzing '%s'.", PTR_TO_S32(pch), pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
        else if (itype == kCellType_String)
        {
            estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a decimal number but found the word '%s'. It found this while analyzing '%s'.", pch, pcontext->ppchExpr[pcontext->iCurToken - 1]);
        }
    }
    else
    {
        estrConcatf(&pcontext->ppchError, "\nThe evaluator expected a decimal number but found nothing. It found this while analyzing '%s'.", pcontext->ppchExpr[pcontext->iCurToken - 1]);
    }

    return fval;
}

/**********************************************************************func*
 * eval_Dup
 *
 */
void eval_Dup(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;
    if(n>=0)
    {
        eaiPush(&pcontext->piTypes, pcontext->piTypes[n]);
        eaPushConst(&pcontext->piVals, pcontext->piVals[n]);
    }
}

/**********************************************************************func*
 * eval_Drop
 *
 */
void eval_Drop(EvalContext *pcontext)
{
    eaiPop(&pcontext->piTypes);
    eaPopConst(&pcontext->piVals);
}

/***************************************************************************/
/* Intrinsic functions */
/***************************************************************************/

static void LogicalAnd(EvalContext *pcontext)
{
    int rhs = eval_IntPop(pcontext);
    int lhs = eval_IntPop(pcontext);
    eval_IntPush(pcontext, lhs&&rhs);
}

static void LogicalOr(EvalContext *pcontext)
{
    int rhs = eval_IntPop(pcontext);
    int lhs = eval_IntPop(pcontext);
    eval_IntPush(pcontext, lhs||rhs);
}

static void LogicalNot(EvalContext *pcontext)
{
    int lhs = eval_IntPop(pcontext);
    eval_IntPush(pcontext, !lhs);
}

#define NUMERIC_OP(type, op) \
{                                                      \
    int n = eaiSize(&pcontext->piTypes)-1;    \
    if(n>=1                                            \
        && (pcontext->piTypes[n]!=kCellType_Int        \
            || pcontext->piTypes[n-1]!=kCellType_Int)) \
    {                                                  \
        float rhs = eval_FloatPop(pcontext);           \
        float lhs = eval_FloatPop(pcontext);           \
        eval_##type##Push(pcontext, op);       \
    }                                                  \
    else                                               \
    {                                                  \
        int rhs = eval_IntPop(pcontext);               \
        int lhs = eval_IntPop(pcontext);               \
        eval_IntPush(pcontext, op);            \
    }                                                  \
}


static void NumericGreater(EvalContext *pcontext)
{
    NUMERIC_OP(Int, lhs > rhs);
}

static void NumericGreaterEqual(EvalContext *pcontext)
{
    NUMERIC_OP(Int, lhs >= rhs);
}

static void NumericLess(EvalContext *pcontext)
{
    NUMERIC_OP(Int, lhs < rhs );
}

static void NumericLessEqual(EvalContext *pcontext)
{
    NUMERIC_OP(Int, lhs <= rhs);
}

static void NumericAdd(EvalContext *pcontext)
{
    NUMERIC_OP(Float, lhs + rhs);
}

static void NumericSubtract(EvalContext *pcontext)
{
    NUMERIC_OP(Float, lhs - rhs);
}

static void NumericMultiply(EvalContext *pcontext)
{
    NUMERIC_OP(Float, lhs * rhs);
}

static void StringEqual(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;
    if(n>=1)
    {
        if(pcontext->piTypes[n]==kCellType_String
            && pcontext->piTypes[n-1]==kCellType_String)
        {
            const char *rhs = eval_StringPop(pcontext);
            const char *lhs = eval_StringPop(pcontext);
            eval_IntPush(pcontext, stricmp(lhs, rhs)==0);
            return;
        } else {
            if(pcontext->bDebug) 
                printf("Found an eq comparing two non-strings.\n");
            eval_BoolPop(pcontext);
            eval_BoolPop(pcontext);
            eval_IntPush(pcontext, 0);
        }
    }
}

static void NumericEqual(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;
    if(n>=1)
    {
        if(pcontext->piTypes[n]==kCellType_String
            && pcontext->piTypes[n-1]==kCellType_String)
        {
            // The user probably meant to use string equality. Help them out.
            if(pcontext->bDebug) printf("Found a == comparing two strings. Using eq instead.\n");
            StringEqual(pcontext);
            return;
        }
    }

    // Finally test for numeric equality
    NUMERIC_OP(Int, lhs==rhs);
}

static void NumericDivide(EvalContext *pcontext)
{
    float rhs = eval_FloatPop(pcontext);
    float lhs = eval_FloatPop(pcontext);

    if(rhs==0)
        eval_FloatPush(pcontext, 0);
    else
        eval_FloatPush(pcontext, lhs / rhs);
}

static void NumericModulo(EvalContext *pcontext)
{
    int rhs = eval_IntPop(pcontext);
    int lhs = eval_IntPop(pcontext);

    if(rhs==0)
        eval_IntPush(pcontext, 0);
    else
        eval_IntPush(pcontext, lhs % rhs);
}

static void NumericPower(EvalContext *pcontext)
{
    float rhs = eval_FloatPop(pcontext);
    float lhs = eval_FloatPop(pcontext);
    eval_FloatPush(pcontext, powf(lhs, rhs));
}

static void NumericMinMax(EvalContext *pcontext)
{
    float max = eval_FloatPop(pcontext);
    float min = eval_FloatPop(pcontext);
    float val = eval_FloatPop(pcontext);

    if(val<min)
        val=min;
    else if(val>max)
        val=max;

    eval_FloatPush(pcontext, val);
}

static void NumericNegate(EvalContext *pcontext)
{
    int n = eaiSize(&pcontext->piTypes)-1;
    if(n>=0)
    {
        if(pcontext->piTypes[n]==kCellType_Int)
        {
            int i = eval_IntPop(pcontext);
            eval_IntPush(pcontext, -i);
        }
        else
        {
            float f = eval_FloatPop(pcontext);
            eval_FloatPush(pcontext, -f);
        }
    }
    else
    {
        eval_FloatPush(pcontext, 0.0f);
    }
}

static void Random(EvalContext *pcontext)
{
    float fRand = rule30Float();
    if (fRand < 0.0f) fRand *= -1;
    eval_FloatPush(pcontext, fRand);
}

static void Store(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);

    int itype = eaiPop(&pcontext->piTypes);
    switch(itype)
    {
        case kCellType_Int:
        {
            int i = PTR_TO_S32(eaPopConst(&pcontext->piVals));
            eval_StoreInt(pcontext, rhs, i);
            break;
        }

        case kCellType_Float:
        {
            const char* pch = eaPopConst(&pcontext->piVals);
            float f = PTR_TO_F32(pch);
            eval_StoreFloat(pcontext, rhs, f);
            break;
        }

        case kCellType_String:
        {
            const char *pch = eaPopConst(&pcontext->piVals);
            eval_StoreString(pcontext, rhs, pch);
            break;
        }

        default:
            eaPopConst(&pcontext->piVals);
            break;
    }
}

static void StoreAdd(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);

    int itype = eaiPop(&pcontext->piTypes);
    switch(itype)
    {
        case kCellType_Int:
        {
            int cur;
            int i = PTR_TO_S32(eaPopConst(&pcontext->piVals));
            eval_FetchInt(pcontext, rhs, &cur);
            eval_StoreInt(pcontext, rhs, cur+i);
            break;
        }

        case kCellType_Float:
        {
            const char* pch = eaPopConst(&pcontext->piVals);
            float cur;
            float f = PTR_TO_F32(pch);
            eval_FetchFloat(pcontext, rhs, &cur);
            eval_StoreFloat(pcontext, rhs, cur+f);
            break;
        }

        case kCellType_String:
        {
            const char *pch = eaPopConst(&pcontext->piVals);
            eval_StoreString(pcontext, rhs, pch);
            break;
        }

        default:
            eaPopConst(&pcontext->piVals);
            break;
    }
}

static void FetchString(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
	const char *pch;

    if(eval_FetchString(pcontext, rhs, &pch))
    {
        eval_StringPush(pcontext, pch);
    }
    else
    {
        eval_StringPush(pcontext, rhs);
    }
}

static void FetchInt(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    int i;

    if(eval_FetchInt(pcontext, rhs, &i))
    {
        eval_IntPush(pcontext, i);
    }
    else
    {
        eval_IntPush(pcontext, 0);
    }
}

static void FetchFloat(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    float f;

    if(eval_FetchFloat(pcontext, rhs, &f))
    {
        eval_FloatPush(pcontext, f);
    }
    else
    {
        eval_FloatPush(pcontext, 0);
    }
}

static void Fetch(EvalContext *pcontext)
{
    const char *rhs = eval_StringPop(pcontext);
    int i;

    if(eval_FetchInt(pcontext, rhs, &i))
    {
        eval_IntPush(pcontext, i);
    }
    else
    {
        float f;
        if(eval_FetchFloat(pcontext, rhs, &f))
        {
            eval_FloatPush(pcontext, f);
        }
        else
        {
			const char *pch;

            if(eval_FetchString(pcontext, rhs, &pch))
            {
                eval_StringPush(pcontext, pch);
            }
            else
            {
                if(pcontext->bDebug)
                    printf("no var found. just pushing %s\n");
                eval_StringPush(pcontext, rhs);
            }
        }
    }
}

static void Forget(EvalContext *pcontext)
{
    eval_StringPop(pcontext);
}

static void Conditional(EvalContext *pcontext)
{
    bool bIsTrue = eval_BoolPop(pcontext);

    if(!bIsTrue)
    {
        // iCurToken is pre-incremented past the Conditional already.
        while(pcontext->iCurToken < pcontext->iNumTokens)
        {
            if(stricmp(pcontext->ppchExpr[pcontext->iCurToken], "endif")==0)
            {
                break;
            }
            pcontext->iCurToken++;
        }
    }
}

static void Goto(EvalContext *pcontext)
{
    int i = 0;
    const char *rhs = eval_StringPop(pcontext);

    while(i < pcontext->iNumTokens)
    {
        if(stricmp(pcontext->ppchExpr[i], rhs)==0)
        {
            pcontext->iCurToken = i;
            break;
        }
        i++;
    }
}

static void ConditionalGoto(EvalContext *pcontext)
{
    int i = 0;
    const char *pchLabel = eval_StringPop(pcontext);
    bool bIsTrue = eval_BoolPop(pcontext);

    // iCurToken is pre-incremented past the Conditional already.
    if(bIsTrue)
    {
        while(i < pcontext->iNumTokens)
        {
            if(stricmp(pcontext->ppchExpr[i], pchLabel)==0)
            {
                pcontext->iCurToken = i;
                break;
            }
            i++;
        }
    }
}

static void Noop(EvalContext *pcontext)
{
    return;
}

static FuncDesc s_FuncTable[] =
{
    { "&&",     LogicalAnd          , 2, 1},
    { "||",     LogicalOr           , 2, 1},
    { "!",      LogicalNot          , 1, 1},
    { ">",      NumericGreater      , 2, 1},
    { "<",      NumericLess         , 2, 1},
    { ">=",     NumericGreaterEqual , 2, 1},
    { "<=",     NumericLessEqual    , 2, 1},
    { "==",     NumericEqual        , 2, 1},

    { "eq",     StringEqual         , 2, 1},

    { "+",      NumericAdd          , 2, 1},
    { "*",      NumericMultiply     , 2, 1},
    { "/",      NumericDivide       , 2, 1},
    { "%",      NumericModulo       , 2, 1},
    { "-",      NumericSubtract     , 2, 1},
    { "pow",    NumericPower        , 2, 1},

    { "negate", NumericNegate       , 1, 1},
    { "rand",   Random              , 0, 1},
    { "minmax", NumericMinMax       , 3, 1},

    { "date>",  ToDate              , 1, 1},

    { ">var",   Store               , 2, 0},
    { ">var+",  StoreAdd            , 2, 0},
    { "var>",   Fetch               , 1, 1},
    { "auto>",  Fetch               , 1, 1},
    { "forget", Forget              , 1, 0},

    { "dup",    eval_Dup            , 1, 2},
    { "drop",   eval_Drop           , 1, 0},

    { "if",     Noop                , 0, 0},
    { "then",   Conditional         , 1, 0},
    { "endif",  Noop                , 0, 0},

    { "label",  eval_Drop           , 1, 0},
    { ":",      eval_Drop           , 1, 0},
    { "goto",   Goto                , 1, 0},
    { "gotoif", ConditionalGoto     , 2, 0},
};

/***************************************************************************/
/* External variable store and fetch */
/***************************************************************************/

void eval_StorePointer(EvalContext *pcontext, const char *pchName, void *pVal)
{
    stashAddPointer(pcontext->hashPointers, pchName, pVal, true);
}

bool eval_FetchPointer(EvalContext *pcontext, const char *pchName, void **ppVal)
{
    return stashFindPointer(pcontext->hashPointers, pchName, ppVal);
}

void eval_ForgetPointer(EvalContext *pcontext, const char *pchName)
{
    stashRemovePointer(pcontext->hashPointers, pchName, NULL);
}

/**********************************************************************func*
 * eval_StoreString
 *
 */
void eval_StoreString(EvalContext *pcontext, const char *pchName, const char *pchVal)
{
    stashAddPointerConst(pcontext->hashStrings, pchName, pchVal, true);
}

/**********************************************************************func*
 * eval_FetchString
 *
 */
bool eval_FetchString(EvalContext *pcontext, const char *pchName, const char **pchVal)
{
    return stashFindPointerConst(pcontext->hashStrings, pchName, pchVal);
}

/**********************************************************************func*
 * eval_ForgetString
 *
 */
void eval_ForgetString(EvalContext *pcontext, const char *pchName)
{
    stashRemovePointer(pcontext->hashStrings, pchName, NULL);
}

/**********************************************************************func*
 * eval_StoreInt
 *
 */
void eval_StoreInt(EvalContext *pcontext, const char *pchName, int iVal)
{
    stashAddInt(pcontext->hashInts, pchName, iVal, true);
}

/**********************************************************************func*
 * eval_FetchInt
 *
 */
bool eval_FetchInt(EvalContext *pcontext, const char *pchName, int *piVal)
{
    return stashFindInt(pcontext->hashInts, pchName, piVal);
}

/**********************************************************************func*
 * eval_ForgetInt
 *
 */
void eval_ForgetInt(EvalContext *pcontext, const char *pchName)
{
    stashRemoveInt(pcontext->hashInts, pchName, NULL);
}

/**********************************************************************func*
 * eval_StoreFloat
 *
 */
void eval_StoreFloat(EvalContext *pcontext, const char *pchName, float fVal)
{
    stashAddInt(pcontext->hashFloats, pchName, *(int *)&fVal, true);
}

/**********************************************************************func*
 * eval_FetchFloat
 *
 */
bool eval_FetchFloat(EvalContext *pcontext, const char *pchName, float *pfVal)
{
    int iVal;
    bool b = stashFindInt(pcontext->hashFloats, pchName, &iVal);
    
    *pfVal = *(float *)&iVal;
    return b;
}

/**********************************************************************func*
 * eval_ForgetFloat
 *
 */
void eval_ForgetFloat(EvalContext *pcontext, const char *pchName)
{
    stashRemoveInt(pcontext->hashFloats, pchName, NULL);
}

/***************************************************************************/
/* EvalContext create/destruct/config */
/***************************************************************************/

static void eval_RegisterFuncEx(EvalContext *pcontext, const char *pchName, EvalFunc pfn,
    int iNumIn, int iNumOut, int reregister)
{
    bool added;

    FuncDesc *pdesc = calloc(sizeof(FuncDesc), 1);
    pdesc->pchName = pchName;
    pdesc->pfn = pfn;
    pdesc->iNumIn = iNumIn;
    pdesc->iNumOut = iNumOut;
    pdesc->reregistered = reregister;

    added = stashAddPointer(pcontext->hashFuncs, pchName, pdesc, false);
    if (!devassertmsg(added, "Do you mean to call eval_ReRegisterFunc?"))
    {
        eval_UnregisterFunc(pcontext, pchName);
        stashAddPointer(pcontext->hashFuncs, pchName, pdesc, true);
    }
}

static void eval_UnregisterFuncEx(EvalContext *pcontext, const char *pchName, int from_reregister_func)
{
    FuncDesc *pdesc;
    if (stashRemovePointer(pcontext->hashFuncs, pchName, &pdesc))
    {
        devassertmsg(!from_reregister_func || !pdesc->reregistered, "Tried to reregister a reregistered function.");
        free(pdesc);
    }
}

/**********************************************************************func*
 * eval_ReRegisterFunc
 *
 */
void eval_ReRegisterFunc(EvalContext *pcontext, const char *pchName, EvalFunc pfn, int iNumIn, int iNumOut)
{
    eval_UnregisterFuncEx(pcontext, pchName, 1);
    eval_RegisterFuncEx(pcontext, pchName, pfn, iNumIn, iNumOut, 1);
}

/**********************************************************************func*
 * eval_RegisterFunc
 *
 */
void eval_RegisterFunc(EvalContext *pcontext, const char *pchName, EvalFunc pfn,
    int iNumIn, int iNumOut)
{
    eval_RegisterFuncEx(pcontext, pchName, pfn, iNumIn, iNumOut, 0);
}


/**********************************************************************func*
 * eval_UnregisterFunc
 *
 */
void eval_UnregisterFunc(EvalContext *pcontext, const char *pchName)
{
    eval_UnregisterFuncEx(pcontext, pchName, 0);
}

/**********************************************************************func*
 * eval_SetDebug
 *
 */
void eval_SetDebug(EvalContext *pcontext, bool bDebug)
{
    pcontext->bDebug = bDebug;
}

/**********************************************************************func*
 * eval_ClearStack
 *
 */
void eval_ClearStack(EvalContext *pcontext)
{
    eaSetSizeConst(&pcontext->piVals, 0);
    eaiSetSize(&pcontext->piTypes, 0);
}

/**********************************************************************func*
 * eval_ClearVariables
 *
 */
void eval_ClearVariables(EvalContext *pcontext)
{
    stashTableClear(pcontext->hashInts);
    stashTableClear(pcontext->hashPointers);
    stashTableClear(pcontext->hashStrings);
    stashTableClear(pcontext->hashFloats);
}

/**********************************************************************func*
 * eval_Free
 *
 */
static void eval_Free(void* mem)
{
    free(mem);
}

/**********************************************************************func*
 * eval_ClearFuncs
 *
 */
void eval_ClearFuncs(EvalContext *pcontext)
{
    stashTableClearEx(pcontext->hashFuncs, NULL, eval_Free);
}

/**********************************************************************func*
 * eval_Init
 *
 */
void eval_Init(EvalContext *pcontext)
{
    int j;

    if(pcontext->piVals)
    {
        eval_ClearStack(pcontext);
        eval_ClearVariables(pcontext);
        eval_ClearFuncs(pcontext);
    }
    else
    {
        eaCreateConst(&pcontext->piVals);
        eaiCreate(&pcontext->piTypes);
        pcontext->hashInts = stashTableCreateWithStringKeys(16, StashDefault);
        pcontext->hashPointers = stashTableCreateWithStringKeys(16, StashDefault);
        pcontext->hashStrings = stashTableCreateWithStringKeys(16, StashDefault);
        pcontext->hashFloats = stashTableCreateWithStringKeys(16, StashDefault);
        pcontext->hashFuncs = stashTableCreateWithStringKeys(16, StashDefault);
    }


    for(j=0; j<ARRAY_SIZE(s_FuncTable); j++)
    {
        eval_RegisterFunc(pcontext, s_FuncTable[j].pchName, s_FuncTable[j].pfn,
            s_FuncTable[j].iNumIn, s_FuncTable[j].iNumOut);
    }

    pcontext->bVerifyStackSize = false;
}

/**********************************************************************func*
 * eval_Create
 *
 */
EvalContext *eval_Create(void)
{
    EvalContext *pcontext = calloc(sizeof(EvalContext), 1);

    eval_Init(pcontext);
    return pcontext;
}

/**********************************************************************func*
 * eval_Destroy
 *
 */
void eval_Destroy(EvalContext *pcontext)
{
    eaDestroyConst(&pcontext->piVals);
    eaiDestroy(&pcontext->piTypes);
    stashTableDestroy(pcontext->hashInts);
    stashTableDestroy(pcontext->hashFloats);
    stashTableDestroy(pcontext->hashPointers);
    stashTableDestroy(pcontext->hashStrings);
    stashTableDestroyEx(pcontext->hashFuncs, NULL, eval_Free);

    free(pcontext);
}

/***************************************************************************/
/* Expression handling */
/***************************************************************************/

/**********************************************************************func*
 * eval_Validate
 *
 */
bool eval_Validate(EvalContext *pcontext, const char *pchItem, const char * const *ppchExpr, char **ppchOut)
{
    int n, i;
    int iSize = 0;
    bool res = true;
    static char errorString[512];
    
    n = eaSize(&ppchExpr);
    if(n==0)
        return true;

    for(i=0; i<n; i++)
    {
        FuncDesc *pfn = FindFuncDesc(pcontext, ppchExpr[i]);
        if(pfn)
        {
            if(iSize<pfn->iNumIn)
            {
                snprintf(errorString,511,"Bad eval for %s: Not enough operands for %s!\n", pchItem, ppchExpr[i]);
                res = false;
                break;
            }
            iSize = iSize + pfn->iNumOut - pfn->iNumIn;
        }
        else
        {
            iSize++;
        }
    }

    if(iSize<1 && res)
    {
        snprintf(errorString,511,"Bad eval for %s: No result!\n", pchItem);
        res = false;
    }
    else if(iSize>1 && res)
    {
        snprintf(errorString,511,"Bad eval for %s: Multiple items (%d) left on stack! (probably means invalid function name)\n", pchItem, iSize);       
        res = false;
    }

    if (res)
    {
        if (ppchOut)
            *ppchOut = NULL;
        return true;
    }
    else
    {
        if (ppchOut)
            *ppchOut = errorString;
        else
            verbose_printf(errorString);

        return false;
    }
}

// ppchExpr is ONLY a char*[] here, NOT an earray!
bool eval_EvaluateEx(EvalContext *pcontext, const char * const *ppchExpr, int iNumTokens)
{
    bool returnMe;

    pcontext->ppchError = NULL; // assumes you cleaned up after yourself last iteration
    pcontext->bDone = false;
    pcontext->iCurToken = 0;
    pcontext->ppchExpr = ppchExpr;
    pcontext->iNumTokens = iNumTokens;

    if(pcontext->bDebug) printf("------\n");

    if(pcontext->iNumTokens==0) return true;

    while(pcontext->iCurToken < pcontext->iNumTokens && !pcontext->bDone)
    {
        EvalFunc pfn = NULL;
        EvalFunc pfnPost = NULL;
        const char *pchToken = ppchExpr[pcontext->iCurToken];

        if(pcontext->bDebug) printf("--> %s\n", pchToken);

        if(pchToken[0]=='@')
        {
            if(pcontext->bDebug) printf("  (var)\n");
            pfnPost = FindFunc(pcontext, "var>");
            pchToken++;
        }
        else if(pchToken[0]=='$')
        {
            if(pcontext->bDebug) printf("  (auto)\n");
            pfnPost = FindFunc(pcontext, "auto>");
            pchToken++;
        }
        else
        {
            pfn = FindFunc(pcontext, pchToken);
        }

        if(pfn)
        {
            if(pcontext->bDebug) printf("  (func)\n");
            pcontext->iCurToken++;
            pfn(pcontext);
        }
        else
        {
            char *pch;
            bool bCvt = false;

            pch = strchr(pchToken, '.');
            if(pch)
            {
                float f = (float)strtod(pchToken, &pch);
                if(pch>pchToken && *pch=='\0')
                {
                    // It seems to have converted to an float
                    if(pcontext->bDebug) printf("  (float)\n");
                    eval_FloatPush(pcontext, f);
                    bCvt = true;
                }
            }
            else
            {
                int j = strtol(pchToken, &pch, 10);
                if(pch>pchToken && *pch=='\0')
                {
                    // It seems to have converted to an integer
                    if(pcontext->bDebug) printf("  (int)\n");
                    eval_IntPush(pcontext, j);
                    bCvt = true;
                }
            }

            if(!bCvt)
            {
                // It didn't convert, put the string on the stack
                if(pcontext->bDebug) printf("  (string)\n");
                eval_StringPush(pcontext, pchToken);
            }

            pcontext->iCurToken++;
        }

        if(pfnPost)
        {
            if(pcontext->bDebug)
            {
                printf("  (post func)\n");
                printf("  {\n");
                printf("    stack on entry:\n");
                eval_Dump(pcontext);
            }

            pfnPost(pcontext);

            if(pcontext->bDebug) printf("  }\n");
        }

        if(pcontext->bDebug)
            eval_Dump(pcontext);

    }

    // This is done in order to force num> evaluation of the top item
    //   on the stack before return. This is used by the badges (at minimum)
    //   and probably any other evaluator which has an implicit num> (or int>).
    // TODO: Perhaps a better way to handle this is by splitting this function
    //   up.
    returnMe = (bool)eval_FloatPeek(pcontext);

    if (pcontext->bVerifyStackSize && eaSize(&pcontext->piVals) != 1)
    {
        estrConcatf(&pcontext->ppchError, "\nEvaluator found %d items on the stack after evaluating, expects 1.", eaSize(&pcontext->piVals));
    }

    if (pcontext->ppchError)
    {
        int i;
        char *pch;
        estrCreate(&pch);
        estrConcatf(&pch, "Error evaluating expression:\n");
        for (i = 0; i < pcontext->iNumTokens; i++)
        {
            estrConcatf(&pch, "%s ", ppchExpr[i]);
        }

        estrConcatf(&pch, "\n\nError messages:%s", pcontext->ppchError);

        if (pcontext->pchFileBlame)
        {
            ErrorFilenamef(pcontext->pchFileBlame, "%s", pch);
        }
        else
        { 
            Errorf(pch);
        }

        estrDestroy(&pch);
        estrDestroy(&pcontext->ppchError);
    }

    return returnMe;
}

bool eval_Evaluate(EvalContext *pcontext, const char * const *ppchExpr)
{
    return eval_EvaluateEx(pcontext, ppchExpr, eaSize(&ppchExpr));
}

static char **s_eaFilenamesForEvalBlame = NULL;

/**********************************************************************func*
 * eval_SetBlame
 * NOTE: does not intern string name. assumes it persists
 */
void eval_SetBlame(EvalContext *pcontext, const char *pchFileBlame)
{
    if( pcontext )
    {
        pcontext->pchFileBlame = pchFileBlame;
    }
}



#ifdef TEST_EVAL
int main(int argc, char *argv[])
{
    EvalContext *pcontext;
    char **ppch;
    int i;

    eaCreate(&ppch);
    for(i=1; i<argc; i++)
    {
        printf("%2d:<%s>\n", i-1, argv[i]);
        eaPush(&ppch, argv[i]);
    }

    pcontext = eval_Create();
    eval_SetDebug(pcontext, true);

    eval_Validate(pcontext, "cmdline", ppch,NULL);

    i = eval_Evaluate(pcontext, ppch);
    if(i)
        printf("--> true\n");
    else
        printf("--> false\n");

    eval_Destroy(pcontext);

    return 0;
}
#endif


/* End of File */
