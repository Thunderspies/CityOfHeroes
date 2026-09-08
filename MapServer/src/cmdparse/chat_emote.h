/***************************************************************************
 *     Copyright (c) 2003-2006, Cryptic Studios
 *     All Rights Reserved
 *     Confidential Property of Cryptic Studios
 ***************************************************************************/
#if (!defined(CHAT_EMOTE_H__)) || (defined(CHAT_EMOTE_PARSE_INFO_DEFINITIONS)&&!defined(CHAT_EMOTE_PARSE_INFO_DEFINED))
#define CHAT_EMOTE_H__


#include <utilitieslib/stdtypes.h>
#include <utilitieslib/utils/textparser.h>

typedef struct Entity Entity;

typedef struct EmoteAnim
{
    const char *pchDisplayName;
    const int *piModeBits;
    const char *pchRequiredToken;
    const char *pchRequiredBadge;
    const char **pchRequires;
    const char *pchPowerReward;
    const char **pchPowerRequires;
    const char *pchStoreProduct;
    int bDevOnly;
} EmoteAnim;

#ifdef CHAT_EMOTE_PARSE_INFO_DEFINITIONS
extern StaticDefine ParsePowerDefines[]; // defined in load_def.c

TokenizerParseInfo ParseEmoteAnim[] =
{
    { "DisplayName",        TOK_STRUCTPARAM|TOK_STRING(EmoteAnim, pchDisplayName, 0) },
    { "ModeBits",            TOK_STRUCTPARAM|TOK_INTARRAY(EmoteAnim, piModeBits), ParsePowerDefines },
    { "{",                    TOK_START,                        0},
    { "RequiredToken",        TOK_STRING(EmoteAnim, pchRequiredToken,0), ParsePowerDefines },
    { "RequiredBadge",        TOK_STRING(EmoteAnim, pchRequiredBadge,0), ParsePowerDefines },
    { "Requires",            TOK_STRINGARRAY(EmoteAnim, pchRequires) },
    { "PowerReward",        TOK_STRING(EmoteAnim, pchPowerReward, 0) },
    { "PowerRequires",        TOK_STRINGARRAY(EmoteAnim, pchPowerRequires) },
    { "StoreProduct",        TOK_STRING(EmoteAnim, pchStoreProduct, 0) },
    { "DevOnly",            TOK_INT(EmoteAnim,bDevOnly,0) },
    { "}",                    TOK_END,      0 },
    { 0 }
};
TokenizerParseInfo ParseDevEmoteAnim[] =
{
    { "DisplayName",  TOK_STRUCTPARAM|TOK_STRING(EmoteAnim,pchDisplayName,0) },
    { "ModeBits",     TOK_STRUCTPARAM|TOK_INTARRAY(EmoteAnim, piModeBits), ParsePowerDefines },
    { "DevOnly",      TOK_INT(EmoteAnim, bDevOnly, 1) },
    { "\n",           TOK_END,                        0 },
    { 0 }
};
#endif

typedef struct EmoteAnims
{
    const EmoteAnim **ppAnims;
} EmoteAnims;


#ifdef CHAT_EMOTE_PARSE_INFO_DEFINITIONS
TokenizerParseInfo ParseEmoteAnims[] =
{
    { "{",           TOK_START,       0 },
    { "Emote",       TOK_STRUCT(EmoteAnims, ppAnims, ParseEmoteAnim) },
    { "DevEmote",    TOK_REDUNDANTNAME | TOK_STRUCT(EmoteAnims, ppAnims, ParseDevEmoteAnim) }, // Dirty hack?
    { "}",           TOK_END,         0 },
    { 0 }
};
#endif

extern SHARED_MEMORY EmoteAnims g_EmoteAnims;

bool EmoteAnimFinalProcess(ParseTable* pti, void* structptr, bool shared_memory);

//-------------------------------------------------------------------------------------
// costume change emotes
//-------------------------------------------------------------------------------------
extern bool gIsCCemote;
extern bool gDetectedCCemote;

bool chatemote_CanDoEmote(const EmoteAnim *anim, Entity *e);
bool matchEmoteName( const char * displayName, const char * string );

#ifdef CHAT_EMOTE_PARSE_INFO_DEFINITIONS
#define CHAT_EMOTE_PARSE_INFO_DEFINED
#endif

#endif /* #ifndef EMOTE_H__ */

/* End of File */

