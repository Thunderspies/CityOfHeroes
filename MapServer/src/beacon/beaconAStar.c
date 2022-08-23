
#include <limits.h>
#include <stdlib.h>
#include <memory.h>
#include <utilitieslib/assert/assert.h>
#include <stdio.h>
#include "beaconAStar.h"
#include <utilitieslib/components/MemoryPool.h>
#include <utilitieslib/stdtypes.h>
#include "dbcomm/dbcomm.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>

#define ASTAR_ASSERTS 0

//-------------------------------------------------------------------
// AStarInfo
//-------------------------------------------------------------------

MP_DEFINE(AStarInfo);

static void initAStarVars(){
    MP_CREATE(AStarInfo, db_state.is_static_map?10000:1024);
}

static AStarInfo* createAStarInfo(){
    AStarInfo* info = MP_ALLOC(AStarInfo);

    // each node starts with infinite cost
    info->costSoFar = INT_MAX;
    
    return info;
}

static void destroyAStarInfo(AStarInfo* info){
    MP_FREE(AStarInfo, info);
}

//-------------------------------------------------------------------
// AStarSearchData
//-------------------------------------------------------------------

MP_DEFINE(AStarSearchData);

AStarSearchData* createAStarSearchData(){
    AStarSearchData* search;
    
    MP_CREATE(AStarSearchData, 1);
    
    search = MP_ALLOC(AStarSearchData);
    
    return search;
}

void initAStarSearchData(AStarSearchData* search){
}

void clearAStarSearchDataTempInfo(AStarSearchData* search){
    if(search){
        search->openSet.size = 0;
    }
}

void destroyAStarSearchData(AStarSearchData* search){
    if(search){
        clearAStarSearchDataTempInfo(search);
        
        MP_FREE(AStarSearchData, search);
    }
}

int AStarInfoCompare(AStarInfo* info1, AStarInfo* info2){
    if(info1->totalCost < info2->totalCost)
        return 1;

    if(info1->totalCost == info2->totalCost)
        return 0;

    return -1;
}

static void AStarUpdateIndex(AStarInfo* info, int index){
    info->queueIndex = index;
}

static int staticCurTotalCost;
static AStarOpenSet curOpenSet;

static void AStarQueuePercolateUp(AStarInfo* targetInfo){
    int curIndex = targetInfo->queueIndex;

    while(1){
        // Compare the current perc target against its parent.
        // If the parent is smaller, heap order has been violated.
        // Restore the order by swapping the target and its parent.

        // Stop further processing if the top of the heap has been reached.
        if(1 != curIndex){
            int parentIndex = curIndex >> 1;
            AStarInfo* parentInfo;
            
            parentInfo = curOpenSet.nodes[parentIndex];
            
            if(parentInfo->totalCost > staticCurTotalCost){
                // Put parent where the child was.
                
                curOpenSet.nodes[curIndex] = parentInfo;
                parentInfo->queueIndex = curIndex;
                
                // Go to parent node.
                
                curIndex = parentIndex;
                
                continue;
            }
        }

        // heap is okay, so put the target value in and we're done.

        curOpenSet.nodes[curIndex] = targetInfo;
        
        targetInfo->queueIndex = curIndex;
        
        return;
    }
}

void AStarQueuePush(AStarInfo* newNode){
    newNode->queueIndex = curOpenSet.size;

    if(curOpenSet.size == curOpenSet.maxSize){
        dynArrayAddStruct(&curOpenSet.nodes, &curOpenSet.size, &curOpenSet.maxSize);
    }else{
        curOpenSet.size++;
    }
    
    AStarQueuePercolateUp(newNode);
}

static void AStarQueuePercolateTopDown(){
    int            curIndex = 1;
    AStarInfo*    targetInfo = curOpenSet.nodes[1];
    
    staticCurTotalCost = targetInfo->totalCost;

    while(1){
        int l_index;
        int r_index;
        
        // Consider the larger child as a possible swap target.

        // If the child is larger, the heap order has been 
        // violated.  Restore heap order by swapping
        // the child with the parent.

        // Get left child index.
        
        l_index = curIndex << 1;

        // Get right child index.
        
        r_index = l_index | 1;

        // Decide which child might be the possible swap target.
        
        if(l_index < curOpenSet.size){
            AStarInfo* swapInfo;
            int swapIndex;
            int swapCost;
            
            // If the right child doesn't exist, the only possible swap
            // target is the left child.
            
            if(r_index >= curOpenSet.size){
                swapIndex = l_index;
                swapInfo = curOpenSet.nodes[swapIndex];
                swapCost = swapInfo->totalCost;
            }else{
                AStarInfo* l_info = curOpenSet.nodes[l_index];
                AStarInfo* r_info = curOpenSet.nodes[r_index];
                int l_cost = l_info->totalCost;
                int r_cost = r_info->totalCost;
                
                if(l_cost < r_cost){
                    swapIndex = l_index;
                    swapInfo = l_info;
                    swapCost = l_cost;
                }else{
                    swapIndex = r_index;
                    swapInfo = r_info;
                    swapCost = r_cost;
                }
            }

            if(swapCost < staticCurTotalCost){
                // Put the child in the parent position.

                curOpenSet.nodes[curIndex] = swapInfo;
                swapInfo->queueIndex = curIndex;
                
                // Update new perc target.
                
                curIndex = swapIndex;
                
                continue;
            }
        }

        // Heap is okay, so put the target value in and we're done.

        curOpenSet.nodes[curIndex] = targetInfo;
        targetInfo->queueIndex = curIndex;
    
        // Heap order not violated... order restored...
        
        return;
    }
}

AStarInfo* AStarQueuePop(){
    AStarInfo* topItem;

    // If there is nothing but the dummy element in the queue, return nothing...
    
    if(curOpenSet.size < 2)
        return NULL;

    // Remove the item at the top of the queue.
    
    topItem = curOpenSet.nodes[1];
    
    #if ASTAR_ASSERTS
    {
        int i;
    
        assert(topItem->queueIndex == 1);
    
        for(i = 2; i < curOpenSet.size; i++){
            assert(curOpenSet.nodes[i] != topItem);
        }
    }
    #endif
        
    topItem->queueIndex = 0;

    // Move the item with the lowest ranking to the top.
    
    curOpenSet.nodes[1] = curOpenSet.nodes[--curOpenSet.size];

    // Restore heap order by percolating the lowest ranking item down.
    
    if(curOpenSet.size > 1){
        AStarQueuePercolateTopDown();
    }
    
    return topItem;
}

static int staticNodeAStarInfoOffset;

#define NODE_TO_ASTARINFO(node) (*(AStarInfo**)(((char*)node)+staticNodeAStarInfoOffset))

static void cleanUpAStar(AStarSearchData* searchData, AStarInfo* infoListHead){
    searchData->searching = 0;

    clearAStarSearchDataTempInfo(searchData);
    
    // Reset the AStarInfo pointers to NULL.
    
    while(infoListHead){
        AStarInfo* next = infoListHead->next;
        NODE_TO_ASTARINFO(infoListHead->ownerNode) = NULL;
        MP_FREE(AStarInfo, infoListHead);
        infoListHead = next;
    }
    
    searchData->openSet = curOpenSet;
}

//-------------------------------------------------------------------
// The masterpiece generic AStar search function.
//-------------------------------------------------------------------

void AStarSearch(AStarSearchData* searchData, NavSearchFunctions* functions){
    void* sourceNode = searchData->sourceNode;
    void* targetNode = searchData->targetNode;
    AStarInfo* curInfo;
    AStarInfo* sourceInfo;
    AStarInfo* bestInfo = NULL;
    int bestCostToDest;
    int curCost;
    int i;
    NavSearchFunctions localFuncs;
    int steps = searchData->steps;
    U32 exploredNodeCount = 0;
    U32 checkedConnectionCount = 0;
    int findClosestOnFail = searchData->findClosestOnFail;
    AStarInfo* infoListHead = NULL;
    
    staticNodeAStarInfoOffset = searchData->nodeAStarInfoOffset;

    initAStarVars();

    memcpy(&localFuncs, functions, sizeof(localFuncs));
    
    if(0){
        printf(    "\n\n\n********** FINDING PATH ****************************\nDestination: %s\n",
                localFuncs.getNodeInfo ? localFuncs.getNodeInfo(targetNode, NULL, NULL) : "No Info");
    }
        
    // Initialize storage required by the search.
    
    curOpenSet = searchData->openSet;

    if(!curOpenSet.maxSize){
        curOpenSet.nodes = dynArrayAddStructs(&curOpenSet.nodes, &curOpenSet.size, &curOpenSet.maxSize, 512);
        curOpenSet.size = 1;
    }
    
    if(!searchData->searching){
        searchData->searching = 1;
        searchData->pathWasOutput = 0;
        
        // Add starting node as the only node in the open set.
        
        sourceInfo = createAStarInfo();
        sourceInfo->ownerNode = sourceNode;
        sourceInfo->costSoFar = 0;
        sourceInfo->totalCost = localFuncs.costToTarget(searchData, NULL, sourceNode, NULL);
        sourceInfo->closed = 0;
        
        bestInfo = sourceInfo;
        bestCostToDest = sourceInfo->totalCost;

        curOpenSet.size = 1;
        
        NODE_TO_ASTARINFO(sourceNode) = sourceInfo;
        
        sourceInfo->next = infoListHead;
        infoListHead = sourceInfo;

        AStarQueuePush(sourceInfo);
    }
        
    // Loop until either a path is found or all possibilities have been exhausted.
    
    while(1){
        void* connBuffer[1000];
        void* nodeBuffer[ARRAY_SIZE(connBuffer)];
        int position;
        
        // Grab the node with the lowest cost from the priority queue.
        
        sourceInfo = AStarQueuePop();
        
        exploredNodeCount++;
                
        // If there is no nodes in the queue then the search ran out
        // of connections, thus the destination is unreachable.
        
        if(!sourceInfo){
            // Clean up and exit.
            
            if(findClosestOnFail){
                if(localFuncs.outputPath){
                    localFuncs.outputPath(searchData, bestInfo);
                }

                searchData->pathWasOutput = 1;
            }else{
                searchData->pathWasOutput = 0;
            }

            searchData->exploredNodeCount = exploredNodeCount;
            searchData->checkedConnectionCount = checkedConnectionCount;
            
            cleanUpAStar(searchData, infoListHead);
            
            return;
        }
        
        #if ASTAR_ASSERTS
            assert(!sourceInfo->queueIndex && (curOpenSet.size < 2 || curOpenSet.nodes[1] != sourceInfo));
        #endif
        
        sourceNode = sourceInfo->ownerNode;
        
        // Close the node so it can never be visited again.
        
        #if ASTAR_ASSERTS
            assert(!sourceInfo->closed && !sourceInfo->queueIndex);
        #endif

        sourceInfo->closed = 1;
        
        // Call the closeNode callback function, so the node can
        // be retrofitted with permanent information if necessary.
        
        if(localFuncs.closeNode){
            localFuncs.closeNode(    sourceInfo->parentInfo ? sourceInfo->parentInfo->ownerNode : NULL,
                                    sourceInfo->connFromPrevNode,
                                    sourceInfo->ownerNode);
        }
        
        // If the node is the destination node, search is complete.

        if(sourceNode == targetNode){
            if(localFuncs.reportClosedInfo){
                localFuncs.reportClosedInfo(sourceInfo, checkedConnectionCount);
            }

            if(localFuncs.outputPath){
                localFuncs.outputPath(searchData, sourceInfo);
            }
            
            searchData->pathWasOutput = 1;

            searchData->exploredNodeCount = exploredNodeCount;
            searchData->checkedConnectionCount = checkedConnectionCount;

            cleanUpAStar(searchData, infoListHead);

            return;
        }
        
        // Update closest node if I've found a closer one.
        
        if(findClosestOnFail){
            int costToDest = sourceInfo->totalCost - sourceInfo->costSoFar;
            
            if(costToDest < bestCostToDest){
                bestCostToDest = costToDest;
                bestInfo = sourceInfo;
            }
        }

        //    Find all connected nodes.

        position = 0;
        
        while(1){
            int success;
            int count = ARRAY_SIZE(connBuffer);
            void** connections = connBuffer;
            void** nodes = nodeBuffer;
            
            PERFINFO_AUTO_START("Get Connections", 1);
                success = localFuncs.getConnections(searchData, sourceNode, &connections, &nodes, &position, &count);
            PERFINFO_AUTO_STOP();
            
            if(!success){
                break;
            }            
            
            PERFINFO_AUTO_START("Analyze Connections", 1);
                for(i = 0; i < count; i++){
                    void*    connection = connections[i];
                    int        alreadyInOpenSet;
                    void*    curNode;
                    int        newCurCSF;        // "cost so far" for current node.
                    
                    if(!connection){
                        continue;
                    }
                    
                    curNode = nodes[i];
                    
                    #if ASTAR_ASSERTS
                        assert(curNode != sourceNode);
                    #endif
                    
                    checkedConnectionCount++;

                    // Make sure we have the proper info attached to this node to continue with the search.

                    curInfo = NODE_TO_ASTARINFO(curNode);
                    
                    if(!curInfo){
                        // No current info, so check if the beacon is reachable before adding to the list.
                        
                        curCost = localFuncs.cost(searchData, sourceNode, connection);

                        if(INT_MAX == curCost){
                            continue;
                        }
                        
                        // The node doesn't exist in this search yet, so add it to the queue.
                        
                        curInfo = createAStarInfo();
                        curInfo->ownerNode = curNode;

                        NODE_TO_ASTARINFO(curNode) = curInfo;
                        
                        curInfo->next = infoListHead;
                        infoListHead = curInfo;
                        
                        alreadyInOpenSet = 0;
                    }
                    else if(curInfo->queueIndex){
                        // Already in the queue, so update the cost.
                        
                        curCost = localFuncs.cost(searchData, sourceNode, connection);

                        if(INT_MAX == curCost){
                            continue;
                        }
                        
                        // Node is already in the open set.

                        alreadyInOpenSet = 1;
                    }
                    else{
                        #if ASTAR_ASSERTS
                            assert(curInfo->closed);
                        #endif
                        
                        // Node must be closed.
                        
                        continue;
                    }

                    #if ASTAR_ASSERTS
                        assert(!curInfo->closed);
                    #endif

                    newCurCSF = sourceInfo->costSoFar + curCost;
                    
                    if(newCurCSF < 0){
                        newCurCSF = INT_MAX;
                    }

                    // The beacon is already in the open set, compare it against the stored g value.
                    //  If the current g value is larger, we've not found a better path to the beacon.
                    //  Leave the beacon alone.

                    //if(alreadyInOpenSet && curInfo->costSoFar < newCurCSF){
                    //    continue;
                    //}

                    //curInfo->costSoFar = newCurCSF;

                    // If we reached this point, either current g is smaller than previous or the node
                    //   is not in either sets.  Either way, we want to calcuate and update some info.
                    // Find estimated cost (h) value (distance to destination).
                    // Find total cost (f) value ( g + h ).

                    staticCurTotalCost = newCurCSF + localFuncs.costToTarget(searchData, sourceNode, curNode, connection);
                    
                    if(staticCurTotalCost < 0){
                        staticCurTotalCost = INT_MAX;
                    }
                    
                    if(!alreadyInOpenSet || staticCurTotalCost < curInfo->totalCost){
                        curInfo->totalCost = staticCurTotalCost;
                        curInfo->costSoFar = newCurCSF;
                    }else{
                        continue;
                    }
                    
                    // Update the node's "parent" to be the current source node (for path construction).
                    
                    #if ASTAR_ASSERTS
                        assert(curInfo != sourceInfo);
                    #endif

                    curInfo->parentInfo = sourceInfo;
                    curInfo->connFromPrevNode = connection;

                    // If the node is the destination node, search is complete.

                    if(alreadyInOpenSet){
                        // Node is already in the open set, so percolate it up because its cost changed.
                        
                        AStarQueuePercolateUp(curInfo);
                    }else{
                        // Node wasn't in the open set yet, so add it.
                        
                        AStarQueuePush(curInfo);
                    }
                }
            PERFINFO_AUTO_STOP();
        }
        
        #if ASTAR_ASSERTS
        {
            for(i = 1; i < curOpenSet.size; i++){
                int l = i << 1;
                int r = l | 1;
                
                assert(l >= curOpenSet.size || curOpenSet.nodes[l]->totalCost >= curOpenSet.nodes[i]->totalCost);
                assert(r >= curOpenSet.size || curOpenSet.nodes[r]->totalCost >= curOpenSet.nodes[i]->totalCost);
            }
        }
        #endif

        if(localFuncs.reportClosedInfo){
            localFuncs.reportClosedInfo(sourceInfo, checkedConnectionCount);
        }

        //if(steps > 0){
        //    if(!--steps){
        //        return;
        //    }
        //}
    }
    
    // Getting here is impossible, so assert for fun.  Maybe format the boot sector.  Whatever works.

    assert(0);
}

