#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/SuperAssert.h"

void FindAutoStructBitField(char *pStruct, int iStructSize, TokenizerParseInfo *pTPI, int iBitHandle)
{
    int iByte, iBit;

    for (iByte = 0; iByte < iStructSize; iByte++)
    {
        if (pStruct[iByte])
        {
            for (iBit = 0; iBit < 8; iBit++)
            {
                if (pStruct[iByte] & ( 1 << iBit))
                {
                    int i;

                    FORALL_PARSETABLE(pTPI, i)
                    {
                        if (TOK_GET_TYPE(pTPI[i].type) == TOK_BIT && (int)(pTPI[i].storeoffset) == iBitHandle)
                        {
                            pTPI[i].storeoffset = iByte;
                            pTPI[i].param = iBit;
                        }
                    }

                    return;
                }
            }
        }
    }

    assertmsg(0, "Couldn't find bitfield bit");
}



