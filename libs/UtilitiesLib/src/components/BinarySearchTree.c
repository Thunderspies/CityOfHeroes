#include "utilitieslib/components/BinarySearchTree.h"

#define NUM_HANDLES_TO_ADD_TO_POOL_AT_ONCE (1 << 10)

static bool sBSTSystemInitted = false;

BSTNodeHandle gFirstFreeNodeHandle = BSTNODE_HANDLE_INVALID;
BSTNode *gpNodePool = NULL;
static int siNumHandlesInPool;


BSTNodeHandle BSTNode_AddNode(BinarySearchTree *pTree, BSTNodeHandle hNode, void *pData, intptr_t iKey);
BSTNodeHandle BSTSystem_GetNewNode(void *pData, intptr_t iKey, BSTNodeHandle hParent, bool bIsLeft, bool bStartBlack);
void BSTSystem_ReleaseNode(BSTNodeHandle hNode);
void BSTNode_AddSubTree(BSTNodeHandle hParent, BSTNodeHandle hTree, bool bAddOnLeft);
void BSTNode_RecurseDecNodeCount(BSTNodeHandle hNode);

void RebalanceAroundAddedNode(BinarySearchTree *pTree, BSTNodeHandle hNode);
void BST_CheckValidity(BinarySearchTree *pTree);
void BST_DumpTree(BinarySearchTree *pTree);


#if 0
void AssertHandleValidity(BSTNodeHandle hNode)
{
    assert(hNode == BSTNODE_HANDLE_INVALID || hNode >= 0 && (int)hNode < siNumHandlesInPool);
}

void AssertNodeValidity(BinarySearchTree *pTree, BSTNodeHandle hNode)
{
    BSTNode *pParent, *pNode, *pLeftChild, *pRightChild;

    AssertHandleValidity(hNode);

    if (hNode == BSTNODE_HANDLE_INVALID)
    {
        return;
    }

    pNode = BSTNodeFromHandle(hNode);

    AssertHandleValidity(pNode->hLeftChild);
    AssertHandleValidity(pNode->hParent);
    AssertHandleValidity(pNode->hRightChild);

    pParent = BSTNodeFromHandle(pNode->hParent);
    pLeftChild = BSTNodeFromHandle(pNode->hLeftChild);
    pRightChild = BSTNodeFromHandle(pNode->hRightChild);

    if (pParent)
    {
        if (pNode->bIsLeftChild)
        {
            assert(pParent->hLeftChild == hNode);
        }
        else
        {
            assert(pParent->hRightChild == hNode);
        }
    }
    else
    {
        assert(pTree->hTopNode == hNode);
    }

    if (pLeftChild)
    {
        assert(pLeftChild->hParent == hNode);
        assert(pLeftChild->bIsLeftChild);
    }

    if (pRightChild)
    {
        assert(pRightChild->hParent == hNode);
        assert(!pRightChild->bIsLeftChild);
    }
}
#else
#define AssertNodeValidity(tree, node) {}
#endif

void BST_Init(BinarySearchTree *pTree)
{
    pTree->hTopNode = BSTNODE_HANDLE_INVALID;
    pTree->bVerbose = false;
}


BSTNodeHandle BST_AddNode(BinarySearchTree *pTree, void *pData, intptr_t iKey)
{
    BSTNodeHandle hRetVal;

    if (pTree->hTopNode != BSTNODE_HANDLE_INVALID)
    {
        hRetVal = BSTNode_AddNode(pTree, pTree->hTopNode, pData, iKey);
    }
    else
    {
        hRetVal =  pTree->hTopNode = BSTSystem_GetNewNode(pData, iKey, BSTNODE_HANDLE_INVALID, false, true);
        
    }

    if (pTree->bVerbose)
    {
        printf("Just added node %d\n", iKey);
        BST_DumpTree(pTree);
        printf("\n\n\n");
    }

    return hRetVal;
}


//this function really wants to recurse, but can't do so without savagely overflowing the stack
BSTNodeHandle BSTNode_AddNode(BinarySearchTree *pTree, BSTNodeHandle hNode, void *pData, intptr_t iKey)
{

    do
    {
        BSTNode *pNode = BSTNodeFromHandle(hNode);

        if (iKey > pNode->data.iKey)
        {
            if (pNode->hRightChild != BSTNODE_HANDLE_INVALID)
            {
                hNode = pNode->hRightChild;
            }
            else
            {
                BSTNodeHandle hRetVal = BSTSystem_GetNewNode(pData, iKey, hNode, false, false);
        
                //need to recalculate pNode, as it may have changed during GetNewNode
                pNode = BSTNodeFromHandle(hNode);
                pNode->hRightChild = hRetVal;

                RebalanceAroundAddedNode(pTree, hRetVal);
                return hRetVal;
            }
        }
        else
        {
            if (pNode->hLeftChild != BSTNODE_HANDLE_INVALID)
            {
                hNode = pNode->hLeftChild;
            }
            else
            {
                BSTNodeHandle hRetVal = BSTSystem_GetNewNode(pData, iKey, hNode, true, false);

                //need to recalculate pNode, as it may have changed during GetNewNode
                pNode = BSTNodeFromHandle(hNode);
                pNode->hLeftChild = hRetVal;


                RebalanceAroundAddedNode(pTree, hRetVal);
                return hRetVal;
            }

        }
    }
    while (1);
}

void BST_RotateLeft(BinarySearchTree *pTree, BSTNode *pNode)
{
    BSTNodeHandle *pParentHandle;
    BSTNodeHandle hStartingRightChild, hStartingLeftChild, hStartingRLGrandChild;
    BSTNode *pStartingRightChild, *pStartingLeftChild, *pStartingRLGrandChild;
    
    BSTNodeHandle hNode;
    assert(pNode);

    hNode = BSTHandleFromNode(pNode);

    if (pNode->hParent == BSTNODE_HANDLE_INVALID)
    {
        pParentHandle = &pTree->hTopNode;
    }
    else
    {
        if (pNode->bIsLeftChild)
        {
            pParentHandle = &(BSTNodeFromHandle(pNode->hParent)->hLeftChild);
        }
        else
        {
            pParentHandle = &(BSTNodeFromHandle(pNode->hParent)->hRightChild);
        }
    }

    hStartingRightChild = pNode->hRightChild;
    assert(hStartingRightChild != BSTNODE_HANDLE_INVALID);
    pStartingRightChild = BSTNodeFromHandle(hStartingRightChild);

    hStartingLeftChild = pNode->hLeftChild;
    pStartingLeftChild = BSTNodeFromHandle(hStartingLeftChild);

    hStartingRLGrandChild = pStartingRightChild->hLeftChild;
    pStartingRLGrandChild = BSTNodeFromHandle(hStartingRLGrandChild);


    AssertNodeValidity(pTree, hStartingRLGrandChild);
    AssertNodeValidity(pTree, hNode);
    AssertNodeValidity(pTree, hStartingRightChild);
    AssertNodeValidity(pTree, hStartingLeftChild);


    *pParentHandle = hStartingRightChild;
    pStartingRightChild->bIsLeftChild = pNode->bIsLeftChild;
    pStartingRightChild->hLeftChild = hNode;
    pStartingRightChild->hParent = pNode->hParent;
    
    pNode->hParent = hStartingRightChild;
    pNode->hLeftChild = hStartingLeftChild;
    pNode->hRightChild = hStartingRLGrandChild;
    pNode->bIsLeftChild = true;

    if (pStartingRLGrandChild)
    {
        pStartingRLGrandChild->bIsLeftChild = false;
        pStartingRLGrandChild->hParent = hNode;


    }

    AssertNodeValidity(pTree, hStartingRLGrandChild);
    AssertNodeValidity(pTree, hNode);
    AssertNodeValidity(pTree, hStartingRightChild);
    AssertNodeValidity(pTree, hStartingLeftChild);

}

void BST_RotateRight(BinarySearchTree *pTree, BSTNode *pNode)
{
    BSTNodeHandle *pParentHandle;
    BSTNodeHandle hStartingLeftChild, hStartingRightChild, hStartingLRGrandChild;
    BSTNode *pStartingLeftChild, *pStartingRightChild, *pStartingLRGrandChild;

    BSTNodeHandle hNode;
    assert(pNode);


    hNode = BSTHandleFromNode(pNode);

    if (pNode->hParent == BSTNODE_HANDLE_INVALID)
    {
        pParentHandle = &pTree->hTopNode;
    }
    else
    {
        if (!pNode->bIsLeftChild)
        {
            pParentHandle = &(BSTNodeFromHandle(pNode->hParent)->hRightChild);
        }
        else
        {
            pParentHandle = &(BSTNodeFromHandle(pNode->hParent)->hLeftChild);
        }
    }

    hStartingLeftChild = pNode->hLeftChild;
    assert(hStartingLeftChild != BSTNODE_HANDLE_INVALID);
    pStartingLeftChild = BSTNodeFromHandle(hStartingLeftChild);

    hStartingRightChild = pNode->hRightChild;
    pStartingRightChild = BSTNodeFromHandle(hStartingRightChild);

    hStartingLRGrandChild = pStartingLeftChild->hRightChild;
    pStartingLRGrandChild = BSTNodeFromHandle(hStartingLRGrandChild);

    AssertNodeValidity(pTree, hNode);
    AssertNodeValidity(pTree, hStartingRightChild);
    AssertNodeValidity(pTree, hStartingLeftChild);
    AssertNodeValidity(pTree, hStartingLRGrandChild);


    *pParentHandle = hStartingLeftChild;
    pStartingLeftChild->bIsLeftChild = pNode->bIsLeftChild;
    pStartingLeftChild->hRightChild = hNode;
    pStartingLeftChild->hParent = pNode->hParent;
    
    pNode->hParent = hStartingLeftChild;
    pNode->hRightChild = hStartingRightChild;
    pNode->hLeftChild = hStartingLRGrandChild;
    pNode->bIsLeftChild = false;

    if (pStartingLRGrandChild)
    {
        pStartingLRGrandChild->bIsLeftChild = true;
        pStartingLRGrandChild->hParent = hNode;

    }


    AssertNodeValidity(pTree, hNode);
    AssertNodeValidity(pTree, hStartingRightChild);
    AssertNodeValidity(pTree, hStartingLeftChild);
    AssertNodeValidity(pTree, hStartingLRGrandChild);

}





//algorithm taken from Wikipedia article on red black trees
void RebalanceAroundAddedNode(BinarySearchTree *pTree, BSTNodeHandle hNode)
{
    int iCurCase = 2;
    BSTNode *pNode = BSTNodeFromHandle(hNode);
    BSTNode *pParent = NULL, *pUncle = NULL, *pGrandParent = NULL;


    do
    {
        switch (iCurCase)
        {
        case 1: //assumes pNode is set
            if (pNode->hParent == BSTNODE_HANDLE_INVALID)
            {
                pNode->bIsRed = false;
                return;
            }
    
            iCurCase = 2;
            //fall through        
    
        case 2: //assumes pNode is set
            pParent = BSTNodeFromHandle(pNode->hParent);

            if (!pParent->bIsRed)
            {
                return;
            }

            iCurCase = 3;
            //fall through

        case 3:
            pGrandParent = BSTNodeFromHandle(pParent->hParent);
            pUncle = BSTNodeFromHandle(pParent->bIsLeftChild ? pGrandParent->hRightChild : pGrandParent->hLeftChild);

            if (pParent->bIsRed && pUncle && pUncle->bIsRed)
            {
                pParent->bIsRed = false;
                pUncle->bIsRed = false;
                pGrandParent->bIsRed = true;

                pNode = pGrandParent;
                iCurCase = 1;
                break;
            }

            iCurCase = 4;
            //fall through

        case 4:
            if (pNode->bIsLeftChild && !pParent->bIsLeftChild)
            {
                BST_RotateRight(pTree, pParent);
                assert(pParent == BSTNodeFromHandle(pNode->hRightChild));
                pNode = pParent;
                pParent = BSTNodeFromHandle(pNode->hParent);
                pGrandParent = BSTNodeFromHandle(pParent->hParent);
                pUncle = BSTNodeFromHandle(pParent->bIsLeftChild ? pGrandParent->hRightChild : pGrandParent->hLeftChild);
            }
            else if (!pNode->bIsLeftChild && pParent->bIsLeftChild)
            {
                BST_RotateLeft(pTree, pParent);
                assert(pParent == BSTNodeFromHandle(pNode->hLeftChild));
                pNode = pParent;
                pParent = BSTNodeFromHandle(pNode->hParent);
                pGrandParent = BSTNodeFromHandle(pParent->hParent);
                pUncle = BSTNodeFromHandle(pParent->bIsLeftChild ? pGrandParent->hRightChild : pGrandParent->hLeftChild);
            }

            iCurCase = 5;
            //fall through

        case 5:
            pParent->bIsRed = false;
            pGrandParent->bIsRed = true;

            if (pNode->bIsLeftChild)
            {
                assert(pParent->bIsLeftChild);
                BST_RotateRight(pTree, pGrandParent);
            }
            else
            {
                assert(!pParent->bIsLeftChild);
                BST_RotateLeft(pTree, pGrandParent);
            }
            return;
        }
        
    }
    while (1);
}


void BST_FixHandlesToMe(BinarySearchTree *pTree, BSTNodeHandle hNode)
{
    BSTNode *pNode = BSTNodeFromHandle(hNode);
    BSTNode *pParent = BSTNodeFromHandle(pNode->hParent);
    BSTNode *pLeftChild = BSTNodeFromHandle(pNode->hLeftChild);
    BSTNode *pRightChild = BSTNodeFromHandle(pNode->hRightChild);

    if (pParent)
    {
        if (pNode->bIsLeftChild)
        {
            pParent->hLeftChild = hNode;
        }
        else
        {
            pParent->hRightChild = hNode;
        }
    }
    else
    {
        pTree->hTopNode = hNode;
    }

    if (pLeftChild)
    {
        pLeftChild->hParent = hNode;
    }

    if (pRightChild)
    {
        pRightChild->hParent = hNode;
    }

}

void BST_SwitchParentChildPair(BinarySearchTree *pTree, BSTNodeHandle hParent, BSTNodeHandle hChild)
{
    BSTNode *pParent = BSTNodeFromHandle(hParent);
    BSTNode *pChild = BSTNodeFromHandle(hChild);
    bool bLeft = pChild->bIsLeftChild;

    BSTNode temp;

    memcpy(&temp, pParent, offsetof(BSTNode, data));
    memcpy(pParent, pChild, offsetof(BSTNode, data));
    memcpy(pChild, &temp, offsetof(BSTNode, data));

    pParent->hParent = hChild;
    
    if (bLeft)
    {
        pChild->hLeftChild = hParent;
    }
    else
    {
        pChild->hRightChild = hParent;
    }

    BST_FixHandlesToMe(pTree, hParent);
    BST_FixHandlesToMe(pTree, hChild);


}

    

void BST_SwitchNodes(BinarySearchTree *pTree, BSTNodeHandle hNode1, BSTNodeHandle hNode2)
{
    BSTNode *pNode1 = BSTNodeFromHandle(hNode1);
    BSTNode *pNode2 = BSTNodeFromHandle(hNode2);

    AssertNodeValidity(pTree, hNode1);
    AssertNodeValidity(pTree, hNode2);


    assert(pNode1 && pNode2);

    if (pNode1->hParent == hNode2)
    {
        BST_SwitchParentChildPair(pTree, hNode2, hNode1);
        AssertNodeValidity(pTree, hNode1);
        AssertNodeValidity(pTree, hNode2);
    }
    else if (pNode1->hLeftChild == hNode2 || pNode1->hRightChild == hNode2)
    {
        BST_SwitchParentChildPair(pTree, hNode1, hNode2);
        AssertNodeValidity(pTree, hNode1);
        AssertNodeValidity(pTree, hNode2);
    }
    else
    {
        BSTNode temp;

        memcpy(&temp, pNode1, offsetof(BSTNode, data));
        memcpy(pNode1, pNode2, offsetof(BSTNode, data));
        memcpy(pNode2, &temp, offsetof(BSTNode, data));

        BST_FixHandlesToMe(pTree, hNode1);
        BST_FixHandlesToMe(pTree, hNode2);


        AssertNodeValidity(pTree, hNode1);
        AssertNodeValidity(pTree, hNode2);
    }
}

BSTNodeHandle FindSuccessorNode(BinarySearchTree *pTree, BSTNode *pNode)
{
    

    BSTNodeHandle hTempHandle = pNode->hRightChild;
    BSTNode *pTempNode = BSTNodeFromHandle(pNode->hRightChild);

    AssertNodeValidity(pTree, hTempHandle);


    assert(pTempNode);

    while (pTempNode->hLeftChild != BSTNODE_HANDLE_INVALID)
    {
        hTempHandle = pTempNode->hLeftChild;
        AssertNodeValidity(pTree, hTempHandle);
        pTempNode = BSTNodeFromHandle(hTempHandle);
    }

    return hTempHandle;
}

void BST_SnipLeafNode(BinarySearchTree *pTree, BSTNodeHandle hNode)
{
    BSTNode *pNode = BSTNodeFromHandle(hNode);

    AssertNodeValidity(pTree, hNode);

    if (pNode->hParent != BSTNODE_HANDLE_INVALID)
    {
        BSTNode *pParent = BSTNodeFromHandle(pNode->hParent);

        if (pNode->bIsLeftChild)
        {
            pParent->hLeftChild = BSTNODE_HANDLE_INVALID;
        }
        else
        {
            pParent->hRightChild = BSTNODE_HANDLE_INVALID;
        }
    }
    else
    {
        pTree->hTopNode = BSTNODE_HANDLE_INVALID;
    }

    BSTSystem_ReleaseNode(hNode);
}

BSTNodeHandle BST_SnipOneChildNode(BinarySearchTree *pTree, BSTNodeHandle hNode)
{
    BSTNode *pNode = BSTNodeFromHandle(hNode);
    BSTNodeHandle hChild;
    BSTNode *pChild;

    AssertNodeValidity(pTree, hNode);

    hChild = pNode->hLeftChild;
    if (hChild == BSTNODE_HANDLE_INVALID)
    {
        hChild = pNode->hRightChild;
    }

    pChild = BSTNodeFromHandle(hChild);

    pChild->bIsLeftChild = pNode->bIsLeftChild;
    pChild->hParent = pNode->hParent;

    if (pNode->hParent == BSTNODE_HANDLE_INVALID)
    {
        pTree->hTopNode = hChild;
    }
    else
    {
        BSTNode *pParent = BSTNodeFromHandle(pNode->hParent);

        if (pNode->bIsLeftChild)
        {
            pParent->hLeftChild = hChild;
        }
        else
        {
            pParent->hRightChild = hChild;
        }
    }
    
    BSTSystem_ReleaseNode(hNode);

    return hChild;
}

//algorithm from wikipedia red black tree page
void BST_DoRemoveBalancingThing(BinarySearchTree *pTree, BSTNodeHandle hNode)
{
    int iCurCase = 1;


    BSTNodeHandle hSibling, hParent = 0;
    BSTNode *pSibling = NULL, *pParent = NULL;
    BSTNode *pNode = BSTNodeFromHandle(hNode);

    AssertNodeValidity(pTree, hNode);

    do
    {

    
        switch (iCurCase)
        {
        case 1:
            if (pNode->hParent == BSTNODE_HANDLE_INVALID)
            {
                return;
            }
            iCurCase = 2;
            //fall through

        case 2:
            hParent = pNode->hParent;
            pParent = BSTNodeFromHandle(hParent);
            hSibling = pNode->bIsLeftChild ? pParent->hRightChild : pParent->hLeftChild;
            pSibling = BSTNodeFromHandle(hSibling);

            if (pSibling)
            {
                AssertNodeValidity(pTree, pSibling->hLeftChild);
                AssertNodeValidity(pTree, pSibling->hRightChild);
            }


            AssertNodeValidity(pTree, hSibling);
            AssertNodeValidity(pTree, hParent);


            if (pSibling->bIsRed)
            {
                pSibling->bIsRed = false;
                pParent->bIsRed = true;

                if (pNode->bIsLeftChild)
                {
                    BST_RotateLeft(pTree, pParent);
                }
                else
                {
                    BST_RotateRight(pTree, pParent);
                }

                hSibling = pNode->bIsLeftChild ? pParent->hRightChild : pParent->hLeftChild;
                pSibling = BSTNodeFromHandle(hSibling);

                AssertNodeValidity(pTree, hNode);
                AssertNodeValidity(pTree, hSibling);
                AssertNodeValidity(pTree, hParent);

                if (pSibling)
                {
                    AssertNodeValidity(pTree, pSibling->hLeftChild);
                    AssertNodeValidity(pTree, pSibling->hRightChild);
                }


                assert(pNode->hParent == hParent);
            }
            
            iCurCase = 3;
            //fall through

        case 3:
            if (!pParent->bIsRed 
                && !pSibling->bIsRed 
                && (pSibling->hLeftChild == BSTNODE_HANDLE_INVALID || !BSTNodeFromHandle(pSibling->hLeftChild)->bIsRed)
                && (pSibling->hRightChild == BSTNODE_HANDLE_INVALID || !BSTNodeFromHandle(pSibling->hRightChild)->bIsRed))
            {
                pSibling->bIsRed = true;

                hNode = hParent;
                pNode = pParent;
                iCurCase = 1;
                break;
            }
            iCurCase = 4;
            //fall through

        case 4:
            if (pParent->bIsRed 
                && !pSibling->bIsRed 
                && (pSibling->hLeftChild == BSTNODE_HANDLE_INVALID || !BSTNodeFromHandle(pSibling->hLeftChild)->bIsRed)
                && (pSibling->hRightChild == BSTNODE_HANDLE_INVALID || !BSTNodeFromHandle(pSibling->hRightChild)->bIsRed))
            {
                pParent->bIsRed = false;
                pSibling->bIsRed= true;
                return;
            }
            iCurCase = 5;
            //fall through

        case 5:
            if (pNode->bIsLeftChild 
                && !pSibling->bIsRed 
                && pSibling->hLeftChild != BSTNODE_HANDLE_INVALID && BSTNodeFromHandle(pSibling->hLeftChild)->bIsRed
                && (pSibling->hRightChild == BSTNODE_HANDLE_INVALID || !BSTNodeFromHandle(pSibling->hRightChild)->bIsRed))
            {
                pSibling->bIsRed = true;
                BSTNodeFromHandle(pSibling->hLeftChild)->bIsRed = false;
                BST_RotateRight(pTree, pSibling);

                assert(hParent == pNode->hParent);

                hSibling = pParent->hRightChild;
                pSibling = BSTNodeFromHandle(hSibling);

                AssertNodeValidity(pTree, hSibling);

            }
            else if (!pNode->bIsLeftChild 
                && !pSibling->bIsRed 
                && pSibling->hRightChild != BSTNODE_HANDLE_INVALID && BSTNodeFromHandle(pSibling->hRightChild)->bIsRed
                && (pSibling->hLeftChild == BSTNODE_HANDLE_INVALID || !BSTNodeFromHandle(pSibling->hLeftChild)->bIsRed))
            {
                pSibling->bIsRed = true;
                BSTNodeFromHandle(pSibling->hRightChild)->bIsRed = false;
                BST_RotateLeft(pTree, pSibling);

                assert(hParent == pNode->hParent);

                hSibling =pParent->hLeftChild;
                pSibling = BSTNodeFromHandle(hSibling);

                AssertNodeValidity(pTree, hSibling);

            }

            iCurCase = 6;

            //fall through
        case 6:
            pSibling->bIsRed = pParent->bIsRed;
            pParent->bIsRed = false;
            if (pNode->bIsLeftChild)
            {
                assert(pSibling->hRightChild != BSTNODE_HANDLE_INVALID);
                assert(BSTNodeFromHandle(pSibling->hRightChild)->bIsRed);
                BSTNodeFromHandle(pSibling->hRightChild)->bIsRed = false;
                BST_RotateLeft(pTree, pParent);
            }
            else
            {
                assert(pSibling->hLeftChild != BSTNODE_HANDLE_INVALID);
                assert(BSTNodeFromHandle(pSibling->hLeftChild)->bIsRed);
                BSTNodeFromHandle(pSibling->hLeftChild)->bIsRed = false;
                BST_RotateRight(pTree, pParent);


            }
            return;
        }
    } while (1);
}



void BST_RemoveNode(BinarySearchTree *pTree, BSTNodeHandle hNode)
{
    BSTNode *pNode = BSTNodeFromHandle(hNode);
    BSTNodeHandle hChild;
    BSTNode *pChild;

    AssertNodeValidity(pTree, hNode);
        
    //first, we need to potentially switch pNode with its successor or predecessor, such that we end up with pNode
    //having at most one child

    if (pNode->hLeftChild != BSTNODE_HANDLE_INVALID && pNode->hRightChild != BSTNODE_HANDLE_INVALID)
    {
        BSTNodeHandle hSuccessor = FindSuccessorNode(pTree, pNode);

        AssertNodeValidity(pTree, hSuccessor);
        

        BST_SwitchNodes(pTree, hNode, hSuccessor);

    }

    assert(pNode->hLeftChild == BSTNODE_HANDLE_INVALID || pNode->hRightChild == BSTNODE_HANDLE_INVALID);


    if (pNode->hLeftChild == BSTNODE_HANDLE_INVALID && pNode->hRightChild == BSTNODE_HANDLE_INVALID)
    {
        if (!pNode->bIsRed)
        {
            if (hNode == 1879)
            {
                hNode = hNode; // Just a line to put a breakpoint on
            }
            BST_DoRemoveBalancingThing(pTree, hNode);
        }

        BST_SnipLeafNode(pTree, hNode);

        return;
    }

    hChild = BST_SnipOneChildNode(pTree, hNode);
    if (pNode->bIsRed)
    {
        return;
    }

    pChild = BSTNodeFromHandle(hChild);

    if (pChild->bIsRed)
    {
        pChild->bIsRed = false;
        return;
    }

    
    BST_DoRemoveBalancingThing(pTree, hChild);
}
    
    
BSTNodeHandle BSTSystem_GetNewNode(void *pData, intptr_t iKey, BSTNodeHandle hParent, bool bIsLeft, bool bStartsBlack)
{
    BSTNodeHandle hReturnVal;
    BSTNode *pNode;

    if (gFirstFreeNodeHandle == BSTNODE_HANDLE_INVALID)
    {
        BSTNode *pNewPool = calloc((NUM_HANDLES_TO_ADD_TO_POOL_AT_ONCE + siNumHandlesInPool), sizeof(BSTNode));
        int i;

        if (gpNodePool)
        {
            memcpy(pNewPool, gpNodePool, sizeof(BSTNode) * siNumHandlesInPool);
            free(gpNodePool);
        }

        gpNodePool = pNewPool;

        for (i = siNumHandlesInPool; i < siNumHandlesInPool + NUM_HANDLES_TO_ADD_TO_POOL_AT_ONCE; i++)
        {
            gpNodePool[i].hLeftChild = (i == (siNumHandlesInPool + NUM_HANDLES_TO_ADD_TO_POOL_AT_ONCE - 1) ? BSTNODE_HANDLE_INVALID : i + 1);
        }

        gFirstFreeNodeHandle = siNumHandlesInPool;
        siNumHandlesInPool += NUM_HANDLES_TO_ADD_TO_POOL_AT_ONCE;

        printf("Now have %d BST handles\n", siNumHandlesInPool);
    }


    
    hReturnVal = gFirstFreeNodeHandle;
    pNode = BSTNodeFromHandle(hReturnVal);

    gFirstFreeNodeHandle = pNode->hLeftChild;

    pNode->data.iKey = iKey;
    pNode->data.pData = pData;
    pNode->hLeftChild = BSTNODE_HANDLE_INVALID;
    pNode->hRightChild = BSTNODE_HANDLE_INVALID;
    pNode->hParent = hParent;
    pNode->bIsLeftChild = bIsLeft;
    pNode->bIsRed = !bStartsBlack;
    return hReturnVal;
    
}

void BSTSystem_ReleaseNode(BSTNodeHandle hNode)
{
    BSTNode *pNode = BSTNodeFromHandle(hNode);
    pNode->hLeftChild = gFirstFreeNodeHandle;
    gFirstFreeNodeHandle = hNode;
}



int BSTNode_FindNodes(BSTNodeHandle hStartingNode, intptr_t iMinKey, intptr_t iMaxKey, BSTNodeHandle *pOutHandles, int iMaxToFind)
{
    BSTNodeHandle workNodes[BST_MAXDEPTH];

    int iCurDepth = 1;
    int iNumFound = 0;

    if (hStartingNode == BSTNODE_HANDLE_INVALID)
    {
        return 0;
    }

    if (iMaxToFind == 0)
    {
        return 0;
    }

    workNodes[0] = hStartingNode;

    while (iCurDepth)
    {
        BSTNodeHandle hCurNode = workNodes[iCurDepth-1];
        BSTNode *pCurNode = BSTNodeFromHandle(hCurNode);
        iCurDepth--;

        if (pCurNode->data.iKey < iMinKey)
        {
            if (pCurNode->hRightChild != BSTNODE_HANDLE_INVALID)
            {
                workNodes[iCurDepth++] = pCurNode->hRightChild;
            }
        }
        else if (pCurNode->data.iKey >= iMaxKey)
        {
            if (pCurNode->hLeftChild != BSTNODE_HANDLE_INVALID)
            {
                workNodes[iCurDepth++] = pCurNode->hLeftChild;
            }
        }
        else
        {
            pOutHandles[iNumFound++] = hCurNode;
            if (iNumFound == iMaxToFind)
            {
                return iNumFound;
            }
        
            if (pCurNode->hRightChild != BSTNODE_HANDLE_INVALID)
            {
                workNodes[iCurDepth++] = pCurNode->hRightChild;
            }

            if (pCurNode->hLeftChild != BSTNODE_HANDLE_INVALID)
            {
                assert(iCurDepth < BST_MAXDEPTH);
                workNodes[iCurDepth++] = pCurNode->hLeftChild;
            }
        }
    }

    return iNumFound;
}




int BST_FindNodes(BinarySearchTree *pTree, intptr_t iMinKey, intptr_t iMaxKey, BSTNodeHandle *pOutHandles, int iMaxToFind)
{    
    if (iMinKey == iMaxKey)
    {
        iMaxKey += 1;
    }
    return BSTNode_FindNodes(pTree->hTopNode, iMinKey, iMaxKey, pOutHandles, iMaxToFind);
}

void BST_InitSearcher(BinarySearchTreeSearcher *pSearcher, BinarySearchTree *pTree, intptr_t iMinKey, intptr_t iMaxKey)
{
    if (iMinKey == iMaxKey)
    {
        iMaxKey += 1;
    }
    
    pSearcher->pTree = pTree;
    pSearcher->iMinKey = iMinKey;
    pSearcher->iMaxKey = iMaxKey;
    

    if (pTree->hTopNode != BSTNODE_HANDLE_INVALID)
    {
        pSearcher->iCurDepth = 1;
        pSearcher->workNodes[0] = pTree->hTopNode;
    }
    else
    {
        pSearcher->iCurDepth = 0;
    }
}

BSTNodeHandle BST_GetNextFromSearcher(BinarySearchTreeSearcher *pSearcher)
{
    while (pSearcher->iCurDepth)
    {
        BSTNodeHandle hCurNode = pSearcher->workNodes[pSearcher->iCurDepth-1];
        BSTNode *pCurNode = BSTNodeFromHandle(hCurNode);
        pSearcher->iCurDepth--;

        if (pCurNode->data.iKey >= pSearcher->iMinKey && pCurNode->data.iKey < pSearcher->iMaxKey || (pSearcher->iMinKey == 1 && pSearcher->iMaxKey == 0))
        {        
            if (pCurNode->hRightChild != BSTNODE_HANDLE_INVALID)
            {
                pSearcher->workNodes[pSearcher->iCurDepth++] = pCurNode->hRightChild;
            }

            if (pCurNode->hLeftChild != BSTNODE_HANDLE_INVALID)
            {
                assert(pSearcher->iCurDepth < BST_MAXDEPTH);
                pSearcher->workNodes[pSearcher->iCurDepth++] = pCurNode->hLeftChild;
            }

            return hCurNode;
        }
        else if (pCurNode->data.iKey < pSearcher->iMinKey)
        {
            if (pCurNode->hRightChild != BSTNODE_HANDLE_INVALID)
            {
                pSearcher->workNodes[pSearcher->iCurDepth++] = pCurNode->hRightChild;
            }
        }
        else 
        {
            if (pCurNode->hLeftChild != BSTNODE_HANDLE_INVALID)
            {
                pSearcher->workNodes[pSearcher->iCurDepth++] = pCurNode->hLeftChild;
            }
        }
    }


    return BSTNODE_HANDLE_INVALID;
}








void BST_CheckValidity(BinarySearchTree *pTree)
{

    BSTNodeHandle workNodes[BST_MAXDEPTH];
    int iTreeDepths[BST_MAXDEPTH];
    BSTNode *pTopNode;

    int iCurDepth = 1;
    int iTreeDepthSum = 0;

    int iMaxLeafDepth = 0;
    int iMinLeafDepth = 1000000;

    if (pTree->hTopNode == BSTNODE_HANDLE_INVALID)
    {
        return;
    }

    workNodes[0] = pTree->hTopNode;
    iTreeDepths[0] = 1;

    pTopNode = BSTNodeFromHandle(pTree->hTopNode);

    assert(!pTopNode->bIsRed);
    

    while (iCurDepth)
    {
        BSTNodeHandle hCurNode = workNodes[iCurDepth-1];
        BSTNode *pCurNode = BSTNodeFromHandle(hCurNode);

        BSTNode *pLeftChild = BSTNodeFromHandle(pCurNode->hLeftChild);
        BSTNode *pRightChild = BSTNodeFromHandle(pCurNode->hRightChild);

        int iCurTreeDepth = iTreeDepths[iCurDepth - 1];

        iTreeDepthSum += iCurTreeDepth;

    
        iCurDepth--;

        if (pLeftChild)
        {
            assert(pLeftChild->hParent == hCurNode);
            assert(pLeftChild->bIsLeftChild == true);

            assert(!(pCurNode->bIsRed && pLeftChild->bIsRed));

            iTreeDepths[iCurDepth] = iCurTreeDepth + 1;
            workNodes[iCurDepth++] = pCurNode->hLeftChild;
        }

        if (pRightChild)
        {
            assert(pRightChild->hParent == hCurNode);
            assert(pRightChild->bIsLeftChild == false);
            assert(iCurDepth < BST_MAXDEPTH);

            assert(!(pCurNode->bIsRed && pLeftChild->bIsRed));

            iTreeDepths[iCurDepth] = iCurTreeDepth + 1;
            workNodes[iCurDepth++] = pCurNode->hRightChild;
        }

        if (!pLeftChild && !pRightChild)
        {
            if (iCurTreeDepth > iMaxLeafDepth)
            {
                iMaxLeafDepth = iCurTreeDepth;
            }

            if (iCurTreeDepth < iMinLeafDepth)
            {
                iMinLeafDepth = iCurTreeDepth;
            }
        }
    }

    assert(iMaxLeafDepth <= 2 * iMinLeafDepth);
}

void BSTSystem_AssertIsEmpty(void)
{
    BSTNode *pTemp;

    int iNumFree = 0;

    pTemp = BSTNodeFromHandle(gFirstFreeNodeHandle);

    while (pTemp)
    {
        iNumFree++;
        pTemp = BSTNodeFromHandle(pTemp->hLeftChild);
    }

    assert(iNumFree == siNumHandlesInPool);
}

#define MAX_TEST_KEY 10000000
#define MAX_TO_DELETE 1024
void BST_Test()
{
    BinarySearchTree tree;

    BST_Init(&tree);

    while (1)
    {
        if (rand() % 10000 == 0)
        {
            int iCounter = 0;

            printf("About to remove all nodes\n");
            while (tree.hTopNode != BSTNODE_HANDLE_INVALID)
            {
                iCounter++;
                BST_RemoveNode(&tree, tree.hTopNode);
            }
            printf("removed %d nodes\n", iCounter);
            BST_CheckValidity(&tree);
            
            BSTSystem_AssertIsEmpty();
        }

        if (rand() % 1000 == 0)
        {
            int iMinToRemove = rand() % MAX_TEST_KEY;
            int iMaxToRemove = rand() % MAX_TEST_KEY;
            int iNumToDelete;
            int i;

            BSTNodeHandle deleteHandles[MAX_TO_DELETE];

            if (iMinToRemove > iMaxToRemove)
            {
                int iTemp = iMinToRemove;
                iMinToRemove = iMaxToRemove;
                iMaxToRemove = iTemp;
            }

            iNumToDelete = BST_FindNodes(&tree, iMinToRemove, iMaxToRemove, deleteHandles, MAX_TO_DELETE);

            printf("About to delete %d nodes\n", iNumToDelete);
            
            for (i=0; i < iNumToDelete; i++)
            {
                BST_RemoveNode(&tree, deleteHandles[i]);
            }
            BST_CheckValidity(&tree);
        }

        BST_AddNode(&tree, NULL, rand() % MAX_TEST_KEY);

    }
                


}
void BST_DumpNode(BSTNodeHandle hNode)
{
    if (hNode != BSTNODE_HANDLE_INVALID)
    {
        BSTNode *pNode = BSTNodeFromHandle(hNode);

        printf("(");
        BST_DumpNode(pNode->hLeftChild);
        printf(",%d,", pNode->data.iKey);
        BST_DumpNode(pNode->hRightChild);
        printf(")");
    }
}

void BST_DumpTree(BinarySearchTree *pTree)
{
    BST_DumpNode(pTree->hTopNode);
    printf("\n");
}



void BST_InitSystem()
{


    if (sBSTSystemInitted)
    {
        return;
    }

    sBSTSystemInitted = true;

    assert(gpNodePool == NULL);

    siNumHandlesInPool = 0;

    gFirstFreeNodeHandle = BSTNODE_HANDLE_INVALID;
}

