#include "Menu.h"
#include <stdlib.h>
#include <string.h>
#include "graphics/font.h"
#include "win/input.h"
#include "UI/uiInput.h"
#include "cmdparse/cmdcommon.h"
#include "group/group.h"
#include <utilitieslib/utils/mathutil.h>
#include "edit/edit_select.h"
#include "edit/edit_net.h"
#include "edit/edit_cmd.h"
#include "win/win_init.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/timing.h>

Menu * newMenu(int x,int y,int width,int height,const char * name) {
    Menu * m=(Menu *)malloc(sizeof(Menu));
    if (m==0) return NULL;
    m->root=newMenuEntry(m,0,name);
    m->lineHeight=9;
    m->tabSpace=1;
    m->lv=newELV(x,y,width,height);
    setELVTextCallback(m->lv,outputText);
    setELVInfo(m->lv,(void *)m);
    m->colorFunc=NULL;
    return m;
}


/* this function handles printing the directory hierarchy into an array of strings
 * which is then used by EditListView.  Only prints the contents of opened directories,
 * and also assigns colors based on EditListView's color scheme.
**/
void recursivePrint(MenuEntry * me,void * theInfo) {
    int * info    =(int *)theInfo;
    int current =(int)(info[0]);
    int start    =(int)(info[1]);
    int end        =(int)(info[2]);
    char ** text=(char ** )(info[4]); // now an array of EStrings!  We've already created them...
    int * colors=(int *)(info[5]);
    int actualLine=current-start;
    int i;

    //silly hack here so that files starting with a '_' don't get shown without showall on
    if (me==NULL || (strcmp(me->theMenu->root->name,"Library")==0 && !edit_state.showall && me->name[0]=='_')) return;

    info[3]++;

    if (current<start || current>end) {
        info[0]++;
        return;
    }

    if (me->theMenu->lv->lineClicked==actualLine) {
        me->opened=!me->opened;
        if (me->clickFunc!=NULL)
            me->clickFunc(me,&me->theMenu->lv->clickInfo);
        me->theMenu->lv->lineClicked=-1;
    }

    for (i=0;i<me->depth*me->theMenu->tabSpace+1;i++)
        estrConcatChar(&text[actualLine], ' ');
    if (me->child!=NULL) {
        estrRemove(&text[actualLine], estrLength(&text[actualLine]) - 1, 1);
        if (me->opened)
            estrConcatChar(&text[actualLine], '-');
        else
            estrConcatChar(&text[actualLine], '+');
    }
    
    estrConcatEString(&text[actualLine], me->name);

    colors[actualLine]=0;
    if (me->theMenu->colorFunc)
        colors[actualLine]=me->theMenu->colorFunc(me,theInfo);
    if (me->theMenu->lv->lineOver==actualLine && me->theMenu->lv->hasFocus) {
        colors[actualLine]=EDIT_LIST_VIEW_MOUSEOVER;
    } else if (colors[actualLine]==0) {
        if (me->color)
            colors[actualLine]=me->color;
        else if (me->child!=NULL)
            colors[actualLine]=EDIT_LIST_VIEW_DIRECTORY;
        else
            colors[actualLine]=EDIT_LIST_VIEW_STANDARD;
    }
    (*((int *)info))++;
}

/* Callback function for EditListView
**/ 
void outputText(int start,int end,char ** text,int * colors,int * max,void * info) {
    MenuEntry * me = ((Menu *)info)->root;
    int vals[6];
    vals[0]=0;        //current line number
    vals[1]=start;
    vals[2]=end;
    vals[3]=0;        //total items that can be displayed
    vals[4]=(int)(text);
    vals[5]=(int)(colors);

    recurseOnAllEntries(me,1,1,vals,recursivePrint);
    *max=vals[3];
}

void displayMenu(Menu * m) {
    handleELV(m->lv);
    displayELV(m->lv);
}

void destroyMenu(Menu * m) {
    recurseOnAllEntries(m->root,0,0,0,destroyMenuEntry);
    destroyELV(m->lv);
    free(m);
}

MenuEntry * addEntryToMenu(Menu * m,const char * name,void (*func)(MenuEntry *,ClickInfo *),void * info) {
    char * nameBuffer;
    char * dir;
    MenuEntry * ret;

    strdup_alloca(nameBuffer,name);
    dir=strtok(nameBuffer,"/");
    
    if (m->root->child==NULL)
    {
        PERFINFO_AUTO_START("newMenuEntry", 1);
            m->root->child=newMenuEntry(m,m->root,dir);
        PERFINFO_AUTO_STOP();
    }

    PERFINFO_AUTO_START("addMenuEntry", 1);
        ret=addMenuEntry(m->root->child,dir,func,info);
    PERFINFO_AUTO_STOP();
        
    return ret;
}

MP_DEFINE(MenuEntry);

void deleteEntryFromMenu(Menu * m,const char * name) {
    char * nameBuffer=_alloca(strlen(name)+1);
    char * dir;
    MenuEntry * mi=m->root;
    MenuEntry * prev=NULL;
    if (mi==NULL) return;
    strcpy(nameBuffer,name);
    dir=strtok(nameBuffer,"/");
    while (dir!=NULL) {
        prev=NULL;
        mi=mi->child;
        if (mi==NULL)
            return;
        while (stricmp(mi->name,dir)!=0) {
            prev=mi;
            mi=mi->sibling;
            if (mi==NULL)
                return;
        }
        dir=strtok(0,"/");
    }
    if (prev!=NULL)
        prev->sibling=mi->sibling;        //if there was a previous sibling, link it up correctly
    else
        mi->parent->child=mi->sibling;    //otherwise, let the parent know its first kid is deleted
    //recurseOnAllItems gets siblings too, so we do the child of mi
    recurseOnAllEntries(mi->child,0,0,0,destroyMenuEntry);
    MP_FREE(MenuEntry, mi);    //then we get mi by itself
}


MenuEntry * newMenuEntry(Menu * m,MenuEntry * parent,const char * name) {
    MenuEntry * me;
    
    MP_CREATE(MenuEntry, 10000);

    me= MP_ALLOC(MenuEntry);

    if (me==0) return NULL;
    me->opened=0;
    if (parent!=NULL)
        me->depth=parent->depth+1;
    else
        me->depth=0;
    me->theMenu=m;
    me->child=0;
    me->sibling=0;
    me->parent=parent;
    me->color=0;
    me->name = 0;

    estrPrintCharString(&me->name, name);

    me->clickFunc=0;
    me->info=0;
    return me;
}

void recurseOnAllEntries(MenuEntry * me,int onlyOpened,int preorder,void * info,void (*func)(MenuEntry *,void *)) {
    if (me==NULL) return;
    if (preorder)
        func(me,info);
    if ((me->opened || !onlyOpened) && me->child!=NULL)
        recurseOnAllEntries(me->child,onlyOpened,preorder,info,func);
    if (me->sibling!=NULL)
        recurseOnAllEntries(me->sibling,onlyOpened,preorder,info,func);
    if (!preorder)
        func(me,info);
}

MenuEntry* findMenuEntry(MenuEntry *me, const char *name)
{
    MenuEntry *ret = NULL;
    if(me)
    {
        if(streq(me->name, name))
            ret = me;
        if(!ret)
            ret = findMenuEntry(me->child, name);
        if(!ret)
            ret = findMenuEntry(me->sibling, name);
    }
    return ret;
}


void destroyMenuEntry(MenuEntry * me,void * ci)
{
    estrDestroy(&me->name);
    MP_FREE(MenuEntry, me);
}

MenuEntry * addMenuEntry(MenuEntry * me,const char * name,void (*func)(MenuEntry *,ClickInfo *),void * info) {
    if (name==NULL)        //we have reached the last level, nothing more to add
        return NULL;
    while(1)
    {
        if (stricmp(name,me->name)==0) {
            char * nextLevel=strtok(NULL,"/");        //this is the right directory for the new MenuEntry
            if (nextLevel!=NULL) {
                if (me->child==NULL) {
                    me->child=newMenuEntry(me->theMenu,me,nextLevel);
                    me->child->depth=me->depth+1;
                }
                return addMenuEntry(me->child,nextLevel,func,info);
            } else {
                me->clickFunc=func;    //but the function has yet to be set, so set it
                me->info=info;        //same with the info
                return me;
            }
        } else {    //this is not the right directory, so try siblings
            if (me->sibling!=NULL)
                //return addMenuEntry(me->sibling,name,func,info);
                me = me->sibling;
            else {    //the appropriate sibling does not exist, create it
                me->sibling=newMenuEntry(me->theMenu,me->parent,name);
                me->sibling->depth=me->depth;
                return addMenuEntry(me->sibling,name,func,info);
            }
        }
    }
    
    assert(0);
    return NULL;
}
