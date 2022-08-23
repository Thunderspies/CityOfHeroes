/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#include <string.h>
#include "demo.h"

#include <utilitieslib/utils/mathutil.h>
#include <utilitieslib/components/StashTable.h>
#include "filter/profanity.h"
#include <utilitieslib/components/earray.h>
#include "formatter/smf_parse.h"
#include "formatter/smf_main.h"
#include "uiEmote.h"
#include <utilitieslib/language/AppLocale.h>
#include "entity/entity.h"
#include "entity/entclient.h"
#include "player/player.h"
#include "player/playerState.h"
#include "UI/uiChat.h"
#include "entity/entVarUpdate.h"
#include "cmdparse/cmdgame.h"
#include <utilitieslib/components/EString.h>
#include "UI/uiOptions.h"


typedef struct TupleNNNN
{
    int iKey;
    unsigned int uiVal1;
    unsigned int uiVal2;
    unsigned int uiVal3;
} TupleNNNN;

typedef struct TextParams
{
    int svr_idx;
    unsigned int uiColorFG;
    unsigned int uiColorBG;
    unsigned int uiColorBorder;
    float fScale;
    float fDuration;
} TextParams;

static SMBlock *SetColorFG(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS]);
static SMBlock *SetColorBG(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS]);
static SMBlock *SetColorBorder(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS]);
static SMBlock *SetScale(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS]);
static SMBlock *SetDuration(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS]);
static SMBlock *ShowIcon(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS]);
static SMBlock *Text(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS]);

static SMTagDef s_aTagDefs[] =
{
//    { "tag",            SMF_TAG(tag_fn_name),            {{ aParams }}
    //            displayLength,    outputToString,    },

    { "text",        1, Text,      }, // I have no idea what this is supposed to be...
    { "color",            SMF_TAG_NONE(SetColorFG),         {{ "val", "black" }},
                0,                true,            },
    { "bgcolor",        SMF_TAG_NONE(SetColorBG),         {{ "val", "white" }},
                0,                true,            },
    { "bordercolor",    SMF_TAG_NONE(SetColorBorder),     {{ "val", "black" }},
                0,                true,            },
    { "scale",            SMF_TAG_NONE(SetScale),           {{ "val", "1.0"   }},
                0,                true,            },
    { "duration",        SMF_TAG_NONE(SetDuration),        {{ "val", "1.0"   }},
                0,                true,            },

    // Tags we want to eat but not do anything about
    { "a",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "b",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "i",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "color2",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "face",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "font",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "link",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "linkbg",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "linkhover",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "linkhoverbg",    SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "outline",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "shadow",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/b",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/i",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/color",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/color2",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/scale",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/face",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/font",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/a",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/link",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/linkbg",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/linkhover",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/linkhoverbg",    SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/outline",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/shadow",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "text",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "ws",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "bsp",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "br",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "table",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/table",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "tr",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/tr",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "td",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/td",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "span",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/span",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "p",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/p",                SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "img",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "nolink",            SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },
    { "/nolink",        SMF_TAG_NONE(Text),                {{ 0 }},
                0,                false,            },

    {0}
};


/**********************************************************************func*
 * SetColorFG
 *
 */
static SMBlock *SetColorFG(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    if(pBlock->iType == -1 && pBlock->pv)
    {
        TextParams *ptp = (TextParams *)pBlock->pv;

        ptp->uiColorFG = sm_GetColor("val", aParams);
    }

    return pBlock;
}

/**********************************************************************func*
 * SetColorBG
 *
 */
static SMBlock *SetColorBG(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    if(pBlock->iType == -1 && pBlock->pv)
    {
        TextParams *ptp = (TextParams *)pBlock->pv;

        ptp->uiColorBG = sm_GetColor("val", aParams);
    }

    return pBlock;
}


/**********************************************************************func*
 * SetColorBorder
 *
 */
static SMBlock *SetColorBorder(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    if(pBlock->iType == -1 && pBlock->pv)
    {
        TextParams *ptp = (TextParams *)pBlock->pv;

        ptp->uiColorBorder = sm_GetColor("val", aParams);
    }

    return pBlock;
}

/**********************************************************************func*
 * SetScale
 *
 */
static SMBlock *SetScale(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    if(pBlock->iType == -1 && pBlock->pv)
    {
        TextParams *ptp = (TextParams *)pBlock->pv;
        float f = atof(aParams[0].pchVal);

        if(f<0.6f)  f=0.6f;
        if(f>1.25f) f=1.25f;

        ptp->fScale = f;
    }

    return pBlock;
}

/**********************************************************************func*
 * SetDuration
 *
 */
static SMBlock *SetDuration(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    if(pBlock->iType == -1 && pBlock->pv)
    {
        TextParams *ptp = (TextParams *)pBlock->pv;
        float f = atof(aParams[0].pchVal);

        if(f<1.5f)  f=1.5f;
        if(f>20.0f) f=20.0f;

        ptp->fDuration = f*30;
    }

    return pBlock;
}

/**********************************************************************func*
 * ShowIcon
 *
 */
static SMBlock* ShowIcon(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    static TupleSS s_aIcons[] =
    {
        { "?",     "chat_icon_question.tga" },
        { "!",     "chat_icon_exclaim.tga"  },
        { "skull", "chat_icon_death.tga"    },
        { 0 },
    };
    TextParams *textparams = (TextParams *)pBlock->pv;
    TupleSS *tup = FindTupleSS(s_aIcons, aParams[0].pchVal);

    if(tup!=NULL)
    {
        //addFloatingInfo(textparams->svr_idx,
        //    tup->pchVal,
        //    textparams->uiColorFG, textparams->uiColorBG, textparams->uiColorBorder,
        //    textparams->fScale,
        //    textparams->fDuration, 0,
        //    kFloaterStyle_Icon, 0);
    }

    return pBlock;
}


/**********************************************************************func*
 * Text
 *
 */
static SMBlock *Text(SMBlock *pBlock, int iTag, TupleSS aParams[SM_MAX_PARAMS])
{
    // Just a stub
    return pBlock;
}

/***********************************************************************
 * Colors for message types  COLOR FOR CHAT BUBBLES
 */
static TupleNNNN s_aMsgColors[] =
{
    // Msg type             text color  background  border
//    { INFO_COMBAT,          0x000000ff, 0xffffffff, 0x000000ff },
//    { INFO_DAMAGE,          0x000000ff, 0xffffffff, 0x000000ff },
//    { INFO_SVRR_COM,        0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_NPC_SAYS,        0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_VILLAIN_SAYS,    0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_PET_COM,            0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_PRIVATE_COM,     0x000000ff, 0xddddddff, 0x000000ff },
    { INFO_TEAM_COM,        0x000000ff, 0xddddddff, 0x000000ff },
    { INFO_LEVELINGPACT_COM,0x000000ff, 0xddddddff, 0x000000ff },
    { INFO_SUPERGROUP_COM,  0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_ALLIANCE_OWN_COM, 0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_ALLIANCE_ALLY_COM,0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_NEARBY_COM,      0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_SHOUT_COM,       0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_REQUEST_COM,     0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_FRIEND_COM,      0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_EMOTE,           0x000000ff, 0xffffffff, 0x000000ff },
//    { INFO_AUCTION,         0x0000ffff, 0xffffffff, 0x000000ff },
//    { INFO_ADMIN_COM,       0x000000ff, 0xffffffff, 0x000000ff },
//    { INFO_USER_ERROR,      0x000000ff, 0xffffffff, 0x000000ff },
//    { INFO_DEBUG_INFO,      0x000000ff, 0xffffffff, 0x000000ff },
    { INFO_GMTELL,            0x000000ff, 0xddddddff, 0x000000ff },
    { INFO_ARENA,            0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_ARENA_ERROR,        0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_ARENA_GLOBAL,    0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_CAPTION,            0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_ARCHITECT_GLOBAL,0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_LEAGUE_COM,        0x0000ffff, 0xffffffff, 0x000000ff },
    { INFO_PRIVATE_NOREPLY_COM,0x000000ff, 0xddddddff, 0x000000ff },
    { -1 }
};

/**********************************************************************func*
 * FindTupleNNNN
 *
 */
static TupleNNNN *FindTupleNNNN(TupleNNNN *pTuples, int iKey)
{
    while(pTuples->iKey>=0)
    {
        if(pTuples->iKey==iKey)
        {
            return pTuples;
        }
        pTuples++;
    }

    return NULL;
}

/**********************************************************************func*
 * SetDefaultFloaterColors
 *
 */
static bool SetDefaultFloaterColors(TextParams *textparams, int type)
{
    TupleNNNN *tup = FindTupleNNNN(s_aMsgColors, type);
    if(tup!=NULL)
    {
        textparams->uiColorFG     = tup->uiVal1;
        textparams->uiColorBG     = tup->uiVal2;
        textparams->uiColorBorder = tup->uiVal3;

        return true;
    }

    return false;
}

/**********************************************************************func*
 * CalcColor
 *
 */
static U32 CalcColor(U32 color, U32 alpha)
{
    return (color & 0xffffff00)|(((alpha*(color&0xff))/255)&0xff);
}

/**********************************************************************func*
 * addFloaterMsg
 *
 */
static void addFloaterMsg(char *pch, TextParams *textparams, int type, float duration, Vec2 position)
{
    if((optionGet(kUO_ShowBallons) || game_state.viewCutScene) && textparams->svr_idx!=0)
    {
        Entity *e = playerPtr();
        Entity *eSrc = entFromId(textparams->svr_idx);
        EFloaterStyle style = kFloaterStyle_Chat;
        Vec3 captionLocation = {0.f, 0.f, 0.f};
        float* locPtr = 0;

        // TODO: This is to make private comms look
        //       like they're not really in the world.
        //       Maybe there is a better way.
        if(type==INFO_PRIVATE_COM
            || type==INFO_TEAM_COM
            || type==INFO_LEVELINGPACT_COM
            || type==INFO_SUPERGROUP_COM
            || type==INFO_ALLIANCE_OWN_COM
            || type==INFO_ALLIANCE_ALLY_COM
            || type==INFO_FRIEND_COM
             || type==INFO_GMTELL
            || type==INFO_LEAGUE_COM
            || type==INFO_PRIVATE_NOREPLY_COM)
         {
            style = kFloaterStyle_Chat_Private;

            if(eSrc==e)
            {
                textparams->uiColorFG = CalcColor(textparams->uiColorFG, 0xcc);
                textparams->uiColorBG = CalcColor(textparams->uiColorBG, 0xcc);
                textparams->uiColorBorder = CalcColor(textparams->uiColorBorder, 0xcc);
            }
        }
        else if(type==INFO_EMOTE)
        {
            style = kFloaterStyle_Emote;
        }
        else if(type==INFO_PET_SAYS||type==INFO_PET_COM)
        {
            style = kFloaterStyle_DeathRattle;
        }
        else if(type==INFO_CAPTION)
        {
            if (position)
            {
                captionLocation[0] = position[0];
                captionLocation[1] = position[1];
            }

            style = kFloaterStyle_Caption;
            locPtr = captionLocation;
        }

        addFloatingInfo(textparams->svr_idx,
            pch,
            textparams->uiColorFG, textparams->uiColorBG, textparams->uiColorBorder,
            textparams->fScale,
            duration, 0,
            style, locPtr);
    }
}

/**********************************************************************func*
 * CleanTextPush
 *
 */
static SMBlock *CleanTextPush(SMBlock *pBlock, const char *pch, int iLen, int rawStartIndex, int *displayStartIndex)
{
    SMBlock *pnew = NULL;

    if(iLen>0)
    {
        pnew = sm_AppendNewBlock(pBlock);
        pnew->pv = (void *)calloc(iLen+1, 1);
        strncpy(pnew->pv, pch, iLen);
        pnew->rawStringStartIndex = rawStartIndex;
        pnew->displayStringStartIndex = *displayStartIndex;
        (*displayStartIndex) += iLen;
        if (pnew->pSMFBlock)
        {
            estrConcatFixedWidth(&pnew->pSMFBlock->outputString, pch, iLen);
        }
    }

    return pnew;
}

/**********************************************************************func*
 * TextPush
 *
 */
static SMBlock *TextPush(SMBlock *pBlock, const unsigned char *pch, int iLen, int preparseStartIndex, int *postparseStartIndex, bool createIfEmpty)
{
    SMBlock *pnew = NULL;

    if(iLen>0)
    {
        if(optionGet(kUO_AllowProfanity))
        {
            CleanTextPush(pBlock, pch, iLen, preparseStartIndex, postparseStartIndex);
        }
        else
        {
            char *pchBuff = _alloca(iLen+2);
            char *pchStart;
            char *pchLast;
            char *pchCur;

            strncpy(pchBuff, pch, iLen);
            pchBuff[iLen]='\0';
            pchBuff[iLen+1]='\0'; // HACK: just let it go.
            pchCur = pchLast = pchStart = pchBuff;

            replace(pchBuff, "-_?.,!", ' ');

            while(*pchLast!='\0')
            {
                if((pchCur=strchr(pchCur, ' '))==NULL)
                {
                    pchCur = pchBuff+iLen;
                }

                if(IsProfanity(pchLast, pchCur-pchLast) || IsProfanity(pch+(pchLast-pchBuff), pchCur-pchLast))
                {
                    // Dump the non-profane up to here.
                    CleanTextPush(pBlock, pch+(pchStart-pchBuff), pchLast-pchStart, preparseStartIndex + (pchStart - pchBuff), postparseStartIndex);
                    pchStart = pchCur;

                    // Dump in a replacement
                    if(rand()%2)
                    {
                        CleanTextPush(pBlock, "<@*&$@#!>", 9, preparseStartIndex + (pchLast - pchBuff), postparseStartIndex);
                    }
                    else
                    {
                        CleanTextPush(pBlock, "<bleep!>", 8, preparseStartIndex + (pchLast - pchBuff), postparseStartIndex);
                    }
                }

                pchCur++;
                pchLast = pchCur;
            }

            CleanTextPush(pBlock, pch+(pchStart-pchBuff), iLen-(pchStart-pchBuff), preparseStartIndex + (pchStart - pchBuff), postparseStartIndex);
        }
    }

    return pnew;
}

void doFloaterMessage(char * pchFinal, TextParams * tp, int type, float duration, int svr_idx, Vec2 position)
{
    // If we're going to show this over an entity's head, then strip
    // off the leader (channel, name, colon, whitepace)
    if(pchFinal[0] != '\0' && svr_idx!=0)
    {
        Entity *eSrc = entFromId(svr_idx);
        int iExtra = 0;

        if(pchFinal[0]=='[')
        {
            char *pos;
            
            if( type == INFO_PET_COM )
                pos = strchr(pchFinal, ')');
            else
                pos = strchr(pchFinal, ']');

            if(pos!=NULL)
            {
                iExtra = pos-pchFinal+1;
                while(pchFinal[iExtra]==' ')
                {
                    iExtra++;
                }
            }
        }

        if(eSrc)
        {

            if( eSrc->petName && strnicmp(eSrc->petName, pchFinal+iExtra, strlen(eSrc->petName))==0)
            {
                iExtra += strlen(eSrc->petName);
            }

            if(strnicmp(eSrc->name, pchFinal+iExtra, strlen(eSrc->name))==0)
            {
                iExtra += strlen(eSrc->name);
            }
            while(pchFinal[iExtra]==' ')
            {
                iExtra++;
            }
            if(pchFinal[iExtra]==':')
            {
                iExtra++;
                while(pchFinal[iExtra]==' ')
                {
                    iExtra++;
                }
            }
        }

        if(pchFinal[iExtra]!='\0')
        {
            //    addFloaterMsg(pchFinal+iExtra, (TextParams *)pBlock->pv, msg);
            addFloaterMsg(pchFinal+iExtra, tp, type, duration, position);
        }
    }
}

/**********************************************************************func*
 * ParseChatText
 *
 */
void ParseChatText(char *pchOrig, INFO_BOX_MSGS msg, float duration, int svr_idx, Vec2 position)
{
    static SMFBlock *pSMFBlock = 0;
    static TextParams *ptp = 0;
    SMBlock *pBlock = 0;

    if (!ptp)
    {
        ptp = (TextParams *)calloc(1, sizeof(TextParams));
    }

    ptp->svr_idx       = svr_idx;
    ptp->fDuration     = 8.0*30;
    ptp->fScale        = 1.0;
    ptp->uiColorFG     = 0x000000ff;
    ptp->uiColorBG     = 0xffffffff;
    ptp->uiColorBorder = 0x000000ff;

    SetDefaultFloaterColors(ptp, msg);

    if (!pSMFBlock)
    {
        pSMFBlock = smfBlock_Create();
        smf_SetFlags(pSMFBlock, SMFEditMode_Unselectable, 0, 0, 0, SMFScrollMode_ExternalOnly,
            SMFOutputMode_StripAllTagsAndCodes, SMFDisplayMode_AllCharacters, SMFContextMenuMode_None, 0, 0, 0, 0);
    }

    // Copy and paste from smf_CreateAndParse() here.
    // Required because I need to set pBlock->pv to ptp.

    pBlock = sm_CreateContainer();
    pBlock->iType = -1;
    pBlock->pv = ptp;
    pBlock->pSMFBlock = pSMFBlock;
    pBlock->pos.alignHoriz = pSMFBlock->defaultHorizAlign;

    sm_ParseInto(pBlock, pchOrig, s_aTagDefs, 0); // use smf_parse's TextPush.

    sm_DestroyBlock(pBlock);

    // END smf_CreateAndParse().
    
    doFloaterMessage(pSMFBlock->outputString, ptp, msg, duration*30, svr_idx, position);
}

char * dealWithPercents( char *instring )
{
    static char *ret=NULL;
    static int retlen=0;
    const char *c;
    char *out;
    int i,deslen, len=strlen(instring);

    if(!instring)
        return NULL;
    deslen = len*2+1;
    if (retlen < deslen) // We need a bigger buffer to return the data
    {
        if (ret)
            free(ret);
        if (deslen<256)
            deslen = 256; // Allocate at least 256 bytes the first time
        ret = malloc(deslen);
        retlen = deslen;
    }
    for (i=0, c=instring, out=ret; i<len ; c++, i++)
    {
        if(*c == '%')
            *out++='%';
        *out++=*c;
    }
    *out=0;
    return ret;
}

static SMBlock *InitBase(SMBlock *pBlock)
{
    return pBlock;
}

char* stripPlayerHTML(char *pchOrig, int profanity_too )
{
    char *s = dealWithPercents(pchOrig);
    static char *pchFinal = 0;
    SMFBlock *pSMFBlock = smfBlock_Create();
    if( profanity_too )
        sm_Parse(s, pSMFBlock, s_aTagDefs, InitBase, TextPush);
    else
        sm_Parse(s, pSMFBlock, s_aTagDefs, InitBase, 0);

    if(!pchFinal)
        estrCreate(&pchFinal);
    estrClear(&pchFinal);
    estrPrintEString(&pchFinal, pSMFBlock->outputString);

    smfBlock_Destroy(pSMFBlock);
    return pchFinal;
}


/* End of File */
