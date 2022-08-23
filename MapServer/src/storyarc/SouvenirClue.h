/*\
 *
 *    SouvenirClue.h/c - Copyright 2003, 2004 Cryptic Studios
 *        All Rights Reserved
 *        Confidential property of Cryptic Studios
 *
 *    Souvenier clues hang on the player forever and are a
 *    memento of their mission.  They have some limitations
 *    compared to regular clues
 *
 */

#ifndef SOUVENIRCLUE_H
#define SOUVENIRCLUE_H

#include "storyarcinterface.h"
#include "storyarc/storyarcCommon.h"

//-------------------------------------------------------------
// Definition reading
//    Server only
//-------------------------------------------------------------
void scLoad();
int scVerifyClueNames(char** clueNames);

//-------------------------------------------------------------
// SouvenirClue lookup
//    Server only.
//-------------------------------------------------------------
const SouvenirClue* scFindClue(char* name);


//-------------------------------------------------------------
// SouvenirClue reward
//    Server only.
//-------------------------------------------------------------

// Adds souvenir clue to a StoryInfo structure,
// causes all SouvenirClues to be sent the next update,
// and inform the player he got a new SouvenirClue.
int scReward(const SouvenirClue* clue, StoryInfo* storyInfo);
int scFindAndReward(const char* clueName, StoryInfo* storyInfo);
void scApplyToEnt(Entity* e, const char** clues);
void scApplyToDbId(int db_id, const char** clues);
void scRemoveDebug(Entity* e, const char* clueName);


//-------------------------------------------------------------
// SouvenirClue transmission.
//-------------------------------------------------------------

void scSendAllHeaders(Entity* player);
void scSendChanged(Entity* e);

// Done when the player requests the details about a SouvenirClue.
void scSendDetails(const SouvenirClue* clue, Entity* player);
void scRequestDetails(Entity* player, int requestedIndex);


//-------------------------------------------------------------
// SouvenirClue utilities
//-------------------------------------------------------------
int scFindIndex(const char* name);
const char* scGetNameByIndex(int index);
int scGetIndex(SouvenirClue* clue);
const SouvenirClue* scGetByIndex(int index);

const SouvenirClue* scFindByName(const char* name);
const char* scGetName(const SouvenirClue* clue);
const char* scIconFile(const SouvenirClue* clue);


//-------------------------------------------------------------
// SouvenirClue save/load
//-------------------------------------------------------------
// Add entries to ent_line_desc for saving and loading
// Add to growStoryInfoArrays() and shrinkStoryInfoArrays() to grow and shrink SouvenirClue array
// Have attributeNames() output SouvenirClue 


//-------------------------------------------------------------
// SouvenirClue display
//    Client only
//-------------------------------------------------------------




#endif // SOUVENIRCLUE_H