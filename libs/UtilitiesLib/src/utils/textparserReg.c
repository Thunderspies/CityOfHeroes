#include "utilitieslib/utils/textparser.h"
#include "utilitieslib/utils/SuperAssert.h"
#include "utilitieslib/components/MemoryPool.h"
#include "utilitieslib/components/earray.h"
#include <stdio.h>
#include <string.h>
#include "utilitieslib/utils/RegistryReader.h"
#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/utils/textparserUtils.h"


// key should be either a full key "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\Coh" or just the post-cryptic bit, "Coh"
int ParserWriteRegistry(const char *key, ParseTable pti[], void *structptr, StructTypeField iOptionFlagsToMatch, StructTypeField iOptionFlagsToExclude)
{
    int succeeded = 1;
    int i;

    char fullKey[MAX_PATH];
    char buf[1024];
    RegReader rr;

    rr = createRegReader();

    if (strStartsWith(key, "HKEY")) {
        strcpy(fullKey, key);
    } else {
        sprintf_s(SAFESTR(fullKey), "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\%s", key);
    }
    initRegReader(rr, fullKey);

    // data segment
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue; // don't allow TOK_REDUNDANTNAME
        if (iOptionFlagsToMatch && !(iOptionFlagsToMatch & pti[i].type)) continue;
        if (iOptionFlagsToExclude && (iOptionFlagsToExclude & pti[i].type)) continue;
        if (pti[i].name && pti[i].name[0] && TokenToSimpleString(pti, i, structptr, SAFESTR(buf), 0))
        {
            rrWriteString(rr, pti[i].name, buf);            
        }
    } // each data token
    destroyRegReader(rr);
    return succeeded;
}

int ParserReadRegistry(const char *key, ParseTable pti[], void *structptr)
{
    int succeeded = 1;
    int i;

    char fullKey[MAX_PATH];
    char buf[1024];
    RegReader rr;

    rr = createRegReader();

    if (strStartsWith(key, "HKEY")) {
        strcpy(fullKey, key);
    } else {
        sprintf_s(SAFESTR(fullKey), "HKEY_CURRENT_USER\\SOFTWARE\\Cryptic\\%s", key);
    }
    initRegReader(rr, fullKey);

    // data segment
    FORALL_PARSEINFO(pti, i)
    {
        if (pti[i].type & TOK_REDUNDANTNAME) continue; // don't allow TOK_REDUNDANTNAME

        if (!pti[i].name[0]) continue;
        if (!rrReadString(rr, pti[i].name, SAFESTR(buf)))
            continue;

        TokenFromSimpleString(pti, i, structptr, buf);
    } // each data token
    destroyRegReader(rr);
    return succeeded;
}
