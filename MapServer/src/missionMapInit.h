#ifndef MISSIONMAPINIT_H
#define MISSIONMAPINIT_H

#include <utilitieslib/components/ArrayOld.h>
#include "group/grouputil.h"
#include <utilitieslib/components/HashTableStack.h>

void initMap(int forceReload);
int groupDefIsSAOverride(GroupDef* def);    // MOVE ME!
int setMissionType(char* name);
Array* mmicGetSpawnAreaStack(GroupDefTraverser* traverser);
HashTableStack mmicGetPropertyStack(GroupDefTraverser* traverser);

void initMapSetup();
void initMapExtractActivationCandidates();
void initMapSortActivationCandidatesByType();
void initMapActivateChosenScenarios();
void initMapActivateSpawnAreas();
void initMapCleanup();

#endif
