/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <stdlib.h> // calloc
#include <string.h> // strncpy

#include "formatter/smf_util.h"
#include "formatter/smf_parse.h"

#include <utilitieslib/stdtypes.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/EString.h>

#include "UI/sprite/sprite_font.h"
#include <utilitieslib/utils/timing.h>

static int s_iTagWS = -1;
static int s_iTagText = -1;

// Global counter for preventing links with <nolink>
int g_preventLinks;

int sm_ParseTagName(char* tagName, int tagNameLength, SMTagDef aTagDefs[])
{
    // strnicmp() implicitly casts tagNameLength to size_t which turns -1
    // into 0xffffffff (32 bit) or 0xffffffffffffffff (64 bit). Unfortunately,
    // the currently used version of the MSVC CRT allows for param _MaxCount a
    // maximum value of _MaxCount <= INT_MAX so -1 will trigger an assertion.
    if (tagNameLength >= 0)
    {
        for (int iTag = 0; aTagDefs[iTag].pchName != NULL; ++iTag)
        {
            if (strnicmp(tagName, aTagDefs[iTag].pchName, tagNameLength) == 0 && (int)strlen(aTagDefs[iTag].pchName) == tagNameLength)
            {
                return iTag;
            }
        }
    }
    return -1;
}

// '%' is the only "not flagged code"
// DO NOT STRIP % CODE IN FLAGGED
static bool smf_ShouldOutputCharacterCodeToString(SMFBlock *pSMFBlock, char theCharacter)
{
    return !pSMFBlock || !(pSMFBlock->outputMode == SMFOutputMode_StripAllTagsAndCodes ||
                           (pSMFBlock->outputMode == SMFOutputMode_StripFlaggedTagsAndCodes && theCharacter != '%'));
}

static bool smf_ShouldOutputTagToString(SMFBlock *pSMFBlock, SMTagDef *tagDef)
{
    return pSMFBlock && (pSMFBlock->outputMode == SMFOutputMode_StripNoTags || 
                         (tagDef->outputToString && (pSMFBlock->outputMode == SMFOutputMode_StripFlaggedTags ||
                                                     pSMFBlock->outputMode == SMFOutputMode_StripFlaggedTagsAndCodes)));
}

/**********************************************************************func*
* TextPush
*
*/
static SMBlock *TextPush(SMBlock *pBlock, const unsigned char *pch, int iLen, int rawStartIndex, int *displayStartIndex, bool createIfEmpty)
{
    SMBlock *pnew = NULL;
    int i = 0;

    if(iLen<1)
    {
        if (createIfEmpty || smf_IsBlockEditable(pBlock->pSMFBlock))
        {
            // Adding in a bit of a hack to ensure that there is at least one sm_text block in every parsed tree structure.
            // I need this to exist for my selection/cursor code.
            pnew = sm_AppendNewBlock(pBlock);
            pnew->iType = s_iTagText;
            pnew->rawStringStartIndex = rawStartIndex;
            pnew->displayStringStartIndex = *displayStartIndex;
            // Since this is an empty text block, I don't need to increase displayStartIndex.
            return pnew;
        }
        else
        {
            return NULL;
        }
    }

    // Split the text string by whitespace.
    // Any whitespace is considered a word break and nothing more.
    while(i<iLen)
    {
        int iStart = i;

        while((pch[i]==' ' || pch[i]=='\r' || pch[i]=='\n' || pch[i]=='\t')
            && i<iLen)
        {
            i++;
        }

        // Whitespace isn't perfect. A series of tags with intervening spaces
        // and no text will make width calculations inaccurate. It shouldn't
        // be too bad, though.
        if(i!=iStart)
        {
            // Don't "only insert a whitespace if the previous block is a text block".

            if (pBlock->pSMFBlock->displayMode == SMFDisplayMode_AsterisksOnly)
            {
                pnew = sm_AppendNewBlockAndData(pBlock, 2);
                pnew->iType = s_iTagText;
                strncpy(pnew->pv, "*", 1);
            }
            else
            {
                pnew = sm_AppendNewBlock(pBlock);
                pnew->iType = s_iTagWS;
            }
            pnew->rawStringStartIndex = rawStartIndex + iStart;
            pnew->displayStringStartIndex = *displayStartIndex;
            (*displayStartIndex)++; // whitespace is only 1 width
            estrConcatChar(&pnew->pSMFBlock->outputString, ' ');
        }

        iStart=i;

        while(pch[i]!=' ' && pch[i]!='\r' && pch[i]!='\n' && pch[i]!='\t'
            && i<iLen)
        {
            i++;
        }

        if(i-iStart>0)
        {
            // We now have a chunk of non-whitespace raw text.
            // Break this chunk up by character codes.
            int i3 = iStart;
            while (i3 < i)
            {
                while (i3 < i && strnicmp(pch + i3, "&nbsp;", 6) &&
                                 strnicmp(pch + i3, "&lt;", 4) &&
                                 strnicmp(pch + i3, "&gt;", 4) &&
                                 strnicmp(pch + i3, "&amp;", 5) &&
                                 strnicmp(pch + i3, "&#37;", 5))
                {
                    i3++;
                }

                pnew = sm_AppendNewBlockAndData(pBlock, i3 - iStart + 1);
                pnew->iType = s_iTagText;
                pnew->rawStringStartIndex = rawStartIndex + iStart;
                pnew->displayStringStartIndex = *displayStartIndex;
                if (pBlock->pSMFBlock->displayMode == SMFDisplayMode_AsterisksOnly)
                {
                    char *temp;
                    int index;
                    estrCreate(&temp);
                    for (index = 0; index < i3 - iStart; index++)
                    {
                        estrConcatChar(&temp, '*');
                    }
                    strncpy(pnew->pv, temp, i3 - iStart);
                    estrDestroy(&temp);
                }
                else
                {
                    strncpy(pnew->pv, pch + iStart, i3 - iStart);
                }
                estrConcatFixedWidth(&pnew->pSMFBlock->outputString, pch + iStart, i3 - iStart);
                (*displayStartIndex) += smBlock_GetDisplayLength(pnew);

                if (!strnicmp(pch + i3, "&nbsp;", 6))
                {
                    if (pBlock->pSMFBlock->displayMode == SMFDisplayMode_AsterisksOnly)
                    {
                        pnew = sm_AppendNewBlockAndData(pBlock, 2);
                        pnew->iType = s_iTagText;
                        strncpy(pnew->pv, "*", 1);
                    }
                    else
                    {
                        pnew = sm_AppendNewBlock(pBlock);
                        pnew->iType = s_iTagWS;
                    }
                    pnew->rawStringStartIndex = rawStartIndex + i3;
                    pnew->displayStringStartIndex = *displayStartIndex;
                    (*displayStartIndex)++; // whitespace is only 1 width
                    if (smf_ShouldOutputCharacterCodeToString(pBlock->pSMFBlock, ' '))
                    {
                        estrConcatStaticCharArray(&pnew->pSMFBlock->outputString, "&nbsp;");
                    }
                    else
                    {
                        estrConcatChar(&pnew->pSMFBlock->outputString, ' ');
                    }

                    i3 += 6;
                    iStart = i3;
                }
                else if (!strnicmp(pch + i3, "&lt;", 4))
                {
                    pnew = sm_AppendNewBlockAndData(pBlock, 2);
                    pnew->iType = s_iTagText;
                    pnew->rawStringStartIndex = rawStartIndex + i3;
                    pnew->displayStringStartIndex = *displayStartIndex;
                    (*displayStartIndex) += 1;
                    if (pBlock->pSMFBlock->displayMode == SMFDisplayMode_AsterisksOnly)
                    {
                        strncpy(pnew->pv, "*", 1);
                    }
                    else
                    {
                        strncpy(pnew->pv, "<", 1);
                    }
                    if (smf_ShouldOutputCharacterCodeToString(pBlock->pSMFBlock, '<'))
                    {
                        estrConcatStaticCharArray(&pnew->pSMFBlock->outputString, "&lt;");
                    }
                    else
                    {
                        estrConcatChar(&pnew->pSMFBlock->outputString, '<');
                    }

                    i3 += 4;
                    iStart = i3;
                }
                else if (!strnicmp(pch + i3, "&gt;", 4))
                {
                    pnew = sm_AppendNewBlockAndData(pBlock, 2);
                    pnew->iType = s_iTagText;
                    pnew->rawStringStartIndex = rawStartIndex + i3;
                    pnew->displayStringStartIndex = *displayStartIndex;
                    (*displayStartIndex) += 1;
                    if (pBlock->pSMFBlock->displayMode == SMFDisplayMode_AsterisksOnly)
                    {
                        strncpy(pnew->pv, "*", 1);
                    }
                    else
                    {
                        strncpy(pnew->pv, ">", 1);
                    }
                    if (smf_ShouldOutputCharacterCodeToString(pBlock->pSMFBlock, '>'))
                    {
                        estrConcatStaticCharArray(&pnew->pSMFBlock->outputString, "&gt;");
                    }
                    else
                    {
                        estrConcatChar(&pnew->pSMFBlock->outputString, '>');
                    }

                    i3 += 4;
                    iStart = i3;
                }
                else if (!strnicmp(pch + i3, "&amp;", 5))
                {
                    pnew = sm_AppendNewBlockAndData(pBlock, 2);
                    pnew->iType = s_iTagText;
                    pnew->rawStringStartIndex = rawStartIndex + i3;
                    pnew->displayStringStartIndex = *displayStartIndex;
                    (*displayStartIndex) += 1;
                    if (pBlock->pSMFBlock->displayMode == SMFDisplayMode_AsterisksOnly)
                    {
                        strncpy(pnew->pv, "*", 1);
                    }
                    else
                    {
                        strncpy(pnew->pv, "&", 1);
                    }
                    if (smf_ShouldOutputCharacterCodeToString(pBlock->pSMFBlock, '&'))
                    {
                        estrConcatStaticCharArray(&pnew->pSMFBlock->outputString, "&amp;");
                    }
                    else
                    {
                        estrConcatChar(&pnew->pSMFBlock->outputString, '&');
                    }

                    i3 += 5;
                    iStart = i3;
                }
                else if (!strnicmp(pch + i3, "&#37;", 5))
                {
                    pnew = sm_AppendNewBlockAndData(pBlock, 2);
                    pnew->iType = s_iTagText;
                    pnew->rawStringStartIndex = rawStartIndex + i3;
                    pnew->displayStringStartIndex = *displayStartIndex;
                    (*displayStartIndex) += 1;
                    if (pBlock->pSMFBlock->displayMode == SMFDisplayMode_AsterisksOnly)
                    {
                        strncpy(pnew->pv, "*", 1);
                    }
                    else
                    {
                        strncpy(pnew->pv, "%", 1);
                    }
                    if (smf_ShouldOutputCharacterCodeToString(pBlock->pSMFBlock, '%'))
                    {
                        estrConcatStaticCharArray(&pnew->pSMFBlock->outputString, "&#37;");
                    }
                    else
                    {
                        estrConcatChar(&pnew->pSMFBlock->outputString, '%');
                    }

                    i3 += 5;
                    iStart = i3;
                }
            }
        }
    }

    return pnew;
}

/**********************************************************************func*
* sm_ParseInto
*
*/
SMBlock *sm_ParseInto(SMBlock *pBlock, char *pchOrig, SMTagDef aTagDefs[], SMTextProc pfnText)
{
    int i = 0;
    int k;
    SMBlock *pBlockTop = pBlock;
    int displayIndex = 0;

    int len;
    if (pchOrig)
    {
        len = (int) strlen(pchOrig);
    }
    else
    {
        len = 0;
    }

    if (pBlock->pSMFBlock && pBlock->pSMFBlock->outputString)
    {
        estrClear(&pBlock->pSMFBlock->outputString);
    }

    if (pfnText == 0)
    {
        pfnText = TextPush;
    }

    g_preventLinks = 0;
    while(i<len)
    {
        k = i;
        while(pchOrig[i]!='<' && i<len)
            i++;

        // skip multiple leading brackets
        while( (i+1)<len && pchOrig[i+1]=='<' )
            i++;

        pfnText(pBlock, pchOrig + k, i - k, k, &displayIndex, false);

        if(pchOrig[i]=='<')
        {
            int iTag;
            int tagNameLength;
            int tagStartIndex;

            // We've got a command
            k = i;
            tagStartIndex = i;
            while(pchOrig[i]!='>' && pchOrig[i]!=' ' && pchOrig[i]!='\r' && pchOrig[i]!='\n' && pchOrig[i]!='\t' && i<len)
            {
                i++;
            }

            // We're at the end of the string or command
            tagNameLength = i - k - 1;
            iTag = sm_ParseTagName(&pchOrig[k + 1], tagNameLength, aTagDefs);

            // if the alphanumeric string following the opening < isn't a tag OR
            // there wasn't an alphanumeric string following the opening < OR
            // there wasn't a closing > for the tag...
            if (iTag == -1 || tagNameLength < 1 || i == len)
            {
                // failed tag, insert the '<' as regular text and carry on
                i = k; 
                // look for closing '>' or a new command then insert entire failed block as regular text
                while( i<len && (pchOrig[i]!='>') && (i > k && pchOrig[i]!='<') )
                {
                    i++;
                }

                pfnText(pBlock, pchOrig + k, i - k + 1, k, &displayIndex, false);
                i++;
            }
            else
            {
                bool bDone = false;
                char aBufs[SM_MAX_PARAMS][128];
                TupleSS aParams[SM_MAX_PARAMS];
                SMTagDef *tag = &aTagDefs[iTag];

                memcpy(aParams, tag->aParams, sizeof(TupleSS)*SM_MAX_PARAMS);

                while(!bDone)
                {
                    k = i;
                    while(pchOrig[i]!='>' && pchOrig[i]!='=' && i<len)
                    {
                        i++;
                    }
                    if(pchOrig[i]!='=')
                    {
                        if(i!=k)
                        {
                            // If there was something afterwards, assume
                            // it's the default (first) parameter.
                            strncpy_s(aBufs[0], i-k, &pchOrig[k+1], _TRUNCATE);
                            aParams[0].pchVal = aBufs[0];
                        }
                        // suck anything remaining as a form of error handling.
                        while(pchOrig[i]!='>' && i<len)
                        {
                            i++;
                        }
                        pBlock = tag->pfn(pBlock, iTag, aParams);
                        if (pBlock)
                        {
                            pBlock->rawStringStartIndex = k;
                            displayIndex += tag->displayLength;
                            pBlock->displayStringStartIndex = displayIndex;
                            if (smf_ShouldOutputTagToString(pBlock->pSMFBlock, tag))
                            {
                                estrConcatFixedWidth(&pBlock->pSMFBlock->outputString, pchOrig + tagStartIndex, i - tagStartIndex + 1);
                            }
                        }
                        else
                        {
                            pBlock = pBlockTop;
                        }
                        bDone = true;
                        break;
                    }
                    else
                    {
                        int l;
                        for(l=0;
                            l<SM_MAX_PARAMS && aParams[l].pchName!=NULL && aParams[l].pchName[0]!=0;
                            l++)
                        {
                            if(strnicmp(&pchOrig[k+1], aParams[l].pchName, i-k-1)==0)
                            {
                                i++; // Skip over the =

                                k = i;

                                if(pchOrig[i]=='"')
                                {
                                    k++; // skip over the "
                                    i++;
                                    while(pchOrig[i]!='"' && i<len)
                                    {
                                        i++;
                                    }
                                }
                                else if(pchOrig[i]=='\'')
                                {
                                    int tmp = i;
                                    k++; // skip over the '
                                    tmp++;
                                    while(pchOrig[tmp]!='>' && tmp<len)
                                    {
                                        if(pchOrig[tmp]=='\'' )
                                            i = tmp;
                                        tmp++;
                                    }
                                }
                                else
                                {
                                    while(pchOrig[i]!='>' && pchOrig[i]!=' ' && pchOrig[i]!='\r' && pchOrig[i]!='\n' && pchOrig[i]!='\t' && i<len)
                                    {
                                        i++;
                                    }
                                }

                                if( i-k+1 > 0 )
                                {
                                    strncpy_s(aBufs[l], i-k+1, &pchOrig[k], _TRUNCATE);
                                    aParams[l].pchVal = aBufs[l];
                                }

                                if(pchOrig[i]=='"' && i<len)
                                {
                                    i++;
                                }
                                else if(pchOrig[i]=='\'' && i<len)
                                {
                                    i++;
                                }

                                if(pchOrig[i]!=' ' && pchOrig[i]!='\r' && pchOrig[i]!='\n' && pchOrig[i]!='\t')
                                {
                                    while(pchOrig[i]!='>' && pchOrig[i]!=' ' && pchOrig[i]!='\r' && pchOrig[i]!='\n' && pchOrig[i]!='\t' && i<len)
                                    {
                                        i++;
                                    }
                                    pBlock = tag->pfn(pBlock, iTag, aParams);
                                    if (pBlock)
                                    {
                                        pBlock->rawStringStartIndex = k;
                                        displayIndex += tag->displayLength;
                                        pBlock->displayStringStartIndex = displayIndex;
                                        if (smf_ShouldOutputTagToString(pBlock->pSMFBlock, tag))
                                        {
                                            estrConcatFixedWidth(&pBlock->pSMFBlock->outputString, pchOrig + tagStartIndex, i - tagStartIndex + 1);
                                        }
                                    }
                                    else
                                    {
                                        pBlock=pBlockTop;
                                    }
                                    bDone = true;
                                }

                                break;
                            }
                        }

                        if(l>=SM_MAX_PARAMS || aParams[l].pchName==NULL)
                        {
                            // OK, the param name wasn't found
                            // Skip to the end of the tag
                            while(pchOrig[i]!='>' && i<len)
                            {
                                i++;
                            }
                        }
                    }
                }

                i++; // move to the next character after the command
            }
        }
        else
        {
            i++; // skip over the crap
        }
    }

    // Adding in a bit of a hack to ensure that there is at least one sm_text block in every parsed tree structure.
    // I need this to exist for my selection/cursor code.
    if (!eaSize(&pBlockTop->ppBlocks) || smf_aTagDefs[pBlockTop->ppBlocks[eaSize(&pBlockTop->ppBlocks) - 1]->iType].id != k_sm_text)
    {
        pfnText(pBlock, "", 0, len, &displayIndex, true);
    }

    if (pBlock->pSMFBlock)
    {
        pBlock->pSMFBlock->displayStringLength = displayIndex;
    }

    return pBlockTop;
}

/**********************************************************************func*
 * sm_Parse
 *
 */
SMBlock *sm_Parse(char *pchOrig, SMFBlock *pSMFBlock, SMTagDef aTagDefs[], SMInitProc pfnInit, SMTextProc pfnText)
{
    SMBlock *pBlock = sm_CreateContainer();

    pBlock->iType = -1;
    pBlock = pfnInit(pBlock);
    pBlock->pSMFBlock = pSMFBlock;
    pBlock->pos.alignHoriz = pSMFBlock->defaultHorizAlign;

    return sm_ParseInto(pBlock, pchOrig, aTagDefs, pfnText);
}

/**********************************************************************func*
 * sm_ws
 *
 */
SMBlock *sm_ws(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    return pBlock;
}

/**********************************************************************func*
 * sm_bsp
 *
 */
SMBlock *sm_bsp(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlockAndData(pBlock, 2);
    pnew->iType = s_iTagText;
    *((char *)pnew->pv) = ' ';

    return pBlock;
}

/**********************************************************************func*
 * sm_br
 *
 */
SMBlock *sm_br(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    return pBlock;
}

/**********************************************************************func*
 * sm_toggle_end
 *
 */
SMBlock *sm_toggle_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    return pBlock;
}

/**********************************************************************func*
* sm_toggle_link_end
*
*/
SMBlock *sm_toggle_link_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    if (g_preventLinks)
        return NULL;
    else
        return sm_toggle_end(pBlock, iTag, aParams);
}

/**********************************************************************func*
 * sm_b
 *
 */
SMBlock *sm_b(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    pnew->pv = (void *)1;

    return pBlock;
}

/**********************************************************************func*
 * sm_i
 *
 */
SMBlock *sm_i(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    pnew->pv = (void *)1;

    return pBlock;
}

/**********************************************************************func*
 * sm_color
 *
 */
SMBlock *sm_color(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    pnew->pv = (void *)(sm_GetColor("color", aParams));
    return pBlock;
}

/**********************************************************************func*
* sm_color
*
*/
SMBlock *sm_color2(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    pnew->pv = (void *)(sm_GetColor("color2", aParams));

    return pBlock;
}


SMBlock *sm_colorhover(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    pnew->pv = (void *)(sm_GetColor("colorhover", aParams));

    return pBlock;
}


SMBlock *sm_colorselect(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    pnew->pv = (void *)(sm_GetColor("colorselect", aParams));

    return pBlock;
}


SMBlock *sm_colorselectbg(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    pnew->pv = (void *)(sm_GetColor("colorselectbg", aParams));

    return pBlock;
}


/**********************************************************************func*
 * sm_scale
 *
 */
SMBlock *sm_scale(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew=sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    pnew->pv = (void *)(int)(atof(sm_GetVal("scale", aParams))*SMF_FONT_SCALE);

    return pBlock;
}

/**********************************************************************func*
 * sm_outline
 *
 */
SMBlock *sm_outline(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew=sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    pnew->pv = (void *)(int)(atoi(sm_GetVal("outline", aParams)));

    return pBlock;
}

/**********************************************************************func*
 * sm_shadow
 *
 */
SMBlock *sm_shadow(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew=sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    pnew->pv = (void *)(int)(atoi(sm_GetVal("shadow", aParams)));

    return pBlock;
}

/**********************************************************************func*
 * sm_face
 *
 */
SMBlock *sm_face(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    char *pch;
    SMBlock *pnew=sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    pch = sm_GetVal("face", aParams);
    if(stricmp(pch, "redcircle")==0 || stricmp(pch, "title")==0)
    {
        pnew->pv = &title_12;
    }
    else if(stricmp(pch, "verdana-bold")==0 || stricmp(pch, "heading")==0)
    {
        pnew->pv = &smfLarge;
    }
    else if(stricmp(pch, "verdana")==0 || stricmp(pch, "body")==0)
    {
        pnew->pv = &smfDefault;
    }
    else if(stricmp(pch, "courier")==0 || stricmp(pch, "monospace")==0 || stricmp(pch, "mono")==0)
    {
        pnew->pv = &smfMono;
    }
    else if (stricmp(pch, "small")==0)
    {
        pnew->pv = &smfSmall;
    }
    else if (stricmp(pch, "computer")==0)
    {
        pnew->pv = &smfComputer;
    }
    else
    {
        pnew->pv = &smfDefault;
    }

    return pBlock;
}

/**********************************************************************func*
 * sm_font
 *
 */
SMBlock *sm_font(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    SMFont *pfont = (SMFont *)&pnew->pv;
    pnew->iType = iTag;

    if(*sm_GetVal("face", aParams)!=0)
    {
        sm_face(pBlock, kFormatTags_Face, aParams);
        pfont->bFace = 1;
    }

    if(*sm_GetVal("scale", aParams)!=0)
    {
        sm_scale(pBlock, kFormatTags_Scale, aParams);
        pfont->bScale = 1;
    }

    if(*sm_GetVal("color", aParams)!=0)
    {
        sm_color(pBlock, kFormatTags_Color, aParams);
        pfont->bColor = 1;
    }

    if(*sm_GetVal("color2", aParams)!=0)
    {
        sm_color2(pBlock, kFormatTags_Color2, aParams);
        pfont->bColor2 = 1;
    }

    if(*sm_GetVal("outline", aParams)!=0)
    {
        sm_outline(pBlock, kFormatTags_Outline, aParams);
        pfont->bOutline = 1;
    }

    if(*sm_GetVal("i", aParams)!=0)
    {
        sm_i(pBlock, kFormatTags_Outline, aParams);
        pfont->bItalic = 1;
    }

    if(*sm_GetVal("shadow", aParams)!=0)
    {
        sm_shadow(pBlock, kFormatTags_Shadow, aParams);
        pfont->bShadow = 1;
    }

    return pBlock;
}

/**********************************************************************func*
 * sm_anchor
 *
 */
SMBlock *sm_anchor(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    if (g_preventLinks)
        return NULL;
    else
    {
        SMAnchor *p;
        SMBlock *pnew=sm_AppendNewBlock(pBlock);
        pnew->iType = iTag;

        p = calloc(sizeof(SMAnchor)+strlen(sm_GetVal("href", aParams))+1, 1);
        strcpy(p->ach, sm_GetVal("href", aParams));

        pnew->pv = p;
        pnew->bFreeOnDestroy = true;

        return pBlock;
    }
}

/**********************************************************************func*
 * sm_align
 *
 */
SMBlock *sm_align(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew=sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    pnew->pv = (void *)sm_GetAlignment("align", aParams);

    return pBlock;
}


/**********************************************************************func*
 * sm_valign
 *
 */
SMBlock *sm_valign(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew=sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    pnew->pv = (void *)sm_GetAlignment("valign", aParams);

    return pBlock;
}

/**********************************************************************func*
 * sm_image
 *
 */
SMBlock *sm_image(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlockAndData(pBlock, sizeof(SMImage));
    SMImage *pImg = (SMImage *)pnew->pv;
    pnew->iType = iTag;

    pImg->iWidth = atoi(sm_GetVal("width", aParams));
    pImg->iHeight = atoi(sm_GetVal("height", aParams));
    pImg->iHighlight = atoi(sm_GetVal("highlight", aParams));
    pImg->iColor = sm_GetColor("color", aParams);
    strcpy(pImg->achTex, sm_GetVal("src", aParams));
    strcpy(pImg->achTexHover, sm_GetVal("srchover", aParams));

    pnew->pos.iBorder = atoi(sm_GetVal("border", aParams));
    pnew->pos.alignHoriz = sm_GetAlignment("align", aParams);

    return pBlock;
}

/**********************************************************************func*
 * sm_table
 *
 */
SMBlock *sm_table(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewContainer(pBlock);
    pnew->iType = iTag;

    pnew->pos.iBorder = atoi(sm_GetVal("border", aParams));

    return pnew;
}

/**********************************************************************func*
 * sm_table_end
 *
 */
SMBlock *sm_table_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    return pBlock->pParent;
}

/**********************************************************************func*
 * sm_tr
 *
 */
SMBlock *sm_tr(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewContainerAndData(pBlock, sizeof(SMRow));
    SMRow *pRow = (SMRow *)pnew->pv;
    char *pch = sm_GetVal("style", aParams);

    pnew->iType = iTag;

    pRow->iHighlight = atoi(sm_GetVal("highlight", aParams));
    pRow->iSelected = atoi(sm_GetVal("selected", aParams));
    pnew->pos.iBorder = atoi(sm_GetVal("border", aParams));

    if (pch && stricmp(pch, "gradient") == 0)
        pRow->iStyle = 1;
    else
        pRow->iStyle = 0;

    return pnew;
}

/**********************************************************************func*
 * sm_tr_end
 *
 */
SMBlock *sm_tr_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    return pBlock->pParent;
}

/**********************************************************************func*
 * sm_td
 *
 */
SMBlock *sm_td(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewContainerAndData(pBlock, sizeof(SMColumn));
    SMColumn *pCol = (SMColumn *)pnew->pv;

    pnew->iType = iTag;

    pCol->iWidthRequested = atoi(sm_GetVal("width", aParams));
    pCol->iHighlight = atoi(sm_GetVal("highlight", aParams));
    pCol->iSelected = atoi(sm_GetVal("selected", aParams));
    pnew->pos.iBorder = atoi(sm_GetVal("border", aParams));
    pnew->pos.alignHoriz = sm_GetAlignment("align", aParams);
    pnew->pos.alignVert = sm_GetAlignment("valign", aParams);
    pnew->bgcolor = sm_GetColor("cellbg", aParams);

    return pnew;
}

/**********************************************************************func*
 * sm_td_end
 *
 */
SMBlock *sm_td_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    return pBlock->pParent;
}

/**********************************************************************func*
 * sm_span
 *
 */
SMBlock *sm_span(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewContainerAndData(pBlock, sizeof(SMColumn));
    SMColumn *pCol = (SMColumn *)pnew->pv;

    pnew->iType = iTag;

    pCol->iWidthRequested = atoi(sm_GetVal("width", aParams));
    pCol->iHighlight = atoi(sm_GetVal("highlight", aParams));
    pnew->pos.alignHoriz = sm_GetAlignment("align", aParams);
    pnew->pos.alignVert = sm_GetAlignment("valign", aParams);

    return pnew;
}

/**********************************************************************func*
 * sm_span_end
 *
 */
SMBlock *sm_span_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    return pBlock->pParent;
}

/**********************************************************************func*
 * sm_p
 *
 */
SMBlock *sm_p(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    return pBlock;
}

/**********************************************************************func*
 * sm_p_end
 *
 */
SMBlock *sm_p_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;

    return pBlock;
}

/**********************************************************************func*
 * sm_nolink
 *
 */
SMBlock *sm_nolink(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    g_preventLinks++;

    return pBlock;
}

/**********************************************************************func*
 * sm_nolink_end
 *
 */
SMBlock *sm_nolink_end(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    SMBlock *pnew = sm_AppendNewBlock(pBlock);
    pnew->iType = iTag;
    if (g_preventLinks)
        g_preventLinks--;

    return pBlock;
}


/**********************************************************************func*
 * sm_text
 *
 */
SMBlock *sm_text(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    return pBlock;
}

SMTagDef smf_aTagDefs[] = // extern'd in sm_parser.h
{
    //{ "tag",            SMF_TAG(tag_fn_name),            {{ aParams }},
    //            displayLength,    outputToString,    },

    { "b",                SMF_TAG(sm_b),                    {{ 0 }},
                0,                true,            },
    { "i",                SMF_TAG(sm_i),                    {{ 0 }},
                0,                true,            },
    { "color",            SMF_TAG(sm_color),                {{ "color", "white" }, {"color2" ""}},
                0,                true,            },
    { "color2",            SMF_TAG(sm_color2),                {{ "color2", "white" }},
                0,                true,            },
    { "colorhover",        SMF_TAG(sm_colorhover),            {{ "colorhover", "white" }},
                0,                true,            },
    { "colorselect",    SMF_TAG(sm_colorselect),        {{ "colorselect", "white" }},
                0,                true,            },
    { "colorselectbg",    SMF_TAG(sm_colorselectbg),        {{ "colorselectbg", "black" }},
                0,                true,            },
    { "scale",            SMF_TAG(sm_scale),                {{ "scale", "1.0" }},
                0,                true,            },
    { "face",            SMF_TAG(sm_face),                {{ "face", "arial" }},
                0,                true,            },
    { "font",            SMF_TAG(sm_font),                {{ "face", "" }, { "scale", "" }, { "color", "" },{ "color2", "" },{ "outline", "" }, { "shadow", "" },{ "i", "" },},
                0,                true,            },
    { "a",                SMF_TAG(sm_anchor),                {{ "href", "" }},
                0,                false,            },
    { "link",            SMF_TAG(sm_color),                {{ "color", "green" }},
                0,                true,            },
    { "linkbg",            SMF_TAG(sm_color),                {{ "color", "0" }},
                0,                true,            },
    { "linkhover",        SMF_TAG(sm_color),                {{ "color", "lawngreen" }},
                0,                true,            },
    { "linkhoverbg",    SMF_TAG(sm_color),                {{ "color", "0" }},
                0,                true,            },
    { "linkselect",        SMF_TAG(sm_color),                {{ "color", "red" }},
                0,                true,            },
    { "linkselectbg",    SMF_TAG(sm_color),                {{ "color", "0" }},
                0,                true,            },
    { "outline",        SMF_TAG(sm_outline),            {{ "outline", "0" }},
                0,                true,            },
    { "shadow",            SMF_TAG(sm_shadow),                {{ "shadow", "0" }},
                0,                true,            },
    //Shannon: I use "time", "t", "emote", "em", and "bubble" for the animation tags, thanks, Matt

    { "/b",                SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/i",                SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/color",            SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/color2",        SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/colorhover",    SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/colorselect",    SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/colorselectbg",    SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/scale",            SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/face",            SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/font",            SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/a",                SMF_TAG(sm_toggle_link_end),    {{ 0 }},
                0,                false,            },
    { "/link",            SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/linkbg",        SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/linkhover",        SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/linkhoverbg",    SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/linkselect",    SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/linkselectbg",    SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/outline",        SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },
    { "/shadow",        SMF_TAG(sm_toggle_end),            {{ 0 }},
                0,                true,            },

    { "text",            SMF_TAG(sm_text),                {{ 0 }},
                -1,                true,            }, // must be last, doesn't use displayLength
    // don't reorder the items above here.
    { "ws",                SMF_TAG(sm_ws),                    {{ 0 }},
                -1,                true,            }, // doesn't use displayLength
    { "bsp",            SMF_TAG(sm_bsp),                {{ 0 }},
                0,                true,            },
    { "br",                SMF_TAG(sm_br),                    {{ 0 }},
                1,                true,            },
    { "table",            SMF_TAG(sm_table),                {{ "border", "0" }},
                0,                true,            },
    { "/table",            SMF_TAG(sm_table_end),            {{ 0 }},
                0,                true,            },
    { "tr",                SMF_TAG(sm_tr),                    {{ "highlight", "1" }, { "border", "2" }, { "selected", "0" }, { "style", "rounded" }},
                0,                true,            },
    { "/tr",            SMF_TAG(sm_tr_end),                {{ 0 }},
                0,                true,            },
    { "td",                SMF_TAG(sm_td),                    {{ "width", "0" }, { "align", "none" }, { "valign", "none" }, { "highlight", "0" }, { "border", "2" }, { "selected", "0" }, { "cellbg", "0" }},
                0,                true,            },
    { "/td",            SMF_TAG(sm_td_end),                {{ 0 }},
                0,                true,            },
    { "span",            SMF_TAG(sm_span),                {{ "width", "0" }, { "align", "none" }, { "valign", "none" }, { "highlight", "0" }},
                1,                true,            },
    { "/span",            SMF_TAG(sm_span_end),            {{ 0 }},
                1,                true,            },
    { "p",                SMF_TAG(sm_p),                    {{ 0 }},
                0,                true,            },
    { "/p",                SMF_TAG(sm_p_end),                {{ 0 }},
                0,                true,            },
    { "img",            SMF_TAG(sm_image),                {{ "src", "white.tga" }, { "srchover", "" }, { "align", "none" }, { "width", "-1" }, { "height", "-1" }, { "border", "4" }, { "highlight", "0" }, { "color", "white" }},
                0,                false,            },
    { "nolink",            SMF_TAG(sm_nolink),                {{ 0 }},
                0,                true,            },
    { "/nolink",        SMF_TAG(sm_nolink_end),            {{ 0 }},
                0,                true,            },

    { "bgcolor",        SMF_TAG_NONE(sm_text),            {{ 0 }},
                0,                true,            },
    { "bordercolor",    SMF_TAG_NONE(sm_text),            {{ 0 }},
                0,                true,            },
    { "duration",        SMF_TAG_NONE(sm_text),            {{ 0 }},
                0,                true,            },
    { 0 },
};


/**********************************************************************func*
 * InitBase
 *
 */
static SMBlock *InitBase(SMBlock *pBlock)
{
    return pBlock;
}

/**********************************************************************func*
 * smf_CreateAndParse
 *
 */
SMBlock *smf_CreateAndParse(char *pch, SMFBlock *pSMFBlock)
{
    SMBlock *retBlock;
    
    PERFINFO_AUTO_START("smf_CreateAndParse", 1);
        // Find the whitespace and text tags
        if(s_iTagText==-1)
        {
            int i = 0;
            SMTagDef *ptag=smf_aTagDefs;

            while(ptag[i].pchName != NULL)
            {
                if(ptag[i].id == k_sm_ws)
                {
                    s_iTagWS = i;
                }
                else if(ptag[i].id == k_sm_text)
                {
                    s_iTagText = i;
                }

                i++;
            }
        }

        PERFINFO_AUTO_START("sm_Parse", 1);
            retBlock = sm_Parse(pch, pSMFBlock, smf_aTagDefs, InitBase, TextPush);
        PERFINFO_AUTO_STOP();
    PERFINFO_AUTO_STOP();
    
    return retBlock;
}





/* End of File */
