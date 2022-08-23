#include "storyarc/storyarcClient.h"
#include <utilitieslib/network/netio.h>
#include <utilitieslib/components/earray.h>
#include "clientcomm/clientcomm.h"
#include "entity/entVarUpdate.h"
#include "UI/uiConsole.h"
#include "UI/uiClue.h"
#include "UI/uiNet.h"
#include <utilitieslib/utils/file.h>

//------------------------------------------------------------------
//------------------------------------------------------------------
// StoryClue
//------------------------------------------------------------------
//------------------------------------------------------------------
StoryClue** storyClues;

void ClueReceive(StoryClue* clue, Packet* pak)
{
    char* s;

    s = pktGetString(pak);
    clue->name = strdup(s);

    if (pktGetBits(pak, 1))
        clue->filename = strdup(pktGetString(pak));

    s = pktGetString(pak);
    clue->displayname = strdup(s);

    s = pktGetString(pak);
    clue->detailtext = strdup(s);

    s = pktGetString(pak);
    clue->iconfile = strdup(s);
}

void ClueListReceive(Packet* pak)
{
    int clueCount;
    int i;

    if(!storyClues)
    {
        eaCreate(&storyClues);
    }
    else
    {
        eaClearEx(&storyClues, StoryClueDestroy);
    }

    // How many clues are we receving?
    clueCount = pktGetBitsPack(pak, 1);

    for(i = 0; i < clueCount; i++)
    {
        StoryClue* clue = StoryClueCreate();
        ClueReceive(clue, pak);
        eaPush(&storyClues, clue);
    }
}

// *********************************************************************************
//  Key clues - just updated on a different schedule from regular clues
// *********************************************************************************
StoryClue** keyClues;

void KeyClueListReceive(Packet* pak)
{
    int clueCount;
    int i;

    if(!keyClues)
    {
        eaCreate(&keyClues);
    }
    else
    {
        eaClearEx(&keyClues, StoryClueDestroy);
    }

    // How many clues are we receving?
    clueCount = pktGetBitsPack(pak, 1);

    for(i = 0; i < clueCount; i++)
    {
        StoryClue* clue = StoryClueCreate();
        ClueReceive(clue, pak);
        eaPush(&keyClues, clue);
    }
}

//------------------------------------------------------------------
//------------------------------------------------------------------
// SouvenirClue
//------------------------------------------------------------------
//------------------------------------------------------------------

void scReceiveAllHeaders(Packet* pak)
{
    int clueCount;
    int i;

    souvenirCluePrepareUpdate();

    // reset flashback cache
    flashbackListClear();

    // Get the number of clues in this update.
    clueCount = pktGetBitsPack(pak, 1);

    // For each clue to be received...
    for(i = 0; i < clueCount; i++)
    {
        //    Create a new SouvenirClue.
        SouvenirClue* clue = scCreate();

        scReceiveHeader(clue, pak);

        souvinerClueAdd( clue );
    }
}

void scReceiveHeader(SouvenirClue* clue, Packet* pak)
{
    //    Get and set the clue uid.
    clue->uid = pktGetBitsPack(pak, 1);

    // Get the filename(only sent in dev mode)
    if (pktGetBits(pak, 1))
        clue->filename = strdup(pktGetString(pak));

    //    Get and set the clue displayName.
    clue->displayName = strdup(pktGetString(pak));

    //    Get and set the clue iconName.
    clue->icon = strdup(pktGetString(pak));
}

// Called when the player attempts to expand a souvenir clue without its details.
void scRequestDetails(SouvenirClue* clue)
{
    if(!clue)
        return;

    START_INPUT_PACKET(pak, CLIENTINP_REQUEST_SOUVENIR_INFO);
    // Send the uid of the clue for which the detail is being requested.
    pktSendBitsPack(pak, 1, clue->uid);
    END_INPUT_PACKET
}

void scReceiveDetails(Packet* pak)
{
    int uid;
    char* description;

    // Get the uid of the clue detail being updated.
    uid = pktGetBitsPack(pak, 1);

    // Get the description of the clue.
    description = pktGetString(pak);

    updateSouvenirClue( uid, description );
}

