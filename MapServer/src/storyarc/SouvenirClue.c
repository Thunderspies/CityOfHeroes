/*\
 *
 *    SouvenirClue.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Souvenir clues hang on the player forever and are a
 *    memento of their mission.  They have some limitations
 *    compared to regular clues
 *
 */

#include "SouvenirClue.h"
#include "storyarcprivate.h"
#include "player/badges_server.h"
#include "comm_game.h"
#include "cmdparse/cmdserver.h"
#include "dbcomm/logcomm.h"
#include "entity/entity.h"

typedef struct
{
    const SouvenirClue** clues;
} SouvenirClueList;

static bool scVerify(TokenizerParseInfo pti[], SouvenirClueList* list);


//-------------------------------------------------------------
// Definition reading
//    Server only
//-------------------------------------------------------------
SHARED_MEMORY SouvenirClueList gSouvenirClueList = {0};

TokenizerParseInfo ParseSouvenirClue[] =
{
    { "{",                TOK_START,            0},
    { "}",                TOK_END,            0},

    { "",                TOK_STRUCTPARAM | TOK_STRING(SouvenirClue, name, 0) },
    { "",                TOK_CURRENTFILE(SouvenirClue, filename) },
    { "Name",            TOK_STRING(SouvenirClue, displayName, 0) },
    { "Icon",            TOK_STRING(SouvenirClue, icon, 0) },
    { "DetailString",    TOK_STRING(SouvenirClue, description, 0) },

    { "", 0, 0 }
};

TokenizerParseInfo ParseSouvenirClueList[] =
{
    { "SouvenirClueDef",    TOK_STRUCT(SouvenirClueList, clues, ParseSouvenirClue) },
    { "", 0, 0 }
};

static bool scPostProcess(TokenizerParseInfo pti[], SouvenirClueList* list)
{
    // Fill in the uid of all clues.
    int i;
    for(i = 0; i < eaSize(&list->clues); i++)
    {
        SouvenirClue* clue = cpp_const_cast(SouvenirClue*)(list->clues[i]);
        clue->uid = i;
    }
    return true;
}

// #define SOUVENIR_CLUE_DIR "Scripts.loc/SouvenirClues"
#define SOUVENIR_CLUE_DIR "Scripts.loc"

void scLoad()
{
    int flags = PARSER_SERVERONLY;
    // Destroy any loaded SouvenirClues.
    if(gSouvenirClueList.clues)
    {
        ParserUnload(ParseSouvenirClueList, &gSouvenirClueList, sizeof(gSouvenirClueList));
        flags = PARSER_SERVERONLY | PARSER_FORCEREBUILD;
    }

    // Have textparser load all the SouvenirClues.
    ParserLoadFilesShared("SM_SouvenirClues.bin", SOUVENIR_CLUE_DIR, ".sc", "SouvenirClues.bin", flags, ParseSouvenirClueList, &gSouvenirClueList, sizeof(gSouvenirClueList), NULL, NULL, scVerify, scPostProcess, NULL);

}

static bool scVerify(TokenizerParseInfo pti[], SouvenirClueList* list)
{
    int i;
    const SouvenirClue** clues = list->clues;
    int noErrors = true;

    // Make sure there are no duplicate names.
    for(i = 0; i < eaSize(&clues); i++)
    {
        int j;

        for(j = i+1; j < eaSize(&clues); j++)
        {
            if(clues[i]->name && clues[j]->name)
            {
                if(stricmp(clues[i]->name, clues[j]->name) == 0)
                {
                    noErrors = 0;
                    ErrorFilenamef(clues[j]->name, "SouvenirClue: Duplicate clue name: \"%s\"\n", clues[j]->name);
                }
            }
        }

        // Make sure a definition name exists.
        if(!clues[i]->name)
        {
            noErrors = 0;
            ErrorFilenamef(clues[j]->name, "SouvenirClue: Missing definition name for clue");
        }

        // Make sure a display name exists.
        if(!clues[i]->displayName)
        {
            noErrors = 0;
            ErrorFilenamef(clues[j]->name, "SouvenirClue: Missing displayname for clue \"%s\"\n", clues[i]->name);
        }

        // Make sure a valid icon exists.
        if(!clues[i]->icon)
        {
            noErrors = 0;
            ErrorFilenamef(clues[j]->name, "SouvenirClue: Missing icon for clue \"%s\"\n", clues[i]->name);
        }

        // Make sure the detail description exists.
        if(!clues[i]->description)
        {
            noErrors = 0;
            ErrorFilenamef(clues[j]->name, "SouvenirClue: Missing description for clue \"%s\"\n", clues[i]->name);
        }
    }

    return noErrors;
}

//-------------------------------------------------------------
// SouvenirClue lookup
//    Server only.
//-------------------------------------------------------------
const SouvenirClue* scFindClue(char* name)
{
    // Find index of the specified clue.
    int index;;

    if(!name)
        return NULL;

    index = scFindIndex(name);

    // If the specified clue cannot be found, return an error.
    if(index == -1)
        return NULL;

    // Get the specified clue by index.
    return scGetByIndex(index);
}


//-------------------------------------------------------------
// SouvenirClue reward
//    Server only.
//-------------------------------------------------------------

int scReward(const SouvenirClue* clue, StoryInfo* storyInfo)
{
    const SouvenirClue*** clueArray;

    // Adds souvenir clue to the player's StoryInfo structure.
    clueArray = &storyInfo->souvenirClues;

    // Does the player already have this clue?
    if(-1 != eaFind(clueArray, clue))
        return 0;
    eaPushConst(clueArray, clue);

    // Set flag so that all SouvenirClues to be sent the next update.
    storyInfo->souvenirClueChanged = 1;

    // Verify that clues per player not exceeded.
    if(eaSize(clueArray) > SOUVENIRCLUES_PER_PLAYER)
    {
        Errorf("Souvenir clue array exceeded");
        log_printf("SClue", "Souvenir clue array exceeded");
    }

    return 1;
}

int scFindAndReward(const char* clueName, StoryInfo* storyInfo)
{
    const SouvenirClue* clue;
    int result;

    clue = scFindByName(clueName);
    if(!clue)
        return 0;

    result = scReward(clue, storyInfo);

    return result;
}

// Debug Only function to remove Souvenir Clues
void scRemoveDebug(Entity* e, const char* clueName)
{
    // NOTE: this does NOT lock story info, but changes the souvenir flag anyway
    const SouvenirClue* clue;
    const SouvenirClue*** clueArray;
    clueArray = &e->storyInfo->souvenirClues;

    clue = scFindByName(clueName);
    if(!clue)
        return;

    if(eaFindAndRemove(clueArray, clue))
        e->storyInfo->souvenirClueChanged = 1;
}

void scApplyToEnt(Entity* e, const char** clues)
{
    int i, n = eaSize(&clues);
//    if (PlayerInTaskForceMode(e)) return; // need to issue SC is Taskforce mode for Flashback
    for(i = 0; i < n; i++)
    {
        // NOTE: this does NOT lock story info, but changes the souvenir flag anyway
        scFindAndReward(clues[i], e->storyInfo);
        badge_RecordSouvenirClue(e, clues[i]);

        LOG_ENT( e, LOG_REWARDS, LOG_LEVEL_IMPORTANT, 0, "[Tbl]:Clue (%s)", clues[i]);
    }
}

void scApplyToDbId(int db_id, const char** clues)
{
    int i;
    Entity *e;

    e = entFromDbId(db_id);
    if (e)
    {
        scApplyToEnt(e, clues);
    }
    else
    {
        char givecmd[1024];

        sprintf(givecmd, "scapply %i ", db_id);
        for (i = 0; i < eaSize(&clues); i++)
        {
            strcat(givecmd, clues[i]);
            strcat(givecmd, ":");
        }
        serverParseClient(givecmd, NULL);
    }
}

//-------------------------------------------------------------
// SouvenirClue transmission.
//-------------------------------------------------------------

// Done on initial player entity send and each time a new clue is added to the player.
void scSendAllHeaders(Entity* player)
{
    int i;
    const SouvenirClue** clues;
    int iSize;
    StoryInfo* storyInfo = StoryInfoFromPlayer(player);

    if(!player || ENTTYPE(player) != ENTTYPE_PLAYER || !storyInfo || !storyInfo->souvenirClues)
        return;

    clues = storyInfo->souvenirClues;

    iSize = eaSize(&clues);
    // Send the number of SouvenirClues to be sent.
    START_PACKET(pak, player, SERVER_SOUVENIRCLUE_UPDATE)
    pktSendBitsPack(pak, 1, iSize);

    // For each SouvenirClue the player has...
    for(i = 0; i < iSize; i++)
    {
        const SouvenirClue* clue = clues[i];
        const char* str;

        //  Send the souvenir uid.
        pktSendBitsPack(pak, 1, clue->uid);

        // Filename for the clue
        pktSendBits(pak, 1, server_state.clickToSource);
        if (server_state.clickToSource)
            pktSendString(pak, clue->filename);

        //    Translate and send the souvenir displayName.
        str = saUtilTableAndEntityLocalize(clue->displayName, 0, player);
        pktSendString(pak, str);

        //    Send the souvenir icon name.
        //pktSendString(pak, clue->icon);
        //    Always send the generic clue icon for now.
        pktSendString(pak, scIconFile(clue));
    }
    END_PACKET
}

void scSendChanged(Entity* e)
{

    if(!e || !e->storyInfo || !e->storyInfo->souvenirClueChanged)
        return;
    scSendAllHeaders(e);
    e->storyInfo->souvenirClueChanged = 0;
}

// Done when the player requests the details about a SouvenirClue.
void scSendDetails(const SouvenirClue* clue, Entity* player)
{
    const char* str;
    if(!clue || !player || !ENTTYPE(player) == ENTTYPE_PLAYER || !player->storyInfo)
        return;

    // Send the souvenir uid.
    START_PACKET(pak, player, SERVER_SOUVENIRCLUE_DETAIL);

    // This identifies the souvenir being updated.
    pktSendBitsPack(pak, 1, clue->uid);

    // Translante and send the souvenir description.
    str = saUtilTableAndEntityLocalize(clue->description, 0, player);
    pktSendString(pak, str);

    END_PACKET
}

void scRequestDetails(Entity* player, int requestedIndex)
{
    const SouvenirClue* clue = scGetByIndex(requestedIndex);
    StoryInfo* storyInfo = StoryInfoFromPlayer(player);

    // If the player actually has the souvenir clue, then send its details.
    if(storyInfo && storyInfo->souvenirClues)
    {
        int i;

        i = eaFind(&storyInfo->souvenirClues, clue);
        if(i != -1)
            scSendDetails(clue, player);
    }
}

//-------------------------------------------------------------
// SouvenirClue save/load
//-------------------------------------------------------------
// TODO:
// Add entries to ent_line_desc for saving and loading
// Add to growStoryInfoArrays() and shrinkStoryInfoArrays() to grow and shrink SouvenirClue array
// Have attributeNames() output SouvenirClue names

int scFindIndex(const char* name)
{
    int i;

    // Perform a linear search on the SouvenirClues array...
    for(i = eaSize(&gSouvenirClueList.clues)-1; i >= 0; i--)
    {
        //    Look for a clue with the matching name.
        if(stricmp(name, gSouvenirClueList.clues[i]->name) == 0)
            //    Return the index.
            return i;
    }

    // If a clue with the matching name is not found, return -1.
    return -1;
}

const char* scGetNameByIndex(int index)
{
    const SouvenirClue* clue = eaGetConst(&gSouvenirClueList.clues, index);
    if(clue)
        return clue->name;

    return NULL;
}

int scGetIndex(SouvenirClue* clue)
{
    // Return the UID of the clue.
    return clue->uid;
}

const SouvenirClue* scGetByIndex(int index)
{
    // Return the SouvenirClue at the specified index.
    return eaGetConst(&gSouvenirClueList.clues, index);
}

const SouvenirClue* scFindByName(const char* name)
{
    int index = scFindIndex(name);
    if(index == -1)
        return NULL;

    return gSouvenirClueList.clues[index];
}

const char* scGetName(const SouvenirClue* clue)
{
    return clue->name;
}

const char* scIconFile(const SouvenirClue* clue)
{
    return "icon_clue_generic.tga"; // not supported yet
}

//-------------------------------------------------------------
// SouvenirClue display
//    Client only
//-------------------------------------------------------------
//TODO:
