#include "UI/uiTabControl.h"
#include "UI/uiGame.h"
#include <utilitieslib/components/earray.h>
#include "assert.h"
#include "graphics/textureatlas.h"
#include "UI/sprite/sprite_text.h"
#include "UI/sprite/sprite_base.h"
#include "UI/sprite/sprite_font.h"
#include "UI/uiUtil.h"
#include "UI/uiInput.h"
#include "UI/uiUtilGame.h"
#include <utilitieslib/utils/mathutil.h>
#include "gameComm/trayCommon.h"
#include "UI/uiTray.h"
#include "UI/uiCursor.h"
#include "graphics/ttFontUtil.h"
#include "UI/uiContextMenu.h"
#include <utilitieslib/utils/utils.h>
#include "cmdparse/cmdcommon.h"
#include "UI/uiScrollBar.h"
#include <utilitieslib/utils/wininclude.h>
//--------------------------------------
// PRIVATE DATA STRUCTS & PROTOTYPES
//

typedef struct uiTab
{
    char *            name;
    uiTabData        data;
    uiTabControl*    parent;
    int                colorOverride;
    int                activeColorOverride;

}uiTab;


typedef struct uiTabControl
{
    uiTab**                tabs;         // master list of all tabs owned by this control
    int                    selected;     // index of active tab
    int                    offset;         // offset of first tab to be displayed on the left side (used for scrolling)
    int                    lastSelected;// Last thing to be selected
    TabType                type;         // type of tab control, used for dragging ops. 
    int                    nextIterIdx; // for iteration -- index of the next tab data to return
    Wdw*                wdw;         // window index of containing window. Used to handle dragging tabs inside a draggable/resizable window
    ContextMenu            *cm;         // context menu for a tab
    float                actualWd;     // used by chat window to exclude clickable area from move bar
    float                minDrawHt;

    // drag hover members
    int                    dragHoverSelect;
    int                    lastIndex;
    DWORD                msHoverStart;
    bool                mouseOverTab;

    uiTabActionFunc        onSelectedCode;        // optional code run when tab is selected
    uiTabActionFunc2    onMovedCode;        // called when objects are moved or swapped. If moved, the 2nd parameter will be NULL
    uiTabFontColorFunc    fontColorFunc;        // allows custom font color to be chosen (NULL will use CLR_WHITE)
    uiTabActionFunc        onDestroy;            // option code to run when tab is destroyed

}uiTabControl;


void uiTabControlMoveTab(uiTabControl * tc, uiTab * tab);
void uiTabControlSwap(uiTab * tab0, uiTab * tab1);

//--------------------------------------
// DRAGGING
//


void buildTabTrayObj(TrayObj *pto, uiTab * tab )
{
    assert(tab);

    memset(pto, 0, sizeof(TrayObj));
    pto->type    = kTrayItemType_Tab;
    pto->tab    = tab;
    pto->scale    = 1.f;
}

//---------------------------------------------------------------------------------------------------------|
//    uiTab
// 


uiTab * uiTabCreate(const char * name, uiTabData data, uiTabControl * parent)
{
    uiTab * tab = calloc(sizeof(uiTab), 1);

    assert(parent);    // for now, must belong to a control upon creation... (unless I think of a good reason otherwise)

    tab->name = strdup(name);    // FUTURE OPTIM: do not copy the string, just point to it
    tab->data = data;
    tab->parent = parent;
    tab->colorOverride = 0;
    tab->activeColorOverride = 0;

    return tab;
}

uiTab * uiTabCreateColorOverride(const char * name, uiTabData data, uiTabControl * parent, int color, int activeColor)
{
    uiTab * tab = calloc(sizeof(uiTab), 1);

    assert(parent);    // for now, must belong to a control upon creation... (unless I think of a good reason otherwise)

    tab->name = strdup(name);    // FUTURE OPTIM: do not copy the string, just point to it
    tab->data = data;
    tab->parent = parent;
    tab->colorOverride = color;
    tab->activeColorOverride = activeColor;

    return tab;
}

uiTab * uiTabCreateCopyData(const char * name, uiTabData data, uiTabControl * parent)
{
    uiTab * tab = calloc(sizeof(uiTab), 1);

    assert(parent);    // for now, must belong to a control upon creation... (unless I think of a good reason otherwise)

    tab->name = strdup(name);    // FUTURE OPTIM: do not copy the string, just point to it
    tab->data = strdup(data);
    tab->parent = parent;
    tab->colorOverride = 0;
    tab->activeColorOverride = 0;

    return tab;
}

void uiTabDestroy(uiTab * tab)
{
    free(tab->name);
    free(tab);
    tab = 0;
}

void uiTabDestroyFreeData(void * data)
{
    uiTab * tab = (uiTab*)data;

    if(tab)
    {
        if( tab->name )
            free(tab->name);
    
        if( tab->data )
            free(tab->data);

        free(tab);
    }
    tab = 0;
}


void uiTabControlSetParentWindow(uiTabControl * tc, WindowName wdw)
{
    assert(wdw >= 0 && wdw < MAX_WINDOW_COUNT);
    tc->wdw = wdwGetWindow(wdw);
}

//---------------------------------------------------------------------------------------------------------|
//    uiTabControl
// 

void defaultTabColorFunc(uiTabData foo, bool selected)
{
    if(selected)
        font_color(CLR_WHITE, CLR_WHITE);
    else
        font_color(CLR_TAB_INACTIVE, CLR_TAB_INACTIVE);
}

//------------------------------------------------------------
//  helper for Edit n continue problems
//----------------------------------------------------------
void uiTabControl_SetSelectCb(uiTabControl *tc, uiTabActionFunc onSelectedCode )
{
    if( verify( tc ))
    {
        tc->onSelectedCode    = onSelectedCode;
    }
}


uiTabControl *    uiTabControlCreate(TabType type, uiTabActionFunc onSelectedCode, uiTabActionFunc2 onMovedCode, uiTabActionFunc onDestroy, uiTabFontColorFunc fontColorFunc, ContextMenu *cm)
{
    return uiTabControlCreateEx(type, onSelectedCode, onMovedCode, onDestroy, fontColorFunc, cm, 0);
}


uiTabControl *    uiTabControlCreateEx(TabType type, uiTabActionFunc onSelectedCode, uiTabActionFunc2 onMovedCode, uiTabActionFunc onDestroy, uiTabFontColorFunc fontColorFunc, ContextMenu *cm, int dragHoverSelect)
{
    uiTabControl * tc = calloc(sizeof(uiTabControl), 1);

    tc->type            = type;
    tc->onSelectedCode    = onSelectedCode;
    tc->onMovedCode        = onMovedCode;
    tc->onDestroy        = onDestroy;
    tc->fontColorFunc    = (fontColorFunc ? fontColorFunc : defaultTabColorFunc);
    tc->cm                = cm;
    tc->dragHoverSelect    = dragHoverSelect;
    tc->lastIndex = -1;
    tc->msHoverStart = 0;
    tc->mouseOverTab = false;

    eaCreate(&tc->tabs);

    return tc;
}



void uiTabControlDestroy(uiTabControl * tc)
{
    if( !tc )
        return;

    if( tc->onDestroy )
        eaClearEx(&tc->tabs, (EArrayItemDestructor) tc->onDestroy );
    else
        eaClearEx(&tc->tabs, (EArrayItemDestructor) uiTabDestroy);

    eaDestroy(&tc->tabs);

    free(tc);
    tc = 0;
}

void uiTabControlAdd(uiTabControl * tc, const char * text, uiTabData data)
{
    eaPush(&tc->tabs, uiTabCreate(text, data, tc));
}

void uiTabControlAddColorOverride(uiTabControl * tc, const char * text, uiTabData data, int color, int activeColor)
{
    eaPush(&tc->tabs, uiTabCreateColorOverride(text, data, tc, color, activeColor));
}

void uiTabControlAddCopy(uiTabControl * tc, const char * text, uiTabData data)
{
    eaPush(&tc->tabs, uiTabCreateCopyData(text, data, tc));
}

void uiTabControlRename(uiTabControl * tc, const char * text, uiTabData data)
{
    int i;

    for(i=0;i<eaSize(&tc->tabs);i++)
    {
        if(tc->tabs[i]->data == data)
        {
            uiTab * tab = tc->tabs[i];
            
            assert(tab->name && text);
            assert(tc == tab->parent);

            free(tab->name);
            tab->name = strdup(text);

            return;
        }
    }

    assert(!"Did not find tab!");
}

void uiTabControlRemove(uiTabControl * tc, uiTabData data)
{
    int i;

    for(i=0;i<eaSize(&tc->tabs);i++)
    {
        if(tc->tabs[i]->data == data)
        {
            uiTab * tab = eaRemove(&tc->tabs, i);
            assert(tc == tab->parent);

            // if we're dragging it, stop!
            if(    cursor.dragging 
                && cursor.drag_obj.type == kTrayItemType_Tab 
                && cursor.drag_obj.tab == tab)
            {
                trayobj_stopDragging();
            }

            if( tc->onDestroy )
                tc->onDestroy(tab);
            else
                uiTabDestroy(tab);

            // adjust selected tab
            tc->selected = min(tc->selected, (eaSize(&tc->tabs) - 1));
            tc->selected = max(tc->selected, 0);

            return;
        }
    }
}

int uiTabControlRemoveByName(uiTabControl * tc, const char * str)
{
    int i, retval = false;

    for(i=0;i<eaSize(&tc->tabs);i++)
    {
        if(strcmp(tc->tabs[i]->name,str)==0)
        {
            uiTab * tab;
            
            if( tc->selected == i )
                retval = true;
            
            tab = eaRemove(&tc->tabs, i);
            assert(tc == tab->parent);

            // if we're dragging it, stop!
            if(    cursor.dragging 
                && cursor.drag_obj.type == kTrayItemType_Tab 
                && cursor.drag_obj.tab == tab)
            {
                trayobj_stopDragging();
            }

            if( tc->onDestroy )
                tc->onDestroy(tab);
            else
                uiTabDestroy(tab);

            // adjust selected tab
            tc->selected = min(tc->selected, (eaSize(&tc->tabs) - 1));
            tc->selected = max(tc->selected, 0);

            return retval;
        }
    }
    return retval;
}


void uiTabControlRemoveAll(uiTabControl * tc)
{
    if( verify( tc ) )
    {
        if( tc->onDestroy )
            eaClearEx(&tc->tabs, tc->onDestroy);
        else
            eaClearEx(&tc->tabs, uiTabDestroy);
        tc->selected = 0;
    }
}

void uiTabControlMergeControls(uiTabControl * dest, uiTabControl * src)
{
    int i;

    if( !dest || !src )
        return;

    for( i = eaSize(&src->tabs)-1; i >= 0; i-- )
    {
        uiTab * tab = src->tabs[i];
        // makes a copy
        uiTabControlAdd(dest, tab->name, tab->data); 

        assert(uiTabControlHasData(dest, tab->data));
        assert(uiTabControlHasData(tab->parent, tab->data));

        // destroys original 
        uiTabControlRemove(tab->parent, tab->data);

        if(src->onMovedCode)
            src->onMovedCode(tab, 0);
    }
}

int uiTabControlMoveTabPublic(uiTabControl *dest, uiTabControl* src, uiTabData data)
{
    int i;

    if(!dest || !src || !data)
        return 0;

    //first find the tab
    for(i=0;i<eaSize(&src->tabs);i++)
    {
        if(src->tabs[i]->data == data)
        {
            uiTabControlMoveTab(dest, src->tabs[i] );
        }
    }

    if(eaSize(&dest->tabs) == 1)
        return 1;
    else
        return 0;
}

void uiTabControlMoveTab(uiTabControl * tc, uiTab * tab)
{
     assert(tc && tab);

    // makes a copy
    uiTabControlAdd(tc, tab->name, tab->data); 

    assert(uiTabControlHasData(tc, tab->data));
    assert(uiTabControlHasData(tab->parent, tab->data));

    // the mover is set to the selected tab
    uiTabControlSelect(tc, tab->data);

    // destroys original 
    uiTabControlRemove(tab->parent, tab->data);


    if(tc->onMovedCode)
        tc->onMovedCode(tab, 0);

}


static void voidswap(void ** a, void ** b)
{
    void * temp = *a;
    *a = *b;
    *b = temp;
}


// swap tab contents, but NOT the parents
void uiTabControlSwap(uiTab * tab0, uiTab * tab1)
{
    int idx0 = eaFind(&tab0->parent->tabs, tab0);
    int idx1 = eaFind(&tab1->parent->tabs, tab1);

    assert(idx0 != -1 && idx1 != -1);
    assert(tab0 != tab1);

    tab0->parent->tabs[idx0] = tab1;
    tab1->parent->tabs[idx1] = tab0;

    voidswap(&tab0->parent, &tab1->parent);
}



bool uiTabControlHasData(uiTabControl * tc, uiTabData data)
{
    int i;

    for(i=0;i<eaSize(&tc->tabs);i++)
    {
        if(tc->tabs[i]->data == data)
        {
            return true;
        }
    }

    return false;
}

void uiTabControlSelect(uiTabControl * tc, uiTabData data)
{
    int i;

    for(i=0;i<eaSize(&tc->tabs);i++)
    {
        if(tc->tabs[i]->data == data)
        {
            tc->selected = i;
            if(tc->onSelectedCode)
                tc->onSelectedCode(data);
            return;
        }
    }
}

void uiTabControlSelectByName(uiTabControl *tc, const char *name)
{
    int i;

    for(i=0;i<eaSize(&tc->tabs);i++)
    {
        if( stricmp(tc->tabs[i]->name, name) == 0)
        {
            tc->selected = i;
            if(tc->onSelectedCode)
                tc->onSelectedCode(NULL);
            return;
        }
    }
}

bool uiTabControlSelectEx(uiTabControl * tc, uiTabData data, uiTabActionFunc onSelectedCode)
{
    int i;

    for(i=0;i<eaSize(&tc->tabs);i++)
    {
        if(tc->tabs[i]->data == data)
        {
            tc->selected = i;
            if(onSelectedCode)
                onSelectedCode(data);
            return 1;
        }
    }

    return 0;
}

int uiTabControlGetSelectedIdx(uiTabControl * tc)
{
    return tc->selected;
}

void uiTabControlSelectByIdx(uiTabControl *tc, int idx)
{
    if( idx >= 0 && idx < eaSize(&tc->tabs) )
        tc->selected = idx;
    else
        tc->selected = 0;
}

uiTabData uiTabControlGetSelectedData(uiTabControl * tc)
{
    return eaSize(&tc->tabs) && verify(EAINRANGE(tc->selected, tc->tabs)) ? tc->tabs[ tc->selected ]->data : NULL;
}

bool uiTabControlSelectNext(uiTabControl * tc)
{
    tc->selected++;
    if(tc->selected >= eaSize(&tc->tabs))
    {
        tc->selected = 0;
        return true;
    }
    
    return false;
}

bool uiTabControlSelectPrev(uiTabControl * tc)
{
    tc->selected--;
    if(tc->selected < 0)
    {
        tc->selected = (eaSize(&tc->tabs) - 1);
        return true;
    }

    return false;
}


uiTabData uiTabControlGetFirst(uiTabControl * tc)
{
    tc->nextIterIdx = 0;
    return uiTabControlGetNext(tc);
}


uiTabData uiTabControlGetNext(uiTabControl * tc)
{
    uiTabData * data = 0;

    assert(tc->nextIterIdx >= 0);

    if(tc->nextIterIdx < eaSize(&tc->tabs))
    {
        data = tc->tabs[ tc->nextIterIdx ]->data;
        tc->nextIterIdx++;
    }

    return data;
}

// not used for iteration, just returns last element
uiTabData uiTabControlGetLast(uiTabControl * tc)
{
    int size = eaSize(&tc->tabs);
    if(size)
        return tc->tabs[ size - 1 ]->data;
    else
        return 0;
}


#define TAB_INSET            20 // amount to push tab in from edge of window
#define BACK_TAB_OFFSET        3  // offset to kee back tab on the bar
#define TAB_OVERLAP         5  // amount to TAB_OVERLAP tabs


enum {LEFT,MID,RIGHT};
#define ARROW_SPACING    10


static bool hasDragHoveredLongEnough(int tabIndex, uiTabControl * tc, CBox* pBox, U32 milliseconds)
{
    if (cursor.dragging && mouseCollision(pBox))
    {
        DWORD time = timeGetTime();
        if (tc->lastIndex == tabIndex)
        {
            if (time > milliseconds + tc->msHoverStart)
            {
                return true;
            }
        }
        else
        {
            tc->lastIndex = tabIndex;
            tc->msHoverStart = time;
        }

        tc->mouseOverTab = true;
    }

    return false;
}

static void finalizeDragHoverTest(uiTabControl * tc)
{
    if (!tc->mouseOverTab)
    {
        tc->lastIndex = -1;
        tc->msHoverStart = 0;
    }
    else
    {
        tc->mouseOverTab = false;
    }
}

uiTabData drawTabControl(uiTabControl * tc, float x, float y, float z, float width, float height, float scale, int color, int activeColor, TabDirection dir )
{
      uiTabData data = 0;
    int isVertical = (dir == TabDirection_Vertical);
    int tabCount = eaSize(&tc->tabs);
    CBox controlBox;
    float origx = x;
    float origy = y;

    bool bDragging = (   cursor.dragging 
                         && cursor.drag_obj.type == kTrayItemType_Tab 
                      && cursor.drag_obj.tab->parent->type == tc->type
                      && tc->type != TabType_Undraggable);

     BuildCBox(&controlBox, x, y, width, height);    // build before x & y are modified, then test at end of function

    if(tabCount > 0)
    {
        int i;
        int startX;
        int startY;
        int arrowsStart;
        int    printFlags = CENTER_X | CENTER_Y;
        bool bStoppedDrawingEarly = false;
        bool bSetArrowsScissor = false;
        bool bSelectedVisible = (tc->lastSelected == tc->selected);

        TTDrawContext * tabFont = &game_9;

        AtlasTex * activeTex[3];
        AtlasTex * inactiveTex[3];
        AtlasTex ** tex = NULL;

        AtlasTex * backArrow, *forwardArrow;


        float arrowScaleY;        
        float arrowScaleX;
        CBox box;

        if (isVertical)
        {
            //    change to up/down arrow
            backArrow  = atlasLoadTexture( "map_zoom_uparrow.tga");    // uses same arrows as map arrow
            forwardArrow = atlasLoadTexture( "map_zoom_downarrow.tga");
            activeTex[LEFT]        = atlasLoadTexture( "League_Tab_Highlight.tga" );
            activeTex[MID]        = atlasLoadTexture( "League_Tab_Meat.tga" );
            activeTex[RIGHT]    = atlasLoadTexture( "League_Tab_Shadow.tga" );

            inactiveTex[LEFT]    = atlasLoadTexture( "League_Tab_Highlight.tga" );
            inactiveTex[MID]    = atlasLoadTexture( "League_Tab_Meat.tga" );
            inactiveTex[RIGHT]    = atlasLoadTexture( "League_Tab_Shadow.tga" );
        }
        else
        {
            backArrow  = atlasLoadTexture( "teamup_arrow_contract.tga");    // uses same arrows as uiTray.c
            forwardArrow = atlasLoadTexture( "teamup_arrow_expand.tga");
            activeTex[LEFT]        = atlasLoadTexture( "map_tab_active_L.tga" );
            activeTex[MID]        = atlasLoadTexture( "map_tab_active_MID.tga" );
            activeTex[RIGHT]    = atlasLoadTexture( "map_tab_active_R.tga" );

            inactiveTex[LEFT]    = atlasLoadTexture( "map_tab_inactive_L.tga" );
            inactiveTex[MID]    = atlasLoadTexture( "map_tab_inactive_MID.tga" );
            inactiveTex[RIGHT]    = atlasLoadTexture( "map_tab_inactive_R.tga" );
        }


        // make arrow height == tab height
        arrowScaleY = ((float)activeTex[MID]->height) / ((float) backArrow->height);        
        arrowScaleX = 1.5 * arrowScaleY;

        if (isVertical)
        {
            arrowScaleY *= 0.7f;
            arrowScaleX *= 0.7f;
            arrowsStart = y + height + (10 - ((backArrow->height + forwardArrow->height)*arrowScaleY + ARROW_SPACING + 5))*scale;
        }
        else
        {
            arrowsStart = x + width + (10 - ((backArrow->width + forwardArrow->width)*arrowScaleX + ARROW_SPACING + 5))*scale;
        }

        {
               BuildCBox(&box, arrowsStart, y, (width-(arrowsStart-x)), activeTex[MID]->height*scale);    
            //BuildCBox(&clipBox, x, y, ((arrowsStart)-x), activeTex[MID]->height*scale);    
             //drawBox(&box, z+1000, CLR_RED, 0);
            //drawBox(&clipBox, z+1000, CLR_RED, 0);
        }

        assert(tc->selected >=0 && tc->selected <= eaSize(&tc->tabs));

        data = tc->tabs[tc->selected]->data;

        startX = x;
        startY = y;

        font(tabFont);

         tc->offset = MINMAX(tc->offset, 0, eaSize(&tc->tabs)-1);

        // extra lip if needed
        if (!isVertical)
        {
            if( tc->offset )
            {
                int tabColor, texZ;
                if(tc->offset-1 == tc->selected)
                {
                    tex = activeTex;
                    tabColor = activeColor;
                    texZ = z+1;
                }
                else
                {
                    tex = inactiveTex;
                    tabColor = color;
                    texZ = z;
                }
                display_sprite( tex[RIGHT], x, y, texZ, scale, scale, tabColor);
                build_cboxxy( tex[RIGHT], &box, scale, scale, x, y );
                if( mouseClickHit( &box, MS_LEFT ) )
                {
                    tc->offset--;
                    tc->selected = tc->offset;

                    data = tc->tabs[tc->selected]->data;

                    if(tc->onSelectedCode)
                        tc->onSelectedCode(data);
                    collisions_off_for_rest_of_frame = 1;

                }

                if( tc->cm && mouseClickHit(&box, MS_RIGHT ) )
                {
                    tc->offset--;
                    tc->selected = tc->offset;
                    data = tc->tabs[tc->selected]->data;
                    contextMenu_set(tc->cm, box.hx, box.ly, data);
                    collisions_off_for_rest_of_frame = 1;
                }
            }

            if(tc->offset)
                x += (activeTex[RIGHT]->width-TAB_OVERLAP)*scale;
        }

        for(i=tc->offset;i<eaSize(&tc->tabs);i++)
        {
            uiTab * tab = tc->tabs[i];
            int texWidth;
            int texHeight;
            int tabColor;
            int tabBorder = 0;
            int clipBorder = 0;

            // right now, does not localize strings... CW: Non draggable tabs DO translate
            int textWidth = tc->type == TabType_Undraggable?str_wd(tabFont, scale, scale, tab->name):str_wd_notranslate(tabFont, scale, scale, tab->name);
            float tabScale;
            int texZ;

             if(i == tc->selected)
            {
                // active
                tex = activeTex;
                if (tab->activeColorOverride)
                {
                    tabColor = tab->activeColorOverride;
                }
                else
                {
                    tabColor = activeColor;
                }
                texZ = z+1;
            }
            else
            {
                // inactive
                tex = inactiveTex;
                if (tab->colorOverride)
                {
                    tabColor = tab->colorOverride;
                }
                else
                {
                    tabColor = color;
                }
                texZ = z;
            }
  
            tabScale = isVertical ? 25.f/tex[MID]->width : (((float)textWidth / scale ) / ((float) tex[MID]->width));
            texWidth = isVertical ? tex[MID]->width*tabScale : tex[LEFT]->width + tex[MID]->width*tabScale + tex[RIGHT]->width;
            texHeight = tex[MID]->height;

            if (isVertical)
            {
                tabBorder = y + texHeight*scale;
                clipBorder = startY + height;
            }
            else
            {
                tabBorder = x + texWidth*scale;
                clipBorder = startX + width;
            }
            if(tabBorder >= arrowsStart)
            {
                // if it's the last one and we're not scrolling, see if it will fit
                if(!(tc->offset == 0 
                    && i == (eaSize(&tc->tabs)-1)
                    && tabBorder < clipBorder))
                {
                    // ok, we're gonna have some kind of clipping...
                    if(!bSetArrowsScissor) // only set the scissor once
                    {
                        set_scissor(true);
                        if (isVertical)
                        {
                            scissor_dims(x, y, texWidth*scale, arrowsStart-y);
                        }
                        else
                        {
                            scissor_dims(x, y, arrowsStart-x, texHeight*scale);
                        }
                        bSetArrowsScissor = true;
                    }

                    // draw the scrollbars and stop drawing more tabs
                     bStoppedDrawingEarly = true;
                    //break;
                }
            }
            else if (i == tc->selected)
            {
                bSelectedVisible = true;
            }
            
            // select font color...
            tc->fontColorFunc(tab->data, (i==tc->selected));

            if( tc->type != TabType_Undraggable )
                printFlags |= NO_MSPRINT;
               cprntEx( x + (texWidth/2)*scale, y + (tex[MID]->height/2 + 0.5)*scale, z+22, 1.1f*scale, 1.1f*scale, printFlags, tc->tabs[i]->name);
            if (isVertical)
            {
                display_sprite( tex[MID],   x, y, texZ, scale*tabScale, scale, tabColor);
                display_sprite( tex[LEFT],  x, y, texZ+1, scale*tabScale, scale, tabColor);
                display_sprite( tex[RIGHT], x, y, texZ+2, scale*tabScale, scale, tabColor);
            }
            else
            {
                display_sprite( tex[LEFT],  x, y, texZ, scale, scale, tabColor);
                display_sprite( tex[MID],   x + tex[LEFT]->width*scale, y, texZ, scale*tabScale, scale, tabColor);
                display_sprite( tex[RIGHT], x + (tex[LEFT]->width + tex[MID]->width*tabScale)*scale, y, texZ, scale, scale, tabColor);
            }
            
             // collisions & dragging...

               BuildCBox(&box, x+TAB_OVERLAP*scale, y, (texWidth -(2*TAB_OVERLAP))*scale, tex[MID]->height*scale);    
              //drawBox(&box, z+1000, CLR_RED, 0);
 
              if( mouseClickHit( &box, MS_LEFT ) ||
                tc->dragHoverSelect && hasDragHoveredLongEnough(i, tc, &box, 400) ) // 400ms is the Windows menu delay, no doubt chosen through dozens of experty-executed usability tests
            {
                data = tab->data;
                tc->selected = i;

                if(x + texWidth*scale >= arrowsStart && bStoppedDrawingEarly)
                    tc->offset++;

                if(tc->onSelectedCode)
                    tc->onSelectedCode(tab->data);
            }

            if( tc->cm && mouseClickHit(&box, MS_RIGHT ) )
            {
                data = tab->data;
                tc->selected = i;

                if(x + texWidth*scale >= arrowsStart)
                    tc->offset++;
                contextMenu_set(tc->cm, box.hx, box.ly, data);
                collisions_off_for_rest_of_frame = 1;
            }

            if(tc->type != TabType_Undraggable )
            {
                // ignore top bar of containing window when checking for drag collision
                CBox box2;
                   int top = (box.ly + 1*PIX3*scale);
                BuildCBox(&box2, box.left, top, (box.right - box.left), box.bottom - top);
        //        drawBox(&box2, z+1000, CLR_RED, 0);
                  if(    mouseLeftDrag( &box2 ) 
                    && ! (tc->wdw && tc->wdw->drag_mode)) // don't drag if we're resizing containing window
                {
                    TrayObj tmp;
                    buildTabTrayObj( &tmp, tab);
                     trayobj_startDragging( &tmp, atlasLoadTexture("map_tab_inactive.tga"), 0  );
                }
            }

             if( bDragging && tab != cursor.drag_obj.tab)
            {
                 if( mouseCollision(&box) && !isDown(MS_LEFT) )
                {
                     uiTabControlSwap(tab, cursor.drag_obj.tab);
                    if(tc->onMovedCode)
                        tc->onMovedCode(cursor.drag_obj.tab, tab);

                     uiTabControlSelect(tc, cursor.drag_obj.tab->data);

                    trayobj_stopDragging();
                    bDragging = false;    // we've already handled the drag & drop
                    collisions_off_for_rest_of_frame = 1;
                }
            }

            if(mouseCollision(&box))
                 collisions_off_for_rest_of_frame = true;


            if (isVertical)
            {
                y += texHeight*scale;
            }
            else
            {
                x += (texWidth - TAB_OVERLAP)*scale;
            }
        }

        finalizeDragHoverTest(tc);

        if( bSetArrowsScissor )
        {
            set_scissor(false);
            bSetArrowsScissor = false;
        }

        tc->actualWd = x - origx;

        // draw scroll arrows... (if necessary)

        if(bStoppedDrawingEarly || tc->offset  > 0)
        {
            int backArrowX, backArrowY, forwardArrowX, forwardArrowY;
            static float timer = 0;
            static int butheld = 0;
        
            int bright = color|0xff;
            int lefthit,righthit;
            brightenColor(&bright, 50);
            if (isVertical)
            {
                backArrowY = arrowsStart;
                forwardArrowY = backArrowY + ((backArrow->height *arrowScaleY)+ARROW_SPACING)*scale;
                backArrowX = forwardArrowX = x + backArrow->width*arrowScaleY*scale*0.5;
            }
            else
            {
                backArrowX  = arrowsStart;//  + 10*scale;
                forwardArrowX = backArrowX + ((backArrow->width*arrowScaleY) + ARROW_SPACING)*scale;
                backArrowY = forwardArrowY = y + (activeTex[MID]->height - backArrow->height*arrowScaleY)*scale;

            }

            lefthit = drawGenericButtonEx(backArrow,  backArrowX, backArrowY, z+1, arrowScaleX*scale, arrowScaleY*scale, color, bright, (tc->offset > 0));
            righthit = drawGenericButtonEx(forwardArrow, forwardArrowX, forwardArrowY, z+1, arrowScaleX*scale, arrowScaleY*scale, color, color, bStoppedDrawingEarly );
            if (!isDown( MS_LEFT ))
            {
                timer = 0;
                butheld = 0;
            }
               if ( D_MOUSEHIT == lefthit)
             {
                tc->offset = max(0, (tc->offset - 1));
            }
            else if (D_MOUSEDOWN == lefthit || (butheld == 1 && tc->offset > 0))
            {
                gScrollBarDragging = true;
                timer += TIMESTEP/3;
                butheld = 1;
                if (timer > 1.0f)
                {
                    tc->offset = max(0, (tc->offset - 1));
                    timer = 0;
                }
            }
             else if ( D_MOUSEHIT == righthit)
            {
                tc->offset = min((eaSize(&tc->tabs)-1), (tc->offset + 1));
            }
            else if (D_MOUSEDOWN == righthit || (butheld == 2 && bStoppedDrawingEarly))
            {
                gScrollBarDragging = true;
                timer += TIMESTEP/3;
                butheld = 2;
                if (timer > 1.0f)
                {
                    tc->offset = min((eaSize(&tc->tabs)-1), (tc->offset + 1));
                    timer = 0;
                }
            }
        }
        if (!bSelectedVisible)
            tc->offset = tc->selected;
        tc->lastSelected = tc->selected;
        if (isVertical)
        {
            int minHt = origy+(tex[MID]->height+ARROW_SPACING)*scale + 2*forwardArrow->height *arrowScaleY*scale;
            y = MAX(minHt,    y);
            tc->minDrawHt = y - origy;
        }
        else
        {
            tc->minDrawHt = activeTex[MID]->height;
        }
    }

    // check for dragging tab into chat box
     if(bDragging)
    {    
        if(mouseCollision(&controlBox) && !isDown( MS_LEFT ))
        {
            if(cursor.drag_obj.tab->parent != tc)
            {
                uiTabControlMoveTab(tc, cursor.drag_obj.tab);
                data = tc->tabs[tc->selected]->data;
            }

            trayobj_stopDragging();
        }
        else if(tc->wdw && tc->wdw->drag_mode)
        {
            // stop dragging if containing window is being resized
            trayobj_stopDragging();
        }
    }

    return data;
}

int uiTabControlChatPhantomAllowable( uiTabControl *tc, TrayObj * obj )
{
     if(eaSize(&tc->tabs) == 0)
        return 0;
    if(eaSize(&tc->tabs) > 1)
        return 1;

    if( tc->tabs[0]->data == obj->tab->data )
        return 0;

    return 1;
}

int uiTabControlNumTabs( uiTabControl * tc )
{
    if( tc && tc->tabs )
        return eaSize(&tc->tabs);
    else
        return 0;

}

float uiTabControlGetWidth( uiTabControl *tc )
{
    if( eaSize(&tc->tabs))
        return tc->actualWd;
    else
        return 0.f;
}

float uiTabControlGetMinDrawHeight(uiTabControl *tc)
{
    if (eaSize(&tc->tabs))
    {
        return tc->minDrawHt;
    }
    return 0.0f;
}



//----------------------------------------
//  get the name of a tab by index
//----------------------------------------
char* uiTabControlGetTabName(uiTabControl * tc, int idx)
{
    char *res = NULL;
    if( verify(tc) && EAINRANGE( idx, tc->tabs ))
    {
        res = tc->tabs[idx]->name;
    }
    return res;
}
