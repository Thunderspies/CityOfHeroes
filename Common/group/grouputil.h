/* File grouputil.h
 *    This file defines several callback mechanism for processing each group in the
 *    group info tree.
 *
 *
 *    This module now provides 2 main ways of processing the group info tree.
 *
 *    The first method simply traverses the group info tree recursively, depth-first,
 *    calling a user defined function for every node in the tree.
 *
 *    The 2nd method is an extension of the first method and works in much the same
 *    way.  The only difference that most traversal state information are now kept
 *    in a single packet of information.  This allows starting, pausing, and resuming
 *    of the group info tree traversal.  This method allows a very controlled traversal
 *    through the group info tree, visiting nodes only when appropriate.  Currently,
 *    this is being used to gather enough information about the map before initializing
 *    the appropriate interactive elements on the map.
 *
 */

#ifndef GROUPUTIL_H
#define GROUPUTIL_H

#include <utilitieslib/stdtypes.h>

typedef struct Array Array;
typedef struct GroupDef GroupDef;
typedef struct DefTracker DefTracker;
typedef struct GroupEnt GroupEnt;
typedef struct GroupInfo GroupInfo;
typedef struct PropertyEnt PropertyEnt;

/**********************************************************************************
 * Traversal Method #1
 */

/* Function type GroupDefProcessor
 *    Defines the signature of a callback function to be used with groupProcessType.
 *    When groupProcessType finds a group of the specified name, it will call
 *    the given GroupDefProessor, passing the processor the group definition
 *    as well as the matrix of the parent group.
 *
 *    The return value of the processor is used to by groupProcessType to determine
 *    whether is should continue searching deeper along the current branch.  This is
 *    used to stop "duplicate" items from being created.  For example, if both the
 *    sole child group and the parent group are marked as a "Beacon", both groups
 *    will be passed to the beacon group processor.  This results in two different
 *    beacons at almost the same location being created.  In this case, it is logical
 *    for the beacon group processor to return zero whenever a "beacon" is encountered.
 *    In this way, only the groupProcessType will move on to other branches of the
 *    group info tree once the parent is processed, leading to only one beacon being
 *    created at the location.
 *
 *    Returns:
 *        Whether the current group should be expanded further.
 *        0 - Do not explore current GroupInfo tree branch further.
 *        ~0 - Keep exploring current GroupInfo tree branch.
 *
 */
typedef int (*GroupDefProcessor)(GroupDef* def, Mat4 parent_mat);
typedef int (*GroupDefTrackerProcessor)(DefTracker* tracker);

/*
 * Traversal Method #1
 **********************************************************************************/



/**********************************************************************************
 * Traversal Method #2
 */

typedef struct GroupDefTraverser GroupDefTraverser;
/* Struct GroupDefTraverserContext
 *    This structure specifies how to allocation/copy/store a piece of user
 *    defined data.  See GroupDefTraverser for an explaination of its real
 *    function.
 */
typedef struct{
    void* context;
    void* (*createContext)(void);
    void (*copyContext)(void* src, void* dst);
    void (*destroyContext)(void* context);
} GroupDefTraverserVContext;

typedef struct{
    void* context;
    void (*beginTraverserProcess)(void* context, GroupDefTraverser* traverser);
    void (*endTraverserProcess)(void* context, GroupDefTraverser* traverser);
} GroupDefTraverserPContext;

/* Struct GroupDefTraverser
 *    The GroupDefTraverser is used to traverse the "GroupInfo tree".  Its main
 *    purpose is to capture the some useful states/data aquired during the
 *    traversal so that it is possible to pause and resume traversal of the
 *    tree later.
 *
 *
 *    User Defined Context:
 *        The GroupDefTraverser is intended for use by the groupProcessDefExX() functions,
 *        which uses recursion to traverse the GroupInfo tree.  The two context structures
 *        carries information from one instance of the recursive call to the next.
 *        GroupDefTraverser carries two such packets of information: a Persistent Context 
 *        and a Volatile Context.  
 *
 *        Persistent Context is used to carry information through out the entire tree 
 *        traversal.
 *
 *        Volitile Context is used for parent calls to communicate information to child
 *        calls only.  As soon as control is returned to the parent, the information is 
 *        destroyed and a new copy is prepared for the next child.  Note that the
 *        volatile context is created/copied/destroyed very frequently.  It is a good
 *        idea to keep volatile functions "light weight".
 *        
 *
 *    Note that the current implementation does not capture the complete state
 *    of the tree traversal as it makes use of recursion to track traversal
 *    progress through the tree.  When a traversal has been stopped, it is impossible
 *    to really resume the traversal.  It only captures enough information to allow
 *    another traversal to begin at the referenced GroupDef node.
 *
 */
struct GroupDefTraverser{
    GroupDef*        def;                // The current group def under examination
    Mat4            mat;                // Position of the current group def
    GroupEnt*        ent;                // The current groupEnt if applicable
    int                no_coll_parents;    // The current nested def->no_coll count.
    
    unsigned char    isInTray    : 1;    // Whether or not this group is in a tray

    unsigned char    bottomUp    : 1;    // Process parents *after* children (makes return value of traverser meaningless)

    unsigned char    hasPContext : 1;
    unsigned char    hasVContext : 1;
    GroupDefTraverserPContext pContext;    // Persistent context
    GroupDefTraverserVContext vContext;    // Volatile context
};

typedef int (*GroupDefProcessorEx)(GroupDefTraverser* traverser);
/*
 * Traversal Method #2
 **********************************************************************************/



/**********************************************************************************
 * Traversal Utilities
 *    The groupExtractSubtrees() utility function is built on top of Traversal method
 *    #2.  It uses Traversal method #2 to visit each node.  For every GroupDef passed
 *    to the user defined extraction critera function that is accepted, it saves off
 *    a GroupDefTraverser into a dynamic array.  It also refrain from traversing
 *    deeper into the tree from the accepted nodes.  At the end of the function,
 *    it passes back the array that contains all the Traversers that it has gathered.
 */
typedef enum{
    EA_NO_EXTRACT,
    EA_EXTRACT,
    EA_NO_BRANCH_EXPLORE,
} ExtractionAction;


typedef ExtractionAction (*ExtractionCriteria)(GroupDefTraverser* def);
typedef int (*ExtractionItemProcess)(GroupDefTraverser* traverser, int groupDefAccepted);
/*
 * Traversal Utilities
 **********************************************************************************/


/**********************************************************************************
 * Utility functions
 */
/*
 * Utility functions
 **********************************************************************************/

// Generated by mkproto
void groupProcessType(const char* type, GroupInfo *group_info, GroupDefProcessor func);
void groupProcessDef(GroupInfo *group_info, GroupDefProcessor process);
void groupProcessDefExBegin(GroupDefTraverser* traverser, GroupInfo *group_info, GroupDefProcessorEx process);
void groupProcessDefExContinue(GroupDefTraverser* traverser, GroupDefProcessorEx process);
GroupDefTraverser* cloneGroupDefTraverser(GroupDefTraverser* traverser);
GroupDefTraverser* createGroupDefTraverser();
void copyGroupDefTraverser(GroupDefTraverser* src, GroupDefTraverser* dst);
void destroyGroupDefTraverser(GroupDefTraverser* traverser);
Array* groupExtractSubtrees(GroupInfo* groupInfo, ExtractionCriteria accept);
Array* groupExtractSubtreesEx(GroupInfo* groupInfo, ExtractionCriteria accept, ExtractionItemProcess process, GroupDefTraverserPContext* pContext, GroupDefTraverserVContext* vContext);
int groupDefIsSpawnArea(GroupDef* def);
int groupDefIsScenario(GroupDef* def);
int groupDefIsDoorGenerator(GroupDef * def);
int groupDefIsMapTransferDoor(GroupDef *def);
int groupDefIsGenerator(GroupDef* def);
int groupDefIsGeneratorGroup(GroupDef* def);
int groupDefIsActivationRequest(GroupDef* def);
PropertyEnt* groupDefFindProperty(GroupDef* def, char* propName);
char* groupDefFindPropertyValue(GroupDef* def, char* propName);
int groupDefAddProperty(GroupDef* def, char* propName, char* propValue);
void groupProcessDefTracker(GroupInfo *group_info, GroupDefTrackerProcessor process);
void groupDefDeleteProperties(GroupDef* def);
void groupCreateAllCtris(void);
// End mkproto
#endif
