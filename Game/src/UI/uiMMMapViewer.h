#ifndef UIMMMAPVIEWER_H
#define UIMMMAPVIEWER_H

#include "group/groupMetaMinimap.h"

//MOVEMENT

typedef struct MMMapViewer
{
    int state;
    int currChoice;
    int wdw;
    int changed;
    int type;

    float x;
    float y;
    float z;
    float sc;

    float wd;
    float ht;
    float maxWd;
    float maxHt;
    float scale;

    float transitionTime;    //time between a click and when an icon is finished moving.
    float transitionProgress;

    ArchitectMapHeader * map;

    char *mapname;            //the name of the map being displayed
}MMMapViewer;


MMMapViewer *mmmapviewer_create();
// This is the default picture browser, it displays in the window the current selected string.  eArray of images to view is passed in here
void mmmapviewer_init( MMMapViewer *mv, float x, float y, float z, float wd, float ht, int wdw );


// for dynamically populating a browser
void mmmapviewer_addPictures( MMMapViewer *mv, ArchitectMapHeader *map);

// clear and free all of the elements
void mmmapviewer_clearPictures( MMMapViewer *mv);

// clear out a pictureBrowser structure to be re-inited
void mmmapviewer_destroy(MMMapViewer *mv);

// sets a specific element to be selected
void mmmapviewer_setId( MMMapViewer *mv, int id);

// set the location of a picture browser
void mmmapviewer_setLoc( MMMapViewer *mv, float x, float y, float z );

// Draws the picture browser
int mmmapviewer_display( MMMapViewer *mv );

// gets and sets the map's name
char *mmmapviewer_getMapName(MMMapViewer *mv );
void mmmapviewer_setMapName(MMMapViewer *mv, char *mapname );


#endif
