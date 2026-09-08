#ifndef MENU_H
#define MENU_H

#include "EditListView.h"
#include <utilitieslib/components/EString.h>

typedef struct MenuEntry MenuEntry; 

/* Menu
 *
 * This structure provides a general way of displaying a directory hierarchy in the editor.
 * This Menu is displayed using EditListView, specifcations of which can be found in the  
 * appropriate header file.  Allows for opening of directories by left-clicking, and allows
 * for an arbitrary function to be run upon clicking a particular entry.
 *
 ********************
 *
 * Creating a new menu:
 * Menu * myMenu = newMenu(x,y,width,height,name);
 *
 * x and y are the coordinates of the upper-left corner of the menu
 *
 * width and height are the width and height of the active region of the menu 
 * (i.e. the area that can be clicked).
 *
 * name is the name of the root directory, "/", and has no function except that 
 * it is shown when displayed on the screen.
 *
 *********************
 *
 * Adding entries to a menu:
 * addEntryToMenu(myMenu,path,function,info);
 *
 * myMenu is the pointer to your menu.
 *
 * path is a string that describes the new Entry, "dir/subdir/entry", for example.
 * Adding that path would automatically add "dir" and "dur/subdir", so it is not 
 * necessary to add each directory, it suffices to add any of its children.  Adding
 * any path twice will have no effect.
 *
 * function is a function pointer to a function of the form: void (*func)(MenuEntry * me,ClickInfo * ci);
 * the function will be called whenever this newly added entry is clicked, and it will be passed the
 * MenuEntry that was clicked, as well as info, which can be a pointer to any arbitrary data.
 * function can be NULL, in which case no function will be run when this path is clicked in the Menu.
 *
 *********************
 *
 * Displaying the menu:
 * displayMenu(myMenu);
 *
 * myMenu is the pointer to your menu, and it is displayed using EditListView's display code.
 *
 * all mouse event handling occurs within this function by EditListView code.
 *
 *********************
 *
 * Removing entries from a menu:
 * deleteEntryFromMenu(myMenu,path);
 * 
 * myMenu is the pointer to your Menu
 *
 * path is the path of the entry you are removing.  Removing a path will also automatically remove
 * any entry that is a child of that path.  So removing "a/b" will remove "a/b/c", for example.
 *
 *********************
 *
 * Destroying the menu:
 * destroyMenu(myMenu);
 *
 * myMenu is the pointer to your Menu, and it and all its entries will be destroyed.  There is 
 * no need to remove entries before calling this function, it will remove and free all memory properly.
 *
**/

typedef struct Menu {
    MenuEntry * root;    //the parent item

                        //formatting parameters
    int tabSpace;        //space an item is indented for each level of depth
    int lineHeight;        //the height of each line, not the height of each character

    EditListView * lv;    //handles all the display stuff

    //customizable stuff
    int  (*colorFunc)(MenuEntry *,void *);    //if not NULL, determines the color that MenuEntrys are
                                            //drawn, if it returns 0 the default coloring is used
} Menu;

Menu * newMenu(int x,int y,int width,int height,const char * name);
void displayMenu(Menu *);    //displays all open items in a menu
void destroyMenu(Menu *);    //destroys a Menu and all of its items
MenuEntry * addEntryToMenu(Menu *,const char *,void (*func)(MenuEntry *,ClickInfo *),void * info);
void deleteEntryFromMenu(Menu *,const char *);    //deletes a directory from the Menu
                                                //will also recursively delete all children

//used as a callback for EditListView, should not be considered public code
void outputText(int,int,char **,int *,int *,void *);






// There should be no need to instantiate a MenuEntry.  Using a Menu should handle everything appropriately.

typedef struct MenuEntry {
    char *name;            // display name of the entry.  Now an EString!
    Menu * theMenu;        //allows all MenuEntrys to get information from the Menu, such as font
                        //or location on the screen
    void * info;        //each MenuEntry can store whatever is needed here
    void (*clickFunc)(MenuEntry *,ClickInfo *);    //if not NULL, it is run when this MenuEntry is clicked

    //state information
    int opened;        //flag that determines if children should be displayed
    int depth;        //distance from this MenuEntry to the root MenuEntry
    int color;        //color that this item should be displayed with, 0 indicates standard coloring

    //this stuff is last so that it can be viewed easily in the debugger
    MenuEntry * parent;    //pointer to the parent MenuEntry (NULL iff root)
    MenuEntry * child;    //pointer to the first child (NULL iff childless)
    MenuEntry * sibling;//pointer to the first sibling (NULL iff this is the last sibling)
    //sibling is used by the parent to be able to find all of its children
} MenuEntry;

MenuEntry * newMenuEntry(Menu *,MenuEntry *,const char * name);
void recurseOnAllEntries(MenuEntry * me,int onlyOpened,int preorder,void * info,void (*func)(MenuEntry *,void *));
void displayMenuEntry(MenuEntry *,void * info);    //displays a MenuEntry and all of its children
                                                //info is a pointer to an array of integers
                                                //giving information about which elements
                                                //are to be displayed
void destroyMenuEntry(MenuEntry *,void * info);    //destroys a MenuEntry and all of its children
MenuEntry * addMenuEntry(MenuEntry *,const char * name,void (*func)(MenuEntry *,ClickInfo *),void * info);

MenuEntry* findMenuEntry(MenuEntry *me, const char *name);

#endif