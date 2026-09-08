#include "seq/AnimBitList.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/utils/error.h>
#include <utilitieslib/utils/fileutil.h>
#include <utilitieslib/utils/FolderCache.h>
#include <utilitieslib/components/StashTable.h>
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/mathutil.h>
#include "seq/seqstate.h"

typedef struct AllAnimLists{
    AnimBitList** lists;
}AllAnimLists;

// AnimBitList** allLists = {0};
AllAnimLists allLists = {0};

char** DebugAnimListList = NULL;


static TokenizerParseInfo parseAnimList[] = {
    { "",                    TOK_STRUCTPARAM | TOK_STRING(AnimBitList,listName, 0)        },
    { "",                    TOK_STRUCTPARAM | TOK_STRINGARRAY(AnimBitList,listContents)    },
    { "",                    TOK_CURRENTFILE(AnimBitList, fileName)        },
    //--------------------------------------------------------------------------
    { "\n",                    TOK_END,            0 },
    //--------------------------------------------------------------------------
    { "", 0, 0 }
};

static TokenizerParseInfo parseAllAnimLists[] = {
    { "AnimList:",            TOK_STRUCT(AllAnimLists,lists,parseAnimList) },
    { "", 0, 0 }
};

StashTable AnimListHashes = {0};

int cmpAnimListName(const void* lData, const void* rData)
{
    const char** l = (const char**)lData;
    const char** r = (const char**)rData;
    const char* lhs = *l;
    const char* rhs = *r;
    return stricmp(lhs,rhs);
}

void alProcessAnimLists()
{
    int i;

    for(i = 0; i < eaSize(&allLists.lists); i++)
    {
        if(stashFindElement(AnimListHashes, allLists.lists[i]->listName, NULL))
            Errorf("Duplicate AnimList name: %s\n", allLists.lists[i]->listName);
        else
        {
            stashAddPointer(AnimListHashes, allLists.lists[i]->listName, allLists.lists[i], false);
            eaPush(&DebugAnimListList, allLists.lists[i]->listName);
        }
    }

    eaQSort(DebugAnimListList, cmpAnimListName);
}

static void alReloadCallBack(const char* relpath, int when)
{
    fileWaitForExclusiveAccess(relpath);
    errorLogFileIsBeingReloaded(relpath);
    if(ParserReloadFile(relpath, parseAllAnimLists, sizeof(AnimBitList), &allLists, NULL, NULL))
    {
        stashTableClear(AnimListHashes);
        eaSetSize(&DebugAnimListList, 0);
        alProcessAnimLists();
    }
    else
        Errorf("Reload error for Behavior Aliases");
}

void loadAnimLists()
{
    ParserLoadFiles("sequencers/AnimLists", ".al", "animlists.bin", 0, parseAllAnimLists, &allLists, NULL, NULL, NULL);

    AnimListHashes = stashTableCreateWithStringKeys(eaSize(&allLists.lists), StashDefault);

    alProcessAnimLists();

    // set up callback
    if(isDevelopmentMode())
        FolderCacheSetCallback(FOLDER_CACHE_CALLBACK_UPDATE, "sequencers/AnimLists/*.al", alReloadCallBack);
}

void alForEachAnimList(StashElementProcessor proc)
{
    stashForEachElement(AnimListHashes, proc);
}

AnimBitList* alGetList(char* name)
{
    AnimBitList* list;

    stashFindPointer(AnimListHashes, name, &list);

    return list;
}

//name = AnimList name from sequencers/AnimLists/*.al
//varStr = VAR_SHOTGUN
//returns var replacement ex: "weapons/riktiShotgun.fx"
char* alGetVariable(char* name, char* varStr)
{
    AnimBitList* list;
    int i;
    int varLen = (int)strlen(varStr);

    if( !name || !varStr || !AnimListHashes )
        return NULL;

    varLen = (int)strlen(varStr);

    stashFindPointer(AnimListHashes, name, &list);

    if( !list )
        return NULL;

    for(i = 0; i < eaSize(&list->listContents); i++)
    {
        if(!strnicmp(varStr, list->listContents[i], varLen) && list->listContents[i][varLen] == ':')
        {
            return list->listContents[i] + varLen + 1; // VARNAME:path
        }
    }

    return NULL;
}

#include "entity/entity.h"
#include "seq/seq.h"

//This is different than AnimLists -- just being var replacement and no Bits, but it's not bad here.
char * entTypeVarReplacement( char * varName, const SeqType * type )
{
    SequencerVar * seqVar;

    if( !varName || !varName[0] || !type || !type->sequencerVarsStash  ) 
        return NULL;

    seqVar = stashFindPointerReturnPointer( type->sequencerVarsStash, varName );
    if( seqVar )
        return seqVar->replacementName;

    return NULL;
}

char * alGetFxNameFromAnimList( Entity * e, char * varName )
{
    char * fxName = 0;
    
    if( !varName || !e )
        return NULL;

    //AI set animlists get first shot at var replacement
    fxName = alGetVariable( e->aiAnimList, varName );

    //Then villaindef's favorite weapon
    if( !fxName )
        fxName = alGetVariable( e->favoriteWeaponAnimList, varName );

    //Then the Enttype file's vars get a chance
    if( !fxName ) 
        fxName = entTypeVarReplacement( varName, e->seq->type );

    return fxName;
}

void alSetBitsFromAnimList( char * animList, U32 * state )
{
    AnimBitList* list = alGetList(animList);
    int i;

    if(!list)
        return;

    for(i = eaSize(&list->listContents)-1; i >= 0; i--)
    {
        if(!stricmp(list->listContents[i], "variation"))
        {
            F32 randFloat = rule30Float();
            if(randFloat < -0.33)
                seqSetState(state, 1, seqGetStateNumberFromName("VARIATIONA"));
            else if (randFloat < 0.33)
                seqSetState(state, 1, seqGetStateNumberFromName("VARIATIONB"));
            // add nothing 1/3rd of the time
        }
        else
            seqSetState(state, 1, seqGetStateNumberFromName(list->listContents[i]));
    }
}

bool alSetAnimList( char ** oldAnimListPtr, const char * newAnimList, int state[] )
{
    char * oldAnimList = 0;
    bool change = 0;

    //No empty strings allowed
    if( newAnimList && !newAnimList[0] )
        newAnimList = 0;

    oldAnimList = *oldAnimListPtr;

    if( !newAnimList && oldAnimList )
        change = 1;
    else if( !oldAnimList && newAnimList )
        change = 1;
    else if( !oldAnimList && !newAnimList )
        change = 0;
    else if( 0 != stricmp( oldAnimList, newAnimList ) )
        change = 1;

    if( change )
    {
        seqClearState( state );
        if( newAnimList )
        {
            oldAnimList = realloc( oldAnimList, strlen(newAnimList)+1 );
            strcpy( oldAnimList, newAnimList );
            alSetBitsFromAnimList( oldAnimList, state );
        }
        else
        {
            SAFE_FREE(oldAnimList);
        }
    }

    *oldAnimListPtr = oldAnimList;
    return change; //now the new AnimList
}


bool alSetAIAnimListOnEntity( Entity * e, const char * animListName )
{
    e->aiAnimListUpdated = alSetAnimList( &e->aiAnimList, animListName, e->aiAnimListState );
    return e->aiAnimListUpdated;
}

bool alSetFavoriteWeaponAnimListOnEntity( Entity * e, const char * animListName )
{
    alSetAnimList( &e->favoriteWeaponAnimList, animListName, e->favoriteWeaponAnimListState );

    return 0; 
}

void alClientAssignAnimList( Entity * e, const char * newAnimList )
{
    if( newAnimList && newAnimList[0] )
    {
        e->aiAnimList = realloc( e->aiAnimList, strlen( newAnimList )+1 );
        strcpy( e->aiAnimList, newAnimList );
    }
    else
    {
        SAFE_FREE(e->aiAnimList);
    }
}

int alDebugGetAnimListCount()
{
    return eaSize(&DebugAnimListList);
}

char* alDebugGetAnimListStr(int idx)
{
    return DebugAnimListList[idx];
}