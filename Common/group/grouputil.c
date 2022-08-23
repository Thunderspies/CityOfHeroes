#include "group/grouputil.h"
#include <utilitieslib/utils/mathutil.h>
#include <stdlib.h>
#include <string.h>
#include <utilitieslib/assert/assert.h>
#include <utilitieslib/utils/utils.h>
#include "group/grouptrack.h"    // for trackerOpen
#include "group/group.h"
#include <utilitieslib/components/ArrayOld.h>
#include <utilitieslib/components/StashTable.h>
#include "group/groupProperties.h"
#include "seq/tricks.h"
#include "seq/anim.h"
#include "NovodeX/NwWrapper.h"
#include <utilitieslib/components/EString.h>

/***************************************************************************************************
 * Function groupProcessType + groupProcessTypeHelper
 *    The groupProcessType function uses the helper function to iterate through a group info tree,
 *    looking for a group def with that matches the specified type name.  When a particualr group
 *    definition matches the specified type name, the GroupDefProcessor function is called to perform
 *    the actual processing.
 */

static void groupProcessTypeHelper(const char* type, GroupDef* def, Mat4 parent_mat, GroupDefProcessor func);

/* Function groupProcessType
 *    This function can be used to visit every node of the given group_info tree
 *    in a recrusive, depth-first, manner.  It passes each GroupDef node found in the
 *    tree that matches the given type name to the user defined callback function.
 *
 *    Parameters:
 *        type - the type to be processed
 *        group_info - the root to start searching for group defs of the matching type
 *        func - the function that the related parameters should be sent to for processing
 *
 */
void groupProcessType(const char* type, GroupInfo *group_info, GroupDefProcessor func){
    DefTracker* ref;
    int            i;
    
    for(i = 0; i < group_info->ref_count; i++)
    {
        ref = group_info->refs[i];
        if (ref->def)
        {
            groupProcessTypeHelper(type, ref->def,ref->mat, func);
        }
    }    
}

static void groupProcessTypeHelper(const char* type, GroupDef* def, Mat4 parent_mat, GroupDefProcessor func){
    int            i;
    GroupEnt    *ent;
    
    if (def->type_name && def->type_name[0])
    {
        if(strcmp(def->type_name, type) == 0)
            if(!func(def, parent_mat))
                return;
    }
    
    for(ent = def->entries,i=0;i<def->count;i++,ent++)
    {
        Mat4 mat;
        
        mulMat4(parent_mat, ent->mat, mat);
        groupProcessTypeHelper(type, ent->def, mat, func);
    }
}

/*
 * Function groupProcessType + groupProcessTypeHelper
 ***************************************************************************************************/



/***************************************************************************************************
 * Function groupProcessDef + groupProcessDefHelper
 *    The groupProcessType function uses the helper function to iterate through a group info tree.  All
 *    encountered group definitions are passed back to the given group definition processor.
 */

/* Function groupProcessDef
 *    This function can be used to visit every node of the given group_info tree
 *    in a recrusive, depth-first, manner.  It passes each GroupDef node found in the
 *    tree to the user defined callback function.
 */
static void groupProcessDefHelper(GroupDef* def, Mat4 parent_mat, GroupDefProcessor process){
    int            i;
    GroupEnt    *ent;
    
    
    if(!process(def, parent_mat))
        return;
    
    for(ent = def->entries,i=0;i<def->count;i++,ent++)
    {
        Mat4 mat;
        
        mulMat4(parent_mat, ent->mat, mat);
        groupProcessDefHelper(ent->def, mat, process);
    }
}

void groupProcessDef(GroupInfo *group_info, GroupDefProcessor process){
    DefTracker* ref;
    int            i;
    
    for(i = 0; i < group_info->ref_count; i++)
    {
        ref = group_info->refs[i];
        if (ref->def)
        {
            groupProcessDefHelper(ref->def,ref->mat, process);
        }
    }
}
/*
 * Function groupProcessDef + groupProcessDefHelper
 ***************************************************************************************************/



/***************************************************************************************************
 * Function groupProcessDef + groupProcessDefHelper
 */

// Used by groupProcessDefEx to uniquely identify a particular node.
static int traverseCount = 0;
/* 
 *     
 */
void groupProcessDefExBegin(GroupDefTraverser* traverser, GroupInfo *group_info, GroupDefProcessorEx process){
    DefTracker* ref;
    int            i;
    
    // If the traverser has a valid volatile context, make sure that it is possible to perform
    // the required operations.
    if(traverser->vContext.context){
        assert(traverser->vContext.copyContext);
        assert(traverser->vContext.createContext);
        assert(traverser->vContext.destroyContext);
    }
    if(traverser->pContext.context){
        assert(traverser->pContext.beginTraverserProcess);
        assert(traverser->pContext.endTraverserProcess);
    }

    traverseCount = 0;
    for(i = 0; i < group_info->ref_count; i++)
    {
        ref = group_info->refs[i];
        if (ref->def)
        {
            traverser->def = ref->def;
            copyMat4(ref->mat, traverser->mat);
            traverser->isInTray = 0;
            traverser->ent = NULL;
            traverser->no_coll_parents = 0;
            groupProcessDefExContinue(traverser, process);
        }
    }
}

/*
 *
 */
void groupProcessDefExContinue(GroupDefTraverser* traverser, GroupDefProcessorEx process){
    int            i;
    GroupEnt*    ent;
    Mat4        curMat;
    void*        origVContext = NULL;
    void*        curVContext = NULL;
    GroupDef*    curDef;
    GroupEnt*    origEnt=traverser->ent;
    int            UID;
    int            isInTray=traverser->isInTray;
    int            no_coll_parents=traverser->no_coll_parents;

    UID = ++traverseCount;
    if(traverser->hasPContext && traverser->pContext.beginTraverserProcess)
        traverser->pContext.beginTraverserProcess(traverser->pContext.context, traverser);

    if(traverser->hasVContext){
    //    Make a copy of the user defined volatile info if it is present.
        origVContext = traverser->vContext.createContext();
        traverser->vContext.copyContext(traverser->vContext.context, origVContext);
    }

    // Check to see if this def is a tray
    if (traverser->def->is_tray)
        traverser->isInTray = 1;

    if (!traverser->bottomUp) {
        // Let the processor process this group def first.
        // If the processor says not to explore further down the tree from this node,
        // don't visit the child nodes.
        if(!process(traverser))
            goto cleanup;
    }
    
    if(traverser->def->no_coll){
        traverser->no_coll_parents++;
    }
    
    if(traverser->hasVContext){
    //    Make a copy of the user defined volatile info if it is present.
        curVContext = traverser->vContext.createContext();
        traverser->vContext.copyContext(traverser->vContext.context, curVContext);
    }
    

    // The processor says to continue down the tree.

    // Copy all the "volatile" info out of the traverser, so a fresh copy can be
    // sent to each child node later.
    
    //    Make a copy of the current matrix.
    copyMat4(traverser->mat, curMat);

    //    Record where the traverser is right now.
    curDef = traverser->def;

    // Examine all child nodes attached to this group definition.
    for(ent = curDef->entries, i = 0; i < curDef->count; i++, ent++)
    {
        // Setup all volatile info for the child node.

        //    Setup the orientation/position matrix for the child node.
        mulMat4(curMat, ent->mat, traverser->mat);

//        if(traverser->hasVContext){
//        //    Setup the user defined volatile info for the child node.
//            traverser->vContext.copyContext(&curVContext, &(traverser->vContext.context));
//        }


        //    Setup the group def cursor.
        traverser->def = ent->def;
        traverser->ent = ent;

        // Process the child definition.
        groupProcessDefExContinue(traverser, process);



        // Restore all volatile info, just in case they were changed by the child node.

        //    Restore the orientation/position matrix.
        //        Restore not required.  The setup will overwrite the matrix passed to the child correctly.

        if(traverser->hasVContext){
        //    Restore the user defined volatile info.
            traverser->vContext.copyContext(curVContext, traverser->vContext.context);
        }
    }

    traverser->no_coll_parents = no_coll_parents;

    //    Restore group def cursor.
    traverser->def = curDef;
    copyMat4(curMat, traverser->mat);
    traverser->ent = origEnt;

    if(traverser->hasVContext){
        //    Restore the user defined volatile info.
        traverser->vContext.copyContext(origVContext, traverser->vContext.context);
    }

    if (traverser->bottomUp) {
        // Let the processor process this group def last.
        process(traverser);
        if(traverser->hasVContext){
            //    Restore the user defined volatile info.
            traverser->vContext.copyContext(origVContext, traverser->vContext.context);
        }
    }

    // Destroy all copied volatile info.
    //    The copied matrix will be cleaned up automatically.
    //    Cleanup the copy of the user defined volatile info.
    if(traverser->hasVContext){
        traverser->vContext.destroyContext(origVContext);
        traverser->vContext.destroyContext(curVContext);
    }

cleanup:
    if(traverser->hasPContext && traverser->pContext.endTraverserProcess)
        traverser->pContext.endTraverserProcess(traverser->pContext.context, traverser);
    // Restore the isInTray bool (might be set above, never cleared above)
    traverser->isInTray = isInTray;
}

/* Function cloneGroupDefTraverser()
 *    This function makes a clone of the given traverser without any of the
 *    attached context.
 *
 *    To make a copy of the group traverser with its full context, see the functions
 *        createGroupDefTraverser(), copyGroupDefTraverser(), and destroyGroupDefTraverser().
 */
GroupDefTraverser* cloneGroupDefTraverser(GroupDefTraverser* traverser){
    GroupDefTraverser* clone;
    clone = calloc(1, sizeof(GroupDefTraverser));
    clone->def = traverser->def;
    memcpy(clone->mat, traverser->mat, sizeof(Mat4));
    return clone;
}

GroupDefTraverser* createGroupDefTraverser(){
    return calloc(1, sizeof(GroupDefTraverser));
}

void copyGroupDefTraverser(GroupDefTraverser* src, GroupDefTraverser* dst){
    memcpy(dst, src, sizeof(GroupDefTraverser));

    if(dst->hasVContext){
        dst->vContext.context = dst->vContext.createContext();
        dst->vContext.copyContext(src->vContext.context, dst->vContext.context);
    }
}

void destroyGroupDefTraverser(GroupDefTraverser* traverser){
    if(traverser->hasVContext){
        traverser->vContext.destroyContext(traverser->vContext.context);
    }

    free(traverser);
}


static Array* extractSubtreesResult;
ExtractionCriteria extractSubtreesAccept;
ExtractionItemProcess extractSubtreesProcess;
static int groupExtractSubtreesProcessor(GroupDefTraverser* traverser){
    int accept = 0;
    ExtractionAction action;
    
    if(extractSubtreesProcess)
        extractSubtreesProcess(traverser, accept);

    action = extractSubtreesAccept(traverser);

    // If the group definition is accepted for extraction,
    if(action & EA_EXTRACT){
        // Create a copy of the traverser and store it for return later.
        GroupDefTraverser* newTraverser = createGroupDefTraverser();
        copyGroupDefTraverser(traverser, newTraverser);
        arrayPushBack(extractSubtreesResult, newTraverser);
    }

    if(action & EA_NO_BRANCH_EXPLORE)
        return 0;
    else
        return 1;
}

Array* groupExtractSubtrees(GroupInfo* groupInfo, ExtractionCriteria accept){
    return groupExtractSubtreesEx(groupInfo, accept, NULL, NULL, NULL);
}

Array* groupExtractSubtreesEx(GroupInfo* groupInfo, ExtractionCriteria accept, ExtractionItemProcess process,
                              GroupDefTraverserPContext* pContext, GroupDefTraverserVContext* vContext){
    GroupDefTraverser traverser;

    // Got good parameters?
    assert(groupInfo && accept);
    
    // There should be nothing in the result array yet.
    assert(!extractSubtreesResult);
    
    // Create the result array before extraction starts.
    extractSubtreesResult = createArray();
    
    extractSubtreesAccept = accept;
    extractSubtreesProcess = process;
    memset(&traverser, 0, sizeof(GroupDefTraverser));

    if(pContext){
        traverser.pContext = *pContext;
        traverser.hasPContext = 1;
    }

    if(vContext){
        traverser.vContext = *vContext;
        traverser.hasVContext = 1;
    }

    // Process the entire tree and have the groupExtractSubtreesProcessor 
    groupProcessDefExBegin(&traverser, groupInfo, groupExtractSubtreesProcessor);

    {
        Array* result = extractSubtreesResult;
        extractSubtreesResult = NULL;
        return result;
    }
}

int groupDefIsSpawnArea(GroupDef* def){
    if(!def->properties)
        return 0;

    if ( stashFindElement(def->properties, "SpawnArea", NULL))
        return 1;
    else
        return 0;
}

int groupDefIsScenario(GroupDef* def){
    if(!def->properties)
        return 0;

    if ( stashFindElement(def->properties, "Scenario", NULL))
        return 1;
    else
        return 0;
}

int groupDefIsDoorGenerator(GroupDef * def)
{
    char* generatedType;

    if(!def->properties)
        return 0;

    generatedType = groupDefFindPropertyValue(def, "Generator");

    if(generatedType && strstri(generatedType, "Door"))
        return 1; 
    else
        return 0;
}

int groupDefIsGenerator(GroupDef* def){
    if(!def->properties)
        return 0;

    if ( stashFindElement(def->properties, "Generator", NULL))
        return 1;
    else
        return 0;
}

int groupDefIsGeneratorGroup(GroupDef* def){
    if(!def->properties)
        return 0;

    if ( stashFindElement(def->properties, "GeneratorGroup", NULL))
        return 1;
    else
        return 0;
}

int groupDefIsActivationRequest(GroupDef* def){
    if(!def->properties)
        return 0;

    if ( stashFindElement(def->properties, "ActivateScenario", NULL))
        return 1;
    else
        return 0;
}

PropertyEnt* groupDefFindProperty(GroupDef* def, char* propName){
    if(!def || !def->properties)
        return NULL;
    else
        return stashFindPointerReturnPointer(def->properties, propName);
}

char* groupDefFindPropertyValue(GroupDef* def, char* propName){
    PropertyEnt* prop;
    
    prop = groupDefFindProperty(def, propName);
    if(!prop)
        return NULL;
    else
        return prop->value_str;
}

int groupDefAddProperty(GroupDef* def, char* propName, char* propValue){
    PropertyEnt* prop;

    if(!def->properties){
        def->properties = stashTableCreateWithStringKeys(4,  StashDeepCopyKeys);
        def->has_properties = 1;
    }
    
    prop = createPropertyEnt();
    strncpy(prop->name_str, propName, PROPERTY_NAME_STRLEN);    
    estrPrintCharString(&prop->value_str, propValue);
    
    if(stashAddPointer(def->properties, prop->name_str, prop, false))
        return 1;
    else
        return 0;
}

void groupDefDeleteProperties(GroupDef* def)
{
    StashElement element;
    StashTableIterator it;
    PropertyEnt* prop;
    int        elements_deleted=0;

    if (!def->properties)
        return;
    stashGetIterator(def->properties, &it);

    while(stashGetNextElement(&it, &element))
    {
        prop = stashElementGetPointer(element);
        destroyPropertyEnt(prop);
        elements_deleted++;
    }
    stashTableDestroy(def->properties);
    def->properties = 0;
    def->has_properties = 0;
}

static void groupProcessDefTrackerHelper(DefTracker *tracker, GroupDefTrackerProcessor process)
{
    int            i;
    
    if(!process(tracker))
        return;

    trackerOpen(tracker);
    for(i=0;i<tracker->count;i++)
    {
        groupProcessDefTrackerHelper(&tracker->entries[i], process);
    }
}

void groupProcessDefTracker(GroupInfo *group_info, GroupDefTrackerProcessor process)
{
    DefTracker* ref;
    int            i;
    
    for(i = 0; i < group_info->ref_count; i++)
    {
        ref = group_info->refs[i];
        if (ref==NULL) continue;
        if (ref->def)
        {
            groupProcessDefTrackerHelper(ref, process);
        }
    }
}

void groupCreateAllCtris()
{
    int i;
    for(i = 0; i < group_info.file_count; i++)
    {
        GroupFile* file = group_info.files[i];
        int j;
        for(j = 0; j < file->count; j++)
        {
            GroupDef* def = file->defs[j];
            if(def && def->in_use && def->model)
            {
                TrickInfo* trick = def->model->trick ? def->model->trick->info : NULL;

                if(!trick || trick->lod_near <= 0 && !(trick->tnode.flags1 & TRICK_NOCOLL))
                {
                    //printf("      Creating Ctris: %s\n", def->model->name);
                    modelCreateCtris(def->model);
                }
                else
                {
                    //printf("  NOT creating Ctris: %s\n", def->model->name);
                }
            }
        }
    }

    #if NOVODEX
        // We did a lot of cooking in modelCreateCtris, free the temp buffer
        nwFreeStreamBuffer();
    #endif
}
/*
void groupIteratorStep(GroupDefIterator* iterator){
    assert(iterator);
    switch(iterator->action){
    case ITER_RESTART:
        

    case ITER_SIBLING:


    case ITER_CHILD:


    case ITER_PARENT:


    }

}

*/
