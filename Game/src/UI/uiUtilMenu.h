#ifndef UIUTILMENU_H
#define UIUTILMENU_H

// This file contains drawing routines for common character creation elements
//--------------------------------------------------------------------------------------------------

#include "UI/uiInclude.h"

typedef struct TTDrawContext TTDrawContext;
// Displays the city background with moving glow
//
void drawBackground( AtlasTex * layover );

// Draws a text header with two dots
//
void drawMenuHeader( float x, float y, float z, int clr1, int clr2, const char *txt, float sc );

// Draws the blue border set used in the menus
//
void drawMenuFrame( int radius, float x, float y, float z, float wd, float ht, int color, int back_color, int noBottom );

// Draws a capsule with the same blue border set
//
void drawMenuCapsule( float x, float y, float z, float wd, float scale, int color, int back_color );

// Just the circle with out the after image and text (for login slider)
int drawNextCircle( float x, float y, float z, float scale, int dir, int locked );

// Draws the next and back arrows on each menu screen
//
int drawNextButton( float x, float y, float z, float scale, int dir, int locked, int glow, const char * txt );

// Draws the scrollin title bar
//
void drawMenuTitle( float x, float y, float z, const char * txt, float scale, int newMenu );

// Draws a checkbox with attached name bar
//
int drawCheckBarEx( float x, float y, float z, float scale, float wd, const char * text, int down, int selectable, int gray, int isBig, int halfText, AtlasTex * icon );
int drawCheckBar( float x, float y, float z, float scale, float wd, const char * text, int down, int selectable, int isBig, int halfText, AtlasTex * icon );

// Draws name bar with no checkbox
//
int drawMenuBarSquished( float x, float y, float z, float sc, float wd, const char * text, int isOn, int drawLeft, int isLit, int grey, int unclickable, int invalid, float squish, int bordercolor, TTDrawContext *buttonFont,int useStandardColors );
#define drawMenuBar(x,y,z,sc,wd,text,isOn,drawLeft,isLit,grey,unclickable, bordercolor) drawMenuBarSquished(x,y,z,sc,wd,text,isOn,drawLeft,isLit,grey,unclickable,0,1.0f, bordercolor, &game_12,0 )

// Lare capsule for enhancement inventory
//
void drawLargeCapsule( float centerX, float centerY, float z, float wd, float screenScale );

// Title with one dot used in origin/class
//
void drawTextWithDot( float x, float y, float z, float scale, const char * txt );


// placehoder
int selectableText( float x, float y, float z, float scale,  const char* txt, int color1, int color2, int center, int translate );

void setHelpOverlay( const char *texName );
void drawHelpOverlay();

void colorRingGetSize( float* x, float* y );

typedef enum
{
    CRD_NONE,
    CRD_SELECTED    = (1 << 0),    // The color ring is selected
    CRD_SELECTABLE = (1 << 1),
    CRD_MOUSEOVER    = (1 << 2),    // The color ring should respond to mouseover
} CRDMode;
int colorRingDraw( float x, float y, float z, CRDMode mode, int color, float scale );

typedef enum
{
    ACB_NONE,
    ACB_ACCEPT = (1 << 0),
    ACB_CANCEL = (1 << 1),
} ACButton;

typedef enum
{
    ACBR_NONE,
    ACBR_ACCEPT = (1 << 0),
    ACBR_CANCEL = (1 << 1),
    ACBR_ACCEPT_LOCKED = (1 << 2),
    ACBR_CANCEL_LOCKED = (1 << 3),
} ACButtonResult;

// Add new nag contexts to the end so the old ones keep the same bit in the
// key we store in the registry.
typedef enum
{
    GRNAG_NONE = 0,
    GRNAG_POWERSET,
    GRNAG_MORALITY,
    GRNAG_CHARSELECT,
    GRNAG_COUNT,
} GRNagContext;

ACButtonResult drawAcceptCancelButton(ACButton grayButtons, float screenScaleX, float screenScaleY);

extern int gHelpOverlay;

void setupUIColors(void);
typedef struct NonLinearMenu NonLinearMenu;
typedef struct NonLinearMenuElement
{
    int menuId;
    const char *title[3];
    int (*hiddenCode)(void);
    int (*cantEnterCode)(void);
    void (*enterCode)(void);
    void (*passThroughCode)(void);
    void (*exitCleanupCode)(void);
    int listId;
    float flashingTimer;
    NonLinearMenu *nlm_owner;
}NonLinearMenuElement;

typedef struct NonLinearMenu
{
    NonLinearMenuElement **elementList;
    int currentMenu;
    int transactionId;
}NonLinearMenu;

int drawNonLinearMenu(NonLinearMenu * nlm, float startx, float y, float z, float wd, float ht, float sc, int currentMenuId, int displayMenuId, int locked);
NonLinearMenu *NLM_allocNonLinearMenu();
void NLM_pushNewNLMElement(NonLinearMenu *nlm, NonLinearMenuElement *nlmElement);
void NLM_incrementTransactionID(NonLinearMenu *nlm);
int drawNLMIconButton(float x, float y, float z, float wd, float ht, char *txt, AtlasTex *tex, int iconYOffSet, int locked);

void setHeadingFontFromSkin(int light);

void buyExpansionPurchase(void* data);

void dialogGoingRogueNag(GRNagContext ctx);
int okToShowGoingRogueNag(GRNagContext ctx);
#endif
